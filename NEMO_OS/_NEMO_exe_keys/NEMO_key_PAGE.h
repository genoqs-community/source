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


//
//
//// Key execution code for DEFAULT keymode
//
//


void key_exec_PAGE( unsigned short keyNdx ){

	unsigned char row = 0;
	unsigned char col = 0;

	unsigned short i, j, k, m = 0;

	Trackstruct* current_track = 0;
	unsigned short temp = 0;

	// Work on the page under the grid cursor
	Pagestruct* target_page = &Page_repository[GRID_CURSOR];


	//
	// TRACK_SELECTORS
	//

	// Turn off the recording track
	if ((keyNdx > 0) && (keyNdx <= 10)) {

		// Assume there is at least one recording track
		if (	( Page_getTrackRecPattern(target_page) != 0 )
// This will disturb the track pitch assignment functionality
//			||	( target_page->REC_bit == ON )
			){

			// Turn the REC bit off, since we are now track recording
			target_page->REC_bit = OFF;

			// This is the pressed key index. Compute the track index
			i = keyNdx - 1;
			i = row_of_track( target_page, target_page->Track[i]->chain_data[PLAY] );

			// Check if we are not selecting a REC enabled track
			if ( (Page_getTrackRecPattern(target_page) & (1 << (keyNdx-1)) ) != 0 ){

				// Disable the recording on that track
				Page_setTrackRecPattern(target_page, 0);
				// target_page->track_REC_pattern = 0;
			}
			else{

				// Switch recording pattern to the pressed track selector' track
				// target_page->track_REC_pattern = (1 << i);
				Page_setTrackRecPattern(target_page, (1 << i));
			}
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
			return;

		} // target_page->stepSelection != 0

		else{

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
			if ((target_page->Track[keyNdx-1]->attr_MISC & (1<<1)) > 0) {

				// Allow it to play again by clearing second bit
				target_page->Track[keyNdx-1]->attr_MISC =
					target_page->Track[keyNdx-1]->attr_MISC & 0xFD;
			}


			// D O U B L E - C L I C K
			if ((DOUBLE_CLICK_TARGET == keyNdx)
					&& (DOUBLE_CLICK_TIMER > DOUBLE_CLICK_ALARM_SENSITIVITY)) {

					// Zoom into the track and make it the single one in selection
					target_page->trackSelection = (1<<(keyNdx-1));

					// Switch zoomlevel
					G_zoom_level = zoomTRACK;
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

		// Active step selection in page - export it!
		if (target_page->stepSelection != 0){

			// Remember the step selection pattern in page and void it for now
			export_stepSELpattern( target_page );
			return;
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

			// Recall of step selection has prio over recall of track selection
			if ( get_stepSEL_cardinality( target_page ) != 0 ){

				// Reactivate the last step selection
				import_stepSELpattern( target_page );
			}

			// If there is a stored track selection
			else if (target_page->trackSelectionStored != 0) {

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

			// This is a double click victim - Select all tracks - NEMO has 4
			target_page->trackSelection = 0x0f;
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
	if (	( 	( keyNdx >= 11 ) && ( keyNdx <= 185 )
			&& 	( ( (keyNdx-10) % 11 ) != 0 ) )
			// Operate only if no track is selected
		&&  ( target_page->trackSelection == 0 )
		){

		// Compute Step coordinates
		row = row_of( keyNdx );
		col = column_of( keyNdx );

		// Assignment of the MIX attribute
		if ( is_pressed_key( KEY_MIX_MASTER ) ){

			// Select the track attribute
			switch( col ){

				// VEL PIT LEN STA
				case 0: 	target_page->mixAttribute = NEMO_ATTR_PITCH;		break;
				case 1: 	target_page->mixAttribute = NEMO_ATTR_VELOCITY;		break;
				case 2: 	target_page->mixAttribute = NEMO_ATTR_LENGTH;		break;
				case 3:		target_page->mixAttribute = NEMO_ATTR_START;		break;
				// AMT GRV MCC
				case 5: 	target_page->mixAttribute = NEMO_ATTR_AMOUNT;		break;
				case 6: 	target_page->mixAttribute = NEMO_ATTR_GROOVE;		break;
				case 7: 	target_page->mixAttribute = NEMO_ATTR_MIDICC;		break;
				// MCH
				case 9:		target_page->mixAttribute = NEMO_ATTR_MIDICH;		break;
				// POS DIR
				case 11: 	target_page->mixAttribute = NEMO_ATTR_POSITION;		break;
				case 12:	target_page->mixAttribute = NEMO_ATTR_DIRECTION;	break;
			}

			return;
		}



		// Check if the PREVIEW Mode button is pressed
		if (	(	( target_page->editorMode == PREVIEW )
				||	( target_page->editorMode == PREVIEW_PERFORM ) )
			){

			// Play the Step information as it would be played by the player
			// or modify the length of the step that was pressed first - JohnK style
			if ( page_preview_step == NULL ){

				// Remember the preview step pressed and its column
				page_preview_step = target_page->Step[row][col];
				page_preview_step_col = col;
				page_preview_step_row = row;

				// And play it in preview..
				if ( target_page->editorMode == PREVIEW ){
					PLAYER_preview_step( target_page, row, col );
				}
			}
			else{

				// Preview Step selected - modify its length according to pressed column
				adjust_preview_stepLEN( col );
			}
		}

		// Do not act on steps when in BIRDSEYE - this is handled in its own file
		if ( target_page->OPS_mode != BIRDSEYE ){

			// Needs some more elaboration but works for now
			target_page->page_clear = OFF;

			// Interpret the button press in the conventional way - step toggle
			interpret_matrix_stepkey( row, col, target_page );
		}

		// d_iag_printf( "0 trackpattern:%d\n", Page_get_trackpattern( target_page, row ) );
		// my_print_bits( Page_get_trackpattern( target_page, row ) );

	} // Matrix key pressed



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

	//
	// RECORD KEY
	//
	// REC alone toggles Track record mode and disables Page record mode.
	// PAGE+REC toggles Page record mode and disables Track record mode.
	// TRACK+REC is handled elsewhere.

	if ( keyNdx == KEY_RECORD ){

		// Ignore the key event if a track selection is active. It will be handled in key_PAGE_sel_TRACK.h.
		if ( !target_page->trackSelection ) {

			// If PAGE+REC is pressed, disable Track record mode and toggle page record mode.
			if ( is_pressed_key( KEY_ZOOM_PAGE ) ) {
				G_track_rec_bit = OFF;
				target_page->REC_bit ^= ON;
			}

			// If just REC is pressed, disable Page record mode, and toggle Track record mode.
			else {
				target_page->REC_bit = OFF;
				G_track_rec_bit ^= ON;
			}
		}
	}


	// GRID mode switch
	if (keyNdx == KEY_ZOOM_GRID) {

		GRID_play_mode = GRID_MIX;
		G_zoom_level = zoomGRID;
	}


	///////////////////////////////////////////////////////////////////////////
	// So far was the KEY_page base.. now come the finer nuances..
	//


	// No track or step is selected. Page is selection-blank.
	if (   ( target_page->trackSelection == 0)
		&& ( target_page->stepSelection  == 0)  ){

		// The PAGE contains no active selection
		switch( target_page->OPS_mode ){

			// BIRDSEYE is different -> only accepts page related stuff
			case BIRDSEYE:

				#include "../_NEMO_exe_keys/NEMO_key_PAGE_sel_NONE_BIRDSEYE.h"		// OK
				break;

			default:

				#include "../_NEMO_exe_keys/NEMO_key_PAGE_sel_NONE.h"				// OK
				break;
		} // switch( target_page->OPS_mode
	}

	// At least one track is selected
	else if ( target_page->trackSelection != 0 ){

		#include "../_NEMO_exe_keys/NEMO_key_PAGE_sel_TRACK.h"						// OK
	}

	// At least one step is selected
	else if ( target_page->stepSelection != 0 ){

		#include "../_NEMO_exe_keys/NEMO_key_PAGE_sel_STEP.h"						// OK
	}


// d_iag_printf( "2 trackpattern:%d\n", Page_get_trackpattern( target_page, row ) );
// my_print_bits( Page_get_trackpattern( target_page, row ) );

}



