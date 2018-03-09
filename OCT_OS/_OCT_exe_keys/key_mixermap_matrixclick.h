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




void key_mixermap_matrixclick( 	unsigned short keyNdx, unsigned char* target_ptr ){

	// Used in the key entry routine for the mixer maps modes
	// unsigned char* target_ptr = NULL;

	unsigned short 	i = 0,
					j = 0;
	
	// Compute the coordinates of the pressed key
	// unsigned char	row = row_of( keyNdx );
	unsigned char	col = column_of( keyNdx );

	// D O U B L E - C L I C K
	if ((DOUBLE_CLICK_TARGET == keyNdx) 
			&& (DOUBLE_CLICK_TIMER > DOUBLE_CLICK_ALARM_SENSITIVITY)) {

		// Double click code:
		//
		// Modify the tens value

		// Break up original value in tens and ones.
		i = *target_ptr / 10;
		j = *target_ptr % 10;				
		
		if ( col < 12 ){

			// Compute new tens
			i = 10 * (col + 1);

			// Get new ones value, leave it as it is otherwise
			if ( col < 9 ){
				
				// j = col + 1;
				j = 0;
			}

			// Write the final value into the attribute
			*target_ptr = normalize( i + j, 0, 127 );							
		}
		
		else{

			// Write the final value into the attribute
			*target_ptr = 0;
		}
	} // second click of double click

	else if (DOUBLE_CLICK_TARGET == 0) {

		DOUBLE_CLICK_TARGET = keyNdx;
		DOUBLE_CLICK_TIMER = ON;								
		// Start the Double click Alarm
		cyg_alarm_initialize(	
			doubleClickAlarm_hdl, 
			cyg_current_time() + DOUBLE_CLICK_ALARM_TIME,
			DOUBLE_CLICK_ALARM_TIME );

		// Single click code:
		//
		if ( col < 9 ){
			// Modify the ones value of the MIDICC. 128 is a special value.
			if ( *target_ptr > 127 ){
				i = 0;
			}
			else {
				i = *target_ptr / 10;
			}
			*target_ptr = normalize( i*10 + col + 1, 0, 127 );		
		}
	} // first click of double click

}


