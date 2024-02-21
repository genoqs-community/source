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


	switch( GRID_play_mode ){

		case GRID_EDIT:

			// GRID_CURSOR shown by blinking 
			GRID_write_dot (GRID_CURSOR, MIR_BLINK);
			// blink page cluster selection
			if ( GRID_p_selection_cluster == ON ) {
				page_cluster_selection( GRID_CURSOR );
			}
			if ( Page_repository[GRID_CURSOR].page_clear == ON ){	
				// If on empty page then light position orange
				GRID_write_dot (GRID_CURSOR, MIR_GREEN);
				GRID_write_dot (GRID_CURSOR, MIR_RED);
			} 

			// NON-EMPTY PAGES shown GREEN or RED
			for (i=0; i < MAX_NROF_PAGES; i++) {
				
				// Page has contents and is not one of the row zero
				if (	(Page_repository[i].page_clear != ON)
					&&	( (i % 10) != 9 )
					){

					// Page PLAYING - i.e. selected in GRID
					if ( page_is_selected_in_GRID( &Page_repository[i] ) ){
						
						// Show it in GREEN
						GRID_write_dot( i, MIR_GREEN );

					}
					else {
						
						// Show it in RED - page is IDLE
						GRID_write_dot( i, MIR_RED );
					}
				} // page_clear != ON
			} // page iterator

			// GRID_write_dot( 39, MIR_GREEN );
			break;



		case GRID_MIX:

			// GRID_CURSOR shown by blinking 

			// Show the current GRID set - but not when called from page
			if ( 	( G_zoom_level == zoomGRID )
				&&	( !is_pressed_key( 10 ) ) ) {

				// GRID SETS: Show the non-empty GRID sets by red lights.
				for ( i=0; i < GRID_NROF_SETS; i++ ){

					// Check the respective GRID set for content and currency
					if ( 	( get_content_GRID_set( i ) )
						&&	( current_GRID_set != i )
						){

						GRID_write_dot( 9 + i*10, MIR_RED );
					}
				}

				GRID_write_dot( 9 + current_GRID_set*10, MIR_GREEN 	);

				// Check if changes have been made to set
				if ( !is_actual_GRID( current_GRID_set ) ){

					GRID_write_dot( 9 + current_GRID_set*10, MIR_RED );
				}

	//			// Blink the current GRID set - but not when called from page BIRDSEYE
	//			if ( G_zoom_level != zoomPAGE ){

				GRID_write_dot( 9 + current_GRID_set*10, MIR_BLINK 	);
			}



			// PAGES: Page iterator
			for ( i=0; i < MAX_NROF_PAGES; i++ ){

				// Page has contents and is not one of the row zero
				if (	(Page_repository[i].page_clear != ON)
					&&	( (i % 10) != 9 )
					){

					// Page PLAYING - i.e. selected in GRID
					if ( page_is_selected_in_GRID( &Page_repository[i] ) ){			

						// Show it in GREEN
						GRID_write_dot( i, MIR_GREEN );
						if (	( G_run_bit == ON )
							&&	( Page_repository[i].attr_STA == 0 )
							&&	( ( GRID_bank_playmodes & ( 1 << (i % 10) ) ) != 0 ) ){
							// Indicate clustered page is page looping indefinitely
							GRID_write_dot( i, MIR_BLINK );
						}
					}
					else {
						// Show it in GREEN
						GRID_write_dot( i, MIR_RED );
					}
				} // page_clear != ON
			} // page iterator


			// Orange the GRID preselection
			for ( j = 0; j < GRID_NROF_BANKS; j++ ){

				// If the page is not already playing anyways.. and page not empty / clear
				if (	( ! page_is_selected_in_GRID( GRID_p_clock_presel[j] ))
					&&	( GRID_p_clock_presel[j]->page_clear != ON )
					){

					// Show the preselection orange
					GRID_write_dot( GRID_p_clock_presel[j]->pageNdx, MIR_RED );
					GRID_write_dot( GRID_p_clock_presel[j]->pageNdx, MIR_GREEN );	
				}
			}

			break;

	} // switch( GRID_play_mode )


	// Write Grid to MIR
	MIR_write_GRID ();
