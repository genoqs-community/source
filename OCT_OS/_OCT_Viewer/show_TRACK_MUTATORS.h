//
// Copyright (C) 2004, 2005, 2006, 2007 genoQs Machines
// This file is part of Octopus_OS - 
// the firmware of the genoQs Machines Octopus, MIDI Control Sequencer
//
// The source code Octopus_OS is available for use in accordance with the 
// GNU General Public License (GPL). For more information about Open Source 
// licensing, please visit http://www.opensource.org/
//
// Octopus_OS is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation; either version 2 of the License, or
// (at your option) any later version.
//
// Octopus_OS is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with Octopus_OS; if not, write to the Free Software
// Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
//





	// Differentiate between single and multi selection
	for (i=0; i<MATRIX_NROF_ROWS; i++){
		if ((target_page->trackSelection & (1<<i)) > 0) {
			if ((target_page->trackSelection ^ (1<<i)) > 0) { 
				// Multiple Track Selection
				j = TRACK_M;
			}
			else {
			// Single Track Selection
				j = TRACK_S;
			}
		}
	}
	
	switch (content) {
		
		case GRID_SET_SWITCHMODE:
			#ifdef FEATURE_ENABLE_SONG_UPE
			// page cluster selection
			if ( GRID_p_selection_cluster == ON ) {

				MIR_write_buttool( RHS,		CLEAR, 				MIR_RED 	);
				MIR_write_buttool( RHS,		CLEAR, 				MIR_GREEN 	);
				if ( CHECK_BIT(page_cluster_op, PGM_CLST_CLR) ) {
					MIR_write_buttool( RHS, 	CLEAR, 				MIR_BLINK 	);
				}

				MIR_write_buttool( RHS,		COPY, 				MIR_RED 	);
				MIR_write_buttool( RHS,		COPY, 				MIR_GREEN 	);
				if ( CHECK_BIT(page_cluster_op, PGM_CLST_CPY) ) {
					MIR_write_buttool( RHS, 	COPY, 				MIR_BLINK 	);
				}

				break;
			}
			#endif
			// SOLO LED
			// Find out whether there is something in the solo buffer
			j = FALSE;
			for ( i=0; i < GRID_NROF_BANKS; i++ ){

				// Check content of current buffer position
				if ( (GRID_p_selection_buffer[i] != NULL) ) {

					j = TRUE;
				}
			}	

			// Page solo condition: grid buffer is empty and page is active				
			if ( ( j == TRUE ) ) {
				
				MIR_write_buttool( RHS,		SOLO, 				MIR_GREEN 	);
				MIR_write_buttool( RHS, 	SOLO, 				MIR_BLINK 	);
			}

			// Show the GRID SET switchmode
//			if ( is_pressed_key( KEY_SELECT_MASTER ) == ON ){
//			if ( is_pressed_key( KEY_PROGRAM ) == ON ){
			if ( is_pressed_key( KEY_SELECT_MASTER ) == ON ){

				GRID_set_switchmode = SEL_CLEAR;
			}
			else {

				GRID_set_switchmode = SEL_RECALL;
			}

			switch( GRID_set_switchmode ){
	
				case SEL_RECALL:
//					MIR_write_dot( KEY_SELECT_MASTER, 	MIR_GREEN 	);
//	MIR_write_dot( LED_PROGRAM, MIR_GREEN );
	MIR_write_dot( LED_SELECT_MASTER, MIR_GREEN );
					break;
	
				case SEL_CLEAR:
//					MIR_write_dot( KEY_SELECT_MASTER, 	MIR_RED 	);
//	MIR_write_dot( LED_PROGRAM, MIR_RED );
	MIR_write_dot( LED_SELECT_MASTER, MIR_RED );

					break;
			} // end of switch( GRID_set_switchmode )


			// Show the mute status of the pages			
			i = 0;
			j = 0;

//			// Iterate through all grid banks 
//			for ( row=0; row < GRID_NROF_BANKS; row ++ ){
//
//				// Make sure the bank is playing a valid page
//				if ( GRID_p_selection[ row ] != NULL ){
//
//					// The page has an active mutepattern
//					if ( GRID_p_selection[ row ]->trackMutepattern != 0 ){
//
//						i |= ( 1 << row );
//					}
//					// The page has a stored track mute patten (which may be activated)
//					else if ( GRID_p_selection[ row ]->trackMutepatternStored != 0 ){
//
//						j |= ( 1 << row );
//					}
//				}
//			} // Bank iterator

			// Overwrite any other assumptions from above
			i = GRID_mutepattern;			

			// Write the pattern to the mutator column
			MIR_write_buttool (RHS, i, MIR_RED);
			MIR_write_buttool (RHS, j, MIR_GREEN);

			break;



		case TRACK_MUTEPATTERN:

			#ifdef FEATURE_ENABLE_SONG_UPE
			// On the measure mutepattern
			if (G_on_the_measure_trackMutepattern != 0) {
				MIR_write_buttool (RHS, G_on_the_measure_trackMutepattern, MIR_RED);
				MIR_write_buttool (RHS, G_on_the_measure_trackMutepattern, MIR_GREEN);
				MIR_write_buttool (RHS, G_on_the_measure_trackMutepattern, MIR_BLINK);
				MIR_write_buttool (RHS, target_page->trackMutepattern, MIR_RED);
			}
			else
			{
				MIR_write_buttool (RHS, target_page->trackMutepattern, MIR_RED);
			}
			#else
				MIR_write_buttool (RHS, target_page->trackMutepattern, MIR_RED);	
			#endif

			break;
			


		case TRACK_SOLOPATTERN:
			MIR_write_buttool (RHS, target_page->trackSolopattern, MIR_GREEN);
			// MIR_write_buttool (RHS, target_page->trackSolopattern, MIR_BLINK);
			break;
		
		
		
		case TRACK_FUNCTIONS:

			// Functions from Panel
			// If in track zoom mode, turn off green light of zoom button
			if ( G_zoom_level == zoomTRACK ){
			
				bitpattern = G_PANEL_matrix[j][ENTITY] ^ ZOOM;
			
				// If the track is muted turn off green light of toggle
				for (i=0; i<MATRIX_NROF_ROWS; i++) {
					// Find the selected track
					if ( (target_page->trackSelection & (1<<i)) > 0 ) {
						//  row is index of the (first) selected track
						row = i;
					}
				}
			
				// Turn off the green part of toggle light
				if ((target_page->trackMutepattern & (1 << row )) != 0) {
					bitpattern ^= TGGL;
				}							
				// Write the pattern
				MIR_write_buttool (	RHS, bitpattern, MIR_GREEN);
			}
			
			// Default mode for green pattern
			else {
				MIR_write_buttool (	RHS, G_PANEL_matrix[j][ENTITY], MIR_GREEN);
			}

			// Write the red pattern (plain)
			MIR_write_buttool (	RHS, G_PANEL_matrix[j][ENTITY], MIR_RED);

			// PASTE: Check if buffer and target selection have the same cardinality
			if ((TRACK_COPY_BUFFER != 0) && 
				(my_bit_cardinality( TRACK_COPY_BUFFER & 0x3FF ) == 
					my_bit_cardinality( target_page->trackSelection ) ) 
				){

				MIR_write_buttool( RHS, PASTE, MIR_GREEN );
				MIR_write_buttool( RHS, PASTE, MIR_RED );
			}


			// SOLO: If the track is soloed, blink the SOLO LED
			for (row=0; row<MATRIX_NROF_ROWS; row++) {
				if ( (((i<<row) & target_page->trackSolopattern) != 0) && 
					 (((i<<row) & target_page->trackSelection)   != 0)    ) {
					MIR_write_dot (KEY_SOLO, MIR_BLINK);
				}
			}			
					

			// Show the EFF status
			switch( Track_get_MISC( target_page->Track[ my_bit2ndx( target_page->trackSelection )], EFF_BIT ) ){
				
				case OFF:
					// Nothing to do
					break;

				case SEND:
					MIR_write_dot (KEY_EFF, MIR_GREEN);
					break;
					
				case RECEIVE:
					MIR_write_dot (KEY_EFF, MIR_RED);
					break;

				case RECEIVESEND:
					MIR_write_dot (KEY_EFF, MIR_GREEN);
					MIR_write_dot (KEY_EFF, MIR_RED);
					break;
			}


			break;		
			
			

		case TRACK_ATTRIBUTE_FUNCTIONS:
			// Build the bitpattern mask
			bitpattern = 0x3ff;

			// Compute the functions from the panel: Intersection of functions of all atributes
			for (i=0; i<MATRIX_NROF_ROWS; i++) {

				// If the attribute is selected
				if ((target_page->trackAttributeSelection & (1<<i)) != OFF) {	

					bitpattern &= G_PANEL_matrix[j][i];
				}
			}
			
			MIR_write_buttool (RHS, bitpattern, MIR_GREEN);
			MIR_write_buttool (RHS, bitpattern, MIR_RED);

			break;



		case STEP_ATTRIBUTES_MAPS:
			// Get the row index of the selected track
			row = my_bit2ndx( target_page->trackSelection );

			// If there is a selected step in the selected track
			if (Page_get_selection_trackpattern( target_page, row ) != 0) {

				// Fetch pattern of operable EDITOR Rotarys for STEP MODIFY
				bitpattern = PANEL_getAttributeMap( STEP_S, MODIFY );

				// Show the available rotarys
				// MIR_write_buttool (RHS, bitpattern, MIR_RED);
				MIR_write_buttool (RHS, bitpattern, MIR_RED);

				// Blink the currently selected attribute
				// MIR_write_buttool 
				//		(RHS, target_page->trackAttributeSelection, MIR_BLINK);
			}
			else {
				// Show the available functions for the map itself
				// MAP Position editor
				// MIR_write_buttool( RHS, 1<<POSITION, MIR_GREEN );
				MIR_write_buttool( RHS, 1<<POSITION, MIR_RED   );

				// MAP Functions: CLEAR and RANDOMIZE
				MIR_write_buttool( RHS, 1<<LENGTH | 1<<START, MIR_GREEN );						
			}
			break;
				
				
				
		case TRACK_ATTRIBUTES_MAP_FUNCTIONS:
			// Very dirty.. uses hand-coded bitmaps pertaining to sel column. Oh well..
			// No groove in green because this is the zoom back message. Quick+D

			// Determine whether to show ZOM in or ZOM out. Watch GROOVE entry below
			// for the unseeming 'i' at green entry.
			if ( target_page->stepSelection == 1 ){ 
				i = 0;
				// i = 1; // ..disabled this to reduce confusion
			}
			else { 
				i = 0; 
			}

			// Depending on which map we are looking at..
			switch( target_page->trackAttributeSelection ){
			
				case (1 << DIRECTION):

					// Identify the track currently selected
					j = my_bit2ndx( target_page->trackSelection );

					// The first 5 directions are not editable
					if ( target_page->Track[j]->attr_DIR > 5 ){

						MIR_write_buttool( 	RHS, 
											1<<LENGTH | 1<<START, 
											MIR_GREEN );

						MIR_write_buttool( 	RHS, 
											1<<LENGTH | 1<<START,
											MIR_RED   );
					}

					MIR_write_buttool( 	RHS, 
										1<<GROOVE, 
										MIR_RED   );
					break;					
			
				default:
					MIR_write_buttool( 	RHS, 
						1<<START | i<<GROOVE | 1<<LENGTH | 1<<DIRECTION, 
										MIR_GREEN );
					MIR_write_buttool( 	RHS, 
						1<<START | 1<<GROOVE | 1<<LENGTH | 1<<DIRECTION, 
										MIR_RED   );
					break;
			}
			break;



		case STEP_FUNCTIONS:
			
			// Check whether a stepAttributeSelection is active.
			// If so, do not use zoom. Use 'i' as a switch further on.
			switch( target_page->stepAttributeSelection ){
				case 0:
					// Basically has no effect, we show the ZOOM.
					i = 0;
					break;

				default:
					// Does not show the ZOOM and TGGL function
					i = 0x3ff;
					break;
			}

			// Show step functions from panel. Single and multiple					
			if (target_page->stepSelection == 1){

				// If in step zoom mode, color zoom button red (by not lighting it green)
				if ( 	( G_zoom_level == zoomSTEP ) 
					&&  ( target_page->stepAttributeSelection == 0 )
					){

					MIR_write_buttool (	RHS, 
										G_PANEL_matrix[STEP_S][ENTITY] ^ ZOOM, 
										MIR_GREEN);							
				}

				// Default mode. Do not show zoom and tggl when an attribute is selected.
				else {
					MIR_write_buttool (	RHS, 
										G_PANEL_matrix[STEP_S][ENTITY] ^ ( (ZOOM|TGGL|COPY) & i ), 
										MIR_GREEN);
				}
				
				// Show the regular functions from panel
				MIR_write_buttool (	RHS, 
									G_PANEL_matrix[STEP_S][ENTITY] ^ ( (ZOOM|TGGL|COPY) & i ), 
									MIR_RED);

				// PASTE LED: Check if buffer and target selection have 
				// ..the appropriate cardinality
				if ( STEP_COPY_BUFFER != NULL ) {

					MIR_write_buttool( RHS, PASTE, MIR_GREEN );
					MIR_write_buttool( RHS, PASTE, MIR_RED );
				}

			}
			
			// Several steps are selected
			else if (target_page->stepSelection > 1){
			
				MIR_write_buttool (RHS, G_PANEL_matrix[STEP_M][ENTITY] ^ (TGGL & i), MIR_GREEN);
				MIR_write_buttool (RHS, G_PANEL_matrix[STEP_M][ENTITY] ^ (TGGL & i), MIR_RED);
			}

			break;



		case KEY_SOLO:
			// If the track is not soloed, light. If it is soloed, blink.
			if (target_page->trackSolopattern & target_page->trackSelection) {
				MIR_write_dot (KEY_SOLO, MIR_GREEN);
				MIR_write_dot (KEY_SOLO, MIR_BLINK);
			}
			else {
				MIR_write_dot (KEY_SOLO, MIR_GREEN);
			}
			break;


	
		case PAGE_FUNCTIONS:
			// Show functions available for page under cursor
			// ..without the ZOOM key here. it is handled by the GRID code,
			// ..as it has a GRID semantic.

			bitpattern = G_PANEL_matrix[PAGE][ENTITY] & (0x3ff^ZOOM);

			if ( target_page->page_clear == ON ){
				// Do not show the CLEAR function available
				bitpattern &= ( 0x3ff ^ CLEAR );
			}
			
			MIR_write_buttool ( RHS, bitpattern, MIR_GREEN );
			MIR_write_buttool ( RHS, bitpattern, MIR_RED );

			// ..and add the PASTE key if necessary
			if (PAGE_COPY_BUFFER != GRID_SELECTION_EMPTY) {
				MIR_write_dot( LED_PASTE, MIR_RED );
				MIR_write_dot( LED_PASTE, MIR_GREEN );
			}					
			
			// SOLO LED: show only when page under cursor is selected
			if ((is_selected_in_GRID( &Page_repository[GRID_CURSOR] )) &&
				(Page_repository[GRID_CURSOR].page_clear == OFF) ) {
				MIR_write_dot( LED_SOLO, MIR_RED );
				MIR_write_dot( LED_SOLO, MIR_GREEN );						
			}
			
			// Find out whether there is something in the solo buffer
			j = FALSE;
			for ( i=0; i < GRID_NROF_BANKS; i++ ){

				// Check content of current buffer position
				if ( (GRID_p_selection_buffer[i] != NULL) ) {

					j = TRUE;
				}
			}	

			// Page solo condition: grid buffer is empty and page is active				
			if ( ( j == TRUE ) ) {

				MIR_write_dot( LED_SOLO, MIR_BLINK );
				MIR_write_dot( LED_SOLO, MIR_RED );
				MIR_write_dot( LED_SOLO, MIR_GREEN );						
			}

			break;


		case KEY_EFF:
			// empty. To be filled by effector code
			break;


	} // switch( content );

