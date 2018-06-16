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


extern void rot_exe_MIX( unsigned char rotNdx, unsigned char direction, Pagestruct* target_page );



// Apply edit encoders to the page as an entity.
void rot_exec_PAGE_global( 	Pagestruct* target_page, 
							unsigned char rotNdx, 
							unsigned char direction ){

	start_EDIT_TIMER();

	// Store the rot index in the mixing track attribute - spaghetti warning!
	target_page->mixingTrack = rotNdx;

	switch( rotNdx ){

		case ROT_VEL:

			// Modify the page velocity
			modify_parameter( 	&Page_repository[GRID_CURSOR].attr_VEL, 
								PAGE_MIN_VEL, PAGE_MAX_VEL, direction, OFF, FIXED );
			break;


		case ROT_PIT:

			// Modify the page pitch
			modify_parameter( 	&Page_repository[GRID_CURSOR].attr_PIT, 
								PAGE_MIN_PIT, PAGE_MAX_PIT, direction, OFF, FIXED );
			break;


		case ROT_LEN:

			// Make the actual changes visible
			GRID_rowzero_pagelength = TRUE;

			// Modify the page length
			modify_parameter( 	&Page_repository[GRID_CURSOR].attr_LEN, 
								PAGE_MIN_LEN, PAGE_MAX_LEN, direction, OFF, FIXED );
			break;


		case ROT_STA:
			
			// Make the actual changes visible
			GRID_rowzero_pagelength = FALSE;

			// Modify the page repeats - this is the page STA value
			modify_parameter( 	&Page_repository[GRID_CURSOR].attr_STA, 
								PAGE_MIN_STA, PAGE_MAX_STA, direction, OFF, FIXED );
			Page_repository[GRID_CURSOR].repeats_left = Page_repository[GRID_CURSOR].attr_STA;
			break;
			
	} // switch( rotNDx )
	
}



// MIXMAP encoder activity
void rot_exec_MIXMAP( 	Pagestruct* target_page, 
						unsigned char rotNdx, 
						unsigned char direction ){

	unsigned char row = 0;

	// x2 - Track row window shift
	unsigned char shiftTrackRow = track_get_window_shift( target_page );

	// BIG KNOB
	if ( rotNdx == 0 ){
	
		// When not running on external clock
		// And no page selected in GRID
		if (	( G_clock_source != EXT )
			&&	( GRID_play_mode != GRID_EDIT )
			){
	
			modify_parameter( &G_master_tempo, MIN_TEMPO, MAX_TEMPO, direction, ON, FIXED);
			G_TIMER_REFILL_update();
		}
	}


	// Do not react on tracks that are not heads in play_heads mode
	// Check if we are playing heads only..
	if 	(	( target_page->CHAINS_PLAY_HEAD == TRUE )
		&&	( rotNdx > 10 )
		){

		// Identify the row of the MIX encoder
		row = shiftTrackRow + rotNdx - 11;

		// Check if track is chain head..
		if ( target_page->Track[row]->chain_data[HEAD] != target_page->Track[row] ){
		
			return;
		}
	}

	// M I X E R: Left rotarys modify the assigned attribute
	rot_exe_MIX( rotNdx, direction, target_page );

	// E D I T O R:   Right rotarys modify hardwired attributes
	rot_exe_EDIT( rotNdx, direction, target_page );
}



// Called to execute the EDIT block operations.
// Semantics can be POS or NEG - call from EDIT block or MIXER block respectively..
void rot_exec_PAGE_local_EDIT( 	Pagestruct* target_page,
								unsigned char rotNdx,
								unsigned char direction,
								unsigned char step_selection ){

	unsigned char row=0;
	unsigned char col=0;
	unsigned int i = 0;

	// x2 - Track row window shift
	unsigned char shiftTrackRow = track_get_window_shift( target_page );

	// Operate on selected steps
	if ( target_page->stepSelection != 0 ){
		
		// Select the edit attribute
		target_page->editAttribute = rotNdx;
		
		// Scan through all steps of the matrix - not very efficient!!
		for (row=0; row < MATRIX_NROF_ROWS; row++) {

			for (col=0; col < MATRIX_NROF_COLUMNS; col++) {

				// STEP SELECTION flag is set
				if (Step_get_status( target_page->Step[row][col], STEPSTAT_SELECT ) == step_selection ){

					// Does what it says - applies the rotary to the step in question.
					apply_rotary_to_step( rotNdx, direction, target_page, row, col );

				} // Step is selected
			} // Matrix column scan
		} // Matrix row scan
	} // step selection == ON


	// No step selection is available, but at least one step button is pressed
	// Entering Roland Fleming mode: make sure that step is on, and edit its atribute 
	else if ( G_key_pressed != 0 ){

		// Modify page velocity and pitch if zoom button is pressed
		if ( G_key_pressed == KEY_ZOOM_PAGE ){
		
			rot_exec_PAGE_global( 	target_page, rotNdx, direction );
		}

		// Some other key is pressed, potentially a step key
		else{
			
			// Linear scan through the buttons in question: Selectors and Steps.
			for( i=1; i <= 185; i++ ){
			
				// Not using the provided infrastructure because of the anomalies. may change
				// There is a key pressed - in the Step Range (exclude the rest)
				if (	( G_pressed_keys[i] >=  11 )
					&&	( G_pressed_keys[i] <= 185 ) 
					&& 	( (( G_pressed_keys[i]-10) % 11) != 0 ) 

					// React only to PITCH, VELOCITY, LENGTH and START rotaries
					&& 	(	( rotNdx == 1 )		// PIT
						||	( rotNdx == 2 ) 	// VEL
						||	( rotNdx == 3 ) 	// LEN
						||	( rotNdx == 4 )	)	// STA
					){

					// Compute the key coordinates
					row = row_of( 		shift_key_track_row( target_page, G_pressed_keys[i], shiftTrackRow ) );
					col = column_of(	G_pressed_keys[i] );
					
					// d_iag_printf( "%d row:%d col:%d  ", pressed_keys[i], row, col );

					// The first knob click activates the step if it is not active
					if ( Step_get_status( target_page->Step[row][col], STEPSTAT_TOGGLE ) == OFF ){

						// If we are not in PREVIEW mode
						if (	( target_page->editorMode != PREVIEW )
							){

							// Make sure the step gets turned on as we turn the knob
							Step_set_status( target_page->Step[row][col], STEPSTAT_TOGGLE, ON );
						}	
					}
						
					// Finally apply the rotary to the step in question.
					apply_rotary_to_step( rotNdx, direction, target_page, row, col );

					// If we are in PREVIEW mode, play the step as well
					if (	( target_page->editorMode == PREVIEW )
						){

						PLAYER_preview_step( target_page, row, col );
					}

				} // key pressed in step range


				// Key pressed in Track Selector range, or track(s) lock-selected
				else if (	
							(	( G_pressed_keys[i] >=  1 )
							&&	( G_pressed_keys[i] <= 10 ) )
						|| 	( G_pressed_keys[i] == 32 )
					){
					
					// Compute the key row
					row = i-1;
					
					rot_exe_EDIT( rotNdx, direction, target_page );

					// If we are in PREVIEW mode, play the track as well
					if (	( target_page->editorMode == PREVIEW )
						// React only to PITCH and VELOCITY rotaries
						&& 	(	( rotNdx == 1 )
							||	( rotNdx == 2 ) )
						){

						PLAYER_preview_track( target_page, row );
					}			

					// Break out of the key iteration
					i = 185;

				} // Key pressed in track selector range

			} // button scan iterator

		} // else-- possibly a button is pressed

	} // key_pressed != 0 - Step for Roland Fleming mode


	// NEMO: this could happen by select-locking, so no key is pressed and still we have to do..
	else if ( target_page->trackSelection != 0 ){
	
		// Apply the rotary to the selected tracks
		rot_exe_EDIT( rotNdx, direction, target_page );

		// If we are in PREVIEW mode, play the track as well
		if (	( target_page->editorMode == PREVIEW )
			// React only to PITCH and VELOCITY rotaries
			&& 	(	( rotNdx == ROT_PIT )
				||	( rotNdx == ROT_VEL ) )
			&&	( my_bit_cardinality( target_page->trackSelection == 1 ) )
			){
			
			// Preview the first track in selection
			PLAYER_preview_track( target_page, my_bit2ndx( target_page->trackSelection ) );
		}
	}
	

	// MIX2EDIT is when the EDIT block acts similarly to the MIX block
	else if ( target_page->editorMode == MIX2EDIT ) {
	
		// Needs more digging - starting the MIX timer is not relevant here
		// --really need a MIX2EDIT timer - not implemented yet
		// start_MIX2EDIT_TIMER();

		// Normalize i value
		i = shiftTrackRow + rotNdx - 1;

		switch ( target_page->attr_mix2edit ){

			case NEMO_ATTR_POSITION:
				Page_wrap_track( target_page, i, direction, 1 );
				break;

			case NEMO_ATTR_LENGTH:
				modify_signed_parameter 
					( &target_page->Track[i]->LEN_factor, 
						target_page->attr_mix2edit_MIN, 
						target_page->attr_mix2edit_MAX, 
						direction, OFF, FIXED);
				break;
			
			case NEMO_ATTR_START:
				modify_signed_parameter 
					( &target_page->Track[i]->STA_factor, 
						target_page->attr_mix2edit_MIN, 
						target_page->attr_mix2edit_MAX, 
						direction, OFF, FIXED);
				break;

			default:
				// Modify the remembered (MIX2EDIT) attribute of track
				modify_signed_parameter 
					( 	(signed char*) ptr_of_track_attr_value( target_page, i, target_page->attr_mix2edit ),
						target_page->attr_mix2edit_MIN, 
						target_page->attr_mix2edit_MAX, 
						direction, OFF, FIXED);
				break;
		}	
	}

	// MCC function of the EDITOR - double implementation here and in the editorblock file
	// Needs cleanup at some point..
	else if ( target_page->editorMode == CONTROL ) {
	
		// No track selected and EDITOR is in Controller Mode -> send MIDI CC data

		// Normalize i value
		i = shiftTrackRow + rotNdx - 1;

		// Check before sending data that a valid CC is assigned (tmoravan)
		if ( ( 	target_page->Track[i]->attr_MCC ) != MIDICC_NONE ){
		
			// Modify the CC amount of track
			modify_signed_parameter 
				(&target_page->Track[i]->attr_CCAMT, 
					TRACK_MIN_CCAMNT, TRACK_MAX_CCAMNT, direction, ON, FIXED);

			// Send MIDICC on the corresponding channel
			MIDI_send( 	MIDI_CC,
						target_page->Track[i]->attr_MCH
						+ target_page->Track[i]->event_offset[ATTR_MIDICH],
						target_page->Track[i]->attr_MCC,					
						target_page->Track[i]->attr_CCAMT 	);
		} // Track is assigned a valid CC value

	}

}			



// Master handler of rotary encoder operations in local PAGE mode
void rot_exec_PAGE_local( 	Pagestruct* target_page, 
							unsigned char rotNdx, 
							unsigned char direction ){

	unsigned char i=0;
	// unsigned char j=0;

	// Mark the page as touched
	target_page->page_clear = OFF;


	// BIG KNOB
	if ( rotNdx == 0){
	
		// When a track is selected, operate on the selected attribute value or factor
		if ( target_page->trackSelection != 0 ){
		
			// Use the EDIT TIMER to show the value in the matrix
			EDIT_TIMER = ON;				
			// Setup alarm for the EDIT TIMER
			cyg_alarm_initialize(	alarm_hdl, 
									cyg_current_time() + TIMEOUT_VALUE, 
									0 );	
			// Select the edit attribute
			target_page->editAttribute = NEMO_selectedTrackAttribute;

			for ( i=0; i < 4; i++ ){
				
				// Operate on the selected tracks
				if ( (target_page->trackSelection & (1<<i)) != 0 ){
	
					switch( NEMO_track_VFF ){
						
						case VFF_VALUE:
							modify_selectedTrackAttribute_VALUE( target_page, i, direction, OFF );
							break;

						case VFF_FACTOR:
							modify_selectedTrackAttribute_FACTOR( target_page, i, direction, OFF );
							break;
					}
				}
			} // Track select finder iterator
		} // Track selection active
		

		// When not running on external clock
		else if ( G_clock_source != EXT ){

			// Don't allow tempo changes during active step selections
			if ( target_page->stepSelection == 0 ){
			
				modify_parameter( &G_master_tempo, MIN_TEMPO, MAX_TEMPO, direction, ON, FIXED);
				G_TIMER_REFILL_update();
			}
		}		
	}


	// MIXER
	//
	if ((rotNdx >= 11) && (rotNdx <= 20) ) {

		// Do not proceed if there is a track selection.
		// Selections are serviced by the EDITOR
		// ..but un-selections may be accessed by the MIXER!! :-) (for steps only)
		if (   (target_page->trackSelection != 0)
			){
			//The exception is setting the keyboard transpose midi channel for selected tracks
			if( (rotNdx == MIXTGT_EXP )) {

				rot_exe_MIX2EDIT( rotNdx, direction, target_page );

			}
			return;
		}
		// Modify the steps that are not selected - like the editor would!
		else if ( target_page->stepSelection != 0 ){
								
			// Align rot index to EDITOR context - simulate EDITOR
			rotNdx = rotNdx - 10;

			// Trigger EDIT block behaviour operations - OFF means act on non-selected steps
			rot_exec_PAGE_local_EDIT( target_page, rotNdx, direction, OFF );
		}
		else{

			// Quick+Dirty fix for something where we may need an additional zoom mode.
			rot_exe_MIX( rotNdx, direction, target_page );
		}
	}


	// E D I T O R
	//
	else if ((rotNdx >= 1) && (rotNdx <= 10)) {

		// Triggers the normal EDIT block behaviour operations - ON means act on the selected steps
		rot_exec_PAGE_local_EDIT( target_page, rotNdx, direction, ON );
	} // else if rotNdx in Editor
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

