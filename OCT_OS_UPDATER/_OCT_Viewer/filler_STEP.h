
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
			if ( Step_get_status( target_page->Step[ target_page->stepSelectionSingleRow ]
								  				   [ target_page->stepSelectionSingleCol ],
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

			show( ELE_MATRIX, STEP_TOGGLE 	);
			show( ELE_MATRIX, STEP_SELECTED );
		}
		else{
			show( ELE_MATRIX, STEP_ATTRIBUTE_VALUES );
		}
		
		// Show the lauflicht of the row the Step is belonging to
		MIR_write_lauflicht_track( target_page->stepSelectionSingleRow );


		// TRACK MUTATORS
		//
		if (target_page->stepSelection != 0) {

			// Step rotarys shown
			if ( MODE_OBJECT_SELECTION != BIRDSEYE ){
				show( ELE_TRACK_MUTATORS, STEP_FUNCTIONS );
			}
		}


		// EDIT INDICATOR
		if (target_page->stepSelection != 0) {

			// Active when a step is selected - show " ' "
			show( ELE_EDIT_INDICATOR, GREEN );

			if (EDIT_TIMER != OFF){
				show( ELE_EDIT_INDICATOR, BLINK );
			}					
		}


		// RETURN
		//
		if ((target_page->trackSelection != 0) ||
			(target_page->stepSelection  != 0)   ) {
			show (ELE_RETURN, GREEN);
		}


		// GLOBALS
		//
		show (ELE_GLOBALS, TRANSPORT);	


		// Show recording mode: here traditional
		if ( target_page->track_REC_pattern != 0 ){
			
			MIR_write_dot( LED_RECORD, MIR_RED );

			// Blink when no ON notes are pending..
			if ( NOTE_queue_counter == 0 ){
				MIR_write_dot( LED_RECORD, MIR_BLINK );						
			}
		}
		// Step recording
		else if ( target_page->REC_bit == ON ){
		
			MIR_write_dot( LED_RECORD, MIR_RED 	 );
			MIR_write_dot( LED_RECORD, MIR_GREEN );
			
			// Blink when no ON notes are pending..
			if ( NOTE_queue_counter == 0 ){
				MIR_write_dot( LED_RECORD, MIR_BLINK );						
			}
		}


		////..replaced by the lined below them..
		//		show (ELE_GLOBALS, NUMERIC_QUADRANT);
		//		show (ELE_GLOBALS, BIG_KNOB);

		// Compute coordinates
		row = target_page->stepSelectionSingleRow;	// Keeps row coordinate of a selected single step
		col = target_page->stepSelectionSingleCol;	// Keeps col coordinate of a selected single step


		// CIRCLES: INNER and OUTER CIRCLE
		if ( MODE_OBJECT_SELECTION == CHORDEYE ){

			// Show chord and strum values in the two circles
			// ..
			// Show absolute pitch in inner circle - orange - status is OFF
			show_pitch_in_circle( 
				target_page->Track[row]->attribute[ATTR_PITCH] +
				target_page->Step[row][col]->attribute[ATTR_PITCH], OFF );
			
			// Show the chord notes assigned to the step in the circle.
			// << 1 due to some historics..
			show_chord_in_circle( (target_page->Step[row][col]->chord_data & 0x7FF) << 1,
				( target_page->Track[row]->attribute[ATTR_PITCH] +
				  target_page->Step[row][col]->attribute[ATTR_PITCH] )%12 );

			// Show the ROLL bit in the octave circle - when roll bit is set
			if ( (target_page->Step[row][col]->attribute[ATTR_STATUS] & (1 << 7)) != 0 ){
				MIR_write_dot( LED_NOTE_Cup, MIR_RED );
			
				if ( (target_page->Step[row][col]->attribute[ATTR_STATUS] & (1 << 6)) == 0 ){
					MIR_write_dot( LED_NOTE_Cup, MIR_GREEN );
				}
			}
			// Show step strum value in the outer circle
			show_strum_in_circle( ( target_page->Step[row][col]->chord_data & 0xF800) >> 11 );

			// Signal the big knob select LED
			MIR_write_dot( LED_SCALE_MYSEL, MIR_RED   );
			MIR_write_dot( LED_SCALE_MYSEL, MIR_GREEN );
		}
		else{
				
			// If an event is set show the range defined for set event.
			if ( Step_get_status( target_page->Step[row][col], STEPSTAT_EVENT ) == ON 
   				){
				
				// Depending on which event is set, show range
				switch( (target_page->Step[row][col]->event_data & 0x0F) + 1 ){

					case ATTR_VELOCITY:

						MIR_write_numeric_C( target_page->Track[row]->event_max[ATTR_VELOCITY] );
						MIR_write_dot( LED_SCALE_MYSEL, MIR_RED   );
						MIR_write_dot( LED_SCALE_MYSEL, MIR_GREEN );
						break;

					case ATTR_PITCH:

						MIR_write_numeric_C( target_page->Track[row]->event_max[ATTR_PITCH] );
						MIR_write_dot( LED_SCALE_MYSEL, MIR_RED   );
						MIR_write_dot( LED_SCALE_MYSEL, MIR_GREEN );
						break;

					case ATTR_LENGTH:

						MIR_write_numeric_C( target_page->Track[row]->event_max[ATTR_LENGTH] );
						MIR_write_dot( LED_SCALE_MYSEL, MIR_RED   );
						MIR_write_dot( LED_SCALE_MYSEL, MIR_GREEN );
						break;

					case ATTR_START:

						MIR_write_numeric_C( target_page->Track[row]->event_max[ATTR_START] );
						MIR_write_dot( LED_SCALE_MYSEL, MIR_RED   );
						MIR_write_dot( LED_SCALE_MYSEL, MIR_GREEN );
						break;

					case ATTR_DIRECTION:

						MIR_write_numeric_C( target_page->Track[row]->event_max[ATTR_DIRECTION] );
						MIR_write_dot( LED_SCALE_MYSEL, MIR_RED   );
						MIR_write_dot( LED_SCALE_MYSEL, MIR_GREEN );
						break;

					case ATTR_AMOUNT:

						MIR_write_numeric_C( target_page->Track[row]->event_max[ATTR_AMOUNT] );
						MIR_write_dot( LED_SCALE_MYSEL, MIR_RED   );
						MIR_write_dot( LED_SCALE_MYSEL, MIR_GREEN );
						break;

					case ATTR_GROOVE:

						MIR_write_numeric_C( target_page->Track[row]->event_max[ATTR_GROOVE] );
						MIR_write_dot( LED_SCALE_MYSEL, MIR_RED   );
						MIR_write_dot( LED_SCALE_MYSEL, MIR_GREEN );
						break;

					case ATTR_MIDICC:

						MIR_write_numeric_C( target_page->Track[row]->event_max[ATTR_MIDICC] );
						MIR_write_dot( LED_SCALE_MYSEL, MIR_RED   );
						MIR_write_dot( LED_SCALE_MYSEL, MIR_GREEN );
						break;

					case ATTR_MIDICH:

						MIR_write_numeric_C( target_page->Track[row]->event_max[ATTR_MIDICH] );
						MIR_write_dot( LED_SCALE_MYSEL, MIR_RED   );
						MIR_write_dot( LED_SCALE_MYSEL, MIR_GREEN );
						break;

				} // switch on selected event type
			}

			else{
				// Show the absolute PIT and VEL of the step in the circle - status is ON
				show_pitch_in_circle( 
					target_page->Track[row]->attribute[ATTR_PITCH] +
					target_page->Step[row][col]->attribute[ATTR_PITCH], ON );
		
				show_velocity_in_circle(
					normalize( 
						target_page->Track[row]->attribute[ATTR_VELOCITY] +
						target_page->Step[row][col]->attribute[ATTR_VELOCITY],
						0, 127 ) );
			}
		}
		
		// Show the STEP CHORD cardinality in the chord indicator. Deep but efficient
		MIR_write_dot( 258 - 
			my_bit_cardinality( target_page->Step[row][col]->chord_data & 0x7FF )+ 0, MIR_RED   );
		MIR_write_dot( 258 - 
			my_bit_cardinality( target_page->Step[row][col]->chord_data & 0x7FF )+ 0, MIR_GREEN );
		

		// show( ELE_GLOBALS, CLOCK );
		
	




