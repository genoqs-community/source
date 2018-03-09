
	// Page selectors, act as always..
	// ..select the page for play in preselection or zoom into it (doubleclick)

	
		// D O U B L E - C L I C K  C O N S T R U C T
		// DOUBLE CLICK SCENARIO
		if (	( DOUBLE_CLICK_TARGET == keyNdx ) 
			&& 	( DOUBLE_CLICK_TIMER   > DOUBLE_CLICK_ALARM_SENSITIVITY ) ) {

			// Double click code
			// ...
			
			// Zoom into the VIEWER_page (implicitly)			
			zoom_level = zoomPAGE;
			VIEWER_page->keyMode = KMOD_PAGE;

			// Move the cursor
			GRID_CURSOR = VIEWER_page->pageNdx;
			
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

			// Compute the coordinates first.
			i 			= row_of(keyNdx) + (10 * column_of (keyNdx));
//			if ( i < MATRIX_NROF_PAGES ){		
				VIEWER_page = &Page_repository[ i ];	
//			}
			
			// Move the cursor.
			GRID_CURSOR = VIEWER_page->pageNdx;

			// d_iag_printf( "page button pressed\n" );

			// Move cursor or zoom into page
			if ( is_pressed_key( KEY_ZOOM_PAGE )){


				// Zoom into the VIEWER_page (implicitly)			
				zoom_level = zoomPAGE;
				VIEWER_page->keyMode = KMOD_PAGE;
			}
		}




