	
	// 
	// GRID  M I X  MODE
	// 
	if ( GRID_play_mode == GRID_MIX ){
	
	
		// MIX INDICATOR - LED corresponding to the rotarys - all active..
		show( ELE_MIX_INDICATOR, RED   );
		show( ELE_MIX_INDICATOR, GREEN );
	
		// TRACK SELECTORS
		show (ELE_TRACK_SELECTORS, GRID_BANK_PLAYMODES);
	
		
		// MATRIX
		show( ELE_MATRIX, GRID );
	
	
		// OCTAVE CIRCLE	 
		// Show the GRID scale status
		if (	( GRID_assistant_page->scaleStatus == SCALE_SEL )
			|| 	( GRID_assistant_page->scaleStatus == SCALE_MOD ) 
			){
			
			// Show the selected notes in scale. Both in MOD and SEL
			show_OCTAVE_CIRCLE_scale_selection( GRID_assistant_page );
			show_SCALE_SELECTOR_scale_selection( GRID_assistant_page );
//			show_GLOBALS_scale_selection( GRID_assistant_page );
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
		show (ELE_TRACK_MUTATORS, GRID_SET_SWITCHMODE);
		// Caution!! There is also some extra code in the ELE_MATRIX / GRID section.
	

		// NUMERIC QUADRANT
		MIR_write_numeric_C( TEMPO );


		//
		// MUTE MASTER
		//
		// Indicate ongoing mute activity
		// if ( GRID_bank_mutepattern != 0 ) {				
		//	show (ELE_MUTE_MASTER, GREEN);
		// }
		// Indicate existing stored mutepattern
		// else if ( GRID_bank_mutepattern_stored != 0 ) {
		//	show (ELE_MUTE_MASTER, RED);
		// }
		
	} // // GRID_playmode == GRID_MIX	
	
	
	
	
	
	
	
	
	
	
		
	
	//
	// GRID  E D I T  MODE
	//
	else if ( GRID_play_mode == GRID_EDIT ){
	
//		// Show the GRID FTS exempt
//		if ( Page_repository[GRID_CURSOR].grid_FTS_exempt == FALSE ){
//		
//			MIR_write_dot( LED_SCALE_SEL, MIR_RED );
//		}
//		else if ( Page_repository[GRID_CURSOR].grid_FTS_exempt == TRUE ){
//		
//			MIR_write_dot( LED_SCALE_SEL, MIR_GREEN );
//		}			
	
	
		// MATRIX
		//
		show (ELE_MATRIX, GRID);
	
		switch( GRID_rowzero_pagelength ){
		
			case FALSE:
				// Show the repeats of the page under cursor. Not in the above because
				// ..for some reason doesn't work.
				MIR_fill_numeric( 1, Page_repository[GRID_CURSOR].repeats_left, 9, MIR_GREEN );
				MIR_fill_numeric( 	 Page_repository[GRID_CURSOR].repeats_left, 
									 Page_repository[GRID_CURSOR].repeats+1, 9, MIR_RED );
				MIR_write_dot( 10, MIR_RED );
				break;
			
			case TRUE:
				// Show the pagelength in hex really - resolution of 16 steps..
				MIR_fill_numeric( 1, (Page_repository[GRID_CURSOR].attribute[ATTR_LENGTH]/16)+1, 9, MIR_RED );
				MIR_point_numeric( Page_repository[GRID_CURSOR].attribute[ATTR_LENGTH]%16, 9, MIR_GREEN );
				MIR_write_dot( 10, MIR_GREEN );	
				break;
		}
	 
		// TRACK MUTATORS
		//
		show (ELE_TRACK_MUTATORS, PAGE_FUNCTIONS);
		// Caution!! There is some extra code in the ELE_MATRIX / GRID section.
	
		// OCTAVE CIRCLE
		show (ELE_OCTAVE_CIRCLE, PAGE_TRANSPOSITION);
		
		// EDIT INDICATOR
		show( ELE_EDIT_INDICATOR, RED );
		
		// NUMERIC QUADRANT
		// MIR_write_numeric_C( Page_repository[GRID_CURSOR].attribute[ATTR_VELOCITY] );
		MIR_write_numeric_C( TEMPO );
	
	
	} // GRID_play_mode == GRID_EDIT
	
	
	
	
	
	
	
	
	

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
		switch (VIEWER_page->mixMode) {
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
	
	// Blink the red program button when ready to save machines state
	if ( 	( RUNBIT == OFF )
		&&	( MODE_OBJECT_SELECTION == BIRDSEYE )
		){

		MIR_write_dot( LED_PROGRAM, MIR_RED   );
		MIR_write_dot( LED_PROGRAM, MIR_BLINK );
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
	
	
	
