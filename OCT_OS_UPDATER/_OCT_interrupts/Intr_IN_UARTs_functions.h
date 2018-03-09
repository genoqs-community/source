



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
				// &&	( zoom_level == zoomDEVICE )
				){

				// RUNNING STATUS: SYSEX
				running_status_byte_IN_UART[UART_ndx] = in_midi_byte;

				switch( in_midi_byte ){
					
					case MIDI_CMD_SYSTEM_COMMON_SYSEX:

						// Reset byte counter of sysex stream, count first byte
						sysex_byte_count = 1;

						sysex_stream_keeper_index 		= 0;
						sysex_header 			= 0;
						midi_in_sysex_nibble 	= 0;
						in_stream_checksum 		= 0;						
						break;


					case MIDI_CMD_SYSTEM_COMMON_SYSEX_END:
						
						// Final SYSEX byte count - for printing purposes	
						sysex_byte_count ++;
						// ..insert print statement here if needed to debug..

						if ( in_stream_checksum == 0 ){
							
							switch( sysex_header ){
							
								// Check against genoQs Machines ID (+ type field)
								case 0x00206400 + _OS_SYSEX_DUMP:
									// OSFILE Dump
									zoom_level = zoomDEVICE;
									DEVICE_STATE = UPLOAD_COMPLETE;
									break;
		
								case 0x00206400 + _CONFIGFILE_SYSEX_DUMP:
									// CONFIGFILE Dump
									zoom_level = zoomDEVICE;									
									DEVICE_STATE = UPLOAD_COMPLETE;
									break;

								// PAGE DUMP
								case 0x00206400 + _PAGE_SYSEX_DUMP:
									// Single Page Dump - revitalize from flatrecord
									Page_import_from_flatrecord_buffer( 
										SYSEX_IN_flatrecord, 
										&Page_repository[ SYSEX_IN_flatrecord->pageNdx ] );								

									// Release the MIDI SYSEX state
									zoom_level = zoomGRID;
									break;
	
								// GRID DUMP
								case 0x00206400 + _GRID_SYSEX_DUMP:

									// Move received contents into the flatrecord buffer
									memcpy( &(memory_image.Grid_flatrecord_buffer),
											SYSEX_IN_flatrecord,
											sizeof( Grid_flatrecord_struct ) ); 

									// Revitalize from flatrecord
									Grid_import_from_flatrecord_buffer();

									// Release the MIDI SYSEX state
									zoom_level = zoomGRID;
									break;
									
							} // switch( sysex_header )		

							sysex_stream_keeper_index = 0;
							MIDI_RECEIVE_SYSEX = FALSE;
							MIDI_SYSEX_HEADER_SEEN = FALSE;
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

//d_iag_printf( "data_byte:%x status:%x\n", in_midi_byte, running_status_byte_IN_UART[ UART_ndx ] );

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
			midi_PGMCH_execute( in_midi_byte, UART_ndx );
			break;
			
		case MIDI_CMD_CHANNEL_PRESSURE:
			break;
		
		case MIDI_CMD_BENDER:
			midi_interpret_BENDER( in_midi_byte, UART_ndx );
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
	
					switch( sysex_header ){
					
						// Check agains genoQs Machines ID (+ type field)
						case 0x00206400 + _OS_SYSEX_DUMP:
							// OS Full Dump
							// Handled separately in zoomDevice mode.
							break;

						case 0x00206400 + _CONFIGFILE_SYSEX_DUMP:
							// OS Full Dump
							// Handled separately in zoomDevice mode.
							break;

						case 0x00206400 + _PAGE_SYSEX_DUMP:
							// Single Page Dump
							// Code keeper array points to a sysex_in buffer.
							sysex_stream_keeper = (unsigned char*) SYSEX_IN_flatrecord;
							break;

						case 0x00206400 + _GRID_SYSEX_DUMP:
							// Grid settings dump
							// Code keeper array points to a sysex_in buffer.
							// sysex_stream_keeper = (unsigned char*) &(memory_image.Grid_flatrecord_buffer);
							sysex_stream_keeper = (unsigned char*) SYSEX_IN_flatrecord;
							break;

						default:
							// Clear running status byte, to ignore the rest of sysex stream
							running_status_byte_IN_UART[ UART_ndx ] = 0;
							sysex_header = 0;
							break;
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

				// The byte has been completed, determine what to do with the byte
				if ( midi_in_sysex_nibble == 0 ){

					switch ( sysex_header ){
					
						// OS Full Dump
						case 0x00206400 + _OS_SYSEX_DUMP:
						case 0x00206400 + _CONFIGFILE_SYSEX_DUMP:
													
							// Accept OS dumps only in zoomDEVICE mode.
							if ( zoom_level != zoomDEVICE ) break;

							DEVICE_STATE = UPLOAD_IN_PROGRESS;

							// Put it into the sysex_stream_keeper array
							sysex_stream_keeper[ sysex_stream_keeper_index ] =
								sysex_to_byte( midi_in_sysex[ 0 ], midi_in_sysex[ 1 ] );						
			
							// Increment the sysex_stream_keeper_pointer
							sysex_stream_keeper_index ++;
							break;
	
		
						// Single Page Dump
						case 0x00206400 + _PAGE_SYSEX_DUMP:

							// Store the MIDI bytes for further processing in SYSEX_IN_flatrecord;
							sysex_stream_keeper[ sysex_stream_keeper_index ] =
								sysex_to_byte( midi_in_sysex[ 0 ], midi_in_sysex[ 1 ] );						
		
							// Increment the sysex_stream_keeper_pointer
							sysex_stream_keeper_index ++;
							break;
								

						// Single Page Dump
						case 0x00206400 + _GRID_SYSEX_DUMP:

							// Store the MIDI bytes for further processing in SYSEX_IN_flatrecord;
							sysex_stream_keeper[ sysex_stream_keeper_index ] =
								sysex_to_byte( midi_in_sysex[ 0 ], midi_in_sysex[ 1 ] );						
		
							// Increment the sysex_stream_keeper_pointer
							sysex_stream_keeper_index ++;
							break;

					} // switch( sysex_header )
				
				} // midi_in_sysex_nibble == 0
			}

			break;
	}
}






