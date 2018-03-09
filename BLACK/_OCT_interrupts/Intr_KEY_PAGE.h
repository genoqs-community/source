

		// Check for pressed track_selectors
		my_pressed_key = is_pressed_keyrange( 1, 10 );
		
		// Some chord button is pressed
		if ( is_pressed_keyrange( 252, 258 ) != FALSE ){
	
			// If a page button is pressed as well, switch to step mode			
			if ( is_pressed_pagerange() != FALSE ){

				// Compute Step coordinates
				row = row_of( is_pressed_pagerange() );
				col = column_of( is_pressed_pagerange() );
				
				// Select the current step formally 
				Step_set_status( VIEWER_page->Step[row][col], STEPSTAT_SELECT, ON );
									
				// Update the step selection counter in page
				VIEWER_page->stepSelection = 1;

				// Remember STEP COORDINATES
				VIEWER_page->stepSelectionSingleRow = row;
				VIEWER_page->stepSelectionSingleCol = col;

				//d_iag_printf( "zooming into step row:%d col:%d ts:%d\n", 
				//		row, col, VIEWER_page->trackSelection );

				// Zoom into the step: Switch the mode
				VIEWER_page->keyMode = KMOD_THIS_STEP;
				zoom_level = zoomSTEP;
			}

			// Do not execute the rest of the clause
			break;
		}
		

		switch( VIEWER_page->OPS_mode ){
			
			case BIRDSEYE:
				// If the zoom key is not pressed anymore, return to default
				if (	!(is_pressed_key( KEY_ZOOM_PAGE ))
					){

					// Enter interactive mode
					VIEWER_page->OPS_mode = INTERACTIVE;
				}
				else{

					// Nothing to do as of yet..
				}
				break;


			case INTERACTIVE:

				// Only act like this in Page mode with no steps selected
				if (	( zoom_level != zoomPAGE )
					||	( VIEWER_page->stepSelection != 0 )
					){
					break;
				}
					
				// Selector is pressed in KMOD_PAGE mode	
				if (	( my_pressed_key != 0 )
					&&	( VIEWER_page->keyMode == KMOD_PAGE )
					){
		
					// Ckeck whether the SEL button is active as well..
					// ..or if we are in passive mode already - 
					if (	( is_pressed_key( KEY_SELECT_MASTER ) ) 
						|| 	( VIEWER_page->OPS_mode == PASSIVE ) 
						){
		
						// Enter PASSIVE mode - basically locking the track selection
						VIEWER_page->OPS_mode = PASSIVE;
					}
				}

				else if ( (is_pressed_key( KEY_ZOOM_PAGE 		)) ){
					
					// Enter the Birds Eye view
					VIEWER_page->OPS_mode = BIRDSEYE;
				}

				else {
					
					// No interesting key is pressed, so forget selection.	
					if ( VIEWER_page->SEL_LOCK == OFF ){
						VIEWER_page->trackSelection = 0;					
					}
				
					if (	( is_pressed_key( KEY_SELECT_MASTER ) ) ){

						VIEWER_page->OPS_mode = PASSIVE;						
					}
				}
				break;


			case PASSIVE:
			
				// Nothing to do yet
				if (	( my_pressed_key == 0 )
					&&	( !(is_pressed_key( KEY_SELECT_MASTER 	)) )
					){

					// Enter interactive mode
					VIEWER_page->OPS_mode = INTERACTIVE;
				}

				break;

		} // switch VIEWER_page->OPS_mode	



