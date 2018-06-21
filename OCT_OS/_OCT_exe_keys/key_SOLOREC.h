
	if ( keyNdx == KEY_TEMPO ){
		G_slow_tempo = G_slow_tempo == OFF ? ON : OFF;
	}

	// Quantize chord buttons
	else if (keyNdx >= LED_QUANTIZE_LOW && keyNdx <= LED_QUANTIZE_HIGH){

		if ((keyNdx - LED_QUANTIZE_LOW + 1) == G_quantize_note){
			// turn quantize off if the same button is pressed twice
			G_quantize_note = 0;
		}
		else {
			G_quantize_note = keyNdx - LED_QUANTIZE_LOW + 1;
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
		pressed = row_of(pressed) + (10 * column_of (pressed));
		if ( Page_repository[pressed].page_clear == ON ){
			Page_repository[pressed].page_clear = OFF;
		}
//		is_pressed_rowzero()

		// Fine tune quantize
		unsigned char xdx = BK_KEY_to_xdx( keyNdx );
		G_quantize_fine_tune = G_quantize_fine_tune == xdx ? 0 : xdx;
	}
