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
	// MIX MASTER
	//
	if (keyNdx == KEY_MIX_MASTER) {
		// .. nothing
	}


	// Go directly to the stored step selection
	// STEP SELECTION PATTERN INDEX
	if (	(	// (keyNdx == KEY_MIXTGT_USR0)
				(keyNdx == KEY_MIXTGT_USR1)
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

		||	( (keyNdx == KEY_MIXTGT_USR0) )
		||	( (keyNdx == KEY_MIXTGT_USR1) )
		||	( (keyNdx == KEY_MIXTGT_USR2) )
		||	( (keyNdx == KEY_MIXTGT_USR3) )
		||	( (keyNdx == KEY_MIXTGT_USR4) )
		||	( (keyNdx == KEY_MIXTGT_USR5) )
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
				case KEY_MIXTGT_VOL: 	target_page->mixTarget = MIXTGT_VOL; 	break;
				case KEY_MIXTGT_PAN: 	target_page->mixTarget = MIXTGT_PAN; 	break;
				case KEY_MIXTGT_MOD: 	target_page->mixTarget = MIXTGT_MOD; 	break;
				case KEY_MIXTGT_EXP: 	target_page->mixTarget = MIXTGT_EXP; 	break;

				case KEY_MIXTGT_USR0: 	target_page->mixTarget = MIXTGT_USR0; 	break;
				case KEY_MIXTGT_USR1: 	target_page->mixTarget = MIXTGT_USR1; 	break;
				case KEY_MIXTGT_USR2: 	target_page->mixTarget = MIXTGT_USR2; 	break;
				case KEY_MIXTGT_USR3: 	target_page->mixTarget = MIXTGT_USR3; 	break;
				case KEY_MIXTGT_USR4: 	target_page->mixTarget = MIXTGT_USR4; 	break;
				case KEY_MIXTGT_USR5: 	target_page->mixTarget = MIXTGT_USR5; 	break;
			}
		} // Single click scenario
	} // MIX TARGET KEY pressed




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
		#ifdef FEATURE_ENABLE_SONG_UPE
		if ( G_unarm_ctrl == ON ){
			unarm_ctrl();
			break;
		}
		#endif
		// Only active if no track is selected
		if (target_page->trackSelection != 0){
			break;
		}
		row = target_page->pageNdx % 10;
		/*
		// Disable MUTE when on-the-measure mode is activated
		else if ( G_on_the_measure_mod_bit == SCALE_MOD && target_page->OPS_mode != BIRDSEYE ){
			break;
		}
		*/
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


					// Or unskip the step if we are in preview mode
					if ( 	( target_page->editorMode == PREVIEW )
						|| 	( target_page->editorMode == PREVIEW_PERFORM )
						){
						// Step is pressed - toggle skip status
						switch( Step_get_status( target_page->Step[row][col], STEPSTAT_SKIP )){
						case ON:
							Step_set_status( target_page->Step[row][col], STEPSTAT_SKIP, OFF );
							break;
						case OFF:
							Step_set_status( target_page->Step[row][col], STEPSTAT_SKIP, ON );
							break;
						}
					}
					// Normal step skip operation
					else{
						// SKIP the STEP - after turning it off!
						Step_set_status( target_page->Step[row][col], STEPSTAT_SKIP, 	ON 	);
					}

				} // Button pressed in the step range
			}
		}
		else if (G_on_the_measure_operation[row]) {
			unarm_page_otm_operation( target_page, OPERATION_MUTE );
			unarm_page_otm_operation( target_page, OPERATION_SOLO );
		}
		// Clear the solo pattern if it exist
		else if (	( target_page->trackSolopattern != 0 )
				||	( G_on_the_measure_pattern[row][OPERATION_SOLO] != OFF )
				) {
			apply_page_mute_pattern_operation( target_page, 0x0, MASK( OPERATION_SOLO ) );
		}
		else {
			// Then clear the mute pattern
			if (	( target_page->trackMutepattern != 0 )
				||	( G_on_the_measure_pattern[row][OPERATION_MUTE] != OFF )
				) {
				apply_page_mute_pattern_operation( target_page, 0x0, MASK( OPERATION_MUTE ) | MASK( OPERATION_NOSTORE ) );
			}
			else {
				// Then fill the mute pattern from store
				apply_page_mute_pattern_operation( target_page, target_page->trackMutepatternStored, MASK( OPERATION_MUTE ) );
			}
		}

		// D O U B L E - C L I C K
		if ((DOUBLE_CLICK_TARGET == keyNdx)
				&& (DOUBLE_CLICK_TIMER > DOUBLE_CLICK_ALARM_SENSITIVITY)) {
			// This is a double click victim - Mute all tracks
			if (target_page->trackSolopattern != 0) {
				// Remove solo pattern immediately
				target_page->trackSolopattern = OFF;
			}
			apply_page_mute_pattern_operation( target_page, 0x3FF, MASK( OPERATION_MUTE ) );
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
	if (	( keyNdx >= 187 )
		&& 	( keyNdx <= 196 )
		){

		// When in preview mode also show the TGL button for the preview step
		if (	(	( target_page->editorMode == PREVIEW )
				|| 	( target_page->editorMode == PREVIEW_PERFORM )
				)
			&&	( page_preview_step != NULL )
			){

			// Toggle the previewed step
			switch ( keyNdx ) {

			// STEP TOGGLE
			case KEY_TGGL:

				// Toggle the step, simply, nothing more fancy
				switch( Step_get_status( page_preview_step, STEPSTAT_TOGGLE )){

					case OFF:
						Step_set_status( page_preview_step, STEPSTAT_TOGGLE, ON );
						break;

					case ON:
						Step_set_status( page_preview_step, STEPSTAT_TOGGLE, OFF );
						break;
				}
				break; // Step toggle


			case KEY_COPY:
				// Make sure to operate on the right object
				if ( page_preview_step != NULL ){

					// Coordinates of already selected step
					row = page_preview_step_row;
					col = page_preview_step_col;

					// Copy the step pointer to the buffer
					STEP_COPY_BUFFER = page_preview_step;

					// Unselect the step (not sure if really needed)
					Step_set_status( page_preview_step, STEPSTAT_SELECT, OFF );
				}
				break;


			case KEY_PASTE:
				// Make sure we have both a target and a source for the paste operation
				if (	( page_preview_step != NULL )
					&&	( STEP_COPY_BUFFER != NULL)
					){

					// Copy step data from buffer to pointer
					Step_copy( STEP_COPY_BUFFER, page_preview_step, false );

					// Unselect the step (not sure if really needed )
					Step_set_status( page_preview_step, STEPSTAT_SELECT, OFF );

					// Questionable at this point..
					#ifdef COPY_BUFFER_FRESH
					// STEP_COPY_BUFFER = NULL;
					#endif
				}
				break;

			case KEY_ZOOM:
				if ( page_preview_step != NULL ){

					// Coordinates of previewed selected step
					row = page_preview_step_row;
					col = page_preview_step_col;

					Step_zoom( target_page, row, col );
				}
				break;
			} // Switch on the mutator buttons

		} // Preview mode with previewed step actions


		// No track or step is selected, or no step pressed in preview mode.
		else if (   (target_page->trackSelection == 0)
			&& (target_page->stepSelection  == 0)  ){


			// If the button pressed falls within the solo pattern, disable its solo status..
			// ..considering the chaimode settings

			if ( 	( has_solo_row_state( target_page, keyNdx - 187 ) )
				||  ( has_solo_row_future_state( target_page, keyNdx - 187 ) ) ) {
				current_track = target_page->Track[ keyNdx-187 ];
				apply_page_track_mute_toggle_operation( target_page, current_track, MASK( OPERATION_SOLO ) );

				// Need to break here, otherwise the key press gets executed further
				break;
			}

/*

			// If there is no active mutepattern, use fresh store variable
			if (target_page->trackMutepattern == 0) {

				target_page->trackMutepatternStored = 0;
			}
*/

			// Start with the pressed track.
			current_track = target_page->Track[ keyNdx-187 ];
			apply_page_track_mute_toggle_operation( target_page, current_track, MASK( OPERATION_MUTE ) );

			// D O U B L E - C L I C K
			if ((DOUBLE_CLICK_TARGET == keyNdx)
					&& (DOUBLE_CLICK_TIMER > DOUBLE_CLICK_ALARM_SENSITIVITY)
				){

				// This is a double click victim, so add track to Solopattern,
				// ..considering the chain mode settings.
				if ( 1 << ( keyNdx-187 ) ) {
					apply_page_track_mute_toggle_operation( target_page, current_track, MASK( OPERATION_SOLO ) );
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

	if ( keyNdx == KEY_ZOOM_TRACK ) {
		TOGGLE_BIT( G_on_the_measure_mod_bit, target_page->pageNdx % 10 );
	}


	//
	// OCTAVE CIRCLE
	//
	// This should only operate when in scale mode already
	// ..or when trying to enter scale mode.
	if ( 	( target_page->scaleStatus != OFF )
		||	( keyNdx == KEY_SCALE_SEL )
		||	( keyNdx == KEY_SCALE_MOD )
		||	( keyNdx == KEY_SCALE_CAD )
		){

		key_ScaleSelector_functions( keyNdx, target_page );
	}


	if ( target_page->scaleStatus == OFF ){

		// This is the mod operation in Page mode
		key_ScaleSelector_mini_functions( keyNdx, target_page );
	}


	// Toggle the SCL_align flag in the page
	if (	( keyNdx == KEY_SCALE_CAD )
		&&	( target_page->force_to_scale == ON )
		){

		target_page->SCL_align ^= ON;
	}


	// In page preview mode
	// In preview mode enter the step velocity, like in MAP preview mode
	if (	(	( target_page->editorMode == PREVIEW )
			||	( target_page->editorMode == PREVIEW_PERFORM )
			)
		&&	( page_preview_step != NULL )
		){

		// Enter velocity via the outer circle
		key_OCT_CIRCLE_xpose_STEP( keyNdx );
	} // preview mode is active


	//
	// CLOCK SOURCE
	//
	if ( keyNdx == KEY_CLOCK ) {

		key_clock_select( target_page, KEY_CLOCK );
	} // KEY_CLOCK was pressed



	// ALIGN LAUFLICHT
	if (	( keyNdx == KEY_ALIGN )
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




	// Leaves the PLAY changes permanent in the page
	if ( keyNdx == KEY_PROGRAM ){
		if ( PLAY_MODE_STATUS == ON ){
			PLAY_MODE_STATUS = OFF;
		}
	}



	//
	// TRANSPORT functions - when nothing is selected in page
	//
	switch( keyNdx ){

		case KEY_STOP:
			if ( G_clock_source != EXT ){
				sequencer_command_STOP();
			}
			break;


		case KEY_PAUSE:
			if ( G_clock_source != EXT ){
				#ifdef FEATURE_ENABLE_SONG_UPE
				sequencer_command_PAUSE(OFF);
				#else

					#ifdef FEATURE_RESET_PAUSE
						// D O U B L E - C L I C K  C O N S T R U C T
						// DOUBLE CLICK SCENARIO
						if ((DOUBLE_CLICK_TARGET == keyNdx)
								&& (DOUBLE_CLICK_TIMER > DOUBLE_CLICK_ALARM_SENSITIVITY)) {

							// Double click code
							// If Sequencer Stopped then Un-Pause any paused tracks ready for Sequencer Run
							if ( G_run_bit == OFF ) {
								//Reset any Track(s) attr_TEMPOMUL == 0 to stored TEMPOMUL value
								for ( i=0; i<MATRIX_NROF_ROWS; i++ ){

									if ( target_page->Track[i]->attr_TEMPOMUL == 0 ){

										#ifdef FEATURE_FIX_CBB_PAUSE
										target_page->Track[i]->attr_TEMPOMUL = target_page->Track[i]->prepause_TEMPOMUL & 0x0F;
										target_page->Track[i]->prepause_TEMPOMUL = 1;
										#else
											target_page->Track[i]->attr_TEMPOMUL = 1;
										#endif

									}
								}
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
							sequencer_command_PAUSE();
						}
					#else
						sequencer_command_PAUSE();
					#endif

				#endif
			}
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

					// D O U B L E - C L I C K  C O N S T R U C T
					// DOUBLE CLICK SCENARIO
					if ((DOUBLE_CLICK_TARGET == keyNdx)
							&& (DOUBLE_CLICK_TIMER > DOUBLE_CLICK_ALARM_SENSITIVITY)) {

						// Double click code
						// ...
						// Add all tracks in recording chain to an artificial track selection
						target_page->trackSelection = page_get_armed_chain_selection( target_page );
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
						// Add the currently recording track to selection
						target_page->trackSelection = Page_getTrackRecPattern(target_page);
					}

					// Clear the achieved track selection
					Page_CLEAR_recording_tracks( target_page );

					// Forget the artificial track selection selection
					target_page->trackSelection = 0;
				}
			}

			// Make sure the sequencer is running
			if ( G_clock_source != EXT ){
				sequencer_command_PLAY();
			}

			break;

	} // Transport keys were hit

