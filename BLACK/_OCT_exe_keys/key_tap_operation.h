

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
								target_page->Track[row]->attribute[ATTR_MIDICH], 
//											target_page->Track[row]->attribute[ATTR_PITCH],
								j,
								target_page->Track[row]->attribute[ATTR_TEMPOMUL]		);

			
			// SEND NOTE ON 
			MIDI_send(		MIDI_NOTE, 
							target_page->Track[row]->attribute[ATTR_MIDICH], 
//										target_page->Track[row]->attribute[ATTR_PITCH], 
							j,
							target_page->Track[row]->attribute[ATTR_VELOCITY] );
			
		} // Selected track found
		
	} // Row iterator

}












// Tap a step into the target page at current locator coordinates - 
// ..when done in the page edit locked ( red ) mode. No quantization.
// Basically set the played step into the appropriate locator position.
void tap_step_edit( 	Pagestruct* target_page, 
						unsigned char source_row, 
						unsigned char source_col ){

	unsigned int current_TTC = 0;
	unsigned char 	row = 0,
					target_col = 0,
					target_start = 0,
					i = 0,
					j = 0,
					track = 0
					;

	// Note TTC coordinate before we get started
	current_TTC = TTC_ABS_VALUE;

	// Further action depends on the chainmode selected
	switch( target_page->chainMode ){


		// Recording in the row of the pressed key
		case CHAINMODE_1:
		
			// Use the row of the pressed step:
			row = source_row;

			// Compute the coordinates of the step to be activated			
			if ( current_TTC <= 6 ){
				
				// Adjust step start value to current TTC - book p.189 - Place step in current column
				target_col 		= target_page->Track[row]->attribute[ATTR_LOCATOR] -1;
				target_start 	= current_TTC-1 + 6;
			}
			else {
				
				// Place step in next column, which may have to wrap
				target_col = get_next_tracklocator( target_page->Track[row], 
									target_page->Track[row]->attribute[ATTR_LOCATOR] ) -1;
				target_start 	= current_TTC-1 -6;
			}

			// Adjust the step START value
			target_page->Step[row][target_col]->attribute[ATTR_START] = target_start;

			// Copy velocity, pitch and length from the source step
			// Adjust the velocity for the target row
			target_page->Step[row][target_col]->attribute[ATTR_VELOCITY]
				= target_page->Step[source_row][source_col]->attribute[ATTR_VELOCITY] + 
				  target_page->Track[source_row]->attribute[ATTR_VELOCITY]
				  - target_page->Track[row]->attribute[ATTR_VELOCITY];

			// Adjust the pitch for the target row
			target_page->Step[row][target_col]->attribute[ATTR_PITCH]
				= target_page->Step[source_row][source_col]->attribute[ATTR_PITCH] + 
				  target_page->Track[source_row]->attribute[ATTR_PITCH]
				  - target_page->Track[row]->attribute[ATTR_PITCH];

			// Length is copied directly with no adjustment
			target_page->Step[row][target_col]->attribute[ATTR_LENGTH]
				= target_page->Step[source_row][source_col]->attribute[ATTR_LENGTH];

			// Turn on Step under the locator (adjusted by 1)
			Step_set_status( target_page->Step[row][target_col], STEPSTAT_TOGGLE, ON );

			// No need to play the note, as it was played already before calling this code..			
			break;



		// Recording is done in the currently playing track
		case CHAINMODE_2:
						
			// Iterate through selected tracks and activate the notes. 
			// if hitting an active note, modify its START value acc. to TTC (j)
			for (track=0; track < 5; track++) {

				// Get the status of the bit "track" as ON or OFF.
				i			= 	(target_page->chainCounter & (1 << track) ) >> (track);
				row 		= 	(track * 2) + i;

				// Found selected track, activate the corresponding step
				if ( (target_page->trackSelection) & (1 << row) ) {
					
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
										target_page->Track[row]->attribute[ATTR_MIDICH], 
//											target_page->Track[row]->attribute[ATTR_PITCH],
										j,
										target_page->Track[row]->attribute[ATTR_TEMPOMUL]		);

					// SEND NOTE ON 
					MIDI_send(		MIDI_NOTE, 
									target_page->Track[row]->attribute[ATTR_MIDICH], 
//										target_page->Track[row]->attribute[ATTR_PITCH], 
									j,
									TRACK_DEF_VELOCITY );
					
				}
			}
			break;
			
			

		case CHAINMODE_3:
		
			// More than one track may be selected
			// ..and recording is only done in a track if it is selected 
			// and is currently playing.
			if (target_page->trackSelection == 0) {
				// If no track is selected, exit
				break;
			}

			// Note TTC coordinate
			current_TTC = TTC_ABS_VALUE;
				
			// Iterate through selected tracks and activate the notes. 
			// if hitting an active note, modify its START value acc. to TTC (j)
			for (track=0; track < 2; track++) {

				// Get the status of the bit "track" as ON or OFF.
				i			= 	(target_page->chainCounter & (0xf << (4*track)) ) >> (4*track);
				row 		= 	(track * 4) + i;

				// Found selected track, activate the corresponding step
				if ( (target_page->trackSelection) & (1 << row) ) {
					
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

					// Turn ON Step under the locator (adjusted by 1)
					Step_set_status( target_page->Step[row][target_col], STEPSTAT_TOGGLE, ON );					
				}
			}
			break;
			


		// Recording into the position of the cursor
		case CHAINMODE_4:

			// Get the status of the bit "track" as ON or OFF.
			i 			= 	target_page->chainCounter;
			row			= 	i;

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

			// Copy velocity, pitch and length from the source step
			// Adjust the velocity for the target row
			target_page->Step[row][target_col]->attribute[ATTR_VELOCITY]
				= target_page->Step[source_row][source_col]->attribute[ATTR_VELOCITY] + 
				  target_page->Track[source_row]->attribute[ATTR_VELOCITY]
				  - target_page->Track[row]->attribute[ATTR_VELOCITY];

			// Adjust the pitch for the target row
			target_page->Step[row][target_col]->attribute[ATTR_PITCH]
				= target_page->Step[source_row][source_col]->attribute[ATTR_PITCH] + 
				  target_page->Track[source_row]->attribute[ATTR_PITCH]
				  - target_page->Track[row]->attribute[ATTR_PITCH];

			// Length is copied directly with no adjustment
			target_page->Step[row][target_col]->attribute[ATTR_LENGTH]
				= target_page->Step[source_row][source_col]->attribute[ATTR_LENGTH];

			// Turn on Step under the locator (adjusted by 1)
			Step_set_status( target_page->Step[row][target_col], STEPSTAT_TOGGLE, ON );

			// No need to play the note, as it was played already before calling this code..			
			break;

	} // switch( target_page->chainMode )
}


