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




extern void 	switch_gridbank( unsigned char target_bank );
extern void 	advance_page_locators( Pagestruct* target_page );
extern void 	MIDI_send( 	unsigned char type,
							unsigned char val0,
							unsigned char val1,
							unsigned char val2 );
extern void 	selected_page_cluster_clear( unsigned char grid_cursor );
extern void 	selected_page_cluster_move( unsigned char grid_cursor, unsigned char prev_grid_cursor );


unsigned int normalize( signed int value,
						signed int min_limit,
						signed int max_limit ) {

	if ( value < min_limit ){
		return min_limit;
	}
	if ( value > max_limit) {
		return max_limit;
	}
	else {
		return value;
	}
}



// Moves the clock presel of a bank into its presel
void preselect_clock_presel( unsigned char target_bank ){

/*	d_iag_printf( ">> presel bank:%d :: %d->%d mode:%d\n",
			target_bank,
			GRID_p_selection[target_bank],
			GRID_p_preselection [target_bank],
			GRID_switch_mode );
*/

	// Move clock_presel into preselection
	GRID_p_preselection[ target_bank ] = GRID_p_clock_presel[ target_bank ];
}



// Computes the start points for a chain in the given bank -
// i.e. first page in current chain and puts them into the preselection.
void compute_chain_startpoints( unsigned char target_bank ){

	unsigned char 	j=0;
	signed short 	next_ndx = 0,
					this_ndx = 0;

	// Stop here if we are pointing to a null page in the bank..
	if ( GRID_p_selection[ target_bank ] == NULL ) return;

	// Remember the starting index
	this_ndx = GRID_p_selection[ target_bank ]->pageNdx;

	// If playmode CHAIN, select the first page in chain (furthest left page).
	if ( (GRID_bank_playmodes & ( 1 << target_bank )) != 0 ){

		// In given bank: select the leftmost neighbor by stepping up to it
		for (j=0; j < MATRIX_NROF_COLUMNS; j++) {

			// Compute the next left index
			next_ndx = this_ndx - (10 * (j+1));

			// d_iag_printf( "left: next_ndx:%d\n", next_ndx );

			// If the left neighbor is either empty or out of the bounds - break,
			// else implicitly continue.
			if (	( next_ndx < 0 ) ||
					( Page_repository[next_ndx].page_clear == ON )	){

				// Return to the last page with content
				next_ndx = this_ndx - (10 * (j));

				break;
			}

		} // leftie iterator

		// Select the neighbor as the next page
		GRID_p_clock_presel[ target_bank ] = &Page_repository[next_ndx];
	}
}





// Computes the preselection for a given GRID bank
void compute_chain_presel( unsigned char target_bank ){

	unsigned char 	j=0;
	signed short 	next_ndx = 0,
					this_ndx = 0;

	// If a manual "clock-" preselection was made..
	if ( 	( GRID_p_clock_presel[ target_bank ] != GRID_p_preselection[ target_bank ]

		// ..(but not not in the case where the bank is not playing at all)
		&&	( GRID_p_preselection[ target_bank ] != NULL )) // FIXME: is this the correct fix?
		){
		
		// break - want to keep it that way.
		return;
	}

	// If bank already active..
	if ( (GRID_p_selection[target_bank]  !=  NULL) ){
		// If playmode CHAIN, select the next page for play.
		// For now take the neighbor to the right, and if none, the furthest left
		if (GRID_bank_playmodes & ( 1 << target_bank )){
			// Decrease the "repeats" value. If it is not 0, break;
			if ( !G_save_song_pos ) {
				GRID_p_selection[ target_bank ]->repeats_left--;
				G_last_ctrl_offset[ target_bank ]++;
			} else {
				if (GRID_p_selection[ target_bank ]->repeats_left >= 1) {
					GRID_p_selection[ target_bank ]->repeats_left--;
					G_last_ctrl_offset[ target_bank ]++;
				} else {
					GRID_p_selection[ target_bank ]->repeats_left = GRID_p_selection[ target_bank ]->attr_STA;
					G_last_ctrl_offset[ target_bank ] = 0;
				}
			}

			// Re-trigger /continuum switch - continuum for length 16(DEF) and more
			if ( GRID_p_selection[ target_bank ]->attr_LEN < PAGE_DEF_LEN ){

				// Stop playing the old page if it was not empty..
				stop_playing_page( GRID_p_selection[target_bank],	G_TTC_abs_value, prev_G_pause_bit );

				// ..and set its locators to 0 for the next round of play.
				set_track_locators( GRID_p_selection[target_bank], NULL, 0, 0 );
			}

			// If the page still has repeats left to play
			if ( GRID_p_selection[ target_bank ]->repeats_left > 0 ){

				// Move on to the next bank and disregard the code below
				return;
			}

			// Compute the index of the right neighbor
			this_ndx = GRID_p_selection[ target_bank ]->pageNdx;
			next_ndx = this_ndx + 10;

			// d_iag_printf( "this_ndx:%d\n", this_ndx );

			// RIGHT neighbor exists and has some content
			if ( 	(next_ndx < MAX_NROF_PAGES) &&
					(Page_repository[next_ndx].page_clear == OFF)
				){

				// Select the neighbor as the next page
				GRID_p_clock_presel[ target_bank ] = &Page_repository[next_ndx];

			} // Right neighbor exists and has content


			// LEFT -most neighbor needs to be found
			else {

				// In every bank/row: select the leftmost neighbor by stepping up to it
				for (j=0; j < MATRIX_NROF_COLUMNS; j++) {

					// Ensure we stay within bounds
					if (next_ndx > 9) {

						// Compute the next left index
						next_ndx = this_ndx - (10 * (j+1));

						// d_iag_printf( "left: next_ndx:%d\n", next_ndx );

						// If the left neighbor ins out of bounds or empty - break,
						// else implicitly continue.
						if (	( next_ndx < 0 ) ||
								( Page_repository[next_ndx].page_clear == ON )	){

							// Return to the last page with content
							next_ndx = this_ndx - (10 * (j));

							break;
						}

					}

				} // leftie iterator

				// Select the neighbor as the next page
				GRID_p_clock_presel[ target_bank ] = &Page_repository[next_ndx];

			} // Right neighbor is not a suitable follow-on

		} // GRID_bank_playmode is CHAIN


		// Reset track locators according to page length even when clusters not active.
		else if ( GRID_p_selection[ target_bank ]->locator == 1 ){

			// This is the re-trigger mode - continuum for length 16(DEF) and more
			if ( GRID_p_selection[ target_bank ]->attr_LEN < PAGE_DEF_LEN ){

				// Stop playing the old page if it was not empty..
				stop_playing_page( GRID_p_selection[target_bank],	G_TTC_abs_value, prev_G_pause_bit );

				// ..and set its locators to 0 for the next round of play.
				set_track_locators( GRID_p_selection[target_bank], NULL, 0, 0 );
			}
		}

	} // bank is active

}



// Activates or deactivates the PLAY (Snapshot) MODE
void PLAY_MODE_switch_new( unsigned char target_bank, unsigned char target_status ){


	switch( target_status ){

		case ON:
			// Send selection to PLAY MODE buffer
			if (	( GRID_p_selection[ target_bank ] 	!= NULL )
				&&	( PLAY_MODE_page[ target_bank ] 	!= NULL )
				){

				// Copy from source to target page..
				Page_copy( GRID_p_selection[target_bank], PLAY_MODE_page[target_bank] );
			}
			PLAY_MODE_STATUS = ON;
			break;


		case OFF:
			// Fetch PLAY MODE buffer pages into selection
			if (	( GRID_p_selection[ target_bank ] 	!= NULL )
				&&	( PLAY_MODE_page[ target_bank ] 	!= NULL )
				){

				// Copy from source to target page..
				Page_copy( PLAY_MODE_page[target_bank], GRID_p_selection[target_bank] );

				// Align imported page to the current locator
				GRID_p_selection[target_bank]->locator = G_global_locator;
				set_page_locators( GRID_p_selection[target_bank], G_global_locator, G_TTC_abs_value );
			}
			PLAY_MODE_STATUS = OFF;
			break;
	}

}


// Toggles the PLAY mode - global operation
void toggle_PLAY_MODE( unsigned char target_status ){

	unsigned char i = 0;

	// Iterate through the grid banks and apply the new status
	for ( i=0; i < GRID_NROF_BANKS; i++ ){

		// Toggle the play mode status for the bank
		PLAY_MODE_switch_new( i, target_status );
	}
}



// Send the program change stored in a single track
void send_track_program_change( Trackstruct* target_track, Pagestruct* target_page ){

	Pagestruct* temp_page = NULL;
	Stepstruct* temp_step = NULL;
	unsigned char adjMCH, ctrl_bank;
	unsigned char n, row, i, j, k = 0;
	signed short next_ndx = 0;
	signed char track_offset = 0;

	// Make sure that the track has valid program change data
	if (	( target_track->program_change != 0 )
		){

		// control track
		if ( Track_get_MISC(target_track, CONTROL_BIT ) ) { // internal pgmch

			for (n = MATRIX_NROF_COLUMNS; n > 0; --n) { // read the event buffer from right to left
				temp_step = target_page->Step[target_track->trackId % 10][n];
				if ( Step_get_status( temp_step, STEPSTAT_SKIP ) == ON ){
					// step->phrase_num and track->velocity are interchangealbe and indicate the measure offset
					if ( target_track->ctrl_offset == temp_step->phrase_num ){ // is it time to fire this event?

						copy_ctrl_step_to_track(target_page, target_track, temp_step);

						if ( (target_page->trackMutepattern & (1<<row_of_track( target_page, target_track ))) == 0) { // is track muted
							// Find the pointer to the page in question
							adjMCH = 9 - target_track->attr_MCH;
							if ( target_track->program_change-1 == 0 ) {
								temp_page = &Page_repository[ adjMCH ]; // first grid column
							} else {
								temp_page = &Page_repository[ ((10 * (target_track->program_change-1)) + adjMCH) ];
							}
							track_offset = target_track->STA_factor;
							// inherit tempo
							if ( CHECK_BIT(target_track->attr_CCAMT, TRK_CTRL_TEMPO) ){
								temp_page->Track[track_offset]->attr_TEMPOMUL = target_track->attr_TEMPOMUL;
								temp_page->Track[track_offset]->attr_TEMPOMUL_SKIP = target_track->attr_TEMPOMUL_SKIP;

							} else if ( CHECK_BIT(target_track->attr_MISC, TRK_CTRL_MOVE) && G_pause_bit == OFF ){
								if ( prev_G_stop_bit == OFF ) {
									selected_page_cluster_move((target_track->bank_change / 10 == 0) ? target_track->bank_change : move_map[target_track->bank_change], temp_page->pageNdx);
								}

							} else if ( CHECK_BIT(target_track->attr_MISC, TRK_CTRL_CLEAR) && G_pause_bit == OFF ){
								if ( prev_G_stop_bit == OFF ) {
									selected_page_cluster_clear( temp_page->pageNdx );
								}

							} else if ( CHECK_BIT(target_track->attr_MISC, TRK_CTRL_MUT_PGTGL) ){
								temp_page->trackMutepattern ^= 1 << track_offset;

							} else { // Event Code -- DIR value

								unsigned char bank = temp_page->pageNdx % 10;

								switch( target_track->attr_DIR ){

								case 1: // Page TOGGLE

									switch( target_track->attr_MCH ){

									// Switch to the respective grid set, according to simulated selector
									case 10:

										current_GRID_set = target_track->program_change - 1;
										unsigned char mode = GRID_switch_mode;
										GRID_switch_mode = GRID_SWITCH_DIRECT;
										import_GRID_set( target_track->program_change - 1 );
										GRID_switch_mode = mode;
										select_page_preselections();
										break;

										// Toggle pages in the respective banks
									case 9: case 8: case 7: case 6: case 5: case 4: case 3: case 2: case 1:

										// Toggle the page playing status (this is reversed OFF is toggle ON)
										switch ( is_selected_in_GRID( temp_page ) && (!is_pre_selected_in_GRID( temp_page ) || G_scroll_bit ) ) {
										case ON:
											if ( prev_G_stop_bit == OFF || G_pause_bit == ON ) {
												GRID_p_preselection[ bank ] = NULL;
												GRID_p_clock_presel[ bank ] = NULL;
												switch_gridbank(bank);
											}
											break;

										case OFF:
											GRID_p_preselection[ bank ] = temp_page;
											GRID_p_clock_presel[ bank ] = temp_page;
											switch_gridbank(bank);
											GRID_p_selection[ bank ] = temp_page;
											GRID_p_selection[bank]->repeats_left = GRID_p_selection[bank]->attr_STA;
											break;
										}
										break;
									}
									break;

									case 2: // Mute all page clusters
										// only fires as the first event when the sequencer is stopped
										if ( G_run_bit == OFF && G_MIDI_timestamp == 0 ){
											for (i=0; i <= GRID_NROF_BANKS; i++) { // for each grid bank
												unsigned char stop = OFF;
												// In given bank: select the leftmost page
												for (j=0; j < MATRIX_NROF_COLUMNS; j++) {
													next_ndx = i + (10 * (j));

													if ( *&Page_repository[next_ndx].pageNdx == target_page->pageNdx )
													{
														break;
													}
													if ( Page_repository[next_ndx].page_clear == OFF ) {

														if ( stop ){
															Page_repository[next_ndx].trackMutepattern = 0; // unmute all tracks after start page
															target_page->trackMutepatternStored = 0;
														}
														else {
															for (k=0; k < MATRIX_NROF_ROWS; k++) {
																Page_repository[next_ndx].trackMutepattern |= 1 << k; // mute all tracks in the start page
															}
														}
														if (stop == OFF) {
															grid_select( &Page_repository[next_ndx], OFF  );
															stop = ON;
														}
													}
												}
											}
										}
										break;

									case 3: // Event based program/bank change using offset

										// Bank change - via controller 0
										if ( target_track->bank_change != 0 ){

											MIDI_send( 	MIDI_CC, target_track->attr_MCH, 0,	target_track->bank_change-1 );
										}

										// Program change as dedicated message
										MIDI_send( 	MIDI_PGMCH,
												normalize( target_track->attr_MCH + target_track->event_offset[ATTR_MIDICH],
														TRACK_MIN_MIDICH, TRACK_MAX_MIDICH ),
														target_track->program_change - 1,
														0 );
										break;
								}
							}

							// Not muted and not paused
							if ( (target_page->trackMutepattern & (1<<row_of_track( target_page, target_track ))) == 0 && G_pause_bit == OFF ) {

								// External MIDI Sequencer [start] FA
								if ( CHECK_BIT(target_track->attr_MISC, TRK_CTRL_EXT_PLAY) ) {

									if ( G_run_bit == ON ) {
										MIDI_send( MIDI_START,
												normalize( target_track->attr_MCH + target_track->event_offset[ATTR_MIDICH],
														TRACK_MIN_MIDICH, TRACK_MAX_MIDICH ), 0, 0);
									} else {
										MIDI_send( MIDI_STOP,
												normalize( target_track->attr_MCH + target_track->event_offset[ATTR_MIDICH],
														TRACK_MIN_MIDICH, TRACK_MAX_MIDICH ), 0, 0);
									}

									// External MIDI Sequencer [stop] FC
								} else if ( CHECK_BIT(target_track->attr_MISC, TRK_CTRL_EXT_STOP) ) {

									MIDI_send( MIDI_STOP,
											normalize( target_track->attr_MCH + target_track->event_offset[ATTR_MIDICH],
													TRACK_MIN_MIDICH, TRACK_MAX_MIDICH ), 0, 0);
								}
							}
						}
					}
				}
			}
		} else {

			// Bank change - via controller 0
			if ( target_track->bank_change != 0 ){

				MIDI_send( 	MIDI_CC, target_track->attr_MCH, 0,	target_track->bank_change-1 );
			}

			// Program change as dedicated message
			MIDI_send( 	MIDI_PGMCH,
					normalize( target_track->attr_MCH + target_track->event_offset[ATTR_MIDICH],
							TRACK_MIN_MIDICH, TRACK_MAX_MIDICH ),
							target_track->program_change - 1,
							0 );
		}
	}
}


// Send the program changes stored in the target page tracks
void send_program_changes( Pagestruct* target_page ){

	unsigned char row = 0;

	for ( row=0; row < MATRIX_NROF_ROWS; row++ ){

		// Send the PgmChanges for all HEAD rows in page
		if ( target_page->Track[row] == target_page->Track[row]->chain_data[HEAD] ){

			target_page->Track[row]->ctrl_offset = 0; // reset event offset counter
			// Send the program change associated with the particular track
			send_track_program_change( target_page->Track[row], target_page );

			// Volume level as dedicated message
			if (target_page->MIXAMT_VOL[row] != 64) {
				MIDI_send( 	MIDI_CC,
						target_page->Track[row]->attr_MCH
						+ target_page->Track[row]->event_offset[ATTR_MIDICH],
						7,
						target_page->MIXAMT_VOL[row] 	);
			}
		}
	}
}


//
// Performs a page switch within the given bank of the grid
//
void switch_gridbank( unsigned char target_bank ){

	// Remember the initial PLAYMODE status
	unsigned char PLAY_MODE_STATUS_original = PLAY_MODE_STATUS;

/*	d_iag_printf( ">> switch bank:%d :: %d->%d mode:%d\n",
			target_bank,
			GRID_p_selection[target_bank],
			GRID_p_preselection [target_bank],
			GRID_switch_mode );
*/

	// HALT the old page
	if ( GRID_p_selection[target_bank] != NULL && G_reset == ON) {
		G_reset = OFF;
		// PLAYMODE handling
		if (	( PLAY_MODE_STATUS_original == ON )
			){

			// Restore the old state of the page from PLAY buffer
			// First turn the PLAY mode OFF, then bring it ON again - down below
			PLAY_MODE_switch_new( target_bank, OFF );
		}

		// Stop playing the old page if it was not empty..
		stop_playing_page( GRID_p_selection[target_bank],	G_TTC_abs_value, prev_G_pause_bit );

		// ..and set its locators to 0 for the next round of play.
		set_track_locators( GRID_p_selection[target_bank], NULL, 0, 0 );

		// Set the page locator to 0, just to be consistent with using
		// a locator of 0 to indicate that page or track is not playing.
		GRID_p_selection[target_bank]->locator = 0;
	}

	// ACTIVATE the new page into the grid selection
	GRID_p_selection [target_bank] = GRID_p_preselection [target_bank];

	// Make sure we do not operate on an invalid page pointer
	if ( GRID_p_selection [target_bank] != NULL ){

		// Reset all track locators - may have changed
		// ..?
		// Wilson - Align imported page to the current locator ( fix Nemo 1 hit page toggle bug)
		if ( PLAY_MODE_STATUS_original == OFF ) {
			// This breaks the Octopus event timing
			 GRID_p_selection[target_bank]->locator = G_global_locator;
			 set_page_locators( GRID_p_selection[target_bank], G_global_locator, G_TTC_abs_value );
		}
		// Send the program change signals stored in the page tracks
		// Make sure we do not repeat what we have done in the Klaus Gstettner mode
		if (	( GRID_switch_mode == GRID_SWITCH_OCLOCK )
			&&	( GRID_p_selection [target_bank]->force_to_scale == TRUE )
			&&	( GRID_p_selection [target_bank]->scaleNotes[ G_scale_ndx ] == SCALE_SIG_CHR )
			){
			// The program change has already been sent out.. no further need
			// ..nothing to do!
		}
		else{
			if ( G_pause_bit == OFF ) {
				GRID_p_selection[ target_bank ]->repeats_left = GRID_p_selection[ target_bank ]->attr_STA;
				G_last_ctrl_page[ target_bank ] = GRID_p_selection[ target_bank ];
				G_last_ctrl_offset[ target_bank ] = 0;
				send_program_changes( GRID_p_selection[ target_bank ] );
			}
		}
		// Advance its locators once, moving them from 0 to 1, indicating activity
		advance_page_locators( GRID_p_selection[target_bank] );

		// PLAYMODE handling
		if (	( PLAY_MODE_STATUS_original == ON )
			){

			// Restore the old state of the page from PLAY buffer
			PLAY_MODE_switch_new( target_bank, ON );
		}
	}
}


//
// Make preselected pages into grid selections, on the clock or directly
// Depending on PAGE follow it may also switch the CURSOR
void select_page_preselections() {

	unsigned char i=0;

	// Quick and dirty fix for selecting pages in grid in stop mode
	unsigned char j = GRID_switch_mode;

	// Work on the page under the grid cursor
	Pagestruct* target_page = &Page_repository[GRID_CURSOR];

	if ( G_run_bit == OFF ){

		GRID_switch_mode = GRID_SWITCH_DIRECT;
	}

	// Iterate through grid bank preseletions looking for change requests
	for (i=0; i < GRID_NROF_BANKS; i++){

		// "Change wanted" condition - there is a new page in queue
		if ( GRID_p_selection[i] != GRID_p_preselection[i] ){

			// This is the one of the page, potential switch
			if ( 	( GRID_p_selection[i]->locator == 1 )
				&&	( GRID_p_selection[i] != NULL )
				){

				// Everybody switches on the ONE - ..both direct and oclock
				switch_gridbank( i );
			}
			else if (	( GRID_p_selection[i] == NULL )
					&&	( G_global_locator == 1 )
				){

				// Switch here out of the bank non-playing mode,
				// ..where the current selection is NULL
				switch_gridbank( i );
			}
			else if (	( GRID_switch_mode == GRID_SWITCH_DIRECT )
				){

				// Switch happens only when in DIRECT mode (or sequencer stopped),
				// ..and ensuring the bank switch is NOT "locked"
				switch_gridbank( i );
			}

			// Switch the VIEWER _page in zoomPAGE mode according to the change, if the row is affected
			//
			// Check the row of the current VIEWER _page and the other conditions
			if ( GRID_p_selection[i] != NULL ){

				if (	( 	(GRID_p_selection[i]->pageNdx % 10) == (target_page->pageNdx % 10) )
					&& 	( 	(G_zoom_level == zoomPAGE)
						|| 	(G_zoom_level == zoomMIXMAP) )
					&& 	( 	follow_flag == FOLLOW_PAGE )
					){

					// We are in the same row, so do the switch of the VIEWER _page
					target_page = GRID_p_selection[i];

					// Update the GRID CURSOR
					GRID_CURSOR = target_page->pageNdx;
				}
			}

		}// "Change wanted" condition

	} // GRID_NROF_BANKS iterator


	// Restore the status of GRID_switch_mode to original value
	if ( G_run_bit == OFF ){
		GRID_switch_mode = j;
	}

}


// Waiting function - used when sending MIDI. waiting for the given UART base.
void Player_wait( unsigned int in_UART_BASE ){

	unsigned int i=0;

	// Wait for the Transmit Buffer to clear
	while( 1 ){

		// Read the UART Status Register
		HAL_READ_UINT32( in_UART_BASE + E7T_UART_STAT, i );

		// Get the interesting byte's bit[7] and mask off the rest
		if ( (i & (1<<7)) != 0 ){

			// Buffer is now clear, exit loop
			break;
		}
	}
}

