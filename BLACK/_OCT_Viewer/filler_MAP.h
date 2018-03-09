

	// TRACK Selectors
	switch( MODE_OBJECT_SELECTION ){

		// Select change track selection
		case BIRDSEYE: 
			// Blink the currently selected track
			i = my_bit2ndx( VIEWER_page->trackSelection ) + 1;
			MIR_write_dot( i, MIR_RED   );
			MIR_write_dot( i, MIR_GREEN );
			MIR_write_dot( i, MIR_BLINK );
			break;

		case EMBEDDED:
			// CIRCLE OF PITCHES
			// If a step is pressed, show its absolute pitch in the inner circle
			i = is_pressed_rowzero();

			// Do not show this when working with the direction map
			if (	( i != 0 )
				&&	( VIEWER_page->trackAttributeSelection != ( 1 << DIRECTION ) )	
				){

				// Compute coordinates
				row = my_bit2ndx( VIEWER_page->trackSelection );
				col = i - 1;						

				// d_iag_printf( "step selected:%d\n", i );
				show_pitch_in_circle( 
					VIEWER_page->Track[row]->attribute[ATTR_PITCH] +
					VIEWER_page->Step[row][col]->attribute[ATTR_PITCH], ON );

				show_velocity_in_circle(
					normalize( 
						VIEWER_page->Track[row]->attribute[ATTR_VELOCITY] +
						VIEWER_page->Step[row][col]->attribute[ATTR_VELOCITY],
						0, 127 ) );
			}
			// break;

		default:
			// TRACK Selectors
			show (ELE_TRACK_SELECTORS, TRACK_ATTRIBUTES_MAPS);			
			show (ELE_TRACK_SELECTORS, TRACK_ATTRIBUTE_SELECTION);
			break;
	}


	// MATRIX
	show( ELE_MATRIX, LAUFLICHT_BOTTOM ) ;
	show( ELE_MATRIX, ATTR_MAP_VALUES  ) ;


	// TRACK MUTATORS
	show( ELE_TRACK_MUTATORS, TRACK_ATTRIBUTES_MAP_FUNCTIONS );

	
	// EDIT MASTER
	switch( VIEWER_page->editorMode ){

		case CONTROL:		
			// Edit acts as MCC source - send MCC data
			show( ELE_EDIT_MASTER, GREEN );
			show( ELE_EDIT_MASTER, RED );
			break;

		case EDIT:
			// This is the regular mode - edit attributes of selection
			show( ELE_EDIT_MASTER, GREEN );
			break;

		case PREVIEW:
			// Same as regular mode, but preview the steps being edited
			show( ELE_EDIT_MASTER, RED );
			break;	

		case MIX2EDIT:		
			// Edit acts as MCC source - send MCC data
			show( ELE_EDIT_INDICATOR, GREEN );
			show( ELE_EDIT_INDICATOR, RED );
			break;			
	}
	
	
	// RETURN
	show (ELE_RETURN, GREEN);


	// GLOBALS
	show( ELE_CHAIN_INDICATOR, ON );
	show (ELE_GLOBALS, TRANSPORT);

	show( ELE_GLOBALS, FOLLOW );


	// TEMPOMUL of track
	// If one track (only) is selected in page
	if ( my_bit_cardinality( VIEWER_page->trackSelection ) == 1 ){

		// Display its tempo multiplier value
		display_Track_TEMPOMUL( 
			VIEWER_page->Track[ my_bit2ndx( VIEWER_page->trackSelection ) ] );
	}


	// RECORDING LED			
	switch ( VIEWER_page->track_REC_pattern == VIEWER_page->trackSelection ){

		case TRUE:
			MIR_write_dot( LED_RECORD, MIR_RED 	 );

			// Blink when no ON notes are pending..
			if ( NOTE_queue_counter == 0 ){
				MIR_write_dot( LED_RECORD, MIR_BLINK );						
			}
			break;

		case FALSE:
			if ( VIEWER_page->track_REC_pattern != 0 ){

				MIR_write_dot( LED_RECORD, MIR_GREEN );

				// Blink when no ON notes are pending..
				if ( NOTE_queue_counter == 0 ){
					MIR_write_dot( LED_RECORD, MIR_BLINK );						
				}
			}
			else{
				MIR_write_dot( LED_RECORD, MIR_GREEN );
			}
			break;
	}




