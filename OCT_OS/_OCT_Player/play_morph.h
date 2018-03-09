
// Contains the functions relevant to morphing


// Get the track morph status in the current page
unsigned char get_track_morph_status( Pagestruct* target_page, unsigned char target_row ){

	unsigned char result = 0;

	// Target row is the source row for morphing
	if ( target_row == target_page->morph_source ){
		result = SOURCE;
	}
	else if ( target_row == target_page->morph_target ){		
		result = TARGET;
	}
	
	return result;
}



// Do the actual morphing computation i.e. number crunching
unsigned char compute_morph( 	Pagestruct* target_page, 
								unsigned char in_value,
								unsigned char source_value	){

	unsigned char result = 0;
	
	

	return result;
}



// Execute morphing functions in the page
unsigned char morph_attribute( 	unsigned char in_attribute, 
								Pagestruct* target_page, 
								unsigned char target_row,
								unsigned char in_value 		){

	unsigned char result = 0;

	// Check if the page has morphing activity in this track
	track_morph_status = get_track_morph_status( target_page, target_row );

	// Depending on the status, store the value or recompute it
	switch( track_morph_status ){
	
		case SOURCE:

			// The value will be passed through after storage
			result = in_value;

			// Store the value in the page context
			switch( in_attribute ){
			
				case ATTR_VELOCITY:
					target_page->morph_source_VEL = in_value;
					break;

				case ATTR_PITCH:
					target_page->morph_source_PIT = in_value;
					break;
			}
			break;
			

		case TARGET:

			switch( in_attribute ){
			
				case ATTR_VELOCITY:
					result 	= compute_morph( 	target_page, 
												in_value, 
												target_page->morph_source_VEL );
					break;

				case ATTR_PITCH:
					result 	= compute_morph( 	target_page, 
												in_value, 
												target_page->morph_source_PIT );
					break;
			}
			break;
	}

	return result;
}


