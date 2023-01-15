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



	
	// 
	// GRID  M I X  MODE
	// 
	if ( GRID_play_mode == GRID_MIX ){
	
	
		// MIX INDICATOR - LED corresponding to the rotarys - all active..
		show( ELE_MIX_INDICATOR, RED   );
		show( ELE_MIX_INDICATOR, GREEN );
	
		// TRACK SELECTORS
		show( ELE_TRACK_SELECTORS, GRID_BANK_PLAYMODES );
	
		
		// MATRIX
		show( ELE_MATRIX, GRIDTRACK );
	
	
		// TRACK MUTATORS
		// Default: mutepattern of grid banks
//		show( ELE_TRACK_MUTATORS, GRID_SET_SWITCHMODE );

		// Caution!! There is also some extra code in the ELE_MATRIX / GRID section.
		
	} // // GRID_playmode == GRID_MIX	
	

	

	//
	// GRID  C C M A P  MODE
	//
	else if ( GRID_play_mode == GRID_CCMAP ){
	
		// Use the elements of the page editing view..

		//
		// MIX INDICATOR - LED corresponding to the rotarys
		//
		show( ELE_MIX_INDICATOR, RED   );
		show( ELE_MIX_INDICATOR, GREEN );

		// MIX MASTER
		switch (target_page->mixMode) {
			case INC:
				show (ELE_MIX_MASTER, GREEN);
				break;
				
			case DEC:
				show (ELE_MIX_MASTER, RED);
				break;
		}

		// TRACK_SELECTORS
		switch( GRID_assistant_page->mixTarget ){

			case MIXTGT_USR1:
			case MIXTGT_USR2:
			case MIXTGT_USR3:
			case MIXTGT_USR4:
			case MIXTGT_USR5:
				// Show the available attributes of the CC_MIXMAP
				show( ELE_TRACK_SELECTORS, CC_MIXMAP_ATTRIBUTES );
				break;
		}

		// MIX TARGET field
		show( ELE_MIXTGT, OPTIONS );

		// MATRIX
		show( ELE_MATRIX, GRID_CC_MAP_VALS );				

		// RETURN
		show (ELE_RETURN, GREEN);		
		
		// NUMERIC QUADRANT
		// MIR_write_numeric_C( G_master_tempo );
	}
	
			
	
	// GLOBALS
	//
	show (ELE_GLOBALS, TRANSPORT);
	show (ELE_GLOBALS, BIG_KNOB);
	show( ELE_GLOBALS, CLOCK );

	// Mix target indicator
	show( ELE_MIXTGT, GRID_assistant_page->mixTarget );

	
	// LED_ZOOM indicates the GRID play mode
	switch( GRID_play_mode ){
		
		case GRID_EDIT:
			// MIR_write_dot( LED_SELECT_MASTER, MIR_RED 	);
			MIR_write_dot( LED_ZOOM, MIR_RED );
			MIR_write_dot( LED_ZOOM, MIR_GREEN 	);
			break;
	
		case GRID_MIX:
			// Nothing to do
			break;
	}
		
	
	// PLAY mode
	switch( PLAY_MODE_STATUS ){
	
		case ON:
			MIR_write_dot( LED_ZOOM_PLAY, MIR_RED );
			MIR_write_dot( LED_ZOOM_PLAY, MIR_GREEN );
			MIR_write_dot( LED_ZOOM_PLAY, MIR_BLINK );
			break;
			
		case OFF:
			MIR_write_dot( LED_ZOOM_PLAY, MIR_GREEN );
			break;
	}
	
	

	// EDIT INDICATOR
	// Show activation status: all are active
	for ( i=0; i < GRID_NROF_BANKS; i++ ){

		if ( GRID_p_selection[i] != NULL ){
		
			if ( GRID_p_selection[i]->trackSelection != 0 ){

				// EDIT INDICATOR
				show( ELE_EDIT_INDICATOR, RED   );
				show( ELE_EDIT_INDICATOR, GREEN );
				show( ELE_EDIT_INDICATOR, BLINK );

//				// SHOW TOGGLE AND SOLO LEDs
//				MIR_write_dot( LED_TGGL, MIR_RED 	);
//				MIR_write_dot( LED_TGGL, MIR_GREEN 	);
//				
//				MIR_write_dot( LED_SOLO, MIR_RED 	);
//				MIR_write_dot( LED_SOLO, MIR_GREEN 	);
			}
		}	
	}
	
	if (EDIT_TIMER != OFF){
		show( ELE_EDIT_INDICATOR, BLINK );
	}					


	// EDIT MASTER
	// Shows the GRIDTRACK EDIT mode
	switch( GRIDTRACK_editmode ){
	
		case ON:
			show( ELE_EDIT_MASTER, RED   );
			show( ELE_EDIT_MASTER, GREEN );	
			show( ELE_EDIT_MASTER, BLINK );	
			break;
			
		case OFF:
			show( ELE_EDIT_MASTER, GREEN );				
			break;
	}
		
	
	// Show the ESC button -> taking you back to the page
	MIR_write_dot( LED_RETURN, MIR_GREEN );
	
	// Write the grid mutepattern to the mutator column
	MIR_write_buttool (RHS, GRID_mutepattern, MIR_RED);

	




