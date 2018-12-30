//
// Copyright (C) 2004, 2005, 2006, 2007 genoQs Machines
// This file is part of Octopus_OS -
// the firmware of the genoQs Machines Octopus, MIDI Control Sequencer
//
// The source code Octopus_OS is available for use in accordance with the
// GNU General Public License (GPL). For more information about Open Source
// licensing, please visit http://www.opensource.org/
//
// Octopus_OS is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation; either version 2 of the License, or
// (at your option) any later version.
//
// Octopus_OS is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with Octopus_OS; if not, write to the Free Software
// Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
//

{
	//---------------------------------------------------------------------------------------
	// RUNTIME ATTRIBUTES
	//---------------------------------------------------------------------------------------

	// Get the step's runtime stretch factor attribute.
	card8 phraseFactor 		= (stepPt->phrase_data & 0x0f);

	// Get the step's runtime phrase type attribute.
	card8 phraseType		= 0;

	#if PHRASE_MODE_SERIOUS
	phraseType = (stepPt->phrase_data >> 4) & 0x0f;
	#endif


	//---------------------------------------------------------------------------------------
	// STRUMS
	//---------------------------------------------------------------------------------------

	// If the step's strumLevel attribute is non-neutral, select the matching strum phrase and type.
	// Strum levels are in range [0,18]. Value 9 is neutral. Value > 9 is increasingly forward strums.
	// Values < 9 are increasingly reversed strums (0 the most heavy).

	booln strumPresetBo = False;

	if ( strumLevel != 9 ) {

		if ( strumLevel >= 9 ){
			strumLevel -= 9;
			phraseNum = PHRASE_STRUM_PRESET_NUM + strumLevel - 1;
			phraseType = PhraseTypeStrumFwd;

		}
		else {
			strumLevel = 9 - strumLevel;
			phraseNum = PHRASE_STRUM_PRESET_NUM + strumLevel - 1;
			phraseType = PhraseTypeStrumRev;
		}
		phraseFactor = PHRASE_GRV_FACT_NEUTRAL;
		strumPresetBo = True;
	}


	//---------------------------------------------------------------------------------------
	// PHRASE OBJECT
	//---------------------------------------------------------------------------------------

	// Copy the referred to phrase object from the repos into our local object.
	// Strum phrases are taken from the preset pool.
	PhraseStruct phraseObj 	=
		strumPresetBo ? PhrasePresets[phraseNum] : PhraseRepos[phraseNum];

	// Give the phrase the runtime phrase type as was defined in the step or by strums.
	if ( phraseType ) {
		PhrasePtSetType( &phraseObj, phraseType );
	}

	// Get the phrase note count and polyphony.
	card8 phraseNoteCt 		= PhrasePtGetNoteCount( &phraseObj );
	card8 phrasePolyphony 	= PhrasePtGetPoly( &phraseObj );
	if ( phrasePolyphony == 0 ) phrasePolyphony = phraseNoteCt;

	// Wobble phrases (where all attribs are shuffled) get shuffled now, before the chord pitches are added.
	// This allows us to change sta, velo, and len of the chord pitches.

	if ( PhrasePtIsRndAll( &phraseObj ) ) {
		PhrasePtShuffleAll( &phraseObj, phraseNoteCt );
	}

	// Timestretch the phrase if the step has a non-neutral timestretch factor defined.
	if ( !PhrasePtIsStrum(&phraseObj) && (phraseFactor != PHRASE_GRV_FACT_NEUTRAL) ) {
		PhrasePtApplyGrv( &phraseObj, phraseFactor );
	}


	//---------------------------------------------------------------------------------------
	// AUX NOTES
	//---------------------------------------------------------------------------------------

	// If the step has aux (chord) notes defined, add the pitch offsets of aux notes to the phrase object.
	// We will only be dealing with the phrase offsets from now on.
	#ifdef FEATURE_ENABLE_CHORD_OCTAVE
	if ( stepAuxNoteCt ) {
		fill_phrase_pool( &phraseObj, stepPt );
	}
	#else
	for ( i=0; i < stepAuxNoteCt; i++ ) {

		// Get the Nth aux note's pitch offset.
		card8 offset = offset_to_chord_aux(step_chord_pattern, i);
		PhrasePtAddPit( &phraseObj, i+1, offset );

		// And get the corresponding 0, 1 or 2 octave offset.
		switch( get_chord_up(stepPt, offset-1) ) {
			case 1:	PhrasePtAddPit( &phraseObj, i+1, 12 ); break;
			case 2:	PhrasePtAddPit( &phraseObj, i+1, 24 ); break;
		}
 	}
	#endif
	//---------------------------------------------------------------------------------------
	// ADAPTIVE or NORMAL PHRASING
	//---------------------------------------------------------------------------------------

	// If the set has explicit auxiliary (chord) notes defined, the note count is defined
	// by the step, and a matching number of notes from the phrase will be used. This is adaptive phrasing.
	// Otherwise, all of the phrase notes will be processed. This is normal phrasing.

	card8 noteCt 		= max( 1, phraseNoteCt );
	card8 polyphony 	= max( 1, phrasePolyphony );
	intn  playNoteCt 	= 1;

	if ( stepAuxNoteCt || stepPolyphony ) {
		noteCt = stepAuxNoteCt + 1;
		polyphony = stepPolyphony + 1;
	}

	//---------------------------------------------------------------------------------------
	// RANDOM-PICK-FROM-NOTEPOOL
	//---------------------------------------------------------------------------------------

	// Based on noteCt and polyphony, decide to play the notes as is, or shuffled with optional 'blanks' inserted.

	// More notes defined that allowed to play. Shuffle the defined notes, and play no
	// more than the first polyphony notes from the phrase.

	if 	( noteCt > polyphony ) {
		PhrasePtShufflePit( &phraseObj, noteCt );
		playNoteCt = polyphony;
	}

	// Bigger polyphony than notes defined. Shuffle the first polyphony notes in the
	// phrase (which adds extra 'blank' notes), and play no more than the first noteCt notes.
	// Blank notes are recognized by a PIT offset of -128.

	else if ( noteCt < polyphony ) {
		for ( i = noteCt; i < polyphony; i++ ) PhrasePtSetPit( &phraseObj, i, -128 );
		PhrasePtShufflePit( &phraseObj, polyphony );
		playNoteCt = noteCt;
	}

	// The 'normal' situation where noteCt and polyphony match. Play un-shuffled, unless the phrase has a random or reverse type.
	else if ( noteCt == polyphony ) {
		playNoteCt = noteCt;

		// Now that there's no shuffling dictated by noteCt vs polyphony,
		// see if the phrase itself has a random or reversed pitch type.

		if ( PhrasePtIsRndPit( &phraseObj ) ) {
			PhrasePtShufflePit( &phraseObj, noteCt );
		}
		else if ( PhrasePtIsRev(&phraseObj) ) {
			PhrasePtReversePit( &phraseObj, noteCt );
		}
	}



	//---------------------------------------------------------------------------------------
	// GENERATE NOTES
	//---------------------------------------------------------------------------------------

	//
	// Old school part of the chord - Play every pitch of the step chord but only
	// to max number of notes allowed to play by polyphony and chord size
	//

	intn noteIx;
	for ( noteIx = 0; noteIx < playNoteCt; noteIx++ ) {

		// Phrase notes with magic PIT value -128  are 'blank' notes mixed in with random-pick-from-pool.
		// Simply skip these.

		if ( PhrasePtGetPit(&phraseObj, noteIx) == -128 ) {
			continue;
		}

		//---------------------------------------------------------------------------------------
		// START OFFSET (STA)
		//---------------------------------------------------------------------------------------
		intn phrase_STA = PhrasePtGetSta( &phraseObj, noteIx );
		// Subject phrase_STA to track speed multiplier if the phrase is not a strum.

		if ( !PhrasePtIsStrum(&phraseObj) ) {

			switch( attr_TEMPOMUL_SKIP & 0x0F ){
			// Track equals or above normal speed
			case 0:
				phrase_STA /= attr_TEMPOMUL;
				break;

			// Track below normal speed
			default:
				phrase_STA *= ((attr_TEMPOMUL_SKIP & 0x0F) + 1);
				break;
			}
		}

		intn note_offset_STA = start_offset + phrase_STA;


		//---------------------------------------------------------------------------------------
		// PITCH
		//---------------------------------------------------------------------------------------

		// Get the pitch offset produced by the aux notes and/or phrase.
		// If rollBo is set, only use the pitch offset of the first note.
		// This allows a phrase to still offset the step's pitch, even though only one pitch is used.

		intn pitch = PhrasePtGetPit( &phraseObj, rollBo ? 0 : noteIx );

		// Calc the full pitch.
		pitch = normalize( pitch_initial + pitch, 0, 127 );

		// FORCE TO SCALE intervention - LOCAL has priority over GLOBAL scale
		// Applies only on real MIDI - not VIRTUAL

		if ( midiCH <= 32 ) {
			if ( ( target_page->force_to_scale == ON )) {

				// If the note index of the initial pitch is not in the scale
				if ( how_selected_in_current_scale( target_page, pitch_to_noteNdx( pitch ) ) == OFF ){
					// Add to initial pitch the offset to the next scale degree
					pitch += offset_to_next_current_degree( target_page, pitch );
				}
			}
			else if (( GRID_assistant_page->force_to_scale == ON ) ){

				// If the note index of the initial pitch is not in the scale
				if ( how_selected_in_current_scale( GRID_assistant_page, pitch_to_noteNdx( pitch ) ) == OFF	){

					// Add to initial pitch the offset to the next scale degree
					pitch += offset_to_next_current_degree( GRID_assistant_page, pitch );
				}
			} // FTS intervention
		} // real MIDI channel



		//---------------------------------------------------------------------------------------
		// VELOCITY
		//---------------------------------------------------------------------------------------
		// Check if the track is hyped and decide on the track base velocity.
		if ( target_page->Track[phys_row]->hyper < 0xF0 ){
			trackBaseVelocity = getHyperstepVelocity( target_page, phys_row, EFF_pool_VEL );
		}
		else{
			trackBaseVelocity = target_page->Track[head_row]->attr_VEL;
		}



		#ifdef EVENTS_FACTORIZED
		intn velocity 	=
			normalize(
				(	(		EFF_pool_VEL
						// Dice - flow shape velocity
						+ 	dice_velocity_offset
						+   PhrasePtGetVel( &phraseObj, noteIx )

						+ (	(	stepPt->attr_VEL
								* Track_VEL_factor[	target_page->Track[head_row]->VEL_factor
													+ target_page->Track[phys_row]->event_offset[ATTR_VELOCITY] ] )
							/ VEL_FACTOR_NEUTRAL_VALUE )

						+	trackBaseVelocity )// target_page->Track[head_row]->attr_VEL )

					*	Track_VEL_factor[ target_page->attr_VEL ] )
				/ VEL_FACTOR_NEUTRAL_VALUE,
				0, 127 );
		#endif // EVENTS_FACTORIZED

		#ifdef EVENTS_ABSOLUTE
		intn velocity 	=
			normalize(
				(	(		EFF_pool_VEL
						// Dice - flow shape velocity
						+ 	dice_velocity_offset
						+	target_page->Track[phys_row]->event_offset[ATTR_VELOCITY]
						+	PhrasePtGetVel( &phraseObj, noteIx )

						+ (	(	stepPt->attr_VEL
								* Track_VEL_factor[	target_page->Track[head_row]->VEL_factor ] )
							/ VEL_FACTOR_NEUTRAL_VALUE )

						+	trackBaseVelocity// target_page->Track[head_row]->attr_VEL )

					*	Track_VEL_factor[ target_page->attr_VEL ] )
				/ VEL_FACTOR_NEUTRAL_VALUE,
				0, 127 );
		#endif // EVENTS_ABSOLUTE



		//---------------------------------------------------------------------------------------
		// SEND MIDI NOTE
		//---------------------------------------------------------------------------------------

		// Channel, pitch, velocity, timestamp offset (trigger)
		MIDI_NOTE_new ( midiCH, pitch, velocity, note_offset_STA );



		//---------------------------------------------------------------------------------------
		// LENGTH - generate the NOTE OFF information
		//---------------------------------------------------------------------------------------

		// NOTE OFF data: channel, pitch, length -- so we have control over length
		// Compute the track clock divisor multiplier - adjusting the length
		if ( (attr_TEMPOMUL_SKIP & 0x0F)> 0 ) {
			j = (attr_TEMPOMUL_SKIP & 0x0F) + 1;
		}
		else {
			j = 1;
		}

		// Compute the real length. The Step length should be multiplied by the track LEN factor here..
		// Step LEN factor taken into account (10fold) and then the value normalized.
		temp = ( ( ( stepPt->attr_LEN
				// Dice - flow shape length
				+ dice_length_offset )
				// Step length multiplier
				* ((stepPt->event_data & 0xF0)>>4) )
				// LEN Factor of the track
				* Track_LEN_factor[ target_page->Track[head_row]->LEN_factor
				                    + target_page->Track[phys_row]->event_offset[ATTR_LENGTH] ] )
				                    / 16;

		temp = normalize( temp, STEP_MIN_LENGTH, STEP_MAX_LENGTH
													* ((stepPt->event_data & 0xF0)>>4) );

		// Now account for the track multiplier, but handle the triplet (1.5) and inverse triplet (1/1.5) cases;
		intn length;
		switch( target_page->Track[phys_row]->attr_TEMPOMUL ){
			case 15:
				// Triplet case
				length 	= ( ( temp + EFF_pool_LEN ) * 10 /15 ) * j;
				break;

			case 25:
				// Inverse triplet case
				length 	= ( ( temp + EFF_pool_LEN ) * 15 /10 ) * j;
				break;

			default:
				// All other cases
				length 	= ( ( temp + EFF_pool_LEN ) /  target_page->Track[phys_row]->attr_TEMPOMUL ) * j;
				break;
		}


		// Add the strum offset to the step length and stay within valid bounds
		length += PhrasePtGetLen( &phraseObj, noteIx );
		if ( length < 1 ) length = 1;

		// ROLL bit is set and nOFF bit is set - play only last nOFF
		if ( rollBo && ((stepPt->attr_STATUS & (1 << 6)) != 0 ) ) {

			j = (playNoteCt - 1);

			// Send only a note OFF for on the last note, since pitches are the same
			if ( noteIx == j ){

				// Send note off only if not legato
				if ( stepPt->attr_LEN != LEGATO	){

					// Make sure we do not accidentally get a legato effect
					temp = length + note_offset_STA;

					if ( temp >= STEP_MAX_LENGTH  * j ){
						temp = STEP_MAX_LENGTH * j;
					}

					// Channel, pitch, velocity, timestamp offset (length)
					MIDI_NOTE_new ( midiCH, pitch, OFF, temp );
				}
				else{
					// LEGATO case: Channel, pitch, velocity, timestamp offset (length)
					MIDI_NOTE_new ( midiCH, pitch, OFF, def_TIMESTAMP_MAX );
				}
			}
		}
		else {
			// Regular case: Send every MIDI OFF, since pitches are different, or nOFF bit says so
			// Defined by the MIDI NOTE OFF, unless step is set to be LEGATO
			if ( stepPt->attr_LEN != LEGATO ){

				// Make sure we do not accidentally get a legato effect
				temp = length + note_offset_STA;
				if ( temp >= STEP_MAX_LENGTH * j * ((stepPt->event_data & 0xF0)>>4) ){
					temp = STEP_MAX_LENGTH * j * ((stepPt->event_data & 0xF0)>>4);
				}

				// Channel, pitch, velocity, timestamp offset (length)
				MIDI_NOTE_new ( midiCH, pitch, OFF, temp );
			}
			else{
				// LEGATO case: Channel, pitch, velocity, timestamp offset (length)
				MIDI_NOTE_new ( midiCH, pitch, OFF, def_TIMESTAMP_MAX );
			}
		}

	} // Chord cardinality iterator
}

