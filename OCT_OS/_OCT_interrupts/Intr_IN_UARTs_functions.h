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



#include "../_OCT_interrupts/interpret_SYSEX.h"



// Gets the status byte and the UART that has received the byte
void read_MIDI_STATUS_BYTE( 	unsigned char in_midi_byte,
								unsigned char UART_ndx 			){

	unsigned char nibble_high = ( 0xF0 & in_midi_byte );
	unsigned char nibble_low  = ( 0x0F & in_midi_byte );
//	unsigned int i = 0;

	switch( nibble_high ){

		case MIDI_CMD_NOTE_ON:
			// Update the running status byte
			G_running_status_byte_IN_UART[ UART_ndx ] = in_midi_byte;
			// Update the data indexes for the print array
			G_midi_in[0] = G_running_status_byte_IN_UART[ UART_ndx ];
			// Reset the receive index to get in sync with input.
			G_midi_data_ndx = 0;
			// d_iag_printf( "new rst[%d]:%x\n", UART_ndx, G_running_status_byte_IN_UART[ UART_ndx ] );
			break;

		case MIDI_CMD_NOTE_OFF:
			// Update the running status byte
			G_running_status_byte_IN_UART[ UART_ndx ] = in_midi_byte;
			// Update the data indexes for the print array
			G_midi_in[0] = G_running_status_byte_IN_UART[ UART_ndx ];
			// Reset the receive index to get in sync with input.
			G_midi_data_ndx = 0;
			// d_iag_printf( "new rst[%d]:%x\n", UART_ndx, G_running_status_byte_IN_UART[ UART_ndx ] );
			break;

		case MIDI_CMD_CONTROL:
			// Update the running status byte
			G_running_status_byte_IN_UART[ UART_ndx ] = in_midi_byte;
			// Update the data indexes for the print array
			G_midi_in[0] = G_running_status_byte_IN_UART[ UART_ndx ];
			// Reset the receive index to get in sync with input.
			G_midi_controller_ndx = 0;
			// d_iag_printf( "new rst[%d]:%x\n", UART_ndx, G_running_status_byte_IN_UART[ UART_ndx ] );
			break;

		case MIDI_CMD_PGM_CHANGE:
			// Update the running status byte
			G_running_status_byte_IN_UART[ UART_ndx ] = in_midi_byte;
			// Update the data indexes for the print array
			G_midi_in[0] = G_running_status_byte_IN_UART[ UART_ndx ];
			break;

		case MIDI_CMD_NOTE_PRESSURE:
			// Update the running status byte
			G_running_status_byte_IN_UART[ UART_ndx ] = in_midi_byte;
			// Update the data indexes for the print array
			G_midi_in[0] = G_running_status_byte_IN_UART[ UART_ndx ];
			break;

		case MIDI_CMD_CHANNEL_PRESSURE:
			// Update the running status byte
			G_running_status_byte_IN_UART[ UART_ndx ] = in_midi_byte;
			// Update the data indexes for the print array
			G_midi_in[0] = G_running_status_byte_IN_UART[ UART_ndx ];
			// Reset the receive index to get in sync with input.
			G_midi_pressure_ndx = 0;
			break;

		case MIDI_CMD_BENDER:
			// Update the running status byte
			G_running_status_byte_IN_UART[ UART_ndx ] = in_midi_byte;
			// Update the data indexes for the print array
			G_midi_in[0] = G_running_status_byte_IN_UART[ UART_ndx ];
			// Reset the receive index to get in sync with input.
			G_midi_bender_ndx = 0;
			break;

		case MIDI_CMD_SYSTEM:

			// MIDI_CMD_SYSTEM_COMMON
			if (	( nibble_low <= 0x07 )
				// &&	( G_zoom_level == zoomDEVICE )
				){

				// RUNNING STATUS: SYSEX
				G_running_status_byte_IN_UART[UART_ndx] = in_midi_byte;

				switch( in_midi_byte ){

					case MIDI_CMD_SYSTEM_COMMON_SYSEX:

						// Reset byte counter of sysex stream, count first byte
						G_sysex_byte_count 			= 1;

						G_sysex_stream_keeper_index = 0;
						G_sysex_header 				= 0;
						G_midi_in_sysex_nibble 		= 0;
						G_in_stream_checksum 		= 0;
						break;


					case MIDI_CMD_SYSTEM_COMMON_SYSEX_END:

						// The total numbers of sysex bytes received
						G_sysex_byte_count++;

//						d_iag_printf( "-> SYSEX stream - size:%d our-cksum:%d their-cksum:%d ndx:%d prev:%d\n",
//								G_sysex_byte_count,
//								G_in_stream_checksum,
//								G_sysex_stream_keeper[G_sysex_stream_keeper_index],
//								G_sysex_stream_keeper_index,
//								G_sysex_stream_keeper[G_sysex_stream_keeper_index -1]);

						// If the checksum test is passed
						if ( G_in_stream_checksum == 0 ) {

							// d_iag_printf( "sysex upload accepted\n");
							interpret_SYSEX_stream( G_sysex_stream_keeper_index );
						}

						// Reset the SYSEX receive state
						G_sysex_byte_count = 0;
						G_sysex_stream_keeper_index = 0;
						MIDI_RECEIVE_SYSEX = FALSE;
						MIDI_SYSEX_HEADER_SEEN = FALSE;

						break;
				}
			}

			// MIDI_CMD_SYSTEM_REALTIME
			else if ( nibble_low >= 0x08 ) {

				// Sequencer is not running, set to be slave..
				// ..and start byte received
				if (	( G_run_bit == OFF )
					&&	( G_clock_source == EXT )
					&&	( in_midi_byte == MIDICLOCK_START )
					){

						CLOCK_MASTER_UART = UART_ndx;
					}

				// No running status to remember here
				// Interpret the byte as a REALTIME message,
				// ..only if coming from the designated source UART.
				if ( CLOCK_MASTER_UART == UART_ndx ){

					G_midi_interpret_REALTIME( in_midi_byte );
				}
			}
			break;
	}
}




// Have read a MIDI data byte - sort through the running status to interpret it
void read_MIDI_DATA_BYTE( unsigned char in_midi_byte, unsigned char UART_ndx ){

	// Check upper nibble of running status byte of the machine
	switch( G_running_status_byte_IN_UART[ UART_ndx ] & 0xF0 ){

		case MIDI_CMD_NOTE_OFF:
			// There is transformation of NOTE_OFF to NOTE_ON in the interpreter
			G_midi_interpret_NOTE_ON( in_midi_byte, UART_ndx );
			break;

		case MIDI_CMD_NOTE_ON:
			G_midi_interpret_NOTE_ON( in_midi_byte, UART_ndx );
			break;

		case MIDI_CMD_NOTE_PRESSURE:
			// Not implemented
			break;

		case MIDI_CMD_CONTROL:
			G_midi_interpret_CONTROL( in_midi_byte, UART_ndx );
			break;

		case MIDI_CMD_PGM_CHANGE:
			midi_PGMCH_execute( in_midi_byte, UART_ndx );
			break;

		case MIDI_CMD_CHANNEL_PRESSURE:
			G_midi_interpret_PRESSURE( in_midi_byte, UART_ndx);
			break;

		case MIDI_CMD_BENDER:
			G_midi_interpret_BENDER( in_midi_byte, UART_ndx );
			break;


		case MIDI_CMD_SYSTEM_COMMON_SYSEX:

			// Machine is receiving SYSEX data, count the bytes
			G_sysex_byte_count++;

			// Add current byte to the checksum
			G_in_stream_checksum ^= in_midi_byte;

			// Decide how to handle the streamed bytes
			switch( G_sysex_byte_count ){

				// SYSEX header content: Manufacturer ID and dump type stored directly, no assembly
				case 0: case 1: case 2: case 3: case 4: case 5:

					// Check if we are receiving OS files
					if ( G_sysex_byte_count == 5 ){

						// OS file dump detected
						if 	(	( in_midi_byte == def_OS_SYSEX_DUMP )
							|| 	( in_midi_byte == def_CONFIGFILE_SYSEX_DUMP )
							){

							// Signal that we are receiving an OS dump
							G_sysex_header = 0x00206400 + in_midi_byte;

							// Ensure the right machine state for OS updates
							if ( G_zoom_level == zoomDEVICE ){
								DEVICE_STATE = UPLOAD_IN_PROGRESS;
							}
							else {
								// Reset the SYSEX receive state
								G_sysex_byte_count 							= 0;
								G_sysex_stream_keeper_index 				= 0;
								MIDI_RECEIVE_SYSEX 							= FALSE;
								MIDI_SYSEX_HEADER_SEEN 						= FALSE;
								G_running_status_byte_IN_UART[ UART_ndx ] 	= 0;
								return;
							}
						}
					}

					// Header goes straight to the stream keeper, keep it as it comes
					G_sysex_stream_keeper[ G_sysex_stream_keeper_index ] = in_midi_byte;

  					// Increment the G_sysex_stream_keeper_pointer
 					if ( G_sysex_stream_keeper_index < G_sysex_stream_keeper_size-1 ) {
 						G_sysex_stream_keeper_index++;
 					}
					break;

				// Data content - these bytes are to be assembled to real 8bit bytes
				default:

					// Pairs of received sysex bytes are assembled to normal bytes
					G_midi_in_sysex[ G_midi_in_sysex_nibble ] = in_midi_byte;

					// Increment the nibble pointer in circle between 0 and 1
					G_midi_in_sysex_nibble = (G_midi_in_sysex_nibble + 1) % 2;

					// The byte has been completed, store it in the keeper array
					if ( G_midi_in_sysex_nibble == 0 ){

						// Store the MIDI bytes for further processing;
						G_sysex_stream_keeper[ G_sysex_stream_keeper_index ] =
							sysex_to_byte( G_midi_in_sysex[ 0 ], G_midi_in_sysex[ 1 ] );

  						// Increment the G_sysex_stream_keeper_pointer
 						if ( G_sysex_stream_keeper_index < G_sysex_stream_keeper_size-1 ) {
 							G_sysex_stream_keeper_index++;
 						}
					}
					break;

			} // switch on G_sysex_byte_count

			break;
	}
}






