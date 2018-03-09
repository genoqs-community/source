


//_______________________________________________________________________________________ 
//
// Take the components of the NOTE ON and do with it whatever fits
void midi_note_execute( 	unsigned char UART_ndx,
							unsigned char status_byte, 
							unsigned char data_byte_1,
							unsigned char data_byte_2	){

	unsigned char 	in_channel		=	status_byte & 0x0F;
	unsigned char 	in_pitch		= 	data_byte_1;
	unsigned char 	in_velocity		= 	data_byte_2;
	unsigned char 	row				=	0;
	unsigned int 	current_TTC		= 	TTC_ABS_VALUE;
	unsigned char 	target_start	=	0;
	unsigned char	target_col		=	0;
	
	// Work on the page under the grid cursor
	Pagestruct* target_page = &Page_repository[GRID_CURSOR];

// PRINT THE NOTE!! (and disregard the rest..)
// d_iag_printf( "note TTC:%d UART:%d status:0x%x pi:0x%x ve:0x%x\n", 
//	current_TTC, UART_ndx, status_byte, data_byte_1, data_byte_2 );
//		return;
	
	// REC STATUS CHECKPOINT
	// Do not continue if ..
	if (		
			// ..we are not in the right mode for MIDI IN recording,
				(	( zoom_level != zoomPAGE 	) 
				&& 	( zoom_level != zoomTRACK  	)
				&& 	( zoom_level != zoomMAP 	)
				&&	( zoom_level != zoomSTEP	)
				)
				
			// ..the page is not in a valid keymode or implicitly zoom mode,
			|| 	(	( target_page->keyMode != KMOD_PAGE 	  	) 
				&&	( target_page->keyMode != KMOD_THIS_TRACK 	)
				&& 	( target_page->keyMode != KMOD_ATTR_MAP 	)
				&& 	( target_page->keyMode != KMOD_THIS_STEP	)
				)

			// ..there is no track enabled for REC,
			|| 	(	( target_page->track_REC_pattern == 0 	) 
				&& 	( target_page->REC_bit == OFF 			)
				)
			
			// ..the page is not playing in GRID,
			||	( is_selected_in_GRID( target_page ) == FALSE )
		){
		
		return;
	}
	
	// Get the index of the REC row. Only one track selected at any time. No quantization.
	row = my_bit2ndx( target_page->track_REC_pattern );

	// Also, do not continue if the in channel does not fit the Track MIDI channel - regardless of port
	if (	( target_page->REC_bit == OFF )
		&&	( in_channel != 
					(	( 	target_page->Track[row]->attribute[ATTR_MIDICH] -1
							+ target_page->Track[row]->event_offset[ATTR_MIDICH] ) %16) )
		){
		return;
	}

	// Handle NOTE OFF messages -> pretend they don't exist, and simply transform them.
	if ( ( status_byte & 0xF0 ) == MIDI_CMD_NOTE_OFF ){

		// transform into NOTE ON with VEL 0
		in_velocity = OFF;
	}

	// Check if we are traditionally recording or step recording
	switch ( target_page->REC_bit ){
	
		// Assigning pitch to selected track or step recording, or forcing input to scale, or..
		case ON:
		
			// Modify the scale composition
			if (	( target_page->scaleStatus == SCALE_MOD )
				||	( target_page->scaleStatus == SCALE_SEL )
				){

				// Do not react to note off signals
				if ( in_velocity != OFF ){
					modify_scale_composition( target_page, pitch_to_noteNdx( in_pitch ) );
				}
			}

			// Send incoming MIDI note further out, but force input it to scale
			else if (	( key_pressed == 0 )
					&&	( zoom_level != zoomSTEP  )
				){

				// Force the incoming MIDI notes to currently active scale
				force_input_to_scale( 	target_page, (UART_ndx*16) + in_channel+1, 
										in_pitch, in_velocity );
			}
			// Something is pressed - need to look deeper
			else if ( key_pressed != 0 ){
			
				// This is either direct step recording or track pitch assignment
				assign_note_to_selection( target_page, in_pitch, in_velocity );
			}
			break;
	

		// Recording notes classically - with tracks enabled for recording
		case OFF:
		
			// Get the index of the REC row. One track selected at any time. No quantization.
			row = my_bit2ndx( target_page->track_REC_pattern );

			// Compute the coordinates of the step to be activated	
			// Adjust the step start value according to current TTC. Logic: see book p.189
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

				// May need to record in a different row, if next column is part of a wrap
//				row = 
			}

			// Record in notes to the rec enabled track
			record_note_to_track( 	target_page, 	row, 		target_col, 
									target_start, 	in_pitch, 	in_velocity );
			break;
								
	} // switch on RUNBIT
}




//_______________________________________________________________________________________ 
//
// Take the components of the CONTROLLER message and do with it whatever fits
void midi_controller_execute( 	unsigned char status_byte, 
								unsigned char data_byte_1,
								unsigned char data_byte_2	){

	unsigned char 	controller_number		= 	data_byte_1;
	unsigned char 	controller_value		= 	data_byte_2;
	unsigned int 	current_TTC				= 	TTC_ABS_VALUE;

	unsigned char 	row=0,
					target_col=0;

	// Work on the page under the grid cursor
	Pagestruct* target_page = &Page_repository[GRID_CURSOR];

	//	// PRINT THE CONTENT!! (and disregard the rest..)
	//	d_iag_printf( "note at TTC:%d ch:0x%x pi:0x%x ve:0x%x\n", 
	//		current_TTC, status_byte, data_byte_1, data_byte_2 );
	//	return;
		
	// REC STATUS CHECKPOINT
	// Do not continue if ..
	if (		
			// ..we are not in the right mode for MIDI IN recording,
				(	( zoom_level != zoomPAGE 	) 
				&&	( zoom_level != zoomMIXMAP	)
				&& 	( zoom_level != zoomTRACK  	)
				&& 	( zoom_level != zoomMAP 	)
				)
				
			// ..the page is not in a valid keymode or implicitly zoom mode,
			|| 	(	( target_page->keyMode != KMOD_PAGE 	  	) 
				&&	( target_page->keyMode != KMOD_THIS_TRACK 	)
				&& 	( target_page->keyMode != KMOD_ATTR_MAP 	)
				)
			
			// ..the page is not playing in GRID,
			||	( is_selected_in_GRID( target_page ) == FALSE )
		){
		
		return;
	}
	
	// Get the index of the REC row. Only one track selected at any time. No quantization.
	row = my_bit2ndx( target_page->track_REC_pattern );

	// Compute the coordinates of the step to be activated	
	// Adjust the step start value according to current TTC. Reasoning: see book p.189
	if ( current_TTC <= 6 ){		

		// Place step in current column
		target_col 		= target_page->Track[row]->attribute[ATTR_LOCATOR] -1;
	}
	else {
		// Place step in next column, which may have to wrap- -1 is locator vs col notation offset.
		target_col 		= 
			get_next_tracklocator( 	target_page->Track[row], 
									target_page->Track[row]->attribute[ATTR_LOCATOR] ) -1;
	}
 

	// Decide how to act on incoming controller data
	switch( zoom_level ){
	
		case zoomMIXMAP:
			// Assign the CC data to the selected map
			CCMAP_learn( status_byte, controller_number, controller_value );		
			break;

		
		default:
			if (	( target_page->track_REC_pattern != 0 )
				&&	( RUNBIT == ON )
				){

				// Update the controller number in the track MCC and controller value in step MCC
				target_page->Track[row]->attribute[ATTR_MIDICC] = controller_number;
				target_page->Track[row]->event_offset[ATTR_MIDICC] = 0;

				target_page->Step[row][target_col]->attribute[ATTR_MIDICC] = controller_value;
			}
			break;
	}
}






//_______________________________________________________________________________________ 
//
// Execute the content of the bender message received
void midi_bender_execute( 	unsigned char status_byte, 
							unsigned char data_byte_1,
							unsigned char data_byte_2 ){

	unsigned int 	current_TTC				= 	TTC_ABS_VALUE;

	unsigned char 	row=0;
	unsigned char	target_col=0;
	unsigned char 	controller_value = 0;

	// Work on the page under the grid cursor
	Pagestruct* target_page = &Page_repository[GRID_CURSOR];

	// Compile the bender value from the 2 data bytes and map to an 8 bit value
	controller_value = map_to_8bit( combineDataBytes( data_byte_1, data_byte_2) );

	// d_iag_printf( "received bender: 0x%x\n", data_short );

	//	// PRINT THE CONTENT!! (and disregard the rest..)
	//	d_iag_printf( "note at TTC:%d ch:0x%x pi:0x%x ve:0x%x\n", 
	//		current_TTC, status_byte, data_byte_1, data_byte_2 );
	//	return;
		
	// REC STATUS CHECKPOINT
	// Do not continue if ..
	if (		
			// ..we are not in the right mode for MIDI IN recording,
				(	( zoom_level != zoomPAGE 	) 
				&&	( zoom_level != zoomMIXMAP	)
				&& 	( zoom_level != zoomTRACK  	)
				&& 	( zoom_level != zoomMAP 	)
				)
				
			// ..the page is not in a valid keymode or implicitly zoom mode,
			|| 	(	( target_page->keyMode != KMOD_PAGE 	  	) 
				&&	( target_page->keyMode != KMOD_THIS_TRACK 	)
				&& 	( target_page->keyMode != KMOD_ATTR_MAP 	)
				)
			
			// ..the page is not playing in GRID,
			||	( is_selected_in_GRID( target_page ) == FALSE )
		){
		
		return;
	}
	
	// Get the index of the REC row. Only one track selected at any time. No quantization.
	row = my_bit2ndx( target_page->track_REC_pattern );

	// Compute the coordinates of the step to be activated	
	// Adjust the step start value according to current TTC. Reasoning: see book p.189
	if ( current_TTC <= 6 ){		

		// Place step in current column
		target_col 		= target_page->Track[row]->attribute[ATTR_LOCATOR] -1;
	}
	else {
		// Place step in next column, which may have to wrap- -1 is locator vs col notation offset.
		target_col 		= 
			get_next_tracklocator( 	target_page->Track[row], 
									target_page->Track[row]->attribute[ATTR_LOCATOR] ) -1;
	}
 

	// Decide how to act on incoming controller data
	switch( zoom_level ){
	
		case zoomMIXMAP:
			// Assign the CC data to the selected map
			// CCMAP_learn( status_byte, controller_number, controller_value );		
			break;

		
		default:
			if (	( target_page->track_REC_pattern != 0 )
				&&	( RUNBIT == ON )
				){

				// Update the controller number in the track MCC and controller value in step MCC
				target_page->Track[row]->attribute[ATTR_MIDICC] = MIDICC_BENDER;
				target_page->Track[row]->event_offset[ATTR_MIDICC] = 0;

				target_page->Step[row][target_col]->attribute[ATTR_MIDICC] = controller_value;
			}
			break;
	}
}


//_______________________________________________________________________________________ 
//
// Interpret a program change message:
// Channel 10 selects the scene to be played: in a %16 fashion
// Channels 1-9 toggle the pages to play - in a %16 fashion
void midi_PGMCH_execute( unsigned char midi_byte, unsigned char UART_ndx ){
 
	unsigned char target_ndx = 0;
	Pagestruct* temp_page = NULL;
	unsigned char page_ndx = 0;
	unsigned char channel = 0;
	
	// Work on the page under the grid cursor
	// Pagestruct* target_page = &Page_repository[GRID_CURSOR];

	// Only act when REC bit is on..
	// if ( target_page->REC_bit == OFF ) return;
	if ( GRID_assistant_page->REC_bit == OFF ) return;

	// Determine channel
	channel = ( running_status_byte_IN_UART[ UART_ndx ] & 0x0F ) + 1; 

	// Compute the target ndx to operate on
	target_ndx = midi_byte % 16;

	// This is the Status byte for the UART at hand - giving the MIDI channel
	switch( channel ){
	
		// Switch to the respective grid set, according to simulated selector
		case 10:

			// RECALL chosen GRID set into the current GRID selection
			current_GRID_set = target_ndx;
			import_GRID_set( target_ndx );							
			break;
			
		// Toggle pages in the respective banks
		case 9: case 8: case 7: case 6: case 5: case 4: case 3: case 2: case 1:
			
			// Calculation: book p.343
			page_ndx = ( 10 * target_ndx ) + ( 9 - channel );

			// Find the pointer to the page in question
			temp_page = &Page_repository[ page_ndx ];

			// Toggle the page playing status 
			switch ( is_selected_in_GRID( temp_page ) ) {
				case ON:
					grid_select( temp_page, OFF );
					break;

				case OFF:
					grid_select( temp_page, ON  );
					break;
			}

			break;
	}
}


