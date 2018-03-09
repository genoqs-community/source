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





void key_ScaleSelector_mini_functions( unsigned int keyNdx, Pagestruct* target_page ){

	unsigned int 	k = 0;


	// This is the page modulation functionality
	if ( 	(keyNdx == KEY_NOTE_C	) ||
			(keyNdx == KEY_NOTE_Cis	) ||
			(keyNdx == KEY_NOTE_D	) ||
			(keyNdx == KEY_NOTE_Dis	) ||
			(keyNdx == KEY_NOTE_E	) ||
			(keyNdx == KEY_NOTE_F	) ||
			(keyNdx == KEY_NOTE_Fis	) ||
			(keyNdx == KEY_NOTE_G	) ||
			(keyNdx == KEY_NOTE_Gis	) ||
			(keyNdx == KEY_NOTE_A	) ||
			(keyNdx == KEY_NOTE_Ais	) ||
			(keyNdx == KEY_NOTE_B	) 		
		){

		if ( target_page->scaleStatus == OFF ){
			return;
		}

		// Transform key Index to a note index;
		k = keyNdx_to_noteNdx( keyNdx );

		// From SCALE_MOD scale key operations
	
		// The book says to Select clicked note as the lead, 
		// ..only if the note is in the scale.
		if ( how_selected_in_current_scale( target_page, k ) != ON  ) {
			return;
		}

		// Store current lead so we know how much to shift pitches
		target_page->scaleLead_old = target_page-> scaleLead[G_scale_ndx];

		// Select the new scale lead
		select_in_scale( target_page, k, LEAD, G_scale_ndx );

		// If the SCALE is set to align, proceed as follows:						
		if ( target_page->SCL_align == ON ){
	
			export_scale_degrees( target_page );
	
			// Export the new lead to the track pitch offsets
			export_scale_lead( target_page );
		}

	}

}



