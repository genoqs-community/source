

void rot_exec_SOLOREC( 	Pagestruct* target_page,
						unsigned char rotNdx,
						unsigned char direction ){

	int i;

	if ( rotNdx == ROT_BIGKNOB )
	{
		PhraseEditGlobalStrum( direction );
		if ( SOLO_rec_strum_latch == ON ){

			if ( G_run_bit == ON && G_track_rec_bit == ON ){

				unsigned int 	current_TTC		= 	G_TTC_abs_value;
				unsigned char 	row				=	0;
				unsigned char	target_col		=	0;

				Pagestruct* target_page = &Page_repository[GRID_CURSOR];

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

						target_page->Step[row][target_col]->chord_data = ( SOLO_strum << 11 )
							| ( target_page->Step[row][target_col]->chord_data & 0x7FF );

						stepToNote(target_page->Step[row][target_col],
								   Rec_repository[grid_col(target_page->pageNdx)].Note[grid_ndx(row, target_col)]);

						if ( SOLO_rec_finalized == TRUE ){

							SOLO_undo_note_all = ON;
						}
						SOLO_strum = 9; // reset
					}
				}
			}
			else {

				applyStrumToPageCluster();
			}
		}
	}

	if ( SOLO_scale_chords_program == ON ){

		for (i=0; i<MATRIX_NROF_COLUMNS; i++) {

			if ( is_pressed_key( 20 + (i * 11)) == TRUE &&
				 Note_get_status( Chord_palette_repository[SOLO_scale_chords_palette_ndx].Arp[i], STEPSTAT_TOGGLE ) == ON
			   ){ // an Arp step is pressed

				modify_parameter(&SOLO_assistant_page->Step[0][i]->attr_LEN,
								  LEGATO,
								  STEP_MAX_LENGTH,
								  direction,
								  OFF,
								  FIXED);

				modify_parameter(&Chord_palette_repository[SOLO_scale_chords_palette_ndx].Arp[i]->attr_LEN,
								  LEGATO,
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

			modify_parameter(&SOLO_assistant_page->Track[0]->LEN_factor,
							  1,
							  TRACK_MAX_LENGTH,
							  direction,
							  OFF,
							  FIXED);
		}

		// modify the chord pitch offset
		if ( rotNdx == 2 && Chord_palette_repository[SOLO_scale_chords_palette_ndx].chord_id != NOP ){

			modify_parameter(&Chord_palette_repository[SOLO_scale_chords_palette_ndx].pitch,
							  STEP_DEF_PITCH,
							  STEP_MAX_PITCH,
							  direction,
							  OFF,
							  FIXED);
		}

		return;
	}

	if ( G_run_bit == ON ){
		return; // Don't allow parameter modification while the machine is playing
	}

	// Set the timer for the active editor
	switch( rotNdx ){
		// EDIT encoders
		case 1: case 3:
		if ( SOLO_has_rec == OFF || G_run_bit == ON ){
			break;
		}
		case 10:
			EDIT_TIMER = ON;
			ROT_INDEX = rotNdx;
			// Setup alarm for the EDIT TIMER
			cyg_alarm_initialize(	alarm_hdl,
									cyg_current_time() + TIMEOUT_VALUE,
									0 );
	}

	// Act according to rot index
	switch( rotNdx ){

		// EDIT encoders
		case 1:
			if ( SOLO_has_rec == ON && G_run_bit == OFF ){
				modify_parameter(&SOLO_normalize_pitch, 0, 16, direction, OFF, FIXED);
			}
			break;
		case 3:
			if ( SOLO_has_rec == ON && G_run_bit == OFF ){
				modify_parameter(&SOLO_normalize_len, 0, 16, direction, OFF, FIXED);
			}
			break;
		case 10:
			// Set the MIDI Channel for solo recording
			modify_parameter(&SOLO_midi_ch, TRACK_MIN_MIDICH, TRACK_MAX_MIDICH, direction, OFF, FIXED);
			assign_solorec_track_midi_ch( SOLO_rec_page->pageNdx );
			break;
	}

}
