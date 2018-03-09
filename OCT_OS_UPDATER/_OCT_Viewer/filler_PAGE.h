

	//
	// P A G E  MODE
	//

	// Base functionality of the page mode
	#include "fill_PAGE.h"

	// NO SELECTION.
	if (	(target_page->trackSelection == 0) 
		&& 	(target_page->stepSelection  == 0)
		){

		// The PAGE contains no active selection
		#include "fill_PAGE_sel_NONE.h"
	}

	// TRACK SELECTION
	else if ( target_page->trackSelection != 0 ){

		#include "fill_PAGE_sel_TRACK.h"
	}		

	// STEP SELECTION
	else if ( target_page->stepSelection != 0 ){

		#include "fill_PAGE_sel_STEP.h"
	}

				
	// TRACK SELECTORS
	if (EDIT_TIMER == ON) {

		// Show the current track selection				
		if (target_page->trackSelection != 0) {

			show( ELE_TRACK_SELECTORS, TRACK_SELECTION );
		}
	}
	else {
		// Default mode
		if (target_page->trackSelection != 0) {

			show( ELE_TRACK_SELECTORS, TRACK_SELECTION );
		}
						
		// Step selection existing
		else if ( target_page->stepSelection != 0 ){

			// Show the available and selected step attribute
			show( ELE_TRACK_SELECTORS, STEP_ATTRIBUTES_ALL );
			show( ELE_TRACK_SELECTORS, STEP_ATTRIBUTE_SELECTION );
		}

		// No selection active
		else {

			if ( 	( target_page->mixTarget == MIXTGT_ATR )
				&&	( is_pressed_key( KEY_MIX_MASTER ) )
				){

				if ( target_page->OPS_mode != BIRDSEYE ){
					show( ELE_TRACK_SELECTORS, MIX_ATTRIBUTE );
				}
			}

			// Show the track EFF Status
			show( ELE_TRACK_SELECTORS, TRACK_EFF_STATUS );

			// Show recording mode: here traditional
			if ( target_page->track_REC_pattern != 0 ){
				
				MIR_write_dot( LED_RECORD, MIR_RED );

				// Blink when no ON notes are pending..
				if ( NOTE_queue_counter == 0 ){
					MIR_write_dot( LED_RECORD, MIR_BLINK );						
				}
			}
			// Step recording
			else if ( target_page->REC_bit == ON ){
			
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




