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
//void key_exec_STEP__MATRIX__VALUE( 	Pagestruct* target_page,
//									Stepstruct* target_step,
//									unsigned char keyNdx,
//									unsigned char row,
//									unsigned char col ){

		// /////
		// ROW I - value entry, depending on selected attribute
		//
		if ( (( keyNdx - NEMO_ROW_I ) % 11) == 0 ){

			// Target coordinates
			row = target_page->stepSelectionSingleRow;
			col = (( keyNdx - NEMO_ROW_I ) / 11) - 1 ;

			// Behave according to selected attribute
			switch( NEMO_selectedStepAttribute ){

				case NEMO_ATTR_PITCH:
					step_enter_value_PITVELAMT(
						&target_step->attr_PIT, STEP_MAX_PITCH, STEP_DEF_PITCH, keyNdx, row, col );
					break;

				case NEMO_ATTR_VELOCITY:
					step_enter_value_PITVELAMT(
						&target_step->attr_VEL, STEP_MAX_VELOCITY, STEP_DEF_VELOCITY, keyNdx, row, col );
					break;

				case NEMO_ATTR_AMOUNT:
					step_enter_value_PITVELAMT(
						&target_step->attr_AMT, STEP_MAX_AMOUNT, STEP_DEF_AMOUNT, keyNdx, row, col );
					break;

				case NEMO_ATTR_LENGTH:
					// Kill event offset ..Set the new value of the LEN_factor
					// target_page->Track[row]->event_offset[NEMO_ATTR_LENGTH] = 0;
					step_enter_value_LEN(
						&target_step->attr_LEN, STEP_MAX_LENGTH, STEP_DEF_LENGTH, keyNdx, row, col );
					break;

				case NEMO_ATTR_START:
					// Kill event offset ..Set the new value of the STA_factor
					// target_page->Track[row]->event_offset[NEMO_ATTR_START] = 0;
					switch( col ){
						case 2: case 3: case 4: case 5: case 6: target_step->attr_STA = col - 1;		break;
						case 7: case 8: 						target_step->attr_STA = STEP_DEF_START;	break;
						case 9: case 10: case 11: case 12: case 13:	target_step->attr_STA = col - 2;	break;
					}
					break;

				case NEMO_ATTR_MIDICC:
					step_enter_value_MCC(
						&target_step->attr_MCC, STEP_MAX_MIDICC, STEP_DEF_MIDICC, keyNdx, row, col );
					break;

				case NEMO_ATTR_GROOVE:

					// D O U B L E - C L I C K
					if ((DOUBLE_CLICK_TARGET == keyNdx)
							&& (DOUBLE_CLICK_TIMER > DOUBLE_CLICK_ALARM_SENSITIVITY)) {

						// Double click code:
						// Set GROOVE attribute value to 0
						target_step->phrase_num = STEP_DEF_GROOVE;
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
						target_step->phrase_num = normalize( col + 1, STEP_MIN_GROOVE, STEP_MAX_GROOVE );
					}
					break;

				case NEMO_ATTR_POSITION:
					// Position selection via keyboard
					break;

			} // switch( NEMO_selectedTrackAttribute )
		} // ROW I Key



		// //////
		// ROW II selects the track attribute as well as some others..
		//
		if ( (( keyNdx - NEMO_ROW_II ) % 11) == 0 ){

			// Target coordinates
			row = target_page->stepSelectionSingleRow;
			col = (( keyNdx - NEMO_ROW_II ) / 11) - 1 ;

			// Select the track attribute
			switch( col ){

				// VEL PIT LEN STA
				case 0: 	NEMO_selectedStepAttribute = NEMO_ATTR_PITCH;			break;
				case 1: 	NEMO_selectedStepAttribute = NEMO_ATTR_VELOCITY;		break;
				case 2: 	NEMO_selectedStepAttribute = NEMO_ATTR_LENGTH;			break;
				case 3:		NEMO_selectedStepAttribute = NEMO_ATTR_START;			break;
				// AMT GRV MCC
				case 5: 	NEMO_selectedStepAttribute = NEMO_ATTR_AMOUNT;			break;
				case 6: 	NEMO_selectedStepAttribute = NEMO_ATTR_GROOVE;			break;
				case 7: 	NEMO_selectedStepAttribute = NEMO_ATTR_MIDICC;			break;
				// POS
				case 11:
					if ( target_step->phrase_num != 0 ){
						NEMO_selectedStepAttribute = NEMO_ATTR_POSITION;
					}
					break;
			}
		} // ROW II Key



		// ////////////
		// NEMO_ROW_III - the value specifiers: Value, Event, Range
		//
		if ( (( keyNdx - NEMO_ROW_III ) % 11) == 0) {

			// Target coordinates
			row = target_page->stepSelectionSingleRow;
			col = (( keyNdx - NEMO_ROW_III ) / 11) - 1 ;

			switch( col ){

				// VER Selection
				case 0:
					// Range allowed only in Event context, when event is set
					if (	( NEMO_selectedStepAttribute == NEMO_ATTR_PITCH )
						||	( NEMO_selectedStepAttribute == NEMO_ATTR_START )
						){
						NEMO_step_VER = VER_CHORD;
					}
					break;

				case 11:
					if ( 	NEMO_selectedStepAttribute != NEMO_ATTR_POSITION ){
						NEMO_step_VER = VER_EVENT;
					}
					break;
			}
		} // ROW III Key

// }


