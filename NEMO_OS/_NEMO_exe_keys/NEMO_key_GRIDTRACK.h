//
// Copyright (C) 2004, 2005, 2006, 2007 genoQs Machines
// This file is part of Octopus_OS - 
// the firmware of the genoQs Machines Octopus, MIDI Control Sequencer
//
// The source code Octopus_OS is available for use in accordance with the 
// GNU General Public License (GPL). For more information about Open Source 
// licensing, please visit http://www.opensource.org/
//
// Octopus_OS is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation; either version 2 of the License, or
// (at your option) any later version.
//
// Octopus_OS is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with Octopus_OS; if not, write to the Free Software
// Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
//


	

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
		
			G_zoom_level = zoomGRID;
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
			if (	( DOUBLE_CLICK_TARGET == keyNdx )
				&& 	( DOUBLE_CLICK_TIMER   > DOUBLE_CLICK_ALARM_SENSITIVITY ) ) {
				// Toggle Mute across cluster
				TOGGLE_BIT( GRID_assistant_page->trackMutepattern, keyNdx - 1 );
			}
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
				GRID_bank_playmodes ^= 1 << ( keyNdx - 1 );
			}
		}
				

		// ALIGN KEY
		// Set locators of playing pages to the GLOBAL LOCATOR
		if ( keyNdx == KEY_ALIGN ){
	
			// Freeze the global locator value
			j = G_global_locator;
			k = G_TTC_abs_value;
			
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
					#include "NEMO_key_GRIDTRACK_matrix.h"			
					break;	
	
			} // switch( keyNdx )
		} // keyNdx in MATRIX		
			
			
		//
		// MUTATORS
		//
//		if ((keyNdx >= 187) && (keyNdx <= 195)) {
//	
//			// Make sure there is a page playing in the bank pressed
//			if ( GRID_p_selection[ keyNdx-187 ] != NULL ){
//	
//				// Toggle the mutepatterns in the pages active in the bank			
//				if ( GRID_p_selection[ keyNdx-187 ]->trackMutepattern != 0) {
//	
//					GRID_p_selection[ keyNdx-187 ]->trackMutepatternStored = 
//										GRID_p_selection[ keyNdx-187 ]->trackMutepattern;
//					GRID_p_selection[ keyNdx-187 ]->trackMutepattern = 0;
//				}
//				else {						
//					// Then fill the mutepattern from store
//					GRID_p_selection[ keyNdx-187 ]->trackMutepattern = 
//						GRID_p_selection[ keyNdx-187 ]->trackMutepatternStored;
//				}			
//			} // Bank is playing a page
//		} // key is a mutator

		// New School - toggle the pages altogether
		if (	(keyNdx >= 187) 
			&& 	(keyNdx <= 195)
			){

			// New school: operate on the GRID mutepattern
			GRID_mutepattern ^= ( 1 << (keyNdx-187) );

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
				bool is_unarm_page_operation = false;
				for ( i=0; i < GRID_NROF_BANKS; i++ ){
					if ( G_on_the_measure_operation[i] ) {
						unarm_page_otm_operation( GRID_p_selection[i], OPERATION_MUTE );
						unarm_page_otm_operation( GRID_p_selection[i], OPERATION_SOLO );
						is_unarm_page_operation = true;
					}
				}

				if( !is_unarm_page_operation ) {
					for ( i=0; i < GRID_NROF_BANKS; i++ ){
						// Make sure there is a page playing in the bank pressed
						if ( GRID_p_selection[i] != NULL ){
							// Toggle the mutepatterns in the pages active in the bank
							if ( GRID_p_selection[i]->trackMutepattern != 0 ) {
								GRID_p_selection[i]->trackMutepatternStored = GRID_p_selection[i]->trackMutepattern;
								apply_page_mute_pattern_operation( GRID_p_selection[i], 0x0, MASK( OPERATION_MUTE ) | MASK( OPERATION_NOSTORE ) );
							}
							else {
								// Then fill the mutepattern from store
								apply_page_mute_pattern_operation( GRID_p_selection[i], GRID_p_selection[i]->trackMutepatternStored, MASK( OPERATION_MUTE ) );
							}
						}
					} // Bank iterator
				}
			}
		} // MUTE_MASTER

	
	
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
			
//			// D O U B L E - C L I C K  C O N S T R U C T
//			// DOUBLE CLICK SCENARIO
			if ((DOUBLE_CLICK_TARGET == keyNdx)
					&& (DOUBLE_CLICK_TIMER > DOUBLE_CLICK_ALARM_SENSITIVITY)) {
//				// Double click code
//				// ...
//				// Zoom into the clicked mix target
//	
//				// Select GRID CCMAP MODE and become PASSIVE
//				GRID_play_mode 		= GRID_CCMAP;
//				GRIDTRACK_OPS_mode 	= PASSIVE;
				TOGGLE_BIT( NEMO_lauflicht_track, 4 );
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
				if( is_pressed_key( KEY_MIX_MASTER ) ) {
					switch( keyNdx ){

						// case KEY_MIXTGT_USR0: 	GRID_assistant_page->mixTarget = MIXTGT_USR0; 	break;
						case KEY_MIXTGT_USR1: 	GRID_assistant_page->mixTarget = MIXTGT_USR1; 	break;
						case KEY_MIXTGT_USR2: 	GRID_assistant_page->mixTarget = MIXTGT_USR2; 	break;
						case KEY_MIXTGT_USR3: 	GRID_assistant_page->mixTarget = MIXTGT_USR3; 	break;
						case KEY_MIXTGT_USR4: 	GRID_assistant_page->mixTarget = MIXTGT_USR4; 	break;
						case KEY_MIXTGT_USR5: 	GRID_assistant_page->mixTarget = MIXTGT_USR5; 	break;
					}
				} else {
					switch( keyNdx ){
						case KEY_MIXTGT_USR1: 	SET_APPLY_MASK( NEMO_lauflicht_track, 0xF0 ); SET_BIT( NEMO_lauflicht_track, NEMO_ROW_I ); 	break;
						case KEY_MIXTGT_USR2: 	SET_APPLY_MASK( NEMO_lauflicht_track, 0xF0 ); SET_BIT( NEMO_lauflicht_track, NEMO_ROW_II ); 	break;
						case KEY_MIXTGT_USR3: 	SET_APPLY_MASK( NEMO_lauflicht_track, 0xF0 ); SET_BIT( NEMO_lauflicht_track, NEMO_ROW_III );	break;
						case KEY_MIXTGT_USR4: 	SET_APPLY_MASK( NEMO_lauflicht_track, 0xF0 ); SET_BIT( NEMO_lauflicht_track, NEMO_ROW_IV ); 	break;
					}
				}

			} // Single click scenario

		} // MIX TARGET KEY pressed
	
	
		// 
		// CLOCK SOURCE
		// 
		if ( keyNdx == KEY_CLOCK ) {

			key_clock_select( target_page, KEY_CLOCK );
		} // KEY_CLOCK was pressed

	
		// PLAY MODE
		// Snapshot key: single click from OFF turns it on.
		// Single click from ON turns it back OFF
		// Double click from ON stores the state like it is.
		if ( keyNdx == KEY_ZOOM_PLAY ){
		
			// D O U B L E - C L I C K  C O N S T R U C T
			// DOUBLE CLICK SCENARIO
			if ((DOUBLE_CLICK_TARGET == keyNdx) 
					&& (DOUBLE_CLICK_TIMER > DOUBLE_CLICK_ALARM_SENSITIVITY)) {
		
				// Double click code
				// ...
				// Fetches the old status back.
				// Toggles the play mode status
				toggle_PLAY_MODE( OFF );
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
				// Leaves the PLAY changes permanent in the page
				switch( PLAY_MODE_STATUS ){
					case ON:
						PLAY_MODE_STATUS = OFF;
						break;
					
					case OFF:
						toggle_PLAY_MODE( ON );
						break;
				}
			}
		}


	} // INTERACTIVE MODE

	
	
	
	
	
	
	
	
	

	else if( GRIDTRACK_OPS_mode == PASSIVE ){

		//
		// TRACK MUTATORS
		//
		if ((keyNdx >= 187) && (keyNdx <= 196)) {
			
			switch (keyNdx) {
				
//				case KEY_TGGL:				
//					// d_iag_printf( "toggle key\n" );
//
//					// Toggle the selected track in page mutepattern
//					for ( i=0; i < GRID_NROF_BANKS; i++ ){
//
//						// Make sure there is a page playing in the bank pressed
//						if ( GRID_p_selection[i] != NULL ){
//	
//							// Apply track selection as mutepattern to the current page
//							// GRID_p_selection[i]->trackMutepattern ^= GRID_p_selection[i]->trackSelection;
//							apply_mutepattern( GRID_p_selection[i], GRID_p_selection[i]->trackSelection );
//
//						} // target page is not NULL
//					} // Bank iterator
//
//					break;

//
//				case KEY_SOLO:
//					// Solo the selected tracks
//
//					// Iterate through the banks
//					for ( row=0; row < GRID_NROF_BANKS; row++ ){
//					
//						// Ignore NULL pointers
//						if ( GRID_p_selection[ row ] != NULL ){
//						
//							// Operate on selected tracks							
//							if ( GRID_p_selection[row]->trackSelection != 0 ){
//							
//								// Mark the soloed tracks in the Solopattern
//								GRID_p_selection[row]->trackSolopattern ^= 
//									GRID_p_selection[row]->trackSelection;
//
//								if ( is_pressed_key( KEY_SELECT_MASTER ) ){
//								
//									// Void the selection
//									GRID_p_selection[row]->trackSelectionStored = 
//										GRID_p_selection[row]->trackSelection;									
//								}		
//
//								// Void the selection
//								GRID_p_selection[row]->trackSelection = 0;
//							}
//						}
//					}
//					break;


//
//				case KEY_ZOOM:
//					
//					// Make sure only one track is selected
//					i = 0; // Trackcount
//					j = 0;
//					for ( row=0; row < GRID_NROF_BANKS; row++ ){
//					
//						if ( GRID_p_selection[ row ] != NULL ){
//						
//							if ( my_bit_cardinality( GRID_p_selection[ row ]->trackSelection ) == 1 ){
//							
//								i ++;
//								j = row;
//							}
//						}
//					}
//
//					// Only continue if precisely one track was selected
//					if ( i == 1 ){
//					
//// d_iag_printf( "track zoom in row:%d\n", row );
//
//						// Select the VIEWER page pertaining to the track.
//						if ( GRID_p_selection[ j ] != NULL ){
//
//							target_page = GRID_p_selection[ j ];
//						}
//						
//						// Move the cursor for the grid
//						GRID_CURSOR = target_page->pageNdx;
//			
//						// Zoom into the target_page (implicitly) and the track there		
//						G_zoom_level = zoomTRACK;
//					}
//					break;
			}
		} // Track mutators

		
		
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
					#include "NEMO_key_GRIDTRACK_matrix.h"										
					break;	
	
			} // switch( keyNdx )
		} // keyNdx in MATRIX

	} // PASSIVE MODE



	// EDIT MASTER KEY
	// Toggles the editmode for gridtrack
	if ( keyNdx == KEY_EDIT_MASTER ){
	
		GRIDTRACK_editmode ^=1 ;
	}



	//
	// TRANSPORT functions - universally valid
	//
	if ( G_clock_source != EXT ){

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
	}

