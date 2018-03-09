

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
				Step_set_status( target_page->Step[row][col], STEPSTAT_SELECT, ON );
									
				// Update the step selection counter in page
				target_page->stepSelection = 1;

				// Remember STEP COORDINATES
				target_page->stepSelectionSingleRow = row;
				target_page->stepSelectionSingleCol = col;

				//d_iag_printf( "zooming into step row:%d col:%d ts:%d\n", 
				//		row, col, target_page->trackSelection );

				// Zoom into the step: Switch the mode
				target_page->keyMode = KMOD_THIS_STEP;
				zoom_level = zoomSTEP;
			}

			// Do not execute the rest of the clause
			break;
		}
		

		switch( target_page->OPS_mode ){
			
			case BIRDSEYE:
				// If the zoom key is not pressed anymore, return to default
				if (	!(is_pressed_key( KEY_ZOOM_PAGE ))
					){

					// Enter interactive mode
					target_page->OPS_mode = INTERACTIVE;
// d_iag_printf( "going 0 BV->INTERACTIVE.. cursor:%d ndx:%d\n", GRID_CURSOR, target_page->pageNdx );	
				}
				else{

					// Nothing to do as of yet..
				}
				break;


			case INTERACTIVE:

				// Only act like this in Page mode with no steps selected
				if (	( zoom_level != zoomPAGE )
					||	( target_page->stepSelection != 0 )
					){
					break;
				}
					
				// Selector is pressed in KMOD_PAGE mode	
				if (	( my_pressed_key != 0 )
					&&	( target_page->keyMode == KMOD_PAGE )
					){
		
					// Ckeck whether the SEL button is active as well..
					// ..or if we are in passive mode already - 
					if (	( is_pressed_key( KEY_SELECT_MASTER ) ) 
						|| 	( target_page->OPS_mode == PASSIVE ) 
						){
		
						// Enter PASSIVE mode - basically locking the track selection
						target_page->OPS_mode = PASSIVE;

// d_iag_printf( "going 1 IV->PASSIVE.. cursor:%d ndx:%d\n", GRID_CURSOR, target_page->pageNdx );	
					}
				}

				else if ( (is_pressed_key( KEY_ZOOM_PAGE 		)) ){
					
					// Enter the Birds Eye view
					target_page->OPS_mode = BIRDSEYE;
					target_page->keyMode = KMOD_PAGE;
	
// d_iag_printf( "going 2 IV->BIRDSEYE.. cursor:%d ndx:%d\n", GRID_CURSOR, target_page->pageNdx );	
				}

				else {
					
					// No interesting key is pressed, so forget selection.	
					if ( target_page->SEL_LOCK == OFF ){
						target_page->trackSelection = 0;					
					}
				
					if (	( is_pressed_key( KEY_SELECT_MASTER ) ) ){

						target_page->OPS_mode = PASSIVE;						

// d_iag_printf( "going 3 IV->PASSIVE.. cursor:%d ndx:%d\n", GRID_CURSOR, target_page->pageNdx );	
					}
				}
				break;


			case PASSIVE:
			
				// Nothing to do yet
				if (	( my_pressed_key == 0 )
					&&	( !(is_pressed_key( KEY_SELECT_MASTER 	)) )
					){

					// Enter interactive mode
					target_page->OPS_mode = INTERACTIVE;
// d_iag_printf( "going 4 PV->INTERACTIVE.. cursor:%d ndx:%d \n", GRID_CURSOR, target_page->pageNdx );	
				}

				break;

		} // switch target_page->OPS_mode	



