

	switch (content) {


		// ELE_MATRIX..
		case STRUM_VALS:

			// Show values depending on the OCT_strum_view_level
			switch( OCT_strum_attribute ){

				case ATTR_VELOCITY:
					for ( i=0; i < MATRIX_NROF_ROWS; i++ ){
						MIR_write_numeric_H(
							OCT_step_strum_VEL [OCT_strum_view_level][9-i] , i);		
					}
					break;
				case ATTR_PITCH:
					for ( i=0; i < MATRIX_NROF_ROWS; i++ ){
						MIR_write_numeric_H(
							OCT_step_strum_PIT [OCT_strum_view_level][9-i] , i);		
					}
					break;
				case ATTR_LENGTH:
					for ( i=0; i < MATRIX_NROF_ROWS; i++ ){
						MIR_write_numeric_H(
							OCT_step_strum_LEN [OCT_strum_view_level][9-i] , i);		
					}
					break;
				case ATTR_START:
					for ( i=0; i < MATRIX_NROF_ROWS; i++ ){
						MIR_write_numeric_H(
							OCT_step_strum_STA [OCT_strum_view_level][9-i] , i);		
					}
					break;
			}
			break;


		// ELE_MATRIX..
		case PAGES:
			// Show the selected page as orange dot blinking
			MIR_write_dot (dot_of_page(target_page->pageNdx), MIR_RED);
			MIR_write_dot (dot_of_page(target_page->pageNdx), MIR_GREEN);
			MIR_write_dot (dot_of_page(target_page->pageNdx), MIR_BLINK);

			// Show all non-empty pages in red (they are not playing)
			for (i=0; i<MAX_NROF_PAGES; i++) {
				if (Page_repository[i].page_clear == OFF) {
					MIR_write_dot (dot_of_page(i), MIR_RED);
				}
			}

			// Show a playing row as blink orange
			break;
		
		
		
		// ELE MATRIX..
		case GRIDTRACK:
					
			// GRID SETS: Show the non-empty GRID sets by red lights.
			for ( i=0; i < GRID_NROF_SETS; i++ ){

				// Check the respective GRID set for content and currency
				if ( 	( get_content_GRID_set( i ) )
					&&	( current_GRID_set != i )
					){
					
					GRID_write_dot( 9 + i*10, MIR_RED );
				}
			}

			GRID_write_dot( 9 + current_GRID_set*10, MIR_GREEN 	);

			// Check if changes have been made to set
			if ( !is_actual_GRID( current_GRID_set ) ){

				GRID_write_dot( 9 + current_GRID_set*10, MIR_RED );			
			}

			// Blink the current GRID set 
			// GRID_write_dot( 9 + current_GRID_set*10, MIR_BLINK 	);

			// Write Grid to MIR - this is necessary here..
			MIR_write_GRID ();


			// SHOW PLAYSTATUS OF TRACKS - OFF/ON/SOLO (red/green/orange)
			for ( j=0; j < GRID_NROF_BANKS; j++ ){

				// If the page is playing in the grid
				if (	( is_selected_in_GRID( GRID_p_selection[j] ))
					&&	( GRID_p_selection[j] != NULL )
					// &&	( GRID_p_clock_presel[j]->page_clear != ON )
					){

					// Show the preselection of the page that is active in the bank
					GRID_write_mutepattern( GRID_p_selection[j], j );
				}
			}

			break;
		
		
		
		
		// ELE_MATRIX..
		case GRID:	
		
			switch( GRID_play_mode ){

				case GRID_EDIT:

					// GRID_CURSOR shown by blinking 
					GRID_write_dot (GRID_CURSOR, MIR_BLINK);
		
					if ( Page_repository[GRID_CURSOR].page_clear == ON ){	
						// If on empty page then light position orange
						GRID_write_dot (GRID_CURSOR, MIR_GREEN);
						GRID_write_dot (GRID_CURSOR, MIR_RED);
					} 

					// NON-EMPTY PAGES shown GREEN or RED
					for (i=0; i < MAX_NROF_PAGES; i++) {
						
						// Page has contents and is not one of the row zero
						if (	(Page_repository[i].page_clear != ON)
							&&	( (i % 10) != 9 )
							){

							// Page PLAYING - i.e. selected in GRID
							if ( is_selected_in_GRID( &Page_repository[i] ) ){
								
								// Show it in GREEN
								GRID_write_dot( i, MIR_GREEN );
							}
							else {
								
								// Show it in RED - page is IDLE
								GRID_write_dot( i, MIR_RED );
							}
						} // page_clear != ON
					} // page iterator
	
					// GRID_write_dot( 39, MIR_GREEN );
					break;



				case GRID_MIX:

					// GRID_CURSOR shown by blinking 
//					GRID_write_dot (GRID_CURSOR, MIR_BLINK);


					// GRID SETS: Show the non-empty GRID sets by red lights.
					for ( i=0; i < GRID_NROF_SETS; i++ ){

						// Check the respective GRID set for content and currency
						if ( 	( get_content_GRID_set( i ) )
							&&	( current_GRID_set != i )
							){
							
							GRID_write_dot( 9 + i*10, MIR_RED );
						}
					}
	
					GRID_write_dot( 9 + current_GRID_set*10, MIR_GREEN 	);

					// Check if changes have been made to set
					if ( !is_actual_GRID( current_GRID_set ) ){

						GRID_write_dot( 9 + current_GRID_set*10, MIR_RED );			
					}

					// Blink the current GRID set - but not when called from page BIRDSEYE
					if ( zoom_level != zoomPAGE ){

						GRID_write_dot( 9 + current_GRID_set*10, MIR_BLINK 	);
					}

					// PAGES: Page iterator
					for ( i=0; i < MAX_NROF_PAGES; i++ ){

						// Page has contents and is not one of the row zero
						if (	(Page_repository[i].page_clear != ON)
							&&	( (i % 10) != 9 )
							){

							// Page PLAYING - i.e. selected in GRID
							if ( is_selected_in_GRID( &Page_repository[i] ) ){			

								// Show it in GREEN
								GRID_write_dot( i, MIR_GREEN );
								// GRID_write_dot( i, MIR_BLINK );								
							}
							else {
								// Show it in GREEN
								GRID_write_dot( i, MIR_RED );
							}
						} // page_clear != ON
					} // page iterator

					// Orange the GRID preselection
					for ( j = 0; j < GRID_NROF_BANKS; j++ ){

						// If the page is not already playing anyways.. and page not empty / clear
						if (	( ! is_selected_in_GRID( GRID_p_clock_presel[j] ))
							&&	( GRID_p_clock_presel[j]->page_clear != ON )
							){

							// Show the preselection orange
							GRID_write_dot( GRID_p_clock_presel[j]->pageNdx, MIR_RED );
							GRID_write_dot( GRID_p_clock_presel[j]->pageNdx, MIR_GREEN );	
						}
					}

					break;

			} // switch( GRID_play_mode )

			// Write Grid to MIR
			MIR_write_GRID ();
			break;



		// ELE_MATRIX..
		case STEP_TOGGLE:
			// MATRIX fields with track bitpatterns (Rows 0-9) of step toggle
			for (i=0; i<MATRIX_NROF_ROWS; i++) {

				// These are the active steps
				MIR_write_trackpattern(   Page_get_trackpattern(target_page, i)
										| Page_get_event_trackpattern( target_page, i ), 
										i, 
										MIR_GREEN);

				// .. the ones containing chords..
//				MIR_write_trackpattern( Page_get_chord_trackpattern( target_page, i ), i, MIR_RED);	

				// .. and here the event mongers - are built into the above
				MIR_write_trackpattern(   Page_get_chord_trackpattern( target_page, i )
										| Page_get_event_trackpattern( target_page, i ), 
										i, 
										MIR_RED   );	
//				MIR_write_trackpattern( Page_get_event_trackpattern( target_page, i ), i, MIR_GREEN );				
			}
			break;



		// ELE_MATRIX..
		case STEP_SELECTION:
			// Blink the Steps whose selection bit is set - assumes they are already colored
			for (i=0; i<MATRIX_NROF_ROWS; i++) {
				MIR_blink_trackpattern( Page_get_selection_trackpattern(target_page, i), i);						
				MIR_augment_trackpattern( 
					Page_get_selectOff_trackpattern( target_page, i ), i, MIR_RED ); 
			}
			break;



		// ELE_MATRIX..
		case STEP_SELECTED:
			row = target_page->stepSelectionSingleRow;
			col = target_page->stepSelectionSingleCol;

			if (Step_get_status( target_page->Step[row][col], STEPSTAT_TOGGLE ) == ON ){
//			if (Step_get_status( VIEWER_step, STEPSTAT_TOGGLE ) == ON ){

				MIR_write_dot( Page_dotIndex( row, col ),  MIR_GREEN );
			}
			else {
				MIR_write_dot( Page_dotIndex( row, col ),  MIR_RED );
			}
			MIR_write_dot( Page_dotIndex( row, col ),  MIR_BLINK);
			
			break;



		// ELE_MATRIX...
		case STEP_SKIPS:
			// Light red the skipped steps
			for (i=0; i < MATRIX_NROF_ROWS; i++){
				MIR_augment_trackpattern( 
					Page_get_skippattern( target_page, i ), i, MIR_RED );
			}
			break;



		// ELE_MATRIX..
		case TRACK_SELECTION:
			// Blink the TRACK SELECTION
			for (i=0; i<MATRIX_NROF_ROWS; i++) {
				if (target_page->trackSelection & (1<<i)) {
					if (Page_get_trackpattern(target_page, i) == 0) {
						// If the track is empty, light everything in red
						// MIR_augment_trackpattern( 0xffff, i, MIR_RED );
					}
					else {
						// If track muted, light it in red 
						if ( (target_page->trackMutepattern & (1<<i)) != 0) {
							MIR_write_trackpattern (Page_get_trackpattern(target_page, i), i, MIR_RED);
						}
						else {
							MIR_write_trackpattern (Page_get_trackpattern(target_page, i), i, MIR_RED);
						}
					}
				}
			}
			break;



		// ELE_MATRIX..
		case LAUFLICHT:
			// Lauflicht master condition (in running mode)
			if (RUNBIT || PAUSEBIT) {
				
				// Check whether the Page is actually being played
				for (i=0; i<GRID_NROF_BANKS; i++) {
					
					// If the target_page is currently playing
					if ( GRID_p_selection[i] == target_page ){
						
						MIR_write_lauflicht ();
					}
				}
				
			}
			break;



		// ELE_MATRIX..
		case LAUFLICHT_TRACK:
			// Lauflicht in selected TRACK only
			if (RUNBIT) {

				for (i=0; i<MATRIX_NROF_ROWS; i++) {

					if ((target_page->trackSelection ^ (1<<i)) == 0) {

						MIR_write_lauflicht_track (i);
						break;
					}
				}

			}
			break;										


		// ELE_MATRIX..
		case LAUFLICHT_BOTTOM:

			// Get index of selected track
			j = my_bit2ndx( target_page->trackSelection );
			
			// Write contents of selected track to rowzero
			MIR_write_trackpattern( Page_get_trackpattern( target_page, j ), 9, MIR_GREEN	    );
			MIR_write_trackpattern( Page_get_skippattern ( target_page, j ), 9, MIR_RED			);


			MIR_write_trackpattern  ( Page_get_trackpattern( target_page, j ), 9, MIR_GREEN );
			// .. and here the event/chord mongers - are built into the above
			MIR_write_trackpattern(   Page_get_chord_trackpattern( target_page, j )
									| Page_get_event_trackpattern( target_page, j ), 
									9, 
									MIR_RED   );	

			MIR_augment_trackpattern( Page_get_skippattern(  target_page, j ), 9, MIR_RED   );
			MIR_augment_trackpattern( Page_get_selection_trackpattern( target_page, j ), 9, MIR_BLINK );


			// Blink the Steps whose selection bit is set
			MIR_blink_trackpattern (Page_get_selection_trackpattern(target_page, j), 9);						

			// Track contents of selected Track in rowzero, lauflicht in the selected track
			if (RUNBIT) {
				
				MIR_write_lauflicht_track( j );
			}
			break;



		// ELE_MATRIX..
		case MIX_ATTRIBUTE_VALUES:
		
			// Display the value in each row, respecting the corresponding ranges					
			// Does not take into account the event offset values
			switch (target_page->mixAttribute) {

				case ATTR_PITCH:
					for (row=0; row<MATRIX_NROF_ROWS; row++) {


						// Show the total pitch value, although change is done only to the track pitch
						j = target_page->Track[row]->attribute[ATTR_PITCH]
							+ target_page->Track[row]->scale_pitch_offset
							+ target_page->Track[row]->lead_pitch_offset;
						
						// Display it in the note representation
						MIR_write_pitch_H( j , row );
					}
					break;
				
				case ATTR_VELOCITY:

					for (row=0; row<MATRIX_NROF_ROWS; row++) {
						MIR_write_numeric_H 
							(	target_page->Track[row]->attribute[ATTR_VELOCITY], row	);
					}
					break;


				case ATTR_AMOUNT:

					for (row=0; row<MATRIX_NROF_ROWS; row++) {
						MIR_write_numeric_H 
							(	target_page->Track[row]->attribute[ATTR_AMOUNT] 
								+ target_page->Track[row]->event_offset[ATTR_AMOUNT], row);
					}
					break;
				

				case ATTR_LENGTH:

					for (row=0; row<MATRIX_NROF_ROWS; row++) {

						MIR_fill_numeric( 1, 	target_page->Track[row]->LEN_factor +1, 
												row, MIR_RED );
						MIR_fill_numeric( 1, 	target_page->Track[row]->LEN_factor +1, 
												row, MIR_GREEN );
					}
					break;



				case ATTR_START:

					for (row=0; row < MATRIX_NROF_ROWS; row++) {

						MIR_fill_numeric( 1, 	target_page->Track[row]->STA_factor +1, 
												row, MIR_RED );
						MIR_fill_numeric( 1, 	target_page->Track[row]->STA_factor +1, 
												row, MIR_GREEN );
					}					
					break;
					
					

				case ATTR_POSITION:
					show( ELE_MATRIX, STEP_TOGGLE );
					break;
				
				

				case ATTR_DIRECTION:
					for (row=0; row<MATRIX_NROF_ROWS; row++) {

						MIR_point_numeric 
							(	target_page->Track[row]->attribute[target_page->mixAttribute] 
								+ target_page->Track[row]->event_offset[ATTR_DIRECTION], row, MIR_RED);

						MIR_point_numeric 
							(	target_page->Track[row]->attribute[target_page->mixAttribute] 
								+ target_page->Track[row]->event_offset[ATTR_DIRECTION], row, MIR_GREEN);
					}
					break;




				case ATTR_GROOVE:
					for (row=0; row<MATRIX_NROF_ROWS; row++) {
						MIR_fill_numeric (	1, 	
											target_page->Track[row]->attribute[ATTR_GROOVE] +1,
											row, MIR_RED);
						MIR_fill_numeric (	1, 
											target_page->Track[row]->attribute[ATTR_GROOVE] +1, 
											row, MIR_GREEN);
					}
					break;
				

				case ATTR_MIDICC:
					for( row=0; row<MATRIX_NROF_ROWS; row++ ){
						if ( (	target_page->Track[row]->attribute[ATTR_MIDICC] ) == MIDICC_NONE 
							){
			
							// Track is set to not send MIDICC, show MIDICC_NONE flag
							MIR_write_trackpattern ( 0x0f, row, MIR_GREEN);
						}
						else if ( (	target_page->Track[row]->attribute[ATTR_MIDICC] ) == MIDICC_BENDER 
							){
								
							// Signal the bender as selected
							MIR_write_trackpattern ( 0x01, row, MIR_RED);	
						}
						else {
							// MIDICC has a valid value 
							MIR_write_numeric_H( target_page->Track[row]->attribute[ATTR_MIDICC], row);
						}
					}
					break;

				case ATTR_MIDICH:
					for (row=0; row<MATRIX_NROF_ROWS; row++) {
						
						// MIDI OUT 1 REAL - channels in GREEN
						if ( 	target_page->Track[row]->attribute[ATTR_MIDICH]
								+ target_page->Track[row]->event_offset[ATTR_MIDICH] <= 16 
							){
							
							MIR_point_numeric 
								(	target_page->Track[row]->attribute[ATTR_MIDICH], 
									row, MIR_GREEN);
						}
						
						// MIDI OUT 2 REAL - channels in RED
						else if ( 	target_page->Track[row]->attribute[ATTR_MIDICH]
								+ target_page->Track[row]->event_offset[ATTR_MIDICH] <= 32 
							){
							
							MIR_point_numeric 
								(	target_page->Track[row]->attribute[ATTR_MIDICH] - 16, 
									row, MIR_RED );
						}
						
						// MIDI OUT 1 VIRTUAL - channels in GREEN BLINK
						else if ( 	target_page->Track[row]->attribute[ATTR_MIDICH]
								+ target_page->Track[row]->event_offset[ATTR_MIDICH] <= 48
							){
							
							MIR_point_numeric 
								(	target_page->Track[row]->attribute[ATTR_MIDICH] - 32, 
									row, MIR_GREEN );
							MIR_point_numeric 
								(	target_page->Track[row]->attribute[ATTR_MIDICH] - 32, 
									row, MIR_BLINK );
						}
						// MIDI OUT 2 VIRTUAL - channels in RED BLINK
						else{

							MIR_point_numeric 
								(	target_page->Track[row]->attribute[ATTR_MIDICH] - 48, 
									row, MIR_RED);
							MIR_point_numeric 
								(	target_page->Track[row]->attribute[ATTR_MIDICH] - 48, 
									row, MIR_BLINK);
						}	
					}
					break;
			}

			// Blink the values for trackSelection
			for (i=0; i<MATRIX_NROF_ROWS; i++){
				if ((1<<i) & (target_page->trackSelection)){
					MIR_blink_track (i, 1, 16);
				}
			}

			break;


		// ELE_MATRIX..
		case HARDWIRED_ATTRIBUTE_VALUES:
		
			// Display the value of each attribute for the SELECTED TRACK	
			// Find first the selected track
			for (i=0; i<MATRIX_NROF_ROWS; i++) {
				if ( (target_page->trackSelection & (1<<i)) > 0 ) {
					//  row is index of the first selected track
					row = i;
				}
			}
			// Now fill the matrix rows with the attribute values of track row

			// VELOCITY
			MIR_write_numeric_H( target_page->Track[row]->attribute[ATTR_VELOCITY], 0);

			// PITCH
			MIR_write_pitch_H( target_page->Track[row]->attribute[ATTR_PITCH], 1);


			// LENGTH
			// New school: showing the LEN factor
			MIR_fill_numeric 	(	1, 
									target_page->Track[row]->LEN_factor + 1
									+ target_page->Track[row]->event_offset[ATTR_LENGTH], 
									2,	MIR_GREEN);
			MIR_fill_numeric 	(	1, 
									target_page->Track[row]->LEN_factor + 1
									+ target_page->Track[row]->event_offset[ATTR_LENGTH], 
									2,	MIR_RED);
			
			// START
			// New school: showing the STA factor
			MIR_fill_numeric 	(	1, 
									target_page->Track[row]->STA_factor + 1
									+ target_page->Track[row]->event_offset[ATTR_START], 
									3,	MIR_RED);
			MIR_fill_numeric 	(	1, 
									target_page->Track[row]->STA_factor + 1
									+ target_page->Track[row]->event_offset[ATTR_START], 
									3,	MIR_GREEN);
			

			// POSITION
			// Position shows the actual track pattern and augments the skipped steps.
			// These are the active steps
			MIR_write_trackpattern  ( Page_get_trackpattern( target_page, row ), 4, MIR_GREEN );						
			// .. and here the event/chord mongers - are built into the above
			MIR_write_trackpattern(   Page_get_chord_trackpattern( target_page, row )
									| Page_get_event_trackpattern( target_page, row ), 
									4, 
									MIR_RED   );	

			MIR_augment_trackpattern( Page_get_skippattern(  target_page, row ), 4, MIR_RED   );
			MIR_augment_trackpattern( Page_get_selection_trackpattern( target_page, row ), 4, MIR_BLINK );

			// DIRECTION
			MIR_point_numeric 	(	target_page->Track[row]->attribute[ATTR_DIRECTION]
									+ target_page->Track[row]->event_offset[ATTR_DIRECTION], 	5, 	MIR_RED );
			MIR_point_numeric 	(	target_page->Track[row]->attribute[ATTR_DIRECTION] 	
									+ target_page->Track[row]->event_offset[ATTR_DIRECTION],	5, 	MIR_GREEN);

			// AMOUNT
			MIR_write_numeric_H (	target_page->Track[row]->attribute[ATTR_AMOUNT]
									+ target_page->Track[row]->event_offset[ATTR_AMOUNT]
									,		6);

			// GROOVE
			MIR_fill_numeric 	(	1, 
									target_page->Track[row]->attribute[ATTR_GROOVE] + 1, 	7,	MIR_RED);
			MIR_fill_numeric 	(	1, 
									target_page->Track[row]->attribute[ATTR_GROOVE] + 1, 	7,	MIR_GREEN);

			// MIDICC
			if ( (	target_page->Track[row]->attribute[ATTR_MIDICC] ) == MIDICC_NONE ){

				// Step is set to play legato, show legato flag
				MIR_write_trackpattern ( 0x0f, 8, MIR_GREEN);
			}
			else if ( (	target_page->Track[row]->attribute[ATTR_MIDICC] ) == MIDICC_BENDER ){
			
				// Signal the bender as selected
				MIR_write_trackpattern ( 0x01, 8, MIR_RED);	
			}
			else {
				// MIDICC has a valid value 
				MIR_write_numeric_H (	target_page->Track[row]->attribute[ATTR_MIDICC], 	8);
			}


			// MIDICH
			if ( target_page->Track[row]->attribute[ATTR_MIDICH] <= 16 ){
				MIR_point_numeric(	target_page->Track[row]->attribute[ATTR_MIDICH], 	9,	MIR_GREEN );
			} 
			else if ( target_page->Track[row]->attribute[ATTR_MIDICH] <= 32 ){
				MIR_point_numeric(	target_page->Track[row]->attribute[ATTR_MIDICH]-16, 9,	MIR_RED );
			}
			else if ( target_page->Track[row]->attribute[ATTR_MIDICH] <= 48 ){
				MIR_point_numeric(	target_page->Track[row]->attribute[ATTR_MIDICH]-32, 9,	MIR_GREEN );
				MIR_point_numeric(	target_page->Track[row]->attribute[ATTR_MIDICH]-32, 9,	MIR_BLINK );
			}
			else if ( target_page->Track[row]->attribute[ATTR_MIDICH] <= 64 ){
				MIR_point_numeric(	target_page->Track[row]->attribute[ATTR_MIDICH]-48, 9,	MIR_RED );
				MIR_point_numeric(	target_page->Track[row]->attribute[ATTR_MIDICH]-48, 9,	MIR_BLINK );
			}
			
			break;


		// ELE_MATRIX...
		case EXTENDED_ATTRIBUTE_VALUES:
		
			for (i=0; i<MATRIX_NROF_ROWS; i++) {


				// If the track is in the active selection
				if ( 	( (target_page->trackSelection & (1<<i)) > 0 )
					&&	( EDIT_TIMER == ON )
					){

					// Show its edited attribute value in its row
					switch( target_page->editAttribute ) {

						case ATTR_VELOCITY:
							MIR_write_numeric_H(	
								target_page->Track[i]->attribute[ATTR_VELOCITY], 	i);
							break;

						case ATTR_PITCH:
							MIR_write_pitch_H(
								target_page->Track[i]->attribute[ATTR_PITCH], i);
							break;

						case ATTR_LENGTH:
							// New school: showing the LEN factor
							MIR_fill_numeric(
								1, 
								target_page->Track[i]->LEN_factor + 1
								+ target_page->Track[i]->event_offset[ATTR_LENGTH], 
								i,	MIR_GREEN);
							MIR_fill_numeric(
								1, 
								target_page->Track[i]->LEN_factor + 1
								+ target_page->Track[i]->event_offset[ATTR_LENGTH], 
								i,	MIR_RED);
							break;

						case ATTR_START:
							// New school: showing the STA factor
							MIR_fill_numeric(
								1, 
								target_page->Track[i]->STA_factor + 1
								+ target_page->Track[i]->event_offset[ATTR_START], 
								i,	MIR_RED);
							MIR_fill_numeric(
								1, 
								target_page->Track[i]->STA_factor + 1
								+ target_page->Track[i]->event_offset[ATTR_START], 
								i,	MIR_GREEN);
							break;

						case ATTR_POSITION:
							MIR_write_trackpattern (Page_get_trackpattern(target_page, i), 			i, MIR_RED);						
							break;

						case ATTR_DIRECTION:
							MIR_point_numeric(
								target_page->Track[i]->attribute[ATTR_DIRECTION]
								+ target_page->Track[i]->event_offset[ATTR_DIRECTION],
								i, 	MIR_RED);
							MIR_point_numeric(
								target_page->Track[i]->attribute[ATTR_DIRECTION]
								+ target_page->Track[i]->event_offset[ATTR_DIRECTION],
								i, 	MIR_GREEN);
							break;

						case ATTR_AMOUNT:
							MIR_write_numeric_H(
								target_page->Track[i]->attribute[ATTR_AMOUNT]
								+ target_page->Track[i]->event_offset[ATTR_AMOUNT], 	
								i);
							break;

						case ATTR_GROOVE:
							MIR_fill_numeric(
								1, 
								target_page->Track[i]->attribute[ATTR_GROOVE]+1,
								i,	MIR_RED);
							MIR_fill_numeric(
								1, 
								target_page->Track[i]->attribute[ATTR_GROOVE]+1,
								i,	MIR_GREEN);
							break;;

						case ATTR_MIDICC:
							if ( (	target_page->Track[i]->attribute[ATTR_MIDICC] )
										== MIDICC_NONE ){
				
								// Track is set to not send MIDICC, show MIDICC_NONE flag
								MIR_write_trackpattern ( 0x0f, i, MIR_GREEN);
							}
							else if ( (	target_page->Track[i]->attribute[ATTR_MIDICC] ) 
										== MIDICC_BENDER 
								){
									
								// Signal the bender as selected
								MIR_write_trackpattern ( 0x01, i, MIR_RED);	
							}
							else {
								// MIDICC has a valid value 
								MIR_write_numeric_H(
									target_page->Track[i]->attribute[ATTR_MIDICC], i);
							}
							break;

						case ATTR_MIDICH:
							if ( 	target_page->Track[i]->attribute[ATTR_MIDICH]
									+ target_page->Track[i]->event_offset[ATTR_MIDICH] <= 16
								){
								
								MIR_point_numeric(
									target_page->Track[i]->attribute[ATTR_MIDICH]-0
									+ target_page->Track[i]->event_offset[ATTR_MIDICH],
									i,	MIR_GREEN);
							}

							else if ( 	target_page->Track[i]->attribute[ATTR_MIDICH]
									+ target_page->Track[i]->event_offset[ATTR_MIDICH] <= 32
								){
								
								MIR_point_numeric(
									target_page->Track[i]->attribute[ATTR_MIDICH]-16
									+ target_page->Track[i]->event_offset[ATTR_MIDICH],
									i,	MIR_RED);
							}

							else if ( 	target_page->Track[i]->attribute[ATTR_MIDICH]
									+ target_page->Track[i]->event_offset[ATTR_MIDICH] <= 48
								){
								
								MIR_point_numeric(
									target_page->Track[i]->attribute[ATTR_MIDICH]-32
									+ target_page->Track[i]->event_offset[ATTR_MIDICH],
									i,	MIR_GREEN);
								MIR_point_numeric(
									target_page->Track[i]->attribute[ATTR_MIDICH]-32
									+ target_page->Track[i]->event_offset[ATTR_MIDICH],
									i,	MIR_BLINK);
							}

							else if ( 	target_page->Track[i]->attribute[ATTR_MIDICH]
									+ target_page->Track[i]->event_offset[ATTR_MIDICH] <= 64
								){
								
								MIR_point_numeric(
									target_page->Track[i]->attribute[ATTR_MIDICH]-48
									+ target_page->Track[i]->event_offset[ATTR_MIDICH],
									i,	MIR_RED );
								MIR_point_numeric(
									target_page->Track[i]->attribute[ATTR_MIDICH]-48
									+ target_page->Track[i]->event_offset[ATTR_MIDICH],
									i,	MIR_BLINK );
							}
							break;
					}			

				}
				
				
				// If the track is in the active selection, or if we are mixing something..
				else if ( 	
//						( (target_page->trackSelection & (1<<i)) > 0 )
//					||	
					( MIX_TIMER == ON )
					){

					if ( MIX_TIMER == ON ){
						i = target_page->mixingTrack;
						MIR_write_dot( LED_MIX_INDICATOR, MIR_BLINK );
						temp = target_page->mixAttribute;
					}
					else if ( EDIT_TIMER == ON ){
						temp = target_page->editAttribute;
					}

					// Clear the displayed track contents
					MIR_write_trackpattern( 0, i, MIR_RED   );
					MIR_write_trackpattern( 0, i, MIR_GREEN );
					MIR_write_trackpattern( 0, i, MIR_BLINK );

					switch( target_page->mixTarget ){
					
						// An attribute is being mixed..
						case MIXTGT_ATR:
						
							// Show its edited attribute value in its row
							switch( temp ) {
		
								case ATTR_VELOCITY:
									MIR_write_numeric_H(
										target_page->Track[i]->attribute[ATTR_VELOCITY], 	i);
									break;
		
								case ATTR_PITCH:
									MIR_write_pitch_H(
										target_page->Track[i]->attribute[ATTR_PITCH], 		i);
									break;
		
								case ATTR_LENGTH:
									// New school: showing the LEN factor
									MIR_fill_numeric(	
										1, 
										target_page->Track[i]->LEN_factor + 1
										+ target_page->Track[i]->event_offset[ATTR_LENGTH], 
										i,	MIR_RED);
									MIR_fill_numeric(	
										1, 
										target_page->Track[i]->LEN_factor + 1
										+ target_page->Track[i]->event_offset[ATTR_LENGTH], 
										i,	MIR_GREEN);
									break;
		
								case ATTR_START:
									MIR_fill_numeric(	
										1, 
										target_page->Track[i]->STA_factor + 1
										+ target_page->Track[i]->event_offset[ATTR_START], 
										i,	MIR_RED);
									MIR_fill_numeric(	
										1, 
										target_page->Track[i]->STA_factor + 1
										+ target_page->Track[i]->event_offset[ATTR_START], 
										i,	MIR_GREEN);
									break;
		
								case ATTR_POSITION:
									MIR_write_trackpattern (Page_get_trackpattern(target_page, i), 			i, MIR_RED);						
									break;
		
								case ATTR_DIRECTION:
									MIR_point_numeric(
										target_page->Track[i]->attribute[ATTR_DIRECTION]
										+ target_page->Track[i]->event_offset[ATTR_DIRECTION], 	
										i, 	MIR_RED);
									MIR_point_numeric(
										target_page->Track[i]->attribute[ATTR_DIRECTION]
										+ target_page->Track[i]->event_offset[ATTR_DIRECTION], 
										i, 	MIR_GREEN);
									break;
		
								case ATTR_AMOUNT:
									MIR_write_numeric_H(
										target_page->Track[i]->attribute[ATTR_AMOUNT]
										+ target_page->Track[i]->event_offset[ATTR_AMOUNT], 	i);
									break;
		
								case ATTR_GROOVE:
									MIR_fill_numeric(
										1, 
										target_page->Track[i]->attribute[ATTR_GROOVE]+1,	
										i,	MIR_RED);
									MIR_fill_numeric(
										1, target_page->Track[i]->attribute[ATTR_GROOVE]+1,	
										i,	MIR_GREEN);
									break;;
		
								case ATTR_MIDICC:
									if ( (	target_page->Track[i]->attribute[ATTR_MIDICC] )
												== MIDICC_NONE ){
						
										// Track is set to not send MIDICC, show MIDICC_NONE flag
										MIR_write_trackpattern ( 0x0f, i, MIR_GREEN);
									}
									else if ( (	target_page->Track[i]->attribute[ATTR_MIDICC] ) == MIDICC_BENDER 
										){
											
										// Signal the bender as selected
										MIR_write_trackpattern ( 0x01, i, MIR_RED);	
									}
									else {
										// MIDICC has a valid value 
										MIR_write_numeric_H(
											target_page->Track[i]->attribute[ATTR_MIDICC], 	i);
									}
									break;
		
								case ATTR_MIDICH:
									if ( 	target_page->Track[i]->attribute[ATTR_MIDICH]
											+ target_page->Track[i]->event_offset[ATTR_MIDICH] <= 16 ){
										
										MIR_point_numeric(
											target_page->Track[i]->attribute[ATTR_MIDICH]
											+ target_page->Track[i]->event_offset[ATTR_MIDICH], 	
											i,	MIR_GREEN );
									}
									
									else if ( 	target_page->Track[i]->attribute[ATTR_MIDICH]
											+ target_page->Track[i]->event_offset[ATTR_MIDICH] <= 32 ){
										
										MIR_point_numeric(
											target_page->Track[i]->attribute[ATTR_MIDICH]-16
											+ target_page->Track[i]->event_offset[ATTR_MIDICH], 	
											i,	MIR_RED );
									}
									
									else if ( 	target_page->Track[i]->attribute[ATTR_MIDICH]
											+ target_page->Track[i]->event_offset[ATTR_MIDICH] <= 48 ){
										
										MIR_point_numeric(
											target_page->Track[i]->attribute[ATTR_MIDICH]-32
											+ target_page->Track[i]->event_offset[ATTR_MIDICH], 	
											i,	MIR_GREEN );
										MIR_point_numeric(
											target_page->Track[i]->attribute[ATTR_MIDICH]-32
											+ target_page->Track[i]->event_offset[ATTR_MIDICH], 	
											i,	MIR_BLINK );
									}
									
									else if ( 	target_page->Track[i]->attribute[ATTR_MIDICH]
											+ target_page->Track[i]->event_offset[ATTR_MIDICH] <= 64 ){
										
										MIR_point_numeric(
											target_page->Track[i]->attribute[ATTR_MIDICH]-32
											+ target_page->Track[i]->event_offset[ATTR_MIDICH], 	
											i,	MIR_RED );
										MIR_point_numeric(
											target_page->Track[i]->attribute[ATTR_MIDICH]-32
											+ target_page->Track[i]->event_offset[ATTR_MIDICH], 	
											i,	MIR_BLINK );
									}
									break;
							}			
							break;
							

						case MIXTGT_USR1:
						case MIXTGT_USR2:
						case MIXTGT_USR3:
						case MIXTGT_USR4:
						case MIXTGT_USR5:
							// Check which page to use as source
							if ( GRID_MAP0_status == ON ){
								temp_page = GRID_assistant_page;
							}
							else{
								temp_page = target_page;
							}
							// Show the available attributes of the CC_MIXMAP
							show( ELE_TRACK_SELECTORS, CC_MIXMAP_ATTRIBUTES );
							MIR_write_numeric_H( temp_page->CC_MIXMAP [target_page->mixTarget][i][CC_MIXMAP_AMT], i );
							break;
		
						// These dont show anything for now
						case MIXTGT_VOL:
							MIR_write_numeric_H( target_page->MIXAMT_VOL[i], i );
							break;
							
						case MIXTGT_PAN:
							MIR_write_numeric_H( target_page->MIXAMT_PAN[i], i );
							break;
							
						case MIXTGT_MOD:
							MIR_write_numeric_H( target_page->MIXAMT_MOD[i], i );
							break;
							
						case MIXTGT_EXP:
							MIR_write_numeric_H( target_page->MIXAMT_EXP[i], i );
							break;
					}
				
					// Exit the for loop because we are done
					if ( MIX_TIMER == ON ){
						i = MATRIX_NROF_ROWS;
					}

				} // Found the right track to process data from..
			}
			break;	


		// ELE_MATRIX..
		case ATTR_MAP_VALUES:
			// Display for each step the value of selected attribute

			// Get row of the selected track
			row = my_bit2ndx( target_page->trackSelection );

			// Get the selected attribute
			attribute = my_bit2ndx( target_page->trackAttributeSelection );

			// Display type depends on the attribute itself
			switch( attribute ){
	
				case VELOCITY:
					// Interval mapping taking place - centerline is 4, range -49 +49
					MIR_write_numeric_V_zeroline_VEL( 4, -49, 49, row, attribute );

					// Show the VEL_factor of the current track
					MIR_write_factor_C( 
						target_page->Track[row]->VEL_factor 
						+ target_page->Track[row]->event_offset[ATTR_VELOCITY] );

					// Show the active Big Knob - choosing certainty_next
					MIR_write_dot( LED_SCALE_MYSEL, MIR_GREEN );
					MIR_write_dot( LED_SCALE_MYSEL, MIR_RED );					
					break;

				case PITCH:
					// Interval mapping taking place - centerline is 4, range -49 +49
					MIR_write_numeric_V_zeroline_PIT( 4, -49, 49, row, attribute );

					// Show the PIT_factor of the current track
					MIR_write_factor_C( 
						target_page->Track[row]->PIT_factor 
						+ target_page->Track[row]->event_offset[ATTR_PITCH] );

					// Show the active Big Knob - choosing certainty_next
					MIR_write_dot( LED_SCALE_MYSEL, MIR_GREEN );
					MIR_write_dot( LED_SCALE_MYSEL, MIR_RED );					
					break;

				case LENGTH:
					// The length has another special display mode. It also approximates
					// ..but under different conditions.
					MIR_show_lengthbar_V_LEN( row );

					// Show the LEN_factor of the current track
					MIR_write_factor_C( 
						target_page->Track[row]->LEN_factor 
						+ target_page->Track[row]->event_offset[ATTR_LENGTH] );

					// Show the active Big Knob - choosing certainty_next
					MIR_write_dot( LED_SCALE_MYSEL, MIR_GREEN );
					MIR_write_dot( LED_SCALE_MYSEL, MIR_RED );					
					break;

				case START: 
					// The start has a special display mode
					MIR_show_startbar_V( row );

					// Show the STA_factor of the current track
					MIR_write_factor_C( 
						target_page->Track[row]->STA_factor 
						+ target_page->Track[row]->event_offset[ATTR_START] );

					// Show the active Big Knob - choosing certainty_next
					MIR_write_dot( LED_SCALE_MYSEL, MIR_GREEN );
					MIR_write_dot( LED_SCALE_MYSEL, MIR_RED );					
					break;


				case DIRECTION:
					// This is the direction editing mode
					// Show the currently selected direction
					MIR_point_numeric( 
						target_page->Track[row]->attribute[ATTR_DIRECTION], 9, MIR_RED );
					MIR_point_numeric( 
						target_page->Track[row]->attribute[ATTR_DIRECTION], 9, MIR_GREEN );

					// Show the currently selected trigger set
					MIR_point_numeric( 
						current_trigger_set, 9, MIR_GREEN );
					MIR_point_numeric( 
						current_trigger_set, 9, MIR_BLINK );
						
					// Show the current triggers in the selected trigger set
					for (i=0; i < MATRIX_NROF_ROWS; i++){
						
						MIR_point_numeric( 
							direction_repository[ target_page->Track[row]->attribute[ATTR_DIRECTION]]
							.trigger_set[ current_trigger_set ].trigger[i], 
							MATRIX_NROF_ROWS - (i+1), MIR_RED );
					}
					
					// Show the "certainty next" parameter of the selected trigger set
					MIR_write_numeric_C( 
							direction_repository[ target_page->Track[row]->attribute[ATTR_DIRECTION]]
							.trigger_set[ current_trigger_set ]
							.certainty_next );

					// Show the active Big Knob - choosing certainty_next
					MIR_write_dot( LED_SCALE_MYSEL, MIR_GREEN );
					MIR_write_dot( LED_SCALE_MYSEL, MIR_RED );					
					break;


				case AMOUNT:
					// Interval mapping taking place - centerline is 4, range -49 +49
					MIR_write_numeric_V_zeroline_VEL( 4, -49, 49, row, attribute );

					// Show the VEL_factor of the current track
					MIR_write_factor_C( 
						target_page->Track[row]->AMT_factor 
						+ target_page->Track[row]->event_offset[ATTR_AMOUNT] );

					// Show the active Big Knob - choosing certainty_next
					MIR_write_dot( LED_SCALE_MYSEL, MIR_GREEN );
					MIR_write_dot( LED_SCALE_MYSEL, MIR_RED );					
					break;


				case GROOVE:
					// Nothing special here - the usual
					MIR_show_groovebar_V( row );

					// Show the VEL_factor of the current track
					MIR_write_factor_C( 
						target_page->Track[row]->GRV_factor 
						+ target_page->Track[row]->event_offset[ATTR_GROOVE] );

					// Show the active Big Knob - choosing certainty_next
					MIR_write_dot( LED_SCALE_MYSEL, MIR_GREEN );
					MIR_write_dot( LED_SCALE_MYSEL, MIR_RED );					
					break;

 
				case MIDICC:
					// Choose the most convenience display method
					switch( target_page->Track[row]->attribute[ATTR_MIDICC] ){

						case MIDICC_BENDER:
							MIR_write_numeric_V_zeroline_MCC_BENDER( 4, -63, 64, row, attribute );
							break;

						default:
							// Interval mapping taking place 
							MIR_write_numeric_V_MCC( target_page, row );		
							break;
					}
	
					// Show the VEL_factor of the current track
					MIR_write_factor_C( 
						target_page->Track[row]->MCC_factor 
						+ target_page->Track[row]->event_offset[ATTR_MIDICC] );

					// Show the active Big Knob - factor selector
					MIR_write_dot( LED_SCALE_MYSEL, MIR_GREEN );
					MIR_write_dot( LED_SCALE_MYSEL, MIR_RED );
					break;	
			}

			break;
	


		// ELE_MATRIX..
		case PLAY_CROSSBARS:
			// Show the keyboard dark keys as bars in the first nine rows
			for (i=0; i<9; i++) {
				// The pattern is hardwired by musical rules
				MIR_write_trackpattern( 0x94A9 , i, MIR_RED 	);
				MIR_write_trackpattern( 0x94A9 , i, MIR_GREEN 	);					
			}
			break;


		
		// ELE_MATRIX..

						
		// Per track: Mix Attribute Value
		// show( ELE_MATRIX, MIX_ATTRIBUTE_VALUES );
		for (i=0; i<MATRIX_NROF_ROWS; i++) {
			
			j = target_page->CC_MIXMAP[target_page->mixTarget][i][target_page->CC_MIXMAP_attribute];

			// Account for the different display needs of the attributes
			switch( target_page->CC_MIXMAP_attribute ){

				case CC_MIXMAP_MCC:
					if ( j == (unsigned char) MIDICC_NONE ){					
						// Track is set to not send MIDICC, show MIDICC_NONE flag
						MIR_write_trackpattern ( 0x0f, i, MIR_GREEN);
					}
					else {
						// MIDICC has a valid value 
						MIR_write_numeric_H ( j, i );
					}
					break;
					
				case CC_MIXMAP_MCH:							
					if ( j <= 16 ){
						MIR_point_numeric( j, 		i, MIR_GREEN);
					}
					else {
						MIR_point_numeric( j-16,	i,	MIR_RED);
					}
					break;
					
				case CC_MIXMAP_AMT:
					MIR_write_numeric_H( j, i );								
					break;
			}
		}




		case GRID_CC_MAP_VALS:
				
			// Per track: Mix Attribute Value
			// show( ELE_MATRIX, MIX_ATTRIBUTE_VALUES );
			for (i=0; i<MATRIX_NROF_ROWS; i++) {
				
				j = GRID_assistant_page->CC_MIXMAP[ GRID_assistant_page->mixTarget ][ i ][ GRID_assistant_page->CC_MIXMAP_attribute];

				// Account for the different display needs of the attributes
				switch( GRID_assistant_page->CC_MIXMAP_attribute ){

					case CC_MIXMAP_MCC:
						if ( j == (unsigned char) MIDICC_NONE ){					
							// Track is set to not send MIDICC, show MIDICC_NONE flag
							MIR_write_trackpattern ( 0x0f, i, MIR_GREEN);
						}
						else {
							// MIDICC has a valid value 
							MIR_write_numeric_H ( j, i );
						}
						break;
						
					case CC_MIXMAP_MCH:							
						if ( j <= 16 ){
							MIR_point_numeric( j, 		i, MIR_GREEN);
						}
						else {
							MIR_point_numeric( j-16,	i,	MIR_RED);
						}
						break;
						
					case CC_MIXMAP_AMT:
						MIR_write_numeric_H( j, i );								
						break;
				}
			}
			break;



		// ELE_MATRIX..
		case MIXAMT_VALS:
					
			// Depends on the selected mix target
			switch( target_page->mixTarget ){
				
				case MIXTGT_ATR:
					// If mixMasterStatus wants to show values
					if (target_page->mixMasterStatus == NEG) {
						
						// Per track: Mix Attribute Value
						show( ELE_MATRIX, MIX_ATTRIBUTE_VALUES );
					}
					else {
						// DEFAULT MATRIX DISPLAY
						show( ELE_MATRIX, STEP_TOGGLE );
						show( ELE_MATRIX, LAUFLICHT );
						
						// Write the edited value in track 
						// ..unless it is LENGTH, START or POSITION					
						if ( 	(target_page->mixAttribute != ATTR_LENGTH)
							 &&	(target_page->mixAttribute != ATTR_START)
							 &&	(target_page->mixAttribute != ATTR_POSITION) 
							 ){

							// Clear the track
							i = target_page->mixingTrack;
							MIR_write_trackpattern( 0, i, MIR_RED   );
							MIR_write_trackpattern( 0, i, MIR_GREEN );
							MIR_write_trackpattern( 0, i, MIR_BLINK );

							// Write attribute value in the mixing track
							j = target_page->Track[i]->attribute[target_page->mixAttribute];
							MIR_write_numeric_H( j, i );
						}
					}							
					break;
					
					
				case MIXTGT_VOL:
					// If mixMasterStatus wants to show values
					if (target_page->mixMasterStatus == NEG) {
						// Per track: Mix Attribute Value
						// show( ELE_MATRIX, MIX_ATTRIBUTE_VALUES );
						for (i=0; i<MATRIX_NROF_ROWS; i++) {
							MIR_write_numeric_H( target_page->MIXAMT_VOL[i], i );
						}
					}
					else {
						// DEFAULT MATRIX DISPLAY
						show( ELE_MATRIX, STEP_TOGGLE );
						show( ELE_MATRIX, LAUFLICHT );
						
						// Write the edited value in track:

						// Clear the track
						i = target_page->mixingTrack;
						MIR_write_trackpattern( 0, i, MIR_RED   );
						MIR_write_trackpattern( 0, i, MIR_GREEN );
						MIR_write_trackpattern( 0, i, MIR_BLINK );

						// Write attribute value in the mixing track
						j = target_page->MIXAMT_VOL[i];
						MIR_write_numeric_H( j, i );
					}
					break;


				case MIXTGT_PAN:
					// If mixMasterStatus wants to show values
					if (target_page->mixMasterStatus == NEG) {
						// Per track: Mix Attribute Value
						// show( ELE_MATRIX, MIX_ATTRIBUTE_VALUES );
						for (i=0; i<MATRIX_NROF_ROWS; i++) {
							MIR_write_numeric_H( target_page->MIXAMT_PAN[i], i );
						}
					}
					else {
						// DEFAULT MATRIX DISPLAY
						show( ELE_MATRIX, STEP_TOGGLE );
						show( ELE_MATRIX, LAUFLICHT );
						
						// Write the edited value in track:

						// Clear the track
						i = target_page->mixingTrack;
						MIR_write_trackpattern( 0, i, MIR_RED   );
						MIR_write_trackpattern( 0, i, MIR_GREEN );
						MIR_write_trackpattern( 0, i, MIR_BLINK );

						// Write attribute value in the mixing track
						j = target_page->MIXAMT_PAN[i];
						MIR_write_numeric_H( j, i );
					}
					break;


				case MIXTGT_MOD:
					// If mixMasterStatus wants to show values
					if (target_page->mixMasterStatus == NEG) {
						// Per track: Mix Attribute Value
						// show( ELE_MATRIX, MIX_ATTRIBUTE_VALUES );
						for (i=0; i<MATRIX_NROF_ROWS; i++) {
							MIR_write_numeric_H( target_page->MIXAMT_MOD[i], i );
						}
					}
					else {
						// DEFAULT MATRIX DISPLAY
						show( ELE_MATRIX, STEP_TOGGLE );
						show( ELE_MATRIX, LAUFLICHT );
						
						// Write the edited value in track:

						// Clear the track
						i = target_page->mixingTrack;
						MIR_write_trackpattern( 0, i, MIR_RED   );
						MIR_write_trackpattern( 0, i, MIR_GREEN );
						MIR_write_trackpattern( 0, i, MIR_BLINK );

						// Write attribute value in the mixing track
						j = target_page->MIXAMT_MOD[i];
						MIR_write_numeric_H( j, i );
					}
					break;

				case MIXTGT_EXP:
					// If mixMasterStatus wants to show values
					if (target_page->mixMasterStatus == NEG) {
						// Per track: Mix Attribute Value
						// show( ELE_MATRIX, MIX_ATTRIBUTE_VALUES );
						for (i=0; i<MATRIX_NROF_ROWS; i++) {
							MIR_write_numeric_H( target_page->MIXAMT_EXP[i], i );
						}
					}
					else {
						// DEFAULT MATRIX DISPLAY
						show( ELE_MATRIX, STEP_TOGGLE );
						show( ELE_MATRIX, LAUFLICHT );
						
						// Write the edited value in track:

						// Clear the track
						i = target_page->mixingTrack;
						MIR_write_trackpattern( 0, i, MIR_RED   );
						MIR_write_trackpattern( 0, i, MIR_GREEN );
						MIR_write_trackpattern( 0, i, MIR_BLINK );

						// Write attribute value in the mixing track
						j = target_page->MIXAMT_EXP[i];
						MIR_write_numeric_H( j, i );
					}
					break;

				// The indexes are 0-5
				case MIXTGT_USR0:
				case MIXTGT_USR1:
				case MIXTGT_USR2:
				case MIXTGT_USR3:
				case MIXTGT_USR4:
				case MIXTGT_USR5:
					
					if ( GRID_MAP0_status == ON ){
						temp_page = GRID_assistant_page;
					}
					else{
						temp_page = target_page;
					}
					
					// If mixMasterStatus wants to show values
					if (target_page->mixMasterStatus == NEG) {
						
						// Per track: Mix Attribute Value
						// show( ELE_MATRIX, MIX_ATTRIBUTE_VALUES );
						for (i=0; i<MATRIX_NROF_ROWS; i++) {
							
							j = temp_page->CC_MIXMAP[target_page->mixTarget][i][target_page->CC_MIXMAP_attribute];

							// Account for the different display needs of the attributes
							switch( target_page->CC_MIXMAP_attribute ){

								case CC_MIXMAP_MCC:
									if ( j == (unsigned char) MIDICC_NONE ){					
										// Track is set to not send MIDICC, show MIDICC_NONE flag
										MIR_write_trackpattern ( 0x0f, i, MIR_GREEN);
									}
									else {
										// MIDICC has a valid value 
										MIR_write_numeric_H ( j, i );
									}
									break;
									
								case CC_MIXMAP_MCH:							
									if ( j <= 16 ){
										MIR_point_numeric( j, 		i, MIR_GREEN);
									}
									else {
										MIR_point_numeric( j-16,	i,	MIR_RED);
									}
									break;
									
								case CC_MIXMAP_AMT:
									MIR_write_numeric_H( j, i );								
									break;
							}
						}
					}
					else {
	
						// DEFAULT MATRIX DISPLAY
						show( ELE_MATRIX, STEP_TOGGLE );
						show( ELE_MATRIX, LAUFLICHT );
						
						// Write the edited value in track:

						// Clear the track
						i = target_page->mixingTrack;
						MIR_write_trackpattern( 0, i, MIR_RED   );
						MIR_write_trackpattern( 0, i, MIR_GREEN );
						MIR_write_trackpattern( 0, i, MIR_BLINK );

						// Write attribute value in the mixing track
						j = temp_page->MIXAMT_USR[i];
						MIR_write_numeric_H( j, i );
					}
					break;					
			}					
			break;					



		// ELE_MATRIX..
		case EDIT_ATTRIBUTE:
		
			// Show the edit attribute of selected track in track row
			for ( i=0; i<MATRIX_NROF_ROWS; i++ ) {

				// Write the value unless it is LENGTH, START or POSITION					
				if ((target_page->trackSelection & (1<<i)) != 0) {

					if ( 	
//							(target_page->editAttribute != ATTR_LENGTH)
//						&&	(target_page->editAttribute != ATTR_START)
//						&&	
							(target_page->editAttribute != ATTR_POSITION)  ){

						// Clear the track
						MIR_write_trackpattern( 0, i, MIR_RED   );
						MIR_write_trackpattern( 0, i, MIR_GREEN );
						MIR_write_trackpattern( 0, i, MIR_BLINK );							

						show( ELE_MATRIX, EXTENDED_ATTRIBUTE_VALUES );
					}
				}
			}
			break;



		// ELE_MATRIX..
		case STEP_ATTRIBUTE_VALUES:
		
			// Shows the attributes of a selected and zoomed in STEP as bars
			// Applies to one selected step.
				
			row = target_page->stepSelectionSingleRow;
			col = target_page->stepSelectionSingleCol;			
			// Now fill the matrix rows with the attribute values of step


			// VELOCITY
			MIR_write_numeric_H( target_page->Step[row][col]->attribute[ATTR_VELOCITY], 	0);

			// PITCH
			MIR_write_numeric_H( target_page->Step[row][col]->attribute[ATTR_PITCH], 		1);

			// LENGTH
			if ( target_page->Step[row][col]->attribute[ATTR_LENGTH] == (signed char) STEP_MAX_LENGTH ){

				// Step is set to play legato, show legato flag
				MIR_write_trackpattern ( 0x0f, 2, MIR_GREEN);
			}
			else {
				// The common case is when the length value is shown
				MIR_write_length_H (target_page->Step[row][col]->attribute[ATTR_LENGTH], 	2);
			}

			// START
			MIR_show_startbar_H (target_page->Step[row][col]->attribute[ATTR_START], 		3);


			// POSITION
			// Shows the track pattern of the step's row and augments the skipped steps.
			MIR_write_trackpattern  ( Page_get_trackpattern( target_page, row ), 4, MIR_GREEN );
			
			// .. and here the event/chord mongers - are built into the above
			MIR_augment_trackpattern(   Page_get_chord_trackpattern( target_page, row )
									| Page_get_event_trackpattern( target_page, row ), 
									4, 
									MIR_RED   );	
			// .. and here the event/chord mongers - are built into the above
			MIR_augment_trackpattern(   Page_get_chord_trackpattern( target_page, row )
									| Page_get_event_trackpattern( target_page, row ), 
									4, 
									MIR_GREEN   );	

			MIR_augment_trackpattern( 	Page_get_skippattern(  target_page, row ), 
										4, 
										MIR_RED   );

			// Blink the selected step
			MIR_augment_trackpattern( Page_get_selection_trackpattern( target_page, row ), 
										4, 
										MIR_BLINK );


			// AMOUNT
			MIR_write_numeric_H (target_page->Step[row][col]->attribute[ATTR_AMOUNT],		6);


			// GROOVE
			MIR_fill_numeric	(1, target_page->Step[row][col]->attribute[ATTR_GROOVE]+2,	7,	MIR_RED);
			MIR_fill_numeric	(1, target_page->Step[row][col]->attribute[ATTR_GROOVE]+2,	7,	MIR_GREEN);


			// MIDICC
			if ( target_page->Step[row][col]->attribute[ATTR_MIDICC] == MIDICC_NONE ){

				// Step is set to play legato, show legato flag
				MIR_write_trackpattern ( 0x0f, 8, MIR_GREEN);
			}
			else {
				// MIDICC has a valid value 
				MIR_write_numeric_H (target_page->Step[row][col]->attribute[ATTR_MIDICC],		8);
			}

			break;


	} // switch MATRIX content


