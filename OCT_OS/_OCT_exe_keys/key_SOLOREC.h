

	// Shift Arp patterns
	if (( keyNdx == ROT_MCH || keyNdx == KEY_PASTE ) && SOLO_scale_chords_program == ON ){

		unsigned char j;
		for (j=0; j<MATRIX_NROF_COLUMNS; j++) {

			if ( is_pressed_key( 20 + (j * 11)) == TRUE ){ // is there a pressed step
				break;
			}
		}
		if ( j == MATRIX_NROF_COLUMNS ){
			shiftArpPattern( (keyNdx == ROT_MCH), 0 ); // no step is pressed
		}
		else {
			shiftArpPattern( (keyNdx == ROT_MCH), j+1 );
		}
		return;
	}

	if ( keyNdx <= 10 ){
		return; // disable track select buttons
	}

	if ( keyNdx == KEY_BK100 ){
		SOLO_big_counter ^= 1;
		SOLO_big_count = (SOLO_rec_measure_hold == ON) ? OFF : SOLO_rec_measure_pos;
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

	if ( SOLO_rec_freeflow == OFF ){
		if ( keyNdx == KEY_PLAY2 ){ // ON = continue playing after the loop after recording
			SOLO_rec_continue_recording ^= 1; // toggle
		}
	}

	if ( keyNdx == KEY_ALIGN ){

		if ( SOLO_scale_chords_program == ON ){
			return;
		}
		if ( SOLO_rec_strum_latch == ON ){

			SOLO_rec_strum_latch = OFF;
			SOLO_rec_show_strum = OFF;

		} else if ( SOLO_rec_show_strum == ON ) {

			SOLO_rec_strum_latch = ON;
			if ( G_track_rec_bit == ON ){
				SOLO_strum = 9; // reset
			}
		}
		else {

			SOLO_rec_show_strum = ON;
		}
	}

	if ( keyNdx == KEY_ZOOM_TRACK && SOLO_rec_finalized == ON && has_space_to_copy_page_cluster_right( GRID_CURSOR ) ){
		saveRec();
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
	if ( keyNdx == KEY_ZOOM_MAP && SOLO_scale_chords == OFF ){
		G_midi_map_controller_mode ^= 1;
	}

	if ( keyNdx == KEY_SELECT_MASTER ){

		if ( SOLO_scale_chords_program == OFF ){
			if ( SOLO_rec_has_MCC == OFF ){
				SOLO_rec_track_preview ^= 1; // toggle grid and page view because we don't have MCC data

			} else if ( ++SOLO_rec_track_preview > SOLOMCC ){
				SOLO_rec_track_preview = SOLOGRID;
			}
		}
		else { // clear Arp pattern
			if ( SOLO_assistant_page->Track[1]->attr_STATUS == OFF ){

				unsigned char hasArp = OFF;
				for (i=0; i<MATRIX_NROF_COLUMNS; i++){

					if ( Note_get_status( Chord_palette_repository[SOLO_scale_chords_palette_ndx].Arp[i], STEPSTAT_TOGGLE ) == ON ){
						hasArp = ON;
						break;
					}
				}
				if (!hasArp) return;
				saveArpPattern(SOLO_scale_chords_palette_ndx);
				clearArpPattern(SOLO_scale_chords_palette_ndx);
			}
			else {
				restoreArpPattern(SOLO_scale_chords_palette_ndx);
			}
		}
	}

	if ( keyNdx == KEY_MUTE_MASTER ){
		SOLO_mute ^= 1;
	}

	if ( keyNdx == KEY_STOP ){

		if ( G_run_bit == ON &&
		   ( SOLO_rec_page != NULL ||
		     SOLO_scale_chords_program == ON ||
		     GRID_CURSOR == SOLO_assistant_page->pageNdx /* Arp */ )
		   ){

			stop_solo_rec( SOLO_rec_freeflow_trim && SOLO_has_rec == ON, ON );
		}
		else {
			send_ALL_NOTES_OFF();
		}
		if ( is_pressed_key( KEY_MUTE_MASTER )){
			send_note_off_full_range( SOLO_midi_ch, G_pitch_segment++ );
		}
	}

	if ( keyNdx == KEY_SCALE_SEL ){

		SOLO_scale_chords ^= 1;

		if ( SOLO_scale_chords == ON ){

			SOLO_last_controller_mode = G_midi_map_controller_mode;
			G_midi_map_controller_mode = ON;

			if ( SOLO_assistant_page->scaleNotes[G_scale_ndx] == SCALE_SIG_CHR ){

				// There are no scales in Chromatic keys so choose Major
				SOLO_assistant_page->scaleNotes[G_scale_ndx] = SCALE_SIG_MAJ;
			}
			else {
				SOLO_last_note_scale = SOLO_assistant_page->scaleNotes[G_scale_ndx];
			}
		}
		else {
			SOLO_scale_chords_program = OFF;
			SOLO_scale_chords_program_keys = OFF;
			SOLO_scale_chords_program_armed = OFF;
			SOLO_scale_chords_last = OFF;
			G_midi_map_controller_mode = SOLO_last_controller_mode;

			if ( SOLO_last_note_scale == SCALE_SIG_CHR ){

				SOLO_assistant_page->scaleNotes[G_scale_ndx] = SOLO_last_note_scale;
			}

		}
	}
	else if ( SOLO_scale_chords == OFF && SOLO_assistant_page->scaleStatus != OFF ){

		key_ScaleSelector_functions( keyNdx, SOLO_assistant_page );

		if ( keyNdx == KEY_PROGRAM ){

			SOLO_scale_chords_program_keys ^= 1;
		}
	}
	else {

		if ( SOLO_scale_chords_program == ON ){

			if ( keyNdx == KEY_PROGRAM ){

				SOLO_scale_chords_program = OFF;
				SOLO_scale_chords_program_keys = OFF;
				SOLO_scale_chords_program_armed = OFF;
				SOLO_scale_chords_last = OFF;
				return;
			}

			if ( row_of(keyNdx) == 9 ){

				// An empty arp key is clicked
				if ( Note_get_status( Chord_palette_repository[SOLO_scale_chords_palette_ndx].Arp[column_of(keyNdx)], STEPSTAT_TOGGLE ) == OFF ){

					// find the end
					unsigned char stopCol = MATRIX_NROF_COLUMNS;
					for (i=0; i<MATRIX_NROF_COLUMNS; i++){
						if ( Step_get_status( SOLO_assistant_page->Step[0][i], STEPSTAT_SKIP ) == ON ){
							stopCol = i;
						}
					}
					if ( column_of(keyNdx) >= stopCol ){
						return; // arp note copy out of arp range
					}
					for (i=0; i<stopCol; i++){

						if ( is_pressed_key( 20 + (i * 11)) == TRUE &&
							 Note_get_status( Chord_palette_repository[SOLO_scale_chords_palette_ndx].Arp[i], STEPSTAT_TOGGLE ) == ON
						   ){ // another Arp step is already pressed with status ON

							// copy the step
							copyNote( Chord_palette_repository[SOLO_scale_chords_palette_ndx].Arp[i],
									  Chord_palette_repository[SOLO_scale_chords_palette_ndx].Arp[column_of(keyNdx)]);
							noteToStep (Chord_palette_repository[SOLO_scale_chords_palette_ndx].Arp[i],
										SOLO_assistant_page->Step[0][column_of(keyNdx)]);
						}
					}
				}

				// D O U B L E - C L I C K  C O N S T R U C T
				// DOUBLE CLICK SCENARIO
				if (	( DOUBLE_CLICK_TARGET == keyNdx )
					&& 	( DOUBLE_CLICK_TIMER   > DOUBLE_CLICK_ALARM_SENSITIVITY ) ) {

					// Double click code
					// ...
					if ( Note_get_status( Chord_palette_repository[SOLO_scale_chords_palette_ndx].Arp[column_of(keyNdx)], STEPSTAT_TOGGLE ) == ON ){

						unsigned char len = SOLO_assistant_page->Step[0][column_of(keyNdx)]->attr_LEN; // keep the LEN
						initNote( Chord_palette_repository[SOLO_scale_chords_palette_ndx].Arp[column_of(keyNdx)] );
						noteToStep( Chord_palette_repository[SOLO_scale_chords_palette_ndx].Arp[column_of(keyNdx)],
									SOLO_assistant_page->Step[0][column_of(keyNdx)]
								  );
						SOLO_assistant_page->Step[0][column_of(keyNdx)]->attr_LEN = len;
						Chord_palette_repository[SOLO_scale_chords_palette_ndx].Arp[column_of(keyNdx)]->attr_LEN = len;
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


					for (j=0; j<OCTAVE; j++) {

						if ( is_pressed_key( 33 + (j * 11)) == TRUE ){ // an Arp step is pressed

							unsigned char pitches[MAX_NOTES];
							Chordstruct* chord = &Chord_palette_repository[SOLO_scale_chords_palette_ndx];
							translateSymbolsToChord(chords[chord->scale][chord->tone][chord->chord_id], pitches, ON);

							for (i=0; i < MAX_NOTES; i++){

								if ( pitches[i] == NOP ) return;
								if ( pitches[i] == j ){

									chordPitchAddToStep( SOLO_assistant_page, column_of(keyNdx), chord->pitch + pitches[i] );
									return;
								}
							}
						}
					}

					if ( Step_get_status( SOLO_assistant_page->Step[0][column_of(keyNdx)], STEPSTAT_TOGGLE ) == ON ){

						EDIT_TIMER = ON;
						cyg_alarm_initialize(	alarm_hdl,
												cyg_current_time() + TIMEOUT_VALUE,
												0 );

						PLAYER_preview_step( SOLO_assistant_page, ARP_TRACK, column_of(keyNdx) ); // play the sound with the note length
					}
				}
			}
			else if ( row_of(keyNdx) == 0 && is_matrix_key(keyNdx) == TRUE ) {

				unsigned char pitches[MAX_NOTES];
				Chordstruct* chord = &Chord_palette_repository[SOLO_scale_chords_palette_ndx];
				translateSymbolsToChord(chords[chord->scale][chord->tone][chord->chord_id], pitches, ON);

				for (i=0; i < MAX_NOTES; i++){

					if ( pitches[i] == NOP ) return;
					if ( pitches[i] == ( column_of(keyNdx) -2) ){

						playChordPitch( (chord->pitch + pitches[i]), SOLO_midi_ch, TRACK_DEF_VELOCITY, STEP_DEF_LENGTH );
						return;
					}
				}
			}

			switch( keyNdx ){

				// OCTAVE CIRCLE NOTE KEYS
				case KEY_NOTE_C:
				case KEY_NOTE_Cis:
				case KEY_NOTE_D:
				case KEY_NOTE_Dis:
				case KEY_NOTE_E:
				case KEY_NOTE_F:
				case KEY_NOTE_Fis:
				case KEY_NOTE_G:
				case KEY_NOTE_Gis:
				case KEY_NOTE_A:
				case KEY_NOTE_Ais:
				case KEY_NOTE_B:
				{
					unsigned char ndx = keyNdx_to_ndx( keyNdx );
					if ( ndx != NOP ){

						// D O U B L E - C L I C K  C O N S T R U C T
						// DOUBLE CLICK SCENARIO
						if (	( DOUBLE_CLICK_TARGET == keyNdx )
							&& 	( DOUBLE_CLICK_TIMER   > DOUBLE_CLICK_ALARM_SENSITIVITY ) ) {

							// Double click code
							// ...
							initChord(&Chord_palette_repository[ndx], ndx);
							SOLO_scale_chords_palette_ndx = NOP;
							SOLO_scale_chords_last = OFF;
							SOLO_scale_chords_program_armed = OFF;

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
							if ( Chord_palette_repository[ndx].chord_id != NOP ){

								SOLO_scale_chords_palette_ndx = ndx;
								playChordstruct(ndx, TRACK_DEF_VELOCITY, SOLO_midi_ch, OFF);
							}
							else {

								unsigned char pressed = OFF;
								if 		( is_pressed_key( KEY_NOTE_C ) 	 == TRUE ) 	pressed = KEY_NOTE_C;
								else if ( is_pressed_key( KEY_NOTE_Cis ) == TRUE )	pressed = KEY_NOTE_Cis;
								else if ( is_pressed_key( KEY_NOTE_D )	 == TRUE ) 	pressed = KEY_NOTE_D;
								else if ( is_pressed_key( KEY_NOTE_Dis ) == TRUE ) 	pressed = KEY_NOTE_Dis;
								else if ( is_pressed_key( KEY_NOTE_E ) 	 == TRUE ) 	pressed = KEY_NOTE_E;
								else if ( is_pressed_key( KEY_NOTE_F )	 == TRUE ) 	pressed = KEY_NOTE_F;
								else if ( is_pressed_key( KEY_NOTE_Fis ) == TRUE ) 	pressed = KEY_NOTE_Fis;
								else if ( is_pressed_key( KEY_NOTE_G ) 	 == TRUE ) 	pressed = KEY_NOTE_G;
								else if ( is_pressed_key( KEY_NOTE_Gis ) == TRUE ) 	pressed = KEY_NOTE_Gis;
								else if ( is_pressed_key( KEY_NOTE_A )	 == TRUE ) 	pressed = KEY_NOTE_A;
								else if ( is_pressed_key( KEY_NOTE_Ais ) == TRUE ) 	pressed = KEY_NOTE_Ais;
								else if ( is_pressed_key( KEY_NOTE_B ) 	 == TRUE ) 	pressed = KEY_NOTE_B;

								// a chord button in the circle is held and an empty space was pressed
								if ( pressed != OFF && Chord_palette_repository[keyNdx_to_ndx(pressed)].chord_id != NOP ){

									copyChord(&Chord_palette_repository[keyNdx_to_ndx(pressed)],
											  &Chord_palette_repository[ndx]);
								}
							}
						}
					}
					break;
				}
			}
		}
		else {
			key_ChordScaleSelector( keyNdx, SOLO_assistant_page );
		}

		if ( keyNdx == KEY_PROGRAM ){

			if ( SOLO_scale_chords_program_keys == ON ){

				SOLO_scale_chords_program_keys = OFF;
			}
			else if ( SOLO_scale_chords_program == OFF ){

				SOLO_scale_chords_program_keys = ON;
			}
		}
		else if ( SOLO_scale_chords_program == ON ){

			if ( keyNdx == KEY_SCALE_CAD && SOLO_scale_chords_program_octave < 3 ){

				SOLO_scale_chords_program_octave++; // This is the location of the palette octave on the keyboard
			}
			else if ( keyNdx == KEY_SCALE_MOD && SOLO_scale_chords_program_octave > -3 ){

				SOLO_scale_chords_program_octave--;
			}
		}
		else if ( SOLO_rec_transpose != ON ){

			if ( keyNdx == KEY_SCALE_CAD && SOLO_scale_chords_octave < 3 ){

				SOLO_scale_chords_octave++;
			}
			else if ( keyNdx == KEY_SCALE_MOD && SOLO_scale_chords_octave > -3 ){

				SOLO_scale_chords_octave--;
			}
		}
	}

	if ( SOLO_rec_transpose == ON ){

		if ( keyNdx == KEY_SCALE_CAD && SOLO_rec_transpose_octave < 2 ){

			transposeTrack(&Page_repository[GRID_CURSOR], 1);
		}
		else if ( keyNdx == KEY_SCALE_MOD && SOLO_rec_transpose_octave > -2 ){

			transposeTrack(&Page_repository[GRID_CURSOR], -1);
		}
		else if ( keyNdx == KEY_SCALE_MOD || keyNdx == KEY_SCALE_CAD ) {

			SOLO_transpose_latch = OFF;
		}
	}

	if ( SOLO_rec_page != NULL || SOLO_scale_chords_program == ON ){ // A record page cluster is selected

		if ( keyNdx == KEY_PLAY1 ){

			if ( SOLO_rec_freeflow == ON && SOLO_rec_measure_hold == OFF ){
				return; // disable play for Freeflow, once measure hold is broken
			}

			SOLO_rec_track_preview = SOLOPAGE;

			if ( SOLO_scale_chords_program == ON && hasArpPattern( SOLO_scale_chords_palette_ndx ) == ON ){

				SOLO_rec_rehearsal = ON;
				reset_page_cluster( SOLO_assistant_page );
				GRID_CURSOR = SOLO_assistant_page->pageNdx;
				SOLO_transpose_GRID_CURSOR = GRID_CURSOR;
				sequencer_command_PLAY();
			}
			else if ( SOLO_rec_rehearsal == OFF ) {

				G_track_rec_bit = OFF;
				SOLO_rec_rehearsal = ON;
				SOLO_rec_record_OTM = OFF;

				if ( SOLO_has_rec == OFF ){
					SOLO_rec_measure_hold = ON;
				}

				if ( G_run_bit == OFF ){

					reset_page_cluster( SOLO_rec_page );
					playSoloRecCluster();
				}
			}
		}

		else if ( keyNdx == KEY_RECORD &&
				  SOLO_rec_record_OTM == OFF &&
				( G_track_rec_bit == OFF || SOLO_rec_rehearsal == ON ) &&
				  SOLO_scale_chords_program == OFF ){ // Record

			SOLO_rec_record_OTM = OFF;

			if ( is_pressed_key(KEY_PLAY1) || ( G_run_bit == OFF && SOLO_has_rec == OFF && SOLO_rec_freeflow == OFF ) )
			{
				SOLO_rec_record_OTM = ON;
			}
			else {
				G_track_rec_bit = ON;
				SOLO_rec_rehearsal = OFF;
			}

			if ( SOLO_has_rec == OFF ){
				SOLO_rec_measure_hold = ON;
			}

			if ( G_run_bit == OFF ){
				reset_page_cluster( SOLO_rec_page );
			}

			if ( G_track_rec_bit == ON && SOLO_rec_strum_latch == ON ){
				SOLO_strum = 9; // reset
			}

			SOLO_rec_track_preview = SOLOPAGE;

			playSoloRecCluster();
		}

		else if ( keyNdx == KEY_CHAINER && SOLO_scale_chords_program == OFF ){ // Clear recording
			clearRec();
		}

		else if ( keyNdx == KEY_EDIT_MASTER ){ // Undo
			undoRec( keyNdx );
		}

		else if ( keyNdx == KEY_FOLLOW && SOLO_scale_chords_program == OFF ){
			SOLO_overdub ^= 1; // toggle
		}

		// Legato
		if ( keyNdx == KEY_ZOOM_STEP && SOLO_has_rec == ON ){
			SOLO_rec_legato ^= 1; // toggle
			applyEffects();
		}

		// Transpose
		if ( keyNdx == KEY_ZOOM_PLAY && SOLO_has_rec == ON ){

			saveOrUndoTranspose();

			SOLO_rec_transpose ^= 1; // toggle

			if ( SOLO_rec_transpose == ON ){

				captureTransposeRecTrackUndo();
				SOLO_undo_note_all = ON;
			}
			else {

				SOLO_transpose_latch = OFF;
			}
		}
	}

	if ( is_matrix_key(keyNdx) == TRUE && G_run_bit == ON && SOLO_rec_track_preview == SOLOPAGE ){

		// Compute Step coordinates
		unsigned char row = row_of( keyNdx );
		unsigned char col = column_of( keyNdx );
		unsigned char orig_row = row;
		unsigned char orig_col = col;
		signed char lead_pitch_offset;

		if ( Step_get_status(target_page->Step[row][col], STEPSTAT_TOGGLE) == ON ){

			if ( SOLO_undo_page_col != NOP ){
				commitMix();
			}

			Pagestruct* target_page = &Page_repository[GRID_CURSOR];

			SOLO_undo_note = grid_ndx_from_key( keyNdx );
			SOLO_undo_note_page_col = grid_col(target_page->pageNdx);
			Notestruct* undoNote = Rec_undo_repository[SOLO_undo_note_page_col].Note[SOLO_undo_note];
			Notestruct* note = Rec_repository[SOLO_undo_note_page_col].Note[SOLO_undo_note];

			if ( is_pressed_key( KEY_MIXTGT_ATR )) {

				if ( col == 0 && row == 0 ) return; // against the top left page edge

				if ( col == 0 ) {
					col = 15;
					row--;
				}
				else {
					col--;
				}

				if ( 9 - row >= target_page->attr_STA ){
					return;
				}

				lead_pitch_offset = target_page->Track[orig_row]->lead_pitch_offset;
				pivotStep(target_page->Step[row][col], note, lead_pitch_offset, STEP_MAX_START);
				clearStepNote(target_page->Step[orig_row][orig_col]);
				initNote(note); // clear the previous note
				note = Rec_repository[SOLO_undo_note_page_col].Note[grid_ndx(row, col)];
				// adjust LEN since we're moving back one column
				target_page->Step[row][col]->attr_LEN = normalize(target_page->Step[row][col]->attr_LEN + STEP_DEF_LENGTH,
																  STEP_MIN_LENGTH,
																  STEP_MAX_LENGTH);
				stepToNote(target_page->Step[row][col], note);

				return;
			}
			else if ( is_pressed_key( KEY_MIXTGT_VOL )) {

				if ( col == 15 && row == 9 ) return; // against the bottom right page edge

				if ( col == 15 ) {
					col = 0;
					row++;
				}
				else {
					col++;
				}

				lead_pitch_offset = target_page->Track[orig_row]->lead_pitch_offset;
				pivotStep(target_page->Step[row][col], note, lead_pitch_offset, STEP_MIN_START);
				clearStepNote(target_page->Step[orig_row][orig_col]);
				initNote(note); // clear the next note
				note = Rec_repository[SOLO_undo_note_page_col].Note[grid_ndx(row, col)];
				stepToNote(target_page->Step[row][col], note);

				return;
			}

			// Turns the step selection off
			interpret_matrix_stepkey( row, col, target_page );

			if ( SOLO_undo_note != NOP ){
				SOLO_undo_note_all = ON;
			}

			copyNote(note, undoNote);
			initNote(note);
		}
	}

	// Clear the record pages
	if (keyNdx == KEY_CLEAR && G_run_bit == OFF && SOLO_scale_chords_program == OFF ){
		unsigned int pressed = is_pressed_pagerange();
		if ( SOLO_rec_page != NULL &&
			 pressed != FALSE &&
			 selected_page_cluster( grid_ndx_from_key(pressed), SOLO_rec_page->pageNdx ) != NOP
		){

			selected_page_cluster_clear(SOLO_rec_page->pageNdx);
			SOLO_rec_legato				= OFF;
			SOLO_normalize_vel	 		= OFF;
			SOLO_normalize_len 			= OFF;
			SOLO_rec_page 				= NULL;
			SOLO_has_rec 				= OFF;
			SOLO_edit_buffer_volatile 	= OFF;
			SOLO_rec_finalized			= OFF;
			SOLO_undo_page_col			= NOP;
			SOLO_undo_page_len			= OFF;
			SOLO_rec_freeflow 			= OFF;
			SOLO_rec_measure_count 		= OFF;
			SOLO_rec_freeflow_measures	= OFF;
			SOLO_rec_measure_pos		= OFF;
			SOLO_pos_marker_in 			= OFF;
			SOLO_pos_marker_out 		= OFF;
			SOLO_pos_in 				= NULL;
			SOLO_pos_out 				= NULL;
			if ( SOLO_rec_has_MCC == ON && SOLO_rec_track_preview == SOLOMCC ){
				SOLO_rec_track_preview		= SOLOPAGE;
			}
			SOLO_rec_has_MCC			= OFF;
			SOLO_rec_is_tape			= OFF;
			G_measure_locator 			= OFF;
			SOLO_rec_transpose			= OFF;
			Solorec_init();
		}
	}

	else if ( keyNdx == KEY_MIX_MASTER &&
			  SOLO_rec_finalized == ON &&
			  G_run_bit == ON &&
			  SOLO_scale_chords_program == OFF &&
			  G_track_rec_bit == OFF &&
			  SOLO_prev_stop == ON ){

		if ( SOLO_pos_marker_in == OFF ){
			SOLO_pos_marker_in = SOLO_rec_measure_pos;
			SOLO_pos_in = &Page_repository[GRID_CURSOR];
		}
		else {
			stop_solo_rec(FALSE, ON);
		}

		ROT_INDEX = REC_MEASURES_SPLIT;
		// Setup alarm for the EDIT TIMER
		cyg_alarm_initialize(	alarm_hdl,
								cyg_current_time() + (TIMEOUT_VALUE / 4),
								0 );
	}

	// Quantize chord buttons
	else if (keyNdx >= KEY_QUANTIZE_LOW && keyNdx <= KEY_QUANTIZE_HIGH){

		SOLO_quantize_note = keyNdx - KEY_QUANTIZE_LOW;
		applyEffects(); // Apply the quantize
	}
	else if ( SOLO_scale_chords_program == OFF &&
			( is_matrix_key(keyNdx) == TRUE || ( SOLO_rec_freeflow == OFF && keyNdx == KEY_CHAINMODE_4 ) ||
		      keyNdx == KEY_CHAINMODE_3 || keyNdx == KEY_CHAINMODE_2 || keyNdx == KEY_ZOOM_PAGE )) {

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
		else if ( is_matrix_key(keyNdx) == FALSE ) {
			return;
		}

		unsigned char heldCol = grid_col(heldNdx);

//		diag_printf("pressed:%d\n", pressed);
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
					SOLO_rec_measure_hold = ON;
					SOLO_rec_is_tape = ON;
					Rec_repository[heldCol].measure_count = MATRIX_NROF_ROWS;
					Rec_undo_repository[heldCol].measure_count = MATRIX_NROF_ROWS;
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
					Rec_undo_repository[heldCol].measure_count = rowZeroTrack;
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

				if ( keyNdx == KEY_ZOOM_PAGE && G_run_bit == OFF ){
					// Enter the page edit warp tunnel
					GRID_CURSOR = heldNdx;
					GRID_p_selection[ SOLO_rec_bank ] = &Page_repository[heldNdx];
					GRID_p_preselection[ SOLO_rec_bank ] = &Page_repository[heldNdx];
					GRID_p_clock_presel[ SOLO_rec_bank ] = &Page_repository[heldNdx];
					GRID_bank_playmodes = 0;
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
					( SOLO_page_play_along[keyRow] != heldNdx) && // and - this page was not already the play along page
					( grid_row(SOLO_rec_page->pageNdx) != keyRow )
				)
				){

				SOLO_page_play_along[keyRow] = heldNdx;

				if ( GRID_p_selection[ keyRow ] != &Page_repository[heldNdx] ){

					SOLO_page_play_along_toggle[keyRow] = heldNdx;
				}

				grid_select( &Page_repository[heldNdx], ON );
			}
			else {
				SOLO_page_play_along[keyRow] = NOP;
			}
		}

		if ( SOLO_rec_finalized == ON && G_run_bit == OFF ){

			if ( keyNdx == LED_CHAINMODE_3 ){
				// do we have enough empty tracks left to double the current track chain
				duplicate_record_track_chain(&Page_repository[ heldNdx ]);
			}

			if ( keyNdx == LED_CHAINMODE_2 ){
				if ( grid_col(last_page_in_cluster(heldNdx)) < 15 ){
					copy_page_right(heldNdx);
				}
			}
		}
	}

	// Fine tune quantize
	unsigned char xdx = BK_KEY_to_xdx( keyNdx );
	if ( xdx != OFF ){
		if ( xdx < 5 ){
			if ( xdx != 4 || SOLO_quantize_fine_tune_edge != 6 ){
				SOLO_quantize_fine_tune_center = xdx;
			}
		}
		else if ( xdx == 5 ){
			SOLO_quantize_fine_tune_drop_edge ^= 1;
		}
		else if ( xdx < 10 ){
			if ( xdx != 6 || SOLO_quantize_fine_tune_center != 4 ){
				SOLO_quantize_fine_tune_edge = xdx;
			}
		}
		applyEffects();
	}

	unsigned char latencyOffset = 0;
	switch (keyNdx) {
		case KEY_MIXTGT_USR1:
			latencyOffset = 1; // toggle 1
			break;
		case KEY_MIXTGT_USR2:
			latencyOffset = 2;// toggle 2
			break;
		case KEY_MIXTGT_USR3:
			latencyOffset = 3;// toggle 3
			break;
		case KEY_MIXTGT_USR4:
			latencyOffset = 4;// toggle 4
			break;
		case KEY_MIXTGT_USR5:
			latencyOffset = 5;// toggle 5
			break;
		case KEY_MIXTGT_USR0:
			if ( SOLO_scale_chords_program == OFF ){
				SOLO_rec_MCC_enabled ^= 1; // toggle MCC enabled
			}
			break;
		default:
		break;
	}
	if ( latencyOffset > 0 ){
		if ( G_TT_external_latency_offset == latencyOffset ){
			G_TT_external_latency_offset = OFF;
		}
		else {
			G_TT_external_latency_offset = latencyOffset;
		}
	}

	// Arp pattern presets
	switch (keyNdx) {
		case KEY_MIXTGT_ATR: // ABC
		case KEY_MIXTGT_VOL: // CBA
		case KEY_MIXTGT_PAN: // BAC
		case KEY_MIXTGT_MOD: // BCA
		case KEY_MIXTGT_EXP: // ACB
		case KEY_MIXTGT_USR0: // CAB
			buildPresetArp( keyNdx );
		default:
			break;
	}

	if ( G_run_bit == OFF && keyNdx == KEY_PAUSE ){

		unsigned int pressed = is_pressed_pagerange();
		unsigned char pressedNdx = grid_ndx_from_key(pressed);
		unsigned char selRec = selected_solo_rec_page( pressedNdx, pressedNdx );

		if ( pressed != OFF
			 && has_valid_record_cluster_format( &Page_repository[ pressedNdx ], ON ) == ON
			 && Page_repository[ pressedNdx ].page_clear == OFF
			 && selRec == OFF
			 && (
				  SOLO_rec_page == NULL ||
				  selected_page_cluster( pressedNdx, SOLO_rec_page->pageNdx ) == NOP
			 )
		   ){

			exitSoloRec();
			pageClusterEnterSoloRec(pressedNdx);
			enterSoloRec();
		}
	}
