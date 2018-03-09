// Key execution code for DEFAULT keymode


	//
	// TRACK_SELECTORS
	//

	// Turn off the recording track
	if ((keyNdx > 0) && (keyNdx <= 10)) {
	
		if ( (target_page->track_REC_pattern & (1 << (keyNdx-1)) ) != 0 ){

			// Disable the recording on that track
			target_page->track_REC_pattern = 0;
		}	
	}
	

	// Do the normal operation - legacy..
	if (	(( keyNdx >= 1 ) && ( keyNdx <= 10 ))
		&&	( target_page->OPS_mode != BIRDSEYE )
		){


		// If we are in PREVIEW mode, preview the track
		if (	( target_page->editorMode == PREVIEW )
			){

			row = keyNdx - 1;
			PLAYER_preview_track( target_page, row );
		}


		// Set the step_attribute_selection
		if (target_page->stepSelection != 0){
			
			// Toggle the attribute selection
			if (target_page->stepAttributeSelection == (1 << (keyNdx-1)) ) {
				target_page->stepAttributeSelection = 0;
			}
			// Make sure we don't react on irelevant buttons..
			else if ( PANEL_get_attributes_all( STEP_S ) & (1 << (keyNdx-1)) ){
				target_page->stepAttributeSelection = (1 << (keyNdx-1));
			}	

			// Terminate the if clause..
			break;			

		} // target_page->stepSelection != 0

		else{

			if ( is_pressed_key( KEY_MIX_MASTER ) ){
					
				// Assign the new mix Attribute
				target_page->mixAttribute = keyNdx;
		
				target_page->trackSelection = 0;
				return;
			}


			// Toggle bit in trackSelection marking the track un/selected
			target_page->trackSelection ^= 1 << (keyNdx-1);

			// Update the store variable
			if ( target_page->OPS_mode == PASSIVE ){
				target_page->trackSelectionStored = target_page->trackSelection;
			}

			// Passive mode: if there is no track selection 
			if ( 	( target_page->trackSelection == 0 ) 
				&&	( target_page->OPS_mode == PASSIVE ) 
				){
	
				// use a fresh store variable
				target_page->trackSelectionStored = 0;
	
				// Also, once the selection is empty again, switch to INTERACTIVE mode.
				target_page->OPS_mode = INTERACTIVE;
				
				return;
			}
	
	
			// If this is a SINGLE PLAYMODE track
			if ((target_page->Track[keyNdx-1]->attribute[ATTR_MISC] & (1<<1)) > 0) {
	
				// Allow it to play again by clearing second bit
				target_page->Track[keyNdx-1]->attribute[ATTR_MISC] = 
					target_page->Track[keyNdx-1]->attribute[ATTR_MISC] & 0xFD;
			}
	
	
			// D O U B L E - C L I C K 
			if ((DOUBLE_CLICK_TARGET == keyNdx) 
					&& (DOUBLE_CLICK_TIMER > DOUBLE_CLICK_ALARM_SENSITIVITY)) {
	
					// Zoom into the track and make it the single one in selection
					target_page->trackSelection = (1<<(keyNdx-1));
					
					// Switch to mode THIS_TRACK
					target_page->keyMode = KMOD_THIS_TRACK;
					
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
		if (target_page->stepSelection != 0){

			// Unselect ALL the selected steps
			for (row=0; row<MATRIX_NROF_ROWS; row++){
				for (col=0; col<MATRIX_NROF_COLUMNS; col++){
					Step_set_status( target_page->Step[row][col], STEPSTAT_SELECT, OFF );
				}
			}
			// Reset the counter of selected steps
			target_page->stepSelection = OFF;
			target_page->stepAttributeSelection = OFF;

			break;
		}

		// Clear trackSelection if exists
		if (target_page->trackSelection != 0) {

			// Store the track selection
			target_page->trackSelectionStored = target_page->trackSelection;

			// Clear the track selection
			target_page->trackSelection = 0;

			// Enter interactive mode
			target_page->OPS_mode = PASSIVE;

			// Clear the SEL lock for track selection
			target_page->SEL_LOCK = OFF;
		} 

		// Fill the selection pattern from store
		else {

			// If there is a stored track selection
			if (target_page->trackSelectionStored != 0) {

				// Select it into the page
				target_page->trackSelection = target_page->trackSelectionStored;						

				// Enter PASSIVE mode
				target_page->OPS_mode = PASSIVE;
				
				target_page->SEL_LOCK = OFF;
			}
		}

		// D O U B L E - C L I C K
		if (	(DOUBLE_CLICK_TARGET == keyNdx) 
			&& 	(DOUBLE_CLICK_TIMER > DOUBLE_CLICK_ALARM_SENSITIVITY)
			){
					
			// This is a double click victim - Select all tracks
			target_page->trackSelection = 0x3ff;
			target_page->trackSelectionStored = target_page->trackSelection;

			// Enter PASSIVE MODE
			target_page->OPS_mode = PASSIVE;
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

		// Compute Step coordinates
		row = row_of(keyNdx);
		col = column_of(keyNdx);

		// Check if the PLAY Mode button is pressed
		if (	( target_page->editorMode == PREVIEW )
			){

			// Play the Step information as it would be played by the player
			PLAYER_preview_step( target_page, row, col );
		}

		// Do not act on steps when in BIRDSEYE - this is handled in its own file
		if ( target_page->OPS_mode != BIRDSEYE ){
				
			// Needs some more elaboration but works for now
			target_page->page_clear = OFF;

			// Interpret the button press in the conventional way
			interpret_matrix_stepkey( row, col, target_page );
		}

		// d_iag_printf( "trackpattern:%d\n", Page_get_trackpattern( target_page, row ) );
		// my_print_bits( Page_get_trackpattern( target_page, row ) );		
	}
	
	
	// Pressing the SELECT button in top right quadrant..
	if (	( keyNdx == KEY_SCALE_MYSEL )
		&&	( target_page->scaleStatus == OFF )
		){

		GRID_bigknob_SELECT = ON;
	}
	// ..or the TEMPO key
	if ( 	( keyNdx == KEY_TEMPO )
		&&	( target_page->scaleStatus == OFF ) 
		){

		GRID_bigknob_SELECT = OFF;
	}


	// Void the REC pattern hence disabling recording.
	if ( keyNdx == KEY_RECORD ){

		switch( target_page->trackSelection ){
		
			case 0:
				if ( target_page->track_REC_pattern == 0 ){

					target_page->REC_bit ^= ON;
				}
				else{
					target_page->track_REC_pattern = 0;
					target_page->REC_bit = OFF;
				}
				break;

			default:
				target_page->REC_bit = OFF;
				break;
		}
	}
	

	// GRID mode switch
	if (keyNdx == KEY_ZOOM_GRID) {

		GRID_play_mode = GRID_MIX;
		zoom_level = zoomGRID;
		target_page->keyMode = KMOD_GRID;
	}






