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




// Key execution in STEP mode
void key_exec_STEP( unsigned int keyNdx ){

	unsigned char row = 0;
	unsigned char col = 0;
	unsigned char j	  = 0;

	// Work on the page under the grid cursor
	Pagestruct* target_page = &Page_repository[GRID_CURSOR];


	//
	// TRACK_SELECTORS
	//
	// Edit Track Attribute Selection if one of the valid ones
	if ( 	( keyNdx > 0 )
		&& 	( keyNdx <= 10 )
		){

		// D O U B L E - C L I C K
		if ((DOUBLE_CLICK_TARGET == keyNdx)
				&& (DOUBLE_CLICK_TIMER > DOUBLE_CLICK_ALARM_SENSITIVITY)) {

			// If clicked on an attribute map
			if ( ( (1 << (keyNdx-1) ) & PANEL_get_trackAttributeMap() ) != 0 ) {

				// Re-store the step event status modified by the single click
				// Click on an active event - turn the event OFF
				if (	( ( keyNdx - 1) ==
					 	  (target_page->Step[ target_page->stepSelectionSingleRow ]
							  		        [ target_page->stepSelectionSingleCol ]->event_data & 0x0F) )
					&&	( Step_get_status( target_page->Step[ target_page->stepSelectionSingleRow ]
									  				        [ target_page->stepSelectionSingleCol ],
									 	   STEPSTAT_EVENT ) == ON )
					){

					Step_set_status( target_page->Step[ target_page->stepSelectionSingleRow ]
									  				  [ target_page->stepSelectionSingleCol ],
									 STEPSTAT_EVENT,
									 OFF );
				}
				else{

					// if ( ( keyNdx != 3 )&&( keyNdx != 4 ) ){

						// Remember the pressed attribute..
						target_page->Step[ target_page->stepSelectionSingleRow ]
								  		 [ target_page->stepSelectionSingleCol ]->event_data
							= 	(target_page->Step[ target_page->stepSelectionSingleRow ]
								  		   	 	  [ target_page->stepSelectionSingleCol ]->event_data & 0xF0)
								| (keyNdx-1);

						// ..and turn the event status bit ON
						Step_set_status( target_page->Step[ target_page->stepSelectionSingleRow ]
										  				  [ target_page->stepSelectionSingleCol ],
										 STEPSTAT_EVENT,
										 ON );
					// } // no STA or LEN
				}


				// Make sure the selected attribute is recorded. Else risk 0 value
				target_page->trackAttributeSelection = 1 << (keyNdx-1);

				// Switch zoomLevel to MAP - unless we go to GRV edit mode (STRUM)
				if ( target_page->trackAttributeSelection == (1<<7) ){

					if ( target_page->stepSelection == 1 ) {
						card8 row = target_page->stepSelectionSingleRow;
						card8 col = target_page->stepSelectionSingleCol;
						PhraseSetCurrent( target_page->Step[row][col]->phrase_num );
					}

					// Switch to the Chordstrum edit view
					G_zoom_level = zoomSTRUM;
				}
				else{
					// Select in track the row of the selected step
					target_page->trackSelection = 1 << target_page->stepSelectionSingleRow;

					// Void the step selection
					target_page->stepSelection = 0;
					Step_set_status(
						target_page->Step[ target_page->stepSelectionSingleRow ]
										 [ target_page->stepSelectionSingleCol ],
						STEPSTAT_SELECT, OFF );

					// All other attributes - switch zoom level
					G_zoom_level = zoomMAP;
				}
			}
		}

		else if (DOUBLE_CLICK_TARGET == 0) {

			DOUBLE_CLICK_TARGET = keyNdx;
			DOUBLE_CLICK_TIMER = ON;
			// Start the Double click Alarm
			cyg_alarm_initialize(
				doubleClickAlarm_hdl,
				cyg_current_time() + DOUBLE_CLICK_ALARM_TIME,
				DOUBLE_CLICK_ALARM_TIME );

			// SINGLE CLICK CODE
			// Initialize the event_offsets
			target_page->Track[ target_page->stepSelectionSingleRow ]->event_offset[ATTR_VELOCITY] 	= 0;
			target_page->Track[ target_page->stepSelectionSingleRow ]->event_offset[ATTR_PITCH] 	= 0;
			target_page->Track[ target_page->stepSelectionSingleRow ]->event_offset[ATTR_LENGTH] 	= 0;
			target_page->Track[ target_page->stepSelectionSingleRow ]->event_offset[ATTR_START] 	= 0;
			target_page->Track[ target_page->stepSelectionSingleRow ]->event_offset[ATTR_DIRECTION] = 0;
			target_page->Track[ target_page->stepSelectionSingleRow ]->event_offset[ATTR_AMOUNT] 	= 0;
			target_page->Track[ target_page->stepSelectionSingleRow ]->event_offset[ATTR_GROOVE] 	= 0;
			target_page->Track[ target_page->stepSelectionSingleRow ]->event_offset[ATTR_MIDICC] 	= 0;
			target_page->Track[ target_page->stepSelectionSingleRow ]->event_offset[ATTR_MIDICH] 	= 0;

			// Now set the event status..
			if (target_page->stepSelection == 1){

				// Click on an active event - turn the event OFF
				if (	( ( keyNdx - 1) ==
					 	  (target_page->Step[ target_page->stepSelectionSingleRow ]
							  		        [ target_page->stepSelectionSingleCol ]->event_data & 0x0F) )
					&&	( Step_get_status( target_page->Step[ target_page->stepSelectionSingleRow ]
									  				        [ target_page->stepSelectionSingleCol ],
									 	   STEPSTAT_EVENT ) == ON )
					){

					Step_set_status( target_page->Step[ target_page->stepSelectionSingleRow ]
									  				  [ target_page->stepSelectionSingleCol ],
									 STEPSTAT_EVENT,
									 OFF );
				}
				else{

					// Remember the pressed attribute..
					target_page->Step[ target_page->stepSelectionSingleRow ]
							  		 [ target_page->stepSelectionSingleCol ]->event_data
						= 	(target_page->Step[ target_page->stepSelectionSingleRow ]
							  		   	 	  [ target_page->stepSelectionSingleCol ]->event_data & 0xF0)
							| (keyNdx-1);

					// ..and turn the event status bit ON
					Step_set_status( target_page->Step[ target_page->stepSelectionSingleRow ]
									  				  [ target_page->stepSelectionSingleCol ],
									 STEPSTAT_EVENT,
									 ON );
				}

				// Terminate the clause..
				return;
			}
		}

	} // Double click target == keyNdx



	//
	// MATRIX
	//

	// Step selection routine, will work only in Birdseye mode..
	if ( 	( MODE_OBJECT_SELECTION == BIRDSEYE )
		&&	( keyNdx >  10 )
		&& 	( keyNdx <= 185 )
		&& 	( ((keyNdx-10) % 11) != 0 )
		){
		// Move step selection around the matrix using the keys

		// Coordinates of already selected step
		row = target_page->stepSelectionSingleRow;
		col = target_page->stepSelectionSingleCol;

		// Un-SELECT current step
		Step_set_status( target_page->Step[row][col], STEPSTAT_SELECT, OFF );
		// ..and decrement the counter of steps selected
		target_page->stepSelection--;

		// If the selected step is pressed again, unselect and leave
		if ( 	( row == row_of		( keyNdx ) )
			&&	( col == column_of	( keyNdx ) )
			){

			// Void the step selection in page
			// Not necessary, done above already

			// Leave the Step zoom mode
			switch( target_page->trackSelection ){

				case 0:
					// Switch back out of step zoom
					G_zoom_level = zoomPAGE;
					break;

				default:
					// Zoomed into here from an attribute map
					// So go back to the MAP
					G_zoom_level = zoomMAP;
					break;
			}
		} // Selected step was pressed again

		else {

			// Coordinates of newly selected step
			row = row_of	( keyNdx );
			col = column_of	( keyNdx );

			// SELECT new step - but only if it is not skipped
//			if ( Step_get_status( target_page->Step[row][col], STEPSTAT_SKIP ) != ON ){

				Step_set_status( target_page->Step[row][col], STEPSTAT_SELECT, ON );

				// ..and increment the counter of steps selected
				target_page->stepSelection++;

				// Remember STEP COORDINATES
				target_page->stepSelectionSingleRow = row;
				target_page->stepSelectionSingleCol = col;

//			} // Step not skipped

		} // Unselected step was pressed
	}


	// Step selection routine, covers non-Birdseye mode..
	// Step data entry via matrix buttons
	else if ( 		( MODE_OBJECT_SELECTION != BIRDSEYE )
				&&	( keyNdx >  10 )
				&& 	( keyNdx <= 185 )
				&& 	( ((keyNdx-10) % 11) != 0 )
		){

		// Coordinates of already selected step
		row = target_page->stepSelectionSingleRow;
		col = target_page->stepSelectionSingleCol;


		//
		// MIDICH - select another step via matrix keys
		//
		if ( ( ( keyNdx - MIDICH ) % 11) == 0 ){

			// Unselect the original step
			Step_set_status(
				target_page->Step[ target_page->stepSelectionSingleRow ]
								 [ target_page->stepSelectionSingleCol ],
				STEPSTAT_SELECT, OFF );

			// Compute new col coordinates - selected step in the same row;
			// target_page->stepSelectionSingleRow = row_of( keyNdx );
			target_page->stepSelectionSingleCol = column_of( keyNdx );

			// Select the new step
			Step_set_status(
				target_page->Step[ target_page->stepSelectionSingleRow ]
								 [ target_page->stepSelectionSingleCol ],
				STEPSTAT_SELECT, ON );
		}


		//
		// VELOCITY - enter via matrix keys
		//
		if ( ((keyNdx - VELOCITY) % 11) == 0 ){

			// D O U B L E - C L I C K
			if (	(	(DOUBLE_CLICK_TARGET == keyNdx)
					&& (DOUBLE_CLICK_TIMER > DOUBLE_CLICK_ALARM_SENSITIVITY) )
				){

				// Double click code: Modify the tens value
				if ( col < 12 ){
					// Write the final tens value into the step attribute
					target_page->Step[row][col]->attr_VEL =
						normalize( (col+1)*10, 0, STEP_MAX_VELOCITY );
				} // col < 12
				else{
					// Write the final value into the attribute
					target_page->Step[row][col]->attr_VEL = STEP_DEF_VELOCITY;
				} // col >= 12
			} // double click is active

//			else if ( DOUBLE_CLICK_TARGET == 0 ){
//
//				DOUBLE_CLICK_TARGET = keyNdx;
//				DOUBLE_CLICK_TIMER = ON;
//				// Start the Double click Alarm
//				cyg_alarm_initialize(
//					doubleClickAlarm_hdl,
//					cyg_current_time() + DOUBLE_CLICK_ALARM_TIME,
//					DOUBLE_CLICK_ALARM_TIME );
//
//				// SINGLE CLICK CODE:
//				if ( col < 9 ){
//
//					// Modify the ones value.
//					target_page->Step[row][col]->attr_VEL =
//						normalize(
//							(target_page->Step[row][col]->attr_VEL/10)*10 + col+1,
//								0, STEP_MAX_VELOCITY );
//				}
//				else{
//					// ...
//				}
//			} // double click not active

		} // key in the VELOCITY row.

	} // Key is in the matrix, non-birdseye




	//
	// TRACK_MUTATORS
	//
	if ((keyNdx >= 187) && (keyNdx <= 196)) {

		// Coordinates of already selected step
		row = target_page->stepSelectionSingleRow;
		col = target_page->stepSelectionSingleCol;


		switch (keyNdx) {

			case KEY_TGGL:

//d_iag_printf( "step:%d chord_size:%d\n",
//				(int) &target_page->Step[row][col],
//				target_page->Step[row][col]->chord_up >> 29 );


				// OPTIMIZE!!! Only need to handle one key here!

				// Do not act if there is a step attribute selected
				if ( target_page->stepAttributeSelection != 0 ){

					break;
				}

//				// Go through all selected steps and toggle their activity status
//				// ..G_scaning through all steps of the matrix - not very efficient!!
//				for (row=0; row < MATRIX_NROF_ROWS; row++) {
//					for (col=0; col < MATRIX_NROF_COLUMNS; col++) {
//
//						// If the selection flag is set
//						if (Step_get_status( target_page->Step[row][col], STEPSTAT_SELECT ) == ON){

							// TOGGLE THE ACTIVITY
							if (Step_get_status( target_page->Step[row][col], STEPSTAT_TOGGLE ) == ON){
								// If the activity flag is set - turn it off
								Step_set_status( target_page->Step[row][col], STEPSTAT_TOGGLE, OFF );
							}
							else {
								// Turn ON step
								Step_set_status( target_page->Step[row][col], STEPSTAT_TOGGLE, ON );
							}

							// REMOVE ANY POSSIBLE SKIP BIT
							Step_set_status( target_page->Step[row][col], STEPSTAT_SKIP, OFF );
//						}
//					}
//				}

				break;



			case KEY_RANDOMIZE:
				// Coordinates of already selected step
				row = target_page->stepSelectionSingleRow;
				col = target_page->stepSelectionSingleCol;

				Step_exe_randomize( target_page, target_page->Step[row][col] );

				break;



			case KEY_CLEAR:
				// Coordinates of already selected step
				row = target_page->stepSelectionSingleRow;
				col = target_page->stepSelectionSingleCol;

				Step_clear( target_page, target_page->Step[row][col] );

				// But re-mark the step as selected and active!
				Step_set_status( target_page->Step[row][col], STEPSTAT_SELECT, ON );
				// Step_set_status( target_page->Step[row][col], STEPSTAT_TOGGLE, ON );

				break;



			case KEY_ZOOM:
				// Do not react if a stepAttributeSelection has been made
				if ( target_page->stepAttributeSelection != 0 ){
					break;
				}

				// An existing track selection means that we have zoomed in
				// from a track attribute map (and not from a page).
				// In that case unzoom into the track attribute map (not into page).
				switch( target_page->trackSelection ){

					case 0:
						// Zoomed into here directly from a page
						if (target_page->stepSelection == 1) {

							// Switch back to Step zoom
							G_zoom_level = zoomPAGE;

							// Unselect the step
							// Update the step selection counter in page
							Step_set_status( target_page->Step[row][col], STEPSTAT_SELECT, OFF );
							target_page->stepSelection = OFF;
						}
						break;

					default:
						// Zoomed into here from an attribute map or a track
						// So go back to the MAP or the TRACK

						// Came here from a map, go back there
						if ( target_page->trackAttributeSelection != OFF ){

							G_zoom_level = zoomMAP;
						}
						// Came here from a track, go back there
						else{

							G_zoom_level = zoomTRACK;
						}

						// Unselect the step
						// Update the step selection counter in page
						Step_set_status( target_page->Step[row][col], STEPSTAT_SELECT, OFF );
						target_page->stepSelection = OFF;

						break;
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
				break;


			case KEY_PASTE:

				if (target_page->stepSelection != 1){
					break;
				}

				// Coordinates of already selected step
				row = target_page->stepSelectionSingleRow;
				col = target_page->stepSelectionSingleCol;

				// Copy step data from buffer to pointer
				if ( STEP_COPY_BUFFER != NULL ){

					Step_copy( STEP_COPY_BUFFER, target_page->Step[row][col], false );
#ifdef COPY_BUFFER_FRESH
					STEP_COPY_BUFFER = NULL;
#endif
				}
				break;

		} // switch( keyNdx )
	}



	//
	// CIRCLE ENTERING FIELDS
	//
	switch( MODE_OBJECT_SELECTION ){

		case CHORDEYE:
		#ifdef FEATURE_ENABLE_CHORD_OCTAVE
		case CHORDEYE_OCTAVE_FIRST:
		case CHORDEYE_OCTAVE_SECOND:
		case CHORDEYE_OCTAVE_THIRD:
		#endif
			// Set chord_size according to the pressed chord button

			// Enter the chord aux notes via inner circle
			key_OCT_CIRCLE_chord_STEP( keyNdx );
			break;

		default:
			// Enter the pitch via inner circle
			key_OCT_CIRCLE_xpose_STEP( keyNdx );
			break;
	}


	//
	// Enter VELOCITY using the BK keys
	//
	if ( 	( keyNdx == 201 )
		||	( keyNdx == 200	)
		||	( keyNdx == 199 )
		|| 	( keyNdx == 198 )
		|| 	( keyNdx == 197 )
		||	( keyNdx == 207 )
		||	( keyNdx == 206 )
		||	( keyNdx == 216 )
		|| 	( keyNdx == 215 )
		||	( keyNdx == 224 )
		){


		// Determine Step coordinates
		row = target_page->stepSelectionSingleRow;
		col = target_page->stepSelectionSingleCol;

		switch( MODE_OBJECT_SELECTION ){

			case CHORDEYE:
			#ifdef FEATURE_ENABLE_CHORD_OCTAVE
			case CHORDEYE_OCTAVE_FIRST:
			case CHORDEYE_OCTAVE_SECOND:
			case CHORDEYE_OCTAVE_THIRD:
			#endif
			// target_page->Step[row][col]->chord_data =
			// 	( target_page->Step[row][col]->chord_data & 0x8FF )
			//	|	( BK_KEY_to_xdx( keyNdx ) << 11 ) ;
				break;

			default:

				// If an event is set show the range defined for set event.
				if ( Step_get_status( target_page->Step[row][col], STEPSTAT_EVENT ) != ON
	   				){

					// Enter velocity via outter circle
					// Account for track offsets when entering directly..
					key_NUMERIC_QUADRANT_velo_STEP( keyNdx );
				}
				break;
		}
	}


	//
	// ZOOM MODE
	//
	// GRID zoom mode
	if (	(keyNdx == KEY_ZOOM_GRID)
		||	(keyNdx == KEY_ZOOM_PAGE)
		|| 	(keyNdx == KEY_ZOOM_TRACK) ){

		switch( keyNdx ){

			case KEY_ZOOM_GRID:
				// Remove the selection inside the page
				target_page->stepSelection = 0;

				// Unselect the step
				row = target_page->stepSelectionSingleRow;
				col = target_page->stepSelectionSingleCol;
				Step_set_status( target_page->Step[row][col], STEPSTAT_SELECT, OFF );

				// Clear the STEP COORDINATES
				target_page->stepSelectionSingleRow = 0;
				target_page->stepSelectionSingleCol = 0;
				target_page->stepSelection = 0;

				G_zoom_level = zoomGRID;
				// Keymode is implicit for GRID
				break;

			case KEY_ZOOM_PAGE:
				// Unselect the step
				row = target_page->stepSelectionSingleRow;
				col = target_page->stepSelectionSingleCol;
				Step_set_status( target_page->Step[row][col], STEPSTAT_SELECT, OFF );

				// Clear the STEP COORDINATES
				target_page->stepSelectionSingleRow = 0;
				target_page->stepSelectionSingleCol = 0;
				target_page->stepSelection = 0;

				// Set the keymode
				G_zoom_level = zoomPAGE;
				break;

			case KEY_ZOOM_TRACK:
				// Select the track that the zoomed step belongs to
				target_page->trackSelection = ( 1 << target_page->stepSelectionSingleRow );
				G_zoom_level = zoomTRACK;
				break;
		}

		// Unselect all potentially selected steps in the track
		// ..to prevent conflicting state of track and step selected
		if ( target_page->stepSelection != 0 ){

			// Calculate the row of the selected track
			row = my_bit2ndx( target_page->trackSelection );

			// Iterate through all steps in that row (selected track)
			for (col=0; col<MATRIX_NROF_COLUMNS; col++) {

				// If step is selected
				if ( Step_get_status( target_page->Step[row][col], STEPSTAT_SELECT ) == ON ){

					// Mark step un-selected
					Step_set_status( target_page->Step[row][col], STEPSTAT_SELECT, OFF);

					// And decrement step selection counter
					target_page->stepSelection --;
				}
			}
		}

	} // keyNdx is a KEY_ZOOM_xxx




	//
	// TRANSPORT buttons
	//

	// Set the LEN multiplier
	if (	( keyNdx == KEY_PLAY1 )
		||	( keyNdx == KEY_PLAY2 )
		||	( keyNdx == KEY_PLAY4 )
		){

		switch( keyNdx ){

			case KEY_PLAY1:
				j = 1;
				break;

			case KEY_PLAY2:
				j = 2;

				// Add one to the keypress
				if ( is_pressed_key( KEY_PLAY1 )){
					j = 3;
				}
				else if ( is_pressed_key( KEY_PLAY4 )){
					j = 8;
				}
				break;

			case KEY_PLAY4:
				j = 4;

				// Add one to the keypress
				if ( is_pressed_key( KEY_PLAY1 )){
					j = 5;
					// Add two additional to the keypress
					if ( is_pressed_key( KEY_PLAY2 )){
						j = 7;
					}
				}
				else if ( is_pressed_key( KEY_PLAY2 )){
						j = 6;
				}
				break;
		} // switch( keyNdx )

		// Coordinates of currently selected step
		row = target_page->stepSelectionSingleRow;
		col = target_page->stepSelectionSingleCol;

		// Set the Step LEN multiplier
		// target_page->Step[row][col]->LEN_multiplier = j;
		target_page->Step[row][col]->event_data =
			( (j<<4) | (target_page->Step[row][col]->event_data & (0x0F)) );


	} // PLAYx key pressed


	//
	// RECORD KEY
	//
	// Toggle record arm status of selected track, and toggle global track record mode along with it
	// so that the track is immediately in record mode.

	if ( keyNdx == KEY_RECORD ) {

		// First off, disable Page record mode.
		target_page->REC_bit = OFF;

		// Check if this track is currently armed.
		// If currently armed, un-arm this and all other tracks. Switch off global track record mode.

		card16 trackSelMap = (1 << target_page->stepSelectionSingleRow);
		booln armedBo = ((Page_getTrackRecPattern(target_page) & trackSelMap) != 0);
		if ( armedBo ) {
			Page_setTrackRecPattern( target_page, 0 );
			G_track_rec_bit = OFF;
		}

		// Else, arm this track and enable global track record mode.
		else {
			Page_setTrackRecPattern( target_page, trackSelMap );
			G_track_rec_bit = ON;
		}

		// Mark page as unclear
		target_page->page_clear = OFF;
	}


	#ifdef FEATURE_ENABLE_CHORD_OCTAVE
	if( keyNdx > 251 && keyNdx < 259 ) {
		// CHORD button
		if ( MODE_OBJECT_SELECTION <= CHORDEYE ) {

			// D O U B L E - C L I C K
			if (	(	(DOUBLE_CLICK_TARGET == keyNdx)
					&& (DOUBLE_CLICK_TIMER > DOUBLE_CLICK_ALARM_SENSITIVITY) )
				){
				MODE_OBJECT_SELECTION = CHORDEYE_OCTAVE_FIRST;
			} else if ( DOUBLE_CLICK_TARGET == 0 ){

				DOUBLE_CLICK_TARGET = keyNdx;
				DOUBLE_CLICK_TIMER = ON;
				// Start the Double click Alarm
				cyg_alarm_initialize(
					doubleClickAlarm_hdl,
					cyg_current_time() + DOUBLE_CLICK_ALARM_TIME,
					DOUBLE_CLICK_ALARM_TIME );

				// SINGLE CLICK CODE:
				// Enter the CHORDEYE view
				MODE_OBJECT_SELECTION = CHORDEYE;
			}
		}
	}

	if( keyNdx == KEY_SCALE_MYSEL ) {
		// Set chord view mode
		MODE_OBJECT_SELECTION = ( MODE_OBJECT_SELECTION == CHORDEYE ) ? CHORDEYE_OCTAVE_FIRST : CHORDEYE;
	} else {
		// 1 - 3 switches stack octave chord view
		switch( keyNdx ) {
			case KEY_SCALE_MOD:
				MODE_OBJECT_SELECTION = CHORDEYE_OCTAVE_FIRST;
				break;
			case KEY_SCALE_SEL:
				MODE_OBJECT_SELECTION = CHORDEYE_OCTAVE_SECOND;
				break;
			case KEY_SCALE_CAD:
				MODE_OBJECT_SELECTION = CHORDEYE_OCTAVE_THIRD;
				break;
		}
	}
	#endif
}


