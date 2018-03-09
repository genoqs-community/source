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


//_______________________________________________________________________________________
//
// Take the components of the NOTE ON and do with it whatever fits
#ifdef NEMO

void midi_note_execute( 	unsigned char inputMidiBus,
							unsigned char status_byte,
							unsigned char data_byte_1,
							unsigned char data_byte_2	){

	// Valid channel values start at 1, so need an offset here
 	unsigned char 	inputMidiChan	=	(status_byte & 0x0F) + 1; 	// Range = [1,16];
	unsigned char 	in_pitch		= 	data_byte_1;
	unsigned char 	in_velocity		= 	data_byte_2;
	unsigned char 	row				=	0;
	unsigned int 	current_TTC		= 	G_TTC_abs_value;
	unsigned char 	target_start	=	0;
	unsigned char	target_col		=	0;
	Pagestruct* 	target_page 	= 	NULL;

	// Set the default output bus and channel to the input bus and channel.
 	// This is where the incoming MIDI note will be sent to, unless re-channeled, below.
 	int outputMidiBus  = inputMidiBus; 		// Range [0, 1].
 	int outputMidiChan = inputMidiChan; 	// Range [1, 16].


	// Handle NOTE OFF messages -> pretend they don't exist, and simply transform them.
	if ( ( status_byte & 0xF0 ) == MIDI_CMD_NOTE_OFF ){

		// transform into NOTE ON with VEL 0
		in_velocity = OFF;
	}


	// Act depending on the zoom level. Assign target page, maybe force to scale..
 	switch( G_zoom_level ){

 	case zoomPAGE:
 		//Wilson - Keyboard Transpose
 		// Only work on the current page.
 		target_page = &Page_repository[GRID_CURSOR];
 		if ( ( G_run_bit == ON )
 			&& 	( inputMidiChan > 0 )
 			&&	(	( Page_getTrackRecPattern(target_page) == 0 	)
 			&& 	( target_page->REC_bit == OFF 			))			) {

 				transpose_selection( target_page, in_pitch, in_velocity, inputMidiChan );
 		 		return;

 		}
 	case zoomTRACK:
 	case zoomSTEP:
 		// Only work on the current page.
 		target_page = &Page_repository[GRID_CURSOR];
 		break;

 	case zoomGRID:
 	case zoomSCALE:
 		// Only work on the current page.
 		target_page = GRID_assistant_page;

 		if ( target_page->REC_bit == ON ){
 			// Force the incoming MIDI notes to currently active scale
 			force_input_to_scale( 	target_page, (inputMidiBus*16) + inputMidiChan,
								in_pitch, in_velocity );
 		}
 		// Nothing else to do..
		return;

		break;
 	}

 	// REC STATUS CHECKPOINT - works only in one of the valid zoom levels
	if (
				(	( G_zoom_level == zoomPAGE 	)
				|| 	( G_zoom_level == zoomTRACK )
				||	( G_zoom_level == zoomSTEP	)
				||	( G_zoom_level == zoomGRID	)
				||	( G_zoom_level == zoomSCALE )
				)

			// ..neither tracks nor page are armed for record.
			|| 	(	( Page_getTrackRecPattern(target_page) != 0 )
				|| 	( target_page->REC_bit == ON 			)
				)

			// ..the page is not playing in GRID,
			||	( is_selected_in_GRID( target_page ) == TRUE )
		){
		// Basically continue execution.
	}else{
		// Nothing to do in here..
		return;
	}

	// Get the index of the REC row. Only one track selected at any time. No quantization.
	// row = my_bit2ndx( target_page->track_REC_pattern );


	// Check if we are traditionally recording or step recording
	switch ( target_page->REC_bit ){

		// -----------------------------------------------------------------------------
		// PAGE RECORD MODE
		// -----------------------------------------------------------------------------

		// Assigning pitch to selected track or step recording, or forcing input to scale, or..
		case ON:

			// Modify the scale composition
			if (	( target_page->scaleStatus == SCALE_MOD )
				||	( target_page->scaleStatus == SCALE_SEL )
				){

  				// Do not react to note off signals
  				if ( in_velocity != OFF ){

  					// Page channel is the same as row0 channel
					if ( target_page->Track[9]->attr_MCH == ((inputMidiBus*16) + inputMidiChan) )
  						modify_scale_composition( target_page, pitch_to_noteNdx( in_pitch ), G_scale_ndx );
  				}
			}

			// Send incoming MIDI note further out, but force input it to scale
			else if (	( G_key_pressed == 0 )
					&&	( target_page->trackSelection == 0 )
					&&	( G_zoom_level != zoomSTEP  )
				){

  				// Force the incoming MIDI notes to currently active scale
 				force_input_to_scale( 	target_page, (inputMidiBus*16) + inputMidiChan,
  										in_pitch, in_velocity );
			}

			// Something is pressed - need to look deeper
			// Or we are in STEP MODE and record right into the step
			else if (	( G_key_pressed != 0 )
					||	( target_page->trackSelection != 0 )
					||	( G_zoom_level == zoomSTEP )
					){

				// This is either direct step recording or track pitch assignment
			 	assign_note_to_selection( target_page, in_pitch, in_velocity );
			}
			break;



		// -----------------------------------------------------------------------------
		// TRACK RECORD MODE
		// -----------------------------------------------------------------------------

  		// Recording notes classically - with tracks enabled for recording
  		case OFF:

  			// Iterate through the rows - to enable multi-track recording
 			for ( row = 0; row < MATRIX_NROF_ROWS; row++ ) {

  				// If track not record enabled, continue, remember it otherwise
 				if ( (Page_getTrackRecPattern(target_page) & (1 << row)) == 0 ) {
  					continue;
  				}


  				// CHANNEL FILTER
 				// --------------
 				// Here are the channel filter rules:
 				// 1. If trackMidiBus is one of the two virtual busses (> 1), recording is not allowed (atm).
 				// 2. If multiple tracks are currently in record select, only MIDI notes with matching bus and channel are accepted.
 				// 3. If however a single track is in record select, the incoming MIDI note is accepted and re-channelled to match the track's bus and channel.

 				// Get the target track's output (MCH) MIDI bus and channel in range [0,3] and [1,16] resp.
 				// Note that we work internally with a MCH range of [1, 64].

 				// Decide which head row to use - relevant for track chaining, where head row may be different
 				card8 headRow = row;
 				if ( target_page->CHAINS_PLAY_HEAD == TRUE ) {
 					headRow = row_of_track( target_page, target_page->Track[row]->chain_data[HEAD] );
 				}

 				int trackMidiBus  	= (target_page->Track[headRow]->attr_MCH -1) / 16;
 				int trackMidiChan 	= ((target_page->Track[headRow]->attr_MCH + target_page->Track[headRow]->event_offset[ATTR_MIDICH] -1) % 16) + 1;
 				int trackArmedCount = Page_getTrackRecCount( target_page );

 				if ( trackMidiBus > 1 ) {
  					continue;
  				}

 				if ( trackArmedCount > 1 ) {
 					if ( (inputMidiBus != trackMidiBus) || (inputMidiChan != trackMidiChan) ) {
 						continue;
 					}
 				}

 				if ( trackArmedCount == 1 ) {
 					outputMidiBus  = trackMidiBus;
 					outputMidiChan = trackMidiChan;
  				}


 				// RECORD
 				// ------
 				// Only record when the sequencer is running and in record mode.
 				// However, even in Stopped mode, the MIDI input is echoed to the outputs, after optional re-channelling.

 				if ( (G_run_bit == ON) && (G_track_rec_bit == ON) ) {

 					// Compute the coordinates of the step to be activated
 					// Adjust the step start value according to current TTC. Logic: see book p.189
 					if ( current_TTC <= 6 ) {

 						// Place step in current column
 						target_col 		= target_page->Track[row]->attr_LOCATOR -1;
 						target_start 	= current_TTC-1 + 6;
 					}
 					else {

 						// Place step in next column, which may have to wrap
 						target_col 		= get_next_tracklocator( target_page->Track[row], target_page->Track[row]->attr_LOCATOR ) -1;
 						target_start 	= current_TTC-1 -6;

 						// Make sure we stay within valid bounds
 						if ( target_start < STEP_MIN_START ) target_start = STEP_MIN_START;

 						// May need to record in a different row, if next column is part of a wrap
 						// row = ... // this is the known LDT bug. Dirty fix now. Breaks on skipped rows.
 						if ( target_col < target_page->Track[row]->attr_LOCATOR-1 ){
 							// There was a wrap, lookup the next row to record
 							row = row_of_track( target_page, target_page->Track[row]->chain_data[NEXT] );
 						}
 					}

 					// Record in notes to the rec enabled track
 					record_note_to_track( 	target_page, 	row, 		target_col,
											target_start, 	in_pitch, 	in_velocity );

 					// Mark the page as touched
 					target_page->page_clear = OFF;
  				}

  			} // Row iterator


 			// MONITORING
 			// ----------
  			// Play the note straight through
 			// Note that the note may have been re-channelled in the meantime.
 			// MIDI_NOTE_new() also uses the internal [1,64] range for MIDI channels.

 			MIDI_NOTE_new( outputMidiBus * 16 + outputMidiChan, scale_pitch(target_page, in_pitch), in_velocity, 0 );

			// Play MIDI queue elements which are due just before current timestamp, including the above..
			play_MIDI_queue( G_MIDI_timestamp );

			break;
	} // switch on G_run_bit
}


#else

//_______________________________________________________________________________________
//
// Take the components of the NOTE ON and do with it whatever fits
void midi_note_execute( 	unsigned char inputMidiBus,
							unsigned char status_byte,
							unsigned char data_byte_1,
							unsigned char data_byte_2	){

	// Valid channel values start at 1, so need an offset here
 	unsigned char 	inputMidiChan	=	(status_byte & 0x0F) + 1; 	// Range = [1,16];
	unsigned char 	in_pitch		= 	data_byte_1;
	unsigned char 	in_velocity		= 	data_byte_2;
	unsigned char 	row				=	0;
	unsigned int 	current_TTC		= 	G_TTC_abs_value;
	unsigned char 	target_start	=	0;
	unsigned char	target_col		=	0;

	// Only work on the current page.
	Pagestruct* target_page 		= &Page_repository[GRID_CURSOR];

	// Set the default output bus and channel to the input bus and channel.
 	// This is where the incoming MIDI note will be sent to, unless re-channelled, below.
 	int outputMidiBus  = inputMidiBus; 		// Range [0, 1].
 	int outputMidiChan = inputMidiChan; 	// Range [1, 16].

 	//Wilson - Keyboard Transpose
	if ( (G_run_bit == ON)
		&& 	( inputMidiChan > 0 )
		&&	(	( Page_getTrackRecPattern(target_page) == 0 	)
		&& 	( target_page->REC_bit == OFF 			))			) {

			// Handle NOTE OFF messages -> pretend they don't exist, and simply transform them.
			if ( ( status_byte & 0xF0 ) == MIDI_CMD_NOTE_OFF ){

				// transform into NOTE ON with VEL 0
				in_velocity = OFF;
			}
			transpose_selection( target_page, in_pitch, in_velocity, inputMidiChan );
			return;
	}

	// PRINT THE NOTE!! (and disregard the rest..)
	// d_iag_printf( "note TTC:%d UART:%d status:0x%x pi:0x%x ve:0x%x\n",
	//	current_TTC, UART_ndx, status_byte, data_byte_1, data_byte_2 );
	//		return;

	// REC STATUS CHECKPOINT
	// Do not continue if ..
	if (
			// ..we are not in the right mode for MIDI IN recording,
				(	( G_zoom_level != zoomPAGE 	)
				&& 	( G_zoom_level != zoomTRACK )
				&& 	( G_zoom_level != zoomMAP 	)
				&&	( G_zoom_level != zoomSTEP	)
				&&	( G_zoom_level != zoomGRID	)
#ifdef NEMO
				&&	( G_zoom_level != zoomSCALE )
#endif
				)

			// ..neither tracks nor page are armed for record.
			|| 	(	( Page_getTrackRecPattern(target_page) == 0 	)
				&& 	( target_page->REC_bit == OFF 			)
				)

			// ..the page is not playing in GRID,
			||	( is_selected_in_GRID( target_page ) == FALSE )
		){

		return;
	}

	// Get the index of the REC row. Only one track selected at any time. No quantization.
	// row = my_bit2ndx( target_page->track_REC_pattern );


	// Handle NOTE OFF messages -> pretend they don't exist, and simply transform them.
	if ( ( status_byte & 0xF0 ) == MIDI_CMD_NOTE_OFF ){

		// transform into NOTE ON with VEL 0
		in_velocity = OFF;
	}

	// d_iag_printf("OK 0\n");

	// Check if we are traditionally recording or step recording
	switch ( target_page->REC_bit ){

		// -----------------------------------------------------------------------------
		// PAGE RECORD MODE
		// -----------------------------------------------------------------------------

		// Assigning pitch to selected track or step recording, or forcing input to scale, or..
		case ON:

			// d_iag_printf("OK 1\n");

			// Modify the scale composition
			if (	( target_page->scaleStatus == SCALE_MOD )
				||	( target_page->scaleStatus == SCALE_SEL )
				){

  				// Do not react to note off signals
  				if ( in_velocity != OFF ){

  					// Page channel is the same as row0 channel
					if ( target_page->Track[9]->attr_MCH == ((inputMidiBus*16) + inputMidiChan) )
  						modify_scale_composition( target_page, pitch_to_noteNdx( in_pitch ), G_scale_ndx );
  				}
			}

			// Send incoming MIDI note further out, but force input it to scale
			else if (	( G_key_pressed == 0 )
					&&	( target_page->trackSelection == 0 )
					&&	( G_zoom_level != zoomSTEP  )
				){

  				// Force the incoming MIDI notes to currently active scale
 				force_input_to_scale( 	target_page, (inputMidiBus*16) + inputMidiChan,
  										in_pitch, in_velocity );
			}

			// Something is pressed - need to look deeper
			// Or we are in STEP MODE and record right into the step
			else if (	( G_key_pressed != 0 )
					||	( target_page->trackSelection != 0 )
					||	( G_zoom_level == zoomSTEP )
					){

				// d_iag_printf("OK 2\n");

				// This is either direct step recording or track pitch assignment
			 	assign_note_to_selection( target_page, in_pitch, in_velocity );
			}
			break;



		// -----------------------------------------------------------------------------
		// TRACK RECORD MODE
		// -----------------------------------------------------------------------------

  		// Recording notes classically - with tracks enabled for recording
  		case OFF:

  			// Iterate through the rows - to enable multi-track recording
 			for ( row = 0; row < MATRIX_NROF_ROWS; row++ ) {

  				// If track not record enabled, continue, remember it otherwise
 				if ( (Page_getTrackRecPattern(target_page) & (1 << row)) == 0 ) {
  					continue;
  				}


  				// CHANNEL FILTER
 				// --------------
 				// Here are the channel filter rules:
 				// 1. If trackMidiBus is one of the two virtual busses (> 1), recording is not allowed (atm).
 				// 2. If multiple tracks are currently in record select, only MIDI notes with matching bus and channel are accepted.
 				// 3. If however a single track is in record select, the incoming MIDI note is accepted and re-channelled to match the track's bus and channel.

 				// Get the target track's output (MCH) MIDI bus and channel in range [0,3] and [1,16] resp.
 				// Note that we work internally with a MCH range of [1, 64].

 				// Decide which head row to use - relevant for track chaining, where head row may be different
 				card8 headRow = row;
 				if ( target_page->CHAINS_PLAY_HEAD == TRUE ) {
 					headRow = row_of_track( target_page, target_page->Track[row]->chain_data[HEAD] );
 				}

 				int trackMidiBus  	= (target_page->Track[headRow]->attr_MCH -1) / 16;
 				int trackMidiChan 	= ((target_page->Track[headRow]->attr_MCH + target_page->Track[headRow]->event_offset[ATTR_MIDICH] -1) % 16) + 1;
 				int trackArmedCount = Page_getTrackRecCount( target_page );

 				if ( trackMidiBus > 1 ) {
  					continue;
  				}

 				if ( trackArmedCount > 1 ) {
 					if ( (inputMidiBus != trackMidiBus) || (inputMidiChan != trackMidiChan) ) {
 						continue;
 					}
 				}

 				if ( trackArmedCount == 1 ) {
 					outputMidiBus  = trackMidiBus;
 					outputMidiChan = trackMidiChan;
  				}


 				// RECORD
 				// ------
 				// Only record when the sequencer is running and in record mode.
 				// However, even in Stopped mode, the MIDI input is echoed to the outputs, after optional re-channelling.

 				if ( (G_run_bit == ON) && (G_track_rec_bit == ON) ) {

 					// Compute the coordinates of the step to be activated
 					// Adjust the step start value according to current TTC. Logic: see book p.189
 					if ( current_TTC <= 6 ) {

 						// Place step in current column
 						target_col 		= target_page->Track[row]->attr_LOCATOR -1;
 						target_start 	= current_TTC-1 + 6;
 					}
 					else {

 						// Place step in next column, which may have to wrap
 						target_col 		= get_next_tracklocator( target_page->Track[row], target_page->Track[row]->attr_LOCATOR ) -1;
 						target_start 	= current_TTC-1 -6;

 						// Make sure we stay within valid bounds
 						if ( target_start < STEP_MIN_START ) target_start = STEP_MIN_START;

 						// May need to record in a different row, if next column is part of a wrap
 						// row = ... // this is the known LDT bug. Dirty fix now. Breaks on skipped rows.
 						if ( target_col < target_page->Track[row]->attr_LOCATOR-1 ){
 							// There was a wrap, lookup the next row to record
 							row = row_of_track( target_page, target_page->Track[row]->chain_data[NEXT] );
 						}
 					}

 					// Record in notes to the rec enabled track
 					record_note_to_track( 	target_page, 	row, 		target_col,
											target_start, 	in_pitch, 	in_velocity );

 					// Mark the page as touched
 					target_page->page_clear = OFF;
  				}

  			} // Row iterator


 			// MONITORING
 			// ----------
  			// Play the note straight through
 			// Note that the note may have been re-channelled in the meantime.
 			// MIDI_NOTE_new() also uses the internal [1,64] range for MIDI channels.

 			MIDI_NOTE_new( outputMidiBus * 16 + outputMidiChan, scale_pitch(target_page, in_pitch), in_velocity, 0 );

			// Play MIDI queue elements which are due just before current timestamp, including the above..
			play_MIDI_queue( G_MIDI_timestamp );

			break;
	} // switch on G_run_bit
}
#endif



// Record CC data into recording page tracks
void Track_record_CC( 	Pagestruct* 	target_page,
 						unsigned char 	inputMidiBus,
						unsigned char 	status_byte,
						unsigned char 	controller_number,
						unsigned char 	controller_value
						){

 	unsigned char 	inputMidiChan	=	(status_byte & 0x0F) + 1;	// range = [1,16].
	unsigned int 	current_TTC		= 	G_TTC_abs_value;
	unsigned char 	row				=	0;
	unsigned char 	target_col		=	0;

	// Set the default output bus and channel to the input bus and channel.
 	// This is where the incoming MIDI note will be sent to, unless re-channelled, below.

 	int outputMidiBus  = inputMidiBus; 		// Range [0, 1].
 	int outputMidiChan = inputMidiChan; 	// Range [1, 16].

	// Iterate through the rows - this enables multi-track recording
	for ( row=0; row < MATRIX_NROF_ROWS; row++ ){

		// If track not record enabled, continue, remember it otherwise
 		if ( (Page_getTrackRecPattern(target_page) & (1 << row)) == 0 ){
  			continue;
  		}

  		// CHANNEL FILTER
 		// --------------
 		// Here are the channel filter rules:
 		// 1. If trackMidiBus is one of the two virtual busses (> 1), recording is not allowed (atm).
 		// 2. If multiple tracks are currently in record select, only MIDI notes with matching bus and channel are accepted.
 		// 3. If however a single track is in record select, the incoming MIDI note is accepted and re-channelled to match the track's bus and channel.

 		// Get the target track's output (MCH) MIDI bus and channel in range [0,3] and [1,16] resp.
 		// Note that we work internally with a MCH range of [1, 64].

 		// Decide which head row to use - relevant for track chaining, where head row may be different
 		card8 headRow = row;
 		if ( target_page->CHAINS_PLAY_HEAD == TRUE ) {
 			headRow = row_of_track( target_page, target_page->Track[row]->chain_data[HEAD] );
 		}

 		int trackMidiBus  	= (target_page->Track[headRow]->attr_MCH -1) / 16;
 		int trackMidiChan 	= ((target_page->Track[headRow]->attr_MCH + target_page->Track[headRow]->event_offset[ATTR_MIDICH] -1) % 16) + 1;
 		int trackArmedCount = Page_getTrackRecCount( target_page );

 		if ( trackMidiBus > 1 ) {
  			continue;
  		}

 		if ( trackArmedCount > 1 ) {
 			if ( (inputMidiBus != trackMidiBus) || (inputMidiChan != trackMidiChan) ) {
 				continue;
 			}
 		}

 		if ( trackArmedCount == 1 ) {
 			outputMidiBus  = trackMidiBus;
 			outputMidiChan = trackMidiChan;
 		}


 		// RECORD
 		// ------
  		// Compute the coordinates of the step to be activated
  		// Adjust the step start value according to current TTC. Reasoning: see book p.189

 		if ( (G_run_bit == ON) && (G_track_rec_bit == ON) ) {

 			if ( current_TTC <= 6 ){
 				// Place step in current column
 				target_col 	= target_page->Track[row]->attr_LOCATOR -1;
 			}
 			else {
 				// Place step in next column, which may have to wrap- -1 is locator vs col notation offset.
 				target_col 	= get_next_tracklocator( target_page->Track[row],
 													 target_page->Track[row]->attr_LOCATOR ) -1;
 			}

 			// Update the controller number in the track MCC and controller value in step MCC
 			target_page->Track[row]->attr_MCC = controller_number;
 			target_page->Track[row]->event_offset[ATTR_MIDICC] = 0;
 			target_page->Step[row][target_col]->attr_MCC = controller_value;
 			target_page->page_clear = OFF;
 		}
 	} // row iterator


 	// MONITORING
 	// ----------
 	// Play the CC straight through
 	// Note that the CC may have been re-channelled in the meantime.

 	MIDI_CC_new( outputMidiBus * 16 + outputMidiChan, controller_number, controller_value, 0 );

 	// Play MIDI queue elements which are due just before current timestamp, including the above..
 	play_MIDI_queue( G_MIDI_timestamp );
}




//_______________________________________________________________________________________
//
// Take the components of the CONTROLLER message and do with it whatever fits
void midi_controller_execute( 	unsigned char UART_ndx,
								unsigned char status_byte,
								unsigned char data_byte_1,
								unsigned char data_byte_2	){

	unsigned char 	controller_number		= 	data_byte_1;
	unsigned char 	controller_value		= 	data_byte_2;

	// Work on the page under the grid cursor
	Pagestruct* target_page = NULL;

	// Act according to zoom level
	switch( G_zoom_level ){

		case zoomMIXMAP:

   			// Direct learning to the grid map page, if necessary
//  		if( GRID_MAP0_status == ON ){
//				target_page = GRID_assistant_page;
//   		}
//			else{
//				target_page = &Page_repository[GRID_CURSOR];
//			}

			// Point to the grid assistant page - only one learning!
			target_page = GRID_assistant_page;

			// Here we expect CC map learning - Assign the CC data to the selected map
			CCMAP_learn( target_page, UART_ndx, status_byte, controller_number, controller_value );
			break;


		case zoomPAGE:
		case zoomTRACK:
		case zoomMAP:

			// Point to the cursor page
			target_page = &Page_repository[GRID_CURSOR];

			// Make sure the page is actually playing in GRID
			if ( is_selected_in_GRID( target_page ) == TRUE ){

				// This is legacy controller recording
				Track_record_CC( target_page, UART_ndx, status_byte, controller_number, controller_value );
			}
			break;
	}
}






//_______________________________________________________________________________________
//
// Execute the content of the bender message received
void midi_bender_execute( 	unsigned char inputMidiBus,				// Range = [0, 1].
							unsigned char status_byte,
							unsigned char data_byte_1,				// LSB bender val.
							unsigned char data_byte_2 ){			// MSB bender val.

	unsigned char 	inputMidiChan	=	(status_byte & 0x0F) + 1;	// Range = [1, 16].
	unsigned int 	current_TTC		= 	G_TTC_abs_value;
	unsigned char 	row				=	0;
	unsigned char	target_col		=	0;

	// Set the default output bus and channel to the input bus and channel.
	// This is where the incoming MIDI note will be sent to, unless re-channelled, below.

	int outputMidiBus  = inputMidiBus; 		// Range [0, 1].
	int outputMidiChan = inputMidiChan; 	// Range [1, 16].


	// Work on the page under the grid cursor
	Pagestruct* target_page = &Page_repository[GRID_CURSOR];

	// Compile the bender value from the 2 data bytes and map to an 8 bit value
	// controller_value = map_to_8bit( combineDataBytes( data_byte_1, data_byte_2) );

	// REC STATUS CHECKPOINT
	// Do not continue if ..
	if (
			// ..we are not in the right mode for MIDI IN recording,
				(	( G_zoom_level != zoomPAGE 	)
				&&	( G_zoom_level != zoomMIXMAP	)
				&& 	( G_zoom_level != zoomTRACK  	)
				&& 	( G_zoom_level != zoomMAP 	)
				)

			// ..the page is not playing in GRID,
			||	( is_selected_in_GRID( target_page ) == FALSE )
		){

		return;
	}


	// Iterate through the rows - to enable multi-track recording
	for ( row = 0; row < MATRIX_NROF_ROWS; row++ ) {

		// If track not record enabled, continue, remember it otherwise
		if ( (Page_getTrackRecPattern(target_page) & (1 << row)) == 0 ){
			continue;
		}


		// CHANNEL FILTER
		// --------------
		// Here are the channel filter rules:
		// 1. If trackMidiBus is one of the two virtual busses (> 1), recording is not allowed (atm).
		// 2. If multiple tracks are currently in record select, only MIDI notes with matching bus and channel are accepted.
		// 3. If however a single track is in record select, the incoming MIDI note is accepted and re-channelled to match the track's bus and channel.

		// Get the target track's output (MCH) MIDI bus and channel in range [0,3] and [1,16] resp.
		// Note that we work internally with a MCH range of [1, 64].

		// Decide which head row to use - relevant for track chaining, where head row may be different
		card8 headRow = row;
		if ( target_page->CHAINS_PLAY_HEAD == TRUE ) {
			headRow = row_of_track( target_page, target_page->Track[row]->chain_data[HEAD] );
		}

		int trackMidiBus  	= (target_page->Track[headRow]->attr_MCH -1) / 16;
		int trackMidiChan 	= ((target_page->Track[headRow]->attr_MCH + target_page->Track[headRow]->event_offset[ATTR_MIDICH] -1) % 16) + 1;
 		int trackArmedCount = Page_getTrackRecCount( target_page );

		if ( trackMidiBus > 1 ) {
			continue;
		}

		if ( trackArmedCount > 1 ) {
			if ( (inputMidiBus != trackMidiBus) || (inputMidiChan != trackMidiChan) ) {
				continue;
			}
		}

		if ( trackArmedCount == 1 ) {
			outputMidiBus  = trackMidiBus;
			outputMidiChan = trackMidiChan;
		}



		// RECORD
		// ------
		// If runnning, record the bender data as a special kind of CC into the track.

		if ( (G_run_bit == ON) && (G_track_rec_bit) ) {

			// Compute the coordinates of the step to be activated
			// Adjust the step start value according to current TTC. Reasoning: see book p.189

			if ( current_TTC <= 6 ) {
				// Place step in current column
				target_col = target_page->Track[row]->attr_LOCATOR -1;
			}
			else {
				// Place step in next column, which may have to wrap- -1 is locator vs col notation offset.
				target_col = get_next_tracklocator( target_page->Track[row],
													target_page->Track[row]->attr_LOCATOR ) -1;
			}


			// Decide how to act on incoming controller data
			switch( G_zoom_level ){
			case zoomMIXMAP:
				// Assign the CC data to the selected map
				// ..
				break;

			default:
				// Update the controller number in the track MCC and controller value in step MCC
				target_page->Track[row]->attr_MCC = MIDICC_BENDER;
				target_page->Track[row]->event_offset[ATTR_MIDICC] = 0;

				// Step position attribute holds the LSB 7-bit nibble for the bender value
				target_page->Step[row][target_col]->attr_POS = data_byte_1;

				// Step ATTR_MIDICC holds the MSB 7-bit nibble of the bender value
				target_page->Step[row][target_col]->attr_MCC = data_byte_2;
				target_page->page_clear = OFF;
				break;
			}
		}
	} // row iterator



	// MONITORING
	// ----------
	// Play the bender value straight through
	// Note that the bender value may have been re-channelled in the meantime.

	unsigned int benderVal = (((unsigned int) data_byte_2) << 7) | (data_byte_1 & 0x7F);
	MIDI_CC_new( outputMidiBus * 16 + outputMidiChan, MIDI_BENDER, benderVal, 0 );

	// Play MIDI queue elements which are due just before current timestamp, including the above..
	play_MIDI_queue( G_MIDI_timestamp );
}



//_______________________________________________________________________________________
//
// Execute the content of the pressure message received. Only one data byte!
void midi_pressure_execute( 	unsigned char inputMidiBus,				// Range = [0, 1].
								unsigned char status_byte,
								unsigned char data_byte 	){

	unsigned char 	inputMidiChan		=	(status_byte & 0x0F) + 1;	// Range = [1, 16].
	unsigned int 	current_TTC			= 	G_TTC_abs_value;
	unsigned char 	row					=	0;
	unsigned char	target_col			=	0;
	unsigned char 	controller_value 	= 	0;


	// Set the default output bus and channel to the input bus and channel.
	// This is where the incoming MIDI note will be sent to, unless re-channelled, below.

	int outputMidiBus  = inputMidiBus; 		// Range [0, 1].
	int outputMidiChan = inputMidiChan; 	// Range [1, 16].


	// Work on the page under the grid cursor
	Pagestruct* target_page = &Page_repository[GRID_CURSOR];

	// Compile the bender value from the 2 data bytes and map to an 8 bit value
	controller_value = data_byte;

	// REC STATUS CHECKPOINT
	// Do not continue if ..
	if (
			// ..we are not in the right mode for MIDI IN recording,
				(	( G_zoom_level != zoomPAGE 	)
				&&	( G_zoom_level != zoomMIXMAP	)
				&& 	( G_zoom_level != zoomTRACK  	)
				&& 	( G_zoom_level != zoomMAP 	)
				)

			// ..the page is not playing in GRID,
			||	( is_selected_in_GRID( target_page ) == FALSE )
		){

		return;
	}

	// Iterate through the rows - to enable multi-track recording
	for ( row = 0; row < MATRIX_NROF_ROWS; row++ ){

		// If track not record enabled, continue, remeber it otherwise
		if ( (Page_getTrackRecPattern(target_page) & (1 << row)) == 0 ){
			continue;
		}


		// CHANNEL FILTER
		// --------------
		// Here are the channel filter rules:
		// 1. If trackMidiBus is one of the two virtual busses (> 1), recording is not allowed (atm).
		// 2. If multiple tracks are currently in record select, only MIDI notes with matching bus and channel are accepted.
		// 3. If however a single track is in record select, the incoming MIDI note is accepted and re-channelled to match the track's bus and channel.

		// Get the target track's output (MCH) MIDI bus and channel in range [0,3] and [1,16] resp.
		// Note that we work internally with a MCH range of [1, 64].

		// Decide which head row to use - relevant for track chaining, where head row may be different
		card8 headRow = row;
		if ( target_page->CHAINS_PLAY_HEAD == TRUE ) {
			headRow = row_of_track( target_page, target_page->Track[row]->chain_data[HEAD] );
		}

		int trackMidiBus  	= (target_page->Track[headRow]->attr_MCH -1) / 16;
		int trackMidiChan 	= ((target_page->Track[headRow]->attr_MCH + target_page->Track[headRow]->event_offset[ATTR_MIDICH] -1) % 16) + 1;
		int trackArmedCount = Page_getTrackRecCount( target_page );

		if ( trackMidiBus > 1 ) {
			continue;
		}

		if ( trackArmedCount > 1 ) {
			if ( (inputMidiBus != trackMidiBus) || (inputMidiChan != trackMidiChan) ) {
				continue;
			}
		}

		if ( trackArmedCount == 1 ) {
			outputMidiBus  = trackMidiBus;
			outputMidiChan = trackMidiChan;
		}



		// RECORD
		// ------
		// Compute the coordinates of the step to be activated
		// Adjust the step start value according to current TTC. Reasoning: see book p.189

		if ( (G_run_bit == ON) && (G_track_rec_bit == ON) ) {
			if ( current_TTC <= 6 ) {
				// Place step in current column
				target_col = target_page->Track[row]->attr_LOCATOR -1;
			}
			else {
				// Place step in next column, which may have to wrap- -1 is locator vs col notation offset.
				target_col = get_next_tracklocator( target_page->Track[row],
													target_page->Track[row]->attr_LOCATOR ) -1;
			}

			// Decide how to act on incoming controller data
			switch( G_zoom_level ){
			case zoomMIXMAP:
				// Assign the CC data to the selected map
				// ..
				break;

			default:
				// Update the controller number in the track MCC and controller value in step MCC
				target_page->Track[row]->attr_MCC = MIDICC_PRESSURE;
				target_page->Track[row]->event_offset[ATTR_MIDICC] = 0;
				target_page->Step[row][target_col]->attr_MCC = controller_value;
				target_page->page_clear = OFF;
				break;

			} // G_zoom_level switch
		}
	} // row iterator



	// MONITORING
	// ----------
	// Play the pressure value straight through
	// Note that the pressure value may have been re-channelled in the meantime.

	MIDI_CC_new( outputMidiBus * 16 + outputMidiChan, MIDI_PRESSURE, controller_value, 0 );

	// Play MIDI queue elements which are due just before current timestamp, including the above..
	play_MIDI_queue( G_MIDI_timestamp );
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

	// This is not available on NEMO
#ifdef NEMO
	return;
#endif

	// Only act when REC bit is on..
	if ( GRID_assistant_page->REC_bit == OFF ) return;


	// Determine channel
	channel = ( G_running_status_byte_IN_UART[ UART_ndx ] & 0x0F ) + 1;

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


