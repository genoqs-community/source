



// Gets the status byte and the UART that has received the byte
void read_MIDI_STATUS_BYTE( 	unsigned char in_midi_byte, 
								unsigned char UART_ndx 			){
	
	unsigned char nibble_high = ( 0xF0 & in_midi_byte );
	unsigned char nibble_low  = ( 0x0F & in_midi_byte );
	//	unsigned char i = 0;

	// Dont show active sensing bytes
//	if ( in_midi_byte != 0xfe ){
//		d_iag_printf( "status_byte:%x hi:%x lo:%x\n", in_midi_byte, nibble_high, nibble_low );
//	}
	
	switch( nibble_high ){

		case MIDI_CMD_NOTE_ON:
			// Update the running status byte
			running_status_byte_IN_UART[ UART_ndx ] = in_midi_byte;
			// Update the data indexes for the print array
			midi_in[0] = running_status_byte_IN_UART[ UART_ndx ];
		
			// NOTE ON received - update array index
			midi_data_ndx = 1;
// d_iag_printf( "new rst[%d]:%x\n", UART_ndx, running_status_byte_IN_UART[ UART_ndx ] );

			break;
			

		case MIDI_CMD_NOTE_OFF:
			// Update the running status byte
			running_status_byte_IN_UART[ UART_ndx ] = in_midi_byte;
			// Update the data indexes for the print array
			midi_in[0] = running_status_byte_IN_UART[ UART_ndx ];
		
			// NOTE OFF received - update array index
			midi_data_ndx = 1;
// d_iag_printf( "new rst[%d]:%x\n", UART_ndx, running_status_byte_IN_UART[ UART_ndx ] );

			break;


		case MIDI_CMD_CONTROL:
			// Update the running status byte
			running_status_byte_IN_UART[ UART_ndx ] = in_midi_byte;

			// Update the data indexes for the print array
			midi_in[0] = running_status_byte_IN_UART[ UART_ndx ];

			// CONTROLLER received - update array index
			midi_controller_ndx = 1;
			break;
			


		case MIDI_CMD_PGM_CHANGE:
			// Update the running status byte
			running_status_byte_IN_UART[ UART_ndx ] = in_midi_byte;
			// Update the data indexes for the print array
			midi_in[0] = running_status_byte_IN_UART[ UART_ndx ];
			break;
			
		case MIDI_CMD_NOTE_PRESSURE:
			// Update the running status byte
			running_status_byte_IN_UART[ UART_ndx ] = in_midi_byte;
			// Update the data indexes for the print array
			midi_in[0] = running_status_byte_IN_UART[ UART_ndx ];
			break;
			
		case MIDI_CMD_CHANNEL_PRESSURE:
			// Update the running status byte
			running_status_byte_IN_UART[ UART_ndx ] = in_midi_byte;
			// Update the data indexes for the print array
			midi_in[0] = running_status_byte_IN_UART[ UART_ndx ];
			break;
			
		case MIDI_CMD_BENDER:
			// Update the running status byte
			running_status_byte_IN_UART[ UART_ndx ] = in_midi_byte;
			// Update the data indexes for the print array
			midi_in[0] = running_status_byte_IN_UART[ UART_ndx ];
			break;
				
			

		case MIDI_CMD_SYSTEM:
			
			// MIDI_CMD_SYSTEM_COMMON
			if (	( nibble_low <= 0x07 )
				&&	( zoom_level == zoomDEVICE )
				){

				// RUNNING STATUS: SYSEX
				running_status_byte_IN_UART[UART_ndx] = in_midi_byte;

				switch( in_midi_byte ){
					
					case MIDI_CMD_SYSTEM_COMMON_SYSEX:

						// Reset byte counter of sysex stream, count first byte
						sysex_byte_count = 1;

						code_keeper_index 		= 0;
						sysex_header 			= 0;
						midi_in_sysex_nibble 	= 0;
						in_stream_checksum 		= 0;						
						break;


					case MIDI_CMD_SYSTEM_COMMON_SYSEX_END:
						
						// Final SYSEX byte count - for printing purposes	
						sysex_byte_count ++;
						// ..insert print statement here..

						// Reset the bytecount
						sysex_byte_count = 0;
						if ( in_stream_checksum == 0 ){
							
							SOFTWARE_UPDATE_RECEIVED = TRUE;
						}
						break;
				}
			}
			
			// MIDI_CMD_SYSTEM_REALTIME
			else if ( nibble_low >= 0x08 ) {

				// Sequencer is not running, set to be slave..
				// ..and start byte received
				if (	( RUNBIT == OFF )
					&&	( CLOCK_SOURCE == EXT )
					&&	( in_midi_byte == MIDICLOCK_START )
					){

						CLOCK_MASTER_UART = UART_ndx;
					}

				// No running status to remember here
				// Interpret the byte as a REALTIME message, 
				// ..only if coming from the designated source UART.
				if ( CLOCK_MASTER_UART == UART_ndx ){

					midi_interpret_REALTIME( in_midi_byte );
				}
			}
			break;
	}
}




// Have read a MIDI data byte - sort through the running status to interpret it
void read_MIDI_DATA_BYTE( unsigned char in_midi_byte, unsigned char UART_ndx ){

	// d_iag_printf( "data_byte:%x status:%x\n", in_midi_byte, running_status_byte_IN_UART[ UART_ndx ] );

	// Check upper nibble of running status byte of the machine
	switch( running_status_byte_IN_UART[ UART_ndx ] & 0xF0 ){

		case MIDI_CMD_NOTE_OFF:
			// There is transformation of NOTE_OFF to NOTE_ON in the interpreter
			midi_interpret_NOTE_ON( in_midi_byte, UART_ndx );
			break;
			
		case MIDI_CMD_NOTE_ON:
			midi_interpret_NOTE_ON( in_midi_byte, UART_ndx );
			break;
			
		case MIDI_CMD_NOTE_PRESSURE:
			break;
			
		case MIDI_CMD_CONTROL:
			midi_interpret_CONTROL( in_midi_byte, UART_ndx );
			break;
		
		case MIDI_CMD_PGM_CHANGE:
			midi_interpret_PGMCH( in_midi_byte, UART_ndx );
			break;
			
		case MIDI_CMD_CHANNEL_PRESSURE:
			break;
		
		case MIDI_CMD_BENDER:
			// midi_interpret_BENDER( in_midi_byte, UART_ndx );
			break;
			

		case MIDI_CMD_SYSTEM_COMMON_SYSEX:

			// Machine is receiving SYSEX data, count the bytes			
			sysex_byte_count++;

			// SYSEX HEADER
			if ( sysex_byte_count <= 5 ){

				// Assemble
				sysex_header |= (in_midi_byte << ( 8 * (5 - sysex_byte_count) ));

				// Analyze
				if ( sysex_byte_count == 4 ){
	
					// Check agains genoQs Machines ID (+ type field)
					if ( sysex_header != 0x00206400 ){
	
						// Clear running status byte, to ignore the rest of sysex stream
						running_status_byte_IN_UART[ UART_ndx ] = 0;
						sysex_header = 0;
					}
					else{
						
						// Initialize the checksum for the sysex stream
						// checksum = 0;
					}
				}
			} // sysex count is <= 5

			// SYSEX DATA
			else{
				
				// Store the midi_byte for further processing
				midi_in_sysex[ midi_in_sysex_nibble ] = in_midi_byte;

				// Add current byte to the checksum
				in_stream_checksum ^= midi_in_sysex[ midi_in_sysex_nibble ];

				// Increment the nibble pointer
				midi_in_sysex_nibble = (midi_in_sysex_nibble + 1) % 2;

				// The byte has been completed, store the full byte in code keeper
				if ( midi_in_sysex_nibble == 0 ){

					// Put it into the code_keeper
					code_keeper[ code_keeper_index ] =
						sysex_to_byte( midi_in_sysex[ 0 ], midi_in_sysex[ 1 ] );						

					// Increment the code_keeper_pointer
					code_keeper_index ++;
				}
			}

			break;
	}
}






