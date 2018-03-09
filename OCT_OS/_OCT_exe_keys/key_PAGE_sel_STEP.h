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
// The case where the page contains an active step selection
//

	// Coordinates of the first selected step in page.

	// COMPUTE STEP COORDINATES: ROW AND COL
	// Compute ROW
	for (row=0; row<MATRIX_NROF_ROWS; row++) {
		j= Page_get_selection_trackpattern( target_page, row );
		if (j != 0) {
			// row was found
			break;
		}
	}
	// COMPUTE COL
	for (col=0; col<MATRIX_NROF_COLUMNS; col++) {
		if ( ((1<<(15-col)) & j) != 0) {
			// col was found
			break;
		}
	}

	// Remember STEP COORDINATES
	target_page->stepSelectionSingleRow = row;
	target_page->stepSelectionSingleCol = col;


	// STEP mode switch
	//
	if (keyNdx == KEY_ZOOM_STEP) {
		if (target_page->stepSelection == 1) {
			G_zoom_level = zoomSTEP;
		}
	}


	// STEP SELECTION PATTERN INDEX
	if (	(keyNdx == KEY_MIXTGT_USR1)
		||	(keyNdx == KEY_MIXTGT_USR2)
		||	(keyNdx == KEY_MIXTGT_USR3)
		||	(keyNdx == KEY_MIXTGT_USR4)
		||	(keyNdx == KEY_MIXTGT_USR5)
		){

		// Export content to old index
		export_stepSELpattern( target_page );

		// Set the new index
		switch( keyNdx ){
			case KEY_MIXTGT_USR1:	target_page->stepSELpattern_ndx = 0;	break;
			case KEY_MIXTGT_USR2:	target_page->stepSELpattern_ndx = 1;	break;
			case KEY_MIXTGT_USR3:	target_page->stepSELpattern_ndx = 2;	break;
			case KEY_MIXTGT_USR4:	target_page->stepSELpattern_ndx = 3;	break;
			case KEY_MIXTGT_USR5:	target_page->stepSELpattern_ndx = 4;	break;
		}

		// Import from new index
		import_stepSELpattern( target_page );
	}



	//
	// TRACK_MUTATORS
	//
	if ((keyNdx >= 187) && (keyNdx <= 196)) {

		switch (keyNdx) {

			case KEY_TGGL:
				// Only active when there is no stepAttributeSelection
				if ( target_page->stepAttributeSelection != 0 ){
					break;
				}

				// Go through all selected steps and toggle their activity status
				// ..G_scaning through all steps of the matrix - not very efficient!!
				// d_iag_printf( "Toggle\n" );

				for (row=0; row < MATRIX_NROF_ROWS; row++) {
					for (col=0; col < MATRIX_NROF_COLUMNS; col++) {

						// If the selection flag is set
						if (Step_get_status( target_page->Step[row][col], STEPSTAT_SELECT ) == ON){

							if( Step_get_status( target_page->Step[row][col], STEPSTAT_TOGGLE ) == ON){

								// If the activity flag is set - turn it off
								Step_set_status( target_page->Step[row][col], STEPSTAT_TOGGLE, OFF );
								// d_iag_printf( "OFF row:%d col:%d\n", row, col );
							}
							else {

								// Turn ON step
								Step_set_status( target_page->Step[row][col], STEPSTAT_TOGGLE, ON );
							}
						}
					}
				}
				break;



			case KEY_CLEAR:
				// Search all steps and act on the ones selected
				for (row=0; row < MATRIX_NROF_ROWS; row++) {
					for (col=0; col < MATRIX_NROF_COLUMNS; col++) {

						// If the selection flag is set, act on step
						if ( Step_get_status( target_page->Step[row][col], STEPSTAT_SELECT ) == ON){

							Step_clear( target_page, target_page->Step[row][col] );
						} // step is selcted

					} // col iterator
				} // row iterator
				break;



			case KEY_RANDOMIZE:

				// Search all steps and act on the ones selected
				for (row=0; row < MATRIX_NROF_ROWS; row++) {
					for (col=0; col < MATRIX_NROF_COLUMNS; col++) {

						// If the selection flag is set, act on step
						if (Step_get_status( target_page->Step[row][col], STEPSTAT_SELECT ) == ON){

							Step_exe_randomize( target_page, target_page->Step[row][col] );
						} // step is selected

					} // col iterator
				} // row iterator
				break;



			case KEY_ZOOM:
				// This works if only one step is selected..
				// ..and no stepAttribute selection is active.
				if (	(target_page->stepSelection == 1)
					&&	(target_page->stepAttributeSelection == 0)	){


					// COMPUTE STEP COORDINATES: ROW AND COL
					// Compute ROW
					for (row=0; row<MATRIX_NROF_ROWS; row++) {
						j= Page_get_selection_trackpattern( target_page, row );
						if (j != 0) {
							// row was found
							break;
						}
					}
					// COMPUTE COL
					for (col=0; col<MATRIX_NROF_COLUMNS; col++) {
						if ( ((1<<(15-col)) & j) != 0) {
							// col was found
							break;
						}
					}

					// Remember STEP COORDINATES
					target_page->stepSelectionSingleRow = row;
					target_page->stepSelectionSingleCol = col;

					// Switch the mode
					G_zoom_level = zoomSTEP;
				}
				break;



			case KEY_COPY:
				if (target_page->stepSelection != 1){
					break;
				}

				// Coordinates of already selected step
				row = target_page->stepSelectionSingleRow;
				col = target_page->stepSelectionSingleCol;

				// Copy the step pointer to the buffer
				STEP_COPY_BUFFER = target_page->Step[row][col];

				// Unselect the step
				Step_set_status( target_page->Step[row][col], STEPSTAT_SELECT, OFF );
				target_page->stepSelection = 0;

				break;



			case KEY_PASTE:
				if (	(target_page->stepSelection != 1)
					||	(STEP_COPY_BUFFER == NULL)
					){

					break;
				}

				// Coordinates of already selected step
				row = target_page->stepSelectionSingleRow;
				col = target_page->stepSelectionSingleCol;

				// Copy step data from buffer to pointer
				Step_copy( STEP_COPY_BUFFER, target_page->Step[row][col]);

				// Unselect the step
				Step_set_status( target_page->Step[row][col], STEPSTAT_SELECT, OFF );
				target_page->stepSelection = 0;

#ifdef COPY_BUFFER_FRESH
				STEP_COPY_BUFFER = NULL;
#endif
				break;
		}

	} // Key is a track mutator


	//
	// TRANSPORT functions - when nothing is selected in page
	//
	if ( G_clock_source != EXT ){

		switch( keyNdx ){

			case KEY_STOP:
				sequencer_command_STOP();
				break;


			case KEY_PAUSE:
				sequencer_command_PAUSE(OFF);
				break;


			case KEY_PLAY1:
			case KEY_PLAY2:
			case KEY_PLAY4:

				// Clear the recording track on PLAY1 while recording..
				if (	( keyNdx == KEY_PLAY1 )
					&&	( G_run_bit == ON )
					){

					// Find and clear the recording track in target_page
					if ( Page_getTrackRecPattern(target_page) != 0 ){

						// Place recording track into artificial selection
						target_page->trackSelection = Page_getTrackRecPattern(target_page);
						// Clear the track
						Page_CLEAR_selected_tracks( target_page );
						// Forget the artificial selection
						target_page->trackSelection = 0;
					}
				}

				// Make sure the sequencer is running
				sequencer_command_PLAY();

				break;
		}
	}


