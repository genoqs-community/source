

void rot_exec_STEP( 	Pagestruct* target_page, 
						unsigned char rotNdx, 
						unsigned char direction ){

		unsigned char row = 0;
		unsigned char col = 0;
		unsigned char temp = 0;
	
		row = target_page->stepSelectionSingleRow;
		col = target_page->stepSelectionSingleCol;

		switch( rotNdx ){
		
			// BIG KNOB
			case 0:
				switch( MODE_OBJECT_SELECTION ){
				
					case CHORDEYE:
					
						// Enter the step strum level
						temp = ( VIEWER_page->Step[row][col]->chord_data & 0xF800 ) >> 11;
						modify_parameter( &temp, 0, 18, direction, OFF, FIXED);
	
						VIEWER_page->Step[row][col]->chord_data = ( temp << 11 )
							| ( VIEWER_page->Step[row][col]->chord_data & 0x7FF );
						break;
				
					default:
					
						// If an event is set show the range defined for set event.
						if ( Step_get_status( VIEWER_page->Step[row][col], STEPSTAT_EVENT ) == ON 
			   				){
							
							// Depending on which event is set, show range
							switch( (VIEWER_page->Step[row][col]->event_data & 0x0F) + 1 ){
			
								case ATTR_VELOCITY:
									modify_parameter( 	&VIEWER_page->Track[row]->range_VEL, 
														TRACK_MIN_RANGE_VEL, 
														TRACK_MAX_RANGE_VEL, 
														direction, ON, FIXED );
									break;
			
								case ATTR_PITCH:			
									modify_parameter( 	&VIEWER_page->Track[row]->range_PIT, 
														TRACK_MIN_RANGE_PIT, 
														TRACK_MAX_RANGE_PIT, 
														direction, ON, FIXED );
									break;
			
								case ATTR_MIDICC:
									modify_parameter( 	&VIEWER_page->Track[row]->range_MCC, 
														STEP_MIN_RANGE_MCC, 
														STEP_MAX_RANGE_MCC, 
														direction, ON, FIXED );
									break;
							} // switch on selected event type
						} // event is set on this step
						break;			
				} // mode selection
				break;



			// EDITOR
			case 1: case 2: case 3: case 4: case 5:
			case 6: case 7: case 8: case 9: case 10:
			
				// Modify the attribute value; normalize the rotNdx
				switch( rotNdx-1 ) {
					
					case VELOCITY:
						modify_signed_parameter(
							&target_page->Step[row][col]->attribute[ATTR_VELOCITY], 
							STEP_MIN_VELOCITY, 	STEP_MAX_VELOCITY, 	direction, ON, FIXED );
						break;
	
					case PITCH:
						modify_signed_parameter(
							&target_page->Step[row][col]->attribute[ATTR_PITCH], 
							STEP_MIN_PITCH, 	STEP_MAX_PITCH, 	direction, ON, FIXED );
						break;
	
					case LENGTH:
						modify_parameter(
							&target_page->Step[row][col]->attribute[ATTR_LENGTH], 
							STEP_MIN_LENGTH, 	((unsigned char)STEP_MAX_LENGTH), 	direction, ON, FIXED );
						break;
	
					case START:
						modify_parameter(
							&target_page->Step[row][col]->attribute[ATTR_START], 
							STEP_MIN_START, 	STEP_MAX_START, 	direction, ON, FIXED );						
						break;
	
					case AMOUNT:
						modify_signed_parameter(
							&target_page->Step[row][col]->attribute[ATTR_AMOUNT], 
							STEP_MIN_AMOUNT, 	STEP_MAX_AMOUNT, 	direction, ON, FIXED );						
						break;
	
					case GROOVE:
						modify_signed_parameter(
							&target_page->Step[row][col]->attribute[ATTR_GROOVE], 
							STEP_MIN_GROOVE, 	STEP_MAX_GROOVE, 	direction, ON, FIXED );						
						break;
	
					case MIDICC:
						modify_signed_parameter(
							&target_page->Step[row][col]->attribute[ATTR_MIDICC], 
							STEP_MIN_MIDICC, 	((unsigned char)STEP_MAX_MIDICC), 	direction, ON, FIXED );						
						break;
				}

				break;
		} // switch( rotNdx )

}
