//
//
//
//	if ((rotNdx >= 11) && (rotNdx <= 20) ) {
//
//		// Restart MIX Timer - this enables the MIX per track view (mixed with page steps)
//		// ..maybe one day. 
//		// You can get the screen that you want if the target_page->mixMasterStatus is "red"
//		// Needs more digging
//		start_MIX_TIMER();
//
//		// Normalized index
//		trackNdx = rotNdx-11;
//
//		// Keep the index for some other processing (in viewer)
//		target_page->mixingTrack = trackNdx;
//		
//		// Act on MAP0 status - important for the MAP rotary activity
//		if( GRID_MAP0_status == ON ){
//			target_page = GRID_assistant_page;
//		}
//		else{
//			target_page = target_page;
//		}
//
//
//		switch( target_page->mixTarget ){
//			
//			case MIXTGT_ATR:
//				#include "rot_mixAttribute.h"
//				break;
//
//
//			case MIXTGT_VOL:
//				switch (target_page->mixMode) {
//					case INC:	// Positive action on selected tracks
//						modify_parameter( &target_page->MIXAMT_VOL[trackNdx], 
//							// 0, 127, direction, ON );
//							CC_MIN_AMOUNT, CC_MAX_AMOUNT, direction, ON, FIXED);
//													
//						MIDI_send( 	MIDI_CC, 
//									target_page->Track[trackNdx]->attribute[ATTR_MIDICH]
//									+ target_page->Track[trackNdx]->event_offset[ATTR_MIDICH],
//									7,
//									target_page->MIXAMT_VOL[trackNdx] 	);					
//						break;
//
//					case DEC:	// Negative action on un-selected tracks
//						for (i=0; i<MATRIX_NROF_ROWS; i++){								
//							if ( i != trackNdx) {
//								modify_parameter( &target_page->MIXAMT_VOL[i], 
//									// 0, 127, direction^0x03, OFF );
//									CC_MIN_AMOUNT, CC_MAX_AMOUNT, direction^0x03, OFF, FIXED);
//								
//								MIDI_send( 	MIDI_CC, 
//											target_page->Track[i]->attribute[ATTR_MIDICH]
//											+ target_page->Track[i]->event_offset[ATTR_MIDICH],
//											7,
//											target_page->MIXAMT_VOL[i] 	);
//							}
//						}
//						break;
//				}
//				break;
//
//
//			case MIXTGT_PAN:
//				switch (target_page->mixMode) {
//					case INC:	// Positive action on selected tracks
//						modify_parameter( &target_page->MIXAMT_PAN[trackNdx], 
//							// 0, 127, direction, ON );
//							CC_MIN_AMOUNT, CC_MAX_AMOUNT, direction, ON, FIXED);
//						
//						MIDI_send( 	MIDI_CC, 
//									target_page->Track[trackNdx]->attribute[ATTR_MIDICH]
//									+ target_page->Track[trackNdx]->event_offset[ATTR_MIDICH],
//									10,
//									target_page->MIXAMT_PAN[trackNdx] 	);					
//						break;
//
//					case DEC:	// Negative action on un-selected tracks
//						for (i=0; i<MATRIX_NROF_ROWS; i++){								
//							if ( i != trackNdx) {
//								modify_parameter( &target_page->MIXAMT_PAN[i], 
//									// 0, 127, direction^0x03, OFF );
//									CC_MIN_AMOUNT, CC_MAX_AMOUNT, direction^0x03, OFF, FIXED);
//								
//								MIDI_send( 	MIDI_CC, 
//											target_page->Track[i]->attribute[ATTR_MIDICH]
//											+ target_page->Track[i]->event_offset[ATTR_MIDICH],
//											10,
//											target_page->MIXAMT_PAN[i] 	);
//							}
//						}
//						break;
//				}
//				break;
//
//
//			case MIXTGT_MOD:
//				switch (target_page->mixMode) {
//					case INC:	// Positive action on selected tracks
//						modify_parameter( &target_page->MIXAMT_MOD[trackNdx], 
//							// 0, 127, direction, ON );
//							CC_MIN_AMOUNT, CC_MAX_AMOUNT, direction, ON, FIXED);
//						
//						MIDI_send( 	MIDI_CC, 
//									target_page->Track[trackNdx]->attribute[ATTR_MIDICH]
//									+ target_page->Track[trackNdx]->event_offset[ATTR_MIDICH],
//									1,
//									target_page->MIXAMT_MOD[trackNdx] 	);					
//						break;
//
//					case DEC:	// Negative action on un-selected tracks
//						for (i=0; i<MATRIX_NROF_ROWS; i++){								
//							if ( i != trackNdx) {
//								modify_parameter( &target_page->MIXAMT_MOD[i], 
//									// 0, 127, direction^0x03, OFF );
//									CC_MIN_AMOUNT, CC_MAX_AMOUNT, direction^0x03, OFF, FIXED);
//								
//								MIDI_send( 	MIDI_CC, 
//											target_page->Track[i]->attribute[ATTR_MIDICH]
//											+ target_page->Track[i]->event_offset[ATTR_MIDICH],
//											1,
//											target_page->MIXAMT_MOD[i] 	);
//							}
//						}
//						break;
//				}
//				break;
//
//
//			case MIXTGT_EXP:
//				switch (target_page->mixMode) {
//					case INC:	// Positive action on selected tracks
//						modify_parameter( &target_page->MIXAMT_EXP[trackNdx], 
//							// 0, 127, direction, ON );
//							CC_MIN_AMOUNT, CC_MAX_AMOUNT, direction, ON, FIXED);
//						
//						MIDI_send( 	MIDI_CC, 
//									target_page->Track[trackNdx]->attribute[ATTR_MIDICH]
//									+ target_page->Track[trackNdx]->event_offset[ATTR_MIDICH],
//									11,
//									target_page->MIXAMT_EXP[trackNdx] 	);					
//						break;
//
//					case DEC:	// Negative action on un-selected tracks
//						for (i=0; i<MATRIX_NROF_ROWS; i++){								
//							if ( i != trackNdx) {
//								modify_parameter( &target_page->MIXAMT_EXP[i], 
//									// 0, 127, direction^0x03, OFF );
//									CC_MIN_AMOUNT, CC_MAX_AMOUNT, direction^0x03, OFF, FIXED);
//								
//								MIDI_send( 	MIDI_CC, 
//											target_page->Track[i]->attribute[ATTR_MIDICH]
//											+ target_page->Track[i]->event_offset[ATTR_MIDICH],
//											11,
//											target_page->MIXAMT_EXP[i] 	);
//							}
//						}
//						break;
//				}
//				break;
//
//
//			case MIXTGT_USR0:
//			case MIXTGT_USR1:
//			case MIXTGT_USR2:
//			case MIXTGT_USR3:
//			case MIXTGT_USR4:
//			case MIXTGT_USR5:
//
//				// In the Page mode - only send MCC, to not confuse the user
//				if ( zoom_level == zoomPAGE ){
//
//					// Modify the parameter at hand
//					modify_signed_parameter( 
//							   	&target_page->CC_MIXMAP 	[target_page->mixTarget][trackNdx]
//															[CC_MIXMAP_AMT], 
//									// 0, 127, direction, ON );
//									CC_MIN_AMOUNT, CC_MAX_AMOUNT, direction, ON, FIXED);
//						
//					// Send MIDICC unless there is no controller selected			
//					if ( ( target_page->CC_MIXMAP[target_page->mixTarget][trackNdx][CC_MIXMAP_MCC]
//							!= (unsigned char) MIDICC_NONE )
//						){
//
//						MIDI_send( 	MIDI_CC, 
//								target_page->CC_MIXMAP  [target_page->mixTarget][trackNdx]
//														[CC_MIXMAP_MCH],
//								target_page->CC_MIXMAP	[target_page->mixTarget][trackNdx]
//														[CC_MIXMAP_MCC],
//								target_page->CC_MIXMAP	[target_page->mixTarget][trackNdx]
//														[CC_MIXMAP_AMT]   );
//					}
//				}
//				else if ( zoom_level == zoomMIXMAP ){
//					
//					// This applies to all other modes
//					switch( target_page->CC_MIXMAP_attribute ) {
//
//						case CC_MIXMAP_AMT:
//							modify_signed_parameter( 
//									   	&target_page->CC_MIXMAP 	[target_page->mixTarget][trackNdx]
//																	[CC_MIXMAP_AMT], 
//										CC_MIN_AMOUNT, CC_MAX_AMOUNT, direction, ON, FIXED);
//											
//							// Send MIDICC unless there is no controller selected			
//							if ( ( target_page->CC_MIXMAP	[target_page->mixTarget]
//															[trackNdx][CC_MIXMAP_MCC]
//										!= (unsigned char) MIDICC_NONE )
//								){
//
//								MIDI_send( 	MIDI_CC, 
//										target_page->CC_MIXMAP  [target_page->mixTarget][trackNdx]
//																[CC_MIXMAP_MCH],
//										target_page->CC_MIXMAP	[target_page->mixTarget][trackNdx]
//																[CC_MIXMAP_MCC],
//										target_page->CC_MIXMAP	[target_page->mixTarget][trackNdx]
//																[CC_MIXMAP_AMT]   );					
//							}
//							break;
//
//						case CC_MIXMAP_MCC:
//							modify_signed_parameter( 
//									   	&target_page->CC_MIXMAP 	[target_page->mixTarget][trackNdx]
//																	[CC_MIXMAP_MCC], 
//											CC_MIN_MCC, CC_MAX_MCC, direction, ON, FIXED);
//							break;
//
//						case CC_MIXMAP_MCH:
//							modify_signed_parameter( 
//									   	&target_page->CC_MIXMAP 	[target_page->mixTarget][trackNdx]
//																	[CC_MIXMAP_MCH], 
//											CC_MIN_MCH, CC_MAX_MCH, direction, ON, FIXED);
//							break;
//					}
//				}
//				break;
//
//		}
//	} // rotNdx part of MIXER
//	
//	
//
