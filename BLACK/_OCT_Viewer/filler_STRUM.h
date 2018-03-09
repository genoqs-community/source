	
	//
	// S T R U M  MAP MODE
	//
	
	//
	// MIX INDICATOR - LED corresponding to the rotarys
	//
	show( ELE_MIX_INDICATOR, RED   );
	show( ELE_MIX_INDICATOR, GREEN );
	
	// SEL Column  - show the available Strum parameters
	MIR_write_dot( 1, 					MIR_GREEN );
	MIR_write_dot( 2, 					MIR_GREEN );
	MIR_write_dot( 3, 					MIR_GREEN );
	MIR_write_dot( 4, 					MIR_GREEN );
	MIR_write_dot( OCT_strum_attribute, MIR_RED 	);
	MIR_write_dot( OCT_strum_attribute, MIR_BLINK 	);
	

	// MIX TARGET field
	//
	// show( ELE_MIXTGT, OPTIONS );
	
	// MUTATORS 
	MIR_write_dot( LED_CLEAR, 			MIR_RED   );
	MIR_write_dot( LED_CLEAR, 			MIR_GREEN );
	MIR_write_dot( LED_RANDOMIZE, 		MIR_RED   );
	MIR_write_dot( LED_RANDOMIZE, 		MIR_GREEN );
	
	
	// MATRIX
	//
	show( ELE_MATRIX, STRUM_VALS );
							

	// RETURN
	show (ELE_RETURN, GREEN);
	
	
	// GLOBALS
	//
	show (ELE_GLOBALS, TRANSPORT);	
	show (ELE_GLOBALS, BIG_KNOB);
		

	// Show the Strum view level in the chord indicator. Deep but efficient
	for ( i=0; i < 7; i++ ){
		MIR_write_dot( 258 - i, MIR_GREEN );
	}

	MIR_write_dot( 258 - OCT_strum_view_level, MIR_RED   );
	// MIR_write_dot( 258 - OCT_strum_view_level, MIR_GREEN );
	MIR_write_dot( 258 - OCT_strum_view_level, MIR_BLINK );
		
	// Signal the big knob select LED
	MIR_write_dot( LED_SELECT, MIR_RED   );
	MIR_write_dot( LED_SELECT, MIR_GREEN );

	// Show the STEP MODE LED lit on, if we were coming from a step..
	// Update the step selection counter in page
	if ( VIEWER_page->stepSelection == 1 ){

		// Show the STEP LED - because you can go back to the step..
		MIR_write_dot( LED_ZOOM_STEP, MIR_GREEN );	

		row = VIEWER_page->stepSelectionSingleRow;
		col = VIEWER_page->stepSelectionSingleCol;

		// Show chord and strum values in the two circles
		// ..
		// Show absolute pitch in inner circle - orange - status is OFF
		show_pitch_in_circle( 
			VIEWER_page->Track[row]->attribute[ATTR_PITCH] +
			VIEWER_page->Step[row][col]->attribute[ATTR_PITCH], OFF );
		
		// Show the chord notes assigned to the step in the circle.
		// << 1 due to some historics..
		show_chord_in_circle( (VIEWER_page->Step[row][col]->chord_data & 0x7FF) << 1,
			( VIEWER_page->Track[row]->attribute[ATTR_PITCH] +
			  VIEWER_page->Step[row][col]->attribute[ATTR_PITCH] )%12 );

		// Show the ROLL bit in the octave circle - when roll bit is set
		if ( (VIEWER_page->Step[row][col]->attribute[ATTR_STATUS] & (1 << 7)) != 0 ){
			MIR_write_dot( LED_NOTE_Cup, MIR_RED );
		
			if ( (VIEWER_page->Step[row][col]->attribute[ATTR_STATUS] & (1 << 6)) == 0 ){
				MIR_write_dot( LED_NOTE_Cup, MIR_GREEN );
			}
		}
		// Show step strum value in the outer circle
		show_strum_in_circle( ( VIEWER_page->Step[row][col]->chord_data & 0xF800) >> 11 );
	}	
	else {
		show (ELE_GLOBALS, NUMERIC_QUADRANT);
	}
	
