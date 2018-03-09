
	//
	// Play every pitch of the step chord
	//
 	
 	for ( i=0; i < (local_chordSize+1); i++ ) {

		// ..local_chordSize+1 was necessary to enter loop! - update load indicator
		PLAY_LOAD ++;

		// STRUM PREPARE - depending on strum direction
		switch( step_strum_dir ){

			case NEG:	
				step_strum_ndx = local_chordSize - i;
				break;

			case POS:
				step_strum_ndx = i;
				break;
		}

		// STRUM OFFSETS: check for the ROLL condition - no ROLL allows pitch changes.
		if ( (target_page->Step[row][locator-1]->attribute[ATTR_STATUS] & (1 << 7)) == 0 ){
			strum_offset_PIT = OCT_step_strum_PIT [step_strum_ndx][step_strum_level];
		}
		strum_offset_VEL = OCT_step_strum_VEL [step_strum_ndx][step_strum_level];
		strum_offset_LEN = OCT_step_strum_LEN [step_strum_ndx][step_strum_level];
		strum_offset_STA = OCT_step_strum_STA [step_strum_ndx][step_strum_level];
		
		// Adjust here the strum_offset_STA with the start_factor.
		// ...
		strum_offset_STA += start_offset;

		// And don't forget the Track chord player version of the code.


		// P I T C H
		// First iteration plays the base note
		pitch  		= normalize( 	pitch_initial
								+	chord_offset 
								+	strum_offset_PIT,
						  	0, 127 );

		// FORCE TO SCALE intervention - LOCAL has priority over GLOBAL scale
		if (	( target_page->force_to_scale == ON )
			){
			// If the note index of the initial pitch is not in the scale
			if ( how_selected_in_current_scale( target_page, pitch_to_noteNdx( pitch ) ) == OFF 
				){		
				// Add to initial pitch the offset to the next scale degree
				pitch += offset_to_next_current_degree( target_page, pitch ); 
			}
		}	
		else if (	( GRID_assistant_page->force_to_scale == ON )
			){

			// If the note index of the initial pitch is not in the scale
			if ( how_selected_in_current_scale( GRID_assistant_page, pitch_to_noteNdx( pitch ) ) == OFF 
				){		
				// Add to initial pitch the offset to the next scale degree
				pitch += offset_to_next_current_degree( GRID_assistant_page, pitch ); 
			}
		} // FTS intervention



		// V E L O C I T Y 
		velocity 	= normalize(	
					EFF_pool_VEL
				+	target_page->Track[row]->event_VEL
				+	strum_offset_VEL
				
//				+	target_page->Step [row][locator-1]->attribute[ATTR_VELOCITY]
				+ (	(	target_page->Step [row][locator-1]->attribute[ATTR_VELOCITY]
					* Track_VEL_factor[target_page->Track[row]->VEL_factor] ) / 50 )
				
				+	target_page->Track[row]->attribute[ATTR_VELOCITY]
				+	target_page->attribute[ATTR_VELOCITY], 
				0, 127 );

		// NOTE OFF data: channel, pitch, length -- so we have control over length
		// Compute the track clock divisor multiplier - adjusting the length
		if ( (target_page->Track[row]->attribute[ATTR_TEMPOMUL_SKIP] & 0x0F) > 0 ){
			j = (target_page->Track[row]->attribute[ATTR_TEMPOMUL_SKIP] & 0x0F) + 1;
		}
		else {
			j = 1;
		}
		// Compute the real length. The Step length should be multiplied by the track LEN factor here.. 
		// Step LEN factor taken into account (10fold) and then the value normalized.
		temp 	= 	(	((unsigned char) target_page->Step[row][locator-1]->attribute[ATTR_LENGTH])
					* Track_LEN_factor[ target_page->Track[row]->LEN_factor ] ) 
					/ 10;
		temp 	= (unsigned char) normalize( temp, STEP_MIN_LENGTH, (unsigned char)STEP_MAX_LENGTH );

		length 	= length2_ticks	(	( 	(	temp + EFF_pool_LEN )
										/  target_page->Track[row]->attribute[ATTR_TEMPOMUL] 
									)
									* j	);

		// Add the strum offset to the step length and stay within bounds
		length 	+= 	strum_offset_LEN;
		if ( length < 1 ) length = 1;

		
		//
		// L E N G T H    - determined by the NOTE OFF.. dependent on the ROLL and nOFF bit
		//
		
		// ROLL bit is set and nOFF bit is set - play only last nOFF
		if ( 	((target_page->Step[row][locator-1]->attribute[ATTR_STATUS] & (1 << 7)) != 0 )
			&&	((target_page->Step[row][locator-1]->attribute[ATTR_STATUS] & (1 << 6)) != 0 )
			){

			if ( step_strum_dir == NEG ){
				j = 0;
			}
			else{
				j = (local_chordSize+1 - 1);
			}

			// Send only a note OFF for on the last note, since pitches are the same
			if ( i == j ){

				if ( 	( ((unsigned char)target_page->Step[row][locator-1]->attribute[ATTR_LENGTH]) 
							!= (unsigned char)STEP_MAX_LENGTH )
					){
			
					// Make sure we do not accidentally get a legato effect
					temp = length + length2_ticks( strum_offset_STA );
					if ( temp >= (unsigned char)STEP_MAX_LENGTH ){
						temp = ((unsigned char)STEP_MAX_LENGTH) - 1;
					}
					
					// Channel, pitch, velocity, timestamp offset (length)
					MIDI_NOTE_new ( midiCH, pitch, OFF, temp );
				}
				else{
					// LEGATO case: Channel, pitch, velocity, timestamp offset (length)
					MIDI_NOTE_new ( midiCH, pitch, OFF, TIMESTAMP_MAX );
				}
			}
		}
		else {
			// Regular case: Send every MIDI OFF, since pitches are different, or nOFF bit says so
			// Defined by the MIDI NOTE OFF, unless step is set to be LEGATO
			if ( 	( ((unsigned char)target_page->Step[row][locator-1]->attribute[ATTR_LENGTH]) 
						!= (unsigned char)STEP_MAX_LENGTH )
				){
		
				// Make sure we do not accidentally get a legato effect
				temp = length + length2_ticks( strum_offset_STA );
				if ( temp >= (unsigned char)STEP_MAX_LENGTH ){
					temp = ((unsigned char)STEP_MAX_LENGTH) - 1;
				}
				
				// Channel, pitch, velocity, timestamp offset (length)
				MIDI_NOTE_new ( midiCH, pitch, OFF, temp );
			}
			else{
				// LEGATO case: Channel, pitch, velocity, timestamp offset (length)
				MIDI_NOTE_new ( midiCH, pitch, OFF, TIMESTAMP_MAX );
			}
		}
		
		
		// MIDI NOTE ON send - channel, pitch, velocity, timestamp offset (trigger)
		MIDI_NOTE_new ( midiCH, pitch, velocity, length2_ticks( strum_offset_STA ) ); 


		// CHORD OFFSET
		// Compute the offset to the next pitch to be played - if Step is not in ROLL mode.
		if( (target_page->Step[row][locator-1]->attribute[ATTR_STATUS] & (1 << 7)) == 0 ){
			
			chord_offset += offset_to_next_chord_aux( step_chord_pattern, chord_offset );
		}
		
	} // Chord cardinality iterator


