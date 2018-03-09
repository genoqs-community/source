

	// TRACK SELECTORS
	//
	if ((keyNdx > 0) && (keyNdx <= 10)) {

		switch( MODE_OBJECT_SELECTION ){

			// Select change track selection
			case BIRDSEYE: 
				// Change the track selection in page
				target_page->trackSelection = ( 1 << (keyNdx - 1) );
				break;

			default:
				// Edit Track Attribute Selection
				if ( ( (1 << (keyNdx-1) ) & PANEL_get_trackAttributeMap() ) != 0 ) {
		
					// Toggle the attribute selection
					if (target_page->trackAttributeSelection == 1<< (keyNdx-1)) {
						
						target_page->trackAttributeSelection = 0;
						target_page->keyMode = KMOD_THIS_TRACK;
						zoom_level = zoomTRACK;
					}
					else {
						
						// Select the new attribute whose map to show.
						target_page->trackAttributeSelection = 1 << (keyNdx-1);
					}	
				}
				break;
		} // switch( MODE_OBJECT_SELECTION );
	}
 


	//
	// MATRIX
	//

	// ROW ZERO in matrix
	// interpret the keypress - unlike we are in the DIR map..
	if ( 	(keyNdx >=  20) && 	(keyNdx <= 185)  
		&& 	( ((keyNdx - 20) % 11) == 0) 	
		&&	( target_page->trackAttributeSelection != (1 << DIRECTION) )
		){

		// Compute coordinates of the pressed step
		col = ((keyNdx-20) / 11);
		row = my_bit2ndx( target_page->trackSelection );

		// Check if the PLAY Mode button is pressed
		if (	( target_page->editorMode == PREVIEW )
			){

			// Play the Step information as it would be played by the player
			PLAYER_preview_step( target_page, row, col );
		}

		// Interpret the keypress
		interpret_matrix_stepkey( row, col, target_page );
		
	} // If this is a bottom Matrix row key										



	// General MATRIX KEYS: paint the SHAPE
	//
	else if ( 	(keyNdx >=  11) && 	(keyNdx <= 185) 
			&& 	( ((keyNdx - 10) % 11) != 0)   ) 		{ 

		// PAINT ATTRIBUTE MAP SHAPE

		// Compute coordinates of steps to modify
		i   = row_of( keyNdx );
		row = my_bit2ndx( target_page->trackSelection );
		col = column_of( keyNdx );

		// d_iag_printf( "key_MAP ndx:%d i:%d row:%d col:%d\n", keyNdx, i, row, col);

		// Set the tens attribute value of the corresponding steps
		switch( my_bit2ndx( target_page->trackAttributeSelection ) +1){

			case ATTR_VELOCITY:
				val_ptr = &(target_page->Step[row][col]->attribute[ATTR_VELOCITY]);
				// Pressing the highest row should produce the max value
				switch( i ){
					case 0:		*val_ptr =  35;					break;
					case 1:		*val_ptr =  25; 				break;
					case 2:		*val_ptr =  15;					break;
					case 3:		*val_ptr =   5;					break;
					case 4:		*val_ptr =   0;					break;
					case 5: 	*val_ptr =  -5;					break;
					case 6:		*val_ptr = -15;					break;
					case 7:		*val_ptr = -25;					break;
					case 8:		*val_ptr = -35;					break;
				}
				break;				

			case ATTR_PITCH:
				val_ptr = &(target_page->Step[row][col]->attribute[ATTR_PITCH]);
				// Pressing the highest row should produce the max value
				switch( i ){
					case 0:		*val_ptr =  24;					break;
					case 1:		*val_ptr =  12; 				break;
					case 2:		*val_ptr =   7;					break;
					case 3:		*val_ptr =   4;					break;
					case 4:		*val_ptr =   0;					break;
					case 5: 	*val_ptr =  -4;					break;
					case 6:		*val_ptr =  -7;					break;
					case 7:		*val_ptr = -12;					break;
					case 8:		*val_ptr = -24;					break;
				}			
				break;


			case ATTR_LENGTH:

				val_ptr = &(target_page->Step[row][col]->attribute[ATTR_LENGTH]);
				// Pressing the highest row should produce the max value
				switch( i ){
					case 0:		*val_ptr =     128;				break;
					case 1:		*val_ptr =      64; 			break;
					case 2:		*val_ptr =      32;				break;
					case 3:		*val_ptr =      24;				break;
					case 4:		*val_ptr =      16;				break;
					case 5: 	*val_ptr =      12;				break;
					case 6:		*val_ptr =    	 8;				break;
					case 7:		*val_ptr = 	  	 4;				break;
					case 8:		*val_ptr =   	 2;				break;
				}			
				break;



			case ATTR_START:

				val_ptr = &(target_page->Step[row][col]->attribute[ATTR_START]);
				// Pressing the highest row should produce the max value
				switch( i ){
					case 0:		*val_ptr =  	10;				break;
					case 1:		*val_ptr =   	9; 				break;
					case 2:		*val_ptr =   	8;				break;
					case 3:		*val_ptr =   	7;				break;
					case 4:		*val_ptr =   	6;				break;
					case 5: 	*val_ptr =   	5;				break;
					case 6:		*val_ptr =    	4;				break;
					case 7:		*val_ptr = 	  	3;				break;
					case 8:		*val_ptr =    	2;				break;
				}			
				break;



			case ATTR_DIRECTION:
				// Click in rowzero
				if ( i == 9 ){
					
					// Select the trigger set of the direction
					current_trigger_set = col+1;
				}
				// Keep directions 1-5 as read-only
				else if ( target_page->Track[row]->attribute[ATTR_DIRECTION] > 5 ){

					if ( 	direction_repository[ target_page->Track[row]->attribute[ATTR_DIRECTION]]
						  	.trigger_set[current_trigger_set]
						  	.trigger[9-i]  == (col + 1)
						){
						direction_repository[ target_page->Track[row]->attribute[ATTR_DIRECTION]]
						.trigger_set[current_trigger_set]
						.trigger[9-i] = 0;
					}
					else{
						direction_repository[ target_page->Track[row]->attribute[ATTR_DIRECTION]]
						.trigger_set[current_trigger_set]
						.trigger[9-i] = col + 1;
					}
				}
				break;



			case ATTR_AMOUNT:
				val_ptr = &(target_page->Step[row][col]->attribute[ATTR_AMOUNT]);
				// Pressing the highest row should produce the max value
				switch( i ){
					case 0:		*val_ptr =  35;					break;
					case 1:		*val_ptr =  25; 				break;
					case 2:		*val_ptr =  15;					break;
					case 3:		*val_ptr =   5;					break;
					case 4:		*val_ptr =   0;					break;
					case 5: 	*val_ptr =  -5;					break;
					case 6:		*val_ptr = -15;					break;
					case 7:		*val_ptr = -25;					break;
					case 8:		*val_ptr = -35;					break;
				}
				break;				



			case ATTR_GROOVE:
				val_ptr = &(target_page->Step[row][col]->attribute[ATTR_GROOVE]);
				// Pressing the highest row should produce the max value
				switch( i ){
					case 0:		*val_ptr =  STEP_MAX_GROOVE;	break;
					case 1:		*val_ptr =  7; 					break;
					case 2:		*val_ptr =  6;					break;
					case 3:		*val_ptr =  5;					break;
					case 4:		*val_ptr =  4;					break;
					case 5: 	*val_ptr = 	3;					break;
					case 6:		*val_ptr = 	2;					break;
					case 7:		*val_ptr = 	1;					break;
					case 8:		*val_ptr = STEP_MIN_GROOVE;		break;
				}
				break;



			case ATTR_MIDICC:
				val_ptr = &(target_page->Step[row][col]->attribute[ATTR_MIDICC]);
					
				if ( target_page->Track[row]->attribute[ATTR_MIDICC] == MIDICC_BENDER ){

					switch( i ){
	
						case 0:		*val_ptr =  35 + 64;			break;
						case 1:		*val_ptr =  25 + 64; 	 		break;
						case 2:		*val_ptr =  15 + 64;			break;
						case 3:		*val_ptr =   5 + 64;			break;
						case 4:		*val_ptr =   0 + 64;			break;
						case 5: 	*val_ptr =  -5 + 64;			break;
						case 6:		*val_ptr = -15 + 64;			break;
						case 7:		*val_ptr = -25 + 64;			break;
						case 8:		*val_ptr = -35 + 64;			break;
					}
				}
				else{
					
					switch( i ){
						
						case 0:		*val_ptr =     120;				break;
						case 1:		*val_ptr =     104; 			break;
						case 2:		*val_ptr =      88;				break;
						case 3:		*val_ptr =      72;				break;
						case 4:		*val_ptr =      64;				break;
						case 5: 	*val_ptr =      56;				break;
						case 6:		*val_ptr =    	40;				break;
						case 7:		*val_ptr = 	  	24;				break;
						case 8:		*val_ptr =   	MIDICC_NONE;	break;	
					}
				}
				break;

		}						
	}
	


	// TRACK MUTATORS
	//
	if ( (keyNdx >= 187) && (keyNdx <= 196) ) {

		// Compute coordinates first
		row = my_bit2ndx( target_page->trackSelection );
		j =   my_bit2ndx( target_page->trackAttributeSelection ) +1;
					
		switch( keyNdx ) {								

			case CLEAR_KEY:	
			
				// Treat the direction map separately from the others
				if ( (my_bit2ndx( target_page->trackAttributeSelection )+1) == ATTR_DIRECTION ){
			
					// CLEAR the current direction to its preset status
					direction_init( target_page->Track[row]->attribute[ATTR_DIRECTION] );
				}
				else{
							// Logic: if some steps selected, act only on those. 
							// Otherwise act on all.
							switch( target_page->stepSelection ){
			
								case 0:
									// No steps are selected, modify all steps in track
									for (col=0; col < MATRIX_NROF_COLUMNS; col++){

										// Reset map values to default
										target_page->Step[row][col]->attribute[j] = 
											stepDefaultAttrValue[j];
									}
									break;					
			
								default:
									// Some steps are selected, find and modify them
									for (col=0; col < MATRIX_NROF_COLUMNS; col++) {
										
										// If the step is selected..
										if ( Step_get_status
												( target_page->Step[row][col], STEPSTAT_SELECT ) == ON ){
									
											// ..Reset Step value to default
											target_page->Step[row][col]->attribute[j] = 
												stepDefaultAttrValue[j];
										}
									}
									break;
							} // switch( target_page->stepSelection )
				}

				break;



			case RANDOMIZE_KEY:

				// Act depending on the selected attribute j
				switch( j ){
				
					case ATTR_DIRECTION:
					
						// Keep directions 1-5 as read-only
						if ( target_page->Track[row]->attribute[ATTR_DIRECTION] > 5 ){
				
							// Randomize the currently selected trigger set
							for ( i=0; i < (MATRIX_NROF_ROWS-1); i++ ){
						
								direction_repository[ target_page->Track[row]->attribute[ATTR_DIRECTION]]
								.trigger_set[current_trigger_set]
								.trigger[9-i] = rand() % 17;
							}	
						}
						break;
					
 
					case ATTR_LENGTH:
						// Logic: if some steps selected, act only on those. 
						// Otherwise act on all.
						switch( target_page->stepSelection ){
		
							case 0:
								// No steps are selected, modify all steps in track
								for (col=0; col < MATRIX_NROF_COLUMNS; col++){
								
									target_page->Step[row][col]->attribute[ATTR_LENGTH] = 
										(signed char) 
										randomStepAttrValue( 	target_page->Step[row][col], 
																ATTR_LENGTH, 
																target_page->Track[row]->attribute[ATTR_AMOUNT]
																+ target_page->Track[row]->event_offset[ATTR_AMOUNT]
																);
								}
								break;					
		
							default:
								// Some steps are selected, find and modify them
								for (col=0; col < MATRIX_NROF_COLUMNS; col++) {
									
									// If the step is selected..
									if ( Step_get_status 
											( target_page->Step[row][col], STEPSTAT_SELECT ) == ON ){
								
										// ..Reset Step value to default
										target_page->Step[row][col]->attribute[ATTR_LENGTH] = 
											(signed char)
											randomStepAttrValue( 	target_page->Step[row][col],
																	ATTR_LENGTH, 
																	target_page->Track[row]->attribute[ATTR_AMOUNT]
																	+ target_page->Track[row]->event_offset[ATTR_AMOUNT]
																	);
									}
								}
								break;
						} // switch( target_page->stepSelection )
						break;


					default:
					
						// Logic: if some steps selected, act only on those. 
						// Otherwise act on all.
						switch( target_page->stepSelection ){
		
							case 0:
								// No steps are selected, modify all steps in track
								for (col=0; col < MATRIX_NROF_COLUMNS; col++){
								
									target_page->Step[row][col]->attribute[j] = 
										(signed char)
										randomStepAttrValue( 	target_page->Step[row][col], 
																j, 
																target_page->Track[row]->attribute[ATTR_AMOUNT]
																+ target_page->Track[row]->event_offset[ATTR_AMOUNT] 
																);
								}
								break;					
		
							default:
								// Some steps are selected, find and modify them
								for (col=0; col < MATRIX_NROF_COLUMNS; col++) {
									
									// If the step is selected..
									if ( Step_get_status 
											( target_page->Step[row][col], STEPSTAT_SELECT ) == ON ){
								
										// ..Reset Step value to default
										target_page->Step[row][col]->attribute[j] = 
										(signed char)
										randomStepAttrValue( 	target_page->Step[row][col],
																j, 
																target_page->Track[row]->attribute[ATTR_AMOUNT]
																+ target_page->Track[row]->event_offset[ATTR_AMOUNT] 
																);
									}
								}
								break;
						} // switch( target_page->stepSelection )
						break;
						
				} // Switch on the selected attribute



				


				break;



			case RMX_KEY:
				
				// Remix the track steps				
				Page_RMX_selected_tracks( target_page );
				break;



			case ZOOM_KEY:
				
				// Take a look at the number of selected steps
				switch( target_page->stepSelection ){

					case 0:
						// None or more steps selected: 
						// ..zoom out into the previos mode
						target_page->keyMode = KMOD_THIS_TRACK;
						// Switch zoom level
						zoom_level = zoomTRACK;				
						break;

					default:
						// More than one step is selected: unselect the selected steps
						// Unselect all potentially selected steps in the track	
						for (col=0; col<MATRIX_NROF_COLUMNS; col++) {
	
							// If step is selected			
							if ( Step_get_status( target_page->Step[row][col], STEPSTAT_SELECT ) == ON ){

								// Mark step unselected
								Step_set_status( target_page->Step[row][col], STEPSTAT_SELECT, OFF);

								// And decrement step selection counter
								target_page->stepSelection --;
							}
						}

						// And zoom out 
						// None or more steps selected: 
						// ..zoom out into the previos mode
						target_page->keyMode = KMOD_THIS_TRACK;
						// Switch zoom level
						zoom_level = zoomTRACK;
						break;
				}
				break; // case ZOOM_KEY

		} // switch keyNdx

	}


	// TRACK CHAIN SELECTORS - active only when sequencer not running.
	key_exe_chainselectors( keyNdx );	


	// ZOOM MODE
	// 
	// GRID zoom mode
	if (	(keyNdx == KEY_ZOOM_GRID)
		||	(keyNdx == KEY_ZOOM_PAGE)
		|| 	(keyNdx == KEY_ZOOM_TRACK) ){
			
		switch( keyNdx ){ 
		
			case KEY_ZOOM_GRID:
				zoom_level = zoomGRID;
				// Keymode is implicit for GRID
				break;
	
			case KEY_ZOOM_PAGE:
				target_page->keyMode = KMOD_PAGE;
				zoom_level = zoomPAGE;					
				break;
				
			case KEY_ZOOM_TRACK:
				target_page->keyMode = KMOD_THIS_TRACK;
				zoom_level = zoomTRACK;
				break;			
		}
	
		// Unselect all potentially selected steps in the track
		// ..to prevent conflicting state of track and step selected
		if ( target_page->stepSelection != 0 ){
			
			// Calculate the row of the selected track
			row = my_bit2ndx( target_page->trackSelection );

			// Iterate through all steps in that row (selected track)
			for (col=0; col<MATRIX_NROF_COLUMNS; col++) {
	
				// If step is selected			
				if ( Step_get_status( target_page->Step[row][col], STEPSTAT_SELECT ) == ON ){

					// Mark step unselected
					Step_set_status( target_page->Step[row][col], STEPSTAT_SELECT, OFF );

					// And decrement step selection counter
					target_page->stepSelection --;
				}
			}	
		}

	} // keyNdx is a KEY_ZOOM_xxx
	


	//
	// FOLLOW_TRACK
	//
	if (	( keyNdx == KEY_FOLLOW )
		&&	( PLAY_MODE_STATUS == OFF )
		){

		// Toggle the FOLLOW_TRACK switch
		if ( follow_flag == FOLLOW_TRACK ){
			
			follow_flag = OFF;
		}
		else{

			follow_flag = FOLLOW_TRACK;
		}
	}



	//
	// EDIT MASTER
	//
	if (keyNdx == KEY_EDIT_MASTER) {
		// D O U B L E - C L I C K
		if (DOUBLE_CLICK_TARGET == keyNdx) {
			// This is the DC target
			// Toggle the edit mode: 0:EDIT  1:CONTROL
			target_page->editorMode = CONTROL;
		}

		// Step_activity is ON: turn it OFF or SELECT it
		else if (DOUBLE_CLICK_TARGET == 0) {

			DOUBLE_CLICK_TARGET = keyNdx;
			DOUBLE_CLICK_TIMER = ON;								
			// Start the Double click Alarm
			cyg_alarm_initialize(	
				doubleClickAlarm_hdl, 
				cyg_current_time() + DOUBLE_CLICK_ALARM_TIME,
				DOUBLE_CLICK_ALARM_TIME );

			// Toggle the edit mode
			switch( target_page->editorMode ){

				case CONTROL:
					target_page->editorMode = EDIT;
					break;
					
				case EDIT:
					target_page->editorMode = PREVIEW;
					break;

				case PREVIEW:
					target_page->editorMode = EDIT;
					break;

				case MIX2EDIT:
					target_page->editorMode = EDIT;
					break;					
			}
		}				
	}


	// 
	// RECORD KEY
	//
	if ( keyNdx == KEY_RECORD ){

		// Make sure only one track is selected
		if ( my_bit_cardinality( target_page->trackSelection ) == 1 ){

			// Only one track can be recorded into at a time - so erase the initial REC pattern
			// Toggle the recording status
			if (	( target_page->track_REC_pattern == target_page->trackSelection )
				||	( target_page->track_REC_pattern == 0 )
				){
				// Toggle the REC pattern - better because it is faster to use.
				target_page->track_REC_pattern ^= (1 << (my_bit2ndx( target_page->trackSelection )));

				// Mark page as unclear
				target_page->page_clear = OFF;
			}
		}
	}


	// 
	// OCTAVE CIRCLE
	//
	if ( MODE_OBJECT_SELECTION == EMBEDDED ){

		// Enter the pitch via inner circle
		key_OCT_CIRCLE_xpose_STEP( keyNdx );

		// Account for track offsets when entering directly..
		key_NUMERIC_QUADRANT_velo_STEP( keyNdx );
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

