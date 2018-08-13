
	// TRANSPORT CONTROLS
	if ( SOLO_rec_page != NULL ){

		// Record button
		MIR_write_dot( LED_RECORD, MIR_RED );
		if ( G_run_bit == ON && G_track_rec_bit == ON ){
			if ( SOLO_rec_rehersal == OFF ){
				MIR_write_dot( LED_RECORD, MIR_GREEN );
			}
			MIR_write_dot( LED_RECORD, MIR_BLINK );
		}
	}

	if ( SOLO_rec_continue_recording == ON ){
		MIR_write_dot( LED_PLAY2, MIR_GREEN );
		MIR_write_dot( LED_PLAY2, MIR_RED );
	}
	else {
		MIR_write_dot( LED_PLAY2, MIR_RED );
	}

	// MIDI NOTE and CC routing and pass through enabled
	if ( G_midi_map_controller_mode == ON ){
		MIR_write_dot (LED_ZOOM_MAP, MIR_RED);
		MIR_write_dot( LED_ZOOM_MAP, MIR_BLINK );
	}

	// Show the end of recording chord flash
	if ( SOLO_rec_ending_flash == OFF ){
		MIR_write_dot( LED_PLAY4, MIR_RED );
	}
	else if ( SOLO_rec_ending_flash == ON ){
		MIR_write_dot( LED_PLAY4, MIR_GREEN );
		MIR_write_dot( LED_PLAY4, MIR_RED );
	}
	else {
		MIR_write_dot( LED_PLAY4, MIR_GREEN );
	}

	if ( G_run_bit == ON ){
		MIR_write_dot( LED_STOP, MIR_RED );
	}

	if ( SOLO_has_rec == ON ){
		if ( G_track_rec_bit == OFF ){
			if ( G_run_bit == ON ){
				MIR_write_dot( LED_PLAY1, MIR_GREEN );
			}
			else {
				MIR_write_dot( LED_PLAY1, MIR_RED );
			}
		}
		if ( SOLO_overdub == OFF ){
			MIR_write_dot( LED_FOLLOW, MIR_RED );
		}
		else {
			MIR_write_dot( LED_FOLLOW, MIR_GREEN );
		}

		// Clear recording
		if ( G_run_bit == OFF ){
			MIR_write_dot( LED_CHAINER, MIR_RED );
		}
	}
	else { // No recording yet

		// Rehersal
		if ( SOLO_rec_rehersal == OFF && SOLO_rec_measure_hold == ON ){
			MIR_write_dot( LED_PLAY1, MIR_RED );
		}
		else if ( SOLO_rec_rehersal == ON ){
			MIR_write_dot( LED_PLAY1, MIR_GREEN );
			MIR_write_dot( LED_PLAY1, MIR_RED );
			MIR_write_dot( LED_PLAY1, MIR_BLINK );
		}
	}

	// Free Flow recording
	if ( SOLO_rec_freeflow == ON ){
		MIR_write_dot( LED_CHAINMODE_4, MIR_RED   );
		MIR_write_dot( LED_CHAINMODE_4, MIR_GREEN );
	}

	// Undo Edit buffer
	if ( SOLO_edit_buffer_volatile == ON ){
		MIR_write_dot( LED_EDIT_MASTER, MIR_RED );
		MIR_write_dot( LED_EDIT_MASTER, MIR_GREEN );
		if ( MIX_TIMER == ON ){
			MIR_write_dot( LED_EDIT_MASTER, MIR_BLINK );
		}
	}

	// ESC
	if ( G_run_bit == OFF ){
		MIR_write_dot( LED_RETURN, MIR_GREEN );
	}

	// Slow tempo toggle
	if ( SOLO_slow_tempo != OFF ){
		MIR_write_dot( LED_TEMPO, MIR_GREEN );
		MIR_write_dot( LED_TEMPO, MIR_BLINK );
	}
	else
	{
		MIR_write_dot( LED_TEMPO, MIR_RED );
	}

	if (SOLO_rec_page != NULL && G_run_bit == OFF){

		MIR_write_dot( LED_CLEAR, MIR_RED ); // LEN / clr
		MIR_write_dot( LED_CLEAR, MIR_GREEN );

		if ( SOLO_has_rec == ON ){

			// Grid controls
			MIR_write_dot( LED_TGGL, MIR_RED ); // VEL
			MIR_write_dot( LED_TGGL, MIR_GREEN );

			MIR_write_dot( LED_ZOOM_STEP, MIR_RED ); // STEP ZOOM for Legato note length toggle
			if ( SOLO_rec_legato == ON ){
				MIR_write_dot( LED_ZOOM_STEP, MIR_GREEN );
			}
		}
	}

	// POS recording split button
	if (SOLO_rec_page != NULL && SOLO_has_rec == ON ){
		MIR_write_dot( LED_FLT, MIR_RED ); // POS
		// The recording is playing and not recording so enable split markers using POS
		if ( G_run_bit == ON && G_track_rec_bit == OFF ){
			MIR_write_dot( LED_FLT, MIR_GREEN ); // POS
			if ( ROT_INDEX == REC_MEASURES_SPLIT ){
				MIR_write_dot( LED_FLT, MIR_BLINK ); // POS
			}
		}
	}

	// Show the MODE - (Special) Grid Solo Zoom
	MIR_write_dot( LED_ZOOM_GRID, MIR_RED );
	MIR_write_dot( LED_ZOOM_GRID, MIR_GREEN );
	MIR_write_dot( LED_ZOOM_GRID, MIR_BLINK );

	MIR_write_dot( LED_PASTE, MIR_RED ); // MCH
	MIR_write_dot( LED_PASTE, MIR_GREEN );


	// Flash the end of recording indicator using the CHORD LEDs
	if ( G_run_bit == ON && SOLO_rec_ending_flash > OFF ){

		if ( SOLO_rec_ending_flash == ON ){ // Flash the last measure

			if ( SOLO_rec_measure_pos == SOLO_rec_measure_count && SOLO_rec_measure_count > 1 ){

				for (i=LED_QUANTIZE_LOW; i <= LED_QUANTIZE_HIGH; i++) {
					MIR_write_dot( i, MIR_RED );
					MIR_write_dot( i, MIR_GREEN );
					MIR_write_dot( i, MIR_BLINK );
				}
			}
		}
		else if ( SOLO_rec_ending_flash > ON ){ // 3 - 2 - 1 ... Flash the measure count down

			if ( SOLO_rec_measure_pos >= SOLO_rec_measure_count - 3 && SOLO_rec_measure_count > 3 ){

				for (i=LED_QUANTIZE_LOW; i <= LED_QUANTIZE_HIGH; i++) {
					if ( SOLO_rec_measure_pos == SOLO_rec_measure_count - 2 ){
						MIR_write_dot( i, MIR_RED );
					}
					else if ( SOLO_rec_measure_pos == SOLO_rec_measure_count - 1 ){
						MIR_write_dot( i, MIR_GREEN );
					}
					else if ( SOLO_rec_measure_pos == SOLO_rec_measure_count ){
						MIR_write_dot( i, MIR_RED );
						MIR_write_dot( i, MIR_GREEN );
					}
					MIR_write_dot( i, MIR_BLINK );
				}
			}
		}
	}
	else {

		// Quantize value
		// CHORD SECTION
		if ( SOLO_quantize_note > OFF && SOLO_has_rec == ON && G_run_bit == OFF ){
			MIR_write_dot( (LED_QUANTIZE_LOW + SOLO_quantize_note -1), MIR_RED );
		}
	}


	// Strum
	// NUMERIC QUADRANT
	if ( SOLO_strum != 9 ){ // 9 is the OFF value
		MIR_write_dot( LED_ALIGN, MIR_GREEN );
		MIR_write_dot( LED_ALIGN, MIR_RED );
	}
	if ( PHRASE_TIMER == ON || is_pressed_key( KEY_ALIGN )){
		MIR_write_dot( LED_ALIGN, MIR_GREEN );
		MIR_write_dot( LED_ALIGN, MIR_RED );
		show_strum_in_circle( SOLO_strum );
	}
	else if ( SOLO_has_rec == ON && G_run_bit == OFF) {
		// Quantize fine tune value
		// NUMERIC QUADRANT
		switch (SOLO_quantize_fine_tune) {
		case 1:
			MIR_write_dot( 201, MIR_RED );
			break;
		case 2:
			MIR_write_dot( 200, MIR_GREEN );
			break;
		case 3:
			MIR_write_dot( 199, MIR_RED );
			break;
		case 4:
			MIR_write_dot( 198, MIR_GREEN );
			break;
		case 5:
			MIR_write_dot( 197, MIR_RED );
			break;
		case 6:
			MIR_write_dot( 207, MIR_GREEN );
			break;
		case 7:
			MIR_write_dot( 206, MIR_RED );
			break;
		case 8:
			MIR_write_dot( 216, MIR_GREEN );
			break;
		case 9:
			MIR_write_dot( 215, MIR_RED );
			break;
		}
	}

	// MATRIX
	for (i=0; i < MAX_NROF_PAGES; i++) {
		// Page has contents and is not one of the row zero
		if (	(Page_repository[i].page_clear != ON)  &&	( (i % 10) != 9 )){
			// This is our Solo Recording cluster
			if ( SOLO_rec_page != NULL && selected_page_cluster( i, SOLO_rec_page->pageNdx ) != OFF ){
				// Page PLAYING - i.e. selected in GRID
				if ( is_selected_in_GRID( &Page_repository[i] ) ){

					// Show it in GREEN
					GRID_write_dot( i, MIR_GREEN );
				}
				else {
					GRID_write_dot( i, MIR_RED );
				}
			}
			else {

				if ( SOLO_page_play_along[i % 10] == i ){
					GRID_write_dot( i, MIR_GREEN );
					GRID_write_dot( i, MIR_BLINK );
				}
				else {
					// This is a muted page in the grid
					GRID_write_dot( i, MIR_RED );
				}
			}
		} // page_clear != ON
	} // page iterator


	// Write Grid to MIR
	MIR_write_GRID ();


	// Show the GRID cursor
	unsigned char temp = cursor_to_dot( GRID_CURSOR );
	unsigned int selRec = selected_solo_rec_page( GRID_CURSOR, temp );
	if ( selRec == ON ||
	   ( SOLO_rec_page != NULL &&
	     selected_page_cluster( GRID_CURSOR, SOLO_rec_page->pageNdx ) != OFF &&
	     is_pressed_key( temp )
	)){

		if ( selRec == OFF && SOLO_rec_page != NULL ){
			MIR_write_dot( LED_CLEAR, MIR_BLINK );
		}
		if ( SOLO_rec_page == NULL && SOLO_rec_freeflow == OFF ){
			// No recording page has been chosen yet so show the Free Flow button flashing
			// when an eligible grid page is pressed
			MIR_write_dot( LED_CHAINMODE_4, MIR_RED   );
			MIR_write_dot( LED_CHAINMODE_4, MIR_GREEN );
			MIR_write_dot( LED_CHAINMODE_4, MIR_BLINK );
		}

		// Show the pressed recording page or the page to the right that may become a recording page
		if ( SOLO_rec_freeflow == OFF || selected_page_cluster( GRID_CURSOR, SOLO_rec_page->pageNdx ) != OFF ){

			MIR_write_dot( temp, MIR_RED   );
			MIR_write_dot( temp, MIR_GREEN );
			MIR_write_dot( temp, MIR_BLINK );

			unsigned int min = 20;
			unsigned int max = 119;
			unsigned int result = 0;

			// Show the row zero measure count for the pressed page
			for( i=min; i <= max; i+=11 ){
				result = (i - 9) / 11;

				if ( SOLO_has_rec == TRUE ){ // has a recording

					if ( SOLO_rec_page != NULL && Rec_repository[column_of(temp)].measure_count >= result ){

						MIR_write_dot( i, MIR_RED );
						// Show the page edit warp tunnel
						MIR_write_dot( LED_ZOOM_PAGE, MIR_RED   );
						MIR_write_dot( LED_ZOOM_PAGE, MIR_BLINK );
					}
				}
				else {

					if ( SOLO_rec_page != NULL && Rec_repository[column_of(temp)].measure_count == result ){

						MIR_write_dot( i, MIR_GREEN );
					}
					else if ( G_run_bit == OFF ) {
						MIR_write_dot( i, MIR_RED );
					}
				}
			}
		}
	}
	// Continue flashing the measure selection for a moment
	else if ( ROT_INDEX == REC_MEASURES_IDX ){

		unsigned int min = 20;
		unsigned int max = 119;
		unsigned int result = 0;

		// Show the row zero measure count for the pressed page
		for( i=min; i <= max; i+=11 ){
			result = (i - 9) / 11;

			if ( SOLO_rec_page != NULL && Rec_repository[SOLO_rec_pressed_col].measure_count == result ){

				MIR_write_dot( i, MIR_GREEN );
				MIR_write_dot( i, MIR_BLINK );
			}
			else {
				MIR_write_dot( i, MIR_RED );
			}
		}
	}

	// Show the row zero measure position
	if ( G_run_bit == ON ){

		// - and end of recording
		// measure hold

		unsigned int min = 20;
		unsigned int max = 119;
		unsigned int result = 0;

		// Show the row zero measure count for the pressed page
		for( i=min; i <= max; i+=11 ){
			result = (i - 9) / 11;

			if ( SOLO_rec_page != NULL && result == G_measure_locator ){ // Show current measure

				MIR_write_dot( i, MIR_GREEN );
				MIR_write_dot( i, MIR_BLINK );
			}
			else if ( result <= GRID_p_selection[SOLO_rec_bank]->attr_STA ) {
				MIR_write_dot( i, MIR_RED );
			}
		}
	}

	// Show the packman at playtime - unless there is a scale selected
	show ( ELE_OCTAVE_CIRCLE, G_global_locator_PICTURE );


	if ( is_pressed_key(KEY_PASTE) || ( EDIT_TIMER == ON && ROT_INDEX == 10 ) )
	{
		if ( SOLO_midi_ch <= 16 ){

			MIR_point_numeric(
				SOLO_midi_ch,
				9,	MIR_GREEN);
		}
		else if ( GRID_p_set_midi_ch <= 32 ){

			MIR_point_numeric(
				SOLO_midi_ch - 16,
				9,	MIR_RED);
		}
	}
	else if ( ( SOLO_rec_page != NULL ) &&
			( is_pressed_key(KEY_TGGL) || ( EDIT_TIMER == ON && ROT_INDEX == 1 ) ))
	{
		MIR_point_numeric(
			SOLO_normalize_pitch,
			0,	MIR_GREEN);
	}
	else if ( ( SOLO_rec_page != NULL ) &&
			( is_pressed_key(KEY_CLEAR) || ( EDIT_TIMER == ON && ROT_INDEX == 3 ) ))
	{
		MIR_point_numeric(
			SOLO_normalize_len,
			2,	MIR_GREEN);
	}


