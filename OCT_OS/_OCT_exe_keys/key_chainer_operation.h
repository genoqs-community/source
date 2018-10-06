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


//void print_chain_data( Pagestruct* target_page ){
//	unsigned char i=0;
//	for (i=0; i<MATRIX_NROF_ROWS; i++){
//		d_iag_printf( "Track:%d HEAD:%d PLAY:%d LOC:%d\n",
//				i,
//				row_of_track( target_page, target_page->Track[i]->chain_data[HEAD]),
//				row_of_track( target_page, target_page->Track[i]->chain_data[PLAY]),
//				target_page->Track[i]->attr_LOCATOR );
//	}
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



// Applies the given mutepattern to the given page considering the chain status of page.
// LSbit is top bit.
void apply_mutepattern( Pagestruct* target_page, unsigned short mutepattern ){

	unsigned char i = 0;
	unsigned char j = 0;
	unsigned char temp = 0;
	unsigned short new_mutepattern = 0;
	unsigned short chain_mutepattern = 0;
	Trackstruct* current_track;

	// This is the working base for the new mutepattern
	new_mutepattern = target_page->trackMutepattern;

	// Row iterator
	for ( i=0; i < MATRIX_NROF_ROWS; i++ ){

		// Disregard the unset bits of the given mutepattern
		if ( (mutepattern & (1 << i)) == 0 ){
			continue;
		}

		// Focus on the current track;
		current_track = target_page->Track[i];

		// Depending on the way we choose the track base..
		switch( target_page->CHAINS_PLAY_HEAD ){

			case TRUE:
				// Act as toggle on the full chain
				// How long is the chain?
				temp = cardinality_of_chain( current_track );

				// Loop through the chain of the selected track and compute chain mutepattern
				chain_mutepattern = 0;
				for ( j=0; j < temp; j++ ){

					// Build the chain mutepattern
					chain_mutepattern |= ( 1 << row_of_track( target_page, current_track ));
					current_track = current_track->chain_data[NEXT];
				}


				// Find out whether the current track is originally muted or not and act
				switch( target_page->trackMutepattern & (1 << i) ){

					case 0:
						// Track is not muted, so mute its chain in the new mutepattern
						new_mutepattern |= chain_mutepattern;
						break;

					default:
						// Track is muted, so un-mute its chain
						new_mutepattern &= ( 0x3FF ^ chain_mutepattern );
						break;
				}

				// Reflect new mutepattern in page
				target_page->trackMutepattern = new_mutepattern;

				break;


			// Act only on the individual track
			case FALSE:
				target_page->trackMutepattern ^= ( 1 << row_of_track( target_page, current_track ));

				break;

		} //switch on the chain status

	} // Row iterator

	// Update the stored variable
	target_page->trackMutepatternStored = target_page->trackMutepattern;
}




// Removes a track from a chain, leaving that chain otherwise alone
void remove_chain_element(	 	Pagestruct* target_page,
								Trackstruct* target_track ){

// d_iag_printf( "0aa:\n" ); print_chain_data( target_page );

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
// d_iag_printf( "0ab T:\n" ); print_chain_data( target_page );

			// If the head is also PLAY, update PLAY to new head
			if( target_track->chain_data[PLAY] == target_track ){

				update_trackchain_PLAY(	target_page,
										target_track->chain_data[NEXT],
										target_track->chain_data[NEXT] ->chain_data[HEAD] );
			}
			break;

		default:
			// Removed element was only a part in the chain
			update_trackchain_HEAD(	target_page,
									target_track->chain_data[NEXT],
									target_track->chain_data[HEAD] );
// d_iag_printf( "0ab F:\n" ); print_chain_data( target_page );

			if( target_track->chain_data[PLAY] == target_track ){

				update_trackchain_PLAY(	target_page,
										target_track->chain_data[NEXT],
										target_track->chain_data[NEXT] ->chain_data[HEAD] );
			}
			else{
				update_trackchain_PLAY(	target_page,
										target_track->chain_data[NEXT],
										target_track->chain_data[NEXT] ->chain_data[PLAY] );
			}
			break;
	}

// d_iag_printf( "0ac:\n" ); print_chain_data( target_page );

	// Bring the head tracks to global locator position
//	target_track->chain_data[NEXT] ->chain_data[HEAD]->attr_LOCATOR = G_global_locator;
//	target_track->chain_data[NEXT] ->chain_data[HEAD]->frame_ndx 	= G_global_locator;

	target_track->chain_data[NEXT] ->chain_data[PLAY]->attr_LOCATOR = G_global_locator;
	target_track->chain_data[NEXT] ->chain_data[PLAY]->frame_ndx 	= G_global_locator;

	// And finally disable current track from play - since it was removed..
	if ( target_track->chain_data[PLAY] != target_track ){
		target_track->attr_LOCATOR = 0;
		target_track->frame_ndx = 0;
	}

// d_iag_printf( "0ad:\n" ); print_chain_data( target_page );

}




// Given track is transformed into a one-track chain.
// If it was part of a chain before, that chain is broken up into a top part and a bottom part.
void make_chain_head( 	Pagestruct* target_page,
						Trackstruct* target_track ){

	// Remove the given track from its current chain
// d_iag_printf( "0a:\n" ); print_chain_data( target_page );
	remove_chain_element( target_page, target_track );

// d_iag_printf( "0b:\n" ); print_chain_data( target_page );

	// HEAD property declaration
	// Make the HEAD be a one track chain for now - before we add any tail if at all
	target_track->chain_data[PLAY] = target_track;
	target_track->chain_data[PREV] = target_track;
	target_track->chain_data[NEXT] = target_track;
	target_track->chain_data[HEAD] = target_track;

	// Align to global locator
	target_track->attr_LOCATOR 	= G_global_locator;
	target_track->frame_ndx 	= G_global_locator;

// d_iag_printf( "0c:\n" ); print_chain_data( target_page );

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

	// Make sure the tail is not playing anymore unless PLAY track
	if ( tail_track->chain_data[PLAY] != tail_track ){
		tail_track->attr_LOCATOR = 0;
		tail_track->frame_ndx = 0;
	}
}


void remove_track_chain( Pagestruct* pagePt ){

	unsigned char row;

	// Initialize the page Track chain data
	for ( row=0; row < MATRIX_NROF_ROWS; row++ ){
		// Init the chain data for 10 unchained rows: each track is by itself
		pagePt->Track[row]->chain_data[HEAD] = pagePt->Track[row];
		pagePt->Track[row]->chain_data[NEXT] = pagePt->Track[row];
		pagePt->Track[row]->chain_data[PREV] = pagePt->Track[row];
		pagePt->Track[row]->chain_data[PLAY] = pagePt->Track[row];
	}
}


// Takes the current selection in the page and makes a chain out of it.
// Previous chains are broken and left alone
void chain_selected_tracks( Pagestruct* target_page ){

	unsigned char head = 0;
	unsigned char tail = 0;
	unsigned char temp = 0;
	unsigned short target_LOC = 0;
	unsigned short target_TTC = 0;
	unsigned char i = 0;

	// Function exits if there is no track selection
	if ( target_page->trackSelection == 0 ) return;

	// Void the REC pattern
	Page_setTrackRecPattern(target_page, 0 );

	// First, identify the head
	head = my_bit2ndx( target_page->trackSelection );

// d_iag_printf( "0:\n" ); print_chain_data( target_page );

	// Break the head from other chain/s and make it a one-track chain
	make_chain_head( target_page, target_page->Track[head] );

// d_iag_printf( "00:\n" ); print_chain_data( target_page );

	temp = head + 1;

	// Iterate down from head and add selected tracks to this chain
	for ( tail = temp; tail < MATRIX_NROF_ROWS; tail++ ){

		// Next selected track is added to the chain of the current head
		if ( (target_page->trackSelection & ( 1 << tail )) != 0 ){

// d_iag_printf( "1:\n" ); print_chain_data( target_page );

			// Remove selected track from its previous chain
			remove_chain_element( target_page, target_page->Track[tail] );

// d_iag_printf( "2:\n" ); print_chain_data( target_page );

			// Make selected track the tail of the chain led by the current head
			make_chain_tail( 	target_page,
								target_page->Track[tail],
								target_page->Track[head] );

// d_iag_printf( "3:\n" ); print_chain_data( target_page );

			// Signal chainer activity in target_page; 4 is an un-#define'd chainmode
			target_page->chainMode = 4;
		}
	} // Tail search iterator


	// Align tracks to global clock - but only the ones in the playing pages
	if ( is_selected_in_GRID( target_page ) ){
		// set_page_locators( target_page, G_global_locator, my_max_value( G_TTC_abs_value, 1 ) );
		target_LOC = G_global_locator;
		target_TTC = my_max_value( G_TTC_abs_value, 1 );
	}
	else{
		// set_page_locators( target_page, 0, 1 );
		target_LOC = 0;
		target_TTC = 1;
	}

// d_iag_printf( "4:\n" ); print_chain_data( target_page );

	// Align all non-PLAY tracks to 0, and do not touch the PLAY tracks
	for ( i=0; i < MATRIX_NROF_ROWS; i++ ){

		// Work only on the PLAY tracks
		if ( target_page->Track[i]->chain_data[PLAY] == target_page->Track[i] ){

			// Set the actual locators
			set_track_locators( target_page, target_page->Track[i], target_LOC, target_TTC );
		}
	}

	// Void the track selection
	target_page->trackSelection = 0;

// d_iag_printf( "5:\n" ); print_chain_data( target_page );
// d_iag_printf( "\n" );

}



