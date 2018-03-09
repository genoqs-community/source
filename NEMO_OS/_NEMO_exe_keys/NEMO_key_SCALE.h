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

	
	// SELECTORS	
	// And actually select the G_scale_ndx switching between scales!
	if (	( keyNdx > 0 ) 	&& 	( keyNdx < 5 )
		){
		
		// Changing this global index is equivalent to switching between scales!
		G_scale_ndx = keyNdx;

		return;
	}


	// Apply scale signatures to the current lead
	// Make this accessible via mutator buttons
	if (	( keyNdx >= 187 ) 	&& 	( keyNdx <= 190 ) ){
		// Point to the right spot for operation
		target_page = GRID_assistant_page;

		switch( keyNdx ){		
			case 187:	target_page->scaleNotes[G_scale_ndx] = SCALE_SIG_PEN;		break;				
			case 188:	target_page->scaleNotes[G_scale_ndx] = SCALE_SIG_MIN;		break;	
			case 189: 	target_page->scaleNotes[G_scale_ndx] = SCALE_SIG_MAJ;		break;
			case 190:	target_page->scaleNotes[G_scale_ndx] = SCALE_SIG_DIM;		break;
		}
		// Shift the signature according to the lead
		target_page->scaleNotes[G_scale_ndx] = 
			my_shift_bitpattern( 	target_page->scaleNotes[G_scale_ndx], 12, INC, 
								(11 - my_bit2ndx(target_page->scaleLead[G_scale_ndx] ) ) 
								);
		return;
	}


	// OCTAVE LINE - actually extended to the full matrix!
	// This should only operate when in scale mode already
	// ..or when trying to enter scale mode.
	if ( 	( GRID_assistant_page->scaleStatus != OFF ) 
		){
		
		key_ScaleSelector_functions( keyNdx, GRID_assistant_page );
	}	
	else if ( GRID_assistant_page->scaleStatus == OFF ){
	
		// This is the mod operation in Page mode
//		key_ScaleSelector_mini_functions( keyNdx, GRID_assistant_page );
	}

	
	// SCALE KEY
	// Lock in the scale and exit scale mode
	if ( keyNdx == KEY_SCALE_SEL ){
	
		// Turn on force to scale and go back to GRID
		GRID_assistant_page->force_to_scale = ON;		
		GRID_assistant_page->scaleStatus = OFF;
	
		G_zoom_level = G_last_zoom_level;		
		return;
	}


	// CLOCK SOURCE
	if ( keyNdx == KEY_CLOCK ) {

		key_clock_select( target_page, KEY_CLOCK );
	} // KEY_CLOCK was pressed


	// PLAY MODE
	// Snapshot key: single click from OFF turns it on.
	// Single click from ON turns it back OFF
	// Double click from ON stores the state like it is.
	if ( keyNdx == KEY_ZOOM_PLAY ){
	
		// D O U B L E - C L I C K  C O N S T R U C T
		// DOUBLE CLICK SCENARIO
		if ((DOUBLE_CLICK_TARGET == keyNdx) 
				&& (DOUBLE_CLICK_TIMER > DOUBLE_CLICK_ALARM_SENSITIVITY)) {
	
			// Double click code
			// ...
			// Fetches the old status back.
			// Toggles the play mode status
			toggle_PLAY_MODE( OFF );
		} // end of double click scenario 
		
		// SINGLE CLICK SCENARIO
		else if (DOUBLE_CLICK_TARGET == 0) {
			
				DOUBLE_CLICK_TARGET = keyNdx;
				DOUBLE_CLICK_TIMER = ON;
				// Start the Double click Alarm
				cyg_alarm_initialize(	
						doubleClickAlarm_hdl, 
						cyg_current_time() + DOUBLE_CLICK_ALARM_TIME,
						DOUBLE_CLICK_ALARM_TIME );
	
			// Single click code
			// ...
			// Leaves the PLAY changes permanent in the page
			switch( PLAY_MODE_STATUS ){
				case ON:
					PLAY_MODE_STATUS = OFF;
					break;
				
				case OFF:
					toggle_PLAY_MODE( ON );
					break;
			}
		}
	}

	
	
	// EDIT MASTER KEY
	// Toggles the SCALE edit mode
	if ( keyNdx == KEY_EDIT_MASTER ){

		// EDIT MASTER showing the scale edit mode
		switch( GRID_assistant_page->scaleStatus ){
	
			case SCALE_MOD:
				GRID_assistant_page->scaleStatus = SCALE_SEL;
				break;
								
			case SCALE_SEL:
				GRID_assistant_page->scaleStatus = SCALE_MOD;
				break;
		}
	}
		
	
	// ESC
	// Lock the SCALE into the machine and exit the SCALE mode
	if ( keyNdx == KEY_RETURN ){
	
		GRID_assistant_page->scaleStatus = OFF;
		GRID_assistant_page->force_to_scale = ON;
		// Go back to GRID or PAGE
		G_zoom_level = G_last_zoom_level;
		return;
	}


	// REC KEY
	// Toggle the REC_bit for the GRID assistant page
	if ( keyNdx == KEY_RECORD ){
	
		GRID_assistant_page->REC_bit ^= ON;
	}
	

	// TRANSPORT functions - universally valid
	switch( keyNdx ){

		case KEY_STOP:
			if ( G_clock_source != EXT ){
				sequencer_command_STOP();
			}
			break;

		case KEY_PAUSE:
			if ( is_pressed_key( KEY_STOP ) ){
				switch( G_clock_source ){
					case OFF:	G_clock_source = EXT;		break;
					case INT:	G_clock_source = EXT;		break;
					case EXT:	G_clock_source = OFF;		break;
				}					
				G_master_blinker = ON;
			}

			// Transport command
			else{			
				if ( G_clock_source != EXT ){
					sequencer_command_PAUSE();
				}
			}
			break;

		case KEY_PLAY1:
		case KEY_PLAY2:
		case KEY_PLAY4:

			// Set the MIDI clock source
			if ( is_pressed_key( KEY_STOP ) ){
				switch( G_clock_source ){
					case OFF:	G_clock_source = INT;		break;
					case INT:	G_clock_source = OFF;		break;
					case EXT:	G_clock_source = INT;		break;
				}
				G_master_blinker = ON;
			}

			// Transport command
			else{
				if ( G_clock_source != EXT ){
					sequencer_command_PLAY();
				}
			}
			break;
	}	

