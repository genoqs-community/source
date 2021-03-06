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

		// ROW I - value entry, depending on selected attribute
		if ( (( keyNdx - NEMO_ROW_I ) % 11) == 0) {

			// Target coordinates
			row = target_page->stepSelectionSingleRow;
			col = (( keyNdx - NEMO_ROW_I ) / 11) - 1 ;

			// Make sure we only accept note key presses
			if ( NEMO_selectedStepAttribute == NEMO_ATTR_PITCH ){
				NEMO_key_OCT_CIRCLE_chord_STEP( keyNdx );
			}
		} // ROW I Key


		// ROW II selects the track attribute
		if ( (( keyNdx - NEMO_ROW_II ) % 11) == 0) {

			// Target coordinates
			row = target_page->stepSelectionSingleRow;
			col = (( keyNdx - NEMO_ROW_II ) / 11) - 1 ;

			switch( col ){
				// VEL STA
				case 0: 	NEMO_selectedStepAttribute = NEMO_ATTR_PITCH;			break;
				case 3:		NEMO_selectedStepAttribute = NEMO_ATTR_START;			break;
				case 5:		NEMO_selectedStepAttribute = NEMO_ATTR_AMOUNT;			break;
			}
		} // ROW II Key


		// NEMO_ROW_III - the value specifiers: Value, Event, Range
		if ( (( keyNdx - NEMO_ROW_III ) % 11) == 0) {

			// Target coordinates
			row = target_page->stepSelectionSingleRow;
			col = (( keyNdx - NEMO_ROW_III ) / 11) - 1 ;

			switch( col ){
				#ifdef FEATURE_ENABLE_CHORD_OCTAVE
				case 0:
					// D O U B L E - C L I C K
					if ((DOUBLE_CLICK_TARGET == keyNdx)
							&& (DOUBLE_CLICK_TIMER > DOUBLE_CLICK_ALARM_SENSITIVITY)) {

						// Double click code:
						// Switch between standard chord UI and chord octave layered UI.
						NEMO_step_VER = NEMO_step_VER >= VER_CHORD_OCTAVE_FIRST ? VER_CHORD : VER_CHORD_OCTAVE_FIRST;
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

						// Cycle octaves in chord octave layered UI otherwise no op.
						NEMO_step_VER = NEMO_step_VER >= VER_CHORD_OCTAVE_FIRST ? max( VER_CHORD_OCTAVE_FIRST, ( NEMO_step_VER + 1 ) % ( VER_CHORD_OCTAVE_THIRD + 1 ) ) : NEMO_step_VER;
					}
					break;
				#endif
				// VER Selection
				case 7:		NEMO_step_VER = VER_VALUE;								break;
				case 11:	NEMO_step_VER = VER_EVENT;								break;
			}
		} // ROW III Key





