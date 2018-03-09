
void key_NUMERIC_QUADRANT_velo_STEP( keyNdx ){

	unsigned char row = 0;
	unsigned char col = 0;

	// Work on the page under the grid cursor
	Pagestruct* target_page = &Page_repository[GRID_CURSOR];
	
	// Depending on who is asking the coordinates come from different sources
	if ( zoom_level == zoomSTEP ){

		row = target_page->stepSelectionSingleRow;
		col = target_page->stepSelectionSingleCol;					
	}
	else{

		// Base coordinates of the step
		row = my_bit2ndx( target_page->trackSelection );
		col = is_pressed_rowzero() - 1;
	}


	// Kill the event offset
	target_page->Track[row]->event_offset[ATTR_VELOCITY] = 0;

	// D O U B L E - C L I C K
	if ((DOUBLE_CLICK_TARGET == keyNdx) 
			&& (DOUBLE_CLICK_TIMER > DOUBLE_CLICK_ALARM_SENSITIVITY)) {

		// Double click code
		target_page->Step[row][col]->attribute[ATTR_VELOCITY] = 
							BK_KEY_to_xdx( keyNdx ) *10 
							- target_page->Track[row]->attribute[ATTR_VELOCITY]; 

		// Stay within bounds
		if ( target_page->Step[row][col]->attribute[ATTR_VELOCITY] > STEP_MAX_VELOCITY ){
			target_page->Step[row][col]->attribute[ATTR_VELOCITY] = STEP_MAX_VELOCITY;
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
		target_page->Step[row][col]->attribute[ATTR_VELOCITY] = 

			((( target_page->Track[row]->attribute[ATTR_VELOCITY]
			    + target_page->Step[row][col]->attribute[ATTR_VELOCITY]) /10 ) *10 )
			+ BK_KEY_to_xdx( keyNdx )
			- target_page->Track[row]->attribute[ATTR_VELOCITY];

		// Stay within bounds
		if ( target_page->Step[row][col]->attribute[ATTR_VELOCITY] > STEP_MAX_VELOCITY ){
			target_page->Step[row][col]->attribute[ATTR_VELOCITY] = STEP_MAX_VELOCITY;
		}
		
	} // SINGLE CLICK

}



void key_NUMERIC_QUADRANT_STEP_VEL_interval( keyNdx ){

	unsigned char row = 0;
	unsigned char col = 0;

	// Work on the page under the grid cursor
	Pagestruct* target_page = &Page_repository[GRID_CURSOR];

	
	// Depending on who is asking the coordinates come from different sources
	if ( zoom_level == zoomSTEP ){

		row = target_page->stepSelectionSingleRow;
		col = target_page->stepSelectionSingleCol;					
	}
	else{

		// Base coordinates of the step
		row = my_bit2ndx( target_page->trackSelection );
		col = is_pressed_rowzero() - 1;
	}


	// Kill the event offset
	target_page->Track[row]->event_offset[ATTR_VELOCITY] = 0;

	// D O U B L E - C L I C K
	if ((DOUBLE_CLICK_TARGET == keyNdx) 
			&& (DOUBLE_CLICK_TIMER > DOUBLE_CLICK_ALARM_SENSITIVITY)) {

		// Double click code
		target_page->Track[row]->event_max[ATTR_VELOCITY] = BK_KEY_to_xdx( keyNdx ) *10 ; 

		// Stay within bounds
		if ( target_page->Track[row]->event_max[ATTR_VELOCITY] > TRACK_MAX_VELOCITY ){
			target_page->Track[row]->event_max[ATTR_VELOCITY] = TRACK_MAX_VELOCITY;
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
		target_page->Track[row]->event_max[ATTR_VELOCITY] = 

			(( target_page->Track[row]->event_max[ATTR_VELOCITY] /10 ) *10 )
			+ BK_KEY_to_xdx( keyNdx );

		// Stay within bounds
		if ( target_page->Track[row]->event_max[ATTR_VELOCITY] > TRACK_MAX_VELOCITY ){
			target_page->Track[row]->event_max[ATTR_VELOCITY] = TRACK_MAX_VELOCITY;
		}
		
	} // SINGLE CLICK

}



void key_NUMERIC_QUADRANT_STEP_PIT_interval( keyNdx ){

	unsigned char row = 0;
	unsigned char col = 0;

	// Work on the page under the grid cursor
	Pagestruct* target_page = &Page_repository[GRID_CURSOR];

	
	// Depending on who is asking the coordinates come from different sources
	if ( zoom_level == zoomSTEP ){

		row = target_page->stepSelectionSingleRow;
		col = target_page->stepSelectionSingleCol;					
	}
	else{

		// Base coordinates of the step
		row = my_bit2ndx( target_page->trackSelection );
		col = is_pressed_rowzero() - 1;
	}


	// Kill the event offset
	target_page->Track[row]->event_offset[ATTR_PITCH] = 0;

	// D O U B L E - C L I C K
	if ((DOUBLE_CLICK_TARGET == keyNdx) 
			&& (DOUBLE_CLICK_TIMER > DOUBLE_CLICK_ALARM_SENSITIVITY)) {

		// Double click code
		target_page->Track[row]->event_max[ATTR_PITCH] = BK_KEY_to_xdx( keyNdx ) *10 ; 

		// Stay within bounds
		if ( target_page->Track[row]->event_max[ATTR_PITCH] > TRACK_MAX_PITCH ){
			target_page->Track[row]->event_max[ATTR_PITCH] = TRACK_MAX_PITCH;
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
		target_page->Track[row]->event_max[ATTR_PITCH] = 

			(( target_page->Track[row]->event_max[ATTR_PITCH] /10 ) *10 )
			+ BK_KEY_to_xdx( keyNdx );

		// Stay within bounds
		if ( target_page->Track[row]->event_max[ATTR_VELOCITY] > TRACK_MAX_PITCH ){
			target_page->Track[row]->event_max[ATTR_VELOCITY] = TRACK_MAX_PITCH;
		}
		
	} // SINGLE CLICK

}
