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

			// Page recall from Flash
			MIR_write_dot( LED_RETURN, MIR_GREEN 	);
			MIR_write_dot( LED_RETURN, MIR_BLINK 	);

//			// Page write to flash
//			if ( G_run_bit == OFF ){
//				MIR_write_dot( LED_PROGRAM, MIR_RED 	);
//				MIR_write_dot( LED_PROGRAM, MIR_BLINK 	);
//			}

			// TRACK SELECTORS
			show( ELE_TRACK_SELECTORS, GRID_BANK_PLAYMODES );

			// TRACK MUTATORS
			show( ELE_TRACK_MUTATORS, PAGE_FUNCTIONS );
			// Caution!! There is some extra code in the ELE_MATRIX / GRID section.


			// EDIT INDICATOR
			show( ELE_EDIT_INDICATOR, RED );

			// Some page attribute is being edited
			if ( EDIT_TIMER == ON ){

				switch( Page_repository[GRID_CURSOR].mixingTrack ){

					case NEMO_ATTR_PITCH:
						MIR_write_pitch_H	( 	target_page->attr_PIT, 0 );
						break;

					case NEMO_ATTR_VELOCITY:
						MIR_fill_numeric 	(	1, target_page->attr_VEL,	0,	MIR_GREEN);
						MIR_fill_numeric 	(	1, target_page->attr_VEL,	0,	MIR_RED);
						break;

					case NEMO_ATTR_LENGTH:
						// Show the pagelength in hex really - resolution of 16 steps..
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

				// Show the GRID cursor
				if( !row_in_page_window( GRID_CURSOR % 10 ) )
					break;
				temp = cursor_to_dot( GRID_CURSOR ) - shiftPageRow;
				MIR_write_dot( temp, MIR_RED   );
				MIR_write_dot( temp, MIR_GREEN );
				MIR_write_dot( temp, MIR_BLINK );
			}

			// no break, Fallthrough intended
			break;


		// This is the general case
		case INTERACTIVE:
		case PASSIVE:

			// Show the hyped tracks
			for ( i=0; i < MATRIX_NROF_ROWS; i++ ){
				if( !row_in_track_window( target_page, i ) )
					continue;

				// Track has a valid hyper step address
				if ( (target_page->Track[i]->hyper >> 4) != 0x0F ){
					j |= ( 1 << ( i - shiftTrackRow ) );
				}
			}
			// Show hyped pattern
			MIR_write_buttool( LHS, j, MIR_SHINE_GREEN );


			// MIX MASTER
			if (  	(target_page->trackSelection == 0)
				&&	(target_page->stepSelection  == 0)
				){

				// NEMO
				if ( target_page->mixTarget == MIXTGT_ATR ){
					// MIX MASTER
					MIR_write_dot( LED_MIX_MASTER, MIR_RED 		);
					MIR_write_dot( LED_MIX_MASTER, MIR_GREEN 	);
				}
				else{
					// MIX INDICATOR - LED corresponding to the rotarys
					MIR_write_dot( LED_MIX_INDICATOR, MIR_RED 		);
					MIR_write_dot( LED_MIX_INDICATOR, MIR_GREEN 	);
				}
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

				MIR_write_dot( LED_MIXTGT_USR1, MIR_GREEN   );
				MIR_write_dot( LED_MIXTGT_USR2, MIR_GREEN   );
				MIR_write_dot( LED_MIXTGT_USR3, MIR_GREEN   );
				MIR_write_dot( LED_MIXTGT_USR4, MIR_GREEN   );

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


			// TRACK MUTATORS
			// Default: solo and mute patterns, concurrently
			show (ELE_TRACK_MUTATORS, TRACK_SOLOPATTERN);
			show (ELE_TRACK_MUTATORS, TRACK_MUTEPATTERN);

			// When in preview mode also show the TGL button for the preview step
			if (	( ( target_page->editorMode == PREVIEW )
					||	( target_page->editorMode == PREVIEW_PERFORM ))
				&&	( page_preview_step != NULL )
				){

		 		MIR_write_dot( LED_TGGL, MIR_GREEN );
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

			}

			if ( target_page->OPS_mode != BIRDSEYE ) {
				if ( CHECK_BIT( G_track_page_chain_mod_bit, ON_THE_MEASURE_MOD ) ) {
					// On the measure mode
					MIR_write_dot( LED_ZOOM_TRACK, MIR_BLINK );
					MIR_write_dot( LED_ZOOM_TRACK, MIR_RED );
					if ( CHECK_BIT( G_track_page_chain_mod_bit, CLUSTER_MOD ) )	{
						// On the measure cluster mode
						MIR_write_dot( LED_ZOOM_TRACK, MIR_GREEN );
					}
				}
				else if ( CHECK_BIT( G_track_page_chain_mod_bit, CLUSTER_MOD ) ) {
					// Cluster mode
					MIR_write_dot( LED_ZOOM_TRACK, MIR_RED );
				}
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
						MIR_write_dot( LED_EDIT_MASTER, MIR_BLINK );

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

