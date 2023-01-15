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





// NEW SCHOOL CHAINSELECTORS
void key_exe_chainselectors( unsigned short keyNdx ){

	unsigned char row = 0;

	// Work on the page under the grid cursor
	Pagestruct* target_page = &Page_repository[GRID_CURSOR];


	if ( (		(keyNdx == KEY_CHAINMODE_4)
			||  (keyNdx == KEY_CHAINMODE_3)
			||	(keyNdx == KEY_CHAINMODE_2)
			|| 	(keyNdx == KEY_CHAINMODE_1) 
			||	(keyNdx == KEY_CHAINER )
			)
		){
	
		// Void the REC pattern
		Page_setTrackRecPattern( target_page, 0 );

		// Act depending on the pressed key
		switch( keyNdx ){
		
			case KEY_CHAINER:
				if ( target_page->chainMode == 4 ){
				
					// Toggles head track listening in page mode
					target_page->CHAINS_PLAY_HEAD ^= 1;					
				}

				break;
				

			// 10 x 1 row
			case KEY_CHAINMODE_1:
				
				if ( target_page->chainMode == CHAINMODE_1 ){
				
					// Toggles the lauflicht in page mode
					target_page->CHAINS_PLAY_HEAD ^= 1;
				}
						
				// Align all tracks in page to play by themselves
				for ( row=0; row < MATRIX_NROF_ROWS; row++ ){
				
					target_page->Track[row]->chain_data[HEAD] = target_page->Track[row];
					target_page->Track[row]->chain_data[PREV] = target_page->Track[row];
					target_page->Track[row]->chain_data[NEXT] = target_page->Track[row];
					target_page->Track[row]->chain_data[PLAY] = target_page->Track[row];
					target_page->Track[row]->attr_LOCATOR = G_global_locator;
					target_page->Track[row]->frame_ndx
						= target_page->Track[row]->attr_LOCATOR;		
				}
				target_page->chainMode = CHAINMODE_1;

				break;
				

			//  5 x 2 rows				
			case KEY_CHAINMODE_2:

				if ( target_page->chainMode == CHAINMODE_2 ){
				
					// Toggles the lauflicht in page mode
					target_page->CHAINS_PLAY_HEAD ^= 1;
				}

				// Align tracks in 5 pairs
				for ( row=0; row < MATRIX_NROF_ROWS; row++ ){
				
					// ROW 1 assignments
					target_page->Track[row]->chain_data[HEAD] = target_page->Track[row];
					target_page->Track[row]->chain_data[PREV] = target_page->Track[row+1];
					target_page->Track[row]->chain_data[NEXT] = target_page->Track[row+1];
					target_page->Track[row]->chain_data[PLAY] = target_page->Track[row];
					target_page->Track[row]->attr_LOCATOR = G_global_locator;
					target_page->Track[row]->frame_ndx
						= target_page->Track[row]->attr_LOCATOR;		
					
					// ROW 2 assignments				
					target_page->Track[row+1]->chain_data[HEAD] = target_page->Track[row];
					target_page->Track[row+1]->chain_data[PREV] = target_page->Track[row];
					target_page->Track[row+1]->chain_data[NEXT] = target_page->Track[row];
					target_page->Track[row+1]->chain_data[PLAY] = target_page->Track[row];
					target_page->Track[row+1]->attr_LOCATOR = 0;
					target_page->Track[row+1]->frame_ndx
						= target_page->Track[row+1]->attr_LOCATOR;		
	
					// Jump offset for the iterator (to the next head track)
					row = row + 1;
				}
				target_page->chainMode = CHAINMODE_2;

				break;
				

			//  2 x 4 rows				
			case KEY_CHAINMODE_3:

				if ( target_page->chainMode == CHAINMODE_3 ){
				
					// Toggles the lauflicht in page mode
					target_page->CHAINS_PLAY_HEAD ^= 1;
				}

				// Align tracks in 2 4-tuples and 1 pair
				for ( row=0; row < MATRIX_NROF_ROWS; row++ ){
				
					// ROW 1 assignments
					target_page->Track[row]->chain_data[HEAD] = target_page->Track[row];
					target_page->Track[row]->chain_data[PREV] = target_page->Track[row+3];
					target_page->Track[row]->chain_data[NEXT] = target_page->Track[row+1];
					target_page->Track[row]->chain_data[PLAY] = target_page->Track[row];
					target_page->Track[row]->attr_LOCATOR = G_global_locator;
					target_page->Track[row]->frame_ndx
						= target_page->Track[row]->attr_LOCATOR;		
					
					// ROW 2 assignment
					target_page->Track[row+1]->chain_data[HEAD] = target_page->Track[row];
					target_page->Track[row+1]->chain_data[PREV] = target_page->Track[row];
					target_page->Track[row+1]->chain_data[NEXT] = target_page->Track[row+2];
					target_page->Track[row+1]->chain_data[PLAY] = target_page->Track[row];
					target_page->Track[row+1]->attr_LOCATOR = 0;
					target_page->Track[row+1]->frame_ndx
						= target_page->Track[row+1]->attr_LOCATOR;		

					// ROW 3 assignment
					target_page->Track[row+2]->chain_data[HEAD] = target_page->Track[row];
					target_page->Track[row+2]->chain_data[PREV] = target_page->Track[row+1];
					target_page->Track[row+2]->chain_data[NEXT] = target_page->Track[row+3];
					target_page->Track[row+2]->chain_data[PLAY] = target_page->Track[row];
					target_page->Track[row+2]->attr_LOCATOR = 0;
					target_page->Track[row+2]->frame_ndx
						= target_page->Track[row+2]->attr_LOCATOR;		

					// ROW 4 assignment
					target_page->Track[row+3]->chain_data[HEAD] = target_page->Track[row];
					target_page->Track[row+3]->chain_data[PREV] = target_page->Track[row+2];
					target_page->Track[row+3]->chain_data[NEXT] = target_page->Track[row];
					target_page->Track[row+3]->chain_data[PLAY] = target_page->Track[row];
					target_page->Track[row+3]->attr_LOCATOR = 0;
					target_page->Track[row+3]->frame_ndx
						= target_page->Track[row+3]->attr_LOCATOR;		

					// Jump offset for the iterator (to the next head track)
					row = row + 3;
					// Ignore the last two tracks remaining
					if ( row > 3 ) 	row = MATRIX_NROF_ROWS;;
				}
				target_page->chainMode = CHAINMODE_3;

				break;


			//  1 x 8 rows
			case KEY_CHAINMODE_4:

				if ( target_page->chainMode == CHAINMODE_4 ){
				
					// Toggles the lauflicht in page mode
					target_page->CHAINS_PLAY_HEAD ^= 1;
				}

				// Align tracks in 1 8-tuple and 1 pair - this is only iterated once
				for ( row=0; row < MATRIX_NROF_ROWS; row++ ){
				
					// ROW 1 assignments
					target_page->Track[0]->chain_data[HEAD] = target_page->Track[0];
					target_page->Track[0]->chain_data[PREV] = target_page->Track[7];
					target_page->Track[0]->chain_data[NEXT] = target_page->Track[1];
					target_page->Track[0]->chain_data[PLAY] = target_page->Track[0];
					target_page->Track[0]->attr_LOCATOR = G_global_locator;
					target_page->Track[0]->frame_ndx
						= target_page->Track[0]->attr_LOCATOR;		
					
					// ROW 2 assignment
					target_page->Track[1]->chain_data[HEAD] = target_page->Track[0];
					target_page->Track[1]->chain_data[PREV] = target_page->Track[0];
					target_page->Track[1]->chain_data[NEXT] = target_page->Track[2];
					target_page->Track[1]->chain_data[PLAY] = target_page->Track[0];
					target_page->Track[1]->attr_LOCATOR = 0;
					target_page->Track[1]->frame_ndx
						= target_page->Track[1]->attr_LOCATOR;		

					// ROW 3 assignment
					target_page->Track[2]->chain_data[HEAD] = target_page->Track[0];
					target_page->Track[2]->chain_data[PREV] = target_page->Track[1];
					target_page->Track[2]->chain_data[NEXT] = target_page->Track[3];
					target_page->Track[2]->chain_data[PLAY] = target_page->Track[0];
					target_page->Track[2]->attr_LOCATOR = 0;
					target_page->Track[2]->frame_ndx
						= target_page->Track[2]->attr_LOCATOR;		

					// ROW 4 assignment
					target_page->Track[3]->chain_data[HEAD] = target_page->Track[0];
					target_page->Track[3]->chain_data[PREV] = target_page->Track[2];
					target_page->Track[3]->chain_data[NEXT] = target_page->Track[4];
					target_page->Track[3]->chain_data[PLAY] = target_page->Track[0];
					target_page->Track[3]->attr_LOCATOR = 0;
					target_page->Track[3]->frame_ndx
						= target_page->Track[3]->attr_LOCATOR;

					// ROW 5 assignments
					target_page->Track[4]->chain_data[HEAD] = target_page->Track[0];
					target_page->Track[4]->chain_data[PREV] = target_page->Track[3];
					target_page->Track[4]->chain_data[NEXT] = target_page->Track[5];
					target_page->Track[4]->chain_data[PLAY] = target_page->Track[0];
					target_page->Track[4]->attr_LOCATOR = 0;
					target_page->Track[4]->frame_ndx
						= target_page->Track[4]->attr_LOCATOR;		
					
					// ROW 6 assignment
					target_page->Track[5]->chain_data[HEAD] = target_page->Track[0];
					target_page->Track[5]->chain_data[PREV] = target_page->Track[4];
					target_page->Track[5]->chain_data[NEXT] = target_page->Track[6];
					target_page->Track[5]->chain_data[PLAY] = target_page->Track[0];
					target_page->Track[5]->attr_LOCATOR = 0;
					target_page->Track[5]->frame_ndx
						= target_page->Track[5]->attr_LOCATOR;		

					// ROW 7 assignment
					target_page->Track[6]->chain_data[HEAD] = target_page->Track[0];
					target_page->Track[6]->chain_data[PREV] = target_page->Track[5];
					target_page->Track[6]->chain_data[NEXT] = target_page->Track[7];
					target_page->Track[6]->chain_data[PLAY] = target_page->Track[0];
					target_page->Track[6]->attr_LOCATOR = 0;
					target_page->Track[6]->frame_ndx
						= target_page->Track[6]->attr_LOCATOR;		

					// ROW 8 assignment
					target_page->Track[7]->chain_data[HEAD] = target_page->Track[0];
					target_page->Track[7]->chain_data[PREV] = target_page->Track[6];
					target_page->Track[7]->chain_data[NEXT] = target_page->Track[0];
					target_page->Track[7]->chain_data[PLAY] = target_page->Track[0];
					target_page->Track[7]->attr_LOCATOR = 0;
					target_page->Track[7]->frame_ndx
						= target_page->Track[7]->attr_LOCATOR;		
					
					// ROW 9 assignment
					target_page->Track[8]->chain_data[HEAD] = target_page->Track[8];
					target_page->Track[8]->chain_data[PREV] = target_page->Track[8];
					target_page->Track[8]->chain_data[NEXT] = target_page->Track[8];
					target_page->Track[8]->chain_data[PLAY] = target_page->Track[8];
					target_page->Track[8]->attr_LOCATOR = G_global_locator;
					target_page->Track[8]->frame_ndx
						= target_page->Track[8]->attr_LOCATOR;		

					// ROW 0 assignment
					target_page->Track[9]->chain_data[HEAD] = target_page->Track[9];
					target_page->Track[9]->chain_data[PREV] = target_page->Track[9];
					target_page->Track[9]->chain_data[NEXT] = target_page->Track[9];
					target_page->Track[9]->chain_data[PLAY] = target_page->Track[9];
					target_page->Track[9]->attr_LOCATOR = G_global_locator;
					target_page->Track[9]->frame_ndx
						= target_page->Track[9]->attr_LOCATOR;		

					// Jump offset for the iterator (to the next head track)
					row = MATRIX_NROF_ROWS;
				}
				target_page->chainMode = CHAINMODE_4;

				break;
				
		} // switch( keyNdx )


		// Applies mutepattern so that it is synced up with chain mode
		apply_mutepattern( target_page, target_page->trackMutepattern );
		
		// Align tracks to global clock - but only the ones in the playing pages
		if ( is_selected_in_GRID( target_page ) ){

			set_page_locators( target_page, G_global_locator, my_max_value( G_TTC_abs_value, 1 ) );
		}
		else{

			set_page_locators( target_page, 0, 1 );
		}
	}
}

