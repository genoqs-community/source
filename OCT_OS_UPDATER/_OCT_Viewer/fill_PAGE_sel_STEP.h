
	switch( target_page->OPS_mode ){
				
		// Shows the page functions needed globally
		case BIRDSEYE:
		
//d_iag_printf( "viewing BIRDSEYE..\n" );
			// Show the page functions as they are visible in the grid

			// TRACK SELECTORS
			show (ELE_TRACK_SELECTORS, GRID_BANK_PLAYMODES);

			// TRACK MUTATORS
			show( ELE_TRACK_MUTATORS, PAGE_FUNCTIONS );
			// Caution!! There is some extra code in the ELE_MATRIX / GRID section.
			
			// OCTAVE CIRCLE
			show( ELE_OCTAVE_CIRCLE, PAGE_TRANSPOSITION );
			
			// EDIT INDICATOR
			show( ELE_EDIT_INDICATOR, RED );
		
			// MATRIX
			show( ELE_MATRIX, GRID );

			// Show the GRID cursor
			temp = cursor_to_dot( GRID_CURSOR );
			MIR_write_dot( temp, MIR_RED   );
			MIR_write_dot( temp, MIR_GREEN );
			MIR_write_dot( temp, MIR_BLINK );
			break;		


		// This is the general case	
		case INTERACTIVE:
		case PASSIVE:

			// SELECT MASTER 
			if (target_page->stepSelection  != 0) {
				// BLINK active for step selection
				show (	ELE_SELECT_MASTER, RED 		);
				show (	ELE_SELECT_MASTER, GREEN 	);
				show (	ELE_SELECT_MASTER, BLINK 	);
			}


			// TRACK MUTATORS
			if (target_page->stepSelection != 0) {
				// Step rotarys shown
				show (ELE_TRACK_MUTATORS, STEP_FUNCTIONS);
			}


			// EDIT INDICATOR
			if (target_page->stepSelection != 0) {

				// Active when a step is selected - show " ' "
				show( ELE_EDIT_INDICATOR, GREEN );

				if (EDIT_TIMER != OFF){
					show( ELE_EDIT_INDICATOR, BLINK );
				}					
			}

			
			// CIRCLES showing step VEL and PIT
			if ( target_page->stepSelection == 1 ){
			
				show_pitch_in_circle( 
					target_page->Track[target_page->stepSelectionSingleRow]->attribute[ATTR_PITCH] +
					target_page->Step[ target_page->stepSelectionSingleRow ]
									 [ target_page->stepSelectionSingleCol ]->attribute[ATTR_PITCH], ON );
		
				show_velocity_in_circle(
					normalize( 
						target_page->Track[target_page->stepSelectionSingleRow]->attribute[ATTR_VELOCITY] +
						target_page->Step[ target_page->stepSelectionSingleRow ]
										 [ target_page->stepSelectionSingleCol ]->attribute[ATTR_VELOCITY],
						0, 127 ) );			
			}

			break;
			
	} // Ops mode switch

	// Transport status
	show( ELE_GLOBALS, TRANSPORT );	


