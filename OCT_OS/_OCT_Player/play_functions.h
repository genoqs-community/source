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

#ifdef FEATURE_ENABLE_DICE
extern unsigned char	dice_page_locator( Pagestruct* target_page, unsigned char page_locator_value );
#endif

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
	if ( 	( GRID_p_clock_presel[ target_bank ] != GRID_p_preselection[ target_bank ] )

		// ..(but not not in the case where the bank is not playing at all)
//		&&	( GRID_p_preselection[ target_bank ] != NULL )
		){

		// break - want to keep it that way.
		return;
	}

	// If bank already active..
	if ( (GRID_p_selection[target_bank]  !=  NULL) ){

		unsigned char repeats_left = GRID_p_selection[ target_bank ]->repeats_left;
		// If playmode CHAIN, select the next page for play.
		// For now take the neighbor to the right, and if none, the furthest left
		if (GRID_bank_playmodes & ( 1 << target_bank )){

			#ifdef FEATURE_ENABLE_SONG_UPE
			// Decrease the "repeats" value. If it is not 0, break;
			if ( !G_save_song_pos ) {
				repeats_left = --GRID_p_selection[ target_bank ]->repeats_left;
				G_last_ctrl_offset[ target_bank ]++;
			} else {
				if (GRID_p_selection[ target_bank ]->repeats_left >= 1) {
					repeats_left = --GRID_p_selection[ target_bank ]->repeats_left;
					G_last_ctrl_offset[ target_bank ]++;
				} else {
					GRID_p_selection[ target_bank ]->repeats_left = GRID_p_selection[ target_bank ]->attr_STA;
					G_last_ctrl_offset[ target_bank ] = 0;
				}
			}

			// Re-trigger /continuum switch - continuum for length 16(DEF) and more
			if ( GRID_p_selection[ target_bank ]->attr_LEN < PAGE_DEF_LEN ){

				// Stop playing the old page if it was not empty..
				if ( repeats_left == 0 ){
					stop_playing_page( GRID_p_selection[target_bank],	G_TTC_abs_value, prev_G_pause_bit );
				}
				// ..and set its locators to 0 for the next round of play.
				set_track_locators( GRID_p_selection[target_bank], NULL, 0, 0 );
			}
			#else
			// Decrease the "repeats" value. If it is not 0, break;
			if( GRID_p_selection[ target_bank ]->repeats_left > 0 ) {
				repeats_left = --GRID_p_selection[ target_bank ]->repeats_left;
			}

			// Re-trigger /continuum switch - continuum for length 16(DEF) and more
			if ( GRID_p_selection[ target_bank ]->attr_LEN < PAGE_DEF_LEN ){

				// Stop playing the old page if it was not empty..
				if ( repeats_left == 0 ){

					stop_playing_page( GRID_p_selection[target_bank],	G_TTC_abs_value );
				}

				// ..and set its locators to 0 for the next round of play.
				set_track_locators( GRID_p_selection[target_bank], NULL, 0, 0 );
			}
			#endif
			// If the page still has repeats left to play
			if (	( repeats_left > 0 )
				|| 	( GRID_p_selection[ target_bank ]->attr_STA == 0 ) ){

				// Move on to the next bank and disregard the code below
				return;
			}

			// Compute the index of the right neighbor
			this_ndx = GRID_p_selection[ target_bank ]->pageNdx;
			next_ndx = this_ndx + 10;

			// d_iag_printf( "this_ndx:%d\n", this_ndx );

			#ifdef FEATURE_SOLO_REC
			// Free Flow recording -- add a page to the end of the cluster
			if ( G_zoom_level == zoomSOLOREC && SOLO_rec_freeflow == ON &&
				 next_ndx < MAX_NROF_PAGES && Page_repository[next_ndx].page_clear == ON)
			{
				create_next_freeflow_page_cluster(next_ndx);
				G_measure_locator = 1;
			}
			#endif

			// RIGHT neighbor exists and has some content
			if ( 	(next_ndx < MAX_NROF_PAGES) &&
					(Page_repository[next_ndx].page_clear == OFF)
				){

				// Select the neighbor as the next page
				GRID_p_clock_presel[ target_bank ] = &Page_repository[next_ndx];

				#ifdef FEATURE_SOLO_REC
				if ( G_zoom_level == zoomSOLOREC && target_bank == SOLO_rec_bank ){

					G_measure_locator = 1; // Reset the measure counter when the page changes
				}
				#endif

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

				#ifdef FEATURE_SOLO_REC
				if ( G_zoom_level == zoomSOLOREC ){
					if ( SOLO_pos_marker_in != OFF ||
						(G_track_rec_bit == ON &&
						 target_bank == SOLO_rec_bank &&
					   ((SOLO_rec_continue_recording == OFF && SOLO_rec_finalized == OFF) ||
					    (SOLO_rec_freeflow == ON && SOLO_rec_finalized == OFF))
						)){

						SOLO_rec_measure_pos--; // Move back one measure because we already incremented
						stop_solo_rec(FALSE); // We have reached the end of the recording so stop
						return;
					}

					SOLO_rec_finalized = ON;
					G_measure_locator = 1; // Reset the measure counter when the page cluster ends
					SOLO_rec_measure_pos = 1;
					Page_repository[next_ndx].repeats_left = Page_repository[next_ndx].attr_STA; // Reset page repeats
				}
				#endif

			} // Right neighbor is not a suitable follow-on

		} // GRID_bank_playmode is CHAIN


		// Reset track locators according to page length even when clusters not active.
		else if ( GRID_p_selection[ target_bank ]->locator == 1 ){

			// This is the re-trigger mode - continuum for length 16(DEF) and more
			if ( GRID_p_selection[ target_bank ]->attr_LEN < PAGE_DEF_LEN ){

				// Stop playing the old page if it was not empty..
				#ifdef FEATURE_ENABLE_SONG_UPE
				stop_playing_page( GRID_p_selection[target_bank],	G_TTC_abs_value, prev_G_pause_bit );
				#else
				stop_playing_page( GRID_p_selection[target_bank],	G_TTC_abs_value );
				#endif
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

	#ifdef FEATURE_ENABLE_SONG_UPE
	Pagestruct* temp_page = NULL;
	Stepstruct* temp_step = NULL;
	unsigned char adjMCH;
	unsigned char n, i, j, k = 0;
	signed short next_ndx = 0;
	signed char track_offset = 0;

	// FIXME this condition does not belong here for UPE
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

			// Make sure that the track has valid program change data
			if (	( target_track->program_change != 0 )   ){

				// Program change as dedicated message
				MIDI_send( 	MIDI_PGMCH,
					normalize( target_track->attr_MCH + target_track->event_offset[ATTR_MIDICH],
							TRACK_MIN_MIDICH, TRACK_MAX_MIDICH ),
							target_track->program_change - 1,
							0 );
			}
		}
	}
	#else

		// Bank change - via controller 0
		if ( target_track->bank_change != 0 ){

			MIDI_send( 	MIDI_CC, target_track->attr_MCH, 0,	target_track->bank_change-1 );
		}

		// Make sure that the track has valid program change data
		if (	( target_track->program_change != 0 )   ){
			// Program change as dedicated message
			MIDI_send( 	MIDI_PGMCH,
				normalize( target_track->attr_MCH + target_track->event_offset[ATTR_MIDICH],
						TRACK_MIN_MIDICH, TRACK_MAX_MIDICH ),
					target_track->program_change - 1,
					0 );
		}
	#endif
}

// Send the program changes stored in the target page tracks
void send_program_changes( Pagestruct* target_page ){

	unsigned char row = 0;

	for ( row=0; row < MATRIX_NROF_ROWS; row++ ){

		// Send the PgmChanges for all HEAD rows in page
		if ( target_page->Track[row] == target_page->Track[row]->chain_data[HEAD] ){
			#ifdef FEATURE_ENABLE_SONG_UPE
			target_page->Track[row]->ctrl_offset = 0; // reset event offset counter
			#endif
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
	#ifdef FEATURE_ENABLE_SONG_UPE
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
	#else
	// HALT the old page
	if ( GRID_p_selection[target_bank] != NULL ) {

		// PLAYMODE handling
		if (	( PLAY_MODE_STATUS_original == ON )
			){

			// Restore the old state of the page from PLAY buffer
			// First turn the PLAY mode OFF, then bring it ON again - down below
			PLAY_MODE_switch_new( target_bank, OFF );
		}

		// Stop playing the old page if it was not empty..
		stop_playing_page( GRID_p_selection[target_bank],	G_TTC_abs_value );

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
			send_program_changes( GRID_p_selection[ target_bank ] );
		}
		// Advance its locators once, moving them from 0 to 1, indicating activity
		advance_page_locators( GRID_p_selection[target_bank] );

		// Refill the page repeats value
		GRID_p_selection[target_bank]->repeats_left = GRID_p_selection[target_bank]->attr_STA;

		// PLAYMODE handling
		if (	( PLAY_MODE_STATUS_original == ON )
			){

			// Restore the old state of the page from PLAY buffer
			PLAY_MODE_switch_new( target_bank, ON );
		}
	}
	#endif
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

				if (	( 	grid_row(GRID_p_selection[i]->pageNdx) == grid_row(target_page->pageNdx) )
					&& 	( 	(G_zoom_level == zoomPAGE)
						|| 	(G_zoom_level == zoomSOLOREC)
						|| 	(G_zoom_level == zoomMIXMAP) )
					#ifdef NEMO
					&&	(	G_track_rec_bit == ON )
					#else
					&& 	( 	follow_flag == FOLLOW_PAGE )
					#endif
					){

					// We are in the same row, so do the switch of the VIEWER _page
					target_page = GRID_p_selection[i];

					// Update the GRID CURSOR
					GRID_CURSOR = target_page->pageNdx;

					#ifdef NEMO
					if( page_is_chain_follow( target_page ) ) {
						// Reset track window to top
						target_page->track_window = NEMO_WINDOW;
					}
					#endif
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

#ifdef FEATURE_ENABLE_CHORD_OCTAVE
// Fill phrase with step chord aux note data
void fill_phrase_pool( PhraseStruct* phraseObj, Stepstruct* target_step ) {

	unsigned char octave_mask = 0;
	unsigned char i = 0;
	unsigned char first_offset 	= 0;
	unsigned char second_offset	= get_chord_cardinality( target_step, CHORD_OCTAVE_FIRST );
	unsigned char third_offset 	= second_offset + get_chord_cardinality( target_step, CHORD_OCTAVE_SECOND );

	for ( i = 0; i < 12; i++ ) {
		octave_mask = get_chord_octave_mask( target_step, i );

		if( i && !octave_mask ) {
			continue;
		}

		if ( i == 0 || CHECK_MASK( octave_mask, CHORD_OCTAVE_FIRST ) ) {
			PhrasePtAddPit( phraseObj, first_offset++, i );
		}

		if ( CHECK_MASK( octave_mask, CHORD_OCTAVE_SECOND ) ) {
			PhrasePtAddPit( phraseObj, ++second_offset, i + 12);
		}

		if ( CHECK_MASK( octave_mask, CHORD_OCTAVE_THIRD ) ) {
			PhrasePtAddPit( phraseObj, ++third_offset, i + 24 );
		}
	}
}
#endif

// Check if trigger stack is unset (random pick)
bool is_trigger_stack_random( Trigger_Stack* trigger_stack ){

	return	( trigger_stack->trigger[0] | trigger_stack->trigger[1] | trigger_stack->trigger[2] | trigger_stack->trigger[3] | trigger_stack->trigger[4]
		    | trigger_stack->trigger[5] | trigger_stack->trigger[6] | trigger_stack->trigger[7] | trigger_stack->trigger[8] | trigger_stack->trigger[9] ) == 0;
}


// Helper check if track trigger is unset (random pick)
bool is_trigger_random( Trackstruct* target_track  ){

	return is_trigger_stack_random( &direction_repository[target_track->attr_DIR + target_track->event_offset[ATTR_DIRECTION]].trigger_set[target_track->frame_ndx] );
}


// Nemo x2 only - Follow chain when follow chain mode and crossing Nemo track window
void track_window_follow_chain( Pagestruct* target_page, Trackstruct* target_track ){
	#ifdef NEMO
	if( 	( target_page->trackSelection == 0 )
		&&	( page_is_chain_follow( target_page ) )
		&& 	( CHECK_BIT( target_page->track_window, row_of_track( target_page, target_track->chain_data[HEAD] ) )
		||  ( CHECK_BIT( target_page->track_window, row_of_track( target_page, target_track->chain_data[HEAD]->chain_data[PREV] ) ) ) )
		&& 	( !CHECK_BIT( target_page->track_window, row_of_track( target_page, target_track->chain_data[NEXT] ) ) ) ) {

		target_page->track_window = CHECK_MASK( target_page->track_window, NEMO_WINDOW ) ? NEMO_WINDOW << NEMO_NROF_ROWSHIFT : NEMO_WINDOW;
	}
	#endif
}


// Get a pointer to the flow factor array
unsigned char get_track_flow_factor( const unsigned char attribute, const unsigned char factor_type, const unsigned char col ) {

	#ifdef NEMO
	switch( attribute ) {
		case NEMO_ATTR_PITCH:
			switch( factor_type ) {
				case FACTOR_INTERVAL_TYPE_SEMITONE:
				return Track_PIT_factor_semitone[col];
				default:
				return Track_PIT_factor[col];
			}
			break;
		case NEMO_ATTR_VELOCITY:return Track_VEL_factor[col];
		case NEMO_ATTR_LENGTH: 	return Track_LEN_factor[col];
		case NEMO_ATTR_START: 	return Track_STA_factor[col];
		case NEMO_ATTR_AMOUNT: 	return Track_AMT_factor[col];
		case NEMO_ATTR_GROOVE:	return Track_GRV_factor[col];
		case NEMO_ATTR_MIDICC: 	return Track_MCC_factor[col];
	}
	#else
	switch( attribute ){
		case ATTR_PITCH:
			switch( factor_type ) {
				case FACTOR_INTERVAL_TYPE_SEMITONE:
				return Track_PIT_factor_semitone[col];
				default:
				return Track_PIT_factor[col];
			}
			break;
		case ATTR_VELOCITY: return Track_VEL_factor[col];
		case ATTR_LENGTH: 	return Track_LEN_factor[col];
		case ATTR_START: 	return Track_STA_factor[col];
		case ATTR_AMOUNT: 	return Track_AMT_factor[col];
		case ATTR_GROOVE:	return Track_GRV_factor[col];
		case ATTR_MIDICC: 	return Track_MCC_factor[col];
	}
	#endif
	return 0;
}

#ifdef FEATURE_ENABLE_DICE
// Dice synchronized attribute changes
void dice_synchronize() {

	if( dice_synced_attr == 0 ) {
		return;
	}

	unsigned char col = 0;
	unsigned char offset = 0;
	unsigned char j;
	unsigned char k;

	Trackstruct* target_dice = throw_dice( NULL );
	if( target_dice ) {
		unsigned char dice_selected_clock = Dice_get_selected_clock_type( target_dice );
		unsigned char dice_length = TRACK_MAX_LENGTH / ( 1 << target_dice->attr_LEN );

		if( dice_synced_attr != ATTR_MISC && Dice_is_quantizer( target_dice ) ) {
			// Dice action quantized to dice grid entry.
			if( G_global_locator % dice_length > 0 ){
				return;
			}
		}

		unsigned char i = 0;

		switch( dice_synced_attr ) {

		case ATTR_MISC:
			j = G_global_locator;
			k = G_TTC_abs_value;

			for ( i = 0; i < GRID_NROF_BANKS; i++ ){
				// If the page is currently playing
				if ( Dice_is_grid_row_diced( target_dice, i ) && GRID_p_selection[i] != NULL ){

					set_page_locators( 	target_dice->attr_STA + GRID_p_selection[i], j % ( target_dice->attr_STA + dice_length ), k );
				}
			}
		break;

		case ATTR_LOCATOR:
			i = dice_synced_value - 1;
			if( Dice_is_grid_row_diced( target_dice, i ) ) {
				j = G_global_locator;
				k = G_TTC_abs_value;

				// Resync previously diced grid row
				// If the page is currently playing
				if ( GRID_p_selection[i] != NULL ){

					set_page_locators( GRID_p_selection[i], j, k );
				}
			}
			DICE_bank->trackSelection ^= 1 << i;

			// Toggle run bit (active when track selection ).
			G_dice_run_bit = ( DICE_bank->trackSelection == 0 ) ? OFF : ON;
			break;

		case ATTR_LENGTH:
			col = dice_synced_value;
			if( col > 8 && col < 14 ){
				target_dice->attr_LEN = col - 9;
				target_dice->attr_TEMPOMUL = 0;
			}

			offset = max( 1, 1 << ( 4 - (  (target_dice->attr_LEN - 1 ) + 1 ) ) );
			target_dice->attr_STA = ROUNDUP( target_dice->attr_STA - ( offset - 1 ), offset );
			break;

		case ATTR_START:
			col = dice_synced_value;
			offset = max( 1, 1 << ( 4 - (  (target_dice->attr_LEN - 1 ) + 1 ) ) );
			target_dice->attr_STA = ROUNDUP( col - ( offset - 1 ), offset );

			break;

		case ATTR_DICE_BANK:
			SEL_DICE_BANK = dice_synced_value;
			break;

		case NEMO_ATTR_G_master_tempoMUL:
			if( Dice_is_edit_tempo( target_dice ) ) {
				Dice_set_TEMPOMUL( target_dice, dice_synced_value, dice_selected_clock );
				Dice_set_TEMPOMUL_SKIP( target_dice, 0, dice_selected_clock );
			}
			break;

		case NEMO_ATTR_G_master_tempoMUL_SKIP:
			if( Dice_is_edit_tempo( target_dice ) ) {
				Dice_set_TEMPOMUL( target_dice, 1, dice_selected_clock );
				Dice_set_TEMPOMUL_SKIP( target_dice, dice_synced_value, dice_selected_clock );
			}
			break;
		}

		if( CHECK_BIT( DICE_bank->editorMode, DICE_ALIGN ) ) {
			// Resync dice page locators
			j = G_global_locator;
			k = G_TTC_abs_value;
			dice_length = TRACK_MAX_LENGTH / ( 1 << target_dice->attr_LEN );
			for ( i=0; i < GRID_NROF_BANKS; i++ ) {
				// If the page is currently playing
				if ( Dice_is_grid_row_diced( target_dice, i ) && GRID_p_selection[i] != NULL ) {

					set_page_locators( 	target_dice->attr_STA + GRID_p_selection[i], j % ( target_dice->attr_STA + dice_length ), k );
				}
			}
			CLEAR_BIT( DICE_bank->editorMode, DICE_ALIGN );
		}
	}

	dice_synced_attr = 0;
	dice_synced_value = 0;
}


// Return current Dice state NULL (inactive)
Trackstruct* throw_dice( Pagestruct* target_page ) {

	Trackstruct* dice = NULL;
	// Dice active for page target
 	if( ( target_page == NULL || ( G_dice_run_bit && ( DICE_bank->trackSelection & ( 1<<(target_page->pageNdx % 10 ) ) ) ) ) ){
 		dice = Dice_get();
 	}

 	return dice;
}


// Align Dice Global Clock locator
void dice_align_clock(Trackstruct* target_dice) {

	set_track_locators( GRID_p_selection[DICE_PAGE], target_dice, G_global_locator, G_TTC_abs_value );
}


// Is locator outside dice frame
bool dice_is_page_locator_skipped( Pagestruct* target_page, const unsigned char locator ) {

	bool skipped = false;
	Trackstruct* target_dice = throw_dice( target_page );

	if( target_dice ) {
		unsigned char length = target_page->attr_LEN / ( 1 << target_dice->attr_LEN );
		skipped	= !( locator >= target_dice->attr_STA && locator < ( target_dice->attr_STA + length ) );
	}

	return skipped;
}


// Get Dice clock offset
const unsigned char dice_clock_offset( Pagestruct * target_page, unsigned char row, unsigned char clock_type ) {

	unsigned char clock_offset = 0;

	Trackstruct* target_dice = throw_dice( target_page );
	if( ( target_page != DICE_bank ) ) {
		switch( clock_type ) {
		case ATTR_G_master_tempoMUL:
			return Dice_get_TEMPOMUL( target_dice, DICE_TRACK_CLOCK );
		case ATTR_G_master_tempoMUL_SKIP:
			return Dice_get_TEMPOMUL_SKIP( target_dice, DICE_TRACK_CLOCK );
		default:
			return 0;
		}
	}

	return clock_offset;
}

// Apply clock offset to to track tempoMUL and tempoMUL_SKIP
const DiceOffsetClock dice_apply_clock_offset( Pagestruct * target_page, unsigned char row ) {

	Trackstruct* target_track = target_page->Track[row];

	DiceOffsetClock dice_clock;
	if( target_track ) {
		dice_clock.attr_TEMPOMUL = target_track->attr_TEMPOMUL;
		dice_clock.attr_TEMPOMUL_SKIP = target_track->attr_TEMPOMUL_SKIP;
	}

	Trackstruct* target_dice = throw_dice( target_page );
	if( target_dice && target_page != DICE_bank ) {
		unsigned char dice_multiplier_offset = 0;
		unsigned char dice_divisor_offset = 0;
		signed char dice_clock_delta = 0;
		Trackstruct* target_track = target_page->Track[row];

		switch( target_track->attr_TEMPOMUL_SKIP & 0x0F ) {

			case 0:
				if( target_track->attr_TEMPOMUL != 15 ) {
					dice_divisor_offset = dice_clock_offset( target_page, row, ATTR_G_master_tempoMUL_SKIP );
					if( dice_divisor_offset > 0 ) {
						dice_clock_delta = target_track->attr_TEMPOMUL - dice_divisor_offset;

						if( dice_clock_delta > 0 ) {
							dice_clock.attr_TEMPOMUL = dice_clock_delta;
						} else {
							dice_clock_delta = dice_clock_delta != 0 ? dice_clock_delta : -1;
							dice_clock.attr_TEMPOMUL = 1;
							dice_clock.attr_TEMPOMUL_SKIP =
									( target_track->attr_TEMPOMUL_SKIP & 0xF0 )
								|	( min( 1 - ( dice_clock_delta + 1 ), 14 ) );

						}
					} else {
						dice_multiplier_offset = max( 0, dice_clock_offset( target_page, row, ATTR_G_master_tempoMUL ) - 1 );
						dice_clock.attr_TEMPOMUL = min( target_track->attr_TEMPOMUL + dice_multiplier_offset, 14 );
					}
				}
				break;

			default:
				if( target_track->attr_TEMPOMUL != 25 ) {

					dice_multiplier_offset = max( 0, dice_clock_offset( target_page, row, ATTR_G_master_tempoMUL ) - 1 );
					if( dice_multiplier_offset > 0 ) {

						dice_clock_delta = ( target_track->attr_TEMPOMUL_SKIP & 0x0F ) - dice_multiplier_offset;

						if( dice_clock_delta > 0 ) {
							dice_clock.attr_TEMPOMUL_SKIP =
									( target_track->attr_TEMPOMUL_SKIP & 0xF0 )
									|	( min( dice_clock_delta, 14 ) );
						} else {
							dice_clock_delta = dice_clock_delta != 0 ? dice_clock_delta : -1;
							dice_clock.attr_TEMPOMUL_SKIP = 0;
							dice_clock.attr_TEMPOMUL = min( 1 - ( dice_clock_delta + 1 ), 14 );
						}
					} else {
						dice_divisor_offset = dice_clock_offset( target_page, row, ATTR_G_master_tempoMUL_SKIP );
						dice_clock.attr_TEMPOMUL_SKIP =
								( target_track->attr_TEMPOMUL_SKIP & 0xF0 )
							|	( min( dice_divisor_offset + ( target_track->attr_TEMPOMUL_SKIP & 0x0F ), 14 ) );
					}
				}
				break;
		}
	}

	return dice_clock;
}



// Get Dice attribute flow offset
signed int dice_attr_flow_offset( Pagestruct * target_page, const unsigned char attribute, const card8 track_locator ) {

	signed int dice_offset = 0;

	Trackstruct* target_dice = throw_dice( target_page );

	if( target_page != DICE_bank && target_dice ) {
		bool is_dice_global_clock = Dice_get_MISC( target_dice, DICE_GLOBAL_CLOCK );

		// Select scaled locator (global / track)
		unsigned char locator = is_dice_global_clock ? max( 1, Dice_get_global_clock_track()->attr_LOCATOR ) : track_locator;

		// Calculate dice note pitch offset.
		unsigned char dice_span = is_dice_global_clock ? TRACK_MAX_LENGTH : 1 << ( 4 - target_dice->attr_LEN );

		// Index of flow pitch.
		unsigned char dice_flow_position = REMAP_RANGE( 0, dice_span - 1, 0, 15, locator - 1 );

		// Attribute factor.
		unsigned char attr_factor = Dice_get_factor( target_dice, attribute );

		// Calculate ATTR offset Remap range.
		unsigned char factor_offset = attr_factor < 17 ? attr_factor : attr_factor - 16;

		DiceFlowAttributes flow_attributes = Dice_get_flow_attributes( attribute );

		if( Dice_is_flow_attribute_flat( attribute ) ) {
			// Behave as straight offset multiplier.
			dice_offset = normalize( get_track_flow_factor( attribute, flow_attributes.attr_INTERVAL, factor_offset ) * flow_attributes.attr_SCALE, -flow_attributes.attr_MAX, flow_attributes.attr_MAX );
		}
		else {

			// Get dice flow pitch scaled by factor
			dice_offset = normalize( Dice_get_flow_attribute_value( attribute, dice_flow_position ) * get_track_flow_factor( attribute, FACTOR_INTERVAL_TYPE_DEFAULT, factor_offset ) / flow_attributes.attr_NEUTRAL, -flow_attributes.attr_MAX, flow_attributes.attr_MAX );
		}

		dice_offset = attr_factor < 17 ? dice_offset : -dice_offset;
	}

	return dice_offset;
}
#endif
