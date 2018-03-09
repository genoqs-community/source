


	// MATRIX - Check for pressed matrix keys 
	my_pressed_key = is_pressed_pagerange();
	
	// Button pressed in range of virtual selectors
	if (	(	( my_pressed_key > 43  ) 
			&&	( my_pressed_key < 153 ) )
		){
			
		GRIDTRACK_OPS_mode = PASSIVE;
		
		// d_iag_printf( "Intr_KEY_GRIDTRACK PASSIVE\n" );
		break;

	}
	else{

		// Remember selections
		if ( is_pressed_key( KEY_SELECT_MASTER ) ){	

			GRIDTRACK_OPS_mode = PASSIVE;

			// Remove all track selections in the pages but store them
			for ( row=0; row < GRID_NROF_BANKS;  row++ ){
		
				if ( GRID_p_selection[row] != NULL ){
			
					GRID_p_selection[row]->trackSelectionStored = 
						GRID_p_selection[row]->trackSelection;
				}
			} // row iterator
		}
		else{
			
			// Remove all track selections in the pages
			for ( row=0; row < GRID_NROF_BANKS;  row++ ){
		
				if ( GRID_p_selection[row] != NULL ){
			
					GRID_p_selection[row]->trackSelection = 0;
				}
			}

			GRIDTRACK_OPS_mode = INTERACTIVE;
		}
		// d_iag_printf( "Intr_KEY_GRIDTRACK switch to INTERACTIVE\n" );
	}




