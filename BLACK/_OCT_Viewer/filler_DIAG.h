
	//
	// FILLER in the DIAG mode of the UI Hardware
	//
	switch( DIAG_rotary_current ){

		// No real rotary is selected. Rotary count starts at 1
		case 200:

			// Iterate through the key color array
			for (i=0; i < 258; i++ ){
			
				// DIAG[i] contains the color as 0 (off), 1 (red), 2 (green), 3 (orange)
				switch( DIAG[i+1] ){
		
					case 0:
						// Nothing to do
						break;
						
					case 1:
						MIR_write_dot( i+1, MIR_RED   );
						if ( i+1 == 258 ){
							MIR_write_dot( 259, MIR_RED   );
							MIR_write_dot( 260, MIR_RED   );
						}
		
						break;
		
					case 2:
						MIR_write_dot( i+1, MIR_GREEN );
						if ( i+1 == 258 ){
							MIR_write_dot( 259, MIR_GREEN );
							MIR_write_dot( 260, MIR_GREEN );
						}
						break;
						
					case 3:
						MIR_write_dot( i+1, MIR_RED   );
						MIR_write_dot( i+1, MIR_GREEN );
						if ( i+1 == 258 ){
							MIR_write_dot( 259, MIR_RED   );
							MIR_write_dot( 260, MIR_RED   );
							MIR_write_dot( 259, MIR_GREEN );
							MIR_write_dot( 260, MIR_GREEN );
						}
						break;
				}
			}
			break;


		default:
			// Indicate the rotary showing
			switch( DIAG_rotary_current ){

				// Big knob
				case 0:
					MIR_write_dot( LED_TEMPO, MIR_RED   );
					MIR_write_dot( LED_TEMPO, MIR_GREEN );
					MIR_write_dot( LED_TEMPO, MIR_BLINK );					
					break;
	
				case 1: case 2: case 3: case 4: case 5: case 6: case 7: case 8: case 9: case 10:
					MIR_write_dot( 186 + DIAG_rotary_current, MIR_RED   );
					MIR_write_dot( 186 + DIAG_rotary_current, MIR_GREEN );
					MIR_write_dot( 186 + DIAG_rotary_current, MIR_BLINK );
					break;

				case 11: case 12: case 13: case 14: case 15: case 16: case 17: case 18: case 19: case 20:
					MIR_write_dot( DIAG_rotary_current - 10, MIR_RED   );
					MIR_write_dot( DIAG_rotary_current - 10, MIR_GREEN );
					MIR_write_dot( DIAG_rotary_current - 10, MIR_BLINK );
					break;
			}
		
			// Show the rotary value in the circle
			MIR_write_numeric_C( DIAG_rotary_value[ DIAG_rotary_current ] );

			break;

	} // switch( DIAG_rotary_current )	
