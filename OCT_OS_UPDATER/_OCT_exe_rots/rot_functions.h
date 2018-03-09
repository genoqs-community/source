//
// Functions pertaining to general encoder operation
//
// genoQs Machines
// 01.10.2005



// QUICKTURN TIMER HANDLING
void start_QUICKTURN_TIMER(){
	
	// Refill multiplier
	ROTARY_QUICKTURN_MULTIPLIER = RQM_MAX;		

	// Setup alarm to decrement it
	cyg_alarm_initialize(	quickturnAlarm_hdl, 
							cyg_current_time() + (RQM_INTERVAL),
							(RQM_INTERVAL) );
}

// New version of modify_parameter with QUICKTURN
void modify_parameter(	signed char* 	in_parameter, 
						unsigned int 	min_val, 
						unsigned int 	max_val, 
						unsigned char 	direction,
						unsigned char 	QTT_status,
						unsigned char 	WRAP_status	) {

	unsigned char i = 0;
	unsigned char temp = 0;
		
//	d_iag_printf( "rot &:%x, val:%d, min:%d max:%d dir:%d\n", 
//						(int) in_parameter, 
//						(unsigned int) *in_parameter,
//						min_val, 
//						max_val, 
//						direction 
//						);
	
	// Repeat according to quickturn indicator
	for (i=0; i < ROTARY_QUICKTURN_MULTIPLIER; i++) {

		// Refresh the temp value
		temp = (unsigned char) *in_parameter;

		// Execute rotary action
		switch (direction) {

			case INC:

				if (temp < max_val) {
//				if (*in_parameter < max_val) {
					*in_parameter = *in_parameter + 1;
				}			
				else if ( WRAP_status == WRAPPED ){
					*in_parameter = min_val;
				}
				break;	

			case DEC:

				if (temp > min_val) {
//				if (*in_parameter > min_val) {
					*in_parameter = *in_parameter - 1;
				}			
				else if ( WRAP_status == WRAPPED ){
					*in_parameter = max_val;
				}
				break;
		}
	}

	// ACTIVATE THE MULTIPLIER - but not in the negative modify mode
	if (QTT_status == ON) {
		start_QUICKTURN_TIMER();
	}
}


// New version of modify_parameter with QUICKTURN
void modify_signed_parameter(	signed char* 	in_parameter, 
								signed int 		min_val, 
								signed int 		max_val, 
								unsigned char 	direction,
								unsigned char 	QTT_status,
								unsigned char 	WRAP_status	) {

	unsigned char i = 0;
	signed char temp = 0;
		
//	d_iag_printf( "Srot &:%x, val:%d, min:%d max:%d dir:%d\n", 
//						(int) in_parameter, 
//						*in_parameter,
//						min_val, 
//						max_val, 
//						direction 
//						);
	
	// Repeat according to quickturn indicator
	for (i=0; i < ROTARY_QUICKTURN_MULTIPLIER; i++) {

		// Refresh the temp value
		temp =  *in_parameter;

		// Execute rotary action
		switch (direction) {

			case INC:

				if (temp < max_val) {
//				if (*in_parameter < max_val) {
					*in_parameter = *in_parameter + 1;
				}			
				else if ( WRAP_status == WRAPPED ){
					*in_parameter = min_val;
				}
				break;	

			case DEC:

				if (temp > min_val) {
//				if (*in_parameter > min_val) {
					*in_parameter = *in_parameter - 1;
				}			
				else if ( WRAP_status == WRAPPED ){
					*in_parameter = max_val;
				}
				break;
		}
	}

	// ACTIVATE THE MULTIPLIER - but not in the negative modify mode
	if (QTT_status == ON) {
		start_QUICKTURN_TIMER();
	}
}





 

//
// SHIFT MAP of target_track for in_attribute in the given direction
//
void shiftAttributeMap( 	Pagestruct* target_page,
							unsigned int row, 
							unsigned int in_attribute, 
							unsigned char direction ){

	// Shifts once and wraps around track boundaries [1,16]
	unsigned int col=0;
	unsigned int buffer=0;
	unsigned int buffer_chord=0;
	
	// Normalize the in_attribute value, to correspond to ATTR_ format
	in_attribute ++;
	
	switch( direction ){
		case DEC:
			// Store first value in the buffer
			buffer = target_page->Step[row][0]->attribute[in_attribute];
			buffer_chord = target_page->Step[row][0]->chord_data;

			// Shift all contents one to the left
			for (col=0; col<MATRIX_NROF_COLUMNS-1; col++) {

				// Make sure we don't shift the event bit in status
				if ( in_attribute == (ATTR_STATUS) ){
	
					target_page->Step[row][col]->attribute[ATTR_STATUS] =
						 	( target_page->Step[row][col]->attribute[ATTR_STATUS] & (1 << 4) )
						|	( target_page->Step[row][col+1]->attribute[ATTR_STATUS] & 0xEF );
				}
				else{

					// Shift the attribute value				
					target_page->Step[row][col]->attribute[in_attribute] = 
						target_page->Step[row][col+1]->attribute[in_attribute];
				}
				
				// Shift the chord data separately
				if ( in_attribute == PITCH ){
					target_page->Step[row][col]->chord_data = 
						target_page->Step[row][col+1]->chord_data;			
				}

			}

			// Put buffer value into the last place
			target_page->Step[row][MATRIX_NROF_COLUMNS-1]->attribute[in_attribute] = buffer;
			
			if ( in_attribute == (ATTR_STATUS) ){
	
				target_page->Step[row][MATRIX_NROF_COLUMNS-1]->attribute[ATTR_STATUS] =
						( buffer & (1 << 4) )
					|	( target_page->Step[row][MATRIX_NROF_COLUMNS-1]->attribute[ATTR_STATUS] & 0xEF );
			}
			else{
				target_page->Step[row][MATRIX_NROF_COLUMNS-1]->attribute[in_attribute] = buffer;
			}

			if ( in_attribute == PITCH ){
				target_page->Step[row][MATRIX_NROF_COLUMNS-1]->chord_data = buffer_chord;
			}
			break;
		

		case INC:
			// Store last value in the buffer		
			buffer = target_page->Step[row][15]->attribute[in_attribute];
			buffer_chord = target_page->Step[row][15]->chord_data;

			// Shift all contents
			for (col=MATRIX_NROF_COLUMNS-1; col>0; col--) {

				// Make sure we don't shift the event bit in status
				if ( in_attribute == (ATTR_STATUS) ){
	
					target_page->Step[row][col]->attribute[ATTR_STATUS] =
						 	( target_page->Step[row][col]->attribute[ATTR_STATUS] & (1 << 4) )
						|	( target_page->Step[row][col-1]->attribute[ATTR_STATUS] & 0xEF );
				}
				else{

					target_page->Step[row][col]->attribute[in_attribute]
						= target_page->Step[row][col-1]->attribute[in_attribute];
				}
				
				// Shift chord data separately
				if ( in_attribute == PITCH ){
					target_page->Step[row][col]->chord_data = 
						target_page->Step[row][col-1]->chord_data;			
				}
			}		

			// Put buffer value into the first place
			// Make sure we don't shift the event bit in status
			if ( in_attribute == (ATTR_STATUS) ){
	
				target_page->Step[row][0]->attribute[ATTR_STATUS] =
						( buffer & 0xEF )
					|	( target_page->Step[row][0]->attribute[ATTR_STATUS] & (1 << 4) );
			}
			else{
				target_page->Step[row][0]->attribute[in_attribute] = buffer;
			}
	
			if ( in_attribute == PITCH ){
				target_page->Step[row][0]->chord_data = buffer_chord;
			}

			break;
	}
}



// Apply the given rotary to the given Step
void apply_rotary_to_step( 	unsigned char rotNdx, 
							unsigned char direction, 
							Stepstruct* target_step ){
	
	
	// Modify the attribute value; normalize the rotNdx
	switch( rotNdx-1 ) {

		case VELOCITY:
			modify_signed_parameter(
				&target_step->attribute[ATTR_VELOCITY], 
				STEP_MIN_VELOCITY, STEP_MAX_VELOCITY, direction, OFF, FIXED );
			break;

		case PITCH:
			modify_signed_parameter(
				&target_step->attribute[ATTR_PITCH], 
				STEP_MIN_PITCH, STEP_MAX_PITCH, direction, OFF, FIXED );
			break;

		case LENGTH:
			modify_parameter(
				&target_step->attribute[ATTR_LENGTH], 
				STEP_MIN_LENGTH, ((unsigned char)STEP_MAX_LENGTH), direction, OFF, FIXED );
			break;

		case START:
			modify_parameter(
				&target_step->attribute[ATTR_START], 
				STEP_MIN_START, STEP_MAX_START, direction, OFF, FIXED );						
			break;

		case AMOUNT:
			modify_signed_parameter(
				&target_step->attribute[ATTR_AMOUNT], 
				STEP_MIN_AMOUNT, STEP_MAX_AMOUNT, direction, OFF, FIXED );						
			break;

		case GROOVE:
			modify_signed_parameter(
				&target_step->attribute[ATTR_GROOVE], 
				STEP_MIN_GROOVE, STEP_MAX_GROOVE, direction, OFF, FIXED );						
			break;

		case MIDICC:
			modify_signed_parameter(
				&target_step->attribute[ATTR_MIDICC], 					
				STEP_MIN_MIDICC, STEP_MAX_MIDICC, direction, OFF, FIXED );						
			break;

	} // switch( rotNDx-1 )

}


