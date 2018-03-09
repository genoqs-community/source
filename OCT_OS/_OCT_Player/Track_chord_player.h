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




	// This is a direct take over from the 0.92 code.

	// PLAY trigger and chord cardinality iterator
	for ( i=0; i < local_chordSize; i++ ) {

		// P I T C H
		// First iteration plays the base note
		intn pitch  = normalize( chord_offset + pitch_initial, 0, 127 );


		// FORCE TO SCALE intervention - GLOBAL has priority over LOCAL scale
		// Applies only to real MIDI channels, not VIRTUAL ones
		if ( midiCH <= 32 ){

			// Unless the page is exempt from GRID scale
			// LOCAL Force-to-Scale
			if (	( target_page->force_to_scale == ON )
				){

				// If the note index of the initial pitch is not in the scale
				if ( how_selected_in_current_scale( target_page, pitch_to_noteNdx( pitch ) ) == OFF
					){

					// Add to initial pitch the offset to the next scale degree
					pitch += offset_to_next_current_degree( target_page, pitch );
				}
			}
			// GLOBAL Force-to-Scale
			else if (	( GRID_assistant_page->force_to_scale == ON )
				){

				// If the note index of the initial pitch is not in the scale
				if ( how_selected_in_current_scale( GRID_assistant_page, pitch_to_noteNdx( pitch ) ) == OFF
					){

					// Add to initial pitch the offset to the next scale degree
					pitch += offset_to_next_current_degree( GRID_assistant_page, pitch );
				}
			} // FTS intervention
		} // real MIDI channel


		// V E L O C I T Y
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

						+ (	(	stepPt->attr_VEL
								* Track_VEL_factor[	target_page->Track[head_row]->VEL_factor
													+ target_page->Track[phys_row]->event_offset[ATTR_VELOCITY] ] )
							/ VEL_FACTOR_NEUTRAL_VALUE )

						+	trackBaseVelocity )//target_page->Track[head_row]->attr_VEL )

					*	Track_VEL_factor[ target_page->attr_VEL ] )
				/ VEL_FACTOR_NEUTRAL_VALUE,
				0, 127 );
		#endif // EVENTS_FACTORIZED

		#ifdef EVENTS_ABSOLUTE
		intn velocity 	=
			normalize(
				(	(		EFF_pool_VEL
						+	target_page->Track[phys_row]->event_offset[ATTR_VELOCITY]

						+ (	(	stepPt->attr_VEL
								* Track_VEL_factor[	target_page->Track[head_row]->VEL_factor ] )
							/ VEL_FACTOR_NEUTRAL_VALUE )

						+	trackBaseVelocity )//target_page->Track[head_row]->attr_VEL )

					*	Track_VEL_factor[ target_page->attr_VEL ] )
				/ VEL_FACTOR_NEUTRAL_VALUE,
				0, 127 );
		#endif // EVENTS_ABSOLUTE



		// NOTE OFF build new-- channel, pitch, length -- so we have control over length
		// compute the length of the OFF note
		// Compute the track clock divisor multiplier - adjusting the length
		if ( (target_page->Track[phys_row]->attr_TEMPOMUL_SKIP & 0x0F) > 0 ){
			j = (target_page->Track[phys_row]->attr_TEMPOMUL_SKIP & 0x0F) + 1;
		}
		else {
			j = 1;
		}

		// LENGTH

		// Apply LEN factor to the step length
		temp 	= (	( target_page->Step[phys_row][locator-1]->attr_LEN
//						* target_page->Step[phys_row][locator-1]->LEN_multiplier )
						* ((target_page->Step[phys_row][locator-1]->event_data & 0xF0)>>4) )
					* Track_LEN_factor[ target_page->Track[head_row]->LEN_factor
								+ target_page->Track[phys_row]->event_offset[ATTR_LENGTH] ] )
					/ 16;

		temp 	= normalize( temp, STEP_MIN_LENGTH,
					STEP_MAX_LENGTH
					* ((target_page->Step[phys_row][locator-1]->event_data & 0xF0)>>4) );
//					* target_page->Step[phys_row][locator-1]->LEN_multiplier );

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

		// Build NOTE ON - make sure to enqueue before note OFF
		MIDI_NOTE_new ( midiCH, pitch, velocity, start_offset );


		// Build NOTE OFF - accounting for the Legato case as well
		switch( target_page->Step[phys_row][locator-1]->attr_LEN ){
			case LEGATO:	length = def_TIMESTAMP_MAX;		break;
			default:		length = start_offset + length;	break;
		}
		MIDI_OFF_build_new ( midiCH, pitch, length );



		// Prepare the next iterations. Get offset to next degree in scale
		// Calculate halftone offset to next degree according to scale
		chord_offset +=
			offset_to_next_current_degree( target_page, pitch_initial + chord_offset );
		// d_iag_printf( "added c FTS1:%d\n", offset_to_next_degree( target_page, pitch_initial ) );

		// Go over to the yet next degree so we have an actual chord
		chord_offset +=
			offset_to_next_current_degree( target_page, pitch_initial + chord_offset );
		// d_iag_printf( "added c FTS2:%d\n", offset_to_next_degree( target_page, pitch_initial ) );

	} // Chord cardinality iterator







