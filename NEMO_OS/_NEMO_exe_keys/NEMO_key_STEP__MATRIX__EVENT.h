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


//void key_exec_STEP__MATRIX__EVENT( 	Pagestruct* target_page, 
//									Stepstruct* target_step,
//									unsigned char keyNdx, 
//									unsigned char row, 
//									unsigned char col ){
//
//		unsigned char temp = 0;

		// /////
		// ROW I - value entry, depending on selected attribute
		// 
		if ( (( keyNdx - NEMO_ROW_I ) % 11) == 0 ){

			// Target coordinates
			row = target_page->stepSelectionSingleRow;
			col = (( keyNdx - NEMO_ROW_I ) / 11) - 1 ;
			#ifdef FEATURE_STEP_EVENT_TRACKS
			temp = ( target_step->event_data & 0x0F ) + 1;
			if (  ( Step_get_status( target_step, STEPSTAT_EVENT ) == ON )
				&&  ( ( temp >= EVENT_FLAG_TRACK_MUTE )
				||	( ( temp ==  ATTR_POSITION )
				&&	( CHECK_BIT( target_step->attr_STATUS, STEP_EVENT_TRACK_ALT_MODE + 5 ) ) )
				||	( ( temp ==  ATTR_DIRECTION )
				&&	( CHECK_BIT( target_step->attr_STATUS, STEP_EVENT_TRACK_ALT_MODE + 5 ) ) )	) )  {

				// When in event mode, adjust the amount
				if ( col < 14 ) {
					// Only support invert / default CLK+ (col 12, 13)
					step_enter_value_PITVELAMT(
						NEMO_ATTR_AMOUNT, &target_step->attr_AMT, TRACK_MAX_RANGE_EVENT, TRACK_DEF_RANGE_EVENT, keyNdx, row, col );
				}
			} else {
				// When in event mode, adjust the amount
				step_enter_value_PITVELAMT(
					NEMO_ATTR_AMOUNT, &target_step->attr_AMT, STEP_MAX_AMOUNT, STEP_DEF_AMOUNT, keyNdx, row, col );
			}
			#else
			// When in event mode, adjust the amount
			step_enter_value_PITVELAMT( 
					&target_step->attr_AMT, STEP_MAX_AMOUNT, STEP_DEF_AMOUNT, keyNdx, row, col );
			#endif
		} // ROW I Key


		// //////
		// ROW II selects the track attribute as well as some others..
		//
		if ( (( keyNdx - NEMO_ROW_II ) % 11) == 0 ){

			// Target coordinates
			row = target_page->stepSelectionSingleRow;
			col = (( keyNdx - NEMO_ROW_II ) / 11) - 1 ;

			// Select the step event attribute
			// Initialize the event_offsets
			//target_page->Track[ target_page->stepSelectionSingleRow ]->event_offset[ATTR_VELOCITY] 	= 0;
			//target_page->Track[ target_page->stepSelectionSingleRow ]->event_offset[ATTR_PITCH] 	= 0;
			//target_page->Track[ target_page->stepSelectionSingleRow ]->event_offset[ATTR_LENGTH] 	= 0;
			//target_page->Track[ target_page->stepSelectionSingleRow ]->event_offset[ATTR_START] 	= 0;
			//target_page->Track[ target_page->stepSelectionSingleRow ]->event_offset[ATTR_DIRECTION] = 0;
			//target_page->Track[ target_page->stepSelectionSingleRow ]->event_offset[ATTR_AMOUNT] 	= 0;
			//target_page->Track[ target_page->stepSelectionSingleRow ]->event_offset[ATTR_GROOVE] 	= 0;
			//target_page->Track[ target_page->stepSelectionSingleRow ]->event_offset[ATTR_MIDICC] 	= 0;
			//target_page->Track[ target_page->stepSelectionSingleRow ]->event_offset[ATTR_MIDICH] 	= 0;

			init_track_event_offsets( target_page->Track[ target_page->stepSelectionSingleRow ] );
			// Now set the event status..
			if (target_page->stepSelection == 1){
				
				// Transform the keypress into to the Octopus model
				temp = ( keyNdx / 11 ) - 1;
				switch( temp ){
					case 0:		temp = 0;	break;	// PIT
					case 1:		temp = 1;	break;	// VEL
					case 2:		temp = 2;	break;	// LEN
					case 3:		temp = 3;	break;	// STA
					case 5:		temp = 6;	break;	// AMT
					case 6:		temp = 7;	break;	// GRV
					case 7:		temp = 8;	break;	// MCC
					case 9:		temp = 9;	break;	// MCH
					case 11:	temp = 4;	break;	// POS
					case 12:	temp = 5;	break;	// DIR
					default:	temp = 20;	break;	// not a valid key press
				}

				// Do not continue if the pressed key was not valid
				if ( temp == 20 ) return;
				#ifdef FEATURE_STEP_EVENT_TRACKS
				unsigned char event_data_attr = temp + 1;
				if ( 	( event_data_attr < EVENT_FLAG_TRACK_MUTE )
						&&	( ( target_step->event_data & 0x0F ) + 1 >= EVENT_FLAG_TRACK_MUTE ) ) {

					target_step->attr_AMT = TRACK_DEF_RANGE_EVENT;
				}
				else if (  ( event_data_attr == NEMO_ATTR_POSITION )
							|| ( event_data_attr == NEMO_ATTR_DIRECTION )  )  {

					// D O U B L E - C L I C K  C O N S T R U C T
					// DOUBLE CLICK SCENARIO
					if ((DOUBLE_CLICK_TARGET == keyNdx)
							&& (DOUBLE_CLICK_TIMER > DOUBLE_CLICK_ALARM_SENSITIVITY)) {

						// Double click code
						// ...
						// POS ALT Mode
						TOGGLE_BIT( target_step->attr_STATUS, STEP_EVENT_TRACK_ALT_MODE + 5 );

						if (	( event_data_attr == NEMO_ATTR_DIRECTION ) ) {
							// Flag Flip Flop -- Check if alt_mode & set Range accordingly
							unsigned char is_altmode = CHECK_BIT( target_step->attr_STATUS, STEP_EVENT_TRACK_ALT_MODE + 5 ) ? TRUE : FALSE;
							if ( is_altmode ) {
								// Set range to default 1 else default 16
								target_page->Track[ target_page->stepSelectionSingleRow ]->event_max[ATTR_DIRECTION] = TRACK_DEF_RANGE_EVENT;  //Default==1
							}
							else {
								target_page->Track[ target_page->stepSelectionSingleRow ]->event_max[ATTR_DIRECTION] = TRACK_MAX_RANGE_DIR; //Default==5(NEMO)
							}
							// Reset attr_AMT to Default
							target_step->attr_AMT = STEP_DEF_AMOUNT;
						}
						else {
							//POS
							target_step->attr_AMT = TRACK_DEF_RANGE_EVENT;
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
					}
				}
				#endif
				// Click on an active event - turn the event OFF
				step_event_attribute_toggle( target_page->Step[ target_page->stepSelectionSingleRow ][ target_page->stepSelectionSingleCol ], temp );

				#ifdef FEATURE_STEP_EVENT_TRACKS
				// CHECK IF POS/DIR & ALT_MODE..
				// Reset attr_AMT to Default if not within target value
				if (  ( Step_get_status( target_step, STEPSTAT_EVENT ) == ON )
					&&  (  ( event_data_attr ==  ATTR_POSITION )
					||	(  event_data_attr ==  ATTR_DIRECTION )	)  )  {

					unsigned char is_altmode = CHECK_BIT( target_step->attr_STATUS, STEP_EVENT_TRACK_ALT_MODE + 5 ) ? TRUE : FALSE;

					if ( ( is_altmode )
						&&  ( ( target_step->attr_AMT > 8 )
						|| ( target_step->attr_AMT < -8 ) )   )  {

						target_step->attr_AMT = STEP_DEF_AMOUNT;
					}
				}
				#endif
			}
		} // ROW II Key
	


		// ////////////
		// ROW III - the value specifiers: Value, Event, Range
		//
		if ( (( keyNdx - NEMO_ROW_III ) % 11) == 0 ){
			
			// Target coordinates
			row = target_page->stepSelectionSingleRow;
			col = (( keyNdx - NEMO_ROW_III ) / 11) - 1 ;

			switch( col ){

				case 7:	
					// This is always allowed
					NEMO_step_VER = VER_VALUE;
					break;					
				
				case 12:
					// Range allowed only in Event context, when event is set
					if (	( NEMO_step_VER == VER_EVENT )
						&&	( Step_get_status( target_page->Step[ target_page->stepSelectionSingleRow ]
											  				    [ target_page->stepSelectionSingleCol ],
												 	   STEPSTAT_EVENT ) == ON )
						){
						NEMO_step_VER = VER_RANGE;
					}
					break;
			}			
		} // ROW III Key

// }


