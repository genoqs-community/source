
	switch( VIEWER_page->OPS_mode ){
				
		// Shows the page functions needed globally
		case BIRDSEYE:
		
//d_iag_printf( "viewing BIRDSEYE..\n" );
			// Show the page functions as they are visible in the grid

			// TRACK SELECTORS
			show (ELE_TRACK_SELECTORS, GRID_BANK_PLAYMODES);

			// TRACK MUTATORS
			show( ELE_TRACK_MUTATORS, PAGE_FUNCTIONS);
			// Caution!! There is some extra code in the ELE_MATRIX / GRID section.
			
			// OCTAVE CIRCLE
			show( ELE_OCTAVE_CIRCLE, PAGE_TRANSPOSITION);
			
			// EDIT INDICATOR
			show( ELE_EDIT_INDICATOR, RED );
		
			// MATRIX
			show( ELE_MATRIX, GRID );

			// Show the GRID cursor
			temp = cursor_to_dot( GRID_CURSOR );
			MIR_write_dot( temp, MIR_RED   );
			MIR_write_dot( temp, MIR_GREEN );
			MIR_write_dot( temp, MIR_BLINK );

			// no break, Fallthrough intended
			break;		

		// This is the general case	
		case INTERACTIVE:
		case PASSIVE:

			// SELECT MASTER 
			if (VIEWER_page->stepSelection  != 0) {
				// BLINK active for step selection
				show (	ELE_SELECT_MASTER, RED 		);
				show (	ELE_SELECT_MASTER, GREEN 	);
				show (	ELE_SELECT_MASTER, BLINK 	);
			}


			// TRACK MUTATORS
			if (VIEWER_page->stepSelection != 0) {
				// Step rotarys shown
				show (ELE_TRACK_MUTATORS, STEP_FUNCTIONS);
			}


			// EDIT INDICATOR
			if (VIEWER_page->stepSelection != 0) {

				// Active when a step is selected - show " ' "
				show( ELE_EDIT_INDICATOR, GREEN );

				if (EDIT_TIMER != OFF){
					show( ELE_EDIT_INDICATOR, BLINK );
				}					
			}
			break;
			
	} // Ops mode switch

	// Transport status
	show( ELE_GLOBALS, TRANSPORT );	


