
	// This is a direct take over from the 0.92 code.

	// PLAY trigger and chord cardinality iterator
	for ( i=0; i < local_chordSize; i++ ) {

		// Add track to concurrent play 
		PLAY_LOAD ++;

		// P I T C H
		// First iteration plays the base note
		pitch  = normalize( chord_offset + pitch_initial, 0, 127 );


		// FORCE TO SCALE intervention - GLOBAL has priority over LOCAL scale
		// Applies only to real MIDI channels, not VIRTUAL ones
		if ( midiCH <= 32 ){

			// Unless the page is exempt from GRID scale
			// GLOBAL Force-to-Scale
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
		velocity 	= 
			normalize(	
				(	(		EFF_pool_VEL
						+	strum_offset_VEL
				
						+ (	(	target_page->Step [phys_row][locator-1]->attribute[ATTR_VELOCITY]
								* Track_VEL_factor[	target_page->Track[head_row]->VEL_factor
													+ target_page->Track[phys_row]->event_offset[ATTR_VELOCITY] ] ) 
							/ FACTOR_NEUTRAL_VALUE )		

						+	target_page->Track[head_row]->attribute[ATTR_VELOCITY] )

					*	Track_VEL_factor[ target_page->attribute[ATTR_VELOCITY] ] )
				/ FACTOR_NEUTRAL_VALUE, 
				0, 127 );




		// NOTE OFF build new-- channel, pitch, length -- so we have control over length
		// compute the length of the OFF note
		// Compute the track clock divisor multiplier - adjusting the length
		if ( (target_page->Track[phys_row]->attribute[ATTR_TEMPOMUL_SKIP] & 0x0F) > 0 ){
			j = (target_page->Track[phys_row]->attribute[ATTR_TEMPOMUL_SKIP] & 0x0F) + 1;
		}
		else {
			j = 1;
		}

		// Compute the real length
		
		// Apply LEN factor to the step length 		
		temp 	= 	(	((unsigned char) target_page->Step[phys_row][locator-1]->attribute[ATTR_LENGTH])
					* Track_LEN_factor[ target_page->Track[head_row]->LEN_factor 
										+ target_page->Track[phys_row]->event_offset[ATTR_LENGTH] ] )
					/ 16;
		temp 	= (unsigned char) normalize( temp, STEP_MIN_LENGTH, (unsigned char)STEP_MAX_LENGTH );

		// Convert absolute length to clock ticks ( 1/192's )
		length 	= length2_ticks	(	( 	(	temp + EFF_pool_LEN )
										/  target_page->Track[phys_row]->attribute[ATTR_TEMPOMUL] 
									)
									* j	);


		// Build NOTE OFF - for non-legato steps
		if ( (unsigned char) target_page->Step[phys_row][locator-1]->attribute[ATTR_LENGTH] 
				!= (unsigned char) STEP_MAX_LENGTH ){

			// -- the non-legato steps		
			MIDI_OFF_build_new ( midiCH, pitch, start_offset + length);
			// d_iag_printf( "                          length:%d\n", length );
		}
		else{

			// --and the legato steps
			MIDI_OFF_build_new ( midiCH, pitch, TIMESTAMP_MAX );
		}
				

		// Build NOTE ON 
		// MIDI NOTE ON send - channel, pitch, velocity, timestamp offset (i.e. trigger time)
		MIDI_NOTE_new ( midiCH, pitch, velocity, start_offset ); 


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







