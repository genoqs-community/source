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



		// ROW I
		// Value of the selected track attribute in the bottom line - and offset calculation for line II
		switch( NEMO_selectedTrackAttribute ){
			
			case NEMO_ATTR_VELOCITY:		
				MIR_write_numeric_H( 	target_page->Track[row]->attr_VEL
									#ifdef EVENTS_ABSOLUTE
									+	target_page->Track[row]->event_offset[NEMO_ATTR_VELOCITY]
									#endif // EVENTS_ABSOLUTE
									, 	0);
				j = 1;	
				break;


			case NEMO_ATTR_PITCH:		
				MIR_write_pitch_H( target_page->Track[row]->attr_PIT
									#ifdef EVENTS_ABSOLUTE
									+ target_page->Track[row]->event_offset[NEMO_ATTR_PITCH]
									#endif // EVENTS_ABSOLUTE
									, 	0);
				j = 0;
				break;


			case NEMO_ATTR_LENGTH:		
				MIR_fill_numeric 	(	1, 
										target_page->Track[row]->LEN_factor
										+ target_page->Track[row]->event_offset[NEMO_ATTR_LENGTH], 
										0,	MIR_GREEN);
				MIR_fill_numeric 	(	1, 
										target_page->Track[row]->LEN_factor
										+ target_page->Track[row]->event_offset[NEMO_ATTR_LENGTH], 
										0,	MIR_RED);
				j = 2;
				break;


			case NEMO_ATTR_START:		
				MIR_fill_numeric 	(	1, 
										target_page->Track[row]->STA_factor
										+ target_page->Track[row]->event_offset[NEMO_ATTR_START], 
										0,	MIR_RED);
				MIR_fill_numeric 	(	1, 
										target_page->Track[row]->STA_factor
										+ target_page->Track[row]->event_offset[NEMO_ATTR_START], 
										0,	MIR_GREEN);
				j = 3;
				break;


			case NEMO_ATTR_POSITION:		
				MIR_point_numeric 	(	target_page->Track[row]->attr_POS + 0
										+ target_page->Track[row]->event_offset[NEMO_ATTR_POSITION], 	0, 	MIR_GREEN );

				MIR_point_numeric 	(	target_page->Track[row]->attr_POS + 0
										+ target_page->Track[row]->event_offset[NEMO_ATTR_POSITION], 	0, 	MIR_RED );
				j = 11;
				break;


			case NEMO_ATTR_DIRECTION:	
				MIR_point_numeric 	(	target_page->Track[row]->attr_DIR
										+ target_page->Track[row]->event_offset[NEMO_ATTR_DIRECTION], 	0, 	MIR_RED );
				MIR_point_numeric 	(	target_page->Track[row]->attr_DIR 	
										+ target_page->Track[row]->event_offset[NEMO_ATTR_DIRECTION],	0, 	MIR_GREEN);
				j = 12;
				break;


			case NEMO_ATTR_AMOUNT:		
				MIR_write_numeric_H (	target_page->Track[row]->attr_AMT
										+ target_page->Track[row]->event_offset[NEMO_ATTR_AMOUNT]
										, 0 );
				j = 5;
				break;
	

			case NEMO_ATTR_GROOVE:		
				MIR_fill_numeric 	(	1, target_page->Track[row]->attr_GRV , 	0,	MIR_RED);
				MIR_fill_numeric 	(	1, target_page->Track[row]->attr_GRV , 	0,	MIR_GREEN);
				j = 6;
				break;


			case NEMO_ATTR_MIDICC:		
				if ( (	target_page->Track[row]->attr_MCC ) == MIDICC_NONE ){
		
					// Show the empty flag
					MIR_write_trackpattern ( 0x0f, 0, MIR_GREEN);
				}
				else if ( (	target_page->Track[row]->attr_MCC ) == MIDICC_BENDER ){
				
					// Signal the bender as selected
					MIR_write_trackpattern ( 0x01, 0, MIR_RED);	
				}
				else if ( (	target_page->Track[row]->attr_MCC ) == MIDICC_PRESSURE ){
				
					// Signal the pressure as selected
					MIR_write_trackpattern ( 0x03, 0, MIR_RED);	
				}
				else {
					// MIDICC has a valid value 
					MIR_write_numeric_H (	target_page->Track[row]->attr_MCC, 	0 );
				}
				j = 7;
				break;


			case NEMO_ATTR_MIDICH:
				j = target_page->Track[row]->attr_MCH 
					+ target_page->Track[row]->event_offset[NEMO_ATTR_MIDICH];
		
				if ( j <= 16 ){
					MIR_point_numeric(	j, 	0,	MIR_GREEN );
				} 
				else if ( j <= 32 ){
					MIR_point_numeric(	j-16, 0,	MIR_RED );
				}
				else if ( j <= 48 ){
					MIR_point_numeric(	j-32, 0,	MIR_GREEN );
					MIR_point_numeric(	j-32, 0,	MIR_BLINK );
				}
				else if ( j <= 64 ){
					MIR_point_numeric(	j-48, 0,	MIR_RED );
					MIR_point_numeric(	j-48, 0,	MIR_BLINK );
				}
				j = 9;
				break;


			case ATTR_CLOCK:

				// Determine color of the display: red for multiplier, green for divisor
				if ( target_page->Track[row]->attr_TEMPOMUL_SKIP == 0 ){
					// Triplet divisor acting up again
					if ( target_page->Track[row]->attr_TEMPOMUL == 25 ){
						MIR_point_numeric( 15, NEMO_ROW_I, MIR_GREEN );
					}
					else{
						// Multiplier indicator, no divisor active
						MIR_point_numeric( target_page->Track[row]->attr_TEMPOMUL, NEMO_ROW_I, MIR_RED );
					}
				}
				else{
					// Divisor indicator
					MIR_point_numeric( target_page->Track[row]->attr_TEMPOMUL_SKIP + 1, NEMO_ROW_I, MIR_GREEN );
				}

				// Input for ROW II
				j = 15;
				break;

			case NEMO_ATTR_PGMCH:		
				if ( target_page->Track[row]->program_change == 0 ){
					// Show the empty flag
					MIR_write_trackpattern ( 0x0f, 0, MIR_GREEN);
				}
				else{
					MIR_write_numeric_H (	target_page->Track[row]->program_change, 0 );
				}
				j = 13;
				break;
		}

