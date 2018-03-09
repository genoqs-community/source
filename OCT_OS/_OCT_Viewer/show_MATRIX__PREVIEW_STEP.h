

	// We are dealing with a pressed key for a selected step.
	// the Matrix shows the length of the step JohnK style, and 
	// the circles VEL and PIT data!


	// It all happens when we have no step selection active
	if ( target_page->stepSelection != 0 ){
		break;
	}

	// MATRIX
	// In the row of the pressed step: show its blinking head and the length
	
	// Get the key index of the pressed step, if there is one. Otherwise done.
	my_pressed_step = is_pressed_steprange();
	if ( my_pressed_step == 0 ) break;

	// Compute coordinates of the pressed step
	row = row_of( my_pressed_step );
	col = column_of( my_pressed_step );
	
	// Show the step length in the matrix
	show_preview_stepLEN( my_pressed_step, row, col, target_page->Step[row][col]->attr_LEN );


	// Show the step VEL and PIT in the circles
	if ( PHRASE_TIMER == ON ) {
		PhraseShowPhraseNum( target_page->Step[row ][col]->phrase_num );
	}
	else {
		show_velocity_in_circle( normalize( target_page->Track[row]->attr_VEL
				+ target_page->Step[row][col]->attr_VEL, 0, 127 ) );
	}
	
	show_pitch_in_circle( 		target_page->Track[row]->attr_PIT 
							+	target_page->Step[row][col]->attr_PIT, ON 	);


	// Show the real STEP CHORD cardinality in the chord indicator in red.
	#ifdef FEATURE_ENABLE_CHORD_OCTAVE
	MIR_write_dot( 258 - get_chord_cardinality( target_page->Step[row][col], CHORD_OCTAVE_ALL ), MIR_RED );
	#else
	MIR_write_dot( 258 - my_bit_cardinality( target_page->Step[row][col]->chord_data & 0x7FF ), MIR_RED   );
	#endif

	MIR_write_dot( 258 - (target_page->Step[row][col]->chord_up >> 29 ), MIR_GREEN );
