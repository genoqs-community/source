
	switch (VIEWER_page->mixAttribute) {

		// SET RELEVANT VALUES
		//
		case ATTR_START:
			// min = TRACK_MIN_START;
			// max = TRACK_MAX_START;
			min = TRACK_MIN_STAFACTOR;
			max = TRACK_MAX_STAFACTOR;
			break;
					
		case ATTR_POSITION:	
			min = TRACK_MIN_POSITION;
			max = TRACK_MAX_POSITION;
			break;
		
		case ATTR_LENGTH:
			// min = TRACK_MIN_LENGTH;
			// max = TRACK_MAX_LENGTH;
			min = TRACK_MIN_LENFACTOR;
			max = TRACK_MAX_LENFACTOR;
			break;
		
		case ATTR_MIDICH:
			min = TRACK_MIN_MIDICH;
			max = TRACK_MAX_MIDICH;
			break;

		// Parameter range 0-127
		case ATTR_VELOCITY:
			min = TRACK_MIN_VELOCITY;
			max = TRACK_MAX_VELOCITY;
			break;
	
		case ATTR_PITCH:
			min = TRACK_MIN_PITCH;
			max = TRACK_MAX_PITCH;
			break;

		case ATTR_MIDICC:
			min = TRACK_MIN_MIDICC;
			max = TRACK_MAX_MIDICC;
			break;

		case ATTR_AMOUNT:		//newcomer: this is actually representing the CC AMOUNT
			min = TRACK_MIN_AMOUNT;
			max = TRACK_MAX_AMOUNT;
			break;
		
		// Direction may vary
		case ATTR_DIRECTION:
			min = TRACK_MIN_DIRECTION;
			max = TRACK_MAX_DIRECTION;
			break;

		// Parameter range 0-16
		case ATTR_GROOVE:
			min = TRACK_MIN_GROOVE;
			max = TRACK_MAX_GROOVE;
			break;
	}	

	// MODIFY ATTRIBUTES
	//
	// If track is part of trackSelection, operate on all selected tracks	
	if ( (1<<(trackNdx)) & VIEWER_page->trackSelection) {

		// Want to modify all tracks in selection
		for (i=0; i<MATRIX_NROF_ROWS; i++){

			// Selected tracks only, considering the mixMode
			switch (VIEWER_page->mixMode) {
	
				case INC:	// Positive action on selected tracks
	
					// controls the quickturn timer in modify_parameter call
					j = ON;
	
					if ( (1<<i) & VIEWER_page->trackSelection ) {

						if (VIEWER_page->mixAttribute == ATTR_POSITION) {
							Page_wrap_track ( VIEWER_page, i, direction, 1);
						}
						
						if (    (VIEWER_page->mixAttribute == ATTR_START) 
							 || (VIEWER_page->mixAttribute == ATTR_LENGTH)  )	{

							// do not use the quickturn timer
							j = OFF;
						}
						
						if (VIEWER_page->mixAttribute == ATTR_MIDICC) {
							k = FIXED;
						}
						else {
							k = FIXED;
						}


						if (	( VIEWER_page->mixAttribute == ATTR_PITCH )
							||	( VIEWER_page->mixAttribute == ATTR_VELOCITY )
							||	( VIEWER_page->mixAttribute == ATTR_MIDICC )
							){
								
							modify_signed_parameter 
								(&VIEWER_page->Track[i]->attribute[VIEWER_page->mixAttribute], 	
									min, max, direction, j, k); // j was originally ON
						}
						else if ( VIEWER_page->mixAttribute == ATTR_LENGTH ){

							modify_parameter 
								(&VIEWER_page->Track[i]->LEN_factor, 	
									min, max, direction, j, k); // j was OFF
						}
						else if ( VIEWER_page->mixAttribute == ATTR_START ){

							modify_parameter 
								(&VIEWER_page->Track[i]->STA_factor, 	
									min, max, direction, j, k); // j was OFF
						}
						else{
							modify_parameter 
								(&VIEWER_page->Track[i]->attribute[VIEWER_page->mixAttribute], 	
									min, max, direction, j, k); // j was originally ON
						}
					}
					break;


				case DEC:	// Negative action on un-selected tracks

					// controls the quickturn timer in modify_parameter call
					j = OFF;

					if ( ! ((1<<i) & VIEWER_page->trackSelection) ) {

						if (VIEWER_page->mixAttribute == ATTR_POSITION){
							Page_wrap_track (VIEWER_page, i, direction^0x03, 1);
						}

						if (    (VIEWER_page->mixAttribute == ATTR_START) 
							 || (VIEWER_page->mixAttribute == ATTR_LENGTH)  )	{

							// do not use the quickturn timer
							j = OFF;
						}
	
						if (VIEWER_page->mixAttribute == ATTR_MIDICC) {
							k = FIXED;
						}
						else {
							k = FIXED;
						}

						if ( VIEWER_page->mixAttribute == ATTR_MIDICC ){
							modify_signed_parameter 
								(&VIEWER_page->Track[i]->attribute[VIEWER_page->mixAttribute], 	
									min, max, direction^0x03, j, k); // j was OFF
						}
						else if ( VIEWER_page->mixAttribute == ATTR_LENGTH ){

							modify_parameter 
								(&VIEWER_page->Track[i]->LEN_factor, 	
									min, max, direction, j, k); // j was OFF
						}
						else if ( VIEWER_page->mixAttribute == ATTR_START ){

							modify_parameter 
								(&VIEWER_page->Track[i]->STA_factor, 	
									min, max, direction^0x03, j, k); // j was OFF
						}
						else{
							modify_parameter 
								(&VIEWER_page->Track[i]->attribute[VIEWER_page->mixAttribute], 	
									min, max, direction^0x03, j, k); // j was OFF
						}
					}
					break;
			} // switch
		} // for
	} // if 	
	
	// Track is being edited by itself, not part of a selection
	else {

		switch (VIEWER_page->mixMode) {

			
			case INC:

				// control the quickturn timer
				j = ON;

				if (VIEWER_page->mixAttribute == ATTR_POSITION){
					Page_wrap_track (VIEWER_page, trackNdx, direction, 1);
				}

				if (   (VIEWER_page->mixAttribute == ATTR_START)
					 ||(VIEWER_page->mixAttribute == ATTR_LENGTH) ) {

					// do not use the quickturn timer
					j = OFF;
				}

				if (VIEWER_page->mixAttribute == ATTR_MIDICC) {
					k = FIXED;
				}
				else {
					k = FIXED;
				}


				if (	( VIEWER_page->mixAttribute == ATTR_PITCH )
					||	( VIEWER_page->mixAttribute == ATTR_VELOCITY )
					||	( VIEWER_page->mixAttribute == ATTR_MIDICC )
					){

					modify_signed_parameter 
						(&VIEWER_page->Track[trackNdx]->attribute[VIEWER_page->mixAttribute], 	
							min, max, direction, j, k); // j was originally ON
				}
				else if ( VIEWER_page->mixAttribute == ATTR_LENGTH ){

					modify_parameter 
						(&VIEWER_page->Track[trackNdx]->LEN_factor, 	
							min, max, direction, j, k); // j was OFF
				}
				else if ( VIEWER_page->mixAttribute == ATTR_START ){

					modify_parameter 
						(&VIEWER_page->Track[trackNdx]->STA_factor, 	
							min, max, direction, j, k); // j was OFF
				}
				else{
					modify_parameter 
						(&VIEWER_page->Track[trackNdx]->attribute[VIEWER_page->mixAttribute], 	
							min, max, direction, j, k); // j was originally ON
				}

				break;


			case DEC:

				for (i=0; i<MATRIX_NROF_ROWS; i++) {

					// controls the quickturn timer in modify_parameter call
					j = OFF;

					// Modify all tracks that are not selected
					if (i != trackNdx) {
					
						if (VIEWER_page->mixAttribute == ATTR_POSITION){
							Page_wrap_track( VIEWER_page, i, direction^0x03, 1 );
						}
					
						if (   (VIEWER_page->mixAttribute == ATTR_START)
							 ||(VIEWER_page->mixAttribute == ATTR_LENGTH) ) {
							
							// do not use the quickturn timer
							j = OFF;
						}

						if (VIEWER_page->mixAttribute == ATTR_MIDICC) {
							k = FIXED;
						}
						else {
							k = FIXED;
						}

						if (	( VIEWER_page->mixAttribute == ATTR_PITCH )
							||	( VIEWER_page->mixAttribute == ATTR_VELOCITY )
							||	( VIEWER_page->mixAttribute == ATTR_MIDICC )
							){

							modify_signed_parameter 
								(&VIEWER_page->Track[i]->attribute[VIEWER_page->mixAttribute], 	
									min, max, direction^0x03, j, k);	// j was OFF
						}
						else if ( VIEWER_page->mixAttribute == ATTR_LENGTH ){

							modify_parameter 
								(&VIEWER_page->Track[i]->LEN_factor, 	
									min, max, direction^0x03, j, k); // j was OFF
						}
						else if ( VIEWER_page->mixAttribute == ATTR_START ){

							modify_parameter 
								(&VIEWER_page->Track[i]->STA_factor, 	
									min, max, direction^0x03, j, k); // j was OFF
						}
						else{
							modify_parameter 
								(&VIEWER_page->Track[i]->attribute[VIEWER_page->mixAttribute], 	
									min, max, direction^0x03, j, k);	// j was OFF
						}

					} // if
				} // for
				break;
				
		} // switch

	} // else
			
	// MAX GROOVE VALUE update for the page - needed for groove play
	if (VIEWER_page->mixAttribute == ATTR_GROOVE) {
		Page_update_maxGroove (VIEWER_page);
	}





