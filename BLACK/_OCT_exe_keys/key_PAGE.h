// Key execution code for DEFAULT keymode


	//
	// TRACK_SELECTORS
	//

	// Turn off the recording track
	if ((keyNdx > 0) && (keyNdx <= 10)) {
	
		if ( (VIEWER_page->track_REC_pattern & (1 << (keyNdx-1)) ) != 0 ){

			// Disable the recording on that track
			VIEWER_page->track_REC_pattern = 0;
		}	
	}
	

	// Do the normal operation - legacy..
	if (	(( keyNdx >= 1 ) && ( keyNdx <= 10 ))
		&&	( VIEWER_page->OPS_mode != BIRDSEYE )
		){


		// If we are in PREVIEW mode, preview the track
		if (	( VIEWER_page->editorMode == PREVIEW )
			){

			row = keyNdx - 1;
			PLAYER_preview_track( VIEWER_page, row );
		}


		// Set the step_attribute_selection
		if (VIEWER_page->stepSelection != 0){
			
			// Toggle the attribute selection
			if (VIEWER_page->stepAttributeSelection == (1 << (keyNdx-1)) ) {
				VIEWER_page->stepAttributeSelection = 0;
			}
			// Make sure we don't react on irelevant buttons..
			else if ( PANEL_get_attributes_all( STEP_S ) & (1 << (keyNdx-1)) ){
				VIEWER_page->stepAttributeSelection = (1 << (keyNdx-1));
			}	

			// Terminate the if clause..
			break;			

		} // VIEWER_page->stepSelection != 0

		else{

			// Toggle bit in trackSelection marking the track un/selected
			VIEWER_page->trackSelection ^= 1 << (keyNdx-1);

			// Update the store variable
			if ( VIEWER_page->OPS_mode == PASSIVE ){
				VIEWER_page->trackSelectionStored = VIEWER_page->trackSelection;
			}

			// Passive mode: if there is no track selection 
			if ( 	( VIEWER_page->trackSelection == 0 ) 
				&&	( VIEWER_page->OPS_mode == PASSIVE ) 
				){
	
				// use a fresh store variable
				VIEWER_page->trackSelectionStored = 0;
	
				// Also, once the selection is empty again, switch to INTERACTIVE mode.
				VIEWER_page->OPS_mode = INTERACTIVE;
				
				return;
			}
	
	
			// If this is a SINGLE PLAYMODE track
			if ((VIEWER_page->Track[keyNdx-1]->attribute[ATTR_MISC] & (1<<1)) > 0) {
	
				// Allow it to play again by clearing second bit
				VIEWER_page->Track[keyNdx-1]->attribute[ATTR_MISC] = 
					VIEWER_page->Track[keyNdx-1]->attribute[ATTR_MISC] & 0xFD;
			}
	
	
			// D O U B L E - C L I C K 
			if ((DOUBLE_CLICK_TARGET == keyNdx) 
					&& (DOUBLE_CLICK_TIMER > DOUBLE_CLICK_ALARM_SENSITIVITY)) {
	
					// Zoom into the track and make it the single one in selection
					VIEWER_page->trackSelection = (1<<(keyNdx-1));
					
					// Switch to mode THIS_TRACK
					VIEWER_page->keyMode = KMOD_THIS_TRACK;
					
					// Switch zoomlevel
					zoom_level = zoomTRACK;
			}
			// Step_activity is ON: turn it OFF or SELECT it
			else if (DOUBLE_CLICK_TARGET == 0) {
					DOUBLE_CLICK_TARGET = keyNdx;
					DOUBLE_CLICK_TIMER = ON;								
					// Start the Double click Alarm
					cyg_alarm_initialize(	
							doubleClickAlarm_hdl, 
							cyg_current_time() + DOUBLE_CLICK_ALARM_TIME,
							DOUBLE_CLICK_ALARM_TIME );

			}
		} // there is no step selection active in page		

	} // key is a track selector



	//
	// SELECT MASTER
	//
	if (keyNdx == KEY_SELECT_MASTER) {

		// Only react when no steps are selected
		if (VIEWER_page->stepSelection != 0){

			// Unselect ALL the selected steps
			for (row=0; row<MATRIX_NROF_ROWS; row++){
				for (col=0; col<MATRIX_NROF_COLUMNS; col++){
					Step_set_status( VIEWER_page->Step[row][col], STEPSTAT_SELECT, OFF );
				}
			}
			// Reset the counter of selected steps
			VIEWER_page->stepSelection = OFF;
			VIEWER_page->stepAttributeSelection = OFF;

			break;
		}

		// Clear trackSelection if exists
		if (VIEWER_page->trackSelection != 0) {

			// Store the track selection
			VIEWER_page->trackSelectionStored = VIEWER_page->trackSelection;

			// Clear the track selection
			VIEWER_page->trackSelection = 0;

			// Enter interactive mode
			VIEWER_page->OPS_mode = PASSIVE;

			// Clear the SEL lock for track selection
			VIEWER_page->SEL_LOCK = OFF;
		} 

		// Fill the selection pattern from store
		else {

			// If there is a stored track selection
			if (VIEWER_page->trackSelectionStored != 0) {

				// Select it into the page
				VIEWER_page->trackSelection = VIEWER_page->trackSelectionStored;						

				// Enter PASSIVE mode
				VIEWER_page->OPS_mode = PASSIVE;
				
				VIEWER_page->SEL_LOCK = OFF;
			}
		}

		// D O U B L E - C L I C K
		if (	(DOUBLE_CLICK_TARGET == keyNdx) 
			&& 	(DOUBLE_CLICK_TIMER > DOUBLE_CLICK_ALARM_SENSITIVITY)
			){
					
			// This is a double click victim - Select all tracks
			VIEWER_page->trackSelection = 0x3ff;
			VIEWER_page->trackSelectionStored = VIEWER_page->trackSelection;

			// Enter PASSIVE MODE
			VIEWER_page->OPS_mode = PASSIVE;
		}

		// Step_activity is ON: turn it OFF or SELECT it
		else if (DOUBLE_CLICK_TARGET == 0) {

				DOUBLE_CLICK_TARGET = keyNdx;
				DOUBLE_CLICK_TIMER = ON;								

				// Start the Double click Alarm
				cyg_alarm_initialize(	
						doubleClickAlarm_hdl, 
						cyg_current_time() + DOUBLE_CLICK_ALARM_TIME,
						DOUBLE_CLICK_ALARM_TIME );
		}
	}



	//
	// MATRIX 
	//
	// Make sure we are within the real matrix range
	if ( 	( keyNdx >= 11 ) && ( keyNdx <= 185 )
		&& 	( ( (keyNdx-10) % 11 ) != 0 ) 
		){ 

		// Block bottom two key rows when in chainmode 3 and 4
		if (   (VIEWER_page->chainMode == CHAINMODE_3) 
			|| (VIEWER_page->chainMode == CHAINMODE_4)  
			){
				
			if ( ((keyNdx - 9) % 11 == 0) || ((keyNdx - 10) % 11 == 0) ){
				// Do nothing here..
				break;
			}
		}

		// Compute Step coordinates
		row = row_of(keyNdx);
		col = column_of(keyNdx);

		// Check if the PLAY Mode button is pressed
		if (	( VIEWER_page->editorMode == PREVIEW )
			){

			// Play the Step information as it would be played by the player
			PLAYER_preview_step( VIEWER_page, row, col );

			// Record the step as it is being tapped in
			// tap_step_edit( VIEWER_page, row, col );
		}

		// Do not act on steps when in BIRDSEYE - this is handled in its own file
		if ( VIEWER_page->OPS_mode != BIRDSEYE ){
				
			// Needs some more elaboration but works for now
			VIEWER_page->page_clear = OFF;

			// Interpret the button press in the conventional way
			interpret_matrix_stepkey( row, col, VIEWER_page );
		}

		// d_iag_printf( "trackpattern:%d\n", Page_get_trackpattern( VIEWER_page, row ) );
		// my_print_bits( Page_get_trackpattern( VIEWER_page, row ) );		
	}
	
	
	// Pressing the SELECT button in top right quadrant..
	if (	( keyNdx == KEY_SELECT )
		&&	( VIEWER_page->scaleStatus == OFF )
		){

		GRID_bigknob_SELECT = ON;
	}
	// ..or the TEMPO key
	if ( 	( keyNdx == KEY_TEMPO )
		&&	( VIEWER_page->scaleStatus == OFF ) 
		){

		GRID_bigknob_SELECT = OFF;
	}


	// Void the REC pattern hence disabling recording.
	if ( keyNdx == KEY_RECORD ){

		switch( VIEWER_page->trackSelection ){
		
			case 0:
				if ( VIEWER_page->track_REC_pattern == 0 ){

					VIEWER_page->REC_bit ^= ON;
				}
				else{
					VIEWER_page->track_REC_pattern = 0;
					VIEWER_page->REC_bit = OFF;
				}
				break;

			default:
				VIEWER_page->REC_bit = OFF;
				break;
		}
	}
	

	// GRID mode switch
	if (keyNdx == KEY_ZOOM_GRID) {

		GRID_play_mode = GRID_MIX;
		zoom_level = zoomGRID;
		VIEWER_page->keyMode = KMOD_GRID;
	}






