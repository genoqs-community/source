



	//
	// OCTAVE CIRCLE
	//
	# include "key_OCT_CIRCLE_xpose_PAGE.h"
	
	
	// GRID FTS EXEMPT
	if (	( keyNdx == KEY_SCALE_SEL )
		){
		
		// Toggle the status
		VIEWER_page->grid_FTS_exempt ^= 1;
	}


	// SELECTORS
	if ((keyNdx >0) && (keyNdx <= 10)) {

		// Toggle the playmodes for the GRID bank.
		GRID_bank_playmodes ^= 1 << (keyNdx-1);
	}
	

	//
	// MATRIX KEYS
	//
	// Select the page corresponding to the key press as the new VIEWER page..
	if ( (keyNdx >= 11) && (keyNdx <= 185) ) { 
		
		switch( keyNdx ){
			
			// These don't belong to the matrix. Only accidentally in the range.
			case 21: 	case 32: 	case 43: 	case 54: 
			case 65: 	case 76:	case 87: 	case 98: 
			case 109: 	case 120: 	case 131: 	case 142:
			case 153: 	case 164: 	case 175:
				// Do nothing
				break;
		
					
			default:
		
				// Compute the page index into the page repository first
				i = row_of(keyNdx) + (10 * column_of (keyNdx));
		
				// Decide what to do
				switch( i ){
				
					// GRID_set selectors. This key falls on a GRID SET
					case 9: 	case 19:	case 29:	case 39:
					case 49:	case 59:	case 69:	case 79:
					case 89:	case 99:	case 109:	case 119:
					case 129:	case 139:	case 149:	case 159:
					
						// Do nothing	
						break;
					
											
					// Page selectors, act as always..
					// ..select the page for play in preselection or zoom into it (doubleclick)
					default:

						VIEWER_page = &Page_repository[ i ];

						// Move the cursor. 
						GRID_CURSOR = VIEWER_page->pageNdx;

						zoom_level = zoomPAGE;
						VIEWER_page->keyMode = KMOD_PAGE;		
						VIEWER_page->trackSelection = OFF;
						VIEWER_page->trackAttributeSelection = OFF;
						
						break;
						
					} // switch( VIEWER_page->pageNdx );

		} // switch( keyNdx )

	} // keyNdx in MATRIX


	//
	// TRACK MUTATORS
	//
	if ((keyNdx >= 187) && (keyNdx <= 196)) {
		
		switch (keyNdx) {
			
			case TGGL_KEY:
				// Toggle the page playing status 
				
				// Classic toggle behavior
				switch ( is_selected_in_GRID( VIEWER_page ) ) {
					case ON:
						grid_select( VIEWER_page, OFF );
						break;

					case OFF:
						grid_select( VIEWER_page, ON  );
						break;
				}

				break;


			case SOLO_KEY:

				// All details were outsourced.. so they can be used elsewhere
				PAGE_toggle_solo();

				break;


			case CLEAR_KEY:
				// Select all tracks in cursor page
				Page_repository[GRID_CURSOR].trackSelection = 0x3FF;

				// Clear selected tracks in page
				Page_CLEAR_selected_tracks( &Page_repository[GRID_CURSOR] );

				PAGE_init( &Page_repository[GRID_CURSOR], Page_repository[GRID_CURSOR].pageNdx );

				// Assign the default pitches to the tracks 
				for ( row=0; row<MATRIX_NROF_ROWS; row++ ){

					(&Page_repository[GRID_CURSOR])->Track[row]->attribute[ATTR_PITCH] =
						TRACK_DEFAULT_PITCH[row];
				}


				// Unselect all tracks in cursor page
				Page_repository[GRID_CURSOR].trackSelection = OFF;

				// Mark page as cleared
				Page_repository[GRID_CURSOR].page_clear = ON;
			
				break;
				


			case RANDOMIZE_KEY:

				// Select all tracks in cursor page
				Page_repository[GRID_CURSOR].trackSelection = 0x3FF;

				// Randomize selected tracks in page - step status
				Page_RNDMZ_selected_tracks( &Page_repository[GRID_CURSOR] );
				
				// Unselect all tracks in cursor page
				Page_repository[GRID_CURSOR].trackSelection = OFF;
				
				// Mark page as touched
				Page_repository[GRID_CURSOR].page_clear = OFF;
				break;



			case FLT_KEY:
				// Only active pages can be flipped. Break on inactive page
				if (is_selected_in_GRID( &Page_repository[GRID_CURSOR] ) == OFF){
					break;
				}

				// Select all tracks in cursor page
				Page_repository[GRID_CURSOR].trackSelection = 0x3FF;

				// Randomize selected tracks in page - step status
				Page_FLT_selected_tracks( &Page_repository[GRID_CURSOR] );
				
				// Unselect all tracks in cursor page
				Page_repository[GRID_CURSOR].trackSelection = OFF;
				
				// Mark page as touched
				Page_repository[GRID_CURSOR].page_clear = OFF;
				break;



			case RMX_KEY:
				// Only active pages can be rmx'ed. Break on inactive page
				if (is_selected_in_GRID( &Page_repository[GRID_CURSOR] ) == OFF){
					break;
				}

				// Select all tracks in cursor page
				Page_repository[GRID_CURSOR].trackSelection = 0x3FF;

				// Randomize selected tracks in page - step status
				Page_RMX_selected_tracks( &Page_repository[GRID_CURSOR] );
				
				// Unselect all tracks in cursor page
				Page_repository[GRID_CURSOR].trackSelection = OFF;
				
				// Mark page as touched
				Page_repository[GRID_CURSOR].page_clear = OFF;
				break;



			case ZOOM_KEY:
				// Zoom into the currently selected page
				VIEWER_page = &Page_repository[GRID_CURSOR];
				zoom_level = zoomPAGE;
				VIEWER_page->keyMode = KMOD_PAGE;
				break;


			case COPY_KEY:
				// Remember the index of the copied page
				PAGE_COPY_BUFFER = GRID_CURSOR;
				break;
										

			case PASTE_KEY:
				// Make sure a page was copied first
				if ( PAGE_COPY_BUFFER != GRID_SELECTION_EMPTY ){

					// Paste the copied page into page under the cursor
					Page_copy( 	&Page_repository[PAGE_COPY_BUFFER], 
								&Page_repository[GRID_CURSOR] 			);
				}				
				// Clear the page copy buffer
				PAGE_COPY_BUFFER = GRID_SELECTION_EMPTY;
				break; 

		} // switch (keyNdx)
	} // Track mutators





	


