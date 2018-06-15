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
	// GRID  M I X  MODE
	//
	if ( GRID_play_mode == GRID_MIX ){


		// Show the tempo as temporary value in the top row
		if ( TEMPO_TIMER == ON ){
			// Write the tempo numeral in row 0
			MIR_write_tempo( G_master_tempo, 0 );
		}
		else{
			// MATRIX
			show( ELE_MATRIX, GRID );
		}

		// MIX INDICATOR - LED corresponding to the rotarys - all active..
		show( ELE_MIX_INDICATOR, RED   );
		show( ELE_MIX_INDICATOR, GREEN );

		// TRACK SELECTORS
		show ( ELE_TRACK_SELECTORS, GRID_BANK_PLAYMODES );


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


		// TRACK MUTATORS
		// Default: mutepattern of grid banks
		// Caution!! There is also some extra code in the ELE_MATRIX / GRID section.
		show( ELE_TRACK_MUTATORS, GRID_SET_SWITCHMODE );

		// Show cursor Marcel style
		if ( is_pressed_key( KEY_ZOOM_PAGE ) ){

			MIR_write_dot( KEY_ZOOM_PAGE, MIR_GREEN );

			// Show the GRID cursor
			if( !row_in_page_window( GRID_CURSOR % 10 ) )
				break;
			temp = cursor_to_dot( GRID_CURSOR ) - shiftPageRow;

			MIR_write_dot( temp, MIR_RED   );
			MIR_write_dot( temp, MIR_GREEN );
			MIR_write_dot( temp, MIR_BLINK );
		}

		// MIX TARGET INDICATOR
		show( ELE_MIXTGT, GRID_assistant_page->mixTarget );


	} // // GRID_playmode == GRID_MIX










	//
	// GRID  E D I T  MODE
	//
	else if ( GRID_play_mode == GRID_EDIT ){

		// MATRIX
		// Show the tempo as temporary value in the top row
		if ( TEMPO_TIMER == ON ){
			// Write the tempo numeral in row 0
			MIR_write_tempo( G_master_tempo, 0 );
		}

		// Some page attribute is being edited
		else if ( EDIT_TIMER == ON ){

			switch( Page_repository[GRID_CURSOR].mixingTrack ){

				case NEMO_ATTR_PITCH:
					MIR_write_pitch_H	( 	target_page->attr_PIT, 0 );
					break;

				case NEMO_ATTR_VELOCITY:
					MIR_fill_numeric 	(	1, target_page->attr_VEL,	0,	MIR_GREEN);
					MIR_fill_numeric 	(	1, target_page->attr_VEL,	0,	MIR_RED);
					break;

				case NEMO_ATTR_LENGTH:
					// Show the page length in hex really - resolution of 16 steps..
					MIR_fill_numeric	( 	1, (Page_repository[GRID_CURSOR].attr_LEN/16), 	0, MIR_RED );
					MIR_point_numeric	( 	Page_repository[GRID_CURSOR].attr_LEN%16, 		0, MIR_GREEN );
					break;

				case NEMO_ATTR_START:
					// Show the repeats of the page under cursor.
					MIR_fill_numeric	( 	1, Page_repository[GRID_CURSOR].repeats_left, 0, MIR_GREEN );
					MIR_fill_numeric	( 	Page_repository[GRID_CURSOR].repeats_left,
										 	Page_repository[GRID_CURSOR].attr_STA + 1
										 	- Page_repository[GRID_CURSOR].repeats_left, 0, MIR_RED );
					break;
			}
		}

		else{

			show( ELE_MATRIX, GRID );
		}


		// TRACK MUTATORS
		show (ELE_TRACK_MUTATORS, PAGE_FUNCTIONS);
		// Caution!! There is some extra code in the ELE_MATRIX / GRID section.

		// EDIT INDICATOR
		show( ELE_EDIT_INDICATOR, RED 	);
		show( ELE_EDIT_INDICATOR, GREEN );


		// SYSEX DUMP INDICATOR - EXPORT CONTENTS (EXC) during stop mode
		if ( G_run_bit == OFF ){

			MIR_write_dot( LED_ALIGN, MIR_RED );
			MIR_write_dot( LED_ALIGN, MIR_GREEN );
		}

		// Show that the page may be saved - when the machine is stopped
		if ( G_run_bit == OFF ){
			MIR_write_dot( LED_PROGRAM, MIR_RED 	);
			MIR_write_dot( LED_PROGRAM, MIR_BLINK 	);
		}

	} // GRID_play_mode == GRID_EDIT






	//
	// GRID  C C M A P  MODE
	//
	else if ( GRID_play_mode == GRID_CCMAP ){

		// Use the elements of the page editing view..


		// Operate on the GRID assistant page
		target_page = GRID_assistant_page;

		//
		// MIX INDICATOR - LED corresponding to the rotarys
		//
		show( ELE_MIX_INDICATOR, RED   );
		show( ELE_MIX_INDICATOR, GREEN );

		// MIX MASTER
		switch (target_page->mixMode) {
			case INC:
				show (ELE_MIX_MASTER, GREEN);
				break;

			case DEC:
				show (ELE_MIX_MASTER, RED);
				break;
		}

		// TRACK_SELECTORS
		switch( GRID_assistant_page->mixTarget ){

			case MIXTGT_USR1:
			case MIXTGT_USR2:
			case MIXTGT_USR3:
			case MIXTGT_USR4:
			case MIXTGT_USR5:
				// Show the available attributes of the CC_MIXMAP
				show( ELE_TRACK_SELECTORS, CC_MIXMAP_ATTRIBUTES );
				break;
		}

		// MIX TARGET field
		show( ELE_MIXTGT, OPTIONS );

		// MATRIX
		// Show the tempo as temporary value in the top row
		if ( TEMPO_TIMER == ON ){
			// Write the tempo numeral in row 0
			MIR_write_tempo( G_master_tempo, 0 );
		}
		else{
			show( ELE_MATRIX, GRID );
		}

		// RETURN
		show (ELE_RETURN, GREEN);
	}




	// GLOBALS
	//

	// Not when we are looking at the clock status
	if ( !is_pressed_key( KEY_STOP ) ){
		show (ELE_GLOBALS, TRANSPORT);
	}

	// In edit mode this is showing the page VEL
	if ( GRID_play_mode != GRID_EDIT ){
		show (ELE_GLOBALS, BIG_KNOB);
	}


	show( ELE_GLOBALS, CLOCK );



	// LED_ZOOM indicates the GRID play mode
	switch( GRID_play_mode ){

		case GRID_EDIT:
			MIR_write_dot( LED_ZOOM, MIR_RED );
			MIR_write_dot( LED_ZOOM, MIR_GREEN 	);
			break;

		case GRID_MIX:
			// Nothing to do
			break;
	}



	// Blink the red program button when ready to save machines state
	// Blink the green EXC button when ready to sysex dump memory content
	if ( 	( G_run_bit == OFF )
		&&	( MODE_OBJECT_SELECTION == BIRDSEYE )
		){

		// PROGRAM
		MIR_write_dot( LED_PROGRAM, MIR_RED   );
		MIR_write_dot( LED_PROGRAM, MIR_BLINK );

		// EXC - Export Content
		MIR_write_dot( LED_ALIGN, MIR_GREEN );
		MIR_write_dot( LED_ALIGN, MIR_BLINK );
	}



	// PLAY mode
	switch( PLAY_MODE_STATUS ){

		case ON:
			MIR_write_dot( LED_ZOOM_PLAY, MIR_RED );
			MIR_write_dot( LED_ZOOM_PLAY, MIR_GREEN );
			MIR_write_dot( LED_ZOOM_PLAY, MIR_BLINK );
			break;

		case OFF:
			MIR_write_dot( LED_ZOOM_PLAY, MIR_GREEN );
			break;
	}


	// REC bit status
	if ( GRID_assistant_page->REC_bit == ON ){

		MIR_write_dot( LED_RECORD, MIR_RED 	 );
		MIR_write_dot( LED_RECORD, MIR_GREEN );
		MIR_write_dot( LED_RECORD, MIR_BLINK );
	}


	// Show the ESC button -> taking you back to the page
	MIR_write_dot( LED_RETURN, MIR_GREEN );


	// EDIT MASTER
	switch( CHECK_BIT( GRID_editmode, 0 ) ){

		case ON:
			show( ELE_EDIT_MASTER, RED   );
			if( !CHECK_BIT( GRID_editmode, 1 ) ) {
				show( ELE_EDIT_MASTER, GREEN );
			}
			MIR_write_dot( LED_EDIT_MASTER, MIR_BLINK );
			break;

		case OFF:
			if( !CHECK_BIT( GRID_editmode, 1 ) ) {
				show( ELE_EDIT_MASTER, GREEN );
			}else {
				show( ELE_EDIT_MASTER, RED );
			}
			break;
	}

	// SCALE button: green (available) or red (engaged)
	show_forcetoscale_status( GRID_assistant_page );

