

// Tap a step into the target page
// Place a note in the selcted tracks at current locator coordinates. 
// No quantization. More than one track may be selected. Only playing tracks are considered.
void tap_step( Pagestruct* target_page ){

	unsigned int current_TTC = 0;
	unsigned char 	row = 0,
					target_col = 0,
					target_start = 0,
					j = 0
					;

	if (target_page->trackSelection == 0) {
		// Needs at least one selected track
		return;
	}
	
	// Note TTC coordinate
	current_TTC = TTC_ABS_VALUE;
	
	// Iterate through selected tracks and activate the notes. 
	// if hitting an active note, modify its START value acc. to TTC (j)
	for (row=0; row<MATRIX_NROF_ROWS; row++) {

		// Found a selected track, activate the corresponding step, if the track is playing
		if (	( (target_page->trackSelection & (1<<row) ) != 0 )
			&&	( target_page->Track[row]->attribute[ATTR_LOCATOR != 0] )
			&&	( target_page->Track[row]->chain_data[PLAY] == target_page->Track[row] )
			){
			
			// Compute the coordinates of the step to be activated
			
			// Adjust the step start value according to current TTC
			// Reasoning: see book p.189
			if ( current_TTC <= 6 ){
				
				// Place step in current column
				target_col 		= target_page->Track[row]->attribute[ATTR_LOCATOR] -1;
				target_start 	= current_TTC-1 + 6;
			}
			else {
				
				// Place step in next column, which may have to wrap
				target_col 		= 
					get_next_tracklocator( 	target_page->Track[row], 
											target_page->Track[row]->attribute[ATTR_LOCATOR] ) -1;
				target_start 	= current_TTC-1 -6;
			}

			// Adjust the step START value
			target_page->Step[row][target_col]->attribute[ATTR_START] = target_start;

			// Turn on Step under the locator (adjusted by 1)
			Step_set_status( target_page->Step[row][target_col], STEPSTAT_TOGGLE, ON );


			// d_iag_printf( "tapped: row:%d t_col%d t_start:%d\n", 
			//							row, target_col, target_start );

			// Play the note that was just set, for a better feeling
			
			j = 		target_page->Step [row][target_col]->attribute[ATTR_PITCH]
	  				+ 	target_page->Track[row]->attribute[ATTR_PITCH]
					+ 	(target_page->attribute[ATTR_PITCH] - MIDDLE_C)
					+ 	(target_page->Track[row]->lead_pitch_offset);					

			// NOTE OFF build		
			MIDI_OFF_build( 	target_page->Step[row][target_col], 0,
								target_page->Track[row]->attribute[ATTR_MIDICH]
								+ target_page->Track[row]->event_offset[ATTR_MIDICH], 
								j,
								target_page->Track[row]->attribute[ATTR_TEMPOMUL]		);

			
			// SEND NOTE ON 
			MIDI_send(		MIDI_NOTE, 
							target_page->Track[row]->attribute[ATTR_MIDICH]
							+ target_page->Track[row]->event_offset[ATTR_MIDICH], 
							j,
							target_page->Track[row]->attribute[ATTR_VELOCITY] );
			
		} // Selected track found
		
	} // Row iterator

}


