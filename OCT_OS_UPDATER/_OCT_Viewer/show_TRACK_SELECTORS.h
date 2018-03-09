


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
				MIR_write_buttool( LHS, GRID_mixAttribute, MIR_RED   );	
				MIR_write_buttool( LHS, GRID_mixAttribute, MIR_GREEN );
				MIR_write_buttool( LHS, GRID_mixAttribute, MIR_BLINK );
				break;
			

			case TRACK_EFF_STATUS:
				// Compute the patterns
				i=0; 
				j=0;
				for ( row=0; row<MATRIX_NROF_ROWS; row++ ){
					
					if( Track_get_MISC( target_page->Track[row], EFF_BIT ) == RECEIVE ){

						// Mark bit in the RED pattern
						i |= ( 1 << row );
					}
					else if( Track_get_MISC( target_page->Track[row], EFF_BIT ) == SEND ){

						// Mark bit in the GREEN pattern
						j |= ( 1 << row );
					}
					else if( Track_get_MISC( target_page->Track[row], EFF_BIT ) == RECEIVESEND ){

						// Mark bit in the RED pattern
						i |= ( 1 << row );
						// Mark bit in the GREEN pattern
						j |= ( 1 << row );
					}
				}

				// Write the patterns
				MIR_write_buttool (LHS, i, MIR_RED  	);
				MIR_write_buttool (LHS, j, MIR_GREEN  	);
				break;

			case TRACK_REC_STATUS:
			 	MIR_write_buttool (LHS, target_page->track_REC_pattern, MIR_BLINK);
				MIR_write_buttool (LHS, target_page->track_REC_pattern, MIR_RED);	
				break;

			case TRACK_SELECTION:
				MIR_write_buttool (LHS, target_page->trackSelection, MIR_RED);	
				MIR_write_buttool (LHS, target_page->trackSelection, MIR_GREEN);
				MIR_write_buttool (LHS, target_page->trackSelection, MIR_BLINK);				

				// Add in green the rest of the chain from a selected track
				// Add in red the head track for the selected track
				if ( my_bit_cardinality( target_page->trackSelection ) == 1 ){

					j = 0;	// Pattern of the chain

					// Identify the currently selected track
					current_track = target_page->Track[my_bit2ndx( target_page->trackSelection )];

					for ( row=0; row < MATRIX_NROF_ROWS; row++ ){
							
						j |= 1 << ( row_of_track( target_page, current_track ) );
						current_track = current_track->chain_data[NEXT];
					}

					// Write in green the chain pattern without the head
					j &= ( 0x3f ^ ( 1 << row_of_track( target_page, current_track->chain_data[HEAD] )) );
					MIR_write_buttool( 	LHS, j, MIR_GREEN );

					// Write in red the chain head
					MIR_write_buttool( 	LHS, 
										1 << row_of_track( target_page, current_track->chain_data[HEAD] ), 
										MIR_RED );
				}

				break;
	

			case TRACK_ATTRIBUTES_RED:
				MIR_write_buttool (LHS, PANEL_get_trackAttributeMap(), MIR_RED);
				break;

			case TRACK_ATTRIBUTES_MAPS:
				MIR_write_buttool ( LHS, PANEL_get_trackAttributeMap(), MIR_GREEN 	);
				MIR_write_buttool ( LHS, PANEL_get_trackAttributeMap(), MIR_RED 	);
				break;

			case TRACK_ATTRIBUTES_ALL:
				MIR_write_buttool (LHS, PANEL_get_attributes_all( TRACK_S ), MIR_GREEN);
				break;				

			case TRACK_ATTRIBUTE_SELECTION:
			 	MIR_write_buttool (LHS, target_page->trackAttributeSelection, MIR_BLINK);
				MIR_write_buttool (LHS, target_page->trackAttributeSelection, MIR_RED);					
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
						1 << (target_page->Step [target_page->stepSelectionSingleRow]
												[target_page->stepSelectionSingleCol]->event_data & 0x0F),
						MIR_BLINK );
//					MIR_write_buttool (
//						LHS, 
//						1 << (target_page->Step [target_page->stepSelectionSingleRow]
//												[target_page->stepSelectionSingleCol]->event_data & 0x0F),
//						MIR_RED   );
				}
				else{
					// Legacy model:
					MIR_write_buttool (LHS, target_page->stepAttributeSelection, MIR_BLINK);
					// MIR_write_buttool (LHS, target_page->stepAttributeSelection, MIR_RED);					
				}

				break;

			case STEP_ATTRIBUTES_ALL:

				if ( zoom_level == zoomSTEP ){
					// Event model: show all available attributes - except LEN and STA
					MIR_write_buttool (LHS, 0x3ff, MIR_GREEN);
					MIR_write_buttool (LHS, 0x1Cf, MIR_RED  );

					// Light only the attributes that have a map
					// MIR_write_buttool( LHS, 0x18f, MIR_GREEN 	);
					// MIR_write_buttool( LHS, 0x18f, MIR_RED 		);
				}
				else{
					// Legacy model:
					MIR_write_buttool (LHS, PANEL_get_attributes_all( STEP_S ), MIR_GREEN);
				}
				break;


			case MIX_ATTRIBUTE:
				MIR_write_dot (target_page->mixAttribute, MIR_RED); 
				MIR_write_dot (target_page->mixAttribute, MIR_GREEN); 
				MIR_write_dot (target_page->mixAttribute, MIR_BLINK); 
				break;

			case MIX_ATTRIBUTE_SELECTED:
				MIR_write_dot (target_page->mixAttribute, MIR_RED);
				MIR_write_dot (target_page->mixAttribute, MIR_GREEN); 				
				MIR_write_dot (target_page->mixAttribute, MIR_BLINK); 
				break;

			case EDIT_ATTRIBUTE:
				MIR_write_dot (target_page->editAttribute, MIR_RED);
				MIR_write_dot (target_page->editAttribute, MIR_GREEN);
				MIR_write_dot (target_page->editAttribute, MIR_BLINK);
				break;



			case CC_MIXMAP_ATTRIBUTES:

				switch( zoom_level ){

					case zoomPAGE:
					case zoomMIXMAP:
					
						// Show available maps
						MIR_write_buttool( LHS, 1<<6 | 1<<8 | 1<<9, MIR_GREEN );
		
						// Show selected map					
						switch( target_page->CC_MIXMAP_attribute ){
							
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



