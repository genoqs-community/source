

// NEW SCHOOL CHAINSELECTORS
void key_exe_chainselectors( unsigned short keyNdx ){

//	unsigned char row = 0;
//
//	// Work on the page under the grid cursor
//	Pagestruct* target_page = &Page_repository[GRID_CURSOR];
//
//
//	if ( (		(keyNdx == KEY_CHAINMODE_4)
//			||  (keyNdx == KEY_CHAINMODE_3)
//			||	(keyNdx == KEY_CHAINMODE_2)
//			|| 	(keyNdx == KEY_CHAINMODE_1) 
//			||	(keyNdx == KEY_CHAINER )
//			)
//		){			
//	
//		switch( keyNdx ){
//		
//			case KEY_CHAINER:
//				if ( target_page->chainMode == 4 ){
//				
//					// Toggles the lauflicht in page mode
//					CHAINS_PLAY_HEAD ^= 1;					
//				}
//
//				break;
//				
//
//			// 10 x 1 row
//			case KEY_CHAINMODE_1:
//				
//				if ( target_page->chainMode == CHAINMODE_1 ){
//				
//					// Toggles the lauflicht in page mode
//					CHAINS_PLAY_HEAD ^= 1;
//				}
//						
//				// Align all tracks in page to play by themselves
//				for ( row=0; row < MATRIX_NROF_ROWS; row++ ){
//				
//					target_page->Track[row]->chain_data[HEAD] = target_page->Track[row];
//					target_page->Track[row]->chain_data[PREV] = target_page->Track[row];
//					target_page->Track[row]->chain_data[NEXT] = target_page->Track[row];
//					target_page->Track[row]->chain_data[PLAY] = target_page->Track[row];
//					target_page->Track[row]->attribute[ATTR_LOCATOR] = GLOBAL_LOCATOR;
//					target_page->Track[row]->trigger_set_ndx
//						= target_page->Track[row]->attribute[ATTR_LOCATOR];		
//				}
//				target_page->chainMode = CHAINMODE_1;
//
//				break;
//				
//
//			//  5 x 2 rows				
//			case KEY_CHAINMODE_2:
//
//				if ( target_page->chainMode == CHAINMODE_2 ){
//				
//					// Toggles the lauflicht in page mode
//					CHAINS_PLAY_HEAD ^= 1;
//				}
//
//				// Align tracks in 5 pairs
//				for ( row=0; row < MATRIX_NROF_ROWS; row++ ){
//				
//					// ROW 1 assignments
//					target_page->Track[row]->chain_data[HEAD] = target_page->Track[row];
//					target_page->Track[row]->chain_data[PREV] = target_page->Track[row+1];
//					target_page->Track[row]->chain_data[NEXT] = target_page->Track[row+1];
//					target_page->Track[row]->chain_data[PLAY] = target_page->Track[row];
//					target_page->Track[row]->attribute[ATTR_LOCATOR] = GLOBAL_LOCATOR;
//					target_page->Track[row]->trigger_set_ndx
//						= target_page->Track[row]->attribute[ATTR_LOCATOR];		
//					
//					// ROW 2 assignments				
//					target_page->Track[row+1]->chain_data[HEAD] = target_page->Track[row];
//					target_page->Track[row+1]->chain_data[PREV] = target_page->Track[row];
//					target_page->Track[row+1]->chain_data[NEXT] = target_page->Track[row];
//					target_page->Track[row+1]->chain_data[PLAY] = target_page->Track[row];
//					target_page->Track[row+1]->attribute[ATTR_LOCATOR] = 0;
//					target_page->Track[row+1]->trigger_set_ndx
//						= target_page->Track[row+1]->attribute[ATTR_LOCATOR];		
//	
//					// Jump offset for the iterator (to the next head track)
//					row = row + 1;
//				}
//				target_page->chainMode = CHAINMODE_2;
//
//				break;
//				
//
//			//  2 x 4 rows				
//			case KEY_CHAINMODE_3:
//
//				if ( target_page->chainMode == CHAINMODE_3 ){
//				
//					// Toggles the lauflicht in page mode
//					CHAINS_PLAY_HEAD ^= 1;
//				}
//
//				// Align tracks in 5 pairs
//				for ( row=0; row < MATRIX_NROF_ROWS; row++ ){
//				
//					// ROW 1 assignments
//					target_page->Track[row]->chain_data[HEAD] = target_page->Track[row];
//					target_page->Track[row]->chain_data[PREV] = target_page->Track[row+3];
//					target_page->Track[row]->chain_data[NEXT] = target_page->Track[row+1];
//					target_page->Track[row]->chain_data[PLAY] = target_page->Track[row];
//					target_page->Track[row]->attribute[ATTR_LOCATOR] = GLOBAL_LOCATOR;
//					target_page->Track[row]->trigger_set_ndx
//						= target_page->Track[row]->attribute[ATTR_LOCATOR];		
//					
//					// ROW 2 assignment
//					target_page->Track[row+1]->chain_data[HEAD] = target_page->Track[row];
//					target_page->Track[row+1]->chain_data[PREV] = target_page->Track[row];
//					target_page->Track[row+1]->chain_data[NEXT] = target_page->Track[row+2];
//					target_page->Track[row+1]->chain_data[PLAY] = target_page->Track[row];
//					target_page->Track[row+1]->attribute[ATTR_LOCATOR] = 0;
//					target_page->Track[row+1]->trigger_set_ndx
//						= target_page->Track[row+1]->attribute[ATTR_LOCATOR];		
//
//					// ROW 3 assignment
//					target_page->Track[row+2]->chain_data[HEAD] = target_page->Track[row];
//					target_page->Track[row+2]->chain_data[PREV] = target_page->Track[row+1];
//					target_page->Track[row+2]->chain_data[NEXT] = target_page->Track[row+3];
//					target_page->Track[row+2]->chain_data[PLAY] = target_page->Track[row];
//					target_page->Track[row+2]->attribute[ATTR_LOCATOR] = 0;
//					target_page->Track[row+2]->trigger_set_ndx
//						= target_page->Track[row+2]->attribute[ATTR_LOCATOR];		
//
//					// ROW 4 assignment
//					target_page->Track[row+3]->chain_data[HEAD] = target_page->Track[row];
//					target_page->Track[row+3]->chain_data[PREV] = target_page->Track[row+2];
//					target_page->Track[row+3]->chain_data[NEXT] = target_page->Track[row];
//					target_page->Track[row+3]->chain_data[PLAY] = target_page->Track[row];
//					target_page->Track[row+3]->attribute[ATTR_LOCATOR] = 0;
//					target_page->Track[row+3]->trigger_set_ndx
//						= target_page->Track[row+3]->attribute[ATTR_LOCATOR];		
//
//					// Jump offset for the iterator (to the next head track)
//					row = row + 3;
//					// Ignore the last two tracks remaining
//					if ( row > 3 ) 	row = MATRIX_NROF_ROWS;;
//				}
//				target_page->chainMode = CHAINMODE_3;
//
//				break;
//
//
//			//  1 x 8 rows
//			case KEY_CHAINMODE_4:
//
//				if ( target_page->chainMode == CHAINMODE_4 ){
//				
//					// Toggles the lauflicht in page mode
//					CHAINS_PLAY_HEAD ^= 1;
//				}
//
//				// Align tracks in 5 pairs
//				for ( row=0; row < MATRIX_NROF_ROWS; row++ ){
//				
//					// ROW 1 assignments
//					target_page->Track[row]->chain_data[HEAD] = target_page->Track[row];
//					target_page->Track[row]->chain_data[PREV] = target_page->Track[row+3];
//					target_page->Track[row]->chain_data[NEXT] = target_page->Track[row+1];
//					target_page->Track[row]->chain_data[PLAY] = target_page->Track[row];
//					target_page->Track[row]->attribute[ATTR_LOCATOR] = GLOBAL_LOCATOR;
//					target_page->Track[row]->trigger_set_ndx
//						= target_page->Track[row]->attribute[ATTR_LOCATOR];		
//					
//					// ROW 2 assignment
//					target_page->Track[row+1]->chain_data[HEAD] = target_page->Track[row];
//					target_page->Track[row+1]->chain_data[PREV] = target_page->Track[row];
//					target_page->Track[row+1]->chain_data[NEXT] = target_page->Track[row+2];
//					target_page->Track[row+1]->chain_data[PLAY] = target_page->Track[row];
//					target_page->Track[row+1]->attribute[ATTR_LOCATOR] = 0;
//					target_page->Track[row+1]->trigger_set_ndx
//						= target_page->Track[row+1]->attribute[ATTR_LOCATOR];		
//
//					// ROW 3 assignment
//					target_page->Track[row+2]->chain_data[HEAD] = target_page->Track[row];
//					target_page->Track[row+2]->chain_data[PREV] = target_page->Track[row+1];
//					target_page->Track[row+2]->chain_data[NEXT] = target_page->Track[row+3];
//					target_page->Track[row+2]->chain_data[PLAY] = target_page->Track[row];
//					target_page->Track[row+2]->attribute[ATTR_LOCATOR] = 0;
//					target_page->Track[row+2]->trigger_set_ndx
//						= target_page->Track[row+2]->attribute[ATTR_LOCATOR];		
//
//					// ROW 4 assignment
//					target_page->Track[row+3]->chain_data[HEAD] = target_page->Track[row];
//					target_page->Track[row+3]->chain_data[PREV] = target_page->Track[row+2];
//					target_page->Track[row+3]->chain_data[NEXT] = target_page->Track[row+4];
//					target_page->Track[row+3]->chain_data[PLAY] = target_page->Track[row];
//					target_page->Track[row+3]->attribute[ATTR_LOCATOR] = 0;
//					target_page->Track[row+3]->trigger_set_ndx
//						= target_page->Track[row+3]->attribute[ATTR_LOCATOR];		
//
//					// ROW 5 assignments
//					target_page->Track[row+4]->chain_data[HEAD] = target_page->Track[row];
//					target_page->Track[row+4]->chain_data[PREV] = target_page->Track[row+3];
//					target_page->Track[row+4]->chain_data[NEXT] = target_page->Track[row+5];
//					target_page->Track[row+4]->chain_data[PLAY] = target_page->Track[row];
//					target_page->Track[row+4]->attribute[ATTR_LOCATOR] = 0;
//					target_page->Track[row+4]->trigger_set_ndx
//						= target_page->Track[row+4]->attribute[ATTR_LOCATOR];		
//					
//					// ROW 6 assignment
//					target_page->Track[row+5]->chain_data[HEAD] = target_page->Track[row];
//					target_page->Track[row+5]->chain_data[PREV] = target_page->Track[row+4];
//					target_page->Track[row+5]->chain_data[NEXT] = target_page->Track[row+6];
//					target_page->Track[row+5]->chain_data[PLAY] = target_page->Track[row];
//					target_page->Track[row+5]->attribute[ATTR_LOCATOR] = 0;
//					target_page->Track[row+5]->trigger_set_ndx
//						= target_page->Track[row+5]->attribute[ATTR_LOCATOR];		
//
//					// ROW 7 assignment
//					target_page->Track[row+6]->chain_data[HEAD] = target_page->Track[row];
//					target_page->Track[row+6]->chain_data[PREV] = target_page->Track[row+5];
//					target_page->Track[row+6]->chain_data[NEXT] = target_page->Track[row+7];
//					target_page->Track[row+6]->chain_data[PLAY] = target_page->Track[row];
//					target_page->Track[row+6]->attribute[ATTR_LOCATOR] = 0;
//					target_page->Track[row+6]->trigger_set_ndx
//						= target_page->Track[row+6]->attribute[ATTR_LOCATOR];		
//
//					// ROW 8 assignment
//					target_page->Track[row+7]->chain_data[HEAD] = target_page->Track[row];
//					target_page->Track[row+7]->chain_data[PREV] = target_page->Track[row+6];
//					target_page->Track[row+7]->chain_data[NEXT] = target_page->Track[row];
//					target_page->Track[row+7]->chain_data[PLAY] = target_page->Track[row];
//					target_page->Track[row+7]->attribute[ATTR_LOCATOR] = 0;
//					target_page->Track[row+7]->trigger_set_ndx
//						= target_page->Track[row+7]->attribute[ATTR_LOCATOR];		
//					
//					// Jump offset for the iterator (to the next head track)
//					row = MATRIX_NROF_ROWS;
//				}
//				target_page->chainMode = CHAINMODE_4;
//
//				break;
//				
//		} // switch( keyNdx )
//
//
//		// Align tracks to global clock
//		// Aligns the target_page to the global play data
//		set_page_locators( target_page, GLOBAL_LOCATOR, TTC_ABS_VALUE );
//	}
}

