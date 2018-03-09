/* OCTOPUS
 * 
 * Interpret incoming key presses
 * genoQs Machines, 07.11.2004
 */

extern void key_ScaleSelector_mini_functions( unsigned int keyNdx, Pagestruct* target_page );
extern void key_ScaleSelector_functions( unsigned int keyNdx, Pagestruct* target_page );
extern void my_sysex_dump( unsigned char dump_type, unsigned char GRID_CURSOR );

// General functions used by other key exec functions
#include "key_functions.h"
#include "key_OCT_CIRCLE_xpose_STEP.h"
#include "key_NUMERIC_QUADRANT_velo_STEP.h"
#include "key_OCT_CIRCLE_chord_STEP.h"
#include "key_STEP.h"
#include "key_ScaleSelector_functions.h"
#include "key_ScaleSelector_mini_functions.h"
#include "key_mixermap_matrixclick.h"
#include "key_exe_chainselectors.h"


// EXECUTE THE KEYPRESS RECEIVED
void executeKey( unsigned int keyNdx ){

	unsigned int  	i=0, 
					j=0, 
					k=0,
					m=0,
					row=0,
					col=0,
					temp=0
					;

	// Work on the page under the grid cursor
	Pagestruct* target_page = &Page_repository[ GRID_CURSOR ];

	// Used to mute a whole chain
	Trackstruct* current_track = NULL;	

	// Page pointer holder
	Pagestruct* temp_page = NULL;

	// Used for syntax simplification
	signed char* val_ptr = NULL;

	// Used in the key entry routine for the mixer maps modes
	unsigned char* target_ptr = NULL;

	
// d_iag_printf("  -> executeKey() keyNdx:%d cursor:%d\n", keyNdx, GRID_CURSOR );


	// 
	// CHORD SELECTOR
	//
	// Double click enters the mode or chord strum editing.
	if( (keyNdx > 251) 
		){
		// D O U B L E - C L I C K
		if (	(DOUBLE_CLICK_TARGET == keyNdx) 
			&& 	(DOUBLE_CLICK_TIMER > DOUBLE_CLICK_ALARM_SENSITIVITY)
			){

			// Double click code: Switch to the Chordstrum edit view
			if ( zoom_level != zoomDEVICE ){
			
				zoom_level = zoomSTRUM;
			}
		}
		// SINGLE CLICK
		else if ( (DOUBLE_CLICK_TARGET == 0) 
			){

			DOUBLE_CLICK_TARGET = keyNdx;
			DOUBLE_CLICK_TIMER = ON;								
			// Start the Double click Alarm
			cyg_alarm_initialize(	
				doubleClickAlarm_hdl, 
				cyg_current_time() + DOUBLE_CLICK_ALARM_TIME,
				DOUBLE_CLICK_ALARM_TIME );

			// Single click code
			if ( zoom_level == zoomSTRUM ){

				// Switch the strum level view
				OCT_strum_view_level = 258 - keyNdx;
			}
		}
	} // Chord selector key pressed


	
	// BIG KNOB KEYS operation
	if ( 	( keyNdx == 201 )
		||	( keyNdx == 200	)
		||	( keyNdx == 199 )
		|| 	( keyNdx == 198 )
		|| 	( keyNdx == 197 )
		||	( keyNdx == 207 )
		||	( keyNdx == 206 )
		||	( keyNdx == 216 )
		|| 	( keyNdx == 215 )
		||	( keyNdx == 224 )
		){
	
		switch( zoom_level ){
			
			case zoomGRID:
			case zoomGRIDTRACK:
			// Enter the TEMPO
			
				// D O U B L E - C L I C K
				if ((DOUBLE_CLICK_TARGET == keyNdx) 
						&& (DOUBLE_CLICK_TIMER > DOUBLE_CLICK_ALARM_SENSITIVITY)) {
		
					// Handle the 100 button
					if (	( keyNdx == 224 )
						){
						
						if 		( TEMPO  < 100 ) TEMPO = 100;
						else if ( TEMPO == 100 ) TEMPO = MIN_TEMPO;
						else if ( TEMPO  > 100 ) TEMPO = TEMPO - 100;

						TIMER_REFILL_update();
					}
					else{
						// Double click modifieds the large value
						TEMPO = ((TEMPO / 100)*100) + ( BK_KEY_to_xdx( keyNdx ) *10);
						TIMER_REFILL_update();
					}
				}
		
				// SINGLE CLICK
				else if (DOUBLE_CLICK_TARGET == 0) {
		
					DOUBLE_CLICK_TARGET = keyNdx;
					DOUBLE_CLICK_TIMER = ON;								
					// Start the Double click Alarm
					cyg_alarm_initialize(	
						doubleClickAlarm_hdl, 
						cyg_current_time() + DOUBLE_CLICK_ALARM_TIME,
						DOUBLE_CLICK_ALARM_TIME );
		
					// Double click on 100 sets the tempo to 100
					if ( keyNdx != 224 ){
		
						// Mmodifies the 1's value, do not react on the 100 button
						TEMPO = (TEMPO / 10)*10 + BK_KEY_to_xdx( keyNdx );
						TIMER_REFILL_update();
					}
				}			
				break;



			case zoomPAGE:
			// TEMPO ENTRY, BANK JUMP, or Prg Chg when track selected

				switch( GRID_bigknob_SELECT ){
		
					// TEMPO ENTRY
					case OFF:
		
						// When a track is selected, modify its program change
						if ( my_bit_cardinality( target_page->trackSelection ) == 1 ){

							// Use val_ptr for syntax simplification
							val_ptr = &target_page->Track[ my_bit2ndx( target_page->trackSelection )]->program_change;
			
							// D O U B L E - C L I C K
							if ((DOUBLE_CLICK_TARGET == keyNdx) 
									&& (DOUBLE_CLICK_TIMER > DOUBLE_CLICK_ALARM_SENSITIVITY)) {
					
								// Handle the 100 button
								if (	( keyNdx == 224 )
									){
									
									if 		( *val_ptr  < 100 ) *val_ptr = 100;
									else if ( *val_ptr == 100 ) *val_ptr = TRACK_MIN_PROGRAMCHANGE;
									else if ( *val_ptr  > 100 ) *val_ptr = *val_ptr - 100;
								}
			
								else{
									// Double click modifieds the large value
									*val_ptr = normalize (
										((*val_ptr / 100)*100) + ( BK_KEY_to_xdx( keyNdx ) *10),
										TRACK_MIN_PROGRAMCHANGE, 
										TRACK_MAX_PROGRAMCHANGE );					
								}
							}
					
							// SINGLE CLICK
							else if (DOUBLE_CLICK_TARGET == 0) {
					
								DOUBLE_CLICK_TARGET = keyNdx;
								DOUBLE_CLICK_TIMER = ON;								
								// Start the Double click Alarm
								cyg_alarm_initialize(	
									doubleClickAlarm_hdl, 
									cyg_current_time() + DOUBLE_CLICK_ALARM_TIME,
									DOUBLE_CLICK_ALARM_TIME );
					
								// Double click on 100 sets the tempo to 100
								if ( keyNdx != 224 ){
					
									// Mmodifies the 1's value, do not react on the 100 button
									*val_ptr = normalize( 
										(*val_ptr / 10)*10 + BK_KEY_to_xdx( keyNdx ),
										TRACK_MIN_PROGRAMCHANGE, 
										TRACK_MAX_PROGRAMCHANGE );
								}
							}
						} // one track selected


						// TEMPO ENTRY
						// D O U B L E - C L I C K
						else if ((DOUBLE_CLICK_TARGET == keyNdx) 
								&& (DOUBLE_CLICK_TIMER > DOUBLE_CLICK_ALARM_SENSITIVITY)) {
				
							// Handle the 100 button
							if (	( keyNdx == 224 )
								){
								
								if 		( TEMPO  < 100 ) TEMPO = 100;
								else if ( TEMPO == 100 ) TEMPO = MIN_TEMPO;
								else if ( TEMPO  > 100 ) TEMPO = TEMPO - 100;
		
								TIMER_REFILL_update();
							}
							else{
								// Double click modifieds the large value
								TEMPO = ((TEMPO / 100)*100) + ( BK_KEY_to_xdx( keyNdx ) *10);
								TIMER_REFILL_update();
							}
						}
				
						// SINGLE CLICK
						else if (DOUBLE_CLICK_TARGET == 0) {
				
							DOUBLE_CLICK_TARGET = keyNdx;
							DOUBLE_CLICK_TIMER = ON;								
							// Start the Double click Alarm
							cyg_alarm_initialize(	
								doubleClickAlarm_hdl, 
								cyg_current_time() + DOUBLE_CLICK_ALARM_TIME,
								DOUBLE_CLICK_ALARM_TIME );
				
							// Double click on 100 sets the tempo to 100
							if ( keyNdx != 224 ){
				
								// Mmodifies the 1's value, do not react on the 100 button
								TEMPO = (TEMPO / 10)*10 + BK_KEY_to_xdx( keyNdx );
								TIMER_REFILL_update();
							}
						}
						break;
		
					// BANK JUMP
					case ON:
		
						// Map the value ranges accordingly
						j = 9 - BK_KEY_to_xdx( keyNdx );
		
						if (	( j >= 0 )
							&&	( GRID_p_selection[j] != NULL )
							){
		
							// Move the viewer page
							target_page = GRID_p_selection[j];
							target_page->keyMode = KMOD_PAGE;
			
							// Move the cursor
							GRID_CURSOR = target_page->pageNdx;
						}
						break;
				}
				break;



			case zoomTRACK:
			// Enter Program Change value
			
				// Use val_ptr for syntax simplification
				val_ptr = &target_page->Track[ my_bit2ndx( target_page->trackSelection )]->program_change;

				// D O U B L E - C L I C K
				if ((DOUBLE_CLICK_TARGET == keyNdx) 
						&& (DOUBLE_CLICK_TIMER > DOUBLE_CLICK_ALARM_SENSITIVITY)) {
		
					// Handle the 100 button
					if (	( keyNdx == 224 )
						){
						
						if 		( *val_ptr  < 100 ) *val_ptr = 100;
						else if ( *val_ptr == 100 ) *val_ptr = TRACK_MIN_PROGRAMCHANGE;
						else if ( *val_ptr  > 100 ) *val_ptr = *val_ptr - 100;
					}

					else{
						// Double click modifieds the large value
						*val_ptr = normalize (
							((*val_ptr / 100)*100) + ( BK_KEY_to_xdx( keyNdx ) *10),
							TRACK_MIN_PROGRAMCHANGE, 
							TRACK_MAX_PROGRAMCHANGE );					
					}
				}
		
				// SINGLE CLICK
				else if (DOUBLE_CLICK_TARGET == 0) {
		
					DOUBLE_CLICK_TARGET = keyNdx;
					DOUBLE_CLICK_TIMER = ON;								
					// Start the Double click Alarm
					cyg_alarm_initialize(	
						doubleClickAlarm_hdl, 
						cyg_current_time() + DOUBLE_CLICK_ALARM_TIME,
						DOUBLE_CLICK_ALARM_TIME );
		
					// Double click on 100 sets the tempo to 100
					if ( keyNdx != 224 ){
		
						// Mmodifies the 1's value, do not react on the 100 button
						*val_ptr = normalize( 
							(*val_ptr / 10)*10 + BK_KEY_to_xdx( keyNdx ),
							TRACK_MIN_PROGRAMCHANGE, 
							TRACK_MAX_PROGRAMCHANGE );
					}
				}
				break;
				
		} // zoom_level switch
		
	} // big knob keys



	// RETURN : back to some default machine state
	if (	( keyNdx == KEY_RETURN )
	
		&&	( zoom_level != zoomDEVICE )
		&&	( zoom_level != zoomDIAG )
		&&	( zoom_level != zoomAMANDA )
		){ 
		
		// Enter machine lock state
		if (	( zoom_level == zoomGRID )
			&& 	( is_pressed_key( KEY_ZOOM_GRID ) )
			){
		
			zoom_level = zoomAMANDA;
			return;
		}
		
		// Everywhere but in GRID mode - where we need to exit the CC_MAP mode
		else if ( zoom_level != zoomGRID ){

			zoom_level = zoomPAGE;
			
			target_page->keyMode = KMOD_PAGE;	
			target_page->trackSelection = OFF;
			target_page->trackAttributeSelection = OFF;
			
			// Unselect all steps in page, if any were selected
			if ( target_page->stepSelection != 0 ){
	
				for (row=0; row<MATRIX_NROF_ROWS; row++){
					for (col=0; col<MATRIX_NROF_COLUMNS; col++){
	
						Step_set_status( target_page->Step[row][col], STEPSTAT_SELECT, OFF );
					}
				}
				// Reset the counter of selected steps
				target_page->stepSelection = OFF;
				target_page->stepAttributeSelection = OFF;
			}
	
			// Reset the timers
			MIX_TIMER 	= OFF;
			EDIT_TIMER 	= OFF;
		
			// Release the MIDI SYSEX state
			MIDI_RECEIVE_SYSEX = FALSE;
			MIDI_SYSEX_HEADER_SEEN = FALSE;
			sysex_stream_keeper_index = 0;
		}		
	}



	// 
	// TRANSPORT FIELD KEYS 
	//
	switch( CLOCK_SOURCE ){

//		case EXT:
//
//			#include "key_TRANSPORT_isEXT.h"	// OK
//			break;
//			
//		case INT:
//		case OFF:
//			
//			#include "key_TRANSPORT_noEXT.h"	// OK
//			break;
	}



	// Mode bound key interpretation
	//
	switch (zoom_level) {
		
//		case zoomABLETON:
//			#include "key_ABLETON.h"
//			break;
//
//		case zoomAMANDA:
//			#include "key_AMANDA.h"				
//			break;
//
//		case zoomSTRUM:
//			#include "key_STRUM.h"				// OK
//			break;
//
//		case zoomDIAG:
//			#include "key_DIAG.h"				// OK
//			break;
//
//		case zoomGRID:
//			#include "key_GRID.h"				// OK
//			break;
//
//		case zoomGRIDTRACK:
//			#include "key_GRIDTRACK.h"				// OK
//			break;

		case zoomDEVICE:
			#include "key_DEVICE.h"			// OK
			break;

//		case zoomPAGE:
//			switch( target_page->keyMode ){
//				
//				case KMOD_PAGE:
//					// Base functionality of the page mode
//					#include "key_PAGE.h"		// OK
//
//					// No track or step is selected. Page is selection-blank.
//					if (   ( target_page->trackSelection == 0) 
//						&& ( target_page->stepSelection  == 0)  ){
//
//						// The PAGE contains no active selection
//						switch( target_page->OPS_mode ){
//
//							// BIRDSEYE is different -> only accepts page related stuff
//							case BIRDSEYE:
//
//								#include "key_PAGE_sel_NONE_BIRDSEYE.h"		// OK
//								break;
//
//							default:
//
//								#include "key_PAGE_sel_NONE.h"				// OK
//								break;
//						} // switch( target_page->OPS_mode
//					}
//					else if ( target_page->trackSelection != 0 ){
//
//						#include "key_PAGE_sel_TRACK.h"						// OK
//					}		
//					else if ( target_page->stepSelection != 0 ){
//
//						#include "key_PAGE_sel_STEP.h"						// OK
//					}
//					break;				
//			}
//			break;
//
//
//		case zoomMIXMAP:
//			#include "key_MIXMAP.h"											// OK
//			break;
//
//
//		case zoomTRACK:
//			switch (target_page->keyMode) {
//
//				case KMOD_THIS_TRACK:
//					// fallthrough intended
//					
//				case KMOD_THIS_TRACK_EXTENDED:
//					
//					#include "key_TRACK.h"									// OK
//
//					break;
//			}
//			break;
//
//		case zoomMAP:
//			switch (target_page->keyMode) {
//
//				case KMOD_ATTR_MAP:
//					#include "key_MAP.h"									// OK
//					break;
//
//			}
//			break;
//			
//
//		case zoomSTEP:
//
//			key_exec_STEP( keyNdx );
//			break;
//
//
//		case zoomPLAY:
//			// ..not working right now..
//			break;
//

	} // switch zoom_level

	// Update HUI with changes
	Page_requestRefresh();
}


