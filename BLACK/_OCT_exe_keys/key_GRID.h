	
	
	
	// GENERAL STUFF - clear the GRID
	if (	( MODE_OBJECT_SELECTION == BIRDSEYE ) ){

		switch( keyNdx ){

			case KEY_CLEAR:
		
				// Empty all step, track and page data
				Octopus_memory_CLR();
				break;
		

			case KEY_PROGRAM:

				// Disregard when sequencer is running
				if ( RUNBIT == ON) break;
				
				// PROGRAM key: EXportContents(EXC) alias save memory to flash
				if (keyNdx == KEY_PROGRAM) {
					
					// Progress bar 1/4
					show_progress_bar( 1, MIR_RED );
					VIEWER_show_MIR();
					
					// Export relevant page contents to the flashbuffer
					for (i=0; i<MAX_NROF_PAGES; i++){
		
						// Copy the contents of all pages to the page buffer				
						Page_export_to_flashbuffer( &Page_repository[i], 
												    &(memory_image.Page_flashbuffer_repository[i]) 		);
		
					}
						
					// Export relevant grid contents to the flashbuffer
					Grid_export_to_flashbuffer();
		
					// Write the pagebuffer to the flash
					Flashbuffer_total_write_to_flash();
					
					// Clear the flashbuffer - leave it empty
					Page_flashbuffer_repository_init();
		
					//d_iag_printf( "->current state saved\n" );
					zoom_level = zoomGRID;
				}
				break;

		} // switch( keyNdx )
	}
	
	
	
	//	
	// GRID   M I X   MODE
	//
	if ( GRID_play_mode == GRID_MIX ){
	
		
		// Switch to GRIDTRACK mode
		if ( keyNdx == KEY_ZOOM_TRACK ){
		
			zoom_level = zoomGRIDTRACK;
			MODE_OBJECT_SELECTION = INTERACTIVE;
		}	


		// SELECTORS	
		// Toggle the playmodes for the GRID bank.
		if ((keyNdx >0) && (keyNdx <= 10)) {
	
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
		// operate on the GRID set or switch to EDIT mode..
		if ( (keyNdx >= 11) && (keyNdx <= 185) ) { 
			
			switch( keyNdx ){
				
				// These don't belong to the matrix. Only accidentally in the range.
				case 21: 	case 32: 	case 43: 	case 54: 
				case 65: 	case 76:	case 87: 	case 98: 
				case 109: 	case 120: 	case 131: 	case 142:
				case 153: 	case 164: 	case 175:
					// Do nothing
					break;
			
						
				default:
			
					// Calculate the pageNdx, to make orientation easier 
					VIEWER_page = &Page_repository [row_of(keyNdx) + 
									(10 * column_of (keyNdx))];
			
					// Decide what to do
					switch( VIEWER_page->pageNdx ){
					
						// GRID_set selectors. Select the SET
						case 9: 	case 19:	case 29:	case 39:
						case 49:	case 59:	case 69:	case 79:
						case 89:	case 99:	case 109:	case 119:
						case 129:	case 139:	case 149:	case 159:
							
							// Calculate the new grid set
							current_GRID_set = VIEWER_page->pageNdx / 10;
	
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
						
												
						// Page selectors, act as always..
						// ..select the page for play in preselection or zoom into it (doubleclick)
						default:
						
							#include "key_GRID_matrix.h"
				
							break;
							
						} // switch( VIEWER_page->pageNdx );
	
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
	
	
	
		// MUTE MASTER
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
		}
		
	
		//TEMPO KEY - toggle the GRID_bank_triggermodes
		if ( keyNdx == KEY_TEMPO ) {
	
			// Toggle the switch mode of the grid
			GRID_switch_mode 		^= 1;		
		}
	
	

		//
		// OCTAVE CIRCLE
		//
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
			
	
	
		// SOLO KEY may need to be active to unsolo a page
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
	
						GRID_p_preselection[i] 		= GRID_p_selection_buffer[i];
						GRID_p_selection[i] 		= GRID_p_selection_buffer[i];
					}
									
					// And clear the buffer as we go along
					GRID_p_selection_buffer[i] = NULL;
				}							
			}
		}
	
	
//		// 
//		// MIX TARGET
//		//
//		// Toggle the Map0 status
//		if ( (keyNdx == KEY_MIXTGT_USR0) ){
//			GRID_MAP0_status ^= 1;
//		}
	
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
				GRID_play_mode 	= GRID_CCMAP;
				GRID_OPS_mode 	= PASSIVE;
				//d_iag_printf( "keyGRID - switched to CCMAP / passive\n" );
	
				
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
		}
	
		// PLAY MODE
		if ( keyNdx == KEY_ZOOM_PLAY ){
	
			// Toggle the play mode status
			PLAY_MODE_switch( PLAY_MODE_STATUS ^ 1 );
		}
	
	
	
	} // GRID_play_mode == GRID_MIX
	
	
	
		
		
		
		
	
	
	
	//	
	// GRID   C C M A P   MODE
	//
	else if ( GRID_play_mode == GRID_CCMAP ){
	
		// MIX TARGETS
		if (	( 	// ( (keyNdx == KEY_MIXTGT_USR0) && (GRID_assistant_page->mixTarget == MIXTGT_USR0) )
					( (keyNdx == KEY_MIXTGT_USR1) && (GRID_assistant_page->mixTarget == MIXTGT_USR1) )
				||	( (keyNdx == KEY_MIXTGT_USR2) && (GRID_assistant_page->mixTarget == MIXTGT_USR2) ) 
				||	( (keyNdx == KEY_MIXTGT_USR3) && (GRID_assistant_page->mixTarget == MIXTGT_USR3) )
				||	( (keyNdx == KEY_MIXTGT_USR4) && (GRID_assistant_page->mixTarget == MIXTGT_USR4) )
				||	( (keyNdx == KEY_MIXTGT_USR5) && (GRID_assistant_page->mixTarget == MIXTGT_USR5) ) )
			){	
				
			GRID_play_mode 								= GRID_MIX;
			GRID_OPS_mode 								= INTERACTIVE;
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
	
	
		// MATRIX KEYS pressed
		if ( (keyNdx > 10) && (keyNdx < 186) && (((keyNdx-10) % 11) != 0) ) { 
	
			// Compute the coordinates of the pressed key
			row = row_of( keyNdx );
			col = column_of( keyNdx );
	
			// Find the target value to be modified
			switch( GRID_assistant_page->CC_MIXMAP_attribute ){
	
				case CC_MIXMAP_AMT:
				case CC_MIXMAP_MCC:
				
					if ( GRID_assistant_page->CC_MIXMAP_attribute == CC_MIXMAP_AMT ){
						target_ptr = 
							&GRID_assistant_page->CC_MIXMAP[GRID_assistant_page->mixTarget][row][CC_MIXMAP_AMT];
					}
					else if ( GRID_assistant_page->CC_MIXMAP_attribute == CC_MIXMAP_MCC ){
						target_ptr = 
							&GRID_assistant_page->CC_MIXMAP[GRID_assistant_page->mixTarget][row][CC_MIXMAP_MCC];
					}
	
					// D O U B L E - C L I C K
					if ((DOUBLE_CLICK_TARGET == keyNdx) 
							&& (DOUBLE_CLICK_TIMER > DOUBLE_CLICK_ALARM_SENSITIVITY)) {
		
						// Double click code:
						//
						// Modify the tens value of the AMOUNT
		
						// Break up original value in tens and ones.
						i = *target_ptr / 10;
						j = *target_ptr % 10;				
						
						if ( col < 12 ){
		
							// Compute new tens
							i = 10 * (col + 1);
		
							// Get new ones value, leave it as it is otherwise
							if ( col < 9 ){
								// j = col + 1;
								// Actually remove it, so we can easily reach zero of ones
								j = 0;
							}
		
							// Write the final value into the attribute
							*target_ptr = normalize( i + j, 0, 127 );							
						}
						
						else{
		
							// Write the final value into the attribute
							*target_ptr = 0;
						}
					} // second click of double click
		
					else if (DOUBLE_CLICK_TARGET == 0) {
		
						DOUBLE_CLICK_TARGET = keyNdx;
						DOUBLE_CLICK_TIMER = ON;								
						// Start the Double click Alarm
						cyg_alarm_initialize(	
							doubleClickAlarm_hdl, 
							cyg_current_time() + DOUBLE_CLICK_ALARM_TIME,
							DOUBLE_CLICK_ALARM_TIME );
		
						// Single click code:
						//
						if ( col < 9 ){
							// Modify the ones value of the MIDICC. 128 is a special value.
							if ( *target_ptr > 127 ){
								i = 0;
							}
							else {
								i = *target_ptr / 10;
							}
							*target_ptr = normalize( i*10 + col + 1, 0, 127 );		
						}
					} // first click of double click
	
					// close case CC_MIXMAP_AMT and CC_MIXMAP_MCC
					break;
	
	
				case CC_MIXMAP_MCH:
				
					target_ptr = 
						&GRID_assistant_page->CC_MIXMAP[GRID_assistant_page->mixTarget][row][CC_MIXMAP_MCH];
	
					// D O U B L E - C L I C K
					if ((DOUBLE_CLICK_TARGET == keyNdx) 
							&& (DOUBLE_CLICK_TIMER > DOUBLE_CLICK_ALARM_SENSITIVITY)) {
		
						// Double click code:
						///
						// Switch the MIDI port
						// Find out the current port: it will be 0 or 1;
						// Note that the ATTR_MIDICH is in the range [1..32].
						i = *target_ptr / 17;
						j = *target_ptr - (16 * i);
		
						// Swith the port indicator
						i ^= 1;
		
						// Write the new value to the attribute
						*target_ptr = (16 * i) + j;
					}
		
					else if (DOUBLE_CLICK_TARGET == 0) {
		
						DOUBLE_CLICK_TARGET = keyNdx;
						DOUBLE_CLICK_TIMER = ON;								
						// Start the Double click Alarm
						cyg_alarm_initialize(	
							doubleClickAlarm_hdl, 
							cyg_current_time() + DOUBLE_CLICK_ALARM_TIME,
							DOUBLE_CLICK_ALARM_TIME );
		
						// Single click code:
						// Find out the current port: it will be 0 or 1;
						// Note that the ATTR_MIDICH is in the range [1..32].
						i = *target_ptr / 17;
					
						// Set the new value of the MIDICH attribute - on the same port.
						*target_ptr = (16 * i) + col + 1;
					}
	
					break;
			}
		} // if( MATRIX key pressed )
	
		else {
			// Just simply switch to the chosen mix target.
			switch( keyNdx ){
				// case KEY_MIXTGT_USR0: 	GRID_assistant_page->mixTarget = MIXTGT_USR0; 	break;
				case KEY_MIXTGT_USR1: 	GRID_assistant_page->mixTarget = MIXTGT_USR1; 	break;
				case KEY_MIXTGT_USR2: 	GRID_assistant_page->mixTarget = MIXTGT_USR2; 	break;
				case KEY_MIXTGT_USR3: 	GRID_assistant_page->mixTarget = MIXTGT_USR3; 	break;
				case KEY_MIXTGT_USR4: 	GRID_assistant_page->mixTarget = MIXTGT_USR4; 	break;
				case KEY_MIXTGT_USR5: 	GRID_assistant_page->mixTarget = MIXTGT_USR5; 	break;		
	
				case KEY_RETURN:
					// Select GRID CCMAP MODE and become PASSIVE
					GRID_play_mode 								= GRID_MIX;
					GRID_OPS_mode 								= INTERACTIVE;
					GRID_assistant_page->CC_MIXMAP_attribute 	= CC_MIXMAP_AMT;
					break;
			} // switch( keyNdx )
		} // else
		
		
		// PLAY MODE
		if ( keyNdx == KEY_ZOOM_PLAY ){
	
			// Toggle the play mode status
			PLAY_MODE_switch( PLAY_MODE_STATUS ^ 1 );
		}
	}
	
	
	
		
		
		
		
		
		
		
		
		
		
		
		
		
		
			
	//
	// GRID  E D I T   MODE
	//
	else if ( GRID_play_mode == GRID_EDIT ){
	
		//
		// SELECTORS
		//
		// Select attribute to be modified by MIXER. 
		// ..for now actually only ATTR_VELOCITY makes a lot of sense. But who knows..		
		if ((keyNdx >0) && (keyNdx <= 10)) {
	
			// Select the GRID mixAttribute. 
			// ..right now only PAGE velocity works, so do not allow switching around.
			//GRID_mixAttribute = 1 << (keyNdx-1);
		}
	
		
		if (	( keyNdx == KEY_SCALE_SEL )
		){
		
			// Toggle the status
			Page_repository[GRID_CURSOR].grid_FTS_exempt ^= 1;
		}


		//
		// MATRIX
		//
		// Select a page in the GRID
		if ( (keyNdx > 10) && (keyNdx < 186) ) { 
	
			switch (keyNdx) {
	
				// These don't belong to matrix
				case 21: 	case 32: 	case 43: 	case 54: 
				case 65: 	case 76:	case 87: 	case 98: 
				case 109: 	case 120: 	case 131: 	case 142:
				case 153: 	case 164: 	case 175:
				
					// So do nothing
					break;
	
				// And these are the REPEATS selectors
				case 20:	case 31:	case 42:	case 53:
				case 64:	case 75:	case 86:	case 97:
				case 108:	case 119:	case 130:	case 141:
				case 152:	case 163:	case 174:	case 185:
	
					switch( GRID_rowzero_pagelength ){

						case FALSE:
							// Set the "repeats" value
							Page_repository[GRID_CURSOR].repeats = (keyNdx - 9) / 11;
							Page_repository[GRID_CURSOR].repeats_left = 
												Page_repository[GRID_CURSOR].repeats;
							break;

						case TRUE:

							// D O U B L E - C L I C K  C O N S T R U C T
							// DOUBLE CLICK SCENARIO
							if ((DOUBLE_CLICK_TARGET == keyNdx) 
									&& (DOUBLE_CLICK_TIMER > DOUBLE_CLICK_ALARM_SENSITIVITY)) {
						
								// Double click code
								// ...
								// Set the "length" large value
								if ( Page_repository[GRID_CURSOR].attribute[ATTR_LENGTH] == 17 ){
									Page_repository[GRID_CURSOR].attribute[ATTR_LENGTH] = 0;
								}
								else{
									Page_repository[GRID_CURSOR].attribute[ATTR_LENGTH] = 
										normalize( ((keyNdx - 9) / 11 ) * 16, 0, 128 );
								}
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
								// Set the "length" small value
								Page_repository[GRID_CURSOR].attribute[ATTR_LENGTH] = 
									normalize( 
										(Page_repository[GRID_CURSOR].attribute[ATTR_LENGTH]/16)*16 
										+ ((keyNdx - 9) / 11 ), 
										0, 128 );
							}
							break;
					} // switch on GRID_rowzero_pagelength
					
					break;
	
	
				default:
	
					// D O U B L E - C L I C K  C O N S T R U C T
					// DOUBLE CLICK SCENARIO
					if ((DOUBLE_CLICK_TARGET == keyNdx) 
							&& (DOUBLE_CLICK_TIMER > DOUBLE_CLICK_ALARM_SENSITIVITY)) {
				
						// Double click code
						// ...
						// Zoom into the clicked page
						zoom_level = zoomPAGE;
						VIEWER_page->keyMode = KMOD_PAGE;					
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
						
						// Only move the cursor around but make no changes
						VIEWER_page = 	&Page_repository [row_of(keyNdx) + (10 * column_of (keyNdx))];
		
						// Update the GRID CURSOR
						GRID_CURSOR = 	VIEWER_page->pageNdx;
	
					}
	
			} // switch keyNdx..
	
		} // if key in range..				
	
	
		//
		// OCTAVE CIRCLE
		//
		# include "key_OCT_CIRCLE_xpose_PAGE.h"
		
	
		//
		// TRACK MUTATORS
		//
		if ((keyNdx >= 187) && (keyNdx <= 196)) {
			
			switch (keyNdx) {
				
				case TGGL_KEY:
					// Toggle the page playing status 
					
					// Classic toggle behavior
					switch ( is_selected_in_GRID( VIEWER_page ) ) {
						case ON:
							grid_select( VIEWER_page, OFF );
							break;
	
						case OFF:
							grid_select( VIEWER_page, ON  );
							break;
					}
					break;
	
	
				case SOLO_KEY:
	
					// All details were outsourced.. so they can be used elsewhere
					PAGE_toggle_solo();
	
					break;
	
	
				case CLEAR_KEY:
					// Select all tracks in cursor page
					Page_repository[GRID_CURSOR].trackSelection = 0x3FF;
	
					// Clear selected tracks in page - only applies to step status
					Page_CLEAR_selected_tracks( &Page_repository[GRID_CURSOR] );
					
					
					// Re-assign the track initial pitches to the tracks in the page
					for ( i=0; i < MATRIX_NROF_ROWS; i++ ){
	
						(&Page_repository[GRID_CURSOR])->Track[row]->attribute[ATTR_PITCH]
							= TRACK_DEFAULT_PITCH[i];
					}
	
					// Unselect all tracks in cursor page
					Page_repository[GRID_CURSOR].trackSelection = OFF;
	
					// Mark page as cleared
					Page_repository[GRID_CURSOR].page_clear = ON;
				
					break;
					
	
	
				case RANDOMIZE_KEY:
					
					// Select all tracks in cursor page
					Page_repository[GRID_CURSOR].trackSelection = 0x3FF;
	
					// Randomize selected tracks in page - step status
					Page_RNDMZ_selected_tracks( &Page_repository[GRID_CURSOR] );
					
					// Unselect all tracks in cursor page
					Page_repository[GRID_CURSOR].trackSelection = OFF;
					
					// Mark page as touched
					Page_repository[GRID_CURSOR].page_clear = OFF;
					break;
	
	
	
//				case FLT_KEY:
//					// Only active pages can be flipped. Break on inactive page
//					if (is_selected_in_GRID( &Page_repository[GRID_CURSOR] ) == OFF){
//						break;
//					}
//	
//					// Select all tracks in cursor page
//					Page_repository[GRID_CURSOR].trackSelection = 0x3FF;
//	
//					// Randomize selected tracks in page - step status
//					Page_FLT_selected_tracks( &Page_repository[GRID_CURSOR] );
//					
//					// Unselect all tracks in cursor page
//					Page_repository[GRID_CURSOR].trackSelection = OFF;
//					
//					// Mark page as touched
//					Page_repository[GRID_CURSOR].page_clear = OFF;
//					break;
	
	
	
				case RMX_KEY:
					// Only active pages can be rmx'ed. Break on inactive page
					if (is_selected_in_GRID( &Page_repository[GRID_CURSOR] ) == OFF){
						break;
					}
	
					// Select all tracks in cursor page
					Page_repository[GRID_CURSOR].trackSelection = 0x3FF;
	
					// Randomize selected tracks in page - step status
					Page_RMX_selected_tracks( &Page_repository[GRID_CURSOR] );
					
					// Unselect all tracks in cursor page
					Page_repository[GRID_CURSOR].trackSelection = OFF;
					
					// Mark page as touched
					Page_repository[GRID_CURSOR].page_clear = OFF;
					break;
	
	
	
				case ZOOM_KEY:
					// Zoom into the currently selected page
					VIEWER_page = &Page_repository[GRID_CURSOR];
					zoom_level = zoomPAGE;
					VIEWER_page->keyMode = KMOD_PAGE;
					break;
	
	
				case COPY_KEY:
					// Remember the index of the copied page
					PAGE_COPY_BUFFER = GRID_CURSOR;
					break;
											
	
				case PASTE_KEY:
					// Make sure a page was copied first
					if ( PAGE_COPY_BUFFER != GRID_SELECTION_EMPTY ){
	
						// Paste the copied page into page under the cursor
						Page_copy( 	&Page_repository[PAGE_COPY_BUFFER], 
									&Page_repository[GRID_CURSOR] 			);
					}				
					// Clear the page copy buffer
					PAGE_COPY_BUFFER = GRID_SELECTION_EMPTY;
					break; 
	
	
			} // switch (keyNdx)
		} // Track mutators
	
		
		// 
		// BIG KNOB BUTTONS
		//
		if (keyNdx == KEY_SELECT) {
			GRID_status = GRID_SELECT;
		}
		
		if (keyNdx == KEY_TEMPO) {
			GRID_status = GRID_DEFAULT;
		}
		
	
		// PAGE Zoom key not available in PERFORM mode as we would not 
		// know which page we are about to enter!!
		if (keyNdx == KEY_ZOOM_PAGE) {
			
			zoom_level = zoomPAGE;
			VIEWER_page->keyMode = KMOD_PAGE;
		}
	
		// Selector of rowzero
		if ( keyNdx == 10 ){
			GRID_rowzero_pagelength ^= 1;
		}

	} // GRID_play_mode == GRID_EDIT
		
	
	
	
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
	
	
