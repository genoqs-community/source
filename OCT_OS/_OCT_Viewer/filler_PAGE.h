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
	// P A G E  MODE
	//

	// Base functionality of the page mode
	#include "fill_PAGE.h"

	// NO SELECTION.
	if (	(target_page->trackSelection == 0)
		&& 	(target_page->stepSelection  == 0)
		){

		// The PAGE contains no active selection
		#include "fill_PAGE_sel_NONE.h"
	}

	// TRACK SELECTION
	else if ( target_page->trackSelection != 0 ){

		#include "fill_PAGE_sel_TRACK.h"
	}

	// STEP SELECTION
	else if ( target_page->stepSelection != 0 ){

		#include "fill_PAGE_sel_STEP.h"
	}

	row = target_page->pageNdx % 10;

	// TRACK SELECTORS
	if (EDIT_TIMER == ON) {

		// Show the current track selection
		if (target_page->trackSelection != 0) {

			show( ELE_TRACK_SELECTORS, TRACK_SELECTION );
		}
	}
	else {
		// Default mode
		if (target_page->trackSelection != 0) {

			show( ELE_TRACK_SELECTORS, TRACK_SELECTION );
		}

		// Step selection existing
		else if ( target_page->stepSelection != 0 ){

			// Show the available and selected step attribute
			show( ELE_TRACK_SELECTORS, STEP_ATTRIBUTES_ALL );
			show( ELE_TRACK_SELECTORS, STEP_ATTRIBUTE_SELECTION );
		}

		// No selection active
		else {

			if ( 	( target_page->mixTarget == MIXTGT_ATR )
				&&	( is_pressed_key( KEY_MIX_MASTER ) )
				){

				if ( target_page->OPS_mode != BIRDSEYE ){
					#ifdef FEATURE_STEP_SHIFT
					show( ELE_TRACK_SELECTORS, MIX_ATTRIBUTE );
					// SHIFT_SKIPS
					if (  (  target_page->mixAttribute == ATTR_POSITION )
							&& ( G_MixShiftSkips == 1 )  )   {

						MIR_write_dot( target_page->mixAttribute,	MIR_SHINE_RED);

					}
					#else
					show( ELE_TRACK_SELECTORS, MIX_ATTRIBUTE );
					#endif
				}
			}

			// Show the track EFF Status
			show( ELE_TRACK_SELECTORS, TRACK_EFF_STATUS );

			// RECORD
			if ( CHECK_BIT( G_on_the_measure_operation[row], OPERATION_RECORD) ) {
				MIR_write_dot( LED_RECORD, MIR_SHINE_RED );
			} else {
				// Global track record mode is red.
				if ( G_track_rec_bit == ON ) {
					MIR_write_dot( LED_RECORD, MIR_RED );
				}
				// Local page record mode is orange.
				else if ( target_page->REC_bit == ON ) {
					MIR_write_dot( LED_RECORD, MIR_GREEN );
					MIR_write_dot( LED_RECORD, MIR_RED );
				}
			}
			// Blink when no ON notes are pending.. and not in BIRDSEYE
			if (	( NOTE_queue == NULL )
				&&	( target_page->OPS_mode != BIRDSEYE )
				){
				MIR_write_dot( LED_RECORD, MIR_BLINK );
			}

			// Show the REC status of tracks
			show( ELE_TRACK_SELECTORS, TRACK_REC_STATUS );
		}
	}

	if ( G_midi_map_controller_mode == OFF ){

		MIR_write_dot (LED_ZOOM_MAP, 		MIR_RED);
		MIR_write_dot( LED_ZOOM_MAP, 		MIR_BLINK );
	}




