
	if ( keyNdx == KEY_TEMPO ){
		G_slow_tempo ^= 1; // toggle
	}

	if ( G_solo_rec_page != NULL ){ // A record page cluster is selected

		if ( keyNdx == KEY_PLAY1 ){
			G_track_rec_bit = OFF;
			G_run_bit = ON;
		}

		else if ( keyNdx == KEY_STOP ){
			G_track_rec_bit = OFF;
			G_run_bit = OFF;
		}

		else if ( keyNdx == KEY_RECORD ){
			G_track_rec_bit = ON;
			G_solo_has_rec = ON;
			G_run_bit = ON;
		}

		else if ( keyNdx == KEY_CHAINER && G_run_bit == OFF ){
			if ( G_solo_has_rec == ON ){
				G_solo_edit_buffer_volatile ^= 1; // toggle
				G_solo_has_rec = OFF;
				MIX_TIMER = ON;
				// Setup alarm for the MIX TIMER
				cyg_alarm_initialize(	alarm_hdl,
										cyg_current_time() + TIMEOUT_VALUE,
										0 );
			}
		}

		else if ( keyNdx == KEY_EDIT_MASTER && G_solo_edit_buffer_volatile == ON ){
			G_solo_has_rec = ON; // XXX rm - only for UI testing
			G_solo_edit_buffer_volatile ^= 1; // toggle
		}

		else if ( keyNdx == KEY_FOLLOW ){
			G_solo_overdub ^= 1; // toggle
		}
	}

	// Clear the record pages
	if (keyNdx == KEY_CLEAR){
		unsigned char temp = cursor_to_dot( GRID_CURSOR );
		if ( G_solo_rec_page != NULL &&
			 selected_page_cluster( GRID_CURSOR, G_solo_rec_page->pageNdx ) != OFF &&
			 is_pressed_key( temp )
		){
			// Clear the page cluster if it is currently selected
			selected_page_cluster_clear(G_solo_rec_page->pageNdx);
			G_solo_normalize_pitch = OFF;
			G_solo_normalize_len = OFF;
			G_solo_rec_page = NULL;
			G_solo_has_rec = OFF;
			G_solo_edit_buffer_volatile = OFF;
			Solorec_init();
		}
	}

	// Quantize chord buttons
	else if (keyNdx >= KEY_QUANTIZE_LOW && keyNdx <= KEY_QUANTIZE_HIGH){

		if ((keyNdx - KEY_QUANTIZE_LOW + 1) == G_quantize_note){
			// turn quantize off if the same button is pressed twice
			G_quantize_note = 0;
		}
		else {
			G_quantize_note = keyNdx - KEY_QUANTIZE_LOW + 1;
		}
	}
	else {

		// GRID PAGE CLUSTER SELECTIONS
		if ( ( (keyNdx < 187)
		) ) {

			unsigned char temp = row_of(keyNdx) + (10 * column_of(keyNdx));
			GRID_CURSOR = temp;
		}

		// Grid page toggle
		unsigned int pressed = is_pressed_pagerange();
		unsigned int rowZeroTrack = is_key_rowzero(keyNdx);
		unsigned int pressedCol = column_of (pressed);
		pressed = row_of(pressed) + (10 * column_of (pressed));
		unsigned char cursor = Page_repository[pressed].pageNdx;

		// Activate a record page - set the measure count
		if ( selected_solo_rec_page( cursor, cursor_to_dot( cursor ) ) == ON ||
			 selected_page_cluster( cursor, G_solo_rec_page->pageNdx ) != OFF ){
			/*
			 * A page with a measure count has been selected on this press
			 */
			// A page is pressed first then step 1 through 10 of row zero to set the measure count
			if ( rowZeroTrack != OFF && rowZeroTrack <= 10 ){
				if ( Page_repository[pressed].page_clear == ON ){
					Page_repository[pressed].page_clear = OFF;
					G_solo_rec_page = &Page_repository[pressed];
				}
				Rec_repository[pressedCol].measure_count = rowZeroTrack;
				G_solo_rec_pressed_col = pressedCol;

				ROT_INDEX = REC_MEASURES_IDX;
				// Setup alarm for the EDIT TIMER
				cyg_alarm_initialize(	alarm_hdl,
										cyg_current_time() + (TIMEOUT_VALUE / 3),
										0 );
			}
		}

		// Fine tune quantize
		unsigned char xdx = BK_KEY_to_xdx( keyNdx );
		G_quantize_fine_tune = G_quantize_fine_tune == xdx ? 0 : xdx;
	}
