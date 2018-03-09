
		// Differentiate between single and multi selection
		for (i=0; i<MATRIX_NROF_ROWS; i++){
			if ((VIEWER_page->trackSelection & (1<<i)) > 0) {
				if ((VIEWER_page->trackSelection ^ (1<<i)) > 0) { 
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
			
			case GRID_BANK_PLAYMODES:
				// Show the play modes of the individual banks: simple or chain
				MIR_write_buttool( LHS, GRID_bank_playmodes & 0x1FF, 			MIR_GREEN );
				MIR_write_buttool( LHS, (GRID_bank_playmodes ^ 0x3FF) & 0x1FF, 	MIR_RED );				
				break;			
			
			case GRID_TRIGGERMODES:
				// Show the triggermodes of the individual Grid lines
//				MIR_write_buttool( LHS, GRID_bank_triggermodes & 0x1FF, 			MIR_RED );
//				MIR_write_buttool( LHS, (GRID_bank_triggermodes ^ 0x3FF) & 0x1FF, 	MIR_GREEN );
				
				j = 0;
				for( i=0; i < GRID_NROF_BANKS; i++ ){
					if( GRID_bank_triggermode[i] == RED ){
						j |= 1 << i;
					}
				}
				MIR_write_buttool( LHS, j & 0x1FF, 				MIR_RED );
				MIR_write_buttool( LHS, (j ^ 0x3FF) & 0x1FF, 	MIR_GREEN );


				break;

			case GRID_MIX_ATTRIBUTE:
				MIR_write_buttool (LHS, GRID_mixAttribute, MIR_RED  );	
				MIR_write_buttool (LHS, GRID_mixAttribute, MIR_GREEN);
				break;
			

			case TRACK_EFF_STATUS:
				// Compute the patterns
				i=0; 
				j=0;
				for ( row=0; row<MATRIX_NROF_ROWS; row++ ){
					
					if( Track_get_MISC( VIEWER_page->Track[row], EFF_BIT ) == RECEIVE ){

						// Mark bit in the RED pattern
						i |= ( 1 << row );
					}
					else if( Track_get_MISC( VIEWER_page->Track[row], EFF_BIT ) == SEND ){

						// Mark bit ib the GREEN pattern
						j |= ( 1 << row );
					}
				}

				// Write the patterns
				MIR_write_buttool (LHS, i, MIR_RED  	);
				MIR_write_buttool (LHS, j, MIR_GREEN  	);
				break;

			case TRACK_REC_STATUS:
			 	MIR_write_buttool (LHS, VIEWER_page->track_REC_pattern, MIR_BLINK);
				MIR_write_buttool (LHS, VIEWER_page->track_REC_pattern, MIR_RED);	
				break;

			case TRACK_SELECTION:
				MIR_write_buttool (LHS, VIEWER_page->trackSelection, MIR_RED);	
				MIR_write_buttool (LHS, VIEWER_page->trackSelection, MIR_GREEN);
				MIR_write_buttool (LHS, VIEWER_page->trackSelection, MIR_BLINK);				

				// Add in blinking green the rest of the chain from a selected track
				if ( my_bit_cardinality( VIEWER_page->trackSelection ) == 1 ){

					j = 0;
					current_track = VIEWER_page->Track[my_bit2ndx( VIEWER_page->trackSelection )];
					for ( row=0; row < MATRIX_NROF_ROWS; row++ ){
							
						j |= 1 << ( row_of_track( VIEWER_page, current_track ) );
						current_track = current_track->chain_data[NEXT];
					}

					MIR_write_buttool (LHS, j, MIR_GREEN);
					// MIR_write_buttool (LHS, j, MIR_BLINK);
				}

				break;
	

			case TRACK_ATTRIBUTES_RED:
				MIR_write_buttool (LHS, PANEL_get_trackAttributeMap(), MIR_RED);
				break;

			case TRACK_ATTRIBUTES_MAPS:
				MIR_write_buttool (LHS, PANEL_get_trackAttributeMap(), MIR_GREEN);
				break;

			case TRACK_ATTRIBUTES_ALL:
				MIR_write_buttool (LHS, PANEL_get_attributes_all( TRACK_S ), MIR_GREEN);
				break;				

			case TRACK_ATTRIBUTE_SELECTION:
			 	MIR_write_buttool (LHS, VIEWER_page->trackAttributeSelection, MIR_BLINK);
				MIR_write_buttool (LHS, VIEWER_page->trackAttributeSelection, MIR_RED);					
				break;									


			case STEP_ATTRIBUTES_RED:
				MIR_write_buttool (LHS, PANEL_get_attributes_all( STEP_S ), MIR_RED);
				break;
			
			case STEP_ATTRIBUTES_GREEN:
				MIR_write_buttool (LHS, PANEL_get_attributes_all( STEP_S ), MIR_GREEN);
				break;

			case STEP_ATTRIBUTE_SELECTION:

				if ( zoom_level == zoomSTEP ){
					// Event model: show selected attribute in step
					MIR_write_buttool (
						LHS, 
						1 << (VIEWER_page->Step [VIEWER_page->stepSelectionSingleRow]
												[VIEWER_page->stepSelectionSingleCol]->event_data & 0x0F),
						MIR_BLINK );
					MIR_write_buttool (
						LHS, 
						1 << (VIEWER_page->Step [VIEWER_page->stepSelectionSingleRow]
												[VIEWER_page->stepSelectionSingleCol]->event_data & 0x0F),
						MIR_RED   );
				}
				else{
					// Legacy model:
					MIR_write_buttool (LHS, VIEWER_page->stepAttributeSelection, MIR_BLINK);
					MIR_write_buttool (LHS, VIEWER_page->stepAttributeSelection, MIR_RED);					
				}

				break;

			case STEP_ATTRIBUTES_ALL:

				if ( zoom_level == zoomSTEP ){
					// Event model: show all available attributes - except LEN and STA
					MIR_write_buttool (LHS, 0x3fF, MIR_GREEN);
					MIR_write_buttool (LHS, 0x3fF, MIR_RED  );
				}
				else{
					// Legacy model:
					MIR_write_buttool (LHS, PANEL_get_attributes_all( STEP_S ), MIR_GREEN);
				}
				break;


			case MIX_ATTRIBUTE:
				MIR_write_dot (VIEWER_page->mixAttribute, MIR_RED); 
				MIR_write_dot (VIEWER_page->mixAttribute, MIR_GREEN); 
				break;

			case MIX_ATTRIBUTE_SELECTED:
				MIR_write_dot (VIEWER_page->mixAttribute, MIR_RED);
				MIR_write_dot (VIEWER_page->mixAttribute, MIR_GREEN); 				
				MIR_write_dot (VIEWER_page->mixAttribute, MIR_BLINK); 
				break;

			case EDIT_ATTRIBUTE:
				MIR_write_dot (VIEWER_page->editAttribute, MIR_RED);
				MIR_write_dot (VIEWER_page->editAttribute, MIR_GREEN);
				MIR_write_dot (VIEWER_page->editAttribute, MIR_BLINK);
				break;



			case CC_MIXMAP_ATTRIBUTES:

				switch( zoom_level ){

					case zoomPAGE:
					
						// Show available maps
						MIR_write_buttool( LHS, 1<<6 | 1<<8 | 1<<9, MIR_GREEN );
		
						// Show selected map					
						switch( VIEWER_page->CC_MIXMAP_attribute ){
							
							case CC_MIXMAP_AMT:
								i = 1<<6;
								break;
							case CC_MIXMAP_MCC:
								i = 1<<8;
								break;
							case CC_MIXMAP_MCH:
								i = 1<<9;
								break;
						}
						MIR_write_buttool( LHS, i, MIR_RED   );
						MIR_write_buttool( LHS, i, MIR_BLINK );
		
						break;


					case zoomGRID:
					case zoomGRIDTRACK:

						// Show available maps
						MIR_write_buttool( LHS, 1<<6 | 1<<8 | 1<<9, MIR_GREEN );
		
						// Show selected map					
						switch(GRID_assistant_page->CC_MIXMAP_attribute ){
							
							case CC_MIXMAP_AMT:
								i = 1<<6;
								break;
							case CC_MIXMAP_MCC:
								i = 1<<8;
								break;
							case CC_MIXMAP_MCH:
								i = 1<<9;
								break;
						}
						MIR_write_buttool( LHS, i, MIR_RED   );
						MIR_write_buttool( LHS, i, MIR_BLINK );

						break;
				}

				break;
		}



