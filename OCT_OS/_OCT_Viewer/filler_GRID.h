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


	// blink page cluster selection
	if ( GRID_p_selection_cluster == ON ) {
		page_cluster_selection( GRID_CURSOR );
	}

#ifdef FEATURE_ENABLE_SONG_UPE
	if ( G_rec_ctrl_track != NULL ){
		MIR_write_dot( LED_MUTE_MASTER, MIR_RED );
		MIR_write_dot( LED_MUTE_MASTER, MIR_GREEN );
		MIR_write_dot( LED_MUTE_MASTER, MIR_BLINK );
	}
#endif

	//
	// GRID  M I X  MODE
	//
	if ( GRID_play_mode == GRID_MIX ){


		// MIX INDICATOR - LED corresponding to the rotarys - all active..
		show( ELE_MIX_INDICATOR, RED   );
		show( ELE_MIX_INDICATOR, GREEN );

		// TRACK SELECTORS
		show ( ELE_TRACK_SELECTORS, GRID_BANK_PLAYMODES );


		// MATRIX
		show( ELE_MATRIX, GRID );



		// OCTAVE CIRCLE
		show_GLOBALS_scale_master( GRID_assistant_page );

		if (	( GRID_assistant_page->scaleStatus == SCALE_SEL )
			||	( GRID_assistant_page->scaleStatus == SCALE_MOD )
			){

			// Show the selected notes in scale. Both in MOD and SEL
			show_OCTAVE_CIRCLE_scale_selection( GRID_assistant_page );
			show_SCALE_SELECTOR_scale_selection( GRID_assistant_page );

			// Show the ESC LED - leaving the scale mode of the GRID asstn page
			MIR_write_dot( LED_RETURN, MIR_GREEN );
		}
		else if ( 	!( ( G_zoom_level == zoomGRID )
				&&	( CHECK_BIT( GRID_p_set_mode, GRID_SET_NOTE_CTRL_ENABLE ) )
				&&	( GRID_p_set_note_offsets[current_GRID_set] != 255 )
				&&	( is_pressed_key( 10 ) ) ) ) {

			// Show the packman at playtime - unless there is a scale selected (or Drum Ctrl PIT EDIT_TIMER preview)
			show ( ELE_OCTAVE_CIRCLE, G_global_locator_PICTURE );
		}


		// TRACK MUTATORS
		// Default: mutepattern of grid banks
		// Caution!! There is also some extra code in the ELE_MATRIX / GRID section.
		show( ELE_TRACK_MUTATORS, GRID_SET_SWITCHMODE );

		#ifdef FEATURE_ENABLE_SONG_UPE
		// NUMERIC QUADRANT
		if ( G_pause_bit == OFF ){
			MIR_write_numeric_C( G_master_tempo );
		}

		// Control track MIX ARMED
		if ( MIX_TRACK != NULL && CHECK_BIT(MIX_TRACK->attr_MISC, TRK_CTRL_MIX) && Track_get_MISC(MIX_TRACK, CONTROL_BIT) ){
			MIR_write_dot( LED_MIX_MASTER, MIR_RED );
			MIR_write_dot( LED_MIX_MASTER, MIR_GREEN );
			MIR_write_dot( LED_MIX_MASTER, MIR_BLINK );
		}

		// Show cursor Marcel style
		if ( is_pressed_key( KEY_ZOOM_PAGE ) ){

			MIR_write_dot( KEY_ZOOM_PAGE, MIR_GREEN );

			// Show the GRID cursor
			temp = cursor_to_dot( GRID_CURSOR );
			MIR_write_dot( temp, MIR_RED   );
			MIR_write_dot( temp, MIR_GREEN );
			MIR_write_dot( temp, MIR_BLINK );

			// Show all control track containing pages in grid
			Trackstruct* temp_track = NULL;
			for (i=0; i<MAX_NROF_TRACKS; i++) {
				temp_track = &Track_repository[i];
				if (Track_get_MISC(temp_track, CONTROL_BIT)) {
					temp = cursor_to_dot( Track_repository[i].trackId / 10 );
					MIR_write_dot( temp, MIR_BLINK   );
				}
			}

		}		
		#else
		// NUMERIC QUADRANT
		MIR_write_numeric_C( G_master_tempo );


		// Show cursor Marcel style
		if ( is_pressed_key( KEY_ZOOM_PAGE ) ){

			MIR_write_dot( KEY_ZOOM_PAGE, MIR_GREEN );

			// Show the GRID cursor
			temp = cursor_to_dot( GRID_CURSOR );
			MIR_write_dot( temp, MIR_RED   );
			MIR_write_dot( temp, MIR_GREEN );
			MIR_write_dot( temp, MIR_BLINK );
		}
		#endif

		#ifdef FEATURE_NOTE_DRUM_CTRL
		show( ELE_DRUM_CTRL, ON );
		#endif
	} // // GRID_playmode == GRID_MIX












	//
	// GRID  E D I T  MODE
	//
	else if ( GRID_play_mode == GRID_EDIT ){

		// MATRIX
		//
		show (ELE_MATRIX, GRID);

		switch( GRID_rowzero_pagelength ){

			case FALSE:
				#ifdef FEATURE_ENABLE_SONG_UPE
				// Show the repeats of the page under cursor
				{
					unsigned char posREP = (Page_repository[GRID_CURSOR].attr_STA - Page_repository[GRID_CURSOR].repeats_left) + 1;
					unsigned char numericSTA = (Page_repository[GRID_CURSOR].attr_STA/16)+1;
					unsigned char numericREP = (posREP/16)+1;
					unsigned char pointSTA = Page_repository[GRID_CURSOR].attr_STA%16;
					unsigned char pointREP = ((Page_repository[GRID_CURSOR].attr_STA - Page_repository[GRID_CURSOR].repeats_left)%16) + 1;
					if ( pointREP >= pointSTA && Page_repository[GRID_CURSOR].repeats_left == 0 ) {
						pointREP = pointREP % pointSTA;
					}

					// Show the pagelength in hex really - resolution of 16 steps..
					MIR_fill_numeric( 1, numericSTA, 9, MIR_RED );
					MIR_point_numeric( pointSTA, 9, MIR_GREEN );
					MIR_write_dot( 10, MIR_GREEN );

					if ( pointREP == pointSTA ) {
						MIR_point_numeric(pointREP, 9, MIR_GREEN );
						MIR_point_numeric(pointREP, 9, MIR_BLINK );
					} else if ( pointREP < numericSTA ) {
						MIR_point_numeric(pointREP, 9, MIR_SHINE_RED );
					} else if ( pointREP < 16 ) {
						MIR_point_numeric(pointREP, 9, MIR_SHINE_GREEN );
					}
					MIR_fill_numeric( 1, numericREP, 9, MIR_BLINK );
				}
				#else
				// Show the repeats of the page under cursor. Not in the above because
				// ..for some reason doesn't work.
				MIR_fill_numeric( 1, Page_repository[GRID_CURSOR].repeats_left, 9, MIR_GREEN );
				MIR_fill_numeric( 	 Page_repository[GRID_CURSOR].repeats_left,
									 Page_repository[GRID_CURSOR].attr_STA + 1, 9, MIR_RED );
				MIR_write_dot( 10, MIR_RED );
				#endif
				break;

			case TRUE:
				// Show the pagelength in hex really - resolution of 16 steps..
				MIR_fill_numeric( 1, (Page_repository[GRID_CURSOR].attr_LEN/16)+1, 9, MIR_RED );
				MIR_point_numeric( Page_repository[GRID_CURSOR].attr_LEN%16, 9, MIR_GREEN );
				MIR_write_dot( 10, MIR_GREEN );
				break;
		}


		// TRACK MUTATORS
		show (ELE_TRACK_MUTATORS, PAGE_FUNCTIONS);
		// Caution!! There is some extra code in the ELE_MATRIX / GRID section.

		// OCTAVE CIRCLE
		show (ELE_OCTAVE_CIRCLE, PAGE_TRANSPOSITION);

		// EDIT INDICATOR
		show( ELE_EDIT_INDICATOR, RED );

		// NUMERIC QUADRANT
		// MIR_write_numeric_C( (&Page_repository[GRID_CURSOR])->attr_VEL );
		MIR_write_factor_C( (&Page_repository[GRID_CURSOR])->attr_VEL );
		// MIR_write_numeric_C( G_master_tempo );

		// SYSEX DUMP INDICATOR - EXPORT CONTENTS (EXC) during stop mode
		if ( G_run_bit == OFF ){

			MIR_write_dot( LED_ALIGN, MIR_RED );
			MIR_write_dot( LED_ALIGN, MIR_GREEN );

			// PROGRAM (save page)
			MIR_write_dot( LED_PROGRAM, MIR_RED   );
			MIR_write_dot( LED_PROGRAM, MIR_BLINK );
		}
	} // GRID_play_mode == GRID_EDIT










	//
	// GRID  C C M A P  MODE
	//
	else if ( GRID_play_mode == GRID_CCMAP ){

		// Use the elements of the page editing view..

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

			case MIXTGT_USR0:
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
		show( ELE_MATRIX, GRID_CC_MAP_VALS );

		// RETURN
		show (ELE_RETURN, GREEN);

		// NUMERIC QUADRANT
		MIR_write_numeric_C( G_master_tempo );

	}



	// GLOBALS
	//
	show (ELE_GLOBALS, TRANSPORT);

	// In edit mode this is showing the page VEL
	if ( GRID_play_mode != GRID_EDIT ){
		show (ELE_GLOBALS, BIG_KNOB);
	}

	show( ELE_GLOBALS, CLOCK );


	// Mix target indicator
	show( ELE_MIXTGT, GRID_assistant_page->mixTarget );


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

	if ( G_midi_map_controller_mode == OFF ){

		MIR_write_dot (LED_ZOOM_MAP, 		MIR_RED);
		MIR_write_dot( LED_ZOOM_MAP, 		MIR_BLINK );
	}

	// EDIT MASTER
	// Shows the GRIDTRACK EDIT mode
	switch( GRID_editmode ){

		case ON:
			show( ELE_EDIT_MASTER, RED   );
			show( ELE_EDIT_MASTER, GREEN );
			show( ELE_EDIT_MASTER, BLINK );
			break;

		case OFF:
			show( ELE_EDIT_MASTER, GREEN );
			break;
	}

#ifdef FEATURE_ENABLE_SONG_UPE
	//
	// Pause Measure Locator Scrolling (PMLS) - enabled
	//
	if ( G_pause_bit == ON ){

		// Show global repeat interval as chord LEDs
		MIR_write_dot( 252 + G_repeats_interval_idx, MIR_GREEN );
		if ( G_repeats_delay == ON )
		{
			MIR_write_dot( 252 + G_repeats_interval_idx, MIR_BLINK );
		}

		// Show 1/5 nibble (track chain preset LEDs)
		if ( G_measure_indicator_part > 0 )
		{
			MIR_write_dot( 206 - G_measure_indicator_part, MIR_GREEN );
		}

		// NUMERIC QUADRANT
		MIR_write_numeric_C( G_measure_indicator_value );

	}
#endif

