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


switch (content) {

	// NUMERIC INDICATOR RELEVANT FOR BIG KNOB
	case NUMERIC_QUADRANT:

		switch( G_zoom_level ){

			case zoomGRIDTRACK:
				// fallthrough

			case zoomGRID:

				switch( GRID_status ){
					case GRID_SELECT:

						// Show selection
						// MIR_write_dot_C( GRID_scene, MIR_GREEN );
						MIR_write_dot_C( GRID_scene, MIR_RED   );
						MIR_write_dot_C( GRID_scene, MIR_BLINK );
						break;


					case GRID_EXPORT:
						MIR_write_dot_C( GRID_scene, MIR_RED   );
						MIR_write_dot_C( GRID_scene, MIR_BLINK );
						break;


					case GRID_DEFAULT:
						MIR_write_numeric_C( G_master_tempo );
						break;
					}
				break;


			// Cases other than GRID and GRIDTRACK
			default:
				if ( target_page->trackSelection == 0 ){

					switch( GRID_bigknob_SELECT ){

						case ON:
							// Only relevant in page mode
							if ( G_zoom_level != zoomPAGE ){
								break;
							}

							if ( target_page->scaleStatus == OFF ){

								// Green Tempo LED
								MIR_write_dot( LED_TEMPO, MIR_GREEN );

								// Orange Select LED
								MIR_write_dot( LED_SCALE_MYSEL, MIR_RED   );
								MIR_write_dot( LED_SCALE_MYSEL, MIR_GREEN );
							}

							// Show the currently active banks
							for( i=0; i<GRID_NROF_BANKS; i++ ){

								// There is a page playing in the bank
								if ( GRID_p_selection[i] != NULL ){

									// All active pages in green
									MIR_write_dot( circleNrField[8-i], MIR_GREEN );

									// Current page blinking orange
									if ( GRID_p_selection[i] == target_page ){

										MIR_write_dot( circleNrField[8-i], MIR_RED   );
										MIR_write_dot( circleNrField[8-i], MIR_BLINK );
									}
								}
							} // bank iterator
							break;

						case OFF:
							// Orange Tempo LED and G_master_tempo value
							if ( target_page->scaleStatus == OFF ){

								MIR_write_dot( LED_TEMPO, MIR_RED   );
								MIR_write_dot( LED_TEMPO, MIR_GREEN );
							}

							// Write the G_master_tempo in the numeric quadrant of the Circle
							// But only if we are not in preview mode with a selected step!
							if (	!(	(	( target_page->editorMode == PREVIEW )
										||	( target_page->editorMode == PREVIEW_PERFORM )
										)
									&&	( is_pressed_steprange() != 0 ))) {

								MIR_write_numeric_C( G_master_tempo );
							}

							// Green Select LED
							if ( G_zoom_level == zoomPAGE ){
								MIR_write_dot( LED_SCALE_MYSEL, MIR_GREEN );
							}
							break;
					} // switch( GRID_bogknob_SELECT )

				} // No active track selection

//				// Some track is selected
//				else{
//
//					// Depending on how many tracks are selected in the page
//					switch( my_bit_cardinality( target_page->trackSelection ) ){
//
//						// Only one track is selected
//						case 1:
//							// Get the pitch of the selected track
//							i = target_page->Track[ my_bit2ndx( target_page->trackSelection ) ]->attr_VEL;
//
//							// Display it in the inner circle
//							MIR_write_numeric_C( i );
//							break;
//
//						// More than one track is selected
//						default:
//							// For now nothing to do
//							break;
//					} // switch( cardinality of trackselection )
//
//				} // some track is selected

				break;

		}
		break;



	// TRANSPORT AREA
	case TRANSPORT:

		if ( G_run_bit == ON ) {

			i = LED_PLAY1;
			// MIDI LOAD MONITOR
			if (G_MIDI_load_monitor1 == MIR_RED){

				MIR_write_dot (258, MIR_RED);
				// MIR_write_dot (i, MIR_BLINK);
			}
			if (G_MIDI_load_monitor2 == MIR_RED){

				MIR_write_dot (257, MIR_RED);
				// MIR_write_dot (i, MIR_BLINK);
			}
			else {

				if ( G_zoom_level != zoomTRACK ){

					MIR_write_dot (i, MIR_GREEN);
				}
			}


			i = LED_PAUSE;

			// CPU LOAD MONITOR
			if ( G_CPU_load_monitor == MIR_RED ){
				// MIR_write_dot (i, MIR_RED);
				// MIR_write_dot (i, MIR_BLINK);
				MIR_write_dot (256, MIR_RED);
				MIR_write_dot (255, MIR_RED);
				MIR_write_dot (254, MIR_RED);
				MIR_write_dot (253, MIR_RED);
				MIR_write_dot (252, MIR_RED);
			}

			// Almost overrunning
			else if ( cpu_load_nearly_at_max() ){
				MIR_write_dot (256, MIR_GREEN);
				MIR_write_dot (255, MIR_GREEN);
				MIR_write_dot (254, MIR_GREEN);

				MIR_write_dot (256, MIR_RED);
				MIR_write_dot (255, MIR_RED);
				MIR_write_dot (254, MIR_RED);
			}
		}

		else {
			// STOP condition
			MIR_write_dot (LED_STOP, MIR_RED);
			// Blink when running on internal clock
			if ( G_clock_source == INT ) {
				// MIR_write_dot (LED_STOP, MIR_BLINK);
			}
			if ( prev_G_stop_bit == OFF && G_pause_bit == ON ){
				MIR_write_dot (LED_STOP, MIR_BLINK);
			}
		}

		// Pause condition
		if (G_pause_bit == ON) {
			// Show the pausebit
			MIR_write_dot( LED_PAUSE, MIR_RED   );
			MIR_write_dot( LED_PAUSE, MIR_GREEN );
			// Blink when running on internal clock
			if ( G_clock_source == INT ){
				// MIR_write_dot( LED_PAUSE, MIR_BLINK );
			}
		}

		break; // TRANSPORT


	// FOLLOW_?? state indicator
	case FOLLOW:
		switch( G_zoom_level ){

			case zoomGRIDTRACK:
			case zoomGRID:

				break;

			case zoomPAGE:
			case zoomMIXMAP:
				if ( follow_flag == FOLLOW_PAGE ){

					MIR_write_dot( LED_FOLLOW, MIR_RED   );
				}
				else {
					MIR_write_dot( LED_FOLLOW, MIR_GREEN     );
				}
				break;

			case zoomTRACK:
			case zoomMAP:
				if ( follow_flag == FOLLOW_TRACK ){

					MIR_write_dot( LED_FOLLOW, MIR_RED );
				}
				else {
					MIR_write_dot( LED_FOLLOW, MIR_GREEN );
				}
				break;
		}
		break; // TRANSPORT




	// Buttons of Big Knob
	case BIG_KNOB:

		switch( G_zoom_level ){

			//
			// GRID zoom 7 mode
			//
			case zoomGRIDTRACK:
			case zoomGRID:

				switch( GRID_status ){

					case GRID_DEFAULT:

						if ( G_pause_bit == OFF ){

							// G_master_tempo shows switchmode for the GRID
							if ( GRID_switch_mode == GRID_SWITCH_OCLOCK ){
								MIR_write_dot( LED_TEMPO, MIR_RED   );
							}
							else{
								MIR_write_dot( LED_TEMPO, MIR_GREEN );
							}

							//ALIGN is available
							MIR_write_dot( LED_ALIGN, MIR_GREEN );

						} else { // PMLS Mode

							//ALIGN is available
							MIR_write_dot( LED_ALIGN, MIR_GREEN );
							if ( (G_measure_indicator_value * 5) + G_measure_indicator_part != G_measure_locator ) {
								MIR_write_dot( LED_ALIGN, MIR_BLINK );
							}
							//BCK is available
							MIR_write_dot( LED_CHAINER, MIR_RED );
							MIR_write_dot( LED_CHAINER, MIR_GREEN );
							//FWD is available
							MIR_write_dot( LED_FOLLOW, MIR_RED );
							MIR_write_dot( LED_FOLLOW, MIR_GREEN );
						}

						break;


					case GRID_SELECT:

						// Show the selected scene
						MIR_write_dot( LED_SCALE_MYSEL, MIR_RED   );
						MIR_write_dot( LED_SCALE_MYSEL, MIR_GREEN );

						// G_master_tempo is available
						MIR_write_dot( LED_TEMPO, MIR_GREEN );
						break;


					case GRID_EXPORT:
						// EXPORT is active
						MIR_write_dot( LED_ALIGN, MIR_RED 	);
						MIR_write_dot( LED_ALIGN, MIR_GREEN );
						MIR_write_dot( LED_ALIGN, MIR_BLINK );

						// SELECT is available
						MIR_write_dot( LED_SCALE_MYSEL, MIR_GREEN );

						// G_master_tempo is available
						MIR_write_dot( LED_TEMPO, MIR_GREEN );
						break;
				}

				break;


			case zoomPAGE:

				if ( G_rec_ctrl_track != NULL ){
					MIR_write_dot( LED_MUTE_MASTER, MIR_RED );
					MIR_write_dot( LED_MUTE_MASTER, MIR_GREEN );
					MIR_write_dot( LED_MUTE_MASTER, MIR_BLINK );
				}

				// In PREVIEW mode show the track VEL in circle
				if (	( target_page->trackSelection == 1 )
					&&	(	( target_page->editorMode == PREVIEW )
						||	( target_page->editorMode == PREVIEW_PERFORM )
						)
					){

					// Get the pitch of the selected track
					i = target_page->Track[ my_bit2ndx( target_page->trackSelection ) ]->attr_VEL;

					// Display it in the inner circle
					MIR_write_numeric_C( i );
				}

				// Empty statement for now
				switch( GRID_bigknob_SELECT ){
					case ON:
						break;
				}


			//
			// Modes other than GRID or PAGE
			//
			default:
				// Program change numbers in track selection mode
				if ( my_bit_cardinality( target_page->trackSelection ) == 1 ){

					// If in track zoom mode
					if ( G_zoom_level == zoomTRACK ){

						switch( target_page->trackAttributeSelection ){

							// VEL
							case ( 1 << 0 ):
								MIR_write_factor_C(
									target_page->Track[ my_bit2ndx( target_page->trackSelection )]->VEL_factor
									#ifdef EVENTS_FACTORIZED
									+ target_page->Track[ my_bit2ndx( target_page->trackSelection )]->event_offset[ATTR_VELOCITY]
									#endif // EVENTS_FACTORIZED
									);
								MIR_write_dot( LED_SCALE_MYSEL, MIR_RED   );
								MIR_write_dot( LED_SCALE_MYSEL, MIR_GREEN );
								break;

							// PIT
							case ( 1 << 1 ):
								MIR_write_factor_C(
									target_page->Track[ my_bit2ndx( target_page->trackSelection )]->PIT_factor
									#ifdef EVENTS_FACTORIZED
									+ target_page->Track[ my_bit2ndx( target_page->trackSelection )]->event_offset[ATTR_PITCH]
									#endif // EVENTS_FACTORIZED
									);
								MIR_write_dot( LED_SCALE_MYSEL, MIR_RED   );
								MIR_write_dot( LED_SCALE_MYSEL, MIR_GREEN );
								break;

							// LEN
							case ( 1 << 2 ):
								MIR_write_factor_C(
									target_page->Track[ my_bit2ndx( target_page->trackSelection )]->LEN_factor
									+ target_page->Track[ my_bit2ndx( target_page->trackSelection )]->event_offset[ATTR_LENGTH] );
								MIR_write_dot( LED_SCALE_MYSEL, MIR_RED   );
								MIR_write_dot( LED_SCALE_MYSEL, MIR_GREEN );
								break;

							// STA
							case ( 1 << 3 ):
								MIR_write_factor_C(
									target_page->Track[ my_bit2ndx( target_page->trackSelection )]->STA_factor
									+ target_page->Track[ my_bit2ndx( target_page->trackSelection )]->event_offset[ATTR_START] );
								MIR_write_dot( LED_SCALE_MYSEL, MIR_RED   );
								MIR_write_dot( LED_SCALE_MYSEL, MIR_GREEN );
								break;

							// AMT
							case ( 1 << 6 ):
								MIR_write_factor_C(
									target_page->Track[ my_bit2ndx( target_page->trackSelection )]->AMT_factor
									+ target_page->Track[ my_bit2ndx( target_page->trackSelection )]->event_offset[ATTR_AMOUNT] );
								MIR_write_dot( LED_SCALE_MYSEL, MIR_RED   );
								MIR_write_dot( LED_SCALE_MYSEL, MIR_GREEN );
								break;

							// GRV
							case ( 1 << 7 ):
								MIR_write_factor_C(
									target_page->Track[ my_bit2ndx( target_page->trackSelection )]->GRV_factor
									+ target_page->Track[ my_bit2ndx( target_page->trackSelection )]->event_offset[ATTR_GROOVE] );
								MIR_write_dot( LED_SCALE_MYSEL, MIR_RED   );
								MIR_write_dot( LED_SCALE_MYSEL, MIR_GREEN );
								break;

							// MCC
							case ( 1 << 8 ):
								MIR_write_factor_C(
									target_page->Track[ my_bit2ndx( target_page->trackSelection )]->MCC_factor
									+ target_page->Track[ my_bit2ndx( target_page->trackSelection )]->event_offset[ATTR_MIDICC] );
								MIR_write_dot( LED_SCALE_MYSEL, MIR_RED   );
								MIR_write_dot( LED_SCALE_MYSEL, MIR_GREEN );
								break;

							case 0:
//								// Write the Program change number in the numeric quadrant of the Circle
//								// ..used to be offset by +1, removed offset based on feedback from Tom
//								MIR_write_numeric_C( target_page->Track[ my_bit2ndx( target_page->trackSelection )]->program_change );
//								MIR_write_dot( LED_PROGRAM, MIR_RED );
//								MIR_write_dot( LED_PROGRAM, MIR_GREEN );

								// Control track MIX ARMED
								if ( Track_get_MISC(target_page->Track[ my_bit2ndx( target_page->trackSelection )], CONTROL_BIT)
										&& CHECK_BIT(target_page->Track[ my_bit2ndx( target_page->trackSelection )]->attr_MISC, TRK_CTRL_MIX)
								){
									MIR_write_dot( LED_MIX_MASTER, MIR_RED );
									MIR_write_dot( LED_MIX_MASTER, MIR_GREEN );
									MIR_write_dot( LED_MIX_MASTER, MIR_BLINK );
								}

								// Control track MOVE SET
								if ( Track_get_MISC(target_page->Track[ my_bit2ndx( target_page->trackSelection )], CONTROL_BIT)
										&& CHECK_BIT(target_page->Track[ my_bit2ndx( target_page->trackSelection )]->attr_MISC, TRK_CTRL_MOVE)
								){
									MIR_write_dot( LED_MIX_MASTER, MIR_RED );
									MIR_write_dot( LED_MIX_MASTER, MIR_GREEN );
								}

								// Control track CLEAR
								if ( Track_get_MISC(target_page->Track[ my_bit2ndx( target_page->trackSelection )], CONTROL_BIT)
										&& CHECK_BIT(target_page->Track[ my_bit2ndx( target_page->trackSelection )]->attr_MISC, TRK_CTRL_CLEAR)
								){
									MIR_write_dot( LED_SELECT_MASTER, MIR_RED );
									MIR_write_dot( LED_SELECT_MASTER, MIR_GREEN );
								}

								// Control track MUT
								if ( Track_get_MISC(target_page->Track[ my_bit2ndx( target_page->trackSelection )], CONTROL_BIT)
									&& CHECK_BIT(target_page->Track[ my_bit2ndx( target_page->trackSelection )]->attr_MISC, TRK_CTRL_MUT_PGTGL)
								){
									MIR_write_dot( LED_MUTE_MASTER, MIR_RED );
									MIR_write_dot( LED_MUTE_MASTER, MIR_GREEN );
								}

								// Control track TEMPO inherit
								if ( Track_get_MISC(target_page->Track[ my_bit2ndx( target_page->trackSelection )], CONTROL_BIT)
										&& CHECK_BIT(target_page->Track[ my_bit2ndx( target_page->trackSelection )]->attr_CCAMT, TRK_CTRL_TEMPO)
								){
									MIR_write_dot( LED_PAUSE, MIR_RED );
									MIR_write_dot( LED_PAUSE, MIR_GREEN );
								}

								// Control track external midi [start] [stop]
								if ( CHECK_BIT(target_page->Track[ my_bit2ndx( target_page->trackSelection )]->attr_MISC, TRK_CTRL_EXT_PLAY)
									&& Track_get_MISC(target_page->Track[ my_bit2ndx( target_page->trackSelection )], CONTROL_BIT)){
										MIR_write_dot( LED_EDIT_MASTER, MIR_RED );
										MIR_write_dot( LED_EDIT_MASTER, MIR_GREEN );
										MIR_write_dot( LED_EDIT_MASTER, MIR_BLINK );
								}
								if ( CHECK_BIT(target_page->Track[ my_bit2ndx( target_page->trackSelection )]->attr_MISC, TRK_CTRL_EXT_STOP)
									&& Track_get_MISC(target_page->Track[ my_bit2ndx( target_page->trackSelection )], CONTROL_BIT)){
										MIR_write_dot( LED_EDIT_MASTER, MIR_RED );
										MIR_write_dot( LED_EDIT_MASTER, MIR_GREEN );
								}

								// Display the program or bank change
								switch( CHANGE_BANK_OR_PGM ){

									case PGM:
										// Write the Program change number in the numeric quadrant of the Circle
										MIR_write_numeric_C( target_page->Track[ my_bit2ndx( target_page->trackSelection )]->program_change );

										if (Track_get_MISC(target_page->Track[ my_bit2ndx( target_page->trackSelection )], CONTROL_BIT)) {
											MIR_write_dot (LED_PROGRAM, MIR_RED);
											MIR_write_dot (LED_PROGRAM, MIR_GREEN);
											MIR_write_dot (LED_PROGRAM, MIR_BLINK);

										} else {
											MIR_write_dot( LED_PROGRAM, MIR_RED );
											MIR_write_dot( LED_PROGRAM, MIR_GREEN );
										}

										MIR_write_dot( LED_SCALE_MYSEL, MIR_GREEN );
										break;

									case BANK:
										// Write the bank change number in the numeric quadrant of the Circle
										MIR_write_numeric_C( target_page->Track[ my_bit2ndx( target_page->trackSelection )]->bank_change );

										if (Track_get_MISC(target_page->Track[ my_bit2ndx( target_page->trackSelection )], CONTROL_BIT)) {
											MIR_write_dot (LED_SCALE_MYSEL, MIR_RED);
											MIR_write_dot (LED_SCALE_MYSEL, MIR_GREEN);
											MIR_write_dot (LED_SCALE_MYSEL, MIR_BLINK);

										} else {
											MIR_write_dot( LED_SCALE_MYSEL, MIR_RED );
											MIR_write_dot( LED_SCALE_MYSEL, MIR_GREEN );
										}

										MIR_write_dot( LED_PROGRAM, MIR_GREEN );
										break;
								}

								// control track
								if (Track_get_MISC(target_page->Track[ my_bit2ndx( target_page->trackSelection )], CONTROL_BIT) == ON) {
									MIR_write_dot (KEY_BK200, MIR_RED);
									MIR_write_dot (KEY_BK200, MIR_GREEN);
									MIR_write_dot (KEY_BK200, MIR_BLINK);
								}

								break;
						}

					}
					else if ( G_zoom_level != zoomSTRUM ){

						// Display the program or bank change
						switch( CHANGE_BANK_OR_PGM ){

							case PGM:
								// Write the Program change number in the numeric quadrant of the Circle
								MIR_write_numeric_C( target_page->Track[ my_bit2ndx( target_page->trackSelection )]->program_change );

								MIR_write_dot( LED_PROGRAM, MIR_RED );
								MIR_write_dot( LED_PROGRAM, MIR_GREEN );

								MIR_write_dot( LED_SCALE_MYSEL, MIR_GREEN );
								break;

							case BANK:
								// Write the bank change number in the numeric quadrant of the Circle
								MIR_write_numeric_C( target_page->Track[ my_bit2ndx( target_page->trackSelection )]->bank_change );

								MIR_write_dot( LED_SCALE_MYSEL, MIR_RED );
								MIR_write_dot( LED_SCALE_MYSEL, MIR_GREEN );

								MIR_write_dot( LED_PROGRAM, MIR_GREEN );
								break;
						}
					}
				}

				break;
		}
		break;



	// Clock Source Indicator
	case CLOCK:

		switch( G_clock_source ){

			case EXT:

//				// Indicate the UART whose clock we are listening to
//				switch( CLOCK_MASTER_UART ){
//
//					case 0:
//						MIR_write_dot( LED_CLOCK, MIR_GREEN 	);
//						break;
//
//					case 1:
//						MIR_write_dot( LED_CLOCK, MIR_RED		);
//						break;
//				}

				// Blink when sequencer running
				 if (G_run_bit == ON) {
					// MIR_write_dot( LED_CLOCK, MIR_BLINK );
					MIR_write_dot( LED_TEMPO, MIR_BLINK );
				 }

				// Show the status of the MIDICLOCK PASSTHROUGH
				switch( MIDICLOCK_PASSTHROUGH ){

					case TRUE:
						MIR_write_dot( LED_CLOCK, MIR_RED );
						break;

					case FALSE:
						MIR_write_dot( LED_CLOCK, MIR_GREEN );
						break;
				}


				break;

			case INT:
				// Visible/available only when the sequencer is not running.
				// if (G_run_bit == OFF) {
					MIR_write_dot( LED_CLOCK, MIR_GREEN );
					MIR_write_dot( LED_CLOCK, MIR_RED 	);
				// }
				break;

			case OFF:
				// Visible/available only when the sequencer is not running.
				if (G_run_bit == OFF) {
					// MIR_write_dot( LED_CLOCK, MIR_GREEN );
					// ..nothing really to do
				}
				break;
		}

		break;


} // switch (content)






