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



	// BIG KNOB
	if ( rotNdx == 0 ){

		// Function used in Track mode, and in Page mode under track selection
		modify_selectedTrackAttribute_VALUE( target_page, i, direction, OFF );
	}


	//	
	// EDIT rotary: editing the hardwired attributes
	//
	else if ((rotNdx >= 1) && (rotNdx <= 10)) {

		// Point the display to the changing attribute					
		NEMO_selectedTrackAttribute = rotNdx;

		// Modify hardwired EDIT Attribute for trackSelection
		switch( rotNdx ){
			
			case NEMO_ATTR_VELOCITY:
				modify_parameter( 	&target_page->Track[i]->attr_VEL, 
									TRACK_MIN_VELOCITY, TRACK_MAX_VELOCITY, direction, ON, FIXED );				
				break;

			case NEMO_ATTR_PITCH:
				modify_signed_parameter( 	&target_page->Track[i]->attr_PIT, 
									TRACK_MIN_PITCH, TRACK_MAX_PITCH, direction, ON, FIXED );				
				break;

			case NEMO_ATTR_LENGTH:
				modify_parameter( 	&target_page->Track[i]->LEN_factor, 
									TRACK_MIN_LENGTH, TRACK_MAX_LENGTH, direction, OFF, FIXED );				
				break;

			case NEMO_ATTR_START:
				modify_parameter( 	&target_page->Track[i]->STA_factor, 
									TRACK_MIN_STAFACTOR, TRACK_MAX_STAFACTOR, direction, OFF, FIXED );				
				break;
		} // switch (rotNdx)
		
	} // rotNdx between 1 and 10

