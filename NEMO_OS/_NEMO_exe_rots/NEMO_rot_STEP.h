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





void NEMO_rot_exec_STEP( 	Pagestruct* target_page, unsigned char rotNdx, unsigned char direction ){

	unsigned char row = 0;
	unsigned char col = 0;
	unsigned char temp = 0;

	row = target_page->stepSelectionSingleRow;
	col = target_page->stepSelectionSingleCol;

	switch( rotNdx ){

		// BIG KNOB
		case 0:

			switch( NEMO_step_VER ){

				case VER_EVENT:
					#ifdef FEATURE_STEP_EVENT_TRACKS
					temp = ( target_page->Step[row][col]->event_data & 0x0F ) + 1;
					if (  ( Step_get_status( target_page->Step[row][col], STEPSTAT_EVENT ) == ON )
						&&  ( ( temp >= EVENT_FLAG_TRACK_MUTE )
						||	( ( temp ==  ATTR_POSITION )
						&&	( CHECK_BIT( target_page->Step[row][col]->attr_STATUS, STEP_EVENT_TRACK_ALT_MODE + 5 ) ) )
						||	( ( temp ==  ATTR_DIRECTION )
						&&	( CHECK_BIT( target_page->Step[row][col]->attr_STATUS, STEP_EVENT_TRACK_ALT_MODE + 5 ) ) )	) )  {

						modify_signed_parameter( &target_page->Step[row][col]->attr_AMT, TRACK_MIN_RANGE_EVENT, TRACK_MAX_RANGE_EVENT, direction, ON, FIXED );
					}
					else {
						modify_signed_parameter( 	&target_page->Step[row][col]->attr_AMT,
												STEP_MIN_AMOUNT, STEP_MAX_AMOUNT, direction, ON, FIXED );
					}
					#else
					// Modify the Amount for the event - since the value is shown anyway
					modify_signed_parameter( 	&target_page->Step[row][col]->attr_AMT,
												STEP_MIN_AMOUNT, STEP_MAX_AMOUNT, direction, ON, FIXED );

					#endif
					break;


				case VER_RANGE:
					// If an event is set show the range defined for set event.
					if ( Step_get_status( target_page->Step[row][col], STEPSTAT_EVENT ) == ON
		   				){
						#ifdef FEATURE_STEP_EVENT_TRACKS
						unsigned char target_value = 0;
						//unsigned char temp = ( target_page->Step[row][col]->event_data & 0x0F ) + 1;
						unsigned char is_altmode = CHECK_BIT( target_page->Step[row][col]->attr_STATUS, STEP_EVENT_TRACK_ALT_MODE + 5 ) ? TRUE : FALSE;
						#endif
						unsigned char temp = ( target_page->Step[row][col]->event_data & 0x0F ) + 1;
						// Depending on which event is set, show range
						switch( temp ){

							case ATTR_VELOCITY: 	// VEL
								modify_parameter( 	&target_page->Track[row]->event_max[NEMO_ATTR_VELOCITY],
													TRACK_MIN_RANGE_VEL, TRACK_MAX_RANGE_VEL,
													direction, OFF, FIXED );
								break;
							case ATTR_PITCH:		// PIT
								modify_parameter( 	&target_page->Track[row]->event_max[NEMO_ATTR_PITCH],
													TRACK_MIN_RANGE_PIT, TRACK_MAX_RANGE_PIT,
													direction, OFF, FIXED );
								break;
							case ATTR_LENGTH:		// LEN
								modify_parameter( 	&target_page->Track[row]->event_max[NEMO_ATTR_LENGTH],
													TRACK_MIN_RANGE_LEN, TRACK_MAX_RANGE_LEN,
													direction, OFF, FIXED );
								break;
							case ATTR_START:		// STA
								modify_parameter( 	&target_page->Track[row]->event_max[NEMO_ATTR_START],
													TRACK_MIN_RANGE_STA, TRACK_MAX_RANGE_STA,
													direction, OFF, FIXED );
								break;
							case ATTR_DIRECTION:	// DIR
								#ifdef FEATURE_STEP_EVENT_TRACKS
								if ( is_altmode ) {
									target_value = target_page->Track[row]->event_max[NEMO_ATTR_DIRECTION] & 0x1F;
									modify_parameter( 	&target_value,
											0, TRACK_MAX_RANGE_EVENT,
											direction, OFF, FIXED );

									target_page->Track[row]->event_max[NEMO_ATTR_DIRECTION] = ( target_page->Track[row]->event_max[NEMO_ATTR_DIRECTION] & 0xE0 ) | target_value;
								}
								else {
									modify_parameter( 	&target_page->Track[row]->event_max[ATTR_DIRECTION],
											TRACK_MIN_RANGE_DIR,
											TRACK_MAX_RANGE_DIR,
											direction, OFF, FIXED );
								}
								#else
								modify_parameter( 	&target_page->Track[row]->event_max[ATTR_DIRECTION],
										TRACK_MIN_RANGE_DIR,
										TRACK_MAX_RANGE_DIR,
										direction, OFF, FIXED );
								#endif
								break;
							case ATTR_POSITION:		// POS
								#ifdef FEATURE_STEP_EVENT_TRACKS
								target_value = target_page->Track[row]->event_max[NEMO_ATTR_POSITION] & 0x1F;
								modify_parameter( 	&target_value,
													0, TRACK_MAX_RANGE_EVENT,
													direction, OFF, FIXED );

								target_page->Track[row]->event_max[NEMO_ATTR_POSITION] = ( target_page->Track[row]->event_max[NEMO_ATTR_POSITION] & 0xE0 ) | target_value;
								#else
								modify_parameter( 	&target_page->Track[row]->event_max[NEMO_ATTR_POSITION],
													TRACK_MIN_RANGE_POS, TRACK_MAX_RANGE_POS,
													direction, OFF, FIXED );
								#endif
								break;
							case ATTR_AMOUNT:		// AMT
								modify_parameter( 	&target_page->Track[row]->event_max[NEMO_ATTR_AMOUNT],
													TRACK_MIN_RANGE_AMT, TRACK_MAX_RANGE_AMT,
													direction, OFF, FIXED );
								break;
							case ATTR_GROOVE:		// GRV
								modify_parameter( 	&target_page->Track[row]->event_max[NEMO_ATTR_GROOVE],
													TRACK_MIN_RANGE_GRV, TRACK_MAX_RANGE_GRV,
													direction, OFF, FIXED );
								break;
							case ATTR_MIDICC:		// MCC
								modify_parameter( 	&target_page->Track[row]->event_max[NEMO_ATTR_MIDICC],
													TRACK_MIN_RANGE_MCC, TRACK_MAX_RANGE_MCC,
													direction, OFF, FIXED );
								break;
							case ATTR_MIDICH:		// MCH
								modify_parameter( 	&target_page->Track[row]->event_max[NEMO_ATTR_MIDICH],
													TRACK_MIN_RANGE_MCH, TRACK_MAX_RANGE_MCH,
													direction, OFF, FIXED );
								break;
							#ifdef FEATURE_STEP_EVENT_TRACKS
							default:
								if ( temp >= EVENT_FLAG_TRACK_MUTE ) {
									target_value = target_page->Track[row]->event_max[temp] & 0x0F;
									modify_parameter( 	&target_value,
														1, TRACK_MAX_RANGE_EVENT,
														direction, OFF, FIXED );
									target_page->Track[row]->event_max[temp] = ( target_page->Track[row]->event_max[temp] & 0xF0 ) | target_value;
								}
								break;
							#endif


						} // switch on selected event type
					}


					break;


				case VER_VALUE:

					// Change the value of the selected attribute of the track
					switch( NEMO_selectedStepAttribute ){

						case NEMO_ATTR_VELOCITY:
							modify_signed_parameter( 	&target_page->Step[row][col]->attr_VEL,
											STEP_MIN_VELOCITY, STEP_MAX_VELOCITY, direction, ON, FIXED );
							break;
						case NEMO_ATTR_PITCH:
							// Allow operations only as long as the pitch is in the valid range.
//							if ( 	(	( direction == INC )
//									&&	( target_page->Track[row]->attr_PIT
//									  	+ target_page->Step[row][col]->attr_PIT ) >= 0 )
//
//								||	(	( direction == DEC )
//									&&	( target_page->Track[row]->attr_PIT
//									  	+ target_page->Step[row][col]->attr_PIT ) >  0 )
//								){

								modify_signed_parameter( 	&target_page->Step[row][col]->attr_PIT,
												STEP_MIN_PITCH, STEP_MAX_PITCH, direction, OFF, FIXED );
//							}
							break;
/*						case NEMO_ATTR_LENGTH:
							modify_parameter( 	&target_page->Step[row][col]->attr_LEN,
												STEP_MIN_LENGTH, STEP_MAX_LENGTH, direction, OFF, FIXED );
							break;
*/						case NEMO_ATTR_LENGTH:
							modify_parameter(	&target_page->Step[row][col]->attr_LEN,
												LEGATO,	 	STEP_MAX_LENGTH, 	direction, OFF, FIXED );
							break;

						case NEMO_ATTR_START:
							modify_parameter( 	&target_page->Step[row][col]->attr_STA,
												STEP_MIN_START, STEP_MAX_START, direction, OFF, FIXED );
							break;
						case NEMO_ATTR_AMOUNT:
							#ifdef FEATURE_STEP_EVENT_TRACKS
							temp = ( target_page->Step[row][col]->event_data & 0x0F ) + 1;
							if (  ( Step_get_status( target_page->Step[row][col], STEPSTAT_EVENT ) == ON )
								&&  ( ( temp >= EVENT_FLAG_TRACK_MUTE )
								||	( ( temp ==  ATTR_POSITION )
								&&	( CHECK_BIT( target_page->Step[row][col]->attr_STATUS, STEP_EVENT_TRACK_ALT_MODE + 5 ) ) )
								||	( ( temp ==  ATTR_DIRECTION )
								&&	( CHECK_BIT( target_page->Step[row][col]->attr_STATUS, STEP_EVENT_TRACK_ALT_MODE + 5 ) ) )	) )  {

								modify_signed_parameter( &target_page->Step[row][col]->attr_AMT, TRACK_MIN_RANGE_EVENT, TRACK_MAX_RANGE_EVENT, direction, ON, FIXED );
							} else {
								modify_signed_parameter( 	&target_page->Step[row][col]->attr_AMT, STEP_MIN_AMOUNT, STEP_MAX_AMOUNT, direction, ON, FIXED );
							}
						#else
							// Modify the Amount for the event - since the value is shown anyway
							modify_signed_parameter( 	&target_page->Step[row][col]->attr_AMT, STEP_MIN_AMOUNT, STEP_MAX_AMOUNT, direction, ON, FIXED );
						#endif
							break;

						case NEMO_ATTR_GROOVE:
							PhraseMultiTweakReset();
							PhraseEditStepNumber( target_page->Step[row][col], direction );
							break;

						case NEMO_ATTR_DIRECTION:
							#ifdef FEATURE_STEP_SHIFT
								StepShiftRot( target_page, direction );
							#endif
							break;

						case NEMO_ATTR_MIDICC:
							modify_signed_parameter( 	&target_page->Step[row][col]->attr_MCC,
												STEP_MIN_MIDICC, STEP_MAX_MIDICC, direction, ON, FIXED );
							break;

						case NEMO_ATTR_POSITION:
							if ( target_page->Step[row][col]->phrase_num != 0 ){
								PhraseMultiTweakReset();
								PhraseEditStepFactor( target_page->Step[row][col], direction );
							}
							break;
					}
					break;


				case VER_CHORD:
				#ifdef FEATURE_ENABLE_CHORD_OCTAVE
				case VER_CHORD_OCTAVE_FIRST:
				case VER_CHORD_OCTAVE_SECOND:
				case VER_CHORD_OCTAVE_THIRD:
				#endif
					// Change the value of the selected attribute of the track
					switch( NEMO_selectedStepAttribute ){

						case NEMO_ATTR_PITCH:
							modify_signed_parameter( 	&target_page->Step[row][col]->attr_PIT,
												STEP_MIN_PITCH, STEP_MAX_PITCH, direction, ON, FIXED );
							break;

						case NEMO_ATTR_START:
							// Enter the step strum level
							row = target_page->stepSelectionSingleRow;
							col = target_page->stepSelectionSingleCol;

							temp = ( target_page->Step[row][col]->chord_data & 0xF800 ) >> 11;
							modify_parameter( &temp, 0, 18, direction, OFF, FIXED);

							target_page->Step[row][col]->chord_data = ( temp << 11 )
								| ( target_page->Step[row][col]->chord_data & 0x7FF );
							break;

						case NEMO_ATTR_AMOUNT:
							// Compute the step polyphony
							temp = ( target_page->Step[row][col]->chord_up >> 29 );
							modify_parameter( &temp, 0, 6, direction, OFF, FIXED);

							// Set the polyphony value into the step
							set_step_chord_size( target_page->Step[row][col], temp );
							break;
					}
					break;


			} // switch( NEMO_step_VER )
			break;



		// EDITOR
		case 1:
		case 2:
		case 3:
		case 4:

			// Point the display to the changing attribute
			NEMO_selectedStepAttribute = rotNdx;

			// Modify the attribute value; normalize the rotNdx
			switch( rotNdx ) {

				case ROT_PIT:
					// Switch to the value display
					if ( NEMO_step_VER < VER_CHORD ){
						NEMO_step_VER = VER_VALUE;
					}

//					if ( 	(	( direction == INC )
//							&&	( target_page->Track[row]->attr_PIT
//							  	+ target_page->Step[row][col]->attr_PIT ) >= 0 )
//
//						||	(	( direction == DEC )
//							&&	( target_page->Track[row]->attr_PIT
//							  	+ target_page->Step[row][col]->attr_PIT ) >  0 )
//						){
						modify_signed_parameter(	&target_page->Step[row][col]->attr_PIT,
							STEP_MIN_PITCH, 	STEP_MAX_PITCH, 	direction, ON, FIXED );
//					}

					break;

				case ROT_VEL:
					// Switch to the value display
					NEMO_step_VER = VER_VALUE;
					modify_signed_parameter(	&target_page->Step[row][col]->attr_VEL,
							STEP_MIN_VELOCITY, 	STEP_MAX_VELOCITY, 	direction, ON, FIXED );
					break;

				case ROT_LEN:
					// Switch to the value display
					NEMO_step_VER = VER_VALUE;
					modify_parameter(			&target_page->Step[row][col]->attr_LEN,
							LEGATO,	 	STEP_MAX_LENGTH, 	direction, ON, FIXED );
					break;

				case ROT_STA:
					// Switch to the value display
					if ( NEMO_step_VER < VER_CHORD ){
						NEMO_step_VER = VER_VALUE;
					}
					modify_parameter(			&target_page->Step[row][col]->attr_STA,
							STEP_MIN_START, 	STEP_MAX_START, 	direction, ON, FIXED );
					break;
			}
			break;

	} // switch( rotNdx )

}
