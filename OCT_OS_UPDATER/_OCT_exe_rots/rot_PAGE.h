

// Apply edit encoders to the page as an entity.
void rot_exec_PAGE_global( 	Pagestruct* target_page, 
							unsigned char rotNdx, 
							unsigned char direction ){

	unsigned char i = 0, j = 0;


	switch( rotNdx ){

		case ROT_VEL:

			// Modify the page velocity
			modify_parameter( 	&Page_repository[GRID_CURSOR].attribute[ATTR_VELOCITY], 
								PAGE_MIN_VELOCITY, PAGE_MAX_VELOCITY, direction, ON, FIXED );
			break;


		case ROT_PIT:

			// Modify the page pitch
			modify_parameter( 	&Page_repository[GRID_CURSOR].attribute[ATTR_PITCH], 
								PAGE_MIN_PITCH, PAGE_MAX_PITCH, direction, ON, FIXED );
			break;


		case ROT_LEN:

			// LENGTH factor modified for all tracks in selected page
			for ( i=0; i < MATRIX_NROF_ROWS; i++ ){
				modify_parameter(
					&(&Page_repository[ GRID_CURSOR ])->Track[i]->LEN_factor,
					TRACK_MIN_LENFACTOR, TRACK_MAX_LENFACTOR, direction, OFF, FIXED );
			}
			break;


		case ROT_STA:

			// START factor modified for all tracks in selected page
			for ( i=0; i < MATRIX_NROF_ROWS; i++ ){
				modify_parameter(
					&(&Page_repository[ GRID_CURSOR ])->Track[i]->STA_factor,
					TRACK_MIN_STAFACTOR, TRACK_MAX_STAFACTOR, direction, OFF, FIXED );
			}
			break;


		case ROT_POS:

			j = (&Page_repository[ GRID_CURSOR ])->locator;
			// Compute the new locator to align the page under cursor to.
			if ( direction == INC ){

				i = ((&Page_repository[ GRID_CURSOR ])->locator % 16) + 1;				
			}
			else if ( direction == DEC ){

				i = (&Page_repository[ GRID_CURSOR ])->locator - 1;
				if ( i == 0 ){	i = 16;	}
			}

			j = TTC_ABS_VALUE;

			// Align the page under the cursor fully to the new locator.
			set_page_locators( &Page_repository[ GRID_CURSOR ], i, j );
			break;
			
	} // switch( rotNDx )
	
}



// MIXMAP encoder activity
void rot_exec_MIXMAP( 	Pagestruct* target_page, 
						unsigned char rotNdx, 
						unsigned char direction ){

	unsigned int i=0, j=0, k=0;

	signed int min = 0;
	signed int max = 0;

	unsigned char trackNdx = 0;


	// M I X E R: Left rotarys modify the assigned attribute
	#include "rot_mixerblock.h"
	// Above is used for DEFAULT + MIXER MODE..

	// E D I T O R:   Right rotarys modify hardwired attributes
	rot_exe_EDIT( rotNdx, direction, target_page );
}






void rot_exec_PAGE_local( 	Pagestruct* target_page, 
							unsigned char rotNdx, 
							unsigned char direction ){

//	unsigned int i=0, j=0, k=0;
//
//	signed int min = 0;
//	signed int max = 0;
//
//	unsigned char row=0;
//	unsigned char col=0;
//
//	unsigned char trackNdx = 0;
//
//	// Mark the page as touched
//	target_page->page_clear = OFF;
//
//
//	// BIG KNOB
//	if ( rotNdx == 0){
//	
//		// Regular operation
//		if ( GRID_bigknob_SELECT == OFF ){
//
//			// When a track is selected, modify its program change
//			if ( my_bit_cardinality( target_page->trackSelection ) == 1 ){
//			
//				// Act as program changer normally..
//				modify_parameter( 	&target_page->Track[ my_bit2ndx( target_page->trackSelection )]->program_change, 
//									TRACK_MIN_PROGRAMCHANGE, TRACK_MAX_PROGRAMCHANGE, direction, ON, FIXED );
//			}
//
//			// When not running on external clock
//			else if ( CLOCK_SOURCE != EXT ){
//
//				modify_parameter( &TEMPO, MIN_TEMPO, MAX_TEMPO, direction, ON, FIXED);
//				TIMER_REFILL_update();
//			}			
//		}
//
//		// Jump across the playing pages
//		else if ( GRID_bigknob_SELECT == ON ){
//
//			switch( direction ){
//
//				case INC:
//					// Get the next page if available
//					j = FALSE;
//
//					for ( i=GRID_NROF_BANKS+1; i>0; i-- ){
//
//						if ( 	( j == TRUE )
//							&&	( GRID_p_selection[i-1] != NULL )
//							){
//
//							// Move the viewer page
//							target_page = GRID_p_selection[i-1];
//							target_page->keyMode = KMOD_PAGE;
//							// Move the cursor
//							GRID_CURSOR = target_page->pageNdx;
//							j = FALSE;
//							break;
//						}
//						
//						if ( GRID_p_selection[i-1] == target_page ){
//							// Remember having found the viewer page
//							j = TRUE;
//						}
//					}
//					break;
//
//				case DEC:
//					// Get the next page if available
//					j = FALSE;
//
//					for ( i=0; i < GRID_NROF_BANKS; i++ ){
//
//						if ( 	( j == TRUE )
//							&&	( GRID_p_selection[i] != NULL )
//							){
//
//							// Move the viewer page
//							target_page = GRID_p_selection[i];
//							target_page->keyMode = KMOD_PAGE;
//							// Move the cursor
//							GRID_CURSOR = target_page->pageNdx;
//							j = FALSE;
//							break;
//						}
//						
//						if ( GRID_p_selection[i] == target_page ){
//							// Remember having found the viewer page
//							j = TRUE;
//						}
//					}
//					break;
//			}
//		} // GRID_bigknob_select == ON		
//	}
//
//
//	// MIXER
//	//
//	if ((rotNdx >= 11) && (rotNdx <= 20) ) {
//
//		// Do not proceed if there is a track or step selection.
//		// Selections are serviced by the EDITOR
//		if (   (target_page->trackSelection != 0)
//			|| (target_page->stepSelection  != 0)  
//			){
//
//			return;
//		}
//		
//		// Quick+Dirty fix for something where we may need an additional zoom mode.
//#include "rot_mixerblock.h"
//	}
//
//
//	// E D I T O R
//	//
//	else if ((rotNdx >= 1) && (rotNdx <= 10)) {
//
//
////		// Operate on selected steps
//		if ( target_page->stepSelection != 0 ){
//			
//			// Select the edit attribute
//			target_page->editAttribute = rotNdx;
//			
//			// Scan through all steps of the matrix - not very efficient!!
//			for (row=0; row < MATRIX_NROF_ROWS; row++) {
//
//				for (col=0; col < MATRIX_NROF_COLUMNS; col++) {
//
//					// STEP SELECTION flag is set
//					if (Step_get_status( target_page->Step[row][col], STEPSTAT_SELECT ) == ON){
//
//						// Does what it says - applies the rotary to the step in question.
//						apply_rotary_to_step( rotNdx, direction, target_page->Step[row][col] );
//
//					} // Step is selected
//				} // Matrix column scan
//			} // Matrix row scan
//		} // step selection == ON
//
//
//		// No step selection is available, but at least one step button is pressed
//		// Entering Roland Fleming mode: make sure that step is on, and edit its atribute 
//		else if ( key_pressed != 0 ){
//
//			// Modify page velocity and pitch if zoom button is pressed
//			if ( key_pressed == KEY_ZOOM_PAGE ){
//			
//				switch( rotNdx ){
//				 
//					// VEL
//					case 1:
//						modify_parameter( 	&target_page->attribute[ATTR_VELOCITY], 
//											PAGE_MIN_VELOCITY, PAGE_MAX_VELOCITY, direction, ON, FIXED );
//						break;
//
//					// PIT
//					case 2:
//						modify_parameter( 	&target_page->attribute[ATTR_PITCH], 
//											PAGE_MIN_PITCH, PAGE_MAX_PITCH, direction, ON, FIXED );
//						break;
//				}
//			}
//
//			// Some other key is pressed, potentially a step key
//			else{
//				
//				// Linear scan through the buttons in question: Selectors and Steps.
//				for( i=0; i <= 185; i++ ){
//				
//					// Not using the provided infrastructure because of the anomalies. may change
//					// There is a key pressed - in the Step Range (exclude the rest)
//					if (	( pressed_keys[i] >=  11 )
//						&&	( pressed_keys[i] <= 185 ) 
//						&& 	( ((pressed_keys[i]-10) % 11) != 0 ) 
//
//						// React only to PITCH, VELOCITY, LENGTH and START rotaries
//						&& 	(	( rotNdx == 1 )		// VEL
//							||	( rotNdx == 2 ) 	// PIT
//							||	( rotNdx == 3 ) 	// LEN
//							||	( rotNdx == 4 )		// STA
//							||	( rotNdx == 7 )		// AMT
//							||	( rotNdx == 8 )		// GRV
//							||	( rotNdx == 9 ) )	// MCC
//						){
//
//						// Compute the key coordinates
//						row = row_of( 		pressed_keys[i] );
//						col = column_of(	pressed_keys[i] );
//						
//						// d_iag_printf( "%d row:%d col:%d  ", pressed_keys[i], row, col );
//
//						// The first knob click activates the step if it is not active
//						if ( Step_get_status( target_page->Step[row][col], STEPSTAT_TOGGLE ) == OFF ){
//
//							// If we are not in PREVIEW mode
//							if (	( target_page->editorMode != PREVIEW )
//								){
//
//								// Make sure the step gets turned on as we turn the knob
//								Step_set_status( target_page->Step[row][col], STEPSTAT_TOGGLE, ON );
//							}	
//						}
//							
//						// Finally apply the rotary to the step in question.
//						apply_rotary_to_step( rotNdx, direction, target_page->Step[row][col] );
//
//						// If we are in PREVIEW mode, play the step as well
//						if (	( target_page->editorMode == PREVIEW )
//							){
//
//							PLAYER_preview_step( target_page, row, col );
//						}
//
//					} // key pressed in step range
//
//
//					// Key pressed in Track Selector range
//					else if (	(	( pressed_keys[i] >=  1 )
//								&&	( pressed_keys[i] <= 10 ) )
//							|| ( pressed_keys[i] == 32 )
////							|| ( target_page->trackSelection != 0 )
//						){
//							
//						// Compute the key row
//						row = i-1;
//						
//						rot_exe_EDIT( rotNdx, direction, target_page );
//
//						// If we are in PREVIEW mode, play the step as well
//						if (	( target_page->editorMode == PREVIEW )
//							// React only to PITCH and VELOCITY rotaries
//							&& 	(	( rotNdx == 1 )
//								||	( rotNdx == 2 ) )
//							){
//
//							PLAYER_preview_track( target_page, row );
//						}			
//					} // Key pressed in selector range
//
//				} // button scan iterator
//			} // else-- possibly a button is pressed
//
//		} // key_pressed != 0 - Step for Roland Fleming mode
//
//	
//		// MIX2EDIT is when the EDIT block acts similarly to the MIX block
//		else if ( target_page->editorMode == MIX2EDIT ) {
//		
//			// Needs more digging - starting the MIX timer is not relevant here
//			// --really need a MIX2EDIT timer - not implemented yet
//			// start_MIX2EDIT_TIMER();
//			// d_iag_printf( "MIX2EDIT rotary\n" );
//	
//			// Normalize i value
//			i = rotNdx - 1;
//
//			switch ( target_page->attr_mix2edit ){
//
//				case ATTR_POSITION:
//					Page_wrap_track( target_page, i, direction, 1 );
//					break;
//
//				case ATTR_LENGTH:
//					modify_signed_parameter 
//						( &target_page->Track[i]->LEN_factor, 
//							target_page->attr_mix2edit_MIN, 
//							target_page->attr_mix2edit_MAX, 
//							direction, OFF, FIXED);
//					break;
//				
//				case ATTR_START:
//					modify_signed_parameter 
//						( &target_page->Track[i]->STA_factor, 
//							target_page->attr_mix2edit_MIN, 
//							target_page->attr_mix2edit_MAX, 
//							direction, OFF, FIXED);
//					break;
//
//				default:
//					// Modify the remembered (MIX2EDIT) attribute of track
//					modify_signed_parameter 
//						( &target_page->Track[i]->attribute[target_page->attr_mix2edit], 
//							target_page->attr_mix2edit_MIN, 
//							target_page->attr_mix2edit_MAX, 
//							direction, OFF, FIXED);
//					break;
//			}	
//		}
//
//		// MCC function of the EDITOR - double implementation here and in the editorblock file
//		// Needs cleanup at some point..
//		else if ( target_page->editorMode == CONTROL ) {
//		
//			// No track selected and EDITOR is in Controller Mode -> send MIDI CC data
//	
//			// Normalize i value
//			i = rotNdx - 1;
//
//			// Check before sending data that a valid CC is assigned (tmoravan)
//			if ( ( 	target_page->Track[i]->attribute[ATTR_MIDICC] ) != MIDICC_NONE ){
//			
//				// Modify the CC amount of track
//				modify_signed_parameter 
//					(&target_page->Track[i]->attribute[ATTR_CCAMNT], 
//						TRACK_MIN_CCAMNT, TRACK_MAX_CCAMNT, direction, ON, FIXED);
//	
//				// Send MIDICC on the corresponding channel
//				MIDI_send( 	MIDI_CC,
//							target_page->Track[i]->attribute[ATTR_MIDICH]
//							+ target_page->Track[i]->event_offset[ATTR_MIDICH],
//							target_page->Track[i]->attribute[ATTR_MIDICC],					
//							target_page->Track[i]->attribute[ATTR_CCAMNT] 	);
//			} // Track is assigned a valid CC value
//
//		}
//
//	} // else if rotNdx in Editor

}



//
// Main encoder handler in PAGE mode
//
void rot_exec_PAGE( 	Pagestruct* target_page, 
						unsigned char rotNdx, 
						unsigned char direction ){

	// Global operations on the track
	if( target_page->OPS_mode == BIRDSEYE ){
		
		rot_exec_PAGE_global( target_page, rotNdx, direction );
	}
	else{
			
		rot_exec_PAGE_local( target_page, rotNdx, direction );
	}
}

