
	if ( keyNdx == KEY_TEMPO ){
		G_slow_tempo = G_slow_tempo == OFF ? ON : OFF;
	}

	// Clear the record pages
	else if (keyNdx == KEY_CLEAR){
		unsigned char temp = cursor_to_dot( GRID_CURSOR );
		if ( G_solo_rec_page != NULL &&
			 selected_page_cluster( GRID_CURSOR, G_solo_rec_page->pageNdx ) &&
			 is_pressed_key( temp )
		){
			// Clear the page cluster if it is currently selected
			selected_page_cluster_clear(G_solo_rec_page->pageNdx);
			G_solo_normalize_pitch = OFF;
			G_solo_normalize_len = OFF;
			G_solo_rec_page = NULL;
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

			unsigned char temp = row_of(keyNdx) + (10 * column_of (keyNdx));
			GRID_CURSOR = temp;
		}

		// Grid page toggle
		unsigned int pressed = is_pressed_pagerange();
		unsigned int rowZeroTrack = is_key_rowzero(keyNdx);
		pressed = row_of(pressed) + (10 * column_of (pressed));
		unsigned char cursor = Page_repository[pressed].pageNdx;

		if ( selected_solo_rec_page( cursor, cursor_to_dot( cursor ) ) == ON ){

			if (keyNdx == KEY_CLEAR && G_solo_rec_page != NULL){
				// Clear the page cluster if it is currently selected
				selected_page_cluster_clear(G_solo_rec_page->pageNdx);
				G_solo_normalize_pitch = OFF;
				G_solo_normalize_len = OFF;
				G_solo_rec_page = NULL;
			}
			// A page is pressed first then step 1 through 10 of row zero to set the measure count
			else if ( Page_repository[pressed].page_clear == ON && rowZeroTrack != OFF && rowZeroTrack <= 10 ){
				Page_repository[pressed].page_clear = OFF;
				G_solo_rec_page = &Page_repository[pressed];
			}
		}

		// Fine tune quantize
		unsigned char xdx = BK_KEY_to_xdx( keyNdx );
		G_quantize_fine_tune = G_quantize_fine_tune == xdx ? 0 : xdx;
	}
