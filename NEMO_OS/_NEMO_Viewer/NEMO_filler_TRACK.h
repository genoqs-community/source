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


	// Establish the index of the current row
	row = my_bit2ndx( target_page->trackSelection );

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


	// SHOW THE CHAIN PLAY HEAD status in the MIX MASTER
	// Make it dependant on whether the track is head in its chain??
	show( ELE_CHAIN_INDICATOR, ON );

	//
	// T R A C K  MODE
	//

	// MIX INDICATOR - LED corresponding to the rotarys
	// - The rotarys select the track to be edited in TRACK mode
	show( ELE_MIX_INDICATOR, RED   );
	show( ELE_MIX_INDICATOR, GREEN );


	// TRACK Selectors
	switch( MODE_OBJECT_SELECTION ){

		// Select change track selection
		case BIRDSEYE:
			// Blink the currently selected track
			i = my_bit2ndx( target_page->trackSelection ) + 1;
			MIR_write_dot( i, MIR_RED   );
			MIR_write_dot( i, MIR_GREEN );
			MIR_write_dot( i, MIR_BLINK );

			// MATRIX
			// Requested by Tom - to see the step status in birdsview
			show( ELE_MATRIX, STEP_TOGGLE );

			break;

		default:
			// Blink the currently selected track
			i = my_bit2ndx( target_page->trackSelection ) + 1;
			MIR_write_dot( i, MIR_RED   );
			MIR_write_dot( i, MIR_GREEN );
			MIR_write_dot( i, MIR_BLINK );

			// MATRIX
			// Per row hardwired track attribute values
			show( ELE_MATRIX, HARDWIRED_ATTRIBUTE_VALUES );

			// If track is not muted show lauflicht
			for (i=0; i<MATRIX_NROF_ROWS; i++) {
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
	// Show the functions Remix and Clear
	MIR_write_dot( LED_RMX, MIR_RED   );
	MIR_write_dot( LED_RMX, MIR_GREEN );
	MIR_write_dot( LED_CLEAR, MIR_RED   );
	MIR_write_dot( LED_CLEAR, MIR_GREEN );

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



	// EDIT MASTER
	show ( ELE_EDIT_INDICATOR, GREEN );
	show ( ELE_EDIT_INDICATOR, RED );


	// MUT MASTER
	// If the track is muted, blink green, otherwise light orange
	if ((target_page->trackSolopattern & ( 1 << row )) != 0 ){
		MIR_write_dot( LED_MUTE_MASTER, MIR_GREEN 	);
		MIR_write_dot( LED_MUTE_MASTER, MIR_BLINK 	);
	}
	else{
		MIR_write_dot( LED_MUTE_MASTER, MIR_GREEN 	);
		MIR_write_dot( LED_MUTE_MASTER, MIR_RED 	);
	}


	// GLOBALS
	show ( ELE_GLOBALS, TRANSPORT );

	show ( ELE_GLOBALS, FOLLOW );
	show ( ELE_GLOBALS, CLOCK );


	// RECORDING LED
	if ( Page_getTrackRecPattern(target_page) == target_page->trackSelection ) {
		MIR_write_dot( LED_RECORD, MIR_RED 	 );
	}
	else if ( Page_getTrackRecPattern(target_page) ) {
		MIR_write_dot( LED_RECORD, MIR_GREEN );
	}
	if ( G_track_rec_bit == ON ) {
		if ( NOTE_queue == NULL ) {
			MIR_write_dot( LED_RECORD, MIR_BLINK );
		}
	}
