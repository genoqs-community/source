

// NEW SCHOOL CHAINSELECTORS
void key_exe_chainselectors( unsigned short keyNdx ){

	unsigned char row = 0;

	if ( (		(keyNdx == KEY_CHAINMODE_4)
			||  (keyNdx == KEY_CHAINMODE_3)
			||	(keyNdx == KEY_CHAINMODE_2)
			|| 	(keyNdx == KEY_CHAINMODE_1) 
			||	(keyNdx == KEY_CHAINER )
			)
		){			
	
		switch( keyNdx ){
		
			case KEY_CHAINER:
				if ( VIEWER_page->chainMode == 4 ){
				
					// Toggles the lauflicht in page mode
					LAUFLICHT_DISPLAY ^= 1;					
				}

				break;
				

			// 10 x 1 row
			case KEY_CHAINMODE_1:
				
				if ( VIEWER_page->chainMode == CHAINMODE_1 ){
				
					// Toggles the lauflicht in page mode
					LAUFLICHT_DISPLAY ^= 1;
				}
						
				// Align all tracks in page to play by themselves
				for ( row=0; row < MATRIX_NROF_ROWS; row++ ){
				
					VIEWER_page->Track[row]->chain_data[HEAD] = VIEWER_page->Track[row];
					VIEWER_page->Track[row]->chain_data[PREV] = VIEWER_page->Track[row];
					VIEWER_page->Track[row]->chain_data[NEXT] = VIEWER_page->Track[row];
					VIEWER_page->Track[row]->chain_data[PLAY] = VIEWER_page->Track[row];
					VIEWER_page->Track[row]->attribute[ATTR_LOCATOR] = GLOBAL_LOCATOR;
					VIEWER_page->Track[row]->trigger_set_ndx
						= VIEWER_page->Track[row]->attribute[ATTR_LOCATOR];		
				}
				VIEWER_page->chainMode = CHAINMODE_1;

				break;
				

			//  5 x 2 rows				
			case KEY_CHAINMODE_2:

				if ( VIEWER_page->chainMode == CHAINMODE_2 ){
				
					// Toggles the lauflicht in page mode
					LAUFLICHT_DISPLAY ^= 1;
				}

				// Align tracks in 5 pairs
				for ( row=0; row < MATRIX_NROF_ROWS; row++ ){
				
					// ROW 1 assignments
					VIEWER_page->Track[row]->chain_data[HEAD] = VIEWER_page->Track[row];
					VIEWER_page->Track[row]->chain_data[PREV] = VIEWER_page->Track[row+1];
					VIEWER_page->Track[row]->chain_data[NEXT] = VIEWER_page->Track[row+1];
					VIEWER_page->Track[row]->chain_data[PLAY] = VIEWER_page->Track[row];
					VIEWER_page->Track[row]->attribute[ATTR_LOCATOR] = GLOBAL_LOCATOR;
					VIEWER_page->Track[row]->trigger_set_ndx
						= VIEWER_page->Track[row]->attribute[ATTR_LOCATOR];		
					
					// ROW 2 assignments				
					VIEWER_page->Track[row+1]->chain_data[HEAD] = VIEWER_page->Track[row];
					VIEWER_page->Track[row+1]->chain_data[PREV] = VIEWER_page->Track[row];
					VIEWER_page->Track[row+1]->chain_data[NEXT] = VIEWER_page->Track[row];
					VIEWER_page->Track[row+1]->chain_data[PLAY] = VIEWER_page->Track[row];
					VIEWER_page->Track[row+1]->attribute[ATTR_LOCATOR] = 0;
					VIEWER_page->Track[row+1]->trigger_set_ndx
						= VIEWER_page->Track[row+1]->attribute[ATTR_LOCATOR];		
	
					// Jump offset for the iterator (to the next head track)
					row = row + 1;
				}
				VIEWER_page->chainMode = CHAINMODE_2;

				break;
				

			//  2 x 4 rows				
			case KEY_CHAINMODE_3:

				if ( VIEWER_page->chainMode == CHAINMODE_3 ){
				
					// Toggles the lauflicht in page mode
					LAUFLICHT_DISPLAY ^= 1;
				}

				// Align tracks in 5 pairs
				for ( row=0; row < MATRIX_NROF_ROWS; row++ ){
				
					// ROW 1 assignments
					VIEWER_page->Track[row]->chain_data[HEAD] = VIEWER_page->Track[row];
					VIEWER_page->Track[row]->chain_data[PREV] = VIEWER_page->Track[row+3];
					VIEWER_page->Track[row]->chain_data[NEXT] = VIEWER_page->Track[row+1];
					VIEWER_page->Track[row]->chain_data[PLAY] = VIEWER_page->Track[row];
					VIEWER_page->Track[row]->attribute[ATTR_LOCATOR] = GLOBAL_LOCATOR;
					VIEWER_page->Track[row]->trigger_set_ndx
						= VIEWER_page->Track[row]->attribute[ATTR_LOCATOR];		
					
					// ROW 2 assignment
					VIEWER_page->Track[row+1]->chain_data[HEAD] = VIEWER_page->Track[row];
					VIEWER_page->Track[row+1]->chain_data[PREV] = VIEWER_page->Track[row];
					VIEWER_page->Track[row+1]->chain_data[NEXT] = VIEWER_page->Track[row+2];
					VIEWER_page->Track[row+1]->chain_data[PLAY] = VIEWER_page->Track[row];
					VIEWER_page->Track[row+1]->attribute[ATTR_LOCATOR] = 0;
					VIEWER_page->Track[row+1]->trigger_set_ndx
						= VIEWER_page->Track[row+1]->attribute[ATTR_LOCATOR];		

					// ROW 3 assignment
					VIEWER_page->Track[row+2]->chain_data[HEAD] = VIEWER_page->Track[row];
					VIEWER_page->Track[row+2]->chain_data[PREV] = VIEWER_page->Track[row+1];
					VIEWER_page->Track[row+2]->chain_data[NEXT] = VIEWER_page->Track[row+3];
					VIEWER_page->Track[row+2]->chain_data[PLAY] = VIEWER_page->Track[row];
					VIEWER_page->Track[row+2]->attribute[ATTR_LOCATOR] = 0;
					VIEWER_page->Track[row+2]->trigger_set_ndx
						= VIEWER_page->Track[row+2]->attribute[ATTR_LOCATOR];		

					// ROW 4 assignment
					VIEWER_page->Track[row+3]->chain_data[HEAD] = VIEWER_page->Track[row];
					VIEWER_page->Track[row+3]->chain_data[PREV] = VIEWER_page->Track[row+2];
					VIEWER_page->Track[row+3]->chain_data[NEXT] = VIEWER_page->Track[row];
					VIEWER_page->Track[row+3]->chain_data[PLAY] = VIEWER_page->Track[row];
					VIEWER_page->Track[row+3]->attribute[ATTR_LOCATOR] = 0;
					VIEWER_page->Track[row+3]->trigger_set_ndx
						= VIEWER_page->Track[row+3]->attribute[ATTR_LOCATOR];		

					// Jump offset for the iterator (to the next head track)
					row = row + 3;
					// Ignore the last two tracks remaining
					if ( row > 3 ) 	row = MATRIX_NROF_ROWS;;
				}
				VIEWER_page->chainMode = CHAINMODE_3;

				break;


			//  1 x 8 rows
			case KEY_CHAINMODE_4:

				if ( VIEWER_page->chainMode == CHAINMODE_4 ){
				
					// Toggles the lauflicht in page mode
					LAUFLICHT_DISPLAY ^= 1;
				}

				// Align tracks in 5 pairs
				for ( row=0; row < MATRIX_NROF_ROWS; row++ ){
				
					// ROW 1 assignments
					VIEWER_page->Track[row]->chain_data[HEAD] = VIEWER_page->Track[row];
					VIEWER_page->Track[row]->chain_data[PREV] = VIEWER_page->Track[row+3];
					VIEWER_page->Track[row]->chain_data[NEXT] = VIEWER_page->Track[row+1];
					VIEWER_page->Track[row]->chain_data[PLAY] = VIEWER_page->Track[row];
					VIEWER_page->Track[row]->attribute[ATTR_LOCATOR] = GLOBAL_LOCATOR;
					VIEWER_page->Track[row]->trigger_set_ndx
						= VIEWER_page->Track[row]->attribute[ATTR_LOCATOR];		
					
					// ROW 2 assignment
					VIEWER_page->Track[row+1]->chain_data[HEAD] = VIEWER_page->Track[row];
					VIEWER_page->Track[row+1]->chain_data[PREV] = VIEWER_page->Track[row];
					VIEWER_page->Track[row+1]->chain_data[NEXT] = VIEWER_page->Track[row+2];
					VIEWER_page->Track[row+1]->chain_data[PLAY] = VIEWER_page->Track[row];
					VIEWER_page->Track[row+1]->attribute[ATTR_LOCATOR] = 0;
					VIEWER_page->Track[row+1]->trigger_set_ndx
						= VIEWER_page->Track[row+1]->attribute[ATTR_LOCATOR];		

					// ROW 3 assignment
					VIEWER_page->Track[row+2]->chain_data[HEAD] = VIEWER_page->Track[row];
					VIEWER_page->Track[row+2]->chain_data[PREV] = VIEWER_page->Track[row+1];
					VIEWER_page->Track[row+2]->chain_data[NEXT] = VIEWER_page->Track[row+3];
					VIEWER_page->Track[row+2]->chain_data[PLAY] = VIEWER_page->Track[row];
					VIEWER_page->Track[row+2]->attribute[ATTR_LOCATOR] = 0;
					VIEWER_page->Track[row+2]->trigger_set_ndx
						= VIEWER_page->Track[row+2]->attribute[ATTR_LOCATOR];		

					// ROW 4 assignment
					VIEWER_page->Track[row+3]->chain_data[HEAD] = VIEWER_page->Track[row];
					VIEWER_page->Track[row+3]->chain_data[PREV] = VIEWER_page->Track[row+2];
					VIEWER_page->Track[row+3]->chain_data[NEXT] = VIEWER_page->Track[row+4];
					VIEWER_page->Track[row+3]->chain_data[PLAY] = VIEWER_page->Track[row];
					VIEWER_page->Track[row+3]->attribute[ATTR_LOCATOR] = 0;
					VIEWER_page->Track[row+3]->trigger_set_ndx
						= VIEWER_page->Track[row+3]->attribute[ATTR_LOCATOR];		

					// ROW 5 assignments
					VIEWER_page->Track[row+4]->chain_data[HEAD] = VIEWER_page->Track[row];
					VIEWER_page->Track[row+4]->chain_data[PREV] = VIEWER_page->Track[row+3];
					VIEWER_page->Track[row+4]->chain_data[NEXT] = VIEWER_page->Track[row+5];
					VIEWER_page->Track[row+4]->chain_data[PLAY] = VIEWER_page->Track[row];
					VIEWER_page->Track[row+4]->attribute[ATTR_LOCATOR] = 0;
					VIEWER_page->Track[row+4]->trigger_set_ndx
						= VIEWER_page->Track[row+4]->attribute[ATTR_LOCATOR];		
					
					// ROW 6 assignment
					VIEWER_page->Track[row+5]->chain_data[HEAD] = VIEWER_page->Track[row];
					VIEWER_page->Track[row+5]->chain_data[PREV] = VIEWER_page->Track[row+4];
					VIEWER_page->Track[row+5]->chain_data[NEXT] = VIEWER_page->Track[row+6];
					VIEWER_page->Track[row+5]->chain_data[PLAY] = VIEWER_page->Track[row];
					VIEWER_page->Track[row+5]->attribute[ATTR_LOCATOR] = 0;
					VIEWER_page->Track[row+5]->trigger_set_ndx
						= VIEWER_page->Track[row+5]->attribute[ATTR_LOCATOR];		

					// ROW 7 assignment
					VIEWER_page->Track[row+6]->chain_data[HEAD] = VIEWER_page->Track[row];
					VIEWER_page->Track[row+6]->chain_data[PREV] = VIEWER_page->Track[row+5];
					VIEWER_page->Track[row+6]->chain_data[NEXT] = VIEWER_page->Track[row+7];
					VIEWER_page->Track[row+6]->chain_data[PLAY] = VIEWER_page->Track[row];
					VIEWER_page->Track[row+6]->attribute[ATTR_LOCATOR] = 0;
					VIEWER_page->Track[row+6]->trigger_set_ndx
						= VIEWER_page->Track[row+6]->attribute[ATTR_LOCATOR];		

					// ROW 8 assignment
					VIEWER_page->Track[row+7]->chain_data[HEAD] = VIEWER_page->Track[row];
					VIEWER_page->Track[row+7]->chain_data[PREV] = VIEWER_page->Track[row+6];
					VIEWER_page->Track[row+7]->chain_data[NEXT] = VIEWER_page->Track[row];
					VIEWER_page->Track[row+7]->chain_data[PLAY] = VIEWER_page->Track[row];
					VIEWER_page->Track[row+7]->attribute[ATTR_LOCATOR] = 0;
					VIEWER_page->Track[row+7]->trigger_set_ndx
						= VIEWER_page->Track[row+7]->attribute[ATTR_LOCATOR];		
					
					// Jump offset for the iterator (to the next head track)
					row = MATRIX_NROF_ROWS;
				}
				VIEWER_page->chainMode = CHAINMODE_4;

				break;
				
		} // switch( keyNdx )


		// Align tracks to global clock
		// Aligns the VIEWER_page to the global play data
		set_page_locators( VIEWER_page, GLOBAL_LOCATOR, TTC_ABS_VALUE );
	}
}

