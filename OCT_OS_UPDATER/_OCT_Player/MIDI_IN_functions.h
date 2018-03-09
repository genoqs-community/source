

extern void program_scale_pitches( 	Pagestruct* target_page );
extern void select_in_scale( 		Pagestruct* target_page,
									unsigned char target_note, 
									unsigned char target_state );



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



// Maps a 14 bit value to a 7 bit interval
unsigned char map_to_8bit( unsigned short _14bit ){

	return ( (_14bit + 1) / 128 );
}

unsigned char map_to_14bit( unsigned char _8bit, unsigned char order ){

	unsigned char result = 0;
	unsigned short _14bit = _8bit * 128;
	
	switch( order ){
	
		case 0:
			// Return the first 7 bits
			result = 0x7f & _14bit;
			break;
		
		case 1:
			// Return the second 7 bits
			result = _14bit >> 7;
			break;
	}

	return result;	
}



//_______________________________________________________________________________________ 
//
// Combines two data bytes into one 14 bit value returned in a short
unsigned short combineDataBytes(unsigned char First, unsigned char Second){

   unsigned short _14bit;

   _14bit = (unsigned short)Second;
   _14bit <<= 7;
   _14bit |= (unsigned short)First;

   return(_14bit);
}



//_______________________________________________________________________________________ 
//
// Pitches the step according to the in_pitch and considering its track pitch
unsigned char apply_VEL_to_step( 	unsigned char in_velocity, 
									Pagestruct* target_page, 
									unsigned char row, 
									unsigned char col ){

	signed char temp = 0;
	unsigned result = FALSE;
	
	// Compute the step pitch offset
	temp = in_velocity - target_page->Track[row]->attribute[ATTR_VELOCITY];
	
	// Apply the computed pitch offset if it is within legal STEP offset range.
	if (	( temp <= STEP_MAX_VELOCITY )
		&&	( temp >= STEP_MIN_VELOCITY )
		){
	
		target_page->Step[row][col]->attribute[ATTR_VELOCITY] = temp;
		result = TRUE;
	}

	return result;
}



//_______________________________________________________________________________________ 
//
// Pitches the step according to the in_pitch and considering its track pitch
unsigned char apply_PIT_to_step( 	unsigned char in_pitch, 
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



//_______________________________________________________________________________________ 
//
// Takes a pitch value and a track and returns the offset between the two
signed char adjust_PIT_to_track( unsigned char in_PIT, Trackstruct* target_track ){
	
	signed char result = 0;

	result = in_PIT - target_track->attribute[ATTR_PITCH];
	result = normalize( result, STEP_MIN_PITCH, STEP_MAX_PITCH );

// d_iag_printf( "in_PIT:%d track_PIT:%d result:%d\n", 
//	in_PIT, target_track->attribute[ATTR_PITCH], result );
	
	return result;
}



//_______________________________________________________________________________________ 
//
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



//_______________________________________________________________________________________ 
//
// Takes a pitch value and a track and retunrs the offset between the two
signed char adjust_VEL_to_track( unsigned char in_VEL, Trackstruct* target_track ){
	
	signed char result = 0;

	result = in_VEL - target_track->attribute[ATTR_VELOCITY];
	result = normalize( result, STEP_MIN_VELOCITY, STEP_MAX_VELOCITY );
	
	return result;
}




//_______________________________________________________________________________________ 
//
// Add the given pitch to the chord structure of the given step
// stackmode INC means to add to whats there, DEC means start fresh
void make_chord( unsigned char stackmode, Stepstruct* target_step, signed char in_pitch ){

	signed char	temp = 0;
	unsigned char bit_offset = 0;
	
	// d_iag_printf( "     making chord with pitch:%d on pit:%d\n", 
	//		in_pitch, target_step->attribute[ATTR_PITCH] );

	// Stackmode DEC: Make the incoming pitch the step pitch. 
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
		||	( in_pitch < (target_step->attribute[ATTR_PITCH] ) )
		){
		in_pitch = ((target_step->attribute[ATTR_PITCH] / 12) * 12) + (in_pitch % 12);
	}

	// d_iag_printf( "     normalized pitch:%d tgt pit:%d\n", 
	//	in_pitch, target_step->attribute[ATTR_PITCH] );

	// Calculate the bit offset into the chord array to toggle the chord bit
	bit_offset = in_pitch - target_step->attribute[ATTR_PITCH];

	// Don't react on the same note (roll) just yet..
	if ( bit_offset > 0 ){

		// Make sure to use every bit in the pattern
		bit_offset --;
		
		// d_iag_printf( "     chord_data initial:%d bo:%d\n", target_step->chord_data, bit_offset );
		//	my_print_bits( target_step->chord_data );

		// Set the bit in question
		target_step->chord_data |= ( 1 << bit_offset );
		
		// Restrict cardinality of the chord to 7, i.e. 6 aux notes allowed
		if ( my_bit_cardinality( target_step->chord_data & 0x7FF ) > 6 ){
			// Reverse the operation..
			target_step->chord_data ^= ( 1 << bit_offset );
		}

		// d_iag_printf( "     chord_data final:%d, bo:%d\n", target_step->chord_data, bit_offset );
		//	my_print_bits( target_step->chord_data );

	}
}



//_______________________________________________________________________________________ 
//
// Check whether the step is already queued -> meaning a note-on has taken place
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



//_______________________________________________________________________________________ 
//
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



//_______________________________________________________________________________________ 
//
// Learm into the current CCMAP
void CCMAP_learn( 	unsigned char status_byte,
					unsigned char controller_number,
					unsigned char controller_value ){

	unsigned char controller_channel = status_byte & 0x0F;
	Pagestruct* target_page = &Page_repository[ GRID_CURSOR ];

	if ( target_page->REC_bit == OFF ) return;

	// d_iag_printf( "CCMAP learning:ch:%d CC:%d val:%d\n", 
	//		controller_channel, controller_number, controller_value );

	// Assign the controller number to the current CCMAP_learner
	switch( target_page->mixTarget ){
	
		case MIXTGT_USR1:
		case MIXTGT_USR2:
		case MIXTGT_USR3:
		case MIXTGT_USR4:
		case MIXTGT_USR5:
		
			target_page->CC_MIXMAP	[ target_page->mixTarget ]
									[ CCMAP_learner ]
									[ CC_MIXMAP_AMT ] = controller_value;
		
			target_page->CC_MIXMAP	[ target_page->mixTarget ]
									[ CCMAP_learner ]
									[ CC_MIXMAP_MCC ] = controller_number;
		
			target_page->CC_MIXMAP	[ target_page->mixTarget ]
									[ CCMAP_learner ]
									[ CC_MIXMAP_MCH ] = controller_channel;		

			break;
	}
}



//_______________________________________________________________________________________ 
//
void force_input_to_scale( 	Pagestruct* target_page, 
							unsigned char in_channel,
							unsigned char in_pitch, 
							unsigned char in_velocity ){

	// FORCE TO SCALE intervention - LOCAL has priority over GLOBAL scale
	if (	( target_page->force_to_scale == ON )
		){
		// If the note index of the initial pitch is not in the scale
		if ( how_selected_in_current_scale( target_page, pitch_to_noteNdx( in_pitch ) ) == OFF 
			){		
			// Add to initial pitch the offset to the next scale degree
			in_pitch += offset_to_next_current_degree( target_page, in_pitch ); 
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
	MIDI_NOTE_new ( in_channel, in_pitch, in_velocity, 0 );

}



//_______________________________________________________________________________________ 
//
void record_note_to_track( 	Pagestruct* target_page, 
							unsigned char row,
							unsigned char target_col,
							unsigned char target_start,
							unsigned char in_pitch, 
							unsigned char in_velocity ){


	NOTEeventstruct* on_event = NULL;
	NOTEeventstruct* off_event = NULL;


	// Do not continue if incoming pitch is not valid for the track
	if ( is_pitch_in_track( in_pitch, target_page->Track[row] ) == FALSE ) return;

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
			
			// Enqueue step to be set: NOTEevent, Stepstruct*, velocity, pitch, start, length;
			// To be set and activated as soon as its note OFF is received.. in the OFF section							
			// Get new event from stack
			on_event = NOTE_stack_pop();
			
			if ( on_event == NULL ) break;

			// PITCH - Fill the simple step pitch or build chord
			switch( is_step_queued( target_page->Step[row][target_col] ) ){
				
				case TRUE:
					// Add the incoming pitch to the chord structure of the step
					make_chord( DEC, target_page->Step[row][target_col], 
							adjust_PIT_to_track( in_pitch, target_page->Track[row] ) );
					break;
		
				case FALSE:

					// If the step is on, add pitch to it and make a chord
					if ( Step_get_status( target_page->Step[row][target_col], STEPSTAT_TOGGLE ) == ON ){
					
						// Add the incoming pitch to the chord structure of the step
						make_chord( INC, target_page->Step[row][target_col], 
								adjust_PIT_to_track( in_pitch, target_page->Track[row] ) );
					}
					else{
						// No chord set for the step, and 9 is the neutral strum value
						target_page->Step[row][target_col]->chord_data	= 9 << 11;
						
						// Fill the step pitch value simply.. after erasing traces of chords
						target_page->Step[row][target_col]->attribute[ATTR_PITCH] = 
							adjust_PIT_to_track( in_pitch, target_page->Track[row] );

						// Fill the step with the right velocity offset
						target_page->Step[row][target_col]->attribute[ATTR_VELOCITY] =
							adjust_VEL_to_track( in_velocity, target_page->Track[row] );
							
						// Turn the Step on
						Step_set_status( target_page->Step[row][target_col], STEPSTAT_TOGGLE, ON );
					}
					break;
			}

			// Fill it with current data
			NOTEevent_fill( 	on_event, 	target_page->Step[row][target_col],
											target_page->Track[row],
											MIDI_timestamp,
											in_velocity,
											in_pitch,
											OFF,
											target_start
						  );
			
			// Insert event into the queue
			NOTE_queue_insert( on_event );

			break;
	} // switch( in_velocity )
}



//_______________________________________________________________________________________ 
//
void assign_note_to_selection( 	Pagestruct* target_page, 
								unsigned char in_pitch, 
								unsigned char in_velocity ){
	
	unsigned char row = 0;
	unsigned char col = 0;
	unsigned char i = 0;

	// Track pitch assignment -> was Marcel style, but now it's transposition style..
	if ( target_page->trackSelection != 0 ){

		for ( i = 0; i < MATRIX_NROF_ROWS; i++ ){

			// Apply to every selected track..
			if ( (target_page->trackSelection & ( 1 << i )) != 0 ){

				// Assign pitch of the incoming note to the recording track
				target_page->Track[i]->attribute[ATTR_PITCH] = in_pitch;

				// Transpose the track pitch by the delta between in_pitch and MIDDLE_C
				// target_page->Track[i]->attribute[ATTR_PITCH] =
				//	normalize ( 
				//		target_page->Track[i]->attribute[ATTR_PITCH] + (in_pitch - MIDDLE_C), 
				//		TRACK_MIN_PITCH, TRACK_MAX_PITCH );
			}
		}
	}

	// When in Step mode - make the step assignment to that step
	else if ( zoom_level == zoomSTEP ){
	
		// Establish the coordinates
		row = target_page->stepSelectionSingleRow;
		col = target_page->stepSelectionSingleCol;
	
		// Record incoming notes to the given position
		record_note_to_track( 	target_page, 	row, 		col, 
								STEP_DEF_START,	in_pitch, 	in_velocity );			
	}

	// Step note entry -> Duncan style based on Roland Fleming Mode
	// At least one step button is pressed and we are not dealing with MIDI OFF
	else if (	( key_pressed != 0 )
			){

		// Linear scan through the buttons in question: Selectors and Steps.
		for( i=0; i <= 185; i++ ){
		
			// Not using the provided infrastructure because of the anomalies. may change
			// There is a key pressed - in the Step Range (exclude the rest)
			if (	( pressed_keys[i] >=  11 )
				&&	( pressed_keys[i] <= 185 ) 
				&& 	( ((pressed_keys[i]-10) % 11) != 0 ) 
				){

				// If we are not in PREVIEW mode
				if (	( target_page->editorMode != PREVIEW )
					){

					// Compute the key coordinates
					row = row_of( 		pressed_keys[i] );
					col = column_of(	pressed_keys[i] );
				
					// Record incoming notes to the given position
					record_note_to_track( 	target_page, 	row, 		col, 
											STEP_DEF_START,	in_pitch, 	in_velocity );									
				} // PREVIEW mode

			} // key pressed in step range

		} // button scan iterator

	} // key_pressed != 0 - Step for Roland Fleming mode			
}



//_______________________________________________________________________________________ 
//
// Make scale changes in page according to the incoming note pitch
	// The mode of operation is as follows:
	// Keys have two base states: OFF, ON, and if ON, a note can also be LEAD.
	// OFF is obvious. ON means tone is selected in the scale, 
	// LEAD means this is the lead tone of the scale - i.e. the counting reference for intervals.
	// Clicks in MOD mode select the lead. Clicks in CAD mode toggle the notes.
	// However, restrictions do apply, such as to make for a meaningful scale.
void modify_scale_composition( Pagestruct* target_page, unsigned char k ){

	// d_iag_printf( "key_SclSel_funs key:%d, sclStat:%d\n", k, target_page->scaleStatus );

	switch( target_page->scaleStatus ){
		
		
		// Change the SCALE - adding and removing notes from it
		case SCALE_SEL:

			switch( how_selected_in_scale( target_page, k ) ){

				case LEAD:
					// Note selected is LEAD
					// LEAD cannot be toggled off!
					break;					
									
				case OFF:
				case ON:
					// Note selected is OFF or ON
					// Add or remove note in scale ^1 corresponds to ON/OFF
					select_in_scale( target_page, k, how_selected_in_scale( target_page, k )^ON );
					
					// Program the new scale into the page						
					if ( target_page->SCL_align == ON ){

						program_scale_pitches( target_page );
					}							
					break;				
			} // switch( how_selected_in_scale( k )
			break;



		// Change the LEAD - used for scale bound trans-position
		case SCALE_MOD:
			
			// Select LEAD only from active notes in scale 
			if ( how_selected_in_scale( target_page, k ) != ON  ) {
				break;
			}

			// Store current lead so we know how much to shift pitches
			target_page->scaleLead_old = target_page->scaleLead[0];

			// Select the new scale lead
			select_in_scale( target_page, k, LEAD );

			// Program the new scale into the page		
			if ( target_page->SCL_align == ON ){

				program_scale_pitches( target_page );
			}
			break;

	} // switch( scale status )


	switch( my_shift_bitpattern( 	target_page->scaleNotes[0], 12, INC,
							my_bit2ndx( target_page->scaleLead[0] )+1) ){
	
		// Don't do anything if we are in a recognized scale
		case SCALE_SIG_PEN:
		case SCALE_SIG_WHL:
		case SCALE_SIG_MAJ:
		case SCALE_SIG_MIN:
		case SCALE_SIG_DIM:
		case SCALE_SIG_CHR:
			break;
		
		default:
		// Remember my scale signature when it is not a pre-set one. 
		target_page->my_scale_signature = 
			my_shift_bitpattern( 	target_page->scaleNotes[0], 12, INC,
									my_bit2ndx( target_page->scaleLead[0] )+1);
			break;
	}
}










