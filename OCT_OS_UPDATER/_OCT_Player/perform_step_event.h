

// Returns the value of the new event offset based on given parameters
unsigned char compute_event_offset( unsigned char current_value,
									unsigned char min_value,
									unsigned char max_value,
									unsigned char current_offset,	
									unsigned char max_offset,
									signed char event_value,
									unsigned char event_val_rnd ){

	unsigned char headroom = 0;
	unsigned char result = 0;
	signed short new_offset = 0;

	// Calculate the headroom for the operation
	headroom = my_min_value( max_offset, max_value - current_value );

	// Attempt to calculate new offset
	new_offset = current_offset + event_value;
	
	// Can happen if the event value is negative - recalculate the new offset
	if ( new_offset < 0 ){

		// Recalculate the new offset: subtract the event from the max offset
		new_offset = max_offset	- abs( current_offset + event_value ) +1;	
	}
		
	// Make sure the result falls withing valid value bounds
	result = normalize( new_offset % (headroom + 1), min_value, max_value );

	// Randomize result if needed
	if ( event_val_rnd == TRUE ){
		
		result = rand() % (result+1);
	}

	return result;
}




// Perform the event programmed in the in step onto the givent track
void perform_step_event( 	Stepstruct* in_step, 
							Trackstruct* target_track, 
							Pagestruct* target_page,
							unsigned char in_row){
				
	unsigned char direction = 0;

	unsigned char* target_val = NULL;
	unsigned char current_value 	= 0;
	unsigned char min_value		 	= 0;
	unsigned char max_value			= 0;
	unsigned char current_offset	= 0;
	unsigned char max_offset		= 0;
	signed char event_value			= 0;
	unsigned char event_val_rnd		= FALSE;

	// The event value is valid across all event attributes
	// event_value = in_step->attribute[ATTR_AMOUNT];

	// Compute the event value considering its factorization
	event_value =
		(	( in_step->attribute[ATTR_AMOUNT]
			  * Track_AMT_factor[	target_track->AMT_factor
				 					+ target_track->event_offset[ATTR_AMOUNT] ] )
			/ FACTOR_NEUTRAL_VALUE );


	// Do not continue if the event is void..
	if ( event_value == 0 ) return;


	// The position attribute requires special handling
	if (	(( in_step->event_data & 0x0F) + 1 ) == ATTR_POSITION ){
		
		// Set the direction
		direction = INC;
		if ( event_value < 0 ) direction = DEC;

		Page_wrap_track( 	target_page, 
							in_row,
							direction, 
							abs( event_value )%16);

		// We are done for POSITION, so return - other attributes handled below
		return;
	}


	// All other attribute events - make the value assignments for final number crunching..
	switch( (in_step->event_data & 0x0F) + 1 ){
	
		case ATTR_VELOCITY:
		
			target_val 		= &target_track->event_offset[ATTR_VELOCITY];
			current_value	= target_track->VEL_factor;
			min_value		= TRACK_MIN_VELFACTOR;
			max_value		= TRACK_MAX_VELFACTOR;
			current_offset	= target_track->event_offset[ATTR_VELOCITY];
			max_offset		= target_track->event_max[ATTR_VELOCITY];				
			break;


		case ATTR_PITCH:

			target_val 		= &target_track->event_offset[ATTR_PITCH];
			current_value	= target_track->PIT_factor;
			min_value		= TRACK_MIN_PITFACTOR;
			max_value		= TRACK_MAX_PITFACTOR;
			current_offset	= target_track->event_offset[ATTR_PITCH];
			max_offset		= target_track->event_max[ATTR_PITCH];
			break;


		case ATTR_LENGTH:
			
			target_val 		= &target_track->event_offset[ATTR_LENGTH];
			current_value	= target_track->LEN_factor;
			min_value		= TRACK_MIN_LENFACTOR;
			max_value		= TRACK_MAX_LENFACTOR;
			current_offset	= target_track->event_offset[ATTR_LENGTH];
			max_offset		= target_track->event_max[ATTR_LENGTH];
			break;


		case ATTR_START:
		
			target_val 		= &target_track->event_offset[ATTR_START];
			current_value	= target_track->STA_factor;
			min_value		= TRACK_MIN_STAFACTOR;
			max_value		= TRACK_MAX_STAFACTOR;
			current_offset	= target_track->event_offset[ATTR_START];
			max_offset		= target_track->event_max[ATTR_START];
			break;


		case ATTR_DIRECTION:

			target_val 		= &target_track->event_offset[ATTR_DIRECTION];
			current_value	= target_track->attribute[ATTR_DIRECTION];
			min_value		= TRACK_MIN_DIRECTION;
			max_value		= TRACK_MAX_DIRECTION;
			current_offset	= target_track->event_offset[ATTR_DIRECTION];
			max_offset		= target_track->event_max[ATTR_DIRECTION];				
			break;


		case ATTR_AMOUNT:

			target_val 		= &target_track->event_offset[ATTR_AMOUNT];
			current_value	= target_track->AMT_factor;
			min_value		= TRACK_MIN_AMTFACTOR;
			max_value		= TRACK_MAX_AMTFACTOR;
			current_offset	= target_track->event_offset[ATTR_AMOUNT];
			max_offset		= target_track->event_max[ATTR_AMOUNT];
			break;


		case ATTR_GROOVE:

			target_val 		= &target_track->event_offset[ATTR_GROOVE];
			current_value	= target_track->GRV_factor;
			min_value		= TRACK_MIN_GRVFACTOR;
			max_value		= TRACK_MAX_GRVFACTOR;
			current_offset	= target_track->event_offset[ATTR_GROOVE];
			max_offset		= target_track->event_max[ATTR_GROOVE];				
			break;


		// -- verified
		case ATTR_MIDICC:

			target_val 		= &target_track->event_offset[ATTR_MIDICC];
			current_value	= target_track->MCC_factor;
			min_value		= TRACK_MIN_MCCFACTOR;
			max_value		= TRACK_MAX_MCCFACTOR;
			current_offset	= target_track->event_offset[ATTR_MIDICC];
			max_offset		= target_track->event_max[ATTR_MIDICC];
			break;


		case ATTR_MIDICH:

			target_val 		= &target_track->event_offset[ATTR_MIDICH];
			current_value	= target_track->attribute[ATTR_MIDICH];
			min_value		= TRACK_MIN_MIDICH;
			max_value		= TRACK_MAX_MIDICH;
			current_offset	= target_track->event_offset[ATTR_MIDICH];
			max_offset		= target_track->event_max[ATTR_MIDICH];				
			break;
	}

	// Steps that have a non-zero groove parameter experience a random event value
	if ( in_step->attribute[ ATTR_GROOVE ] != 0 ){
		
		event_val_rnd = TRUE;
	}

	// Number-crunch the new offset value and update the event offset.
	*target_val = compute_event_offset(	current_value, min_value, max_value,
										current_offset, max_offset, event_value, event_val_rnd );
}




