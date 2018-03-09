/* OCTOPUS
 * Definitions of the TRACK data and functions
 * 20.11.2003 genoQs Machines
 */


typedef struct trackstruct {

	// A PC of -1 means none is active, the rest is 0-128
	signed char program_change;

	// Holds the trigger data for the track: (pg. 331 book)
	// Index of trigger set in the track direction array
	unsigned char trigger_set_ndx;

	// Copy of the trigger set from the direction array
	unsigned char trigger[10];


	// Holds the chain data in array: CAPO (0), NEXT (1), PREV (2). 
	// PLAY(4) indicates which track in the chain is playing.
	struct trackstruct* chain_data[4];
	
	// Hold the range values for VEL and PIT modulation
	
	unsigned char range_VEL;
	unsigned char range_PIT;
	unsigned char range_MCC;

	unsigned char event_VEL;
	unsigned char event_PIT;
	unsigned char event_MCC;


	// Each track has a TTC enabling it to basically run its own clock.
	unsigned char TTC;

	// Stores the pitch offsets, which get produced by changing the scale and the lead
	signed char scale_pitch_offset;
	signed char lead_pitch_offset;
	
	// Factors for the track - factoring the step attributes maps
	unsigned char VEL_factor;
	unsigned char PIT_factor;
	unsigned char LEN_factor;
	unsigned char STA_factor;
	unsigned char GRV_factor;
	unsigned char MCC_factor;

	// Makes track attributes accessible by #defined labels
	signed char attribute[ TRACK_NROF_ATTRIBUTES ];
	// Source 		Meaning					Array index
	// ----------   ----------------       -------------
	// Attributes
	//	#define 	ATTR_SELECT				0
	//	#define		ATTR_VELOCITY			1
	//	#define 	ATTR_PITCH				2
	//	#define 	ATTR_LENGTH				3
	//	#define 	ATTR_START				4
	//	#define 	ATTR_POSITION			5
	//	#define		ATTR_DIRECTION			6
	//	#define		ATTR_AMOUNT				7
	//	#define 	ATTR_GROOVE				8
	//	#define 	ATTR_MIDICC				9
	//	#define 	ATTR_MIDICH				10
	//	#define 	ATTR_LOCATOR			11
	//	#define 	ATTR_MISC				12		// Collection of bits, keeping all sorts of info
													// |_|_|_|_|_|_|_|_|
													//	7 6 5 4 3 2 1 0
													//  | | | | | | | +------> DIRECTION :: RETURN
													//  | / | \ | / |          OFF : current direction: increasing
													//  |   |   |   |          ON  : current direction: decreasing
													//  |   |   |   |
													//  |   |   |   +--------> DIRECTION :: SINGLE
													//	|	|	|			   OFF : play, track end not reached
													//	|	|	|			   ON  : track end reached	
													//	|	|	|
													//	|	|	+------------> CHORD
													//	|	|				   Stores one of the 7 track chord flags
													//	|	|				   as a numeric value -> 0-6.
													//	|	|
													//	|	+----------------> REC 
													//						   1 means that recording is enabled on this track
													//	|
													//	+--------------------> EFF VALUE ::
													//						   OFF 	   : EFF not active
													//						   SEND    : Send track values into EFF pool
													//						   RECEIVE : be influenced by the EFF pool

	//	#define		ATTR_CCAMNT				13		// This seems to be a duplicate of AMOUNT, hence empty space

	//	#define 	ATTR_TEMPOMUL			14		// Stores the track tempo multiplier as numeric value.
													// Valid values are as follows:
													// 0	- 	Track is paused
													// 1	-	1x Master tempo
													// 2	-	2x
													// 3	-	3x
													// 4	-	4x
													// 5	-	5x
													// 12	-	1/2x
													// 14	-	1/4x			

	//	#define		ATTR_TEMPOMUL_SKIP		15		// Counter used by the fractional TEMPOMULs (12 and 14)
													// |_|_|_|_|_|_|_|_|
													//	7 6 5 4 3 2 1 0
													//  | | | | + + + +------> SET SKIPPER VALUE
													//  + + + +--------------> CURRENT SKIPPER VALUE
} Trackstruct;



// This implementes the construction function for Track
void Track_init( Trackstruct* target_track ){
	
	unsigned char i=0;

	// Init the trigger repository
	for ( i=0; i<10; i++ ){
		target_track->trigger[i] = 0;
	}	
	target_track->trigger_set_ndx			= 0;

	// Init the chain data for 10 unchained rows: each track is by itself
	target_track->chain_data[HEAD] = target_track;
	target_track->chain_data[NEXT] = target_track;
	target_track->chain_data[PREV] = target_track;
	target_track->chain_data[PLAY] = target_track;
	
	// Initialization sequence
	target_track->attribute[ATTR_STATUS] 	= OFF;
	target_track->attribute[ATTR_VELOCITY] 	= TRACK_DEF_VELOCITY;
	target_track->attribute[ATTR_PITCH] 	= TRACK_DEF_PITCH;
	target_track->attribute[ATTR_LENGTH] 	= TRACK_DEF_LENGTH;
	target_track->attribute[ATTR_START] 	= TRACK_DEF_START;
	target_track->attribute[ATTR_POSITION]	= TRACK_DEF_POSITION;
	target_track->attribute[ATTR_DIRECTION]	= TRACK_DEF_DIRECTION;
	target_track->attribute[ATTR_AMOUNT]	= TRACK_DEF_AMOUNT;
	target_track->attribute[ATTR_GROOVE]	= TRACK_DEF_GROOVE;
	target_track->attribute[ATTR_MIDICC]	= TRACK_DEF_MIDICC;
	target_track->attribute[ATTR_MIDICH]	= TRACK_DEF_MIDICH;
	target_track->attribute[ATTR_LOCATOR]	= 0;
	target_track->attribute[ATTR_MISC]      = (1 << 2);		// sets the chord bit
	target_track->attribute[ATTR_CCAMNT]	= 0;
	target_track->attribute[ATTR_TEMPOMUL]	= 1;

	target_track->scale_pitch_offset		= 0;
	target_track->lead_pitch_offset			= 0;
	target_track->TTC						= 0;

	target_track->range_VEL					= TRACK_DEF_RANGE_VEL;
	target_track->event_VEL					= 0;
	target_track->range_PIT					= TRACK_DEF_RANGE_PIT;
	target_track->event_PIT					= 0;
	target_track->range_MCC					= STEP_DEF_RANGE_MCC;
	target_track->event_MCC					= 0;

	// Index into the factoring array for the attribute map factors
	target_track->LEN_factor				= TRACK_DEF_LENFACTOR;
	target_track->STA_factor				= TRACK_DEF_STAFACTOR;
	target_track->VEL_factor				= TRACK_DEF_VELFACTOR;
	target_track->PIT_factor				= TRACK_DEF_PITFACTOR;
	target_track->GRV_factor				= TRACK_DEF_GRVFACTOR;
	target_track->MCC_factor				= TRACK_DEF_GRVFACTOR;
	
	target_track->program_change			= TRACK_DEF_PROGRAMCHANGE;
}



// Gets the data from the MISC attribute, according to given flag
unsigned char Track_get_MISC( 	Trackstruct* target_track,
								unsigned char target_flag 	){	
	unsigned char result=0;

	switch( target_flag ){

		case CHORD_BIT:
			// Return the value of the CHORD value in the target track
			result = (target_track->attribute[ATTR_MISC] & 0x1C) >> 2;
			break;
			
		case EFF_BIT:
			// Check the status of the EFF bits in the target track
			result = (target_track->attribute[ATTR_MISC] & 0xC0) >> 6;
			break;
	}

	return result;
}


// Sets the data into the MISC attribute, according to given flag
void Track_set_MISC( 			Trackstruct* target_track,
								unsigned char target_flag, 
								unsigned char in_value 		){	

	unsigned char temp = target_track->attribute[ATTR_MISC];

	switch( target_flag ){

		case CHORD_BIT:
			// Erase the three chord bits
			temp &= 0xE3;
			// Set the value of the CHORD value in the target track
			target_track->attribute[ATTR_MISC] = ( temp | (in_value << 2) );
			break;
			
		case EFF_BIT:
			// Erase the EFF bit
			temp &= 0x3F;
			// Set the new EFF bit
			target_track->attribute[ATTR_MISC] = ( temp | (in_value << 6) );
			break;
	}
}


