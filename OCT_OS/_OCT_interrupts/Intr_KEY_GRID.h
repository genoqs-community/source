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





		// Continue only when in interactive mode.
		if (	( GRID_OPS_mode == PASSIVE )
			){
			break;
		}

		// MATRIX - Check for pressed matrix keys 
		my_pressed_page = is_pressed_pagerange();
		
		switch( my_pressed_page ){
	
			// These keys don't belong to the page set, are just in the range.
			case 0:

			// Row zero
			case 20:	case 31:	case 42:	case 53:
			case 64:	case 75:	case 86:	case 97:
			case 108:	case 119:	case 130:	case 141:
			case 152:	case 163:	case 174:	case 185:

			// Bottom row
			case 21: 	case 32: 	case 43: 	case 54: 
			case 65: 	case 76:	case 87:  	case 164: 	
			case 175:
				
				// ..no relevant key is pressed, GRID is in MIX mode
				GRID_play_mode = GRID_MIX;
				break;				

			// CCMAP keys
			case 98: 	case 109: 	case 120: 	case 131: 	
			case 142:	case 153:
				break;

			// PAGE BUTTONS: Relevant key is pressed and is a valid page
			default:

				// Check whether a mutator key is pressed as well - 
				// ..in that case operate on the pressed page straight away.
				my_pressed_mutator = is_pressed_keyrange( 187, 196 );
				
				// Make sure the pressed key is only executed once
				if ( (	GRID_play_mode != GRID_EDIT )
					){
					
					switch( my_pressed_mutator ){

						case 0:
							// Nothing to do.. 
							break;
		
						case KEY_TGGL:
							cyg_mbox_tryput ( keyNdx_mbox_handle, (void*) KEY_TGGL );	
							break;

						case KEY_CLEAR:
							cyg_mbox_tryput ( keyNdx_mbox_handle, (void*) KEY_CLEAR );
							break;

						case KEY_SOLO:
							cyg_mbox_tryput ( keyNdx_mbox_handle, (void*) KEY_SOLO );
							break;

						case KEY_RANDOMIZE:
							cyg_mbox_tryput ( keyNdx_mbox_handle, (void*) KEY_RANDOMIZE );
							break;

						case KEY_ZOOM:
							cyg_mbox_tryput ( keyNdx_mbox_handle, (void*) KEY_ZOOM );
							break;
					}
				}
				
				// Leave room for the page toggle mode..
				if ( GRID_editmode == ON ){

					// Switch to (or just keep) GRID edit mode
					GRID_play_mode = GRID_EDIT;
				}

				break;

		} // switch( my_pressed_key )


		switch( MODE_OBJECT_SELECTION ){
			case BIRDSEYE: 
				// If the zoom key is not pressed anymore, return to default
				if (	!(is_pressed_key( KEY_ZOOM_GRID ))
					){

					// Enter interactive mode
					MODE_OBJECT_SELECTION = INTERACTIVE;
				}
				else{
					// Nothing to do as of yet..
				}
				break;
		}

		if ( (is_pressed_key( KEY_ZOOM_GRID )) 
			){

			// Enter the Birds Eye view
			MODE_OBJECT_SELECTION = BIRDSEYE;
		}


