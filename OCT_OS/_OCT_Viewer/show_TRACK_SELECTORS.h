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

			case GRID_BANK_PLAYMODES:
				// Show the play modes of the individual banks: simple or chain
				MIR_write_buttool( LHS, GRID_bank_playmodes & 0x1FF, 			MIR_GREEN );
				MIR_write_buttool( LHS, ( ( GRID_bank_playmodes ^ 0x3FF ) | GRID_assistant_page->trackMutepattern ) & 0x1FF, 	MIR_RED );
				MIR_write_buttool( LHS, ( GRID_bank_playmodes & GRID_assistant_page->trackMutepattern ) & 0x1FF, MIR_BLINK );
				break;

//			case GRID_TRIGGERMODES:
//				// Show the triggermodes of the individual Grid lines
//				MIR_write_buttool( LHS, GRID_bank_triggermodes & 0x1FF, 			MIR_RED );
//				MIR_write_buttool( LHS, (GRID_bank_triggermodes ^ 0x3FF) & 0x1FF, 	MIR_GREEN );
//
//				j = 0;
//				for( i=0; i < GRID_NROF_BANKS; i++ ){
//					if( GRID_bank_triggermode[i] == RED ){
//						j |= 1 << i;
//					}
//				}
//				MIR_write_buttool( LHS, j & 0x1FF, 				MIR_RED );
//				MIR_write_buttool( LHS, (j ^ 0x3FF) & 0x1FF, 	MIR_GREEN );
//				break;

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
			 	MIR_write_buttool (LHS, Page_getTrackRecPattern(target_page), MIR_BLINK);
				MIR_write_buttool (LHS, Page_getTrackRecPattern(target_page), MIR_RED);
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

					// Compute the green pattern - all tracks in chain
					for ( row=0; row < MATRIX_NROF_ROWS; row++ ){

						// If current track is not head, mark it as tail
						if ( current_track != current_track->chain_data[HEAD] ){

							j |= 1 << ( row_of_track( target_page, current_track ) );
						}

						// Advance current track
						current_track = current_track->chain_data[NEXT];
					}

					// Write in green the chain pattern ( comes without the head )
					MIR_write_buttool( 	LHS, j, MIR_GREEN );

					// Add in red the missing head
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

				if ( G_zoom_level == zoomSTEP ){

					// Event model: show selected attribute in step
					#ifdef FEATURE_STEP_EVENT_TRACKS
					unsigned char event_data_attr = APPLY_MASK( target_page->Step [target_page->stepSelectionSingleRow][target_page->stepSelectionSingleCol]->event_data, 0x0F ) + 1;
					// Finally point out the event in the row
					if( 	( ( event_data_attr == NEMO_ATTR_POSITION ) || ( event_data_attr == NEMO_ATTR_DIRECTION ) )
						// POS OR DIR ALT MODE
					&& 	( CHECK_BIT( target_page->Step[ target_page->stepSelectionSingleRow ]
													  [ target_page->stepSelectionSingleCol ]->attr_STATUS, STEP_EVENT_TRACK_ALT_MODE + 5 ) ) ) {

					MIR_write_buttool (
						LHS,
						1 << (target_page->Step [target_page->stepSelectionSingleRow]
												[target_page->stepSelectionSingleCol]->event_data & 0x0F),
												MIR_SHINE_GREEN );
					} else {
						MIR_write_buttool (
							LHS,
							1 << (target_page->Step [target_page->stepSelectionSingleRow]
													[target_page->stepSelectionSingleCol]->event_data & 0x0F),
													MIR_BLINK );
					}
					#else
					MIR_write_buttool (
						LHS,
						1 << (target_page->Step [target_page->stepSelectionSingleRow]
												[target_page->stepSelectionSingleCol]->event_data & 0x0F),
												MIR_BLINK );
					#endif

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

				if ( G_zoom_level == zoomSTEP ){
					// Event model: show all available attributes - except LEN and STA
					MIR_write_buttool (LHS, 0x3FF, MIR_GREEN);
#ifdef EVENTS_FACTORIZED
					MIR_write_buttool (LHS, 0x1CF, MIR_RED  );
#endif // EVENTS_FACTORIZED
#ifdef EVENTS_ABSOLUTE
					MIR_write_buttool (LHS, 0x1CC, MIR_RED  );
#endif // EVENTS_ABSOLUTE

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

				switch( G_zoom_level ){

					case zoomPAGE:
					case zoomMIXMAP:

						// Show available maps
						if ( G_zoom_level == zoomMIXMAP ){
							MIR_write_buttool( LHS, 1<<6 | 1<<8 | 1<<9, MIR_GREEN );
						}

						// if ( GRID_MAP0_status == ON ){
							target_page = GRID_assistant_page;
						// }

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

						if ( G_zoom_level == zoomMIXMAP ){
							MIR_write_buttool( LHS, i, MIR_RED   );
							MIR_write_buttool( LHS, i, MIR_BLINK );
						}

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



