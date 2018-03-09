


void rot_exe_EDIT( 	unsigned char rotNdx, 
					unsigned char direction, 
					Pagestruct* target_page ){

	unsigned char i=0;
			
	// d_iag_printf( "rotNdx:%d dir:%d pgNdx:%d\n", rotNdx, direction, target_page->pageNdx );

	// EDIT_TIMER handling
	//
	// Reset timer on every turn, so it doesnt run out
	if (target_page->trackSelection != 0) {

		EDIT_TIMER = ON;				
		// Setup alarm for the EDIT TIMER
		cyg_alarm_initialize(	alarm_hdl, 
								cyg_current_time() + TIMEOUT_VALUE,
								0 );

		// Select the edit attribute
		target_page->editAttribute = rotNdx;

		target_page->keyMode = KMOD_PAGE;

	} // Case where some track is selected

	
	else {

		switch( target_page->editorMode == CONTROL ) {
		
			// No track selected and EDITOR is in Controller Mode -> send MIDI CC data
			case CONTROL:
	
				// Normalize i value to get to the track index
				i = rotNdx - 1;
	
				// Check before sending data that a valid CC is assigned (tmoravan)
				if ( target_page->Track[i]->attribute[ATTR_MIDICC] != MIDICC_NONE ){
				
					// Modify the CC amount of track
					modify_signed_parameter 
						(&target_page->Track[i]->attribute[ATTR_CCAMNT], 
							TRACK_MIN_CCAMNT, TRACK_MAX_CCAMNT, direction, ON, FIXED);
		
					// Send MIDICC on the corresponding channel
					MIDI_send( 	MIDI_CC,
								target_page->Track[i]->attribute[ATTR_MIDICH],
								target_page->Track[i]->attribute[ATTR_MIDICC],					
								target_page->Track[i]->attribute[ATTR_CCAMNT] 	);
				} // Track is assigned a valid CC value
				break;
	
		}
	}



	// 
	// ATTRIBUTE MODIFY - EDITOR of trackSelection (hardwired)
	//
	switch (rotNdx) {
		case 1:		// VELOCITY
			// For every track in selection modify the corresponding attribute
			for (i=0; i< 10; i++) {
				if ((1<<i) & (target_page->trackSelection)) {

					target_page->Track[i]->event_VEL = 0;

					modify_parameter 
						(&target_page->Track[i]->attribute[ATTR_VELOCITY], 
						 	TRACK_MIN_VELOCITY, TRACK_MAX_VELOCITY, direction, OFF, FIXED);
				}
			}
			// Start the quickturn timer after the first full track iteration
			// start_QUICKTURN_TIMER();
			break;


		case 2:		// PITCH
			// For every track in selection modify the corresponding attribute
			for (i=0; i< 10; i++) {
				if ((1<<i) & (target_page->trackSelection)) {

					target_page->Track[i]->event_PIT = 0;

					modify_parameter 
						(&target_page->Track[i]->attribute[ATTR_PITCH], 
							TRACK_MIN_PITCH, TRACK_MAX_PITCH, direction, OFF, FIXED);	
				}
			}
			// Start the quickturn timer after the first full track iteration
			// start_QUICKTURN_TIMER();
			break;


		case 3:		// LENGTH
			// For every track in selection modify the corresponding attribute
			for (i=0; i< 10; i++) {
				if ((1<<i) & (target_page->trackSelection)) {

					// Newschool - LEN factor
					modify_parameter(  
						&target_page->Track[i]->LEN_factor, 
						// 1, 16, direction, ON);
						TRACK_MIN_LENFACTOR, TRACK_MAX_LENFACTOR, direction, OFF, FIXED);
				}
			}
			// Start the quickturn timer after the first full track iteration
			// Disable here to prevent overshooting of length and therefore 
			// ..undefined results.
			// start_QUICKTURN_TIMER();
			break;
			

		case 4:		// START
			// For every track in selection modify the corresponding attribute
			for (i=0; i< 10; i++) {
				if ((1<<i) & (target_page->trackSelection)) {

					// Newschool - STA factor
					modify_parameter( 
						&target_page->Track[i]->STA_factor, 
						TRACK_MIN_STAFACTOR, TRACK_MAX_STAFACTOR, direction, OFF, FIXED);
				}
			}
			// Start the quickturn timer after the first full track iteration
			// Disable here to prevent overshooting of length and therefore 
			// ..undefined results.
			// start_QUICKTURN_TIMER();
			break;
			

		case 5:		// POSITION
			// For every track in selection modify the corresponding attribute
			for (i=0; i< 10; i++) {

				if ((1<<i) & (target_page->trackSelection)) {

					// Position computation done manually for now for every side
					Page_wrap_track( target_page, i, direction, 1 );

					modify_parameter 
						(&target_page->Track[i]->attribute[ATTR_POSITION], 
							TRACK_MIN_POSITION, TRACK_MAX_POSITION, direction, OFF, FIXED);
				}
			}
			break;
			

		case 6:		// DIRECTION
			// For every track in selection modify the corresponding attribute
			for (i=0; i< 10; i++) {
				if ((1<<i) & (target_page->trackSelection)) {
					modify_parameter 
						(&target_page->Track[i]->attribute[ATTR_DIRECTION], 
						 	TRACK_MIN_DIRECTION, TRACK_MAX_DIRECTION, direction, OFF, FIXED);
				}
			}
			break;
		

		case 7:		// AMOUNT
			// For every track in selection modify the corresponding attribute
			for (i=0; i< 10; i++) {

				if ((1<<i) & (target_page->trackSelection)) {

					modify_parameter 
						(&target_page->Track[i]->attribute[ATTR_AMOUNT], 
						 	TRACK_MIN_AMOUNT, TRACK_MAX_AMOUNT, direction, OFF, FIXED);
				}
			}
			break;
			

		case 8:		// GROOVE
			// For every track in selection modify the corresponding attribute
			for (i=0; i< 10; i++) {
				if ((1<<i) & (target_page->trackSelection)) {
					modify_parameter 
						(&target_page->Track[i]->attribute[ATTR_GROOVE], 
						 	TRACK_MIN_GROOVE, TRACK_MAX_GROOVE, direction, OFF, FIXED);
				}
			}
			// Update the max groove value for the page
			Page_update_maxGroove (target_page);
			break;
			

		case 9:		// MIDICC
			// For every track in selection modify the corresponding attribute
			for (i=0; i< 10; i++) {
				if ((1<<i) & (target_page->trackSelection)) {

					target_page->Track[i]->event_MCC = 0;

					modify_signed_parameter 
						(&target_page->Track[i]->attribute[ATTR_MIDICC], 
						 	TRACK_MIN_MIDICC, TRACK_MAX_MIDICC, direction, OFF, FIXED);
				}
			}
			break;
			

		case 10:	// MIDICH	
			// For every track in selection modify the corresponding attribute
			for (i=0; i< 10; i++) {
				if ((1<<i) & (target_page->trackSelection)) {
					modify_parameter 
						(&target_page->Track[i]->attribute[ATTR_MIDICH], 
						 	TRACK_MIN_MIDICH, TRACK_MAX_MIDICH, direction, OFF, FIXED);
				}
			}
			break;
	} // switch (rotNdx)

}

