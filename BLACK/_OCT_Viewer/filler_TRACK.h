


	switch (VIEWER_page->keyMode) {
	//
	// T R A C K  MODE
	//
		case KMOD_THIS_TRACK:
		
			// MIX INDICATOR - LED corresponding to the rotarys
			// - The rotarys select the track to be edited in TRACK mode
			show( ELE_MIX_INDICATOR, RED   );
			show( ELE_MIX_INDICATOR, GREEN );

			// TRACK Selectors
			switch( MODE_OBJECT_SELECTION ){

				// Select change track selection
				case BIRDSEYE: 
					// Blink the currently selected track
					i = my_bit2ndx( VIEWER_page->trackSelection ) + 1;
					MIR_write_dot( i, MIR_RED   );
					MIR_write_dot( i, MIR_GREEN );
					MIR_write_dot( i, MIR_BLINK );

					// MATRIX
					// Requested by Tom - to see the step status in birdsview
					show( ELE_MATRIX, STEP_TOGGLE );

					break;

				default:
					show (ELE_TRACK_SELECTORS, TRACK_ATTRIBUTES_MAPS);			
					show (ELE_TRACK_SELECTORS, TRACK_ATTRIBUTE_SELECTION);
					
					// MATRIX
					// Per row hardwired track attribute values
					show( ELE_MATRIX, HARDWIRED_ATTRIBUTE_VALUES );
					
					// If track is not muted show lauflicht
					for (i=0; i<MATRIX_NROF_ROWS; i++) {
						if ( (VIEWER_page->trackSelection & (1<<i)) > 0 ) {
							//  row is index of the first selected track
							row = i;
						}
					}
								break;
			}
						
			
			// Show the lauflicht in track
			//if ((VIEWER_page->trackMutepattern & (1 << row )) == 0) {
			show (ELE_MATRIX, LAUFLICHT_TRACK) ;
			//}
																	
			// RETURN
			show (ELE_RETURN, GREEN);

			// TRACK MUTATORS
			if (VIEWER_page->trackAttributeSelection == 0) {
				
				show( ELE_TRACK_MUTATORS, TRACK_FUNCTIONS );
			} else {

				show( ELE_TRACK_MUTATORS, TRACK_ATTRIBUTE_FUNCTIONS );
			}

			// MUTE MASTER
			// show (ELE_MUTE_MASTER, GREEN);

			// EDIT MASTER
			show (ELE_EDIT_INDICATOR, GREEN);
			show (ELE_EDIT_INDICATOR, RED);
			if (VIEWER_page->trackAttributeSelection == 0){
				// show (ELE_EDIT_MASTER, BLINK);
			}

			// GLOBALS
			show( ELE_CHAIN_INDICATOR, ON );
			show (ELE_GLOBALS, TRANSPORT);	
			show ( ELE_GLOBALS, NUMERIC_QUADRANT );
			
			show (ELE_GLOBALS, BIG_KNOB);
			// show( ELE_GLOBALS, CLOCK );
			show( ELE_GLOBALS, FOLLOW );


			// TEMPOMUL VALUE for the TRACK
			display_Track_TEMPOMUL( 
				VIEWER_page->Track[ my_bit2ndx( VIEWER_page->trackSelection ) ] );


			// CHORD SELECTOR
			if ( my_bit_cardinality( VIEWER_page->trackSelection ) == 1 ){
				show( ELE_CHORD_SELECTOR, CHORD_SIZE_TRACK );									
			}


			// ALIGN - Indicate that ALIGN is available
			if ( VIEWER_page->scaleStatus == OFF ){
	
				// Check whether alignment of page is needed
				// ..save some CPU cycles here.. 
				// if ( page_needs_align( VIEWER_page ) ){
						
					MIR_write_dot( LED_ALIGN, MIR_GREEN );
				// }
			}

			// OCTAVE CIRCLE - showing the track pitch, ready for trans-position
			show( ELE_OCTAVE_CIRCLE, TRACK_TRANSPOSITION );


			// RECORDING LED			
			switch ( VIEWER_page->track_REC_pattern == VIEWER_page->trackSelection ){

				case TRUE:
					MIR_write_dot( LED_RECORD, MIR_RED 	 );
					// Blink when no ON notes are pending..
					if ( NOTE_queue_counter == 0 ){
						MIR_write_dot( LED_RECORD, MIR_BLINK );						
					}
					break;

				case FALSE:
					if ( VIEWER_page->track_REC_pattern != 0 ){
						MIR_write_dot( LED_RECORD, MIR_GREEN );
						// Blink when no ON notes are pending..
						if ( NOTE_queue_counter == 0 ){
							MIR_write_dot( LED_RECORD, MIR_BLINK );						
						}
					}
					else{
						MIR_write_dot( LED_RECORD, MIR_GREEN );
					}
					break;
			}
			break;


		// 
		// THIS TRACK EXTENDED mode - shows the EDIT-ed selection's attribute values
		//
		case KMOD_THIS_TRACK_EXTENDED:

			// TRACK Selectors
			show (ELE_TRACK_SELECTORS, EDIT_ATTRIBUTE);
								
			// MATRIX
			// Per row hardwired track attribute values
			show (ELE_MATRIX, EXTENDED_ATTRIBUTE_VALUES);

			// show (ELE_MATRIX, LAUFLICHT_TRACK) ;
								
			// RETURN
			show (ELE_RETURN, GREEN);

			// MUTATORS
			if (VIEWER_page->trackAttributeSelection == 0) {
				
				show (ELE_TRACK_MUTATORS, TRACK_FUNCTIONS);
			} else {

				show (ELE_TRACK_MUTATORS, TRACK_ATTRIBUTE_FUNCTIONS);
			}

			// EDIT MASTER
			// show (ELE_EDIT_MASTER, GREEN);

			// GLOBALS
			show (ELE_GLOBALS, TRANSPORT);	
			show (ELE_GLOBALS, NUMERIC_QUADRANT);
			show (ELE_GLOBALS, BIG_KNOB);
			show( ELE_GLOBALS, CLOCK );

			// ALIGN - Indicate that ALIGN is available
			if ( VIEWER_page->scaleStatus == OFF ){
	
				// Check whether alignment of page is needed
				if ( page_needs_align( VIEWER_page ) ){
						
					MIR_write_dot( LED_ALIGN, MIR_GREEN );
				}
			}

			break;
	}

