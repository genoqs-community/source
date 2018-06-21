

void rot_exec_SOLOREC( 	Pagestruct* target_page,
						unsigned char rotNdx,
						unsigned char direction ){

	if ( rotNdx == ROT_BIGKNOB )
	{
		PhraseEditGlobalStrum( direction );
	}

	// Set the timer for the active editor
	switch( rotNdx ){
		// EDIT encoders
		case 1: case 3: case 10:
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
			if ( G_solo_rec_page != NULL ){
				modify_parameter(&G_solo_normalize_pitch, 0, 16, direction, OFF, FIXED);
			}
			break;
		case 3:
			if ( G_solo_rec_page != NULL ){
				modify_parameter(&G_solo_normalize_len, 0, 16, direction, OFF, FIXED);
			}
			break;
		case 10:
			// Set the MIDI Channel for solo recording
			modify_parameter(&G_solo_midi_ch, TRACK_MIN_MIDICH, TRACK_MAX_MIDICH, direction, OFF, FIXED);
			break;
	}

}
