

void rot_exec_GRIDTRACK( 	Pagestruct* target_page, 
							unsigned char rotNdx, 
							unsigned char direction ){

	unsigned char i = 0;

	// BIG KNOB
	if ( rotNdx == 0 ){
	
		// When not running on external clock
		if ( CLOCK_SOURCE != EXT ){

			modify_parameter( &TEMPO, MIN_TEMPO, MAX_TEMPO, direction, ON, FIXED);
			TIMER_REFILL_update();
		}				
	}


	// EDITOR
	if ((rotNdx >= 1) && (rotNdx <= 10)) {

		// Iterate through the active banks
		for ( i=0; i < GRID_NROF_BANKS; i++ ){

			// Make sure there is a page playing in the bank pressed
			if ( GRID_p_selection[i] != NULL ){

				// Apply rotary to the selected tracks
				rot_exe_EDIT( rotNdx, direction, GRID_p_selection[i] );

			} // target page is not NULL
		} // Bank iterator		
	}

}



