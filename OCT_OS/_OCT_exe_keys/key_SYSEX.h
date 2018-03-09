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



	// Return to GRID mode
	if ( keyNdx == KEY_RETURN ){
		G_zoom_level = zoomGRID;
	}


	// PAGE DUMP - dump an individual page if it it not empty
	if ( 	( keyNdx >= 11 ) && ( keyNdx <= 185 )
		&& 	( ( (keyNdx-10) % 11 ) != 0 ) 
		){ 
	
		// Compute Page index (coordinates)
		row = row_of( keyNdx );
		col = column_of( keyNdx );

		// Exclude row 0 from the matter..
		if ( row == 9 ) return;

		// Compute the page index
		j = col*10 + row;

		// Send only the pages that have content
		if ( Page_repository[ j ].page_clear == FALSE ){
			my_sysex_dump( def_PAGE_SYSEX_DUMP, j, 6, 8 );	
		}
	}


	// BANK DUMP - dump all non-empty pages from a bank to sysex
	if ((keyNdx > 0) && (keyNdx <= 10)) {
		int objectCt = 0;
		int objectIx = 0;

		// Explicit row index
		row = keyNdx - 1;

		// SCENE DUMP
		if ( row == 9 ) {
			my_sysex_dump( def_SCENE_SYSEX_DUMP, 0, 1, 1 );
			return;
		}

		// Count objects involved.
		for ( col=0; col < MATRIX_NROF_COLUMNS; col++ ){
			j = col*10 + row;
			if ( Page_repository[ j ].page_clear == FALSE ){
				objectCt++;
			}
		}

		// Send each column in the given row
		for ( col=0; col < MATRIX_NROF_COLUMNS; col++ ){
			// Compute the page index
			j = col*10 + row;
			// Send only the pages that have content
			if ( Page_repository[ j ].page_clear == FALSE ){
				my_sysex_dump( def_PAGE_SYSEX_DUMP, j, objectIx++, objectCt );
			}
		}
	}


	// GRID SETTINGS DUMP
	if ( keyNdx == KEY_MIX_MASTER ){
		// The 0 parameter is just a place holder -  not processed further
		my_sysex_dump( def_GRID_SYSEX_DUMP, 0, 6, 8 );	
	}



	// ALL PAGES DUMP
	if ( keyNdx == KEY_SELECT_MASTER ){
		int objectCt = 0;
		int objectIx = 0;

		// Count the number of objects.
		for ( row = 0; row < GRID_NROF_BANKS; row++ ){
			for ( col=0; col < MATRIX_NROF_COLUMNS; col++ ){
				j = col*10 + row;
				if ( Page_repository[ j ].page_clear == FALSE ){
					objectCt++;
				}
			}
		}

		// Send all banks in one shot
		for ( row = 0; row < GRID_NROF_BANKS; row++ ){
			// Send each column in the given row
			for ( col=0; col < MATRIX_NROF_COLUMNS; col++ ){
				// Compute the page index
				j = col*10 + row;
				// Send only the pages that have content
				if ( Page_repository[ j ].page_clear == FALSE ){
					my_sysex_dump( def_PAGE_SYSEX_DUMP, j, objectIx++, objectCt );
				}
			} 
		} 
	}



	// ALL DATA DUMP
	if ( keyNdx == KEY_EDIT_MASTER ) {
		int objectCt = 1;
		int objectIx = 0;

		// Count number of pages involved.
		for ( row = 0; row < GRID_NROF_BANKS; row++ ){
			for ( col=0; col < MATRIX_NROF_COLUMNS; col++ ){
				j = col*10 + row;
				if ( Page_repository[ j ].page_clear == FALSE ){
					objectCt++;
				}
			} 
		} 
		
		// Send all banks in one shot
		for ( row = 0; row < GRID_NROF_BANKS; row++ ){
			// Send each column in the given row
			for ( col=0; col < MATRIX_NROF_COLUMNS; col++ ){
				// Compute the page index
				j = col*10 + row;
				// Send only the pages that have content
				if ( Page_repository[ j ].page_clear == FALSE ) {
					my_sysex_dump( def_PAGE_SYSEX_DUMP, j, objectIx++, objectCt );
				}
			} 
		} 

		// Send the GRID settings
		// The 0 parameter is just a place holder -  not processed further
		my_sysex_dump( def_GRID_SYSEX_DUMP, 0, objectIx, objectCt );	
	}


