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

#ifdef FEATURE_CLICK_PLUS
// Value entry in Step mode
void step_enter_value_PITVELAMT( 	unsigned char target_attr,
									signed char* target_val,
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
		#ifdef FEATURE_CLICK_PLUS
		else {
			switch( target_attr ) {
			case ATTR_PITCH:
				if( col == 13 ) {
					*target_val = target_default;
				} else if ( col == 14 && ( *target_val >= -target_max + 12 ) ) {
					// This is the octave- (8vb) button
					*target_val -= 12;
				} else if ( col == 15 && ( *target_val <= target_max - 12 ) ) {
					// This is the octave- (8vb) button
					*target_val += 12;
				}
				break;
			case ATTR_AMOUNT:
				if( col == 13 || col == 14 ) {
					*target_val = target_default;
				}
				else if ( col == 15 )  {
					// Write the value -127 (Eff Step Mask) into the attribute
					*target_val = -127;
				}
				break;
			default:
				*target_val = target_default;
				break;
			}
		}
		#else
		else{
			// Write the default value into the attribute
			*target_val = target_default;
		}
		#endif
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
		else if ( col == 12 ){
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
#endif

void step_event_attribute_toggle( Stepstruct* target_step, unsigned char attribute ) {
	// Click on an active event - turn the event OFF
	if (	( ( attribute ) == (target_step->event_data & 0x0F) )
		&&	( Step_get_status( target_step, STEPSTAT_EVENT ) == ON )
		){

		Step_set_status( target_step, STEPSTAT_EVENT,  OFF );
	} else{

		// Remember the pressed attribute..
		target_step->event_data = (target_step->event_data & 0xF0 ) | ( attribute );

		// ..and turn the event status bit ON
		Step_set_status( target_step, STEPSTAT_EVENT, ON );
	}
}

// Key execution in STEP mode
void key_exec_STEP( unsigned int keyNdx ){

	unsigned char row = 0;
	unsigned char col = 0;
	unsigned char j	  = 0;
	unsigned char temp= 0;

	#ifdef FEATURE_CLICK_PLUS
		unsigned char i	= 0;
		unsigned char clkcol = 0;
	#endif

	// Work on the page under the grid cursor
	Pagestruct* target_page = &Page_repository[GRID_CURSOR];
	Stepstruct* target_step = target_page->Step	[target_page->stepSelectionSingleRow]
	                                           	[target_page->stepSelectionSingleCol];

	#ifdef FEATURE_STEP_EVENT_TRACKS

	temp = ( target_step->event_data & 0x0F ) + 1;
	unsigned char is_altmode = CHECK_BIT( target_step->attr_STATUS, STEP_EVENT_TRACK_ALT_MODE + 5 ) ? TRUE : FALSE;

	#endif

	//
	// TRACK_SELECTORS
	//
	// Edit Track Attribute Selection if one of the valid ones
	if ( 	( keyNdx > 0 )
		&& 	( keyNdx <= 10 )
		){

		// D O U B L E - C L I C K
		if ((DOUBLE_CLICK_TARGET == keyNdx)
				&& (DOUBLE_CLICK_TIMER > DOUBLE_CLICK_ALARM_SENSITIVITY)) {
		#ifdef FEATURE_STEP_EVENT_TRACKS
		temp = ( target_step->event_data & 0x0F ) + 1;
		if (	( temp == ATTR_POSITION ) ) {
			// Flag Flip Flop
			TOGGLE_BIT( target_step->attr_STATUS, STEP_EVENT_TRACK_ALT_MODE + 5 );
			// Set range to default 1
			target_page->Track[ target_page->stepSelectionSingleRow ]->event_max[ATTR_POSITION] = TRACK_DEF_RANGE_EVENT;  //Default==1
			// Reset attr_AMT to Default is not within target value
			target_step->attr_AMT = STEP_DEF_AMOUNT;
		}

		if (	( temp == ATTR_DIRECTION ) ) {
			// Flag Flip Flop -- Track Toggle Shift Skips
			TOGGLE_BIT( target_step->attr_STATUS, STEP_EVENT_TRACK_ALT_MODE + 5 );

			is_altmode = CHECK_BIT( target_step->attr_STATUS, STEP_EVENT_TRACK_ALT_MODE + 5 ) ? TRUE : FALSE;

			if ( is_altmode ) {
				// Set range to default 1 else default 16
				target_page->Track[ target_page->stepSelectionSingleRow ]->event_max[ATTR_DIRECTION] = TRACK_DEF_RANGE_EVENT;  //Default==1

				// Reset attr_AMT to Default
				target_step->attr_AMT = STEP_DEF_AMOUNT;

			}
			else {
				target_page->Track[ target_page->stepSelectionSingleRow ]->event_max[ATTR_DIRECTION] = TRACK_DEF_RANGE_AMT; //Default==16
			}
			// Terminate the clause....
			// Exclude DIRECTION MAP (CUSTOM EDIT) zoom from zoomStep (availbale from zoomTrack only)
			return;
		}

		#endif

			// If clicked on an attribute map
			if ( ( (1 << (keyNdx-1) ) & PANEL_get_trackAttributeMap() ) != 0 ) {
				// Re-store the step event status modified by the single click
				// Click on an active event - turn the event OFF
				if (	( ( keyNdx - 1) ==
					 	  (target_page->Step[ target_page->stepSelectionSingleRow ]
							  		        [ target_page->stepSelectionSingleCol ]->event_data & 0x0F) )
					&&	( Step_get_status( target_page->Step[ target_page->stepSelectionSingleRow ]
									  				        [ target_page->stepSelectionSingleCol ],
									 	   STEPSTAT_EVENT ) == ON )
					){

					Step_set_status( target_page->Step[ target_page->stepSelectionSingleRow ]
									  				  [ target_page->stepSelectionSingleCol ],
									 STEPSTAT_EVENT,
									 OFF );
				}
				else{

					// if ( ( keyNdx != 3 )&&( keyNdx != 4 ) ){

						// Remember the pressed attribute..
						target_page->Step[ target_page->stepSelectionSingleRow ]
								  		 [ target_page->stepSelectionSingleCol ]->event_data
							= 	(target_page->Step[ target_page->stepSelectionSingleRow ]
								  		   	 	  [ target_page->stepSelectionSingleCol ]->event_data & 0xF0)
								| (keyNdx-1);

						// ..and turn the event status bit ON
						Step_set_status( target_page->Step[ target_page->stepSelectionSingleRow ]
										  				  [ target_page->stepSelectionSingleCol ],
										 STEPSTAT_EVENT,
										 ON );

						#ifdef FEATURE_STEP_EVENT_TRACKS

						// CHECK IF POS/DIR & ALT_MODE..
						// Reset attr_AMT to Default if not within target value
						temp = ( target_step->event_data & 0x0F ) + 1;
						if (	( temp == ATTR_POSITION )
								||	 ( temp ==  ATTR_DIRECTION ) ) {

							is_altmode = CHECK_BIT( target_step->attr_STATUS, STEP_EVENT_TRACK_ALT_MODE + 5 ) ? TRUE : FALSE;

							if ( ( is_altmode )
									&&  ( ( target_step->attr_AMT >10 )
											|| ( target_step->attr_AMT < -10 ) )   )  {

								target_step->attr_AMT = STEP_DEF_AMOUNT;
							}
							if (  ( is_altmode )
									&&  ( target_page->Track[ target_page->stepSelectionSingleRow ]->event_max[ATTR_DIRECTION] >10 )  )  {

								// Set range to default 1
								target_page->Track[ target_page->stepSelectionSingleRow ]->event_max[ATTR_DIRECTION] = TRACK_DEF_RANGE_EVENT;
							}
						}
						#endif


					// } // no STA or LEN
				}

				// Make sure the selected attribute is recorded. Else risk 0 value
				target_page->trackAttributeSelection = 1 << (keyNdx-1);

				// Switch zoomLevel to MAP - unless we go to GRV edit mode (STRUM)
				if ( target_page->trackAttributeSelection == (1<<7) ){

					if ( target_page->stepSelection == 1 ) {
						card8 row = target_page->stepSelectionSingleRow;
						card8 col = target_page->stepSelectionSingleCol;
						PhraseSetCurrent( target_page->Step[row][col]->phrase_num );
					}

					// Switch to the Chordstrum edit view
					G_zoom_level = zoomSTRUM;
				}
				else{
					// Select in track the row of the selected step
					target_page->trackSelection = 1 << target_page->stepSelectionSingleRow;

					// Void the step selection
					target_page->stepSelection = 0;
					Step_set_status(
						target_page->Step[ target_page->stepSelectionSingleRow ]
										 [ target_page->stepSelectionSingleCol ],
						STEPSTAT_SELECT, OFF );

					// All other attributes - switch zoom level
					G_zoom_level = zoomMAP;
				}
			}
		}

		else if (DOUBLE_CLICK_TARGET == 0) {

			DOUBLE_CLICK_TARGET = keyNdx;
			DOUBLE_CLICK_TIMER = ON;
			// Start the Double click Alarm
			cyg_alarm_initialize(
				doubleClickAlarm_hdl,
				cyg_current_time() + DOUBLE_CLICK_ALARM_TIME,
				DOUBLE_CLICK_ALARM_TIME );

			// SINGLE CLICK CODE
			// Initialize the event_offsets
			target_page->Track[ target_page->stepSelectionSingleRow ]->event_offset[ATTR_VELOCITY] 	= 0;
			target_page->Track[ target_page->stepSelectionSingleRow ]->event_offset[ATTR_PITCH] 	= 0;
			target_page->Track[ target_page->stepSelectionSingleRow ]->event_offset[ATTR_LENGTH] 	= 0;
			target_page->Track[ target_page->stepSelectionSingleRow ]->event_offset[ATTR_START] 	= 0;
			target_page->Track[ target_page->stepSelectionSingleRow ]->event_offset[ATTR_DIRECTION] = 0;
			target_page->Track[ target_page->stepSelectionSingleRow ]->event_offset[ATTR_AMOUNT] 	= 0;
			target_page->Track[ target_page->stepSelectionSingleRow ]->event_offset[ATTR_GROOVE] 	= 0;
			target_page->Track[ target_page->stepSelectionSingleRow ]->event_offset[ATTR_MIDICC] 	= 0;
			target_page->Track[ target_page->stepSelectionSingleRow ]->event_offset[ATTR_MIDICH] 	= 0;

			// Now set the event status..
			if (target_page->stepSelection == 1){

				// Click on an active event - turn the event OFF
				if (	( ( keyNdx - 1) ==
					 	  (target_page->Step[ target_page->stepSelectionSingleRow ]
							  		        [ target_page->stepSelectionSingleCol ]->event_data & 0x0F) )
					&&	( Step_get_status( target_page->Step[ target_page->stepSelectionSingleRow ]
									  				        [ target_page->stepSelectionSingleCol ],
									 	   STEPSTAT_EVENT ) == ON )
					){

					Step_set_status( target_page->Step[ target_page->stepSelectionSingleRow ]
									  				  [ target_page->stepSelectionSingleCol ],
									 STEPSTAT_EVENT,
									 OFF );
				}
				else{

					// Remember the pressed attribute..
					target_page->Step[ target_page->stepSelectionSingleRow ]
							  		 [ target_page->stepSelectionSingleCol ]->event_data
						= 	(target_page->Step[ target_page->stepSelectionSingleRow ]
							  		   	 	  [ target_page->stepSelectionSingleCol ]->event_data & 0xF0)
							| (keyNdx-1);

					// ..and turn the event status bit ON
					Step_set_status( target_page->Step[ target_page->stepSelectionSingleRow ]
									  				  [ target_page->stepSelectionSingleCol ],
									 STEPSTAT_EVENT,
									 ON );
				}

				#ifdef FEATURE_STEP_EVENT_TRACKS

				// CHECK IF POS/DIR & ALT_MODE..
				// Reset attr_AMT to Default if not within target value
				temp = ( target_step->event_data & 0x0F ) + 1;
				if (	( temp == ATTR_POSITION )
							||	 ( temp ==  ATTR_DIRECTION ) ) {

					is_altmode = CHECK_BIT( target_step->attr_STATUS, STEP_EVENT_TRACK_ALT_MODE + 5 ) ? TRUE : FALSE;

					if ( ( is_altmode )
							&&  ( ( target_step->attr_AMT >10 )
									|| ( target_step->attr_AMT < -10 ) )   )  {

						target_step->attr_AMT = STEP_DEF_AMOUNT;
					}
					if (  ( is_altmode )
								&&  ( target_page->Track[ target_page->stepSelectionSingleRow ]->event_max[ATTR_DIRECTION] >10 )  )  {

						// Set range to default 1
						target_page->Track[ target_page->stepSelectionSingleRow ]->event_max[ATTR_DIRECTION] = TRACK_DEF_RANGE_EVENT;
					}
				}
				#endif


				// Terminate the clause..
				return;
			}
		}

	} // Double click target == keyNdx



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

			// SELECT new step - but only if it is not skipped
//			if ( Step_get_status( target_page->Step[row][col], STEPSTAT_SKIP ) != ON ){

				Step_set_status( target_page->Step[row][col], STEPSTAT_SELECT, ON );

				// ..and increment the counter of steps selected
				target_page->stepSelection++;

				// Remember STEP COORDINATES
				target_page->stepSelectionSingleRow = row;
				target_page->stepSelectionSingleCol = col;

				// Update the selected_step (as when entered on zoomSTEP)
				target_page->selected_step = target_page->Step[ target_page->stepSelectionSingleRow ]
															  [ target_page->stepSelectionSingleCol ];

//			} // Step not skipped

		} // Unselected step was pressed
	}


	// Step selection routine, covers non-Birdseye mode..
	// Step data entry via matrix buttons
	else if ( 		( MODE_OBJECT_SELECTION != BIRDSEYE )
				&&	( keyNdx >  10 )
				&& 	( keyNdx <= 185 )
				&& 	( ((keyNdx-10) % 11) != 0 )
		){

		// Coordinates of already selected step
		row = target_page->stepSelectionSingleRow;
		col = target_page->stepSelectionSingleCol;


		//
		// MIDICH - select another step via matrix keys
		//
		if ( ( ( keyNdx - MIDICH ) % 11) == 0 ){

			// Unselect the original step
			Step_set_status(
				target_page->Step[ target_page->stepSelectionSingleRow ]
								 [ target_page->stepSelectionSingleCol ],
				STEPSTAT_SELECT, OFF );

			// Compute new col coordinates - selected step in the same row;
			// target_page->stepSelectionSingleRow = row_of( keyNdx );
			target_page->stepSelectionSingleCol = column_of( keyNdx );

			// Select the new step
			Step_set_status(
				target_page->Step[ target_page->stepSelectionSingleRow ]
								 [ target_page->stepSelectionSingleCol ],
				STEPSTAT_SELECT, ON );

			// Update the selected_step (as when entered on zoomSTEP)
			target_page->selected_step = target_page->Step[ target_page->stepSelectionSingleRow ]
								                          [ target_page->stepSelectionSingleCol ];
		}


		//
		// VELOCITY - enter via matrix keys
		//

	#ifdef FEATURE_CLICK_PLUS

	if ( ((keyNdx - VELOCITY) % 11) == 0 ) {
		clkcol = ((keyNdx - VELOCITY ) / 11) - 1;
		// Kill the event offset
		target_page->Track[row]->event_offset[ATTR_VELOCITY] = 0;
		// Set keyed value
		step_enter_value_PITVELAMT( ATTR_VELOCITY, &target_page->Step[row][col]->attr_VEL, STEP_MAX_VELOCITY, STEP_DEF_VELOCITY, keyNdx, row, clkcol );
	} // key in the VELOCITY row.
	#else
	if ( ((keyNdx - VELOCITY) % 11) == 0 ){

		// D O U B L E - C L I C K
		if (	(	(DOUBLE_CLICK_TARGET == keyNdx)
				&& (DOUBLE_CLICK_TIMER > DOUBLE_CLICK_ALARM_SENSITIVITY) )
			){

			// Double click code: Modify the tens value
			if ( col < 12 ){
				// Write the final tens value into the step attribute
				target_page->Step[row][col]->attr_VEL =
						normalize( (col+1)*10, 0, STEP_MAX_VELOCITY );
			} // col < 12
			else{
				// Write the final value into the attribute
				target_page->Step[row][col]->attr_VEL = STEP_DEF_VELOCITY;
			} // col >= 12
		} // double click is active

//				else if ( DOUBLE_CLICK_TARGET == 0 ){
//
//					DOUBLE_CLICK_TARGET = keyNdx;
//					DOUBLE_CLICK_TIMER = ON;
//					// Start the Double click Alarm
//					cyg_alarm_initialize(
//						doubleClickAlarm_hdl,
//						cyg_current_time() + DOUBLE_CLICK_ALARM_TIME,
//						DOUBLE_CLICK_ALARM_TIME );
//
//					// SINGLE CLICK CODE:
//					if ( col < 9 ){
//
//						// Modify the ones value.
//						target_page->Step[row][col]->attr_VEL =
//							normalize(
//								(target_page->Step[row][col]->attr_VEL/10)*10 + col+1,
//									0, STEP_MAX_VELOCITY );
//					}
//					else{
//						// ...
//					}
//				} // double click not active

	} // key in the VELOCITY row.
	#endif


	#ifdef FEATURE_CLICK_PLUS

	if ( ((keyNdx - PITCH) % 11) == 0 ) {
		clkcol = ((keyNdx - PITCH ) / 11) - 1;
		// Kill the event offset
		target_page->Track[row]->event_offset[ATTR_PITCH] = 0;
		// Set keyed value
		step_enter_value_PITVELAMT( ATTR_PITCH, &target_page->Step[row][col]->attr_PIT, STEP_MAX_PITCH, STEP_DEF_PITCH, keyNdx, row, clkcol );
	} // key in the PITCH row.


	//
	// LENGTH - enter via matrix keys
	//
	#ifdef FEATURE_TEMPO_MULT_PLUS
	// Set the Step LEN Multiplier using the LEN row Matrix keys
	if ( is_pressed_key( KEY_TEMPO ) ) {

		if ( (keyNdx > 10) && (keyNdx <= 185) && (((keyNdx-10) % 11) != 0) ) {

			if ( ((keyNdx - LENGTH) % 11) == 0) {

				// Set the Step LEN multiplier with POS row buttons
				j = ((keyNdx - LENGTH) / 11);

				if ( j <9 ) {
					// Only set Step LEN multiplier if valid case

					// Coordinates of currently selected step
					row = target_page->stepSelectionSingleRow;
					col = target_page->stepSelectionSingleCol;

					// Set the Step LEN multiplier
					// target_page->Step[row][col]->LEN_multiplier = j;
					target_page->Step[row][col]->event_data =
						( (j<<4) | (target_page->Step[row][col]->event_data & (0x0F)) );

				}
			}
		}
	}
	else if ( ((keyNdx - LENGTH) % 11) == 0 ) {
		clkcol = ((keyNdx - PITCH ) / 11) - 1;
		step_enter_value_LEN( &target_page->Step[row][col]->attr_LEN, STEP_MAX_LENGTH, STEP_DEF_LENGTH, keyNdx, row, clkcol );
	} // key in the LENGTH row.
	#else
	if ( ((keyNdx - LENGTH) % 11) == 0 ) {
			clkcol = ((keyNdx - PITCH ) / 11) - 1;
			step_enter_value_LEN( &target_page->Step[row][col]->attr_LEN, STEP_MAX_LENGTH, STEP_DEF_LENGTH, keyNdx, row, clkcol );
	} // key in the LENGTH row.
	#endif // TEMPO_MULT_PLUS


	//
	// AMOUNT - enter via matrix keys
	//

	else if ( ((keyNdx - AMOUNT) % 11) == 0) {
		// Column of target step
		clkcol = ((keyNdx - AMOUNT) / 11) -1 ;
		// Kill the event offset
		target_page->Track[row]->event_offset[ATTR_AMOUNT] = 0;
		// Set keyed value
		#ifdef FEATURE_STEP_EVENT_TRACKS
		temp = ( target_step->event_data & 0x0F ) + 1;
		if (  ( Step_get_status( target_step, STEPSTAT_EVENT ) == ON )
			&&  ( ( temp >= EVENT_FLAG_TRACK_MUTE )
			||	( ( temp ==  ATTR_POSITION )
			&&	( CHECK_BIT( target_page->Step[row][col]->attr_STATUS, STEP_EVENT_TRACK_ALT_MODE + 5 ) ) )
			||	( ( temp ==  ATTR_DIRECTION )
			&&	( CHECK_BIT( target_page->Step[row][col]->attr_STATUS, STEP_EVENT_TRACK_ALT_MODE + 5 ) ) )	) )  {

			// When in event mode, adjust the amount
			if ( clkcol < 14 ) {
				// Only support invert / default CLK+ (col 12, 13)
				step_enter_value_PITVELAMT( ATTR_AMOUNT, &target_page->Step[row][col]->attr_AMT, TRACK_MAX_RANGE_EVENT, TRACK_DEF_RANGE_EVENT, keyNdx, row, clkcol );
			}
		} else {
			// When in event mode, adjust the amount
			step_enter_value_PITVELAMT( ATTR_AMOUNT, &target_page->Step[row][col]->attr_AMT, STEP_MAX_AMOUNT, STEP_DEF_AMOUNT, keyNdx, row, clkcol );
		}
		#else
		// When in event mode, adjust the amount
		step_enter_value_PITVELAMT( ATTR_AMOUNT, &target_page->Step[row][col]->attr_AMT, STEP_MAX_AMOUNT, STEP_DEF_AMOUNT, keyNdx, row, clkcol );
		#endif
	} // key in the AMOUNT row.



	//
	// GROOVE (Step_Phrase)- enter via matrix keys (only if Step is ON)
	//
	else if( Step_get_status( target_page->Step[row][col], STEPSTAT_TOGGLE ) == ON) {

		if ( ((keyNdx - GROOVE) % 11) == 0) {

			// D O U B L E - C L I C K
			if ((DOUBLE_CLICK_TARGET == keyNdx)
					&& (DOUBLE_CLICK_TIMER > DOUBLE_CLICK_ALARM_SENSITIVITY)) {

				i = target_page->Step[row][col]->phrase_num;

				// Column of target step
				clkcol = ((keyNdx - GROOVE) / 11) - 1 ;

				if ( clkcol < 16 ) {

					if ( i <= 16) {
						// Green - Dbl-Clk for Red
						if ( ( clkcol + 1) == i ) {
							// Same clkcol (ie +16)
							target_page->Step[row][col]->phrase_num = clkcol + 17;
						}
						else if ( ( clkcol + 1) < i ) {
							target_page->Step[row][col]->phrase_num = i - clkcol  + 17;
						}
						else if ( ( clkcol + 1) > i ) {
							target_page->Step[row][col]->phrase_num = clkcol - i + 17;
						}
					}
					else if ( ( i >= 17) && ( i <= 32 ) ) {
						// Red - Dbl-Clk for Orange
						i = i - 16;
						if ( ( clkcol + 1) == i ) {
							// Same clkcol (ie +16)
							target_page->Step[row][col]->phrase_num = clkcol + 33;
						}
						else if ( ( clkcol + 1) < i ) {
							target_page->Step[row][col]->phrase_num = i - clkcol  + 33;
						}
						else if ( ( clkcol + 1) > i ) {
							target_page->Step[row][col]->phrase_num = clkcol - i + 33;
						}
					}
					else if ( ( i >= 33) && ( i < 48 ) ) {
						// Orange - Dbl-Clk for Cancel
						i = i - 32;
						if ( ( clkcol + 1) == i ) {
							// Same clkcol (ie +16)
							//target_page->Step[row][col]->phrase_num = clkcol + 1;
							target_page->Step[row][col]->phrase_num = 0;
						}
						else if ( ( clkcol + 1) < i ) {
							//target_page->Step[row][col]->phrase_num = i - clkcol  + 1;
							target_page->Step[row][col]->phrase_num = 0;
						}
						else if ( ( clkcol + 1) > i ) {
							//target_page->Step[row][col]->phrase_num = clkcol - i + 1;
							target_page->Step[row][col]->phrase_num = 0;
						}
					}
				}
			}

			else if (DOUBLE_CLICK_TARGET == 0) {

				DOUBLE_CLICK_TARGET = keyNdx;
				DOUBLE_CLICK_TIMER = ON;
				// Start the Double click Alarm
				cyg_alarm_initialize(
					doubleClickAlarm_hdl,
					cyg_current_time() + DOUBLE_CLICK_ALARM_TIME,
					DOUBLE_CLICK_ALARM_TIME );

				i = target_page->Step[row][col]->phrase_num;

				// Column of target step
				clkcol = ((keyNdx - GROOVE) / 11) - 1 ;

				if ( clkcol < 16 ) {
					if ( i <= 16) {
						// Green
						target_page->Step[row][col]->phrase_num = normalize( clkcol + 1, 0, 48 );
					}
					else if ( ( i >= 17) && ( i <= 32 ) ) {
						// Red
						target_page->Step[row][col]->phrase_num = normalize( 16 + clkcol + 1, 0, 48 );
					}
					else if ( ( i >= 33) && ( i <= 48 ) ) {
						// Orange
						target_page->Step[row][col]->phrase_num = normalize( 32 + clkcol + 1, 0, 48 );
					}
				}
			}
		} // key in the GROOVE row.

	} // STEPSTAT_TOGGLE == ON


	//
	// MCC - enter via matrix keys
	//
	if ( ((keyNdx - MIDICC) % 11) == 0) {

		// D O U B L E - C L I C K
		if ((DOUBLE_CLICK_TARGET == keyNdx)
				&& (DOUBLE_CLICK_TIMER > DOUBLE_CLICK_ALARM_SENSITIVITY)) {

			// Kill the event offset
			target_page->Track[row]->event_offset[ATTR_MIDICC] = 0;

			// Double click code:
			// Break up original value in tens and ones.
			i = target_page->Step[row][col]->attr_MCC / 10;
			j = target_page->Step[row][col]->attr_MCC % 10;

			// Column of target step
			clkcol = ((keyNdx - MIDICC) / 11) - 1 ;

			if ( clkcol < 12 ){

				// Compute new tens
				i = 10 * (clkcol + 1);
				// Get new ones value, leave it as it is otherwise
				if ( clkcol < 9 ){
					j = 0;
				}
				// Write the final value into the attribute
				target_page->Step[row][col]->attr_MCC = normalize( i + j, 0, (clkcol+1)*10 );
			}
			else{
				// Write the final value into the attribute
				target_page->Step[row][col]->attr_MCC = TRACK_DEF_MIDICC;
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

			// Kill the event offset
			target_page->Track[row]->event_offset[ATTR_MIDICC] = 0;

			// Column of target step
			clkcol = ((keyNdx - MIDICC) / 11) - 1 ;

			if ( clkcol < 9 ){
				// Modify the ones value of the MIDICC. 128 is a special value.
				if ( target_page->Step[row][col]->attr_MCC >= 127 ){
					i = 0;
				}
				else {
					i = target_page->Step[row][col]->attr_MCC / 10;
				}
				target_page->Step[row][col]->attr_MCC = normalize( i*10 + clkcol + 1, 0, 127 );
			}
		}
	} // key in the MCC row.

#endif

} // Key is in the matrix, non-birdseye




	//
	// TRACK_MUTATORS
	//
	if ((keyNdx >= 187) && (keyNdx <= 196)) {

		// Coordinates of already selected step
		row = target_page->stepSelectionSingleRow;
		col = target_page->stepSelectionSingleCol;


		switch (keyNdx) {

			case KEY_TGGL:

//d_iag_printf( "step:%d chord_size:%d\n",
//				(int) &target_page->Step[row][col],
//				target_page->Step[row][col]->chord_up >> 29 );


				// OPTIMIZE!!! Only need to handle one key here!

				// Do not act if there is a step attribute selected
				if ( target_page->stepAttributeSelection != 0 ){

					break;
				}

//				// Go through all selected steps and toggle their activity status
//				// ..G_scaning through all steps of the matrix - not very efficient!!
//				for (row=0; row < MATRIX_NROF_ROWS; row++) {
//					for (col=0; col < MATRIX_NROF_COLUMNS; col++) {
//
//						// If the selection flag is set
//						if (Step_get_status( target_page->Step[row][col], STEPSTAT_SELECT ) == ON){

							// TOGGLE THE ACTIVITY
							if (Step_get_status( target_page->Step[row][col], STEPSTAT_TOGGLE ) == ON){
								// If the activity flag is set - turn it off
								Step_set_status( target_page->Step[row][col], STEPSTAT_TOGGLE, OFF );
							}
							else {
								// Turn ON step
								Step_set_status( target_page->Step[row][col], STEPSTAT_TOGGLE, ON );
							}

							// REMOVE ANY POSSIBLE SKIP BIT
							Step_set_status( target_page->Step[row][col], STEPSTAT_SKIP, OFF );
//						}
//					}
//				}

				break;



			case KEY_RANDOMIZE:
				// Coordinates of already selected step
				row = target_page->stepSelectionSingleRow;
				col = target_page->stepSelectionSingleCol;

				Step_exe_randomize( target_page, target_page->Step[row][col] );

				break;



			case KEY_CLEAR:
				// Coordinates of already selected step
				row = target_page->stepSelectionSingleRow;
				col = target_page->stepSelectionSingleCol;

				Step_clear( target_page, target_page->Step[row][col] );

				// But re-mark the step as selected and active!
				Step_set_status( target_page->Step[row][col], STEPSTAT_SELECT, ON );
				// Step_set_status( target_page->Step[row][col], STEPSTAT_TOGGLE, ON );

				break;



			case KEY_ZOOM:
				// Do not react if a stepAttributeSelection has been made
				if ( target_page->stepAttributeSelection != 0 ){
					break;
				}

				// An existing track selection means that we have zoomed in
				// from a track attribute map (and not from a page).
				// In that case unzoom into the track attribute map (not into page).
				switch( target_page->trackSelection ){

					case 0:
						// Zoomed into here directly from a page
						if (target_page->stepSelection == 1) {

							// Switch back to Step zoom
							G_zoom_level = zoomPAGE;

							// Unselect the step
							// Update the step selection counter in page
							Step_set_status( target_page->Step[row][col], STEPSTAT_SELECT, OFF );
							target_page->stepSelection = OFF;
						}
						break;

					default:
						// Zoomed into here from an attribute map or a track
						// So go back to the MAP or the TRACK

						// Came here from a map, go back there
						if ( target_page->trackAttributeSelection != OFF ){

							G_zoom_level = zoomMAP;
						}
						// Came here from a track, go back there
						else{

							G_zoom_level = zoomTRACK;
						}

						// Unselect the step
						// Update the step selection counter in page
						Step_set_status( target_page->Step[row][col], STEPSTAT_SELECT, OFF );
						target_page->stepSelection = OFF;

						break;
				}
				break;


			case KEY_COPY:
				if (target_page->stepSelection != 1){
					break;
				}

				// Coordinates of already selected step
				row = target_page->stepSelectionSingleRow;
				col = target_page->stepSelectionSingleCol;

				// Copy the step pointer to the buffer
				STEP_COPY_BUFFER = target_page->Step[row][col];
				break;


			case KEY_PASTE:

				if (target_page->stepSelection != 1){
					break;
				}

				// Coordinates of already selected step
				row = target_page->stepSelectionSingleRow;
				col = target_page->stepSelectionSingleCol;

				// Copy step data from buffer to pointer
				if ( STEP_COPY_BUFFER != NULL ){

					Step_copy( STEP_COPY_BUFFER, target_page->Step[row][col], false );
#ifdef COPY_BUFFER_FRESH
					STEP_COPY_BUFFER = NULL;
#endif
				}
				break;

		} // switch( keyNdx )
	}
	#ifdef FEATURE_STEP_EVENT_TRACKS
	//
	// MIX TARGETS
	//
	else if (	( (keyNdx == KEY_MIXTGT_USR1) )
			||	( (keyNdx == KEY_MIXTGT_USR2) )
			||	( (keyNdx == KEY_MIXTGT_USR3) )
			||	( (keyNdx == KEY_MIXTGT_USR4) ) ){
		unsigned char temp = 0;
		unsigned char event_data_attr = ( target_step->event_data & 0x0F ) + 1;
		// Initialize the event_offsets
		init_track_event_offsets( target_page->Track[ target_page->stepSelectionSingleRow ] );
		// Now set the event status..
		if (target_page->stepSelection == 1){

			switch( keyNdx ){
				case KEY_MIXTGT_USR1: 	temp = 10;	break;	// EVENT_FLAG_TRACK_MUTE
				case KEY_MIXTGT_USR2: 	temp = 11;	break;	// EVENT_FLAG_TRACK_SOLO
				case KEY_MIXTGT_USR3: 	temp = 12;	break;	// EVENT_FLAG_TRACK_RECORD
				case KEY_MIXTGT_USR4: 	temp = 13;	break;	// EVENT_FLAG_TRACK_PAUSE
				default:	temp = 20;	break;	// not a valid key press
			}

			if ( event_data_attr < EVENT_FLAG_TRACK_MUTE ) {
				target_step->attr_AMT = 0;
			} else {
				// D O U B L E - C L I C K  C O N S T R U C T
				// DOUBLE CLICK SCENARIO
				if ((DOUBLE_CLICK_TARGET == keyNdx)
						&& (DOUBLE_CLICK_TIMER > DOUBLE_CLICK_ALARM_SENSITIVITY)) {

					// Double click code
					// ...
					// Flag Flip Flop
					TOGGLE_BIT( target_step->attr_STATUS, STEP_EVENT_TRACK_ALT_MODE + 5 );
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
				}
			}
			step_event_attribute_toggle( target_step, temp );
		}
	}
	#endif


	//
	// CIRCLE ENTERING FIELDS
	//
	switch( MODE_OBJECT_SELECTION ){

		case CHORDEYE:
		#ifdef FEATURE_ENABLE_CHORD_OCTAVE
		case CHORDEYE_OCTAVE_FIRST:
		case CHORDEYE_OCTAVE_SECOND:
		case CHORDEYE_OCTAVE_THIRD:
		#endif
			// Set chord_size according to the pressed chord button

			// Enter the chord aux notes via inner circle
			key_OCT_CIRCLE_chord_STEP( keyNdx );
			break;

		default:
			// Enter the pitch via inner circle
			key_OCT_CIRCLE_xpose_STEP( keyNdx );
			break;
	}


	//
	// Enter VELOCITY using the BK keys
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


		// Determine Step coordinates
		row = target_page->stepSelectionSingleRow;
		col = target_page->stepSelectionSingleCol;

		switch( MODE_OBJECT_SELECTION ){

			case CHORDEYE:
			#ifdef FEATURE_ENABLE_CHORD_OCTAVE
			case CHORDEYE_OCTAVE_FIRST:
			case CHORDEYE_OCTAVE_SECOND:
			case CHORDEYE_OCTAVE_THIRD:
			#endif
			// target_page->Step[row][col]->chord_data =
			// 	( target_page->Step[row][col]->chord_data & 0x8FF )
			//	|	( BK_KEY_to_xdx( keyNdx ) << 11 ) ;
				break;

			default:

				// If an event is set show the range defined for set event.
				if ( Step_get_status( target_page->Step[row][col], STEPSTAT_EVENT ) != ON
	   				){

					// Enter velocity via outter circle
					// Account for track offsets when entering directly..
					key_NUMERIC_QUADRANT_velo_STEP( keyNdx );
				}
				break;
		}
	}


	//
	// ZOOM MODE
	//
	// GRID zoom mode
	if (	(keyNdx == KEY_ZOOM_GRID)
		||	(keyNdx == KEY_ZOOM_PAGE)
		|| 	(keyNdx == KEY_ZOOM_TRACK) ){

		switch( keyNdx ){

			case KEY_ZOOM_GRID:
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
				target_page->selected_step = NULL;

				G_zoom_level = zoomGRID;
				// Keymode is implicit for GRID
				break;

			case KEY_ZOOM_PAGE:
				// Unselect the step
				row = target_page->stepSelectionSingleRow;
				col = target_page->stepSelectionSingleCol;
				Step_set_status( target_page->Step[row][col], STEPSTAT_SELECT, OFF );

				// Clear the STEP COORDINATES
				target_page->stepSelectionSingleRow = 0;
				target_page->stepSelectionSingleCol = 0;
				target_page->stepSelection = 0;
				target_page->selected_step = NULL;

				// Set the keymode
				G_zoom_level = zoomPAGE;
				break;

			case KEY_ZOOM_TRACK:
				// Select the track that the zoomed step belongs to
				target_page->trackSelection = ( 1 << target_page->stepSelectionSingleRow );
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

					// Mark step un-selected
					Step_set_status( target_page->Step[row][col], STEPSTAT_SELECT, OFF);

					// And decrement step selection counter
					target_page->stepSelection --;
				}
			}
			target_page->selected_step = NULL;
		}

	} // keyNdx is a KEY_ZOOM_xxx



	#ifdef FEATURE_TEMPO_MULT_PLUS
	if ( is_pressed_key( KEY_TEMPO ) ) {
		// Enable Transport PLAY, PAUSE & STOP when KEY_TEMPO held
		if ( keyNdx == KEY_PLAY1 ) {
			// Make sure the sequencer is running
			if ( G_clock_source != EXT ) {
				sequencer_command_PLAY();
			}
		}

		if ( keyNdx == KEY_STOP ) {
			if ( G_clock_source != EXT ) {
				sequencer_command_STOP();
			}
		}
	}
	else {
		// Set the LEN multiplier with TRANSPORT buttons
		if (	( keyNdx == KEY_PLAY1 )
			||	( keyNdx == KEY_PLAY2 )
			||	( keyNdx == KEY_PLAY4 )
		){

		switch( keyNdx ){

			case KEY_PLAY1:
				j = 1;
				break;

			case KEY_PLAY2:
				j = 2;
				// Add one to the keypress
				if ( is_pressed_key( KEY_PLAY1 )){
					j = 3;
				}
				else if ( is_pressed_key( KEY_PLAY4 )){
					j = 8;
				}
				break;

			case KEY_PLAY4:
				j = 4;

				// Add one to the keypress
				if ( is_pressed_key( KEY_PLAY1 )){
					j = 5;
					// Add two additional to the keypress
					if ( is_pressed_key( KEY_PLAY2 )){
						j = 7;
					}
				}
				else if ( is_pressed_key( KEY_PLAY2 )){
					j = 6;
				}
				break;
			} // switch( keyNdx )

			// Coordinates of currently selected step
			row = target_page->stepSelectionSingleRow;
			col = target_page->stepSelectionSingleCol;

			// Set the Step LEN multiplier
			// target_page->Step[row][col]->LEN_multiplier = j;
			target_page->Step[row][col]->event_data =
					( (j<<4) | (target_page->Step[row][col]->event_data & (0x0F)) );

		} // PLAYx key pressed
	}

	#else
	//
	// TRANSPORT buttons
	//

	// Set the LEN multiplier
	if (	( keyNdx == KEY_PLAY1 )
		||	( keyNdx == KEY_PLAY2 )
		||	( keyNdx == KEY_PLAY4 )
	){

	switch( keyNdx ){

		case KEY_PLAY1:
			j = 1;
			break;

		case KEY_PLAY2:
			j = 2;

			// Add one to the keypress
			if ( is_pressed_key( KEY_PLAY1 )){
				j = 3;
			}
			else if ( is_pressed_key( KEY_PLAY4 )){
				j = 8;
			}
			break;

		case KEY_PLAY4:
			j = 4;

			// Add one to the keypress
			if ( is_pressed_key( KEY_PLAY1 )){
				j = 5;
				// Add two additional to the keypress
				if ( is_pressed_key( KEY_PLAY2 )){
					j = 7;
				}
			}
			else if ( is_pressed_key( KEY_PLAY2 )){
				j = 6;
			}
			break;
	} // switch( keyNdx )

	// Coordinates of currently selected step
	row = target_page->stepSelectionSingleRow;
	col = target_page->stepSelectionSingleCol;

	// Set the Step LEN multiplier
	// target_page->Step[row][col]->LEN_multiplier = j;
	target_page->Step[row][col]->event_data =
			( (j<<4) | (target_page->Step[row][col]->event_data & (0x0F)) );

	} // PLAYx key pressed

	#endif


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


	#ifdef FEATURE_ENABLE_CHORD_OCTAVE
	if( keyNdx > 251 && keyNdx < 259 ) {
		// CHORD button
		if ( MODE_OBJECT_SELECTION <= CHORDEYE ) {

			// D O U B L E - C L I C K
			if (	(	(DOUBLE_CLICK_TARGET == keyNdx)
					&& (DOUBLE_CLICK_TIMER > DOUBLE_CLICK_ALARM_SENSITIVITY) )
				){
				MODE_OBJECT_SELECTION = CHORDEYE_OCTAVE_FIRST;
			} else if ( DOUBLE_CLICK_TARGET == 0 ){

				DOUBLE_CLICK_TARGET = keyNdx;
				DOUBLE_CLICK_TIMER = ON;
				// Start the Double click Alarm
				cyg_alarm_initialize(
					doubleClickAlarm_hdl,
					cyg_current_time() + DOUBLE_CLICK_ALARM_TIME,
					DOUBLE_CLICK_ALARM_TIME );

				// SINGLE CLICK CODE:
				// Enter the CHORDEYE view
				MODE_OBJECT_SELECTION = CHORDEYE;
			}
		}
	}

	if( keyNdx == KEY_SCALE_MYSEL ) {
		// Set chord view mode
		MODE_OBJECT_SELECTION = ( MODE_OBJECT_SELECTION == CHORDEYE ) ? CHORDEYE_OCTAVE_FIRST : CHORDEYE;
	} else {
		// 1 - 3 switches stack octave chord view
		switch( keyNdx ) {
			case KEY_SCALE_MOD:
				MODE_OBJECT_SELECTION = CHORDEYE_OCTAVE_FIRST;
				break;
			case KEY_SCALE_SEL:
				MODE_OBJECT_SELECTION = CHORDEYE_OCTAVE_SECOND;
				break;
			case KEY_SCALE_CAD:
				MODE_OBJECT_SELECTION = CHORDEYE_OCTAVE_THIRD;
				break;
		}
	}
	#endif


	#ifdef FEATURE_ZOOMSTEP_PLUS
	if ( keyNdx == KEY_MUTE_MASTER ){
		row = target_page->stepSelectionSingleRow;
		col = target_page->stepSelectionSingleCol;

		if (  (  Step_get_status( target_page->Step[row][col], STEPSTAT_SKIP )  ) == OFF )  {
			Step_set_status( target_page->Step[row][col], STEPSTAT_SKIP, ON );
		}
		else {
			Step_set_status( target_page->Step[row][col], STEPSTAT_SKIP, OFF );
		}

	}
	#endif
}


