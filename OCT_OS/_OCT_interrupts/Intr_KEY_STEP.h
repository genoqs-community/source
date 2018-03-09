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





		switch( MODE_OBJECT_SELECTION ){
			
			case BIRDSEYE: 
				// If the zoom key is not pressed anymore, return to default
				if (	!(is_pressed_key( KEY_ZOOM_STEP ))
					){

					// Enter interactive mode
					MODE_OBJECT_SELECTION = INTERACTIVE;
				}
				else{
					// Nothing to do as of yet..
				}
				break;

			case CHORDEYE:
				// If no more chord button pressed anymore, return to default
				if ( is_pressed_keyrange( 252, 258 ) == 0
					){

					// Enter interactive mode
					MODE_OBJECT_SELECTION = INTERACTIVE;
				}

				break;
		}

		// STEP Mode button pressed
		if ( (is_pressed_key( KEY_ZOOM_STEP )) 
			){

			// Enter the Birds Eye view
			MODE_OBJECT_SELECTION = BIRDSEYE;
		}
		// Some chord button is pressed
		else if ( is_pressed_keyrange( 252, 258 ) != 0 
			){

			// Get the chord key pressed and compute the size
			temp = is_pressed_keyrange( 252, 258 );
	
			if (	( temp >= 252 )
				&&	( G_zoom_level == zoomSTEP )
				){

				// Set the size value into the step
				set_step_chord_size( 
					target_page->Step[target_page->stepSelectionSingleRow]
									 [target_page->stepSelectionSingleCol], 258 - temp );	
			}


			// Enter the CHORDEYE view
			MODE_OBJECT_SELECTION = CHORDEYE;					
		}




