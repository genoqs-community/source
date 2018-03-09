

	//
	// TRACK SELECTORS
	//
	if ((keyNdx > 0) && (keyNdx <= 10)) {						

		switch( MODE_OBJECT_SELECTION ){

			// Select change track selection
			case BIRDSEYE: 
				// Change the track selection in page
				target_page->trackSelection = ( 1 << (keyNdx - 1) );
				break;


			// Map selector is held pressed
			case PASSIVE:

//				break;


			// No map selector kep pressed, regular interactive mode
			case INTERACTIVE:

				// Edit Track Attribute Selection if one of the valid ones						
				if ( ( (1 << (keyNdx-1) ) & PANEL_get_trackAttributeMap() ) != 0 ) {
		
					// D O U B L E - C L I C K
					if ((DOUBLE_CLICK_TARGET == keyNdx) 
							&& (DOUBLE_CLICK_TIMER > DOUBLE_CLICK_ALARM_SENSITIVITY)) {
		
						// Make sure the selected attribute is recorded. Else risk 0 value
						target_page->trackAttributeSelection = 1 << (keyNdx-1);
		
						// Switch zoomLevel
						zoom_level = zoomMAP;
		
						// Switch to keymode KMOD_ATTR_MAP
						target_page->keyMode = KMOD_ATTR_MAP;
					}
		
					else if (DOUBLE_CLICK_TARGET == 0) {
		
						DOUBLE_CLICK_TARGET = keyNdx;
						DOUBLE_CLICK_TIMER = ON;								
						// Start the Double click Alarm
						cyg_alarm_initialize(	
							doubleClickAlarm_hdl, 
							cyg_current_time() + DOUBLE_CLICK_ALARM_TIME,
							DOUBLE_CLICK_ALARM_TIME );
		
						// Toggle the attribute selection
						if (target_page->trackAttributeSelection == (1 << (keyNdx-1)) ) {
							target_page->trackAttributeSelection = 0;
						}
						else {
							target_page->trackAttributeSelection = (1 << (keyNdx-1));
						}	
					}					
				} // Double click target == keyNdx

				else{
				
					// target_page->trackAttributeSelection = 0;
				}

				break;
		}

	} // key is a track selector



	//
	// MATRIX KEYS 
	//
	// If the key pressed is in the Matrix
	if ( (keyNdx > 10) && (keyNdx <= 185) && (((keyNdx-10) % 11) != 0) ) { 


		// VELOCITY
		//
		if ( ((keyNdx - VELOCITY) % 11) == 0) {
			
			// Row of target track
			row = my_bit2ndx( target_page->trackSelection );

			// Column of target step
			col = ((keyNdx - VELOCITY) / 11) - 1 ;

			// Kill the event offset
			target_page->Track[row]->event_offset[ATTR_VELOCITY] = 0;

			// D O U B L E - C L I C K
			if ((DOUBLE_CLICK_TARGET == keyNdx) 
					&& (DOUBLE_CLICK_TIMER > DOUBLE_CLICK_ALARM_SENSITIVITY)) {

				// Double click code:
				//
				// Modify the tens value of the AMOUNT

				// Break up original value in tens and ones.
				i = target_page->Track[row]->attribute[ATTR_VELOCITY] / 10;
				j = target_page->Track[row]->attribute[ATTR_VELOCITY] % 10;				
				
				if ( col < 12 ){

					// Compute new tens
					i = 10 * (col + 1);

					// Get new ones value, leave it as it is otherwise
					if ( col < 9 ){
						// j = col + 1;
						j = 0;
					}

					// Write the final value into the attribute
					target_page->Track[row]->attribute[ATTR_VELOCITY] = 
						normalize( i + j, 0, 127 );
				}
				else{

					// Write the final value into the attribute
					target_page->Track[row]->attribute[ATTR_VELOCITY] = TRACK_DEF_VELOCITY;
				}
			}

			else if (DOUBLE_CLICK_TARGET == 0) {

				DOUBLE_CLICK_TARGET = keyNdx;
				DOUBLE_CLICK_TIMER = ON;								
				// Start the Double click Alarm
				cyg_alarm_initialize(	
					doubleClickAlarm_hdl, 
					cyg_current_time() + DOUBLE_CLICK_ALARM_TIME,
					DOUBLE_CLICK_ALARM_TIME );

				// Single click code:
				//
				if ( col < 9 ){

					if ( target_page->Track[row]->attribute[ATTR_VELOCITY] >= 127 ){
						i = 0;
					}
					else {
						i = target_page->Track[row]->attribute[ATTR_VELOCITY] / 10;
					}
					target_page->Track[row]->attribute[ATTR_VELOCITY] = 
						normalize( i*10 + col + 1 - target_page->Track[row]->event_offset[ATTR_VELOCITY], 0, TRACK_MAX_VELOCITY );
				}
			}
		} // key in the AMOUNT row.



		// POSITION (position) row allows step toggle edit for the track 
		// The functionality should be identical to the one in Page mode.
		//
		if ( ((keyNdx - POSITION) % 11) == 0) {
			
//////////////////////////////

			// Needs some more elaboration but works for now
			target_page->page_clear = OFF;
	
			// Compute Step coordinates
			// Row of target track
			row = my_bit2ndx( target_page->trackSelection );
	
			// Column of target step
			col = ((keyNdx - POSITION) / 11) - 1 ;
						
			// d_iag_printf( "step row:%d col:%d ts:%d\n", 
			//	row, col, target_page->trackSelection );

			// Toggle, un/select or un/mute the step in question
			interpret_matrix_stepkey( row, col, target_page );

			// d_iag_printf( "trackpattern:%d\n", Page_get_trackpattern( target_page, row ) );
			// my_print_bits( Page_get_trackpattern( target_page, row ) );
			
/////////////////////////////
	
		} // key is in POSITION row
	
	

		// PITCH row allows direct pitch entry for the track 
		//
		if ( ((keyNdx - PITCH) % 11) == 0) {
			// Key press is in the PITCH row

			// Row of target track
			row = my_bit2ndx( target_page->trackSelection );

			// Column of target step
			col = ((keyNdx - PITCH) / 11) - 1 ;

			// Kill the event offset
			target_page->Track[row]->event_offset[ATTR_PITCH] = 0;

			// Absolute keyindex of pressed button
			j = ((col+1) * 11) + (row);

			// This is a scale button
			if ( (col >= 2) && (col <= 13) ){
				
				// Adjust the pitch accordingly	
				target_page->Track[row]->attribute[ATTR_PITCH] =
						(target_page->Track[row]->attribute[ATTR_PITCH] / 12) * 12 
					+ 	(col - 2);
			}

			// This is the decrease 'r'-button
			if ( 	(col == 14 ) 
				&& 	(target_page->Track[row]->attribute[ATTR_PITCH] >= (TRACK_MIN_PITCH+12)) 
				){
				
				// Decrease one octave
//				target_page->Track[row]->attribute[ATTR_PITCH] -= 12;
			}

			// This is the increase 'r'-button
			if ( 	(col == 15 ) 
				&& 	(target_page->Track[row]->attribute[ATTR_PITCH] <= (TRACK_MAX_PITCH-12) )
				){
				
				// Increase one octave
//				target_page->Track[row]->attribute[ATTR_PITCH] += 12;
			}
		}
			
				
	
		// START (START) row allows direct entry of data. 
		// LENGTH (LENGTH) 
		// More complex - need to block them from conflicting with each other,
		// so right now both will stay as they are.



		// START row allows direct entry of data. 
		//
		if ( ((keyNdx - START) % 11) == 0) {
			
			// Target coordinates
			row = my_bit2ndx( target_page->trackSelection );
			col = ((keyNdx - START) / 11) - 1 ;

			// Set the new value of the STA_factor			
			target_page->Track[row]->STA_factor = col +1;
			
		} // key in the START row.


		// LENGTH row allows direct entry of data. 
		//
		if ( ((keyNdx - LENGTH) % 11) == 0) {
			
			// Target coordinates
			row = my_bit2ndx( target_page->trackSelection );
			col = ((keyNdx - LENGTH) / 11) - 1 ;

			// Set the new value of the LEN_factor			
			target_page->Track[row]->LEN_factor = col +1;

		} // key in the START row.
	

		// DIRECTION (direction) row allows direct entry of data. 
		//
		if ( ((keyNdx - DIRECTION) % 11) == 0) {
			
			// Row of target track
			row = my_bit2ndx( target_page->trackSelection );

			// Column of target step
			col = ((keyNdx - DIRECTION) / 11) - 1 ;

			// Set the new value of the GROOVE attribute - on the same port.
			target_page->Track[row]->attribute[ATTR_DIRECTION] 
				= normalize( col + 1, 0, TRACK_MAX_DIRECTION );
		} // key in the DIRECTION row.
	


		// AMOUNT (Track amount) row allows direct entry of data. 
		//
		if ( ((keyNdx - AMOUNT) % 11) == 0) {
			
			target_page->Track[row]->event_offset[ATTR_AMOUNT] = 0;

			// Row of target track
			row = my_bit2ndx( target_page->trackSelection );

			// Column of target step
			col = ((keyNdx - AMOUNT) / 11) - 1 ;

			// D O U B L E - C L I C K
			if ((DOUBLE_CLICK_TARGET == keyNdx) 
					&& (DOUBLE_CLICK_TIMER > DOUBLE_CLICK_ALARM_SENSITIVITY)) {

				// Double click code:
				//
				// Modify the tens value of the AMOUNT

				// Break up original value in tens and ones.
				i = target_page->Track[row]->attribute[ATTR_AMOUNT] / 10;
				j = target_page->Track[row]->attribute[ATTR_AMOUNT] % 10;				
				
				if ( col < 12 ){

					// Compute new tens
					i = 10 * (col + 1);

					// Get new ones value, leave it as it is otherwise
					if ( col < 9 ){
						// j = col + 1;
						j = 0;
					}

					// Write the final value into the attribute
					target_page->Track[row]->attribute[ATTR_AMOUNT] = 
						normalize( i + j, 0, 127 );
				}
				else{

					// Write the final value into the attribute
					target_page->Track[row]->attribute[ATTR_AMOUNT] = TRACK_DEF_AMOUNT;
				}
			}

			else if (DOUBLE_CLICK_TARGET == 0) {

				DOUBLE_CLICK_TARGET = keyNdx;
				DOUBLE_CLICK_TIMER = ON;								
				// Start the Double click Alarm
				cyg_alarm_initialize(	
					doubleClickAlarm_hdl, 
					cyg_current_time() + DOUBLE_CLICK_ALARM_TIME,
					DOUBLE_CLICK_ALARM_TIME );

				// Single click code:
				//
				if ( col < 9 ){
					// Modify the ones value of the MIDICC. 128 is a special value.
					if ( target_page->Track[row]->attribute[ATTR_AMOUNT] >= 127 ){
						i = 0;
					}
					else {
						i = target_page->Track[row]->attribute[ATTR_AMOUNT] / 10;
					}
					target_page->Track[row]->attribute[ATTR_AMOUNT] = 
						normalize( i*10 + col + 1, 0, 127 );
				}
			}
		} // key in the AMOUNT row.

	
	

		// GROOVE (groove) row allows direct entry of data. 
		//
		if ( ((keyNdx - GROOVE) % 11) == 0) {
			
			// Row of target track
			row = my_bit2ndx( target_page->trackSelection );

			// Column of target step
			col = ((keyNdx - GROOVE) / 11) - 1 ;

			// D O U B L E - C L I C K
			if ((DOUBLE_CLICK_TARGET == keyNdx) 
					&& (DOUBLE_CLICK_TIMER > DOUBLE_CLICK_ALARM_SENSITIVITY)) {

				// Double click code:
				//
				// Ste GROOVE attribute value to 0				
				target_page->Track[row]->attribute[ATTR_GROOVE] = 0;
			}

			else if (DOUBLE_CLICK_TARGET == 0) {

				DOUBLE_CLICK_TARGET = keyNdx;
				DOUBLE_CLICK_TIMER = ON;								
				// Start the Double click Alarm
				cyg_alarm_initialize(	
					doubleClickAlarm_hdl, 
					cyg_current_time() + DOUBLE_CLICK_ALARM_TIME,
					DOUBLE_CLICK_ALARM_TIME );
			
				// Set the new value of the GROOVE attribute - on the same port.
				target_page->Track[row]->attribute[ATTR_GROOVE] = col + 1;
			}
		} // key in the GROOVE row.




		// MIDICC (MIDI CC) row allows direct entry of data. 
		//
		if ( ((keyNdx - MIDICC) % 11) == 0) {
			
			// Row of target track
			row = my_bit2ndx( target_page->trackSelection );

			// Column of target step
			col = ((keyNdx - MIDICC) / 11) - 1 ;

			target_page->Track[i]->event_offset[ATTR_MIDICC] = 0;

			// D O U B L E - C L I C K
			if ((DOUBLE_CLICK_TARGET == keyNdx) 
					&& (DOUBLE_CLICK_TIMER > DOUBLE_CLICK_ALARM_SENSITIVITY)) {

				// Double click code:
				//
				// Modify the tens value of the MIDICC

				// Break up original value in tens and ones.
				i = target_page->Track[row]->attribute[ATTR_MIDICC] / 10;
				j = target_page->Track[row]->attribute[ATTR_MIDICC] % 10;				
				
				if ( col < 12 ){

					// Compute new tens
					i = 10 * (col + 1);

					// Get new ones value, leave it as it is otherwise
					if ( col < 9 ){
						// j = col + 1;
						j = 0;
					}

					// Write the final value into the attribute
					target_page->Track[row]->attribute[ATTR_MIDICC] = 
						normalize( i + j, 0, 127 );
				}
				else{

					// Write the final value into the attribute
					target_page->Track[row]->attribute[ATTR_MIDICC] = TRACK_DEF_MIDICC;
				}
			}

			else if (DOUBLE_CLICK_TARGET == 0) {

				DOUBLE_CLICK_TARGET = keyNdx;
				DOUBLE_CLICK_TIMER = ON;								
				// Start the Double click Alarm
				cyg_alarm_initialize(	
					doubleClickAlarm_hdl, 
					cyg_current_time() + DOUBLE_CLICK_ALARM_TIME,
					DOUBLE_CLICK_ALARM_TIME );

				// Single click code:
				//
				if ( col < 9 ){
					// Modify the ones value of the MIDICC. 128 is a special value.
					if ( target_page->Track[row]->attribute[ATTR_MIDICC] >= 127 ){
						i = 0;
					}
					else {
						i = target_page->Track[row]->attribute[ATTR_MIDICC] / 10;
					}
					target_page->Track[row]->attribute[ATTR_MIDICC] = 
						normalize( i*10 + col + 1, 0, 127 );
				}
			}
		} // key in the MIDICC row.



	
	
		// MIDICH (MIDI Channel) row allows direct entry of data. 
		// Single click selects channel on the current port. Double click on the other port. 
		// Disregards the virtual midi channels
		if ( ((keyNdx - MIDICH) % 11) == 0) {
			
			// Row of target track
			row = my_bit2ndx( target_page->trackSelection );

			// Column of target step
			col = ((keyNdx - MIDICH) / 11) - 1 ;

			// D O U B L E - C L I C K
			if ((DOUBLE_CLICK_TARGET == keyNdx) 
					&& (DOUBLE_CLICK_TIMER > DOUBLE_CLICK_ALARM_SENSITIVITY)) {

				// Double click code:
				///
				// Switch the MIDI port
				// Find out the current port: it will be 0 or 1;
				// Note that the ATTR_MIDICH is in the range [1..32].
				i = target_page->Track[row]->attribute[ATTR_MIDICH] / 17;
				j = target_page->Track[row]->attribute[ATTR_MIDICH] - (16 * i);

				// Swith the port indicator
				i ^= 1;

				// Write the new value to the attribute
				target_page->Track[row]->attribute[ATTR_MIDICH] = (16 * i) + j;
			}

			else if (DOUBLE_CLICK_TARGET == 0) {

				DOUBLE_CLICK_TARGET = keyNdx;
				DOUBLE_CLICK_TIMER = ON;								
				// Start the Double click Alarm
				cyg_alarm_initialize(	
					doubleClickAlarm_hdl, 
					cyg_current_time() + DOUBLE_CLICK_ALARM_TIME,
					DOUBLE_CLICK_ALARM_TIME );

				// Single click code:
				// Find out the current port: it will be 0 or 1;
				// Note that the ATTR_MIDICH is in the range [1..32].
				i = target_page->Track[row]->attribute[ATTR_MIDICH] / 17;
			
				// Set the new value of the MIDICH attribute - on the same port.
				target_page->Track[row]->attribute[ATTR_MIDICH] = (16 * i) + col + 1;
			}
		} // key in the MIDICH row.

	} // If this is a key in the Matrix
	
	

	//
	// TRACK MUTATORS
	//				
	// Identify selected track so we know what to operate on
	for (i=0; i<MATRIX_NROF_ROWS; i++){
		if ( ((1<<i) & (target_page->trackSelection)) != 0){
			row = i;
		}
	}

	// ENTITY MODE:
	if (target_page->trackAttributeSelection == OFF) {

		// TGGL function: set or unset the mute bit on the track
		if (keyNdx == TGGL_KEY) {
		
			// Toggle the mute bit pattern
			target_page->trackMutepattern ^= (1<<row);
		}

		
		// SOLO function: set or unset the solo bit of the track
		if (keyNdx == SOLO_KEY) {
		
			// Toggle the SOLO bits on the selected track - double click ensures only one track selected
			target_page->trackSolopattern ^= (1<<row);
		}
			
		
		// CLEAR function					
		if (keyNdx == CLEAR_KEY) {
		
			// Clear the contents of the track
			for (col = 0; col < MATRIX_NROF_COLUMNS; col++) {

				target_page->Step[row][col]->attribute[ATTR_STATUS] = OFF;
				Step_init( target_page->Step[row][col] );
			}
		}
		
		
		// RANDOMIZE function
		if (keyNdx == RANDOMIZE_KEY) {

			// Randomize the selected track in page. This format should be used everywhere in here..
			Page_RNDMZ_selected_tracks( target_page );
		}
		
		
		// FLIP TRACK function
		if( keyNdx == FLT_KEY ){
			
			if ( my_bit_cardinality( target_page->trackSelection ) > 1 ){

				// Flatten the selected tracks in page. This format should be used everywhere in here..
				Page_FLT_selected_tracks( target_page );
			}
		}


		// REMIX TRACK function
		if( keyNdx == RMX_KEY ){

			// Remix the selected tracks in page. 
			Page_RMX_selected_tracks( target_page );
		}

		
		// EFF TRACK function
		if( keyNdx == EFF_KEY ){

			// Calculate the row of the selected track
			row = my_bit2ndx( target_page->trackSelection );
			
			// Cycle through the EFF states: OFF (0), SEND (1), RECEIVE (2), RECEIVESEND (3)
			temp = Track_get_MISC( target_page->Track[row], EFF_BIT );
			Track_set_MISC( target_page->Track[row], EFF_BIT, (temp+1) % 4 );

//			// D O U B L E - C L I C K  C O N S T R U C T
//			// DOUBLE CLICK SCENARIO
//			if ((DOUBLE_CLICK_TARGET == keyNdx) 
//					&& (DOUBLE_CLICK_TIMER > DOUBLE_CLICK_ALARM_SENSITIVITY)) {
//		
//				// Double click code
//				// ...
//				// Set the status to RECEIVE
//				Track_set_MISC( target_page->Track[row], EFF_BIT, RECEIVE );
//			
//			} // end of double click scenario 	
//			// SINGLE CLICK SCENARIO
//			else if (DOUBLE_CLICK_TARGET == 0) {
//				
//					DOUBLE_CLICK_TARGET = keyNdx;
//					DOUBLE_CLICK_TIMER = ON;
//					// Start the Double click Alarm
//					cyg_alarm_initialize(	
//							doubleClickAlarm_hdl, 
//							cyg_current_time() + DOUBLE_CLICK_ALARM_TIME,
//							DOUBLE_CLICK_ALARM_TIME );
//		
//				// Single click code
//				// ...				
//				// Cycle through the EFF states: OFF, SEND, remove the RECEIVE bit
//				switch( Track_get_MISC( target_page->Track[row], EFF_BIT ) ){
//
//					case OFF:
//						Track_set_MISC( target_page->Track[row], EFF_BIT, SEND );
//						break;
//						
//					case SEND:
//					case RECEIVE:
//						Track_set_MISC( target_page->Track[row], EFF_BIT, OFF );
//						break;
//				}
//			} // Single click scenario

		} // EFF_KEY


		// ZOOM function
		if (keyNdx == ZOOM_KEY) {

			// Make sure there are no steps selected 
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
						Step_set_status( target_page->Step[row][col], STEPSTAT_SELECT, OFF);
		
						// And decrement step selection counter
						target_page->stepSelection --;
					}
				}	
			}
			
			// ..and Zoom out into the previos mode
			target_page->keyMode = KMOD_PAGE;
			
			// Switch zoom level
			zoom_level = zoomPAGE;
		}

		
		// COPY function
		if (keyNdx == COPY_KEY) {
			
			// Remember the selected track in terms of page and row
			TRACK_COPY_BUFFER = ( 	target_page->trackSelection 
								| ( target_page->pageNdx << 10 ) 
								);						

			// Clear the track selection
			target_page->trackSelection = 0;
			zoom_level = zoomPAGE;
			target_page->keyMode = KMOD_PAGE;
		}

		// PASTE function
		if (keyNdx == PASTE_KEY) {
			
			// Check if there is anything in the copy buffer
			if ( TRACK_COPY_BUFFER != 0 ) {
				
				// Get the 10 trackselection bits
				row = my_bit2ndx( TRACK_COPY_BUFFER & 0x3FF );

				// Get the source page index
				j = TRACK_COPY_BUFFER >> 10;

				// Execute the copy operation
				Track_copy( &Page_repository[j], row,
							target_page, my_bit2ndx( target_page->trackSelection ) );
			}

			TRACK_COPY_BUFFER = 0;
		}
	} // Entity mode operations



	// ATTRIBUTE selected mode operations
	else {

		switch (target_page->trackAttributeSelection) {


			case (1<< VELOCITY):

				if (keyNdx == RANDOMIZE_KEY){
					
					for ( i=0; i < MATRIX_NROF_COLUMNS; i++ ){
						
						// Randomize step velocity
						target_page->Step[row][i]->attribute[ATTR_VELOCITY] 
							= randomize_byamount(	target_page->Step[row][i]->attribute[ATTR_VELOCITY],
													target_page->Track[row]->attribute[ATTR_AMOUNT]
													+ target_page->Track[row]->event_offset[ATTR_AMOUNT]
													,
													STEP_MIN_VELOCITY,
													STEP_MAX_VELOCITY );
					}
				}								

				if (keyNdx == CLEAR_KEY){

					// Reset the velocity map of the track - i.e. all track velocities
					for ( i=0; i < MATRIX_NROF_COLUMNS; i++ ){
					
						target_page->Step[row][i]->attribute[ATTR_VELOCITY] = STEP_DEF_VELOCITY;
						target_page->Track[row]->VEL_factor = TRACK_DEF_VELFACTOR;
					}
				}

				break;



			case (1<< PITCH):

				if (keyNdx == RANDOMIZE_KEY){

					for ( i=0; i < MATRIX_NROF_COLUMNS; i++ ){
						
						// Randomize step velocity
						target_page->Step[row][i]->attribute[ATTR_PITCH] 
							= randomize_byamount(	target_page->Step[row][i]->attribute[ATTR_PITCH],
													target_page->Track[row]->attribute[ATTR_AMOUNT]
													+ target_page->Track[row]->event_offset[ATTR_AMOUNT]
													,
													STEP_MIN_PITCH,
													STEP_MAX_PITCH );
					}
				}

				if (keyNdx == CLEAR_KEY){

					// Reset the velocity map of the track - i.e. all track velocities
					for ( i=0; i < MATRIX_NROF_COLUMNS; i++ ){
					
						target_page->Step[row][i]->attribute[ATTR_PITCH] = STEP_DEF_PITCH;
						target_page->Track[row]->PIT_factor = TRACK_DEF_PITFACTOR;
						target_page->Track[row]->event_offset[ATTR_PITCH] = 0;
					}
				}
				break;
			
			

			case ( 1<< LENGTH ):

				if (keyNdx == RANDOMIZE_KEY){

					for ( i=0; i < MATRIX_NROF_COLUMNS; i++ ){
						
						// Randomize step velocity
						target_page->Step[row][i]->attribute[ATTR_LENGTH] 
							= randomize_byamount(	target_page->Step[row][i]->attribute[ATTR_LENGTH],
													target_page->Track[row]->attribute[ATTR_AMOUNT]
													+ target_page->Track[row]->event_offset[ATTR_AMOUNT]
													,
													STEP_MIN_LENGTH,
													STEP_MAX_LENGTH );
					}
				}

				if (keyNdx == CLEAR_KEY){

					// Reset the velocity map of the track - i.e. all track velocities
					for ( i=0; i < MATRIX_NROF_COLUMNS; i++ ){
					
						target_page->Step[row][i]->attribute[ATTR_LENGTH] = STEP_DEF_LENGTH;
						target_page->Track[row]->LEN_factor = TRACK_DEF_LENFACTOR;
					}
				}
				break;
			
			

			case ( 1<< START ):

				if (keyNdx == RANDOMIZE_KEY){

					for ( i=0; i < MATRIX_NROF_COLUMNS; i++ ){
						
						// Randomize step velocity
						target_page->Step[row][i]->attribute[ATTR_START] 
							= randomize_byamount(	target_page->Step[row][i]->attribute[ATTR_START],
													target_page->Track[row]->attribute[ATTR_AMOUNT]
													+ target_page->Track[row]->event_offset[ATTR_AMOUNT]
													,
													STEP_MIN_START,
													STEP_MAX_START );
					}
				}

				if (keyNdx == CLEAR_KEY){

					// Reset the velocity map of the track - i.e. all track velocities
					for ( i=0; i < MATRIX_NROF_COLUMNS; i++ ){
					
						target_page->Step[row][i]->attribute[ATTR_START] = STEP_DEF_START;
						target_page->Track[row]->STA_factor = TRACK_DEF_STAFACTOR;
					}
				}
				break;
			
				

			case ( 1<< MIDICC ):

				if (keyNdx == RANDOMIZE_KEY){

					for ( i=0; i < MATRIX_NROF_COLUMNS; i++ ){
						
						// Randomize step velocity
						target_page->Step[row][i]->attribute[ATTR_MIDICC] 
							= randomize_byamount(	target_page->Step[row][i]->attribute[ATTR_MIDICC],
													target_page->Track[row]->attribute[ATTR_AMOUNT]
													+ target_page->Track[row]->event_offset[ATTR_AMOUNT]
													,
													STEP_MIN_MIDICC,
													STEP_MAX_START );
					}
				}								

				if (keyNdx == CLEAR_KEY){

					// Reset the velocity map of the track - i.e. all track velocities
					for ( i=0; i < MATRIX_NROF_COLUMNS; i++ ){
					
						target_page->Step[row][i]->attribute[ATTR_MIDICC] = STEP_DEF_MIDICC;
						target_page->Track[row]->MCC_factor = TRACK_DEF_MIDICC;
					}
				}
				break;
				

			case (1<< GROOVE):

				if (keyNdx == RANDOMIZE_KEY){

					for ( i=0; i < MATRIX_NROF_COLUMNS; i++ ){
						
						// Randomize step velocity
						target_page->Step[row][i]->attribute[ATTR_GROOVE]
							= randomize_byamount(	target_page->Step[row][i]->attribute[ATTR_GROOVE],
													target_page->Track[row]->attribute[ATTR_AMOUNT]
													+ target_page->Track[row]->event_offset[ATTR_AMOUNT]
													,
													STEP_MIN_GROOVE,
													STEP_MAX_GROOVE );
					}
				}

				if (keyNdx == CLEAR_KEY){
					
					// Reset the velocity map of the track - i.e. all track velocities
					for ( i=0; i < MATRIX_NROF_COLUMNS; i++ ){
					
						target_page->Step[row][i]->attribute[ATTR_GROOVE] = STEP_DEF_GROOVE;
						target_page->Track[row]->event_offset[ATTR_GROOVE] = 0;
						target_page->Track[row]->GRV_factor = TRACK_DEF_GRVFACTOR;
					}
				}
				break;
				
			case (1<< MIDICH):
				break;
		}

		if (keyNdx == ZOOM_KEY){
			
			// Zoom into the attribute MAP
			target_page->keyMode = KMOD_ATTR_MAP;
			// Switch zoomLevel
			zoom_level = zoomMAP;
		}
	}



	//
	// ZOOM SELECTOR
	// 
	if (keyNdx == KEY_ZOOM_GRID) {

		target_page->trackSelection = 0;
		target_page->keyMode = KMOD_PAGE;
		
		zoom_level = zoomGRID;
		// Keymode is implicit for GRID
	}

	if (keyNdx == KEY_ZOOM_PAGE) {
		zoom_level = zoomPAGE;
		target_page->keyMode = KMOD_PAGE;
		target_page->trackSelection = 0;
	}

	if (keyNdx == KEY_ZOOM_MAP) {
		// React only if there is one attribute selected 
		if (target_page->trackAttributeSelection != 0){
			zoom_level = zoomMAP;
			target_page->keyMode = KMOD_ATTR_MAP;
		}
	}

	if (keyNdx == KEY_ZOOM_PLAY) {
//	 	zoom_level = zoomPLAY;
//		target_page->keyMode = KMOD_PLAY;
	}


	// 
	// TRACK CHAIN SELECTORS - active only when sequencer not running.
	//
	key_exe_chainselectors( keyNdx );

	//
	// BIG KNOB KEYS
	//
	if ( keyNdx == KEY_PROGRAM ){
		
		// Send the program change Value on the selected track(s)
		for (i=0; i < MATRIX_NROF_ROWS; i++) {

			if (	( (target_page->trackSelection & (1<<i)) != 0 )
				&&	( ((unsigned char) target_page->Track[ my_bit2ndx( target_page->trackSelection )]->program_change) > 0 )
				){

				MIDI_send( 	MIDI_PGMCH, 
							target_page->Track[i]->attribute[ATTR_MIDICH]
							+ target_page->Track[i]->event_offset[ATTR_MIDICH], 
							target_page->Track[ my_bit2ndx( target_page->trackSelection )]->program_change - 1, 
							0 );
			}
		}
	}
	
	
	//
	// FOLLOW_TRACK
	//
	if (	( keyNdx == KEY_FOLLOW )
		&&	( PLAY_MODE_STATUS == OFF )
		){

		// Toggle the FOLLOW_TRACK switch
		if (	( follow_flag == FOLLOW_TRACK )
			){
			
			follow_flag = OFF;
		}
		else{

			follow_flag = FOLLOW_TRACK;
		}
	}

		

	//
	// ALIGN LAUFLICHT 
	//
	if (	( keyNdx == KEY_ALIGN )
		){
		
		// Aligns only the target_page to the global play data
		set_page_locators( target_page, GLOBAL_LOCATOR, TTC_ABS_VALUE );
	}


		

	// 
	// CHORD SELECTOR
	//
	// Selects the chord cardinality (page or track??)
	if( (keyNdx > 251) 
		){
			
		// Pretty ghetto: this is the target value of the chord bit		
		j = 7 - (keyNdx - 252);	

//		if ( my_bit_cardinality( target_page->trackSelection ) == 1 ){			

		// Modify all selected tracks
		for( i=0; i < MATRIX_NROF_ROWS; i++ ){
	
			if ((target_page->trackSelection & (1<<i)) != 0){
	
// Disabled the track chord for now		
				Track_set_MISC( target_page->Track[i], CHORD_BIT, j );
			} // track is selected
		} // row iterator

	} // Chord selector key pressed




	// 
	// OCTAVE CIRCLE
	//
	// Enable track trans-positions with one click and directly
	#include "key_OCT_CIRCLE_xpose_TRACK.h"	



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
	// Enter PROGRAM, range_VEL or range_PIT using the BK keys
	// 
	if ( 	( keyNdx == 201 )
		||	( keyNdx == 200	)
		||	( keyNdx == 199 )
		|| 	( keyNdx == 198 )
		|| 	( keyNdx == 197 )
		||	( keyNdx == 207 )
		||	( keyNdx == 206 )
		||	( keyNdx == 216 )
		|| 	( keyNdx == 215 )
		||	( keyNdx == 224 )
		){

		switch( target_page->trackAttributeSelection ){
		
//			// VEL
//			case 1:
//				// Use numeric quadrant to enter VEL interval
//				key_NUMERIC_QUADRANT_STEP_VEL_interval( keyNdx );
//				break;
//
//			// PIT
//			case 2:
//				// Use numeric quadrant to enter PIT interval
//				key_NUMERIC_QUADRANT_STEP_PIT_interval( keyNdx );
//				break;

			// none selected
			case 0:
				// Use numeric quadrant to enter PROGRAM for change
				// key_NUMERIC_QUADRANT_TRACK_PROGRAM( keyNdx );
				break;
		}
//			// Enter velocity via outter circle
//			// Account for track offsets when entering directly..
//			key_NUMERIC_QUADRANT_velo_STEP( keyNdx );
//		}
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

#include "key_TRACK_multiplier.h"			

			break;
	}	





