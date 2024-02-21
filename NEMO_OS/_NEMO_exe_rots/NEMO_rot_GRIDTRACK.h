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





void rot_exec_GRIDTRACK( 	Pagestruct* target_page, 
							unsigned char rotNdx, 
							unsigned char direction ){

	unsigned char i = 0;

	// BIG KNOB
	if ( rotNdx == 0 ){
	
		// When not running on external clock
		if ( G_clock_source != EXT ){

			modify_parameter( &G_master_tempo, MIN_TEMPO, MAX_TEMPO, direction, ON, FIXED);
			G_TIMER_REFILL_update();
		}				
	}


	// EDITOR
	if ((rotNdx >= 1) && (rotNdx <= 10)) {

		// Iterate through the active banks
		for ( i=0; i < GRID_NROF_BANKS; i++ ){

			// Make sure there is a page playing in the bank pressed
			if ( GRID_p_selection[i] != NULL ){

				// Apply rotary to the selected tracks
				rot_exe_EDIT( rotNdx, direction, GRID_p_selection[i] );

			} // target page is not NULL
		} // Bank iterator		
	}

	// Ensure the right mode for operation
	if ( ( rotNdx >= 11 ) && ( rotNdx <= 20 ) ) {
		// Send the corresponding CC data!
		rot_exe_MIX( rotNdx, direction, GRID_assistant_page );
	}
}



