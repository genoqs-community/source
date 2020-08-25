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



// Only allow selection of right neighbor pages that are not connecting to an existing page cluster
unsigned char selected_solo_rec_page( unsigned char held, unsigned char pressedNdx ){

	if ( SOLO_has_rec || grid_row(held) == 9 ){
		return OFF; // new page chains cannot be added if there is a recording
	}

	if ( Page_repository[held].page_clear == ON &&
	   ( SOLO_rec_page == NULL &&
	   (
	   ( grid_col(held) == 0 && Page_repository[grid_ndx_next_col(held)].page_clear == ON ) ||
	   ( grid_col(held) == 15 && Page_repository[grid_ndx_prev_col(held)].page_clear == ON ) ||
	   ( Page_repository[grid_ndx_prev_col(held)].page_clear == ON && Page_repository[grid_ndx_next_col(held)].page_clear == ON )
	   ))) {

		return ON;
	}
	else if ( SOLO_rec_page != NULL && SOLO_rec_page->pageNdx == grid_ndx_prev_col(held))
	{
		return ON;
	}
	return OFF;
}

// apply a track mute toggle for a single page
void apply_page_track_mute_toggle( Pagestruct* target_page, Trackstruct* current_track, unsigned short*	trackMutepattern ){

	// MUTE operation depending on the chainstatus - Head or Segment
	// Head: Check the chain status: mute all tracks in the same chain (new model)
	// Segment: each track handled on its own.

	unsigned char temp;
	unsigned short mutepattern = *trackMutepattern;

	// Depending on the way we choose the track base..
	switch( target_page->CHAINS_PLAY_HEAD ){

		case TRUE:
			// Act as toggle on the full chain
			// How long is the chain?
			temp = cardinality_of_chain( current_track );

			// Loop through the chain of the selected track and mute all chain tracks
			int i;
			for ( i=0; i < temp; i++ ){

				mutepattern ^= ( 1 << row_of_track( target_page, current_track ));
				current_track = current_track->chain_data[NEXT];
			}
			break;

		// Act only on the individual track
		case FALSE:
			mutepattern ^=
				( 1 << row_of_track( target_page, current_track ));
			break;
	}

	*trackMutepattern = mutepattern;
}

// Computers target page cluster start index (or if not clustered own page index)
signed short find_cluster_start_index( Pagestruct* target_page ) {

	Pagestruct* temp_page = &Page_repository[ target_page->pageNdx ];
	signed short 	prev_ndx = 0,
					this_ndx = 0;

	// Compute the index of the right neighbor
	this_ndx = temp_page->pageNdx;
	if (Page_repository[this_ndx].page_clear == ON) {
		return this_ndx;
	}

	prev_ndx = (this_ndx >= 10) ?  this_ndx - 10 : 255;

	// Track back to beginning of cluster selection
	while ( 	(prev_ndx < MAX_NROF_PAGES) &&
			(Page_repository[prev_ndx].page_clear == OFF)
	){
		temp_page = &Page_repository[ prev_ndx ];
		this_ndx = temp_page->pageNdx;
		prev_ndx = (this_ndx >= 10) ?  this_ndx - 10 : 255;
	}

	return this_ndx;
}

// Apply a track mute toggle for the entire page cluster
void apply_page_cluster_track_mute_toggle( Pagestruct* target_page, Trackstruct* current_track, unsigned char operation_flags ){

	// select all pages in cluster
	Pagestruct* temp_page = &Page_repository[ target_page->pageNdx ];
	Trackstruct* next_track = current_track;

	signed short this_ndx = find_cluster_start_index( temp_page );

	// track forward to set the mute pattern
	while ( 	(this_ndx < MAX_NROF_PAGES) &&
			(Page_repository[this_ndx].page_clear == OFF)
	){
		temp_page = &Page_repository[ this_ndx ];
		next_track = temp_page->Track[next_track->trackId % 10];
		if ( CHECK_BIT( operation_flags, OPERATION_MUTE ) ) {
			apply_page_track_mute_toggle( temp_page, next_track, &temp_page->trackMutepattern );
			temp_page->trackMutepatternStored = temp_page->trackMutepattern;
		} else {
			apply_page_track_mute_toggle( temp_page, next_track, &temp_page->trackSolopattern );
		}
		this_ndx += 10;
	}
}

// Apply a mute pattern for the entire page cluster
void apply_page_cluster_mute_pattern( Pagestruct* target_page, unsigned short pattern, unsigned char operation_flags ){

	// select all pages in cluster
	Pagestruct* temp_page = &Page_repository[ target_page->pageNdx ];
	signed short this_ndx = find_cluster_start_index( temp_page );

	// track forward to set the mute pattern
	while ( 	(this_ndx < MAX_NROF_PAGES) &&
			(Page_repository[this_ndx].page_clear == OFF)
	){
		temp_page = &Page_repository[ this_ndx ];
		if ( CHECK_BIT( operation_flags, OPERATION_MUTE ) ) {
			temp_page->trackMutepattern = pattern;
			if (pattern) {
				temp_page->trackMutepatternStored = pattern;
			}
		} else {
			temp_page->trackSolopattern = pattern;
		}
		this_ndx += 10;
	}
}

unsigned int is_page_in_cluster( Pagestruct* temp_page, unsigned char pageNdx )
{
	unsigned char	this_ndx = 0;
	unsigned char	prev_ndx = 0;

	this_ndx = temp_page->pageNdx;

	// Scan pageNdx forwards
	prev_ndx = this_ndx;
	while (		( prev_ndx < MAX_NROF_PAGES )
			&&	( Page_repository[prev_ndx].page_clear == OFF )
	){
		if ( Page_repository[ prev_ndx ].pageNdx == pageNdx ) {
			return 1;
		}
		prev_ndx = prev_ndx + 10;
	}

	// Scan pageNdx backwards
	prev_ndx = this_ndx - 10;
	while (		( prev_ndx < MAX_NROF_PAGES ) // -1 wrap
			&&	( Page_repository[prev_ndx].page_clear == OFF )
	){
		if ( Page_repository[ prev_ndx ].pageNdx == pageNdx ) {
			return 1;
		}
		prev_ndx = prev_ndx - 10;
	}

	return 0;
}

// Indicates a page cluster selection
unsigned char selected_page_cluster_pressed( unsigned char grid_cursor, unsigned char prev_grid_cursor ){

	Pagestruct* temp_page = &Page_repository[ grid_cursor ];

	if ( prev_grid_cursor == grid_cursor || prev_grid_cursor - 10 == grid_cursor) {

		if ( Page_repository[prev_grid_cursor - 10].page_clear == ON && prev_grid_cursor != grid_cursor ){
			return OFF;
		}
		return ON;
	}

	signed short	this_ndx = 0,
					prev_ndx = 0;

	// Compute the index of the right neighbor
	this_ndx = temp_page->pageNdx;
	prev_ndx = ( this_ndx > 10 ) ? this_ndx - 10 : 255;

	if ( prev_ndx < prev_grid_cursor - 10 ) {
		return 0;
	}

	while ( 	(this_ndx > 0) &&
				(Page_repository[this_ndx].page_clear == OFF)
		){
		if ( this_ndx < 10 ) {
			return 0;
		}
		this_ndx -= 10;
		if ( this_ndx == prev_grid_cursor ) {
			return 1;
		}
	}

	return 0;
}

// clears a selected page cluster
void selected_page_cluster_clear( unsigned char grid_cursor ){

	// select all pages in cluster
	Pagestruct* temp_page = &Page_repository[ grid_cursor ];

	signed short 	prev_ndx = 0,
					this_ndx = 0;

	// Compute the index of the right neighbor
	this_ndx = temp_page->pageNdx;
	if (Page_repository[this_ndx].page_clear == ON) {
		return;
	}

	prev_ndx = (this_ndx >= 10) ?  this_ndx - 10 : 255;

	// track back to beginning of cluster selection
	while ( 	(prev_ndx < MAX_NROF_PAGES) &&
			(Page_repository[prev_ndx].page_clear == OFF)
	){
		temp_page = &Page_repository[ prev_ndx ];
		this_ndx = temp_page->pageNdx;
		prev_ndx = (this_ndx >= 10) ?  this_ndx - 10 : 255;
	}

	// track forward to clear
	while ( 	(this_ndx < MAX_NROF_PAGES) &&
			(Page_repository[this_ndx].page_clear == OFF)
	){
		Page_clear( &Page_repository[ this_ndx ] );
		this_ndx += 10;
	}
}

unsigned char count_to_last_page_in_grid_row( unsigned char target_page ){

	return 16 - (target_page / MATRIX_NROF_ROWS);
}

unsigned char last_page_in_grid_row( unsigned char target_page ){

	return target_page % MATRIX_NROF_ROWS + 150;
}

void copy_page_right( unsigned char target_page ){

	unsigned char last = last_page_in_cluster(target_page) + 10;
	Page_copy( &Page_repository[ target_page ], &Page_repository[ last ] );
	recPageCopy( grid_col(target_page), grid_col(last) );
}

unsigned char last_page_in_cluster( unsigned char target_page ){

	Pagestruct* temp_page = &Page_repository[ target_page ];

	signed short 	this_ndx = 0;
	unsigned char	last_page = last_page_in_grid_row(target_page);

	this_ndx = temp_page->pageNdx;

	// track forward
	while ( 	(this_ndx <= last_page) &&
			(Page_repository[this_ndx].page_clear == OFF)
	){
		this_ndx += 10;
	}

	return this_ndx - 10;
}

unsigned char has_empty_grid_row_ahead( unsigned char target_page ){

	Pagestruct* temp_page = &Page_repository[ target_page ];

	signed short 	this_ndx = 0;
	unsigned char	last_page = last_page_in_grid_row(target_page);

	if ( target_page % 10 == 9 ) // don't show the grid scene row
	{
		return FALSE;
	}

	this_ndx = temp_page->pageNdx;

	// track forward
	while ( 	(this_ndx < last_page) &&
			(Page_repository[this_ndx].page_clear == ON)
	){
		this_ndx += 10;
	}

	return this_ndx == last_page;
}

unsigned char has_space_to_copy_page_cluster_right ( unsigned char target_page ){
	unsigned char start = grid_col(first_page_in_cluster(target_page));
	unsigned char end = grid_col(last_page_in_cluster(target_page));
	return end + 1 + (end - start) < 15;
}

unsigned char first_page_in_cluster( unsigned char target_page ){

	Pagestruct* temp_page = &Page_repository[ target_page ];

	signed short 	prev_ndx = 0,
					this_ndx = 0;

	this_ndx = temp_page->pageNdx;
	prev_ndx = (this_ndx >= 10) ?  this_ndx - 10 : 255;

	// track back to beginning of cluster selection
	while ( 	(prev_ndx < MAX_NROF_PAGES) &&
			(Page_repository[prev_ndx].page_clear == OFF)
	){
		temp_page = &Page_repository[ prev_ndx ];
		this_ndx = temp_page->pageNdx;
		prev_ndx = (this_ndx >= 10) ?  this_ndx - 10 : 255;
	}

	return this_ndx;
}

void assign_solorec_track_midi_ch( unsigned char target_page ){

	Pagestruct* temp_page = &Page_repository[ target_page ];

	signed short 	prev_ndx = 0,
					this_ndx = 0,
					row;

	this_ndx = temp_page->pageNdx;
	prev_ndx = (this_ndx >= 10) ?  this_ndx - 10 : 255;

	// track back to beginning of cluster selection
	while ( 	(prev_ndx < MAX_NROF_PAGES) &&
			(Page_repository[prev_ndx].page_clear == OFF)
	){
		temp_page = &Page_repository[ prev_ndx ];
		this_ndx = temp_page->pageNdx;
		prev_ndx = (this_ndx >= 10) ?  this_ndx - 10 : 255;
	}

	// track forward
	while ( 	(this_ndx < MAX_NROF_PAGES) &&
			(Page_repository[this_ndx].page_clear == OFF)
	){
		for ( row=0; row < MATRIX_NROF_ROWS; row++ ){
			Page_repository[this_ndx].Track[row]->attr_MCH = SOLO_midi_ch;
		}
		this_ndx += 10;
	}

	SOLO_assistant_page->Track[ARP_BUFFER_TRACK]->attr_MCH = SOLO_midi_ch;
}

// Is the cursor a member of the target page cluster
// Returns the column of the grid cursor
unsigned char selected_page_cluster( unsigned char grid_cursor, unsigned char target_page ){

	// select all pages in cluster
	Pagestruct* temp_page = &Page_repository[ grid_cursor ];

	signed short 	prev_ndx = 0,
					this_ndx = 0;

	// Compute the index of the right neighbor
	this_ndx = temp_page->pageNdx;
	if (Page_repository[this_ndx].page_clear == ON) {
		return NOP;
	}

	prev_ndx = (this_ndx >= 10) ?  this_ndx - 10 : 255;

	// track back to beginning of cluster selection
	while ( 	(prev_ndx < MAX_NROF_PAGES) &&
			(Page_repository[prev_ndx].page_clear == OFF)
	){
		temp_page = &Page_repository[ prev_ndx ];
		this_ndx = temp_page->pageNdx;
		prev_ndx = (this_ndx >= 10) ?  this_ndx - 10 : 255;
	}

	// track forward
	while ( 	(this_ndx < MAX_NROF_PAGES) &&
			(Page_repository[this_ndx].page_clear == OFF)
	){
		if ( Page_repository[ this_ndx ].pageNdx == target_page ){

			return grid_col(grid_cursor);
		}
		this_ndx += 10;
	}
	return NOP;
}

void stop_solo_rec( unsigned char trim, unsigned char stop ){

	if ( SOLO_pos_marker_in != OFF ){
		SOLO_pos_marker_out = SOLO_rec_measure_pos;
		SOLO_pos_out = &Page_repository[GRID_CURSOR];
	}

	flush_note_on_queue( &Page_repository[GRID_CURSOR], SOLO_midi_ch );

	if ( SOLO_rec_rehearsal == OFF && SOLO_rec_measure_hold == OFF ){
		freeflowOff( trim );
	}

	saveOrUndoTranspose();

	if (stop){
		sequencer_STOP( true );
		sequencer_RESET( false );
		// Reset all locators in assistant page
		set_page_locators( SOLO_assistant_page, 0, 0 );
		G_track_rec_bit = OFF;
	} else {
		play_MIDI_queue( G_MIDI_timestamp );
		rebuild_undo_using_rec_notes();
		if ( SOLO_rec_page ){
			SOLO_edit_buffer_volatile = OFF;
		}
		G_track_rec_bit_latch = ON;
	}

	SOLO_rec_finalized				= ON;
	G_measure_locator				= OFF;
	SOLO_rec_measure_pos 			= OFF;
	SOLO_rec_rehearsal				= OFF;
	SOLO_scale_chords_arp_cursor 	= NOP;
	SOLO_transpose_latch 			= OFF;
	SOLO_rec_measure_hold_latch		= OFF;
//	SOLO_rec_track_preview			= SOLOGRID;
	SOLO_scale_chords_prev_on_ndx	= NOP;
	G_skip_step = NULL;
	SOLO_rec_CLOCKSTART_OTM			= OFF;
	SOLO_rec_record_OTM				= OFF;

	if ( SOLO_scale_chords_program == ON ){
		SOLO_rec_measure_hold = OFF;
		GRID_CURSOR = SOLO_rec_page->pageNdx;
	}

	// Reset the grid cursor for the recording page cluster
	if ( SOLO_rec_page != NULL && stop ){
		reset_page_cluster( SOLO_rec_page );
	}

	if ( SOLO_pos_marker_in != OFF ){
		cut_by_pos_markers();
	}
}

void clear_pages_left( Pagestruct* temp_page ){

	signed short 	prev_ndx = 0,
					this_ndx = 0;

	Pagestruct* start_page = temp_page;

	this_ndx = temp_page->pageNdx;
	prev_ndx = (this_ndx >= 10) ?  this_ndx - 10 : 255;

	// track back to beginning of cluster selection
	while ( 	(prev_ndx < MAX_NROF_PAGES) &&
			(Page_repository[prev_ndx].page_clear == OFF)
	){
		temp_page = &Page_repository[ prev_ndx ];
		Page_clear( temp_page );
		this_ndx = temp_page->pageNdx;
		prev_ndx = (this_ndx >= 10) ?  this_ndx - 10 : 255;
	}

	SOLO_rec_page = start_page;
	GRID_p_selection[ SOLO_rec_bank ] = start_page;
	GRID_p_preselection[ SOLO_rec_bank ] = start_page;
	GRID_p_clock_presel[ SOLO_rec_bank ] = start_page;
	GRID_CURSOR = start_page->pageNdx;
}

void clear_pages_right( Pagestruct* temp_page ){

	signed short 	prev_ndx = 0,
					this_ndx = 0;

	prev_ndx = temp_page->pageNdx;
	this_ndx = prev_ndx += 10;

	// track forward
	while ( 	(this_ndx < MAX_NROF_PAGES) &&
			(Page_repository[this_ndx].page_clear == OFF)
	){
		temp_page = &Page_repository[this_ndx];
		Page_clear( temp_page );
		this_ndx += 10;
	}
}

void reset_page_cluster( Pagestruct* temp_page ){

	signed short 	prev_ndx = 0,
					this_ndx = 0,
					m, n, i;
	Pagestruct* first_page;


	if ( G_run_bit == ON ){
		return;
	}

	// Remove the pause	state for good;
	G_pause_bit 	= OFF;

	// Reset the global locator
	G_global_locator = 0;

	SOLO_rec_CLOCKSTART_OTM = OFF;

	this_ndx = temp_page->pageNdx;
	prev_ndx = (this_ndx >= 10) ?  this_ndx - 10 : 255;

	// track back to beginning of cluster selection
	while ( 	(prev_ndx < MAX_NROF_PAGES) &&
			(Page_repository[prev_ndx].page_clear == OFF)
	){
		temp_page = &Page_repository[ prev_ndx ];
		this_ndx = temp_page->pageNdx;
		prev_ndx = (this_ndx >= 10) ?  this_ndx - 10 : 255;
	}

	temp_page = &Page_repository[this_ndx];
	first_page = temp_page;

	// PAGE PLAY from grid selection: each bank has at most one active page
	for ( i=0; i < GRID_NROF_BANKS; i++ ){

		if ( SOLO_page_play_along[ i ] != NOP ){
			continue;
		}

		GRID_p_selection[ i ] = NULL;
		GRID_p_preselection[ i ] = NULL;
		GRID_p_clock_presel[ i ] = NULL;
	}

	GRID_p_selection[ SOLO_rec_bank ] = temp_page;
	GRID_p_preselection[ SOLO_rec_bank ] = temp_page;
	GRID_p_clock_presel[ SOLO_rec_bank ] = temp_page;
	GRID_CURSOR = temp_page->pageNdx;
	SOLO_transpose_GRID_CURSOR = GRID_CURSOR;
	follow_flag = FOLLOW_PAGE;

	SOLO_rec_measure_count = 0;

	// track forward
	while ( 	(this_ndx < MAX_NROF_PAGES) &&
			(Page_repository[this_ndx].page_clear == OFF)
	){

		temp_page = &Page_repository[this_ndx];

		// reset first
		temp_page->attr_STA = Rec_repository[grid_col(temp_page->pageNdx)].measure_count;
		temp_page->repeats_left = temp_page->attr_STA; // reset page repeats

		SOLO_rec_measure_count += (unsigned short) temp_page->attr_STA;

		// reset first
		remove_track_chain(temp_page);
		Page_setTrackRecPattern(temp_page, 0);

		temp_page->trackSelection = 0;
		set_page_locators( temp_page, 0, 0 );

		set_track_locators( temp_page, NULL, 0, 0 );
		temp_page->locator = 0;

		m = MATRIX_NROF_ROWS - temp_page->attr_STA; // from the bottom up

		for (n=9; n >= m; --n) { // each measure
			temp_page->trackSelection |= ( 1 << n );
			temp_page->Track[n]->attr_MCH = SOLO_midi_ch;
		}

		chain_selected_tracks( temp_page );
		Page_setTrackRecPatternBit( temp_page, (n+1) );

		temp_page->trackSelection = 0;
		set_page_locators( temp_page, 0, 0 );

		set_track_locators( temp_page, NULL, 0, 0 );
		temp_page->locator = 0;

		this_ndx += 10;
	}
}

// copies a selected page cluster
void selected_page_cluster_copy( unsigned char grid_cursor, unsigned char prev_grid_cursor ){

	selected_page_cluster_clear( grid_cursor );

	// select all pages in cluster
	Pagestruct* temp_page = &Page_repository[ prev_grid_cursor ];

	unsigned char 	orig_grid_cursor = grid_cursor;
	unsigned char 	last_cpy_prev_on = OFF;
	signed short 	prev_ndx = 0,
					this_ndx = 0;

	// Compute the index of the right neighbor
	this_ndx = temp_page->pageNdx;
	prev_ndx = (this_ndx >= 10) ?  this_ndx - 10 : 255;

	// track back to beginning of cluster selection
	while ( 	(prev_ndx < MAX_NROF_PAGES) &&
			(Page_repository[prev_ndx].page_clear == OFF)
	){
		temp_page = &Page_repository[ prev_ndx ];
		this_ndx = temp_page->pageNdx;
		prev_ndx = (this_ndx >= 10) ?  this_ndx - 10 : 255;
	}

	// track forward to copy
	while ( 	(this_ndx < MAX_NROF_PAGES) &&
			(Page_repository[this_ndx].page_clear == OFF)
			&& (grid_cursor < MAX_NROF_PAGES)
			&& this_ndx != orig_grid_cursor
	){
		if ( Page_repository[ grid_cursor ].page_clear == OFF ) {
			// last one on
			last_cpy_prev_on = ON;
		} else {
			last_cpy_prev_on = OFF;
		}
		Page_copy( &Page_repository[ this_ndx ], &Page_repository[ grid_cursor ] );
		this_ndx += 10;
		grid_cursor += 10;
	}

	if ( last_cpy_prev_on ) {
		while ( 	(grid_cursor < MAX_NROF_PAGES) &&
				(Page_repository[grid_cursor].page_clear == OFF)
		){
			Page_clear( &Page_repository[ grid_cursor ] );
			grid_cursor += 10;
		}
	}

}


// moves a selected page cluster
void selected_page_cluster_move( unsigned char grid_cursor, unsigned char prev_grid_cursor ){

	if (Page_repository[prev_grid_cursor].page_clear == OFF) {
		selected_page_cluster_clear( grid_cursor );
	}

	// select all pages in cluster
	Pagestruct* temp_page = &Page_repository[ prev_grid_cursor ];
	Pagestruct* cursor_page = &Page_repository[ grid_cursor ];

	unsigned char 	i;
	unsigned char 	orig_grid_cursor = grid_cursor;
	unsigned char 	last_cpy_prev_on = OFF;
	signed short 	prev_ndx = 0,
					this_ndx = 0;

	// Compute the index of the right neighbor
	this_ndx = temp_page->pageNdx;
	prev_ndx = (this_ndx >= 10) ?  this_ndx - 10 : 255;

	// track back to beginning of cluster selection
	while ( 	(prev_ndx < MAX_NROF_PAGES) &&
			(Page_repository[prev_ndx].page_clear == OFF)
	){
		temp_page = &Page_repository[ prev_ndx ];
		this_ndx = temp_page->pageNdx;
		prev_ndx = (this_ndx >= 10) ?  this_ndx - 10 : 255;
	}

	// track forward to copy
	while ( 	(this_ndx < MAX_NROF_PAGES) &&
			(Page_repository[this_ndx].page_clear == OFF)
			&& (grid_cursor < MAX_NROF_PAGES)
			&& this_ndx != orig_grid_cursor
	){
		if ( Page_repository[ grid_cursor ].page_clear == OFF ) {
			// last one on
			last_cpy_prev_on = ON;
		} else {
			last_cpy_prev_on = OFF;
		}
		Page_copy( &Page_repository[ this_ndx ], &Page_repository[ grid_cursor ] );
		// Move page toggle with cluster
		if ( GRID_p_preselection[ this_ndx%10 ] == &Page_repository[ this_ndx ] ) {
			GRID_p_preselection[ this_ndx%10 ] = NULL;
			GRID_p_clock_presel[ this_ndx%10 ] = NULL;
			cursor_page = &Page_repository[ grid_cursor ];
			cursor_page->repeats_left = Page_repository[ this_ndx ].attr_STA;
			// Add page to the GRID_preselection directly
			GRID_p_preselection[ grid_cursor%10 ] = cursor_page;
			// Propagate the change over to clock presel
			GRID_p_clock_presel[ grid_cursor%10 ] = GRID_p_preselection[ grid_cursor%10 ];
			select_page_preselections();
		} else if ( GRID_p_preselection[ grid_cursor%10 ] == &Page_repository[ grid_cursor ] ) {
			// clear selection if move target is on but source is not
			GRID_p_preselection[ grid_cursor%10 ] = NULL;
			GRID_p_clock_presel[ grid_cursor%10 ] = NULL;
			GRID_p_selection_buffer[ grid_cursor%10 ] = NULL;
			set_track_locators( &Page_repository[ grid_cursor ], NULL, 0, 0 );
			select_page_preselections();
		}

		// Clear the GRID set for this page if it was selected
		for (i=0; i < GRID_NROF_SETS; i++){

			if ( GRID_p_set[i][grid_row(this_ndx)]->pageNdx == prev_grid_cursor ){

				GRID_p_set[i][grid_row(this_ndx)] = NULL;
			}
		}

		// clear the old locator
		if ( this_ndx%10 != grid_cursor%10 ) {
			GRID_p_selection_buffer[ this_ndx%10 ] = NULL;
			set_track_locators( &Page_repository[ this_ndx ], NULL, 0, 0 );
		}
		Page_clear( &Page_repository[ this_ndx ] );
		this_ndx += 10;
		grid_cursor += 10;
	}

	// RHS overflow on copy - clear remaining ghost pages
	if ( grid_cursor >= MAX_NROF_PAGES &&
			(Page_repository[this_ndx].page_clear == OFF)
	){
		while ( 	(this_ndx < MAX_NROF_PAGES) &&
				(Page_repository[this_ndx].page_clear == OFF)
				&& (orig_grid_cursor != this_ndx)
		){
			Page_clear( &Page_repository[ this_ndx ] );
			this_ndx += 10;
		}
	}

	if ( last_cpy_prev_on ) {
		// right clear existing on cluster clobber
		while ( 	(grid_cursor < MAX_NROF_PAGES) &&
				(Page_repository[grid_cursor].page_clear == OFF)
		){
			Page_clear( &Page_repository[ grid_cursor ] );
			grid_cursor += 10;
		}
	}

#ifdef FEATURE_ENABLE_SONG_UPE
	// --------------------------------------------------------------------------------
	// Control Track Reference Page Selection
	//
	if ( MIX_TRACK != NULL && CHECK_BIT(MIX_TRACK->attr_MISC, TRK_CTRL_MIX) && Track_get_MISC(MIX_TRACK, CONTROL_BIT) ){
		MIX_TRACK->bank_change = (orig_grid_cursor / 10 == 0) ?  orig_grid_cursor : move_page_map[orig_grid_cursor];
		CLEAR_BIT(MIX_TRACK->attr_MISC, TRK_CTRL_MIX);
		SET_BIT(MIX_TRACK->attr_MISC, TRK_CTRL_MOVE);
	}
	// --------------------------------------------------------------------------------
#endif
}
