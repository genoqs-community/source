
//
// S T E P  MAP MODE
//

		// TRACK SELECTORS
		//
		// Show the available and the selected step attribute/s
		// --> rather :: Show the available and the selected step event type
		if ( MODE_OBJECT_SELECTION != BIRDSEYE ){

			// Switched to the new model inside..
			// Show available event attributes
			show( ELE_TRACK_SELECTORS, STEP_ATTRIBUTES_ALL );

			// Show currently selected event attribute
			if ( Step_get_status( VIEWER_page->Step[ VIEWER_page->stepSelectionSingleRow ]
								  				   [ VIEWER_page->stepSelectionSingleCol ],
   								  STEPSTAT_EVENT ) == ON 
   				){
						
				show( ELE_TRACK_SELECTORS, STEP_ATTRIBUTE_SELECTION );
   			}
		}

		
		// MATRIX
		// 
		// Blinks the selected step and shows its attribute values..
		// ..which are editable via editor
		if ( MODE_OBJECT_SELECTION == BIRDSEYE ){

			show( ELE_MATRIX, STEP_TOGGLE );
			show( ELE_MATRIX, STEP_SELECTED );
		}
		else{
			show( ELE_MATRIX, STEP_ATTRIBUTE_VALUES );
		}
		
		// Show the lauflicht of the row the Step is belonging to
		MIR_write_lauflicht_track( VIEWER_page->stepSelectionSingleRow );


		// TRACK MUTATORS
		//
		if (VIEWER_page->stepSelection != 0) {

			// Step rotarys shown
			if ( MODE_OBJECT_SELECTION != BIRDSEYE ){
				show( ELE_TRACK_MUTATORS, STEP_FUNCTIONS );
			}
		}

		// RETURN
		//
		if ((VIEWER_page->trackSelection != 0) ||
			(VIEWER_page->stepSelection  != 0)   ) {
			show (ELE_RETURN, GREEN);
		}


		// GLOBALS
		//
		show (ELE_GLOBALS, TRANSPORT);	

		////..replaced by the lined below them..
		//		show (ELE_GLOBALS, NUMERIC_QUADRANT);
		//		show (ELE_GLOBALS, BIG_KNOB);

		// Compute coordinates
		row = VIEWER_page->stepSelectionSingleRow;	// Keeps row coordinate of a selected single step
		col = VIEWER_page->stepSelectionSingleCol;	// Keeps col coordinate of a selected single step


		// CIRCLES: INNER and OUTER CIRCLE
		if ( MODE_OBJECT_SELECTION == CHORDEYE ){

			// Show chord and strum values in the two circles
			// ..
			// Show absolute pitch in inner circle - orange - status is OFF
			show_pitch_in_circle( 
				VIEWER_page->Track[row]->attribute[ATTR_PITCH] +
				VIEWER_page->Track[row]->event_PIT +
				VIEWER_page->Step[row][col]->attribute[ATTR_PITCH], OFF );
			
			// Show the chord notes assigned to the step in the circle.
			// << 1 due to some historics..
			show_chord_in_circle( (VIEWER_page->Step[row][col]->chord_data & 0x7FF) << 1,
				( VIEWER_page->Track[row]->attribute[ATTR_PITCH] +
				  VIEWER_page->Track[row]->event_PIT +
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

			// Signal the big knob select LED
			MIR_write_dot( LED_SELECT, MIR_RED   );
			MIR_write_dot( LED_SELECT, MIR_GREEN );
		}
		else{
				
			// If an event is set show the range defined for set event.
			if ( Step_get_status( VIEWER_page->Step[row][col], STEPSTAT_EVENT ) == ON 
   				){
				
				// Depending on which event is set, show range
				switch( (VIEWER_page->Step[row][col]->event_data & 0x0F) + 1 ){

					case ATTR_VELOCITY:

						MIR_write_numeric_C( VIEWER_page->Track[row]->range_VEL );
						MIR_write_dot( LED_SELECT, MIR_RED   );
						MIR_write_dot( LED_SELECT, MIR_GREEN );
						break;

					case ATTR_PITCH:

						MIR_write_numeric_C( VIEWER_page->Track[row]->range_PIT );
						MIR_write_dot( LED_SELECT, MIR_RED   );
						MIR_write_dot( LED_SELECT, MIR_GREEN );
						break;

					case ATTR_MIDICC:

						MIR_write_numeric_C( VIEWER_page->Track[row]->range_MCC );
						MIR_write_dot( LED_SELECT, MIR_RED   );
						MIR_write_dot( LED_SELECT, MIR_GREEN );
						break;
				} // switch on selected event type
			}

			else{
				// Show the absolute PIT and VEL of the step in the circle - status is ON
				show_pitch_in_circle( 
					VIEWER_page->Track[row]->attribute[ATTR_PITCH] +
					VIEWER_page->Track[row]->event_PIT +
					VIEWER_page->Step[row][col]->attribute[ATTR_PITCH], ON );
		
				show_velocity_in_circle(
					normalize( 
						VIEWER_page->Track[row]->attribute[ATTR_VELOCITY] +
	 					VIEWER_page->Track[row]->event_VEL +
						VIEWER_page->Step[row][col]->attribute[ATTR_VELOCITY],
						0, 127 ) );
			}
		}
		
		// Show the STEP CHORD cardinality in the chord indicator. Deep but efficient
		MIR_write_dot( 258 - 
			my_bit_cardinality( VIEWER_page->Step[row][col]->chord_data & 0x7FF )+ 0, MIR_RED   );
		MIR_write_dot( 258 - 
			my_bit_cardinality( VIEWER_page->Step[row][col]->chord_data & 0x7FF )+ 0, MIR_GREEN );
		

		// show( ELE_GLOBALS, CLOCK );
		
	




