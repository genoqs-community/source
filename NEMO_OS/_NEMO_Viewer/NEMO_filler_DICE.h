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
	// D I C E MODE
	//
	// GRID mode available for selection
	MIR_write_dot (LED_ZOOM_GRID,	MIR_GREEN);


	// MIX INDICATOR - LED corresponding to the rotarys - all active..
	show( ELE_MIX_INDICATOR, RED   );
	show( ELE_MIX_INDICATOR, GREEN );

	// Show the MIDI Clock state
	if ( is_pressed_key( KEY_STOP ) ){

		switch( G_clock_source ){

			case OFF:
				// Light the options green
				MIR_write_dot( LED_PLAY1, 	MIR_GREEN );
				MIR_write_dot( LED_PAUSE, 	MIR_GREEN );
				break;

			case EXT:
				MIR_write_dot( LED_PAUSE,	MIR_GREEN );
				MIR_write_dot( LED_PAUSE,	MIR_RED );
				MIR_write_dot( LED_PAUSE, 	MIR_BLINK );

				MIR_write_dot( LED_PLAY1, 	MIR_GREEN );
				break;

			case INT:
				MIR_write_dot( LED_PLAY1,	MIR_GREEN );
				MIR_write_dot( LED_PLAY1,	MIR_RED );
				MIR_write_dot( LED_PLAY1, 	MIR_BLINK );

				MIR_write_dot( LED_PAUSE, 	MIR_GREEN );
				break;
		}
	}

	// GLOBALS
	//
	show (ELE_GLOBALS, BIG_KNOB);
	show( ELE_GLOBALS, CLOCK );

	show (ELE_GLOBALS, TRANSPORT);

	MIR_write_dot( LED_ZOOM, MIR_RED );
	MIR_write_dot( LED_ZOOM, MIR_BLINK 	);

	// TRACK SELECTORS
	if (EDIT_TIMER == ON) {

		// Show the current track selection
		if (DICE_bank->trackSelection != 0) {

			show( ELE_TRACK_SELECTORS, DICE_GRID_SELECTION );
		}
	}
	else {
		// Default mode
		show( ELE_TRACK_SELECTORS, DICE_GRID_SELECTION );
	}

	// SELECT MASTER
	// Indicate ongoing mute activity
	if ( DICE_bank->trackSelection != 0 ){
		show( ELE_SELECT_MASTER, GREEN );
	}
	// Indicate existing stored select pattern
	else if (DICE_bank->trackSelectionStored != 0) {
		show (ELE_SELECT_MASTER, RED);
	}


	// MIX TARGET INDICATOR
	show( ELE_MIXTGT, GRID_assistant_page->mixTarget );

	// TRACK MUTATORS
	// Default: mutepattern of grid banks
	// Caution!! There is also some extra code in the ELE_MATRIX / GRID section.
	show( ELE_TRACK_MUTATORS, GRID_SET_SWITCHMODE );

	// MUTE MASTER
	// Indicate ongoing mute activity
	if ( GRID_mutepattern != 0 ){

		show( ELE_MUTE_MASTER, GREEN );
	}
	// Indicate existing stored mute pattern
	else if (DICE_bank->trackMutepatternStored != 0) {
		show (ELE_MUTE_MASTER, RED);
	}

	// MATRIX
	// Per row hardwired track attribute values flow shapes
	show( ELE_MATRIX, HARDWIRED_ATTRIBUTE_VALUES );

	// MATRIX
	show( ELE_MATRIX, DICE );

	// EDIT INDICATOR
	show( ELE_EDIT_INDICATOR, RED 	);
	show( ELE_EDIT_INDICATOR, GREEN );

	if (EDIT_TIMER != OFF){
		show( ELE_EDIT_INDICATOR, BLINK );
	}

	// Dice bank selected indicator.
	unsigned char j = 0;
	switch( DICE_bank->mixTarget ) {
		case 0:
			j = LED_MCC_STREAM1;
			break;
		case 1:
			j = LED_MCC_STREAM2;
			break;
		case 2:
			j = LED_MCC_STREAM3;
			break;
		case 3:
			j = LED_MCC_STREAM4;
			break;
	}
	// MIX MASTER
	// Dice chain follow mode off / on
	if( Dice_get_MISC( target_dice, DICE_CHAIN_FOLLOW ) ) {
		MIR_write_dot( LED_MIX_MASTER, MIR_RED );
	}
	MIR_write_dot( LED_MIX_MASTER, MIR_GREEN );

	// EDIT MASTER
	// Show activation status: all are active
	if( CHECK_BIT( DICE_bank->editorMode, DICE_QUANT ) ) {
		show( ELE_EDIT_MASTER, GREEN );
		show( ELE_EDIT_MASTER, BLINK );
	} else {
		show( ELE_EDIT_MASTER, GREEN );
		show( ELE_EDIT_MASTER, RED );
	}

	show( ELE_EDIT_MASTER, BLINK );

	// Show selection
	MIR_write_dot ( j, MIR_RED );
	MIR_write_dot ( j, MIR_GREEN );

	if( is_pressed_key( KEY_MCC_STREAM1 ) || is_pressed_key( KEY_MCC_STREAM2 ) ||
		is_pressed_key( KEY_MCC_STREAM3 ) || is_pressed_key( KEY_MCC_STREAM4 )	) {
		// Show that the page may be saved - when the machine is stopped
		if ( G_run_bit == OFF ){
			MIR_write_dot( LED_PROGRAM, MIR_RED 	);
			MIR_write_dot( LED_PROGRAM, MIR_BLINK 	);
		}

		// Dice bank selector held clone mode.
		if( is_pressed_key( KEY_MCC_STREAM1 ) ) {
			MIR_write_dot( LED_MCC_STREAM1, MIR_GREEN );
			MIR_write_dot( LED_MCC_STREAM2, MIR_BLINK );
			MIR_write_dot( LED_MCC_STREAM3, MIR_BLINK );
			MIR_write_dot( LED_MCC_STREAM4, MIR_BLINK );
			MIR_write_dot( LED_MCC_STREAM2, MIR_RED );
			MIR_write_dot( LED_MCC_STREAM3, MIR_RED );
			MIR_write_dot( LED_MCC_STREAM4, MIR_RED );
		}

		if( is_pressed_key( KEY_MCC_STREAM2 ) ) {
			MIR_write_dot( LED_MCC_STREAM2, MIR_GREEN );
			MIR_write_dot( LED_MCC_STREAM1, MIR_BLINK );
			MIR_write_dot( LED_MCC_STREAM3, MIR_BLINK );
			MIR_write_dot( LED_MCC_STREAM4, MIR_BLINK );
			MIR_write_dot( LED_MCC_STREAM1, MIR_RED );
			MIR_write_dot( LED_MCC_STREAM3, MIR_RED );
			MIR_write_dot( LED_MCC_STREAM4, MIR_RED );
		}

		if( is_pressed_key( KEY_MCC_STREAM3 ) ){
			MIR_write_dot( LED_MCC_STREAM3, MIR_GREEN );
			MIR_write_dot( LED_MCC_STREAM1, MIR_BLINK );
			MIR_write_dot( LED_MCC_STREAM2, MIR_BLINK );
			MIR_write_dot( LED_MCC_STREAM4, MIR_BLINK );
			MIR_write_dot( LED_MCC_STREAM1, MIR_RED );
			MIR_write_dot( LED_MCC_STREAM2, MIR_RED );
			MIR_write_dot( LED_MCC_STREAM4, MIR_RED );
		}

		if( is_pressed_key( KEY_MCC_STREAM4 ) ){
			MIR_write_dot( LED_MCC_STREAM4, MIR_GREEN );
			MIR_write_dot( LED_MCC_STREAM1, MIR_BLINK );
			MIR_write_dot( LED_MCC_STREAM2, MIR_BLINK );
			MIR_write_dot( LED_MCC_STREAM3, MIR_BLINK );
			MIR_write_dot( LED_MCC_STREAM1, MIR_RED );
			MIR_write_dot( LED_MCC_STREAM2, MIR_RED );
			MIR_write_dot( LED_MCC_STREAM3, MIR_RED );
		}
	}
	// Show the current step selection set
	if ( is_pressed_key( KEY_SELECT_MASTER )
		){
		switch( DICE_bank->stepSELpattern_ndx ){

			case 0:	MIR_write_dot( LED_MCC_STREAM1, MIR_GREEN 	);
					MIR_write_dot( LED_MCC_STREAM1, MIR_BLINK 	);
				break;
			case 1:	MIR_write_dot( LED_MCC_STREAM2, MIR_GREEN 	);
					MIR_write_dot( LED_MCC_STREAM2, MIR_BLINK 	);
				break;
			case 2:	MIR_write_dot( LED_MCC_STREAM3, MIR_GREEN 	);
					MIR_write_dot( LED_MCC_STREAM3, MIR_BLINK 	);
				break;
			case 3:	MIR_write_dot( LED_MCC_STREAM4, MIR_GREEN 	);
					MIR_write_dot( LED_MCC_STREAM4, MIR_BLINK 	);
				break;
		} // switch
	}

	// Show the ESC button -> taking you back to the page
	MIR_write_dot( LED_RETURN, MIR_GREEN );

	// Original cursor.
	unsigned char gridCursor = GRID_CURSOR;
	for ( i=0; i < GRID_NROF_BANKS; i++ ){
		// Display play status of 1st Dice enabled bank.
		if ( GRID_p_selection[ i ] != NULL && Dice_is_grid_row_diced( i ) ){
			// Temp override cursor.
			GRID_CURSOR = GRID_p_selection[ i ]->pageNdx;
			break;
		}
	}

	// Restore GRID_CURSOR.
	GRID_CURSOR = gridCursor;
