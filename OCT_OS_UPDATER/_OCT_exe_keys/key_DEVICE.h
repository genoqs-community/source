
		switch( DEVICE_STATE ){

		
			// The hidden dump commands..
			case DEFAULT:


				// EXECUTABLE DUMP: the executable currently stored in flash
				if ( keyNdx == KEY_MIX_MASTER ){
		
					// Dumps the executable from flash as SYSEX data 
					my_flash_content_dump( _OS_SYSEX_DUMP );											
				}

				// REDBOOT CONFIG DUMP: the configuration segment for redboot
				if ( keyNdx == KEY_SELECT_MASTER ){
		
					my_flash_content_dump( _CONFIGFILE_SYSEX_DUMP );
				}
				

				// DIAG MODE entry
				if ( keyNdx == 1 ){

					// D O U B L E - C L I C K  C O N S T R U C T
					// DOUBLE CLICK SCENARIO
					if (	( DOUBLE_CLICK_TARGET == keyNdx ) 
						&& 	( DOUBLE_CLICK_TIMER   > DOUBLE_CLICK_ALARM_SENSITIVITY ) ) {
			
						// Double click code
						// ...
						zoom_level = zoomDIAG;
						// Note: There is no exit from DIAG mode other than reset
					
					} // end of double click scenario 
					
			
					// SINGLE CLICK SCENARIO
					else if (DOUBLE_CLICK_TARGET == 0) {
						
							DOUBLE_CLICK_TARGET = keyNdx;
							DOUBLE_CLICK_TIMER = ON;
							// Start the Double click Alarm
							cyg_alarm_initialize(	
									doubleClickAlarm_hdl, 
									cyg_current_time() + DOUBLE_CLICK_ALARM_TIME,
									DOUBLE_CLICK_ALARM_TIME );
			
						// Single click code
						// ...			
					}
				} // Key to enter DIAG Mode

				break;



			case UPLOAD_COMPLETE:

				// STORE
				// Store the content of sysex_stream_keeper to flash
				if ( keyNdx == KEY_PROGRAM ){

					switch( sysex_header ){
					
						// OSFILE UPDATE WAS RECEIVED
						case 0x00206400 + _OS_SYSEX_DUMP:

							// Write the OS file to flash
							my_executable_program_to_flash();
							break;

						// CONFIGFILE UPDATE RECEIVED 
						case 0x00206400 + _CONFIGFILE_SYSEX_DUMP:

							// Write configfile to flash
							my_configfile_program_to_flash();							
							break;
					}

					// Trigger the global load and save mode
					zoom_level = zoomDEVICE;
					// Forget about the sysex dump!
					sysex_byte_count = 0;

					// Bring the device back to default state
					DEVICE_STATE = DEFAULT;				
				}				


				// EXIT
				// Refresh the memory
				if (keyNdx == KEY_RETURN) {

					// Init the stream keeper array
					switch( sysex_header ){

						// OSFILE UPDATE WAS RECEIVED
						case 0x00206400 + _OS_SYSEX_DUMP:

							memset( sysex_stream_keeper, 0, OSFILE_SIZE );
							break;

						// CONFIGFILE UPDATE RECEIVED 
						case 0x00206400 + _CONFIGFILE_SYSEX_DUMP:

							memset( sysex_stream_keeper, 0, CONFIGFILE_SIZE );
							break;
					}
					
					// Trigger the global load and save mode
					zoom_level = zoomDEVICE;
					// Forget about the sysex dump!
					sysex_byte_count = 0;

					// Bring the device back to default state
					DEVICE_STATE = DEFAULT;	
				}
				break;
		}





