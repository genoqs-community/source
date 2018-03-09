

	// Choose the strum attribute to show and edit
	if (	( keyNdx >= 1 ) 
		&& 	( keyNdx <= 4 )
		){
	
		OCT_strum_attribute = keyNdx;
	}

	
	// CLEAR CONTENTS
	if ( keyNdx == KEY_CLEAR ){

		// Clear the currently shown values
		switch( OCT_strum_attribute ){

			case ATTR_VELOCITY:	
				// Make  VEL entries neutral				
				for ( i=0; i<MATRIX_NROF_ROWS; i++ ){
					OCT_step_strum_VEL	[OCT_strum_view_level][i] = 0;
				}
				break;

			case ATTR_PITCH:
				// Make all PIT entries neutral				
				for ( i=0; i<MATRIX_NROF_ROWS; i++ ){
					OCT_step_strum_PIT	[OCT_strum_view_level][i] = 0;
				}
				break;

			case ATTR_LENGTH:
				for ( i=0; i<MATRIX_NROF_ROWS; i++ ){
					OCT_step_strum_LEN	[OCT_strum_view_level][i] = 0;
				}
				break;

			case ATTR_START:
				for ( i=0; i<MATRIX_NROF_ROWS; i++ ){
					OCT_step_strum_STA	[OCT_strum_view_level][i] = 
						OCT_step_strum_STA_preset	[OCT_strum_view_level][i];
				}
				break;
		}
	}



	// RANDOMIZE CONTENTS
	if ( keyNdx == KEY_RANDOMIZE ){

		// Clear the currently shown values
		switch( OCT_strum_attribute ){

			case ATTR_VELOCITY:	
				// Make  VEL entries neutral				
				for ( i=0; i<MATRIX_NROF_ROWS; i++ ){
					OCT_step_strum_VEL	[OCT_strum_view_level][i] = (rand() % 80) - 40;
				}
				break;

			case ATTR_PITCH:
				// Make all PIT entries neutral				
				for ( i=0; i<MATRIX_NROF_ROWS; i++ ){
					OCT_step_strum_PIT	[OCT_strum_view_level][i] = (rand() % 80) - 40;
				}
				break;

			case ATTR_LENGTH:
				for ( i=0; i<MATRIX_NROF_ROWS; i++ ){
					OCT_step_strum_LEN	[OCT_strum_view_level][i] = (rand() % 80) - 40;
				}
				break;

			case ATTR_START:
				for ( i=0; i<MATRIX_NROF_ROWS; i++ ){
					OCT_step_strum_STA	[OCT_strum_view_level][i] = (rand() % 127);
				}
				break;
		}
	}


	// Allow STEP context related operations
	if ( target_page->stepSelection == 1 ){
		
		// Enter the chord aux notes via inner circle
		key_OCT_CIRCLE_chord_STEP( keyNdx );

		// Switch back to STEP mode
		if (	( keyNdx == KEY_ZOOM_STEP )
			){
	
			target_page->keyMode = KMOD_THIS_STEP;
			zoom_level = zoomSTEP;		
		}
	}

	
	
	//
	// TRANSPORT functions - when nothing is selected in page
	//
	switch( keyNdx ){

		case KEY_STOP:
			sequencer_command_STOP();
			break;


		case KEY_PAUSE:
			sequencer_command_PAUSE();
			break;


		case KEY_PLAY1:
		case KEY_PLAY2:
		case KEY_PLAY4:

			// Make sure the sequencer is running
			sequencer_command_PLAY();

			break;
	}	


