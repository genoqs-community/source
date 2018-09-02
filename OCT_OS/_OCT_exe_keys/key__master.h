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

extern void CheckAndShowBadThreads( unsigned char flag );
extern void Step_clear( Pagestruct* target_page, Stepstruct* target_step );
extern void Step_exe_randomize( Pagestruct* target_page, Stepstruct* target_step );

/* OCTOPUS
 *
 * Interpret incoming key presses
 * genoQs Machines, 07.11.2004
 */


// General functions used by other key exec functions
#include "key_functions.h"
#include "key_OCT_CIRCLE_xpose_STEP.h"
#include "key_NUMERIC_QUADRANT_velo_STEP.h"
#include "key_OCT_CIRCLE_chord_STEP.h"
#include "key_STEP.h"
#include "key_STEP_exe_CLR.h"
#include "key_STEP_exe_RND.h"
#include "key_ScaleSelector_functions.h"
#include "key_ScaleSelector_mini_functions.h"
#include "key_mixermap_matrixclick.h"
#include "key_exe_chainselectors.h"
// #include "key_ABLETON.h"
#include "key_DIAG.h"



// EXECUTE THE KEYPRESS RECEIVED
void executeKey( unsigned int keyNdx ){

	unsigned int  	i=0,
					j=0,
					k=0,
					m=0,
					row=0,
					col=0,
					temp=0
					;

	// decide whether to save the current song position
	if ( keyNdx == KEY_STOP ) {
		G_save_song_pos = ON;
	} else if ( keyNdx == KEY_PLAY1 ) {
		G_save_song_pos = !G_run_bit;
		SEQUENCER_JUST_RESTARTED = OFF;
	}
#ifdef FEATURE_ENABLE_SONG_UPE
	else if ( keyNdx == KEY_PAUSE ) {
		G_save_song_pos = !prev_G_pause_bit;
	}
#endif

	#ifdef FEATURE_SOLO_REC
//	diag_printf("keyNdx=%d\n", keyNdx);
	if ( G_zoom_level == zoomSOLOREC ){
		if ( keyNdx == KEY_RETURN ){
			if ( G_run_bit == OFF ){
				exit_solo_recording();
			}
			return;
		}
	}
	if ( SOLO_has_rec == TRUE && G_zoom_level != zoomSOLOREC ){ // Page Warp

		if ( keyNdx == KEY_ZOOM_PAGE || keyNdx == KEY_ZOOM_GRID || is_pressed_key(KEY_ZOOM_PAGE) ){
			return;
		}

		// Escape back to SoloRec zoom by pressing ESC in Page zoom
		if ( G_zoom_level == zoomPAGE ){

			if ( keyNdx == KEY_RETURN ){
				G_zoom_level = zoomSOLOREC;
				return;
			}
		}
	}
	#endif


	// Work on the page under the grid cursor
	Pagestruct* target_page = &Page_repository[ GRID_CURSOR ];

	// Used to mute a whole chain
	Trackstruct* current_track = NULL;

	// Page pointer holder
	Pagestruct* temp_page = NULL;
	static Pagestruct* previous_page = NULL;
	static Pagestruct* prev_previous_page = NULL; // used to validate page cluster selections
	static unsigned char prev_previous_page_clear = ON;
	// Used for syntax simplification
	signed char* val_ptr = NULL;

	// Used in the key entry routine for the mixer maps modes
	unsigned char* target_ptr = NULL;

// d_iag_printf("  -> executeKey() keyNdx:%d cursor:%d GRIDmode:%d\n",
//		keyNdx, GRID_CURSOR, GRID_play_mode );


	//
	// CHORD SELECTOR
	//
	// Double click enters the mode or chord strum editing.
	if( (keyNdx > 251)
		){
		if ( G_zoom_level == zoomSTRUM ){
#if PHRASE_EDIT_VIEW_NEMO
			// Switch the strum level view
			PhraseCurNoteIx = 258 - keyNdx;
#endif

#if PHRASE_EDIT_VIEW_OCTO
			PhraseEditPoly( PhraseCurPhraseIx, 258 - keyNdx + 1 );
#endif
		}
	} // Chord selector key pressed


//// Show thread status
//if ( keyNdx == 252 ){
//	CheckAndShowBadThreads( TRUE );
//}


	// BIG KNOB KEYS operation
	else if (
			( keyNdx == 201 )
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

		switch( G_zoom_level ){


			case zoomMAP:
				// In key_MAP section
				break;


			case zoomGRID:
			case zoomGRIDTRACK:
			// Enter the G_master_tempo
				#ifdef FEATURE_ENABLE_SONG_UPE
				if ( G_pause_bit == OFF ){
					// Enter the G_master_tempo

					// D O U B L E - C L I C K
					if ((DOUBLE_CLICK_TARGET == keyNdx)
							&& (DOUBLE_CLICK_TIMER > DOUBLE_CLICK_ALARM_SENSITIVITY)) {

						// Handle the 100 button
						if (	( keyNdx == 224 )
						){

							if 		( G_master_tempo  < 100 ) G_master_tempo = 100;
							else if ( G_master_tempo == 100 ) G_master_tempo = MIN_TEMPO;
							else if ( G_master_tempo  > 100 ) G_master_tempo = G_master_tempo - 100;

							G_TIMER_REFILL_update();
						}
						else{
							// Double click modifieds the large value
							G_master_tempo = ((G_master_tempo / 100)*100) + ( BK_KEY_to_xdx( keyNdx ) *10);
							G_TIMER_REFILL_update();
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

						// Double click on 100 sets the tempo to 100
						if ( keyNdx != 224 ){

							// Mmodifies the 1's value, do not react on the 100 button
							G_master_tempo = (G_master_tempo / 10)*10 + BK_KEY_to_xdx( keyNdx );
							G_TIMER_REFILL_update();
						}
					}

				} else { // PMLS Mode - Measure Counter Value Nibbles

					// D O U B L E - C L I C K
					if ((DOUBLE_CLICK_TARGET == keyNdx)
							&& (DOUBLE_CLICK_TIMER > DOUBLE_CLICK_ALARM_SENSITIVITY)) {

						// Handle the 100 button
						if (	( keyNdx == 224 )
						){

							if 		( G_measure_indicator_value  < 100 ) G_measure_indicator_value = 100;
							else if ( G_measure_indicator_value == 100 ) G_measure_indicator_value = 0;
							else if ( G_measure_indicator_value  > 100 ) G_master_tempo = G_measure_indicator_value - 100;
						}
						else if ( keyNdx == 233 ) {

							if 		( G_measure_indicator_value  < 200 ) G_measure_indicator_value = 200;
							else if ( G_measure_indicator_value == 200 ) G_measure_indicator_value = 0;
							else if ( G_measure_indicator_value  > 200 ) G_master_tempo = G_measure_indicator_value - 200;
						}
						else{
							// Double click modifieds the large value
							G_measure_indicator_value = ((G_measure_indicator_value / 100)*100) + ( BK_KEY_to_xdx( keyNdx ) *10);
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

						// Double click on 100 sets the tempo to 100
						if ( keyNdx != 224 ){

							// Mmodifies the 1's value, do not react on the 100 button
							G_measure_indicator_value = (G_measure_indicator_value / 10)*10 + BK_KEY_to_xdx( keyNdx );
						}
					}

				}
				#else
				// D O U B L E - C L I C K
				if ((DOUBLE_CLICK_TARGET == keyNdx)
						&& (DOUBLE_CLICK_TIMER > DOUBLE_CLICK_ALARM_SENSITIVITY)) {

					// Handle the 100 button
					if (	( keyNdx == 224 )
						){

						if 		( G_master_tempo  < 100 ) G_master_tempo = 100;
						else if ( G_master_tempo == 100 ) G_master_tempo = MIN_TEMPO;
						else if ( G_master_tempo  > 100 ) G_master_tempo = G_master_tempo - 100;

						G_TIMER_REFILL_update();
					}
					else{
						// Double click modifieds the large value
						G_master_tempo = ((G_master_tempo / 100)*100) + ( BK_KEY_to_xdx( keyNdx ) *10);
						G_TIMER_REFILL_update();
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

					// Double click on 100 sets the tempo to 100
					if ( keyNdx != 224 ){

						// Mmodifies the 1's value, do not react on the 100 button
						G_master_tempo = (G_master_tempo / 10)*10 + BK_KEY_to_xdx( keyNdx );
						G_TIMER_REFILL_update();
					}
				}
				#endif
				break;



			case zoomPAGE:
			case zoomMIXMAP:
			// G_master_tempo ENTRY, BANK JUMP, or Prg Chg when track selected

				switch( GRID_bigknob_SELECT ){

					// G_master_tempo ENTRY, PROGRAM CHANGE, etc.
					case OFF:

						// When a track is selected, modify its program change
						if ( my_bit_cardinality( target_page->trackSelection ) == 1 ){
							// In preview mode enter the track velocity
							if (	(	( target_page->editorMode == PREVIEW )
									||	( target_page->editorMode == PREVIEW_PERFORM )
									) ){
								// Set in key_PAGE_sel_TRACK.h
								break;
							}

							// Use val_ptr for syntax simplification
							val_ptr = &target_page->Track[ my_bit2ndx( target_page->trackSelection )]->program_change;

							// D O U B L E - C L I C K
							if ((DOUBLE_CLICK_TARGET == keyNdx)
									&& (DOUBLE_CLICK_TIMER > DOUBLE_CLICK_ALARM_SENSITIVITY)) {

								// Handle the 100 button
								if (	( keyNdx == 224 )
									){

									if 		( *val_ptr  < 100 ) *val_ptr = 100;
									else if ( *val_ptr == 100 ) *val_ptr = TRACK_MIN_PROGRAMCHANGE;
									else if ( *val_ptr  > 100 ) *val_ptr = *val_ptr - 100;
								}

								else{
									// Double click modifieds the large value
									*val_ptr = normalize (
										((*val_ptr / 100)*100) + ( BK_KEY_to_xdx( keyNdx ) *10),
										TRACK_MIN_PROGRAMCHANGE,
										TRACK_MAX_PROGRAMCHANGE );
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

								// Double click on 100 sets the tempo to 100
								if ( keyNdx != 224 ){

									// Mmodifies the 1's value, do not react on the 100 button
									*val_ptr = normalize(
										(*val_ptr / 10)*10 + BK_KEY_to_xdx( keyNdx ),
										TRACK_MIN_PROGRAMCHANGE,
										TRACK_MAX_PROGRAMCHANGE );
								}
							}
						} // one (some) track is selected


						// G_master_tempo ENTRY
						// D O U B L E - C L I C K
						else if ((DOUBLE_CLICK_TARGET == keyNdx)
								&& (DOUBLE_CLICK_TIMER > DOUBLE_CLICK_ALARM_SENSITIVITY)) {

							// Don't allow tempo changes during active step selections
							if ( target_page->stepSelection == 0 ){

								// In preview mode enter the step velocity, like in MAP preview mode
								if (	(	( target_page->editorMode == PREVIEW )
										||	( target_page->editorMode == PREVIEW_PERFORM )
										)
									&&	( page_preview_step != NULL )
									){

									// Enter velocity via the outer circle
									key_NUMERIC_QUADRANT_velo_STEP( keyNdx );
								} // preview mode is active

								else{
									// In normal mode simply enter tempo
									// Handle the 100 button
									if (	( keyNdx == 224 )
										){

										if 		( G_master_tempo  < 100 ) G_master_tempo = 100;
										else if ( G_master_tempo == 100 ) G_master_tempo = MIN_TEMPO;
										else if ( G_master_tempo  > 100 ) G_master_tempo = G_master_tempo - 100;

										G_TIMER_REFILL_update();
									}
									else{
										// Double click modifieds the large value
										G_master_tempo = ((G_master_tempo / 100)*100) + ( BK_KEY_to_xdx( keyNdx ) *10);
										G_TIMER_REFILL_update();
									}
								} // not in preview mode

							} // Step selection is not active
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

							// Double click on 100 sets the tempo to 100
							if ( keyNdx != 224 ){

								// Don't allow tempo changes during active step selections
								if ( target_page->stepSelection == 0 ){

									// In preview mode enter the step velocity, like in MAP preview mode
									if (	(	( target_page->editorMode == PREVIEW )
											||	( target_page->editorMode == PREVIEW_PERFORM )
											)
										&&	( page_preview_step != NULL )
										){

										// Enter velocity via the outer circle
										// DIRTY!! - otherwise only double click from the function..
										DOUBLE_CLICK_TARGET = 0;
										key_NUMERIC_QUADRANT_velo_STEP( keyNdx );
									} // preview mode is active

									else{

										// Mmodifies the 1's value, do not react on the 100 button
										G_master_tempo = (G_master_tempo / 10)*10 + BK_KEY_to_xdx( keyNdx );
										G_TIMER_REFILL_update();
									}
								}
							}
						}

						break;



					// BANK JUMP
					case ON:

						// Map the value ranges accordingly
						j = 9 - BK_KEY_to_xdx( keyNdx );

						if (	( j >= 0 )
							&&	( GRID_p_selection[j] != NULL )
							){

							// Move the viewer page
							target_page = GRID_p_selection[j];

							// Move the cursor
							GRID_CURSOR = target_page->pageNdx;
						}
						break;
				}
				break;



			case zoomTRACK:
			// Enter Program Change value

				// Use val_ptr for syntax simplification
				switch( CHANGE_BANK_OR_PGM ){
				case PGM:
					val_ptr = &target_page->Track[ my_bit2ndx( target_page->trackSelection )]->program_change;
					break;
				case BANK:
					val_ptr = &target_page->Track[ my_bit2ndx( target_page->trackSelection )]->bank_change;
					break;
				}

				// D O U B L E - C L I C K
				if ((DOUBLE_CLICK_TARGET == keyNdx)
						&& (DOUBLE_CLICK_TIMER > DOUBLE_CLICK_ALARM_SENSITIVITY)) {

					// Handle the 100 button
					if (	( keyNdx == 224 )
						){

						if 		( *val_ptr  < 100 ) *val_ptr = 100;
						else if ( *val_ptr == 100 ) *val_ptr = TRACK_MIN_PROGRAMCHANGE;
						else if ( *val_ptr  > 100 ) *val_ptr = *val_ptr - 100;
					}

					else{
						// Double click modifieds the large value
						*val_ptr = normalize (
							((*val_ptr / 100)*100) + ( BK_KEY_to_xdx( keyNdx ) *10),
							TRACK_MIN_PROGRAMCHANGE,
							TRACK_MAX_PROGRAMCHANGE );
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

					// Double click on 100 sets the tempo to 100
					if ( keyNdx != 224 ){

						// Mmodifies the 1's value, do not react on the 100 button
						*val_ptr = normalize(
							(*val_ptr / 10)*10 + BK_KEY_to_xdx( keyNdx ),
							TRACK_MIN_PROGRAMCHANGE,
							TRACK_MAX_PROGRAMCHANGE );
					}
				}
				break;

		} // G_zoom_level switch

	} // big knob keys

	#ifdef FEATURE_ENABLE_SONG_UPE
	// Activate control track
	else if ( keyNdx == KEY_BK200 && G_zoom_level == zoomTRACK ) {

		// D O U B L E - C L I C K
		if ((DOUBLE_CLICK_TARGET == keyNdx)
				&& (DOUBLE_CLICK_TIMER > DOUBLE_CLICK_ALARM_SENSITIVITY)) {

			make_control_track(target_page, my_bit2ndx( target_page->trackSelection ));
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

			val_ptr = &target_page->Track[ my_bit2ndx( target_page->trackSelection )]->attr_MISC;
			clear_ctrl_track( target_page, target_page->Track[ my_bit2ndx( target_page->trackSelection )] );
			if ( Track_get_MISC( target_page->Track[ my_bit2ndx( target_page->trackSelection )], CONTROL_BIT ) == ON ) {
				*val_ptr = OFF;
			}
		}
	}
	#endif
	// RETURN : back to some default machine state
	else if (	( keyNdx == KEY_RETURN )

		&&	( G_zoom_level != zoomSYSEX )
		&&	( G_zoom_level != zoomDEVICE )
		&&	( G_zoom_level != zoomDIAG )
		&&	( G_zoom_level != zoomAMANDA )
		){

		// Enter machine lock state
		if (	( G_zoom_level == zoomGRID )
			&& 	( is_pressed_key( KEY_ZOOM_GRID ) )
			){

			G_zoom_level = zoomAMANDA;
			return;
		}

		// Everywhere but in GRID mode - where we need to exit the CC_MAP mode
		else if (	( G_zoom_level != zoomGRID )
				||	(	( G_zoom_level == zoomGRID )
					&&	( GRID_play_mode != GRID_CCMAP) )
			){

			if ( 	( G_zoom_level == zoomPAGE )
					&& 	( target_page->OPS_mode == BIRDSEYE )
					){
				// Load page from flash
				Flash_read_page( GRID_CURSOR );

				// Enter interactive mode
				target_page->OPS_mode = PASSIVE;
			}

			G_zoom_level = zoomPAGE;

			target_page->trackSelection = OFF;
			target_page->trackAttributeSelection = OFF;



			// Unselect all steps in page, if any were selected
			if ( target_page->stepSelection != 0 ){

				for (row=0; row<MATRIX_NROF_ROWS; row++){

					// Clear the stepSELpattern in the page
					target_page->stepSELpattern[target_page->stepSELpattern_ndx][row] = 0;

					// Clear the selection flag for each step in page
					for (col=0; col<MATRIX_NROF_COLUMNS; col++){

						Step_set_status( target_page->Step[row][col], STEPSTAT_SELECT, OFF );
					}
				}
				// Reset the counter of selected steps
				target_page->stepSelection = OFF;
				target_page->stepAttributeSelection = OFF;
			}

			// Reset the timers
			MIX_TIMER 	= OFF;
			EDIT_TIMER 	= OFF;

			// Release the MIDI SYSEX state
			MIDI_RECEIVE_SYSEX = FALSE;
			MIDI_SYSEX_HEADER_SEEN = FALSE;
			G_sysex_stream_keeper_index = 0;
		}
	}


	// Mode bound key interpretation. Should all be like STEP.. using function calls.
	//
	switch (G_zoom_level) {

		case zoomSYSEX:
			#include "key_SYSEX.h"
			break;

/*		case zoomABLETON:
			key_exec_ABLETON( keyNdx );
			break;
*/
		case zoomAMANDA:
			#include "key_AMANDA.h"
			break;

		case zoomSTRUM:
			#include "key_STRUM.h"
			break;

		case zoomDIAG:
			key_exec_DIAG( keyNdx );
			break;

		case zoomGRID:
			#include "key_GRID.h"
			break;

		case zoomSOLOREC:
			#include "key_SOLOREC.h"
			break;

		case zoomGRIDTRACK:
			#include "key_GRIDTRACK.h"
			break;

		case zoomDEVICE:
			#include "key_DEVICE.h"
			break;

		case zoomPAGE:
			#include "key_PAGE.h"
			break;


		case zoomMIXMAP:
			#include "key_MIXMAP.h"
			break;

		case zoomTRACK:
			#include "key_TRACK.h"
			break;

		case zoomMAP:
			#include "key_MAP.h"
			break;


		case zoomSTEP:
			key_exec_STEP( keyNdx );
			break;


		case zoomPLAY:
			// ..not relevant right now..
			break;


	} // switch G_zoom_level

	// Update HUI with changes
	Page_requestRefresh();
}


