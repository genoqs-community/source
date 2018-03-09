

void key_ScaleSelector_mini_functions( unsigned int keyNdx, Pagestruct* target_page ){

	unsigned int 	k = 0;


	// This is the page modulation functionality
	if ( 	(keyNdx == KEY_NOTE_C	) ||
	 // (keyNdx == KEY_NOTE_Cup	) ||
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

		if ( target_page->force_to_scale == OFF ){
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
		target_page->scaleLead_old = target_page->scaleLead[0];

		// target_page->scaleSelection = 0;
//		target_page->scaleNotes[0] = 0;
// j = target_page->force_to_scale;
// target_page->force_to_scale = OFF;
// Get an actual picture of the scale
// extract_scale_and_lead( target_page );
// target_page->force_to_scale = j;	

		// Select the new scale lead
		select_in_scale( target_page, k, LEAD );

		// If the SCALE is set to align, proceed as follows:						
		if ( target_page->SCL_align == ON ){
	
			export_scale_degrees( target_page );
	
			// Export the new lead to the track pitch offsets
			export_scale_lead( target_page );
		}

	}

}



