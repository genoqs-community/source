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

				#ifdef CE_OS_ADDON_BUILD
				MIR_write_dot( LED_EDIT_MASTER,		MIR_SHINE_GREEN);
				#endif

				// Show the Software Version
				MIR_write_tempo( 
					SW_VERSION_MAJOR*100 + SW_VERSION_MINOR*10 + SW_VERSION_RELEASE, 0 );
				MIR_write_numeric_H( SW_VERSION_INTERNAL, 1 );

				break;
				
				
			case UPLOAD_IN_PROGRESS:

				// Show status		
				switch( G_sysex_header ){
				
					case 0x00206400 + def_OS_SYSEX_DUMP:
						// Indicate upload in progress -> GREEN!
						MIR_write_dot(	LED_PROGRAM, MIR_GREEN 	);
						MIR_write_dot( 	LED_PROGRAM, MIR_BLINK 	);

						temp = def_OSFILE_SIZE;
						color = MIR_GREEN;
						break;

					case 0x00206400 + def_CONFIGFILE_SYSEX_DUMP:
						// Indicate upload in progress -> RED!
						MIR_write_dot(	LED_PROGRAM, MIR_RED 	);				
						MIR_write_dot( 	LED_PROGRAM, MIR_BLINK 	);

						temp = def_CONFIGFILE_SIZE;
						color = MIR_RED;
						break;
				}

				j = (2 * temp) / 64;	
				for ( i=0; i < ( G_sysex_byte_count / j ); i++ ){

 					MIR_write_dot( NEMO_cursor_to_dot( i ), color );
				}
				break;
		
		
			case UPLOAD_COMPLETE:

				// Show status		
				switch( G_sysex_header ){
				
					case 0x00206400 + def_OS_SYSEX_DUMP:
						temp = def_OSFILE_SIZE;
						color = MIR_GREEN;
						break;

					case 0x00206400 + def_CONFIGFILE_SYSEX_DUMP:
						temp = def_CONFIGFILE_SIZE;
						color = MIR_RED;
						break;
				}

				// Indicate flash programming - having received correct number of bytes
				MIR_write_dot(	LED_PROGRAM, MIR_RED );
				MIR_write_dot( 	LED_PROGRAM, MIR_BLINK );

				// ..or EXIT
				MIR_write_dot( LED_RETURN,	MIR_GREEN);
				break;
		}
		

