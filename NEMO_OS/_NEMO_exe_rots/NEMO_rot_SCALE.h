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





void rot_exe_SCALE( 	Pagestruct* target_page,
						unsigned char rotNdx,
						unsigned char direction ){


	// SCALE shifting around..
	target_page = GRID_assistant_page;	

	switch( rotNdx ){
	
		// BIG KNOB
		case 0:
		// PIT encoder
		case 1:

			// Shift the scale lead left or right
			target_page->scaleLead[G_scale_ndx] = 
					my_shift_bitpattern( 	target_page->scaleLead[G_scale_ndx], 
											12, 
											direction, 
											1 
										);
			// Shift the scale notes left or right
			target_page->scaleNotes[G_scale_ndx] = 
					my_shift_bitpattern( 	target_page->scaleNotes[G_scale_ndx], 
											12, 
											direction, 
											1 
										);
			break;
		#ifdef FEATURE_ENABLE_KB_TRANSPOSE
		case MIXTGT_EXP:
			EDIT_TIMER = ON;

			// Setup alarm for the EDIT TIMER
			cyg_alarm_initialize(	alarm_hdl,
									cyg_current_time() + TIMEOUT_VALUE,
									0 );
			target_page->Track[9]->event_offset[ATTR_MIDITCH] = 0;
			target_page->editAttribute	= NEMO_ATTR_MIDICH;
			modify_parameter
				(&target_page->Track[9]->attr_MCH,
					TRACK_MIN_MIDICH, TRACK_MAX_MIDICH, direction, ON, FIXED);
			break;
		#endif	
	}

}



