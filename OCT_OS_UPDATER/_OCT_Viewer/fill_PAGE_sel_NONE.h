//
// NO SELECTION IN PAGE
//

// Accomodate the BIRDSEYE view


	switch( target_page->OPS_mode ){
				
		// Shows the page functions needed globally
		case BIRDSEYE:

			// Show the page functions as they are visible in the grid

			// TRACK SELECTORS
			show( ELE_TRACK_SELECTORS, GRID_BANK_PLAYMODES );

			// TRACK MUTATORS
			show( ELE_TRACK_MUTATORS, PAGE_FUNCTIONS );
			// Caution!! There is some extra code in the ELE_MATRIX / GRID section.
			
			// OCTAVE CIRCLE
			show( ELE_OCTAVE_CIRCLE, PAGE_TRANSPOSITION );
			
			// VELOCITY CIRCLE
			// MIR_write_numeric_C( target_page->attribute[ATTR_VELOCITY] );
			MIR_write_factor_C( target_page->attribute[ATTR_VELOCITY] );

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
			if (  	(target_page->trackSelection == 0)
				&&	(target_page->stepSelection  == 0)  
				){

				// Only if no track is selected
				if (target_page->mixMasterStatus == POS) {
					show (ELE_MIX_MASTER, RED);
				}
				else {
					show (ELE_MIX_MASTER, GREEN);
				}

				// MIX INDICATOR - LED corresponding to the rotarys
				show( ELE_MIX_INDICATOR, RED   );
				show( ELE_MIX_INDICATOR, GREEN );
			}
													

			
			// MIX TARGET field
			if (  	( target_page->trackSelection == 0 )
				&&	( target_page->stepSelection  == 0 )   
				){
				// OFF here means show only the selection.. (oh well..)
				show( ELE_MIXTGT, OFF );
			}				


			// SELECT MASTER
			if (target_page->trackSelectionStored != 0) {
				// Show RCL option
				show (ELE_SELECT_MASTER, RED);
			}	



			// MUTE MASTER
			// If nothing in page is selected	
			if (	( target_page->trackSelection == 0 ) 
				&&	( target_page->stepSelection  == 0 )    ){

				// Indicate ongoing mute or solo activity
				if (	( target_page->trackMutepattern != 0 )
					||	( target_page->trackSolopattern != 0 )    ){

					show( ELE_MUTE_MASTER, GREEN );

					// Blink in the solo mode
					if ( target_page->trackSolopattern != 0 ){

					 	show( ELE_MUTE_MASTER, BLINK );
					}
				}

				// Indicate existing stored mutepattern
				else if (target_page->trackMutepatternStored != 0) {

					show (ELE_MUTE_MASTER, RED);
				}
			}


			// TRACK CHAINING
			show( ELE_CHAIN_INDICATOR, ON );


			// TRACK MUTATORS
			// Default: solo and mute patterns, concurrently
			show (ELE_TRACK_MUTATORS, TRACK_SOLOPATTERN);
			show (ELE_TRACK_MUTATORS, TRACK_MUTEPATTERN);
			
			
			// EDIT MASTER
			// Activity only when no track is selected
			if ( target_page->trackSelection == 0 ){
		
				switch( target_page->editorMode ){
		
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
			if ( target_page->editorMode == MIX2EDIT ){

				show( ELE_EDIT_INDICATOR, RED   );
				show( ELE_EDIT_INDICATOR, GREEN );				
			}
			

			// GLOBALS
			if ( target_page->trackSelection == 0 ){

				show( ELE_GLOBALS, TRANSPORT );	
				show( ELE_GLOBALS, NUMERIC_QUADRANT );
				show( ELE_GLOBALS, BIG_KNOB );
				show( ELE_GLOBALS, CLOCK );
				show( ELE_GLOBALS, FOLLOW );			

				show_GLOBALS_scale_master( target_page );
			}


			// OCTAVE CIRCLE
			// In PAGE mode show the SCALE, trans-position is in GRID
			// ..but make sure we are not in BIRDSEYE mode
			if (	(	( target_page->scaleStatus == SCALE_SEL ) 	
					||	( target_page->scaleStatus == SCALE_MOD ) )
				&&	( target_page->OPS_mode != BIRDSEYE )
				){
				
				// Show the selected notes in scale. Both in MOD and SEL
				show_OCTAVE_CIRCLE_scale_selection( target_page );
				show_SCALE_SELECTOR_scale_selection( target_page );
			}
			

			break; // break for default:


	} // switch( target_page->OPS_mode

