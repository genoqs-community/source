


		switch( DEVICE_STATE ){
		
			case DEFAULT:
			
				// ZOOM DEVICE MODE INDICATORS
				// Show all Mode buttons on
				MIR_write_dot (LED_ZOOM_GRID,		MIR_GREEN);
				MIR_write_dot (LED_ZOOM_GRID, 		MIR_RED);
				MIR_write_dot (LED_ZOOM_GRID, 		MIR_BLINK);			
				
				MIR_write_dot (LED_ZOOM_PAGE,		MIR_GREEN);
				MIR_write_dot (LED_ZOOM_PAGE, 		MIR_RED);
		
				MIR_write_dot (LED_ZOOM_TRACK,		MIR_GREEN);
				MIR_write_dot (LED_ZOOM_TRACK, 		MIR_RED);
		
				MIR_write_dot (LED_ZOOM_STEP,		MIR_GREEN);
				MIR_write_dot (LED_ZOOM_STEP, 		MIR_RED);
		
				MIR_write_dot (LED_ZOOM_MAP,		MIR_GREEN);
				MIR_write_dot (LED_ZOOM_MAP, 		MIR_RED);
		
				MIR_write_dot (LED_ZOOM_PLAY,		MIR_GREEN);
				MIR_write_dot (LED_ZOOM_PLAY, 		MIR_RED);
		
				// Show the RETURN key
				MIR_write_dot( LED_RETURN,			MIR_GREEN);
		
				// Show the Software Version
				MIR_write_numeric_C(
					SW_VERSION_MAJOR*100 + SW_VERSION_MINOR*10 + SW_VERSION_RELEASE );		
				MIR_write_numeric_H( SW_VERSION_INTERNAL, 9 );
				break;
				
				
			case UPLOAD_IN_PROGRESS:
			
				// Indicate upload in progress
				MIR_write_dot(	LED_PROGRAM, MIR_GREEN 	);
				MIR_write_dot(	LED_PROGRAM, MIR_RED 	);				
				MIR_write_dot( 	LED_PROGRAM, MIR_BLINK 	);

				// Show status		
				switch( sysex_header ){
				
					case 0x00206400 + _OS_SYSEX_DUMP:
						temp = OSFILE_SIZE;
						color = MIR_GREEN;
						break;

					case 0x00206400 + _CONFIGFILE_SYSEX_DUMP:
						temp = CONFIGFILE_SIZE;
						color = MIR_RED;
						break;
				}

				j = (2 * temp) / 160;	
				for ( i=0; i < ( sysex_byte_count / j ); i++ ){

					MIR_write_dot( cursor_to_dot( i ), color );
				}
				break;
		
		
			case UPLOAD_COMPLETE:

				// Show status		
				switch( sysex_header ){
				
					case 0x00206400 + _OS_SYSEX_DUMP:
						temp = OSFILE_SIZE;
						color = MIR_GREEN;
						break;

					case 0x00206400 + _CONFIGFILE_SYSEX_DUMP:
						temp = CONFIGFILE_SIZE;
						color = MIR_RED;
						break;
				}
				
				
				// Indicate flash programming - having received correct number of bytes
				// if ( sysex_byte_count == (temp*2 + 7) ){
					
					MIR_write_dot(	LED_PROGRAM, MIR_RED );
					MIR_write_dot( 	LED_PROGRAM, MIR_BLINK );
				// }
				// else{
				//
				//	MIR_write_dot(	LED_PROGRAM, MIR_RED );
				//	MIR_write_dot(	LED_PROGRAM, MIR_GREEN );
				//	MIR_write_dot( 	LED_PROGRAM, MIR_BLINK );				
				// }

				// ..or EXIT
				MIR_write_dot( LED_RETURN,	MIR_GREEN);
				break;
		}
		

