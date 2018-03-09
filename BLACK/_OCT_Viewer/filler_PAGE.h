

	//
	// P A G E  MODE
	//
	switch (VIEWER_page->keyMode) {


		case KMOD_PAGE:

			// Base functionality of the page mode
			#include "fill_PAGE.h"

			// NO SELECTION.
			if (	(VIEWER_page->trackSelection == 0) 
				&& 	(VIEWER_page->stepSelection  == 0)
				){

				// The PAGE contains no active selection
				#include "fill_PAGE_sel_NONE.h"
			}

			// TRACK SELECTION
			else if ( VIEWER_page->trackSelection != 0 ){

				#include "fill_PAGE_sel_TRACK.h"
			}		

			// STEP SELECTION
			else if ( VIEWER_page->stepSelection != 0 ){

				#include "fill_PAGE_sel_STEP.h"
			}

						
			// TRACK SELECTORS
			if (EDIT_TIMER == ON) {

				// Show the current track selection				
				if (VIEWER_page->trackSelection != 0) {

					show (ELE_TRACK_SELECTORS, TRACK_SELECTION);
				}
			}
			else {
				// Default mode
				if (VIEWER_page->trackSelection != 0) {

					show (ELE_TRACK_SELECTORS, TRACK_SELECTION);
				}
								
				// Step selection existing
				else if ( VIEWER_page->stepSelection != 0 ){

					// Show the available and selected step attribute
					show( ELE_TRACK_SELECTORS, STEP_ATTRIBUTES_ALL );
					show( ELE_TRACK_SELECTORS, STEP_ATTRIBUTE_SELECTION );
				}

				// No selection active
				else {

					if (VIEWER_page->mixTarget == MIXTGT_ATR) {

						if ( VIEWER_page->OPS_mode != BIRDSEYE ){
							show (ELE_TRACK_SELECTORS, MIX_ATTRIBUTE);		
						}
					}

					// Show the track EFF Status
					show( ELE_TRACK_SELECTORS, TRACK_EFF_STATUS );

					// Show recording mode: here traditional
					if ( VIEWER_page->track_REC_pattern != 0 ){
						
						MIR_write_dot( LED_RECORD, MIR_RED );

						// Blink when no ON notes are pending..
						if ( NOTE_queue_counter == 0 ){
							MIR_write_dot( LED_RECORD, MIR_BLINK );						
						}
					}
					// Step recording
					else if ( VIEWER_page->REC_bit == ON ){
					
						MIR_write_dot( LED_RECORD, MIR_RED 	 );
						MIR_write_dot( LED_RECORD, MIR_GREEN );
						
						// Blink when no ON notes are pending..
						if ( NOTE_queue_counter == 0 ){
							MIR_write_dot( LED_RECORD, MIR_BLINK );						
						}
					}

					// Show the REC status of tracks
					show( ELE_TRACK_SELECTORS, TRACK_REC_STATUS );
				}
			}

			break;




	//
	// M I X E R  MAP MODE
	//
		case KMOD_MIXER_MAP:
			//
			// MIX INDICATOR - LED corresponding to the rotarys
			//
			show( ELE_MIX_INDICATOR, RED   );
			show( ELE_MIX_INDICATOR, GREEN );
			// show( ELE_MIX_INDICATOR, BLINK );

			//
			// MIX MASTER
			//
			switch (VIEWER_page->mixMode) {
				case INC:
					show (ELE_MIX_MASTER, GREEN);
					break;
					
				case DEC:
					show (ELE_MIX_MASTER, RED);
					break;
			}

			//
			// TRACK_SELECTORS
			//
			switch( VIEWER_page->mixTarget ){
	
				case MIXTGT_ATR:
					// Selectable Attribute Set
					show (ELE_TRACK_SELECTORS, TRACK_ATTRIBUTES_ALL);

					// Selected attribute
					show (ELE_TRACK_SELECTORS, MIX_ATTRIBUTE_SELECTED);		
					
					// ALIGN LED
					MIR_write_dot( LED_ALIGN, MIR_GREEN );		
					break;

				case MIXTGT_USR1:
				case MIXTGT_USR2:
				case MIXTGT_USR3:
				case MIXTGT_USR4:
				case MIXTGT_USR5:
					// Show the available attributes of the CC_MIXMAP
					show( ELE_TRACK_SELECTORS, CC_MIXMAP_ATTRIBUTES );
					break;

				// These dont show anything for now
				case MIXTGT_VOL:
				case MIXTGT_PAN:
				case MIXTGT_MOD:
				case MIXTGT_EXP:
					break;
			}

			switch( GRID_MAP0_status ){		
				case ON:
					MIR_write_dot( LED_MIXTGT_USR0, MIR_RED   );
					// MIR_write_dot( LED_MIXTGT_USR0, MIR_GREEN );
					break;
	
				case OFF:
					MIR_write_dot( LED_MIXTGT_USR0, MIR_GREEN );
					break;
			}
			

			// SELECT MASTER
			// 
			// see TRACK SELECTORS


			// MIX TARGET field
			//
			show( ELE_MIXTGT, OPTIONS );


			// MATRIX
			//
			show( ELE_MATRIX, MIXAMT_VALS );
								

			// EDIT MASTER
			switch( VIEWER_page->editorMode ){
	
				case CONTROL:		
					// Edit acts as MCC source - send MCC data
					show( ELE_EDIT_MASTER, GREEN );
					show( ELE_EDIT_MASTER, RED );
					break;

				case EDIT:
					// This is the regular mode - edit attributes of selection
					show( ELE_EDIT_MASTER, GREEN );
					break;

				case PREVIEW:
					// Same as regular mode, but preview the steps being edited
					show( ELE_EDIT_MASTER, RED );
					break;	

				case MIX2EDIT:
					show( ELE_EDIT_INDICATOR, GREEN );
					show( ELE_EDIT_INDICATOR, RED );
					break;
			}
				
			
			// MUTATORS - when ATR is selected
			if ( VIEWER_page->mixTarget == MIXTGT_ATR ){

				// RND
				MIR_write_dot( LED_RANDOMIZE, MIR_GREEN );
				MIR_write_dot( LED_RANDOMIZE, MIR_RED );
												
				// CLR
				MIR_write_dot( LED_CLEAR, MIR_GREEN );
				MIR_write_dot( LED_CLEAR, MIR_RED );
	
				// CPY
				MIR_write_dot( LED_COPY, MIR_GREEN );
				MIR_write_dot( LED_COPY, MIR_RED );
				
				// PST
				// Make sure we have a valid buffer on store..
				if ( ATTR_MAP_COPY_BUFFER[10] == VIEWER_page->mixAttribute ){
				
					MIR_write_dot( LED_PASTE, MIR_GREEN );
					MIR_write_dot( LED_PASTE, MIR_RED );			
				}
			}
	
	
			// RETURN
			show (ELE_RETURN, GREEN);


			// GLOBALS
			//
			show (ELE_GLOBALS, TRANSPORT);	
			show (ELE_GLOBALS, NUMERIC_QUADRANT);
			show (ELE_GLOBALS, BIG_KNOB);
			

			// SCALE MASTER
			show_GLOBALS_scale_master( VIEWER_page );

			// OCTAVE CIRCLE
			// In PAGE mode show the SCALE, trans-position is in GRID
			if (	( VIEWER_page->scaleStatus == SCALE_SEL )
				||	( VIEWER_page->scaleStatus == SCALE_MOD ) 
				){
				
				// Show the selected notes in scale. Both in MOD and SEL
				// show (ELE_OCTAVE_CIRCLE, SCALE_SELECTION);
				show_OCTAVE_CIRCLE_scale_selection( VIEWER_page );

				// if ( VIEWER_page->scaleStatus == SCALE_SEL ){
					// This allows for editing to be done. No need otherwise.
				show_SCALE_SELECTOR_scale_selection( VIEWER_page );
				// }

			}

			// When forced to scale, show the scale
			if ( VIEWER_page->force_to_scale == ON ){
				// Show the selected notes in scale. Both in MOD and SEL
				// show (ELE_OCTAVE_CIRCLE, SCALE_SELECTION);
				show_OCTAVE_CIRCLE_scale_selection( VIEWER_page );
				// show_OCTAVE_CIRCLE_scale_align( VIEWER_page );
			}		


			break;

	
	//
	// T H I S   P A G E
	//			
		case KMOD_THIS_PAGE:
			show (ELE_RETURN, GREEN);
			break;


	
	//
	// C A D E N C E 
	//
		case KMOD_CADENCE_EDIT:
		
			// MIX MASTER
			//
			// ..no function for now
													
			
			// MIX TARGET field
			//
			// ..no function for now
			
			
			// TRACK SELECTORS
			//
			// ..no function for now

			
			// SELECT MASTER 
			//
			// ..no function for now


			// MUTE MASTER
			//
			// ..no function for now
			

			// TRACK CHAINING
			//
			show( ELE_CHAIN_INDICATOR, CADENCE_STATUS );


			// MATRIX
			//		
			show( ELE_MATRIX, CADENCE_STATUS );


			// TRACK MUTATORS
			//
			show( ELE_TRACK_MUTATORS, CADENCE_STATUS );

			// EDIT MASTER
			//
			// ..no function for now


			// EDIT INDICATOR
			//
			// ..no function for now
			

			// RETURN
			//
			if ((VIEWER_page->trackSelection != 0) ||
				(VIEWER_page->stepSelection  != 0) || 
				(VIEWER_page->scaleStatus != OFF )   ) {

				show (ELE_RETURN, GREEN);
			}

			// GLOBALS
			//
			show (ELE_GLOBALS, TRANSPORT);	
			show (ELE_GLOBALS, NUMERIC_QUADRANT);
			show (ELE_GLOBALS, BIG_KNOB);
			show( ELE_GLOBALS, CLOCK );
			
			// SCALE MASTER
			show_GLOBALS_scale_master( VIEWER_page );
			
			// OCTAVE CIRCLE
			// In PAGE mode show the SCALE, trans-position is in GRID
			show( ELE_OCTAVE_CIRCLE, SELECTED_CADENCE_KEY );
			
			// CHORD SELECTOR
			show( ELE_CHORD_SELECTOR, SELECTED_CADENCE_KEY );

			// SCALE SELECTOR
			show( ELE_SCALE_SELECTOR, SELECTED_CADENCE_KEY );

			break;


	} // switch (VIEWER_page->keymode)



