
//// display the chains in the page
//void show_chains_in_page( Pagestruct* target_page ){
//
//	unsigned char row=0;
//	unsigned char i = 0;
//	Trackstruct* head_ptr = NULL;
//	Trackstruct* tail_ptr = NULL;
//	
//	// Row iterator
//	for ( row=0; row < MATRIX_NROF_ROWS; row++ ){
//	
//		// Find the next head in page
//		if ( target_page->Track[row]->chain_data[HEAD] == target_page->Track[row] ){
//		
//			diag_printf( "head:%d ", row );
//
//			head_ptr = target_page->Track[row]->chain_data[HEAD];
//			diag_printf( "(%d) - ", row_of_track( target_page, head_ptr ) );
//		
//			// Show the head's tail
//			tail_ptr = head_ptr;
//			for ( i=0; i < MATRIX_NROF_ROWS; i++ ){
//
//				tail_ptr = tail_ptr->chain_data[NEXT];
//				
//				if ( tail_ptr == head_ptr ){
//					
//					// Terminate the search
//					i = MATRIX_NROF_ROWS;
//					diag_printf( "\n" );
//				}
//				else{
//					
//					// Print the tail pointer
//					diag_printf( " %d", row_of_track( target_page, tail_ptr ) );
//				}
//			} // tail iterator			
//		} // Head was found
//	} // Row iterator
//
//	diag_printf( "\n" );
//}


// Take a track and tell how many tracks are in the same chain
unsigned char cardinality_of_chain( Trackstruct* target_track ){

	unsigned char i = 0;
	Trackstruct* current_track;

	current_track = target_track->chain_data[NEXT];
	for ( i=1; i <= MATRIX_NROF_ROWS; i++ ){
	
		// Check if we are back to start - full loop
		// d_iag_printf( "track:%d current:%d\n", target_track, current_track );
		if ( current_track == target_track ){
			return i;
		}	
	
		// Advance the current track
		current_track = current_track->chain_data[NEXT];	
	}

	return i;
}


// Removes a track from a chain, leaving that chain otherwise alone
void remove_chain_element(	 	Pagestruct* target_page, 
								Trackstruct* target_track ){

	// Link the new chain over the current track
	( target_track->chain_data[PREV] )->chain_data[NEXT] = 
		target_track->chain_data[NEXT];

	( target_track->chain_data[NEXT] )->chain_data[PREV] = 
		target_track->chain_data[PREV];
	
	// Establish HEAD
	switch( target_track == target_track->chain_data[HEAD] ){

		case TRUE:
			// Removed element was the head of the chain
			update_trackchain_HEAD(	target_page, 
									target_track->chain_data[NEXT],
									target_track->chain_data[NEXT] );
			break;
		
		default:
			// Removed element was only a part in the chain
			update_trackchain_HEAD(	target_page, 
									target_track->chain_data[NEXT],
									target_track->chain_data[HEAD] );
			break;
	}

	// PLAY of upper chain is its own HEAD
	update_trackchain_PLAY(	target_page, 
							target_track->chain_data[NEXT],
							target_track->chain_data[NEXT] ->chain_data[HEAD] );

	// Bring the head tracks to global locator position
	target_track->chain_data[NEXT] ->chain_data[HEAD]->attribute[ATTR_LOCATOR] = GLOBAL_LOCATOR;
	target_track->chain_data[NEXT] ->chain_data[HEAD]->trigger_set_ndx = GLOBAL_LOCATOR;

	// And finally disable current track from play - since it was removed..
	target_track->attribute[ATTR_LOCATOR] = 0;	
	target_track->trigger_set_ndx = 0;
}




// Given track is transformed into a one-track chain. 
// If it was part of a chain before, that chain is broken up into a top part and a bottom part.
void make_chain_head( 	Pagestruct* target_page, 
						Trackstruct* target_track ){

	// Remove the given track from its current chain
	remove_chain_element( target_page, target_track );

	// HEAD property declaration	
	// Make the HEAD be a one track chain for now - before we add the tail
	target_track->chain_data[PLAY] = target_track;
	target_track->chain_data[PREV] = target_track;
	target_track->chain_data[NEXT] = target_track;
	target_track->chain_data[HEAD] = target_track;	

	// Align to global locator
	target_track->attribute[ATTR_LOCATOR] = GLOBAL_LOCATOR;
	target_track->trigger_set_ndx = GLOBAL_LOCATOR;
}



// Add a tail track to the chain led by the given head.
void make_chain_tail( 	Pagestruct* target_page, 
						Trackstruct* tail_track, 
						Trackstruct* head_track ){

	// Connect the tail track into the chain
	tail_track->chain_data[PREV] = head_track->chain_data[PREV];
	( head_track->chain_data[PREV] )->chain_data[NEXT] = tail_track;

	// Close the chain between tail track and head
	head_track->chain_data[PREV] = tail_track;
	tail_track->chain_data[NEXT] = head_track;
	
	// Update the HEAD
	tail_track->chain_data[HEAD] = head_track;
	
	// Update the PLAY overall
	update_trackchain_PLAY(	target_page, head_track, head_track );

	// Make sure the chain is not playing anymore
	tail_track->attribute[ATTR_LOCATOR] = 0;
	tail_track->trigger_set_ndx = 0;
}



// Takes the current selection in the page and makes a chain out of it.
// Previous chains are broken and left alone
void chain_selected_tracks( Pagestruct* target_page ){

	unsigned char head = 0;
	unsigned char tail = 0;

	// Only meaningful if there is a track selection
	if ( target_page->trackSelection == 0 ) return;

	// Build the chain made up of the selected tracks
	for ( head=0; head < MATRIX_NROF_ROWS; head++ ){
	
		// The first track found is the HEAD of the chain. Make a one-track chain with it.
		if ( (target_page->trackSelection & ( 1 << head )) != 0 ){
			// d_iag_printf( "   HEAD found:%d\n", head );

			// Break the head from other chain/s and make it a one-track chain
			make_chain_head( target_page, target_page->Track[head] );
	
			// Iterate down from head and add selected tracks to this chain
			for ( tail = head+1; tail < MATRIX_NROF_ROWS; tail++ ){
			
				// The next selected track is added to the chain of the found head:
				if ( (target_page->trackSelection & ( 1 << tail )) != 0 ){

					// Remove a tail track from its previous chain
					remove_chain_element( target_page, target_page->Track[tail] );

					// Make tail track the tail of the chain led by the current head
					make_chain_tail( 	target_page, 
										target_page->Track[tail], 
										target_page->Track[head] );

					// Signal chainer activity in target_page; 4 is an un-#define'd chainmode
					target_page->chainMode = 4;
				} 
			} // Tail search iterator
			
			// Void the track selection
			target_page->trackSelection = 0;

			// Head was found and processed, no need to search further
			head = MATRIX_NROF_ROWS;
			
		} // Head finder condition
	} // Head search iterator
	
}






