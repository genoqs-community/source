
	// This is the content of the UART0_OUT and UART1_OUT threads 


	int* 			outMsg=0;
	unsigned char 	type=0, 
					channel=0, 
					command=0, 
					pitch=0, 
					velo=0, 
					value=0, 
					controller=0;
	
	while (1) {
		
		// Read in the next MIDI OUT message
		outMsg = cyg_mbox_get( UARTx_OUT_mbox_handle );
			
		// Determine message type and channel
		type 		= ( ((int)outMsg) & 0xFF000000) >>  24;
		channel 	= ( ((int)outMsg) & 0x00FF0000) >> 	16;

		// Show what is in the MIDI pipe
		// d_iag_printf( "\nUARTx_OUT_Msg:0x%x ty:0x%x ch:0x%x\n", (int)outMsg, type, channel );

		switch( type ){


			case MIDI_NOTE:		
				pitch		= ( ((int)outMsg) & 0x0000FF00) >>  8;
				velo		= ( ((int)outMsg) & 0x000000FF) >>  0;
			
				// Differentiate between MIDI NOTE ON and MIDI NOTE OFF
				switch( velo ){
	
					case 0:
						// Transform this to a NOTE OFF message
						if ( *running_status_byte != (0x80+(channel-1)) ){
		
							// Update the running status byte
							*running_status_byte = 0x80+(channel-1);
							
							// Send the status byte
							HAL_WRITE_UINT32( E7T_UARTx_BASE + E7T_UART_TXBUF, *running_status_byte);
							Player_wait( E7T_UARTx_BASE );
							
							// Default OFF velocity for NOTE OFF
							velo = 64;
						}						
						break;
					
					default:
						// d_iag_printf( "**** NOTE_OUT: ch:%d pi:%d ve:%d TTC:%d\n", 
						//	channel, pitch, velo, TTC_ABS_VALUE );	
		
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
					
				//Send the velocity
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
				
				//Send first 7 bits of the value - the controller labeling is misleading here
				HAL_WRITE_UINT32( E7T_UARTx_BASE + E7T_UART_TXBUF, controller );
				Player_wait( E7T_UARTx_BASE );
				
				//Send second 7 bits of the value number
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
	
