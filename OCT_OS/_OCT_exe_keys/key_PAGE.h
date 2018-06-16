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


//// DEBUG Player glitches in chains
//if ( keyNdx == KEY_RETURN ){
//	d_iag_printf( "G_TTC_abs_value:%d\n", G_TTC_abs_value );
//	for ( i=0; i<MATRIX_NROF_ROWS; i++ ){
//		d_iag_printf( "row%d LOC:%d TTC:%d\n", i, target_page->Track[i]->attr_LOCATOR, target_page->Track[i]->TTC );
//	}
//}



// Key execution code for DEFAULT keymode
	#ifdef FEATURE_ENABLE_SONG_UPE
	if (keyNdx == KEY_MUTE_MASTER){
		char selection = OFF;
		for( i=1; i <= 10; i++ ){
			if (is_pressed_key(i) == ON){ // is a ctrl buffer key pressed
				selection = ON;
				break;
			}
		}
		if ( selection == OFF ){
			if (G_rec_ctrl_page != NULL){
				G_unarm_ctrl = ON;
			}
		}
		for( i=0; i < 10; i++ ){
			if (is_pressed_key(i + 1) == ON){
				quick_assign_control_track(target_page, i);
				target_page->Track[i]->attr_STATUS = 0;
				break;
			}
		}
	}

	// toggle control track step buffer active
	for( i=0; i < 10; i++ ){
		if (keyNdx == (i + 1) ){
			if ( Track_get_MISC(target_page->Track[i], CONTROL_BIT ) && target_page->Track[i]->attr_STATUS == 127
					&& Page_get_skippattern( target_page, i ) != 65535) {
				target_page->Track[i]->attr_STATUS = 0;

			} else if ( Track_get_MISC(target_page->Track[i], CONTROL_BIT ) && target_page->Track[i]->attr_STATUS == 0
					&& Page_get_skippattern( target_page, i ) != 65535) {
				target_page->Track[i]->attr_STATUS = 127;
				// remove control track if there are no step buffers set
				for ( j=0; j < MATRIX_NROF_COLUMNS; j++ ){
					if ( Step_get_status( target_page->Step[i][j], STEPSTAT_SKIP ) == ON ){
						break;
					}
					if ( j == MATRIX_NROF_COLUMNS - 1 ){
						target_page->Track[i]->attr_STATUS = 0;
					}
				}
			}
			break;
		}
	}
	#endif
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

			// Toggle recording bits in the REC pattern
			// target_page->track_REC_pattern ^= (1 << i);

			// Check if we are not selecting a REC enabled track
			if ( (Page_getTrackRecPattern(target_page) & (1 << (keyNdx-1)) ) != 0 ){

				// Allow MCH to retain record status
				G_prev_rec_page_pattern = Page_getTrackRecPattern(target_page);
				G_prev_rec_page = target_page->pageNdx;

				// Disable the recording on that track
				Page_setTrackRecPattern( target_page, 0 );
				G_track_rec_bit = OFF;
			}
			else{

				// Switch recording pattern to the pressed track selector' track
				Page_setTrackRecPattern( target_page, (1 << i) );

				G_prev_rec_page_pattern = Page_getTrackRecPattern(target_page);
				G_prev_rec_page = target_page->pageNdx;
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
			break;

		} // target_page->stepSelection != 0

		else{

			if ( is_pressed_key( KEY_MIX_MASTER ) ){

				// Assign the new mix Attribute
				target_page->mixAttribute = keyNdx;

				target_page->trackSelection = 0;
				return;
			}
			#ifdef FEATURE_ENABLE_KB_TRANSPOSE
			//backup PIT as ghost pitch on selection
			target_page->Track[keyNdx-1]->attr_GST = target_page->Track[keyNdx-1]->attr_PIT;
			#endif
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

			#ifdef FEATURE_ENABLE_SONG_UPE
			if ( Track_get_MISC(target_page->Track[keyNdx-1], CONTROL_BIT) == OFF ) {
				// If this is a SINGLE PLAYMODE track
				if ((target_page->Track[keyNdx-1]->attr_MISC & (1<<1)) > 0) {

					// Allow it to play again by clearing second bit
					target_page->Track[keyNdx-1]->attr_MISC =
							target_page->Track[keyNdx-1]->attr_MISC & 0xFD;
				}
			}			
			#else
			// If this is a SINGLE PLAYMODE track
			if ((target_page->Track[keyNdx-1]->attr_MISC & (1<<1)) > 0) {

				// Allow it to play again by clearing second bit
				target_page->Track[keyNdx-1]->attr_MISC =
					target_page->Track[keyNdx-1]->attr_MISC & 0xFD;
			}
			#endif

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
	if (	( 	( keyNdx >= 11 ) && ( keyNdx <= 185 )
			&& 	( ( (keyNdx-10) % 11 ) != 0 ) )
			// Operate only if no track is selected
		&&  ( target_page->trackSelection == 0 )
		){

		// Compute Step coordinates
		row = row_of( keyNdx );
		col = column_of( keyNdx );

		// Check if EDIT mode
		if (	(	( target_page->editorMode == PREVIEW )
				||	( target_page->editorMode == PREVIEW_PERFORM )
				)
			&&	( target_page->OPS_mode != BIRDSEYE )
			){

			// Play the Step information as it would be played by the player
			// or modify the length of the step that was pressed first - JohnK style
			if ( page_preview_step == NULL ){

				// Remember the preview step pressed and its column
				page_preview_step = target_page->Step[row][col];
				page_preview_step_col = col;
				page_preview_step_row = row;

				switch( is_pressed_key( KEY_SELECT_MASTER ) ){

					case FALSE:
//						if ( Step_get_status( target_page->Step[row][col], STEPSTAT_SELECT ) == ON ){
						if ( target_page->stepSelection != 0 ){
							// Turns the step selection off
							interpret_matrix_stepkey( row, col, target_page );
						}
						else{
							// And play it in preview.. unless step is selected
							if ( target_page->editorMode == PREVIEW ){
								PLAYER_preview_step( target_page, row, col );
							}
						}
						break;

					case TRUE:
						// Allow to act as step selection
						if ( Step_get_status( target_page->Step[row][col], STEPSTAT_TOGGLE ) == ON ){
							interpret_matrix_stepkey( row, col, target_page );
						}
						break;
				}
			}
			else{

				// A preview Step is selected - modify its length according to pressed column
				adjust_preview_stepLEN( col );
			}
		}

		// Do not act on steps when in BIRDSEYE - this is handled in its own file
		else if ( target_page->OPS_mode != BIRDSEYE ){

			// Needs some more elaboration but works for now
			target_page->page_clear = OFF;

			// Interpret the button press in the conventional way - step toggle.
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
	// ..or the G_master_tempo key
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

//				if ( target_page->priv_track_REC_pattern != 0 ){
					G_track_rec_bit ^= ON;
//				}

			}
		}
	}


	// GRID mode switch
	if (keyNdx == KEY_ZOOM_GRID) {

		GRID_play_mode = GRID_MIX;
		G_zoom_level = zoomGRID;
	}


	// PLAY MODE
	if ( keyNdx == KEY_ZOOM_PLAY ){
		// Toggles the PLAY MODE status
		toggle_PLAY_MODE( PLAY_MODE_STATUS^1 );
	}


	// MIDI CC routing and pass through enabled
	if ( keyNdx == KEY_ZOOM_MAP ){
		G_midi_map_controller_mode ^= 1;
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

				#include "key_PAGE_sel_NONE_BIRDSEYE.h"		// OK
				break;

			default:

				#include "key_PAGE_sel_NONE.h"				// OK
				break;
		} // switch( target_page->OPS_mode
	}
	else if ( target_page->trackSelection != 0 ){

		#include "key_PAGE_sel_TRACK.h"						// OK
	}
	else if ( target_page->stepSelection != 0 ){

		#include "key_PAGE_sel_STEP.h"						// OK
	}











