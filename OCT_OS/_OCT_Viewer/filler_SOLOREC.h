
	// TRANSPORT CONTROLS
	if ( SOLO_rec_page != NULL && SOLO_scale_chords_program == OFF ){

		// Record button
		MIR_write_dot( LED_RECORD, MIR_RED );
		if ( G_run_bit == ON && G_track_rec_bit == ON ){
			if ( SOLO_rec_rehersal == OFF ){
				MIR_write_dot( LED_RECORD, MIR_GREEN );
			}
			MIR_write_dot( LED_RECORD, MIR_BLINK );
		}
	}

	if ( G_run_bit == ON && SOLO_rec_has_MCC == ON && SOLO_scale_chords_program == OFF ){
		MIR_write_dot( LED_CHAINER, MIR_GREEN );
		MIR_write_dot( LED_CHAINER, MIR_RED );
	}

	if ( SOLO_has_rec == OFF && SOLO_rec_freeflow == OFF ){
		if ( SOLO_rec_continue_recording == ON ){
			MIR_write_dot( LED_PLAY2, MIR_GREEN );
		}
		else {
			MIR_write_dot( LED_PLAY2, MIR_RED );
		}
	}

	if ( SOLO_scale_chords == ON ){
		MIR_write_dot (LED_SCALE_SEL, MIR_GREEN); // MIDI notes as chords
		MIR_write_dot (LED_SCALE_SEL, MIR_RED);
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
	else if ( is_pressed_key( KEY_MUTE_MASTER ) ){
		MIR_write_dot( LED_STOP, MIR_RED );
		MIR_write_dot( LED_STOP, MIR_BLINK );
	}

	if ( SOLO_has_rec == ON || ( SOLO_scale_chords_program == ON && hasArpPattern( SOLO_scale_chords_palette_ndx ) == ON )){
		if ( G_track_rec_bit == OFF && SOLO_rec_freeflow == OFF ){
			if ( G_run_bit == ON ){
				MIR_write_dot( LED_PLAY1, MIR_GREEN );
			}
			else {
				MIR_write_dot( LED_PLAY1, MIR_RED );
			}
		}
		if ( SOLO_scale_chords_program == OFF ){
			if ( SOLO_overdub == OFF ){
				MIR_write_dot( LED_FOLLOW, MIR_GREEN );
				MIR_write_dot( LED_FOLLOW, MIR_RED );
			}
			else {
				MIR_write_dot( LED_FOLLOW, MIR_RED );
				MIR_write_dot( LED_FOLLOW, MIR_BLINK );
			}

			// Clear recording
			if ( G_run_bit == OFF ){
				MIR_write_dot( LED_CHAINER, MIR_RED );
			}
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

		// Flash every 4 measures when in Free Flow recording
		if ( (SOLO_rec_measure_pos % 4) == 0 && G_run_bit == ON ){

			MIR_write_dot( LED_CHAINMODE_4, MIR_RED );
			MIR_write_dot( LED_CHAINMODE_4, MIR_BLINK );
		}
		else {
			MIR_write_dot( LED_CHAINMODE_4, MIR_RED   );
			MIR_write_dot( LED_CHAINMODE_4, MIR_GREEN );
		}
	}

	// Undo Edit buffer
	if ( SOLO_edit_buffer_volatile == ON && G_run_bit == OFF ){
		MIR_write_dot( LED_EDIT_MASTER, MIR_RED );
		MIR_write_dot( LED_EDIT_MASTER, MIR_GREEN );
		if ( MIX_TIMER == ON ){
			MIR_write_dot( LED_EDIT_MASTER, MIR_BLINK );
		}
	}
	else if ( SOLO_undo_note != NOP ){
		MIR_write_dot( LED_EDIT_MASTER, MIR_GREEN );
	}
	else if ( SOLO_undo_note_all == ON ){
		// MIDI overdub - undo the entire page
		MIR_write_dot( LED_EDIT_MASTER, MIR_GREEN );
		MIR_write_dot( LED_EDIT_MASTER, MIR_RED );
	}

	if ( SOLO_scale_chords_program == OFF ){

		// SEL toggle track preview - measure POS
		MIR_write_dot( LED_SELECT_MASTER, MIR_RED );
		if ( SOLO_rec_track_preview >= SOLOPAGE ){
			MIR_write_dot( LED_SELECT_MASTER, MIR_GREEN );
		}
		if ( SOLO_rec_track_preview == SOLOMCC ){
			MIR_write_dot( LED_SELECT_MASTER, MIR_BLINK );
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

	if (SOLO_rec_page != NULL){

		if ( SOLO_has_rec == ON ){

			if ( G_run_bit == OFF ){
				// Grid controls
				if ( SOLO_scale_chords_program == OFF ){

					MIR_write_dot( LED_TGGL, MIR_RED ); // VEL
					MIR_write_dot( LED_TGGL, MIR_GREEN );
				}

				// Legato
				if ( SOLO_rec_legato == ON ){
					MIR_write_dot( LED_ZOOM_STEP, MIR_GREEN );
				}
				else {
					MIR_write_dot( LED_ZOOM_STEP, MIR_RED ); // STEP ZOOM for Legato note length toggle
				}
			}

			// Transpose
			if ( SOLO_rec_transpose == ON ){
				MIR_write_dot( LED_ZOOM_PLAY, MIR_GREEN );
				MIR_write_dot( LED_ZOOM_PLAY, MIR_BLINK );
			}
			else if ( SOLO_scale_chords_program == OFF ) {
				MIR_write_dot( LED_ZOOM_PLAY, MIR_RED );
			}
		}
	}

	if ( SOLO_scale_chords_program == OFF && GRID_CURSOR != SOLO_assistant_page->pageNdx /* Arp */ ){

		// MIX recording split button
		if ( ROT_INDEX == REC_MEASURES_SPLIT && G_run_bit == OFF ){
			MIR_write_dot( LED_MIX_MASTER, MIR_RED );
			MIR_write_dot( LED_MIX_MASTER, MIR_BLINK );
		}

		if ( SOLO_rec_finalized == ON && G_run_bit == ON ){
			MIR_write_dot( LED_MIX_MASTER, MIR_RED );
			// The recording is playing and not recording so enable split markers using POS
			if ( G_run_bit == ON && G_track_rec_bit == OFF ){
				MIR_write_dot( LED_MIX_MASTER, MIR_GREEN );
				if ( SOLO_pos_marker_in != OFF ){
					MIR_write_dot( LED_MIX_MASTER, MIR_BLINK );
				}
				if ( SOLO_pos_marker_in != OFF ){
					MIR_write_dot( LED_MIX_INDICATOR, MIR_GREEN );
				}
			}
		}
	}

	if ( SOLO_rec_has_MCC == ON ){
		MIR_write_dot( LED_EDIT_INDICATOR, MIR_GREEN );
		MIR_write_dot( LED_EDIT_INDICATOR, MIR_RED );
	}

	// Show the MODE - (Special) Grid Solo Zoom
	MIR_write_dot( LED_ZOOM_GRID, MIR_RED );
	MIR_write_dot( LED_ZOOM_GRID, MIR_GREEN );
	MIR_write_dot( LED_ZOOM_GRID, MIR_BLINK );

	if ( SOLO_scale_chords_program == OFF ){

		MIR_write_dot( LED_PASTE, MIR_RED ); // MCH
		MIR_write_dot( LED_PASTE, MIR_GREEN );
	}
	else {

		MIR_write_dot( LED_PASTE, MIR_RED ); // Arp shift left

		// Arp presets
		MIR_write_dot( LED_MIXTGT_ATR, MIR_GREEN ); // ABC
		if ( is_pressed_key( LED_MIXTGT_ATR )){
			MIR_write_dot( LED_MIXTGT_ATR, MIR_RED );
		}
		MIR_write_dot( LED_MIXTGT_VOL, MIR_GREEN ); // CBA
		if ( is_pressed_key( LED_MIXTGT_VOL )){
			MIR_write_dot( LED_MIXTGT_VOL, MIR_RED );
		}
		MIR_write_dot( LED_MIXTGT_PAN, MIR_GREEN ); // BAC
		if ( is_pressed_key( LED_MIXTGT_PAN )){
			MIR_write_dot( LED_MIXTGT_PAN, MIR_RED );
		}
		MIR_write_dot( LED_MIXTGT_MOD, MIR_GREEN ); // BCA
		if ( is_pressed_key( LED_MIXTGT_MOD )){
			MIR_write_dot( LED_MIXTGT_MOD, MIR_RED );
		}
		MIR_write_dot( LED_MIXTGT_EXP, MIR_GREEN ); // ACB
		if ( is_pressed_key( LED_MIXTGT_EXP )){
			MIR_write_dot( LED_MIXTGT_EXP, MIR_RED );
		}
		MIR_write_dot( LED_MIXTGT_USR0, MIR_GREEN ); // CAB
		if ( is_pressed_key( LED_MIXTGT_USR0 )){
			MIR_write_dot( LED_MIXTGT_USR0, MIR_RED );
		}
	}

	unsigned short measures = (SOLO_rec_freeflow == OFF) ? SOLO_rec_measure_count : SOLO_rec_freeflow_measures;

	// Flash the end of recording indicator using the CHORD LEDs
	if ( SOLO_rec_ending_flash == ON && G_run_bit == ON &&
		 SOLO_rec_measure_pos == measures &&
		 measures > 1 ){ // Flash the last measure

		for (i=LED_QUANTIZE_FIRST; i <= LED_QUANTIZE_HIGH; i++) {
			MIR_write_dot( i, MIR_RED );
			MIR_write_dot( i, MIR_GREEN );
			MIR_write_dot( i, MIR_BLINK );
		}
	}
	else if ( SOLO_rec_ending_flash > ON && G_run_bit == ON &&
			  SOLO_rec_measure_pos > (measures - 3) &&
			  measures > 3 ){ // 3 - 2 - 1 ... Flash the measure count down

		for (i=LED_QUANTIZE_FIRST; i <= LED_QUANTIZE_HIGH; i++) {
			if ( SOLO_rec_measure_pos == measures - 2 ){
				MIR_write_dot( i, MIR_RED );
			}
			else if ( SOLO_rec_measure_pos == measures - 1 ){
				MIR_write_dot( i, MIR_GREEN );
			}
			else if ( SOLO_rec_measure_pos == measures ){
				MIR_write_dot( i, MIR_RED );
				MIR_write_dot( i, MIR_GREEN );
			}
			MIR_write_dot( i, MIR_BLINK );
		}
	}
	else {

		// Quantize value
		// CHORD SECTION
		if ( SOLO_quantize_note == OFF ){

			MIR_write_dot( LED_QUANTIZE_LOW, MIR_RED );
		}
		else {
			MIR_write_dot( (LED_QUANTIZE_LOW + SOLO_quantize_note), MIR_GREEN );
		}

		if ( SOLO_rec_quantize_first_beat == TRUE ){
			MIR_write_dot( LED_QUANTIZE_FIRST, MIR_GREEN );
		}
		MIR_write_dot( LED_QUANTIZE_FIRST, MIR_RED );
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
	else {
		// Quantize fine tune value
		// NUMERIC QUADRANT
		switch (SOLO_quantize_fine_tune_center) {
		case 1:
			MIR_write_dot( 201, MIR_RED );
			break;
		case 2:
			MIR_write_dot( 200, MIR_GREEN );
			break;
		case 3:
			MIR_write_dot( 199, MIR_GREEN );
			break;
		case 4:
			MIR_write_dot( 198, MIR_GREEN );
			break;
		}

		if ( SOLO_quantize_fine_tune_drop_edge == OFF ){
			MIR_write_dot( 197, MIR_GREEN );
			MIR_write_dot( 197, MIR_RED );
		}
		else {
			MIR_write_dot( 197, MIR_RED ); // drop the notes
		}

		switch (SOLO_quantize_fine_tune_edge) {
		case 6:
			MIR_write_dot( 207, MIR_GREEN );
			break;
		case 7:
			MIR_write_dot( 206, MIR_GREEN );
			break;
		case 8:
			MIR_write_dot( 216, MIR_GREEN );
			break;
		case 9:
			MIR_write_dot( 215, MIR_RED );
			break;
		}
	}


	if ( SOLO_rec_track_preview == SOLOGRID ||
		 G_run_bit == OFF ||
		 SOLO_scale_chords_program == ON ||
		 TEMPO_TIMER == ON ||
		 GRID_CURSOR == SOLO_assistant_page->pageNdx /* Arp */
	   ){

		// MATRIX
		for (i=0; i < MAX_NROF_PAGES; i++) {

			if (( TEMPO_TIMER == ON && grid_row(i) == 0 ) || SOLO_scale_chords_program == ON ){
				continue; // we are showing the chord notes in the top row
			}

			int pressed_grid = is_pressed_pagerange();
			unsigned char pressed_ndx = grid_ndx_from_key(pressed_grid);
			// DEBUG -- see the undo notes in the grid
//			if ( Rec_repository[grid_col(SOLO_rec_page->pageNdx)].Note[i]->status == ON ){
			if ( SOLO_rec_track_preview != SOLOGRID &&
				 pressed_grid != FALSE &&
				 grid_row(pressed_ndx) == SOLO_rec_bank &&
				 Rec_repository[grid_col(pressed_ndx)].Note[i]->status == ON
			   ){

				// Shot the notes in the pressed page
				GRID_write_dot( i, MIR_GREEN );
				if ( is_note_chord(Rec_repository[grid_col(pressed_ndx)].Note[i]) ){
					GRID_write_dot( i, MIR_RED );
				}
			}
//			if ( is_pressed_pagerange() != FALSE && Rec_undo_repository[grid_col(grid_ndx_from_key(is_pressed_pagerange()))].Note[i]->status == ON ){
//
//				GRID_write_dot( i, MIR_RED );
//				GRID_write_dot( i, MIR_BLINK );
//			}

			// Page has contents and is not one of the row zero
			if (	(Page_repository[i].page_clear != ON)  &&	( grid_row(i) != 9 )){
				// This is our Solo Recording cluster
				if ( SOLO_rec_page != NULL && selected_page_cluster( i, SOLO_rec_page->pageNdx ) != NOP ){
					// Page PLAYING - i.e. selected in GRID
					if ( is_selected_in_GRID( &Page_repository[i] ) ){

						// Show it in GREEN
						GRID_write_dot( i, MIR_GREEN );

						// Flash for Free Flow
						if ( SOLO_rec_freeflow == ON ){
							GRID_write_dot( i, MIR_RED );
							GRID_write_dot( i, MIR_BLINK );
						}
					}
					else {
						GRID_write_dot( i, MIR_RED );
					}
				}
				else {

					if ( SOLO_page_play_along[grid_row(i)] == i ){
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
	}


	// Show the GRID cursor
	unsigned char flashClear = OFF;
	unsigned int pressed = is_pressed_pagerange();
	unsigned char pressedNdx = grid_ndx_from_key(pressed); // the key that is pressed FIXME: short circuit
	unsigned char selRec = selected_solo_rec_page( pressedNdx, pressedNdx );

	if ( SOLO_scale_chords_program == OFF &&
	   ( selRec == ON ||
	   ( SOLO_rec_page != NULL &&
	     selected_page_cluster( pressedNdx, SOLO_rec_page->pageNdx ) != NOP
	))){

		if ( selRec == OFF && SOLO_rec_page != NULL && SOLO_scale_chords_program == OFF ){
			MIR_write_dot( LED_CLEAR, MIR_BLINK );
			flashClear = ON;
		}

		if ( SOLO_rec_finalized == ON && G_run_bit == OFF ){
			// do we have enough empty tracks left to double the current track chain
			if ( (10 / Rec_repository[ grid_col(pressedNdx) ].measure_count) >= 2 ){
				MIR_write_dot( LED_CHAINMODE_3, MIR_RED   );
				MIR_write_dot( LED_CHAINMODE_3, MIR_GREEN );
			}

			if ( grid_col(pressedNdx) < 15 ){
				MIR_write_dot( LED_CHAINMODE_2, MIR_RED   );
				MIR_write_dot( LED_CHAINMODE_2, MIR_GREEN );
			}
		}

		if ( SOLO_rec_page == NULL && SOLO_rec_freeflow == OFF && has_empty_grid_row_ahead(pressedNdx) == TRUE ){
			// No recording page has been chosen yet so show the Free Flow button flashing
			// when an eligible grid page is pressed
			MIR_write_dot( LED_CHAINMODE_4, MIR_RED   );
			MIR_write_dot( LED_CHAINMODE_4, MIR_GREEN );
			MIR_write_dot( LED_CHAINMODE_4, MIR_BLINK );
		}

		// Show the pressed recording page or the page to the right that may become a recording page
		if ( pressed != FALSE && ( SOLO_rec_freeflow == OFF || selected_page_cluster( pressedNdx, SOLO_rec_page->pageNdx ) != NOP )){

			MIR_write_dot( pressed, MIR_RED   );
			MIR_write_dot( pressed, MIR_GREEN );
			MIR_write_dot( pressed, MIR_BLINK );

			unsigned int min = 20;
			unsigned int max = 119;
			unsigned int result = 0;

			if ( SOLO_has_rec == TRUE && G_run_bit == OFF ){
				// Show the page edit warp tunnel
				MIR_write_dot( LED_ZOOM_PAGE, MIR_RED   );
				MIR_write_dot( LED_ZOOM_PAGE, MIR_BLINK );
			}

			if ( SOLO_rec_track_preview == SOLOGRID || SOLO_has_rec == FALSE ){

				// Show the row zero measure count for the pressed page
				for( i=min; i <= max; i+=11 ){
					result = (i - 9) / 11;

					if ( SOLO_has_rec == TRUE ){ // has a recording

						if ( SOLO_rec_page != NULL && Rec_repository[ grid_col(pressedNdx) ].measure_count >= result ){

							MIR_write_dot( i, MIR_RED );
						}
					}
					else {

						if ( SOLO_rec_page != NULL && Rec_repository[ grid_col(pressedNdx) ].measure_count == result ){

							MIR_write_dot( i, MIR_GREEN );
						}
						else if ( G_run_bit == OFF ) {
							MIR_write_dot( i, MIR_RED );
						}
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

	if ( SOLO_rec_page != NULL && G_run_bit == OFF && SOLO_scale_chords_program == OFF ){

		MIR_write_dot( LED_CLEAR, MIR_RED ); // LEN / clr
		if ( !flashClear ){
			MIR_write_dot( LED_CLEAR, MIR_GREEN );
		}
	}

	if ( G_run_bit == ON ){

		// Show the row zero measure position
		if ( SOLO_rec_track_preview == SOLOGRID && GRID_CURSOR != SOLO_assistant_page->pageNdx /* Arp */ ){

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

		if ( SOLO_rec_track_preview == SOLOPAGE ){

			if ( SOLO_scale_chords_program == OFF && GRID_CURSOR != SOLO_assistant_page->pageNdx /* !Arp */ ){

				// MATRIX
				show ( ELE_MATRIX, STEP_TOGGLE );

				// Show the REC status of tracks
				show( ELE_TRACK_SELECTORS, TRACK_REC_STATUS );

				MIR_write_lauflicht ();
			}

			if ( SOLO_scale_chords == OFF ){

				MIR_write_dot (LED_SCALE_MYSEL, MIR_GREEN); // Scale

				if ( SOLO_rec_transpose == ON ){

					show_OCTAVE_CIRCLE_chord_octave_transpose_selection( SOLO_rec_transpose_octave );
					MIR_write_dot( LED_SCALE_SEL, MIR_RED );
				}
				else {
					// OCTAVE CIRCLE
					show_GLOBALS_scale_master( SOLO_assistant_page );
				}

				// Show the selected notes in scale. Both in MOD and SEL
				show_OCTAVE_CIRCLE_scale_selection( SOLO_assistant_page );
			}
			else {
				// Chord tone select
				show_OCTAVE_CIRCLE_chord_tone_selection( SOLO_assistant_page );
				show_OCTAVE_CIRCLE_chord_octave_transpose_selection( SOLO_scale_chords_program_octave );
			}
			show_SCALE_SELECTOR_scale_selection( SOLO_assistant_page );

			if ( SOLO_scale_chords_program_keys == ON ){
				MIR_write_dot (LED_PROGRAM, MIR_GREEN);
			}
		}
		else if ( SOLO_rec_track_preview == SOLOMCC ){

			// MATRIX
			if ( G_run_bit == ON ){

				// Show the current MCC track chase-light at the bottom
				unsigned char trackNdx = my_bit2ndx( Page_repository[GRID_CURSOR].priv_track_REC_pattern );
				MIR_write_lauflicht_track( trackNdx, 9 );
			}
			show ( ELE_MATRIX, ATTR_MAP_VALUES );
		}
		else {

			// Show the packman at playtime - unless there is a scale selected
			show ( ELE_OCTAVE_CIRCLE, G_global_locator_PICTURE );
		}
	}
	else {

		if ( SOLO_scale_chords == OFF ){

			MIR_write_dot (LED_SCALE_MYSEL, MIR_GREEN); // Scale

			if ( SOLO_rec_transpose == ON ){

				show_OCTAVE_CIRCLE_chord_octave_transpose_selection( SOLO_rec_transpose_octave );
				MIR_write_dot( LED_SCALE_SEL, MIR_RED );
			}
			else {
				// OCTAVE CIRCLE
				show_GLOBALS_scale_master( SOLO_assistant_page );
			}

			// Show the selected notes in scale. Both in MOD and SEL
			show_OCTAVE_CIRCLE_scale_selection( SOLO_assistant_page );
			show_SCALE_SELECTOR_scale_selection( SOLO_assistant_page );

			if ( SOLO_scale_chords_program_keys == ON ){
				MIR_write_dot (LED_PROGRAM, MIR_GREEN);
			}
		}
		else {

			// Chord tone select and grid display
			show_OCTAVE_CIRCLE_chord_tone_selection( SOLO_assistant_page );

			if ( SOLO_scale_chords_program == OFF ){

				if ( SOLO_rec_transpose == ON ){

					show_OCTAVE_CIRCLE_chord_octave_transpose_selection( SOLO_rec_transpose_octave );
				}
				else {
					show_OCTAVE_CIRCLE_chord_octave_transpose_selection( SOLO_scale_chords_octave );
				}
				show_SCALE_SELECTOR_scale_selection( SOLO_assistant_page );
				if ( SOLO_scale_chords_program_keys == ON ){
					MIR_write_dot (LED_PROGRAM, MIR_GREEN);
				}
			}
			else {
				MIR_write_dot (LED_PROGRAM, MIR_GREEN); // Program chord keys
				MIR_write_dot (LED_PROGRAM, MIR_RED);
				MIR_write_dot (LED_PROGRAM, MIR_BLINK);
				show_OCTAVE_CIRCLE_chord_octave_transpose_selection( SOLO_scale_chords_program_octave );
			}
			if ( SOLO_scale_chords_program_armed == ON ){
				MIR_write_dot( LED_SCALE_MYSEL, MIR_RED );
				MIR_write_dot( LED_SCALE_MYSEL, MIR_BLINK );
			}
		}
	}

	if ( SOLO_scale_chords_program == ON && SOLO_scale_chords_palette_ndx != NOP ){

		unsigned char len = NOP, pressed = FALSE;

		for (i=0; i<MATRIX_NROF_COLUMNS; i++){ // scan arp steps

			if ( Note_get_status( Chord_palette_repository[SOLO_scale_chords_palette_ndx].Arp[i], STEPSTAT_TOGGLE ) == ON ){ // there is an arp

				pressed = is_pressed_key( 20 + (i * 11));
				if ( pressed == TRUE ){ // an Arp step is pressed

					// show the pressed step length
					len = Chord_palette_repository[SOLO_scale_chords_palette_ndx].Arp[i]->attr_LEN;
					break;
				}

				// show the arp length
				len = Chord_palette_repository[SOLO_scale_chords_palette_ndx].attr_LEN;
			}
		}
		if ( len == NOP ) return; // not an arp

		// LENGTH
		if ( pressed == TRUE ){

			if ( len == LEGATO ){

				// Step is set to play legato, show legato flag
				MIR_write_trackpattern ( 0x0f, 2, MIR_GREEN );
			}
			else {
				// The common case is when the length value is shown
				MIR_write_length_H( len, 2 );
			}
		}
		else {

			MIR_point_numeric(
				len,
				2,	MIR_GREEN);
			MIR_point_numeric(
				len,
				2,	MIR_RED);
		}
	}

	if (( is_pressed_key(KEY_PASTE) || ( EDIT_TIMER == ON && ROT_INDEX == 10 )) && SOLO_scale_chords_program == OFF )
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
	else if ( ( SOLO_rec_page != NULL && SOLO_scale_chords_program == OFF ) &&
			( is_pressed_key(KEY_TGGL) || ( EDIT_TIMER == ON && ROT_INDEX == 1 ) ))
	{
		MIR_point_numeric(
			SOLO_normalize_pitch,
			0,	MIR_GREEN);
	}
	else if ( ( SOLO_rec_page != NULL && SOLO_scale_chords_program == OFF ) &&
			( is_pressed_key(KEY_CLEAR) || ( EDIT_TIMER == ON && ROT_INDEX == 3 ) ))
	{
		MIR_point_numeric(
			SOLO_normalize_len,
			2,	MIR_GREEN);
	}

	switch (G_TT_external_latency_offset) {
		case 1:
			MIR_write_dot( LED_MIXTGT_USR1,	MIR_RED);
			break;
		case 2:
			MIR_write_dot( LED_MIXTGT_USR2,	MIR_RED);
			break;
		case 3:
			MIR_write_dot( LED_MIXTGT_USR3, MIR_RED);
			break;
		case 4:
			MIR_write_dot( LED_MIXTGT_USR4,	MIR_RED);
			break;
		case 5:
			MIR_write_dot( LED_MIXTGT_USR5,	MIR_RED);
			break;
		default:
			break;
	}

