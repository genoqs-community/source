
	// D O U B L E - C L I C K  C O N S T R U C T
	// DOUBLE CLICK SCENARIO
	if (	( DOUBLE_CLICK_TARGET == keyNdx ) 
		&& 	( DOUBLE_CLICK_TIMER   > DOUBLE_CLICK_ALARM_SENSITIVITY ) ) {

		// Double click code
		// ...
		if ( keyNdx == KEY_ZOOM_GRID ){
		
			// Ressurect from the dead..
			zoom_level = zoomPAGE;
			
			target_page->keyMode = KMOD_PAGE;	
			target_page->trackSelection = OFF;
			target_page->trackAttributeSelection = OFF;
			
			// Unselect all steps in page
			if ( target_page->stepSelection != 0 ){
	
				for (row=0; row<MATRIX_NROF_ROWS; row++){
					for (col=0; col<MATRIX_NROF_COLUMNS; col++){
	
						Step_set_status( target_page->Step[row][col], STEPSTAT_SELECT, OFF );
					}
				}
				// Reset the counter of selected steps
				target_page->stepSelection = OFF;
				target_page->stepAttributeSelection = OFF;
			}
	
			// Reset the timers
			MIX_TIMER 	= OFF;
			EDIT_TIMER 	= OFF;
	
			// Reset the REC bit
			target_page->REC_bit = OFF;
	
			// Release the MIDI SYSEX state
			MIDI_RECEIVE_SYSEX = FALSE;
			MIDI_SYSEX_HEADER_SEEN = FALSE;
			sysex_stream_keeper_index = 0;
		}		
		
	} // end of double click scenario 
	

	// SINGLE CLICK SCENARIO
	else if (DOUBLE_CLICK_TARGET == 0) {
		
			DOUBLE_CLICK_TARGET = keyNdx;
			DOUBLE_CLICK_TIMER = ON;
			// Start the Double click Alarm
			cyg_alarm_initialize(	
					doubleClickAlarm_hdl, 
					cyg_current_time() + DOUBLE_CLICK_ALARM_TIME,
					DOUBLE_CLICK_ALARM_TIME );

		// Single click code
		// ...
		
		// nothing to do
	}





