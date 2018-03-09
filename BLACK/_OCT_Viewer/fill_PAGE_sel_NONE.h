//
// NO SELECTION IN PAGE
//

// Accomodate the BIRDSEYE view

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

			// MIX MASTER
			if (  	(VIEWER_page->trackSelection == 0)
				&&	(VIEWER_page->stepSelection  == 0)  
				){

				// Only if no track is selected
				if (VIEWER_page->mixMasterStatus == POS) {
					// show (ELE_MIX_MASTER, RED);
				}
				else {
					// show (ELE_MIX_MASTER, GREEN);
				}

				// MIX INDICATOR - LED corresponding to the rotarys
				//
				show( ELE_MIX_INDICATOR, RED   );
				show( ELE_MIX_INDICATOR, GREEN );
			}
													

			
			// MIX TARGET field
			if (  	( VIEWER_page->trackSelection == 0 )
				&&	( VIEWER_page->stepSelection  == 0 )   
				){
				// OFF here means show only the selection.. (oh well..)
				show( ELE_MIXTGT, OFF );
			}				


			// SELECT MASTER
			if (VIEWER_page->trackSelectionStored != 0) {
				// Show RCL option
				show (ELE_SELECT_MASTER, RED);
			}	



			// MUTE MASTER
			// If nothing in page is selected	
			if (	( VIEWER_page->trackSelection == 0 ) 
				&&	( VIEWER_page->stepSelection  == 0 )    ){

				// Indicate ongoing mute or solo activity
				if (	( VIEWER_page->trackMutepattern != 0 )
					||	( VIEWER_page->trackSolopattern != 0 )    ){

					show( ELE_MUTE_MASTER, GREEN );

					// Blink in the solo mode
					if ( VIEWER_page->trackSolopattern != 0 ){

					 	show( ELE_MUTE_MASTER, BLINK );
					}
				}

				// Indicate existing stored mutepattern
				else if (VIEWER_page->trackMutepatternStored != 0) {

					show (ELE_MUTE_MASTER, RED);
				}
			}


			// TRACK CHAINING
			show( ELE_CHAIN_INDICATOR, ON );


			// TRACK MUTATORS
			// Default: solo and mute patterns, concurrently
//			if (VIEWER_page->trackSolopattern != 0) {
				show (ELE_TRACK_MUTATORS, TRACK_SOLOPATTERN);
//			}
//			else {
				show (ELE_TRACK_MUTATORS, TRACK_MUTEPATTERN);
//			}
			
			
			// EDIT MASTER
			// Activity only when no track is selected
			if ( VIEWER_page->trackSelection == 0 ){
		
				switch( VIEWER_page->editorMode ){
		
					case CONTROL:		
						// Edit acts as MCC source - send MCC data
						show( ELE_EDIT_MASTER, GREEN );
						show( ELE_EDIT_MASTER, RED );
						break;

					case EDIT:
						// This is the regular mode - edit attributes of selection
						show( ELE_EDIT_MASTER, GREEN );
						break;

					case PREVIEW:
						// Same as regular mode, but preview the steps being edited
						show( ELE_EDIT_MASTER, RED );
						break;	
				}
			}


			// EDIT INDICATOR
			// No track or step is selected, indicate MCC activity of EDIT block
			if ( VIEWER_page->editorMode == MIX2EDIT ){

				show( ELE_EDIT_INDICATOR, RED   );
				show( ELE_EDIT_INDICATOR, GREEN );				
			}
			

			// GLOBALS
			if ( VIEWER_page->trackSelection == 0 ){

				show( ELE_GLOBALS, TRANSPORT );	
				show( ELE_GLOBALS, NUMERIC_QUADRANT );
				show( ELE_GLOBALS, BIG_KNOB );
				show( ELE_GLOBALS, CLOCK );
				show( ELE_GLOBALS, FOLLOW );			

				show_GLOBALS_scale_master( VIEWER_page );
			}


			// OCTAVE CIRCLE
			// In PAGE mode show the SCALE, trans-position is in GRID
			// ..but make sure we are not in BIRDSEYE mode
			if (	( 	( VIEWER_page->scaleStatus == SCALE_SEL )
//					|| 	( VIEWER_page->scaleStatus == SCALE_MOD ) 	
					)
				&&	( VIEWER_page->OPS_mode != BIRDSEYE )
				){
				
				// Show the selected notes in scale. Both in MOD and SEL
				// show( ELE_OCTAVE_CIRCLE,  SCALE_SELECTION );
				show_OCTAVE_CIRCLE_scale_selection( VIEWER_page );
				show_SCALE_SELECTOR_scale_selection( VIEWER_page );
			}
			
			// When forced to scale, show the scale
			if (	( VIEWER_page->scaleStatus == SCALE_MOD )
//					( VIEWER_page->force_to_scale == ON )
				&&	( VIEWER_page->OPS_mode != BIRDSEYE )
				){

				// Show the selected notes in scale. Both in MOD and SEL
				show_SCALE_SELECTOR_scale_selection( VIEWER_page );
				show_OCTAVE_CIRCLE_scale_selection( VIEWER_page );
			}		

			break; // break for default:


	} // switch( VIEWER_page->OPS_mode

