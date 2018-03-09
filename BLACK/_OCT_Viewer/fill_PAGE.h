
			// MATRIX
			show (ELE_MATRIX, STEP_TOGGLE);
			
			// Track selection has priority over Step Selection
			if (VIEWER_page->trackSelection != 0) {
				// Show selected tracks
//			 	show (ELE_MATRIX, TRACK_SELECTION);
			} 
			else {
				// Show selected steps
				show (ELE_MATRIX, STEP_SELECTION);
			}
			
			// Show step skips
			show (ELE_MATRIX, STEP_SKIPS);
			
			// Track boundaries
			show (ELE_MATRIX, TRACK_BOUNDARIES);
			
			// Lauflicht - this is the Tommi request
			if (LAUFLICHT_DISPLAY == ON){
				
				show( ELE_MATRIX, LAUFLICHT );
			}
			

			// If the editor is just being used
			if ( EDIT_TIMER == ON ) {
				show (ELE_MATRIX, EDIT_ATTRIBUTE);
				MIR_write_dot( LED_RETURN, MIR_GREEN );
			}

			// If the MIXER is just being used
			if ( MIX_TIMER == ON ) {
				show( ELE_MATRIX, EXTENDED_ATTRIBUTE_VALUES );
				MIR_write_dot( LED_RETURN, MIR_GREEN );
			}


			// RETURN
			if (	
					( VIEWER_page->stepSelection  	!= 0 ) 
				|| 	( VIEWER_page->scaleStatus 		!= OFF )   
				){
				
				show (ELE_RETURN, GREEN);
			}


			// ALIGN - Indicate that ALIGN is available
			if ( VIEWER_page->force_to_scale == OFF ){
						
				MIR_write_dot( LED_ALIGN, MIR_GREEN );
				
				// In track mode this is orange
				if ( VIEWER_page->trackSelection != 0 ){

					MIR_write_dot( LED_ALIGN, MIR_RED 	);
					MIR_write_dot( LED_ALIGN, MIR_BLINK );
				}
			}

			
			// PLAY mode
			switch( PLAY_MODE_STATUS ){
			
				case ON:
					MIR_write_dot( LED_ZOOM_PLAY, MIR_GREEN );
					MIR_write_dot( LED_ZOOM_PLAY, MIR_BLINK );

					MIR_write_dot( LED_PROGRAM, MIR_RED 	);
					// MIR_write_dot( LED_PROGRAM, MIR_BLINK 	);					
					break;
					
				case OFF:
					MIR_write_dot( LED_ZOOM_PLAY, MIR_RED );
					break;
			}
			
