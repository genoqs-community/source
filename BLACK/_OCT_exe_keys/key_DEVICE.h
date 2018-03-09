

		switch( SOFTWARE_UPDATE_RECEIVED ){

			case TRUE:

				// PROGRAM KEY
				// Store the content of code_keeper to flash
				if ( keyNdx == KEY_PROGRAM ){
					
					my_executable_program_to_flash();
				}

				if (keyNdx == KEY_RETURN) {
					
					for ( temp=0; temp < CODE_KEEPER_SIZE; temp++ ){
						code_keeper[temp] = 0;
					}

					// Trigger the global load and save mode
					zoom_level = zoomDEVICE;
					// Forget about the sysex dump!
					sysex_byte_count = 0;
					SOFTWARE_UPDATE_RECEIVED = FALSE;
				}
				break;
			

			case FALSE:
			
				// EXECUTABLE DUMP: the executable currently stored in flash
				if ( keyNdx == KEY_MIX_MASTER ){
		
					// Dumps the executable from flash as SYSEX data 
					my_executable_dump( CODE_KEEPER_SIZE );
				}

				// SELECT MASTER										
				if ( keyNdx == KEY_SELECT_MASTER ){
		
					// read_my_flash();
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


				// Software update received switch break		
				break;
		}






