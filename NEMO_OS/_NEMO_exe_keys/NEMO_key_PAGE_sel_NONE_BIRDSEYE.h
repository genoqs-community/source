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
// There is no octave circle in NEMO
//	# include HEADER(_OCT_exe_keys/key_OCT_CIRCLE_xpose_PAGE.h)



	// SELECTORS
	if ((keyNdx >0) && (keyNdx <= 10)) {
		// Toggle the playmodes for the GRID bank.
		GRID_bank_playmodes ^= 1 << (shiftPageRow + keyNdx - 1);
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
//		#warning GS: Call has no effect b/c point not reached; function returns before this..
//		Flash_read_page( GRID_CURSOR );
//		return;
//	}

	//
	// MATRIX KEYS
	//
	// Select the page corresponding to the key press as the new VIEWER page..
	else if ( (keyNdx >= 11) && (keyNdx <= 185) ) {

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
				i = row_of(shiftPageRow + keyNdx) + (10 * column_of (keyNdx));

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
		switch (keyNdx) {

			case KEY_PLAY1:
				// Toggle the page playing status - classic toggle behavior
				switch ( is_selected_in_GRID( target_page ) ) {
					case ON:
						grid_select( target_page, OFF );
						break;

					case OFF:
						grid_select( target_page, ON  );
						break;
				}
				break;


			case KEY_MUTE_MASTER:
				// Solo the page - all details were outsourced.. so they can be used elsewhere
				PAGE_toggle_solo();
				break;


			case KEY_CLEAR:
				// Select all tracks in cursor page
				Page_repository[GRID_CURSOR].trackSelection = NEMO_MAX_WINDOW;

				// Clear selected tracks in page
				Page_CLEAR_selected_tracks( &Page_repository[GRID_CURSOR] );

				PAGE_init( &Page_repository[GRID_CURSOR], Page_repository[GRID_CURSOR].pageNdx, False );
				// PAGE_init (Pagestruct* target_page, pageid_t pageId, booln firstInitBo );


				// Initialize the page Track chain data
				for ( row=0; row < MATRIX_NROF_ROWS; row++ ){

					// Init the chain data for 10 unchained rows: each track is by itself
					(&Page_repository[GRID_CURSOR])->Track[row]->chain_data[HEAD] = (&Page_repository[GRID_CURSOR])->Track[row];
					(&Page_repository[GRID_CURSOR])->Track[row]->chain_data[NEXT] = (&Page_repository[GRID_CURSOR])->Track[row];
					(&Page_repository[GRID_CURSOR])->Track[row]->chain_data[PREV] = (&Page_repository[GRID_CURSOR])->Track[row];
					(&Page_repository[GRID_CURSOR])->Track[row]->chain_data[PLAY] = (&Page_repository[GRID_CURSOR])->Track[row];
				}

				// Assign the default pitches to the tracks
				for ( row=0; row<MATRIX_NROF_ROWS; row++ ){

					(&Page_repository[GRID_CURSOR])->Track[row]->attr_PIT =
						TRACK_DEFAULT_PITCH[row];

					// Reset track directions to normal
					(&Page_repository[GRID_CURSOR])->Track[row]->attr_DIR = TRACK_DEF_DIRECTION;
				}


				// Unselect all tracks in cursor page
				Page_repository[GRID_CURSOR].trackSelection = OFF;

				// Mark page as cleared
				Page_repository[GRID_CURSOR].page_clear = ON;

				break;



			case KEY_RMX:

				// Select all tracks in cursor page
				Page_repository[GRID_CURSOR].trackSelection = NEMO_MAX_WINDOW;

				switch( target_page->page_clear ){

					case ON:
						// Randomize selected tracks in page - step status
						Page_RNDMZ_selected_tracks( target_page );
						break;

					case OFF:
						// Randomize selected tracks in page - step status
						Page_RMX_selected_tracks( target_page );
						break;
				}

				// Unselect all tracks in cursor page
				Page_repository[GRID_CURSOR].trackSelection = OFF;

				// Mark page as touched
				Page_repository[GRID_CURSOR].page_clear = OFF;

				// Mark page as touched
				target_page->page_clear = OFF;
				break;



			case KEY_FLT:
				// Only active pages can be flipped. Break on inactive page
				if (is_selected_in_GRID( &Page_repository[GRID_CURSOR] ) == OFF){
					break;
				}

				// Select all tracks in cursor page
				Page_repository[GRID_CURSOR].trackSelection = NEMO_MAX_WINDOW;

				// Flatten all selected tracks in page - step status
				Page_FLT_selected_tracks( &Page_repository[GRID_CURSOR] );

				// Unselect all tracks in cursor page
				Page_repository[GRID_CURSOR].trackSelection = OFF;

				// Mark page as touched
				Page_repository[GRID_CURSOR].page_clear = OFF;
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




