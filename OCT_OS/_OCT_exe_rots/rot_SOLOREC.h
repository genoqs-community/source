

void rot_exec_SOLOREC( 	Pagestruct* target_page,
						unsigned char rotNdx,
						unsigned char direction ){

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
			break;
	}

}
