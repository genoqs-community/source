	
	// 
	// GRID  M I X  MODE
	// 
	if ( GRID_play_mode == GRID_MIX ){
	
	
		// MIX INDICATOR - LED corresponding to the rotarys - all active..
		show( ELE_MIX_INDICATOR, RED   );
		show( ELE_MIX_INDICATOR, GREEN );
	
		// TRACK SELECTORS
		show( ELE_TRACK_SELECTORS, GRID_BANK_PLAYMODES );
	
		
		// MATRIX
		show( ELE_MATRIX, GRIDTRACK );
	
	
		// OCTAVE CIRCLE	 
		// Show the GRID scale status
		if (	( GRID_assistant_page->scaleStatus == SCALE_SEL )
			|| 	( GRID_assistant_page->scaleStatus == SCALE_MOD ) 
			){
			
			// Show the selected notes in scale. Both in MOD and SEL
			show_OCTAVE_CIRCLE_scale_selection( GRID_assistant_page );
			show_SCALE_SELECTOR_scale_selection( GRID_assistant_page );
			show_GLOBALS_scale_master( GRID_assistant_page );
		}
		
		// When forced to scale, show the scale
		else if (	( GRID_assistant_page->force_to_scale == ON )
			){

			// Show the selected notes in scale. Both in MOD and SEL
			show_OCTAVE_CIRCLE_scale_selection( GRID_assistant_page );
			show_GLOBALS_scale_master( GRID_assistant_page );
		}		
		else{

			// Show the packman at playtime - unless there is a scale selected
			show (ELE_OCTAVE_CIRCLE, GLOBAL_LOCATOR_PICTURE);	
		}
		
		// TRACK MUTATORS
		// Default: mutepattern of grid banks
		show( ELE_TRACK_MUTATORS, GRID_SET_SWITCHMODE );

		// Caution!! There is also some extra code in the ELE_MATRIX / GRID section.
	
		// NUMERIC QUADRANT
		MIR_write_numeric_C( TEMPO );
		
	} // // GRID_playmode == GRID_MIX	
	

	

	//
	// GRID  C C M A P  MODE
	//
	else if ( GRID_play_mode == GRID_CCMAP ){
	
		// Use the elements of the page editing view..

		//
		// MIX INDICATOR - LED corresponding to the rotarys
		//
		show( ELE_MIX_INDICATOR, RED   );
		show( ELE_MIX_INDICATOR, GREEN );

		// MIX MASTER
		switch (target_page->mixMode) {
			case INC:
				show (ELE_MIX_MASTER, GREEN);
				break;
				
			case DEC:
				show (ELE_MIX_MASTER, RED);
				break;
		}

		// TRACK_SELECTORS
		switch( GRID_assistant_page->mixTarget ){

			case MIXTGT_USR1:
			case MIXTGT_USR2:
			case MIXTGT_USR3:
			case MIXTGT_USR4:
			case MIXTGT_USR5:
				// Show the available attributes of the CC_MIXMAP
				show( ELE_TRACK_SELECTORS, CC_MIXMAP_ATTRIBUTES );
				break;
		}

		// MIX TARGET field
		show( ELE_MIXTGT, OPTIONS );

		// MATRIX
		show( ELE_MATRIX, GRID_CC_MAP_VALS );				

		// RETURN
		show (ELE_RETURN, GREEN);		
		
		// NUMERIC QUADRANT
		MIR_write_numeric_C( TEMPO );
		
	}
	
			
	
	// GLOBALS
	//
	show (ELE_GLOBALS, TRANSPORT);
	show (ELE_GLOBALS, BIG_KNOB);
	show( ELE_GLOBALS, CLOCK );
	

	// Mix target indicator
	show( ELE_MIXTGT, GRID_assistant_page->mixTarget );

	
	// LED_ZOOM indicates the GRID play mode
	switch( GRID_play_mode ){
		
		case GRID_EDIT:
			// MIR_write_dot( LED_SELECT_MASTER, MIR_RED 	);
			MIR_write_dot( LED_ZOOM, MIR_RED );
			MIR_write_dot( LED_ZOOM, MIR_GREEN 	);
			break;
	
		case GRID_MIX:
			// Nothing to do
			break;
	}
		
	
	// PLAY mode
	switch( PLAY_MODE_STATUS ){
	
		case ON:
			MIR_write_dot( LED_ZOOM_PLAY, MIR_GREEN );
			MIR_write_dot( LED_ZOOM_PLAY, MIR_BLINK );

			// MIR_write_dot( LED_PROGRAM, MIR_RED 	);
			// MIR_write_dot( LED_PROGRAM, MIR_BLINK 	);					
			break;
			
		case OFF:
			MIR_write_dot( LED_ZOOM_PLAY, MIR_RED );
			break;
	}
	
	

	// EDIT INDICATOR
	// Show activation status: all are active
	for ( i=0; i < GRID_NROF_BANKS; i++ ){

		if ( GRID_p_selection[i] != NULL ){
		
			if ( GRID_p_selection[i]->trackSelection != 0 ){

				// EDIT INDICATOR
				show( ELE_EDIT_INDICATOR, RED   );
				show( ELE_EDIT_INDICATOR, GREEN );
				show( ELE_EDIT_INDICATOR, BLINK );

				// SHOW TOGGLE AND SOLO LEDs
				MIR_write_dot( LED_TGGL, MIR_RED 	);
				MIR_write_dot( LED_TGGL, MIR_GREEN 	);
				
				MIR_write_dot( LED_SOLO, MIR_RED 	);
				MIR_write_dot( LED_SOLO, MIR_GREEN 	);
			}
		}	
	}
	
	
	if (EDIT_TIMER != OFF){
		show( ELE_EDIT_INDICATOR, BLINK );
	}					



	// PITCH AND VEL CIRCLES
	// Show the track data (like in page) for the single selected track
	// Show activation status: all are active

	// Make sure only one track is selected so it can be shown
	j = 0; // Counts the track selections
	for ( i=0; i < GRID_NROF_BANKS; i++ ){
	
		if ( GRID_p_selection[i] != NULL ){
		
			if (	( GRID_p_selection[i]->trackSelection != 0 )
				&&	( my_bit_cardinality( GRID_p_selection[i]->trackSelection ) == 1 )
				){
			
				j ++;
			}
		}
	}	

	// Only one track is selected in the GRID
	if ( j == 1 ){
		
		for ( i=0; i < GRID_NROF_BANKS; i++ ){
	
			// A page is active in the bank
			if ( GRID_p_selection[i] != NULL ){
			
				if (	( GRID_p_selection[i]->trackSelection != 0 )
					&&	( my_bit_cardinality( GRID_p_selection[i]->trackSelection ) == 1 )
					){
	
					// OCTAVE CIRCLE
					show (ELE_OCTAVE_CIRCLE, TRACK_TRANSPOSITION);

					// Terminate the loop
					i = GRID_NROF_BANKS;
				}
			} 
		} // Bank iterator	
	}
		
	
	




