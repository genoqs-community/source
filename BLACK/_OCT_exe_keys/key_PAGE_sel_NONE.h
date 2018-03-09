

	//
	// MIX MASTER
	//
	if (keyNdx == KEY_MIX_MASTER) {
		//		// Only react when no steps are selected
		//		if (VIEWER_page->stepSelection != 0){
		//			break;
		//		}
		//
		//		// Toggle the Status of the mix master
		//		VIEWER_page->mixMasterStatus ^= 1;	
	}



	// 
	// MIX TARGET
	//

	// Toggle the Map0 status
	if ( (keyNdx == KEY_MIXTGT_USR0) ){

		if (	( VIEWER_page->mixTarget == MIXTGT_USR1 )
			||	( VIEWER_page->mixTarget == MIXTGT_USR2 )
			||	( VIEWER_page->mixTarget == MIXTGT_USR3 )
			||	( VIEWER_page->mixTarget == MIXTGT_USR4 )
			||	( VIEWER_page->mixTarget == MIXTGT_USR5 )
			){	
		
			// Toggle status only once a map is selected
			GRID_MAP0_status ^= 1;
		}
		else{
			VIEWER_page->mixTarget = MIXTGT_USR1;
		}
	}

	if ( 	( (keyNdx == KEY_MIXTGT_ATR ) )
		||	( (keyNdx == KEY_MIXTGT_VOL ) )
		||	( (keyNdx == KEY_MIXTGT_PAN ) )
		||	( (keyNdx == KEY_MIXTGT_MOD ) )
		||	( (keyNdx == KEY_MIXTGT_EXP ) )
		||	( (keyNdx == KEY_MIXTGT_USR1) )
		||	( (keyNdx == KEY_MIXTGT_USR2) ) 
		||	( (keyNdx == KEY_MIXTGT_USR3) )
		||	( (keyNdx == KEY_MIXTGT_USR4) )
		||	( (keyNdx == KEY_MIXTGT_USR5) ) 
		){
		
		// D O U B L E - C L I C K  C O N S T R U C T
		// DOUBLE CLICK SCENARIO
		if ((DOUBLE_CLICK_TARGET == keyNdx) 
				&& (DOUBLE_CLICK_TIMER > DOUBLE_CLICK_ALARM_SENSITIVITY)) {
	
			// Double click code
			// ...
			// Zoom into the clicked mix target

			// Select MIXER MAP MODE
			VIEWER_page->keyMode = KMOD_MIXER_MAP;

		} // end of double click scenario 
		
	
		// SINGLE CLICK SCENARIO
		else if (DOUBLE_CLICK_TARGET == 0) {
			
				DOUBLE_CLICK_TARGET = keyNdx;
				DOUBLE_CLICK_TIMER = ON;
				// Start the Double click Alarm
				cyg_alarm_initialize(	
						doubleClickAlarm_hdl, 
						cyg_current_time() + DOUBLE_CLICK_ALARM_TIME,
						DOUBLE_CLICK_ALARM_TIME );
	
			// Single click code
			// ...				
			// Just simply switch to the chosen mix target.
			switch( keyNdx ){

				case KEY_MIXTGT_ATR: 	VIEWER_page->mixTarget = MIXTGT_ATR; 	break;
				case KEY_MIXTGT_VOL: 	VIEWER_page->mixTarget = MIXTGT_VOL; 	break;
				case KEY_MIXTGT_PAN: 	VIEWER_page->mixTarget = MIXTGT_PAN; 	break;
				case KEY_MIXTGT_MOD: 	VIEWER_page->mixTarget = MIXTGT_MOD; 	break;
				case KEY_MIXTGT_EXP: 	VIEWER_page->mixTarget = MIXTGT_EXP; 	break;
				case KEY_MIXTGT_USR1: 	VIEWER_page->mixTarget = MIXTGT_USR1; 	break;
				case KEY_MIXTGT_USR2: 	VIEWER_page->mixTarget = MIXTGT_USR2; 	break;
				case KEY_MIXTGT_USR3: 	VIEWER_page->mixTarget = MIXTGT_USR3; 	break;
				case KEY_MIXTGT_USR4: 	VIEWER_page->mixTarget = MIXTGT_USR4; 	break;
				case KEY_MIXTGT_USR5: 	VIEWER_page->mixTarget = MIXTGT_USR5; 	break;		
			}
		} // Single click scenario
	} // MIX TARGET KEY pressed




	// FOLLOW_PAGE
	if (	( keyNdx == KEY_FOLLOW ) 
		&&	( VIEWER_page->trackSelection == 0)
		&&	( PLAY_MODE_STATUS == OFF )
		){

		// Toggle the FOLLOW_PAGE switch
		if ( follow_flag == FOLLOW_PAGE ){
			
			follow_flag = OFF;
		}
		else{

			follow_flag = FOLLOW_PAGE;
	
			// Point the VIEWER_page to the page currently playing in its row
			i = VIEWER_page->pageNdx % 10;

			// If there is a page playing in that row, point VIEWER_page to it
			if ( GRID_p_selection[i] != NULL ){

				VIEWER_page = GRID_p_selection[i];
				VIEWER_page->keyMode = KMOD_PAGE;
			}
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
			VIEWER_page->editorMode = CONTROL;
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
			switch( VIEWER_page->editorMode ){

				case CONTROL:
					VIEWER_page->editorMode = EDIT;
					break;
					
				case EDIT:
					VIEWER_page->editorMode = PREVIEW;
					break;

				case PREVIEW:
					VIEWER_page->editorMode = EDIT;
					break;

				case MIX2EDIT:
					VIEWER_page->editorMode = EDIT;
					break;
			}
		}				
	}




	//
	// MUTE MASTER
	//
	if (keyNdx == KEY_MUTE_MASTER) {
		// Only active if no track is selected
		if (VIEWER_page->trackSelection != 0){
			break;
		}

		// Clear the solopattern if it exist
		if (VIEWER_page->trackSolopattern != 0) {
			VIEWER_page->trackSolopattern = 0;
		}
		else {
			// Then clear the solopattern
			if (VIEWER_page->trackMutepattern != 0) {
				VIEWER_page->trackMutepattern = 0;
			}
			else {						
				// Then fill the mutepattern from store
				VIEWER_page->trackMutepattern = 
					VIEWER_page->trackMutepatternStored;
			}
		}

		// D O U B L E - C L I C K
		if ((DOUBLE_CLICK_TARGET == keyNdx) 
				&& (DOUBLE_CLICK_TIMER > DOUBLE_CLICK_ALARM_SENSITIVITY)) {
			// This is a double click victim - Mute all tracks
			VIEWER_page->trackMutepattern = 0x3ff;
			VIEWER_page->trackMutepatternStored = VIEWER_page->trackMutepattern;							
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
		}
	}





	//
	// TRACK_MUTATORS
	//
	if ((keyNdx >= 187) && (keyNdx <= 196)) {

		// No track or step is selected. Page is selection-blank.
		if (   (VIEWER_page->trackSelection == 0) 
			&& (VIEWER_page->stepSelection  == 0)  ){


			// If the button pressed falls within the solo pattern, disable its solo status..
			// ..considering the chaimode settings
			if ( ((1<<(keyNdx-187)) & VIEWER_page->trackSolopattern) != 0 ){

				// Loop through the chain of the selected track and mute all chain tracks
				// Start with the pressed track
				current_track = VIEWER_page->Track[ keyNdx-187 ];
				temp = cardinality_of_chain( current_track );
				for ( i=0; i < temp; i++ ){
					
					VIEWER_page->trackSolopattern ^= ( 1 << row_of_track( VIEWER_page, current_track ));
					current_track = current_track->chain_data[NEXT];
				}

				// Need to break here, otherwise the key press gets executed further
				break;
			}



			// If there is no active mutepattern, use fresh store variable
			if (VIEWER_page->trackMutepattern == 0) {

				VIEWER_page->trackMutepatternStored = 0;
			}
									


			// MUTE operation depending on the chainmodes - was yesterday.
			// Now check the chain status: mute all tracks in the same chain (new model)
	
			// Loop through the chain of the selected track and mute all chain tracks
			// Start with the pressed track. Disaböled on Marcel' groove request
			current_track = VIEWER_page->Track[ keyNdx-187 ];
//			temp = cardinality_of_chain( current_track );
//			for ( i=0; i < temp; i++ ){
				
				VIEWER_page->trackMutepattern ^= ( 1 << row_of_track( VIEWER_page, current_track ));
//				current_track = current_track->chain_data[NEXT];
//			}
									

			// Update the stored variable
			VIEWER_page->trackMutepatternStored = VIEWER_page->trackMutepattern;


			// D O U B L E - C L I C K
			if ((DOUBLE_CLICK_TARGET == keyNdx) 
					&& (DOUBLE_CLICK_TIMER > DOUBLE_CLICK_ALARM_SENSITIVITY)
				){

				// This is a double click victim, so add track to Solopattern,
				// ..considering the chain mode settings.
				if ( ((1<<(keyNdx-187)) & VIEWER_page->trackMutepattern) == 0 ) { 
					
					// Loop through the chain of the selected track and mute all chain tracks
					// Start with the pressed track
					current_track = VIEWER_page->Track[ keyNdx-187 ];
					temp = cardinality_of_chain( current_track );
					for ( i=0; i < temp; i++ ){
						
						VIEWER_page->trackSolopattern ^= ( 1 << row_of_track( VIEWER_page, current_track ));
						current_track = current_track->chain_data[NEXT];
					}	
					
				} // double click key was not in mutepattern
			}// Double click victim.

			// Step_activity is ON: turn it OFF or SELECT it
			else if (DOUBLE_CLICK_TARGET == 0) {

					 DOUBLE_CLICK_TARGET = keyNdx;
					 DOUBLE_CLICK_TIMER = ON;
					// Start the Double click Alarm
					cyg_alarm_initialize(	
							doubleClickAlarm_hdl, 
							cyg_current_time() + DOUBLE_CLICK_ALARM_TIME,
							DOUBLE_CLICK_ALARM_TIME );
			}
			
		} // No track or step is selected		
		
	} // Key is a track mutator	



	// 
	// TRACK CHAIN SELECTORS
	//
	key_exe_chainselectors( keyNdx );	



	//
	// OCTAVE CIRCLE
	//
	// This should only operate when in scale mode already
	// ..or when trying to enter scale mode.
	if ( 	( VIEWER_page->scaleStatus != OFF ) 
		||	( VIEWER_page->force_to_scale == ON )
		||	( keyNdx == KEY_SCALE_SEL )
		||	( keyNdx == KEY_SCALE_MOD )
		||	( keyNdx == KEY_SCALE_CAD ) 	
		){
		
		key_ScaleSelector_functions( keyNdx, VIEWER_page );
	}


	if ( VIEWER_page->scaleStatus == OFF ){

		// This is the mod operation in Page mode
		key_ScaleSelector_mini_functions( keyNdx, VIEWER_page );
	}


	// Toggle the SCL_align flag in the page
	if (	( keyNdx == KEY_SCALE_CAD )
		&&	( VIEWER_page->force_to_scale == ON )
		){

		VIEWER_page->SCL_align ^= ON;
		// d_iag_printf( "SCL_align:%d\n", VIEWER_page->SCL_align );
	}



	// 
	// CLOCK SOURCE
	// 
	if ( keyNdx == KEY_CLOCK ) {

		// Do not listen to it when sequencer is running, DANGER!!
		// However allow it to operate when we are slave - to prevent lock..
		// ..in case the master suddenly disappears before sending a Stop command
		if ( 	( ( RUNBIT == OFF ) || ( CLOCK_SOURCE == EXT ) )
			&& 	( VIEWER_page->trackSelection == 0)
			){

			// D O U B L E - C L I C K  C O N S T R U C T
			// DOUBLE CLICK SCENARIO
			if ((DOUBLE_CLICK_TARGET == keyNdx) 
					&& (DOUBLE_CLICK_TIMER > DOUBLE_CLICK_ALARM_SENSITIVITY)) {
		
				// Double click code
				// ...
				// Toggle the clock source between INT and EXT
				switch ( CLOCK_SOURCE )	{
	
					case OFF:
					case INT:
						// Stop the sequencer properly
						sequencer_STOP();
						// Switch the clock source
						CLOCK_SOURCE = EXT;
						// Make sure the CLOCK LED is lit
						GLOBALBLINKER = ON;
						break;
		
					case EXT:
						// Switch the clock source
						CLOCK_SOURCE = OFF;
						break;

				} // switch (CLOCK_SOURCE)
				
				// Coming back from losing the master before having seen a Stop
				if ( RUNBIT == ON ){

					// Stop the sequencer properly
					sequencer_STOP();
					// Switch the clock source
					CLOCK_SOURCE = OFF;
					// Make sure the CLOCK LED is lit
					GLOBALBLINKER = ON;
				}
			} // end of double click scenario 
			
		
			// SINGLE CLICK SCENARIO
			else if (DOUBLE_CLICK_TARGET == 0) {
				
				DOUBLE_CLICK_TARGET = keyNdx;
				DOUBLE_CLICK_TIMER = ON;
				// Start the Double click Alarm
				cyg_alarm_initialize(	
						doubleClickAlarm_hdl, 
						cyg_current_time() + DOUBLE_CLICK_ALARM_TIME,
						DOUBLE_CLICK_ALARM_TIME );
	
				// Single click code
				// ...
				
				// Toggle the clock source
				switch ( CLOCK_SOURCE )	{
	
					case INT:
						CLOCK_SOURCE = OFF;
						break;
				
					case OFF:
						CLOCK_SOURCE = INT;
						break;
				} // switch (CLOCK_SOURCE)
			} //end of single click scenario

		} // RUNBIT == OFF

	} // KEY_CLOCK was pressed



	// ALIGN LAUFLICHT 
	if (	( keyNdx == KEY_ALIGN )
		){
		
		// Align page to global locator 

		// Act like this only when we are not dealing with scales.
		// and no track is selected
		if ( 	(VIEWER_page->scaleStatus == OFF)
			&& 	(VIEWER_page->trackSelection == 0) 
			){

			// Aligns only the VIEWER_page to the global play data
			set_page_locators( VIEWER_page, GLOBAL_LOCATOR, TTC_ABS_VALUE );
		}

	} // KEY_ALIGN was pressed



	// PLAY MODE
	if ( keyNdx == KEY_ZOOM_PLAY ){

		// Toggle the play mode status
		PLAY_MODE_switch( PLAY_MODE_STATUS ^ 1 );
	}


	// Leaves the PLAY changes permanent in the page
	if ( keyNdx == KEY_PROGRAM ){
		if ( PLAY_MODE_STATUS == ON ){
			PLAY_MODE_STATUS = OFF;
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
			
			// Clear the recording track on PLAY1 while recording..
			if (	( keyNdx == KEY_PLAY1 )
				&&	( RUNBIT == ON )
				){

				// Find and clear the recording track in VIEWER_page
				if ( VIEWER_page->track_REC_pattern != 0 ){
					
					// Add all tracks in recording chain to an artificial track selection
					temp = my_bit2ndx( VIEWER_page->track_REC_pattern );					

					// Starting track is the currently recording one
					current_track = VIEWER_page->Track[temp];

					// Iterate through the chain and place all tracks in artificial selection
					for ( i = temp; i < MATRIX_NROF_ROWS; i++ ){
					
						// Place current track into the selection
						VIEWER_page->trackSelection |= 
							( 1 << row_of_track( VIEWER_page, current_track ) );

						// Move the track pointer along the chain
						current_track = current_track->chain_data[NEXT];
					}

					// Clear the achieved track selection
					Page_CLEAR_selected_tracks( VIEWER_page );

					// Forget the artificial track selection selection
					VIEWER_page->trackSelection = 0;
				}
			}

			// Make sure the sequencer is running
			sequencer_command_PLAY();

			break;

	}	


