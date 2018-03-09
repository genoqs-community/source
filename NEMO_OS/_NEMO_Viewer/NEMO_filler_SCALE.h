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


	// TRACK SELECTORS	
	// Light them green, as they are all available
	MIR_write_dot( 1, MIR_GREEN );
	MIR_write_dot( 2, MIR_GREEN );
	MIR_write_dot( 3, MIR_GREEN );
	MIR_write_dot( 4, MIR_GREEN );
	// Show selected the current octave slot- 1 should be replaced by G_scale_ndx
	MIR_write_dot( G_scale_ndx, MIR_RED 	);
	MIR_write_dot( G_scale_ndx, MIR_BLINK 	);
	
	// TRACK MUTATORS
	// Green, indicating there is something there: preset scales!
	MIR_write_dot( 187, MIR_GREEN );
	MIR_write_dot( 188, MIR_GREEN );
	MIR_write_dot( 189, MIR_GREEN );
	MIR_write_dot( 190, MIR_GREEN );


	// MATRIX
	// Shows in the top row the selected scale
	show_OCTAVE_CIRCLE_scale_selection( GRID_assistant_page );
	

	// Show the MIDI Clock state or transport bar
	if ( is_pressed_key( KEY_STOP ) ){
	
		switch( G_clock_source ){
		
			case OFF:
				// Light the options green
				MIR_write_dot( LED_PLAY1, 	MIR_GREEN );
				MIR_write_dot( LED_PAUSE, 	MIR_GREEN );
				break;

			case EXT:
				MIR_write_dot( LED_PAUSE,	MIR_GREEN );
				MIR_write_dot( LED_PAUSE,	MIR_RED );
				MIR_write_dot( LED_PAUSE, 	MIR_BLINK );				

				MIR_write_dot( LED_PLAY1, 	MIR_GREEN );
				break;

			case INT:
				MIR_write_dot( LED_PLAY1,	MIR_GREEN );
				MIR_write_dot( LED_PLAY1,	MIR_RED );
				MIR_write_dot( LED_PLAY1, 	MIR_BLINK );				

				MIR_write_dot( LED_PAUSE, 	MIR_GREEN );
				break;
		}
	}
	else{

		// When we are not looking at the clock status
		show( ELE_GLOBALS, TRANSPORT );
	}


	// ESC
	// Show the ESC LED - leaving the scale mode of the GRID asstn page
	MIR_write_dot( LED_RETURN, MIR_GREEN );

	
//	// In edit mode this is showing the page VEL
//	if ( GRID_play_mode != GRID_EDIT ){
//		show (ELE_GLOBALS, BIG_KNOB);
//	}
	
	// No idea what this does.. anyway.. probably nothing bad.
	show( ELE_GLOBALS, CLOCK );
		
	
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
	
	
	// REC bit status
	if ( GRID_assistant_page->REC_bit == ON ){
			
		MIR_write_dot( LED_RECORD, MIR_RED 	 );
		MIR_write_dot( LED_RECORD, MIR_GREEN );		
		MIR_write_dot( LED_RECORD, MIR_BLINK );		
	}	

	// Show the ESC button -> taking you back to the page
	MIR_write_dot( LED_RETURN, MIR_GREEN );


	// EDIT MASTER showing the scale edit mode
	switch( GRID_assistant_page->scaleStatus ){

		case OFF:			
			break;

		case SCALE_MOD:
			show( ELE_EDIT_MASTER, RED   );
			show( ELE_EDIT_MASTER, BLINK );
			break;
							
		case SCALE_SEL:
			show( ELE_EDIT_MASTER, GREEN );		
			show( ELE_EDIT_MASTER, BLINK );
			break;
	}


//	// MIX TARGET target indicator
//	show( ELE_MIXTGT, GRID_assistant_page->mixTarget );
	
//	// LED_ZOOM indicates the GRID play mode
//	switch( GRID_play_mode ){
//		
//		case GRID_EDIT:
//			// MIR_write_dot( LED_SELECT_MASTER, MIR_RED 	);
//			MIR_write_dot( LED_ZOOM, MIR_RED );
//			MIR_write_dot( LED_ZOOM, MIR_GREEN 	);
//			break;
//	
//		case GRID_MIX:
//			// Nothing to do
//			break;
//	}
	
//	// MIX INDICATOR - LED corresponding to the rotarys - all active..
//	show( ELE_MIX_INDICATOR, RED   );
//	show( ELE_MIX_INDICATOR, GREEN );




