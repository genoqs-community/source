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
// NO SELECTION IN PAGE
//

// Accomodate the BIRDSEYE view


	switch( target_page->OPS_mode ){

		// Shows the page functions needed globally
		case BIRDSEYE:

			// Show the page functions as they are visible in the grid

			// REC Button
			MIR_write_dot( LED_RECORD, MIR_GREEN );

			// TRACK SELECTORS
			show( ELE_TRACK_SELECTORS, GRID_BANK_PLAYMODES );

			// TRACK MUTATORS
			show( ELE_TRACK_MUTATORS, PAGE_FUNCTIONS );
			// Caution!! There is some extra code in the ELE_MATRIX / GRID section.

			// OCTAVE CIRCLE
			show( ELE_OCTAVE_CIRCLE, PAGE_TRANSPOSITION );

			// VELOCITY CIRCLE
			MIR_write_factor_C( target_page->attr_VEL );

			// RETURN (load page)
			MIR_write_dot( LED_RETURN, MIR_GREEN   );
			MIR_write_dot( LED_RETURN, MIR_BLINK );

			// EDIT INDICATOR
			show( ELE_EDIT_INDICATOR, RED );

			// MATRIX
			show( ELE_MATRIX, GRID );

			// Show the page LEN/STA values
			switch( GRID_rowzero_pagelength ){

				case FALSE:
				#ifdef FEATURE_ENABLE_SONG_UPE
				{
					unsigned char posREP = (Page_repository[GRID_CURSOR].attr_STA - Page_repository[GRID_CURSOR].repeats_left) + 1;
					unsigned char numericSTA = (Page_repository[GRID_CURSOR].attr_STA/16)+1;
					unsigned char numericREP = (posREP/16)+1;
					unsigned char pointSTA = Page_repository[GRID_CURSOR].attr_STA%16;
					unsigned char pointREP = ((Page_repository[GRID_CURSOR].attr_STA - Page_repository[GRID_CURSOR].repeats_left)%16) + 1;

					// Show the pagelength in hex really - resolution of 16 steps..
					MIR_fill_numeric( 1, numericSTA, 9, MIR_RED );
					MIR_point_numeric( pointSTA, 9, MIR_GREEN );
					MIR_write_dot( 10, MIR_GREEN );

					if ( pointREP == numericREP && posREP >= 16 ) {
						MIR_point_numeric(pointREP, 9, MIR_GREEN );
						MIR_point_numeric(pointREP, 9, MIR_BLINK );
					} else if ( pointREP < numericSTA ) {
						MIR_point_numeric(pointREP, 9, MIR_SHINE_RED );
					} else {
						MIR_point_numeric(pointREP, 9, MIR_SHINE_GREEN );
					}
					MIR_fill_numeric( 1, numericREP, 9, MIR_BLINK );
				}
				#else
					// Show the repeats of the page under cursor. Not in the above because
					// ..for some reason doesn't work.
					MIR_fill_numeric( 1, target_page->repeats_left, 9, MIR_GREEN );
					MIR_fill_numeric( 	 target_page->repeats_left,
										 target_page->attr_STA + 1, 9, MIR_RED );
					MIR_write_dot( 10, MIR_RED );
				#endif
				break;
				case TRUE:
					// Show the pagelength in hex really - resolution of 16 steps..
					MIR_fill_numeric( 1, (target_page->attr_LEN/16)+1, 9, MIR_RED );
					MIR_point_numeric( target_page->attr_LEN%16, 9, MIR_GREEN );
					MIR_write_dot( 10, MIR_GREEN );
					break;
			}


			// Show the GRID cursor
			temp = cursor_to_dot( GRID_CURSOR );
			MIR_write_dot( temp, MIR_RED   );
			MIR_write_dot( temp, MIR_GREEN );
			MIR_write_dot( temp, MIR_BLINK );

			break;


		// This is the general case
		case INTERACTIVE:
		case PASSIVE:

			// Show the hyped tracks
			for ( i=0; i < MATRIX_NROF_ROWS; i++ ){
				// Track has a valid hyper step address
				if ( (target_page->Track[i]->hyper >> 4) != 0x0F ){
					j |= (1<<i);
				}
			}
			// Show hyped pattern
			MIR_write_buttool( LHS, j, MIR_SHINE_GREEN );


			// MIX MASTER
			if (  	(target_page->trackSelection == 0)
				&&	(target_page->stepSelection  == 0)
				){

				// Only if no track is selected
				if (target_page->mixMasterStatus == POS) {
					show (ELE_MIX_MASTER, RED);
				}
				else {
					show (ELE_MIX_MASTER, GREEN);
				}

				// MIX INDICATOR - LED corresponding to the rotarys
				show( ELE_MIX_INDICATOR, RED   );
				show( ELE_MIX_INDICATOR, GREEN );
			}



			// MIX TARGET field
			if (  	( target_page->trackSelection == 0 )
				&&	( target_page->stepSelection  == 0 )
				){
				// OFF here means show only the selection.. (oh well..)
				show( ELE_MIXTGT, OFF );
			}
			// Show the current step selection set
			if ( is_pressed_key( KEY_SELECT_MASTER )
				){
				switch( target_page->stepSELpattern_ndx ){

					case 0:	MIR_write_dot( LED_MIXTGT_USR1, MIR_GREEN 	);
							MIR_write_dot( LED_MIXTGT_USR1, MIR_BLINK 	);
						break;
					case 1:	MIR_write_dot( LED_MIXTGT_USR2, MIR_GREEN 	);
							MIR_write_dot( LED_MIXTGT_USR2, MIR_BLINK 	);
						break;
					case 2:	MIR_write_dot( LED_MIXTGT_USR3, MIR_GREEN 	);
							MIR_write_dot( LED_MIXTGT_USR3, MIR_BLINK 	);
						break;
					case 3:	MIR_write_dot( LED_MIXTGT_USR4, MIR_GREEN 	);
							MIR_write_dot( LED_MIXTGT_USR4, MIR_BLINK 	);
						break;
					case 4:	MIR_write_dot( LED_MIXTGT_USR5, MIR_GREEN 	);
							MIR_write_dot( LED_MIXTGT_USR5, MIR_BLINK 	);
						break;
				} // switch
			}



			// SELECT MASTER
			if (target_page->trackSelectionStored != 0) {
				// Show RCL option
				show (ELE_SELECT_MASTER, RED);
			}
			
			#ifdef FEATURE_ENABLE_SONG_UPE
			// CONTROL TRACK INDICATOR
			for (i=0; i<MATRIX_NROF_ROWS; i++) {
				if ( Track_get_MISC(target_page->Track[i], CONTROL_BIT) == ON && target_page->Track[i]->attr_STATUS != 127){
					MIR_write_buttool( 	LHS,
							1 << i,
							MIR_RED );
					MIR_write_buttool( 	LHS,
							1 << i,
							MIR_GREEN );
				}
			}
			#endif


			// MUTE MASTER
			// If nothing in page is selected
			if (	( target_page->trackSelection == 0 )
				&&	( target_page->stepSelection  == 0 )    ){

				// Indicate ongoing mute or solo activity
				if (	( target_page->trackMutepattern != 0 )
					||	( target_page->trackSolopattern != 0 )    ){

					show( ELE_MUTE_MASTER, GREEN );

					// Blink in the solo mode
					if ( target_page->trackSolopattern != 0 ){

					 	show( ELE_MUTE_MASTER, BLINK );
					}
				}

				// Indicate existing stored mutepattern
				else if (target_page->trackMutepatternStored != 0) {

					show (ELE_MUTE_MASTER, RED);
				}
			}


			// TRACK CHAINING
			show( ELE_CHAIN_INDICATOR, ON );


			// TRACK MUTATORS
			// Default: solo and mute patterns, concurrently
			show (ELE_TRACK_MUTATORS, TRACK_SOLOPATTERN);
			show (ELE_TRACK_MUTATORS, TRACK_MUTEPATTERN);

			// When in preview mode also show TGL, ZOM, CPY and PST for preview step
			if (	( 	( target_page->editorMode == PREVIEW )
					||	( target_page->editorMode == PREVIEW_PERFORM )
					)
				&&	( page_preview_step != NULL )
				){

		 		MIR_write_dot( LED_TGGL, MIR_GREEN );
		 		MIR_write_dot( LED_ZOOM, MIR_GREEN );
		 		MIR_write_dot( LED_COPY, MIR_GREEN );

		 		if (STEP_COPY_BUFFER != NULL ){
		 			MIR_write_dot( LED_PASTE, MIR_GREEN );
		 		}
			}


			// EDIT INDICATOR
			// No track or step is selected, indicate MCC activity of EDIT block
			if ( target_page->editorMode == MIX2EDIT ){

				show( ELE_EDIT_INDICATOR, RED   );
				show( ELE_EDIT_INDICATOR, GREEN );
			}


			// GLOBALS
			if ( target_page->trackSelection == 0 ){

				show( ELE_GLOBALS, TRANSPORT );
				show( ELE_GLOBALS, NUMERIC_QUADRANT );
				show( ELE_GLOBALS, BIG_KNOB );
				show( ELE_GLOBALS, CLOCK );
				show( ELE_GLOBALS, FOLLOW );

				show_GLOBALS_scale_master( target_page );
			}


			// OCTAVE CIRCLE
			// In PAGE mode show the SCALE, trans-position is in GRID
			// ..but make sure we are not in BIRDSEYE mode
			if (	(	( target_page->scaleStatus == SCALE_SEL )
					||	( target_page->scaleStatus == SCALE_MOD ) )
				&&	( target_page->OPS_mode != BIRDSEYE )
				){

				// Show the selected notes in scale. Both in MOD and SEL
				show_OCTAVE_CIRCLE_scale_selection( target_page );
				show_SCALE_SELECTOR_scale_selection( target_page );
			}

			if ( G_track_page_chain_mod_bit == ON && target_page->OPS_mode != BIRDSEYE )
			{
				MIR_write_dot( LED_SCALE_MOD,	MIR_RED	);
			}
			// on the measure mode
			else if ( G_track_page_chain_mod_bit == SCALE_MOD && target_page->OPS_mode != BIRDSEYE )
			{
				MIR_write_dot( LED_SCALE_MOD,	MIR_RED );
				MIR_write_dot( LED_SCALE_MOD,	MIR_GREEN );
				MIR_write_dot( LED_SCALE_MOD,	MIR_BLINK );
			}

			// EDIT MASTER
			// Activity only when no track is selected
			if ( target_page->trackSelection == 0 ){

				switch( target_page->editorMode ){

					case CONTROL:
						// Edit acts as MCC source - send MCC data
						show( ELE_EDIT_MASTER, RED );
						break;

					case EDIT:
						// This is the regular mode - edit attributes of selection
						show( ELE_EDIT_MASTER, GREEN );
						break;

					case PREVIEW:
						// Same as regular mode, but preview the steps being edited
						show( ELE_EDIT_MASTER, RED );
						show( ELE_EDIT_MASTER, GREEN );
						show( ELE_EDIT_MASTER, BLINK );

						// This is the JohnK preview mode
						show( ELE_MATRIX, PREVIEW_STEP );
						break;

					case PREVIEW_PERFORM:
						// Same as PREVIEW mode, but the MIDI stuff is not sent to out
						show( ELE_EDIT_MASTER, GREEN );
						show( ELE_EDIT_MASTER, BLINK );

						// This is the JohnK preview mode
						show( ELE_MATRIX, PREVIEW_STEP );
						break;

					case MIX2EDIT:
						// Edit acts as MCC source - send MCC data
						// show( ELE_EDIT_INDICATOR, GREEN );
						show( ELE_EDIT_INDICATOR, RED );
						break;
				}
			}


			break; // break for default:


	} // switch( target_page->OPS_mode

