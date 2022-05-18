

void rot_exec_SOLOREC( 	Pagestruct* target_page,
						unsigned char rotNdx,
						unsigned char direction ){

	int i;

	if ( rotNdx == ROT_BIGKNOB )
	{
		PhraseEditGlobalStrum( direction );

		if ( G_run_bit == ON && G_track_rec_bit == ON ){

			unsigned int 	current_TTC		= 	G_TTC_abs_value;
			unsigned char 	row				=	0;
			unsigned char	target_col		=	0;

			Pagestruct* target_page = &Page_repository[GRID_CURSOR];

			signed char strum_val = SOLO_strum;

			if ( SOLO_rec_strum_latch == OFF ){
				strum_val = 9; // reset
			}

			// Iterate through the rows - to enable multi-track recording
			for ( row = 0; row < MATRIX_NROF_ROWS; row++ ) {

				// If track not record enabled, continue, remember it otherwise
				if ( (Page_getTrackRecPattern(target_page) & (1 << row)) == 0 ){
					continue;
				}

				if ( current_TTC <= 6 ) {
					// Place step in current column
					target_col = target_page->Track[row]->attr_LOCATOR -1;
				}
				else {
					// Place step in next column, which may have to wrap- -1 is locator vs col notation offset.
					target_col = get_next_tracklocator( target_page->Track[row],
														target_page->Track[row]->attr_LOCATOR ) -1;
				}

				if ( Step_get_status( target_page->Step[row][target_col], STEPSTAT_TOGGLE ) == ON){

					if ( SOLO_rec_finalized == TRUE ){

						if ( SOLO_undo_note_all == OFF ){

							// save the undo notes
							commitMix();
						}

						SOLO_undo_note_all = ON;
					}

					target_page->Step[row][target_col]->chord_data = ( strum_val << 11 )
						| ( target_page->Step[row][target_col]->chord_data & 0x7FF );

					stepToNote(target_page->Step[row][target_col],
							   Rec_repository[grid_col(target_page->pageNdx)].Note[grid_ndx(row, target_col)]);

				}
			}
		}
		else {

			applyStrumToPageCluster();
		}
	}

	if ( SOLO_scale_chords_program == ON ){

		for (i=0; i<MATRIX_NROF_COLUMNS; i++) {

			if ( is_pressed_key( 20 + (i * 11)) == TRUE &&
				 Note_get_status( Chord_palette_repository[SOLO_scale_chords_palette_ndx].Arp[i], STEPSTAT_TOGGLE ) == ON
			   ){ // an Arp step is pressed

				modify_parameter(&SOLO_assistant_page->Step[ARP_TRACK][i]->attr_LEN,
								  1,
								  STEP_MAX_LENGTH,
								  direction,
								  OFF,
								  FIXED);

				modify_parameter(&Chord_palette_repository[SOLO_scale_chords_palette_ndx].Arp[i]->attr_LEN,
								  1,
								  STEP_MAX_LENGTH,
								  direction,
								  OFF,
								  FIXED);

				return;
			}
		}

		// modify the Arp length
		if ( rotNdx == 3 && Chord_palette_repository[SOLO_scale_chords_palette_ndx].chord_id != NOP ){

			modify_parameter(&Chord_palette_repository[SOLO_scale_chords_palette_ndx].attr_LEN,
							  1,
							  TRACK_MAX_LENGTH,
							  direction,
							  OFF,
							  FIXED);

			modify_parameter(&SOLO_assistant_page->Track[ARP_TRACK]->LEN_factor,
							  1,
							  TRACK_MAX_LENGTH,
							  direction,
							  OFF,
							  FIXED);

		}

		// modify the chord pitch offset
		if ( rotNdx == 2 && Chord_palette_repository[SOLO_scale_chords_palette_ndx].chord_id != NOP ){

			MIDI_send( MIDI_CC, SOLO_midi_ch, 123, 0 ); // All Notes Off for this instrument

			modify_parameter(&Chord_palette_repository[SOLO_scale_chords_palette_ndx].pitch,
							  STEP_DEF_PITCH,
							  STEP_MAX_PITCH,
							  direction,
							  OFF,
							  FIXED);
		}

		return;
	}


	if ( SOLO_has_rec == ON && SOLO_rec_finalized == ON ){
		// Act according to rot index
		switch( rotNdx ){

			// EDIT encoders
			case 1:

				modify_parameter(&SOLO_normalize_vel, 0, MATRIX_NROF_COLUMNS, direction, OFF, FIXED);
				SOLO_apply_effects_alarm = ON;
				break;
			case 3:
				modify_parameter(&SOLO_normalize_len, 0, MATRIX_NROF_COLUMNS, direction, OFF, FIXED);
				SOLO_apply_effects_alarm = ON;
				break;
			case ROT_7:
				modify_signed_parameter(&SOLO_len_adjust, -MATRIX_NROF_COLUMNS, MATRIX_NROF_COLUMNS, direction, OFF, FIXED);
				SOLO_apply_effects_alarm = ON;
				break;
		}
	}

	// Set the timer for the active editor
	switch( rotNdx ){
		// EDIT encoders
		case 1: case 3: case ROT_7:
		if ( SOLO_has_rec == OFF ){
			break;
		}
		default:
			EDIT_TIMER = ON;
			ROT_INDEX = rotNdx;
			// Setup alarm for the EDIT TIMER
			cyg_alarm_initialize(	alarm_hdl,
									cyg_current_time() + TIMEOUT_VALUE / 3,
									0 );
	}

	// Act according to rot index
	switch( rotNdx ){

		case 10:
			MIDI_send( MIDI_CC, SOLO_midi_ch, 123, 0 ); // All Notes Off for this instrument

			// Set the MIDI Channel for solo recording
			modify_parameter(&SOLO_midi_ch, TRACK_MIN_MIDICH, TRACK_MAX_MIDICH, direction, OFF, FIXED);
			assign_solorec_track_midi_ch( SOLO_rec_page->pageNdx );
			break;
	}

}
