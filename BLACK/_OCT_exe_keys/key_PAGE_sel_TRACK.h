// 
// The case where the page contains an active track selection
//

	// CHAINER
	if ( keyNdx == KEY_CHAINER ){

		// Chain the currently selected tracks in the VIEWER_page
		chain_selected_tracks( VIEWER_page );
	}


	// ZOOM_STEP -> Tap in some notes ( steps ) 
	//
	// This is the TAP KEY, provided a track is selected
	
	if (	( keyNdx == KEY_ZOOM_STEP )
		&&	( VIEWER_page->trackSelection != 0 )
		&& 	( RUNBIT == ON )
		&& 	( is_selected_in_GRID( VIEWER_page ) )
		){
		
		// Tap a step into the target page
		tap_step( VIEWER_page );
	
	} // TAP KEY



	// TRACK "zoom" if single selection	
	if (keyNdx == KEY_ZOOM_TRACK) {

		for (i=0; i<10; i++) {

			// Single track selection condition
			if (  ((VIEWER_page->trackSelection & (1<<i)) != 0) &&
			      ((VIEWER_page->trackSelection ^ (1<<i)) == 0)    ) {

				// Switch to mode THIS_TRACK
				VIEWER_page->keyMode = KMOD_THIS_TRACK;

				// Switch zoomlevel
				zoom_level = zoomTRACK;

				break;
			}
		}
	}
	



	//
	// TRACK_MUTATORS
	//
	if ((keyNdx >= 187) && (keyNdx <= 196)) {

		// Call track entity FUNCTION on track selection
		switch (keyNdx) {
				
			case TGGL_KEY:
			
				for (row = 0; row < MATRIX_NROF_ROWS; row++) {
					// Check track_selection if function applies to this row
					if ( VIEWER_page->trackSelection & (1 << row)) {										
						// TGGL function: toggle mute bit on selected tracks
						VIEWER_page->trackMutepattern ^= (1<<row);									 
					}
				}
				// Clear track Selection
				VIEWER_page->trackSelection = 0;
				break;
							
																						
			case SOLO_KEY:		
				// Mark the soloed tracks in the Solopattern
				VIEWER_page->trackSolopattern ^= VIEWER_page->trackSelection;
				VIEWER_page->trackSelection = 0;
				break;
			
			
			case CLEAR_KEY: 
				Page_CLEAR_selected_tracks( VIEWER_page );					
				break;
		
		
			case RANDOMIZE_KEY:
				// Randomize the Toggle state of the selected tracks
				Page_RNDMZ_selected_tracks( VIEWER_page );
				break;
		
		
			case FLT_KEY:
				// Flatten the track content. See function for details.
				if ( my_bit_cardinality( VIEWER_page->trackSelection ) > 1 ){
	
					Page_FLT_selected_tracks( VIEWER_page );
				}
				break;
				
			
			case RMX_KEY:
				// Remix track contents. See function for details.
				Page_RMX_selected_tracks( VIEWER_page );
				break;
				
			
			case EFF_KEY:
		
				// Calculate the row of the selected track
				// row = my_bit2ndx( VIEWER_page->trackSelection );

				// D O U B L E - C L I C K  C O N S T R U C T
				// DOUBLE CLICK SCENARIO
				if ((DOUBLE_CLICK_TARGET == keyNdx) 
						&& (DOUBLE_CLICK_TIMER > DOUBLE_CLICK_ALARM_SENSITIVITY)) {
			
					// Double click code
					// ...
					// Set the status of selected tracks to RECEIVE
					// Track_set_MISC( VIEWER_page->Track[row], EFF_BIT, RECEIVE );
					
					for (row = 0; row < MATRIX_NROF_ROWS; row++) {
						// Check track_selection if function applies to this row
						if ( VIEWER_page->trackSelection & (1 << row)) {										
							// TGGL function: toggle mute bit on selected tracks
							Track_set_MISC( VIEWER_page->Track[row], EFF_BIT, RECEIVE );
						}
					} // row iterator					
				
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
					// Cycle through the EFF states: OFF, SEND, remove the RECEIVE bit
					for (row = 0; row < MATRIX_NROF_ROWS; row++) {
						if ( VIEWER_page->trackSelection & (1 << row)) {										

							switch( Track_get_MISC( VIEWER_page->Track[row], EFF_BIT ) ){
		
								case OFF:
									Track_set_MISC( VIEWER_page->Track[row], EFF_BIT, SEND );
									break;
									
								case SEND:
								case RECEIVE:
									Track_set_MISC( VIEWER_page->Track[row], EFF_BIT, OFF );
									break;

							} // EFF status switch
						} // row is selected
					} // row iterator

				} // Single click scenario

				break;
			
			
			
			case ZOOM_KEY:
			
				// Iterate through rows..
				for (row=0; row<MATRIX_NROF_ROWS; row++) {
		
					// ..and check condition for the TRACK->ZOOM operation
					if ((VIEWER_page->trackSelection ^ (1<<row)) == 0) {
		
						// Switch to mode THIS_TRACK
						VIEWER_page->keyMode = KMOD_THIS_TRACK;
		
						// Switch zoom level
						zoom_level = zoomTRACK;
					}
				}				
				break;
			
			
		
			case COPY_KEY:
		
				// D O U B L E - C L I C K
				// DOUBLE CLICK SCENARIO
				if ((DOUBLE_CLICK_TARGET == keyNdx) 
						&& (DOUBLE_CLICK_TIMER > DOUBLE_CLICK_ALARM_SENSITIVITY)) {
		
					// DOUBLE CLICK CODE
					// ... 
					// Move copy buffer into next empty page of bank 
					// Ensure that buffer is not empty
					if (TRACK_COPY_BUFFER != 0) {
		
						// Find the next empty page in bank of VIEWER_page and switch
						j = VIEWER_page->pageNdx % 10;
		
						for ( i = j; i < (j + 150); i += 10 ){
		
							if ( i > MAX_NROF_PAGES ) {
								break;
							}
		
							if (Page_repository[i].page_clear == ON) {
		
								// Switch viewer to the found empty page
								VIEWER_page = &Page_repository[i];
								break;
							}
						}
		
						// Break if no empty page found in bank
						if (i == j+150) {
							break;
						}
																				
						// Get the source page index
						m = (TRACK_COPY_BUFFER >> 10);
			
						// Leave only the track bitpattern (first 10 bits) in copy buffer
						TRACK_COPY_BUFFER &= 0x3FF;
		
						// Get the initial cardinality of track buffer
						k = my_bit_cardinality( TRACK_COPY_BUFFER );
			
						// Create artificial track selection in target page
						VIEWER_page->trackSelection = ( 0x3FF >> (10 - k) );
						
						// d_iag_printf( "TCB initial:%d cardinality:%d\n", TRACK_COPY_BUFFER, k );
						// my_print_bits( TRACK_COPY_BUFFER );
						
						// Paste the buffer tracks into the artificial selection
						for (i=0; i<k; i++) {
		
							// d_iag_printf( "track_copy:%d\n", i );
		
							// Copy first buffer track into first target track
							Track_copy( &Page_repository[m], 	
										my_bit2ndx( TRACK_COPY_BUFFER ),
										VIEWER_page, 
										i );
		
							// Clear the bit of the track just copied
							TRACK_COPY_BUFFER ^= (1 << my_bit2ndx( TRACK_COPY_BUFFER ));
		
							// d_iag_printf( "     TCB new:%d\n", TRACK_COPY_BUFFER );									
							// my_print_bits( TRACK_COPY_BUFFER );
		
						}
		
						// Mark the target page as modified - just in case it was fresh
						VIEWER_page->page_clear = OFF;	
						
						// Unmark the track selection in new VIEWER page
						VIEWER_page->trackSelection = OFF;
				
						// Stop playing the page and reset its track locators
						stop_playing_page( &Page_repository[m], TTC_ABS_VALUE );
						set_track_locators( &Page_repository[m], 0, 0 );
		
						// Remove source page from GRID_selection
						grid_select( &Page_repository[m], OFF );
						
						// And put the target page into the preselection
						grid_select( VIEWER_page, ON );
		
						// GRID parameter update
						GRID_CURSOR = VIEWER_page->pageNdx;
						GRID_p_selection[ j%10 ] = VIEWER_page;
		
					} // if (TRACK_COPY_BUFFER != 0)			
				} // DOUBLE CLICK SCENARIO
						
		
				// SINGLE CLICK SCENARIO
				else if (DOUBLE_CLICK_TARGET == 0) {
						DOUBLE_CLICK_TARGET = keyNdx;
						DOUBLE_CLICK_TIMER = ON;
						// Start the Double click Alarm
						cyg_alarm_initialize(	
								doubleClickAlarm_hdl, 
								cyg_current_time() + DOUBLE_CLICK_ALARM_TIME,
								DOUBLE_CLICK_ALARM_TIME );
		
					// SINGLE CLICK CODE
					// ...
					// Remember the selected tracks in terms of page and row					
		
					TRACK_COPY_BUFFER = 
						( VIEWER_page->trackSelection | (VIEWER_page->pageNdx << 10) );
		
					// Reset the trackselection in source page
					// VIEWER_page->trackSelection = 0;
				}
		
				break;
			
			
		
			case PASTE_KEY:
			
				// Don't continue if there is no valid data
				if ( TRACK_COPY_BUFFER == 0){

					break;
				}
			
				// Get the bit cardinality of track selection in copy buffer
				k = my_bit_cardinality( TRACK_COPY_BUFFER & 0x3FF );
				
				// Get the source page index
				j = TRACK_COPY_BUFFER >> 10;
				
				// Act according to cardinality
				switch (k) {
		
					case 0: 
						// Copy buffer is empty - do nothing
						break;																
		
					default:
						// Execute the multiple copy operation
						// Check cardinality of current track selection
						if (my_bit_cardinality( VIEWER_page->trackSelection ) == k) {
		
							// Copy buffer into track selection
							for (i=0; i < k; i++) {
		
								// Copy first buffer track into first target track
								Track_copy( &Page_repository[j], 	my_bit2ndx( TRACK_COPY_BUFFER & 0x3FF ),
											VIEWER_page, 			my_bit2ndx( VIEWER_page->trackSelection ) );
		
								// Clear the bits of the processed tracks in the TRACK_COPY_BUFFER
								TRACK_COPY_BUFFER ^= (1 << my_bit2ndx( TRACK_COPY_BUFFER & 0x3FF ));
								VIEWER_page->trackSelection ^= (1 << my_bit2ndx( VIEWER_page->trackSelection ));	
							}
						}
						
						// Mark the target page as modified - in case it was fresh
						VIEWER_page->page_clear = OFF;
		
						break;
				} // switch (k) 
				
		
				// Return to the "normal" status
				VIEWER_page->trackSelection = 0;
		
				TRACK_COPY_BUFFER = 0;

				break;		
		} // switch (keyNdx)
		
	} // Key is a track mutator	





	// ALIGN LAUFLICHT 
	if ( keyNdx == KEY_ALIGN ){
		
		// Retrigger the tracks in Track selection			
		for ( i=0; i<MATRIX_NROF_ROWS; i++ ){

			// Act only on the selected tracks, if playing
			if (	( VIEWER_page->trackSelection & (1<<i) )
				&&	( VIEWER_page->Track[i]->attribute[ATTR_LOCATOR] != 0 )
				){

				VIEWER_page->Track[i]->trigger_set_ndx = 0;
				VIEWER_page->Track[i]->TTC = 0;
			}
		}								
	}



	// PROGRAM CHANGE command
	if ( keyNdx == KEY_PROGRAM ){

		// Only act if there is just one track selected
		if ( my_bit_cardinality( VIEWER_page->trackSelection ) == 1 ){

			// Send the program change Value on the selected track(s)
			for (i=0; i<MATRIX_NROF_ROWS; i++) {
	
				if (	( (VIEWER_page->trackSelection & (1<<i) ) != 0 )
					&&	( VIEWER_page->Track[ my_bit2ndx( VIEWER_page->trackSelection )]->program_change > 0 )
					){
	
					MIDI_send( 	MIDI_PGMCH, 
								VIEWER_page->Track[i]->attribute[ATTR_MIDICH], 
								VIEWER_page->Track[ my_bit2ndx( VIEWER_page->trackSelection )]->program_change - 1,
								0 );
				}
			}
		} // only one track is selected
	}
		
		
	//
	// MIX MASTER
	//
	if (keyNdx == KEY_MIX_MASTER) {
		
		// D O U B L E - C L I C K   C O D E
		if (DOUBLE_CLICK_TARGET == keyNdx) {
			// Nothing to do
		}

		// Assign a new attribute to the MIX block - similar to MIX2EDIT
		else if (DOUBLE_CLICK_TARGET == 0) {

			DOUBLE_CLICK_TARGET = keyNdx;
			DOUBLE_CLICK_TIMER = ON;								
			// Start the Double click Alarm
			cyg_alarm_initialize(	
				doubleClickAlarm_hdl, 
				cyg_current_time() + DOUBLE_CLICK_ALARM_TIME,
				DOUBLE_CLICK_ALARM_TIME );

			// SINGLE CLICK CODE
			// React only when a single track is selected
			if ( my_bit_cardinality( VIEWER_page->trackSelection ) == 1 ){

				// Assign the new mix Attribute
				VIEWER_page->mixAttribute = 1 + my_bit2ndx( VIEWER_page->trackSelection );
				// d_iag_printf( "mix2edit:%d\n", VIEWER_page->attr_mix2edit );
			} // only one track is selected

			VIEWER_page->trackSelection = 0;

		} // Single click code
	} // keyNdx == MIX_MASTER
	
		

	//
	// EDIT MASTER
	//
	if (keyNdx == KEY_EDIT_MASTER) {
		
		// D O U B L E - C L I C K   C O D E
		if (DOUBLE_CLICK_TARGET == keyNdx) {
			// Nothing to do
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

			// SINGLE CLICK CODE
			// React only when a single track is selected
			if ( my_bit_cardinality( VIEWER_page->trackSelection ) == 1 ){

				// Switch the edit mode to CONTROL;  0:EDIT  1:CONTROL
				VIEWER_page->editorMode = MIX2EDIT;

				// Assign the attr_mix2edit values as needed; 
				// Compute the index first:
				VIEWER_page->attr_mix2edit = 1 + my_bit2ndx( VIEWER_page->trackSelection );
				
				// Assign the min and max values
				switch( VIEWER_page->attr_mix2edit ){

					case ATTR_VELOCITY:
						VIEWER_page->attr_mix2edit_MIN = TRACK_MIN_VELOCITY;
						VIEWER_page->attr_mix2edit_MAX = TRACK_MAX_VELOCITY;
						break;

					case ATTR_PITCH:
						VIEWER_page->attr_mix2edit_MIN = TRACK_MIN_PITCH;
						VIEWER_page->attr_mix2edit_MAX = TRACK_MAX_PITCH;
						break;

					case ATTR_LENGTH:
						VIEWER_page->attr_mix2edit_MIN = TRACK_MIN_LENFACTOR;
						VIEWER_page->attr_mix2edit_MAX = TRACK_MAX_LENFACTOR;
						break;

					case ATTR_START:
						VIEWER_page->attr_mix2edit_MIN = TRACK_MIN_STAFACTOR;
						VIEWER_page->attr_mix2edit_MAX = TRACK_MAX_STAFACTOR;
						break;

					case ATTR_POSITION:
						VIEWER_page->attr_mix2edit_MIN = TRACK_MIN_POSITION;
						VIEWER_page->attr_mix2edit_MAX = TRACK_MAX_POSITION;
						break;

					case ATTR_DIRECTION:
						VIEWER_page->attr_mix2edit_MIN = TRACK_MIN_DIRECTION;
						VIEWER_page->attr_mix2edit_MAX = TRACK_MAX_DIRECTION;
						break;

					case ATTR_AMOUNT:
						VIEWER_page->attr_mix2edit_MIN = TRACK_MIN_AMOUNT;
						VIEWER_page->attr_mix2edit_MAX = TRACK_MAX_AMOUNT;
						break;

					case ATTR_GROOVE:
						VIEWER_page->attr_mix2edit_MIN = TRACK_MIN_GROOVE;
						VIEWER_page->attr_mix2edit_MAX = TRACK_MAX_GROOVE;
						break;

					case ATTR_MIDICC:
						VIEWER_page->attr_mix2edit_MIN = TRACK_MIN_MIDICC;
						VIEWER_page->attr_mix2edit_MAX = TRACK_MAX_MIDICC;
						break;

					case ATTR_MIDICH:
						VIEWER_page->attr_mix2edit_MIN = TRACK_MIN_MIDICH;
						VIEWER_page->attr_mix2edit_MAX = TRACK_MAX_MIDICH;
						break;
				} // switch on attr_mix2edit to get the min and max values

				// d_iag_printf( "mix2edit:%d\n", VIEWER_page->attr_mix2edit );

			} // only one track is selected
		} // Single click code
	} // keyNdx == EDIT_MASTER



	// 
	// OCTAVE CIRCLE
	//
	// Enable track trans-positions with one click and directly
	#include "key_OCT_CIRCLE_xpose_TRACK.h"	

	

	//
	// TRANSPORT functions - when nothing is selected in page
	//
	switch( keyNdx ){

		case KEY_STOP:
			sequencer_command_STOP();
			break;


		case KEY_PAUSE:
			// PAUSE SELECTED TRACK(S)
		
			// Determine the chain divisor
			switch( VIEWER_page->chainMode ){
				
				case CHAINMODE_1:	j = 1;		break;
				case CHAINMODE_2:	j = 2;		break;
				case CHAINMODE_3:	j = 4;		break;
				case CHAINMODE_4:	j = 8;		break;
			}
	
			// PAUSE the tracks in Track selection			
			for ( i=0; i<MATRIX_NROF_ROWS; i++ ){
	
				// Act only on the selected tracks that are playing
				if (	(	( VIEWER_page->trackSelection & (1<<i) )
						&&	( VIEWER_page->Track[i]->attribute[ATTR_LOCATOR] != 0 ) )
					||	( VIEWER_page->Track[i]->attribute[ATTR_TEMPOMUL] == 0 )
					){
	
					// Prepare the TTC to be wrapped
					VIEWER_page->Track[i]->TTC = 12;
	
					// Parameters are: page, row, chain divisor
					advance_track_TTC_new( 	VIEWER_page, VIEWER_page->Track[i] );
	 
					// Now play step in track at new location: page, row, column
					// Check if the step is set first..
					if ( Step_get_status( 
							VIEWER_page->Step[i][VIEWER_page->Track[i]->attribute[ATTR_LOCATOR]-1],
							STEPSTAT_TOGGLE == ON )
						){
						
						VIEWER_page->Track[i]->attribute[ATTR_TEMPOMUL] = 1;
	
						PLAYER_preview_step(
							VIEWER_page, i, VIEWER_page->Track[i]->attribute[ATTR_LOCATOR]-1 );
					}
					
					// Finally pause the track: tempomul = 0
					VIEWER_page->Track[i]->attribute[ATTR_TEMPOMUL] = 0;				
				}
			}
			break;


		case KEY_PLAY1:
		case KEY_PLAY2:
		case KEY_PLAY4:

			// Depending on what you are holding down: start play or set tempo
			if ( is_pressed_key( KEY_SELECT_MASTER ) == TRUE ){
				
				// Set the tempo of the selected tracks
#include "key_TRACK_multiplier.h"								
			}
			else{
				
				// Start playing the sequencer
				sequencer_command_PLAY();
	
				// UN-PAUSE the tracks in Track selection			
				for ( i=0; i<MATRIX_NROF_ROWS; i++ ){
	
					// Finally pause the track: tempomul = 0
					if ( VIEWER_page->Track[i]->attribute[ATTR_TEMPOMUL] == 0 ){
						VIEWER_page->Track[i]->attribute[ATTR_TEMPOMUL] = 1;
					}
				}
			}
			break;


		case KEY_RECORD:
			// Make sure only one track is selected - to operate on
			if ( my_bit_cardinality( VIEWER_page->trackSelection ) == 1 ){
	
				// Only one track can be recorded into at a time - so erase the initial REC pattern
				VIEWER_page->track_REC_pattern = 0;
	
				// Toggle the REC pattern - better because it is faster to use.
				VIEWER_page->track_REC_pattern ^= (1 << (my_bit2ndx( VIEWER_page->trackSelection )));

				// Mark page as un_clear
				VIEWER_page->page_clear = OFF;
			}
			break;
	}	



	// 
	// CHORD SELECTOR
	//
	// Selects the chord cardinality (page or track??)
	if( (keyNdx > 251) 
		){
			
		// Pretty ghetto: this is the target value of the chord bit		
		j = 7 - (keyNdx - 252);	

//		if ( my_bit_cardinality( VIEWER_page->trackSelection ) == 1 ){			

		// Modify all selected tracks
		for( i=0; i < MATRIX_NROF_ROWS; i++ ){
	
			if ((VIEWER_page->trackSelection & (1<<i)) != 0){
	
// Disabled the track chord for now		
				Track_set_MISC( VIEWER_page->Track[i], CHORD_BIT, j );
			} // track is selected
		} // row iterator

	} // Chord selector key pressed



