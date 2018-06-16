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
// The case where the page contains an active track selection
//

	// Switch the chains play head status of the page
	if ( keyNdx == KEY_MIX_MASTER ){

		// Toggles head track listening in page mode
		target_page->CHAINS_PLAY_HEAD ^= 1;
	}

	// Invoke track solo
	if (keyNdx == KEY_MUTE_MASTER ){

		// Loop through the chain of the selected track and mute all chain tracks
		// Start with the pressed track
		current_track = target_page->Track[ my_bit2ndx( target_page->trackSelection )];
		temp = cardinality_of_chain( current_track );
		for ( i=0; i < temp; i++ ){

			target_page->trackSolopattern ^= ( 1 << row_of_track( target_page, current_track ));
			current_track = current_track->chain_data[NEXT];
		}
	}


	// This is the TAP KEY, provided a track is selected
	if (	( keyNdx == KEY_ZOOM_STEP )
		&& 	( G_run_bit == ON )
		&& 	( target_page->trackSelection != 0 )
		&&	( !is_pressed_key( KEY_SELECT_MASTER ) )
		&& 	( is_selected_in_GRID( target_page ) ) ){

		// Tap a step into the target page
		tap_step( target_page );

	} // TAP KEY
	#ifdef FEATURE_ENABLE_KB_TRANSPOSE
	// x2 - Toggle transpose abs pitch mode
	if (	( keyNdx == KEY_SCALE_SEL )
		&&	( target_page->trackSelection != 0 )	){

		target_page->pitch_abs ^= 1;

	}
	#endif
	// TRACK "zoom" if single selection
	if (keyNdx == KEY_ZOOM_TRACK) {

		for (i=0; i<10; i++) {

			// Single track selection condition
			if (  ((target_page->trackSelection & (1<<i)) != 0) &&
			      ((target_page->trackSelection ^ (1<<i)) == 0)    ) {

				// Switch zoomlevel to TRACK
				G_zoom_level = zoomTRACK;

				break;
			}
		}
	}


	//
	// MATRIX KEYS
	//
	// HYPERSTEP - Create by making a Track/Step pairing
	if ( 	( keyNdx >= 11 ) && ( keyNdx <= 185 )
		&& 	( ( (keyNdx-10) % 11 ) != 0 )
		){

		// Extract the track index from current track selection
		i = my_bit2ndx( target_page->trackSelection );

		// Check if there is a hyper already for the selected track
		if ( target_page->Track[i]->hyper < 0xF0 ){

			// Extract the coordinates of the responsible hyperstep
			row = target_page->Track[i]->hyper >> 4;
			col = target_page->Track[i]->hyper & 0x0F;

			// Go to that step and void its hyperTrack_ndx
			target_page->Step[row][col]->hyperTrack_ndx = 10;

			// Reset the step length of the previous hyper to the default length
			target_page->Step[row][col]->attr_LEN = STEP_DEF_LENGTH;
		}

		// Compute coordinates of the pressed step
		row = row_of( keyNdx );
		col = column_of( keyNdx );

		// Do not allow hyperstep to be on the track that is selected
		if ( row != i ){

			// Remove track from chain
			// ..

			// Assign the selected track to the pressed step
			target_page->Step[row][col]->hyperTrack_ndx = i;

			// Store the new step address as the new track hyper
			target_page->Track[i]->hyper = (row << 4) | col;

			// Shut the track up
			target_page->Track[i]->chain_data[PLAY] = NULL;
			target_page->Track[i]->frame_ndx 		= 0;
			target_page->Track[i]->attr_LOCATOR 	= 0;
			target_page->Track[i]->TTC 				= 0;
			target_page->Track[i]->gatePosition		= 0;

			// Make the hyper length more user friendly - normal tempo
			target_page->Step[row][col]->attr_LEN = STEP_DEF_LENGTH*16;
		}
		else{
			// Void the track hyper
			target_page->Track[row]->hyper = 0xF0;

			// ..and reactivate the track for play.
			target_page->Track[i]->chain_data[PLAY] = target_page->Track[i];

			// Align track to the global locator and TTC
			target_page->Track[i]->attr_LOCATOR 	= G_global_locator;
			target_page->Track[i]->frame_ndx 		= G_global_locator;
			target_page->Track[i]->TTC				= G_TTC_abs_value;
		}

		// Make sure to turn the hyped step OFF!
		Step_set_status( target_page->Step[ row ][ col ], STEPSTAT_SELECT, OFF );

		return;
	}


	//
	// TRACK_MUTATORS
	//
	if ((keyNdx >= 187) && (keyNdx <= 196)) {

		// Call track entity FUNCTION on track selection
		switch (keyNdx) {

			case KEY_CHAIN:
				// Chain the currently selected tracks in the target_page
				chain_selected_tracks( target_page );

				// Clear track Selection
				target_page->trackSelection = 0;
				break;


			case KEY_FLT:
				// Flatten the track content. See function for details.
				if ( target_page->trackSelection != 0 ){

					Page_FLT_selected_tracks( target_page );
				}

				target_page->trackSelection = 0;
				break;


//			case KEY_RANDOMIZE:
//				// Randomize the Toggle state of the selected tracks
//				Page_RNDMZ_selected_tracks( target_page );
//				break;


			case KEY_RMX:
				// Remix track contents. See function for details.
				// Page_RMX_selected_tracks( target_page );
				// Page_RNDMZ_selected_tracks( target_page );
				NEMO_Page_RNDMZ_selected_tracks( target_page );
				break;


			case KEY_CLEAR:
				Page_CLEAR_selected_tracks( target_page );
				break;

		} // switch (keyNdx)

	} // Key is a track mutator



	// KEY_COPY:
	if ( keyNdx == KEY_COPY ){

		// D O U B L E - C L I C K
		// DOUBLE CLICK SCENARIO
		if ((DOUBLE_CLICK_TARGET == keyNdx)
				&& (DOUBLE_CLICK_TIMER > DOUBLE_CLICK_ALARM_SENSITIVITY)) {

			// DOUBLE CLICK CODE
			// ...
			// Move copy buffer into next empty page of bank
			// Ensure that buffer is not empty
			if (TRACK_COPY_BUFFER != 0) {

				// Find the next empty page in bank of target_page and switch
				j = target_page->pageNdx % 10;

				for ( i = j; i < (j + 150); i += 10 ){

					if ( i > MAX_NROF_PAGES ) {
						break;
					}

					if (Page_repository[i].page_clear == ON) {

						// Switch viewer to the found empty page
						target_page = &Page_repository[i];
						break;
					}
				}

				// Break if no empty page found in bank
				if (i == j+150) {
					return;
				}

				// Get the source page index
				m = (TRACK_COPY_BUFFER >> 10);

				// Leave only the track bitpattern (first 10 bits) in copy buffer
				TRACK_COPY_BUFFER &= NEMO_MAX_WINDOW;

				// Get the initial cardinality of track buffer
				k = my_bit_cardinality( TRACK_COPY_BUFFER );

				// Create artificial track selection in target page
				target_page->trackSelection = ( NEMO_MAX_WINDOW >> (10 - k) );

				// d_iag_printf( "TCB initial:%d cardinality:%d\n", TRACK_COPY_BUFFER, k );
				// my_print_bits( TRACK_COPY_BUFFER );

				// Paste the buffer tracks into the artificial selection
				for (i=0; i<k; i++) {

					// d_iag_printf( "track_copy:%d\n", i );

					// Copy first buffer track into first target track
					Track_copy( &Page_repository[m],
								my_bit2ndx( TRACK_COPY_BUFFER ),
								target_page,
								i );

					// Clear the bit of the track just copied
					TRACK_COPY_BUFFER ^= (1 << my_bit2ndx( TRACK_COPY_BUFFER ));

					// d_iag_printf( "     TCB new:%d\n", TRACK_COPY_BUFFER );
					// my_print_bits( TRACK_COPY_BUFFER );

				}

				// Mark the target page as modified - just in case it was fresh
				target_page->page_clear = OFF;

				// Unmark the track selection in new VIEWER page
				target_page->trackSelection = OFF;

				// Stop playing the page and reset its track locators
				stop_playing_page( &Page_repository[m], G_TTC_abs_value );
				set_track_locators( &Page_repository[m], NULL, 0, 0 );

				// Remove source page from GRID_selection
				grid_select( &Page_repository[m], OFF );

				// And put the target page into the preselection
				grid_select( target_page, ON );

				// GRID parameter update
				GRID_CURSOR = target_page->pageNdx;
				GRID_p_selection[ j%10 ] = target_page;

			} // if (TRACK_COPY_BUFFER != 0)
		} // DOUBLE CLICK SCENARIO


		// SINGLE CLICK SCENARIO
		else if (DOUBLE_CLICK_TARGET == 0) {
				DOUBLE_CLICK_TARGET = keyNdx;
				DOUBLE_CLICK_TIMER = ON;
				// Start the Double click Alarm
				cyg_alarm_initialize(
						doubleClickAlarm_hdl,
						cyg_current_time() + DOUBLE_CLICK_ALARM_TIME,
						DOUBLE_CLICK_ALARM_TIME );

			// SINGLE CLICK CODE
			// ...
			// Remember the selected tracks in terms of page and row

			TRACK_COPY_BUFFER =
				( target_page->trackSelection | (target_page->pageNdx << 10) );

			// Reset the trackselection in source page
			// target_page->trackSelection = 0;
		}
	}


	// KEY_PASTE:
	if ( keyNdx == KEY_PASTE ){

		// Don't continue if there is no valid data
		if ( TRACK_COPY_BUFFER == 0){

			return;
		}

		// Get the bit cardinality of track selection in copy buffer
		k = my_bit_cardinality( TRACK_COPY_BUFFER & NEMO_MAX_WINDOW );

		// Get the source page index
		j = TRACK_COPY_BUFFER >> 10;

		// Act according to cardinality
		switch (k) {

			case 0:
				// Copy buffer is empty - do nothing
				break;

			default:
				// Execute the multiple copy operation
				// Check cardinality of current track selection
				if (my_bit_cardinality( target_page->trackSelection ) == k) {

					// Remember the track copy buffer
					temp = TRACK_COPY_BUFFER;

					// Copy buffer into track selection
					for (i=0; i < k; i++) {

						// Copy first buffer track into first target track
						Track_copy( &Page_repository[j], 	my_bit2ndx( TRACK_COPY_BUFFER & NEMO_MAX_WINDOW ),
									target_page, 			my_bit2ndx( target_page->trackSelection ) );

						// Clear the bits of the processed tracks in the TRACK_COPY_BUFFER
						TRACK_COPY_BUFFER ^= (1 << my_bit2ndx( TRACK_COPY_BUFFER & NEMO_MAX_WINDOW ));
						target_page->trackSelection ^= (1 << my_bit2ndx( target_page->trackSelection ));
					}
				}

				// Mark the target page as modified - in case it was fresh
				target_page->page_clear = OFF;

				break;
		} // switch (k)


		// Restore the track copy buffer state
		TRACK_COPY_BUFFER = temp;

#ifdef COPY_BUFFER_FRESH
		TRACK_COPY_BUFFER = 0;
#endif
	}








	// ALIGN LAUFLICHT
	if (	( keyNdx == KEY_PLAY1 )
		&&	( G_run_bit == ON )
		){

		// Track selection gets retriggered
		if ( target_page->trackSelection != 0 ){

			// Retrigger the tracks in Track selection
			for ( i=0; i<MATRIX_NROF_ROWS; i++ ){

				// Act only on the selected tracks, if playing
				if (	( target_page->trackSelection & (1<<i) )
					&&	( target_page->Track[i]->attr_LOCATOR != 0 )
					){

					target_page->Track[i]->frame_ndx = 0;
					target_page->Track[i]->TTC = 0;
				}
			}
		}
		else{

			// Aligns only the target_page to the global play data
			set_page_locators( target_page, G_global_locator, G_TTC_abs_value );
		}
	}



	// PROGRAM CHANGE command
	if ( keyNdx == KEY_PROGRAM ){

		// Only act if there is just one track selected
		if ( my_bit_cardinality( target_page->trackSelection ) == 1 ){

			// Send the program change Value on the selected track(s)
			for (i=0; i<MATRIX_NROF_ROWS; i++) {

				if (	( (target_page->trackSelection & (1<<i) ) != 0 )
					&&	( target_page->Track[ my_bit2ndx( target_page->trackSelection )]->program_change > 0 )
					){

					MIDI_send( 	MIDI_PGMCH,
								target_page->Track[i]->attr_MCH
								+ target_page->Track[i]->event_offset[ATTR_MIDICH],
								target_page->Track[ my_bit2ndx( target_page->trackSelection )]->program_change - 1,
								0 );
				}
			}
		} // only one track is selected
	}



	//
	// EDIT MASTER
	//
	if (keyNdx == KEY_EDIT_MASTER) {

		// D O U B L E - C L I C K   C O D E
		if (DOUBLE_CLICK_TARGET == keyNdx) {
			// Nothing to do
		}

		// Step_activity is ON: turn it OFF or SELECT it
		else if (DOUBLE_CLICK_TARGET == 0) {

			DOUBLE_CLICK_TARGET = keyNdx;
			DOUBLE_CLICK_TIMER = ON;
			// Start the Double click Alarm
			cyg_alarm_initialize(
				doubleClickAlarm_hdl,
				cyg_current_time() + DOUBLE_CLICK_ALARM_TIME,
				DOUBLE_CLICK_ALARM_TIME );

			// SINGLE CLICK CODE
			// React only when a single track is selected
			if ( my_bit_cardinality( target_page->trackSelection ) == 1 ){

				// Switch the edit mode to CONTROL;  0:EDIT  1:CONTROL
				target_page->editorMode = MIX2EDIT;

				// Assign the mix2edit values as needed;
				// Compute the index first:
				switch( my_bit2ndx( target_page->trackSelection ) ){

					case 0:	target_page->attr_mix2edit = NEMO_ATTR_PITCH;		break;
					case 1:	target_page->attr_mix2edit = NEMO_ATTR_VELOCITY;	break;
					case 2:	target_page->attr_mix2edit = NEMO_ATTR_LENGTH;		break;
					case 3:	target_page->attr_mix2edit = NEMO_ATTR_START;		break;
				}
				// target_page->attr_mix2edit = 1 + my_bit2ndx( target_page->trackSelection );

				// Assign the min and max values
				switch( target_page->attr_mix2edit ){

					case NEMO_ATTR_VELOCITY:
						target_page->attr_mix2edit_MIN = TRACK_MIN_VELOCITY;
						target_page->attr_mix2edit_MAX = TRACK_MAX_VELOCITY;
						break;

					case NEMO_ATTR_PITCH:
						target_page->attr_mix2edit_MIN = TRACK_MIN_PITCH;
						target_page->attr_mix2edit_MAX = TRACK_MAX_PITCH;
						break;

					case NEMO_ATTR_LENGTH:
						target_page->attr_mix2edit_MIN = TRACK_MIN_LENFACTOR;
						target_page->attr_mix2edit_MAX = TRACK_MAX_LENFACTOR;
						break;

					case NEMO_ATTR_START:
						target_page->attr_mix2edit_MIN = TRACK_MIN_STAFACTOR;
						target_page->attr_mix2edit_MAX = TRACK_MAX_STAFACTOR;
						break;

					case NEMO_ATTR_POSITION:
						target_page->attr_mix2edit_MIN = TRACK_MIN_POSITION;
						target_page->attr_mix2edit_MAX = TRACK_MAX_POSITION;
						break;

					case NEMO_ATTR_DIRECTION:
						target_page->attr_mix2edit_MIN = TRACK_MIN_DIRECTION;
						target_page->attr_mix2edit_MAX = TRACK_MAX_DIRECTION;
						break;

					case NEMO_ATTR_AMOUNT:
						target_page->attr_mix2edit_MIN = TRACK_MIN_AMOUNT;
						target_page->attr_mix2edit_MAX = TRACK_MAX_AMOUNT;
						break;

					case NEMO_ATTR_GROOVE:
						target_page->attr_mix2edit_MIN = TRACK_MIN_GROOVE;
						target_page->attr_mix2edit_MAX = TRACK_MAX_GROOVE;
						break;

					case NEMO_ATTR_MIDICC:
						target_page->attr_mix2edit_MIN = TRACK_MIN_MIDICC;
						target_page->attr_mix2edit_MAX = TRACK_MAX_MIDICC;
						break;

					case NEMO_ATTR_MIDICH:
						target_page->attr_mix2edit_MIN = TRACK_MIN_MIDICH;
						target_page->attr_mix2edit_MAX = TRACK_MAX_MIDICH;
						break;
				} // switch on attr_mix2edit to get the min and max values

			} // only one track is selected
		} // Single click code
	} // keyNdx == EDIT_MASTER



	//
	// TRANSPORT functions - when nothing is selected in page
	//
	switch( keyNdx ){

		case KEY_STOP:
			if ( G_clock_source != EXT ){

				sequencer_command_STOP();
			}
			break;


		case KEY_PAUSE:
			// PAUSE SELECTED TRACK(S)

			// Make sure we are running on internal clock.
			if ( G_clock_source == EXT ) break;

			// Determine the chain divisor
			switch( target_page->chainMode ){

				case CHAINMODE_1:	j = 1;		break;
				case CHAINMODE_2:	j = 2;		break;
				case CHAINMODE_3:	j = 4;		break;
				case CHAINMODE_4:	j = 8;		break;
			}

			// PAUSE the tracks in Track selection
			for ( i=0; i<MATRIX_NROF_ROWS; i++ ){

				// Act only on the selected tracks that are playing
				if (	(	( target_page->trackSelection & (1<<i) )
						&&	( target_page->Track[i]->attr_LOCATOR != 0 ) )
					||	( target_page->Track[i]->attr_TEMPOMUL == 0 )
					){

					// Prepare the TTC to be wrapped
					target_page->Track[i]->TTC = 12;

					// Parameters are: page, row, chain divisor
					advance_track_TTC( 	target_page, target_page->Track[i] );

					// Now play step in track at new location: page, row, column
					// Check if the step is set first..
					if ( Step_get_status(
							target_page->Step[i][target_page->Track[i]->attr_LOCATOR-1],
							STEPSTAT_TOGGLE == ON )
						){

						target_page->Track[i]->attr_TEMPOMUL = 1;

						PLAYER_preview_step(
							target_page, i, target_page->Track[i]->attr_LOCATOR-1 );
					}

					// Finally pause the track: tempomul = 0
					target_page->Track[i]->attr_TEMPOMUL = 0;
				}
			}
			break;


		case KEY_PLAY1:
		case KEY_PLAY2:
		case KEY_PLAY4:

			// Depending on what you are holding down: start play or set tempo
			if ( is_pressed_key( KEY_SELECT_MASTER ) == TRUE ){

				// Set the tempo of the selected tracks
				// .. in Octopus, but not in NEMO
			}
			else if ( G_clock_source != EXT ){

				// Start playing the sequencer
				sequencer_command_PLAY();

				// UN-PAUSE the tracks in Track selection
				for ( i=0; i<MATRIX_NROF_ROWS; i++ ){

					// Finally pause the track: tempomul = 0
					if ( target_page->Track[i]->attr_TEMPOMUL == 0 ){
						target_page->Track[i]->attr_TEMPOMUL = 1;
					}
				}
			}
			break;


		case KEY_RECORD:

			// Allow for multiple track recording
			// Init the REC selection
			Page_setTrackRecPattern(target_page, 0);

			// Row iterator - place all selected tracks into the REC selection
			for ( i=0; i < MATRIX_NROF_ROWS; i++ ){

				// If track is selected
				if ( target_page->trackSelection & (1 << i) ){

					// Place its playing track into the REC_selection
					Page_setTrackRecPatternBit( target_page,  row_of_track( target_page, target_page->Track[i]->chain_data[PLAY]) );
				}
			}
			// Void the track selection
			target_page->trackSelection = 0;

			break;
	}





