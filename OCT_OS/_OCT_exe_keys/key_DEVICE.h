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




		switch( DEVICE_STATE ){


			// The hidden dump commands..
			case DEFAULT:


				// EXECUTABLE DUMP: the executable currently stored in flash
				if ( keyNdx == KEY_MIX_MASTER ){
					// Specialized OS dump function
					my_OS_dump();
				}



				// D O U B L E - C L I C K  C O N S T R U C T
				// DOUBLE CLICK SCENARIO
				if (	( DOUBLE_CLICK_TARGET == keyNdx )
					&& 	( DOUBLE_CLICK_TIMER   > DOUBLE_CLICK_ALARM_SENSITIVITY ) ) {

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
					if ( keyNdx == KEY_SCALE_MYSEL ){
						G_MIDI_B_priority ^= 1; // toggle
						Flash_write_grid();
					}
					else if ( keyNdx == KEY_ZOOM_GRID ){
						G_initZoom ^= 1; // toggle
						Flash_write_grid();
					}

					unsigned char latencyOffset = 0;
					switch (keyNdx) {
						case KEY_MIXTGT_USR1:
							latencyOffset = 1; // toggle 1
							break;
						case KEY_MIXTGT_USR2:
							latencyOffset = 2;// toggle 2
							break;
						case KEY_MIXTGT_USR3:
							latencyOffset = 3;// toggle 3
							break;
						case KEY_MIXTGT_USR4:
							latencyOffset = 4;// toggle 4
							break;
						case KEY_MIXTGT_USR5:
							latencyOffset = 5;// toggle 5
							break;
						default:
						break;
					}
					if ( latencyOffset > 0 ){
						if ( G_TT_external_latency_offset == latencyOffset ){
							G_TT_external_latency_offset = OFF;
						}
						else {
							G_TT_external_latency_offset = latencyOffset;
						}
						Flash_write_grid();
					}
				}

				// DIAG MODE entry
				if ( keyNdx == 1 ){

					// D O U B L E - C L I C K  C O N S T R U C T
					// DOUBLE CLICK SCENARIO
					if (	( DOUBLE_CLICK_TARGET == keyNdx )
						&& 	( DOUBLE_CLICK_TIMER   > DOUBLE_CLICK_ALARM_SENSITIVITY ) ) {

						// Double click code
						// ...
						G_zoom_level = zoomDIAG;
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
				// Store the content of G_sysex_stream_keeper to flash
				if ( keyNdx == KEY_PROGRAM ){

					switch( G_sysex_stream_keeper[ 3 ] ){

						// OSFILE UPDATE WAS RECEIVED
						case def_OS_SYSEX_DUMP:

							// Move the stream keeper pointer beyond the sysex head
//							G_sysex_stream_keeper = &G_sysex_stream_keeper[4];

							// Write the OS file to flash
							my_OS_program_to_flash( &G_sysex_stream_keeper[4] );
							break;

						// CONFIGFILE UPDATE RECEIVED
						case def_CONFIGFILE_SYSEX_DUMP:

							// Move the stream keeper pointer beyond the sysex head
//							G_sysex_stream_keeper = &G_sysex_stream_keeper[4];

							// Write configfile to flash
							my_configfile_program_to_flash( &G_sysex_stream_keeper[4] );
							break;
					}

					// Trigger the global load and save mode
					G_zoom_level = zoomDEVICE;

					// Forget about the sysex dump!
					G_sysex_byte_count = 0;

					// Bring the device back to default state
					DEVICE_STATE = DEFAULT;
				}


				// EXIT
				// Refresh the memory
				if (keyNdx == KEY_RETURN) {

					// Init the stream keeper array
					switch( G_sysex_header ){

						// OSFILE UPDATE WAS RECEIVED
						case 0x00206400 + def_OS_SYSEX_DUMP:
							memset( G_sysex_stream_keeper, 0, def_OSFILE_SIZE );
							break;

						// CONFIGFILE UPDATE RECEIVED
						case 0x00206400 + def_CONFIGFILE_SYSEX_DUMP:
							memset( G_sysex_stream_keeper, 0, def_CONFIGFILE_SIZE );
							break;
					}

					// Trigger the global load and save mode
					G_zoom_level = zoomDEVICE;
					// Forget about the sysex dump!
					G_sysex_byte_count = 0;

					// Bring the device back to default state
					DEVICE_STATE = DEFAULT;
				}
				break;
		}





