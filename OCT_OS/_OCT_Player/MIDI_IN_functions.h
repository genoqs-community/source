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





extern void program_scale_pitches( 	Pagestruct* target_page );
extern void select_in_scale( 		Pagestruct* target_page,
									unsigned char target_note,
									unsigned char target_state,
									unsigned char scale_ndx );

extern unsigned char advance_record_finger( Pagestruct* target_page, unsigned char position );

extern void 			midi_note_execute( 		unsigned char UART_ndx,
												unsigned char status_byte,
												unsigned char data_byte_1,
												unsigned char data_byte_2	);


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
	temp = in_velocity - target_page->Track[row]->attr_VEL;

	// Apply the computed pitch offset if it is within legal STEP offset range.
//	if (	( temp <= STEP_MAX_VELOCITY )
//		&&	( temp >= STEP_MIN_VELOCITY )
//		){

		target_page->Step[row][col]->attr_VEL = temp;
		result = TRUE;
//	}

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
	temp = in_pitch - target_page->Track[row]->attr_PIT;

	// Apply the computed pitch offset if it is within legal STEP offset range.
//	if (	( temp <= STEP_MAX_PITCH )
//		&&	( temp >= STEP_MIN_PITCH )
//		){

		// Actually should be adding the pitch to the chord of the step if step is ON
		target_page->Step[row][col]->attr_PIT = temp;
		result = TRUE;
//	}

	return result;
}



//_______________________________________________________________________________________
//
// Takes a pitch value and a track and returns the offset between the two
signed char adjust_PIT_to_trackpitch( unsigned char in_PIT, unsigned char track_pitch ){

	signed char result = 0;

	result = in_PIT - track_pitch;
	result = normalize( result, STEP_MIN_PITCH, STEP_MAX_PITCH );

	return result;
}



//_______________________________________________________________________________________
//
// Takes a pitch value and a track and returns the offset between the two
signed char adjust_PIT_to_track( unsigned char in_PIT, Trackstruct* target_track ){

	signed char result = 0;

	result = in_PIT - target_track->attr_PIT;
	result = normalize( result, STEP_MIN_PITCH, STEP_MAX_PITCH );

	return result;
}



//_______________________________________________________________________________________
//
// Checks whether the given pitch can be part of the given track
unsigned char is_pitch_in_track( unsigned char in_PIT, Trackstruct* target_track ){

	unsigned char result = FALSE;
	signed short delta = in_PIT - target_track->attr_PIT;

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

	result = in_VEL - target_track->attr_VEL;
	result = normalize( result, STEP_MIN_VELOCITY, STEP_MAX_VELOCITY );

	return result;
}




// Update the chord size value according to chord cardinality in step
void set_step_chord_size( Stepstruct* target_step, unsigned char size ){

	unsigned int temp=0;

	switch( size ){

		case 200:
			// This is a flag to update the value to the natural value
			temp = my_bit_cardinality( target_step->chord_data & 0x7FF );
			break;

		default:
			// Set the value to the passed value
			temp = size;
			break;
	}

	if ( temp < 8 ){

		// Shift the number into position
		temp = ( temp << 29 );

		// Clear the old chord size value in the 3 leftmost bits
		target_step->chord_up &= 0x1FFFFFFF;

		// Enter the new value in the three leftmost bits
		target_step->chord_up |= temp;

// d_iag_printf( "step:%d chord_size:%d\n", (int) &target_step, target_step->chord_up >> 29 );

	}
}



//_______________________________________________________________________________________
//
// Add the given pitch to the chord structure of the given step
// stackmode INC means to add to whats there, DEC means start fresh
void make_chord( Stepstruct* target_step, signed char track_pitch, signed char in_pitch ){

	unsigned char 	i				= 0;
	unsigned char 	j				= 0;
	unsigned char 	bit_offset 		= 0;
	signed char 	step_abs_pitch 	= 0;
	unsigned char 	pitch_delta 	= 0;
	unsigned char   step_pitches[7] = { 200, 200, 200, 200, 200, 200, 200 };
	unsigned char 	chord_offset 	= 0;
	unsigned char 	local_chordSize	= 0;
	unsigned char 	lowest_pitch 	= 200;

//	d_iag_printf( "make_chord*%x stepBase:%d trackBase:%d add:%d\n",
//			target_step, target_step->attr_PIT, track_pitch, in_pitch );

	// Restrict cardinality of the chord to 7, i.e. 6 aux notes allowed
	if ( my_bit_cardinality( target_step->chord_data & 0x7FF ) >= 7 ){
		// Do not continue
		return;
	}

	// Compute the absolute pitch of the step we are recording into.
	step_abs_pitch = normalize( track_pitch + target_step->attr_PIT, 0, 127);

// d_iag_printf( "x0 step_abs_pitch:%d\n", step_abs_pitch );

	// If incoming pitch is lower bring the step down to that pitch.
	// Remember all pitches so far and bring all back, lowest first.
	if ( in_pitch < step_abs_pitch ){

		// Find the size of the note stack making up the chord
		local_chordSize = my_bit_cardinality( target_step->chord_data & 0x07FF )+1;

		// Save the base pitch of the step before computing the chord stack
		step_pitches[0] = step_abs_pitch;
// d_iag_printf( "x0 pNdx[%d]:%d\n", i, step_pitches[i] );

		// Remember all pitches from the step chord stack
		for ( i=1; i < local_chordSize; i++ ){

			// Get the absolute chord offset for the step note
			chord_offset = offset_to_chord_aux( target_step->chord_data & 0x7FF, i-1 );

			// Get the chord up status for the note
			switch( get_chord_up( target_step, chord_offset )){
				case 1: chord_offset += 12; 	break;
				case 2: chord_offset += 24; 	break;
			}

			// Generate the step pitch from base and chord offset
			step_pitches[i] = step_abs_pitch + chord_offset;

// d_iag_printf( "x1 pNdx[%d]:%d\n", i, step_pitches[i] );
		}

		// Clear step chord data and up, bring the step down to in_pitch
		target_step->chord_data = (9 << 11);
		target_step->phrase_num = 0;
		target_step->phrase_data = PHRASE_DATA_INIT_VAL;
		target_step->chord_up 	= 0;
		target_step->attr_PIT 	= adjust_PIT_to_trackpitch( in_pitch, track_pitch );

		// Add the other pitches to the pitch stack
		for ( i=0; i < local_chordSize; i++ ){

			// Initialize the reference point
			lowest_pitch = 200;

			// Find the lowest pitch in the array
			for (j=0; j < local_chordSize; j++ ){

				if ( step_pitches[j] < lowest_pitch ){
					lowest_pitch = step_pitches[j];
					step_pitches[j] = 200;
				}
			}

			// Add the lowest pitch to the chord
			if ( lowest_pitch != 200 ){
// d_iag_printf( "add pNdx[%d]:%d\n", i, lowest_pitch );
				make_chord( target_step, track_pitch, lowest_pitch );
			}
		}
	}

	else {

		// Store the difference between the pitches
		pitch_delta = in_pitch - step_abs_pitch;

		// Compute the bit offset of in_pitch into the chord data array
		bit_offset = pitch_delta % 12;

		// Don't react on the same note (roll) just yet.. but on all other offsets
		if ( bit_offset == 0 ){
			 return;
		}

		// Normalize the offset to use every bit in the pattern
		bit_offset --;

		// Set the offset bit in question
		target_step->chord_data |= ( 1 << bit_offset );

		// Now set any chord_up bits that are necessary
		if (		( pitch_delta > 12 ) &&	( pitch_delta < 24 )
			){
			// Bit needs to play one octave higher
			set_chord_up( target_step, bit_offset, 1 );
		}
		else if ( 	( pitch_delta > 24 ) &&	( pitch_delta < 36 )
			){
			// Bit needs to play two octaves higher
			set_chord_up( target_step, bit_offset, 2 );
		}

		// Update the chord size to natural value
		set_step_chord_size( target_step, 200 );
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
void G_midi_insert_event( 	NOTEeventstruct* in_event,
							unsigned int in_timestamp,
							Trackstruct* target_track 	){

	unsigned int stepLength = 0;
	unsigned int stepLengthMax = 0;
	unsigned char multiplier = 0;

	// VELOCITY
	// Compute the velocity offset with reference to track VEL.
	in_event->target_step->attr_VEL 	=
		adjust_VEL_to_track( in_event->velocity, in_event->target_track );

	// LENGTH
	// Establish the actual length of the note in 1/192 units with reference to global clock.
	stepLength = normalize( in_timestamp - in_event->timestamp, STEP_MIN_LENGTH, STEP_MAX_LENGTH*8 );
//  d_iag_printf( "\n0:LEN:%d\n", stepLength );

	// Apply track clock multipliers and divisors, produce value with reference to track clock.
	stepLength = scaleToTrackClock( stepLength, in_event->target_track, NEG );
// d_iag_printf( "1:LEN:%d\n", stepLength );

	// Match LEN value into step, using the step LEN multiplier. Max multiplier is 8!
	stepLengthMax = scaleToTrackClock( STEP_MAX_LENGTH, in_event->target_track, NEG );
	stepLengthMax = 192;
 // d_iag_printf( "2:LENmax:%d\n", stepLengthMax );

	// Compute the step LEN multiplier - to be used as needed
	multiplier = 1 + ( (stepLength-1) / stepLengthMax );
// d_iag_printf( "3:multiplier:%d\n", multiplier );

	switch( multiplier ){

		case 1:
			// The length fits into the normal step LEN, simply assign it
			// nothing to do
			break;

		default:
			// The results are going to be somewhat quantified, but the max error is 7/192 over 8 tracks!
			stepLength = ( stepLength / multiplier );
			break;
	}

// d_iag_printf( "4:LEN:%d\n", stepLength );

	// Assign the step LEN value
	in_event->target_step->attr_LEN = stepLength;

	// Assign the step LEN multiplier
	in_event->target_step->event_data =
		(  (in_event->target_step->event_data & 0x0F) | (multiplier << 4)  );


	// START
	in_event->target_step->attr_STA = in_event->start;


	// STATUS adjustment: turn on Step under the locator (adjusted by 1)
	Step_set_status( in_event->target_step, STEPSTAT_TOGGLE, ON );


	// Final step: reG_scan_cycle the event - by placing it back onto the stack
	NOTE_stack_push( in_event );
}



//_______________________________________________________________________________________
//
// Learn into the current CCMAP
void CCMAP_learn( 	Pagestruct* target_page,
					unsigned char UART_ndx,
					unsigned char status_byte,
					unsigned char controller_number,
					unsigned char controller_value ){

	unsigned char controller_channel = ((status_byte & 0x0F) + 1 ) + ( 16 * UART_ndx );

#ifdef NEMO
	// nothing to do
#else
	// Make sure the page is rec enabled
	if ( target_page->REC_bit == OFF ) return;
#endif

	// Assign the controller number to the current CCMAP_learner
	switch( target_page->mixTarget ){

		case MIXTGT_USR0:
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

#ifdef NEMO
	target_page = GRID_assistant_page;
#endif

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

	// Play MIDI queue elements which are due just before current timestamp, including the above..
	play_MIDI_queue( G_MIDI_timestamp );
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
	signed short in_pitch_delta = 0;

	// Validate the target column number and stop if column is strange
	if ( target_col > 15 ) return;

	// Transform the in_pitch according to play chains head status
	// Check if we are recording in a chained track first
	if ( target_page->Track[row]->chain_data[NEXT] != target_page->Track[row] ){
		// The track is part of a chain, so check the chain play heads status
		if ( target_page->CHAINS_PLAY_HEAD == TRUE ){
			// Adjust the pitch to fit the head track pitch. If it doesn't fit, return!
			in_pitch_delta = target_page->Track[row]->chain_data[HEAD]->attr_PIT
				- target_page->Track[row]->attr_PIT;

			if (	( in_pitch + in_pitch_delta > 127 )
				||	( in_pitch + in_pitch_delta < 0 )
				){
					return;
			}
			else {
				in_pitch += in_pitch_delta;
			}
		}
	}


	// Determine whether we are dealing with a note ON or OFF
	switch( in_velocity ){

		// NOTE OFF scenario
		case OFF:

			// Remove from queue an appropriately pitched event
			off_event = NOTE_queue_remove( in_pitch );

			if ( off_event != NULL ){

				// Insert event into the sequencing engine - i.e. activate corresponding step!
				G_midi_insert_event( off_event, G_MIDI_timestamp, target_page->Track[row] );
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
					make_chord( target_page->Step[row][target_col],
								target_page->Track[row]->attr_PIT, in_pitch );
					break;

				case FALSE:

					// If the step is on, add pitch to it and make a chord
					if ( Step_get_status( target_page->Step[row][target_col], STEPSTAT_TOGGLE ) == ON ){

						// Add the incoming pitch to the chord structure of the step
						make_chord( target_page->Step[row][target_col],
									target_page->Track[row]->attr_PIT, in_pitch );
					}
					else{

						// CHORD - No chord set for the step, and 0 is the neutral strum value
						target_page->Step[row][target_col]->chord_data	= (9 << 11);
						target_page->Step[row][target_col]->phrase_num	= 0;
						target_page->Step[row][target_col]->phrase_data	= PHRASE_DATA_INIT_VAL;
						// Clear the chord up data of the step
						target_page->Step[row][target_col]->chord_up = 0;

						// PITCH - Fill the step pitch value simply.. after erasing traces of chords
						target_page->Step[row][target_col]->attr_PIT =
							adjust_PIT_to_track( in_pitch, target_page->Track[row] );

						// VELOCITY - Fill the step with the right velocity offset
						target_page->Step[row][target_col]->attr_VEL =
							adjust_VEL_to_track( in_velocity, target_page->Track[row] );

						// START - Fill the step with the right start offset
						target_page->Step[row][target_col]->attr_STA =
							target_start;

						// STATUS - Turn the Step on
						Step_set_status( target_page->Step[row][target_col], STEPSTAT_TOGGLE, ON );
					}
					break;
			}

			// Fill the on-event with the current step data
			NOTEevent_fill( 	on_event, 	target_page->Step[row][target_col],
											target_page->Track[row],
											G_MIDI_timestamp,
											in_velocity,
											in_pitch,
											OFF,
											target_start
						  );

			// Insert event into the queue, waiting for its NOTE OFF to arrive.
			NOTE_queue_insert( on_event );

			break;

	} // switch( in_velocity )
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
void NEMO_modify_scale_composition( 	Pagestruct* target_page,
										unsigned char k,
										unsigned char scale_ndx ){


	switch( target_page->scaleStatus ){


		// Change the SCALE - adding and removing notes from it
		case SCALE_SEL:

			switch( how_selected_in_scale( target_page, k, scale_ndx ) ){

				case LEAD:
					// Note selected is LEAD
					// LEAD cannot be toggled off!
					break;

				case OFF:
				case ON:
					// Note selected is OFF or ON
					// Add or remove note in scale ^1 corresponds to ON/OFF
					select_in_scale( target_page, k,
						how_selected_in_scale( target_page, k, scale_ndx )^ON, scale_ndx );

					// Program the new scale into the page
					if ( target_page->SCL_align == ON ){

						program_scale_pitches( target_page );
					}
					break;
			}
			break;



		// Change the LEAD - used for scale bound trans-position
		case SCALE_MOD:

//			// Select LEAD only from active notes in scale
//			if ( how_selected_in_scale( target_page, k ) != ON  ) {
//				break;
//			}

//d_iag_printf( "0 current scale+lead:\n" );
//my_print_bits( target_page-> scaleNotes[G_scale_ndx] );
//my_print_bits( target_page-> scaleLead[G_scale_ndx] );

			// Store current scale, so we can rebuild it after LEAD change
			// Pretend the old lead was C, and store the scale pattern
			target_page->scaleNotes_old =
				my_shift_bitpattern(	target_page-> scaleNotes[ scale_ndx ], 12, DEC,
										(11 - my_bit2ndx(target_page-> scaleLead[ scale_ndx ]) )  );

// d_iag_printf( "0.5 old scale from C:\n" );
// my_print_bits( target_page->scaleNotes_old );

			// Select the new scale lead
			select_in_scale( target_page, k, LEAD, scale_ndx );

// d_iag_printf( "1 current scale+lead:\n" );
// my_print_bits( target_page-> scaleNotes[G_scale_ndx] );
// my_print_bits( target_page-> scaleLead[G_scale_ndx] );

			// Adjust the scale as to carry over to the new base pitch
			// Shift the signature according to the lead
			target_page-> scaleNotes[ scale_ndx ] =
				my_shift_bitpattern( 	target_page->scaleNotes_old, 12, INC,
										(11 - my_bit2ndx(target_page-> scaleLead[ scale_ndx ] ) )  );

// d_iag_printf( "2 current scale+lead:\n" );
// my_print_bits( target_page-> scaleNotes[G_scale_ndx] );
// my_print_bits( target_page-> scaleLead[G_scale_ndx] );

			// Declare this as status quo needed below
			target_page->scaleLead_old 		= target_page-> scaleLead[ scale_ndx ];
			target_page->scaleNotes_old 	= target_page-> scaleNotes[ scale_ndx ];

			// Program the new scale into the page
			if ( target_page->SCL_align == ON ){

				program_scale_pitches( target_page );
			}
			break;

	} // switch( scale status )


	switch( my_shift_bitpattern( 	target_page-> scaleNotes[ scale_ndx ], 12, INC,
							my_bit2ndx( target_page-> scaleLead[ scale_ndx ] )+1) ){

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
			my_shift_bitpattern( 	target_page-> scaleNotes[ scale_ndx ], 12, INC,
									my_bit2ndx( target_page-> scaleLead[ scale_ndx ] )+1);
			break;
	}
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
void modify_scale_composition( 	Pagestruct* target_page,
								unsigned char k,
								unsigned char scale_ndx ){


	switch( target_page->scaleStatus ){


		// Change the SCALE - adding and removing notes from it
		case SCALE_SEL:

			switch( how_selected_in_scale( target_page, k, scale_ndx ) ){

				case LEAD:
					// Note selected is LEAD
					// LEAD cannot be toggled off!
					break;

				case OFF:
				case ON:
					// Note selected is OFF or ON
					// Add or remove note in scale ^1 corresponds to ON/OFF
					select_in_scale( target_page, k,
						how_selected_in_scale( target_page, k, scale_ndx )^ON, scale_ndx );

					// Program the new scale into the page
					if ( target_page->SCL_align == ON ){

						program_scale_pitches( target_page );
					}
					break;
			} // switch( how_selected_in_scale( k )
			break;



		// Change the LEAD - used for scale bound trans-position
		case SCALE_MOD:

//			// Select LEAD only from active notes in scale
//			if ( how_selected_in_scale( target_page, k ) != ON  ) {
//				break;
//			}

//d_iag_printf( "0 current scale+lead:\n" );
//my_print_bits( target_page-> scaleNotes[G_scale_ndx] );
//my_print_bits( target_page-> scaleLead[G_scale_ndx] );

			// Store current scale, so we can rebuild it after LEAD change
			// Pretend the old lead was C, and store the scale pattern
			target_page->scaleNotes_old =
				my_shift_bitpattern(	target_page-> scaleNotes[ scale_ndx ], 12, DEC,
										(11 - my_bit2ndx(target_page-> scaleLead[ scale_ndx ]) )  );

// d_iag_printf( "0.5 old scale from C:\n" );
// my_print_bits( target_page->scaleNotes_old );

			// Select the new scale lead
			select_in_scale( target_page, k, LEAD, scale_ndx );

// d_iag_printf( "1 current scale+lead:\n" );
// my_print_bits( target_page-> scaleNotes[G_scale_ndx] );
// my_print_bits( target_page-> scaleLead[G_scale_ndx] );

			// Adjust the scale as to carry over to the new base pitch
			// Shift the signature according to the lead
			target_page-> scaleNotes[ scale_ndx ] =
				my_shift_bitpattern( 	target_page->scaleNotes_old, 12, INC,
										(11 - my_bit2ndx(target_page-> scaleLead[ scale_ndx ] ) )  );

// d_iag_printf( "2 current scale+lead:\n" );
// my_print_bits( target_page-> scaleNotes[G_scale_ndx] );
// my_print_bits( target_page-> scaleLead[G_scale_ndx] );

			// Declare this as status quo needed below
			target_page->scaleLead_old 		= target_page-> scaleLead[ scale_ndx ];
			target_page->scaleNotes_old 	= target_page-> scaleNotes[ scale_ndx ];

			// Program the new scale into the page
			if ( target_page->SCL_align == ON ){

				program_scale_pitches( target_page );
			}
			break;

	} // switch( scale status )


	switch( my_shift_bitpattern( 	target_page-> scaleNotes[ scale_ndx ], 12, INC,
							my_bit2ndx( target_page-> scaleLead[ scale_ndx ] )+1) ){

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
			my_shift_bitpattern( 	target_page-> scaleNotes[ scale_ndx ], 12, INC,
									my_bit2ndx( target_page-> scaleLead[ scale_ndx ] )+1);
			break;
	}
}


//Wilson - Transpose pitch assignment
//Offset track base pitch on incomming transpose channel attr_TCH
//Restore pitch when key velocity is strong
//NOTE_OFF restores previous pitch (GST attribute)
void transpose_selection(	Pagestruct* target_page,
							unsigned char in_pitch,
							unsigned char in_velocity,
							unsigned char inputMidiChan	) {
	unsigned char i = 0;

	for ( i=0; i < MATRIX_NROF_ROWS; i++ ){
		// Apply to every selected track..
		if ( *target_page->Track[i]->attr_TCH
			&& ( in_velocity > 0 )
			&& ( inputMidiChan == *target_page->Track[i]->attr_TCH) ) {
				// velocity is strong trigger mode
			 	SET_BIT_VALUE( target_page->Track[i]->attr_EMISC, GST_TOGGLE, (int) (in_velocity > 88) );
				if ( CHECK_BIT( target_page->Track[i]->attr_EMISC, GST_TOGGLE) ) {
					target_page->Track[i]->attr_PIT = target_page->Track[i]->attr_GST;
				} else {
					// Assign pitch of the incoming note to the recording track
					if( target_page->pitch_abs ){

						target_page->Track[i]->attr_PIT = in_pitch;

					} else {

						target_page->Track[i]->attr_PIT = in_pitch + (60 - target_page->Track[i]->attr_GST); //offset relative fixed to middle C midi 60
					//	printf( "in pitch: %d\n",in_pitch );
					}
				}

		}
	}
}

//_______________________________________________________________________________________
//
void assign_note_to_selection( 	Pagestruct* target_page,
								unsigned char in_pitch,
								unsigned char in_velocity ) {

	unsigned char row = 0;
	unsigned char col = 0;
	unsigned char i = 0;

	// Track pitch assignment -> transposition style..
	if ( target_page->trackSelection != 0 ){

		// d_iag_printf("OK 3\n");

		for ( i=0; i < MATRIX_NROF_ROWS; i++ ){

			// Apply to every selected track..
			if ( (target_page->trackSelection & ( 1 << i )) != 0 ){

				// Assign pitch of the incoming note to the recording track
				target_page->Track[i]->attr_PIT = in_pitch;
			}
		}
	}


	// STEP ASSIGNMENT - when in Step mode - make the step assignment to that step
	else if ( G_zoom_level == zoomSTEP ){

		// Establish the coordinates
		row = target_page->stepSelectionSingleRow;
		col = target_page->stepSelectionSingleCol;

		// Record incoming notes to the given position
		record_note_to_track( 	target_page, 	row, 		col,
								STEP_DEF_START,	in_pitch, 	in_velocity );
	}


	// STEP RECORD -> Duncan style based on Roland Fleming Mode
	// At least one step button is pressed and we are not dealing with MIDI OFF
	else if (	( G_key_pressed != 0 )

			// Make sure we are in Page mode
			&& 	( G_zoom_level == zoomPAGE )

			// Make sure we are not in preview mode
			&&	( target_page->editorMode != PREVIEW )
		){


		// d_iag_printf( "OK 4\n" );

		// Linear G_scan through the buttons in question: Selectors and Steps.
		for( i=0; i <= 185; i++ ){

			// Not using the provided infrastructure because of the anomalies. May change..
			// There is a key pressed - in the Step Range (and excluding the rest of matrix)
			if (	( G_pressed_keys[i] >=  11 )
				&&	( G_pressed_keys[i] <= 185 )
				&& 	( ((G_pressed_keys[i]-10) % 11) != 0 )
				){

				// Check status of the virtual record finger
				if ( STEP_RECORD_FINGER == 0 ){

					// Line up record finger to real finger - this is the first step in session
					STEP_RECORD_FINGER = G_pressed_keys[i];
				}

				// Compute the key coordinates
				row = row_of( 		STEP_RECORD_FINGER );
				col = column_of(	STEP_RECORD_FINGER );

				// Record incoming notes to the given position
				record_note_to_track( 	target_page, 	row, 		col,
										STEP_DEF_START,	in_pitch, 	in_velocity );

				// Fix the length to default
				target_page->Step[row][col]->attr_LEN = STEP_DEF_LENGTH;

				// Prepare for potentially next appended step - the button is still pressed
				if (	( STEP_RECORD_FINGER != 0 )

					// ..we are seeing a key press, not a release
					&&	( in_velocity == 0 )

					// ..and there are no more unreleased keys on the keyboard
					&&	( NOTE_queue == NULL )
					){

					// Advance the record finger to the next valid step in the sequence
					STEP_RECORD_FINGER =
						advance_record_finger( target_page, STEP_RECORD_FINGER );
				}



				// MONITORING
	 			// ----------
 				card8 headRow = row;
 				if ( target_page->CHAINS_PLAY_HEAD == TRUE ) {
 					headRow = row_of_track( target_page, target_page->Track[row]->chain_data[HEAD] );
 				}

 				int trackMidiBus  	= (target_page->Track[headRow]->attr_MCH -1) / 16;
 				int trackMidiChan 	= ((target_page->Track[headRow]->attr_MCH + target_page->Track[headRow]->event_offset[ATTR_MIDICH] -1) % 16) + 1;

				MIDI_NOTE_new( trackMidiBus * 16 + trackMidiChan, scale_pitch(target_page, in_pitch), in_velocity, 0 );
				play_MIDI_queue( G_MIDI_timestamp );


				// Do not continue to G_scan steps after having recorded first one.
				// Otherwise we get a conflict within the Step record finger logic.
				return;

			} // key pressed in step range

		} // button G_scan iterator

	} // G_key_pressed != 0 - Step for Roland Fleming mode
}


int scale_pitch( Pagestruct* pagePt, int pitch )
{
	if ( pagePt->force_to_scale == ON ) {

		// If the note index of the initial pitch is not in the scale
		if ( how_selected_in_current_scale( pagePt, pitch_to_noteNdx( pitch ) ) == OFF ){
			// Add to initial pitch the offset to the next scale degree
			pitch += offset_to_next_current_degree( pagePt, pitch );
		}
	}

	return pitch;
}
