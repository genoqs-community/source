
void key_NUMERIC_QUADRANT_velo_STEP( keyNdx ){

	unsigned char row = 0;
	unsigned char col = 0;

	
	// Depending on who is asking the coordinates come from different sources
	if ( zoom_level == zoomSTEP ){

		row = VIEWER_page->stepSelectionSingleRow;
		col = VIEWER_page->stepSelectionSingleCol;					
	}
	else{

		// Base coordinates of the step
		row = my_bit2ndx( VIEWER_page->trackSelection );
		col = is_pressed_rowzero() - 1;
	}


	// Kill the event offset
	VIEWER_page->Track[row]->event_VEL = 0;

	// D O U B L E - C L I C K
	if ((DOUBLE_CLICK_TARGET == keyNdx) 
			&& (DOUBLE_CLICK_TIMER > DOUBLE_CLICK_ALARM_SENSITIVITY)) {

		// Double click code
		VIEWER_page->Step[row][col]->attribute[ATTR_VELOCITY] = 
							BK_KEY_to_xdx( keyNdx ) *10 
							- VIEWER_page->Track[row]->attribute[ATTR_VELOCITY]; 

		// Stay within bounds
		if ( VIEWER_page->Step[row][col]->attribute[ATTR_VELOCITY] > STEP_MAX_VELOCITY ){
			VIEWER_page->Step[row][col]->attribute[ATTR_VELOCITY] = STEP_MAX_VELOCITY;
		}

	} // Double click code


	// SINGLE CLICK
	else if (DOUBLE_CLICK_TARGET == 0) {
		
		DOUBLE_CLICK_TARGET = keyNdx;
		DOUBLE_CLICK_TIMER = ON;
		// Start the Double click Alarm
		cyg_alarm_initialize(	
			doubleClickAlarm_hdl, 
			cyg_current_time() + DOUBLE_CLICK_ALARM_TIME,
			DOUBLE_CLICK_ALARM_TIME );

		// Single click code:
		// Work on the sum and split it up again.
		VIEWER_page->Step[row][col]->attribute[ATTR_VELOCITY] = 

			((( VIEWER_page->Track[row]->attribute[ATTR_VELOCITY]
			    + VIEWER_page->Step[row][col]->attribute[ATTR_VELOCITY]) /10 ) *10 )
			+ BK_KEY_to_xdx( keyNdx )
			- VIEWER_page->Track[row]->attribute[ATTR_VELOCITY];

		// Stay within bounds
		if ( VIEWER_page->Step[row][col]->attribute[ATTR_VELOCITY] > STEP_MAX_VELOCITY ){
			VIEWER_page->Step[row][col]->attribute[ATTR_VELOCITY] = STEP_MAX_VELOCITY;
		}
		
	} // SINGLE CLICK

}



void key_NUMERIC_QUADRANT_STEP_VEL_interval( keyNdx ){

	unsigned char row = 0;
	unsigned char col = 0;

	
	// Depending on who is asking the coordinates come from different sources
	if ( zoom_level == zoomSTEP ){

		row = VIEWER_page->stepSelectionSingleRow;
		col = VIEWER_page->stepSelectionSingleCol;					
	}
	else{

		// Base coordinates of the step
		row = my_bit2ndx( VIEWER_page->trackSelection );
		col = is_pressed_rowzero() - 1;
	}


	// Kill the event offset
	VIEWER_page->Track[row]->event_VEL = 0;

	// D O U B L E - C L I C K
	if ((DOUBLE_CLICK_TARGET == keyNdx) 
			&& (DOUBLE_CLICK_TIMER > DOUBLE_CLICK_ALARM_SENSITIVITY)) {

		// Double click code
		VIEWER_page->Track[row]->range_VEL = BK_KEY_to_xdx( keyNdx ) *10 ; 

		// Stay within bounds
		if ( VIEWER_page->Track[row]->range_VEL > TRACK_MAX_VELOCITY ){
			VIEWER_page->Track[row]->range_VEL = TRACK_MAX_VELOCITY;
		}

	} // Double click code


	// SINGLE CLICK
	else if (DOUBLE_CLICK_TARGET == 0) {
		
		DOUBLE_CLICK_TARGET = keyNdx;
		DOUBLE_CLICK_TIMER = ON;
		// Start the Double click Alarm
		cyg_alarm_initialize(	
			doubleClickAlarm_hdl, 
			cyg_current_time() + DOUBLE_CLICK_ALARM_TIME,
			DOUBLE_CLICK_ALARM_TIME );

		// Single click code:
		// Work on the sum and split it up again.
		VIEWER_page->Track[row]->range_VEL = 

			(( VIEWER_page->Track[row]->range_VEL /10 ) *10 )
			+ BK_KEY_to_xdx( keyNdx );

		// Stay within bounds
		if ( VIEWER_page->Track[row]->range_VEL > TRACK_MAX_VELOCITY ){
			VIEWER_page->Track[row]->range_VEL = TRACK_MAX_VELOCITY;
		}
		
	} // SINGLE CLICK

}



void key_NUMERIC_QUADRANT_STEP_PIT_interval( keyNdx ){

	unsigned char row = 0;
	unsigned char col = 0;

	
	// Depending on who is asking the coordinates come from different sources
	if ( zoom_level == zoomSTEP ){

		row = VIEWER_page->stepSelectionSingleRow;
		col = VIEWER_page->stepSelectionSingleCol;					
	}
	else{

		// Base coordinates of the step
		row = my_bit2ndx( VIEWER_page->trackSelection );
		col = is_pressed_rowzero() - 1;
	}


	// Kill the event offset
	VIEWER_page->Track[row]->event_PIT = 0;

	// D O U B L E - C L I C K
	if ((DOUBLE_CLICK_TARGET == keyNdx) 
			&& (DOUBLE_CLICK_TIMER > DOUBLE_CLICK_ALARM_SENSITIVITY)) {

		// Double click code
		VIEWER_page->Track[row]->range_PIT = BK_KEY_to_xdx( keyNdx ) *10 ; 

		// Stay within bounds
		if ( VIEWER_page->Track[row]->range_PIT > TRACK_MAX_PITCH ){
			VIEWER_page->Track[row]->range_PIT = TRACK_MAX_PITCH;
		}

	} // Double click code


	// SINGLE CLICK
	else if (DOUBLE_CLICK_TARGET == 0) {
		
		DOUBLE_CLICK_TARGET = keyNdx;
		DOUBLE_CLICK_TIMER = ON;
		// Start the Double click Alarm
		cyg_alarm_initialize(	
			doubleClickAlarm_hdl, 
			cyg_current_time() + DOUBLE_CLICK_ALARM_TIME,
			DOUBLE_CLICK_ALARM_TIME );

		// Single click code:
		// Work on the sum and split it up again.
		VIEWER_page->Track[row]->range_PIT = 

			(( VIEWER_page->Track[row]->range_PIT /10 ) *10 )
			+ BK_KEY_to_xdx( keyNdx );

		// Stay within bounds
		if ( VIEWER_page->Track[row]->range_VEL > TRACK_MAX_PITCH ){
			VIEWER_page->Track[row]->range_VEL = TRACK_MAX_PITCH;
		}
		
	} // SINGLE CLICK

}
