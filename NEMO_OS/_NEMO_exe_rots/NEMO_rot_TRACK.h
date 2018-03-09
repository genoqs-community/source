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






void rot_exec_TRACK( 	Pagestruct* target_page, 
						unsigned char rotNdx, 
						unsigned char direction ){


	unsigned char i = 0;
	
	unsigned char row = 0;
	unsigned char col = 0;

	// EDIT TIMER - Reset on every turn	
	if (EDIT_TIMER == ON) {
		
		EDIT_TIMER = ON;
		// Setup alarm for the EDITOR TIMER
		cyg_alarm_initialize(	alarm_hdl, 
								cyg_current_time() + TIMEOUT_VALUE,
								0 );

		if (rotNdx != ATTR_START) {
			// IF.. because I like the bar display better than the START dot.
			target_page->editAttribute = rotNdx;
		}
	}


		// Execute on the finger_on_a_step mode before anything else
		if ( G_key_pressed != 0 ){

			// Linear scan through the buttons in question: Selectors and Steps.
			for( i=0; i <= 185; i++ ){
				
				// There is a key pressed - in the Step Range
				if (	( G_pressed_keys[i] >=  11 )
					&&	( G_pressed_keys[i] <= 185 ) 

					// And precisely in the position row
					&& 	( (( G_pressed_keys[i]-3) % 11) == 0 ) 
					){

					// Compute the step coordinates. Row is that of the track selection in page
					row = my_bit2ndx( target_page->trackSelection );
					col = column_of	( G_pressed_keys[i] );							
					//d_iag_printf( "rotary /step: row:%d col%d\n", row, col);

					// Just to avoid any bad pointing down the line.
					if ( row > 10 ) break;

					// d_iag_printf( "%d row:%d col:%d  ", pressed_keys[i], row, col );

					// The first knob click activates the step if it is not active
					if ( Step_get_status( target_page->Step[row][col], STEPSTAT_TOGGLE ) == OFF ){

						// Make sure the step gets turned on as we turn the knob
						Step_set_status( target_page->Step[row][col], STEPSTAT_TOGGLE, ON );
					}
					else {
						
						// Finally apply the rotary to the step in question.
						apply_rotary_to_step( rotNdx, direction, target_page, row, col );
					}
				} // key pressed in step range
			} // button iterator

		} // key_pressed != 0
		else{
				
			// Identify track to operate on
			i = my_bit2ndx( target_page->trackSelection );
		
			// Operate on track - React according to selected modus
			switch( NEMO_track_VFF ){
				
				// FLOW
				case VFF_FLOW:		
		//			#include "NEMO_rot_TRACK__FLOW.h"
					break;
		
				// FACTOR
				case VFF_FACTOR:		
					#include "NEMO_rot_TRACK__FACTOR.h"
					break;
		
				// VALUE
				case VFF_VALUE:	
					#include "NEMO_rot_TRACK__VALUE.h"
					break;
			}		
		}


	// MIX rotarys
	if ((rotNdx >= 11) && (rotNdx <= 20)) {

		// Jump to the track of the rotary by changing the track selection in page
		target_page->trackSelection = ( 1 << (rotNdx - 11) );

	} // rotNdx between 11 and 20

}


