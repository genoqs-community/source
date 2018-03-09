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




	switch( target_page->OPS_mode ){
				
		// Shows the page functions needed globally
		case BIRDSEYE:
		
//d_iag_printf( "viewing BIRDSEYE..\n" );
			// Show the page functions as they are visible in the grid

			// TRACK SELECTORS
			show (ELE_TRACK_SELECTORS, GRID_BANK_PLAYMODES);

			// TRACK MUTATORS
			show( ELE_TRACK_MUTATORS, PAGE_FUNCTIONS );
			// Caution!! There is some extra code in the ELE_MATRIX / GRID section.
			
			// OCTAVE CIRCLE
			show( ELE_OCTAVE_CIRCLE, PAGE_TRANSPOSITION );
			
			// EDIT INDICATOR
			show( ELE_EDIT_INDICATOR, RED );
		
			// MATRIX
			show( ELE_MATRIX, GRID );

			// Show the GRID cursor
			temp = cursor_to_dot( GRID_CURSOR );
			MIR_write_dot( temp, MIR_RED   );
			MIR_write_dot( temp, MIR_GREEN );
			MIR_write_dot( temp, MIR_BLINK );
			break;		


		// This is the general case	
		case INTERACTIVE:
		case PASSIVE:

			// SELECT MASTER 
			if (target_page->stepSelection  != 0) {
				// BLINK active for step selection
				show (	ELE_SELECT_MASTER, RED 		);
				show (	ELE_SELECT_MASTER, GREEN 	);
				show (	ELE_SELECT_MASTER, BLINK 	);
			}


//			// TRACK MUTATORS
//			if (target_page->stepSelection != 0) {
//				// Step rotarys shown
//				show (ELE_TRACK_MUTATORS, STEP_FUNCTIONS);
//			}

			
			// SELECTION MAP SELECTOR (Bottom line)
			MIR_write_dot( LED_MIXTGT_USR1, MIR_GREEN   );
			MIR_write_dot( LED_MIXTGT_USR2, MIR_GREEN   );
			MIR_write_dot( LED_MIXTGT_USR3, MIR_GREEN   );
			MIR_write_dot( LED_MIXTGT_USR4, MIR_GREEN   );

			switch( target_page->stepSELpattern_ndx ){
			
				case 0:	MIR_write_dot( LED_MIXTGT_USR1, MIR_RED 	); 
						MIR_write_dot( LED_MIXTGT_USR1, MIR_BLINK 	);
					break;
				case 1:	MIR_write_dot( LED_MIXTGT_USR2, MIR_RED 	); 
						MIR_write_dot( LED_MIXTGT_USR2, MIR_BLINK 	);
					break;
				case 2:	MIR_write_dot( LED_MIXTGT_USR3, MIR_RED 	); 
						MIR_write_dot( LED_MIXTGT_USR3, MIR_BLINK 	);
					break;
				case 3:	MIR_write_dot( LED_MIXTGT_USR4, MIR_RED 	); 
						MIR_write_dot( LED_MIXTGT_USR4, MIR_BLINK 	);
					break;
				case 4:	MIR_write_dot( LED_MIXTGT_USR5, MIR_RED 	); 
						MIR_write_dot( LED_MIXTGT_USR5, MIR_BLINK 	);
					break;
			}


			// EDIT INDICATOR
			if (target_page->stepSelection != 0) {

				// Active when a step is selected - show " ' "
				show( ELE_EDIT_INDICATOR, GREEN );

				if (EDIT_TIMER != OFF){
					show( ELE_EDIT_INDICATOR, BLINK );
				}					
			}

			break;
			
	} // Ops mode switch

	// Transport status
	show( ELE_GLOBALS, TRANSPORT );	


