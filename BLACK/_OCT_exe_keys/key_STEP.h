
// Key execution in STEP mode
void key_exec_STEP( unsigned int keyNdx ){

	unsigned char row = 0;
	unsigned char col = 0;

	//
	// TRACK_SELECTORS
	//
	if ((keyNdx > 0) && (keyNdx <= 10)) {

		// Set the event status..
		if (VIEWER_page->stepSelection == 1){
			
			// Click on an active event - turn the event OFF
			if (	( ( keyNdx - 1) == 
				 	  (VIEWER_page->Step[ VIEWER_page->stepSelectionSingleRow ]
						  		        [ VIEWER_page->stepSelectionSingleCol ]->event_data & 0x0F) )
				&&	( Step_get_status( VIEWER_page->Step[ VIEWER_page->stepSelectionSingleRow ]
								  				        [ VIEWER_page->stepSelectionSingleCol ],
								 	   STEPSTAT_EVENT ) == ON )
				){

				// Turn the EVENT bit OFF
				// if ( ( keyNdx != 3 )&&( keyNdx != 4 ) ){

					Step_set_status( VIEWER_page->Step[ VIEWER_page->stepSelectionSingleRow ]
									  				  [ VIEWER_page->stepSelectionSingleCol ],
									 STEPSTAT_EVENT, 
									 OFF );
				// } // no STA or LEN
			}
			else{
				
				// if ( ( keyNdx != 3 )&&( keyNdx != 4 ) ){

					// Remember the pressed attribute..
					VIEWER_page->Step[ VIEWER_page->stepSelectionSingleRow ]
							  		 [ VIEWER_page->stepSelectionSingleCol ]->event_data
						= 	(VIEWER_page->Step[ VIEWER_page->stepSelectionSingleRow ]
							  		   	 	  [ VIEWER_page->stepSelectionSingleCol ]->event_data & 0xF0) 
							| (keyNdx-1);

					// ..and turn the event status bit ON
					Step_set_status( VIEWER_page->Step[ VIEWER_page->stepSelectionSingleRow ]
									  				  [ VIEWER_page->stepSelectionSingleCol ],
									 STEPSTAT_EVENT, 
									 ON );
				// } // no STA or LEN
			}

			// Terminate the clause..
			return;
		}

	} // Key is a SEL button





	//
	// MATRIX 
	//
	
	// Step selection routine, will work only in Birdseye mode..
	if ( 	( MODE_OBJECT_SELECTION == BIRDSEYE )
		&&	( keyNdx >  10 ) 
		&& 	( keyNdx < 186 ) 
		&& 	( ((keyNdx-10) % 11) != 0 ) 
		){
		// Move step selection around the matrix using the keys
		
		// Coordinates of already selected step
		row = VIEWER_page->stepSelectionSingleRow;
		col = VIEWER_page->stepSelectionSingleCol;
		
		// Un-SELECT current step							
		Step_set_status( VIEWER_page->Step[row][col], STEPSTAT_SELECT, OFF );								
		// ..and decrement the counter of steps selected
		VIEWER_page->stepSelection--;								

		// If the selected step is pressed again, unselect and leave
		if ( 	( row == row_of		( keyNdx ) )
			&&	( col == column_of	( keyNdx ) )
			){
		
			// Void the step selection in page
			// Not necessary, done above already
			
			// Leave the Step zoom mode
			switch( VIEWER_page->trackSelection ){

				case 0:
					// Switch back out of step zoom
					VIEWER_page->keyMode = KMOD_PAGE;
					zoom_level = zoomPAGE;
					break;

				default:
					// Zoomed into here from an attribute map
					// So go back to the MAP
					VIEWER_page->keyMode = KMOD_ATTR_MAP;
					zoom_level = zoomMAP;				
					break;
			}		
		} // Selected step was pressed again
		else {
	
			// Coordinates of newly selected step
			row = row_of	( keyNdx );
			col = column_of	( keyNdx );
			
			// SELECT new step - but only if it is not skipped
			if ( Step_get_status( VIEWER_page->Step[row][col], STEPSTAT_SKIP ) == OFF ){
	
				Step_set_status( VIEWER_page->Step[row][col], STEPSTAT_SELECT, ON );
				
				// ..and increment the counter of steps selected
				VIEWER_page->stepSelection++;
			
				// Remember STEP COORDINATES
				VIEWER_page->stepSelectionSingleRow = row;
				VIEWER_page->stepSelectionSingleCol = col;

			} // Step not skipped
		} // Unselected step was pressed
	}


	// Step selection routine, covers non-Birdseye mode..
	// Step data entry via matrix buttons
	else if ( 		( MODE_OBJECT_SELECTION != BIRDSEYE )
				&&	( keyNdx >  10 ) 
				&& 	( keyNdx < 186 ) 
				&& 	( ((keyNdx-10) % 11) != 0 ) 
		){

		// Coordinates of already selected step
		row = VIEWER_page->stepSelectionSingleRow;
		col = VIEWER_page->stepSelectionSingleCol;

		
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
					VIEWER_page->Step[row][col]->attribute[ATTR_VELOCITY] =
						normalize( (col+1)*10, 0, STEP_MAX_VELOCITY );
				} // col < 12
				else{

					// Write the final value into the attribute
					VIEWER_page->Step[row][col]->attribute[ATTR_VELOCITY] = 
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
//					VIEWER_page->Step[row][col]->attribute[ATTR_VELOCITY] = 
//						normalize( 
//							(VIEWER_page->Step[row][col]->attribute[ATTR_VELOCITY]/10)*10 + col+1, 
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
		row = VIEWER_page->stepSelectionSingleRow;
		col = VIEWER_page->stepSelectionSingleCol;


		switch (keyNdx) {

			case TGGL_KEY:
				// OPTIMIZE!!! Only need to handle one key here!
				
				// Do not act if there is a step attribute selected
				if ( VIEWER_page->stepAttributeSelection != 0 ){

					break;
				}

//				// Go through all selected steps and toggle their activity status
//				// ..scaning through all steps of the matrix - not very efficient!!				
//				for (row=0; row < MATRIX_NROF_ROWS; row++) {
//					for (col=0; col < MATRIX_NROF_COLUMNS; col++) {
//
//						// If the selection flag is set
//						if (Step_get_status( VIEWER_page->Step[row][col], STEPSTAT_SELECT ) == ON){

							// TOGGLE THE ACTIVITY
							if (Step_get_status( VIEWER_page->Step[row][col], STEPSTAT_TOGGLE ) == ON){
								// If the activity flag is set - turn it off
								Step_set_status( VIEWER_page->Step[row][col], STEPSTAT_TOGGLE, OFF );
							}
							else {
								// Turn ON step
								Step_set_status( VIEWER_page->Step[row][col], STEPSTAT_TOGGLE, ON );
							}

							// REMOVE ANY POSSIBLE SKIP BIT
							Step_set_status( VIEWER_page->Step[row][col], STEPSTAT_SKIP, OFF );
//						}
//					}
//				}

				break;



			case RANDOMIZE_KEY:
				// Coordinates of already selected step
				row = VIEWER_page->stepSelectionSingleRow;
				col = VIEWER_page->stepSelectionSingleCol;
		
				#include "key_STEP_exe_RND.h"

				break;

			

			case CLEAR_KEY:
				// Coordinates of already selected step
				row = VIEWER_page->stepSelectionSingleRow;
				col = VIEWER_page->stepSelectionSingleCol;
		
				#include "key_STEP_exe_CLR.h"
				
				// But re-mark the step as selected and active!
				Step_set_status( VIEWER_page->Step[row][col], STEPSTAT_SELECT, ON );
				// Step_set_status( VIEWER_page->Step[row][col], STEPSTAT_TOGGLE, ON );
				
				break;	
			


			case ZOOM_KEY:
				// Do not react if a stepAttributeSelection has been made
				if ( VIEWER_page->stepAttributeSelection != 0 ){
					break;
				}

				// An existing track selection means that we have zoomed in 
				// from a track attribute map (and not from a page).
				// In that case unzoom into the track attribute map (not into page).
				switch( VIEWER_page->trackSelection ){

					case 0:
						// Zoomed into here directly from a page
						if (VIEWER_page->stepSelection == 1) {

							// Switch back to Step zoom
							VIEWER_page->keyMode = KMOD_PAGE;
							zoom_level = zoomPAGE;

							// Unselect the step
							// Update the step selection counter in page
							Step_set_status( VIEWER_page->Step[row][col], STEPSTAT_SELECT, OFF );
							VIEWER_page->stepSelection = OFF;
						}							
						break;

					default:
						// Zoomed into here from an attribute map or a track
						// So go back to the MAP or the TRACK
	
						// Came here from a map, go back there
						if ( VIEWER_page->trackAttributeSelection != OFF ){

							VIEWER_page->keyMode = KMOD_ATTR_MAP;
							zoom_level = zoomMAP;				
						}
						// Came here from a track, go back there
						else{

							VIEWER_page->keyMode = KMOD_THIS_TRACK;
							zoom_level = zoomTRACK;
						}

						// Unselect the step
						// Update the step selection counter in page
						Step_set_status( VIEWER_page->Step[row][col], STEPSTAT_SELECT, OFF );
						VIEWER_page->stepSelection = OFF;

						break;
				}
				break;
				
				
			case COPY_KEY:		
				if (VIEWER_page->stepSelection != 1){
					break;
				}
		
				// Coordinates of already selected step
				row = VIEWER_page->stepSelectionSingleRow;
				col = VIEWER_page->stepSelectionSingleCol;
		
				// Copy the step pointer to the buffer
				STEP_COPY_BUFFER = VIEWER_page->Step[row][col];		
				break;
		
		
			case PASTE_KEY:	

				if (VIEWER_page->stepSelection != 1){
					break;
				}

				// Coordinates of already selected step
				row = VIEWER_page->stepSelectionSingleRow;
				col = VIEWER_page->stepSelectionSingleCol;
		
				// Copy step data from buffer to pointer
				if ( STEP_COPY_BUFFER != NULL ){

					Step_copy( STEP_COPY_BUFFER, VIEWER_page->Step[row][col]);	
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
		row = VIEWER_page->stepSelectionSingleRow;
		col = VIEWER_page->stepSelectionSingleCol;

		switch( MODE_OBJECT_SELECTION ){
		
			case CHORDEYE:

			// VIEWER_page->Step[row][col]->chord_data = 
			// 	( VIEWER_page->Step[row][col]->chord_data & 0x8FF )
			//	|	( BK_KEY_to_xdx( keyNdx ) << 11 ) ;
				break;
		
			default:
			
				// If an event is set show the range defined for set event.
				if ( Step_get_status( VIEWER_page->Step[row][col], STEPSTAT_EVENT ) != ON 
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
				VIEWER_page->stepSelection = 0;
				
				// Unselect the step
				row = VIEWER_page->stepSelectionSingleRow;
				col = VIEWER_page->stepSelectionSingleCol;
				Step_set_status( VIEWER_page->Step[row][col], STEPSTAT_SELECT, OFF );

				// Clear the STEP COORDINATES
				VIEWER_page->stepSelectionSingleRow = 0;
				VIEWER_page->stepSelectionSingleCol = 0;
				VIEWER_page->stepSelection = 0;

				VIEWER_page->keyMode = KMOD_PAGE;
				zoom_level = zoomGRID;
				// Keymode is implicit for GRID			
				break;
	
			case KEY_ZOOM_PAGE:
				// Unselect the step
				row = VIEWER_page->stepSelectionSingleRow;
				col = VIEWER_page->stepSelectionSingleCol;
				Step_set_status( VIEWER_page->Step[row][col], STEPSTAT_SELECT, OFF );

				// Clear the STEP COORDINATES
				VIEWER_page->stepSelectionSingleRow = 0;
				VIEWER_page->stepSelectionSingleCol = 0;
				VIEWER_page->stepSelection = 0;

				// Set the keymode
				VIEWER_page->keyMode = KMOD_PAGE;
				zoom_level = zoomPAGE;					
				break;
				
			case KEY_ZOOM_TRACK:
				// Select the track that the zoomed step belongs to
				VIEWER_page->trackSelection = ( 1 << VIEWER_page->stepSelectionSingleRow );
				VIEWER_page->keyMode = KMOD_THIS_TRACK;
				zoom_level = zoomTRACK;
				break;			

			case KEY_ZOOM_PLAY:
//				if (VIEWER_page->trackSelection != 0) {
//					zoom_level = zoomPLAY;
//				}
				break;
		}
	
		// Unselect all potentially selected steps in the track
		// ..to prevent conflicting state of track and step selected
		if ( VIEWER_page->stepSelection != 0 ){
			
			// Calculate the row of the selected track
			row = my_bit2ndx( VIEWER_page->trackSelection );

			// Iterate through all steps in that row (selected track)
			for (col=0; col<MATRIX_NROF_COLUMNS; col++) {
	
				// If step is selected			
				if ( Step_get_status( VIEWER_page->Step[row][col], STEPSTAT_SELECT ) == ON ){

					// Mark step un-selected
					Step_set_status( VIEWER_page->Step[row][col], STEPSTAT_SELECT, OFF);

					// And decrement step selection counter
					VIEWER_page->stepSelection --;
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

}


