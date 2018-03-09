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
				// Show the VEL_factor of the current track
				MIR_fill_numeric( 		1,
										target_page->Track[row]->VEL_factor
										#ifdef EVENTS_FACTORIZED						
										+ target_page->Track[row]->event_offset[NEMO_ATTR_VELOCITY] 
										#endif // EVENTS_FACTORIZED
										, 0, MIR_GREEN );
				MIR_fill_numeric( 		1,
										target_page->Track[row]->VEL_factor
										#ifdef EVENTS_FACTORIZED						
										+ target_page->Track[row]->event_offset[NEMO_ATTR_VELOCITY] 
										#endif // EVENTS_FACTORIZED
										, 0, MIR_RED );
				j = 1;	
				break;


			case NEMO_ATTR_PITCH:		
				// Show the PIT_factor of the current track
				MIR_fill_numeric(		1, 
										target_page->Track[row]->PIT_factor
										#ifdef EVENTS_FACTORIZED
										+ target_page->Track[row]->event_offset[NEMO_ATTR_PITCH] 
										#endif // EVENTS_FACTORIZED
										, 0, MIR_GREEN );
				// Show the PIT_factor of the current track
				MIR_fill_numeric(		1, 
										target_page->Track[row]->PIT_factor
										#ifdef EVENTS_FACTORIZED
										+ target_page->Track[row]->event_offset[NEMO_ATTR_PITCH] 
										#endif // EVENTS_FACTORIZED
										, 0, MIR_RED );
	
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


			case NEMO_ATTR_AMOUNT:		
				// Show the AMT_factor of the current track
				MIR_fill_numeric	(	1,  
										target_page->Track[row]->AMT_factor
										+ target_page->Track[row]->event_offset[NEMO_ATTR_AMOUNT] 
										, 0, MIR_GREEN );
				MIR_fill_numeric	(	1,  
										target_page->Track[row]->AMT_factor
										+ target_page->Track[row]->event_offset[NEMO_ATTR_AMOUNT] 
										, 0, MIR_RED );
				j = 5;
				break;
	

			case NEMO_ATTR_GROOVE:
				// Show the GRV_factor of the current track		
				MIR_fill_numeric	( 	1,
										target_page->Track[row]->GRV_factor
										+ target_page->Track[row]->event_offset[ATTR_GROOVE]
										, 0, MIR_GREEN );
				MIR_fill_numeric	( 	1,
										target_page->Track[row]->GRV_factor
										+ target_page->Track[row]->event_offset[ATTR_GROOVE]
										, 0, MIR_RED );


				j = 6;
				break;


			case NEMO_ATTR_MIDICC:		
				// Show the MCC_factor of the current track
				MIR_fill_numeric	(	1,
										target_page->Track[row]->MCC_factor
										+ target_page->Track[row]->event_offset[ATTR_MIDICC] 
										, 0, MIR_GREEN );
				MIR_fill_numeric	(	1,
										target_page->Track[row]->MCC_factor
										+ target_page->Track[row]->event_offset[ATTR_MIDICC] 
										, 0, MIR_RED );
				j = 7;
				break;

		}

