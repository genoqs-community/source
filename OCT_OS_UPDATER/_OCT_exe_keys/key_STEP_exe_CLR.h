//
// CLR function application on STEP object
//

	// Check the stepAttributeSelection
	switch( target_page->stepAttributeSelection ){

		case OFF: 
			// Init all Step parameters
			Step_init( target_page->Step[row][col] );

			// But re-mark the step as selected and active!
			Step_set_status( target_page->Step[row][col], STEPSTAT_SELECT, ON );

			// Duncan/Tom asked to leave it alone..
			// Step_set_status( target_page->Step[row][col], STEPSTAT_TOGGLE, ON );
			break;

		// Init individual attributes
		case (1 << VELOCITY):
			target_page->Step[row][col]->attribute[ATTR_VELOCITY]
				= STEP_DEF_VELOCITY;
			break;
			
		case (1 << PITCH):
			target_page->Step[row][col]->attribute[ATTR_PITCH] 
				= STEP_DEF_PITCH;
			break;
		
		case (1 << LENGTH):
			target_page->Step[row][col]->attribute[ATTR_LENGTH] 
				= STEP_DEF_LENGTH;
			break;
		
		case (1 << START):
			target_page->Step[row][col]->attribute[ATTR_START] 
				= STEP_DEF_START;
			break;

		case (1 << AMOUNT):
			target_page->Step[row][col]->attribute[ATTR_AMOUNT] 
				= STEP_DEF_AMOUNT;
			break;

		case (1 << MIDICC):
			target_page->Step[row][col]->attribute[ATTR_MIDICC] 
				= STEP_DEF_MIDICC;
			break;

	} // stepAttributeSelection switch







