//
// RND function application on STEP object
//

	// Check the stepAttributeSelection
	switch( VIEWER_page->stepAttributeSelection ){
	
		case OFF: 
			// Randomize all Step parameters
			Step_randomize( VIEWER_page->Step[row][col] );
			break;
	
		// Randomize individual attributes
		case (1 << VELOCITY):
			VIEWER_page->Step[row][col]->attribute[ATTR_VELOCITY]
				= randomStepAttrValue( 	VIEWER_page->Step[row][col],
										ATTR_VELOCITY, 
										VIEWER_page->Track[row]->attribute[ATTR_AMOUNT] );
			break;
			
		case (1 << PITCH):
			VIEWER_page->Step[row][col]->attribute[ATTR_PITCH] 
				= randomStepAttrValue( 	VIEWER_page->Step[row][col],
										ATTR_PITCH, 
										VIEWER_page->Track[row]->attribute[ATTR_AMOUNT]  );
			break;
		
		case (1 << LENGTH):
			VIEWER_page->Step[row][col]->attribute[ATTR_LENGTH] 
				= randomStepAttrValue( 	VIEWER_page->Step[row][col], 
										ATTR_LENGTH, 
										VIEWER_page->Track[row]->attribute[ATTR_AMOUNT]  );
			break;
		
		case (1 << START):
			VIEWER_page->Step[row][col]->attribute[ATTR_START] 
				= randomStepAttrValue( 	VIEWER_page->Step[row][col],
										ATTR_START, 
										VIEWER_page->Track[row]->attribute[ATTR_AMOUNT]  );
			break;
	
		case (1 << AMOUNT):
			VIEWER_page->Step[row][col]->attribute[ATTR_AMOUNT] 
				= randomStepAttrValue( 	VIEWER_page->Step[row][col],
										ATTR_AMOUNT, 
										VIEWER_page->Track[row]->attribute[ATTR_AMOUNT]  );
			break;

		case (1 << MIDICC):
			VIEWER_page->Step[row][col]->attribute[ATTR_MIDICC] 
				= randomStepAttrValue( 	VIEWER_page->Step[row][col],
										ATTR_MIDICC, 
										VIEWER_page->Track[row]->attribute[ATTR_AMOUNT]  );
			break;

	} // stepAttributeSelection switch




