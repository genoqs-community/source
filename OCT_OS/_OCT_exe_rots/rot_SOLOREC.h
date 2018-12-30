

void rot_exec_SOLOREC( 	Pagestruct* target_page,
						unsigned char rotNdx,
						unsigned char direction ){

	int i;

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
							  LEGATO,
							  STEP_MAX_LENGTH,
							  direction,
							  OFF,
							  FIXED);
		}
		return;
	}

	if ( G_run_bit == ON ){
		return; // Don't allow parameter modification while the machine is playing
	}

	if ( rotNdx == ROT_BIGKNOB && SOLO_has_rec == ON )
	{
		PhraseEditGlobalStrum( direction );
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
