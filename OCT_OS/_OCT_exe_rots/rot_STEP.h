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





void rot_exec_STEP( 	Pagestruct* target_page,
						unsigned char rotNdx,
						unsigned char direction ){

		unsigned char row = 0;
		unsigned char col = 0;
		unsigned char temp = 0;

		row = target_page->stepSelectionSingleRow;
		col = target_page->stepSelectionSingleCol;

		switch( rotNdx ){

			// BIG KNOB
			case 0:
				switch( MODE_OBJECT_SELECTION ){

					case CHORDEYE:
					#ifdef FEATURE_ENABLE_CHORD_OCTAVE
					case CHORDEYE_OCTAVE_FIRST:
					case CHORDEYE_OCTAVE_SECOND:
					case CHORDEYE_OCTAVE_THIRD:
					#endif
						// Enter the step strum level
//						PhraseMultiTweakReset();
//						PhraseEditStepNumber( target_page->Step[row][col], direction );

						// Enter the step strum level
						temp = ( target_page->Step[row][col]->chord_data & 0xF800 ) >> 11;
						modify_parameter( &temp, 0, 18, direction, OFF, FIXED);

						target_page->Step[row][col]->chord_data = ( temp << 11 )
							| ( target_page->Step[row][col]->chord_data & 0x7FF );

						break;

					default:

						// If an event is set show the range defined for set event.
						if ( Step_get_status( target_page->Step[row][col], STEPSTAT_EVENT ) == ON
			   				){

							// Depending on which event is set, show range
							switch( (target_page->Step[row][col]->event_data & 0x0F) + 1 ){

								case ATTR_VELOCITY:
									modify_parameter( 	&target_page->Track[row]->event_max[ATTR_VELOCITY],
														TRACK_MIN_RANGE_VEL,
														TRACK_MAX_RANGE_VEL,
														direction, OFF, FIXED );
									break;

								case ATTR_PITCH:
									modify_parameter( 	&target_page->Track[row]->event_max[ATTR_PITCH],
														TRACK_MIN_RANGE_PIT,
														TRACK_MAX_RANGE_PIT,
														direction, OFF, FIXED );
									break;

								case ATTR_LENGTH:
									modify_parameter( 	&target_page->Track[row]->event_max[ATTR_LENGTH],
														TRACK_MIN_RANGE_LEN,
														TRACK_MAX_RANGE_LEN,
														direction, OFF, FIXED );
									break;

								case ATTR_START:
									modify_parameter( 	&target_page->Track[row]->event_max[ATTR_START],
														TRACK_MIN_RANGE_STA,
														TRACK_MAX_RANGE_STA,
														direction, OFF, FIXED );
									break;

								case ATTR_DIRECTION:
									modify_parameter( 	&target_page->Track[row]->event_max[ATTR_DIRECTION],
														TRACK_MIN_RANGE_DIR,
														TRACK_MAX_RANGE_DIR,
														direction, OFF, FIXED );
									break;

								case ATTR_AMOUNT:
									modify_parameter( 	&target_page->Track[row]->event_max[ATTR_AMOUNT],
														TRACK_MIN_RANGE_AMT,
														TRACK_MAX_RANGE_AMT,
														direction, OFF, FIXED );
									break;

								case ATTR_GROOVE:
									modify_parameter( 	&target_page->Track[row]->event_max[ATTR_GROOVE],
														TRACK_MIN_RANGE_GRV,
														TRACK_MAX_RANGE_GRV,
														direction, OFF, FIXED );
									break;

								case ATTR_MIDICC:
									modify_parameter( 	&target_page->Track[row]->event_max[ATTR_MIDICC],
														TRACK_MIN_RANGE_MCC,
														TRACK_MAX_RANGE_MCC,
														direction, OFF, FIXED );
									break;

								case ATTR_MIDICH:
									modify_parameter( 	&target_page->Track[row]->event_max[ATTR_MIDICH],
														TRACK_MIN_RANGE_MCH,
														TRACK_MAX_RANGE_MCH,
														direction, OFF, FIXED );
									break;

							} // switch on selected event type
						} // event is set on this step
						break;
				} // mode selection
				break;



			// EDITOR
			case 1: case 2: case 3: case 4: case 5:
			case 6: case 7: case 8: case 9: case 10:

				// Modify the attribute value; normalize the rotNdx
				switch( rotNdx-1 ) {

					case VELOCITY:
						modify_signed_parameter(
							&target_page->Step[row][col]->attr_VEL,
							STEP_MIN_VELOCITY, 	STEP_MAX_VELOCITY, 	direction, ON, FIXED );
						break;

					case PITCH:
						modify_signed_parameter(
							&target_page->Step[row][col]->attr_PIT,
							STEP_MIN_PITCH, 	STEP_MAX_PITCH, 	direction, ON, FIXED );
						break;

					case LENGTH:
						modify_parameter(
							&target_page->Step[row][col]->attr_LEN,
// old						STEP_MIN_LENGTH, 	((unsigned char)STEP_MAX_LENGTH), 	direction, ON, FIXED );
//							STEP_MIN_LENGTH, 	STEP_MAX_LENGTH, 	direction, ON, WRAPPED );
							LEGATO,	 	STEP_MAX_LENGTH, 	direction, ON, FIXED );
						break;

					case START:
						modify_parameter(
							&target_page->Step[row][col]->attr_STA,
							STEP_MIN_START, 	STEP_MAX_START, 	direction, ON, FIXED );
						break;

					case POSITION:
						if ( target_page->Step[row][col]->phrase_num != 0 ){
							PhraseMultiTweakReset();
							PhraseEditStepFactor( target_page->Step[row][col], direction );
						}
						break;

					#if PHRASE_MODE_SERIOUS
					case DIRECTION:
						PhraseMultiTweakReset();
						PhraseEditStepType( target_page->Step[row][col], direction );
						break;
					#endif

					case AMOUNT:
						modify_signed_parameter(
							&target_page->Step[row][col]->attr_AMT,
							STEP_MIN_AMOUNT, 	STEP_MAX_AMOUNT, 	direction, ON, FIXED );
						break;

					case GROOVE:
						PhraseMultiTweakReset();
						PhraseEditStepNumber( target_page->Step[row][col], direction );
						break;

					case MIDICC:
						modify_signed_parameter(
							&target_page->Step[row][col]->attr_MCC,
							STEP_MIN_MIDICC, 	((unsigned char)STEP_MAX_MIDICC), 	direction, ON, FIXED );
				}

				break;
		} // switch( rotNdx )

}
