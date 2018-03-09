
	

// d_iag_printf( "GRIDTRACK_OPS_mode:%d, keyNdx:%d\n", GRIDTRACK_OPS_mode, keyNdx );


	//
	// INTERACTIVE OPS MODE
	// 	
	if( GRIDTRACK_OPS_mode == INTERACTIVE ){


		if ( keyNdx == KEY_SELECT_MASTER ){
		
			// Fetch the stored selections into the selections
			for ( row=0; row < GRID_NROF_BANKS;  row++ ){
		
				if ( GRID_p_selection[row] != NULL ){
			
					GRID_p_selection[row]->trackSelection = 
						GRID_p_selection[row]->trackSelectionStored;
				}
			} // row iterator			

			GRIDTRACK_OPS_mode = PASSIVE;
		}


		// GRID mode switch
		if (	( keyNdx == KEY_ZOOM_TRACK )
			||	( keyNdx == KEY_ZOOM_GRID )
			){
		
			zoom_level = zoomGRID;
		}	
		
		
				//	
		// GRID   C C M A P   MODE
		//
		if ( GRID_play_mode == GRID_CCMAP ){
		
			// MIX TARGETS
			if (	( 	// ( (keyNdx == KEY_MIXTGT_USR0) && (GRID_assistant_page->mixTarget == MIXTGT_USR0) )
						( (keyNdx == KEY_MIXTGT_USR1) && (GRID_assistant_page->mixTarget == MIXTGT_USR1) )
					||	( (keyNdx == KEY_MIXTGT_USR2) && (GRID_assistant_page->mixTarget == MIXTGT_USR2) ) 
					||	( (keyNdx == KEY_MIXTGT_USR3) && (GRID_assistant_page->mixTarget == MIXTGT_USR3) )
					||	( (keyNdx == KEY_MIXTGT_USR4) && (GRID_assistant_page->mixTarget == MIXTGT_USR4) )
					||	( (keyNdx == KEY_MIXTGT_USR5) && (GRID_assistant_page->mixTarget == MIXTGT_USR5) ) )
				){	
					
				GRID_play_mode 								= GRID_MIX;
				GRIDTRACK_OPS_mode 							= INTERACTIVE;
				GRID_assistant_page->CC_MIXMAP_attribute 	= CC_MIXMAP_AMT;
				// d_iag_printf( "keyGRID - switched to MIX / interactive\n" );
			}
		
			// TRACK SELECTORS - select the CC attribute under investigation
			else if ((keyNdx >= 1) && (keyNdx <= 10)) {
				
				// CC_MIXMAP mode selected
				switch( keyNdx ) {
		
					case 7: 
						GRID_assistant_page->CC_MIXMAP_attribute = CC_MIXMAP_AMT;
						break;
		
					case 9:
						GRID_assistant_page->CC_MIXMAP_attribute = CC_MIXMAP_MCC;
						break;
		
					case 10:
						GRID_assistant_page->CC_MIXMAP_attribute = CC_MIXMAP_MCH;
						break;
				}
			}
		}
		// SELECTORS
		else if ((keyNdx >0) && (keyNdx <= 10)) {
	
			// Toggle the playmodes for the GRID bank.
			GRID_bank_playmodes ^= 1 << (keyNdx-1);
		}
				

		// ALIGN KEY
		// Set locators of playing pages to the GLOBAL LOCATOR
		if ( keyNdx == KEY_ALIGN ){
	
			// Freeze the global locator value
			j = GLOBAL_LOCATOR;
			k = TTC_ABS_VALUE;
			
			for ( i=0; i< GRID_NROF_BANKS; i++ ){
	
				// If the page is currently playing
				if ( GRID_p_selection[i] != NULL ){
	
					set_page_locators( 	GRID_p_selection[i], j, k );
				}
			}
		}
			

		// MATRIX
		// operate on the virtual track selectors..
		if ( (keyNdx >= 11) && (keyNdx <= 185) ){
			
			switch( keyNdx ){
				
				// These don't belong to the matrix. Only accidentally in the range, do nothing.
				case 21: 	case 32: 	case 43: 	case 54: 
				case 65: 	case 76:	case 87: 	case 98: 
				case 109: 	case 120: 	case 131: 	case 142:
				case 153: 	case 164: 	case 175: 	break;
								
				default:
			
					// Move the target_page pointer to pressed position
					target_page = &Page_repository [row_of(keyNdx) + 
									(10 * column_of (keyNdx))];
					GRID_CURSOR = target_page->pageNdx;
			
					// Decide what to do depending on index
					switch( target_page->pageNdx ){
					
						//
						// Activate a GRID SET
						// 
						case 9: 	case 19:	case 29:	case 39:
						case 49:	case 59:	case 69:	case 79:
						case 89:	case 99:	case 109:	case 119:
						case 129:	case 139:	case 149:	case 159:
								
							// Calculate the new grid set
							current_GRID_set = target_page->pageNdx / 10;
	
							switch( GRID_set_switchmode ){
								
								case SEL_RECALL:
									// RECALL chosen GRID set into the current GRID selection
									import_GRID_set( current_GRID_set );
									break;
	
								case SEL_CLEAR:
									// CLEAR target GRID selection filling it with current GRID
									export_GRID_set( current_GRID_set );
									break;
							}
							break;
						
												
						// 
						// VIRTUAL TRACK SELECTORS
						//
						default:
							#include "key_GRIDTRACK_matrix.h"			
							break;
							
					} // switch( target_page->pageNdx );
					break;	
	
			} // switch( keyNdx )
		} // keyNdx in MATRIX		
			
			
		//
		// MUTATORS
		//
		if ((keyNdx >= 187) && (keyNdx <= 196)) {
	
			// Make sure there is a page playing in the bank pressed
			if ( GRID_p_selection[ keyNdx-187 ] != NULL ){
	
				// Toggle the mutepatterns in the pages active in the bank			
				if ( GRID_p_selection[ keyNdx-187 ]->trackMutepattern != 0) {
	
					GRID_p_selection[ keyNdx-187 ]->trackMutepatternStored = 
										GRID_p_selection[ keyNdx-187 ]->trackMutepattern;
					GRID_p_selection[ keyNdx-187 ]->trackMutepattern = 0;
				}
				else {						
					// Then fill the mutepattern from store
					GRID_p_selection[ keyNdx-187 ]->trackMutepattern = 
						GRID_p_selection[ keyNdx-187 ]->trackMutepatternStored;
				}			
			} // Bank is playing a page
		} // key is a mutator


		//
		// MUTE MASTER
		//
		if (keyNdx == KEY_MUTE_MASTER) {
	
			// D O U B L E - C L I C K
			if ((DOUBLE_CLICK_TARGET == keyNdx) 
					&& (DOUBLE_CLICK_TIMER > DOUBLE_CLICK_ALARM_SENSITIVITY)) {
				// This is a double click victim - Mute all tracks
				// GRID_bank_mutepattern = 0x3ff;
				// GRID_bank_mutepattern_stored = GRID_bank_mutepattern;							
			}
	
			// Step_activity is ON: turn it OFF or SELECT it
			else if (DOUBLE_CLICK_TARGET == 0) {
	
				DOUBLE_CLICK_TARGET = keyNdx;
				DOUBLE_CLICK_TIMER = ON;
				// Start the Double click Alarm
				cyg_alarm_initialize(	
						doubleClickAlarm_hdl, 
						cyg_current_time() + DOUBLE_CLICK_ALARM_TIME,
						DOUBLE_CLICK_ALARM_TIME );
	
				// SINGLE CLICK CODE
				for ( i=0; i < GRID_NROF_BANKS; i++ ){
	
					// Make sure there is a page playing in the bank pressed
					if ( GRID_p_selection[ i ] != NULL ){
			
						// Toggle the mutepatterns in the pages active in the bank			
						if ( GRID_p_selection[ i ]->trackMutepattern != 0) {
			
							GRID_p_selection[ i ]->trackMutepatternStored = 
												GRID_p_selection[ i ]->trackMutepattern;
							GRID_p_selection[ i ]->trackMutepattern = 0;
						}
						else {						
							// Then fill the mutepattern from store
							GRID_p_selection[ i ]->trackMutepattern = 
								GRID_p_selection[ i ]->trackMutepatternStored;
						}			
					}
				} // Bank iterator
			}
		} // MUTE_MASTER


		// 
		// TEMPO KEY - toggle the GRID_bank_triggermodes
		//
		if ( keyNdx == KEY_TEMPO ) {
		
			// Toggle the switch mode of the grid
			GRID_switch_mode 		^= 1;		
		}
	
	
		//
		// SOLO KEY may need to be active to unsolo a page
		//
		if ( keyNdx == KEY_SOLO ){
	
			// Active when there is a solo active in page
			// Find out whether there is something in the solo buffer
			j = FALSE;
			for ( i=0; i < GRID_NROF_BANKS; i++ ){
	
				// Check content of current buffer position
				if ( (GRID_p_selection_buffer[i] != NULL) ) {
	
					j = TRUE;
				}
			}	
	
			// Page solo condition: Un-SOLO the grid state			
			if ( ( j == TRUE ) ) {
				
				// Grid buffer has some content, transfer content and empty
				for (i=0; i < GRID_NROF_BANKS; i++) {
	
					// Propagate the copy top down.
					// Copy the buffer into the clock selection
					GRID_p_clock_presel[i] 		= GRID_p_selection_buffer[i];
	
					if ( GRID_switch_mode == GRID_SWITCH_DIRECT ){
	
						GRID_p_preselection[i] 	= GRID_p_selection_buffer[i];
						GRID_p_selection[i] 	= GRID_p_selection_buffer[i];
					}
									
					// And clear the buffer as we go along
					GRID_p_selection_buffer[i] = NULL;
				}							
			}
		} // KEY_SOLO
	
	
		//
		// MIXER MAPS
		//	
		if (	
				( (keyNdx == KEY_MIXTGT_USR1) )
			||	( (keyNdx == KEY_MIXTGT_USR2) ) 
			||	( (keyNdx == KEY_MIXTGT_USR3) )
			||	( (keyNdx == KEY_MIXTGT_USR4) )
			||	( (keyNdx == KEY_MIXTGT_USR5) ) 
			){
			
			// D O U B L E - C L I C K  C O N S T R U C T
			// DOUBLE CLICK SCENARIO
			if ((DOUBLE_CLICK_TARGET == keyNdx) 
					&& (DOUBLE_CLICK_TIMER > DOUBLE_CLICK_ALARM_SENSITIVITY)) {
		
				// Double click code
				// ...
				// Zoom into the clicked mix target
	
				// Select GRID CCMAP MODE and become PASSIVE
				GRID_play_mode 		= GRID_CCMAP;
				GRIDTRACK_OPS_mode 	= PASSIVE;
				//d_iag_printf( "keyGRID - switched to CCMAP / passive\n" );
	
				
			} // end of double click scenario 
			
		
			// SINGLE CLICK SCENARIO
			else if (DOUBLE_CLICK_TARGET == 0) {
				
					DOUBLE_CLICK_TARGET = keyNdx;
					DOUBLE_CLICK_TIMER 	= ON;
					// Start the Double click Alarm
					cyg_alarm_initialize(	
							doubleClickAlarm_hdl, 
							cyg_current_time() + DOUBLE_CLICK_ALARM_TIME,
							DOUBLE_CLICK_ALARM_TIME );
		
				// Single click code
				// ...				
				// Just simply switch to the chosen mix target.
				switch( keyNdx ){
	
					// case KEY_MIXTGT_USR0: 	GRID_assistant_page->mixTarget = MIXTGT_USR0; 	break;
					case KEY_MIXTGT_USR1: 	GRID_assistant_page->mixTarget = MIXTGT_USR1; 	break;
					case KEY_MIXTGT_USR2: 	GRID_assistant_page->mixTarget = MIXTGT_USR2; 	break;
					case KEY_MIXTGT_USR3: 	GRID_assistant_page->mixTarget = MIXTGT_USR3; 	break;
					case KEY_MIXTGT_USR4: 	GRID_assistant_page->mixTarget = MIXTGT_USR4; 	break;
					case KEY_MIXTGT_USR5: 	GRID_assistant_page->mixTarget = MIXTGT_USR5; 	break;		
				}
			} // Single click scenario
		} // MIX TARGET KEY pressed
	
	
		//
		// CLOCK SOURCE
		// 
		if ( keyNdx == KEY_CLOCK ) {
	
			// Do not listen to it when sequencer is running, DANGER!!
			if ( RUNBIT == OFF ){
	
				// D O U B L E - C L I C K  C O N S T R U C T
				// DOUBLE CLICK SCENARIO
				if ((DOUBLE_CLICK_TARGET == keyNdx) 
						&& (DOUBLE_CLICK_TIMER > DOUBLE_CLICK_ALARM_SENSITIVITY)) {
			
					// Double click code
					// ...
					// Toggle the clock source between INT and EXT
					switch ( CLOCK_SOURCE )	{
		
						case OFF:
						case INT:
							// Stop the sequencer properly
							sequencer_STOP();
							// Switch the clock source
							CLOCK_SOURCE = EXT;
							// Make sure the CLOCK LED is lit
							GLOBALBLINKER = ON;
							break;
			
						case EXT:
							// Switch the clock source
							CLOCK_SOURCE = OFF;
							break;
	
					} // switch (CLOCK_SOURCE)
					
				} // end of double click scenario 
				
			
				// SINGLE CLICK SCENARIO
				else if (DOUBLE_CLICK_TARGET == 0) {
					
					DOUBLE_CLICK_TARGET = keyNdx;
					DOUBLE_CLICK_TIMER = ON;
					// Start the Double click Alarm
					cyg_alarm_initialize(	
							doubleClickAlarm_hdl, 
							cyg_current_time() + DOUBLE_CLICK_ALARM_TIME,
							DOUBLE_CLICK_ALARM_TIME );
		
					// Single click code
					// ...
					
					// Toggle the clock source
					switch ( CLOCK_SOURCE )	{
		
						case INT:
							CLOCK_SOURCE = OFF;
							break;
					
						case OFF:
							CLOCK_SOURCE = INT;
							break;
					} // switch (CLOCK_SOURCE)
				} //end of single click scenario	
			}
		} // KEY_CLOCK
	
	
		// PLAY MODE
		if ( keyNdx == KEY_ZOOM_PLAY ){
	
			// Toggle the play mode status
			PLAY_MODE_switch( PLAY_MODE_STATUS ^ 1 );
		}


	} // INTERACTIVE MODE

	
	
	
	
	
	
	
	
	

	else if( GRIDTRACK_OPS_mode == PASSIVE ){

		//
		// TRACK MUTATORS
		//
		if ((keyNdx >= 187) && (keyNdx <= 196)) {
			
			switch (keyNdx) {
				
				case KEY_TGGL:				
					// d_iag_printf( "toggle key\n" );

					// Toggle the selected track in page mutepattern
					for ( i=0; i < GRID_NROF_BANKS; i++ ){

						// Make sure there is a page playing in the bank pressed
						if ( GRID_p_selection[i] != NULL ){
	
							// Apply track selection to mutepattern
							GRID_p_selection[i]->trackMutepattern ^= 
								GRID_p_selection[i]->trackSelection;
						} // target page is not NULL
					} // Bank iterator
					break;


				case KEY_SOLO:
					// Solo the selected tracks

					// Iterate through the banks
					for ( row=0; row < GRID_NROF_BANKS; row++ ){
					
						// Ignore NULL pointers
						if ( GRID_p_selection[ row ] != NULL ){
						
							// Operate on selected tracks							
							if ( GRID_p_selection[row]->trackSelection != 0 ){
							
								// Mark the soloed tracks in the Solopattern
								GRID_p_selection[row]->trackSolopattern ^= 
									GRID_p_selection[row]->trackSelection;

								if ( is_pressed_key( KEY_SELECT_MASTER ) ){
								
									// Void the selection
									GRID_p_selection[row]->trackSelectionStored = 
										GRID_p_selection[row]->trackSelection;									
								}		

								// Void the selection
								GRID_p_selection[row]->trackSelection = 0;
							}
						}
					}
					break;



				case KEY_ZOOM:
					
					// Make sure only one track is selected
					i = 0; // Trackcount
					j = 0;
					for ( row=0; row < GRID_NROF_BANKS; row++ ){
					
						if ( GRID_p_selection[ row ] != NULL ){
						
							if ( my_bit_cardinality( GRID_p_selection[ row ]->trackSelection ) == 1 ){
							
								i ++;
								j = row;
							}
						}
					}

					// Only continue if precisely one track was selected
					if ( i == 1 ){
					
// d_iag_printf( "track zoom in row:%d\n", row );

						// Select the VIEWER page pertaining to the track.
						if ( GRID_p_selection[ j ] != NULL ){

							target_page = GRID_p_selection[ j ];
						}
						
						// Move the cursor for the grid
						GRID_CURSOR = target_page->pageNdx;
			
						// Zoom into the target_page (implicitly) and the track there		
						target_page->keyMode = KMOD_THIS_TRACK;
						zoom_level = zoomTRACK;
					}

					break;
			}
		} // Track mutators



		//
		// OCTAVE CIRCLE
		//
		// If the key is in the Octave circle:
		if ( 	(keyNdx == KEY_NOTE_C	)
		 	|| 	(keyNdx == KEY_NOTE_Cup	)
			||	(keyNdx == KEY_NOTE_Cis	)
			||	(keyNdx == KEY_NOTE_D	)
			||	(keyNdx == KEY_NOTE_Dis	)
			||	(keyNdx == KEY_NOTE_E	)
			||	(keyNdx == KEY_NOTE_F	)
			||	(keyNdx == KEY_NOTE_Fis	)
			||	(keyNdx == KEY_NOTE_G	)
			||	(keyNdx == KEY_NOTE_Gis	)
			||	(keyNdx == KEY_NOTE_A	)
			||	(keyNdx == KEY_NOTE_Ais	)
			||	(keyNdx == KEY_NOTE_B	)
			){
	
			// Make sure only one track is selected so it can be modified
			j = 0; // Counts the track selections
			for ( i=0; i < GRID_NROF_BANKS; i++ ){
			
				if ( GRID_p_selection[i] != NULL ){
				
					if (	( GRID_p_selection[i]->trackSelection != 0 )
						&&	( my_bit_cardinality( GRID_p_selection[i]->trackSelection ) == 1 )
						){
					
						j ++;
					}
				}
			}	
		
			// Only one track is selected in the GRID
			if ( j == 1 ){
		
				// Enable track trans-positions with one click and directly, when a track is selected
				#include "key_OCT_CIRCLE_xpose_TRACK_in_GRID.h"	
			}
		}		
		
		// This should only operate when in scale mode already
		// ..or when trying to enter scale mode.
		if ( 	( GRID_assistant_page->scaleStatus != OFF ) 
			||	( keyNdx == KEY_SCALE_SEL )
			||	( keyNdx == KEY_SCALE_MOD )
			||	( keyNdx == KEY_SCALE_CAD ) 	
			){
	
			key_ScaleSelector_functions( keyNdx, GRID_assistant_page );
		}
		else if ( GRID_assistant_page->scaleStatus == OFF ){
	
			// This is the mod operation in Page mode
			key_ScaleSelector_mini_functions( keyNdx, GRID_assistant_page );
		}	



		// MATRIX
		// operate on the virtual track selectors..
		if ( (keyNdx >= 11) && (keyNdx <= 185) ){
			
			switch( keyNdx ){
				
				// These don't belong to the matrix. Only accidentally in the range, do nothing.
				case 21: 	case 32: 	case 43: 	case 54: 
				case 65: 	case 76:	case 87: 	case 98: 
				case 109: 	case 120: 	case 131: 	case 142:
				case 153: 	case 164: 	case 175: 	break;
								
				default:
															
					// 
					// VIRTUAL TRACK SELECTORS
					//
					#include "key_GRIDTRACK_matrix.h"										
					break;	
	
			} // switch( keyNdx )
		} // keyNdx in MATRIX

	} // PASSIVE MODE




	//
	// TRANSPORT functions - universally valid
	//
	switch( keyNdx ){

		case KEY_STOP:
			sequencer_command_STOP();
			break;

		case KEY_PAUSE:
			sequencer_command_PAUSE();
			break;

		case KEY_PLAY1:
		case KEY_PLAY2:
		case KEY_PLAY4:
			sequencer_command_PLAY();
			break;
	}	






	







	
	
	
	

