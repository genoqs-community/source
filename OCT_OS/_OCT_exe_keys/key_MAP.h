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





	// TRACK SELECTORS
	//
	if ((keyNdx > 0) && (keyNdx <= 10)) {

		switch( MODE_OBJECT_SELECTION ){

			// Select change track selection
			case BIRDSEYE:
				// Change the track selection in page
				target_page->trackSelection = ( 1 << (keyNdx - 1) );
				break;

			default:
				// Edit Track Attribute Selection
				if ( ( (1 << (keyNdx-1) ) & PANEL_get_trackAttributeMap() ) != 0 ) {

					// Toggle the attribute selection
					if (target_page->trackAttributeSelection == 1<< (keyNdx-1)) {

						target_page->trackAttributeSelection = 0;
						G_zoom_level = zoomTRACK;
					}
					else {

						// Select the new attribute whose map to show.
						target_page->trackAttributeSelection = 1 << (keyNdx-1);
					}
				}
				break;
		} // switch( MODE_OBJECT_SELECTION );
	}



	//
	// MATRIX
	//

	// ROW ZERO in matrix
	// interpret the keypress - unlike we are in the DIR map..
	if ( 	(keyNdx >=  20) && 	(keyNdx <= 185)
		&& 	( ((keyNdx - 20) % 11) == 0)
		&&	( target_page->trackAttributeSelection != (1 << DIRECTION) )
		){

		// Compute coordinates of the pressed step
		col = ((keyNdx-20) / 11);
		row = my_bit2ndx( target_page->trackSelection );

		// Check if the PLAY Mode button is pressed
		if (	( target_page->editorMode == PREVIEW )
			){

			// Play the Step information as it would be played by the player
			PLAYER_preview_step( target_page, row, col );
		}

		// Interpret the keypress
		interpret_matrix_stepkey( row, col, target_page );

	} // If this is a bottom Matrix row key



	// General MATRIX KEYS: paint the SHAPE
	//
	else if ( 	(keyNdx >=  11) && 	(keyNdx <= 185)
			&& 	( ((keyNdx - 10) % 11) != 0)   ) 		{

		// PAINT ATTRIBUTE MAP SHAPE

		// Compute coordinates of steps to modify
		i   = row_of( keyNdx );
		row = my_bit2ndx( target_page->trackSelection );
		col = column_of( keyNdx );

		// d_iag_printf( "key_MAP ndx:%d i:%d row:%d col:%d\n", keyNdx, i, row, col);

		// Set the tens attribute value of the corresponding steps
		switch( my_bit2ndx( target_page->trackAttributeSelection ) +1){

			case ATTR_VELOCITY:
				val_ptr = &(target_page->Step[row][col]->attr_VEL);
				// Pressing the highest row should produce the max value
				switch( i ){
					case 0:		*val_ptr =  35;					break;
					case 1:		*val_ptr =  25; 				break;
					case 2:		*val_ptr =  15;					break;
					case 3:		*val_ptr =   5;					break;
					case 4:		*val_ptr =   0;					break;
					case 5: 	*val_ptr =  -5;					break;
					case 6:		*val_ptr = -15;					break;
					case 7:		*val_ptr = -25;					break;
					case 8:		*val_ptr = -35;					break;
				}
				break;

			case ATTR_PITCH:
				val_ptr = &(target_page->Step[row][col]->attr_PIT);
				// Pressing the highest row should produce the max value
				switch( i ){
					case 0:		*val_ptr =  24;					break;
					case 1:		*val_ptr =  12; 				break;
					case 2:		*val_ptr =   7;					break;
					case 3:		*val_ptr =   4;					break;
					case 4:		*val_ptr =   0;					break;
					case 5: 	*val_ptr =  -4;					break;
					case 6:		*val_ptr =  -7;					break;
					case 7:		*val_ptr = -12;					break;
					case 8:		*val_ptr = -24;					break;
				}
				break;


			case ATTR_LENGTH:

				val_ptr = &(target_page->Step[row][col]->attr_LEN);
				// Pressing the highest row should produce the max value
				switch( i ){
					case 0:		*val_ptr =     128;				break;
					case 1:		*val_ptr =      64; 			break;
					case 2:		*val_ptr =      32;				break;
					case 3:		*val_ptr =      24;				break;
					case 4:		*val_ptr =      16;				break;
					case 5: 	*val_ptr =      12;				break;
					case 6:		*val_ptr =    	 8;				break;
					case 7:		*val_ptr = 	  	 4;				break;
					case 8:		*val_ptr =   	 2;				break;
				}
				break;



			case ATTR_START:

				val_ptr = &(target_page->Step[row][col]->attr_STA);
				// Pressing the highest row should produce the max value
				switch( i ){
					case 0:		*val_ptr =  	10;				break;
					case 1:		*val_ptr =   	9; 				break;
					case 2:		*val_ptr =   	8;				break;
					case 3:		*val_ptr =   	7;				break;
					case 4:		*val_ptr =   	6;				break;
					case 5: 	*val_ptr =   	5;				break;
					case 6:		*val_ptr =    	4;				break;
					case 7:		*val_ptr = 	  	3;				break;
					case 8:		*val_ptr =    	2;				break;
				}
				break;



			case ATTR_DIRECTION:
				// Click in rowzero
				if ( i == 9 ){

					// Select the trigger set of the direction
					current_trigger_set = col+1;
				}
				// Keep directions 1-5 as read-only
				else if ( target_page->Track[row]->attr_DIR > 5 ){

					if ( 	direction_repository[ target_page->Track[row]->attr_DIR]
						  	.trigger_set[current_trigger_set]
						  	.trigger[9-i]  == (col + 1)
						){
						direction_repository[ target_page->Track[row]->attr_DIR]
						.trigger_set[current_trigger_set]
						.trigger[9-i] = 0;
					}
					else{
						direction_repository[ target_page->Track[row]->attr_DIR]
						.trigger_set[current_trigger_set]
						.trigger[9-i] = col + 1;
					}
				}
				break;



			case ATTR_AMOUNT:
				val_ptr = &(target_page->Step[row][col]->attr_AMT);
				// Pressing the highest row should produce the max value
				switch( i ){
					case 0:		*val_ptr =  35;					break;
					case 1:		*val_ptr =  25; 				break;
					case 2:		*val_ptr =  15;					break;
					case 3:		*val_ptr =   5;					break;
					case 4:		*val_ptr =   0;					break;
					case 5: 	*val_ptr =  -5;					break;
					case 6:		*val_ptr = -15;					break;
					case 7:		*val_ptr = -25;					break;
					case 8:		*val_ptr = -35;					break;
				}
				break;


#if GROEF || 1
			case ATTR_GROOVE:
				val_ptr = &(target_page->Step[row][col]->phrase_num);
				// Pressing the highest row should produce the max value
				switch( i ){
					case 0:		*val_ptr =  STEP_MAX_GROOVE;	break;
					case 1:		*val_ptr = ((STEP_MAX_GROOVE+1)/8) * 7;	 break;
					case 2:		*val_ptr = ((STEP_MAX_GROOVE+1)/8) * 6;	 break;
					case 3:		*val_ptr = ((STEP_MAX_GROOVE+1)/8) * 5;	 break;
					case 4:		*val_ptr = ((STEP_MAX_GROOVE+1)/8) * 4;	 break;
					case 5: 	*val_ptr = ((STEP_MAX_GROOVE+1)/8) * 3;	 break;
					case 6:		*val_ptr = ((STEP_MAX_GROOVE+1)/8) * 2;	 break;
					case 7:		*val_ptr = ((STEP_MAX_GROOVE+1)/8) * 1;	 break;
					case 8:		*val_ptr = STEP_MIN_GROOVE;		break;
				}
				break;
#endif


			case ATTR_MIDICC:
				val_ptr = &(target_page->Step[row][col]->attr_MCC);

				if ( target_page->Track[row]->attr_MCC == MIDICC_BENDER ){

					switch( i ){

						case 0:		*val_ptr =  35 + 64;			break;
						case 1:		*val_ptr =  25 + 64; 	 		break;
						case 2:		*val_ptr =  15 + 64;			break;
						case 3:		*val_ptr =   5 + 64;			break;
						case 4:		*val_ptr =   0 + 64;			break;
						case 5: 	*val_ptr =  -5 + 64;			break;
						case 6:		*val_ptr = -15 + 64;			break;
						case 7:		*val_ptr = -25 + 64;			break;
						case 8:		*val_ptr = -35 + 64;			break;
					}
				}
				else{

					switch( i ){

						case 0:		*val_ptr =     120;				break;
						case 1:		*val_ptr =     104; 			break;
						case 2:		*val_ptr =      88;				break;
						case 3:		*val_ptr =      72;				break;
						case 4:		*val_ptr =      64;				break;
						case 5: 	*val_ptr =      56;				break;
						case 6:		*val_ptr =    	40;				break;
						case 7:		*val_ptr = 	  	24;				break;
						case 8:		*val_ptr =   	MIDICC_NONE;	break;
					}
				}
				break;

		}
	}



	// TRACK MUTATORS
	//
	if ( (keyNdx >= 187) && (keyNdx <= 196) ) {

		// Compute coordinates first
		row = my_bit2ndx( target_page->trackSelection );
		// Identify the attribute we are currently working on
		j =   my_bit2ndx( target_page->trackAttributeSelection ) +1;

		switch( keyNdx ) {

			case KEY_CLEAR:

				// Treat the direction map separately from the others
				if ( j == ATTR_DIRECTION ){

					// CLEAR the current direction to its preset status
					direction_init( target_page->Track[row]->attr_DIR );
				}
				else{
							// Logic: if some steps selected, act only on those.
							// Otherwise act on all.
							switch( target_page->stepSelection ){

								case 0:
									// No steps are selected, modify all steps in track
									for (col=0; col < MATRIX_NROF_COLUMNS; col++){

										// Reset map values to default
//										target_page->Step[row][col]->attribute[j] =
										*(ptr_of_step_attr_value( target_page, row, col, j )) =
											stepDefaultAttrValue[j];

										// Reset bender map first byte values to default
										target_page->Step[row][col]->attr_POS
											= STEP_DEF_POSITION;
									}

									// Check the MIDI CC case - handle with care..
									if ( j == ATTR_MIDICC ){

										// If CC is Bender, also send center value.
										// A combined value of 0x2000 is indicates "Pitch Wheel centered"
										if ( target_page->Track[row]->attr_MCC
												== MIDICC_BENDER ){

											// Send the Bender center message
											MIDI_send(  MIDI_BENDER,
														target_page->Track[row]->attr_MCH,
														0, 0x40 );
// The first data bits 0 to 6 are bits 0 to 6 of the 14-bit value.
// The second data bits 0 to 6 are really bits 7 to 13 of the 14-bit value.
										}
									}
									break;

								default:
									// Some steps are selected, find and modify them
									for (col=0; col < MATRIX_NROF_COLUMNS; col++) {

										// If the step is selected..
										if ( Step_get_status
												( target_page->Step[row][col], STEPSTAT_SELECT ) == ON ){

											// ..Reset Step value to default
//											target_page->Step[row][col]->attribute[j] =
											*(ptr_of_step_attr_value( target_page, row, col, j )) =
												stepDefaultAttrValue[j];
										}
									}
									break;
							} // switch( target_page->stepSelection )
				}

				break;



			case KEY_RANDOMIZE:

				// Act depending on the selected attribute j
				switch( j ){

					case ATTR_DIRECTION:

						// Keep directions 1-5 as read-only
						if ( target_page->Track[row]->attr_DIR > 5 ){

//							// Randomize the currently selected trigger set
//							for ( i=0; i < (MATRIX_NROF_ROWS-1); i++ ){
//
//								direction_repository[ target_page->Track[row]->attr_DIR]
//								.trigger_set[current_trigger_set]
//								.trigger[9-i] = rand() % 17;
//							}

							// Randomize all trigger sets
							for ( j=0; j < MATRIX_NROF_COLUMNS; j++ ){
								for ( i=0; i < (MATRIX_NROF_ROWS-1); i++ ){

									direction_repository[ target_page->Track[row]->attr_DIR]
										.trigger_set[j+1]
										.trigger[9-i] = rand() % 17;
								}
							}
						}
						break;


					case ATTR_LENGTH:
						// Logic: if some steps selected, act only on those.
						// Otherwise act on all.
						switch( target_page->stepSelection ){

							case 0:
								// No steps are selected, modify all steps in track
								for (col=0; col < MATRIX_NROF_COLUMNS; col++){

									target_page->Step[row][col]->attr_LEN =
										(signed char)
										randomStepAttrValue( 	target_page->Step[row][col],
																ATTR_LENGTH,
																target_page->Track[row]->attr_AMT
																+ target_page->Track[row]->event_offset[ATTR_AMOUNT]
																);
								}
								break;

							default:
								// Some steps are selected, find and modify them
								for (col=0; col < MATRIX_NROF_COLUMNS; col++) {

									// If the step is selected..
									if ( Step_get_status
											( target_page->Step[row][col], STEPSTAT_SELECT ) == ON ){

										// ..Reset Step value to default
										target_page->Step[row][col]->attr_LEN =
											(signed char)
											randomStepAttrValue( 	target_page->Step[row][col],
																	ATTR_LENGTH,
																	target_page->Track[row]->attr_AMT
																	+ target_page->Track[row]->event_offset[ATTR_AMOUNT]
																	);
									}
								}
								break;
						} // switch( target_page->stepSelection )
						break;


					default:

						// Logic: if some steps selected, act only on those.
						// Otherwise act on all.
						switch( target_page->stepSelection ){

							case 0:
								// No steps are selected, modify all steps in track
								for (col=0; col < MATRIX_NROF_COLUMNS; col++){

//									target_page->Step[row][col]->attribute[j] =
									*(ptr_of_step_attr_value( target_page, row, col, j )) =
										(signed char)
										randomStepAttrValue( 	target_page->Step[row][col],
																j,
																target_page->Track[row]->attr_AMT
																+ target_page->Track[row]->event_offset[ATTR_AMOUNT]
																);
								}
								break;

							default:
								// Some steps are selected, find and modify them
								for (col=0; col < MATRIX_NROF_COLUMNS; col++) {

									// If the step is selected..
									if ( Step_get_status
											( target_page->Step[row][col], STEPSTAT_SELECT ) == ON ){

										// ..Reset Step value to default
//										target_page->Step[row][col]->attribute[j] =
										*(ptr_of_step_attr_value( target_page, row, col, j )) =
										(signed char)
										randomStepAttrValue( 	target_page->Step[row][col],
																j,
																target_page->Track[row]->attr_AMT
																+ target_page->Track[row]->event_offset[ATTR_AMOUNT]
																);
									}
								}
								break;
						} // switch( target_page->stepSelection )
						break;

				} // Switch on the selected attribute






				break;



			case KEY_RMX:

				// Remix the track steps
				Page_RMX_selected_tracks( target_page );
				break;



			case KEY_ZOOM:

				// Take a look at the number of selected steps
				switch( target_page->stepSelection ){

					case 0:
						// None or more steps selected:
						// ..zoom out into the previos mode
						// Switch zoom level
						G_zoom_level = zoomTRACK;
						break;

					default:
						// More than one step is selected: unselect the selected steps
						// Unselect all potentially selected steps in the track
						for (col=0; col<MATRIX_NROF_COLUMNS; col++) {

							// If step is selected
							if ( Step_get_status( target_page->Step[row][col], STEPSTAT_SELECT ) == ON ){

								// Mark step unselected
								Step_set_status( target_page->Step[row][col], STEPSTAT_SELECT, OFF);

								// And decrement step selection counter
								target_page->stepSelection --;
							}
						}

						// And zoom out
						// None or more steps selected:
						// ..zoom out into the previos mode
						// Switch zoom level
						G_zoom_level = zoomTRACK;
						break;
				}
				break; // case KEY_ZOOM

		} // switch keyNdx

	}


	// TRACK CHAIN SELECTORS - active only when sequencer not running.
	key_exe_chainselectors( keyNdx );


	// ZOOM MODE
	//
	// GRID zoom mode
	if (	(keyNdx == KEY_ZOOM_GRID)
		||	(keyNdx == KEY_ZOOM_PAGE)
		|| 	(keyNdx == KEY_ZOOM_TRACK) ){

		switch( keyNdx ){

			case KEY_ZOOM_GRID:
				G_zoom_level = zoomGRID;
				break;

			case KEY_ZOOM_PAGE:
				G_zoom_level = zoomPAGE;
				break;

			case KEY_ZOOM_TRACK:
				G_zoom_level = zoomTRACK;
				break;
		}

		// Unselect all potentially selected steps in the track
		// ..to prevent conflicting state of track and step selected
		if ( target_page->stepSelection != 0 ){

			// Calculate the row of the selected track
			row = my_bit2ndx( target_page->trackSelection );

			// Iterate through all steps in that row (selected track)
			for (col=0; col<MATRIX_NROF_COLUMNS; col++) {

				// If step is selected
				if ( Step_get_status( target_page->Step[row][col], STEPSTAT_SELECT ) == ON ){

					// Mark step unselected
					Step_set_status( target_page->Step[row][col], STEPSTAT_SELECT, OFF );

					// And decrement step selection counter
					target_page->stepSelection --;
				}
			}
		}

	} // keyNdx is a KEY_ZOOM_xxx



	// PLAY MODE Switcher
	if ( keyNdx == KEY_ZOOM_PLAY ){

		// Toggles the play mode
		toggle_PLAY_MODE( PLAY_MODE_STATUS^1 );
	}



	//
	// FOLLOW_TRACK
	//
	if (	( keyNdx == KEY_FOLLOW )
		&&	( PLAY_MODE_STATUS == OFF )
		){

		// Toggle the FOLLOW_TRACK switch
		if ( follow_flag == FOLLOW_TRACK ){

			follow_flag = OFF;
		}
		else{

			follow_flag = FOLLOW_TRACK;
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
	// RECORD KEY
	//
	// Toggle record arm status of selected track, and toggle global track record mode along with it
	// so that the track is immediately in record mode.

	if ( keyNdx == KEY_RECORD ){

		// Make sure only one track is selected
		if ( my_bit_cardinality( target_page->trackSelection ) == 1 ){

			// First off, disable Page record mode.
			target_page->REC_bit = OFF;

			// Check if this track is currently armed.
			// If currently armed, un-arm this and all other tracks. Switch off global track record mode.
			booln armedBo = ((Page_getTrackRecPattern(target_page) & target_page->trackSelection) != 0);
			if ( armedBo ) {
				Page_setTrackRecPattern( target_page, 0 );
				G_track_rec_bit = OFF;
			}

			// Else, arm this track and enable global track record mode.
			else {
				Page_setTrackRecPattern( target_page, target_page->trackSelection );
				G_track_rec_bit = ON;
			}

			// Mark page as unclear
			target_page->page_clear = OFF;
		}
	}



	//
	// OCTAVE CIRCLE
	//
	if ( MODE_OBJECT_SELECTION == EMBEDDED ){

		// Enter the pitch via inner circle
		key_OCT_CIRCLE_xpose_STEP( keyNdx );

		// Account for track offsets when entering directly..
		key_NUMERIC_QUADRANT_velo_STEP( keyNdx );
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
				#ifdef FEATURE_ENABLE_SONG_UPE
				sequencer_command_PAUSE(OFF);
				#else
				sequencer_command_PAUSE();
				#endif
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
	// BIG KNOB KEYS operation
	//
	if ( 	( keyNdx == 201 )
		||	( keyNdx == 200	)
		||	( keyNdx == 199 )
		|| 	( keyNdx == 198 )
		|| 	( keyNdx == 197 )
		||	( keyNdx == 207 )
		||	( keyNdx == 206 )
		||	( keyNdx == 216 )
		|| 	( keyNdx == 215 )
		||	( keyNdx == 224 )
		){

		switch( my_bit2ndx( target_page->trackAttributeSelection ) +1 ){

			// In DIR we deal with certainty_next
			case ATTR_DIRECTION:

				// Compute row of track selected
				row = my_bit2ndx( target_page->trackSelection );

				// D O U B L E - C L I C K
				if ((DOUBLE_CLICK_TARGET == keyNdx)
						&& (DOUBLE_CLICK_TIMER > DOUBLE_CLICK_ALARM_SENSITIVITY)) {

					// Handle the 100 button - double click sets certainty_next to 0
					if (	( keyNdx == 224 )
						){

						direction_repository[ target_page->Track[row]->attr_DIR ]
							.trigger_set[ current_trigger_set ]
							.certainty_next = 0;
					}
					else{
						// Double click modifieds the large value
						direction_repository[ target_page->Track[row]->attr_DIR ]
							.trigger_set[ current_trigger_set ]
							.certainty_next =

//								(
//									(direction_repository[ target_page->Track[row]->attr_DIR ]
//									.trigger_set[ current_trigger_set ]
//									.certainty_next / 100)*100)
//									+
									( BK_KEY_to_xdx( keyNdx ) *10)
									;
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

					// Single click on non-100 keys sets the ones value, if not at max already
					if (	( keyNdx != 224 )
						&&	( direction_repository[ target_page->Track[row]->attr_DIR ]
								.trigger_set[ current_trigger_set ]
								.certainty_next < 100 )
						){

						// Mmodifies the 1's value, keeping the 10s value
						direction_repository[ target_page->Track[row]->attr_DIR ]
							.trigger_set[ current_trigger_set ]
							.certainty_next =
								( direction_repository[ target_page->Track[row]->attr_DIR ]
									.trigger_set[ current_trigger_set ]
									.certainty_next / 10) *10
								+
								BK_KEY_to_xdx( keyNdx )
								;
					}

					// Single click on 100 sets to 100
					else if (keyNdx == 224 ){

						direction_repository[ target_page->Track[row]->attr_DIR ]
							.trigger_set[ current_trigger_set ]
							.certainty_next = 100;
					}
				}
				break;


			// All other maps..
			default:

				break;
		}

	}


	//
	// CLOCK SOURCE
	//
	if ( keyNdx == KEY_CLOCK ) {

		key_clock_select( target_page, KEY_CLOCK );
	} // KEY_CLOCK was pressed



	//
	// CHORD SELECTOR
	//
	// Selects the CC resolution for the track
	if ( 	( keyNdx > 251 )
		){

		// Make sure we are in the right mode
		if (	( G_zoom_level == zoomMAP )
			&&	( target_page->trackAttributeSelection == ( 1 << MIDICC ) )
			){

			// Pretty ghetto but efficient..
			target_page->Track[my_bit2ndx( target_page->trackSelection )]->CC_resolution = (8 - (keyNdx-251));
		}

	} // Chord selector key pressed



