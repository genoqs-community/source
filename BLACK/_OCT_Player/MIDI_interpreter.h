
// MIDI defintions


#define 	MIDI_STATUS_BYTE 					0x80
#define 	MIDI_DATA_BYTE 						0x00

#define 	MIDI_CMD_NOTE_OFF   				0x80
#define 	MIDI_CMD_NOTE_ON   					0x90
#define 	MIDI_CMD_NOTE_PRESSURE   			0xa0
#define 	MIDI_CMD_CONTROL   					0xb0
#define 	MIDI_CMD_PGM_CHANGE   				0xc0
#define 	MIDI_CMD_CHANNEL_PRESSURE   		0xd0
#define 	MIDI_CMD_BENDER   					0xe0

#define		MIDI_CMD_SYSTEM						0xF0

#define 	MIDI_CMD_SYSTEM_COMMON_SYSEX   		0xF0
#define 	MIDI_CMD_SYSTEM_COMMON_MTC_QUARTER  0xF1
#define 	MIDI_CMD_SYSTEM_COMMON_SONG_POS   	0xF2
#define 	MIDI_CMD_SYSTEM_COMMON_SONG_SELECT  0xF3
#define 	MIDI_CMD_SYSTEM_COMMON_TUNE_REQUEST 0xF6
#define 	MIDI_CMD_SYSTEM_COMMON_SYSEX_END   	0xF7

#define 	MIDI_CMD_SYSTEM_REALTIME_CLOCK   	0xf8
#define 	MIDI_CMD_SYSTEM_REALTIME_START   	0xfa
#define 	MIDI_CMD_SYSTEM_REALTIME_CONTINUE   0xfb
#define 	MIDI_CMD_SYSTEM_REALTIME_STOP   	0xfc
#define 	MIDI_CMD_SYSTEM_REALTIME_SENSING   	0xfe
#define 	MIDI_CMD_SYSTEM_REALTIME_RESET   	0xff


//
//// Combines two data bytes into one 14 bit value returned in a short
//unsigned short combineDataBytes(unsigned char First, unsigned char Second){
//
//   unsigned short _14bit;
//
//   _14bit = (unsigned short)Second;
//   _14bit <<= 7;
//   _14bit |= (unsigned short)First;
//
//   return(_14bit);
//}


// Interprets the midi_byte as a BENDER / PITCH WHEEL message
/* Status: 	0xE0 to 0xEF where the low nibble is the MIDI channel.
 * Data:	Two data bytes follow the Status. They are combined to a 14-bit value
 */
//void midi_interpret_BENDER( unsigned char midi_byte, unsigned char UART_ndx ){
//
//	// If data byte was received without prior status byte, fill in the running status
//	if ( midi_controller_ndx == 0 ){
//
//		// Take the runnning status byte into account
//		midi_controller_IN_UART[UART_ndx][midi_controller_ndx] = 
//			running_status_byte_IN_UART[ UART_ndx ];
//
//		// Update running index of the data bytes
//		midi_controller_ndx = 1;
//	}
//
//	// Interpret the byte as a DATA byte.
//	midi_controller_IN_UART[UART_ndx][midi_controller_ndx] = midi_byte;
//		
//	// Point the index to the next data byte
//	midi_controller_ndx++;
//	
//	// Check for the completeness of the note.
//	// If the second data byte is not invalid (0xff) then we have a note.
//	if ( midi_controller_IN_UART[UART_ndx][2] != 0xff ) {
//		
//		// Execute the midi note -> Record only when the sequencer is running
//		if ( RUNBIT == ON ){
//			
//			midi_controller_execute( 	midi_controller_IN_UART[UART_ndx][0], 
//										midi_controller_IN_UART[UART_ndx][1], 
//										midi_controller_IN_UART[UART_ndx][2] 	);	
//		
//			// Clear the note and the data byte index
//			midi_controller_IN_UART[UART_ndx][1] = 0xff;
//			midi_controller_IN_UART[UART_ndx][2] = 0xff;
//		
//			midi_controller_ndx = 0;
//		} // sequencer is running
//	} 
//
//}
//


// Pitches the step accodring to the in_pitch and considering its track pitch
unsigned char apply_MIDIpitch_to_step( 	unsigned char in_pitch, 
										Pagestruct* target_page, 
										unsigned char row, 
										unsigned char col ){

	signed char temp = 0;
	unsigned result = FALSE;
	
	// Compute the step pitch offset
	temp = in_pitch - target_page->Track[row]->attribute[ATTR_PITCH];
	
	// Apply the computed pitch offset if it is within legal STEP offset range.
	if (	( temp <= STEP_MAX_PITCH )
		&&	( temp >= STEP_MIN_PITCH )
		){
	
		// Actually should be adding the pitch to the chord of the step if step is ON
		target_page->Step[row][col]->attribute[ATTR_PITCH] = temp;
		result = TRUE;
	}

	return result;
}



// Take the components of the CONTROLLER message and do with it whatever fits
void midi_controller_execute( 	unsigned char status_byte, 
								unsigned char data_byte_1,
								unsigned char data_byte_2	){

	unsigned char 	controller_number		= 	data_byte_1;
	unsigned char 	controller_value		= 	data_byte_2;
	unsigned int 	current_TTC				= 	TTC_ABS_VALUE;

	unsigned char 	row=0,
					target_col=0;

	//	// PRINT THE CONTENT!! (and disregard the rest..)
	//	d_iag_printf( "note at TTC:%d ch:0x%x pi:0x%x ve:0x%x\n", 
	//		current_TTC, status_byte, data_byte_1, data_byte_2 );
	//	return;
		
	// REC STATUS CHECKPOINT
	// Do not continue if ..
	if (		
			// ..we are not in the right mode for MIDI IN recording,
				(	( zoom_level != zoomPAGE 	) 
				&& 	( zoom_level != zoomTRACK  	)
				&& 	( zoom_level != zoomMAP 	))
				
			// ..the page is not in a valid keymode or implicitly zoom mode,
			|| 	(	( VIEWER_page->keyMode != KMOD_PAGE 	  	) 
				&&	( VIEWER_page->keyMode != KMOD_THIS_TRACK 	)
				&& 	( VIEWER_page->keyMode != KMOD_ATTR_MAP 	))

			// ..there is no track enabled for REC,
			|| 	(	VIEWER_page->track_REC_pattern == 0 	) 
			
			// ..the page is not playing in GRID,
			||	( is_selected_in_GRID( VIEWER_page ) == FALSE )
		){
		
		return;
	}
	
	
	// Get the index of the REC row. Only one track selected at any time. No quantization.
	row = my_bit2ndx( VIEWER_page->track_REC_pattern );

	// Compute the coordinates of the step to be activated	
	// Adjust the step start value according to current TTC. Reasoning: see book p.189
	if ( current_TTC <= 6 ){		

		// Place step in current column
		target_col 		= VIEWER_page->Track[row]->attribute[ATTR_LOCATOR] -1;
	}
	else {
		// Place step in next column, which may have to wrap- -1 is locator vs col notation offset.
		target_col 		= 
			get_next_tracklocator( 	VIEWER_page->Track[row], 
									VIEWER_page->Track[row]->attribute[ATTR_LOCATOR] ) -1;
	}
 
	// Update the controller number in the track MCC and controller value in step MCC
	VIEWER_page->Track[row]->attribute[ATTR_MIDICC] = controller_number;

	VIEWER_page->Step[row][target_col]->attribute[ATTR_MIDICC] = controller_value;
}



// Interpret a program change message:
// Channel 10 selects the scene to be played: in a %16 fashion
// Channels 1-9 toggle the pages to play - in a %16 fashion
void midi_interpret_PGMCH( unsigned char midi_byte, unsigned char UART_ndx ){
 
	unsigned char target_ndx = 0;
	Pagestruct* target_page = NULL;
	unsigned char page_ndx = 0;
	unsigned char channel = 0;
	
	// Only act when REC bit is on..
	if ( VIEWER_page->REC_bit == OFF ) return;

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
			target_page = &Page_repository[ page_ndx ];

			// Toggle the page playing status 
			switch ( is_selected_in_GRID( target_page ) ) {
				case ON:
					grid_select( target_page, OFF );
					break;

				case OFF:
					grid_select( target_page, ON  );
					break;
			}

			break;
	}
}




// Interprets the midi_byte as a CONTROL message
/* Status: 	0xB0 to 0xBF where the low nibble is the MIDI channel.
 * Data:	Two data bytes follow the Status.
 * 			First:  controller number (0 to 127). Indicates which controller is affected.
 * 			Second: value to which the controller should be set (0 to 127).
 */
void midi_interpret_CONTROL( unsigned char midi_byte, unsigned char UART_ndx ){

	// If data byte was received without prior status byte, fill in the running status
	if ( midi_controller_ndx == 0 ){

		// Take the runnning status byte into account
		midi_controller_IN_UART[UART_ndx][midi_controller_ndx] = 
			running_status_byte_IN_UART[ UART_ndx ];

		// Update running index of the data bytes
		midi_controller_ndx = 1;
	}

	// Interpret the byte as a DATA byte.
	midi_controller_IN_UART[UART_ndx][midi_controller_ndx] = midi_byte;
		
	// Point the index to the next data byte
	midi_controller_ndx++;
	
	// Check for the completeness of the note.
	// If the second data byte is not invalid (0xff) then we have a note.
	if ( midi_controller_IN_UART[UART_ndx][2] != 0xff ) {
		
		// Execute the midi note -> Record only when the sequencer is running
		if ( RUNBIT == ON ){
			
			midi_controller_execute( 	midi_controller_IN_UART[UART_ndx][0], 
										midi_controller_IN_UART[UART_ndx][1], 
										midi_controller_IN_UART[UART_ndx][2] 	);	
		
			// Clear the note and the data byte index
			midi_controller_IN_UART[UART_ndx][1] = 0xff;
			midi_controller_IN_UART[UART_ndx][2] = 0xff;
		
			midi_controller_ndx = 0;
		} // sequencer is running
	} 

}




// Interprets the midi_byte as a REALTIME messages
void midi_interpret_REALTIME( unsigned char midi_byte ){

	// MTC reference:
	// 0xf8   timing clock
	// 0xf9   undefined			->not implemented
	// 0xfa   start
	// 0xfb   continue
	// 0xfc   stop
	// 0xfd   undefined			->not implemented
	// 0xfe   active sensing	->not implemented
	// 0xff   system reset		->not implemented

	// unsigned int i = 0;

	// EXT clock is a must to continue..
	if ( CLOCK_SOURCE != EXT ) return;

	// REAL TIME MESSAGES
	switch ( midi_byte ) {

		// CLOCK
		case MIDICLOCK_CLOCK:
			
			// Drive the sequencer to play..
			// TWICE!! ..as required by the MIDI Clock resolution
			cyg_semaphore_post( &sem_driveSequencer );
			cyg_semaphore_post( &sem_driveSequencer );
			break;
			

		// START
		case MIDICLOCK_START:

			// Only react if running on external clock
			// Need to reset because ...
		 	sequencer_RESET();

			midi_clock_in_counter++;
		
			// Jumpstart the sequencer
			sequencer_START();
			break;
			

		// CONTINUE
		case MIDICLOCK_CONTINUE:

			RUNBIT 		= ON;
			PAUSEBIT 	= OFF;
			break;
			

		// STOP
		case MIDICLOCK_STOP:

			// First STOP causes a pause, second one stops slave for good.
			switch ( RUNBIT ){
				case ON:
					// This is not a full STOP command, can be pause as well
					sequencer_HALT();
					break;

				case OFF:
					// This is a full stop for the slave machine.
					sequencer_STOP();
					break;
			}
			midi_clock_in_counter = 0;

			// Just show the page to refresh the stop update
			Page_requestRefresh();

			break;
	}
}



// Interprets the midi_byte as part of a NOTE_ON (or NOTE_OFF) message
void midi_interpret_NOTE_ON( unsigned char midi_byte, unsigned char UART_ndx ){

	// Store the byte in the circular array	- just so it can be printed out.
	// DO NOT USE PRINTFS OR THE LIKE HERE, OR THE INPUT IS DELAYED AND SCRAMBLED

	// If data byte was received without prior status byte, 
	// .. or the status byte was not filled in yet, fill with in the running status
	if (	( midi_data_ndx == 0 ) 
		|| 	( midi_note_IN_UART[UART_ndx][0] == 0xff )
		){

		// Take the runnning status byte into account - can be one of NOTE_ON or NOTE_OFF
		midi_note_IN_UART[UART_ndx][0] = running_status_byte_IN_UART[ UART_ndx ];
		// Print buffer
		midi_in[0] = running_status_byte_IN_UART[ UART_ndx ];

		// Update running index of the data bytes
		midi_data_ndx = 1;		

// d_iag_printf( "got rst:0x%x\n", midi_note_IN_UART[UART_ndx][0] );
	}
						
	// Interpret the byte as a DATA byte.
	midi_note_IN_UART[UART_ndx][midi_data_ndx] = midi_byte;
	midi_in[midi_data_ndx] = midi_byte;
		
	// Point the index to the next data byte
	midi_data_ndx++;
	
	// Check for the completeness of the note.
	// If the second data byte is not invalid (0xff) then we have a note.
//	if ( midi_note_IN_UART[UART_ndx][2] != 0xff ) {
	if ( midi_data_ndx > 2 ) {
		
//		d_iag_printf( "assembled: st:0x%x pi:0x%x ve:0x%x\n", 
//									midi_note_IN_UART[UART_ndx][0], 
//									midi_note_IN_UART[UART_ndx][1], 
//									midi_note_IN_UART[UART_ndx][2]	);

		// Execute the midi note -> Record only when the sequencer is running
		switch( RUNBIT ){
			
			case OFF:
				// Take pitch over from played note into recording track
				// do step recording, or play through..
			case ON:			
				// Record midi note - pass the UART ndx along, for the pass through ops
				midi_note_execute( 	UART_ndx,
									midi_note_IN_UART[UART_ndx][0], 
									midi_note_IN_UART[UART_ndx][1], 
									midi_note_IN_UART[UART_ndx][2] 	);	
				break;				
		} // sequencer is running - or not
			
		// Clear the note and the data byte index
		midi_note_IN_UART[UART_ndx][0] = 0xff;
		midi_note_IN_UART[UART_ndx][1] = 0xff;
		midi_note_IN_UART[UART_ndx][2] = 0xff;
		
		midi_data_ndx = 0;
	}

}




// Takes a pitch value and a track and retunrs the offset between the two
signed char adjust_PIT_to_track( unsigned char in_PIT, Trackstruct* target_track ){
	
	signed char result = 0;

	result = in_PIT - target_track->attribute[ATTR_PITCH];
	result = normalize( result, STEP_MIN_PITCH, STEP_MAX_PITCH );
//d_iag_printf( "in_PIT:%d track_PIT:%d result:%d\n", 
//	in_PIT, target_track->attribute[ATTR_PITCH], result );
	
	return result;
}

// Checks whether the given pitch can be part of the given track
unsigned char is_pitch_in_track( unsigned char in_PIT, Trackstruct* target_track ){

	unsigned char result = FALSE;
	signed short delta = in_PIT - target_track->attribute[ATTR_PITCH];
		
	if (	( delta <= STEP_MAX_PITCH )
		&&	( delta >= STEP_MIN_PITCH ) 
		){
			
		result = TRUE;
	}
	
	return result;
}


// Takes a pitch value and a track and retunrs the offset between the two
signed char adjust_VEL_to_track( unsigned char in_VEL, Trackstruct* target_track ){
	
	signed char result = 0;

	result = in_VEL - target_track->attribute[ATTR_VELOCITY];
	result = normalize( result, STEP_MIN_VELOCITY, STEP_MAX_VELOCITY );
	
	return result;
}




// Add the given pitch to the chord structure of the given step
// stackmode INC means to add to whats there, DEC means start fresh
void make_chord( unsigned char stackmode, Stepstruct* target_step, signed char in_pitch ){

	signed char	temp = 0;
	unsigned char bit_offset = 0;
	
	// Stackmode DEC: Make the lowest incoming pitch the step pitch. 
	// This is when starting a chord from scratch..
	if ( stackmode == DEC ){
		if ( in_pitch < target_step->attribute[ATTR_PITCH] ){
			temp = target_step->attribute[ATTR_PITCH];
			target_step->attribute[ATTR_PITCH] = in_pitch;
			in_pitch = temp;
		}
	}
	
	// Normalize the higher pitch into the step's upper octave
	if (	( in_pitch > (target_step->attribute[ATTR_PITCH] + 12) )
		||	( in_pitch < (target_step->attribute[ATTR_PITCH] - 12) )
		){
		in_pitch = ((target_step->attribute[ATTR_PITCH] / 12) * 12) + (in_pitch % 12);
	}

	// Calculate the bit offset into the chord array to toggle the chord bit
	bit_offset = in_pitch - target_step->attribute[ATTR_PITCH];

	// Don't react on the same note (roll) just yet..
	if ( bit_offset > 0 ){

		// Make sure to use every bit in the pattern
		bit_offset --;
		
		// Set the bit in question
		target_step->chord_data |= ( 1 << bit_offset );
		
		// Restrict cardinality of the chord to 7, i.e. 6 aux notes allowed
		if ( my_bit_cardinality( target_step->chord_data & 0x7FF ) > 6 ){
			// Reverse the operation..
			target_step->chord_data ^= ( 1 << bit_offset );
		}
	}
}



// Check whether the step is already queued -> meaning a note on has taken place
unsigned char is_step_queued( Stepstruct* in_step ){

	unsigned char i = 0;
	NOTEeventstruct* cursor = NOTE_queue;

	for ( i=0; i < NOTE_NROF_EVENTS; i++ ){
		
		if ( cursor == NULL ) return FALSE;
		if ( cursor->target_step == in_step ) return TRUE;
		cursor = cursor->next;
	}
	
	return FALSE;
}



// Inserts a note event into the sequencing engine, so it is played.
// Basically reads out the event values and fills them into the target step.
void midi_insert_event( NOTEeventstruct* in_event, unsigned int in_timestamp ){

	// VELOCITY
	in_event->target_step->attribute[ATTR_VELOCITY] 	= 
		adjust_VEL_to_track( in_event->velocity, in_event->target_track );

	// LENGTH
	in_event->target_step->attribute[ATTR_LENGTH] 	= 
		normalize( 	(	(in_timestamp - in_event->timestamp) / 12)*16 
					+	(((in_timestamp - in_event->timestamp) % 12)*4)/3,
					(unsigned char) STEP_MIN_LENGTH, 
					(unsigned char) STEP_MAX_LENGTH );

	// START
	in_event->target_step->attribute[ATTR_START] 		= in_event->start;
	
	// STATUS adjustment: turn on Step under the locator (adjusted by 1)
	Step_set_status( in_event->target_step, STEPSTAT_TOGGLE, ON );

	// Recycle the event - by placing it back onto the stack
	NOTE_stack_push( in_event );
}



// Take the components of the NOTE ON and do with it whatever fits
void midi_note_execute( 	unsigned char UART_ndx,
							unsigned char status_byte, 
							unsigned char data_byte_1,
							unsigned char data_byte_2	){

	unsigned char 	in_channel		=	status_byte & 0x0F;
	unsigned char 	in_pitch		= 	data_byte_1;
	unsigned char 	in_velocity		= 	data_byte_2;
	unsigned int 	current_TTC		= 	TTC_ABS_VALUE;
	unsigned char 	i				= 	0;

	unsigned char 	row=0, 
					col=0,
					target_start=0, 
					target_col=0,
					temp=0;

	NOTEeventstruct* on_event = NULL;
	NOTEeventstruct* off_event = NULL;

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
				&& 	( zoom_level != zoomMAP 	))
				
			// ..the page is not in a valid keymode or implicitly zoom mode,
			|| 	(	( VIEWER_page->keyMode != KMOD_PAGE 	  	) 
				&&	( VIEWER_page->keyMode != KMOD_THIS_TRACK 	)
				&& 	( VIEWER_page->keyMode != KMOD_ATTR_MAP 	))

			// ..there is no track enabled for REC,
			|| 	(	( VIEWER_page->track_REC_pattern == 0 	) 
				&& 	( VIEWER_page->REC_bit == OFF 			))
			
			// ..the page is not playing in GRID,
			||	( is_selected_in_GRID( VIEWER_page ) == FALSE )
		){
		
		return;
	}
	
	// Get the index of the REC row. Only one track selected at any time. No quantization.
	row = my_bit2ndx( VIEWER_page->track_REC_pattern );

	// Also, do not continue if the in channel does not fit the Track MIDI channel - regarless of port
	if (	( VIEWER_page->REC_bit == OFF )
		&&	( in_channel != ((VIEWER_page->Track[row]->attribute[ATTR_MIDICH] -1) %16) )
		){
		return;
	}


	// Handle NOTE OFF messages -> pretend they don't exist, simply transform them.
	if ( ( status_byte & 0xF0 ) == MIDI_CMD_NOTE_OFF ){
		// transform into NOTE ON with VEL 0
		in_velocity = OFF;
	}


	// Check if we are traditionally recording or step recording
	switch ( VIEWER_page->REC_bit ){
	
		// Assigning pitch to selected track or step recording, or forcing input to scale
		case ON:
		
			// Track pitch assignment -> Marcel style
			if ( VIEWER_page->trackSelection != 0 ){

				for ( i = 0; i < MATRIX_NROF_ROWS; i++ ){

					// Apply to every selected track..
					if ( (VIEWER_page->trackSelection & ( 1 << i )) != 0 ){

						// Assign pitch of the incoming note to the recording track
						VIEWER_page->Track[i]->attribute[ATTR_PITCH] = in_pitch;
					}
				}
			}

			// Step note entry -> Duncan style based on Roland Fleming Mode
			// At least one step button is pressed and we are not dealing with MIDI OFF
			else if (	( in_velocity != OFF )
					&&	( key_pressed != 0 )
					){

				// Linear scan through the buttons in question: Selectors and Steps.
				for( i=0; i <= 185; i++ ){
				
					// Not using the provided infrastructure because of the anomalies. may change
					// There is a key pressed - in the Step Range (exclude the rest)
					if (	( pressed_keys[i] >=  11 )
						&&	( pressed_keys[i] <= 185 ) 
						&& 	( ((pressed_keys[i]-10) % 11) != 0 ) 
						){

						// Compute the key coordinates
						row = row_of( 		pressed_keys[i] );
						col = column_of(	pressed_keys[i] );
						
						// d_iag_printf( "%d row:%d col:%d  ", pressed_keys[i], row, col );

						// The MIDI note activates the step if it is not active already
						if ( Step_get_status( VIEWER_page->Step[row][col], STEPSTAT_TOGGLE ) == OFF ){

							// If we are not in PREVIEW mode
							if (	( VIEWER_page->editorMode != PREVIEW )
								){
												
								// If the step is OFF, take the pitch over and turn ON
								switch( Step_get_status( VIEWER_page->Step[row][col], STEPSTAT_TOGGLE ) ){
								
									case TRUE:

										// Build chord of the selected step
										// Add the incoming pitch to the chord structure of the step
										make_chord( INC, VIEWER_page->Step[row][col], 
											adjust_PIT_to_track( in_pitch, VIEWER_page->Track[row] ) );
										break;
									
									case OFF:
										// Apply the MIDI pitch to the pressed steps
										temp = apply_MIDIpitch_to_step( in_pitch, VIEWER_page, row, col );
				
										// If application was successful, turn step ON
										if ( temp == TRUE ){
											Step_set_status( VIEWER_page->Step[row][col], STEPSTAT_TOGGLE, ON );
										}
										break;
								}
							}
						}
					} // key pressed in step range
				} // button scan iterator
			} // key_pressed != 0 - Step for Roland Fleming mode
			

			// Send incoming MIDI note further out, after forcing it to scale
			else if ( key_pressed == 0 ){

				// FORCE TO SCALE intervention - LOCAL has priority over GLOBAL scale
				if (	( VIEWER_page->force_to_scale == ON )
					){
					// If the note index of the initial pitch is not in the scale
					if ( how_selected_in_current_scale( VIEWER_page, pitch_to_noteNdx( in_pitch ) ) == OFF 
						){		
						// Add to initial pitch the offset to the next scale degree
						in_pitch += offset_to_next_current_degree( VIEWER_page, in_pitch ); 
					}
				}	
				// GLOBAL SCALE FTS, if page is not active
				else if (	( GRID_assistant_page->force_to_scale == ON )
					){
		
					// If the note index of the initial pitch is not in the scale
					if ( how_selected_in_current_scale( GRID_assistant_page, pitch_to_noteNdx( in_pitch ) ) == OFF 
						){
						// Add to initial pitch the offset to the next scale degree
						in_pitch += offset_to_next_current_degree( GRID_assistant_page, in_pitch ); 
					}
				} // FTS intervention

				// Channel (+1 offset needed), pitch, velocity, trigger time - 0 means NOW
				MIDI_NOTE_new ( (UART_ndx*16) + in_channel+1, in_pitch, in_velocity, 0 );
			}
			break;
	
	

		// Recording notes classically - sequencer is running
		case OFF:
		
			// Do not continue if incoming pitch is not valid for the track
			if ( is_pitch_in_track( in_pitch, VIEWER_page->Track[row] ) == FALSE ){

				break;
			}

			// Determine whether we are dealing with a note ON or OFF
			switch( in_velocity ){
		
				// NOTE OFF scenario
				case OFF:
		
					// Remove from queue an appropriately pitched event 
					off_event = NOTE_queue_remove( in_pitch );

					if ( off_event != NULL ){
						midi_insert_event( off_event, MIDI_timestamp );
					}					
					break;
		
		
				// NOTE ON scenario
				default:
					
					// Compute the coordinates of the step to be activated	
					// Adjust the step start value according to current TTC
					// Reasoning: see book p.189
					if ( current_TTC <= 6 ){
						
						// Place step in current column
						target_col 		= VIEWER_page->Track[row]->attribute[ATTR_LOCATOR] -1;
						target_start 	= current_TTC-1 + 6;
					}
					else {
						
						// Place step in next column, which may have to wrap
						target_col 		= 
							get_next_tracklocator( 	VIEWER_page->Track[row], 
													VIEWER_page->Track[row]->attribute[ATTR_LOCATOR] ) -1;
						target_start 	= current_TTC-1 -6;
					}
					// Enqueue step to be set: NOTEevent, Stepstruct*, velocity, pitch, start, length;
					// To be set and activated as soon as its note OFF is received.. in the OFF section							
					// Get new in_event from stack
					on_event = NOTE_stack_pop();
					if ( on_event == NULL ) break;
		
					// PITCH - Fill the simple step pitch or build chord
					switch( is_step_queued( VIEWER_page->Step[row][target_col] ) ){
						
						case TRUE:
							// Add the incoming pitch to the chord structure of the step
							make_chord( DEC, VIEWER_page->Step[row][target_col], 
									adjust_PIT_to_track( in_pitch, VIEWER_page->Track[row] ) );
							break;
				
						case FALSE:
		
							// If the step is on, add pitch to it and make a chord
							if ( Step_get_status( VIEWER_page->Step[row][target_col], STEPSTAT_TOGGLE ) == ON ){
								// Add the incoming pitch to the chord structure of the step
								make_chord( INC, VIEWER_page->Step[row][target_col], 
										adjust_PIT_to_track( in_pitch, VIEWER_page->Track[row] ) );
							}
							else{
								// No chord set for the step, but 9 is the neutral strum value
								VIEWER_page->Step[row][target_col]->chord_data	= 9 << 11;
								
								// Fill the step pitch value simply.. after erasing traces of chords
								VIEWER_page->Step[row][target_col]->attribute[ATTR_PITCH] = 
									adjust_PIT_to_track( in_pitch, VIEWER_page->Track[row] );
							}
							break;
					}
		
					// Fill it with current data
					NOTEevent_fill( 	on_event, 	VIEWER_page->Step[row][target_col],
													VIEWER_page->Track[row],
													MIDI_timestamp,
													in_velocity,
													in_pitch,
													OFF,
													target_start
								  );
					
					// Insert it into the queue
					NOTE_queue_insert( on_event );
					// NOTE_queue_print();
		
					break;
			} // switch( in_velocity )		
			break;
					
	} // switch on RUNBIT

}
