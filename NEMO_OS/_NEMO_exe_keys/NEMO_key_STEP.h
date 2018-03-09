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



void step_enter_range( 				signed char* target_val,
									signed char target_max,
									signed char target_default,
									unsigned int keyNdx,
									unsigned char row,
									unsigned char col ){

	// D O U B L E - C L I C K
	if ((DOUBLE_CLICK_TARGET == keyNdx)
			&& (DOUBLE_CLICK_TIMER > DOUBLE_CLICK_ALARM_SENSITIVITY)) {

		// Double click code:
		// Set GROOVE attribute value to 0
		*target_val = target_default;
	}
	// SINGLE CLICK
	else if (DOUBLE_CLICK_TARGET == 0) {

		DOUBLE_CLICK_TARGET = keyNdx;
		DOUBLE_CLICK_TIMER = ON;
		// Start the Double click Alarm
		cyg_alarm_initialize(
			doubleClickAlarm_hdl,
			cyg_current_time() + DOUBLE_CLICK_ALARM_TIME,
			DOUBLE_CLICK_ALARM_TIME );

		// Set the new value of the GROOVE attribute - on the same port.
		*target_val = normalize( col+1, 0, target_max );
	}
}


// Value entry in Step mode
void step_enter_value_PITVELAMT( 	signed char* target_val,
									signed char target_max,
									signed char target_default,
									unsigned int keyNdx,
									unsigned char row,
									unsigned char col ){

	unsigned char j	  	= 0;
	unsigned char i	  	= 0;
	unsigned char sign	= POS;

	// D O U B L E - C L I C K
	if ((DOUBLE_CLICK_TARGET == keyNdx)
			&& (DOUBLE_CLICK_TIMER > DOUBLE_CLICK_ALARM_SENSITIVITY)) {

		// Double click code:
		// Break up original value in tens and ones.
		i = *target_val / 10;
		j = *target_val % 10;

		if ( col < 12 ){

			// Compute new tens
			i = 10 * (col + 1);

			// Get new ones value, leave it as it is otherwise
			if ( col < 9 ){
				// j = col + 1;
				j = 0;
			}

			if ( *target_val < 0 ){
				sign = NEG;
				*target_val *= -1;
			}

			// Write the final value into the attribute
			*target_val = normalize( i + j, 0, target_max );

			// Finally build in the negative sign, as needed
			if ( sign == NEG ){
				*target_val *= -1;
				sign = POS;
			}
		}
		else{
			// Write the default value into the attribute
			*target_val = target_default;
		}
	}
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
		if ( col < 9 ){

			// When negtive remember the sign, transform, and operate normally
			if ( *target_val < 0 ){
				sign = NEG;
				*target_val *= -1;
			}

			if ( *target_val >= target_max ){
				i = 0;
			}
			else {
				i = *target_val / 10;
			}

			*target_val = normalize( i*10 + col + 1, 0, target_max );

			// Build in the negative sign, as needed
			if ( sign == NEG ){
				*target_val *= -1;
				sign = POS;
			}
		}
		else if ( col == 15 ){
			// Modify the sign of the number
			*target_val *= -1 ;
		}
	}
}


// Value entry in Step mode
void step_enter_value_LEN( 	unsigned char* target_val,
							unsigned char target_max,
							unsigned char target_default,
							unsigned int keyNdx,
							unsigned char row,
							unsigned char col ){

	unsigned char j	  	= 0;
	unsigned char i	  	= 0;

	// D O U B L E - C L I C K
	if ((DOUBLE_CLICK_TARGET == keyNdx)
			&& (DOUBLE_CLICK_TIMER > DOUBLE_CLICK_ALARM_SENSITIVITY)) {

		// Double click code:
		// Break up original value in tens and ones.
		i = *target_val / 12;
		j = *target_val % 12;

		if ( col < 16 ){

			// Compute new tens
			i = 12 * (col + 1);

			// Get new ones value, leave it as it is otherwise
			if ( col < 12 ){
				// j = col + 1;
				j = 0;
			}

			// Write the final value into the attribute
			*target_val = normalize( i + j, 0, target_max );
		}
		else{
			// Write the default value into the attribute
			*target_val = target_default;
		}
	}
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
		if ( col < 11 ){

			if ( *target_val >= target_max ){
				i = 0;
			}
			else {
				i = *target_val / 12;
			}

			*target_val = normalize( i*12 + col + 1, 0, target_max );
		}
	}
}



// Value entry in Step mode
void step_enter_value_MCC( 	signed char* target_val,
							signed char target_max,
							signed char target_default,
							unsigned int keyNdx,
							unsigned char row,
							unsigned char col ){

	unsigned char j	  	= 0;
	unsigned char i	  	= 0;

	if ( NEMO_track_VFF == VFF_VALUE ){

		// D O U B L E - C L I C K
		if ((DOUBLE_CLICK_TARGET == keyNdx)
				&& (DOUBLE_CLICK_TIMER > DOUBLE_CLICK_ALARM_SENSITIVITY)) {

			// Double click code:
			// Break up original value in tens and ones.
			i = *target_val / 10;
			j = *target_val % 10;

			if ( col < 12 ){

				// Compute new tens
				i = 10 * (col + 1);
				// Get new ones value, leave it as it is otherwise
				if ( col < 9 ){
					j = 0;
				}
				// Write the final value into the attribute
				*target_val = normalize( i + j, 0, (col+1)*10 );
			}
			else{
				// Write the final value into the attribute
				*target_val = TRACK_DEF_MIDICC;
			}
		}
		// SINGLE-CLICK
		else if (DOUBLE_CLICK_TARGET == 0) {

			DOUBLE_CLICK_TARGET = keyNdx;
			DOUBLE_CLICK_TIMER = ON;
			// Start the Double click Alarm
			cyg_alarm_initialize(
				doubleClickAlarm_hdl,
				cyg_current_time() + DOUBLE_CLICK_ALARM_TIME,
				DOUBLE_CLICK_ALARM_TIME );

			// Single click code:
			if ( col < 9 ){
				// Modify the ones value of the MIDICC. 128 is a special value.
				if ( *target_val >= 127 ){
					i = 0;
				}
				else {
					i = *target_val / 10;
				}

				*target_val = normalize( i*10 + col + 1, 0, 127 );
			}
		}
	}
	else{

		// D O U B L E - C L I C K
		if ((DOUBLE_CLICK_TARGET == keyNdx)
				&& (DOUBLE_CLICK_TIMER > DOUBLE_CLICK_ALARM_SENSITIVITY)) {

			// Double click code:
			// Adjust the factor
			*target_val = 0;
		}
		// SINGLE CLICK
		else if ( DOUBLE_CLICK_TARGET == 0 ){

			DOUBLE_CLICK_TARGET = keyNdx;
			DOUBLE_CLICK_TIMER = ON;
			// Start the Double click Alarm
			cyg_alarm_initialize(
				doubleClickAlarm_hdl,
				cyg_current_time() + DOUBLE_CLICK_ALARM_TIME,
				DOUBLE_CLICK_ALARM_TIME );

			// Single click code:
			*target_val = col + 1;
		}
	}

}




// Key execution in STEP mode
void key_exec_STEP( unsigned int keyNdx ){

	unsigned char row 	= 0;
	unsigned char col 	= 0;
	unsigned char i	  	= 0;
	unsigned char temp 	= 0;

	// Work on the page under the grid cursor
	Pagestruct* target_page = &Page_repository[GRID_CURSOR];

	// ..and on the selected step
	Stepstruct* target_step = target_page->Step	[target_page->stepSelectionSingleRow]
												[target_page->stepSelectionSingleCol];

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

			Step_set_status( target_page->Step[row][col], STEPSTAT_SELECT, ON );

			// ..and increment the counter of steps selected
			target_page->stepSelection++;

			// Remember STEP COORDINATES
			target_page->stepSelectionSingleRow = row;
			target_page->stepSelectionSingleCol = col;

		} // Unselected step was pressed

		// Do not continue further
		return;
	}



	// MATRIX KEYS
	// If the key pressed is in the Matrix
	if ( (keyNdx > 10) && (keyNdx <= 185) && (((keyNdx-10) % 11) != 0) ){

		// ROW I-III
		// Functions depending on VER MODE
		switch( NEMO_step_VER ){

			case VER_VALUE:
				#include "NEMO_key_STEP__MATRIX__VALUE.h"
				break;
			case VER_EVENT:
				#include "NEMO_key_STEP__MATRIX__EVENT.h"
				break;
			case VER_RANGE:
				#include "NEMO_key_STEP__MATRIX__RANGE.h"
				break;
			case VER_CHORD:
				#include "NEMO_key_STEP__MATRIX__CHORD.h"
				break;
		}

		// ROW IV
		// Allows step selection
		// The functionality should be identical to the one in Page mode.
		if ( 	(( keyNdx - NEMO_ROW_IV ) % 11) == 0
			){

			// Row of target track and column of target step
			row = my_bit2ndx( target_page->trackSelection );
			col = (( keyNdx - NEMO_ROW_IV ) / 11) - 1 ;

			// Needs some more elaboration but works for now
			target_page->page_clear = OFF;

			// Check if the step is selected. If yes, toggle it.
			switch( Step_get_status( target_page->Step	[ target_page->stepSelectionSingleRow ]
														[ col ], STEPSTAT_SELECT )
				){

				case ON:
					// Toggle the step on and off - but operate on unskipped steps only
					if ( Step_get_status(
							target_page->Step[ target_page->stepSelectionSingleRow ]
										 	 [ target_page->stepSelectionSingleCol ], STEPSTAT_SKIP ) == OFF ){
						switch( Step_get_status(
							target_page->Step[ target_page->stepSelectionSingleRow ]
										 	 [ target_page->stepSelectionSingleCol ], STEPSTAT_TOGGLE ) ){
							case ON:
								Step_set_status(
									target_page->Step[ target_page->stepSelectionSingleRow ]
													 [ target_page->stepSelectionSingleCol ], STEPSTAT_TOGGLE, OFF );
								break;
							case OFF:
								Step_set_status(
									target_page->Step[ target_page->stepSelectionSingleRow ]
													 [ target_page->stepSelectionSingleCol ], STEPSTAT_TOGGLE, ON );
								break;
						}
					}
					break;

				case OFF:
					// Code similar to one in the Octopus position line
					// Unselect the original step
					Step_set_status(
						target_page->Step[ target_page->stepSelectionSingleRow ]
										 [ target_page->stepSelectionSingleCol ], STEPSTAT_SELECT, OFF );

					// Compute new col coordinates - selected step in the same row;
					// target_page->stepSelectionSingleRow = row_of( keyNdx );
					target_page->stepSelectionSingleCol = col;

					// Select the new step
					Step_set_status(
						target_page->Step[ target_page->stepSelectionSingleRow ]
										 [ target_page->stepSelectionSingleCol ], STEPSTAT_SELECT, ON );

					// Secure the jump to a step that has no event set
					if ( NEMO_step_VER == VER_RANGE ){

						if ( Step_get_status( target_page->Step [ target_page->stepSelectionSingleRow ]
									  				        	[ target_page->stepSelectionSingleCol ],
									 	   STEPSTAT_EVENT ) != ON ){
							NEMO_step_VER = VER_EVENT;
						}
					}
					break;
			}
		} // ROW IV Key

	} // Key in the Matrix



	// //////////////
	// TRACK MUTATORS
	//

	// ENTITY MODE:
	if (target_page->trackAttributeSelection == OFF) {

		// CLEAR function
		if ( keyNdx == KEY_CLEAR ) {

			// Clear track fully
			Step_init( target_page->Step[ target_page->stepSelectionSingleRow ]
								 		[ target_page->stepSelectionSingleCol ] );

			Step_set_status(
				target_page->Step[ target_page->stepSelectionSingleRow ]
								 [ target_page->stepSelectionSingleCol ], STEPSTAT_SELECT, ON );
		}

	} // Entity mode operations



	//
	// ZOOM SELECTOR
	//
	if (keyNdx == KEY_ZOOM_GRID) {
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
		return;
	}

	if (keyNdx == KEY_ZOOM_PAGE) {
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
		return;
	}

	if ( keyNdx == KEY_ZOOM_TRACK ){
		// Unselect the step
		row = target_page->stepSelectionSingleRow;
		col = target_page->stepSelectionSingleCol;
		Step_set_status( target_page->Step[row][col], STEPSTAT_SELECT, OFF );

		// Select the track that the zoomed step belongs to
		target_page->trackSelection = ( 1 << target_page->stepSelectionSingleRow );

		// Clear the STEP COORDINATES
		target_page->stepSelectionSingleRow = 0;
		target_page->stepSelectionSingleCol = 0;
		target_page->stepSelection = 0;

		G_zoom_level = zoomTRACK;
		return;
	}

	// PLAY MODE Switcher
	// Snapshot key: single click from OFF turns it on.
	// Single click from ON turns it back OFF
	// Double click from ON stores the state like it is.
	if ( keyNdx == KEY_ZOOM_PLAY ){

		// D O U B L E - C L I C K  C O N S T R U C T
		// DOUBLE CLICK SCENARIO
		if ((DOUBLE_CLICK_TARGET == keyNdx)
				&& (DOUBLE_CLICK_TIMER > DOUBLE_CLICK_ALARM_SENSITIVITY)) {

			// Double click code
			// ...
			// Fetches the old status back.
			// Toggles the play mode status
			toggle_PLAY_MODE( OFF );
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
			// Leaves the PLAY changes permanent in the page
			switch( PLAY_MODE_STATUS ){
				case ON:
					PLAY_MODE_STATUS = OFF;
					break;

				case OFF:
					toggle_PLAY_MODE( ON );
					break;
			}
		}
	}



	//
	// BIG KNOB KEYS
	//
	if ( keyNdx == KEY_PROGRAM ){

		// If program change is already selected
		if ( CHANGE_BANK_OR_PGM  == PGM ){

			// Send the program change Value on the selected track(s)
			for (i=0; i < MATRIX_NROF_ROWS; i++) {

				if (	( (target_page->trackSelection & (1<<i)) != 0 )
					&&	( ((unsigned char) target_page->Track[ my_bit2ndx( target_page->trackSelection )]->program_change) > 0 )
					){

					MIDI_send( 	MIDI_PGMCH,
								target_page->Track[i]->attr_MCH
								+ target_page->Track[i]->event_offset[ATTR_MIDICH],
								target_page->Track[ my_bit2ndx( target_page->trackSelection )]->program_change - 1,
								0 );
				}
			}
		} // Program is selected

		else{

			// Select the program mode
			CHANGE_BANK_OR_PGM = PGM;
		}
	}



	//
	// FOLLOW_TRACK
	//
	if (	( keyNdx == KEY_FOLLOW )
		&&	( PLAY_MODE_STATUS == OFF )
		){

		// Toggle the FOLLOW_TRACK switch
		if (	( follow_flag == FOLLOW_TRACK )
			){

			follow_flag = OFF;
		}
		else{

			follow_flag = FOLLOW_TRACK;
		}
	}



	//
	// ALIGN LAUFLICHT
	//
	if (	( keyNdx == KEY_ALIGN )
		){
		// Aligns only the target_page to the global play data
		set_page_locators( target_page, G_global_locator, G_TTC_abs_value );
	}


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


	//
	// TRANSPORT functions - when nothing is selected in page
	//
	if ( G_clock_source != EXT ){

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

	// ALIGN LAUFLICHT
	if (	( keyNdx == KEY_PLAY1 )
		&&	( G_run_bit == ON )
		){

		// Aligns only the target_page to the global play data
		set_page_locators( target_page, G_global_locator, G_TTC_abs_value );
	} // KEY_ALIGN was pressed


	// COPY STEP
	if ( keyNdx == KEY_COPY ){
		if (target_page->stepSelection != 1){
			return;
		}

		// Coordinates of already selected step
		row = target_page->stepSelectionSingleRow;
		col = target_page->stepSelectionSingleCol;

		// Copy the step pointer to the buffer
		STEP_COPY_BUFFER = target_page->Step[row][col];
	}


	// PASTE STEP if one way copy'ed
	if ( keyNdx == KEY_PASTE ){
		if (target_page->stepSelection != 1){
			return;
		}

		// Coordinates of already selected step
		row = target_page->stepSelectionSingleRow;
		col = target_page->stepSelectionSingleCol;

		// Copy step data from buffer to pointer
		if ( STEP_COPY_BUFFER != NULL ){

			Step_copy( STEP_COPY_BUFFER, target_page->Step[row][col]);
	#ifdef COPY_BUFFER_FRESH
			STEP_COPY_BUFFER = NULL;
	#endif
		}
	}
}
