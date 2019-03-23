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


	#ifdef FEATURE_ENABLE_SONG_UPE
	if (keyNdx == KEY_MUTE_MASTER){

		if ( G_rec_ctrl_page != NULL ){
			unarm_ctrl();
			return;
		}

		G_rec_ctrl_track = NULL;
		G_rec_ctrl_page = NULL;

		unsigned int pressed = is_pressed_pagerange();
		pressed = row_of(pressed) + (10 * column_of (pressed));

		if ( Page_repository[pressed].page_clear == OFF ){
			quick_assign_control_track(&Page_repository[pressed], ctrl_event_assign_target_page(&Page_repository[pressed]));
		}
	}
	#endif


	// GRID PAGE CLUSTER SELECTIONS
	if ( ( (keyNdx > 10 && keyNdx < 187)
	) ) {


		temp = row_of(keyNdx) + (10 * column_of (keyNdx));
		GRID_CURSOR = temp;

//			diag_printf("prev: %d\n", PREV_GRID_CURSOR);
//			diag_printf("grid: %d\n", GRID_CURSOR);
//			diag_printf("ndx: %d\n", keyNdx);
//			diag_printf("temp: %d\n", temp);
//			diag_printf("press: %d\n", is_pressed_key(keyNdx - 11));
//			diag_printf("gress: %d\n", is_pressed_key(GRID_CURSOR));

		if ( GRID_p_selection_cluster == ON ) {

			if (! ( (keyNdx >= 187)
					&& 	(keyNdx <= 195)
			) ) {

				if ( selected_page_cluster_pressed( GRID_CURSOR, PREV_GRID_CURSOR ) ) {

					if ( CHECK_BIT(page_cluster_op, PGM_CLST_CLR) ) {
						selected_page_cluster_clear( GRID_CURSOR );
					}

				} else if ( CHECK_BIT(page_cluster_op, PGM_CLST_CPY) ) {

					selected_page_cluster_copy( GRID_CURSOR, PREV_GRID_CURSOR );

				} else if ( !CHECK_BIT(page_cluster_op, PGM_CLST_CLR) ){

					selected_page_cluster_move( GRID_CURSOR, PREV_GRID_CURSOR );
				}
			}
		}

		if ( PREV_GRID_CURSOR + 10 == GRID_CURSOR && is_pressed_key(keyNdx - 11) ) {

			#ifdef FEATURE_ENABLE_SONG_UPE
			// --------------------------------------------------------------------------------
			// Control Track Reference Page Selection
			//
			if ( MIX_TRACK != NULL && CHECK_BIT(MIX_TRACK->attr_MISC, TRK_CTRL_MIX) && Track_get_MISC(MIX_TRACK, CONTROL_BIT) ){
				MIX_TRACK->program_change = PREV_GRID_CURSOR / 10 + 1;
				MIX_TRACK->attr_MCH = 9 - (PREV_GRID_CURSOR % 10);
			}
			// --------------------------------------------------------------------------------
			#endif

			previous_page = &Page_repository[ PREV_GRID_CURSOR ];

			if ( PREV_GRID_CURSOR >= 10 ) { // there is a prev_prev in the row

				prev_previous_page = &Page_repository[ PREV_GRID_CURSOR - 10 ];
				prev_previous_page_clear = prev_previous_page->page_clear;
			} else {

				prev_previous_page_clear = ON;
			}
			temp_page = &Page_repository[ GRID_CURSOR ];

			if (( previous_page->page_clear == OFF && temp_page->page_clear == OFF && prev_previous_page_clear == ON ) ||
			   // single page
			   (( grid_col(PREV_GRID_CURSOR) == 0 && Page_repository[ GRID_CURSOR + 10 ].page_clear == ON ) || // left edge
			    ( grid_col(GRID_CURSOR) == 15 && previous_page->page_clear == ON ) || // right edge
				( Page_repository[ GRID_CURSOR ].page_clear == ON && previous_page->page_clear == OFF && prev_previous_page_clear == ON ) || // right side select
				( Page_repository[ GRID_CURSOR + 10 ].page_clear == ON && Page_repository[ GRID_CURSOR ].page_clear == OFF && previous_page->page_clear == ON ) ||
				( Page_repository[ GRID_CURSOR ].page_clear == OFF && previous_page->page_clear == ON && Page_repository[ GRID_CURSOR + 10 ].page_clear == ON ))){

				GRID_p_selection_cluster = ON;

				// right side select, assuming an adjacent cluster may exist
				if (//( grid_col(PREV_GRID_CURSOR) == 0 && Page_repository[ GRID_CURSOR + 10 ].page_clear == ON ) ||
					( Page_repository[ GRID_CURSOR ].page_clear == ON && previous_page->page_clear == OFF && prev_previous_page_clear == ON )){

					GRID_CURSOR = PREV_GRID_CURSOR; // move the cursor left to the single un-cleared page
					PREV_GRID_CURSOR = temp;
				}
				// single page left side select
				else if (! (previous_page->page_clear == OFF && temp_page->page_clear == OFF && prev_previous_page_clear == ON) ){

					PREV_GRID_CURSOR = GRID_CURSOR;
				}

			} else {

				GRID_p_selection_cluster = OFF;
				CLEAR_BIT(page_cluster_op, PGM_CLST_CLR);
				CLEAR_BIT(page_cluster_op, PGM_CLST_CPY);
			}

		} else if ( (keyNdx >= 11) && (keyNdx <= 185) ) {
			if (! ( (keyNdx >= 187)
					&& 	(keyNdx <= 195)
			) ) {

				GRID_p_selection_cluster = OFF;
				CLEAR_BIT(page_cluster_op, PGM_CLST_CLR);
				CLEAR_BIT(page_cluster_op, PGM_CLST_CPY);
			}

		}
		PREV_GRID_CURSOR = GRID_CURSOR;
	}
	// -- END GRID PAGE CLUSTER SELECTIONS





	// GENERAL STUFF - clear the GRID
	if (	( MODE_OBJECT_SELECTION == BIRDSEYE ) ){

		switch( keyNdx ){

			case KEY_CLEAR:

				// Empty all step, track and page data
				Octopus_memory_CLR();
				break;


			// Save full grid to flash
			case KEY_PROGRAM:

				// Disregard when sequencer is running
				if ( G_run_bit == ON ) break;

				// PROGRAM key: save memory to flash
				if (keyNdx == KEY_PROGRAM) {

					show_progress_bar( 1, MIR_RED );
					VIEWER_show_MIR();

					Flash_write_grid();
					Flash_write_all_pages();

					//d_iag_printf( "->current state saved\n" );
					G_zoom_level = zoomGRID;
				}
				break;


				// Dump page contents via sysex - EXC is export contents
				case KEY_ALIGN:
					if ( 	( G_run_bit == OFF )
						){

						// Enter zoomSYSEX - mode where to do sysex dumps
						G_zoom_level = zoomSYSEX;
					}
					break;

		} // switch( keyNdx )
	}









	//
	// GRID   M I X   MODE
	//
	if ( GRID_play_mode == GRID_MIX ){

//		d_iag_printf("   4 key_GRID- GRID_play_mode:%d GRID_editmode:%d\n",
//				GRID_play_mode, GRID_editmode );



		// Switch to GRIDTRACK mode
		if ( keyNdx == KEY_ZOOM_TRACK ){

			G_zoom_level = zoomGRIDTRACK;
			MODE_OBJECT_SELECTION = INTERACTIVE;
		}

//		// Switch to ABLETON mode
//		if ( keyNdx == KEY_ZOOM_MAP ){
//
//			// G_zoom_level = zoomABLETON;
//			// MODE_OBJECT_SELECTION = INTERACTIVE;
//		}

		// MIDI NOTE and CC routing and pass through enabled
		if ( keyNdx == KEY_ZOOM_MAP ){
			G_midi_map_controller_mode ^= 1;
		}


		// SELECTORS
		// Toggle the playmodes for the GRID bank.
		if ((keyNdx >0) && (keyNdx <= 10)) {

			GRID_bank_playmodes ^= 1 << (keyNdx-1);
		}


		// ALIGN KEY
		// Set locators of playing pages to the GLOBAL LOCATOR
		if ( keyNdx == KEY_ALIGN ){
			#ifdef FEATURE_ENABLE_SONG_UPE
			if ( G_pause_bit == OFF ){
				// Freeze the global locator value
				j = G_global_locator;
				k = G_TTC_abs_value;

				for ( i=0; i< GRID_NROF_BANKS; i++ ){

					// If the page is currently playing
					if ( GRID_p_selection[i] != NULL ){

						set_page_locators( 	GRID_p_selection[i], j, k );
					}
				}
			} else { // PMLS

				G_measure_locator = (G_measure_indicator_value * 5) + G_measure_indicator_part;
				align_measure_locators();
				sequencer_command_PAUSE(ON);
			}			
			#else
			// Freeze the global locator value
			j = G_global_locator;
			k = G_TTC_abs_value;

			for ( i=0; i< GRID_NROF_BANKS; i++ ){

				// If the page is currently playing
				if ( GRID_p_selection[i] != NULL ){

					set_page_locators( 	GRID_p_selection[i], j, k );
				}
			}
			#endif
		}


		// RECORD KEY
		// Toggle the REC_bit for the GRID assistant page. Disable Track- and Page record modes.

		if ( keyNdx == KEY_RECORD ){
			G_track_rec_bit = OFF;
			Page_repository[ GRID_CURSOR ].REC_bit = OFF;
			GRID_assistant_page->REC_bit ^= ON;
		}


		// PROGRAM KEY - debugging scaffold
		if ( keyNdx == KEY_PROGRAM ){

			// D O U B L E - C L I C K  C O N S T R U C T
			// DOUBLE CLICK SCENARIO
			if (	( DOUBLE_CLICK_TARGET == keyNdx )
				&& 	( DOUBLE_CLICK_TIMER   > DOUBLE_CLICK_ALARM_SENSITIVITY ) ) {

				// Double click code
				// ...
				Octopus_recall_flash();

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
				// nothing to do
			}
		}
		#ifdef FEATURE_ENABLE_SONG_UPE
		if ( keyNdx == KEY_MIX_MASTER ){
			CLEAR_BIT(MIX_TRACK->attr_MISC, TRK_CTRL_MOVE);
			CLEAR_BIT(MIX_TRACK->attr_MISC, TRK_CTRL_MIX);
			CLEAR_BIT(MIX_TRACK->attr_MISC, TRK_CTRL_CLEAR);
			CLEAR_BIT(MIX_TRACK->attr_MISC, TRK_CTRL_MUT_PGTGL);
			CLEAR_BIT(MIX_TRACK->attr_CCAMT, TRK_CTRL_TEMPO);
			SET_BIT(MIX_TRACK->attr_MISC, TRK_CTRL_PGTGL);
			MIX_TRACK = NULL;
		}
		#endif

		#ifdef FEATURE_SOLO_REC
		// Grid Scene change note event send on-the-measure
		if ( keyNdx == 10 ){ // grid selection row 0 button

			if ( GRID_p_set_note_offsets[current_GRID_set] == 255 )
			{
				GRID_p_set_note_offsets[current_GRID_set] = 0;
			}
			else
			{
				GRID_p_set_note_offsets[current_GRID_set] = 255; // turn note send off
			}
		}
		#endif

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

					// Compute the grid coordinates from the key index
					temp = row_of(keyNdx) + (10 * column_of (keyNdx));

					// Decide what to do according to coordinates of pressed key
					switch( temp ){

						// GRID_set selectors. Select the SET
						case 9: 	case 19:	case 29:	case 39:
						case 49:	case 59:	case 69:	case 79:
						case 89:	case 99:	case 109:	case 119:
						case 129:	case 139:	case 149:	case 159:

							// Calculate the new grid set
							current_GRID_set = temp / 10;

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

							// D O U B L E - C L I C K  C O N S T R U C T
							// DOUBLE CLICK SCENARIO
							if (	( DOUBLE_CLICK_TARGET == keyNdx )
								&& 	( DOUBLE_CLICK_TIMER   > DOUBLE_CLICK_ALARM_SENSITIVITY ) ) {

								// Double click code
								// ...

								// d_iag_printf("   6 key_GRID- GRID_play_mode:%d GRID_editmode:%d\n",
								//		GRID_play_mode, GRID_editmode );

								// Restore the page playing status from before
								// Toggle the page playing status --> classic toggle behavior
								// Note that this is effectively reversing the queue status
								if ( GRID_editmode != ON ){
									switch ( is_selected_in_GRID( target_page ) ) {
										case ON:
											grid_select( target_page, ON );
											break;

										case OFF:
											grid_select( target_page, OFF  );
											break;
									}
//								}

								// d_iag_printf( "8 previous_page:%d tgt:%d\n", previous_page->pageNdx, target_page->pageNdx );


								if ( previous_page != NULL ){
									// Reactivate the previous page for play in grid
									grid_select( previous_page, ON );
								}
								else{
				// d_iag_printf( "10 previous_page NULL\n" );

									GRID_p_selection[ target_page->pageNdx % 10 ] = NULL;
									GRID_p_preselection[ target_page->pageNdx % 10 ] = NULL;
									GRID_p_clock_presel[ target_page->pageNdx % 10 ] = NULL;
								}
			}

								// Disable the program change send queue
								G_pgmch_pending_page = NULL;

// d_iag_printf( "2 previous_page:%d tgt:%d\n", previous_page->pageNdx, target_page->pageNdx );

								// Zoom into the target_page (implicitly)
								GRID_CURSOR	= temp;

								target_page = &Page_repository[ GRID_CURSOR ];
								G_zoom_level = zoomPAGE;

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

								// Need this here so we can grab pages in GRID
								GRID_CURSOR = temp;
								target_page = &Page_repository[ GRID_CURSOR ];

								// Account for the edit mode in GRID
								if ( GRID_editmode == ON ){

									// Move cursor or zoom into page
									if ( is_pressed_key( KEY_ZOOM_PAGE )){

										// Compute the coordinates of the cursor first.
										GRID_CURSOR	= temp;

										temp_page = &Page_repository[ GRID_CURSOR ];

										G_zoom_level = zoomPAGE;
									}
								} // GRID editormode is active

								else{
									// Remember the page currently selected in the bank
									previous_page = GRID_p_clock_presel[ target_page->pageNdx % 10 ];

									// Toggle the page playing status
									// Classic toggle behavior
									switch ( is_selected_in_GRID( target_page ) ) {
										case ON:
											grid_select( target_page, OFF );
											#ifdef FEATURE_ENABLE_SONG_UPE
											ctrl_event_page_toggle( target_page );
											#endif
											break;

										case OFF:
											grid_select( target_page, ON  );
											#ifdef FEATURE_ENABLE_SONG_UPE
											ctrl_event_page_toggle( target_page );
											#endif
											// Klaus Gstettner request: send PgmCh when queuing page
											// i.e. the o'clock switchmode (queue mechanism)..
											// ..and the page needs to have a chromatic scale selected
											if (	( GRID_switch_mode == GRID_SWITCH_OCLOCK )
												&&	( target_page->force_to_scale == TRUE )
												&&	( target_page->scaleNotes[ G_scale_ndx ] == SCALE_SIG_CHR )
												){

												// Queue up the page for sending pgmch
												G_pgmch_pending_page = target_page;
											}
											break;
									}
								} // GRID editormode is not active
							} // Single click scenario

							break;

						} // switch( target_page->pageNdx );

			} // switch( keyNdx )

		} // keyNdx in MATRIX


		//
		// MUTATORS
		//
		if (	(keyNdx >= 187)
			&& 	(keyNdx <= 195)
			){
			// page cluster selection -- disable everything else
			if ( GRID_p_selection_cluster == ON ) {

				switch (keyNdx) {
					case KEY_CLEAR:
						if ( CHECK_BIT(page_cluster_op, PGM_CLST_CLR) ) {
							CLEAR_BIT(page_cluster_op, PGM_CLST_CLR);
						} else {
							SET_BIT(page_cluster_op, PGM_CLST_CLR);
							CLEAR_BIT(page_cluster_op, PGM_CLST_CPY);
						}
					break;

					case KEY_COPY:
						if ( CHECK_BIT(page_cluster_op, PGM_CLST_CPY) ) {
							CLEAR_BIT(page_cluster_op, PGM_CLST_CPY);
						} else {
							SET_BIT(page_cluster_op, PGM_CLST_CPY);
							CLEAR_BIT(page_cluster_op, PGM_CLST_CLR);
						}
					break;
				}

				break; // disable all other MUT keys
			}

// Old-school - toggling the page mutepatterns
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

			// New school: operate on the GRID mutepattern
			GRID_mutepattern ^= ( 1 << (keyNdx-187) );

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


		//G_master_tempo KEY - toggle the GRID_bank_triggermodes
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


		//
		// MIX TARGET
		//
		if (
				( (keyNdx == KEY_MIXTGT_USR0) )
			||	( (keyNdx == KEY_MIXTGT_USR1) )
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

					case KEY_MIXTGT_USR0: 	GRID_assistant_page->mixTarget = MIXTGT_USR0; 	break;
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
			#ifdef FEATURE_ENABLE_SONG_UPE
			if ( G_pause_bit == OFF ){
				key_clock_select( target_page, KEY_CLOCK );
			}
			#else
			key_clock_select( target_page, KEY_CLOCK );
			#endif
		} // KEY_CLOCK was pressed


		// PLAY MODE
		if ( keyNdx == KEY_ZOOM_PLAY ){

			// Toggles the play mode
			toggle_PLAY_MODE( PLAY_MODE_STATUS^1 );
		}



	} // GRID_play_mode == GRID_MIX










	//
	// GRID   C C M A P   MODE
	//
	else if ( GRID_play_mode == GRID_CCMAP ){

		// MIX TARGETS
		if (	( 	( (keyNdx == KEY_MIXTGT_USR0) && (GRID_assistant_page->mixTarget == MIXTGT_USR0) )
				||	( (keyNdx == KEY_MIXTGT_USR1) && (GRID_assistant_page->mixTarget == MIXTGT_USR1) )
				||	( (keyNdx == KEY_MIXTGT_USR2) && (GRID_assistant_page->mixTarget == MIXTGT_USR2) )
				||	( (keyNdx == KEY_MIXTGT_USR3) && (GRID_assistant_page->mixTarget == MIXTGT_USR3) )
				||	( (keyNdx == KEY_MIXTGT_USR4) && (GRID_assistant_page->mixTarget == MIXTGT_USR4) )
				||	( (keyNdx == KEY_MIXTGT_USR5) && (GRID_assistant_page->mixTarget == MIXTGT_USR5) ) )
			){

			GRID_play_mode 								= GRID_MIX;
			GRID_OPS_mode 								= INTERACTIVE;
			GRID_assistant_page->CC_MIXMAP_attribute 	= CC_MIXMAP_AMT;
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
						// Disregarding the virtual MIDI channels
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
				case KEY_MIXTGT_USR0: 	GRID_assistant_page->mixTarget = MIXTGT_USR0; 	break;
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

			// Toggles the play mode
			toggle_PLAY_MODE( PLAY_MODE_STATUS^1 );
		}

	} // GRID_play_mode is CC MAP















	//
	// GRID  E D I T   MODE
	//
	else if ( GRID_play_mode == GRID_EDIT ){

		// d_iag_printf("   2 key_GRID- GRID_play_mode:%d GRID_editmode:%d\n",
		//		GRID_play_mode, GRID_editmode );


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
					// ..so do nothing
					break;

				// And these are the REPEATS selectors
				case 20:	case 31:	case 42:	case 53:
				case 64:	case 75:	case 86:	case 97:
				case 108:	case 119:	case 130:	case 141:
				case 152:	case 163:	case 174:	case 185:

					switch( GRID_rowzero_pagelength ){

						case FALSE:
							#ifdef FEATURE_ENABLE_SONG_UPE
							// Repeats statement removed
							#else
							// D O U B L E - C L I C K  C O N S T R U C T
							// DOUBLE CLICK SCENARIO
							if ((DOUBLE_CLICK_TARGET == keyNdx)
									&& (DOUBLE_CLICK_TIMER > DOUBLE_CLICK_ALARM_SENSITIVITY)) {

								// Double click code
								// ...
								// Set repeat forever
								target_page->attr_STA = 0;
								target_page->repeats_left = 1;
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
								// Set the "repeats" value
								target_page->attr_STA = (keyNdx - 9) / 11;
								target_page->repeats_left = target_page->attr_STA;
							}
							#endif
							break;

						case TRUE:

							// D O U B L E - C L I C K  C O N S T R U C T
							// DOUBLE CLICK SCENARIO
							if ((DOUBLE_CLICK_TARGET == keyNdx)
									&& (DOUBLE_CLICK_TIMER > DOUBLE_CLICK_ALARM_SENSITIVITY)) {

								// Double click code
								// ...
								// Set the "length" large value
								if ( target_page->attr_LEN == 17 ){

									target_page->attr_LEN = 1;
								}

								else{

									target_page->attr_LEN =
										normalize( ((keyNdx - 9) / 11 ) * 16,
														PAGE_MIN_LEN, PAGE_MAX_LEN );
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
								target_page->attr_LEN =
									normalize(
										(target_page->attr_LEN/16)*16
										+ ((keyNdx - 9) / 11 ),
										PAGE_MIN_LEN, PAGE_MAX_LEN );
							}
							break;
					} // switch on GRID_rowzero_pagelength

					break;


				// These are the actual pages in the grid
				default:

					// D O U B L E - C L I C K  C O N S T R U C T
					// DOUBLE CLICK SCENARIO
					if ((DOUBLE_CLICK_TARGET == keyNdx)
							&& (DOUBLE_CLICK_TIMER > DOUBLE_CLICK_ALARM_SENSITIVITY)) {

						// Double click code
						// ...
						// Zoom into the clicked page
						GRID_CURSOR = row_of(keyNdx) + (10 * column_of (keyNdx));

						target_page = 	&Page_repository[ GRID_CURSOR ];

						G_zoom_level = zoomPAGE;

	// d_iag_printf( "3 cursor:%d\n", GRID_CURSOR );
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

						// Update the GRID CURSOR
						// GRID_CURSOR = row_of(keyNdx) + (10 * column_of (keyNdx));
						// target_page = 	&Page_repository[ GRID_CURSOR ];
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

				case KEY_TGGL:
					// Toggle the page playing status

					// Classic toggle behavior
					switch ( is_selected_in_GRID( target_page ) ) {
						case ON:
							grid_select( target_page, OFF );
							#ifdef FEATURE_ENABLE_SONG_UPE
							ctrl_event_page_toggle( target_page );
							#endif
							break;

						case OFF:
							grid_select( target_page, ON  );
							target_page->page_clear = OFF;
							#ifdef FEATURE_ENABLE_SONG_UPE
							ctrl_event_page_toggle( target_page );
							#endif
							// Klaus Gstettner request: send PgmCh when queuing page
							// i.e. the o'clock switchmode (queue mechanism)..
							// ..and the page needs to have a chromatic scale selected
							if (	( GRID_switch_mode == GRID_SWITCH_OCLOCK )
								&&	( target_page->force_to_scale == TRUE )
								&&	( target_page->scaleNotes[ G_scale_ndx ] == SCALE_SIG_CHR )
								){
								send_program_changes( target_page );
							}
							break;
					}
					break;


				case KEY_SOLO:

					// All details were outsourced.. so they can be used elsewhere
					PAGE_toggle_solo();

					break;


				case KEY_CLEAR:
					// Select all tracks in cursor page
					target_page->trackSelection = 0x3FF;

					// Clear selected tracks in page - only applies to step status
					Page_CLEAR_selected_tracks( target_page );

					for ( i=0; i < MATRIX_NROF_ROWS; i++ ){

						// Re-assign the track initial pitches to the tracks in the page
						target_page->Track[row]->attr_PIT = TRACK_DEFAULT_PITCH[i];

						// Reset track directions to normal
						target_page->Track[row]->attr_DIR = TRACK_DEF_DIRECTION;
					}

					// Unselect all tracks in cursor page
					target_page->trackSelection 		= OFF;
					target_page->trackMutepattern		= OFF;
					target_page->trackMutepatternStored = OFF;
					target_page->trackSolopattern		= OFF;

					// Remove page from grid selections
					GRID_p_selection[ target_page->pageNdx % 10 ] = NULL;
					GRID_p_preselection[ target_page->pageNdx % 10 ] = NULL;
					GRID_p_clock_presel[ target_page->pageNdx % 10 ] = NULL;

					// Mark page as cleared
					target_page->page_clear = ON;

					// Initialize the page Track chain data
					for ( row=0; row < MATRIX_NROF_ROWS; row++ ){

						// Init the chain data for 10 unchained rows: each track is by itself
						target_page->Track[row]->chain_data[HEAD] = target_page->Track[row];
						target_page->Track[row]->chain_data[NEXT] = target_page->Track[row];
						target_page->Track[row]->chain_data[PREV] = target_page->Track[row];
						target_page->Track[row]->chain_data[PLAY] = target_page->Track[row];
					}
					#ifdef FEATURE_ENABLE_SONG_UPE
					PAGE_init(target_page, target_page->pageNdx, false);
					#endif
					break;



				case KEY_RANDOMIZE:

					// Select all tracks in cursor page
					target_page->trackSelection = 0x3FF;

					// Randomize selected tracks in page - step status
					Page_RNDMZ_selected_tracks( target_page );

					// Unselect all tracks in cursor page
					target_page->trackSelection = OFF;

					// Mark page as touched
					target_page->page_clear = OFF;
					break;



				case KEY_RMX:
					// Only active pages can be rmx'ed. Break on inactive page
					if (is_selected_in_GRID( target_page ) == OFF){
						break;
					}

					// Select all tracks in cursor page
					target_page->trackSelection = 0x3FF;

					// Randomize selected tracks in page - step status
					Page_RMX_selected_tracks( target_page );

					// Unselect all tracks in cursor page
					target_page->trackSelection = OFF;

					// Mark page as touched
					target_page->page_clear = OFF;
					break;



				case KEY_ZOOM:
					// Zoom into the currently selected page
					target_page = &Page_repository[GRID_CURSOR];

					G_zoom_level = zoomPAGE;
// d_iag_printf( "4 cursor:%d\n", GRID_CURSOR );

					break;


				case KEY_COPY:
					// Remember the index of the copied page
					PAGE_COPY_BUFFER = GRID_CURSOR;
					break;


				case KEY_PASTE:
					// Make sure a page was copied first
					if ( PAGE_COPY_BUFFER != GRID_SELECTION_EMPTY ){

						// Paste the copied page into page under the cursor
						Page_copy( 	&Page_repository[PAGE_COPY_BUFFER],
									&Page_repository[GRID_CURSOR] 			);
					}

#ifdef	COPY_BUFFER_FRESH
					// Clear the page copy buffer
					PAGE_COPY_BUFFER = GRID_SELECTION_EMPTY;
#endif
					break;


			} // switch (keyNdx)
		} // Track mutators


		//
		// BIG KNOB BUTTONS
		//
		if ( keyNdx == KEY_SCALE_MYSEL ) {
			GRID_status = GRID_SELECT;
		}

		if (keyNdx == KEY_TEMPO) {
			GRID_status = GRID_DEFAULT;
		}


		// PAGE Zoom key not available in PERFORM mode as we would not
		// know which page we are about to enter!!
		if (keyNdx == KEY_ZOOM_PAGE) {
			G_zoom_level = zoomPAGE;
		}

		// Selector of rowzero
		if ( keyNdx == 10 ){
			GRID_rowzero_pagelength ^= 1;
		}

		// Save current page.
		if ( keyNdx == KEY_PROGRAM ) {
			if ( G_run_bit == OFF ){
				Flash_write_page( GRID_CURSOR );
			}
			return;
		}

	} // GRID_play_mode == GRID_EDIT




	// EDIT MASTER KEY
	// Toggles the editmode for GRID
	#ifdef FEATURE_ENABLE_SONG_UPE
	if ( keyNdx == KEY_EDIT_MASTER && G_rec_ctrl_page == NULL){
	#else
	if ( keyNdx == KEY_EDIT_MASTER ){
	#endif
		GRID_editmode ^=1 ;
	}




	//
	// TRANSPORT functions - universally valid
	//
	if ( G_clock_source != EXT ){

		switch( keyNdx ){

			#ifdef FEATURE_ENABLE_SONG_UPE

			case KEY_PAUSE:
				if ( G_pause_bit == OFF ){
					sequencer_command_PAUSE(ON);
				} else if ( prev_G_stop_bit == OFF ){
					sequencer_command_PAUSE(OFF);
				} else {
					G_pause_bit = OFF;
				}
				break;
			#else
			case KEY_STOP:
				G_stop_bit = ON;
				unsigned char pause = G_pause_bit;
				sequencer_command_STOP();
				G_pause_bit = pause;
				break;

			case KEY_PAUSE:
				#ifdef FEATURE_SOLO_REC
				// the sequencer is not paused and it is not running, i.e. stopped
				if ( G_pause_bit == OFF && G_run_bit == OFF ){
					SOLO_rec_save_playmodes = GRID_bank_playmodes;
					if ( GRID_p_selection_cluster == ON ){
						if ( has_valid_record_cluster_format( &Page_repository[GRID_CURSOR] ) == ON ){
							pageClusterEnterSoloRec( GRID_CURSOR );
							enterSoloRec();
						}
					}
					else {
						enterSoloRec();
					}
				}
				#endif
				sequencer_command_PAUSE();
				break;

			#endif
			case KEY_PLAY1:
			case KEY_PLAY2:
			case KEY_PLAY4:
				sequencer_command_PLAY();
				break;
		}
	}
	else
	{
		switch( keyNdx ){
			case KEY_STOP:
				G_stop_bit = ON;
				unsigned char pause = G_pause_bit;
				sequencer_command_STOP();
				G_pause_bit = pause;
				break;

			#ifdef FEATURE_SOLO_REC
			case KEY_PAUSE:
				// the sequencer is not paused and it is not running, i.e. stopped
				if ( G_pause_bit == OFF && G_run_bit == OFF ){
					SOLO_rec_save_playmodes = GRID_bank_playmodes;
					if ( GRID_p_selection_cluster == ON ){
						if ( has_valid_record_cluster_format( &Page_repository[GRID_CURSOR] ) == ON ){
							pageClusterEnterSoloRec( GRID_CURSOR );
							enterSoloRec();
						}
					}
					else {
						enterSoloRec();
					}
				}
				break;
			#endif
		}
	}

	#ifdef FEATURE_ENABLE_SONG_UPE
	//
	// Pause Measure Locator Scrolling (PMLS) - enabled
	//
	if ( G_pause_bit == ON ){

		// Some 1/5 nibble button is pressed (track chain preset buttons)
		if ( keyNdx >= 202 && keyNdx <= 205
		){
			G_measure_indicator_part = 206 - keyNdx;
		}

		// Some chord button is pressed
		if ( keyNdx >= 252 && keyNdx <= 258
		){
			G_repeats_interval_idx = keyNdx - 252;
//			G_repeats_delay = OFF;
			G_repeats_delay = ON;

			// D O U B L E - C L I C K
			if ((DOUBLE_CLICK_TARGET == keyNdx)
					&& (DOUBLE_CLICK_TIMER > DOUBLE_CLICK_ALARM_SENSITIVITY)) {

				// set the delay flag
//				G_repeats_delay = ON;
			}
			// SINGLE CLICK
			else if (DOUBLE_CLICK_TARGET == 0) {

				DOUBLE_CLICK_TARGET = keyNdx;
				DOUBLE_CLICK_TIMER = ON;
				// Start the Double click Alarm
				cyg_alarm_initialize(
						doubleClickAlarm_hdl,
						cyg_current_time() + DOUBLE_CLICK_ALARM_TIME,
						DOUBLE_CLICK_ALARM_TIME );

			}
		}

		if ( keyNdx == KEY_CHAINER // Jog Backwards
		){
			if ( G_measure_indicator_part == 0 )
			{
				if ( G_measure_indicator_value > 0 ){
					G_measure_indicator_value--;
					G_measure_indicator_part = 4;
				}
				else {
					return;
				}
			}
			else {
				G_measure_indicator_part--;
			}
		}

		if ( keyNdx == KEY_FOLLOW // Jog Forwards
		){
			if ( G_measure_indicator_part +1 == 5 )
			{
				G_measure_indicator_value++;
				G_measure_indicator_part = 0;
			}
			else {
				G_measure_indicator_part++;
			}
		}

	}
#endif



//	d_iag_printf(" key_GRID- GRID_play_mode:%d GRID_editmode:%d\n",
//			GRID_play_mode, GRID_editmode );


