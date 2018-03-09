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




	// This is the content of the UART0_OUT and UART1_OUT threads 


	unsigned int 	outMsg=0;
	unsigned int*	message = NULL;

	unsigned char 	type=0, 
					channel=0, 
					command=0, 
					pitch=0, 
					velo=0, 
					value=0, 
					controller=0;
	
	while (1) {
		
		// Read in the next MIDI OUT message
		message = cyg_mbox_get( UARTx_OUT_mbox_handle );
			
		// Don't continue if data is not consistent
		if ( message == NULL ) continue;
		outMsg = *( message );

		// Determine message type and channel
		type 		= ( outMsg & 0xFF000000) >>  	24;
		channel 	= ( outMsg & 0x00FF0000) >> 	16;

		switch( type ){


			case MIDI_NOTE:		
				pitch		= ( outMsg & 0x0000FF00) >>  8;
				velo		= ( outMsg & 0x000000FF) >>  0;
			
		// Show what is in the MIDI pipe
		// d_iag_printf( "\nUARTx_OUT_Msg:0x%x ty:0x%x ch:0x%x pi:0x%x ve:0x%x\n", 
		//	(int)outMsg, type, channel, pitch, velo );

				// Differentiate between MIDI NOTE ON and MIDI NOTE OFF
				// Transform NOTE ON with velocity 0 to NOTE OFF with velocity 64
				switch( velo ){
	
					case 0:
						// d_iag_printf( "**** NOTE_OUT: ch:%d pi:%d ve:%d TTC:%d\n", 
						//	channel, pitch, velo, G_TTC_abs_value );	

						// Transform this to a NOTE OFF message
						if ( *running_status_byte != (0x80+(channel-1)) ){
		
							// Update the running status byte
							*running_status_byte = 0x80+(channel-1);

							// Send the status byte
							HAL_WRITE_UINT32( E7T_UARTx_BASE + E7T_UART_TXBUF, *running_status_byte);
							Player_wait( E7T_UARTx_BASE );
						}
							
						// Default OFF velocity for NOTE OFF is 64
						velo = 64;

						break;
					
					default:
						// d_iag_printf( "**** NOTE_OUT: ch:%d pi:%d ve:%d TTC:%d\n", 
						//	channel, pitch, velo, G_TTC_abs_value );	
		
						// Send StepON+channel: STATUS BYTE - check if still valid
						if ( *running_status_byte != (0x90+(channel-1)) ){
		
							// Update the running status byte
							*running_status_byte = 0x90+(channel-1);
							
							// Send the status byte
							HAL_WRITE_UINT32( E7T_UARTx_BASE + E7T_UART_TXBUF, *running_status_byte);
							Player_wait( E7T_UARTx_BASE );
						}
						break;				
				}

					
				//Send the Pitch
				HAL_WRITE_UINT32( E7T_UARTx_BASE + E7T_UART_TXBUF, pitch );
				Player_wait( E7T_UARTx_BASE );
					
				//Send the Velocity
				HAL_WRITE_UINT32( E7T_UARTx_BASE + E7T_UART_TXBUF, velo );
				Player_wait( E7T_UARTx_BASE );
				
				break;
			
			
			
			case MIDI_PGMCH:
				index		= ( ((int)outMsg) & 0x0000FF00) >>  8;

				// Send PGMCH+channel: STATUS BYTE - check if still valid
				if ( *running_status_byte != (0xC0+(channel-1)) ){

					// Update the running status byte
					*running_status_byte = 0xC0+(channel-1);
					
					// Send the status byte
					HAL_WRITE_UINT32( E7T_UARTx_BASE + E7T_UART_TXBUF, *running_status_byte);
					Player_wait( E7T_UARTx_BASE );
				}
				
				//Send program index
				HAL_WRITE_UINT32( E7T_UARTx_BASE + E7T_UART_TXBUF, index);
				Player_wait( E7T_UARTx_BASE );

				break;
				
				
				
			case MIDI_CC:
				controller	= ( ((int)outMsg) & 0x0000FF00) >>  8;
				value		= ( ((int)outMsg) & 0x000000FF) >>  0;
				
				// Send MIDICC+channel: STATUS BYTE - check if still valid
				if ( *running_status_byte != (0xB0+(channel-1)) ){

					// Update the running status byte
					*running_status_byte = 0xB0+(channel-1);
					
					// Send the status byte
					HAL_WRITE_UINT32( E7T_UARTx_BASE + E7T_UART_TXBUF, *running_status_byte );
					Player_wait( E7T_UARTx_BASE );
				}
				
				//Send controller number
				HAL_WRITE_UINT32( E7T_UARTx_BASE + E7T_UART_TXBUF, controller );
				Player_wait( E7T_UARTx_BASE );
				
				//Send value of controller
				HAL_WRITE_UINT32( E7T_UARTx_BASE + E7T_UART_TXBUF, value );
				Player_wait( E7T_UARTx_BASE );
				
				break;
				


			case MIDI_PRESSURE:
				// The last bytes may be ignored - should be 0 from earlier msg. assembly
				value		= ( ((int)outMsg) & 0x0000FF00) >>  8;

				// Send MIDICC+channel: STATUS BYTE - check if still valid
				if ( *running_status_byte != (0xD0+(channel-1)) ){

					// Update the running status byte
					*running_status_byte = 0xD0+(channel-1);
					
					// Send the status byte
					HAL_WRITE_UINT32( E7T_UARTx_BASE + E7T_UART_TXBUF, *running_status_byte );
					Player_wait( E7T_UARTx_BASE );
				}
				
				//Send the value number
				HAL_WRITE_UINT32( E7T_UARTx_BASE + E7T_UART_TXBUF, value );
				Player_wait( E7T_UARTx_BASE );

				break;



			case MIDI_BENDER:
				controller	= ( ((int)outMsg) & 0x0000FF00) >>  8;
				value		= ( ((int)outMsg) & 0x000000FF) >>  0;

				// Send MIDICC+channel: STATUS BYTE - check if still valid
				if ( *running_status_byte != (0xE0+(channel-1)) ){

					// Update the running status byte
					*running_status_byte = 0xE0+(channel-1);
					
					// Send the status byte
					HAL_WRITE_UINT32( E7T_UARTx_BASE + E7T_UART_TXBUF, *running_status_byte );
					Player_wait( E7T_UARTx_BASE );
				}
				
				//Send first 7 value bits (least significant) - the controller labeling is misleading here
				HAL_WRITE_UINT32( E7T_UARTx_BASE + E7T_UART_TXBUF, controller );
				Player_wait( E7T_UARTx_BASE );
				
				//Send second 7 value bits (most significant)
				HAL_WRITE_UINT32( E7T_UARTx_BASE + E7T_UART_TXBUF, value );
				Player_wait( E7T_UARTx_BASE );

				break;

			
				
			case MIDI_CLOCK:
				command = channel;		// First byte after type

				// Send clock command
				HAL_WRITE_UINT32( E7T_UARTx_BASE + E7T_UART_TXBUF, command );
				Player_wait( E7T_UARTx_BASE );
				
				break;
		} // switch( type )
	} // while( 1 )
	
