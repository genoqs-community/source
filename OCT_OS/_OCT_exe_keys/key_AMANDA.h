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




	// D O U B L E - C L I C K  C O N S T R U C T
	// DOUBLE CLICK SCENARIO
	if (	( DOUBLE_CLICK_TARGET == keyNdx ) 
		&& 	( DOUBLE_CLICK_TIMER   > DOUBLE_CLICK_ALARM_SENSITIVITY ) ) {

		// Double click code
		// ...
		if ( keyNdx == KEY_ZOOM_GRID ){
		
			// Make sure to clear any message codes
			GRID_message_code = OFF;

			// Ressurect from the seemingly dead..
			G_zoom_level = zoomGRID;
		}		
		
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
		
		// nothing to do
	}





