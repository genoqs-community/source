

extern void 			advance_page_locators( Pagestruct* target_page );
extern void 			MIDI_send( 	unsigned char type, 
									unsigned char val0, 
									unsigned char val1, 
									unsigned char val2 );



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

		// If playmode CHAIN, select the next page for play.
		// For now take the neighbor to the right, and if none, the furthest left
		if (GRID_bank_playmodes & ( 1 << target_bank )){
			
			// Decrease the "repeats" value. If it is not 0, break;	
			GRID_p_selection[ target_bank ]->repeats_left --;

			// If the page still has repeats left to play
			if ( GRID_p_selection[ target_bank ]->repeats_left > 0 ){					

				// Move on to the next bank and disregard the code below
				return;
			}

			// Else - implicitly - Update repeats value - this is during play and hence no +1 offset.
			GRID_p_selection[ target_bank ]->repeats_left = 
								GRID_p_selection[ target_bank ]->repeats;
			
			// Reset all locators in the page that is being moved away from..
//			set_page_locators( GRID_p_selection[ target_bank ], 1, 1 );
			
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

			// d_iag_printf( "      next_ndx:%d\n", next_ndx );

		} // GRID_bank_playmode is CHAIN
	} // bank is active

}


// Activates or deactivates the PLAY MODE
void PLAY_MODE_switch( unsigned char target_status ){

	unsigned char i=0;
	
	switch( target_status ){

		case ON:
			// Copy grid selection pages into the PLAY MODE buffer
			for ( i=0; i < GRID_NROF_BANKS; i++ ){

				if ( GRID_p_selection[i] != NULL ){
					// Copy from source to target page..
					Page_copy( GRID_p_selection[i], PLAY_MODE_page[i] );
				}
			}
			PLAY_MODE_STATUS = ON;
			follow_flag = FOLLOW_PAGE;
			break;
			
		case OFF:
			// Fetch PLAY MODE buffer pages into selection
			for ( i=0; i < GRID_NROF_BANKS; i++ ){

				if ( GRID_p_selection[i] != NULL ){ 
//				if ( PLAY_MODE_page[i] != NULL ){ 
					// Copy from source to target page..
					Page_copy( PLAY_MODE_page[i], GRID_p_selection[i] );
					GRID_p_selection[i]->locator = GLOBAL_LOCATOR;
				}
			}
			PLAY_MODE_STATUS = OFF;
			follow_flag = OFF;
			break;
	}	
	// Make sure to not lose sight.. (literally)
	VIEWER_page->keyMode = KMOD_PAGE;
}


// Send the program changes stored in the target page tracks
void send_program_changes( Pagestruct* target_page ){

	unsigned char row = 0;
	
	for ( row=0; row < MATRIX_NROF_ROWS; row++ ){
	
		if ( target_page->Track[row]->program_change != 0 ){

			MIDI_send( 	MIDI_PGMCH, 
						target_page->Track[row]->attribute[ATTR_MIDICH], 
						target_page->Track[row]->program_change - 1,
						0 );
		}
	}
}


//
// Performs a page switch within the given bank of the grid
//
void switch_gridbank( unsigned char target_bank ){
	
	// If page has some content, do the switch
	
	// HALT the old page
	if ( GRID_p_selection[target_bank] != NULL ) {

		// Stop playing the old page if it was not empty..
		stop_playing_page( GRID_p_selection[target_bank],	TTC_ABS_VALUE );

		// ..and set its locators to 0 for the next round of play.
		set_track_locators( GRID_p_selection[target_bank], 0, 0 );

		// Set the page locator to 0, just to be consistent with using
		// a locator of 0 to indicate that page or track is not playing.
		GRID_p_selection[target_bank]->locator = 0;

		// PLAYMODE handling
		if (	( PLAY_MODE_STATUS == ON )
			&&	( PLAY_MODE_page[target_bank] != NULL )
			){
			// Restore the old state of the page from PLAY buffer
			Page_copy( PLAY_MODE_page[target_bank], GRID_p_selection[target_bank] );
		}
	}

	// ACTIVATE the new page into the grid selection
	GRID_p_selection [target_bank] = GRID_p_preselection [target_bank];
	
	// Make sure we do not advance locators of an invalid page pointer
	if ( GRID_p_selection [target_bank] != NULL ){

		// Send the program change signals stored in the page tracks
		send_program_changes( GRID_p_selection[target_bank] );

		// Advance its locators once, moving them from 0 to 1, indicating activity
		advance_page_locators( GRID_p_selection[target_bank] );	

		// PLAYMODE handling
		if ( PLAY_MODE_STATUS == ON ){
			// Save new page in the PLAY MODE buffer
			Page_copy( GRID_p_selection[target_bank], PLAY_MODE_page[target_bank] );
		}
	}
}


//
// Make preselected pages into grid selections, on the clock or directly
// Depending on PAGE follow it may also switch the VIEWER_page
void select_page_preselections() {

	unsigned char i=0;
	
	// Quick and dirty fix for selecting pages in grid in stop mode
	unsigned char j = GRID_switch_mode;
	if ( RUNBIT == OFF ){
		
		GRID_switch_mode = GRID_SWITCH_DIRECT;
	}
	
	
	// Iterate through grid bank preseletions looking for change requests
	for (i=0; i < GRID_NROF_BANKS; i++){
		
		// "Change wanted" condition - there is a new page in queue
		if ( GRID_p_selection[i] != GRID_p_preselection[i] ){

			//d_iag_printf( "  ..switching(bank:%d): %d->%d\n", 
			//	i, 
			//	GRID_p_selection[i]->pageNdx, 
			//	GRID_p_preselection[i]->pageNdx );

			// This is the one of the page, potential switch		
			if ( 	( GRID_p_selection[i]->locator == 1 ) 
				&&	( GRID_p_selection[i] != NULL )
				){

				// Everybody switches on the ONE - ..both direct and oclock
				switch_gridbank( i );
			}
			else if (	( GRID_p_selection[i] == NULL )
					&&	( GLOBAL_LOCATOR == 1 )
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
			
			// Switch the VIEWER_page in zoomPAGE mode according to the change, if the row is affected
			//
			// Check the row of the current VIEWER_page and the other conditions
			if ( GRID_p_selection[i] != NULL ){

				if (	( 	(GRID_p_selection[i]->pageNdx % 10) == (VIEWER_page->pageNdx % 10) )
					&& 	( 	zoom_level 	== zoomPAGE ) 
					&& 	( 	follow_flag == FOLLOW_PAGE )
					){
		
					// We are in the same row, so do the switch of the VIEWER_page
					VIEWER_page = GRID_p_selection[i];
					VIEWER_page->keyMode = KMOD_PAGE;

					// Update the GRID CURSOR
					GRID_CURSOR = VIEWER_page->pageNdx;
				}
			}

			//d_iag_printf( "    switch done! sel:%d pre:%d\n", 
			// 	GRID_p_selection[i]->pageNdx, GRID_p_preselection[i]->pageNdx );
		
		}// "Change wanted" condition

	} // GRID_NROF_BANKS iterator


	// Restore the status of GRID_switch_mode to original value
	if ( RUNBIT == OFF ){		
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

