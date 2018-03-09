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



void key_NUMERIC_QUADRANT_velo_TRACK( keyNdx ){

	unsigned char row = 0;

	Trackstruct* target_track = NULL;

	// Work on the page under the grid cursor
	Pagestruct* target_page = &Page_repository[GRID_CURSOR];


	// Base coordinates of track
	row = my_bit2ndx( target_page->trackSelection );
	target_track = target_page->Track[row];


	// Kill the event offset
	target_page->Track[row]->event_offset[ATTR_VELOCITY] = 0;


	// D O U B L E - C L I C K
	if ((DOUBLE_CLICK_TARGET == keyNdx)
			&& (DOUBLE_CLICK_TIMER > DOUBLE_CLICK_ALARM_SENSITIVITY)) {

		// Double click code
		target_track->attr_VEL = 	BK_KEY_to_xdx( keyNdx ) *10;

		// Stay within bounds
		target_track->attr_VEL =
			normalize( target_track->attr_VEL, TRACK_MIN_VELOCITY, TRACK_MAX_VELOCITY );

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
		target_track->attr_VEL =
			((( target_page->Track[row]->attr_VEL) /10 ) *10 )
			+ BK_KEY_to_xdx( keyNdx );

		// Stay within bounds
		target_track->attr_VEL =
			normalize( target_track->attr_VEL, TRACK_MIN_VELOCITY, TRACK_MAX_VELOCITY );

	} // SINGLE CLICK

}




void key_NUMERIC_QUADRANT_velo_STEP( keyNdx ){

	// Only work if keyNdx is one of the Big Knob (tempo, etc) keys.
	if ( keyNdx == KEY_BK1
			|| keyNdx == KEY_BK2
			|| keyNdx == KEY_BK3
			|| keyNdx == KEY_BK4
			|| keyNdx == KEY_BK5
			|| keyNdx == KEY_BK6
			|| keyNdx == KEY_BK7
			|| keyNdx == KEY_BK8
			|| keyNdx == KEY_BK9
			|| keyNdx == KEY_BK100 ) {

		unsigned char row = 0;
		unsigned char col = 0;

		Stepstruct* target_step = NULL;

		// Work on the page under the grid cursor
		Pagestruct* target_page = &Page_repository[GRID_CURSOR];

		// Depending on who is asking the coordinates come from different sources
		if ( G_zoom_level == zoomSTEP ){

			row = target_page->stepSelectionSingleRow;
			col = target_page->stepSelectionSingleCol;
			target_step = target_page->Step[row][col];
		}

		// Page preview mode
		else if (	( G_zoom_level == zoomPAGE )
				&&	(	( target_page->editorMode == PREVIEW )
					||	( target_page->editorMode == PREVIEW_PERFORM )
					)
				&&	( page_preview_step != NULL )
		){

			row = page_preview_step_row;
			target_step = page_preview_step;
		}

		else{

			// Base coordinates of the step
			row = my_bit2ndx( target_page->trackSelection );
			col = is_pressed_rowzero() - 1;
			target_step = target_page->Step[row][col];
		}

		// Kill the event offset
		target_page->Track[row]->event_offset[ATTR_VELOCITY] = 0;


		// D O U B L E - C L I C K
		if ((DOUBLE_CLICK_TARGET == keyNdx)
				&& (DOUBLE_CLICK_TIMER > DOUBLE_CLICK_ALARM_SENSITIVITY)) {

			// Double click code
			target_step->attr_VEL = 	BK_KEY_to_xdx( keyNdx ) *10
			- target_page->Track[row]->attr_VEL;

			// Stay within bounds
			target_step->attr_VEL =
				normalize( target_step->attr_VEL, STEP_MIN_VELOCITY, STEP_MAX_VELOCITY );

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
			target_step->attr_VEL =

				((( target_page->Track[row]->attr_VEL
						+ target_step->attr_VEL) /10 ) *10 )
						+ BK_KEY_to_xdx( keyNdx )
						- target_page->Track[row]->attr_VEL;

			// Stay within bounds
			target_step->attr_VEL =
				normalize( target_step->attr_VEL, STEP_MIN_VELOCITY, STEP_MAX_VELOCITY );

		} // SINGLE CLICK
	}
}



void key_NUMERIC_QUADRANT_STEP_VEL_interval( keyNdx ){

	unsigned char row = 0;
	unsigned char col = 0;

	// Work on the page under the grid cursor
	Pagestruct* target_page = &Page_repository[GRID_CURSOR];


	// Depending on who is asking the coordinates come from different sources
	if ( G_zoom_level == zoomSTEP ){

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
	if ( G_zoom_level == zoomSTEP ){

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
