//
// RND function application on STEP object
//

	// Check the stepAttributeSelection
	switch( target_page->stepAttributeSelection ){
	
		case OFF: 
			// Randomize all Step parameters
			Step_randomize( target_page->Step[row][col] );
			break;
	
		// Randomize individual attributes
		case (1 << VELOCITY):
			target_page->Step[row][col]->attribute[ATTR_VELOCITY]
				= randomStepAttrValue( 	target_page->Step[row][col],
										ATTR_VELOCITY, 
										target_page->Track[row]->attribute[ATTR_AMOUNT]
										+ target_page->Track[row]->event_offset[ATTR_AMOUNT] 
										);
			break;
			
		case (1 << PITCH):
			target_page->Step[row][col]->attribute[ATTR_PITCH] 
				= randomStepAttrValue( 	target_page->Step[row][col],
										ATTR_PITCH, 
										target_page->Track[row]->attribute[ATTR_AMOUNT]
										+ target_page->Track[row]->event_offset[ATTR_AMOUNT] 
										);
			break;
		
		case (1 << LENGTH):
			target_page->Step[row][col]->attribute[ATTR_LENGTH] 
				= randomStepAttrValue( 	target_page->Step[row][col], 
										ATTR_LENGTH, 
										target_page->Track[row]->attribute[ATTR_AMOUNT]
										+ target_page->Track[row]->event_offset[ATTR_AMOUNT] 
										);
			break;
		
		case (1 << START):
			target_page->Step[row][col]->attribute[ATTR_START] 
				= randomStepAttrValue( 	target_page->Step[row][col],
										ATTR_START, 
										target_page->Track[row]->attribute[ATTR_AMOUNT]
										+ target_page->Track[row]->event_offset[ATTR_AMOUNT] 
										);
			break;
	
		case (1 << AMOUNT):
			target_page->Step[row][col]->attribute[ATTR_AMOUNT] 
				= randomStepAttrValue( 	target_page->Step[row][col],
										ATTR_AMOUNT, 
										target_page->Track[row]->attribute[ATTR_AMOUNT]
										+ target_page->Track[row]->event_offset[ATTR_AMOUNT] 
										);
			break;

		case (1 << MIDICC):
			target_page->Step[row][col]->attribute[ATTR_MIDICC] 
				= randomStepAttrValue( 	target_page->Step[row][col],
										ATTR_MIDICC, 
										target_page->Track[row]->attribute[ATTR_AMOUNT]
										+ target_page->Track[row]->event_offset[ATTR_AMOUNT] 
										);
			break;

	} // stepAttributeSelection switch




