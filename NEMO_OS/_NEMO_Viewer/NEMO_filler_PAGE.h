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
	#include "NEMO_fill_PAGE.h"


	// TRACK SELECTION on the fly
	switch ( my_bit_cardinality( target_page->trackSelection )) {

		// No track selection
		case 0:
			// Check for step selection
			if ( target_page->stepSelection  == 0 ){
		#include "NEMO_fill_PAGE_sel_NONE.h"
			}
			else {
		#include "NEMO_fill_PAGE_sel_STEP.h"
			}
			break;


		// Multiple tracks selected
		default:
			#include "NEMO_fill_PAGE_sel_TRACK.h"
			break;
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
			// --not applicable for NEMO
//			show( ELE_TRACK_SELECTORS, STEP_ATTRIBUTES_ALL );
//			show( ELE_TRACK_SELECTORS, STEP_ATTRIBUTE_SELECTION );

			// When in preview mode also show TGL, CPY and PST for preview step
			if (	( 	( target_page->editorMode == PREVIEW )
					||	( target_page->editorMode == PREVIEW_PERFORM )
					)
				&&	( page_preview_step != NULL )
				){

		 		MIR_write_dot( LED_TGGL, MIR_GREEN );
		 		MIR_write_dot( LED_COPY, MIR_GREEN );

		 		if (STEP_COPY_BUFFER != NULL ){
		 			MIR_write_dot( LED_PASTE, MIR_GREEN );
		 		}
			}

		}

		// No selection active
		else {

			if ( 	( target_page->mixTarget == MIXTGT_ATR )
				&&	( is_pressed_key( KEY_MIX_MASTER ) )
				){

				if ( target_page->OPS_mode != BIRDSEYE ){

					// The Octopus way..
					// show( ELE_TRACK_SELECTORS, MIX_ATTRIBUTE );

					// The NEMO way..
					// All attributes are available
					MIR_write_trackpattern( 0x0000, NEMO_ROW_I, MIR_GREEN );
					MIR_write_trackpattern( 0xF758, NEMO_ROW_II, MIR_GREEN );
					MIR_write_trackpattern( 0x0000, NEMO_ROW_III, MIR_GREEN );
					MIR_write_trackpattern( 0x0000, NEMO_ROW_IV, MIR_GREEN );

					// Blink the one assigned/selected - find out j
					switch( target_page->mixAttribute ){
						case NEMO_ATTR_PITCH:		j = 0;		break;
						case NEMO_ATTR_VELOCITY:	j = 1;		break;
						case NEMO_ATTR_LENGTH:		j = 2;		break;
						case NEMO_ATTR_START:		j = 3;		break;
						case NEMO_ATTR_AMOUNT:		j = 5;		break;
						case NEMO_ATTR_GROOVE:		j = 6;		break;
						case NEMO_ATTR_MIDICC:		j = 7;		break;
						case NEMO_ATTR_MIDICH:		j = 9;		break;
						case NEMO_ATTR_POSITION:	j = 11;		break;
						case NEMO_ATTR_DIRECTION:	j = 12;		break;
					}

					// Show selected attribute - one only
					MIR_write_trackpattern( 1 << (15 - j ), 	NEMO_ROW_II, MIR_RED );
					MIR_write_trackpattern( 1 << (15 - j ), 	NEMO_ROW_II, MIR_BLINK );
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

			// SCALE button: green (locked to own scale) or red (engaged GRID scale)
			// Inverse logic from what you see in Octopus for example..
			switch( target_page->force_to_scale ){

				case ON:
					MIR_write_dot( LED_SCALE_SEL, MIR_GREEN );
					break;

				case OFF:
					MIR_write_dot( LED_SCALE_SEL, MIR_RED );
					break;
			}
		}
	}




