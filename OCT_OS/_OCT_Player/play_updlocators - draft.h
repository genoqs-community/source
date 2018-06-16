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

		// Set all locators except head track to 0;
//		if ( current_track != new_HEAD_track ){
		if ( current_track != current_track->chain_data[PLAY] ){

			current_track->attr_LOCATOR = 0;
			current_track->frame_ndx = 0;
		}

		// Advance the current track along the chain
		current_track = current_track->chain_data[NEXT];
	}
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

	unsigned char i=0;
	Trackstruct* current_track = NULL;

	// Get the entry into the chain
	current_track = entry_track;

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
}




// Jumps over Steps that are skipped - by trigger and returns the new offset in the given track.
unsigned char jump_skipped_position( 	Pagestruct* target_page,
										unsigned char row,
										unsigned char candidate_position,
										unsigned char direction	) {

	unsigned char i = 0;
	unsigned char result_position = 0;

	// Nothing to do on a given 0 position
	if ( candidate_position == 0 ) return 0;

	// Assume the candidate is good!
	result_position = candidate_position;

	// Step at position is not skipped - probably the default case..
	if ( Step_get_status( target_page->Step[row][ result_position-1 ], STEPSTAT_SKIP ) != ON ){

		return result_position;
	}

	// Step at candidate position is skipped, so find the next unskipped one.
	// Depending on the move direction look left or right of candidate position
	switch( direction ){

		case DIRECTION_RANDOM:  // == 5
			// Fallthrough intended

		case DIRECTION_LINEAR:	// == 1

			// Check for skips on the position until right one is found. Move to right!
			for ( i=0; i < MATRIX_NROF_COLUMNS; i++ ){

				// Step at position is skipped
				if ( Step_get_status( target_page->Step[row][ result_position-1 ], STEPSTAT_SKIP ) == ON ){

					// Increment position
					result_position ++;

					// Go only to track end, do not wrap because a chain may be in place
					if ( result_position >= MATRIX_NROF_COLUMNS ){

						i = MATRIX_NROF_COLUMNS;
					}
				}
				// Step is not skipped, nothing to do!
				else {
					// Terminate the loop
					i = 200;
				}
			} // column iterator
			break;


		case DIRECTION_REVERSE:	// == 2

			// Logic: if the next step is skipped, decrement again
			for ( i=0; i < MATRIX_NROF_COLUMNS; i++ ){

				// Step at position is skipped
				if ( Step_get_status( target_page->Step[row][result_position-1], STEPSTAT_SKIP ) == ON ){

					// Decrement position if still in range
					if ( result_position > 1 ){
						 result_position --;
					}

					// Go only to track start, do not wrap because a chain may be in place
					if ( result_position < 1 ){

						i = MATRIX_NROF_COLUMNS;
					}
				}
				// Step is not skipped, nothing to do
				else {
					// Terminate the loop
					i = 200;
				}
			} // column iterator
			break;

	} // switch( direction );


	// Indicates that the entire track rest is skipped
	if ( i == MATRIX_NROF_COLUMNS ){

		// This tells the caller to stop playing the track for now and move on.
		result_position = 0;
	}

	return result_position;
}



// Takes in a candidate frame (position index).
// Jumps over Steps that are skipped - by trigger and
// returns a new frame index in the track given by row.
unsigned char jump_skipped_position_new( 	Pagestruct* target_page,
											Trackstruct* target_track,
											unsigned char candidate_frame,
											unsigned char direction	) {

	unsigned char col = 0;
	unsigned char row = row_of_track( target_page, target_track );

	if ( candidate_frame == 0 ) return 0;

	switch( direction ){

		case DIRECTION_LINEAR:		// == 1

			// Cases that need coverage with regard to skips:
			// 1. Next candidate NOT within bounds
			// 2. Next candidate NOT skipped and NOT within bounds

			// Logic: if the next step is skipped, increment - else break.
			for ( col=0; col < MATRIX_NROF_COLUMNS; col++ ){

				// If the candidate iterator has reached the track boundary
				if ( candidate_frame > MATRIX_NROF_COLUMNS ){

					break;
				}
				// If new step in SKIP mode
				else if ( Step_get_status( target_page->Step[row][ candidate_frame-1 ], STEPSTAT_SKIP ) == ON ){

					// Increment offset
					candidate_frame ++;
				}
				// Step is simply not skipped
				else {

					// No (more) skip found, so break.
					break;
				}
			}

			// Indicates that the entire track content is skipped
			if (	(col == MATRIX_NROF_COLUMNS)
				||	(candidate_frame > TRACK_DEF_LENGTH )
				){

				// This will tell the caller to stop playing the track and move on.
				candidate_frame = 0;
			}

			break;




		case DIRECTION_REVERSE:		// == 2

			// Logic: if the next step is skipped, decrement again
			for ( col=0; col < MATRIX_NROF_COLUMNS; col++ ){

				// If new step in SKIP mode
				if ( Step_get_status( target_page->Step[row][candidate_frame-1], STEPSTAT_SKIP ) == ON ){

					// Decrement offset if still in range
					if ( candidate_frame > 0 ){
						 candidate_frame --;
					}
					else{
						candidate_frame = 16;
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
				candidate_frame = 0;
			}
			break;

	} // switch( direction );
	// Candidate now contains the return value

	return candidate_frame;
}



// Returns the direction that a track naturally (by frame) wants to move in
unsigned get_next_frame_direction( Pagestruct* target_page, Trackstruct* target_track ){

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




// Returns the index of the next frame in a track - according to certainty value
unsigned char get_next_frame_ndx( Pagestruct* target_page, Trackstruct* target_track ){

	unsigned char result = 0;
	unsigned char candidate_frame = 0;
	unsigned char direction = 0;
	unsigned char certainty = 0;

	certainty = direction_repository[ 	target_track->attr_DIR
									+ 	target_track->event_offset[ATTR_DIRECTION] ]
						.trigger_set[ target_track->frame_ndx ]
						.certainty_next;

	if ( ((rand() % 100) ) < certainty ){

		// FORWARD moving result: direction is FORWARD
		direction = DIRECTION_LINEAR;
		candidate_frame = (target_track->frame_ndx +1) % 17;
	}
	else{

		// BACKWARD moving result: direction is REVERSE
		direction = DIRECTION_REVERSE;
		candidate_frame = 1 + ( ( target_track->frame_ndx + 14 ) % 16 );
	}

//	if (	( target_page->pageNdx == 8 )
//		&&	( row_of_track( target_page, target_track ) == 9 )
//		){
//
//		d_iag_printf( "     candidate:%d\n", candidate );
//	}


	// Jump over potentially skipped steps resulting from the new frame candidate
// --> Don't skip frames - skip only triggers when steps are skipped..
	// result = jump_skipped_position_new( target_page, target_track, candidate_frame, direction );


//	if (	( target_page->pageNdx == 8 )
//		&&	( row_of_track( target_page, target_track ) == 9 )
//		){
//
//		d_iag_printf( "     result returned:%d\n", result );
//	}

	return result;
}

// TRUE if direction frame has no triggers, FALSE if at least one trigger found!
unsigned char is_direction_frame_empty( unsigned char DIR_ndx, unsigned char frame_ndx ){

	unsigned char i = 0;

	// Iterate through the frame to find the result
	for ( i=0; i < MATRIX_NROF_ROWS-1; i++ ){

		if ( direction_repository[ DIR_ndx ].trigger_set[ frame_ndx ].trigger[i] != 0 ){
			return FALSE;
		}
	}
	// All triggers in set were 0
	return TRUE;
}



// Returns the next "good" trigger for the given track - including move across direction frames.
// A return trigger of 0 is the most interesting case.
// There can be various reasons for this: Track WRAP, track SKIP, or chain SWITCH
unsigned char get_next_trigger( Pagestruct* target_page, unsigned char row ){

	unsigned char i=0;
	unsigned char result = 0;

	unsigned char current_trigger = 0;
	Trackstruct* target_track = target_page->Track[row];

	// Get the current trigger
	current_trigger = target_track->attr_LOCATOR;


	// TRIGGER STACK SECURING
	// Overcoming the stop state of the track
	if (	( current_trigger == 0 )
		||	( target_track->frame_ndx == 0 )
		){

		// Update the frame index in track
		target_track->frame_ndx = get_next_frame_ndx( target_page, target_track );

		// Refill trigger stack of track - refill phase I
		memcpy( &target_track->trigger,
				&direction_repository[ target_track->attr_DIR
										+ target_track->event_offset[ATTR_DIRECTION] ]
					.trigger_set[ target_track->frame_ndx ].trigger,
				10 );
	}


	// TRIGGER EXTRACTION
	// Iterate through the current trigger stack of the track
	for (i=0; i < MATRIX_NROF_ROWS; i++){

		// Current trigger stored in the track
		result = target_page->Track[row]->trigger[i];

		// Trigger found
		if ( result > 0 ){

			// Consume the trigger
			target_page->Track[row]->trigger[i] = 0;

			// Skip-validate the new position
			result = jump_skipped_position( target_page, row, result,
						get_next_frame_direction( target_page, target_track ) 	);

			// Check status after skip validation: 0 means all steps are skipped in track
			if ( result == 0 ){

				// Break the loop denoting skipped track
				i = 200;
			}
			else{
				// Break trigger stack iterator - denoting interrupted (vs natural) break
				i = 100;
			}
		}
	} // Row iterator


	// Two possible causes: trigger stack is empty, or track skipped altogether
	if ( result == 0 ){

		switch( i ){

			// Loop has completed "unsuccessfully" - trigger stack empty
			case MATRIX_NROF_ROWS:

				// Two possible causes: all consumed, or empty to begin with. Check!
				switch( is_direction_frame_empty( 	target_track->attr_DIR
													+ target_track->event_offset[ATTR_DIRECTION],
													target_track->frame_ndx )	){

					// Empty from start: pick a skip-validated random position, in a random direction..
					case TRUE:
						result = jump_skipped_position( target_page, row, (rand() % 16)+1, (rand()%2)+1 );
						break;


					// We have consumed all, but skips were in the way: refresh trigger stack and frame index!
					case FALSE:

						// Update the frame index in track
						target_track->frame_ndx = get_next_frame_ndx( target_page, target_track );

						// Refill trigger stack of track - refill phase I
						memcpy( &target_track->trigger,
								&direction_repository[ target_track->attr_DIR
														+ target_track->event_offset[ATTR_DIRECTION] ]
									.trigger_set[ target_track->frame_ndx ].trigger,
								10 );

						// Call recursively on the newly refilled trigger stack
						result = get_next_trigger( target_page, row );
						break;

				} // direction_frame empty?
				break;


			case 100:
				break;


			// Track skipped altogether: loop was interrupted, all steps skipped
			case 200:

				result = 0;
				break;
		}
	}

	// Return the found trigger
	return result;
}



// Advance the locator of one given track, setting it to the next valid (unskipped) position.
// Returns the index of the track wrapped to, or the same track, if not wrapped
//____________________________________________________________________________________________
Trackstruct* advance_track_locator( 	Pagestruct* target_page,
										Trackstruct* target_track,
										unsigned char depth ){

	unsigned char new_locator 	= 0;		// Newly computed locator position
	unsigned char j				= 0;		// Utility variable
	unsigned char row  			= row_of_track( target_page, target_track );

	// Cover case of TTC coming from 0 in chain shifting..
	target_track->TTC = 1;

	// Get the new position of the locator for the track, up till the end of the track.
	// This involves moving across direction frames as well!
	new_locator = get_next_trigger( target_page, row );

	// Assign the new locator value to the track
	target_track->attr_LOCATOR = new_locator;

	// A new pos of 0 is the most interesting case.
	// There can be various reasons for this: Track WRAP, track SKIP, or chain SWITCH
	if ( new_locator == 0 ){

		// WRAP in the same track - ( if ("track is not part of a chain") )
		if ( target_track == target_track->chain_data[NEXT] ){

			// Get the new position after the wrap in the track
			new_locator = get_next_trigger( target_page, row );

			// SKIP of the rest of track, all track is skipped, so nothing to do
			if ( new_locator == 0 ){

				target_track = NULL;
			}
			else{

				// Assign the new locator value to the track
				target_track->attr_LOCATOR = new_locator;
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
			// Dealing with a proper result
			( target_track != NULL )

		// We are operating in the page under the cursor,
		&&	( target_page == &Page_repository[GRID_CURSOR] )

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


	// Return the wrap to other track - condition flag
	return target_track;
}




// Check if recording is going on in the chain of a given track
unsigned char recording_active_in_chainof( Pagestruct* target_page, unsigned char row ){

	unsigned char i = 0;
	unsigned char result = FALSE;
	Trackstruct* current_track = target_page->Track[row];

	for ( i=0; i < MATRIX_NROF_ROWS; i++ ){

		if ( target_page->track_REC_pattern &
					( 1 << row_of_track( target_page, current_track ) )
		){

			result = TRUE;
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

	// Advance the row TTC first..
	target_track->TTC = 	1 + (target_track->TTC % 12);

	// Remember it for later lookup
	row = row_of_track( target_page, target_track );

	// Advance the track LOC (locator) on the 'one' of their TTC
	if ( 	( target_track->TTC 	== 1 )
		){

		// Universal track locator advancement - will return the next track in chain, or NULL
		// The returned track will have the correct locator ready to go.
		target_track = advance_track_locator( target_page, target_track, 0 );

		// REC FLAG CARRY - carry REC flag over with the cursor, if recording going on in chain at hand
		if (	( target_page->track_REC_pattern != 0 )
			&&	( recording_active_in_chainof( target_page, row ) )
			&&	( target_track != NULL )
			){

			// Unset the recording bit from the original track..
			target_page->track_REC_pattern ^= ( 1 << row );

			// ..and set the recording bit of the target page to the PLAY track of the given trow
			target_page->track_REC_pattern ^=
				( 1 <<  row_of_track( target_page, target_track->chain_data[PLAY] ) );
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
	unsigned char row = row_of_track( target_page, target_track );

	// d_iag_printf( "        locator at:%d\n", target_track->attr_LOCATOR );

	// Get the new position of the locator for the track
	new_pos = get_next_trigger( target_page, row );

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
			new_pos = get_next_trigger( target_page, row );

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



