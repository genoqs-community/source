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







	//
	// OCTAVE CIRCLE
	//
	# include "key_OCT_CIRCLE_xpose_PAGE.h"



	// SELECTORS
	if ((keyNdx >0) && (keyNdx <= 10)) {

		// Toggle the playmodes for the GRID bank.
		GRID_bank_playmodes ^= 1 << (keyNdx-1);
	}

//	// Save current page.
//	if ( keyNdx == KEY_PROGRAM ) {
//		if ( G_run_bit == OFF ){
//			Flash_write_page( GRID_CURSOR );
//		}
//		return;
//	}

//	// Load (restore) current page.
//	if ( keyNdx == KEY_RETURN ) {
//		Flash_read_page( GRID_CURSOR );
//		return;
//	}

	//
	// MATRIX KEYS
	//
	// Select the page corresponding to the key press as the new VIEWER page..
	if ( (keyNdx >= 11) && (keyNdx <= 185) ) {

		switch( keyNdx ){

			// These don't belong to the matrix. Only accidentally in the range.
			case 21: 	case 32: 	case 43: 	case 54:
			case 65: 	case 76:	case 87: 	case 98:
			case 109: 	case 120: 	case 131: 	case 142:
			case 153: 	case 164: 	case 175:
				// Do nothing
				break;


			default:

				// Compute the page index into the page repository first
				i = row_of(keyNdx) + (10 * column_of (keyNdx));

				// Decide what to do
				switch( i ){

					// GRID_set selectors. This key falls on a GRID SET
					case 9: 	case 19:	case 29:	case 39:
					case 49:	case 59:	case 69:	case 79:
					case 89:	case 99:	case 109:	case 119:
					case 129:	case 139:	case 149:	case 159:

						// Do nothing
						break;


					// Page selectors, act as always..
					// ..select the page for play in preselection or zoom into it (doubleclick)
					default:

						// Move the cursor.
						GRID_CURSOR = i;
						target_page = &Page_repository[ GRID_CURSOR ];

						G_zoom_level = zoomPAGE;
						target_page->trackSelection = OFF;
						target_page->trackAttributeSelection = OFF;

						break;

					} // switch( target_page->pageNdx );

		} // switch( keyNdx )

	} // keyNdx in MATRIX


	//
	// TRACK MUTATORS
	//
	if ((keyNdx >= 187) && (keyNdx <= 196)) {

		switch (keyNdx) {

			case KEY_TGGL:
				// Toggle the page playing status

				// Classic toggle behavior
				switch ( page_is_selected_in_GRID( target_page ) ) {

					case ON:
						grid_select( target_page, OFF );
						break;

					case OFF:
						grid_select( target_page, ON  );
						target_page->page_clear = OFF;
						break;
				}

				break;


			case KEY_SOLO:

				// All details were outsourced.. so they can be used elsewhere
				PAGE_toggle_solo();

				break;


			case KEY_CLEAR:
				Page_clear( &Page_repository[GRID_CURSOR] );
				break;



			case KEY_RANDOMIZE:

				// Select all tracks in cursor page
				Page_repository[GRID_CURSOR].trackSelection = 0x3FF;

				// Randomize selected tracks in page - step status
				Page_RNDMZ_selected_tracks( &Page_repository[GRID_CURSOR] );

				// Unselect all tracks in cursor page
				Page_repository[GRID_CURSOR].trackSelection = OFF;

				// Mark page as touched
				Page_repository[GRID_CURSOR].page_clear = OFF;
				break;



			case KEY_FLT:
				// Only active pages can be flipped. Break on inactive page
				if (page_is_selected_in_GRID( &Page_repository[GRID_CURSOR] ) == OFF){
					break;
				}

				// Select all tracks in cursor page
				Page_repository[GRID_CURSOR].trackSelection = 0x3FF;

				// Randomize selected tracks in page - step status
				Page_FLT_selected_tracks( &Page_repository[GRID_CURSOR] );

				// Unselect all tracks in cursor page
				Page_repository[GRID_CURSOR].trackSelection = OFF;

				// Mark page as touched
				Page_repository[GRID_CURSOR].page_clear = OFF;
				break;



			case KEY_RMX:
				// Only active pages can be rmx'ed. Break on inactive page
				if (page_is_selected_in_GRID( &Page_repository[GRID_CURSOR] ) == OFF){
					break;
				}

				// Select all tracks in cursor page
				Page_repository[GRID_CURSOR].trackSelection = 0x3FF;

				// Randomize selected tracks in page - step status
				Page_RMX_selected_tracks( &Page_repository[GRID_CURSOR] );

				// Unselect all tracks in cursor page
				Page_repository[GRID_CURSOR].trackSelection = OFF;

				// Mark page as touched
				Page_repository[GRID_CURSOR].page_clear = OFF;
				break;



			case KEY_ZOOM:
				// Zoom into the currently selected page
				target_page = &Page_repository[GRID_CURSOR];
				G_zoom_level = zoomPAGE;
				break;


			case KEY_COPY:
				// Remember the index of the copied page
				PAGE_COPY_BUFFER = GRID_CURSOR;
				break;


			case KEY_PASTE:
				// Make sure a page was copied first
				if ( PAGE_COPY_BUFFER != GRID_SELECTION_EMPTY ){

					// Paste the copied page into page under the cursor
					Page_copy( 	&Page_repository[PAGE_COPY_BUFFER],
								&Page_repository[GRID_CURSOR] 			);
				}
#ifdef COPY_BUFFER_FRESH
				// Clear the page copy buffer
				PAGE_COPY_BUFFER = GRID_SELECTION_EMPTY;
#endif
				break;

		} // switch (keyNdx)
	} // Track mutators





