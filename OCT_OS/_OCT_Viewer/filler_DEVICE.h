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
				if ( G_DEVICE_dirty == TRUE ) {
					MIR_write_dot (LED_PROGRAM, 	MIR_RED);
					MIR_write_dot (LED_PROGRAM, 	MIR_BLINK);
				}

				if ( G_MIDI_B_priority ) {
					MIR_write_dot (LED_SCALE_MYSEL,		MIR_RED);
				} else {
					MIR_write_dot (LED_SCALE_MYSEL,		MIR_GREEN);
				}

				if ( G_initZoom ) {
					MIR_write_dot (LED_ZOOM_GRID,		MIR_GREEN);
				} else {
					MIR_write_dot (LED_ZOOM_GRID, 		MIR_RED);
				}

				/*
				// Anti-Echo
				if ( G_midi_map_controller_mode == ON ){
					MIR_write_dot (LED_ZOOM_MAP, MIR_RED);
					MIR_write_dot (LED_ZOOM_MAP, MIR_BLINK);
				}

				// Show the RETURN key
				MIR_write_dot( LED_RETURN,			MIR_GREEN);

				switch (G_TT_external_latency_offset) {
					case 1:
						MIR_write_dot( LED_MIXTGT_USR1,	MIR_RED);
						break;
					case 2:
						MIR_write_dot( LED_MIXTGT_USR2,	MIR_RED);
						break;
					case 3:
						MIR_write_dot( LED_MIXTGT_USR3, MIR_RED);
						break;
					case 4:
						MIR_write_dot( LED_MIXTGT_USR4,	MIR_RED);
						break;
					case 5:
						MIR_write_dot( LED_MIXTGT_USR5,	MIR_RED);
						break;
					default:
						break;
				}
*/
				#ifdef CE_OS_ADDON_BUILD
				MIR_write_dot( LED_EDIT_MASTER,		MIR_SHINE_GREEN);
				#endif

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
				switch( G_sysex_stream_keeper[ 3 ] ){
				
					case def_OS_SYSEX_DUMP:
						temp = def_OSFILE_SIZE;
						color = MIR_GREEN;
						break;

					case def_CONFIGFILE_SYSEX_DUMP:
						temp = def_CONFIGFILE_SIZE;
						color = MIR_RED;
						break;
				}

				j = (2 * temp) / 160;	
				for ( i=0; i < ( G_sysex_byte_count / j ); i++ ){

					MIR_write_dot( cursor_to_dot( i ), color );
				}
				break;
		
		
			case UPLOAD_COMPLETE:

				// Show status		
				switch( G_sysex_stream_keeper[ 3 ] ){
				
					case def_OS_SYSEX_DUMP:
						temp = def_OSFILE_SIZE;
						color = MIR_GREEN;
						break;

					case def_CONFIGFILE_SYSEX_DUMP:
						temp = def_CONFIGFILE_SIZE;
						color = MIR_RED;
						break;
				}
				
				
				// Indicate flash programming - having received correct number of bytes
				// if ( G_sysex_byte_count == (temp*2 + 7) ){
					
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
		

