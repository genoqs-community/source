

	// MATRIX
	//
	if ( (keyNdx < 186) && (((keyNdx - 20) % 11) == 0) ) {
		// MATRIX BOTTOM ROW

		// Compute coordinates
		col = ((keyNdx-20) / 11) + 1;
		row = my_bit2ndx( VIEWER_page->trackSelection );

		// Compute real keyNdx for the step
		keyNdx = row + (col * 11);
		
		// This implements the select function as well..
		// Step_activity is OFF: turn it ON with or without select
		if (Step_get_status( VIEWER_page->Step[row_of(keyNdx)][column_of(keyNdx)], STEPSTAT_TOGGLE ) == OFF) {

			// Turn step ON..
			Step_set_status
				( VIEWER_page->Step[row_of(keyNdx)][column_of(keyNdx)], STEPSTAT_TOGGLE, ON);

			// ..and select it if this is the second click of a double click
			if (DOUBLE_CLICK_TARGET == keyNdx) {
				// This is a double click victim
				// Set Selection Bit
				Step_set_status
					( VIEWER_page->Step[row_of(keyNdx)][column_of(keyNdx)], STEPSTAT_SELECT, ON);
				// Increment the selection counter
				VIEWER_page->stepSelection++;
			}
		} else {
			// Step_activity is ON: turn it OFF or SELECT it
			// If step is selected turn selection bit OFF
			if (Step_get_status 
					( VIEWER_page->Step[row_of(keyNdx)][column_of(keyNdx)], STEPSTAT_SELECT ) == ON) {
				Step_set_status 
					( VIEWER_page->Step[row_of(keyNdx)][column_of(keyNdx)], STEPSTAT_SELECT, OFF);								
				// Decrement the selection counter
				VIEWER_page->stepSelection--;								
			} else {
				// Else turn step activity OFF.. 
				Step_set_status 
					(VIEWER_page->Step[row_of(keyNdx)][column_of(keyNdx)], STEPSTAT_TOGGLE, OFF);

				// ..but remember it, and start DC timer
				if (DOUBLE_CLICK_TARGET == 0) {
					DOUBLE_CLICK_TARGET = keyNdx;
					DOUBLE_CLICK_TIMER = ON;								
					// Start the Double click Alarm
					cyg_alarm_initialize(	
						doubleClickAlarm_hdl, 
						cyg_current_time() + DOUBLE_CLICK_ALARM_TIME,
						DOUBLE_CLICK_ALARM_TIME );
				}
			}
		} // get activity of step 
	} // If this is a bottom Matrix row key	


	else if ( (keyNdx > 10) && (keyNdx < 186) && (((keyNdx-10) % 11) != 0) ) { 
		// PLAY THE NOTES
	
		// Compute row of track
		row = my_bit2ndx( VIEWER_page->trackSelection );
		
		// Compute pitch
		i = Page_getPitchOf( keyNdx );
		
		// Compute velo
		// j = 64;
		
		j = VIEWER_page->bigKnobAmt * 5;
		
		// This is for the case without MIDI OFF on falling edge				
//				// Create coresponding MIDI OFF on the queue
//				// TRACK_buildNOTEOFF( row, locator, midiCH, pitch );
//				TRACK_buildNOTEOFF( 
//									row,
//									VIEWER_page->Track[row]->attribute[ATTR_LOCATOR],
//									VIEWER_page->Track[row]->attribute[ATTR_MIDICH],
//									i
//								  );
//
//				// d_iag_printf( "OFF: ro:%d ch:%d pi:%d\n", row, VIEWER_page->Track[row]->attribute[ATTR_MIDICH], i );
//
//				// PLAY the note
//				// void play_step(char channel, char pitch, char velo)

		// Prepare info for MIDI OFF note: channel and Pitch
		PLAY_MIDI_OFF[OFF_MIDICH] 	= VIEWER_page->Track[row]->attribute[ATTR_MIDICH];
		PLAY_MIDI_OFF[OFF_PITCH]	= i + VIEWER_page->Track[row]->attribute[ATTR_PITCH];

		MIDI_send(	MIDI_NOTE, 
					VIEWER_page->Track[row]->attribute[ATTR_MIDICH], 
					i, 
					j );

		// RECORD the note
		row = my_bit2ndx( VIEWER_page->trackSelection );
		col = VIEWER_page->Track[row]->attribute[ATTR_LOCATOR]-1;
		
		// Set step pitch
		VIEWER_page->Step[row][col]->attribute[ATTR_PITCH] 		= i;

		// Set step velocity
		VIEWER_page->Step[row][col]->attribute[ATTR_VELOCITY] 	= j;

		// Turn ON step
		Step_set_status( VIEWER_page->Step[row][col], STEPSTAT_TOGGLE, ON );

	}
	
	//
	// ZOOM SELECTOR
	// 
	if (keyNdx == KEY_ZOOM_GRID) {
		zoom_level = zoomGRID;
		// Keymode is implicit for GRID
	}

	if (keyNdx == KEY_ZOOM_PAGE) {
		zoom_level = zoomPAGE;
		VIEWER_page->keyMode = KMOD_PAGE;
	}

//			if (keyNdx == KEY_ZOOM_MAP) {
//				// React only if there is one attribute selected 
//				if (VIEWER_page->trackAttributeSelection != 0){
//					zoom_level = zoomMAP;
//					VIEWER_page->keyMode = KMOD_ATTR_MAP;
//				}
//			}

	if (keyNdx == KEY_ZOOM_TRACK) {
	 	zoom_level = zoomTRACK;
		VIEWER_page->keyMode = KMOD_THIS_TRACK;
	}













