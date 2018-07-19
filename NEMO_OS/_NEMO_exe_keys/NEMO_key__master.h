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



/* NEMO
 *
 * Interpret incoming key presses
 * genoQs Machines, 07.11.2004
 */

extern void CheckAndShowBadThreads( unsigned char flag );
extern void key_ScaleSelector_mini_functions( unsigned int keyNdx, Pagestruct* target_page );
extern void key_ScaleSelector_functions( unsigned int keyNdx, Pagestruct* target_page );
extern void my_sysex_dump( unsigned char dump_type, unsigned char my_grid_cursor, int objectIx, int objectCt );

// General functions used by other key exec functions
#include "../_NEMO_exe_keys/NEMO_key_functions.h"
#include "../_NEMO_exe_keys/NEMO_key_STEP.h"
#include "../_NEMO_exe_keys/NEMO_key_PAGE.h"
#include "../_NEMO_exe_keys/NEMO_key_OCT_CIRCLE_xpose_STEP.h"
#include "../_NEMO_exe_keys/NEMO_key_OCT_CIRCLE_chord_STEP.h"
#include "../_NEMO_exe_keys/NEMO_key_ScaleSelector_functions.h"
#ifdef FEATURE_ENABLE_DICE
#include "../_NEMO_exe_keys/NEMO_key_DICE.h"
#endif
#include HEADER(_OCT_exe_keys/key_ScaleSelector_mini_functions.h)
#include HEADER(_OCT_exe_keys/key_mixermap_matrixclick.h)
#include HEADER(_OCT_exe_keys/key_exe_chainselectors.h)
// #include HEADER(_OCT_exe_keys/key_ABLETON.h)


// EXECUTE THE KEYPRESS RECEIVED
void executeKey( unsigned int keyNdx ){

	unsigned int  	i=0,
					j=0,
					k=0,
//					m=0,
					row=0,
					col=0,
					temp=0
					;


	// Work on the page under the grid cursor
	Pagestruct* target_page = &Page_repository[ GRID_CURSOR ];

	// x2 - Track row index shift
	unsigned char shiftPageRow = page_get_window_shift();

	// x2 - Track row window shift
	unsigned char shiftTrackRow = track_get_window_shift(target_page);

	// Used to mute a whole chain
//	Trackstruct* current_track = NULL;

	// Page pointer holder
	Pagestruct* temp_page = NULL;
	static Pagestruct* previous_page = NULL;
	static Pagestruct* prev_previous_page = NULL; // used to validate page cluster selections
	static unsigned char prev_previous_page_clear = ON;

	// Used in the key entry routine for the mixer maps modes
	unsigned char* target_ptr = NULL;



	// This is the ESC key - decide here what to do.
	// The statement should also incorporate the spaghetti block from above.
	if ( keyNdx == KEY_RETURN ){

		switch( G_zoom_level ){
			#ifdef FEATURE_ENABLE_DICE
			case zoomDICE:
				G_zoom_level = zoomGRID;
				break;
			#endif
			case zoomGRID:
				// Enter machine lock state
				if ( is_pressed_key( KEY_ZOOM_GRID ) ){
					G_zoom_level = zoomAMANDA;

					// Show thread status
//					CheckAndShowBadThreads( TRUE );
				}
				else{
					if ( GRID_CURSOR == GRID_assistant_page->pageNdx ){
						GRID_CURSOR = G_last_cursor;
					}
					G_zoom_level = zoomPAGE;
				}
				break;

			case zoomPAGE:
				// Disable possible selections of steps or tracks
				// Active step selection in page - export it!
				if (target_page->stepSelection != 0){

					// Remember the step selection pattern in page and void it for now
					export_stepSELpattern( target_page );
					return;
				}
				// Clear trackSelection if exists
				else if (target_page->trackSelection != 0) {

					// Store the track selection
					target_page->trackSelectionStored = target_page->trackSelection;

					// Clear the track selection
					target_page->trackSelection = 0;

					// Enter interactive mode
					target_page->OPS_mode = PASSIVE;

					// Clear the SEL lock for track selection
					target_page->SEL_LOCK = OFF;
				}
				else if ( target_page->OPS_mode == BIRDSEYE ){
					// Load page from flash -- this should be handled in BIRDSEYE..
					Flash_read_page( GRID_CURSOR );

					// Enter interactive mode
					target_page->OPS_mode = PASSIVE;
				}

				break;

			case zoomMIXMAP:
				if ( GRID_CURSOR == GRID_assistant_page->pageNdx ){
					GRID_CURSOR = G_last_cursor;
				}
				G_zoom_level = zoomPAGE;
				break;

			case zoomTRACK:
			case zoomSTEP:
				target_page->trackSelection = OFF;
				target_page->trackAttributeSelection = OFF;

				// Un-select all steps in page, if any were selected
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
				G_zoom_level = zoomPAGE;
				break;

			case zoomSCALE:
				// Exit the scale edit state
				GRID_assistant_page->scaleStatus = OFF;
				GRID_assistant_page->force_to_scale = OFF;
				G_zoom_level = zoomGRID;
				break;

			case zoomSYSEX:
			case zoomGRIDTRACK:
				// Return to GRID mode
				G_zoom_level = zoomGRID;
				break;
		}

		// Need ESC still in DIAG - to turn on the LED. But not otherwise.
		if ( G_zoom_level != zoomDIAG ){
			// No more place to go from here!
			return;
		}
	}





	// Mode bound key interpretation. Should all be like STEP.. using function calls.
	//
	switch (G_zoom_level) {

		case zoomSYSEX:
			// x2 - toggle page row window from (1-4) (5-8)
			if ( keyNdx == KEY_ZOOM_STEP ) {
				page_shift_window();
				break;
			}
			keyNdx = shift_key_page_row( keyNdx, shiftPageRow );
			#include "NEMO_key_SYSEX.h"
			break;

/*		case zoomABLETON:
			key_exec_ABLETON( keyNdx );
			break;
*/
		case zoomAMANDA:
			#include HEADER(_OCT_exe_keys/key_AMANDA.h)
			break;

		case zoomDIAG:
			#include "NEMO_key_DIAG.h"
			break;

		#ifdef FEATURE_ENABLE_DICE
		case zoomDICE:
			if( ( keyNdx < 11 ) || ( keyNdx > 185 ) )
			{
				// Toggle page row window from (1-4) (5-8)
				if ( keyNdx == KEY_ZOOM_STEP ) {
					page_shift_window();
				}
				keyNdx = shift_key_page_row( keyNdx, shiftPageRow );
			}
			key_exec_DICE( keyNdx );
			break;
		#endif

		case zoomGRID:
			// x2 - Shift page row window from (1-4) (5-8)
			if ( keyNdx == KEY_ZOOM_STEP ) {
				page_shift_window();
			}
			keyNdx = shift_key_page_row( keyNdx, shiftPageRow );

			#include "NEMO_key_GRID.h"
			break;

		case zoomGRIDTRACK:
			// x2 - Shift page row window from (1-4) (5-8)
			if ( keyNdx == KEY_ZOOM_STEP ) {
				page_shift_window();
			}
			keyNdx = shift_key_page_row( keyNdx, shiftPageRow );

			#include "NEMO_key_GRIDTRACK.h"
			break;

		case zoomDEVICE:
			#include "NEMO_key_DEVICE.h"
			break;

		case zoomPAGE:
			// x2 - Shift track row window from (1-4) (5-8)
			if ( keyNdx == KEY_ZOOM_STEP ) {
				if( ( target_page->OPS_mode == BIRDSEYE ) ) {
					page_shift_window();
				}else if(	( is_pressed_key( KEY_SELECT_MASTER ) )
					||	( !( ( G_run_bit == ON )
					&&	( target_page->trackSelection != 0 ) )
					&&	( !( ( ( target_page->editorMode == PREVIEW )
					||	( target_page->editorMode == PREVIEW_PERFORM ) )
					&& 	is_pressed_steprange() ) ) ) ) {
					track_shift_window( target_page );
				}
			}

			keyNdx = shift_key_track_row( target_page, keyNdx, shiftTrackRow );

			key_exec_PAGE( keyNdx );
			break;

		case zoomMIXMAP:
			// x2 - Shift track row window from (1-4) (5-8)
			if( ( target_page->mixTarget == MIXTGT_USR1 )
			|| 	( target_page->mixTarget == MIXTGT_USR2 )
			|| 	( target_page->mixTarget == MIXTGT_USR3 )
			|| 	( target_page->mixTarget == MIXTGT_USR4 ) ) {

				shiftTrackRow = track_get_window_shift( GRID_assistant_page );
				if ( ( keyNdx == KEY_ZOOM_STEP ) ) {
					track_shift_window( GRID_assistant_page );
				}

				if ( !( ( keyNdx > 0 && keyNdx <= 20 )
				|| ( keyNdx >= 187 && keyNdx <= 195 ) )
				&& ( !is_pressed_key( KEY_MIX_MASTER ) ) ) {
					keyNdx = shift_key_track_row( GRID_assistant_page, keyNdx, shiftTrackRow );
				}
			}else {
				if ( ( keyNdx == KEY_ZOOM_STEP ) ) {
					track_shift_window( target_page );
				}

				if ( !( ( keyNdx > 0 && keyNdx <= 20 )
				|| ( keyNdx >= 187 && keyNdx <= 195 ) )
				&& ( !is_pressed_key( KEY_MIX_MASTER ) ) ) {
					keyNdx = shift_key_track_row( target_page, keyNdx, shiftTrackRow );
				}
			}

			#include "NEMO_key_MIXMAP.h"
			break;

		case zoomTRACK:
			// x2 - Shift track row window from (1-4) (5-8)
			if	( ( keyNdx == KEY_ZOOM_STEP )
				&&	( target_page->stepSelection != 1 ) ) {
				track_shift_window( target_page );
				track_shift_window_track_selection( target_page );
			}

			if ( ( keyNdx >0 ) && ( keyNdx <= 10 ) ) {
				keyNdx = shift_key_track_row( target_page, keyNdx, shiftTrackRow );
			}
			#include "NEMO_key_TRACK.h"
			break;

		case zoomMAP:
			// Doesn't apply to NEMO
			// #include "../_OCT_exe_keys/key_MAP.h"
			break;


		case zoomSTEP:
			if( MODE_OBJECT_SELECTION == BIRDSEYE ){
				keyNdx = shift_key_track_row( target_page, keyNdx, shiftTrackRow );
			}

			key_exec_STEP( keyNdx );
			break;


		case zoomPLAY:
			// ..not relevant right now..
			break;

		case zoomSCALE:
			#include "NEMO_key_SCALE.h"
			break;

	} // switch G_zoom_level

	// Update HUI with changes
	Page_requestRefresh();
}


