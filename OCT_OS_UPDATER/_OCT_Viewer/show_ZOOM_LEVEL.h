

switch( content ) {

	case zoomGRID:

		// GRID button blinking ORANGE - or lighting, when playing
		MIR_write_dot (LED_ZOOM_GRID,		MIR_GREEN);
		MIR_write_dot (LED_ZOOM_GRID, 		MIR_RED);
		MIR_write_dot( LED_ZOOM_GRID, 		MIR_BLINK );
		
		// TRACK button lit GREEN - GRIDTRACK is available
		MIR_write_dot (LED_ZOOM_TRACK,		MIR_GREEN);

		// PAGE mode available for zoom in GRID_EDIT mode
		if ( GRID_play_mode == GRID_EDIT ){
			// PAGE available
			MIR_write_dot (LED_ZOOM_PAGE, MIR_GREEN);				
		}
		
		// GRID mode selected				
		// PAGE mode available for selection only in EDIT mode.
		// Perform mode does not show which page we are about to enter!
		if ( GRID_play_mode == GRID_EDIT ){		
		}
		break;

								
	case zoomGRIDTRACK:

		// GRID button blinking ORANGE - or lighting, when playing
		MIR_write_dot (LED_ZOOM_GRID,		MIR_GREEN);
		MIR_write_dot (LED_ZOOM_GRID, 		MIR_RED);
		
		// TRACK button lit GREEN - GRIDTRACK is available
		MIR_write_dot (LED_ZOOM_TRACK,		MIR_GREEN);
		MIR_write_dot( LED_ZOOM_TRACK, 		MIR_RED  );
		MIR_write_dot (LED_ZOOM_TRACK,		MIR_BLINK);

		// PAGE mode available for zoom in GRID_EDIT mode
		if ( GRID_play_mode == GRID_EDIT ){
			// PAGE available
			MIR_write_dot (LED_ZOOM_PAGE, MIR_GREEN);				
		}
		
		// GRID mode selected				
		// PAGE mode available for selection only in EDIT mode.
		// Perform mode does not show which page we are about to enter!
		if ( GRID_play_mode == GRID_EDIT ){		
		}
		break;
	

									
	case zoomPAGE:
	case zoomMIXMAP:
	
		if (		(target_page->keyMode == KMOD_PAGE)
			){

			// If the page is playing in GRID
			if ( is_selected_in_GRID( target_page ) ) {

				// Check whether it is soloed
				j = FALSE;
				for ( i=0; i < GRID_NROF_BANKS; i++ ){

					if ( (GRID_p_selection_buffer[i] == target_page) ) {

						j = TRUE;
					}
				}

				// Page is soloed so blink green
				if ( j == TRUE ){
					
					MIR_write_dot (LED_ZOOM_PAGE, 		MIR_GREEN);
					MIR_write_dot (LED_ZOOM_PAGE, 		MIR_BLINK);										
				}
				else {
					
					// Blink page indicator in orange - unless CAD is selected
					// or sequencer is running..
					MIR_write_dot (LED_ZOOM_PAGE, 		MIR_GREEN);
					MIR_write_dot (LED_ZOOM_PAGE, 		MIR_RED);
	
					// Check whether CAD or MOD or SEL is selected
					// If it is, do not blink page LED
					if (	( target_page->scaleStatus 	!= SCALE_CAD) 
						){
	
						MIR_write_dot( LED_ZOOM_PAGE, 		MIR_BLINK );
					}
				}
			}
			// Page is not part of the GRID selection
			else {

				// Light PAGE mode indicator red
				MIR_write_dot( LED_ZOOM_PAGE, MIR_RED );
				MIR_write_dot( LED_ZOOM_PAGE, MIR_BLINK );

				// If page is in the clock preselection light orange (no blink)
				j = FALSE;
				for ( i=0; i < GRID_NROF_BANKS; i++ ){
					
					if ( GRID_p_clock_presel[i] == target_page ){
				
						MIR_write_dot (LED_ZOOM_PAGE, 	MIR_GREEN);			
						break;
					}
				} // GRID BANK iterator
			}
		}

		else {

			// Show only in green because we are in Step mode
			MIR_write_dot( LED_ZOOM_PAGE, 			MIR_GREEN);
		}
										
		// Show the GRID MODE available
		MIR_write_dot (LED_ZOOM_GRID, 		MIR_GREEN);
		
		// TRACK and PLAY mode available as well if a single track is selected
		// ..and page is not in Cadence edit mode
		for (i=0; i<10; i++) {
			if (  // ( target_page->keyMode != KMOD_CADENCE_EDIT ) &&
				  ((target_page->trackSelection & (1<<i)) != 0) &&
			      ((target_page->trackSelection ^ (1<<i)) == 0)    ) {

				// TRACK mode
				MIR_write_dot (LED_ZOOM_TRACK, MIR_GREEN);
			}
		}

		// Indicate the track TAP function for the stelected track
		if ( target_page->trackSelection != 0 ){

			MIR_write_dot( LED_ZOOM_STEP, MIR_RED );
		}


	// SPAGHETTI WARNING: this needs to be handled by zoomSTEP which is not in place
		// Indicate the STEP mode is active or can be activated
		if (	(target_page->stepSelection == 1)
//				&& (target_page->keyMode != KMOD_CADENCE_EDIT ) 
				){

			MIR_write_dot( LED_ZOOM_STEP, MIR_GREEN );
		}

		if (target_page->keyMode == KMOD_THIS_STEP){

			MIR_write_dot( LED_ZOOM_STEP, MIR_RED   );
			if ( RUNBIT == OFF ){

				MIR_write_dot( LED_ZOOM_STEP, 		MIR_BLINK );
			}
		}

		break;



	case zoomTRACK:

		// TRACK mode selected
		MIR_write_dot (LED_ZOOM_TRACK, 		MIR_GREEN);
		MIR_write_dot (LED_ZOOM_TRACK, 		MIR_RED);

//		if ( RUNBIT == OFF ){
			MIR_write_dot( LED_ZOOM_TRACK, 		MIR_BLINK );
//		}
		
		// PAGE mode
		// Indicate whether the page is currently playing 
		if ( is_selected_in_GRID( target_page ) ) {
			
			// Page is active in the Grid
			MIR_write_dot (LED_ZOOM_PAGE, 		MIR_GREEN);
		}				
		// Page is not part of the GRID selection
		else {

			// Blink PAGE mode indicator red
			MIR_write_dot (LED_ZOOM_PAGE, 		MIR_RED);
			// MIR_write_dot (LED_ZOOM_PAGE, 		MIR_BLINK);
		}

		// GRID mode available for selection
		MIR_write_dot (LED_ZOOM_GRID,	MIR_GREEN);

		// PLAY mode available for selected track
//		MIR_write_dot (LED_ZOOM_PLAY, 	MIR_GREEN);
		
		// MAP mode available as well if an attribute is selected
		if (target_page->trackAttributeSelection != 0) {
			MIR_write_dot (LED_ZOOM_MAP, MIR_GREEN);
		}
		break;



	case zoomMAP:
		// GRID mode available for selection
		MIR_write_dot (LED_ZOOM_GRID,		MIR_GREEN);

		// PAGE mode
		// Indicate whether the page is currently playing 
		if ( is_selected_in_GRID( target_page ) ) {
			
			// Page is active in the Grid
			MIR_write_dot (LED_ZOOM_PAGE, 		MIR_GREEN);
		}				
		// Page is not part of the GRID selection
		else {

			// Blink PAGE mode indicator red
			MIR_write_dot (LED_ZOOM_PAGE, 		MIR_RED);
			// MIR_write_dot (LED_ZOOM_PAGE, 		MIR_BLINK);
		}
		
		// TRACK mode available for selection
		MIR_write_dot (LED_ZOOM_TRACK, 		MIR_GREEN);

		// MAP mode selected
		MIR_write_dot (LED_ZOOM_MAP, 		MIR_GREEN);
		MIR_write_dot (LED_ZOOM_MAP, 		MIR_RED);
		
//		if ( RUNBIT == OFF ){
			MIR_write_dot( LED_ZOOM_MAP, 		MIR_BLINK );					
//		}
		break;



	case zoomSTEP:
	
		// PAGE mode simply in green because we are in Step mode
		MIR_write_dot( LED_ZOOM_PAGE, 		MIR_GREEN );
										
		// GRID mode available for selection
		MIR_write_dot( LED_ZOOM_GRID, 		MIR_GREEN );
		
		// TRACK mode available as well - for the track of the step 
		MIR_write_dot (LED_ZOOM_TRACK, MIR_GREEN);

		// STEP mode is active
		MIR_write_dot( LED_ZOOM_STEP, 		MIR_GREEN );
		MIR_write_dot( LED_ZOOM_STEP, 		MIR_RED   );
		
//		if ( RUNBIT == OFF ){
			MIR_write_dot( LED_ZOOM_STEP, 	MIR_BLINK );
//		}
		break;




	case zoomPLAY:
		// PLAY mode selected
		MIR_write_dot (LED_ZOOM_PLAY,		MIR_GREEN);
		MIR_write_dot (LED_ZOOM_PLAY, 		MIR_RED);

//		if ( RUNBIT == OFF ){
			MIR_write_dot( LED_ZOOM_PLAY, 		MIR_BLINK );					
//		}
		
		// GRID mode available for selection
		MIR_write_dot (LED_ZOOM_GRID,		MIR_GREEN);

		// PAGE mode
		// Indicate whether the page is currently playing 
		if ( is_selected_in_GRID( target_page ) ) {
			
			// Page is active in the Grid
			MIR_write_dot (LED_ZOOM_PAGE, 		MIR_GREEN);
		}				
		// Page is not part of the GRID selection
		else {

			// Blink PAGE mode indicator red
			MIR_write_dot (LED_ZOOM_PAGE, 		MIR_RED);
			// MIR_write_dot (LED_ZOOM_PAGE, 		MIR_BLINK);
		}
		
		// TRACK mode available for selection
		MIR_write_dot (LED_ZOOM_TRACK, 		MIR_GREEN);
		break;
}




