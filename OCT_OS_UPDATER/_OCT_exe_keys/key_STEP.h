
// Key execution in STEP mode
void key_exec_STEP( unsigned int keyNdx ){

	unsigned char row = 0;
	unsigned char col = 0;

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

				// Select in track the row of the selected step
				target_page->trackSelection = 1 << target_page->stepSelectionSingleRow;

				// Void the step selection
				target_page->stepSelection = 0;
				Step_set_status( 
					target_page->Step[ target_page->stepSelectionSingleRow ]
									 [ target_page->stepSelectionSingleCol ],
					STEPSTAT_SELECT, OFF ); 

				// Switch zoomLevel
				zoom_level = zoomMAP;

				// Switch to keymode KMOD_ATTR_MAP
				target_page->keyMode = KMOD_ATTR_MAP;
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
			target_page->Track[ target_page->stepSelectionSingleRow ]->event_offset[ATTR_VELOCITY] = 0;
			target_page->Track[ target_page->stepSelectionSingleRow ]->event_offset[ATTR_PITCH] = 0;
			target_page->Track[ target_page->stepSelectionSingleRow ]->event_offset[ATTR_LENGTH] = 0;
			target_page->Track[ target_page->stepSelectionSingleRow ]->event_offset[ATTR_START] = 0;
			target_page->Track[ target_page->stepSelectionSingleRow ]->event_offset[ATTR_DIRECTION] = 0;
			target_page->Track[ target_page->stepSelectionSingleRow ]->event_offset[ATTR_AMOUNT] = 0;
			target_page->Track[ target_page->stepSelectionSingleRow ]->event_offset[ATTR_GROOVE] = 0;
			target_page->Track[ target_page->stepSelectionSingleRow ]->event_offset[ATTR_MIDICC] = 0;
			target_page->Track[ target_page->stepSelectionSingleRow ]->event_offset[ATTR_MIDICH] = 0;
	
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
	
				// Terminate the clause..
				return;
			}
		}

	} // Double click target == keyNdx


	if ((keyNdx > 0) && (keyNdx <= 10)) {

	} // Key is a SEL button




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
					target_page->keyMode = KMOD_PAGE;
					zoom_level = zoomPAGE;
					break;

				default:
					// Zoomed into here from an attribute map
					// So go back to the MAP
					target_page->keyMode = KMOD_ATTR_MAP;
					zoom_level = zoomMAP;				
					break;
			}		
		} // Selected step was pressed again
		else {
	
			// Coordinates of newly selected step
			row = row_of	( keyNdx );
			col = column_of	( keyNdx );
			
			// SELECT new step - but only if it is not skipped
			if ( Step_get_status( target_page->Step[row][col], STEPSTAT_SKIP ) == OFF ){
	
				Step_set_status( target_page->Step[row][col], STEPSTAT_SELECT, ON );
				
				// ..and increment the counter of steps selected
				target_page->stepSelection++;
			
				// Remember STEP COORDINATES
				target_page->stepSelectionSingleRow = row;
				target_page->stepSelectionSingleCol = col;

			} // Step not skipped
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
		// POSITION - select another step via matrix keys
		//
		if ( ( ( keyNdx - POSITION ) % 11) == 0 ){
			
			// Unselect the original step
			Step_set_status( 
				target_page->Step[ target_page->stepSelectionSingleRow ]
								 [ target_page->stepSelectionSingleCol ],
				STEPSTAT_SELECT, OFF ); 
			
			// Compute new coordinates - selected step in the same row;
			target_page->stepSelectionSingleRow = row_of( keyNdx );
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
					target_page->Step[row][col]->attribute[ATTR_VELOCITY] =
						normalize( (col+1)*10, 0, STEP_MAX_VELOCITY );
				} // col < 12
				else{

					// Write the final value into the attribute
					target_page->Step[row][col]->attribute[ATTR_VELOCITY] = 
						STEP_DEF_VELOCITY;
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
//					target_page->Step[row][col]->attribute[ATTR_VELOCITY] = 
//						normalize( 
//							(target_page->Step[row][col]->attribute[ATTR_VELOCITY]/10)*10 + col+1, 
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

			case TGGL_KEY:
				// OPTIMIZE!!! Only need to handle one key here!
				
				// Do not act if there is a step attribute selected
				if ( target_page->stepAttributeSelection != 0 ){

					break;
				}

//				// Go through all selected steps and toggle their activity status
//				// ..scaning through all steps of the matrix - not very efficient!!				
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



			case RANDOMIZE_KEY:
				// Coordinates of already selected step
				row = target_page->stepSelectionSingleRow;
				col = target_page->stepSelectionSingleCol;
		
				#include "key_STEP_exe_RND.h"

				break;

			

			case CLEAR_KEY:
				// Coordinates of already selected step
				row = target_page->stepSelectionSingleRow;
				col = target_page->stepSelectionSingleCol;
		
				#include "key_STEP_exe_CLR.h"
				
				// But re-mark the step as selected and active!
				Step_set_status( target_page->Step[row][col], STEPSTAT_SELECT, ON );
				// Step_set_status( target_page->Step[row][col], STEPSTAT_TOGGLE, ON );
				
				break;	
			


			case ZOOM_KEY:
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
							target_page->keyMode = KMOD_PAGE;
							zoom_level = zoomPAGE;

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

							target_page->keyMode = KMOD_ATTR_MAP;
							zoom_level = zoomMAP;				
						}
						// Came here from a track, go back there
						else{

							target_page->keyMode = KMOD_THIS_TRACK;
							zoom_level = zoomTRACK;
						}

						// Unselect the step
						// Update the step selection counter in page
						Step_set_status( target_page->Step[row][col], STEPSTAT_SELECT, OFF );
						target_page->stepSelection = OFF;

						break;
				}
				break;
				
				
			case COPY_KEY:		
				if (target_page->stepSelection != 1){
					break;
				}
		
				// Coordinates of already selected step
				row = target_page->stepSelectionSingleRow;
				col = target_page->stepSelectionSingleCol;
		
				// Copy the step pointer to the buffer
				STEP_COPY_BUFFER = target_page->Step[row][col];		
				break;
		
		
			case PASTE_KEY:	

				if (target_page->stepSelection != 1){
					break;
				}

				// Coordinates of already selected step
				row = target_page->stepSelectionSingleRow;
				col = target_page->stepSelectionSingleCol;
		
				// Copy step data from buffer to pointer
				if ( STEP_COPY_BUFFER != NULL ){

					Step_copy( STEP_COPY_BUFFER, target_page->Step[row][col]);	
					STEP_COPY_BUFFER = NULL;
				}			
				break;

		} // switch( keyNdx )
	}



	//
	// CIRCLE ENTERING FIELDS
	//
	if ( MODE_OBJECT_SELECTION == CHORDEYE ){
		// Enter the chord aux notes via inner circle
		key_OCT_CIRCLE_chord_STEP( keyNdx );
	}
	else{
		// Enter the pitch via inner circle
		key_OCT_CIRCLE_xpose_STEP( keyNdx );
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
		||	(keyNdx == KEY_ZOOM_PLAY)
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

				target_page->keyMode = KMOD_PAGE;
				zoom_level = zoomGRID;
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
				target_page->keyMode = KMOD_PAGE;
				zoom_level = zoomPAGE;					
				break;
				
			case KEY_ZOOM_TRACK:
				// Select the track that the zoomed step belongs to
				target_page->trackSelection = ( 1 << target_page->stepSelectionSingleRow );
				target_page->keyMode = KMOD_THIS_TRACK;
				zoom_level = zoomTRACK;
				break;			

			case KEY_ZOOM_PLAY:
//				if (target_page->trackSelection != 0) {
//					zoom_level = zoomPLAY;
//				}
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
	// TRANSPORT functions - when nothing is selected in page
	//
	switch( keyNdx ){

		case KEY_STOP:
			sequencer_command_STOP();
			break;


		case KEY_PAUSE:
			sequencer_command_PAUSE();
			break;


		case KEY_PLAY1:
		case KEY_PLAY2:
		case KEY_PLAY4:

			// Make sure the sequencer is running
			sequencer_command_PLAY();

			break;
	}	



	// Void the REC pattern hence disabling recording.
	if ( keyNdx == KEY_RECORD ){

		if ( target_page->track_REC_pattern == 0 ){

			target_page->REC_bit ^= ON;
		}
		else{
			target_page->track_REC_pattern = 0;
			target_page->REC_bit = OFF;
		}
	}



}


