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



// Key execution in MIXER MAP keymode

	//
	// TRACK SELECTORS
	//


#ifdef NEMO

	// Learning is going on, toggle learn and REC bit..
	if ((keyNdx >= 1) && (keyNdx <= 10)) {

		if (	( target_page->REC_bit == ON )
			){

			// Toggle bit in trackSelection marking the track un/selected
			CCMAP_learner = keyNdx-1;
		}
		else{

			// Restart MIX Timer if it was started somewhere else
			restart_MIX_TIMER();

			// Action depends on the mix target

			// CC_MIXMAP mode selected
			if (target_page->mixTarget < 6) {

				if ( GRID_MAP0_status == ON ){

					target_page = GRID_assistant_page;
				}

				switch( keyNdx ) {
					case 7:
						target_page->CC_MIXMAP_attribute = CC_MIXMAP_AMT;
						break;

					case 9:
						target_page->CC_MIXMAP_attribute = CC_MIXMAP_MCC;
						break;

					case 10:
						target_page->CC_MIXMAP_attribute = CC_MIXMAP_MCH;
						break;
				}
			}
			// MIXTGT_ATR mode
			else if (target_page->mixTarget == 10) {

				// Edit MIX ATTRIBUTE - exit if same attribute selected
				if (target_page->mixAttribute == keyNdx) {

					G_zoom_level = zoomPAGE;
				}
				else {

					target_page->mixAttribute = keyNdx;
				}
			}
		}
	}
#else
		switch( keyNdx ){

			case 1:
				target_page->mixAttribute = NEMO_ATTR_PITCH;
				break;
			case 2:
				target_page->mixAttribute = NEMO_ATTR_VELOCITY;
				break;
			case 3:
				target_page->mixAttribute = NEMO_ATTR_LENGTH;
				break;
			case 4:
				target_page->mixAttribute = NEMO_ATTR_START;
				break;
		}
#endif


	//
	// MATRIX
	//
	// OFF (originally)
	if ( (keyNdx > 10) && (keyNdx <= 185) && (((keyNdx-10) % 11) != 0) ) {

		// Compute the coordinates of the pressed key
		row = row_of( keyNdx );
		col = column_of( keyNdx );

		// Do not react on tracks that are not heads in play_heads mode
		// Check if we are playing heads only..
		if ( target_page->CHAINS_PLAY_HEAD == TRUE ){

			// Check if track is chain head..
			if ( target_page->Track[row]->chain_data[HEAD] != target_page->Track[row] ){

				return;
			}
		}

		// Assignment of the MIX attribute
		if ( 	is_pressed_key( KEY_MIX_MASTER )
			&&	( target_page->mixTarget == MIXTGT_ATR )
			){

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


		// Check the attribute being edited. For now pitch is special:
		switch( target_page->mixTarget ){

			case MIXTGT_ATR:

				switch( target_page->mixAttribute ){

					// This is a special case but allows for great live control
					case NEMO_ATTR_VELOCITY:

						// Identify the target value
						target_ptr = &target_page->Track[row]->attr_VEL;

						// Modify the tens value of the target value

						// Break up original value in tens and ones.
						i = *target_ptr / 10;
						j = *target_ptr % 10;

						if ( col < 12 ){

							// Compute new tens
							i = 10 * (col + 1);

							// Get new ones value, leave it as it is otherwise
							if ( col < 9 ){

								// j = col + 1;
								// Remove it so we can reach zero of ones.
								j = 0;
							}

							// Write the final value into the attribute
							*target_ptr = normalize( i + j, 0, 127 );
						}

						// Act only in the last column as 0 stabilizer
						else if ( col == 15 ){

							// Write the final value into the attribute
							*target_ptr = 0;
						}

						break;

					// One may add VEL to this list to make the model more consistent
					case NEMO_ATTR_AMOUNT:
					case NEMO_ATTR_MIDICC:

						if ( target_page->mixAttribute == ATTR_AMOUNT ){
							target_ptr =
								&target_page->Track[row]->attr_AMT;
						}
						else if ( target_page->mixAttribute == ATTR_MIDICC ){
							target_ptr =
								&target_page->Track[row]->attr_MCC;
						}


						// D O U B L E - C L I C K
						if ((DOUBLE_CLICK_TARGET == keyNdx)
								&& (DOUBLE_CLICK_TIMER > DOUBLE_CLICK_ALARM_SENSITIVITY)) {

							// Double click code:
							//
							// Modify the tens value of the target value

							// Break up original value in tens and ones.
							i = *target_ptr / 10;
							j = *target_ptr % 10;

							if ( col < 12 ){

								// Compute new tens
								i = 10 * (col + 1);

								// Get new ones value, leave it as it is otherwise
								if ( col < 9 ){

									// j = col + 1;
									// Remove it so we can reach zero of ones.
									j = 0;
								}

								// Write the final value into the attribute
								*target_ptr = normalize( i + j, 0, 127 );
							}

							// Act in the last column as 0 stabilizer
							else if (col == 15){

								// Write the final value into the attribute
								*target_ptr = 0;
							}
						} // second click of double click

						else if (DOUBLE_CLICK_TARGET == 0) {

							DOUBLE_CLICK_TARGET = keyNdx;
							DOUBLE_CLICK_TIMER = ON;
							// Start the Double click Alarm
							cyg_alarm_initialize(
								doubleClickAlarm_hdl,
								cyg_current_time() + DOUBLE_CLICK_ALARM_TIME,
								DOUBLE_CLICK_ALARM_TIME );

							// Single click code:
							//
							if ( col < 9 ){
								// Modify the ones value of the MIDICC. 128 is a special value.
								if ( *target_ptr > 127 ){
									i = 0;
								}
								else {
									i = *target_ptr / 10;
								}
								*target_ptr = normalize( i*10 + col + 1, 0, 127 );
							}
						} // first click of double click

						// close case ATTR_VELOCITY _AMOUNT _MIDICC
						break;


					case NEMO_ATTR_PITCH:

						// Make sure we are in the notes field:
						if ( ( col < 2 ) || (col > 13) )  {

							// Move octave of track
							if (col == 14) {
								// Move octave down if possible
								if (target_page->Track[row]->attr_PIT > 11){

									target_page->Track[row]->attr_PIT =
										((target_page->Track[row]->attr_PIT / 12)-1) * 12 +
										   target_page->Track[row]->attr_PIT % 12 ;
								}
							}

							if (col == 15) {
								// Move octave up if possible
								if (target_page->Track[row]->attr_PIT < 116){

									target_page->Track[row]->attr_PIT =
										((target_page->Track[row]->attr_PIT / 12)+1) * 12 +
										   target_page->Track[row]->attr_PIT % 12 ;
								}
							}

						}
						else {

							// Change track pitch according to key
							target_page->Track[row]->attr_PIT =
								(target_page->Track[row]->attr_PIT / 12)*12 + col -2;
						 }
						break;



					case NEMO_ATTR_START:
						target_ptr = &target_page->Track[row]->STA_factor;
						// Set the new value of the STA factor.
						*target_ptr = normalize( col, 0, TRACK_MAX_STAFACTOR );
						break;

					case NEMO_ATTR_LENGTH:
						target_ptr = &target_page->Track[row]->LEN_factor;
						// Set the new value of the LEN factor - on the same port.
						*target_ptr = normalize( col, 0, TRACK_MAX_LENFACTOR );
						break;

					case NEMO_ATTR_DIRECTION:
						target_ptr = &target_page->Track[row]->attr_DIR;
						// Set the new value of the GROOVE attribute - on the same port.
						*target_ptr = normalize( col + 1, 0, TRACK_MAX_DIRECTION );
						break;

					case NEMO_ATTR_GROOVE:
						target_ptr = &target_page->Track[row]->attr_GRV;

						// D O U B L E - C L I C K
						if ((DOUBLE_CLICK_TARGET == keyNdx)
								&& (DOUBLE_CLICK_TIMER > DOUBLE_CLICK_ALARM_SENSITIVITY)) {

							// Double click code:
							//
							// Ste GROOVE attribute value to 0
							*target_ptr = 0;
						}

						else if (DOUBLE_CLICK_TARGET == 0) {

							DOUBLE_CLICK_TARGET = keyNdx;
							DOUBLE_CLICK_TIMER = ON;
							// Start the Double click Alarm
							cyg_alarm_initialize(
								doubleClickAlarm_hdl,
								cyg_current_time() + DOUBLE_CLICK_ALARM_TIME,
								DOUBLE_CLICK_ALARM_TIME );

							// Set the new value of the GROOVE attribute - on the same port.
							*target_ptr = col;
						}
						break;


					case NEMO_ATTR_MIDICH:

						target_ptr =
							&target_page->Track[row]->attr_MCH;

						// D O U B L E - C L I C K
						if ((DOUBLE_CLICK_TARGET == keyNdx)
								&& (DOUBLE_CLICK_TIMER > DOUBLE_CLICK_ALARM_SENSITIVITY)) {

							// Double click code:
							///
							// Switch the MIDI port
							// Find out the current port: it will be 0 or 1;
							// Note that the ATTR_MIDICH is in the range [1..32].
							// Disregard the virtual MIDI channels
							i = *target_ptr / 17;
							j = *target_ptr - (16 * i);

							// Swith the port indicator
							i ^= 1;

							// Write the new value to the attribute
							*target_ptr = (16 * i) + j;
						}

						else if (DOUBLE_CLICK_TARGET == 0) {

							DOUBLE_CLICK_TARGET = keyNdx;
							DOUBLE_CLICK_TIMER = ON;
							// Start the Double click Alarm
							cyg_alarm_initialize(
								doubleClickAlarm_hdl,
								cyg_current_time() + DOUBLE_CLICK_ALARM_TIME,
								DOUBLE_CLICK_ALARM_TIME );

							// Single click code:
							// Find out the current port: it will be 0 or 1;
							// Note that the ATTR_MIDICH is in the range [1..32].
							i = *target_ptr / 17;

							// Set the new value of the MIDICH attribute - on the same port.
							*target_ptr = (16 * i) + col + 1;
						}

						break;

				} // switch( target_page->mixAttribute )
				break; // MIXTGT_ATR


			case MIXTGT_USR1:
			case MIXTGT_USR2:
			case MIXTGT_USR3:
			case MIXTGT_USR4:

				// Operate on the GRID assistant page
				GRID_assistant_page->mixTarget = target_page->mixTarget;
				target_page = GRID_assistant_page;

				if ( is_pressed_key( KEY_MIX_MASTER ) ){

					// Select the MIXMAP attribute
					switch( keyNdx ){

						case 67:	 	// AMT
							target_page->CC_MIXMAP_attribute = CC_MIXMAP_AMT;
							break;
						case 89:		// MCC
							target_page->CC_MIXMAP_attribute = CC_MIXMAP_MCC;
							break;
						case 111:		// MCH
							target_page->CC_MIXMAP_attribute = CC_MIXMAP_MCH;
							break;
					}

					break;
				}

				// Find the target value to be modified
				switch( target_page->CC_MIXMAP_attribute ) {

					case CC_MIXMAP_AMT:
					case CC_MIXMAP_MCC:

						if ( target_page->CC_MIXMAP_attribute == CC_MIXMAP_AMT ){
							target_ptr =
								&target_page->CC_MIXMAP[target_page->mixTarget][row][CC_MIXMAP_AMT];
						}
						else if ( target_page->CC_MIXMAP_attribute == CC_MIXMAP_MCC ){
							target_ptr =
								&target_page->CC_MIXMAP[target_page->mixTarget][row][CC_MIXMAP_MCC];
						}

						// D O U B L E - C L I C K
						if ((DOUBLE_CLICK_TARGET == keyNdx)
								&& (DOUBLE_CLICK_TIMER > DOUBLE_CLICK_ALARM_SENSITIVITY)) {

							// Double click code:
							//
							// Modify the tens value of the AMOUNT

							// Break up original value in tens and ones.
							i = *target_ptr / 10;
							j = *target_ptr % 10;

							if ( col < 12 ){

								// Compute new tens
								i = 10 * (col + 1);

								// Get new ones value, leave it as it is otherwise
								if ( col < 9 ){

									// j = col + 1;
									j = 0;
								}

								// Write the final value into the attribute
								*target_ptr = normalize( i + j, 0, 127 );
							}

							// Last column acts as zero stabilizer
							else if (col == 15){

								// Write the final value into the attribute
								*target_ptr = 0;
							}
						} // second click of double click

						else if (DOUBLE_CLICK_TARGET == 0) {

							DOUBLE_CLICK_TARGET = keyNdx;
							DOUBLE_CLICK_TIMER = ON;
							// Start the Double click Alarm
							cyg_alarm_initialize(
								doubleClickAlarm_hdl,
								cyg_current_time() + DOUBLE_CLICK_ALARM_TIME,
								DOUBLE_CLICK_ALARM_TIME );

							// Single click code:
							//
							if ( col < 9 ){
								// Modify the ones value of the MIDICC. 128 is a special value.
								if ( *target_ptr > 127 ){
									i = 0;
								}
								else {
									i = *target_ptr / 10;
								}
								*target_ptr = normalize( i*10 + col + 1, 0, 127 );
							}
						} // first click of double click

						// close case CC_MIXMAP_AMT and CC_MIXMAP_MCC
						break;


					case CC_MIXMAP_MCH:

						target_ptr =
							&target_page->CC_MIXMAP[target_page->mixTarget][row][CC_MIXMAP_MCH];

						// D O U B L E - C L I C K
						if ((DOUBLE_CLICK_TARGET == keyNdx)
								&& (DOUBLE_CLICK_TIMER > DOUBLE_CLICK_ALARM_SENSITIVITY)) {

							// Double click code:
							///
							// Switch the MIDI port
							// Find out the current port: it will be 0 or 1;
							// Note that the ATTR_MIDICH is in the range [1..32].
							// Disregard the virtual MIDI channels
							i = *target_ptr / 17;
							j = *target_ptr - (16 * i);

							// Swith the port indicator
							i ^= 1;

							// Write the new value to the attribute
							*target_ptr = (16 * i) + j;
						}

						else if (DOUBLE_CLICK_TARGET == 0) {

							DOUBLE_CLICK_TARGET = keyNdx;
							DOUBLE_CLICK_TIMER = ON;
							// Start the Double click Alarm
							cyg_alarm_initialize(
								doubleClickAlarm_hdl,
								cyg_current_time() + DOUBLE_CLICK_ALARM_TIME,
								DOUBLE_CLICK_ALARM_TIME );

							// Single click code:
							// Find out the current port: it will be 0 or 1;
							// Note that the ATTR_MIDICH is in the range [1..32].
							i = *target_ptr / 17;

							// Set the new value of the MIDICH attribute - on the same port.
							*target_ptr = (16 * i) + col + 1;
						}

						break;
				} // switch( target_page->CC_MIXMAP_attribute )




		} // switch( target_page->mixTarget )

	}// If key in matrix range


	//
	// ZOOM MODE
	//

	// GRID mode switch
	//
	if (keyNdx == KEY_ZOOM_GRID) {
		G_zoom_level = zoomGRID;
	}

	// PAGE mode switch
	//
	if (keyNdx == KEY_ZOOM_PAGE) {

		if ( G_zoom_level == zoomMIXMAP ){

			// Go back to page mode
			G_zoom_level = zoomPAGE;

			target_page->REC_bit = OFF;
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



	// Sort tracks by value in display
//	if (	( keyNdx == KEY_ALIGN )
//		){
//
//		// Act like this only when we are not dealing with scales.
//		// and no track is selected
//		if ( 	(target_page->scaleStatus == OFF)
//			&& 	(target_page->trackSelection == 0)
//			){
//
//			// Sorts the tracks of the target_page (descending order) by current mix attribute
//			// ..only when dealing with some ATR
//			if ( target_page->mixTarget == MIXTGT_ATR ){
//
//				sort_tracks_by_ATR( target_page, target_page->mixAttribute );
//			}
//
//			// Aligns the target_page to the global play data
//			set_page_locators( target_page, G_global_locator, G_TTC_abs_value );
//		}
//	} // KEY_ALIGN was pressed


	if ( keyNdx == KEY_RMX ){

		// RANDOMIZES the current mix attribute values of the VIEWER page
		// ..only when dealing with some ATR
		if ( target_page->mixTarget == MIXTGT_ATR ){

			rand_tracks_by_ATR( target_page, target_page->mixAttribute );
		}

		// Mark page as touched
		target_page->page_clear = OFF;
	}


	else if ( keyNdx == KEY_CLEAR ){

		switch ( target_page->mixTarget ){

			case MIXTGT_ATR:
				clear_tracks_by_ATR( target_page, target_page->mixAttribute );
				break;

			case MIXTGT_USR1:
			case MIXTGT_USR2:
			case MIXTGT_USR3:
			case MIXTGT_USR4:
				clear_CC_MIXMAP( GRID_assistant_page, target_page->mixTarget );
				break;
		}
	} // KEY_CLEAR



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


	//
	// RECORD KEY
	//
	// REC alone toggles Track record mode and disables Page record mode.
	// PAGE+REC toggles Page record mode and disables Track record mode.
	// TRACK+REC is handled elsewhere.

	if ( keyNdx == KEY_RECORD ) {

		// Ignore the key event if a track selection is active. It will be handled in key_PAGE_sel_TRACK.h.
		// In MixMap mode we are only interested in Page record mode to support CC map learn mode.
		// The record relies to GRID assistant page
		target_page = GRID_assistant_page;

		if ( !target_page->trackSelection ) {
			G_track_rec_bit = OFF;
			target_page->REC_bit ^= ON;
		}
	}



	// Switch between CC maps
	if ( 	// ( (keyNdx == KEY_MIXTGT_ATR ) )
			// Only active when looking at CC maps
			( target_page->mixTarget != MIXTGT_ATR )
		||	( (keyNdx == KEY_MIXTGT_VOL ) )
		||	( (keyNdx == KEY_MIXTGT_PAN ) )
		||	( (keyNdx == KEY_MIXTGT_MOD ) )
		||	( (keyNdx == KEY_MIXTGT_EXP ) )
		){

		// Just simply switch to the chosen mix target.
		switch( keyNdx ){
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
	} // MIX TARGET KEY pressed


