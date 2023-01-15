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




	// Page selectors, act as always..
	// ..select the page for play in preselection or zoom into it (doubleclick)

		// Determine coordinates of the key index
		row =  row_of( keyNdx );
		col =  column_of( keyNdx );

		// D O U B L E - C L I C K  C O N S T R U C T
		// DOUBLE CLICK SCENARIO
		if (	( DOUBLE_CLICK_TARGET == keyNdx )
			&& 	( DOUBLE_CLICK_TIMER   > DOUBLE_CLICK_ALARM_SENSITIVITY ) ) {

			// Double click code
			// ...
			if ( GRIDTRACK_OPS_mode == PASSIVE ){
				break;
			}


			// Repeated Single click code - to neutralize previous press
			// Do not react on invalid presses
			if (	( col > 12 )
				||	( col <  3 )
				||	( GRID_p_selection[ row ] == NULL )
				){
				return;
			}

			// ..true Double click code:
			// Select the VIEWER page pertaining to the track.
			target_page = &Page_repository[ GRID_p_selection[ row ]->pageNdx ];
			// Move the cursor for the grid
			GRID_CURSOR = target_page->pageNdx;

			// Put the track into the track selection
			target_page->trackSelection ^= mirror( 1 << (col-3), 10 );

			// Zoom into the target_page (implicitly) and the track there
			G_zoom_level = zoomTRACK;

		} // end of double click scenario


		// SINGLE CLICK SCENARIO
		else // if (DOUBLE_CLICK_TARGET == 0)
			{
				DOUBLE_CLICK_TARGET = keyNdx;
				DOUBLE_CLICK_TIMER = ON;
				// Start the Double click Alarm
				cyg_alarm_initialize(
						doubleClickAlarm_hdl,
						cyg_current_time() + DOUBLE_CLICK_ALARM_TIME,
						DOUBLE_CLICK_ALARM_TIME );

			// Single click code
			// ...


			// Do not react on invalid presses - outside matrix or inactive pages
			if (	( col > 12 )
				||	( col <  3 )
				||	( GRID_p_selection[ row ] == NULL )
				){

				return;
			}



			switch( GRIDTRACK_editmode ){

				case ON:

					// If gridtrack border is pressed, toggle track status
					// Otherwise toggle selection status
					switch( is_pressed_gridtrack_border() ){

						case TRUE:
							// Adjust the current selection pattern
							GRID_p_selection[row]->trackMutepattern ^= mirror( 1 << ( col - 3 ), 10 );
							break;


						case FALSE:
							// Adjust the current selection pattern
							GRID_p_selection[row]->trackSelection ^= mirror( 1 << ( col - 3 ), 10 );

							// Remember it accordingly
							if ( is_pressed_key( KEY_SELECT_MASTER ) ){

								GRID_p_selection[row]->trackSelectionStored =
									GRID_p_selection[row]->trackSelection;
							}
							break;
					} // pressed gridtrack border
					break;

				case OFF:
					// Adjust the current selection pattern
					GRID_p_selection[row]->trackMutepattern ^= mirror( 1 << ( col - 3 ), 10 );
					break;

			} // GRIDTRACK edit mode

		} // Single click scenario




