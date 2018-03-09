//
// UPDATE TRACK LOCATOR of given track - related functions.
//



// Takes a track, and returns the locator value following the given curr_locator
unsigned char get_next_tracklocator( 	Trackstruct* target_track, 
										unsigned char curr_locator ){

	// Connect Length to Start
	unsigned char oldLocator 	= 	curr_locator;
	unsigned char next_locator 	= 	0;
	unsigned char length 		= 	target_track->attribute[ATTR_LENGTH];
	unsigned char offset 		= 	oldLocator;
	
	switch( target_track->attribute[ATTR_DIRECTION] ){
		
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
		if ( current_track != new_HEAD_track ){
		
			current_track->attribute[ATTR_LOCATOR] = 0;	
			current_track->trigger_set_ndx = 0;
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
			current_track->attribute[ATTR_LOCATOR] = 0;
		}

		// Advance the current track along the chain
		current_track = current_track->chain_data[NEXT];
	}
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

		case DIRECTION_LINEAR:
		
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
			
			
		case DIRECTION_REVERSE:

			// Logic: if the next step is skipped, decrement again
			for ( col=0; col < MATRIX_NROF_COLUMNS; col++ ){
				
				// If new step in SKIP mode
				if ( Step_get_status( target_page->Step[row][candidate-1], STEPSTAT_SKIP ) == ON ){
		
					// Decrement offset if still in range
					if ( candidate > 0 ){
						 candidate --;
					}
					else{
						candidate = 15;
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

		case DIRECTION_LINEAR:
		
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
			break;
			
			
		case DIRECTION_REVERSE:

			// Logic: if the next step is skipped, decrement again
			for ( col=0; col < MATRIX_NROF_COLUMNS; col++ ){
				
				// If new step in SKIP mode
				if ( Step_get_status( target_page->Step[row][candidate-1], STEPSTAT_SKIP ) == ON ){
		
					// Decrement offset if still in range
					if ( candidate > 0 ){
						 candidate --;
					}
					else{
						candidate = 15;
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
			break;
			
	} // switch( direction );		
	// Candidate now contains the return value

	return candidate;
}



// Returns the next trigger of the given track
unsigned char get_next_trigger_new( Pagestruct* target_page, unsigned char row ){

	unsigned char i=0;
	unsigned char result = 0;
	unsigned char direction = DIRECTION_LINEAR;


	// Make sure the current trugger set is skipped, ignore it by returning 0
	if ( Step_get_status( 
			target_page->Step[row][target_page->Track[row]->trigger_set_ndx - 1], 
				STEPSTAT_SKIP ) == ON ){
		return 0;
	}

	// Iterate through the current trigger stack of the track
	for (i=0; i<MATRIX_NROF_ROWS; i++){
	
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
			if ( result < target_page->Track[row]->attribute[ATTR_LOCATOR] ){
				direction = DIRECTION_REVERSE;
			}
			// Jump skipped positions - repeating position does not need skipping..
			if ( result != target_page->Track[row]->attribute[ATTR_LOCATOR] ){

//if (row > 7)
//d_iag_printf( "GNT: --jump trigger; result:%d:\n", result );

				result = jump_skipped_position( target_page, row, result, direction );
			}
	
			return result;
		}

	}

	return result;
}




// Returns the index of the next trigger set in a track - according to certainty value
unsigned char get_next_trigger_set( Pagestruct* target_page, unsigned char row ){

	unsigned char result = 0;
	unsigned char candidate = 0;
	unsigned char direction = 0;

//if (row > 7)
//d_iag_printf( "GNTS:--row:%d t_set:%d\n", row, target_page->Track[row]->trigger_set_ndx );

	if ( ((rand() % 100) ) <
				direction_repository[ target_page->Track[row]->attribute[ATTR_DIRECTION] ]
				.trigger_set[ target_page->Track[row]->trigger_set_ndx ]
				.certainty_next ){
	
		// FORWARD moving result: direction is FORWARD
		direction = DIRECTION_LINEAR;
		candidate = (target_page->Track[row]->trigger_set_ndx +1) % 17;
	}
	else{
	
		// BACKWARD moving result: direction is REVERSE
		direction = DIRECTION_REVERSE;
		candidate = 1 + (( target_page->Track[row]->trigger_set_ndx + 15 ) % 16 );
	}

	// Jump over potentially skipped steps at new resulting position

//if (row > 7)
//d_iag_printf( "GNTS:--jump trigger set:%d\n", candidate );

	result = jump_skipped_position( target_page, row, candidate, direction );
		
	return result;
}


// Returns the index of the next trigger set in a track - according to certainty value
unsigned char get_next_trigger_set_new( Pagestruct* target_page, Trackstruct* target_track ){

	unsigned char result = 0;
	unsigned char candidate = 0;
	unsigned char direction = 0;

	if ( ((rand() % 100) ) <
			direction_repository[ target_track->attribute[ATTR_DIRECTION] ]
			.trigger_set[ target_track->trigger_set_ndx ].certainty_next ){
	
		// FORWARD moving result: direction is FORWARD
		direction = DIRECTION_LINEAR;
		candidate = (target_track->trigger_set_ndx +1) % 17;
	}
	else{
	
		// BACKWARD moving result: direction is REVERSE
		direction = DIRECTION_REVERSE;
		candidate = 1 + (( target_track->trigger_set_ndx + 15 ) % 16 );
	}

	// Jump over potentially skipped steps at new resulting position
	result = jump_skipped_position_new( target_page, target_track, candidate, direction );
		
	return result;
}




// Returns the next locator of a given track in the given page.
unsigned char get_next_trackposition_new( Pagestruct* target_page, Trackstruct* target_track ){

	unsigned int old_offset	=	0;
	signed   int result		=	0;	// Goes below 0 (becomes =-1) at some point in time
	unsigned int locator 	= 	target_track->attribute[ATTR_LOCATOR];
	unsigned char row 		= 	row_of_track( target_page, target_track );

	// Starting point is current locator - remember the offset; 	
	old_offset	= 	locator;

	// Overcome the stop state of the track
	if (	( locator == 0 )
		||	( target_track->trigger_set_ndx == 0 )
		){
	
		target_track->trigger_set_ndx 
			= get_next_trigger_set_new( target_page, target_track );

		// Refill trigger stack of track - refill I
		memcpy( &target_track->trigger, 
				&direction_repository[ target_track->attribute[ATTR_DIRECTION] ]
					.trigger_set[ target_track->trigger_set_ndx ].trigger,
				10 );
	}

	// Get next trigger from the trigger stack
	result = get_next_trigger_new( target_page, row );	
	
	// 0 means trigger stack was empty, refresh it
	if ( result == 0 ){

		// Advance Trigger set ndx - according to certainty value in the track
		target_track->trigger_set_ndx = get_next_trigger_set_new( target_page, target_track );
		
		// The trigger set has wrapped - signal the wrap
		if ( target_track->trigger_set_ndx == 0 ){

			result = 0;
		}
		else{

			// Refill trigger stack of track - refill II
			memcpy( &target_track->trigger, 
					&direction_repository[ target_track->attribute[ATTR_DIRECTION] ]
					.trigger_set[ target_track->trigger_set_ndx ].trigger,
					10 );

			// Get next trigger from the trigger stack
			result = get_next_trigger_new( target_page, row );

			// The new trigger stack is empty (offset is still 0).. play a random position!
			if ( result == 0 ){
			
				// Generate random offset position
				result = rand() % 17;
			}
		} // Track did not want to wrap
	}
	
	return result;
}



// Advance the locator of one track. Returns the index of the track wrapped to.
Trackstruct* advance_track_locator_new( 	Pagestruct* target_page, 
											Trackstruct* target_track,
											unsigned char depth ){

	unsigned char new_pos 	= 0;
	unsigned char j			= 0;

//	unsigned char row = row_of_track( target_page, target_track );

//if ( row > 7 )
//d_iag_printf( "    adv_trk_LOC track:%d LOC:%d\n", 
//	row_of_track( target_page, target_track ), target_track->attribute[ATTR_LOCATOR] );


	// Get the new position of the locator for the track
	new_pos = get_next_trackposition_new( target_page, target_track );

//if ( row > 7 )
//d_iag_printf( "    adv_trk_LOC new_pos calculated:%d\n", new_pos );


	// Assign the new locator value to the track				
	target_track->attribute[ATTR_LOCATOR] = new_pos;
	// Cover case of TTC coming from 0 in chain shifting..
	target_track->TTC = 1;
	
//if ( row > 7 )
//d_iag_printf( "    adv_trk_LOC new_pos assigned:%d\n", new_pos );


	// A new pos of 0 is the most interesting case.
	// There can be various reasons for this: Track WRAP, track SKIP, or chain SWITCH
	if ( new_pos == 0 ){

		// WRAP in the same track - ( if ("track is not part of a chain") )
		if ( target_track == target_track->chain_data[NEXT] ){

			// Get the new position after the wrap
			new_pos = get_next_trackposition_new( target_page, target_track );
	
			// SKIP of the track, nothing to do
			if ( new_pos == 0 ){
	
				return NULL;
			}
			else{

				// Assign the new locator value to the track				
				target_track->attribute[ATTR_LOCATOR] = new_pos;
			}
		}

		// SWITCH to the next track in the chain (track is part of a chain)
		else if ( target_track->chain_data[NEXT] != target_track ){

			// Make the NEXT track the PLAY track in the whole track chain
			update_trackchain_PLAY( target_page, target_track, target_track->chain_data[NEXT] );

			// Switch focus to the new play track
			target_track = target_track->chain_data[PLAY];
			
//if ( row > 7 )
//d_iag_printf( "    adv_trk_LOC updated track:%d LOC:%d\n", 
//	row_of_track( target_page, target_track ), target_track->attribute[ATTR_LOCATOR] );

			// Limit on recursion
			if ( depth < 10 ){

				// Advance the locator of the newly pointed track
				target_track = advance_track_locator_new( target_page, target_track, depth+1 );
			}
			else{
				
				// Return NULL saying that there is nothing to do
				target_track = NULL;
			}
						
		} // Switch to the next track
	}

	// Consider the FOLLOW case - it switches the trackSelection
	if (
		// We are operating in the VIEWER_page,	
		(target_page == VIEWER_page )
		
		&& ( target_track != NULL )

		// One track or one of its maps (implicitly only one track selected) is zoomed into,
		&& 	( (zoom_level == zoomTRACK) || (zoom_level == zoomMAP) )

		// The FOLLOW is active.
		&&	( follow_flag == FOLLOW_TRACK )		 
		){

		// Identify the index of the selected track in page
		j = my_bit2ndx( VIEWER_page->trackSelection );
		
		VIEWER_page->trackSelection = 
			( 1 << row_of_track( target_page, VIEWER_page->Track[j]->chain_data[PLAY] ));
		
	} // if FOLLOW - the big condition


	// Return the wrap to other track condition flag
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



// Advance track TTC and locator for given track in page, according to chain setting.
Trackstruct* advance_track_TTC_new( Pagestruct* target_page, Trackstruct* target_track ){

	unsigned char row = 0;

	// Advance the row TTC first..
	target_track->TTC = 	1 + (target_track->TTC % 12);

	row = row_of_track( target_page, target_track );

//if ( row > 7 )
//d_iag_printf( "  adv_TTC: track:%d TTC:%d\n", row, target_track->TTC );

	// Advance the track LOC (locator) on the 'one' of their TTC
	if ( 	(target_track->TTC 	== 1 )
		){

		// Universal track locator advancement - will return the next track in chain, or NULL
		target_track = advance_track_locator_new( target_page, target_track, 0 );

		// Carry the REC flag all over with the cursor, if recording going on in the chain at hand
		if (	( target_page->track_REC_pattern != 0 )
			&&	( recording_active_in_chainof( target_page, row ) )
			&&	( target_track != NULL )
			){
			
			// Set the recording bit of the target page to the PLAY track of the given trow
			target_page->track_REC_pattern = 
				( 1 <<  row_of_track( target_page, target_track->chain_data[PLAY] ) );
		}
	}

	// d_iag_printf( "   ---advanced: row[%d]->LOC:%d ->TTC:%d\n", 
	//	row, target_page->Track[row]->attribute[ATTR_LOCATOR], target_page->Track[row]->TTC );

	return target_track;
}



