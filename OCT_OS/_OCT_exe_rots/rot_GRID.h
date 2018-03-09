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




extern void rot_exec_PAGE_global( 	Pagestruct* target_page, 
									unsigned char rotNdx, 
									unsigned char direction );


void rot_exec_GRID( 	Pagestruct* target_page, 
						unsigned char rotNdx, 
						unsigned char direction ){
							

	unsigned char trackNdx = 0;
	
	// Act according to rot index
	switch( rotNdx ){
	
		// BIG KNOB
		case 0:
			// When not running on external clock
			// And no page selected in GRID
			if (	( G_clock_source != EXT )
				&&	( GRID_play_mode != GRID_EDIT )
				){
	
				if ( G_pause_bit == OFF ){
					modify_parameter( &G_master_tempo, MIN_TEMPO, MAX_TEMPO, direction, ON, FIXED);
					G_TIMER_REFILL_update();

				} else { // PMLS Mode
					modify_parameter( &G_measure_indicator_value, 0, 254, direction, ON, FIXED);
				}
			}
			break;

		
		// EDIT encoders
		case 1: case 2: case 3: case 4: case 5: 
		case 6: case 7: case 8: case 9: case 10:

			// Ensure the right mode for the encoder operation - i.e. page is selected
			if ( GRID_play_mode == GRID_EDIT ){

				// Apply global page modifications
				rot_exec_PAGE_global( target_page, rotNdx, direction );
			}
			break;
			

		// MIX encoders
		case 11: case 12: case 13: case 14: case 15: 
		case 16: case 17: case 18: case 19: case 20:

			// Ensure the right mode for operation
			if (( GRID_play_mode != GRID_CCMAP ) && ( GRID_play_mode != GRID_MIX )) break;
	
			// Restart MIX Timer
			// restart_MIX_TIMER();
								
			// Normalized index - milseading naming (legacy)
			trackNdx = rotNdx-11;
										
			// This applies to all other modes
			switch( GRID_assistant_page->CC_MIXMAP_attribute ) {
	
				case CC_MIXMAP_AMT:
					modify_parameter( 
							   	&GRID_assistant_page->CC_MIXMAP [GRID_assistant_page->mixTarget][trackNdx][CC_MIXMAP_AMT], 
								// 0, 127, direction, ON );
								CC_MIN_AMOUNT, CC_MAX_AMOUNT, direction, ON, FIXED);
	
					if ( GRID_assistant_page->CC_MIXMAP [GRID_assistant_page->mixTarget] [trackNdx] [CC_MIXMAP_MCC] != (unsigned char) MIDICC_NONE ){
						MIDI_send( 	MIDI_CC, 
								GRID_assistant_page->CC_MIXMAP [GRID_assistant_page->mixTarget] [trackNdx] [CC_MIXMAP_MCH],
								GRID_assistant_page->CC_MIXMAP [GRID_assistant_page->mixTarget] [trackNdx] [CC_MIXMAP_MCC],
								GRID_assistant_page->CC_MIXMAP [GRID_assistant_page->mixTarget] [trackNdx] [CC_MIXMAP_AMT]   );					
					}
					break;
	
				case CC_MIXMAP_MCC:
					modify_signed_parameter( 
							   	&GRID_assistant_page->CC_MIXMAP [GRID_assistant_page->mixTarget] [trackNdx] [CC_MIXMAP_MCC], 
								// 0, 127, direction, ON );
								CC_MIN_MCC, CC_MAX_MCC, direction, ON, FIXED);
					break;
	
				case CC_MIXMAP_MCH:
					modify_parameter( 
							   	&GRID_assistant_page->CC_MIXMAP [GRID_assistant_page->mixTarget] [trackNdx] [CC_MIXMAP_MCH], 
								// 1,  16, direction, ON );
								CC_MIN_MCH, CC_MAX_MCH, direction, ON, FIXED);
					break;
			} // switch on MIXMAP attribute

			break;
	}
}








