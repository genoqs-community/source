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



	if (	( target_page->editorMode == PREVIEW )
		||	( target_page->editorMode == PREVIEW_PERFORM )
		){

		// Coordinates of already pressed step
		row = page_preview_step_row;
		col = page_preview_step_col;

		switch( keyNdx ){

		case KEY_COPY:
				if ( page_preview_step == NULL ){
					break;
				}

				// Copy the step pointer to the buffer
				STEP_COPY_BUFFER = target_page->Step[row][col];
				break;

		case KEY_PASTE:
				if (	( page_preview_step == NULL )
					||	(STEP_COPY_BUFFER == NULL)
					){
					break;
				}

				// Copy step data from buffer to pointer
				Step_copy( STEP_COPY_BUFFER, target_page->Step[row][col]);

#ifdef COPY_BUFFER_FRESH
				STEP_COPY_BUFFER = NULL;
#endif
				break;
		}
	}


	//
	// MIX MASTER
	//
	if (keyNdx == KEY_MIX_MASTER) {

		if ( page_preview_step != NULL ) return;

		// NEMO: act as MIXTGT selector
		target_page->mixTarget = MIXTGT_ATR;
	}



	//
	// MIX TARGET
	//

	// Toggle the Map0 status
	if ( (keyNdx == KEY_MIXTGT_USR0) ){

		if ( page_preview_step != NULL ) return;

		if (	( target_page->mixTarget == MIXTGT_USR1 )
			||	( target_page->mixTarget == MIXTGT_USR2 )
			||	( target_page->mixTarget == MIXTGT_USR3 )
			||	( target_page->mixTarget == MIXTGT_USR4 )
			||	( target_page->mixTarget == MIXTGT_USR5 )
			){

			// Toggle status only once a map is selected
			GRID_MAP0_status ^= 1;
		}
		else{
			target_page->mixTarget = MIXTGT_USR1;
		}
	}


	// Make sure we don't let this superimpose on step copy/paste
	if (	(		(target_page->editorMode == PREVIEW)
				|| 	(target_page->editorMode == PREVIEW_PERFORM) )
		&& 	( page_preview_step == NULL )
		){

		// Go directly to the stored step selection
		// STEP SELECTION PATTERN INDEX
		if (	(	(keyNdx == KEY_MIXTGT_USR1)
				||	(keyNdx == KEY_MIXTGT_USR2)
				||	(keyNdx == KEY_MIXTGT_USR3)
				||	(keyNdx == KEY_MIXTGT_USR4)
				||	(keyNdx == KEY_MIXTGT_USR5) )
			&&	(	is_pressed_key( KEY_SELECT_MASTER ))
			){

			// Export content to old index
			// export_stepSELpattern( target_page );

			// Set the new index
			switch( keyNdx ){
				case KEY_MIXTGT_USR1:	target_page->stepSELpattern_ndx = 0;	break;
				case KEY_MIXTGT_USR2:	target_page->stepSELpattern_ndx = 1;	break;
				case KEY_MIXTGT_USR3:	target_page->stepSELpattern_ndx = 2;	break;
				case KEY_MIXTGT_USR4:	target_page->stepSELpattern_ndx = 3;	break;
				case KEY_MIXTGT_USR5:	target_page->stepSELpattern_ndx = 4;	break;
			}

			// Import from new index
			import_stepSELpattern( target_page );
		}


		else if ( 	( (keyNdx == KEY_MIXTGT_ATR ) )
			||	( (keyNdx == KEY_MIXTGT_VOL ) )
			||	( (keyNdx == KEY_MIXTGT_PAN ) )
			||	( (keyNdx == KEY_MIXTGT_MOD ) )
			||	( (keyNdx == KEY_MIXTGT_EXP ) )
			){

			// D O U B L E - C L I C K  C O N S T R U C T
			// DOUBLE CLICK SCENARIO
			if ((DOUBLE_CLICK_TARGET == keyNdx)
					&& (DOUBLE_CLICK_TIMER > DOUBLE_CLICK_ALARM_SENSITIVITY)) {

				// Double click code
				// ...
				// Zoom into the clicked mix target

				// Select MIXER MAP MODE
				G_zoom_level = zoomMIXMAP;
				target_page->REC_bit = OFF;

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
				// Just simply switch to the chosen mix target.
				switch( keyNdx ){

					case KEY_MIXTGT_ATR: 	target_page->mixTarget = MIXTGT_ATR; 	break;

					// Sound control block: A, B, C, D -> changed to CC Sound Control blocks
	//				case KEY_MIXTGT_VOL: 	target_page->mixTarget = MIXTGT_VOL; 	break;
	//				case KEY_MIXTGT_PAN: 	target_page->mixTarget = MIXTGT_PAN; 	break;
	//				case KEY_MIXTGT_MOD: 	target_page->mixTarget = MIXTGT_MOD; 	break;
	//				case KEY_MIXTGT_EXP: 	target_page->mixTarget = MIXTGT_EXP; 	break;
					case KEY_MIXTGT_VOL: 	target_page->mixTarget = MIXTGT_USR1; 	break;
					case KEY_MIXTGT_PAN: 	target_page->mixTarget = MIXTGT_USR2; 	break;
					case KEY_MIXTGT_MOD: 	target_page->mixTarget = MIXTGT_USR3; 	break;
					case KEY_MIXTGT_EXP: 	target_page->mixTarget = MIXTGT_USR4; 	break;
				}
			} // Single click scenario
		} // MIX TARGET KEY pressed

	}


	// FOLLOW_PAGE
	if (	( keyNdx == KEY_FOLLOW )
		&&	( target_page->trackSelection == 0)
		&&	( PLAY_MODE_STATUS == OFF )
		){

		// Toggle the FOLLOW_PAGE switch
		if ( follow_flag == FOLLOW_PAGE ){

			follow_flag = OFF;
		}
		else{

			follow_flag = FOLLOW_PAGE;

			// Point the target_page to the page currently playing in its row
			i = target_page->pageNdx % 10;

			// If there is a page playing in that row, point target_page to it
			if ( GRID_p_selection[i] != NULL ){

				target_page = GRID_p_selection[i];
			}
		}
	}



	//
	// EDIT MASTER
	//
	if (keyNdx == KEY_EDIT_MASTER) {

		// D O U B L E - C L I C K
		if (DOUBLE_CLICK_TARGET == keyNdx) {
			// This is the DC target
			// Toggle the edit mode: 0:EDIT  1:CONTROL

			// Disable this type of functionality for now.. EDIT mode takes over
			target_page->editorMode = CONTROL;
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

			// Toggle the edit mode
			switch( target_page->editorMode ){

				case CONTROL:
					target_page->editorMode = EDIT;
					break;

				case EDIT:
					target_page->editorMode = PREVIEW;
					break;

				case PREVIEW:
					target_page->editorMode = PREVIEW_PERFORM;
					break;

				// Preview perform - Preview with suppressed preview of notes
				case PREVIEW_PERFORM:
					target_page->editorMode = EDIT;
					break;

				case MIX2EDIT:
					target_page->editorMode = EDIT;
					break;
			}
		}
	}




	//
	// MUTE MASTER
	//
	if (keyNdx == KEY_MUTE_MASTER) {
		// Only active if no track is selected
		if (target_page->trackSelection != 0){
			break;
		}

//		// Check if a STEP key is pressed. If so, skip that step.
//		unsigned char temp = is_pressed_steprange();
//		if ( temp != 0 ){
//			// Compute the step coordinates
//			int row = row_of( temp );
//			int col = column_of( temp );
//
//			// SKIP the STEP - after turning it off!
//			Step_set_status( target_page->Step[row][col], STEPSTAT_TOGGLE, 	OFF );
//			Step_set_status( target_page->Step[row][col], STEPSTAT_SKIP, 	ON 	);
//		}

		// Check if a STEP key is pressed. If so, skip that step - actually supports multiple steps.
		unsigned char temp = is_pressed_steprange();

		if ( temp != 0 ){
			// Linear G_scan through the step buttons
			for( i=0; i <= 185; i++ ){

				// Not using the provided infrastructure because of the anomalies. may change
				// There is a key pressed - in the Step Range (exclude the rest)
				if (	( G_pressed_keys[i] >=  11 )
					&&	( G_pressed_keys[i] <= 185 )
					&& 	( ((G_pressed_keys[i]-10) % 11) != 0 )
					){

					// Compute the key coordinates
					row = row_of( 		G_pressed_keys[i] );
					col = column_of(	G_pressed_keys[i] );

					// SKIP the STEP - after turning it off!
					Step_set_status( target_page->Step[row][col], STEPSTAT_TOGGLE, 	OFF );
					Step_set_status( target_page->Step[row][col], STEPSTAT_SKIP, 	ON 	);
				}
			}
		}


		// Clear the solopattern if it exist
		else if (target_page->trackSolopattern != 0) {
			target_page->trackSolopattern = 0;
		}
		else {
			// Then clear the solopattern
			if (target_page->trackMutepattern != 0) {
				target_page->trackMutepattern = 0;
			}
			else {
				// Then fill the mutepattern from store
				target_page->trackMutepattern =
					target_page->trackMutepatternStored;
			}
		}

		// D O U B L E - C L I C K
		if ((DOUBLE_CLICK_TARGET == keyNdx)
				&& (DOUBLE_CLICK_TIMER > DOUBLE_CLICK_ALARM_SENSITIVITY)) {
			// This is a double click victim - Mute all tracks
			target_page->trackMutepattern = 0x0f;
			target_page->trackMutepatternStored = target_page->trackMutepattern;
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
	// TRACK_MUTATORS
	//
	if ((keyNdx >= 187) && (keyNdx <= 196)) {

		// When in preview mode also show the TGL button for the preview step
		if (	( ( target_page->editorMode == PREVIEW )
				||	( target_page->editorMode == PREVIEW_PERFORM ) )
			&&	( page_preview_step != NULL )
			){

			// Toggle the previewed step
			if ( keyNdx == KEY_PLAY1 ){

				// Toggle the step, simply, nothing more fancy
				switch( Step_get_status( page_preview_step, STEPSTAT_TOGGLE )){

					case OFF:
						Step_set_status( page_preview_step, STEPSTAT_TOGGLE, ON );
						break;

					case ON:
						Step_set_status( page_preview_step, STEPSTAT_TOGGLE, OFF );
						break;
				}

			} // Step toggle
		} // Preview mode with previewed step


		// No track or step is selected, or no step pressed in preview mode.
		else if (   (target_page->trackSelection == 0)
			&& (target_page->stepSelection  == 0)  ){


			// If the button pressed falls within the solo pattern, disable its solo status..
			// ..considering the chaimode settings
			if ( ((1<<(keyNdx-187)) & target_page->trackSolopattern) != 0 ){

				// Loop through the chain of the selected track and mute all chain tracks
				// Start with the pressed track
				current_track = target_page->Track[ keyNdx-187 ];
				temp = cardinality_of_chain( current_track );
				for ( i=0; i < temp; i++ ){

					target_page->trackSolopattern ^= ( 1 << row_of_track( target_page, current_track ));
					current_track = current_track->chain_data[NEXT];
				}

				// Need to break here, otherwise the key press gets executed further
				break;
			}



			// If there is no active mutepattern, use fresh store variable
			if (target_page->trackMutepattern == 0) {

				target_page->trackMutepatternStored = 0;
			}



			// MUTE operation depending on the chainstatus - Head or Segment
			// Head: Check the chain status: mute all tracks in the same chain (new model)
			// Segment: each track handled on its own.

			// Start with the pressed track.
			current_track = target_page->Track[ keyNdx-187 ];

			// Depending on the way we choose the track base..
			switch( target_page->CHAINS_PLAY_HEAD ){

				case TRUE:
					// Act as toggle on the full chain
					// How long is the chain?
					temp = cardinality_of_chain( current_track );

					// Loop through the chain of the selected track and mute all chain tracks
					for ( i=0; i < temp; i++ ){

						target_page->trackMutepattern ^= ( 1 << row_of_track( target_page, current_track ));
						current_track = current_track->chain_data[NEXT];
					}
					break;

				// Act only on the individual track
				case FALSE:
					target_page->trackMutepattern ^=
						( 1 << row_of_track( target_page, current_track ));
					break;
			}


			// Update the stored variable
			target_page->trackMutepatternStored = target_page->trackMutepattern;


			// D O U B L E - C L I C K
			if ((DOUBLE_CLICK_TARGET == keyNdx)
					&& (DOUBLE_CLICK_TIMER > DOUBLE_CLICK_ALARM_SENSITIVITY)
				){

				// This is a double click victim, so add track to Solopattern,
				// ..considering the chain mode settings.
				if ( ((1<<(keyNdx-187)) & target_page->trackMutepattern) == 0 ) {

					// Loop through the chain of the selected track and mute all chain tracks
					// Start with the pressed track
					current_track = target_page->Track[ keyNdx-187 ];
					temp = cardinality_of_chain( current_track );
					for ( i=0; i < temp; i++ ){

						target_page->trackSolopattern ^= ( 1 << row_of_track( target_page, current_track ));
						current_track = current_track->chain_data[NEXT];
					}

				} // double click key was not in mutepattern
			}// Double click victim.

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

		} // No track or step is selected

	} // Key is a track mutator



	//
	// TRACK CHAIN SELECTORS
	//
	key_exe_chainselectors( keyNdx );


	if ( keyNdx == KEY_SCALE_SEL ){

//		// Switch zoom level to zoomSCALE
//		G_last_zoom_level = zoomPAGE;
//		G_zoom_level = zoomSCALE;
//		GRID_assistant_page->scaleStatus = SCALE_SEL;
//		GRID_assistant_page->force_to_scale = ON;

		// Actually toggle the page own scale.
		// The page own scale is always Chr, so it can be exempted from grid scale.
		target_page->force_to_scale ^= ON;
	}

//	// Toggle the SCL_align flag in the page
//	if (	( keyNdx == KEY_SCALE_CAD )
//		&&	( target_page->force_to_scale == ON )
//		){
//
//		target_page->SCL_align ^= ON;
//	}

	// In page preview mode
	// In preview mode enter the step velocity, like in MAP preview mode
	if (	( ( target_page->editorMode == PREVIEW )
			||	( target_page->editorMode == PREVIEW_PERFORM ) )
		&&	( page_preview_step != NULL )
		){

		// Enter velocity via the outer circle
		// key_OCT_CIRCLE_xpose_STEP( keyNdx );
	} // preview mode is active


	//
	// CLOCK SOURCE
	//
	if ( keyNdx == KEY_CLOCK ) {

		key_clock_select( target_page, KEY_CLOCK );
	} // KEY_CLOCK was pressed





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
					// Fetches the old status back
					// toggle_PLAY_MODE( OFF );
					break;

				case OFF:
					// Stores the "old" status
					toggle_PLAY_MODE( ON );
					break;
			}
		}
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

				// Clear the recording track on PLAY1 while recording..
				if (	( keyNdx == KEY_PLAY1 )
					&&	( G_run_bit == ON )
					){

					// Find and clear the recording track in target_page
					if ( Page_getTrackRecPattern(target_page) != 0 ){

						// Add all tracks in recording chain to an artificial track selection
						temp = my_bit2ndx( Page_getTrackRecPattern(target_page) );

						// Starting track is the currently recording one
						current_track = target_page->Track[temp];

						// Iterate through the chain and place all tracks in artificial selection
						for ( i = 0; i < MATRIX_NROF_ROWS; i++ ){

							// Place current track into the selection
							target_page->trackSelection |=
								( 1 << row_of_track( target_page, current_track ) );

							// Move the track pointer along the chain
							current_track = current_track->chain_data[NEXT];
						}

						// Clear the achieved track selection
						Page_CLEAR_selected_tracks( target_page );

						// Forget the artificial track selection selection
						target_page->trackSelection = 0;
					}
				}

				// Make sure the sequencer is running
				sequencer_command_PLAY();

				break;
		}
	}


	// ALIGN LAUFLICHT
	if (	( keyNdx == KEY_ALIGN )
		&&	( G_run_bit == ON )
		){

		// Align page to global locator - i.e. all tracks in page

		// Act like this only when we are not dealing with scales.
		// and no track is selected
		if ( 	// (target_page->scaleStatus == OFF)
				// &&
				(target_page->trackSelection == 0)
			){

			// Aligns only the target_page to the global play data
			set_page_locators( target_page, G_global_locator, G_TTC_abs_value );
		}

	} // KEY_ALIGN was pressed


