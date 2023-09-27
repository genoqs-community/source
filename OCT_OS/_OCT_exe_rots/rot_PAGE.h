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


extern void rot_exe_MIX( unsigned char rotNdx, unsigned char direction, Pagestruct* target_page );


// Apply edit encoders to the page as an entity.
void rot_exec_PAGE_global( 	Pagestruct* target_page,
							unsigned char rotNdx,
							unsigned char direction ){

	unsigned char i, j;
	i = j = 0;

	switch( rotNdx ){

		case ROT_VEL:

			// Modify the page velocity
			modify_parameter( 	&Page_repository[GRID_CURSOR].attr_VEL,
								PAGE_MIN_VEL, PAGE_MAX_VEL, direction, OFF, FIXED );
			break;


		case ROT_PIT:

			// Modify the page pitch
			modify_parameter( 	&Page_repository[GRID_CURSOR].attr_PIT,
								PAGE_MIN_PIT, PAGE_MAX_PIT, direction, OFF, FIXED );
			break;


		case ROT_LEN:

			// Make the actual changes visible
			GRID_rowzero_pagelength = TRUE;

			// Modify the page length
			modify_parameter( 	&Page_repository[GRID_CURSOR].attr_LEN,
								PAGE_MIN_LEN, PAGE_MAX_LEN, direction, OFF, FIXED );
			break;


		case ROT_STA:

			// Make the actual changes visible
			GRID_rowzero_pagelength = FALSE;

			// Modify the page repeats - this is the page STA value
			modify_parameter( 	&Page_repository[GRID_CURSOR].attr_STA,
								PAGE_MIN_STA, PAGE_MAX_STA, direction, OFF, FIXED );
			Page_repository[GRID_CURSOR].repeats_left = Page_repository[GRID_CURSOR].attr_STA;
			break;


		case ROT_POS:
			#ifdef FEATURE_ENABLE_SONG_UPE
			// Use POS to set the page repeat counter
			// Modify the page repeats
			modify_parameter( 	&Page_repository[GRID_CURSOR].repeats_left,
								PAGE_MIN_STA, Page_repository[GRID_CURSOR].attr_STA, (( direction == INC ) ? DEC : INC ), OFF, FIXED );
			#else
			j = (&Page_repository[ GRID_CURSOR ])->locator;
			// Compute the new locator to align the page under cursor to.
			if ( direction == INC ){

				i = ((&Page_repository[ GRID_CURSOR ])->locator % 16) + 1;
			}
			else if ( direction == DEC ){

				i = (&Page_repository[ GRID_CURSOR ])->locator - 1;
				if ( i == 0 ){	i = 16;	}
			}

			j = G_TTC_abs_value;

			// Align the page under the cursor fully to the new locator.
			set_page_locators( &Page_repository[ GRID_CURSOR ], i, j );
			#endif
			break;

	} // switch( rotNDx )

}



// MIXMAP encoder activity
void rot_exec_MIXMAP( 	Pagestruct* target_page,
						unsigned char rotNdx,
						unsigned char direction ){

	unsigned char row = 0;

	// BIG KNOB
	if ( rotNdx == 0 ){

		// When not running on external clock
		// And no page selected in GRID
		if (	( G_clock_source != EXT )
			&&	( GRID_play_mode != GRID_EDIT )
			){

			modify_parameter( &G_master_tempo, MIN_TEMPO, MAX_TEMPO, direction, ON, FIXED);
			G_TIMER_REFILL_update();
		}
	}


	// Do not react on tracks that are not heads in play_heads mode
	// Check if we are playing heads only..
	if 	(	( target_page->CHAINS_PLAY_HEAD == TRUE )
		&&	( rotNdx > 10 )
		){

		// Identify the row of the MIX encoder
		row = rotNdx - 11;

		// Check if track is chain head..
		if ( target_page->Track[row]->chain_data[HEAD] != target_page->Track[row] ){

			return;
		}
	}

	// M I X E R: Left rotarys modify the assigned attribute
	rot_exe_MIX( rotNdx, direction, target_page );

	// E D I T O R:   Right rotarys modify hardwired attributes
	rot_exe_EDIT( rotNdx, direction, target_page );
}



// Called to execute the EDIT block operations.
// Semantics can be POS or NEG - call from EDIT block or MIXER block respectively..
void rot_exec_PAGE_local_EDIT( 	Pagestruct* target_page,
								unsigned char rotNdx,
								unsigned char direction,
								unsigned char step_selection ){

	unsigned char row=0;
	unsigned char col=0;
	unsigned int i = 0;

	PhraseMultiTweakStart( step_selection == ON );

	// Operate on selected steps
	if ( target_page->stepSelection != 0 ){

		#ifdef FEATURE_STEP_SHIFT
			if ( rotNdx == 6 )  {
				StepSwapVertical( target_page, direction );
			}
			else {
				// Select the edit attribute
				target_page->editAttribute = rotNdx;

				// Scan through all steps of the matrix - not very efficient!!
				for (row=0; row < MATRIX_NROF_ROWS; row++) {

					// When processing non-selected steps via the MIXER block pots, skip rows that do not have
					// any selected steps. We don't want the MIXER pot to affect rows that are not participated in the selection.

					if ( step_selection == OFF ) {
						for ( col=0; col < MATRIX_NROF_COLUMNS; col++ ) {
							if ( Step_get_status(target_page->Step[row][col], STEPSTAT_SELECT) == ON ) {
								break;
							}
						}
						if ( col == MATRIX_NROF_COLUMNS ) {
							continue;
						}
					}

					// Okay, this row is involved in the selection, and can be processed.
					for (col=0; col < MATRIX_NROF_COLUMNS; col++) {

						// STEP SELECTION flag is set
						if (Step_get_status( target_page->Step[row][col], STEPSTAT_SELECT ) == step_selection ){

							// Does what it says - applies the rotary to the step in question.
							apply_rotary_to_step( rotNdx, direction, target_page, row, col );

						} // Step is selected
					} // Matrix column G_scan

				} // Matrix row G_scan
			} // else

		#else
			// Select the edit attribute
			target_page->editAttribute = rotNdx;

			// Scan through all steps of the matrix - not very efficient!!
			for (row=0; row < MATRIX_NROF_ROWS; row++) {

				// When processing non-selected steps via the MIXER block pots, skip rows that do not have
				// any selected steps. We don't want the MIXER pot to affect rows that are not participated in the selection.

				if ( step_selection == OFF ) {
					for ( col=0; col < MATRIX_NROF_COLUMNS; col++ ) {
						if ( Step_get_status(target_page->Step[row][col], STEPSTAT_SELECT) == ON ) {
							break;
						}
					}
					if ( col == MATRIX_NROF_COLUMNS ) {
						continue;
					}
				}

				// Okay, this row is involved in the selection, and can be processed.
				for (col=0; col < MATRIX_NROF_COLUMNS; col++) {

					// STEP SELECTION flag is set
					if (Step_get_status( target_page->Step[row][col], STEPSTAT_SELECT ) == step_selection ){

						// Does what it says - applies the rotary to the step in question.
						apply_rotary_to_step( rotNdx, direction, target_page, row, col );

					} // Step is selected
				} // Matrix column G_scan

			} // Matrix row G_scan
		#endif

	} // step selection == ON


	// No step selection is available, but at least one step button is pressed
	// Entering Roland Fleming mode: make sure that step is on, and edit its atribute
	else if ( G_key_pressed != 0 ){

		// Modify page attributes if zoom button is pressed
		if ( G_key_pressed == KEY_ZOOM_PAGE ){

			rot_exec_PAGE_global( 	target_page, rotNdx, direction );
		}

		// Some other key is pressed, potentially a step key
		else{

			// Linear G_scan through the buttons in question: Selectors and Steps.
			for( i=0; i <= 185; i++ ){

				// Not using the provided infrastructure because of the anomalies. may change
				// There is a key pressed - in the Step Range (exclude the rest)
				if (	( G_pressed_keys[i] >=  11 )
					&&	( G_pressed_keys[i] <= 185 )
					&& 	( ((G_pressed_keys[i]-10) % 11) != 0 )

					// React only to PITCH, VELOCITY, LENGTH and START rotaries
					&& 	(	( rotNdx == 1 )		// VEL
						||	( rotNdx == 2 ) 	// PIT
						||	( rotNdx == 3 ) 	// LEN
						||	( rotNdx == 4 )		// STA
						||	( rotNdx == 5 )		// POS (phrase-factor)
						||	( rotNdx == 6 )		// DIR (phrase-type)
						||	( rotNdx == 7 )		// AMT
						||	( rotNdx == 8 )		// GRV (phrase-num)
						||	( rotNdx == 9 )		// MCC
						)
					){

					// Compute the key coordinates
					row = row_of( 		G_pressed_keys[i] );
					col = column_of(	G_pressed_keys[i] );

					// d_iag_printf( "%d row:%d col:%d  ", G_pressed_keys[i], row, col );

					// The first knob click activates the step if it is not active
					if ( Step_get_status( target_page->Step[row][col], STEPSTAT_TOGGLE ) == OFF ){

						// If we are not in PREVIEW mode
						if (	(	( target_page->editorMode != PREVIEW )
								&&	( target_page->editorMode != PREVIEW_PERFORM )
								)
							){

							// Make sure the step gets turned on as we turn the knob
							Step_set_status( target_page->Step[row][col], STEPSTAT_TOGGLE, ON );
						}
					}

					// Finally apply the rotary to the step in question.
					apply_rotary_to_step( rotNdx, direction, target_page, row, col );

					// If we are in PREVIEW mode, play the step as well
					if (	( target_page->editorMode == PREVIEW )
						){

						PLAYER_preview_step( target_page, row, col );
					}
				} // key pressed in step range - Roland fleming mode


				// Key pressed in Track Selector range
				else if (
							(	( G_pressed_keys[i] >=  1 )
							&&	( G_pressed_keys[i] <= 10 ) )
						|| 	( G_pressed_keys[i] == 32 )
					){

					// Compute the key row
					row = i-1;

					rot_exe_EDIT( rotNdx, direction, target_page );

					// If we are in PREVIEW mode, play the track as well
					if (	( target_page->editorMode == PREVIEW )
						// React only to PITCH and VELOCITY rotaries
						&& 	(	( rotNdx == 1 )
							||	( rotNdx == 2 ) )
						){

						PLAYER_preview_track( target_page, row );
					}

					// Break out of the key iteration
					i = 185;

				} // Key pressed in track selector range

			} // button G_scan iterator

		} // else-- possibly a button is pressed

	} // G_key_pressed != 0 - Step for Roland Fleming mode


	// Track is selected
	else if ( target_page->trackSelection != 0 ){

		// If we are in PREVIEW mode, play the track as well
		if (	( target_page->editorMode == PREVIEW )
			// React only to PITCH and VELOCITY rotaries
			&& 	(	( rotNdx == 1 )
				||	( rotNdx == 2 ) )
			){
			// Iterate through the selected rows
			for ( i=0; i<MATRIX_NROF_ROWS; i++ ){

				if ( ((target_page->trackSelection & (1<<i)) != 0) ){
					// Generate preview signal from track
					PLAYER_preview_track( target_page, i );
				}
			}
		}

		// General case
		else{
			// Interpret the rotary applied to track/s selected
			rot_exe_EDIT( rotNdx, direction, target_page );
		}
	}

	// MIX2EDIT is when the EDIT block acts similarly to the MIX block
	else if ( target_page->editorMode == MIX2EDIT ) {

		// Needs more digging - starting the MIX timer is not relevant here
		// --really need a MIX2EDIT timer - not implemented yet
		// start_MIX2EDIT_TIMER();
		// d_iag_printf( "MIX2EDIT rotary\n" );

		// Normalize i value
		i = rotNdx - 1;

		switch ( target_page->attr_mix2edit ){

			case ATTR_POSITION:
				Page_wrap_track( target_page, i, direction, 1 );
				break;

			case ATTR_LENGTH:
				modify_signed_parameter
					( &target_page->Track[i]->LEN_factor,
						target_page->attr_mix2edit_MIN,
						target_page->attr_mix2edit_MAX,
						direction, OFF, FIXED);
				break;

			case ATTR_START:
				modify_signed_parameter
					( &target_page->Track[i]->STA_factor,
						target_page->attr_mix2edit_MIN,
						target_page->attr_mix2edit_MAX,
						direction, OFF, FIXED);
				break;

			default:
				// Modify the remembered (MIX2EDIT) attribute of track
				modify_signed_parameter
					( 	(signed char*) ptr_of_track_attr_value( target_page, i, target_page->attr_mix2edit ),
						target_page->attr_mix2edit_MIN,
						target_page->attr_mix2edit_MAX,
						direction, OFF, FIXED);
				break;
		}
	}

	// MCC function of the EDITOR - double implementation here and in the editorblock file
	// Needs cleanup at some point..
	else if ( target_page->editorMode == CONTROL ) {

		// No track selected and EDITOR is in Controller Mode -> send MIDI CC data

		// Normalize i value
		i = rotNdx - 1;

		// Check before sending data that a valid CC is assigned (tmoravan)
		if ( ( 	target_page->Track[i]->attr_MCC ) != MIDICC_NONE ){

			// Modify the CC amount of track
			modify_signed_parameter
				(&target_page->Track[i]->attr_CCAMT,
					TRACK_MIN_CCAMNT, TRACK_MAX_CCAMNT, direction, ON, FIXED);

			// Send MIDICC on the corresponding channel
			MIDI_send( 	MIDI_CC,
						target_page->Track[i]->attr_MCH
						+ target_page->Track[i]->event_offset[ATTR_MIDICH],
						target_page->Track[i]->attr_MCC,
						target_page->Track[i]->attr_CCAMT 	);
		} // Track is assigned a valid CC value

	}

}



// Master handler of rotary encoder operations in local PAGE mode
void rot_exec_PAGE_local( 	Pagestruct* target_page,
							unsigned char rotNdx,
							unsigned char direction ){

	unsigned char i=0;
	unsigned char j=0;

	// Mark the page as touched
	target_page->page_clear = OFF;


	// BIG KNOB
	if ( rotNdx == 0){

		// Regular operation
		if ( GRID_bigknob_SELECT == OFF ){

			// When a track is selected, modify its program change
			if ( my_bit_cardinality( target_page->trackSelection ) == 1 ){
				modify_parameter( 	&target_page->Track[ my_bit2ndx( target_page->trackSelection )]->program_change,
									TRACK_MIN_PROGRAMCHANGE, TRACK_MAX_PROGRAMCHANGE, direction, ON, FIXED );
			}

			// When a single step is selected, change velocity.
			else if ( target_page->stepSelection == 1 ) {
				int row = target_page->stepSelectionSingleRow;
				int col = target_page->stepSelectionSingleCol;
				modify_signed_parameter(
								&target_page->Step[row][col]->attr_VEL,
								STEP_MIN_VELOCITY, STEP_MAX_VELOCITY, direction, OFF, FIXED );
			}

			// Nothing selected.
			else if ( target_page->stepSelection == 0 ) {
				int stepKey;

				// If a step key is held down in PREVIEW mode, change the step's velocity.
				if ( 	(	(target_page->editorMode == PREVIEW)
						||	(target_page->editorMode == PREVIEW_PERFORM)
						)
					&& 	(target_page->OPS_mode != BIRDSEYE)
					&& 	(stepKey = is_pressed_steprange() )
					){

					int row = row_of( stepKey );
					int col = column_of( stepKey );
					modify_signed_parameter(
									&target_page->Step[row][col]->attr_VEL,
									STEP_MIN_VELOCITY, STEP_MAX_VELOCITY, direction, OFF, FIXED );
				}

				// Else, change the global tempo.
				else if ( G_clock_source != EXT ) {
					modify_parameter( &G_master_tempo, MIN_TEMPO, MAX_TEMPO, direction, ON, FIXED);
					G_TIMER_REFILL_update();
				}
			}
		}


		// Jump across the playing pages
		else if ( GRID_bigknob_SELECT == ON ){

			switch( direction ){

				case INC:
					// Get the next page if available
					j = FALSE;

					for ( i = GRID_NROF_BANKS+1; i > 0; i-- ){

						if ( 	( j == TRUE )
							&&	( GRID_p_selection[i-1] != NULL )
							){

							// Move the viewer page
							target_page = GRID_p_selection[i-1];

							// Move the cursor
							GRID_CURSOR = target_page->pageNdx;
							j = FALSE;
							break;
						}

						if ( GRID_p_selection[i-1] == target_page ){
							// Remember having found the viewer page
							j = TRUE;
						}
					}
					break;

				case DEC:
					// Get the next page if available
					j = FALSE;

					for ( i=0; i < GRID_NROF_BANKS; i++ ){

						if ( 	( j == TRUE )
							&&	( GRID_p_selection[i] != NULL )
							){

							// Move the viewer page
							target_page = GRID_p_selection[i];

							// Move the cursor
							GRID_CURSOR = target_page->pageNdx;
							j = FALSE;
							break;
						}

						if ( GRID_p_selection[i] == target_page ){
							// Remember having found the viewer page
							j = TRUE;
						}
					}
					break;
			}
		} // GRID_bigknob_select == ON
	}


	// MIXER
	//
	if ((rotNdx >= 11) && (rotNdx <= 20) ) {

		// Do not proceed if there is a track selection.
		// Selections are serviced by the EDITOR
		// ..but un-selections may be accessed by the MIXER!! :-) (for steps only)
		if (   (target_page->trackSelection != 0)
			){
			#ifdef FEATURE_ENABLE_KEYBOARD_TRANSPOSE
			//The exception is setting the keyboard transpose midi channel for selected tracks
			if( (rotNdx == ROT_0)) {

				rot_exe_MIX2EDIT( rotNdx, direction, target_page );

			}
			#endif
			return;
		}
		// Modify the steps that are not selected - like the editor would!
		else if ( target_page->stepSelection != 0 ){

			// Align rot index to EDITOR context - simulate EDITOR
			rotNdx = rotNdx - 10;

			// Trigger EDIT block behaviour operations - OFF means act on non-selected steps
			rot_exec_PAGE_local_EDIT( target_page, rotNdx, direction, OFF );
		}
		else{

			// Quick+Dirty fix for something where we may need an additional zoom mode.
			rot_exe_MIX( rotNdx, direction, target_page );
		}
	}


	// E D I T O R
	//
	else if ((rotNdx >= 1) && (rotNdx <= 10)) {

		// Triggers the normal EDIT block behaviour operations - ON means act on the selected steps
		rot_exec_PAGE_local_EDIT( target_page, rotNdx, direction, ON );
	} // else if rotNdx in Editor

}



//
// Main encoder handler in PAGE mode
//
void rot_exec_PAGE( 	Pagestruct* target_page,
						unsigned char rotNdx,
						unsigned char direction ){

	// Global operations on the track
	if( target_page->OPS_mode == BIRDSEYE ){

		rot_exec_PAGE_global( target_page, rotNdx, direction );
	}
	else{

		rot_exec_PAGE_local( target_page, rotNdx, direction );
	}
}

