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
unsigned int selected_solo_rec_page( unsigned char grid_cursor, unsigned char dot ){

	if ( SOLO_has_rec ){
		return 0; // new page chains cannot be added if there is a recording
	}

	if ( Page_repository[grid_cursor].page_clear == ON && is_pressed_key(dot) &&
	   ( dot < 20 || // first column
			   ( SOLO_rec_page == NULL && Page_repository[grid_cursor - 10].page_clear == ON) ||
				 SOLO_rec_page != NULL
	   ) &&
	   ( SOLO_rec_page == NULL || SOLO_rec_page->pageNdx == (grid_cursor - 10) )){

		return 1;
	}
	return 0;
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

// apply a track mute toggle for the entire page cluster
void apply_page_cluster_track_mute_toggle( Pagestruct* target_page, Trackstruct* current_track ){

	// select all pages in cluster
	Pagestruct* temp_page = &Page_repository[ target_page->pageNdx ];
	Trackstruct* next_track = current_track;

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

	// track forward to set the mute pattern
	while ( 	(this_ndx < MAX_NROF_PAGES) &&
			(Page_repository[this_ndx].page_clear == OFF)
	){
		temp_page = &Page_repository[ this_ndx ];
		next_track = temp_page->Track[next_track->trackId % 10];
		apply_page_track_mute_toggle( temp_page, next_track, &temp_page->trackMutepattern );
		this_ndx += 10;
	}
}

unsigned int selected_page_cluster_right_neighbor( Pagestruct* temp_page, unsigned char pageNdx )
{
	signed short	this_ndx = 0;

	// Compute the index of the right neighbor
	this_ndx = temp_page->pageNdx;
	if (Page_repository[this_ndx].pageNdx == pageNdx)
	{
		return 1;
	}
	if (this_ndx % 10 == MATRIX_NROF_COLUMNS)
	{
		return 0;
	}
	if (Page_repository[this_ndx + 10].page_clear == OFF && Page_repository[this_ndx + 10].pageNdx == pageNdx){
		return 1;
	}
	return 0;
}

// Indicates a page cluster selection
unsigned char selected_page_cluster_pressed( unsigned char grid_cursor, unsigned char prev_grid_cursor ){

	Pagestruct* temp_page = &Page_repository[ grid_cursor ];

	if ( prev_grid_cursor == grid_cursor || prev_grid_cursor - 10 == grid_cursor) {
		return 1;
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
		return 0;
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

			return column_of(grid_cursor);
		}
		this_ndx += 10;
	}
	return 0;
}

void stop_solo_rec(){

	sequencer_STOP( true );
	sequencer_RESET( false );
	if ( SOLO_rec_measure_hold == OFF ){
		SOLO_has_rec = ON;
	}
	G_track_rec_bit = OFF;
	G_measure_locator = OFF;
	SOLO_pos_marker_in = OFF;
	SOLO_pos_marker_out = OFF;
	SOLO_rec_measure_pos = OFF;
	// Reset the grid cursor for the recording page cluster
	if ( SOLO_rec_page != NULL ){
		reset_page_cluster( SOLO_rec_page, FALSE );
	}
}

void reset_page_cluster( Pagestruct* target_page, unsigned char resetTackSelections ){

	signed short 	prev_ndx = 0,
					this_ndx = 0;

	Pagestruct* temp_page = target_page;

	prev_ndx = target_page->pageNdx;
	if (!Page_repository[this_ndx].page_clear == ON) {

		// track back to beginning of cluster selection
		while ( 	(prev_ndx < MAX_NROF_PAGES) &&
				(Page_repository[prev_ndx].page_clear == OFF)
		){
			temp_page = &Page_repository[ prev_ndx ];
			temp_page->repeats_left = temp_page->attr_STA; // reset page repeats

			if ( resetTackSelections == TRUE ){
				Page_setTrackRecPattern(temp_page, 0);
				temp_page->trackSelection = 0;
			}

			this_ndx = temp_page->pageNdx;
			prev_ndx = (this_ndx >= 10) ?  this_ndx - 10 : 255;
		}
	}

	GRID_p_selection[ SOLO_rec_bank ] = temp_page;
	GRID_p_preselection[ SOLO_rec_bank ] = temp_page;
	GRID_p_clock_presel[ SOLO_rec_bank ] = temp_page;
	GRID_CURSOR = temp_page->pageNdx;
	follow_flag = FOLLOW_PAGE;
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

	if ( grid_cursor / 10 == 15 ) { // don't clear if col 16
		return;
	}

	if (Page_repository[prev_grid_cursor].page_clear == OFF) {
		selected_page_cluster_clear( grid_cursor );
	}

	// select all pages in cluster
	Pagestruct* temp_page = &Page_repository[ prev_grid_cursor ];
	Pagestruct* cursor_page = &Page_repository[ grid_cursor ];

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
