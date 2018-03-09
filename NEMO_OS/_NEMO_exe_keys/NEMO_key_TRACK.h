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




	// Switch the chains play head status of the page
	if ( keyNdx == KEY_MIX_MASTER ){

		// Toggles head track listening in page mode
		target_page->CHAINS_PLAY_HEAD ^= 1;
	}


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
			// ..fallthrough needed

			// No map selector kep pressed, regular interactive mode
			case INTERACTIVE:
				// Change the track selection in page
				target_page->trackSelection = ( 1 << (keyNdx - 1) );
				break;
		}
	} // key is a track selector





	// ///////////
	//
	// MATRIX KEYS
	//
	// If the key pressed is in the Matrix
	if ( (keyNdx > 10) && (keyNdx <= 185) && (((keyNdx-10) % 11) != 0) ){


		// /////
		// ROW I - value entry, depending on selected attribute
		//
		if ( (( keyNdx - NEMO_ROW_I ) % 11) == 0) {

			// Row of target track and column of target step
			row = my_bit2ndx( target_page->trackSelection );
			col = (( keyNdx - NEMO_ROW_I ) / 11) - 1 ;


			// Behave according to selected attribute
			switch( NEMO_selectedTrackAttribute ){


				case NEMO_ATTR_PITCH:

					// Kill the event offset
					target_page->Track[row]->event_offset[NEMO_ATTR_PITCH] = 0;

					if ( NEMO_track_VFF == VFF_VALUE ){

						// This is a regular scale key
						if ( (col >= 2) && (col <= 13) ){
							if ( ( (target_page->Track[row]->attr_PIT / 12) * 12
									+ (col - 2) ) < TRACK_MAX_PITCH
								){
								// Adjust the pitch accordingly	by keypress
								target_page->Track[row]->attr_PIT =
										(target_page->Track[row]->attr_PIT / 12) * 12
									+ 	(col - 2);
							}
						}
						// This is the octave- (8vb) button
						else if (col == 14){
							if ( target_page->Track[row]->attr_PIT >= (TRACK_MIN_PITCH + 12) ){
								target_page->Track[row]->attr_PIT -= 12;
							}
						}
						// This is the octave+ (8va) button
						else if (col == 15){
							if ( target_page->Track[row]->attr_PIT <= (TRACK_MAX_PITCH - 12) ){
								target_page->Track[row]->attr_PIT += 12;
							}
						}
					}
					else{
						// Set the new value of the PIT_factor
						target_page->Track[row]->PIT_factor = col +1;
					}
					break;


				case NEMO_ATTR_VELOCITY:

					// Kill the event offset
					target_page->Track[row]->event_offset[NEMO_ATTR_VELOCITY] = 0;

					if ( NEMO_track_VFF == VFF_VALUE ){

						// D O U B L E - C L I C K
						if ((DOUBLE_CLICK_TARGET == keyNdx)
								&& (DOUBLE_CLICK_TIMER > DOUBLE_CLICK_ALARM_SENSITIVITY)) {

							// Double click code:
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
								target_page->Track[row]->attr_VEL = normalize( i + j, 0, 127 );
							}
							else{
								// Write the final value into the attribute
								target_page->Track[row]->attr_VEL = TRACK_DEF_VELOCITY;
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
								if ( target_page->Track[row]->attr_VEL >= 127 ){
									i = 0;
								}
								else {
									i = target_page->Track[row]->attr_VEL / 10;
								}
								target_page->Track[row]->attr_VEL =
									normalize( i*10 + col + 1 - target_page->Track[row]->event_offset[NEMO_ATTR_VELOCITY],
													0, TRACK_MAX_VELOCITY );
							}
						}
					}
					else{

						// Set the new value of the VEL_factor
						target_page->Track[row]->VEL_factor = col +1;
					}
					break;


				case NEMO_ATTR_LENGTH:

					// Kill event offset
					target_page->Track[row]->event_offset[NEMO_ATTR_LENGTH] = 0;

					// Set the new value of the STA_factor
					target_page->Track[row]->LEN_factor = col +1;
					break;


				case NEMO_ATTR_START:

					// Kill event offset
					target_page->Track[row]->event_offset[NEMO_ATTR_START] = 0;

					// Set the new value of the STA_factor
					target_page->Track[row]->STA_factor = col +1;
					break;


				case NEMO_ATTR_AMOUNT:

					// Kill event offset
					target_page->Track[row]->event_offset[NEMO_ATTR_AMOUNT] = 0;

					if ( NEMO_track_VFF == VFF_VALUE ){

						// D O U B L E - C L I C K
						if ((DOUBLE_CLICK_TARGET == keyNdx)
								&& (DOUBLE_CLICK_TIMER > DOUBLE_CLICK_ALARM_SENSITIVITY)) {

							// Double click code:
							// Break up original value in tens and ones.
							i = target_page->Track[row]->attr_AMT / 10;
							j = target_page->Track[row]->attr_AMT % 10;

							if ( col < 12 ){

								// Compute new tens
								i = 10 * (col + 1);
								// Get new ones value, leave it as it is otherwise
								if ( col < 9 ){
									// j = col + 1;
									j = 0;
								}
								// Write the final value into the attribute
								target_page->Track[row]->attr_AMT = normalize( i + j, 0, 127 );
							}
							else{
								// Write the final value into the attribute
								target_page->Track[row]->attr_AMT = TRACK_DEF_AMOUNT;
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
							//
							if ( col < 9 ){
								// Modify the ones value of the MIDICC. 128 is a special value.
								if ( target_page->Track[row]->attr_AMT >= 127 ){
									i = 0;
								}
								else {
									i = target_page->Track[row]->attr_AMT / 10;
								}
								target_page->Track[row]->attr_AMT = normalize( i*10 + col + 1, 0, 127 );
							}
						}
					} // VFF_VALUE

					else{

						// D O U B L E - C L I C K
						if ((DOUBLE_CLICK_TARGET == keyNdx)
								&& (DOUBLE_CLICK_TIMER > DOUBLE_CLICK_ALARM_SENSITIVITY)) {

							// Double click code:
							// Adjust the factor
							target_page->Track[row]->AMT_factor = 0;
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
							target_page->Track[row]->AMT_factor = col + 1;
						}
					}

					break;


				case NEMO_ATTR_GROOVE:

					// Kill event offset
					target_page->Track[row]->event_offset[NEMO_ATTR_GROOVE] = 0;

					if ( NEMO_track_VFF == VFF_VALUE ){

						// D O U B L E - C L I C K
						if ((DOUBLE_CLICK_TARGET == keyNdx)
								&& (DOUBLE_CLICK_TIMER > DOUBLE_CLICK_ALARM_SENSITIVITY)) {

							// Double click code:
							// Set GROOVE attribute value to 0
							target_page->Track[row]->attr_GRV = TRACK_DEF_GROOVE;
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
							target_page->Track[row]->attr_GRV = col + 1;
						}
					}
					else{

						// D O U B L E - C L I C K
						if ((DOUBLE_CLICK_TARGET == keyNdx)
								&& (DOUBLE_CLICK_TIMER > DOUBLE_CLICK_ALARM_SENSITIVITY)) {

							// Double click code:
							// Adjust the factor
							target_page->Track[row]->GRV_factor = 0;
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
							target_page->Track[row]->GRV_factor = col + 1;
						}
					}

					break;


				case NEMO_ATTR_MIDICC:

					// Kill event offset
					target_page->Track[row]->event_offset[NEMO_ATTR_MIDICC] = 0;

					if ( NEMO_track_VFF == VFF_VALUE ){

						// D O U B L E - C L I C K
						if ((DOUBLE_CLICK_TARGET == keyNdx)
								&& (DOUBLE_CLICK_TIMER > DOUBLE_CLICK_ALARM_SENSITIVITY)) {

							// Double click code:
							// Break up original value in tens and ones.
							i = target_page->Track[row]->attr_MCC / 10;
							j = target_page->Track[row]->attr_MCC % 10;

							if ( col < 12 ){

								// Compute new tens
								i = 10 * (col + 1);
								// Get new ones value, leave it as it is otherwise
								if ( col < 9 ){
									j = 0;
								}
								// Write the final value into the attribute
								target_page->Track[row]->attr_MCC = normalize( i + j, 0, 127 );
							}
							else{
								// Write the final value into the attribute
								target_page->Track[row]->attr_MCC = TRACK_DEF_MIDICC;
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
					}
					else{

						// D O U B L E - C L I C K
						if ((DOUBLE_CLICK_TARGET == keyNdx)
								&& (DOUBLE_CLICK_TIMER > DOUBLE_CLICK_ALARM_SENSITIVITY)) {

							// Double click code:
							// Adjust the factor
							target_page->Track[row]->MCC_factor = 0;
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
							target_page->Track[row]->MCC_factor = col + 1;
						}
					}

					break;


				case NEMO_ATTR_MIDICH:

					// Kill event offset
					target_page->Track[row]->event_offset[NEMO_ATTR_MIDICH] = 0;

					// D O U B L E - C L I C K
					if ((DOUBLE_CLICK_TARGET == keyNdx)
							&& (DOUBLE_CLICK_TIMER > DOUBLE_CLICK_ALARM_SENSITIVITY)) {

						// Double click code:
						///
						// Switch the MIDI port
						// Find out the current port: it will be 0 or 1;
						// Note that the ATTR_MIDICH is in the range [1..32] via keypress.
						i = target_page->Track[row]->attr_MCH / 17;
						j = target_page->Track[row]->attr_MCH - (16 * i);

						// Switch the port indicator
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
					break;


				case NEMO_ATTR_POSITION:

					// Kill event offset
					target_page->Track[row]->event_offset[NEMO_ATTR_POSITION] = 0;

					if ( col < target_page->Track[row]->attr_POS ){
						// Press left of current position
						j = DEC;	i = target_page->Track[row]->attr_POS - col;
					}
					else{
						// Press right of current position
						j = INC; 	i = col - target_page->Track[row]->attr_POS;
					}
					// Execute offset right away
					Page_wrap_track( target_page, row, j, i);
					target_page->Track[row]->attr_POS = col;
					break;


				case NEMO_ATTR_DIRECTION:

					// Kill event offset
					target_page->Track[row]->event_offset[NEMO_ATTR_DIRECTION] = 0;

					// Set the new value of the GROOVE attribute - on the same port.
					target_page->Track[row]->attr_DIR
						= normalize( col + 1, 0, TRACK_MAX_DIRECTION );
					break;


				case NEMO_ATTR_CLOCK:

					// Double click selects the clock value as divisor
					if (	( DOUBLE_CLICK_TARGET == keyNdx )
						&& 	( DOUBLE_CLICK_TIMER > DOUBLE_CLICK_ALARM_SENSITIVITY )
						){

						// SINGLE CLICK: Set the tempo multiplier of the current track
						// Single click selects the clock value as multiplier
						switch( col ){

							// Multiplier selection - regular. Includes triplet multiplier on col 14 (key 15)
							case 0: case 1: case 2: case 3: case 4: case 5: case 6: case 7: case 14: case 15:
								j = col + 1;

								// If the track is not playing, it is safe to adjust the multiplier immediately
								if ( target_page->Track[row]->attr_LOCATOR == 0 ){
									// Update the multipler and clear the buffer
									target_page->Track[row]->attr_TEMPOMUL 		= j;
									// Update the divisor
									target_page->Track[row]->attr_TEMPOMUL_SKIP	= 0;
								} // current Track is not playing
								// Track is playing so queue until the next global TTC1
								else{
									// Set the tempo multiplier of selected tracks
									track_tempo_mul = j;
									// Make sure to get rid of any skip values
									track_tempo_div = 0;
									// Remember the track for the clock change
									track_tempo_ptr = target_page->Track[row];
									// Remember the page for the clock change
									track_tempo_page = target_page;
								}
								break;
						}

/*						// DOUBLE CLICK CODE
						// Determine the wanted multiplicator / skip value..
						// ..and store it into the track attribute
						switch( col ){

							case 0:		break;
							// Divisor 2, 3, 4, 5, 6, 7, 8, 16
							case 1: case 2: case 3: case 4: case 5: case 6: case 7: case 15:
								j = col;

								// If the track is not playing, it is safe to adjust the multiplier immediately
								if ( target_page->Track[row]->attr_LOCATOR == 0 ){
									// Update the multipler and clear the buffer
									target_page->Track[row]->attr_TEMPOMUL 	= 1;
									// Update the divisor and clear the buffer
									target_page->Track[row]->attr_TEMPOMUL_SKIP	= j;
								} // current Track is not playing
								else{
									// Set the tempo multiplier of selected tracks
									track_tempo_div = j;
									// Set the tempo multiplier of selected tracks
									track_tempo_mul = 1;
									// Remember the track for the clock change
									track_tempo_ptr = target_page->Track[row];
									// Remember the page for the clock change
									track_tempo_page = target_page;
								}
								break;

							// Inverse triplet multiplier
							case 14:
								j = 25;
								// Set the tempo multiplier of selected tracks
								track_tempo_div = 0;
								// Set the tempo multiplier of selected tracks
								track_tempo_mul = j;
								// Remember the track for the clock change
								track_tempo_ptr = target_page->Track[row];
								// Remember the track for the clock change
								track_tempo_page = target_page;

								// Needs handling like the multiplier, not like the divisor.. oh well.
								// If the track is not playing, it is safe to adjust the multiplier immediately
								if ( target_page->Track[row]->attr_LOCATOR == 0 ){
									// Update the multipler and clear the buffer
									target_page->Track[row]->attr_TEMPOMUL 		= j;
									// Update the divisor
									target_page->Track[row]->attr_TEMPOMUL_SKIP	= 0;
								} // current Track is not playing
								// Track is playing so queue until the next global TTC1
								else{
									// Set the tempo multiplier of selected tracks
									track_tempo_mul = j;
									// Make sure to get rid of any skip values
									track_tempo_div = 0;
									// Remember the track for the clock change
									track_tempo_ptr = target_page->Track[row];
									// Remember the page for the clock change
									track_tempo_page = target_page;
								}
								return; // we are actually done..
								break; // not needed, but looking nice ;-)

						}
*/					}

					// Start thinking about double clicking
					else if ( DOUBLE_CLICK_TARGET == 0 ) {

						// Prepare double click timer for zooming into the step
						DOUBLE_CLICK_TARGET = keyNdx;
						DOUBLE_CLICK_TIMER = ON;

						// Start the Double click Alarm
						cyg_alarm_initialize(
							doubleClickAlarm_hdl,
							cyg_current_time() + DOUBLE_CLICK_ALARM_TIME,
							DOUBLE_CLICK_ALARM_TIME );

/*						// SINGLE CLICK: Set the tempo multiplier of the current track
						// Single click selects the clock value as multiplier
						switch( col ){

							// Multiplier selection - regular. Includes triplet multiplier on col 14 (key 15)
							case 0: case 1: case 2: case 3: case 4: case 5: case 6: case 7: case 14: case 15:
								j = col + 1;

								// If the track is not playing, it is safe to adjust the multiplier immediately
								if ( target_page->Track[row]->attr_LOCATOR == 0 ){
									// Update the multipler and clear the buffer
									target_page->Track[row]->attr_TEMPOMUL 		= j;
									// Update the divisor
									target_page->Track[row]->attr_TEMPOMUL_SKIP	= 0;
								} // current Track is not playing
								// Track is playing so queue until the next global TTC1
								else{
									// Set the tempo multiplier of selected tracks
									track_tempo_mul = j;
									// Make sure to get rid of any skip values
									track_tempo_div = 0;
									// Remember the track for the clock change
									track_tempo_ptr = target_page->Track[row];
									// Remember the page for the clock change
									track_tempo_page = target_page;
								}
								break;
						}
*/
						// DOUBLE CLICK CODE
						// Determine the wanted multiplicator / skip value..
						// ..and store it into the track attribute
						switch( col ){

							case 0:		break;
							// Divisor 2, 3, 4, 5, 6, 7, 8, 16
							case 1: case 2: case 3: case 4: case 5: case 6: case 7: case 15:
								j = col;

								// If the track is not playing, it is safe to adjust the multiplier immediately
								if ( target_page->Track[row]->attr_LOCATOR == 0 ){
									// Update the multipler and clear the buffer
									target_page->Track[row]->attr_TEMPOMUL 	= 1;
									// Update the divisor and clear the buffer
									target_page->Track[row]->attr_TEMPOMUL_SKIP	= j;
								} // current Track is not playing
								else{
									// Set the tempo multiplier of selected tracks
									track_tempo_div = j;
									// Set the tempo multiplier of selected tracks
									track_tempo_mul = 1;
									// Remember the track for the clock change
									track_tempo_ptr = target_page->Track[row];
									// Remember the page for the clock change
									track_tempo_page = target_page;
								}
								break;

							// Inverse triplet multiplier
							case 14:
								j = 25;
								// Set the tempo multiplier of selected tracks
								track_tempo_div = 0;
								// Set the tempo multiplier of selected tracks
								track_tempo_mul = j;
								// Remember the track for the clock change
								track_tempo_ptr = target_page->Track[row];
								// Remember the track for the clock change
								track_tempo_page = target_page;

								// Needs handling like the multiplier, not like the divisor.. oh well.
								// If the track is not playing, it is safe to adjust the multiplier immediately
								if ( target_page->Track[row]->attr_LOCATOR == 0 ){
									// Update the multipler and clear the buffer
									target_page->Track[row]->attr_TEMPOMUL 		= j;
									// Update the divisor
									target_page->Track[row]->attr_TEMPOMUL_SKIP	= 0;
								} // current Track is not playing
								// Track is playing so queue until the next global TTC1
								else{
									// Set the tempo multiplier of selected tracks
									track_tempo_mul = j;
									// Make sure to get rid of any skip values
									track_tempo_div = 0;
									// Remember the track for the clock change
									track_tempo_ptr = target_page->Track[row];
									// Remember the page for the clock change
									track_tempo_page = target_page;
								}
								return; // we are actually done..
								break; // not needed, but looking nice ;-)

						}

					} // Single click, CLOCK
					break;


				case NEMO_ATTR_PGMCH:

					if ( NEMO_track_VFF == VFF_VALUE ){

						// D O U B L E - C L I C K
						if ((DOUBLE_CLICK_TARGET == keyNdx)
								&& (DOUBLE_CLICK_TIMER > DOUBLE_CLICK_ALARM_SENSITIVITY)) {

							// Double click code:
							// Break up original value in tens and ones.
							i = target_page->Track[row]->program_change / 10;
							j = target_page->Track[row]->program_change % 10;

							if ( col < 12 ){

								// Compute new tens
								i = 10 * (col + 1);
								// Get new ones value, leave it as it is otherwise
								if ( col < 9 ){
									// j = col + 1;
									j = 0;
								}
								// Write the final value into the attribute
								target_page->Track[row]->program_change = normalize( i + j, 0, 127 );
							}
							else{
								// Write the final value into the attribute
								target_page->Track[row]->program_change = TRACK_DEF_PROGRAMCHANGE;
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
							//
							if ( col < 9 ){
								// Modify the ones value of the MIDICC. 128 is a special value.
								if ( target_page->Track[row]->program_change >= 127 ){
									i = 0;
								}
								else {
									i = target_page->Track[row]->program_change / 10;
								}
								target_page->Track[row]->program_change = normalize( i*10 + col + 1, 0, 127 );
							}
						}

						// Send the program change right away
						MIDI_send( 	MIDI_PGMCH,
									target_page->Track[row]->attr_MCH
									+ target_page->Track[row]->event_offset[NEMO_ATTR_MIDICH],
									target_page->Track[row]->program_change - 1,
									0 );

					} // VFF_VALUE

					else{

						// D O U B L E - C L I C K
						if ((DOUBLE_CLICK_TARGET == keyNdx)
								&& (DOUBLE_CLICK_TIMER > DOUBLE_CLICK_ALARM_SENSITIVITY)) {

							// Double click code:
							// Adjust the factor
							target_page->Track[row]->program_change = 0;
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
							target_page->Track[row]->program_change = col + 1;
						}
					}

					break;

			} // switch( NEMO_selectedTrackAttribute )
		} // ROW I Key



		// //////
		// ROW II selects the track attribute as well as some others..
		//
		if ( (( keyNdx - NEMO_ROW_II ) % 11) == 0) {

			// Target coordinates
			row = my_bit2ndx( target_page->trackSelection );
			col = (( keyNdx - NEMO_ROW_II ) / 11) - 1 ;

			// Select the track attribute
			switch( col ){

				// VEL PIT LEN STA
				case 0: 	NEMO_selectedTrackAttribute = NEMO_ATTR_PITCH;			break;
				case 1: 	NEMO_selectedTrackAttribute = NEMO_ATTR_VELOCITY;		break;
				case 2: 	NEMO_selectedTrackAttribute = NEMO_ATTR_LENGTH;			break;
				case 3:		NEMO_selectedTrackAttribute = NEMO_ATTR_START;			break;
				// AMT GRV MCC
				case 5: 	NEMO_selectedTrackAttribute = NEMO_ATTR_AMOUNT;			break;
				case 6: 	NEMO_selectedTrackAttribute = NEMO_ATTR_GROOVE;			break;
				case 7: 	NEMO_selectedTrackAttribute = NEMO_ATTR_MIDICC;			break;
				// MCH
				case 9:
					if ( NEMO_track_VFF == VFF_VALUE ){
						NEMO_selectedTrackAttribute = NEMO_ATTR_MIDICH;
					}
					break;
				// POS
				case 11:
					if ( NEMO_track_VFF == VFF_VALUE ){
						NEMO_selectedTrackAttribute = NEMO_ATTR_POSITION;
					}
					break;
				// DIR
				case 12:
					if ( NEMO_track_VFF == VFF_VALUE ){
						NEMO_selectedTrackAttribute = NEMO_ATTR_DIRECTION;
					}
					break;
				// PGMCH
				case 13:
					if ( NEMO_track_VFF == VFF_VALUE ){
						NEMO_selectedTrackAttribute = NEMO_ATTR_PGMCH;
					}
					break;
				// CLK
				case 15:
					if ( NEMO_track_VFF == VFF_VALUE ){
						NEMO_selectedTrackAttribute = NEMO_ATTR_CLOCK;
					}
					break;
			}
		} // ROW II Key



		// ////////////
		// NEMO_ROW_III - the trackSpecifiers
		//
		if ( (( keyNdx - NEMO_ROW_III ) % 11) == 0) {

			// Target coordinates
			row = my_bit2ndx( target_page->trackSelection );
			col = (( keyNdx - NEMO_ROW_III ) / 11) - 1 ;

			switch( col ){

				case 2:
					// FEEDER button
					// Cycle through the EFF states: OFF (0), FEEDER (1), LISTENER (2), LISTENINGFEEDER (3)
					temp = Track_get_MISC( target_page->Track[row], EFF_BIT );
					switch( temp ){

						case 0:	Track_set_MISC( target_page->Track[row], EFF_BIT, 1 );	break;
						case 1:	Track_set_MISC( target_page->Track[row], EFF_BIT, 0 );	break;
						case 2:	Track_set_MISC( target_page->Track[row], EFF_BIT, 3 );	break;
						case 3:	Track_set_MISC( target_page->Track[row], EFF_BIT, 2 );	break;
					}
					break;

				case 3:
					// LISTENER button
					// Cycle through the EFF states: OFF (0), SEND (1), RECEIVE (2), RECEIVESEND (3)
					temp = Track_get_MISC( target_page->Track[row], EFF_BIT );
					switch( temp ){

						case 0:	Track_set_MISC( target_page->Track[row], EFF_BIT, 2 );	break;
						case 1:	Track_set_MISC( target_page->Track[row], EFF_BIT, 3 );	break;
						case 2:	Track_set_MISC( target_page->Track[row], EFF_BIT, 0 );	break;
						case 3:	Track_set_MISC( target_page->Track[row], EFF_BIT, 1 );	break;
					}
					break;


				// VFF Selection
				case 7:
					// This is always allowed
					NEMO_track_VFF = VFF_VALUE;
					break;

				case 8:
					// Make sure the attribute is allowed to have a factor
					if ( 	(NEMO_selectedTrackAttribute != NEMO_ATTR_MIDICH )
						&&	(NEMO_selectedTrackAttribute != NEMO_ATTR_POSITION )
						&&	(NEMO_selectedTrackAttribute != NEMO_ATTR_DIRECTION )
						&&	(NEMO_selectedTrackAttribute != NEMO_ATTR_CLOCK )
						){
						NEMO_track_VFF = VFF_FACTOR;
					}
					break;
// VFF Flow is now implicit in factor
//				case 9:
//					// Make sure the attribute is allowed to have a flow
//					if ( 	(NEMO_selectedTrackAttribute != NEMO_ATTR_MIDICH )
//						&&	(NEMO_selectedTrackAttribute != NEMO_ATTR_POSITION )
//						&&	(NEMO_selectedTrackAttribute != NEMO_ATTR_DIRECTION )
//						&&	(NEMO_selectedTrackAttribute != NEMO_ATTR_CLOCK )
//						){
//						NEMO_track_VFF = VFF_FLOW;
//					}
					break;

//				case 11:
//					// COPY	- Remember the selected track in terms of page and row
//					TRACK_COPY_BUFFER = ( 	target_page->trackSelection
//										| ( target_page->pageNdx << 10 )
//										);
//					break;
//
//				case 12:
//					// Paste - Get the trackselection bits
//					row = my_bit2ndx( TRACK_COPY_BUFFER & 0x3FF );
//
//					// Get the source page index
//					j = TRACK_COPY_BUFFER >> 10;
//
//					// Execute the copy operation
//					Track_copy( &Page_repository[j], row,
//								target_page, my_bit2ndx( target_page->trackSelection ) );
//					break;
//
//				case 13:
//					// Clear - Clear track fully
//					Track_clear_full( target_page, row );
//					break;
//
//				case 15:
//					// 1/Clock
//
//					break;
			}
		} // ROW III Key


		// //////
		// ROW IV allows flow selection / track editing
		//

		// Allows step toggle edit for the track
		// The functionality should be identical to the one in Page mode.
		//
		if ( (( keyNdx - NEMO_ROW_IV ) % 11) == 0) {

			// Row of target track and column of target step
			row = my_bit2ndx( target_page->trackSelection );
			col = (( keyNdx - NEMO_ROW_IV ) / 11) - 1 ;

			// Needs some more elaboration but works for now
			target_page->page_clear = OFF;

			// In value mode we see the track
			if ( NEMO_track_VFF == VFF_VALUE ){
				// Toggle, un/select or un/mute the step in question
				interpret_matrix_stepkey( row, col, target_page );
			}
			else{
				// In factor mode select flow shape for the current attribute
				if ( col < 10 ){
					// Params: shapeNdx, target_attribute, page, row;
					assign_flow_shape( col, NEMO_selectedTrackAttribute, target_page, row );
					target_page->Track[row]->flow_shape[ NEMO_selectedTrackAttribute ] = col;
				}
					// Show the resolution if MCC
				else if ( NEMO_selectedTrackAttribute == NEMO_ATTR_MIDICC ){

					switch( col ){

						case 12:
							target_page->Track[row]->CC_resolution = 1;
							break;

						case 13:
							target_page->Track[row]->CC_resolution = 3;
							break;

						case 14:
							target_page->Track[row]->CC_resolution = 5;
							break;

						case 15:
							target_page->Track[row]->CC_resolution = 7;
							break;
					} // switch
				} // MIDICC factor selected
			}
		} // ROW IV Key

	} // Key in the Matrix


	// MUT Main button
	if ( keyNdx == KEY_MUTE_MASTER ){
		row = my_bit2ndx( target_page->trackSelection );
		target_page->trackSolopattern ^= ( 1 << row );
	}


	// //////////////
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

//		// TGGL function: set or unset the mute bit on the track
//		if (keyNdx == KEY_TGGL) {
//
//			// Toggle the mute bit pattern
//			target_page->trackMutepattern ^= (1<<row);
//		}

//		// SOLO function: set or unset the solo bit of the track
//		if (keyNdx == KEY_SOLO) {
//
//			// Toggle the SOLO bits on the selected track - double click ensures only one track selected
//			target_page->trackSolopattern ^= (1<<row);
//		}


		// CLEAR function
		if ( keyNdx == KEY_CLEAR ) {

			// Clear track fully
			Track_clear_full( target_page, row );
		}


		// RANDOMIZE function
		if (keyNdx == KEY_RMX) {

			// Randomize the selected track in page. This format should be used everywhere in here..
			Page_RNDMZ_selected_tracks( target_page );

			// Mark page as touched
			target_page->page_clear = OFF;
		}


		// FLIP TRACK function
		if( keyNdx == KEY_FLT ){

			if ( my_bit_cardinality( target_page->trackSelection ) > 1 ){

				// Flatten the selected tracks in page. This format should be used everywhere in here..
				Page_FLT_selected_tracks( target_page );
			}
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

			TRACK_COPY_BUFFER = 0;
		}
	} // Entity mode operations



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
	// TRACK CHAIN SELECTORS
	//
	key_exe_chainselectors( keyNdx );


//	//
//	// BIG KNOB KEYS
//	//
//	if ( keyNdx == KEY_PROGRAM ){
//
//		// If program change is already selected
//		if ( CHANGE_BANK_OR_PGM  == PGM ){
//
//			// Send the program change Value on the selected track(s)
//			for (i=0; i < MATRIX_NROF_ROWS; i++) {
//
//				if (	( (target_page->trackSelection & (1<<i)) != 0 )
//					&&	( ((unsigned char) target_page->Track[ my_bit2ndx( target_page->trackSelection )]->program_change) > 0 )
//					){
//
//					MIDI_send( 	MIDI_PGMCH,
//								target_page->Track[i]->attr_MCH
//								+ target_page->Track[i]->event_offset[ATTR_MIDICH],
//								target_page->Track[ my_bit2ndx( target_page->trackSelection )]->program_change - 1,
//								0 );
//				}
//			}
//		} // Program is selected
//
//		else{
//
//			// Select the program mode
//			CHANGE_BANK_OR_PGM = PGM;
//		}
//	}



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



