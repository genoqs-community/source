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
// UPDATE TRACK LOCATOR of given track - related functions.
//



// Takes a track, and returns the locator value following the given curr_locator
unsigned char get_next_tracklocator( 	Trackstruct* target_track,
										unsigned char curr_locator ){

	// Connect Length to Start
	unsigned char oldLocator 	= 	curr_locator;
	unsigned char next_locator 	= 	0;
	unsigned char length 		= 	TRACK_DEF_LENGTH;
	unsigned char offset 		= 	oldLocator;

	switch( 	target_track->attr_DIR
			+ 	target_track->event_offset[ATTR_DIRECTION] ){

		case DIRECTION_LINEAR:
			// Increment Offset
			offset = (offset % length) + 1;
			break;

		case DIRECTION_REVERSE:
			break;

		case DIRECTION_RETURN:
			break;

		case DIRECTION_RANDOM:
			break;

		case DIRECTION_GAME:
			break;
	}

	// Compute the next locator
	next_locator = offset;

//	// Signal that there was a wrap
//	if ( next_locator < curr_locator ){
//		next_locator += 100;
//	}

	return next_locator;
}



// Makes given track the HEAD track in the chain of the entry track
void update_trackchain_HEAD( 	Pagestruct* target_page,
								Trackstruct* entry_track,
								Trackstruct* new_HEAD_track ){

	unsigned char i=0;
	Trackstruct* current_track = NULL;

	// Get the entry into the chain
	current_track = entry_track;

	// Update the HEAD info in the full chain.
	for (i=0; i < MATRIX_NROF_ROWS; i++ ){

		// Update the play info in current track
		current_track->chain_data[HEAD] = new_HEAD_track;

		// Set all locators except play track to 0;
		if ( current_track != current_track->chain_data[PLAY] ){

			current_track->attr_LOCATOR = 0;
			current_track->frame_ndx = 0;
		}

		// Advance the current track along the chain
		current_track = current_track->chain_data[NEXT];
	}
}



// Sequential search, very slow
Pagestruct* page_of_step( Pagestruct* target_page, Stepstruct* target_step ){

	unsigned char row, col;
	unsigned char this_ndx = first_page_in_cluster(target_page->pageNdx);

	// track forward
	while ( 	(this_ndx < MAX_NROF_PAGES) &&
			(Page_repository[this_ndx].page_clear == OFF)
	){

		for ( row=0; row < MATRIX_NROF_ROWS; row++ ){

			for ( col=0; col < MATRIX_NROF_COLUMNS; col++ ){

				if ( target_step == target_page->Step[row][col] ){

					return target_page;
				}
			}
		}

		this_ndx += 10;
	}
	return NULL;
}



unsigned char column_of_step( Pagestruct* target_page, Stepstruct* target_step, unsigned char row ){

	unsigned char col=0;

	for ( col=0; col < MATRIX_NROF_COLUMNS; col++ ){

		if ( target_step == target_page->Step[row][col] ){
			break;
		}
	}
	return col;
}



// Take a page and a track pointer and return the row index of the given track
unsigned char row_of_track( Pagestruct* target_page, Trackstruct* target_track ){

	unsigned char row=0;

	for ( row=0; row < MATRIX_NROF_ROWS; row++ ){

		if ( target_track == target_page->Track[row] ){
			break;
		}
	}
	return row;
}



// Check if all steps in a track are skipped
unsigned char is_skipped_track( Pagestruct* target_page, unsigned char row ){

	unsigned char result = 0;
	unsigned char i = 0;

	result = TRUE;
	for ( i=0; i < MATRIX_NROF_COLUMNS; i++ ){

		if ( Step_get_status( target_page->Step[row][i], STEPSTAT_SKIP ) == OFF ){

			result = FALSE;
			break;
		}
	}

	return result;
}



// Makes given track the PLAY track in a chain of tracks
void update_trackchain_PLAY( 	Pagestruct* target_page,
								Trackstruct* entry_track,
								Trackstruct* new_PLAY_track ){

	#ifdef FEATURE_SOLO_REC
	if (( SOLO_rec_measure_hold == ON || SOLO_rec_measure_hold_OTM == ON ) && G_run_bit == ON ){
		return;
	}
	#endif

	unsigned char i=0;
	Trackstruct* current_track = NULL;

	// Get the entry into the chain
	current_track = entry_track;

// d_iag_printf( "0bba:\n" ); print_chain_data( target_page );

	// Get the next valid (unskipped) PLAY track, starting with the given one..
	for ( i=0; i < MATRIX_NROF_ROWS; i++ ){

		if ( is_skipped_track( target_page, row_of_track( target_page, new_PLAY_track )) == TRUE ){

			// d_iag_printf(" skipped PLAY track detected\n" );
			// Advance the chain data pointer
			new_PLAY_track = new_PLAY_track->chain_data[NEXT];
		}
		else {
			// Exit the loop
			i = MATRIX_NROF_ROWS;
		}
	}
	// d_iag_printf(" new_PLAY_track:%d\n", row_of_track( target_page, new_PLAY_track ) );


// d_iag_printf( "0bbb:\n" ); print_chain_data( target_page );

	// Update the PLAY info in the full chain.
	for (i=0; i < MATRIX_NROF_ROWS; i++ ){

		// Update the play info in current track.
		current_track->chain_data[PLAY] = new_PLAY_track;

		// If it's not itself, then set locator to 0
		if ( current_track != new_PLAY_track ){
			current_track->attr_LOCATOR = 0;
		}

		// Advance the current track along the chain
		current_track = current_track->chain_data[NEXT];
	}

// d_iag_printf( "0bb:\n" ); print_chain_data( target_page );

}




// Jumps over Steps that are skipped - by trigger and returns the new offset in the track given by row.
unsigned char jump_skipped_position( 	Pagestruct* target_page,
										unsigned char row,
										unsigned char candidate,
										unsigned char direction	) {

	unsigned char col = 0;

	if ( candidate == 0 ) return 0;

//if (row > 7)
//d_iag_printf( "JSP     jumping: in: can:%d dir:%d\n", candidate, direction );

	switch( direction ){

		case DIRECTION_LINEAR:	// == 1

			// Logic: if the next step is skipped, increment - else break.
			for ( col=0; col < MATRIX_NROF_COLUMNS; col++ ){

				// If new step in SKIP mode
				if ( Step_get_status( target_page->Step[row][ candidate-1 ], STEPSTAT_SKIP ) == ON ){

					// Increment offset
					candidate ++;
				}
				// If the candidate iterator has reached the track boundary
				else if ( candidate >= MATRIX_NROF_COLUMNS ){

					break;
				}
				// Step is simply not skipped
				else {

					// No (more) skip found, so break.
					break;
				}
			} // column iterator

			// Indicates that the entire track content is skipped
			if (	(col == MATRIX_NROF_COLUMNS)
				||	(candidate > TRACK_DEF_LENGTH )
				){

				// This will tell the caller to stop playing the track and move on.
				candidate = 0;
			}

//if (row > 7)
//d_iag_printf( "JSP     L-advanced candidate to:%d\n", candidate );

			break;


		case DIRECTION_REVERSE:	// == 2

			// Logic: if the next step is skipped, decrement again
			for ( col=0; col < MATRIX_NROF_COLUMNS; col++ ){

				// If new step in SKIP mode
				if ( Step_get_status( target_page->Step[row][candidate-1], STEPSTAT_SKIP ) == ON ){

					// Decrement offset if still in range
					if ( candidate > 0 ){
						 candidate --;
					}
					else{
						candidate = 16;
					}
				}
				else {

					// No (more) skip found, so break.
					break;
				}
			} // column iterator

			// Indicates that the entire track content is skipped
			if (	(col == MATRIX_NROF_COLUMNS)
				){

				// This tells the caller to stop playing the track and move on.
				candidate = 0;
			}
			// d_iag_printf( "jump fin offset:%d\n", candidate );

//if (row > 7)
//d_iag_printf( "JSP     R-advanced candidate to:%d\n", candidate );

			break;

	} // switch( direction );

	// Candidate now contains the return value

//if (row > 7)
//d_iag_printf( "JSP     jump result:%d\n", candidate );

	return candidate;
}



// Jumps over Steps that are skipped - by trigger and returns the new offset in the track given by row.
unsigned char jump_skipped_position_new( 	Pagestruct* target_page,
											Trackstruct* target_track,
											unsigned char candidate,
											unsigned char direction	) {

	unsigned char col = 0;
	unsigned char row = row_of_track( target_page, target_track );


	if ( candidate == 0 ) return 0;

	switch( direction ){

		case DIRECTION_LINEAR:		// == 1

			// Cases that need coverage with regard to skips:
			// 1. Next candidate NOT within bouunds
			// 2. Next candidate NOT skipped and NOT within bounds

			// Logic: if the next step is skipped, increment - else break.
			for ( col=0; col < MATRIX_NROF_COLUMNS; col++ ){

				// If the candidate iterator has reached the track boundary
				if ( candidate > MATRIX_NROF_COLUMNS ){

					break;
				}
				// If new step in SKIP mode
				else if ( Step_get_status( target_page->Step[row][ candidate-1 ], STEPSTAT_SKIP ) == ON ){

					// Increment offset
					candidate ++;
				}
				// Step is simply not skipped
				else {

					// No (more) skip found, so break.
					break;
				}
			}

			// Indicates that the entire track content is skipped
			if (	(col == MATRIX_NROF_COLUMNS)
				||	(candidate > TRACK_DEF_LENGTH )
				){

				// This will tell the caller to stop playing the track and move on.
				candidate = 0;
			}
			break;


		case DIRECTION_REVERSE:		// == 2

			// Logic: if the next step is skipped, decrement again
			for ( col=0; col < MATRIX_NROF_COLUMNS; col++ ){

				// If new step in SKIP mode
				if ( Step_get_status( target_page->Step[row][candidate-1], STEPSTAT_SKIP ) == ON ){

					// Decrement offset if still in range
					if ( candidate > 0 ){
						 candidate --;
					}
					else{
						// Candidate is 0, signals track end and suggests wrap
						//candidate = 16;
						candidate = 0;
						break;
					}
				}
				else {

					// No (more) skip found, so break.
					break;
				}
			} // column iterator

			// Indicates that all steps in track are skipped
			if (	(col == MATRIX_NROF_COLUMNS)
				){

				// This tells the caller to stop playing the track and move on.
				candidate = 0;
			}
			break;

	} // switch( direction );
	// Candidate now contains the return value

	return candidate;
}



// Jump to next position having active STEP trigger destination.
unsigned char jump_skipped_trigger_position(	Pagestruct* target_page,
												Trackstruct* target_track,
												unsigned char candidate,
												unsigned char direction ){

	if ( candidate == 0 ) return 0;

	unsigned char i = 0, j = 0, k = 0;
	unsigned char origin = candidate;
	unsigned char row = row_of_track( target_page, target_track );

	Trigger_Stack* trigger_stack = direction_repository[target_track->attr_DIR + target_track->event_offset[ATTR_DIRECTION]].trigger_set;

	for ( i = 0; i < MATRIX_NROF_COLUMNS; k = 0, i++){
		for ( j = 0; j < MATRIX_NROF_ROWS; j++ ){
			if( trigger_stack[candidate].trigger[j] ){
				if ( Step_get_status( target_page->Step[row][trigger_stack[candidate].trigger[j] - 1], STEPSTAT_SKIP ) == OFF ){
					#ifdef FEATURE_ENABLE_DICE
					if ( !dice_is_page_locator_skipped( target_page, trigger_stack[candidate].trigger[j] - 1 ) ){
						break;
					}
					#else
					break;
					#endif
				}
				k++;
			}
		}

		if ( j < MATRIX_NROF_ROWS ){
			break;
		}else{
			if ( k == 0 ){
				// is_trigger_stack_random
				if ( Step_get_status( target_page->Step[row][candidate - 1], STEPSTAT_SKIP ) == ON ){
					candidate = jump_skipped_position_new( target_page, target_track, candidate, direction );
				}
				break;
			}

			candidate = ( direction == DIRECTION_LINEAR ) ? ( origin + ( i + 1 ) ) % 17 : ( ( origin - i ) + 16 ) % 17;
		}
	}

	return candidate;
}

// Jump to next random (non skipped) position.
unsigned char get_random_position( Pagestruct* target_page, Trackstruct* target_track ){

	unsigned char col = 0;
	unsigned char row = row_of_track( target_page, target_track );

	unsigned char active_length = 0;
	unsigned char active_list[16];

	for ( col = 0; col < MATRIX_NROF_COLUMNS; col++ ){
		if ( Step_get_status( target_page->Step[row][col], STEPSTAT_SKIP ) == OFF ){
			active_list[active_length] = col;
			active_length++;
		}
	}

	unsigned char result = 1 + ( active_list[rand() % active_length] );

	#ifdef FEATURE_ENABLE_DICE
	if( dice_is_page_locator_skipped( target_page, result ) ) {
		result = 0;
	}
	#endif

	return result;
}


// Returns the next trigger of the given track - from its trigger stack
unsigned char get_next_trigger( Pagestruct* target_page, unsigned char row ){

	unsigned char i=0;
	unsigned char result = 0;
	unsigned char direction = DIRECTION_LINEAR;


	// Iterate through the current trigger stack of the track
	for (i=0; i < MATRIX_NROF_ROWS; i++){

		result = target_page->Track[row]->trigger[i];

		// Ignore trigger if its step is skipped
		if ( Step_get_status( target_page->Step[row][result-1], STEPSTAT_SKIP ) == ON ){

			// Consume the trigger so we can move on..
			target_page->Track[row]->trigger[i] = 0;

			// Ignore trigger because its step is skipped
			result = 0;
		}

		// First non-zero trigger is returned
		if ( result > 0 ){

			// Zero the consumed trigger
			target_page->Track[row]->trigger[i] = 0;

			// Determine the move direction - shortened for performance purposes
			if ( result < target_page->Track[row]->attr_LOCATOR ){
				direction = DIRECTION_REVERSE;
			}
			// Jump skipped positions - repeating position does not need skipping..
			if ( result != target_page->Track[row]->attr_LOCATOR ){

				result = jump_skipped_position( target_page, row, result, direction );
			}

			return result;
		}

	}

	return result;
}



// Returns the direction that a track naturally (by frame) wants to move in, starting from given frame_ndx
unsigned get_next_frame_direction( Pagestruct* target_page, Trackstruct* target_track, unsigned char frame_ndx ){

	unsigned char direction = 0;
	unsigned char certainty = 0;

	// Some syntactic simplification
	certainty = direction_repository[ 	target_track->attr_DIR
									+ 	target_track->event_offset[ATTR_DIRECTION] ]
						.trigger_set[ target_track->frame_ndx ]
						.certainty_next;

	// Determine direction
	if ( ((rand() % 100) ) < certainty ){

		// FORWARD moving result: direction is FORWARD
		direction = DIRECTION_LINEAR;
	}
	else{

		// BACKWARD moving result: direction is REVERSE
		direction = DIRECTION_REVERSE;
	}

	return direction;
}




// Returns the index of the next frame (i.e. trigger set) in a track - according to current certainty value
unsigned char get_next_frame_ndx( Pagestruct* target_page, Trackstruct* target_track ){

	unsigned char result = 0;
	unsigned char candidate = 0;
	unsigned char direction = 0;
	unsigned char certainty = 0;


//	if (	( target_page->pageNdx == 8 )
//		&&	( row_of_track( target_page, target_track ) == 9 )
//		){
//
//		d_iag_printf( "     candidate I:%d frame:%d loc:%d\n",
//			candidate, target_track->frame_ndx, target_track->attr_LOCATOR );
//	}

	// Extract the certainty of the next frame.
	// If we are coming from a stop state, use the certainty of the first un-skipped frame
	if (	( target_track->frame_ndx == 0 )
		||	( target_track->attr_LOCATOR == 0 )
		){

		// Assume you are on step 1 and jump forward from there to the next unskipped position.
		candidate = 1;
		//candidate = jump_skipped_position_new( target_page, target_track, candidate, DIRECTION_LINEAR );
		// Find next candidate and depending on certainty outcome the result
		result = candidate = jump_skipped_trigger_position( target_page, target_track, candidate, DIRECTION_LINEAR );
		// Extract the certainty from reached frame
		certainty = direction_repository[ 	target_track->attr_DIR
									+ 	target_track->event_offset[ATTR_DIRECTION] ]
						.trigger_set[ candidate ]
						.certainty_next;
	}
	else{

		// Extract the certainty
		certainty = direction_repository[ 	target_track->attr_DIR
									+ 	target_track->event_offset[ATTR_DIRECTION] ]
						.trigger_set[ target_track->frame_ndx ]
						.certainty_next;
	}


//	if (	( target_page->pageNdx == 8 )
//		&&	( row_of_track( target_page, target_track ) == 9 )
//		){
//
//		d_iag_printf( "     candidate II:%d\n", candidate );
//	}

	// Based on certainty, compute the next candidate and the move direction
	if ( certainty == 100 || ( ( ( rand() % 100) ) < certainty ) ){

		// FORWARD moving result: direction is FORWARD
		direction = DIRECTION_LINEAR;
		candidate = (target_track->frame_ndx +1) % 17;
	}
	else{

		// BACKWARD moving result: direction is REVERSE
		direction = DIRECTION_REVERSE;
		candidate = (target_track->frame_ndx + 16) % 17;
	}


//	if (	( target_page->pageNdx == 8 )
//		&&	( row_of_track( target_page, target_track ) == 9 )
//		){
//
//		d_iag_printf( "     candidate III:%d\n", candidate );
//	}


	// Jump over potentially skipped steps at new resulting position
	//result = jump_skipped_position_new( target_page, target_track, candidate, direction );
	if ( candidate != result ) {
		result = jump_skipped_trigger_position( target_page, target_track, candidate, direction );
	}
//	if (	( target_page->pageNdx == 8 )
//		&&	( row_of_track( target_page, target_track ) == 9 )
//		){
//
//		d_iag_printf( "     result returned:%d\n", result );
//	}

	//every 4th tick
	return result;
}



// Returns the next locator of a given track in the given page.
unsigned char get_next_trackposition( Pagestruct* target_page, Trackstruct* target_track ){

	signed   int result		=	0;	// Goes below 0 (becomes =-1) at some point in time
	unsigned int locator 	= 	target_track->attr_LOCATOR;
	unsigned char row 		= 	row_of_track( target_page, target_track );

	// Overcome the stop state of the track
	if (	( locator == 0 )
		||	( target_track->frame_ndx == 0 )
		){

		target_track->frame_ndx
			= get_next_frame_ndx( target_page, target_track );

		// Refill trigger stack of track - refill phase I
		memcpy( &target_track->trigger,
				&direction_repository[ target_track->attr_DIR
										+ target_track->event_offset[ATTR_DIRECTION] ]
					.trigger_set[ target_track->frame_ndx ].trigger,
				10 );

		if( is_trigger_random( target_track ) ){
			// No trigger data get random position
			target_track->trigger[1] = get_random_position( target_page, target_track );
		}
	}

	// Get next trigger from the current trigger stack
	result = get_next_trigger( target_page, row );



//	if (	( target_page->pageNdx == 8 )
//		&&	( row == 9 )
//		){
//
//		d_iag_printf( "next trigger (initl):%d\n", result );
//	}



	// 0 means trigger stack was empty, refresh it
	if ( result == 0 ){

		// Advance Trigger set ndx - according to certainty value in the track
		target_track->frame_ndx = get_next_frame_ndx( target_page, target_track );


//	if (	( target_page->pageNdx == 8 )
//		&&	( row == 9 )
//		){
//
//		d_iag_printf( "   new frame_ndx:%d\n", target_track->frame_ndx );
//	}


		// The frame index is wrapping has wrapped - signal the wrap
		if ( target_track->frame_ndx == 0 ){

			result = 0;
		}
		else{

			// Refill trigger stack of track - refill phase II
			memcpy( &target_track->trigger,
					&direction_repository[ target_track->attr_DIR
											+ target_track->event_offset[ATTR_DIRECTION] ]
					.trigger_set[ target_track->frame_ndx ].trigger,
					10 );

			if( is_trigger_random( target_track ) ){
				// No trigger data get random position
				target_track->trigger[1] = get_random_position( target_page, target_track );
			}
//	if (	( target_page->pageNdx == 8 )
//		&&	( row == 9 )
//		){
//
//		d_iag_printf( "   memcpy checkpoint\n" );
//	}


			// Get next trigger from the trigger stack
			result = get_next_trigger( target_page, row );

	//	if (	( target_page->pageNdx == 8 )
	//		&&	( row == 9 )
	//		){
	//
	//		d_iag_printf( "   result trigger:%d\n\n", result );
	//	}


//	if (	( target_page->pageNdx == 8 )
//		&&	( row == 9 )
//		){
//
//		d_iag_printf( "   result trigger:%d\n\n", result );
//	}

		} // Track did not want to wrap
	}


//	if (	( target_page->pageNdx == 8 )
//		&&	( row == 9 )
//		){
//
//		d_iag_printf( "next trigger (final):%d\n\n", result );
//	}

	return result;
}



// Advance the locator of one track.
// Returns the index of the track wrapped to, or same track, if not wrapped
//____________________________________________________________________________________________
Trackstruct* advance_track_locator( 	Pagestruct* target_page,
										Trackstruct* target_track,
										unsigned char depth ){

	unsigned char new_pos 	= 0;
	unsigned char j			= 0;

	// Remember the initial target track for comparison later on
	Trackstruct* original_track = target_track;

	// Get the new position of the locator for the track
	new_pos = get_next_trackposition( target_page, target_track );

	// Assign the new locator value to the track
	target_track->attr_LOCATOR = new_pos;

	// Cover case of TTC coming from 0 in chain shifting..
	target_track->TTC = 1;

	// A new pos of 0 is the most interesting case.
	// There can be various reasons for this: Track WRAP, track SKIP, or chain SWITCH
	if ( new_pos == 0 ){

		// WRAP in the same track - ( if ("track is not part of a chain") )
		if ( target_track == target_track->chain_data[NEXT] ){

			// Get the new position after the wrap
			new_pos = get_next_trackposition( target_page, target_track );

			// SKIP of the track, nothing to do
			if ( new_pos == 0 ){

				return NULL;
			}
			else{

				// Assign the new locator value to the track
				target_track->attr_LOCATOR = new_pos;
			}
		}

		// SWITCH to the next track in the chain (track is part of a chain)
		else if ( target_track->chain_data[NEXT] != target_track ){
			#ifdef NEMO
			track_window_follow_chain( target_page, target_track );
			#endif

			// Make the NEXT track the PLAY track in the whole track chain
			update_trackchain_PLAY( target_page, target_track, target_track->chain_data[NEXT] );

			// Switch focus to the new play track
			target_track = target_track->chain_data[PLAY];

			// Limit on recursion
			if ( depth < 10 ){

				// Advance the locator of the newly pointed track
				target_track = advance_track_locator( target_page, target_track, depth+1 );
			}
			else{

				// Return NULL saying that there is nothing to do
				target_track = NULL;
			}

		} // Switch to the next track
	}


	// Consider the FOLLOW case - it switches the trackSelection
	if (
			// We are operating in the page under the cursor,
			( target_page == &Page_repository[GRID_CURSOR] )

		&& 	( target_track != NULL )

			// One track or one of its maps (implicitly only one track selected) is zoomed into,
		&& 	( (G_zoom_level == zoomTRACK) || (G_zoom_level == zoomMAP) )

			// The FOLLOW is active.
		&&	( follow_flag == FOLLOW_TRACK )
		){

		// Identify the index of the selected track in page
		j = my_bit2ndx( target_page->trackSelection );

		target_page->trackSelection =
			( 1 << row_of_track( target_page, target_page->Track[j]->chain_data[PLAY] ));

	} // if FOLLOW - the big condition


	// Make sure we don't lose ticks because of fresh tracks with TTC 0
	if (target_track != NULL ){
		target_track->TTC = 1;
	}


	// Support PgmCh send on tracks that are part of a chain.
	// Compare initial track with target track
	#ifdef FEATURE_ENABLE_SONG_UPE
	if ( original_track != target_track && G_pause_bit == OFF ){
		// Send the program change for the new track
		send_track_program_change( target_track, target_page );
	}
	#else
	if ( original_track != target_track ){
		// Send the program change for the new track
		send_track_program_change( target_track, target_page );
	}
	#endif

	// Return the wrap to other track - condition flag
	return target_track;
}




// Check if recording is going on in the chain of a given track
Trackstruct* recording_active_in_chainof( Pagestruct* target_page, unsigned char row ){

	unsigned char i = 0;
	Trackstruct* result = NULL;
	Trackstruct* current_track = target_page->Track[row];

	for ( i=0; i < MATRIX_NROF_ROWS; i++ ){

		if ( Page_getTrackRecPattern(target_page) &
					( 1 << row_of_track( target_page, current_track ) )
		){

			result = current_track;
			break;
		}

		// Advance the current track pointer
		current_track = current_track->chain_data[NEXT];
	}

	return result;
}



//________________________________________________________________________________________
// Advance track TTC and locator for given track in page, according to chain setting.
Trackstruct* advance_track_TTC( Pagestruct* target_page, Trackstruct* target_track ){

	unsigned char row = 0;
	Trackstruct* recording_track = NULL;

	// Advance the row TTC first..
	target_track->TTC = 	1 + (target_track->TTC % 12);

	// Remember it for later lookup
	row = row_of_track( target_page, target_track );

	// Advance the track LOC (locator) on the 'one' of their TTC
	if ( 	( target_track->TTC 	== 1 )
		){

		// Universal track locator advancement - will return the next track in chain, or NULL
		target_track = advance_track_locator( target_page, target_track, 0 );

		if( target_track->chain_data[NEXT] != target_track ) {

			recording_track = recording_active_in_chainof( target_page, row );

			// REC FLAG CARRY - carry REC flag over with the cursor, if recording going on in chain at hand
			if (	( Page_getTrackRecPattern(target_page) != 0 )
				&&	( recording_track != NULL )
				&&	( target_track != NULL )
				){

				// Unset the recording bit from the original track..
				//target_page->track_REC_pattern ^= ( 1 << row );
				Page_flipTrackRecPatternBit( target_page, row_of_track( target_page, recording_track) );

				// ..and set the recording bit of the target page to the PLAY track of the given trow
				// target_page->track_REC_pattern ^= ( 1 <<  row_of_track( target_page, target_track->chain_data[PLAY] ) );
				Page_flipTrackRecPatternBit( target_page, row_of_track( target_page, target_track->chain_data[PLAY]) );
			}
		}
	}

	return target_track;
}



//___________________________________________________________________________________________
// Advance the locator of one track. Returns the index of the track wrapped to, or old track
Trackstruct* get_next_finger( 	Pagestruct* target_page,
								Trackstruct* target_track,
								unsigned char depth ){

	unsigned char new_pos 	= 0;

	// d_iag_printf( "        locator at:%d\n", target_track->attr_LOCATOR );

	// Get the new position of the locator for the track
	new_pos = get_next_trackposition( target_page, target_track );

	// d_iag_printf( "        new_pos:%d\n", new_pos );

	// Assign the new locator value to the track
	target_track->attr_LOCATOR = new_pos;

	// Cover case of TTC coming from 0 in chain shifting..
	target_track->TTC = 1;

	// A new pos of 0 is the most interesting case.
	// There can be various reasons for this: Track WRAP, track SKIP, or chain SWITCH
	if ( new_pos == 0 ){

		// WRAP in the same track - ( if ("track is not part of a chain") )
		if ( target_track == target_track->chain_data[NEXT] ){

			// Get the new position after the wrap
			new_pos = get_next_trackposition( target_page, target_track );

			// SKIP of the track, nothing to do
			if ( new_pos == 0 ){

				return NULL;
			}
			else{

				// Assign the new locator value to the track
				target_track->attr_LOCATOR = new_pos;
			}
		}

		// SWITCH to the next track in the chain (track is part of a chain)
		else if ( target_track->chain_data[NEXT] != target_track ){

			// Make the NEXT track the PLAY track in the whole track chain
			update_trackchain_PLAY( target_page, target_track, target_track->chain_data[NEXT] );

			// Switch focus to the new play track
			target_track = target_track->chain_data[PLAY];

			// Limit on recursion
			if ( depth < 10 ){

				// Advance the locator of the newly pointed track
				target_track = get_next_finger( target_page, target_track, depth+1 );
			}
			else{

				// Return NULL saying that there is nothing to do
				target_track = NULL;
			}

		} // Switch to the next track
	}

	// Return the wrap to other track condition flag
	return target_track;
}



//___________________________________________________________________________________________
// Advance the locator of one track. Returns the index of the track wrapped to.
// Get a step position and return the next valid step index, as an absolute number
unsigned char advance_record_finger( 	Pagestruct* target_page,
										unsigned char in_position ){

	unsigned char result = 0;
	unsigned char row = 0;
	unsigned char col = 0;
	Trackstruct* target_track = NULL;

	// Copy target page into buffer and continue working on buffer, simulating reality
	Page_copy( target_page, BUFFER_page );

	// Compute current coordinates and identify target track
	row = row_of( 		in_position );
	col = column_of(	in_position );

	// Identify and set up the target track according to current coordinates
	target_track = BUFFER_page->Track[row];
	target_track->attr_LOCATOR = col + 1;
	target_track->frame_ndx = target_track->attr_LOCATOR;

	// Get the next finger - which is inherent in the track index and its locator
	target_track = get_next_finger( BUFFER_page, target_track, 0 );

	// Re-assemble index from coordinates
	if ( target_track != NULL ){

		// d_iag_printf( "     0 next loc:%d\n", target_track->attr_LOCATOR );

		// Jump over potentially skipped steps
		target_track->attr_LOCATOR =
			jump_skipped_position_new( 	BUFFER_page, target_track,
										target_track->attr_LOCATOR,
										DIRECTION_LINEAR );

		// d_iag_printf( "     1 next loc:%d\n", target_track->attr_LOCATOR );

		row = row_of_track( BUFFER_page, target_track );
		col = target_track->attr_LOCATOR - 1;
	}

	// Compute the result as a step button index
	result = 	(col + 1) * 11 	+ (row);

	return result;
}



