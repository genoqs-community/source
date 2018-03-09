
	// Page selectors, act as always..
	// ..select the page for play in preselection or zoom into it (doubleclick)

	
		// D O U B L E - C L I C K  C O N S T R U C T
		// DOUBLE CLICK SCENARIO
		if (	( DOUBLE_CLICK_TARGET == keyNdx ) 
			&& 	( DOUBLE_CLICK_TIMER   > DOUBLE_CLICK_ALARM_SENSITIVITY ) ) {

			// Double click code
			// ...
			if ( GRIDTRACK_OPS_mode == PASSIVE ){
				break;
			}			


			// Repeated Single click code - to neutralize previous press
			// Do not react on invalid presses
			if (	( column_of( keyNdx ) > 12 )
				||	( column_of( keyNdx ) <  3 )
				||	( GRID_p_selection[ row_of( keyNdx ) ] == NULL )
				){				
				return;
			}

			// ..true Double click code: 
			// Select the VIEWER page pertaining to the track.
			target_page = &Page_repository[ GRID_p_selection[ row_of(keyNdx) ]->pageNdx ];	
			// Move the cursor for the grid
			GRID_CURSOR = target_page->pageNdx;

			// Put the track into the track selection
			target_page->trackSelection ^= mirror( 1 << (column_of( keyNdx )-3), 10 );

			// Zoom into the target_page (implicitly) and the track there		
			target_page->keyMode = KMOD_THIS_TRACK;
			zoom_level = zoomTRACK;

		} // end of double click scenario 
		

		// SINGLE CLICK SCENARIO
		else if (DOUBLE_CLICK_TARGET == 0) {
			
				DOUBLE_CLICK_TARGET = keyNdx;
				DOUBLE_CLICK_TIMER = ON;
				// Start the Double click Alarm
				cyg_alarm_initialize(	
						doubleClickAlarm_hdl, 
						cyg_current_time() + DOUBLE_CLICK_ALARM_TIME,
						DOUBLE_CLICK_ALARM_TIME );

			// Single click code
			// ...
			
			// Do not react on invalid presses
			if (	( column_of( keyNdx ) > 12 )
				||	( column_of( keyNdx ) <  3 )
				||	( GRID_p_selection[ row_of( keyNdx ) ] == NULL )
				){
				
				return;
			}			

			// Determine row of the key index
			row =  row_of( keyNdx );

			// If gridtrack border is pressed, toggle track status
			// Otherwise toggle selection status
			switch( is_pressed_gridtrack_border() ){
			
				case TRUE:
					// Adjust the current selection pattern
					GRID_p_selection[row]->trackMutepattern ^= mirror( 1 << ( column_of( keyNdx ) - 3 ), 10 );
					break;
				

				case FALSE:
					// Adjust the current selection pattern
					GRID_p_selection[row]->trackSelection ^= mirror( 1 << ( column_of( keyNdx ) - 3 ), 10 );
		
					// Remember it accordingly
					if ( is_pressed_key( KEY_SELECT_MASTER ) ){
					
						GRID_p_selection[row]->trackSelectionStored = 
							GRID_p_selection[row]->trackSelection;
					}

					break;
			}


		}




