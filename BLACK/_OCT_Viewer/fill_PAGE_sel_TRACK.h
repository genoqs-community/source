

			// Show CLR option
			show (ELE_SELECT_MASTER, GREEN);

			// TRACK MUTATORS
			// Track functions shown
			show ( ELE_TRACK_MUTATORS, TRACK_FUNCTIONS );

			// CHAINER LED
			MIR_write_dot( LED_CHAINER, MIR_GREEN );
			

			// EDIT INDICATOR
			// Show activation status: all are active
			show( ELE_EDIT_INDICATOR, RED   );
			show( ELE_EDIT_INDICATOR, GREEN );
			// show( ELE_EDIT_INDICATOR, BLINK );
			
			// Only one track is selected
			if (	( my_bit_cardinality( VIEWER_page->trackSelection ) == 1 )
				&&	( my_bit2ndx( VIEWER_page->trackSelection ) != 
						VIEWER_page->attr_mix2edit - 1 )
				){ 
				
				show( ELE_EDIT_INDICATOR, BLINK );

				// Show the EDIT MASTER in green
				show( ELE_EDIT_MASTER, GREEN );
			}
			
			if (EDIT_TIMER != OFF){
				show( ELE_EDIT_INDICATOR, BLINK );
			}					
			

			// GLOBALS
			show( ELE_GLOBALS, NUMERIC_QUADRANT );
			show( ELE_GLOBALS, BIG_KNOB );
			// ESC
			MIR_write_dot( LED_RETURN, MIR_GREEN );


			// TEMPOMUL of track
			// If one track (only) is selected in page
			if ( my_bit_cardinality( VIEWER_page->trackSelection ) == 1 ){

				// Display its tempo multiplier value
				display_Track_TEMPOMUL( 
					VIEWER_page->Track[ my_bit2ndx( VIEWER_page->trackSelection ) ] );

				// Show the recording status for the track
//				if ( ( VIEWER_page->Track[ my_bit2ndx( VIEWER_page->trackSelection ) ]
//					->attribute[ATTR_MISC] & (1<<5)) != 0 
				if ( ( VIEWER_page->trackSelection & VIEWER_page->track_REC_pattern ) != 0
					){
					MIR_write_dot( LED_RECORD, MIR_RED );
				}
				else{
					MIR_write_dot( LED_RECORD, MIR_GREEN );
				}
			}


			// OCTAVE CIRCLE
			show (ELE_OCTAVE_CIRCLE, TRACK_TRANSPOSITION);


			// CHORD SELECTOR
			if ( my_bit_cardinality( VIEWER_page->trackSelection ) == 1 ){
				show( ELE_CHORD_SELECTOR, CHORD_SIZE_TRACK );									
			}

