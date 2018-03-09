// Key execution in MIXER MAP keymode

	//
	// TRACK SELECTORS
	//

	if ((keyNdx >= 1) && (keyNdx <= 10)) {

		if (	( target_page->REC_bit == ON )
			){
		
			// Toggle bit in trackSelection marking the track un/selected
			CCMAP_learner = keyNdx-1;
		}
		else{

			// Restart MIX Timer if it was started somewhere else
			restart_MIX_TIMER();
			
			// Action depends on the mix target
			
			// CC_MIXMAP mode selected
			if (target_page->mixTarget < 6) {
				switch( keyNdx ) {
					case 7: 
						target_page->CC_MIXMAP_attribute = CC_MIXMAP_AMT;
						break;
	
					case 9:
						target_page->CC_MIXMAP_attribute = CC_MIXMAP_MCC;
						break;
	
					case 10:
						target_page->CC_MIXMAP_attribute = CC_MIXMAP_MCH;
						break;
				}
			} 
			// MIXTGT_ATR mode
			else if (target_page->mixTarget == 10) {
				
				// Edit MIX ATTRIBUTE - exit if same attribute selected
				if (target_page->mixAttribute == keyNdx) {
	
					target_page->keyMode = KMOD_PAGE;
				} 
				else {
	
					target_page->mixAttribute = keyNdx;
				}
			}
		}
	}


	//
	// MIX MASTER
	//
	if (keyNdx == KEY_MIX_MASTER) {
		// Toggle the mixMode: INC or DEC
		target_page->mixMode ^= 0x03;
	}


	// 
	// MIX TARGET
	//
	if (keyNdx == KEY_MIXTGT_ATR) {
		if ( target_page->mixTarget == MIXTGT_ATR ){
			target_page->keyMode = KMOD_PAGE;
		}
		else {	
			restart_MIX_TIMER();
			target_page->mixTarget = MIXTGT_ATR;
		}
	
		// Set the CC_MIXMAP_attribute to amount, to protect user from side effects.
		target_page->CC_MIXMAP_attribute = CC_MIXMAP_AMT;
	}

	if (keyNdx == KEY_MIXTGT_VOL) {
		if ( target_page->mixTarget == MIXTGT_VOL ){
			target_page->keyMode = KMOD_PAGE;
		}
		else {	
			restart_MIX_TIMER();
			target_page->mixTarget = MIXTGT_VOL;
		}
		// Set the CC_MIXMAP_attribute to amount, to protect user from side effects.
		target_page->CC_MIXMAP_attribute = CC_MIXMAP_AMT;
	}

	if (keyNdx == KEY_MIXTGT_PAN) {
		if ( target_page->mixTarget == MIXTGT_PAN ){
			target_page->keyMode = KMOD_PAGE;
		}
		else {	
			restart_MIX_TIMER();
			target_page->mixTarget = MIXTGT_PAN;
		}
		// Set the CC_MIXMAP_attribute to amount, to protect user from side effects.
		target_page->CC_MIXMAP_attribute = CC_MIXMAP_AMT;
	}

	if (keyNdx == KEY_MIXTGT_MOD) {
		if ( target_page->mixTarget == MIXTGT_MOD ){
			target_page->keyMode = KMOD_PAGE;
		}
		else {	
			restart_MIX_TIMER();
			target_page->mixTarget = MIXTGT_MOD;
		}
		// Set the CC_MIXMAP_attribute to amount, to protect user from side effects.
		target_page->CC_MIXMAP_attribute = CC_MIXMAP_AMT;
	}

	if (keyNdx == KEY_MIXTGT_EXP) {
		if ( target_page->mixTarget == MIXTGT_EXP ){
			target_page->keyMode = KMOD_PAGE;
		}
		else {	
			restart_MIX_TIMER();
			target_page->mixTarget = MIXTGT_EXP;
		}
		// Set the CC_MIXMAP_attribute to amount, to protect user from side effects.
		target_page->CC_MIXMAP_attribute = CC_MIXMAP_AMT;
	}

// MAP0 Model demands this..
	if (keyNdx == KEY_MIXTGT_USR0) {

//		GRID_MAP0_status ^= 1;
//		if ( target_page->mixTarget == MIXTGT_USR0 ){
//			target_page->keyMode = KMOD_PAGE;
//		}
//		else {	
//			restart_MIX_TIMER();
//			target_page->mixTarget = MIXTGT_USR0;
//		}
//		// Set the CC_MIXMAP_attribute to amount, to protect user from side effects.
//		target_page->CC_MIXMAP_attribute = CC_MIXMAP_AMT;
	}

	if (keyNdx == KEY_MIXTGT_USR1) {
		if ( target_page->mixTarget == MIXTGT_USR1 ){
			target_page->keyMode = KMOD_PAGE;
		}
		else {	
			restart_MIX_TIMER();
			target_page->mixTarget = MIXTGT_USR1;
		}
		// Set the CC_MIXMAP_attribute to amount, to protect user from side effects.
		target_page->CC_MIXMAP_attribute = CC_MIXMAP_AMT;
	}

	if (keyNdx == KEY_MIXTGT_USR2) {
		if ( target_page->mixTarget == MIXTGT_USR2 ){
			target_page->keyMode = KMOD_PAGE;
		}
		else {	
			restart_MIX_TIMER();
			target_page->mixTarget = MIXTGT_USR2;
		}
		// Set the CC_MIXMAP_attribute to amount, to protect user from side effects.
		target_page->CC_MIXMAP_attribute = CC_MIXMAP_AMT;
	}

	if (keyNdx == KEY_MIXTGT_USR3) {
		if ( target_page->mixTarget == MIXTGT_USR3 ){
			target_page->keyMode = KMOD_PAGE;
		}
		else {	
			restart_MIX_TIMER();
			target_page->mixTarget = MIXTGT_USR3;
		}
		// Set the CC_MIXMAP_attribute to amount, to protect user from side effects.
		target_page->CC_MIXMAP_attribute = CC_MIXMAP_AMT;
	}

	if (keyNdx == KEY_MIXTGT_USR4) {
		if ( target_page->mixTarget == MIXTGT_USR4 ){
			target_page->keyMode = KMOD_PAGE;
		}
		else {	
			restart_MIX_TIMER();
			target_page->mixTarget = MIXTGT_USR4;
		}
		// Set the CC_MIXMAP_attribute to amount, to protect user from side effects.
		target_page->CC_MIXMAP_attribute = CC_MIXMAP_AMT;
	}

	if (keyNdx == KEY_MIXTGT_USR5) {
		if ( target_page->mixTarget == MIXTGT_USR5 ){
			target_page->keyMode = KMOD_PAGE;
		}
		else {	
			restart_MIX_TIMER();
			target_page->mixTarget = MIXTGT_USR5;
		}
		// Set the CC_MIXMAP_attribute to amount, to protect user from side effects.
		target_page->CC_MIXMAP_attribute = CC_MIXMAP_AMT;
	}




	//
	// MATRIX
	//
	// OFF (originally)
	if ( (keyNdx > 10) && (keyNdx <= 185) && (((keyNdx-10) % 11) != 0) ) { 

		// Compute the coordinates of the pressed key
		row = row_of( keyNdx );
		col = column_of( keyNdx );

		// Check the attribute being edited. For now pitch is special:
		switch( target_page->mixTarget ){

			case MIXTGT_ATR:

				switch( target_page->mixAttribute ){
					
					case ATTR_VELOCITY:
					case ATTR_AMOUNT:
					case ATTR_MIDICC:
					
						// Fix the target
						if ( target_page->mixAttribute == ATTR_VELOCITY ){
							target_ptr = 
								&target_page->Track[row]->attribute[ATTR_VELOCITY];
						}
						else if ( target_page->mixAttribute == ATTR_AMOUNT ){
							target_ptr = 
								&target_page->Track[row]->attribute[ATTR_AMOUNT];
						}
						else if ( target_page->mixAttribute == ATTR_MIDICC ){
							target_ptr = 
								&target_page->Track[row]->attribute[ATTR_MIDICC];
						}
						

						// D O U B L E - C L I C K
						if ((DOUBLE_CLICK_TARGET == keyNdx) 
								&& (DOUBLE_CLICK_TIMER > DOUBLE_CLICK_ALARM_SENSITIVITY)) {
			
							// Double click code:
							//
							// Modify the tens value of the AMOUNT
			
							// Break up original value in tens and ones.
							i = *target_ptr / 10;
							j = *target_ptr % 10;				
							
							if ( col < 12 ){
			
								// Compute new tens
								i = 10 * (col + 1);
			
								// Get new ones value, leave it as it is otherwise
								if ( col < 9 ){
									
									// j = col + 1;
									// Remove it so we can reach zero of ones.
									j = 0;
								}
			
								// Write the final value into the attribute
								*target_ptr = normalize( i + j, 0, 127 );							
							}
							
							else{
			
								// Write the final value into the attribute
								*target_ptr = 0;
							}
						} // second click of double click
			
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
								if ( *target_ptr > 127 ){
									i = 0;
								}
								else {
									i = *target_ptr / 10;
								}
								*target_ptr = normalize( i*10 + col + 1, 0, 127 );		
							}
						} // first click of double click

						// close case ATTR_VELOCITY _AMOUNT _MIDICC
						break;


					case ATTR_PITCH:
			
						// Make sure we are in the notes field:
						if ( ( col < 2 ) || (col > 13) )  {
			
							// Move octave of track
							if (col == 14) {
								// Move octave down if possible
								if (target_page->Track[row]->attribute[ATTR_PITCH] > 11){
									
									target_page->Track[row]->attribute[ATTR_PITCH] = 
										((target_page->Track[row]->attribute[ATTR_PITCH] / 12)-1) * 12 + 
										   target_page->Track[row]->attribute[ATTR_PITCH] % 12 ;
								}
							}
			
							if (col == 15) {
								// Move octave up if possible
								if (target_page->Track[row]->attribute[ATTR_PITCH] < 116){
									
									target_page->Track[row]->attribute[ATTR_PITCH] = 
										((target_page->Track[row]->attribute[ATTR_PITCH] / 12)+1) * 12 + 
										   target_page->Track[row]->attribute[ATTR_PITCH] % 12 ;
								}
							}
			
						}
						else {
							
							// Change track pitch according to key
							target_page->Track[row]->attribute[ATTR_PITCH] = 
								(target_page->Track[row]->attribute[ATTR_PITCH] / 12)*12 + col -2;
						 }
						break;
					 					 		
				
		
					case ATTR_DIRECTION:
			
						target_ptr = 
							&target_page->Track[row]->attribute[ATTR_DIRECTION];

						// Set the new value of the GROOVE attribute - on the same port.
						*target_ptr = normalize( col + 1, 0, TRACK_MAX_DIRECTION );
						break;
					 					 		
					 					 		
		
					case ATTR_GROOVE:
					
						target_ptr = 
							&target_page->Track[row]->attribute[ATTR_GROOVE];
	
						// D O U B L E - C L I C K
						if ((DOUBLE_CLICK_TARGET == keyNdx) 
								&& (DOUBLE_CLICK_TIMER > DOUBLE_CLICK_ALARM_SENSITIVITY)) {
			
							// Double click code:
							//
							// Ste GROOVE attribute value to 0				
							*target_ptr = 0;
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
							*target_ptr = col + 1;
						}
						break;	 					 		
					
					
					case ATTR_MIDICH:
					
						target_ptr = 
							&target_page->Track[row]->attribute[ATTR_MIDICH];

						// D O U B L E - C L I C K
						if ((DOUBLE_CLICK_TARGET == keyNdx) 
								&& (DOUBLE_CLICK_TIMER > DOUBLE_CLICK_ALARM_SENSITIVITY)) {
			
							// Double click code:
							///
							// Switch the MIDI port
							// Find out the current port: it will be 0 or 1;
							// Note that the ATTR_MIDICH is in the range [1..32].
							// Disregard the virtual MIDI channels
							i = *target_ptr / 17;
							j = *target_ptr - (16 * i);
			
							// Swith the port indicator
							i ^= 1;
			
							// Write the new value to the attribute
							*target_ptr = (16 * i) + j;
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
							i = *target_ptr / 17;
						
							// Set the new value of the MIDICH attribute - on the same port.
							*target_ptr = (16 * i) + col + 1;
						}						

						break;
						
				} // switch( target_page->mixAttribute )
				break; // MIXTGT_ATR


			case MIXTGT_VOL:
				target_ptr = &target_page->MIXAMT_VOL[row];
				key_mixermap_matrixclick( keyNdx, target_ptr );
				break;
								
			case MIXTGT_PAN:
				target_ptr = &target_page->MIXAMT_PAN[row];
				key_mixermap_matrixclick( keyNdx, target_ptr );
				break;
			
			case MIXTGT_MOD:
				target_ptr = &target_page->MIXAMT_MOD[row];
				key_mixermap_matrixclick( keyNdx, target_ptr );
				break;
			
			case MIXTGT_EXP:
				target_ptr = &target_page->MIXAMT_EXP[row];
				key_mixermap_matrixclick( keyNdx, target_ptr );
				break;


			case MIXTGT_USR0:
			case MIXTGT_USR1:
			case MIXTGT_USR2:
			case MIXTGT_USR3:
			case MIXTGT_USR4:
			case MIXTGT_USR5:

				// Find the target value to be modified
				switch( target_page->CC_MIXMAP_attribute ) {

					case CC_MIXMAP_AMT:
					case CC_MIXMAP_MCC:
					
						if ( target_page->CC_MIXMAP_attribute == CC_MIXMAP_AMT ){
							target_ptr = 
								&target_page->CC_MIXMAP[target_page->mixTarget][row][CC_MIXMAP_AMT];
						}
						else if ( target_page->CC_MIXMAP_attribute == CC_MIXMAP_MCC ){
							target_ptr = 
								&target_page->CC_MIXMAP[target_page->mixTarget][row][CC_MIXMAP_MCC];
						}

						// D O U B L E - C L I C K
						if ((DOUBLE_CLICK_TARGET == keyNdx) 
								&& (DOUBLE_CLICK_TIMER > DOUBLE_CLICK_ALARM_SENSITIVITY)) {
			
							// Double click code:
							//
							// Modify the tens value of the AMOUNT
			
							// Break up original value in tens and ones.
							i = *target_ptr / 10;
							j = *target_ptr % 10;				
							
							if ( col < 12 ){
			
								// Compute new tens
								i = 10 * (col + 1);
			
								// Get new ones value, leave it as it is otherwise
								if ( col < 9 ){
									
									// j = col + 1;
									j = 0;
								}
			
								// Write the final value into the attribute
								*target_ptr = normalize( i + j, 0, 127 );							
							}
							
							else{
			
								// Write the final value into the attribute
								*target_ptr = 0;
							}
						} // second click of double click
			
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
								if ( *target_ptr > 127 ){
									i = 0;
								}
								else {
									i = *target_ptr / 10;
								}
								*target_ptr = normalize( i*10 + col + 1, 0, 127 );		
							}
						} // first click of double click

						// close case CC_MIXMAP_AMT and CC_MIXMAP_MCC
						break;


					case CC_MIXMAP_MCH:
					
						target_ptr = 
							&target_page->CC_MIXMAP[target_page->mixTarget][row][CC_MIXMAP_MCH];

						// D O U B L E - C L I C K
						if ((DOUBLE_CLICK_TARGET == keyNdx) 
								&& (DOUBLE_CLICK_TIMER > DOUBLE_CLICK_ALARM_SENSITIVITY)) {
			
							// Double click code:
							///
							// Switch the MIDI port
							// Find out the current port: it will be 0 or 1;
							// Note that the ATTR_MIDICH is in the range [1..32].
							// Disregard the virtual MIDI channels
							i = *target_ptr / 17;
							j = *target_ptr - (16 * i);
			
							// Swith the port indicator
							i ^= 1;
			
							// Write the new value to the attribute
							*target_ptr = (16 * i) + j;
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
							i = *target_ptr / 17;
						
							// Set the new value of the MIDICH attribute - on the same port.
							*target_ptr = (16 * i) + col + 1;
						}

						break;
				}
				



		} // switch( target_page->mixTarget )

	}// If key in matrix range


	//
	// ZOOM MODE
	// 
	
	// GRID mode switch
	//
	if (keyNdx == KEY_ZOOM_GRID) {
		zoom_level = zoomGRID;
	}
	
	// PAGE mode switch
	// 
	if (keyNdx == KEY_ZOOM_PAGE) {

		// if (target_page->keyMode == KMOD_MIXER_MAP){
		if ( zoom_level == zoomMIXMAP ){

			// Go back to page mode
			zoom_level = zoomPAGE;
			target_page->keyMode = KMOD_PAGE;
			target_page->REC_bit = OFF;
		}
	}	


	//
	// OCTAVE CIRCLE
	//
	// Selects the scale for a page
	key_ScaleSelector_functions( keyNdx, target_page );


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


	// When in ATR mode, offer to program the track offsets into the steps.
	// .. to be elaborated
	if ( keyNdx == KEY_PROGRAM ){
		
		if ( target_page->mixTarget == MIXTGT_ATR ){

			// Program the track offsets into the steps
		}
	}
	

	// Toggle the SCL_align flag in the page
	if (	// ( keyNdx == KEY_PROGRAM )
			( keyNdx == KEY_SCALE_CAD )
		&&	( target_page->force_to_scale == ON )
		){

		target_page->SCL_align ^= ON;
		// d_iag_printf( "SCL_align:%d\n", target_page->SCL_align );
	}


	// ALIGN LAUFLICHT 
	else if (	( keyNdx == KEY_ALIGN )
		){
		
		// Act like this only when we are not dealing with scales.
		// and no track is selected
		if ( 	(target_page->scaleStatus == OFF)
			&& 	(target_page->trackSelection == 0) 
			){

			// Sorts the tracks of the target_page (descending order) by current mix attribute
			// ..only when dealing with some ATR
			if ( target_page->mixTarget == MIXTGT_ATR ){

				sort_tracks_by_ATR( target_page, target_page->mixAttribute );
			}			

			// Aligns the target_page to the global play data
			set_page_locators( target_page, GLOBAL_LOCATOR, TTC_ABS_VALUE );
		}

		// Toggle the force to scale bit for the page
		else if ( 	( target_page->scaleStatus != OFF )
				&&	( target_page->trackSelection == 0 )
			){

			target_page->force_to_scale ^= 1;			
		}

	} // KEY_ALIGN was pressed


	else if ( keyNdx == KEY_RANDOMIZE ){
		
		// RANDOMIZES the current mix attribute values of the VIEWER page
		// ..only when dealing with some ATR
		if ( target_page->mixTarget == MIXTGT_ATR ){

			rand_tracks_by_ATR( target_page, target_page->mixAttribute );
		}
	}	

	else if ( keyNdx == KEY_CLEAR ){
		
		// RANDOMIZES the current mix attribute values of the VIEWER page
		// ..only when dealing with some ATR
		if ( target_page->mixTarget == MIXTGT_ATR ){

			clear_tracks_by_ATR( target_page, target_page->mixAttribute );
		}
	}	

	else if ( keyNdx == KEY_COPY ){
		
		// The semantics of the buffered values
		ATTR_MAP_COPY_BUFFER[10] = target_page->mixAttribute;

		// COPIES the map attributes to the buffer
		for ( i=0; i < MATRIX_NROF_ROWS; i++ ){
		
			ATTR_MAP_COPY_BUFFER[i] = 
				target_page->Track[i]->attribute[ target_page->mixAttribute ];
		}
		// d_iag_printf( "map copied - atr:%d\n", target_page->mixAttribute );
	}	

	else if ( keyNdx == KEY_PASTE ){
		
		// The semantics of the buffered values
		if ( ATTR_MAP_COPY_BUFFER[10] == target_page->mixAttribute ){

			// COPIES the buffer attributes to the map
			for ( i=0; i < MATRIX_NROF_ROWS; i++ ){

				target_page->Track[i]->attribute[ target_page->mixAttribute ] = 	
					ATTR_MAP_COPY_BUFFER[i];
			}

			// Mark the buffer empty
			ATTR_MAP_COPY_BUFFER[10] = EMPTY;
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


	// Void the REC pattern hence disabling recording.
	if ( keyNdx == KEY_RECORD ){

		target_page->REC_bit ^= ON;
	}



