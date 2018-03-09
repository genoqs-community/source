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




	// MATRIX - Check for pressed matrix keys 
	my_pressed_key = is_pressed_pagerange();
	
	// Button pressed in range of virtual selectors
	if (	(	( my_pressed_key > 0  ) 
			&&	( my_pressed_key < 48 ) )
		){

		// This is the traditional way of select and operate
		GRIDTRACK_OPS_mode = PASSIVE;		
		// d_iag_printf( "Intr_KEY_GRIDTRACK PASSIVE\n" );
		break;

	}
	else{

		// Remember selections
		if ( is_pressed_key( KEY_SELECT_MASTER ) ){	

			GRIDTRACK_OPS_mode = PASSIVE;

			// Remove all track selections in the pages but store them
			for ( row=0; row < GRID_NROF_BANKS;  row++ ){
		
				if ( GRID_p_selection[row] != NULL ){
			
					GRID_p_selection[row]->trackSelectionStored = 
						GRID_p_selection[row]->trackSelection;
				}
			} // row iterator
		}
		else{
			
			// Remove all track selections in the pages
			for ( row=0; row < GRID_NROF_BANKS;  row++ ){
		
				if ( GRID_p_selection[row] != NULL ){
			
					GRID_p_selection[row]->trackSelection = 0;
				}
			}

			GRIDTRACK_OPS_mode = INTERACTIVE;
		}
		// d_iag_printf( "Intr_KEY_GRIDTRACK switch to INTERACTIVE\n" );
	}




