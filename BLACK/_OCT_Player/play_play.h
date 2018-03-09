

unsigned char advance_track_TTC( 				Pagestruct* target_page,
												unsigned char row );	

Trackstruct* advance_track_TTC_new( 			Pagestruct* target_page,
												Trackstruct* target_track );															

extern unsigned char active_row_in_chunk(		unsigned char chunk, 
												unsigned char chainMode, 
												unsigned char chainCounter 	);

extern unsigned char how_selected_in_scale( 	Pagestruct* target_page, 
												unsigned char target_note 	);

extern unsigned char how_selected_in_current_scale( 	
												Pagestruct* target_page, 
												unsigned char target_note 	);

extern unsigned short extract_current_scale( 	Pagestruct* target_page );

extern void 			play_MIDI_queue( 		unsigned int in_MIDI_timestamp );



// Convert a step length from Step->attribute[ATTR_LENGTH]
// to the corresponding number of TTC ticks. Basically map a value in [0..16] to [0..12]
unsigned int length2_ticks( unsigned int length ){
	
	unsigned int ticks=0, LOC_offset=0, TTC_offset=0;
	
	LOC_offset = (length / 16);
	TTC_offset = (length % 16);
	
	// Simple non-deterministic mapping of 16 to 24 for TTC_offset! 
	// Using rand()%2 to simulate "*1,5": (rand()%2). P.169 book
	switch ( TTC_offset % 2 ){
		case 0:
			TTC_offset += (TTC_offset/2);
			break;

		case 1:
			TTC_offset += (TTC_offset/2 + rand()%2);			
			break;
	}
	// Now map the value to the space [0..12] -> p.169 book !! by the /2 operation
	// ..the detour via 24 is merely historic and surely can be optimized out.
	TTC_offset = TTC_offset / 2;
	
	ticks = ( LOC_offset * OCTDEF_TTC_PER_16TH )  +  TTC_offset;
	

	// Make sure we don't return a value of 0!! 
	// This has no meaning and confuses things.
	if (ticks == 0) {
	
		// 1 is the smallest meaningful value for length ticks.
		ticks = 1;
	}

	return ticks;
}



// Takes the length of a step from its attribute and returns the
// value it represents in terms of clockticks (TTC_ABS format)
unsigned int get_clockticks_length( 	Stepstruct* target_step, 
										unsigned char tempo_multiplier ){

	// Get the step length: basically a space saver
	unsigned char step_length = 
		(unsigned char)target_step->attribute[ATTR_LENGTH];

	// Total distance in TTC ticks to the needed noteOFF.
	return( length2_ticks( step_length )  /  tempo_multiplier );
}



// Build MIDI OFF event for given track attributes and line it up in the MIDI queue
void MIDI_OFF_build_trackpreview( 	unsigned char midiCH, 
									unsigned char pitch 	){

	unsigned int OFF_timestamp = 0;
	
	// Get an empty MIDI event from the MIDI stack
	MIDIeventstruct* MIDI_OFF_event = 	MIDI_stack_pop();

	// Compute the timestamp for playing the MIDI OFF. Current time + length.
	OFF_timestamp 	= 	MIDI_timestamp 
						+ 	length2_ticks( STEP_DEF_LENGTH );

	// Fill in the event data for MIDI OFF
	MIDIevent_fill( 	MIDI_OFF_event, 
						OFF_timestamp, 
						MIDI_NOTE, 
						midiCH, 
						pitch, 
						OFF ); 

	// Line the event in the MIDI Queue
	MIDI_queue_insert( MIDI_OFF_event );	
}



// Enqueue MIDI note given by: channel, pitch, velocity, length
void MIDI_NOTE_new( 	unsigned char midiCH, 
						unsigned char pitch,
						unsigned char velocity,
						unsigned int  length	){

	// Compute the timestamp of the event from current MIDI_timestamp and given length
	unsigned int event_timestamp = MIDI_timestamp + length;

	// This is the legato case - or when passing MIDI notes through
	if ( length == TIMESTAMP_MAX )	event_timestamp	= 	TIMESTAMP_MAX;
	
	// Get an empty MIDI event from the MIDI stack
	MIDIeventstruct* note_event = 	MIDI_stack_pop();
 	
	// Fill the event data for MIDI NOTE
	MIDIevent_fill( 	note_event, 
						event_timestamp, 
						MIDI_NOTE, 
						midiCH, 
						pitch, 
						velocity ); 

	// Line up the event in the MIDI Queue
	MIDI_queue_insert( note_event );
}



// Build MIDI OFF event with the given coordinates
void MIDI_OFF_build_new(	unsigned char midiCH, 
							unsigned char pitch,
							unsigned int length	){

	unsigned int OFF_timestamp = 0;
	
	// Get an empty MIDI event from the MIDI stack
	MIDIeventstruct* MIDI_OFF_event = 	MIDI_stack_pop();

	// Compute the timestamp for playing the MIDI OFF. Current time + length.
	if ( length == TIMESTAMP_MAX ){
		OFF_timestamp	= 	TIMESTAMP_MAX;
	}
	else{	
		OFF_timestamp 	= 	MIDI_timestamp + length;
	}
	
	// Fill in the event data for MIDI OFF
	MIDIevent_fill( 	MIDI_OFF_event, 
						OFF_timestamp, 
						MIDI_NOTE, 
						midiCH, 
						pitch, 
						OFF ); 

	// Line the event in the MIDI Queue
	MIDI_queue_insert( MIDI_OFF_event );
}



// Build MIDI OFF event for given step and line it up in the MIDI queue
void MIDI_OFF_build( 	Stepstruct* target_step, 
						unsigned char length_offset,
						unsigned char midiCH, 
						unsigned char pitch,
						unsigned char tempo_multiplier	){

	unsigned int OFF_timestamp = 0;
	
	// Get an empty MIDI event from the MIDI stack
	MIDIeventstruct* MIDI_OFF_event = 	MIDI_stack_pop();

	// Compute the timestamp for playing the MIDI OFF. Current time + length.
	OFF_timestamp 	= 	MIDI_timestamp 
						+ 	get_clockticks_length( target_step, tempo_multiplier )
						+	( length2_ticks( length_offset )  /  tempo_multiplier  );

	// Fill in the event data for MIDI OFF
	MIDIevent_fill( 	MIDI_OFF_event, 
						OFF_timestamp, 
						MIDI_NOTE, 
						midiCH, 
						pitch, 
						OFF ); 

	// Line the event in the MIDI Queue
	MIDI_queue_insert( MIDI_OFF_event );
}



signed int my_max_value( signed int in_1, signed int in_2 ){

	if ( in_1 < in_2 ){ 
		return in_2;
	}
	else{ 
		return in_1;
	}
}


// Return the smaller of two numbers
signed int my_min_value( signed int in_1, signed int in_2 ){

	if ( in_1 < in_2 ){ 
		return in_1;
	}
	else{ 
		return in_2;
	}
}


// Perform the event programmed in the in step onto the givent track
void perform_step_event( 	Stepstruct* in_step, 
							Trackstruct* target_track, 
							Pagestruct* target_page,
							unsigned char in_row){
				
	unsigned char direction = 0;
	unsigned char temp_1 = 0;
	unsigned char temp_2 = 0;

	// Depending on the attribute of the event, do things..
	switch( (in_step->event_data & 0x0F) + 1 ){
	
		case ATTR_VELOCITY:

			// For now just keep adding the step amount to the track - but ceil at 127
			temp_1 = 1 + my_min_value( target_track->range_VEL, 
						TRACK_MAX_VELOCITY - target_track->attribute[ATTR_VELOCITY] );

			// Wrap-preparing negative step amounts
			if ( (target_track->event_VEL + in_step->attribute[ATTR_AMOUNT]) < 0 ){
				temp_2 = 1 + target_track->range_VEL 
						- abs( target_track->event_VEL + in_step->attribute[ATTR_AMOUNT] );
			}
			// Or simply adding step amounts..
			else{
				temp_2 = target_track->event_VEL + in_step->attribute[ATTR_AMOUNT];
			}
				
			target_track->event_VEL = 
				normalize( temp_2 % temp_1, TRACK_MIN_VELOCITY, TRACK_MAX_VELOCITY );			
			break;


		case ATTR_PITCH:

			// For now just keep adding the step amount to the track - but ceil at 127
			temp_1 = 1 + my_min_value( target_track->range_PIT, 
						TRACK_MAX_PITCH - target_track->attribute[ATTR_PITCH] );

			// Wrap-preparing negative step amounts
			if ( (target_track->event_PIT + in_step->attribute[ATTR_AMOUNT]) < 0 ){
				temp_2 = 1 + target_track->range_PIT 
						- abs( target_track->event_PIT + in_step->attribute[ATTR_AMOUNT] );
			}
			// Or simply adding step amounts..
			else{
				temp_2 = target_track->event_PIT + in_step->attribute[ATTR_AMOUNT];
			}
				
			target_track->event_PIT = 
				normalize( temp_2 % temp_1, TRACK_MIN_PITCH, TRACK_MAX_PITCH );			

			break;


		case ATTR_LENGTH:
//			target_track->attribute[ATTR_LENGTH] = normalize(
//				my_max_value( 	0, 	target_track->attribute[ATTR_LENGTH] 
//									+ 	in_step->attribute[ATTR_AMOUNT] ),
//								0,
//								TRACK_MAX_LENFACTOR );

			target_track->LEN_factor = 
				normalize(
					my_max_value( 0, target_track->LEN_factor + in_step->attribute[ATTR_AMOUNT] ),
					0, TRACK_MAX_LENFACTOR );
			break;


		case ATTR_START:
//			target_track->attribute[ATTR_START] = normalize(
//				my_max_value( 	0, 
//								my_min_value( 	target_track->attribute[ATTR_START] 
//								               	+ in_step->attribute[ATTR_AMOUNT],
//
//								               	target_track->attribute[ATTR_LENGTH]
//								               	- target_track->attribute[ATTR_START] 
//								             ) 
//							 ),
//				0,
//				TRACK_MAX_STAFACTOR );

			target_track->STA_factor = 
				normalize(
					my_max_value( 0, target_track->STA_factor + in_step->attribute[ATTR_AMOUNT] ),
					0, TRACK_MAX_STAFACTOR );
			break;


		case ATTR_POSITION:

			if ( in_step->attribute[ATTR_AMOUNT] >= 0 ){
				direction = INC;
			}
			else{
				direction = DEC;
			}
			if ( in_step->attribute[ATTR_AMOUNT] != 0 ){
				Page_wrap_track( 	target_page, 
									in_row,
									direction, 
									abs( in_step->attribute[ATTR_AMOUNT] )%16);
			}
			break;


		case ATTR_DIRECTION:
			target_track->attribute[ATTR_DIRECTION] = 
				abs( target_track->attribute[ATTR_DIRECTION] + in_step->attribute[ATTR_AMOUNT] )
				% TRACK_MAX_DIRECTION;
			break;


		case ATTR_AMOUNT:
			target_track->attribute[ATTR_AMOUNT] = 
				abs( target_track->attribute[ATTR_AMOUNT] + in_step->attribute[ATTR_AMOUNT] )
				% TRACK_MAX_AMOUNT;

			break;


		case ATTR_GROOVE:
			target_track->attribute[ATTR_GROOVE] = 
				abs( target_track->attribute[ATTR_GROOVE] + in_step->attribute[ATTR_AMOUNT] )
				% TRACK_MAX_GROOVE;
			break;


		case ATTR_MIDICC:
			// target_track->attribute[ATTR_MIDICC] = 
			//	abs( target_track->attribute[ATTR_MIDICC] + in_step->attribute[ATTR_AMOUNT] )
			//	% TRACK_MAX_MIDICC;

			// For now just keep adding the step amount to the track - but ceil at 127
			temp_1 = 1 + my_min_value( target_track->range_MCC, 
						STEP_MAX_MIDICC - in_step->attribute[ATTR_MIDICC] );

			// Wrap-preparing negative step amounts
			if ( (target_track->event_MCC + in_step->attribute[ATTR_AMOUNT]) < 0 ){
				temp_2 = 1+ target_track->range_MCC
						- abs( target_track->event_MCC + in_step->attribute[ATTR_AMOUNT] );
			}
			// Or simply adding step amounts..
			else{
				temp_2 = target_track->event_MCC + in_step->attribute[ATTR_AMOUNT];
			}
				
			target_track->event_MCC = 
				normalize( temp_2 % temp_1, STEP_MIN_MIDICC, STEP_MAX_MIDICC );

			break;


		case ATTR_MIDICH:
			target_track->attribute[ATTR_MIDICH] = 
				abs( target_track->attribute[ATTR_MIDICH] + in_step->attribute[ATTR_AMOUNT] )
				% TRACK_MAX_MIDICH;
			break;
	}
}





// Play the NOTE ON events in the given row of target_page
void play_row_ON( 	Pagestruct* 	target_page,
					unsigned char 	row, 
					unsigned int 	bitactivity, 
					unsigned char 	locator,
					unsigned int 	which_col,
					unsigned int 	in_TTC_ABS_VALUE,
					unsigned char 	multiplier_flag ){

	signed int 		length			= 0;
	signed int 		temp			= 0;
	unsigned char 	i				= 0,
					j 				= 0,
					pitch_initial 	= 0,
					midiCH 			= 0,
					chord_offset 	= 0,
					start_offset	= 0,
					local_chordSize = 0;
					
	signed char 	EFF_pool_VEL 	= 0,
					EFF_pool_PIT	= 0,
					EFF_pool_LEN	= 0;

	signed char		pitch 			= 0,
					velocity 		= 0;
	
	unsigned short 	step_chord_pattern 	= 0;
	unsigned char 	step_strum_level	= 0;
	unsigned char 	step_strum_dir 	= 0;
	unsigned char 	step_strum_ndx	= 0;

	// Strum offsets to spice up things..
	signed char		strum_offset_VEL = 0;
	signed char		strum_offset_PIT = 0;
	signed char		strum_offset_LEN = 0;
	signed char		strum_offset_STA = 0;
	

	// Do not play track if its locator is at 0
	if ( locator == 0 ) return;

	// Do not play track if muted - this is already taken care of in terms of set play bits..
	// ..but need to do it again so we can mute the EFF function of an empty track.
	if ( ( target_page->trackMutepattern & (1<<row) ) != 0 ) return;

	// Only in CURRENT mode - implement EFFector 
	if ( which_col == CURRENT ){

		// 
		// EFF pool generation - attribute offset - regardless of the active steps in track
		//
		switch( Track_get_MISC( target_page->Track[row], EFF_BIT ) ){
	
			// Move the EFF values into the EFF pool
			case SEND:
				// This means that EFF_pool has been already filled, no need to continue		
				if ( multiplier_flag != 0 ) break;
				// VEL
				target_page->EFF_pool[ ATTR_VELOCITY ] +=
					(		target_page->Step[row][locator-1]->attribute[ATTR_VELOCITY]
						- 	STEP_DEF_VELOCITY );
				// PIT
				target_page->EFF_pool[ ATTR_PITCH ] +=
					(		target_page->Step[row][locator-1]->attribute[ATTR_PITCH]
						-	STEP_DEF_PITCH );		
				// LEN
				target_page->EFF_pool[ ATTR_LENGTH ] +=
					(		(unsigned char) target_page->Step[row][locator-1]->attribute[ATTR_LENGTH]
						-	STEP_DEF_LENGTH );
				// START
				// .. pretty hard to do
				// MCC
				// target_page->EFF_pool[ ATTR_MCC ] +=
				//	target_page->Step[row][locator-1]->attribute[ATTR_MCC];
				break;
	
			// Get the EFF values out of the EFF pool
			case RECEIVE:
				// Only these for now, the usual suspects. PIT and VEL watch their interval ranges!
				EFF_pool_VEL = 
					target_page->EFF_pool[ ATTR_VELOCITY ] % target_page->Track[row]->range_VEL;
				EFF_pool_PIT = 
					target_page->EFF_pool[ ATTR_PITCH ] % target_page->Track[row]->range_PIT;
				EFF_pool_LEN = 
					target_page->EFF_pool[ ATTR_LENGTH ];
				break;
		} // EFF pool generation - switch on EFF bit of track
	}

	// Is the actual Step playing ?? - This check is needed here and not earlier, 
	// ..or the EFF pool will not be filled
	if (( bitactivity & (1 << row) ) == 0 ){		return;	}
	
	
	//
	// NOTE ON data gathering
	//
	
	// CHANNEL:
	midiCH 	= target_page->Track[row]->attribute[ATTR_MIDICH];


	// PITCH: Differentiate between playing the pitch offsets or the natural pitches
	// This computation of the initial pitch should suffice. FTS applied in chord loop

	// PIT factor computation for the step pitch offset
	pitch_initial	= normalize(
			// Modulators
			  EFF_pool_PIT
			+ target_page->Track[row]->event_PIT
		  	
			// Step - factorized pitch attribute value
		  	// + target_page->Step [row][locator-1]->attribute[ATTR_PITCH]
			+ ( (	target_page->Step [row][locator-1]->attribute[ATTR_PITCH]
					* Track_PIT_factor[target_page->Track[row]->PIT_factor] ) / 25 )
					  	
			// Track
		  	+ target_page->Track[row]->attribute[ATTR_PITCH]
			+ VIEWER_page->Track[row]->scale_pitch_offset
			+ VIEWER_page->Track[row]->lead_pitch_offset			
			
			// Page
			+ (target_page->attribute[ATTR_PITCH] - MIDDLE_C), 	
		  	
		  	0, 127);
	
	// A pitch of 0 is generally flagged as empty, so can't be played now.
	if ( pitch == 0 ) pitch = 1;


	// STRUM level and direction
	step_strum_level = (target_page->Step[row][locator-1]->chord_data & 0xF800) >> 11;
	if ( step_strum_level >= 9 ){
		step_strum_dir = POS;
		step_strum_level = step_strum_level - 9;
	}
	else{
		step_strum_dir = NEG;
		step_strum_level = 9 - step_strum_level;
	}


	// 
	// S T E P   P L A Y trigger and chord cardinality iterator.
	//

	// Chord pattern, size and offset init. STEP chord overrules TRACK chord.
	step_chord_pattern 	= (target_page->Step[row][locator-1]->chord_data & 0x7FF) << 1;
	local_chordSize 	= my_bit_cardinality( step_chord_pattern );
	chord_offset 		= 0;


	// START Offset computation - depending on the column we are looking at:
	switch( which_col ){

		case CURRENT:
			if ( target_page->Step[row][locator-1]->attribute[ATTR_START] >= STEP_DEF_START ){

				// Term simplification
				temp = target_page->Step[row][locator-1]->attribute[ATTR_START] - STEP_DEF_START;

				// Computing the start offset by Duncan - factorizing
				start_offset = 
					normalize ( ( temp * Track_STA_factor[target_page->Track[row]->STA_factor] ) / 100,
								0,
								192 );

				// Add to start_offset the groove offset
				start_offset = start_offset + 
					// Factorized groove offset.. this is really Heisenberg.
					( ( get_track_GRV_offset( target_page, row, locator-1, CURRENT ) 
					* Track_GRV_factor[target_page->Track[row]->GRV_factor] ) / 25 ); 

				start_offset = start_offset + 
					get_step_GRV_offset( target_page, row, locator-1, CURRENT );
			}
			else{
				return;
			}
			break;


		case NEXT:
			if ( target_page->Step[row][locator-1]->attribute[ATTR_START] < STEP_DEF_START ){

				// Term simplification
				temp = 1 + STEP_DEF_START - target_page->Step[row][locator-1]->attribute[ATTR_START];

				// Computing the start offset by Duncan
				start_offset = 
					normalize ( (	(STEP_MAX_START + 1) 
						- (( temp * Track_STA_factor[target_page->Track[row]->STA_factor] ) / 100) ),
							0,
							192);

				// Add to start_offset the groove offset
				start_offset = start_offset + 
					get_track_GRV_offset( target_page, row, locator-1, NEXT );

				start_offset = start_offset + 
					get_step_GRV_offset( target_page, row, locator-1, NEXT );
			}
			else{
				return;
			}
			break;
	}

	// d_iag_printf( "PLAY_row_ON->step - start_offset:%d\n", start_offset );

	// Decide whether to play step or track chord, depending on STEP chordsize.
	switch( local_chordSize ){

		case 0:
			// TRACK CHORD may be played, since there are no aux pitches in step
			// ..so update chordsize to the track's chordsize.
			local_chordSize = Track_get_MISC( target_page->Track[row], CHORD_BIT );

			#include "Track_chord_player.h"
			break;

		default:
			// STEP CHORD scenario - step has auxilliary pitches
			#include "Step_chord_player.h"
			break;
	}
}




// Play the MCC events in the given row
void play_row_MCC( 	Pagestruct* target_page,
					unsigned char row, 
					unsigned int MCCpattern, 
					unsigned int groove_indicator,
					unsigned int in_TTC_ABS_VALUE, 
					unsigned char multiplier_flag ){

	unsigned int 	locator 		= 0;
	unsigned char 	midiCH 			= 0,
					controller		= 0,
					value	 		= 0,
					which			= CURRENT;


	// The default: track is not sending any MIDICC data. 
	if ( target_page->Track[row]->attribute[ATTR_MIDICC] == MIDICC_NONE ){
		return;
	}

	// Calculate current column: value in interval [1,16]
	locator = target_page->Track[row]->attribute[ATTR_LOCATOR];		

	// MCC data - if the bit on the current track is set play it
	if ( MCCpattern & (1 << row) ){

		// Account the play load 
		PLAY_LOAD ++;

		// Check the groove indicator. If necessary, look at next column for
		// the attributes relevant for the note OFF. So virtually advance the locator.
		if ( groove_indicator == NEXT ){

			// NEXT COLUMN playbits (i.e. steps pulled forward): added to the bitpattern
			locator = get_next_tracklocator( target_page->Track[row], locator );
			which = NEXT;
		}

		// Assemble the MCC message: channel, controller, amount
		midiCH 		= 	target_page->Track[row]->attribute[ATTR_MIDICH];
		controller 	= 	target_page->Track[row]->attribute[ATTR_MIDICC];

		value 		= 	normalize( 		
							// target_page->Step[row][locator-1]->attribute[ATTR_MIDICC]
			   				( (	target_page->Step[row][locator-1]->attribute[ATTR_MIDICC]
								* Track_MCC_factor[target_page->Track[row]->MCC_factor] ) / 25 )

						+ 	target_page->Track[row]->event_MCC,
							0, 127 );

		// SEND MCC
		MIDI_send(	MIDI_CC, midiCH, controller, value );

	} // MCCpattern has a bit in this row

}






//
// PLAY the selected target_page at this point in time
// 
// Play a row in the page - the multiplier flag used later in play_row_ON
// ..indicates how many times a track has been playing so far..

// NOW only gets called on the beat, and needs to enque everything nicely.
void PLAYER_play_row( 	Pagestruct* 	target_page, 
						unsigned char 	target_row,
						unsigned char 	multiplier_flag,
						unsigned char 	col, 
						unsigned char 	which_col	){
	
	unsigned int bitactivity = 0;
	unsigned int MCCactivity = 0;

// d_iag_printf( "_play_row() working on row:%d LOC:%d TTC:%d\n", 
//		target_row, target_page->Track[target_row]->attribute[ATTR_LOCATOR], 
//		target_page->Track[target_row]->TTC );

	//
	// EVENT PROCESSING
	// Event operations from CURRENT step - only if the step is playing but before EFF
	if (	( which_col == CURRENT )
		&&	( Step_get_status( target_page->Step[target_row][col-1], STEPSTAT_EVENT ) == ON )
		){
	
		// Apply the step event instruction to the given track
		perform_step_event( target_page->Step[target_row][col-1], 
							target_page->Track[target_row],
							target_page,
							target_row );
	}


	/////////////////
	// MCC PROCESSING -> enqueue Step MCC (better to do before note is played)
	MCCactivity = 
		Step_get_MCC_ACTIVITY( target_page->Step[target_row][col-1] ) << target_row;	

	// If MCC is active in step	
	if ( MCCactivity != 0 ){
		
		// Apply mutepattern: i.e. dont play muted tracks
		MCCactivity = MCCactivity & (~target_page->trackMutepattern);
	
		// Apply solopattern: i.e. only play soloed tracks
		if (target_page->trackSolopattern != 0) {		
			MCCactivity = MCCactivity & target_page->trackSolopattern;
		}

		// MCC is still active after mute and solo operations
		if ( MCCactivity != 0 ){

			// MCC sent for enqueing
			play_row_MCC(	target_page, 
							target_row, 
							MCCactivity, 
							which_col, 
							target_page->Track[target_row]->TTC,
							multiplier_flag );
		}
	} // MCC is active in step


	//////////////////
	// NOTE PROCESSING 	-> enqueue Step NOTE
	bitactivity = 
		Step_get_status( target_page->Step[target_row][col-1], STEPSTAT_TOGGLE ) << target_row;

	// If note is active in step
	if ( bitactivity != 0){

		// Apply mutepattern: i.e. dont play muted tracks
		bitactivity = bitactivity & (~target_page->trackMutepattern);
			
		// Apply solopattern: i.e. only play soloed tracks
		if (target_page->trackSolopattern != 0) {
			bitactivity = bitactivity & target_page->trackSolopattern;
		}

		// NOTE sent for enqueing - even if it may not be active - may need its EFF input
		play_row_ON( 	target_page, 
						target_row, 
						bitactivity, 
						col,
						which_col,
						target_page->Track[target_row]->TTC, 
						multiplier_flag );

	} // note is active in step


	//////////////////
	// PLAY MIDI QUEUE - otherwise events wait in queue till next timer interrupt
	play_MIDI_queue( MIDI_timestamp );
}




// return the status of the midi monitor responsible for the track
unsigned char get_MIDI_MONITOR_for_track( 	Pagestruct* target_page,
											unsigned char track ){

		if ( target_page->Track[track]->attribute[ATTR_MIDICH] < 16 ){

			return MIDI_LOAD_MONITOR1;
		}
		else{

			return MIDI_LOAD_MONITOR2;
		}
}





// Play the track (track is a chain of 1 or more rows) currently playing in given row
void play_track( Pagestruct* target_page, unsigned char row ){

	unsigned char i = 0;
	// Temp storage of midi monitor 1 or 2
	unsigned char 	MIDI_LOAD_MONITOR = 0;
	Trackstruct* target_track = target_page->Track[row];
	
//if ( row > 7 )
//d_iag_printf( "Play_track: track:%d\n", row );

	// Check whether we need to tempo skip..
	switch( target_track->attribute[ATTR_TEMPOMUL_SKIP] >> 4 ){

		// Play the track - no skipping, or skipped and time to play it
		case 0:
		
			// TEMPOMUL iterator - Execute as often as indicated by tempo multiplier
			for ( i=0; i < target_track->attribute[ATTR_TEMPOMUL]; i++ ){
	
//if ( row > 7 )
//d_iag_printf( " play_track: operate on track:%d LOC:%d TTC:%d\n", 
//	row_of_track( target_page, target_track ), target_track->attribute[ATTR_LOCATOR], target_track->TTC );

				// Always stay on top of the current track, following chains
				target_track = advance_track_TTC_new ( target_page, target_track );
				
//if ( row > 7 )
//d_iag_printf( " play_track: adv_TTC result: track:%d LOC:%d TTC:%d\n",  
//	row_of_track( target_page, target_track ), target_track->attribute[ATTR_LOCATOR], target_track->TTC );

				// Break immediately on NULL return
				if ( target_track == NULL ) { 

//if ( row > 7 )
//d_iag_printf( " play_track: adv_TTC result: NULL\n" );

					return;
				}
								
				// Playload check
				if ( PLAY_LOAD > PLAY_LIMIT ){
	
					CPU_LOAD_MONITOR 	= MIR_RED;		
					PLAY_LOAD = 0;
	
					// Set the alarm for CPU load green maker		
					cyg_alarm_initialize(	CPU_overloadAlarm_hdl, 
											cyg_current_time() + CPU_RED_INTERVAL,
											0 );			
				}

				// Decide on the MIDI monitor to check 
				MIDI_LOAD_MONITOR = get_MIDI_MONITOR_for_track( target_page, row );
	
				// Avoid sequencer oveerflow - do not play row if we are in the red
				if (	( MIDI_LOAD_MONITOR  != MIR_RED )
					&&	( CPU_LOAD_MONITOR   != MIR_RED )
					){

					// Play the row on the full beat of the track, if it is playing
					if (	( target_track->TTC == 1 )
						&&	( target_track->attribute[ATTR_LOCATOR] != 0 )
						){
		
						// Extract the row of the target track
						row = row_of_track( target_page, target_track );
		
						// Play row information as MIDI data - enqueue as needed
						// page, track, multiplier flag, column to be played, CURRENT or NEXT
						// Current Column - steps: straight, pushed
						PLAYER_play_row( 	
							target_page, row, i, 
							target_track->attribute[ATTR_LOCATOR],
							CURRENT );
					
						// Next Column - steps: pulled
						PLAYER_play_row( 	
							target_page, row, i, 
							get_next_tracklocator( 	target_track, 
													target_track->attribute[ATTR_LOCATOR] ),
							NEXT );
					}
				}					

			} // TEMPOMUL iterator

			// Refill the upper nibble of the skipper indicator, counting the skips
			if ( (target_track->attribute[ATTR_TEMPOMUL_SKIP] & 0x0F) != 0 ){

				target_track->attribute[ATTR_TEMPOMUL_SKIP] =
						(target_track->attribute[ATTR_TEMPOMUL_SKIP] & 0x0F)
					|	((target_track->attribute[ATTR_TEMPOMUL_SKIP] & 0x0F) << 4) ;	
										
			}
			break;
			

		// SKIP the track for now, just decrement the skip counter
		default:

			// Decrement the value of the upper nibble in the SKIP counter
			target_track->attribute[ATTR_TEMPOMUL_SKIP] =
				// Decremented upper nibble
					(((target_track->attribute[ATTR_TEMPOMUL_SKIP] >> 4) -1) << 4)
				// Lower nibble stays the same
				|	( target_track->attribute[ATTR_TEMPOMUL_SKIP] & 0x0F);
			break;				
			
	}// switch on the track skip attribute
}




// Plays the given target page
void PLAYER_play_page_new	( 		Pagestruct* 	target_page,
									unsigned char 	in_TTC_ABS_VALUE ) {
							
	unsigned int 	row		= 0;

	// GENERATE TRACK PLAYLIST
	for (row = 0; row < MATRIX_NROF_ROWS; row ++ ){
	
		// Copy the track poiner into the playlist
		if ( target_page->Track[row]->chain_data[PLAY] == target_page->Track[row] ){

			Track_playlist[row] = target_page->Track[row];
//if ( row > 7 )
//d_iag_printf( "play_page: playlisted track:%d\n", row );

		}
		else{
			
			// Initialize the playlist position
			Track_playlist[row] = NULL;
		}
	}

	// PLAY THE PLAYLIST
	for ( row = 0; row < MATRIX_NROF_ROWS; row ++ ){
	
		if ( Track_playlist[row] != NULL ){
		
			play_track( target_page, row );
		}
	}

	// After playing the last row in page -> clear the EFF values in page.
	// ..they will be computed again in the next round.
	// VEL
	target_page->EFF_pool[ ATTR_VELOCITY ] = 0;		
	// PIT
	target_page->EFF_pool[ ATTR_PITCH 	]  = 0;
	// LEN
	target_page->EFF_pool[ ATTR_LENGTH 	]  = 0;
}



// Preview function for a particular track - used to pre-audit a track's base
void PLAYER_preview_track( 	Pagestruct* target_page,
							unsigned char row 			){

	unsigned char midiCH	=	target_page->Track[row]->attribute[ATTR_MIDICH];

	unsigned char pitch		= normalize( 
					  			STEP_DEF_PITCH +
					  			target_page->Track[row]->attribute[ATTR_PITCH] + 
								target_page->attribute[ATTR_PITCH] - MIDDLE_C, 

					  			0, 127 );
	
	unsigned char velocity	= normalize(
								STEP_DEF_VELOCITY +
							  	target_page->Track[row]->attribute[ATTR_VELOCITY] +
							  	target_page->attribute[ATTR_VELOCITY], 

							  	0, 127);
	
	// NOTE OFF build		
	MIDI_OFF_build_trackpreview( midiCH, pitch );

	// NOTE ON send
	MIDI_send(	MIDI_NOTE, midiCH, pitch, velocity );
}


 
// Preview function for a particular step - used to pre-audit a step - needs chord support
void PLAYER_preview_step( 	Pagestruct* target_page, 
							unsigned char row, 
							unsigned char col			){

	// NOTE sent for enqueing - even if it may not be active - may need its EFF input
	play_row_ON( 	target_page, 
					row, 
					(1 << row), 
					col+1,
					CURRENT,
					target_page->Track[row]->TTC, 
					1 );

	//////////////////
	// PLAY MIDI QUEUE - otherwise events wait in queue till next timer interrupt
	play_MIDI_queue( MIDI_timestamp+1 );
}


