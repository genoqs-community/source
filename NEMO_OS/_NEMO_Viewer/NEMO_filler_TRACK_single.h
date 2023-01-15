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




	// ZOOM LEVEL -- TRACK mode selected
	MIR_write_dot (LED_ZOOM_TRACK, 		MIR_RED);
	MIR_write_dot (LED_ZOOM_TRACK, 		MIR_GREEN);
	MIR_write_dot( LED_ZOOM_TRACK, 		MIR_BLINK );

	// PAGE mode
	// Indicate whether the page is currently playing
	if ( is_selected_in_GRID( target_page ) ) {

		// Page is active in the Grid
		MIR_write_dot (LED_ZOOM_PAGE, 		MIR_GREEN);
	}
	// Page is not part of the GRID selection
	else {

		// Blink PAGE mode indicator red
		MIR_write_dot (LED_ZOOM_PAGE, 		MIR_RED);
	}

	// GRID mode available for selection
	MIR_write_dot (LED_ZOOM_GRID,	MIR_GREEN);

	// MAP mode available as well if an attribute is selected
	if (target_page->trackAttributeSelection != 0) {

		MIR_write_dot (LED_ZOOM_MAP, MIR_GREEN);
	}


	// PLAY mode indicator
	switch( PLAY_MODE_STATUS ){

		case ON:
			MIR_write_dot( LED_ZOOM_PLAY, MIR_RED );
			MIR_write_dot( LED_ZOOM_PLAY, MIR_GREEN );
			MIR_write_dot( LED_ZOOM_PLAY, MIR_BLINK );

			// Here no PROGRAM LED because the button has a different function
			break;

		case OFF:
			MIR_write_dot( LED_ZOOM_PLAY, MIR_GREEN );
			break;
	}



	//
	// T R A C K  MODE
	//

	// TRACK Selectors
	switch( MODE_OBJECT_SELECTION ){

		// Select change track selection
		case BIRDSEYE:
			i = my_bit2ndx( target_page->trackSelection ) + 1;
			if( !row_in_track_window( target_page, i - 1 ) )
				break;
			// Blink the currently selected track
			MIR_write_dot( i - shiftTrackRow, MIR_RED   );
			MIR_write_dot( i - shiftTrackRow, MIR_GREEN );
			MIR_write_dot( i - shiftTrackRow, MIR_BLINK );

			// MATRIX
			// Requested by Tom - to see the step status in birdsview
			show( ELE_MATRIX, STEP_TOGGLE );

			break;

		default:
			show ( ELE_TRACK_SELECTORS, TRACK_ATTRIBUTES_MAPS );
			show ( ELE_TRACK_SELECTORS, TRACK_ATTRIBUTE_SELECTION );

			// MATRIX
			// Per row hardwired track attribute values
			show( ELE_MATRIX, HARDWIRED_ATTRIBUTE_VALUES );

			// If track is not muted show lauflicht
			for (i=0; i<MATRIX_NROF_ROWS; i++) {
				if ( !row_in_track_window( target_page, i ) )
					continue;

				if ( (target_page->trackSelection & (1<<i)) > 0 ) {
					//  row is index of the first selected track
					row = i;
				}
			}
			break;
	}


	// Show the lauflicht in track
	show ( ELE_MATRIX, LAUFLICHT_TRACK ) ;

	// ESCAPE
	show ( ELE_RETURN, GREEN );

	// TRACK MUTATORS
	if (target_page->trackAttributeSelection == 0) {

		show( ELE_TRACK_MUTATORS, TRACK_FUNCTIONS );

		// Copy and Paste
		MIR_write_dot( LED_COPY, MIR_GREEN 	);
		MIR_write_dot( LED_COPY, MIR_RED 	);

		// PASTE: Check if buffer and target selection have the same cardinality
		if ((TRACK_COPY_BUFFER != 0) &&
			(my_bit_cardinality( TRACK_COPY_BUFFER & 0x3FF ) ==
				my_bit_cardinality( target_page->trackSelection ) )
			){

			MIR_write_dot( LED_PASTE, MIR_GREEN );
			MIR_write_dot( LED_PASTE, MIR_RED );
		}
	}
	else {

		show( ELE_TRACK_MUTATORS, TRACK_ATTRIBUTE_FUNCTIONS );
	}

	// EDIT MASTER
	show ( ELE_EDIT_INDICATOR, GREEN );
	show ( ELE_EDIT_INDICATOR, RED );

	// GLOBALS
	show ( ELE_GLOBALS, TRANSPORT );
	show ( ELE_GLOBALS, NUMERIC_QUADRANT );

	show ( ELE_GLOBALS, BIG_KNOB );
	show ( ELE_GLOBALS, FOLLOW );
	show ( ELE_GLOBALS, CLOCK );


	// TEMPOMUL VALUE for the TRACK
	display_Track_TEMPOMUL(
		target_page->Track[ my_bit2ndx( target_page->trackSelection ) ] );


	// CHORD SELECTOR
	if ( my_bit_cardinality( target_page->trackSelection ) == 1 ){

		show( ELE_CHORD_SELECTOR, CHORD_SIZE_TRACK );
	}


	// ALIGN - Indicate that ALIGN is available
	if ( target_page->scaleStatus == OFF ){

//		MIR_write_dot( LED_ALIGN, MIR_GREEN );
	}

	// OCTAVE CIRCLE - showing the track pitch, ready for trans-position
	show( ELE_OCTAVE_CIRCLE, TRACK_TRANSPOSITION );


	// RECORDING LED
	switch ( target_page->track_REC_pattern == target_page->trackSelection ){

		case TRUE:
			MIR_write_dot( LED_RECORD, MIR_RED 	 );
			// Blink when no ON notes are pending..
			if ( NOTE_queue == NULL ){
				MIR_write_dot( LED_RECORD, MIR_BLINK );
			}
			break;

		case FALSE:
			if ( target_page->track_REC_pattern != 0 ){
				MIR_write_dot( LED_RECORD, MIR_GREEN );
				// Blink when no ON notes are pending..
				if ( NOTE_queue == NULL ){
					MIR_write_dot( LED_RECORD, MIR_BLINK );
				}
			}
			else{
				MIR_write_dot( LED_RECORD, MIR_GREEN );
			}
			break;
	}

