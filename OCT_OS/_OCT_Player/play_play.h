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





Trackstruct* advance_track_TTC( 				Pagestruct* target_page,
												Trackstruct* target_track );

extern unsigned char how_selected_in_scale( 	Pagestruct* target_page,
												unsigned char target_note,
												unsigned char scaleNdx 	);

extern unsigned char how_selected_in_current_scale(
												Pagestruct* target_page,
												unsigned char target_note 	);

extern unsigned short extract_current_scale( 	Pagestruct* target_page );

extern void 			play_MIDI_queue( 		unsigned int in_G_MIDI_timestamp );
extern unsigned char 	map_to_14bit( 			unsigned char _8bit, unsigned char order );
extern void 	PLAYER_play_track( Pagestruct* target_page, unsigned char row );


#include "perform_step_event.h"



// Basically map a value in [0..16] to [0..12] so it can be used in the step length context.
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


//_______________________________________________________________________________________
//
// Build MIDI OFF event for given track attributes and line it up in the MIDI queue
void MIDI_OFF_build_trackpreview( 	unsigned char midiCH,
									unsigned char pitch 	){

	unsigned int OFF_timestamp = 0;

	// Get an empty MIDI event from the MIDI stack
	MIDIeventstruct* MIDI_OFF_event = 	MIDI_stack_pop();

	// Compute the timestamp for playing the MIDI OFF. Current time + length.
	OFF_timestamp 	= 	G_MIDI_timestamp + STEP_DEF_LENGTH;

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



//_______________________________________________________________________________________
//
// Enqueue MIDI note given by: channel, pitch, velocity, length (when)
void MIDI_NOTE_new( 	unsigned char midiCH,
						unsigned char pitch,
						unsigned char velocity,
						unsigned int  when	){

	// Compute the timestamp of the event from current G_MIDI_timestamp and given length
	unsigned int event_timestamp = G_MIDI_timestamp + when;

	// This is the legato case - or when passing MIDI notes through
	if ( when == def_TIMESTAMP_MAX )	event_timestamp	= 	def_TIMESTAMP_MAX;

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



// Enqueue MIDI CC message, defined by channel, controller, val1 and val2
// The when parameter is an offset into the future from current timestamp
void MIDI_CC_new( 	unsigned char midiCH,
					unsigned int val1,
					unsigned int val2,
					unsigned int when		){

	// Compute the timestamp of the event from current G_MIDI_timestamp and given length
	unsigned int event_timestamp = G_MIDI_timestamp + when;

	// This is the legato case - or when passing MIDI notes through
	if ( when == def_TIMESTAMP_MAX )	event_timestamp	= 	def_TIMESTAMP_MAX;

	// Get an empty MIDI event from the MIDI stack
	MIDIeventstruct* cc_event = 	MIDI_stack_pop();

	// Determine the correct type of the message and assemble
	switch( val1 ){

		case MIDI_BENDER:
			// Fill the event data for MIDI CC message
//d_iag_printf( "new BEN:%d ch:%d val2:%d\n", MIDI_PRESSURE, midiCH, val2 );
			MIDIevent_fill( 	cc_event,
								event_timestamp,
								MIDI_BENDER,
								midiCH,
								(val2 & 0x7F), 				// LSB 7 bits, data byte 1.
								((val2 >> 7) & 0x7F) ); 	// MSB 7 bits, data byte 2.
			break;

		case MIDI_PRESSURE:
			// Fill the event data for MIDI CC message
			MIDIevent_fill( 	cc_event,
								event_timestamp,
								MIDI_PRESSURE,
								midiCH,
								val2,
								0 );
			break;

		default:
			// Fill the event data for MIDI CC message
			MIDIevent_fill( 	cc_event,
								event_timestamp,
								MIDI_CC,
								midiCH,
								val1,
								val2 );
			break;
	}

	// Line up the event in the MIDI Queue
	MIDI_queue_insert( cc_event );
}




//_______________________________________________________________________________________
//
// Build MIDI OFF event with the given coordinates
void MIDI_OFF_build_new(	unsigned char midiCH,
							unsigned char pitch,
							unsigned int length	){

	unsigned int OFF_timestamp = 0;

	// Get an empty MIDI event from the MIDI stack
	MIDIeventstruct* MIDI_OFF_event = 	MIDI_stack_pop();

	// Compute the timestamp for playing the MIDI OFF. Current time + length.
	if ( length == def_TIMESTAMP_MAX ){
		OFF_timestamp	= 	def_TIMESTAMP_MAX;
	}
	else{
		OFF_timestamp 	= 	G_MIDI_timestamp + length;
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



//_______________________________________________________________________________________
//
// Scale the track clock into the provided value. Scales up (POS) and down (NEG)
unsigned int scaleToTrackClock( 	unsigned int start_offset,
									Pagestruct * target_page,
									Trackstruct * target_track,
									unsigned char scaleFlag 	){

	unsigned char clock_multiplier 	= 0;
	unsigned char clock_divisor		= 0;

	unsigned char attr_TEMPOMUL = target_track->attr_TEMPOMUL;
	unsigned char attr_TEMPOMUL_SKIP = target_track->attr_TEMPOMUL_SKIP;

	#ifdef FEATURE_ENABLE_DICE
	unsigned char row = row_of_track( target_page, target_track );
	DiceOffsetClock dice_clock = dice_apply_clock_offset( target_page, row );
	attr_TEMPOMUL = dice_clock.attr_TEMPOMUL;
	attr_TEMPOMUL_SKIP = dice_clock.attr_TEMPOMUL_SKIP;
	#endif

	// EXTRACT THE TRACK CLOCK DATA
	// Check the clock multiplier / divisor and extract values
	switch( attr_TEMPOMUL_SKIP & 0x0F ){

		// No divisor, only a multiplier value
		case 0:
			// Fill in the multiplier from the track
			clock_multiplier = attr_TEMPOMUL;
			break;

		default:
			// Fill in with the real divisor value
			clock_divisor = (attr_TEMPOMUL_SKIP & 0x0F) + 1;
			break;
	}

	// Filter the triplet and inverse triplet cases: play as normal
	if (	( clock_multiplier == 15 )
		||	( clock_divisor == 26 )
		){
		clock_multiplier = 1;
		clock_divisor = 0;
	}

// d_iag_printf( "   in:%d div:%d mul:%d ", start_offset, clock_divisor, clock_multiplier );

	// FACTOR TRACK CLOCK INTO THE VALUE
	// Operation depends on the scale flag..
	switch( scaleFlag ){
		case POS:
			// Multiplier present therefore multiplier only
			if ( clock_divisor == 0 ){
				start_offset /= clock_multiplier;
			}
			// Divisor present, scale offset down accordingly
			else{
				start_offset *= clock_divisor;
			}
			break;

		case NEG:
			// Multiplier present therefore multiplier only
			if ( clock_divisor == 0 ){
				start_offset *= clock_multiplier;
			}
			// Divisor present, scale offset down accordingly
			else{
				start_offset /= clock_divisor;
			}
			break;
	}

// d_iag_printf( "out:%d\n", start_offset );

	return start_offset;
}


// Computes the pitch value that a hyperstep injects as base pitch into a track
unsigned char getHyperstepPitch( 	Pagestruct* pagePt,
									unsigned char row,
									unsigned char EFF_pool_PIT ){

	Trackstruct* trackPt 		= pagePt->Track[row];
	unsigned char hyperstepRow	= trackPt->hyper >> 4;
	unsigned char hyperstepCol	= trackPt->hyper & 0x0F;
	Stepstruct* stepPt			= pagePt->Step[hyperstepRow][hyperstepCol];
	unsigned char runningPitch 	= 0;
	unsigned char temp 			= 0;

	// Once hyperstep identified, the track we care about is the hyperstep track
	trackPt = pagePt->Track[hyperstepRow];

	// --- Compute the hyperpitch for the identified hyperstep ---
	// TRACK: 	Basic pitch computation from track of the hyperstep
	runningPitch 	+= 	( 	trackPt->attr_PIT
						+	trackPt->scale_pitch_offset
						+	trackPt->lead_pitch_offset
						);

	// EFF: 	Add the effector pool
	runningPitch	+= 	EFF_pool_PIT;

	// STEP: 	Add the factorized step pitch offset. Temp is syntactic sugar.
	temp 			= 	trackPt->PIT_factor + trackPt->event_offset[ATTR_PITCH];
	runningPitch	+= 	( 	( stepPt->attr_PIT * Track_PIT_factor[ temp ])
							/ PIT_FACTOR_NEUTRAL_VALUE );

	//runningPitch	+= 	stepPt->attr_PIT;

	// TOTAL: 	Normalize the pitch value into the valid interval
	runningPitch 	= normalize( runningPitch, 0, 127 );

	// Close and return the value
	return runningPitch;
}


// Computes the pitch value that a hyperstep injects as base pitch into a track
unsigned char getHyperstepVelocity( 	Pagestruct* pagePt,
										unsigned char row,
										unsigned char EFF_pool_VEL ){

	Trackstruct* trackPt 		= pagePt->Track[row];
	unsigned char hyperstepRow	= trackPt->hyper >> 4;
	unsigned char hyperstepCol	= trackPt->hyper & 0x0F;
	Stepstruct* stepPt			= pagePt->Step[hyperstepRow][hyperstepCol];
	unsigned char runningVelocity = 0;
	unsigned char temp 			= 0;

	// Once hyperstep identified, the track we care about is the hyperstep track
	trackPt = pagePt->Track[hyperstepRow];

	// --- Compute the hypervelocity for the identified hyperstep ---
	// TRACK:	Base track velocity as starting point
	runningVelocity	+=	trackPt->attr_VEL;

	// EFF:		Add the effector pool
	runningVelocity	+=	EFF_pool_VEL;

	// STEP:	Add the factorized step velocity offset. Temp is syntactic sugar.
	temp			= 	trackPt->VEL_factor + trackPt->event_offset[ATTR_VELOCITY];
	runningVelocity	+=	(	( stepPt->attr_VEL * Track_VEL_factor[ temp ])
							/ VEL_FACTOR_NEUTRAL_VALUE );

	// TOTAL:	Normalize the velocity value into the valid interval
	runningVelocity =	normalize( runningVelocity, 0, 127 );

	// Close and return the value
	return runningVelocity;
}




//_______________________________________________________________________________________
//
// Play the NOTE ON events in the given row of target_page
void play_row_ON( 	Pagestruct* 	target_page,
					const card8 	phys_row, 		// The physical row to be played
					unsigned int 	bitactivity,
					const card8 	locator,
					unsigned int 	which_col,
					unsigned int 	in_G_TTC_abs_value,
					unsigned char 	multiplier_flag )
{
	Stepstruct*		stepPt 			= target_page->Step[phys_row][locator-1];

	if( 	( ( which_col == CURRENT )
		&&	( stepPt->attr_STA < STEP_DEF_START ) )
		||	( ( which_col == NEXT )
		&&	( stepPt->attr_STA >= STEP_DEF_START ) ) ) {
		// STA outside current play column
		return;
	}

	unsigned char 	head_row;
	signed int 		temp;
	unsigned char 	i,
					j,
					pitch_initial,
					trackBasePitch,
					trackBaseVelocity,
					midiCH;
	unsigned int	start_offset	= 0;
	signed char 	EFF_pool_VEL 	= 0,
					EFF_pool_PIT	= 0,
					EFF_pool_LEN	= 0;

	card8 			phraseNum 		= stepPt->phrase_num;

	// Get step strum level, in range [0, 18]. Value 9 is the neutral value.
	card8 			strumLevel  	= ( target_page->Step[phys_row][locator-1]->chord_data & 0xF800 ) >> 11;

	signed char dice_pitch_offset 	= 0;
	signed char dice_velocity_offset= 0;
	signed int dice_length_offset	= 0;
	signed char dice_start_offset	= 0;

	unsigned char attr_TEMPOMUL = target_page->Track[phys_row]->attr_TEMPOMUL;
	unsigned char attr_TEMPOMUL_SKIP = target_page->Track[phys_row]->attr_TEMPOMUL_SKIP;
	
	// Do not play track if its locator is at 0
	if ( locator == 0 ) {
		return;
	}

	// Do not play track if muted - this is already taken care of in terms of set play bits..
	// ..but need to do it again so we can mute the EFF function of an empty track.
	if ( ( target_page->trackMutepattern & (1 << phys_row) ) != 0 ) {
		return;
	}


	// Decide which head row to use - relevant for track chaining, where head row may be different
	if ( target_page->CHAINS_PLAY_HEAD == TRUE ){
		head_row = row_of_track( target_page, target_page->Track[phys_row]->chain_data[HEAD] );
	}
	else {
		head_row = phys_row;
	}

	#ifdef FEATURE_ENABLE_DICE
	DiceOffsetClock dice_clock = dice_apply_clock_offset( target_page, phys_row );
	attr_TEMPOMUL = dice_clock.attr_TEMPOMUL;
	attr_TEMPOMUL_SKIP = dice_clock.attr_TEMPOMUL_SKIP;

	#ifdef NEMO
	dice_pitch_offset = dice_attr_flow_offset( target_page, NEMO_ATTR_PITCH, locator );
	dice_velocity_offset 	= dice_attr_flow_offset( target_page, NEMO_ATTR_VELOCITY, locator );
	dice_length_offset		= dice_attr_flow_offset( target_page, NEMO_ATTR_LENGTH, locator );
	dice_start_offset	 	= dice_attr_flow_offset( target_page, NEMO_ATTR_START, locator );
	#else
	dice_pitch_offset = dice_attr_flow_offset( target_page, ATTR_PITCH, locator );
	dice_velocity_offset 	= dice_attr_flow_offset( target_page, ATTR_VELOCITY, locator );
	dice_length_offset		= dice_attr_flow_offset( target_page, ATTR_LENGTH, locator );
	dice_start_offset	 	= dice_attr_flow_offset( target_page, ATTR_START, locator );
	#endif
	#endif

	// Only in CURRENT call mode
	// -> EFFector
	// -> HYPERSTEP

	if ((which_col == CURRENT)) {

		// EFFECTOR
		// outsourced
		#include "play_effector.h"


		// HYPERSTEP
		// check the hyper status and activate / retrigger tracks
		if ( stepPt->hyperTrack_ndx < 10 ){

			// Make the hyped track be its own PLAY (to overcome post-assignment state)
			if ( target_page->Track[stepPt->hyperTrack_ndx]->chain_data[PLAY] == NULL ){
				target_page->Track[stepPt->hyperTrack_ndx]->chain_data[PLAY] =
					target_page->Track[stepPt->hyperTrack_ndx];
			}

			// Fire up the hyped track after setting it all to the start:
			// Set all track params to start condition
			target_page->Track[stepPt->hyperTrack_ndx]->attr_LOCATOR 	= 0;
			target_page->Track[stepPt->hyperTrack_ndx]->TTC 			= 0;
			target_page->Track[stepPt->hyperTrack_ndx]->frame_ndx 		= 0;
			target_page->Track[stepPt->hyperTrack_ndx]->gatePosition 	= 0;
			// Initial fire of the start track
			PLAYER_play_track( target_page, stepPt->hyperTrack_ndx ) ;

			// Since hyper step is only a track trigger, we can return. No note play necessary.
			if ( HYPERSTEPS_PLAY_NOTES ){
				return;
			}
		} // Track is hyped
	}

	// Is the actual Step playing ?? - This check is needed here and not earlier,
	// ..otherwise the EFF pool will not be filled

	if (( bitactivity & (1 << phys_row) ) == 0 ) {
		return;
	}




	//
	// NOTE ON data gathering
	//

	// CHANNEL:
	midiCH = target_page->Track[head_row]->attr_MCH
			+ target_page->Track[head_row]->event_offset[ATTR_MIDICH];


	// Chord pattern, size and offset init. STEP chord overrules TRACK chord.
	#ifdef FEATURE_ENABLE_CHORD_OCTAVE
	card8 stepAuxNoteCt 		= get_chord_cardinality( stepPt, CHORD_OCTAVE_ALL );
	#else
	card16 step_chord_pattern 	= (stepPt->chord_data & 0x7FF);
	card8 stepAuxNoteCt 		= my_bit_cardinality( step_chord_pattern );
	#endif
	card8 stepPolyphony 		= (stepPt->chord_up >> 29);
	booln rollBo      			= stepPt->attr_STATUS & (1 << 7);


	//---------------------------------------------------------------------------------------
	// PITCH-INITIAL
	//---------------------------------------------------------------------------------------

	// Hyped tracks take over the pitch of their respective hyper-step.
	// Check if the track is hyped and decide on the track base pitch.
	if ( target_page->Track[phys_row]->hyper < 0xF0 ){
		trackBasePitch = getHyperstepPitch( target_page, phys_row, EFF_pool_PIT );
	}
	else{
		trackBasePitch = target_page->Track[head_row]->attr_PIT;
	}


	// PITCH: Differentiate between playing the pitch offsets or the natural pitches
	// This computation of the initial pitch should suffice. FTS applied in chord loop
	#ifdef EVENTS_FACTORIZED
	//________FACTORIZED version
	// PIT factor computation for the step pitch offset
	pitch_initial	= normalize(
			// Modulators
			  EFF_pool_PIT
			// Dice - flow shape pitch
			+ dice_pitch_offset
			// Step - factorized pitch attribute value
			+ ( stepPt->attr_PIT
					*  Track_PIT_factor[	target_page->Track[head_row]->PIT_factor
											+ target_page->Track[phys_row]->event_offset[ATTR_PITCH] ]
				/ PIT_FACTOR_NEUTRAL_VALUE )

			// Track
		    + trackBasePitch // target_page->Track[head_row]->attr_PIT
			+ target_page->Track[head_row]->scale_pitch_offset
			+ target_page->Track[head_row]->lead_pitch_offset

			// Page
			+ ( target_page->attr_PIT - MIDDLE_C ),

		  	0, 127);
	#endif // EVENTS_FACTORIZED

	#ifdef EVENTS_ABSOLUTE
	//________ABSOLUTE version
	pitch_initial	= normalize(
			// Modulators
			  EFF_pool_PIT
			// Dice flow shape pitch
			+ dice_pitch_offset
			// + target_page->Track[row]->event_PIT
			+ target_page->Track[phys_row]->event_offset[ATTR_PITCH]

			// Step
		  	// + target_page->Step [row][locator-1]->attr_PIT
			// Step - factorized pitch attribute value
			+ ( stepPt->attr_PIT
					*  Track_PIT_factor[ target_page->Track[head_row]->PIT_factor ]
				/ PIT_FACTOR_NEUTRAL_VALUE )

			// Track
		  	+ trackBasePitch // target_page->Track[head_row]->attr_PIT
			+ target_page->Track[head_row]->scale_pitch_offset
			+ target_page->Track[head_row]->lead_pitch_offset

			// Page
			+ ( target_page->attr_PIT - MIDDLE_C ),

		  	0, 127);
	#endif // EVENTS_ABSOLUTE



	//---------------------------------------------------------------------------------------
	// START OFFSET
	//---------------------------------------------------------------------------------------

	// START Offset computation - depending on the column we are looking at:
	switch( which_col ){

		case CURRENT:
			// Neutral and pushed start values

			// 1 / 6: Term simplification
			start_offset = stepPt->attr_STA - STEP_DEF_START;

			// Dice flow shape start
			start_offset += dice_start_offset;

			// 2 / 6: Apply event offset to track STA factor and store the value
			temp = target_page->Track[head_row]->STA_factor
							+ target_page->Track[phys_row]->event_offset[ATTR_START];

			// 3 / 6: Apply the event'ed track factor to the start offset
			start_offset = (start_offset * Track_STA_factor[ temp ] );
			start_offset = (start_offset / STA_FACTOR_NEUTRAL_VALUE );

			// 4 / 6: Apply the 909 groove offset (track native)
			start_offset += get_track_GRV_offset( target_page, head_row, locator-1, CURRENT );

			// 5 / 6: Apply the track clock multiplier
			start_offset = scaleToTrackClock( start_offset, target_page, target_page->Track[phys_row], POS );

			// 6 / 6: Normalize the value of the start offset
			start_offset = normalize( start_offset, 0, 192 );
			break;


		case NEXT:
			// Consider all the pulled start value

			// 1 / X: Term simplification
			start_offset = (STEP_DEF_START - stepPt->attr_STA);

			// Dice flow shape start
			start_offset += dice_start_offset;

			// 2 / X: Apply event offset to track STA factor and store the value
			temp = target_page->Track[head_row]->STA_factor
							+ target_page->Track[phys_row]->event_offset[ATTR_START];

			// 3 / X: Apply the event'ed track factor to the start offset
			start_offset = (start_offset * Track_STA_factor[ temp ] );
			start_offset = (start_offset / STA_FACTOR_NEUTRAL_VALUE );

			// 4 / X: Apply the 909 groove offset (track native)
			start_offset += get_track_GRV_offset( target_page, head_row, locator-1, CURRENT );

			// 5 / X: Apply the track clock multiplier
			start_offset = scaleToTrackClock( start_offset, target_page, target_page->Track[phys_row], POS );

			// 6 / X: Normalize the value of the start offset. Scale the 12/192 appropriately
			start_offset = normalize( start_offset, 0, 12 );
			temp = scaleToTrackClock( 12, target_page, target_page->Track[phys_row], POS );
			start_offset = temp - start_offset;
			break;
	}



	// Decide whether to play step or track chord.
	if ( 	   (stepPolyphony == 0)
			&& (stepAuxNoteCt == 0)
			&& (phraseNum == 0)
			&& (strumLevel == 9) ) {

		card8 local_chordSize = Track_get_MISC( target_page->Track[phys_row], CHORD_BIT );
		int8  chord_offset = 0;
		#include "Track_chord_player.h"
	}

	else{
		// STEP CHORD scenario - step has auxiliary pitches
		#include "Step_chord_player.h"
	}
}



// Generates the intermediate PRESSURE messages for given params and enqueues them
void send_intermediate_PRESSURE( 	unsigned char channel,
									unsigned short last_val,
									unsigned short next_val,
									unsigned char resolution ){

	unsigned char i=0;
	signed char offset = 0;
	unsigned char when = 0;
	unsigned short amount = 0;

	// Size of the CC difference in each interval
	offset 	= ( (next_val - last_val) / resolution );

	// Buffer the value to be increased
	amount = last_val;

	// Generate the intermediate CC messages
	for ( i=0; i < resolution-1; i++ ){

		// Update the CC value
		amount += offset;

		// Fetch delay from global table
		when = CC_resolution_delay[ resolution ][ i ];

		// Enqueue the CC message
		MIDI_CC_new( channel, MIDI_PRESSURE, amount, when );
	}
}



// Generates the intermediate PRESSURE messages for given params and enqueues them
void send_intermediate_BENDER( 	unsigned char channel,
								unsigned short last_val,
								unsigned short next_val,
								unsigned char resolution ){

	unsigned char i=0;
	int offset = 0;
	unsigned char when = 0;
	unsigned short amount = 0;

	// Size of the CC difference in each interval
	offset 	= ( (next_val - last_val) / resolution );

	// Buffer the value to be increased
	amount = last_val;

//d_iag_printf( "BEN ch:%d last:%d next:%d off:%d\n", channel, last_val, next_val, offset );

	// Generate the intermediate CC messages
	for ( i=0; i < resolution-1; i++ ){

		// Update the CC value
		amount += offset;

		// Fetch delay from global table
		when = CC_resolution_delay[ resolution ][ i ];

//d_iag_printf( "    ch:%d BEN:%d amt:%d whe:%d\n", channel, MIDI_BENDER, amount, when );

		// Enqueue the CC message
		MIDI_CC_new( channel, MIDI_BENDER, amount, when );
	}
}



// Generates the intermediate CC messages for given params and enqueues them
void send_intermediate_CC( 	signed char controller,
							unsigned char channel,
							unsigned short last_CC,
							unsigned short next_CC,
							Trackstruct* target_track ){
//							unsigned char resolution ){

	unsigned char i=0;
	signed char CC_offset = 0;
	unsigned char when = 0;
	unsigned short amount = 0;
	unsigned char resolution = 0;

	// Fetch resolution
	resolution = target_track->CC_resolution;

	// Size of the CC difference in each interval
	CC_offset 	= ( (next_CC - last_CC) / resolution );

	// Buffer the value to be increased
	amount = last_CC;

	// Generate the intermediate CC messages
	for ( i=0; i < resolution-1; i++ ){

		// Update the CC value
		amount += CC_offset;

		// Fetch delay from global table
		when = CC_resolution_delay[ resolution ][ i ];

		// Account for clock multiplier
		switch( target_track->attr_TEMPOMUL_SKIP & 0x0F ){

			// Track equals or above normal speed
			case 0:
				when = when / ( target_track->attr_TEMPOMUL );
				break;

			// Track below normal speed
			default:
				when = when * ( (target_track->attr_TEMPOMUL_SKIP & 0x0F) + 1);
				break;
		}

		// Enqueue the CC message
		MIDI_CC_new( channel, controller, amount, when );
	}
}




//_______________________________________________________________________________________
//
// Play the MCC events in the given row
unsigned int play_row_MCC( 	Pagestruct* target_page,
							unsigned char phys_row,
							unsigned int MCCpattern,
							unsigned int lookahead_indicator,
							unsigned int in_G_TTC_abs_value,
							unsigned char multiplier_flag,
							unsigned int last_CC 				){

	unsigned int 	locator 		= 0;
	unsigned char 	midiCH 			= 0,
					which			= CURRENT;
	signed int		value	 		= 0;


	unsigned char	EFF_pool_MCC	= 0;
	signed char 	controller		= 0;		// May also be bender (-2), or pressure (-3), hence signed

	unsigned char 	head_row = 0;

	// Decide which head row to use - relevant for track chaining
	if ( target_page->CHAINS_PLAY_HEAD == TRUE ){
		head_row = row_of_track( target_page, target_page->Track[phys_row]->chain_data[HEAD] );
	}
	else {
		head_row = phys_row;
	}

	// The default: track is not sending any MIDICC data.
	controller = target_page->Track[head_row]->attr_MCC;
	if ( controller == MIDICC_NONE ) {
		return MIDICC_NONE;
	}

	// Calculate current column: value in interval [1,16]
	locator = target_page->Track[phys_row]->attr_LOCATOR;

	signed char dice_midicc_offset = 0;
	#ifdef FEATURE_ENABLE_DICE
	dice_midicc_offset = dice_attr_flow_offset( target_page, ATTR_MIDICC, locator );
	#endif
	// MCC data - if the bit on the current track is set play it
	if ( ( MCCpattern & (1 << phys_row) ) ){
		// Check the groove indicator. If necessary, look at next column for
		// the attributes relevant for the note OFF. So virtually advance the locator.
		if ( lookahead_indicator == NEXT ){

			// NEXT COLUMN playbits (i.e. steps pulled forward): added to the bitpattern
			locator = get_next_tracklocator( target_page->Track[phys_row], locator );
			which = NEXT;
		}

		// Only in CURRENT mode - implement EFFector
		if ( (which == CURRENT) ){
			// EFF pool generation - attribute offset - regardless of the active steps in track
			switch( Track_get_MISC( target_page->Track[phys_row], EFF_BIT ) ){

				case SEND:
					// MIDICC
					// target_page->EFF_pool[ ATTR_MIDICC ] +=
					//	(		(unsigned char) target_page->Step[row][locator-1]->attr_MCC
					//		-	STEP_DEF_MIDICC );
					target_page->EFF_pool[ ATTR_MIDICC ] +=
						normalize(	( 	target_page->Step[phys_row][locator-1]->attr_MCC
							*  Track_MCC_factor[	target_page->Track[head_row]->MCC_factor
												+ target_page->Track[phys_row]->event_offset[ATTR_MIDICC] ]
							/ MCC_FACTOR_NEUTRAL_VALUE ), STEP_MIN_MIDICC, STEP_MAX_MIDICC );
					break;

				case RECEIVE:
					// MIDICC
					EFF_pool_MCC = target_page->EFF_pool[ ATTR_MIDICC ];
					break;

				case RECEIVESEND:
					// MIDICC
					EFF_pool_MCC = target_page->EFF_pool[ ATTR_MIDICC ];

					target_page->EFF_pool[ ATTR_MIDICC ] +=
							normalize(	( 	target_page->Step[phys_row][locator-1]->attr_MCC
							*  Track_MCC_factor[	target_page->Track[head_row]->MCC_factor
												+ target_page->Track[phys_row]->event_offset[ATTR_MIDICC] ]
							/ MCC_FACTOR_NEUTRAL_VALUE ), STEP_MIN_MIDICC, STEP_MAX_MIDICC );
					break;
			}
		}


		// Assemble the MCC message: channel, controller, amount
		midiCH 		= 	target_page->Track[head_row]->attr_MCH
						+ target_page->Track[head_row]->event_offset[ATTR_MIDICH];

		controller 	= 	target_page->Track[head_row]->attr_MCC;

		// Compute the value of the controller to be sent
		switch( target_page->Step[phys_row][locator-1]->attr_MCC ){

			case MIDICC_NONE:
				value = MIDICC_NONE;
				break;

			default:

				value =	normalize(
								 	EFF_pool_MCC + dice_midicc_offset +
					   		 		(	(	target_page->Step[phys_row][locator-1]->attr_MCC
											* Track_MCC_factor[	target_page->Track[head_row]->MCC_factor
																+ target_page->Track[phys_row]->event_offset[ATTR_MIDICC] ] )
										/ MCC_FACTOR_NEUTRAL_VALUE ),
									0, 127 );
				break;
		}

		// No CC event sent on the step, no need to continue
		if ( value == MIDICC_NONE ){
			return MIDICC_NONE;
		}
		// We are currently recording CC MIDI IN so don't play the CC back out at the same time
		else if ( G_track_rec_bit == ON && G_midi_map_controller_mode == OFF ){
			return MIDICC_NONE;
		}


		// Depending on the column that is played - play now or enqueue for later
		switch( which ){

			// Send the controller message to MIDI - immediately
			case CURRENT:

				switch( controller ){
					case MIDICC_BENDER:
						// The first data bits 0 to 6 are bits 0 to 6 of the 14-bit value.
						// The second data bits 0 to 6 are really bits 7 to 13 of the 14-bit value.
						// Step position attribute holds the second 7-bit nibble for the bender value
						MIDI_send(  MIDI_BENDER,
									midiCH,
									target_page->Step[phys_row][locator-1]->attr_POS,
									value );
						last_CC = (((value & 0x7F) << 7) | (target_page->Step[phys_row][locator-1]->attr_POS & 0x7F));
						break;

					case MIDICC_PRESSURE:
						MIDI_send(  MIDI_PRESSURE,
									midiCH,
									value,
									0 );
						last_CC = value;
						break;

					default:

// d_iag_printf( "CURRENT: ctr:%d ch:%d value:%d\n", controller, midiCH, value );
						MIDI_send(	MIDI_CC,
									midiCH,
									controller,
									value );
						last_CC = value;
						break;
				} // CURRENT controller
				break;


			// Enqueue intermediate CC messages to be sent out subsequently
			case NEXT:

				// If the previous step sent no CC, don't continue.
				if ( last_CC == MIDICC_NONE ) break;

				// If the resolution of the track does not need it, don't continue
				if ( target_page->Track[head_row]->CC_resolution < 2 ) break;

//d_iag_printf( "NEXT: ctr:%d ch:%d value:%d last_CC:%d\n", controller, midiCH, value, last_CC );

				// Generate the intermediate CC events and place them on the queue
				switch( controller ){

					case MIDICC_BENDER:
						send_intermediate_BENDER(
												midiCH,
												last_CC,
												((value & 0x7F) << 7) | (target_page->Step[phys_row][locator-1]->attr_POS & 0x7F),
												target_page->Track[head_row]->CC_resolution );
						break;

					case MIDICC_PRESSURE:
						send_intermediate_PRESSURE(
												midiCH,
												last_CC,
												value,
												target_page->Track[head_row]->CC_resolution );
						break;

					default:
						send_intermediate_CC( 	controller,
												midiCH,
												last_CC,
												value,
												target_page->Track[head_row] );
//												target_page->Track[head_row]->CC_resolution );
						break;
				}

				// Clean up any history for this current/next segment
				last_CC = MIDICC_NONE;
				break;

		} // Column selector

	} // MCCpattern has a bit in this row

	return last_CC;
}



//_______________________________________________________________________________________
//
// PLAY the selected target_page at this point in time
// Play a row in the page - the multiplier flag used later in play_row_ON
// ..indicates how many times a track has been playing so far..
// NOW only gets called on the beat, and needs to enque everything nicely.

// Returns the value of the CC played. Needed for the resolution computation.
unsigned int PLAYER_play_row( 	Pagestruct* 	target_page,
								unsigned char 	target_row,
								unsigned char 	multiplier_flag,
								unsigned char 	col,
								unsigned char 	which_col,
								unsigned int 	last_CC		){

	unsigned int bitactivity = 0;
	unsigned int MCCactivity = 0;

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
							target_row,
							col);



	}

	/////////////////
	// MCC PROCESSING -> enqueue Step MCC (before note is played)
	// Also, MCC is only played on the current column on the beat, no pulls.
	unsigned char dice_MCC = 0;
	#ifdef FEATURE_ENABLE_DICE
	Trackstruct* target_dice = throw_dice(target_page);
	if( target_dice ){
		dice_MCC = target_dice->MCC_factor;
	}
	#endif
	switch( which_col ){

		case NEXT:
			// Add the next round to compute the intermediate values based on old value
		case CURRENT:


			// Compute activity flag of the current step
			MCCactivity = ( Step_get_MCC_ACTIVITY( target_page->Step[target_row][col-1] ) << target_row ) | ( dice_MCC > 0 );

			// No need to continue if no MCC activity.
			if ( MCCactivity == 0) break;

			// Apply mutepattern: i.e. dont play muted tracks
			MCCactivity = MCCactivity & (~target_page->trackMutepattern);

			// Apply solopattern: i.e. only play soloed tracks
			if (target_page->trackSolopattern != 0) {
				MCCactivity = MCCactivity & target_page->trackSolopattern;
			}

			// MCC is still active after mute and solo operations
			if (	( dice_MCC != 0 || MCCactivity != 0 )
				){

				// MCC sent for enqueing
				last_CC = play_row_MCC(	target_page, target_row,
										MCCactivity,
										which_col,
										target_page->Track[target_row]->TTC,
										multiplier_flag,
										last_CC );
			}
			break;
	}



	//////////////////
	// NOTE PROCESSING 	-> enqueue Step NOTE by signaling its activity
	// Need to make sure we handle the chain situation where the next step is in a different row
	// Otherwise we run into the LDT bug
	bitactivity = Step_get_status( target_page->Step[target_row][col-1], STEPSTAT_TOGGLE ) << target_row;


	// If note is active in step
	if ( bitactivity != 0){

		// Apply mutepattern: i.e. dont play muted tracks
		bitactivity = bitactivity & (~target_page->trackMutepattern);

		// Apply solopattern: i.e. only play soloed tracks
		if (target_page->trackSolopattern != 0) {
			bitactivity = bitactivity & target_page->trackSolopattern;
		}
	} // note is active in step

	// NOTE sent for enqueing - even if step is not active - may need its EFF input
	play_row_ON( 	target_page, target_row, bitactivity, col,
					which_col, target_page->Track[target_row]->TTC, multiplier_flag );

	//////////////////
	// PLAY MIDI QUEUE - otherwise events wait in queue till next timer interrupt
	play_MIDI_queue( G_MIDI_timestamp );

	return last_CC;
}




//_______________________________________________________________________________________
//
// return the status of the midi monitor responsible for the track
unsigned char get_MIDI_MONITOR_for_track( Trackstruct* target_track ){

	if ( (	target_track->attr_MCH + target_track->event_offset[ATTR_MIDICH] ) < 16 ){

		return G_MIDI_load_monitor1;
	}
	else{

		return G_MIDI_load_monitor2;
	}
}


// Adjust the track multiplier of a track
void adjust_tempomul( Pagestruct* target_page, Trackstruct* target_track ){

	unsigned char i = 0;

	// Check if track is listed for a clock change, and the track is playing..
	if (	( target_page 	== track_tempo_page )
		&&	( target_track 	== track_tempo_ptr  )
		&& 	( target_track->attr_LOCATOR != 0 )
		){

// d_iag_printf( "clock switch: in_G_TTC_abs_value:%d G_LOC:%d\n", G_TTC_abs_value, G_global_locator );

			// Update the multipler and clear the buffer
			target_track->attr_TEMPOMUL 		= track_tempo_mul;

			// Update the divisor and clear the buffer
			target_track->attr_TEMPOMUL_SKIP	= track_tempo_div;

// d_iag_printf( "   row:%d loc:%d ttc:%d\n", row_of_track( target_page, target_track ), target_track->attr_LOCATOR, target_track->TTC );
// d_iag_printf( "   ..switching\n" );

			// Align the track to the current flow - in time for effector content to catch on
			// Set the TTC and LOC values
			//target_track->attr_LOCATOR 	= target_track->attr_LOCATOR;
			target_track->frame_ndx 	= target_track->attr_LOCATOR;
			target_track->TTC 			= 1;

			// Adjust the trigger sets for the new position
			for (i=1; i < MATRIX_NROF_ROWS; i++ ){

				// Empty the track trigger sets
				target_track->trigger[i] = 0;
			}

// d_iag_printf( "   row:%d loc:%d ttc:%d\n", row_of_track( target_page, target_track ), target_track->attr_LOCATOR, target_track->TTC );
// d_iag_printf( "\n" );

			// Void the clock switch buffer
			track_tempo_mul = 0;
			track_tempo_div = 0;
			track_tempo_ptr  = NULL;
			track_tempo_page = NULL;

	} // Track listed for clock change
}



// Single trigger of track, corresponding to a single TTC value
Trackstruct* TTC_trigger_track( 	Pagestruct* target_page,
									Trackstruct* target_track,
									unsigned char multiplier ){

	// Pass-through variable of data between current and next trigger round
	unsigned int last_CC = MIDICC_NONE;

	unsigned char row = 0;

	// Temp storage of midi monitor 1 or 2
	unsigned char 	MIDI_LOAD_MONITOR = 0;

//if ( row_of_track( target_page, target_track) < 3 ){
//	d_iag_printf( "TTC_trigger_track:%d LOC:%d TTC:%d\n",
//		row_of_track( target_page, target_track), target_track->attr_LOCATOR, target_track->TTC );
//}

	// Always stay on top of the current track, following chains
	target_track = advance_track_TTC ( target_page, target_track );


//if ( row_of_track( target_page, target_track) < 3 ){
//	d_iag_printf( "        new track:%d LOC:%d TTC:%d\n",
//		row_of_track( target_page, target_track), target_track->attr_LOCATOR, target_track->TTC );
//}

	// Break immediately on NULL return
	if ( target_track == NULL ) {
		return NULL;
	}

	// Adjust the track tempomul if it is required
	if ( G_TTC_abs_value == 1 ){
		adjust_tempomul( target_page, target_track );
	}

	#ifdef FEATURE_ENABLE_SONG_UPE
	if (G_align_bit){
		return target_track;
	}
	#endif
	// Measure CPU load. If we hit the max load, set the CPU monitor to MIR_RED.
	// The MIR_RED state will prevent any following tracks from being played.
	// The CPU load value and the overload LEDs will be reset in cpu_load_reset() in kickSequencerThread().

	cpu_load_measure();

	if ( cpu_load_at_max() ) {

		G_CPU_load_monitor = MIR_RED;
		cyg_alarm_initialize( CPU_overloadAlarm_hdl, cyg_current_time() + CPU_RED_INTERVAL,	0 );
	}

	// Decide on the MIDI monitor to check
	MIDI_LOAD_MONITOR = get_MIDI_MONITOR_for_track( target_track );

	// Avoid sequencer overflow - do not play row if we are in the red
	if ( (MIDI_LOAD_MONITOR != MIR_RED) && (G_CPU_load_monitor != MIR_RED) ) {

		// Play the row on the full beat of the track, if it is playing
		if (	( target_track->TTC == 1 )
			&&	( target_track->attr_LOCATOR != 0 )

			// ..and if page is not muted in the GRID
			&& 	( (GRID_mutepattern & ( 1<<(target_page->pageNdx % 10))) == 0 )

			){

			// Extract the row of the target track
			row = row_of_track( target_page, target_track );

			// Play row information as MIDI data - enqueue as needed
			// page, track, multiplier flag, column to be played, CURRENT or NEXT
			// Current Column - steps: straight, pushed
			// Consolidate the coordinates for the function call
			unsigned char target_row 		=  row;
			unsigned char target_locator 	=  target_track->attr_LOCATOR;

			last_CC = PLAYER_play_row( 	target_page, target_row, multiplier,
										target_locator,	CURRENT, MIDICC_NONE );

			// Next Column - steps: pulled. Ensure we are handling chain wraps!
			// Consolidate the coordinates for the function call
			target_locator = get_next_tracklocator( target_track, target_track->attr_LOCATOR );

			if ( target_locator < target_track->attr_LOCATOR ){
				// There is a wrap, so lookup the next row to play
				row = row_of_track( target_page, target_page->Track[row]->chain_data[NEXT] );
			}

			target_row = row;

			PLAYER_play_row(	target_page, target_row, multiplier,
								target_locator, NEXT, last_CC );
		}
	}

	return target_track;
}


// Compute the size of the playing gate
unsigned short getGateSize( Trackstruct* hyperTrackPt, Stepstruct* hyperStepPt ){

	unsigned char gateSize = 0;
	unsigned char temp = 0;

	temp 		= hyperTrackPt->LEN_factor + hyperTrackPt->event_offset[ATTR_LENGTH];
	gateSize	= 	((	hyperStepPt->attr_LEN * ( (hyperStepPt->event_data & 0xF0)>>4 ) )
					*	Track_LEN_factor[ temp ]) / 16;
	gateSize	= normalize( gateSize, 12, 192 );

	return gateSize;
}


// Returns the number of triggers we need to play for the current position of
// a hyped track.
unsigned char getPlayCount( Pagestruct* pagePt, unsigned char row ){

	// Will be returned as the play count value
	unsigned char 	result 			= 0;

	Trackstruct* 	trackPt 		= pagePt->Track[row];
	Trackstruct*	hyperTrackPt	= pagePt->Track[trackPt->hyper >> 4];
	// Pointer to the hyperstep of the track
	Stepstruct*		hyperStepPt		= pagePt->Step[trackPt->hyper >> 4][trackPt->hyper & 0x0F];

	// Size of the gate is the length of the hyperstep. Default is 192
	unsigned short gateSize = getGateSize( hyperTrackPt, hyperStepPt );
	// d_iag_printf( "\n--- gateSize:%d gatePosition:%d\n", gateSize, trackPt->gatePosition );

	// This may be modified later on to consider shorter tracks (if skipped steps)
	unsigned short	signalSize	=
		192 - ( 12 * my_bit_cardinality( Page_get_skippattern( pagePt, row) ) );

	// The base number of triggers based on speed multiple calculation
	unsigned short 	multiplier	= (signalSize / gateSize);

	// The triggers that need to be distributed across
	unsigned short 	overflow	= signalSize - (multiplier * gateSize);
	//		d_iag_printf( "multiplier:%d overflow:%d\n", multiplier, overflow );

	// The interval size of the extra triggers
	float 	trigIntvl	= (float) gateSize / (float) overflow;
	//		d_iag_printf( "trigIntvl:%f\n", trigIntvl );

	// Compute the number of extra triggers sent so far
	float triggers_done = trackPt->gatePosition / trigIntvl;

	// Update the gate position of the track
	trackPt->gatePosition 		= (trackPt->gatePosition + 1); //  % gateSize;
	//		d_iag_printf( "gatePosition:%d\n", trackPt->gatePosition );

	// Compute the number of extra triggers that should have been sent
	float triggers_open = trackPt->gatePosition / trigIntvl;
	// d_iag_printf( "triggers_open:%f(%d) _done:%f(%d) /:%f\n",
	//	triggers_open, (short)triggers_open, triggers_done, (short)triggers_done, triggers_open/triggers_done );

	// Return at least the multiplier value, or one more if we are on the extra trigger
	result = multiplier;
	if (	( (short)triggers_open > (short)triggers_done )
		){
		result += 1;
		//			d_iag_printf( "   NOW trigIntvl:%f\n", trigIntvl );
	}

	return result;
}



// Play the track (track is a chain of 1 or more rows) currently playing in given row
void PLAYER_play_track( Pagestruct* target_page, unsigned char row ){

	unsigned char i = 0;
	unsigned char temp = 0;
	unsigned char playCount = 0;
	// Pointer to the hyperstep of the track
	Stepstruct*	  hyperStepPt;
	unsigned char gateSize = 0;

	// Extract the track pointer - syntax simplification
	Trackstruct* target_track 	= target_page->Track[row];
	Trackstruct* trackPt		= target_track;
	Trackstruct* origin_track 	= target_page->Track[row];

	#ifdef FEATURE_ENABLE_SONG_UPE
	if ( Track_get_MISC(target_track, CONTROL_BIT ) ) {
		return;
	}
	#endif

	// -----------------------------------------------------------------------------
	// Handle hypersteps - generate triggers for the track
	// The track is being hyped, so handle..
	if ( trackPt->hyper < 0xF0 ){

		// Find out how many times to trigger the track for
		playCount 	= getPlayCount( target_page, row );

		// Identify the hyperstep of the track
		hyperStepPt	= target_page->Step[trackPt->hyper >> 4][trackPt->hyper & 0x0F];

		// Extract the gate length (corresponding to length of the hyperstep)
//		gateSize	= normalize( hyperStepPt->attr_LEN, 12, 192 );
		gateSize	= getGateSize( target_page->Track[trackPt->hyper >> 4], hyperStepPt );

		// playCount iterator - Execute as often as indicated by the playCount
		for ( i=0; i < playCount; i++ ){

			// Trigger the Track on TTC values, but only if it is still on!
			if (	( target_track != NULL )
				&& 	( target_track->chain_data[PLAY] != NULL )
				){
				target_track = TTC_trigger_track( target_page, target_track, i );
			}

			// Check if we have played the full frame
			// Stop track from playing once gate has been fully played
			if (	( trackPt->gatePosition >= gateSize )
				&&	( trackPt->TTC == 12 )
					){

				// Shut the track up
				trackPt->chain_data[PLAY] 	= NULL;
				trackPt->frame_ndx 			= 0;
				trackPt->attr_LOCATOR 		= 0;
				trackPt->TTC 				= 0;

				// Update the gate position
				trackPt->gatePosition		= 0;
			}

		}

//		d_iag_printf( "     after: TTC:%d LOC:%d frame_ndx:%d\n",
//				trackPt->TTC, trackPt->attr_LOCATOR, trackPt->frame_ndx );

		// Do not continue operation - rest is reserved for non-hyped tracks
		return;
	}
	// -----------------------------------------------------------------------------


	unsigned char attr_TEMPOMUL = target_track->attr_TEMPOMUL;
	unsigned char attr_TEMPOMUL_SKIP = target_track->attr_TEMPOMUL_SKIP;
	bool has_adjust_tempomul = ( G_TTC_abs_value == 1 && track_tempo_ptr );

	#ifdef FEATURE_ENABLE_DICE
	DiceOffsetClock dice_clock = dice_apply_clock_offset( target_page, row );
	attr_TEMPOMUL = dice_clock.attr_TEMPOMUL;
	attr_TEMPOMUL_SKIP = dice_clock.attr_TEMPOMUL_SKIP;
	#endif

	switch( target_track->attr_TEMPOMUL_SKIP >> 4 ){

		// Play the track - no skipping, or was skipped and now time to play it
		// Includes the multipliers
		case 0:

			switch( target_track->attr_TEMPOMUL ){

				// Triplet tempo multiplier: flagged as 15!!
				case 15:

					// Counter for the triplet multiplier. Trigger once, or twice, alternatingly.
					temp = 1 + ( G_TTC_abs_value % 2 );

					// G_master_tempoMUL iterator - Execute as often as indicated by tempo multiplier
					for ( i=0; i < temp; i++ ){

						// Trigger the Track on TTC values
						if (	( target_track == origin_track )
							&&	( target_track != NULL )
							){
							target_track = TTC_trigger_track( target_page, target_track, i );
						}
					}
					break;


				// Inverse triplet multiplier
				case 25:

					// Counter for the inverse triplet multiplier
					temp = my_min_value( G_TTC_abs_value % 3, 1 );

					// G_master_tempoMUL iterator - Execute as often as indicated by tempo multiplier
					for ( i=0; i < temp; i++ ){

						// Trigger the Track on TTC values
						if (	( target_track == origin_track )
							&&	( target_track != NULL )
							){
							target_track = TTC_trigger_track( target_page, target_track, i );
						}
					}
					break;


				// Regular tempo multipliers
				default:

					// G_master_tempoMUL iterator - Execute as often as indicated by tempo multiplier
					// or until there is a track switch due to a chain (origin != target)


					for ( i=0; i < attr_TEMPOMUL; i++ ){

						// Trigger the Track on TTC values
						if (	( target_track == origin_track )
							&&	( target_track != NULL )
							){
							target_track = TTC_trigger_track( target_page, target_track, i );
						}
					}

					// Pick up operation in the potentially next track (e.g. in chains)
					// The first click is already done, do the rest..
					if ( target_track != origin_track ){
						#ifdef FEATURE_ENABLE_DICE
						attr_TEMPOMUL = dice_apply_clock_offset( target_page, row_of_track( target_page, target_track ) ).attr_TEMPOMUL;
						#else
						attr_TEMPOMUL = target_track->attr_TEMPOMUL;
						#endif						
						for ( i=0; i < attr_TEMPOMUL-1; i++ ){

							// Trigger the Track on TTC values - as long as it doesn't become NULL
							if ( target_track != NULL ){
								target_track = TTC_trigger_track( target_page, target_track, i );
							}
						}
					}

					break;

			} // Switch on G_master_tempoMUL

			attr_TEMPOMUL = target_track->attr_TEMPOMUL;
			attr_TEMPOMUL_SKIP = target_track->attr_TEMPOMUL_SKIP;

			if( has_adjust_tempomul ){
				// Tempo updated recalculate skip
				#ifdef FEATURE_ENABLE_DICE
				attr_TEMPOMUL_SKIP = dice_apply_clock_offset( target_page, row ).attr_TEMPOMUL_SKIP;
				#else
				attr_TEMPOMUL_SKIP = target_track->attr_TEMPOMUL_SKIP;
				#endif
			}

			// Refill the upper nibble of the skipper indicator (which is counting the skips)
			if ( 	( ( attr_TEMPOMUL_SKIP & 0x0F) != 0 )
				){

				target_track->attr_TEMPOMUL_SKIP =
						(target_track->attr_TEMPOMUL_SKIP & 0x0F)
					|	((attr_TEMPOMUL_SKIP & 0x0F) << 4) ;
			}

			break;


		// Track is being skipped for now, just decrement the skip counter this time around
		default:

			// Decrement the value of the upper nibble in the SKIP counter
			target_track->attr_TEMPOMUL_SKIP =
				// Decremented upper nibble
					(((target_track->attr_TEMPOMUL_SKIP >> 4) -1) << 4)
				// Lower nibble stays the same, so assemble the new value
				|	( target_track->attr_TEMPOMUL_SKIP & 0x0F);
			break;

	}// switch on the track skip attribute
}



//_______________________________________________________________________________________
//
// Plays the given target page at the current TTC value
void PLAYER_play_page( 		Pagestruct* 	target_page,
							unsigned char 	in_G_TTC_abs_value ) {

	unsigned int row = 0;

	// GENERATE TRACK PLAYLIST - the tracks that are gonna play..
	for ( row = 0; row < MATRIX_NROF_ROWS; row ++ ){

		// Copy the track pointer into the play playlist
		if ( target_page->Track[row]->chain_data[PLAY] == target_page->Track[row] ){

			Track_playlist[row] = target_page->Track[row];
		}
		// Otherwise mark playlist position empty
		else{

			// Initialize the playlist position
			Track_playlist[row] = NULL;
		}
	}

	// Clear the Effector pools on the page beat, before it gets filled up again
	if ( in_G_TTC_abs_value == 1 ){

		// Clear the EFF values in page.
		// ..they will be computed again in the next round of TTC going from 1 to 16
		target_page->EFF_pool[ ATTR_VELOCITY ]  = 0;
		target_page->EFF_pool[ ATTR_PITCH 	 ]  = 0;
		target_page->EFF_pool[ ATTR_LENGTH 	 ]  = 0;
		target_page->EFF_pool[ ATTR_MIDICC 	 ]  = 0;
	}

	// PLAY THE PLAYLIST of this particular page, as generated above
	for ( row=0; row < MATRIX_NROF_ROWS; row ++ ){

		// Operate on valid playlist entry, and if the page bank is not muted in the grid!
		if ( Track_playlist[row] != NULL ){

			PLAYER_play_track( target_page, row );
		}
	}

}



//_______________________________________________________________________________________
//
// Preview function for a particular track - used to pre-audit a track's base
void PLAYER_preview_track( 	Pagestruct* target_page,
							unsigned char row 			){

	unsigned char midiCH	=	target_page->Track[row]->attr_MCH
								+ target_page->Track[row]->event_offset[ATTR_MIDICH];

	unsigned char pitch		= normalize(
					  			STEP_DEF_PITCH +
					  			target_page->Track[row]->attr_PIT +
								target_page->attr_PIT - MIDDLE_C,
					  			0, 127 );

	unsigned char velocity	= normalize(
								( ( STEP_DEF_VELOCITY +
								    + target_page->Track[row]->attr_VEL )
							  	  * Track_VEL_factor[ target_page->attr_VEL ] )
							  	/ VEL_FACTOR_NEUTRAL_VALUE,
							  	0, 127);

	// NOTE ON send
	MIDI_send(	MIDI_NOTE, midiCH, pitch, velocity );

	// NOTE OFF build
	MIDI_OFF_build_trackpreview( midiCH, pitch );
}



//_______________________________________________________________________________________
//
// Preview function for a particular step - used to pre-audit a step - needs chord support
void PLAYER_preview_step( 	Pagestruct* target_page,
							unsigned char row,
							unsigned char col			){

	unsigned char which_col = 0;

	// Decide based on the start value of the step in question
	switch( target_page->Step[row][col]->attr_STA < STEP_DEF_START ){
	case TRUE:
		which_col = NEXT;
		break;

	case FALSE:
		which_col = CURRENT;
		break;
	}

	// NOTE sent for enqueing - even if it may not be active - may need its EFF input
	play_row_ON( 	target_page,
					row,
					(1 << row),
					col+1,
					which_col,
					target_page->Track[row]->TTC,
					1 );

	//////////////////
	// PLAY MIDI QUEUE - otherwise events wait in queue till next timer interrupt
	play_MIDI_queue( G_MIDI_timestamp+1 );
}


