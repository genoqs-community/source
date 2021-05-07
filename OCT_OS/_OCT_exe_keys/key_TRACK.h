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
	// TRACK SELECTORS
	//
	if ((keyNdx > 0) && (keyNdx <= 10)) {

		switch( MODE_OBJECT_SELECTION ){

			// Select change track selection
			case BIRDSEYE:
				// Change the track selection in page
				target_page->trackSelection = ( 1 << (keyNdx - 1) );
				break;


			// Map selector is held pressed
			case PASSIVE:
//				break;


			// No map selector kep pressed, regular interactive mode
			case INTERACTIVE:

				// Edit Track Attribute Selection if one of the valid ones
				if ( ( (1 << (keyNdx-1) ) & PANEL_get_trackAttributeMap() ) != 0 ) {

					// D O U B L E - C L I C K
					if ((DOUBLE_CLICK_TARGET == keyNdx)
							&& (DOUBLE_CLICK_TIMER > DOUBLE_CLICK_ALARM_SENSITIVITY)) {

						// Make sure the selected attribute is recorded. Else risk 0 value
						target_page->trackAttributeSelection = 1 << (keyNdx-1);

						// Switch zoomLevel to MAP - unless we go to GRV edit mode (STRUM)
						if ( target_page->trackAttributeSelection == (1<<7) ){
							// Switch to the Chordstrum edit view
							G_zoom_level = zoomSTRUM;

//							if ( target_page->stepSelection == 1 ) {
//								card8 row = target_page->stepSelectionSingleRow;
//								card8 col = target_page->stepSelectionSingleCol;
//								PhraseSetCurrent( target_page->Step[row][col]->phrase_num );
//							}
						}
						else{
							// All other attributes
							G_zoom_level = zoomMAP;
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

						// Toggle the attribute selection
						if (target_page->trackAttributeSelection == (1 << (keyNdx-1)) ) {
							target_page->trackAttributeSelection = 0;
						}
						else {
							target_page->trackAttributeSelection = (1 << (keyNdx-1));
						}
					}
				} // Double click target == keyNdx

				else{

					// target_page->trackAttributeSelection = 0;
				}

				break;
		}

	} // key is a track selector



	//
	// MATRIX KEYS
	//
	// If the key pressed is in the Matrix
	if ( (keyNdx > 10) && (keyNdx <= 185) && (((keyNdx-10) % 11) != 0) ) {


		// VELOCITY
		//
		if ( ((keyNdx - VELOCITY) % 11) == 0) {

			// Row of target track
			row = my_bit2ndx( target_page->trackSelection );

			// Column of target step
			col = ((keyNdx - VELOCITY) / 11) - 1 ;

			// Kill the event offset
			target_page->Track[row]->event_offset[ATTR_VELOCITY] = 0;

			// D O U B L E - C L I C K
			if ((DOUBLE_CLICK_TARGET == keyNdx)
					&& (DOUBLE_CLICK_TIMER > DOUBLE_CLICK_ALARM_SENSITIVITY)) {

				// Double click code:
				//
				// Modify the tens value of the AMOUNT

				// Break up original value in tens and ones.
				i = target_page->Track[row]->attr_VEL / 10;
				j = target_page->Track[row]->attr_VEL % 10;

				if ( col < 12 ){

					// Compute new tens
					i = 10 * (col + 1);

					// Get new ones value, leave it as it is otherwise
					if ( col < 9 ){
						// j = col + 1;
						j = 0;
					}

					// Write the final value into the attribute
					target_page->Track[row]->attr_VEL =
						normalize( i + j, 0, 127 );
				}
				else{

					// Write the final value into the attribute
					target_page->Track[row]->attr_VEL = TRACK_DEF_VELOCITY;
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

				// Single click code:
				//
				if ( col < 9 ){

					if ( target_page->Track[row]->attr_VEL >= 127 ){
						i = 0;
					}
					else {
						i = target_page->Track[row]->attr_VEL / 10;
					}
					target_page->Track[row]->attr_VEL =
						normalize( i*10 + col + 1 - target_page->Track[row]->event_offset[ATTR_VELOCITY], 0, TRACK_MAX_VELOCITY );
				}
			}
		} // key in the AMOUNT row.



		// POSITION (position) row allows step toggle edit for the track
		// The functionality should be identical to the one in Page mode.
		//
		if ( ((keyNdx - POSITION) % 11) == 0) {

			// Needs some more elaboration but works for now
			target_page->page_clear = OFF;

			// Compute Step coordinates
			// Row of target track
			row = my_bit2ndx( target_page->trackSelection );

			// Column of target step
			col = ((keyNdx - POSITION) / 11) - 1 ;

			// d_iag_printf( "step row:%d col:%d ts:%d\n",
			//	row, col, target_page->trackSelection );

			// Toggle, un/select or un/mute the step in question
			interpret_matrix_stepkey( row, col, target_page );

		} // key is in POSITION row



		// PITCH row allows direct pitch entry for the track
		//
		if ( ((keyNdx - PITCH) % 11) == 0) {

			// D O U B L E - C L I C K
			if ((DOUBLE_CLICK_TARGET == keyNdx)
					&& (DOUBLE_CLICK_TIMER > DOUBLE_CLICK_ALARM_SENSITIVITY)) {

				// Double click code:
				//
				// Modify the scale setting
				// Row of target track
				row = my_bit2ndx( target_page->trackSelection );

				// Column of target step
				col = ((keyNdx - PITCH) / 11) - 1 ;

				// Make sure we stay in range
				if ( ( (col+1) * 12) < TRACK_MAX_PITCH ){

					// Kill the event offset
					target_page->Track[row]->event_offset[ATTR_PITCH] = 0;

					// Adjust the pitch accordingly	- set the octave pitch.
					target_page->Track[row]->attr_PIT = (col+1) * 12;
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

				// Single click code:
				//
				if ( ((keyNdx - PITCH) % 11) == 0) {
					// Key press is in the PITCH row

					// Row of target track
					row = my_bit2ndx( target_page->trackSelection );

					// Column of target step
					col = ((keyNdx - PITCH) / 11) - 1 ;

					// Kill the event offset
					target_page->Track[row]->event_offset[ATTR_PITCH] = 0;

					// Absolute keyindex of pressed button
					j = ((col+1) * 11) + (row);

					// We are within the legal range
					if ( ( (target_page->Track[row]->attr_PIT / 12) * 12
								+ (col - 2) ) < TRACK_MAX_PITCH ){

						// This is a scale button
						if ( (col >= 2) && (col <= 13) ){

							// Adjust the pitch accordingly
							target_page->Track[row]->attr_PIT =
									(target_page->Track[row]->attr_PIT / 12) * 12
								+ 	(col - 2);
						}
					}
				}
			} // Single click code
		} // key is in PITCH row



		// START (START) row allows direct entry of data.
		// LENGTH (LENGTH)
		// More complex - need to block them from conflicting with each other,
		// so right now both will stay as they are.



		// START row allows direct entry of data.
		//
		if ( ((keyNdx - START) % 11) == 0) {

			// Target coordinates
			row = my_bit2ndx( target_page->trackSelection );
			col = ((keyNdx - START) / 11) - 1 ;

			// Set the new value of the STA_factor
			target_page->Track[row]->STA_factor = col +1;

		} // key in the START row.


		// LENGTH row allows direct entry of data.
		//
		if ( ((keyNdx - LENGTH) % 11) == 0) {

			// Target coordinates
			row = my_bit2ndx( target_page->trackSelection );
			col = ((keyNdx - LENGTH) / 11) - 1 ;

			// Set the new value of the LEN_factor
			target_page->Track[row]->LEN_factor = col +1;

		} // key in the START row.


		// DIRECTION (direction) row allows direct entry of data.
		//
		if ( ((keyNdx - DIRECTION) % 11) == 0) {

			// Row of target track
			row = my_bit2ndx( target_page->trackSelection );

			// Column of target step
			col = ((keyNdx - DIRECTION) / 11) - 1 ;

			// Set the new value of the GROOVE attribute - on the same port.
			target_page->Track[row]->attr_DIR
				= normalize( col + 1, 0, TRACK_MAX_DIRECTION );
		} // key in the DIRECTION row.



		// AMOUNT (Track amount) row allows direct entry of data.
		//
		if ( ((keyNdx - AMOUNT) % 11) == 0) {

			target_page->Track[row]->event_offset[ATTR_AMOUNT] = 0;

			// Row of target track
			row = my_bit2ndx( target_page->trackSelection );

			// Column of target step
			col = ((keyNdx - AMOUNT) / 11) - 1 ;

			// D O U B L E - C L I C K
			if ((DOUBLE_CLICK_TARGET == keyNdx)
					&& (DOUBLE_CLICK_TIMER > DOUBLE_CLICK_ALARM_SENSITIVITY)) {

				// Double click code:
				//
				// Modify the tens value of the AMOUNT

				// Break up original value in tens and ones.
				i = target_page->Track[row]->attr_AMT / 10;

				if ( col < 10 ){

					// Compute new tens
					i = 10 * (col + 1);

					// Write the final value into the attribute
					target_page->Track[row]->attr_AMT =
						normalize( i, 0, 100 );
				}
				else{

					// Write the final value into the attribute
					target_page->Track[row]->attr_AMT = TRACK_DEF_AMOUNT;
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

				// Single click code:
				//
				if ( col < 9 ){
					// Modify the ones value of the MIDICC. 128 is a special value.
					if ( target_page->Track[row]->attr_AMT >= 127 ){
						i = 0;
					}
					else {
						i = target_page->Track[row]->attr_AMT / 10;
					}
					target_page->Track[row]->attr_AMT =
						normalize( i*10 + col + 1, 0, 127 );
				}
			}
		} // key in the AMOUNT row.




		// GROOVE (groove) row allows direct entry of data.
		//
		if ( ((keyNdx - GROOVE) % 11) == 0) {

			// Row of target track
			row = my_bit2ndx( target_page->trackSelection );

			// Column of target step
			col = ((keyNdx - GROOVE) / 11) - 1 ;

			// D O U B L E - C L I C K
			if ((DOUBLE_CLICK_TARGET == keyNdx)
					&& (DOUBLE_CLICK_TIMER > DOUBLE_CLICK_ALARM_SENSITIVITY)) {

				// Double click code:
				//
				// Set GROOVE attribute value to 0
				target_page->Track[row]->attr_GRV = 0;
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
				target_page->Track[row]->attr_GRV = col + 1;
			}
		} // key in the GROOVE row.




		// MIDICC (MIDI CC) row allows direct entry of data.
		//
		if ( ((keyNdx - MIDICC) % 11) == 0) {

			// Row of target track
			row = my_bit2ndx( target_page->trackSelection );

			// Column of target step
			col = ((keyNdx - MIDICC) / 11) - 1 ;

			target_page->Track[i]->event_offset[ATTR_MIDICC] = 0;

			// D O U B L E - C L I C K
			if ((DOUBLE_CLICK_TARGET == keyNdx)
					&& (DOUBLE_CLICK_TIMER > DOUBLE_CLICK_ALARM_SENSITIVITY)) {

				// Double click code:
				//
				// Modify the tens value of the MIDICC

				// Break up original value in tens and ones.
				i = target_page->Track[row]->attr_MCC / 10;
				j = target_page->Track[row]->attr_MCC % 10;

				if ( col < 12 ){

					// Compute new tens
					i = 10 * (col + 1);

					// Get new ones value, leave it as it is otherwise
					if ( col < 9 ){
						// j = col + 1;
						j = 0;
					}

					// Write the final value into the attribute
					target_page->Track[row]->attr_MCC =
						normalize( i + j, 0, 127 );
				}
				else{

					// Write the final value into the attribute
					target_page->Track[row]->attr_MCC = TRACK_DEF_MIDICC;
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

				// Single click code:
				//
				if ( col < 9 ){
					// Modify the ones value of the MIDICC. 128 is a special value.
					if ( target_page->Track[row]->attr_MCC >= 127 ){
						i = 0;
					}
					else {
						i = target_page->Track[row]->attr_MCC / 10;
					}
					target_page->Track[row]->attr_MCC =
						normalize( i*10 + col + 1, 0, 127 );
				}
			}
		} // key in the MIDICC row.





		// MIDICH (MIDI Channel) row allows direct entry of data.
		// Single click selects channel on the current port. Double click on the other port.
		// Disregards the virtual midi channels
		if ( ((keyNdx - MIDICH) % 11) == 0) {

			// Row of target track
			row = my_bit2ndx( target_page->trackSelection );

			// Column of target step
			col = ((keyNdx - MIDICH) / 11) - 1 ;

			// D O U B L E - C L I C K
			if ((DOUBLE_CLICK_TARGET == keyNdx)
					&& (DOUBLE_CLICK_TIMER > DOUBLE_CLICK_ALARM_SENSITIVITY)) {

				// Double click code:
				///
				// Switch the MIDI port
				// Find out the current port: it will be 0 or 1;
				// Note that the ATTR_MIDICH is in the range [1..32].
				i = target_page->Track[row]->attr_MCH / 17;
				j = target_page->Track[row]->attr_MCH - (16 * i);

				// Swith the port indicator
				i ^= 1;

				// Write the new value to the attribute
				target_page->Track[row]->attr_MCH = (16 * i) + j;
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
				i = target_page->Track[row]->attr_MCH / 17;

				// Set the new value of the MIDICH attribute - on the same port.
				target_page->Track[row]->attr_MCH = (16 * i) + col + 1;
			}
		} // key in the MIDICH row.

	} // If this is a key in the Matrix



	//
	// TRACK MUTATORS
	//
	// Identify selected track so we know what to operate on
	for (i=0; i<MATRIX_NROF_ROWS; i++){
		if ( ((1<<i) & (target_page->trackSelection)) != 0){
			row = i;
		}
	}

	// ENTITY MODE:
	if (target_page->trackAttributeSelection == OFF) {

		// TGGL function: set or unset the mute bit on the track
		if (keyNdx == KEY_TGGL) {
			apply_page_track_mute_toggle_operation( target_page, target_page->Track[row], MASK( OPERATION_MUTE ) );
			if( CHECK_BIT( target_page->trackSolopattern, row ) ) {
				apply_page_track_mute_toggle_operation( target_page, target_page->Track[row], MASK( OPERATION_SOLO ) );
			}
		}


		// SOLO function: set or unset the solo bit of the track
		if (keyNdx == KEY_SOLO) {
			if( CHECK_BIT( target_page->trackMutepattern, row ) ) {
				apply_page_track_mute_toggle_operation( target_page, target_page->Track[row], MASK( OPERATION_MUTE ) );
			}
			apply_page_track_mute_toggle_operation( target_page, target_page->Track[row], MASK( OPERATION_SOLO ) );
		}


		// CLEAR function
		if ( keyNdx == KEY_CLEAR ) {

			// Clear track fully
			Track_clear_full( target_page, row );
		}


		// RANDOMIZE function
		if (keyNdx == KEY_RANDOMIZE) {

			// Randomize the selected track in page. This format should be used everywhere in here..
			Page_RNDMZ_selected_tracks( target_page );
		}


		// FLIP TRACK function
		if( keyNdx == KEY_FLT ){

			if ( my_bit_cardinality( target_page->trackSelection ) > 1 ){

				// Flatten the selected tracks in page. This format should be used everywhere in here..
				Page_FLT_selected_tracks( target_page );
			}
		}


		// REMIX TRACK function
		if( keyNdx == KEY_RMX ){

			// Remix the selected tracks in page.
			Page_RMX_selected_tracks( target_page );
		}


		// EFF TRACK function
		if( keyNdx == KEY_EFF ){

			// Calculate the row of the selected track
			row = my_bit2ndx( target_page->trackSelection );

			// Cycle through the EFF states: OFF (0), SEND (1), RECEIVE (2), RECEIVESEND (3)
			temp = Track_get_MISC( target_page->Track[row], EFF_BIT );
			Track_set_MISC( target_page->Track[row], EFF_BIT, (temp+1) % 4 );

		} // KEY_EFF


		// ZOOM function
		if (keyNdx == KEY_ZOOM) {

			// Make sure there are no steps selected
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
						Step_set_status( target_page->Step[row][col], STEPSTAT_SELECT, OFF);

						// And decrement step selection counter
						target_page->stepSelection --;
					}
				}
			}

			// ..and Zoom out into the previos mode -> Switch zoom level
			G_zoom_level = zoomPAGE;
		}


		// COPY function
		if (keyNdx == KEY_COPY) {

			// Remember the selected track in terms of page and row
			TRACK_COPY_BUFFER = ( 	target_page->trackSelection
								| ( target_page->pageNdx << 10 )
								);

			// Clear the track selection
			target_page->trackSelection = 0;

			G_zoom_level = zoomPAGE;
		}

		// PASTE function
		if (keyNdx == KEY_PASTE) {

			// Check if there is anything in the copy buffer
			if ( TRACK_COPY_BUFFER != 0 ) {

				// Get the 10 trackselection bits
				row = my_bit2ndx( TRACK_COPY_BUFFER & 0x3FF );

				// Get the source page index
				j = TRACK_COPY_BUFFER >> 10;

				// Execute the copy operation
				Track_copy( &Page_repository[j], row,
							target_page, my_bit2ndx( target_page->trackSelection ) );
			}

#ifdef COPY_BUFFER_FRESH
			TRACK_COPY_BUFFER = 0;
#endif

		}
	} // Entity mode operations



	// ATTRIBUTE selected mode operations
	else {

		switch (target_page->trackAttributeSelection) {


			case (1<< VELOCITY):

				if (keyNdx == KEY_RANDOMIZE){

					for ( i=0; i < MATRIX_NROF_COLUMNS; i++ ){

						// Randomize step velocity
						target_page->Step[row][i]->attr_VEL
							= randomize_byamount(	target_page->Step[row][i]->attr_VEL,
													target_page->Track[row]->attr_AMT
													+ target_page->Track[row]->event_offset[ATTR_AMOUNT]
													,
													STEP_MIN_VELOCITY,
													STEP_MAX_VELOCITY );
					}
				}

				if (keyNdx == KEY_CLEAR){

					// Reset the velocity map of the track - i.e. all track velocities
					for ( i=0; i < MATRIX_NROF_COLUMNS; i++ ){

						target_page->Step[row][i]->attr_VEL = STEP_DEF_VELOCITY;
						target_page->Track[row]->VEL_factor = TRACK_DEF_VELFACTOR;
					}
				}

				break;



			case (1<< PITCH):

				if (keyNdx == KEY_RANDOMIZE){

					for ( i=0; i < MATRIX_NROF_COLUMNS; i++ ){

						// Randomize step velocity
						target_page->Step[row][i]->attr_PIT
							= randomize_byamount(	target_page->Step[row][i]->attr_PIT,
													target_page->Track[row]->attr_AMT
													+ target_page->Track[row]->event_offset[ATTR_AMOUNT]
													,
													STEP_MIN_PITCH,
													STEP_MAX_PITCH );
					}
				}

				if (keyNdx == KEY_CLEAR){

					// Reset the velocity map of the track - i.e. all track velocities
					for ( i=0; i < MATRIX_NROF_COLUMNS; i++ ){

						target_page->Step[row][i]->attr_PIT = STEP_DEF_PITCH;
						target_page->Track[row]->PIT_factor = TRACK_DEF_PITFACTOR;
						target_page->Track[row]->event_offset[ATTR_PITCH] = 0;
					}
				}
				break;



			case ( 1<< LENGTH ):

				if (keyNdx == KEY_RANDOMIZE){

					for ( i=0; i < MATRIX_NROF_COLUMNS; i++ ){

						// Randomize step velocity
						target_page->Step[row][i]->attr_LEN
							= randomize_byamount(	target_page->Step[row][i]->attr_LEN,
													target_page->Track[row]->attr_AMT
													+ target_page->Track[row]->event_offset[ATTR_AMOUNT]
													,
													STEP_MIN_LENGTH,
													STEP_MAX_LENGTH );
					}
				}

				if (keyNdx == KEY_CLEAR){

					// Reset the velocity map of the track - i.e. all track velocities
					for ( i=0; i < MATRIX_NROF_COLUMNS; i++ ){

						target_page->Step[row][i]->attr_LEN = STEP_DEF_LENGTH;
						target_page->Track[row]->LEN_factor = TRACK_DEF_LENFACTOR;
					}
				}
				break;



			case ( 1<< START ):

				if (keyNdx == KEY_RANDOMIZE){

					for ( i=0; i < MATRIX_NROF_COLUMNS; i++ ){

						// Randomize step velocity
						target_page->Step[row][i]->attr_STA
							= randomize_byamount(	target_page->Step[row][i]->attr_STA,
													target_page->Track[row]->attr_AMT
													+ target_page->Track[row]->event_offset[ATTR_AMOUNT]
													,
													STEP_MIN_START,
													STEP_MAX_START );
					}
				}

				if (keyNdx == KEY_CLEAR){

					// Reset the velocity map of the track - i.e. all track velocities
					for ( i=0; i < MATRIX_NROF_COLUMNS; i++ ){

						target_page->Step[row][i]->attr_STA = STEP_DEF_START;
						target_page->Track[row]->STA_factor = TRACK_DEF_STAFACTOR;
					}
				}
				break;



			case ( 1<< MIDICC ):

				if (keyNdx == KEY_RANDOMIZE){

					for ( i=0; i < MATRIX_NROF_COLUMNS; i++ ){

						// Randomize step velocity
						target_page->Step[row][i]->attr_MCC
							= randomize_byamount(	target_page->Step[row][i]->attr_MCC,
													target_page->Track[row]->attr_AMT
													+ target_page->Track[row]->event_offset[ATTR_AMOUNT]
													,
													STEP_MIN_MIDICC,
													STEP_MAX_START );
					}
				}

				if (keyNdx == KEY_CLEAR){

					// Reset the velocity map of the track - i.e. all track velocities
					for ( i=0; i < MATRIX_NROF_COLUMNS; i++ ){

						target_page->Step[row][i]->attr_MCC = STEP_DEF_MIDICC;
						target_page->Track[row]->MCC_factor = TRACK_DEF_MIDICC;
					}
				}
				break;

#if GROEF || 1
			case (1<< GROOVE):

				if (keyNdx == KEY_RANDOMIZE){

					for ( i=0; i < MATRIX_NROF_COLUMNS; i++ ){

						target_page->Step[row][i]->phrase_num
							= randomize_byamount(	target_page->Step[row][i]->phrase_num,
													target_page->Track[row]->attr_AMT
													+ target_page->Track[row]->event_offset[ATTR_AMOUNT]
													,
													STEP_MIN_GROOVE,
													STEP_MAX_GROOVE );
					}
				}

				if (keyNdx == KEY_CLEAR){

					// Reset the velocity map of the track - i.e. all track velocities
					for ( i=0; i < MATRIX_NROF_COLUMNS; i++ ){
						target_page->Step[row][i]->phrase_num = STEP_DEF_GROOVE;
						target_page->Track[row]->event_offset[ATTR_GROOVE] = 0;
						target_page->Track[row]->GRV_factor = TRACK_DEF_GRVFACTOR;
					}
				}
				break;
#endif

			case (1<< MIDICH):
				break;
		}

		if (keyNdx == KEY_ZOOM){

			// Zoom into the attribute MAP
			G_zoom_level = zoomMAP;
		}
	}



	//
	// ZOOM SELECTOR
	//
	if (keyNdx == KEY_ZOOM_GRID) {

		target_page->trackSelection = 0;

		G_zoom_level = zoomGRID;
		// Keymode is implicit for GRID
	}

	if (keyNdx == KEY_ZOOM_PAGE) {
		G_zoom_level = zoomPAGE;

		target_page->trackSelection = 0;
	}

	if (keyNdx == KEY_ZOOM_MAP) {
		// React only if there is one attribute selected
		if (target_page->trackAttributeSelection != 0){
			G_zoom_level = zoomMAP;
		}
	}

	// PLAY MODE Switcher
	if ( keyNdx == KEY_ZOOM_PLAY ){

		// Toggles the play mode
		toggle_PLAY_MODE( PLAY_MODE_STATUS^1 );
	}


	//
	// TRACK CHAIN SELECTORS - active only when sequencer not running.
	//
	key_exe_chainselectors( keyNdx );

	#ifdef FEATURE_ENABLE_SONG_UPE
	if ( Track_get_MISC(target_page->Track[ my_bit2ndx( target_page->trackSelection )], CONTROL_BIT) ) {
		// NOTE: should have used a bitmask -- don't feel like refactoring it now

		//
		// CONTROL TRACK MUT REFERENCE TRACK
		//
		if ( keyNdx == KEY_MUTE_MASTER ){
			if ( CHECK_BIT(target_page->Track[ my_bit2ndx( target_page->trackSelection )]->attr_MISC, TRK_CTRL_MUT_PGTGL) ) {
				MIX_TRACK = NULL;
				CLEAR_BIT(target_page->Track[ my_bit2ndx( target_page->trackSelection )]->attr_CCAMT, TRK_CTRL_TEMPO);
				CLEAR_BIT(target_page->Track[ my_bit2ndx( target_page->trackSelection )]->attr_MISC, TRK_CTRL_MOVE);
				CLEAR_BIT(target_page->Track[ my_bit2ndx( target_page->trackSelection )]->attr_MISC, TRK_CTRL_MIX);
				CLEAR_BIT(target_page->Track[ my_bit2ndx( target_page->trackSelection )]->attr_MISC, TRK_CTRL_CLEAR);
				CLEAR_BIT(target_page->Track[ my_bit2ndx( target_page->trackSelection )]->attr_MISC, TRK_CTRL_MUT_PGTGL);
				SET_BIT(target_page->Track[ my_bit2ndx( target_page->trackSelection )]->attr_MISC, TRK_CTRL_PGTGL);
			} else {
				MIX_TRACK = NULL;
				CLEAR_BIT(target_page->Track[ my_bit2ndx( target_page->trackSelection )]->attr_CCAMT, TRK_CTRL_TEMPO);
				CLEAR_BIT(target_page->Track[ my_bit2ndx( target_page->trackSelection )]->attr_MISC, TRK_CTRL_MOVE);
				CLEAR_BIT(target_page->Track[ my_bit2ndx( target_page->trackSelection )]->attr_MISC, TRK_CTRL_MIX);
				CLEAR_BIT(target_page->Track[ my_bit2ndx( target_page->trackSelection )]->attr_MISC, TRK_CTRL_CLEAR);
				CLEAR_BIT(target_page->Track[ my_bit2ndx( target_page->trackSelection )]->attr_MISC, TRK_CTRL_PGTGL);
				SET_BIT(target_page->Track[ my_bit2ndx( target_page->trackSelection )]->attr_MISC, TRK_CTRL_MUT_PGTGL);
			}
		}

		//
		// CONTROL TRACK ARM MIX FOR MOVE
		//
		if ( keyNdx == KEY_MIX_MASTER ){
			if ( CHECK_BIT(target_page->Track[ my_bit2ndx( target_page->trackSelection )]->attr_MISC, TRK_CTRL_MIX) ) {
				CLEAR_BIT(target_page->Track[ my_bit2ndx( target_page->trackSelection )]->attr_CCAMT, TRK_CTRL_TEMPO);
				CLEAR_BIT(target_page->Track[ my_bit2ndx( target_page->trackSelection )]->attr_MISC, TRK_CTRL_MOVE);
				CLEAR_BIT(target_page->Track[ my_bit2ndx( target_page->trackSelection )]->attr_MISC, TRK_CTRL_MIX);
				CLEAR_BIT(target_page->Track[ my_bit2ndx( target_page->trackSelection )]->attr_MISC, TRK_CTRL_CLEAR);
				CLEAR_BIT(target_page->Track[ my_bit2ndx( target_page->trackSelection )]->attr_MISC, TRK_CTRL_MUT_PGTGL);
				MIX_TRACK = NULL;
				SET_BIT(target_page->Track[ my_bit2ndx( target_page->trackSelection )]->attr_MISC, TRK_CTRL_PGTGL);
			} else {
				CLEAR_BIT(target_page->Track[ my_bit2ndx( target_page->trackSelection )]->attr_CCAMT, TRK_CTRL_TEMPO);
				CLEAR_BIT(target_page->Track[ my_bit2ndx( target_page->trackSelection )]->attr_MISC, TRK_CTRL_MOVE);
				CLEAR_BIT(target_page->Track[ my_bit2ndx( target_page->trackSelection )]->attr_MISC, TRK_CTRL_CLEAR);
				CLEAR_BIT(target_page->Track[ my_bit2ndx( target_page->trackSelection )]->attr_MISC, TRK_CTRL_PGTGL);
				CLEAR_BIT(target_page->Track[ my_bit2ndx( target_page->trackSelection )]->attr_MISC, TRK_CTRL_MUT_PGTGL);
				MIX_TRACK = target_page->Track[ my_bit2ndx( target_page->trackSelection )];
				SET_BIT(target_page->Track[ my_bit2ndx( target_page->trackSelection )]->attr_MISC, TRK_CTRL_MIX);
			}
		}

		//
		// CONTROL TRACK CLEAR REFERENCE TRACK
		//
		if ( keyNdx == KEY_SELECT_MASTER ){
			if ( CHECK_BIT(target_page->Track[ my_bit2ndx( target_page->trackSelection )]->attr_MISC, TRK_CTRL_CLEAR) ) {
				CLEAR_BIT(target_page->Track[ my_bit2ndx( target_page->trackSelection )]->attr_CCAMT, TRK_CTRL_TEMPO);
				CLEAR_BIT(target_page->Track[ my_bit2ndx( target_page->trackSelection )]->attr_MISC, TRK_CTRL_MIX);
				CLEAR_BIT(target_page->Track[ my_bit2ndx( target_page->trackSelection )]->attr_MISC, TRK_CTRL_CLEAR);
				CLEAR_BIT(target_page->Track[ my_bit2ndx( target_page->trackSelection )]->attr_MISC, TRK_CTRL_MUT_PGTGL);
				SET_BIT(target_page->Track[ my_bit2ndx( target_page->trackSelection )]->attr_MISC, TRK_CTRL_PGTGL);
			} else {
				CLEAR_BIT(target_page->Track[ my_bit2ndx( target_page->trackSelection )]->attr_CCAMT, TRK_CTRL_TEMPO);
				CLEAR_BIT(target_page->Track[ my_bit2ndx( target_page->trackSelection )]->attr_MISC, TRK_CTRL_MIX);
				CLEAR_BIT(target_page->Track[ my_bit2ndx( target_page->trackSelection )]->attr_MISC, TRK_CTRL_PGTGL);
				CLEAR_BIT(target_page->Track[ my_bit2ndx( target_page->trackSelection )]->attr_MISC, TRK_CTRL_MUT_PGTGL);
				SET_BIT(target_page->Track[ my_bit2ndx( target_page->trackSelection )]->attr_MISC, TRK_CTRL_CLEAR);
			}
		}

		//
		// CONTROL TRACK REFERENCE TRACK INHERIT TEMPO
		//
		if ( keyNdx == KEY_PAUSE ){
			if ( CHECK_BIT(target_page->Track[ my_bit2ndx( target_page->trackSelection )]->attr_CCAMT, TRK_CTRL_TEMPO) ) {
				CLEAR_BIT(target_page->Track[ my_bit2ndx( target_page->trackSelection )]->attr_CCAMT, TRK_CTRL_TEMPO);
				CLEAR_BIT(target_page->Track[ my_bit2ndx( target_page->trackSelection )]->attr_MISC, TRK_CTRL_MIX);
				CLEAR_BIT(target_page->Track[ my_bit2ndx( target_page->trackSelection )]->attr_MISC, TRK_CTRL_CLEAR);
				CLEAR_BIT(target_page->Track[ my_bit2ndx( target_page->trackSelection )]->attr_MISC, TRK_CTRL_MUT_PGTGL);
				SET_BIT(target_page->Track[ my_bit2ndx( target_page->trackSelection )]->attr_MISC, TRK_CTRL_PGTGL);
			} else {
				CLEAR_BIT(target_page->Track[ my_bit2ndx( target_page->trackSelection )]->attr_MISC, TRK_CTRL_MIX);
				CLEAR_BIT(target_page->Track[ my_bit2ndx( target_page->trackSelection )]->attr_MISC, TRK_CTRL_CLEAR);
				CLEAR_BIT(target_page->Track[ my_bit2ndx( target_page->trackSelection )]->attr_MISC, TRK_CTRL_PGTGL);
				CLEAR_BIT(target_page->Track[ my_bit2ndx( target_page->trackSelection )]->attr_MISC, TRK_CTRL_MUT_PGTGL);
				SET_BIT(target_page->Track[ my_bit2ndx( target_page->trackSelection )]->attr_CCAMT, TRK_CTRL_TEMPO);
			}
		}

		//
		// CONTROL TRACK External [START] [STOP]
		//
		if ( keyNdx == KEY_PLAY1 ){
			SET_BIT(target_page->Track[ my_bit2ndx( target_page->trackSelection )]->attr_MISC, TRK_CTRL_EXT_PLAY);
			CLEAR_BIT(target_page->Track[ my_bit2ndx( target_page->trackSelection )]->attr_MISC, TRK_CTRL_EXT_STOP);
		}
		if ( keyNdx == KEY_STOP ){
			SET_BIT(target_page->Track[ my_bit2ndx( target_page->trackSelection )]->attr_MISC, TRK_CTRL_EXT_STOP);
			CLEAR_BIT(target_page->Track[ my_bit2ndx( target_page->trackSelection )]->attr_MISC, TRK_CTRL_EXT_PLAY);
		}
		if ( keyNdx == KEY_EDIT_MASTER ){
			CLEAR_BIT(target_page->Track[ my_bit2ndx( target_page->trackSelection )]->attr_MISC, TRK_CTRL_EXT_PLAY);
			CLEAR_BIT(target_page->Track[ my_bit2ndx( target_page->trackSelection )]->attr_MISC, TRK_CTRL_EXT_STOP);
		}
	}
	#endif

	//
	// BIG KNOB KEYS
	//
	if ( keyNdx == KEY_PROGRAM ){

		// If program change is already selected
		if ( CHANGE_BANK_OR_PGM  == PGM ) {

			// Send the program change Value on the selected track(s)
			for (i=0; i < MATRIX_NROF_ROWS; i++) {

				if (	( (target_page->trackSelection & (1<<i)) != 0 )
					&&	( ((unsigned char) target_page->Track[ my_bit2ndx( target_page->trackSelection )]->program_change) > 0 )
					#ifdef FEATURE_ENABLE_SONG_UPE
					&& ( Track_get_MISC(target_page->Track[i], CONTROL_BIT) == OFF || target_page->Track[i]->attr_MCH >= 16 )
					#endif
				){
					MIDI_send( 	MIDI_PGMCH,
								target_page->Track[i]->attr_MCH
								+ target_page->Track[i]->event_offset[ATTR_MIDICH],
								target_page->Track[ my_bit2ndx( target_page->trackSelection )]->program_change - 1,
								0 );

					// Volume level as dedicated message
					if (target_page->MIXAMT_VOL[i] != 64) {
						MIDI_send( 	MIDI_CC,
								target_page->Track[i]->attr_MCH
								+ target_page->Track[i]->event_offset[ATTR_MIDICH],
								7,
								target_page->MIXAMT_VOL[i] 	);
					}
				}
			}
		} // Program is selected

		else{

			// Select the program mode
			CHANGE_BANK_OR_PGM = PGM;
		}
	}

	if ( keyNdx == KEY_SCALE_MYSEL ){

		// If program change is already selected
		if ( CHANGE_BANK_OR_PGM  == BANK ){

			// Send the bank change Value on the selected track(s)
			for (i=0; i < MATRIX_NROF_ROWS; i++) {

				if (	( (target_page->trackSelection & (1<<i)) != 0 )
					&&	( ((unsigned char) target_page->Track[ my_bit2ndx( target_page->trackSelection )]->bank_change) > 0 )
					#ifdef FEATURE_ENABLE_SONG_UPE
					&& ( Track_get_MISC(target_page->Track[i], CONTROL_BIT) == OFF ) // ! internal pgmch
					#endif
				){

					// Bank change on controller 0
					MIDI_send( 	MIDI_CC,
								target_page->Track[ my_bit2ndx( target_page->trackSelection )]->attr_MCH,
								0,
								target_page->Track[ my_bit2ndx( target_page->trackSelection )]->bank_change - 1 );

					// Program change as dedicated message
					MIDI_send( 	MIDI_PGMCH,
								target_page->Track[i]->attr_MCH
								+ target_page->Track[i]->event_offset[ATTR_MIDICH],
								target_page->Track[ my_bit2ndx( target_page->trackSelection )]->program_change - 1,
								0 );
				}
			}
		} // Program is selected

		else{

			// Select the bank mode
			CHANGE_BANK_OR_PGM = BANK;
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
	// CHORD SELECTOR
	//
	// Selects the chord cardinality (page or track??)
	if( (keyNdx > 251)
		){

		// Pretty ghetto: this is the target value of the chord bit
		j = 7 - (keyNdx - 252);

		// Modify all selected tracks
		for( i=0; i < MATRIX_NROF_ROWS; i++ ){

			if ((target_page->trackSelection & (1<<i)) != 0){

				// Enabled the track chord for now
				Track_set_MISC( target_page->Track[i], CHORD_BIT, j );

			} // track is selected
		} // row iterator

	} // Chord selector key pressed




	//
	// OCTAVE CIRCLE
	//
	// Enable track trans-positions with one click and directly
	#include "key_OCT_CIRCLE_xpose_TRACK.h"



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
	// Enter PROGRAM, range_VEL or range_PIT using the BK keys
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

		switch( target_page->trackAttributeSelection ){

//			// VEL
//			case 1:
//				// Use numeric quadrant to enter VEL interval
//				key_NUMERIC_QUADRANT_STEP_VEL_interval( keyNdx );
//				break;
//
//			// PIT
//			case 2:
//				// Use numeric quadrant to enter PIT interval
//				key_NUMERIC_QUADRANT_STEP_PIT_interval( keyNdx );
//				break;

			// none selected
			case 0:
				// Use numeric quadrant to enter PROGRAM for change
				// key_NUMERIC_QUADRANT_TRACK_PROGRAM( keyNdx );
				break;
		}
//			// Enter velocity via outter circle
//			// Account for track offsets when entering directly..
//			key_NUMERIC_QUADRANT_velo_STEP( keyNdx );
//		}
	}



	//
	// TRANSPORT functions - when nothing is selected in page
	//

// in TRACK mode --> Transport functions only!
//	if ( G_clock_source != EXT ){
//
//		switch( keyNdx ){
//
//			case KEY_STOP:
//				sequencer_command_STOP();
//				break;
//
//
//			case KEY_PAUSE:
//				sequencer_command_PAUSE();
//				break;
//
//
//			case KEY_PLAY1:
//			case KEY_PLAY2:
//			case KEY_PLAY4:
//
//				// Make sure the sequencer is running
//				sequencer_command_PLAY();
//				break;
//		}
//	}

	// Allow track multipliers to always work, including in slave mode
	if (	( keyNdx == KEY_PLAY1 )
		||	( keyNdx == KEY_PLAY2 )
		||	( keyNdx == KEY_PLAY4 )
		){

		#include "key_TRACK_multiplier.h"
	}


	//
	// CLOCK SOURCE
	//
	if ( keyNdx == KEY_CLOCK ) {

		key_clock_select( target_page, KEY_CLOCK );
	} // KEY_CLOCK was pressed




