
	if ( keyNdx <= 10 ){
		return; // disable track select buttons
	}

	if ( keyNdx == KEY_TEMPO ){

		if ( SOLO_slow_tempo != OFF ){ // slow tempo
			G_master_tempo = SOLO_slow_tempo;
			SOLO_slow_tempo = OFF;
		}
		else { // reset to original tempo
			SOLO_slow_tempo = G_master_tempo;
			G_master_tempo = 100;
		}
	}

	if ( keyNdx == KEY_PLAY2 ){ // ON = continue playing after the loop after recording
		SOLO_rec_continue_recording ^= 1; // toggle
	}

	if ( keyNdx == KEY_PLAY4 ){ // values = OFF, ON, 1.2.3.
		if ( ++SOLO_rec_ending_flash == QBIT ){
			SOLO_rec_ending_flash = OFF;
		}
	}

	if ( keyNdx == KEY_QUANTIZE_FIRST ){
		SOLO_rec_quantize_first_beat ^= 1;
	}

	// MIDI NOTE and CC routing and pass through enabled
	if ( keyNdx == KEY_ZOOM_MAP ){
		G_midi_map_controller_mode ^= 1;
	}

	if ( keyNdx == KEY_SELECT_MASTER ){
		if ( SOLO_rec_has_MCC == FALSE ){
			SOLO_rec_track_preview ^= 1; // toggle grid and page view because we don't have MCC data

		} else if ( ++SOLO_rec_track_preview > SOLOMCC ){
			SOLO_rec_track_preview = SOLOGRID;
		}
	}

	if ( keyNdx == KEY_STOP ){
		if ( G_run_bit == ON && SOLO_rec_page != NULL ){
			stop_solo_rec( SOLO_rec_freeflow_trim && SOLO_has_rec == ON );
		}
		else {
			send_ALL_NOTES_OFF();
			if ( is_pressed_key( KEY_MUTE_MASTER )){
				send_note_off_full_range();
			}
		}
	}

	if ( SOLO_rec_page != NULL ){ // A record page cluster is selected

		if ( keyNdx == KEY_PLAY1 ){
			if ( SOLO_has_rec == ON && G_run_bit == OFF ){
				G_track_rec_bit = OFF;
				reset_page_cluster( SOLO_rec_page );
				sequencer_command_PLAY();
			}
			if ( SOLO_rec_measure_hold == ON && G_run_bit == ON ){
				SOLO_rec_rehersal ^= 1;
			}
		}

		else if ( keyNdx == KEY_RECORD ){ // Record
			G_track_rec_bit ^= 1; // toggle
			if ( SOLO_has_rec == OFF ){
				SOLO_rec_measure_hold = ON;
			}
			reset_page_cluster( SOLO_rec_page );
			sequencer_command_PLAY();
		}

		else if ( keyNdx == KEY_CHAINER && G_run_bit == OFF ){ // Clear recording
			if ( SOLO_has_rec == ON ){
				SOLO_edit_buffer_volatile ^= 1; // toggle
				SOLO_has_rec = OFF;
				freeflowOff(FALSE);
				// Clear the pages
				clear_page_record_track_chain(SOLO_rec_page);
				MIX_TIMER = ON;
				// Setup alarm for the MIX TIMER
				cyg_alarm_initialize(	alarm_hdl,
										cyg_current_time() + TIMEOUT_VALUE,
										0 );
			}
		}

		else if ( keyNdx == KEY_EDIT_MASTER && SOLO_edit_buffer_volatile == ON ){
			SOLO_has_rec = ON; // XXX rm - only for UI testing
			SOLO_edit_buffer_volatile ^= 1; // toggle
		}

		else if ( keyNdx == KEY_FOLLOW ){
			SOLO_overdub ^= 1; // toggle
		}

		// Legato
		if ( keyNdx == KEY_ZOOM_STEP && G_run_bit == OFF &&  SOLO_has_rec == ON ){
			SOLO_rec_legato ^= 1; // toggle
		}
	}

	if ( keyNdx < 187 && G_run_bit == ON && SOLO_rec_track_preview == SOLOPAGE ){

		// Compute Step coordinates
		unsigned char row = row_of( keyNdx );
		unsigned char col = column_of( keyNdx );
		Pagestruct* target_page = &Page_repository[GRID_CURSOR];

		// Turns the step selection off
		interpret_matrix_stepkey( row, col, target_page );
	}

	// Clear the record pages
	if (keyNdx == KEY_CLEAR && G_run_bit == OFF){
		unsigned int pressed = is_pressed_pagerange();
		if ( SOLO_rec_page != NULL &&
			 pressed != FALSE &&
			 selected_page_cluster( grid_ndx_from_key(pressed), SOLO_rec_page->pageNdx ) != NOP
		){

			selected_page_cluster_clear(SOLO_rec_page->pageNdx);
			SOLO_normalize_pitch 		= OFF;
			SOLO_normalize_len 			= OFF;
			SOLO_rec_page 				= NULL;
			SOLO_has_rec 				= OFF;
			SOLO_edit_buffer_volatile 	= OFF;
			SOLO_rec_freeflow 			= OFF;
			SOLO_rec_measure_count 		= OFF;
			SOLO_rec_freeflow_measures	= OFF;
			SOLO_rec_measure_pos		= OFF;
			SOLO_pos_marker_in 			= OFF;
			SOLO_pos_marker_out 		= OFF;
			SOLO_pos_in 				= NULL;
			SOLO_pos_out 				= NULL;
			SOLO_rec_has_MCC			= OFF;
			G_measure_locator 			= OFF;
			Solorec_init();
		}
	}

	else if (keyNdx == KEY_FLT){

		if ( SOLO_has_rec == ON && G_run_bit == ON ){

			if ( SOLO_pos_marker_in == OFF ){
				SOLO_pos_marker_in = SOLO_rec_measure_pos;
				SOLO_pos_in = &Page_repository[GRID_CURSOR];
			}
			else {
				stop_solo_rec(FALSE);
			}

			ROT_INDEX = REC_MEASURES_SPLIT;
			// Setup alarm for the EDIT TIMER
			cyg_alarm_initialize(	alarm_hdl,
									cyg_current_time() + (TIMEOUT_VALUE),
									0 );
		}
	}

	// Quantize chord buttons
	else if (keyNdx >= KEY_QUANTIZE_LOW && keyNdx <= KEY_QUANTIZE_HIGH){

		if ((keyNdx - KEY_QUANTIZE_LOW) != SOLO_quantize_note){

			SOLO_quantize_note = keyNdx - KEY_QUANTIZE_LOW;
			quantize(SOLO_rec_page); // Apply the quantize
		}
	}
	else {

		// GRID PAGE CLUSTER SELECTIONS

		// The key that is held
		unsigned char heldNdx = grid_ndx_from_key(keyNdx);

		// Grid page toggle
		unsigned char keyRow = row_of(keyNdx);
		unsigned int pressed = is_pressed_pagerange();
		unsigned char isPressed = pressed != FALSE;
		unsigned char pressedNdx = grid_ndx_from_key(pressed); // the gridNdx that is pressed
		unsigned int rowZeroTrack = is_key_rowzero(keyNdx);

		if (isPressed) {
			heldNdx = pressedNdx;
			pressedNdx = grid_ndx_from_key(keyNdx);
		}

		unsigned char heldCol = grid_col(heldNdx);

//		diag_printf("key:%d\n", keyNdx);
//		diag_printf("heldNdx:%d\n", heldNdx);
//		diag_printf("is_pressed:%d\n", isPressed);
//		diag_printf("pressedNdx:%d\n", pressedNdx);
//		diag_printf("keyRow:%d\n", keyRow);
//		diag_printf("hCol:%d\n", heldCol);
//		diag_printf("heldCol:%d\n", grid_col(heldNdx));
//		diag_printf("heldRow:%d\n", grid_row(heldNdx));
//		diag_printf("heldX:%d\n", grid_ndx(grid_row(heldNdx), grid_col(heldNdx)));
//		diag_printf("solorec:%d\n", selected_solo_rec_page( heldNdx, pressedNdx ));
//		diag_printf("gridc:%d\n", GRID_CURSOR);
//		diag_printf("zerom:%d\n", rowZeroTrack);
//
//		diag_printf("1:%d\n", Page_repository[heldNdx].page_clear == ON);
//		diag_printf("2:%d\n", grid_col(heldNdx) == 0);
//		diag_printf("3:%d\n", Page_repository[grid_ndx_prev_col(heldNdx)].page_clear == ON);
//		diag_printf("4:%d\n", SOLO_rec_page == NULL);

		// Activate a record page - set the measure count
		if (
			 ( selected_solo_rec_page( heldNdx, pressedNdx ) == ON ||
			   selected_page_cluster( heldNdx, SOLO_rec_page->pageNdx ) != NOP )){

			/*
			 * A page with a measure count has been selected on this press
			 */
			if ( !SOLO_has_rec ){ // does not have a recording yet

				if ( has_empty_grid_row_ahead(heldNdx) == TRUE && keyNdx == KEY_CHAINMODE_4 &&
					 SOLO_rec_page == NULL && Page_repository[heldNdx].page_clear == ON ){

					// Free Flow!
					SOLO_rec_page = &Page_repository[heldNdx];
					GRID_CURSOR = SOLO_rec_page->pageNdx;
					SOLO_rec_freeflow = ON;
					Rec_repository[heldCol].measure_count = MATRIX_NROF_ROWS;
					create_next_freeflow_page_cluster(heldNdx);
					SOLO_rec_freeflow_measures = count_to_last_page_in_grid_row(heldNdx) * MATRIX_NROF_ROWS;

					reset_page_cluster( SOLO_rec_page );
				}
				// A page is pressed first then step 1 through 10 of row zero to set the measure count
				else if ( rowZeroTrack != OFF && rowZeroTrack <= 10 && SOLO_rec_freeflow == OFF ){

					if ( Page_repository[heldNdx].page_clear == ON ){
						/*
						 * ########################################
						 * A record page was created!
						 * ########################################
						 */
						SOLO_rec_page = &Page_repository[heldNdx];

						if ( SOLO_has_rec == OFF ){
							GRID_CURSOR = SOLO_rec_page->pageNdx;
						}
					}

					SOLO_rec_pressed_col = heldCol;
					Rec_repository[heldCol].measure_count = rowZeroTrack;
					create_page_record_track_chain(SOLO_rec_page, rowZeroTrack);
					reset_page_cluster( SOLO_rec_page );

					// Snow the measure count for a few extra blinks
					ROT_INDEX = REC_MEASURES_IDX;
					// Setup alarm for the EDIT TIMER
					cyg_alarm_initialize(	alarm_hdl,
											cyg_current_time() + (TIMEOUT_VALUE / 3),
											0 );
				}
			}
			else { // Has a recording

				if ( keyNdx == KEY_ZOOM_PAGE ){
					// Enter the page edit warp tunnel
					GRID_CURSOR = heldNdx;
					G_zoom_level = zoomPAGE;
				}
			}
		}

		// Toggle the play along status of existing pages in the grid
		else if ( SOLO_rec_page != NULL && Page_repository[heldNdx].page_clear == OFF ){

			// no play along pages set for this row
			// and - this row does not contain the solo recording page
			if ((( SOLO_page_play_along[keyRow] == NOP && grid_row(SOLO_rec_page->pageNdx) != keyRow ))
				|| // or
					(( grid_row(SOLO_page_play_along[keyRow]) == keyRow ) && // a page in a play along row was pressed
					( SOLO_page_play_along[keyRow] != heldNdx ) // and - this page was not already the play along page
				)){
				SOLO_page_play_along[keyRow] = heldNdx;
			}
			else {
				SOLO_page_play_along[keyRow] = NOP;
			}
		}

		if (SOLO_has_rec == ON && G_run_bit == OFF){
			// Fine tune quantize
			unsigned char xdx = BK_KEY_to_xdx( keyNdx );
			SOLO_quantize_fine_tune = SOLO_quantize_fine_tune == xdx ? 0 : xdx;
		}
	}
