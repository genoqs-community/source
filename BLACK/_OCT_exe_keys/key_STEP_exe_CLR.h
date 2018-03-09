//
// CLR function application on STEP object
//

	// Check the stepAttributeSelection
	switch( VIEWER_page->stepAttributeSelection ){

		case OFF: 
			// Init all Step parameters
			Step_init( VIEWER_page->Step[row][col] );

			// But re-mark the step as selected and active!
			Step_set_status( VIEWER_page->Step[row][col], STEPSTAT_SELECT, ON );

			// Duncan/Tom asked to leave it alone..
			// Step_set_status( VIEWER_page->Step[row][col], STEPSTAT_TOGGLE, ON );
			break;

		// Init individual attributes
		case (1 << VELOCITY):
			VIEWER_page->Step[row][col]->attribute[ATTR_VELOCITY]
				= STEP_DEF_VELOCITY;
			break;
			
		case (1 << PITCH):
			VIEWER_page->Step[row][col]->attribute[ATTR_PITCH] 
				= STEP_DEF_PITCH;
			break;
		
		case (1 << LENGTH):
			VIEWER_page->Step[row][col]->attribute[ATTR_LENGTH] 
				= STEP_DEF_LENGTH;
			break;
		
		case (1 << START):
			VIEWER_page->Step[row][col]->attribute[ATTR_START] 
				= STEP_DEF_START;
			break;

		case (1 << AMOUNT):
			VIEWER_page->Step[row][col]->attribute[ATTR_AMOUNT] 
				= STEP_DEF_AMOUNT;
			break;

		case (1 << MIDICC):
			VIEWER_page->Step[row][col]->attribute[ATTR_MIDICC] 
				= STEP_DEF_MIDICC;
			break;

	} // stepAttributeSelection switch







