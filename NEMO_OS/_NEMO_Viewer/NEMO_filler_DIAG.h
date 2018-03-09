//
// Copyright (C) 2004, 2005, 2006, 2007 genoQs Machines
// This file is part of Octopus_OS -
// the firmware of the genoQs Machines Octopus, MIDI Control Sequencer
//
// The source code Octopus_OS is available for use in accordance with the
// GNU General Public License (GPL). For more information about Open Source
// licensing, please visit http://www.opensource.org/
//
// Octopus_OS is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation; either version 2 of the License, or
// (at your option) any later version.
//
// Octopus_OS is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with Octopus_OS; if not, write to the Free Software
// Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
//




	//
	// FILLER in the DIAG mode of the UI Hardware
	//
	switch( DIAG_rotary_current ){

		// No real rotary is selected. Rotary count starts at 1
		case 200:

			// Iterate through the key color array
			for (i=0; i < 258; i++ ){

				// DIAG[i] contains the color as 0 (off), 1 (red), 2 (green), 3 (orange), 4 (rainbow)
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


		// Big knob is selected
		case 0:

			switch( DIAG_rotary_value[ DIAG_rotary_current ] ){

				case 255:
					for ( i=0; i < 261; i++ ){
						MIR_write_dot( i, MIR_RED );
					}
					break;

				case 254:
					for ( i=0; i < 261; i++ ){
						MIR_write_dot( i, MIR_GREEN );
					}
					break;

				case 253:
					for ( i=0; i < 261; i++ ){
						MIR_write_dot( i, MIR_SHINE_GREEN );
					}
					break;

				default:
					// Show the rotary value in the circle
 					MIR_write_tempo( DIAG_rotary_value[ DIAG_rotary_current ], 0 );
					break;
			}

			MIR_write_dot( LED_TEMPO, MIR_RED   );
			MIR_write_dot( LED_TEMPO, MIR_GREEN );
			MIR_write_dot( LED_TEMPO, MIR_BLINK );
			break;



		default:
			// Indicate the rotary showing
			switch( DIAG_rotary_current ){

				case 1: case 2: case 3: case 4: case 5: case 6: case 7: case 8: case 9: case 10:
					MIR_write_dot( 186 + DIAG_rotary_current, MIR_RED   );
					MIR_write_dot( 186 + DIAG_rotary_current, MIR_GREEN );
					MIR_write_dot( 186 + DIAG_rotary_current, MIR_BLINK );

					MIR_write_dot( LED_EDIT_INDICATOR, MIR_RED   );
					MIR_write_dot( LED_EDIT_INDICATOR, MIR_GREEN );
					MIR_write_dot( LED_EDIT_INDICATOR, MIR_BLINK );
					break;

				case 11: case 12: case 13: case 14: case 15: case 16: case 17: case 18: case 19: case 20:
					MIR_write_dot( DIAG_rotary_current - 10, MIR_RED   );
					MIR_write_dot( DIAG_rotary_current - 10, MIR_GREEN );
					MIR_write_dot( DIAG_rotary_current - 10, MIR_BLINK );

					MIR_write_dot( LED_MIX_INDICATOR, MIR_RED   );
					MIR_write_dot( LED_MIX_INDICATOR, MIR_GREEN );
					MIR_write_dot( LED_MIX_INDICATOR, MIR_BLINK );
					break;
			}

			// Show the rotary value in the corresponding line
			switch( DIAG_rotary_current ){
			case 0:		i = 0;		break;
			case 1:		i = 0;		break;
			case 2:		i = 1;		break;
			case 3:		i = 2;		break;
			case 4:		i = 3;		break;
			case 11:	i = 0;		break;
			case 12:	i = 1;		break;
			case 13:	i = 2;		break;
			case 14:	i = 3;		break;
			}

			MIR_write_tempo( DIAG_rotary_value[ DIAG_rotary_current ], i );

			break;

	} // switch( DIAG_rotary_current )
