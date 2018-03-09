/* OCTOPUS
 * Definitions of the step data and functions
 * This will later be the STEP object
 * 08.11.2003 genoQs Machines
 */


typedef struct stepstruct{
	
	unsigned char  event_data;
	// |__|__|__|__|__|__|__|__|
	//   7  6  5  4  3  2  1  0
	//   |  |  |  |  |  |  |  |  
	//   |  |  |  |  +--+--+--+------------------------------>  4 bits of attribute index 
	
	unsigned short chord_data;
	// |__|__|__|__|__|__|__|__|__|__|__|__|__|__|__|__|
	//  15 14 13 12 11 10  9  8  7  6  5  4  3  2  1  0
	//   |  |  |  |  |  |  |  |  |  |  |  |  |  |  |  |
	//   |  |  |  |  |  +--+--+--+--+--+--+--+--+--+--+------> 11 bits of auxiliary notes
	//   |  |  |  |  |
	//   +--+--+--+--+--------------------------------------->  5 bit strum value

	
	/* Step status: 8 bits hold the step status (counting right to left):
	 * ..contents of attribute[ATTR_STATUS];
	 *	+--+--+--+--+--+--+--+--+
	 *	| 7| 6| 5| 4| 3| 2| 1| 0| 
	 * 	+--+--+--+--+--+--+--+--+
	 *                         
	 * 						 |-0|
	 * 						   0  OFF					(TOGGLE)
	 *                         1  ON 					(TOGGLE)
	 *                    |-1|
	 * 					    0  	  UN-SELECTED			(SELECT)
	 * 				      	1     SELECTED				(SELECT)
	 *                 |-2|
	 * 					 0 		  UN-SKIPPED			(SKIP)
	 * 				  	 1		  SKIPPED				(SKIP)
	 * 				|-3|	
	 * 				  0			  Step always played	(PROVISORY)
	 * 				  1			  STEP PROVISORY		(PROVISORY)
	 * 			 |-4|
	 * 			   0			  EVENT OFF				(EVENT)
	 * 			   1			  EVENT ON				(EVENT)
	 * 		  |-5|			
	 * 			0				  empty..
	 * 			1	
	 * 
	 * 	   |-6|                   
	 *       0                    ROLL plays all noteoffs					  
	 *       1                    ROLL plays only last noteoff
	 *  |-7|
	 *    0						  ROLL is OFF
	 *    1 					  ROLL is ON
	 *	  
	 */	

	// The step attributes are offsets relative to the values of the corresponding
	// track attributes of the tracks they belong to.

	signed char attribute[ STEP_NROF_ATTRIBUTES ];
	// currently the first 10 are used
	// Source 		Meaning					Array index
	// 	manual		ATTR_STATUS				0	
	//#define		ATTR_VELOCITY			1
	//#define 		ATTR_PITCH				2
	//#define 		ATTR_LENGTH				3
	//#define 		ATTR_START				4	
	//#define 		ATTR_POSITION			5
	//#define		ATTR_DIRECTION			6
	//#define		ATTR_AMOUNT				7
	//#define 		ATTR_GROOVE				8
	//#define 		ATTR_MIDICC				9

} Stepstruct;


// Stores the pointed to a selected step. Only one at a time may be selected.
Stepstruct* STEP_COPY_BUFFER = NULL;

// --> moved to Step.h, after Stepstruct has been defined.



// Get the step MCC activity
unsigned char Step_get_MCC_ACTIVITY (Stepstruct* target_step){

	// Check whether we have a valid CC amount or not 
	if ( target_step->attribute[ATTR_MIDICC] != MIDICC_NONE ) {

		return ON;
	}
	else {

		return OFF;
	}

}


// 
// STEP PROVISORY SETTER
// 
void Step_set_PROVISORY( Stepstruct* target_step, unsigned char in_value ){
	// Set provisory bit to 0 by &ing with binary 0xFB (clean the skip bit first)
	target_step->attribute[ATTR_STATUS] &= 0xF7;
	
	// Normalize in_value
	if (in_value != OFF) {
		in_value = 1 << 3;
	}

	// Write the in_value into PROVISORY bit
	target_step->attribute[ATTR_STATUS] |= in_value;
}


// 
// STEP PROVISORY GETTER
//
unsigned char Step_get_PROVISORY (Stepstruct* target_step) {

	if (target_step->attribute[ATTR_STATUS] & (1 << 3) ){
		return ON;
	}
	else {
		return OFF;
	}
}


//
// STEP STATUS GETTER (universal). Flags known are STEPSTAT_TOGGLE, _SELECT, _SKIP, _EVENT
//
unsigned char Step_get_status (	Stepstruct* target_step, 
								unsigned char target_bit){
									
	if ( target_step->attribute[ATTR_STATUS] & (1 << target_bit) ) {

		return ON;
	}
	else {

		return OFF;
	}
}

//
// STEP STATUS SETTER (universal). Flags are STEPSTAT_TOGGLE, _SELECT, _SKIP
// 
void Step_set_status (			Stepstruct* target_step, 
								unsigned char target_bit, 
								unsigned char in_value		) {
	// Set target bit to 0
	target_step->attribute[ATTR_STATUS] &= (0xFF ^ (1 << target_bit));
	
	// Apply the in_value
	if (in_value == ON) {
		target_step->attribute[ATTR_STATUS] |= (1 << target_bit);
	}
}



// This implementes the construction function for Step
void Step_init( Stepstruct* target_step ){

	// No event for the step initially (in ATTR_STATUS), but default is ATTR_VELOCITY (0);
	target_step->event_data					= 	0;

	// No chord set for the step, but 9 is the neutral strum value
	target_step->chord_data					= 	9 << 11;

	// Initialization sequence
	target_step->attribute[ATTR_STATUS] 	=	OFF;
	target_step->attribute[ATTR_VELOCITY] 	=	STEP_DEF_VELOCITY;
	target_step->attribute[ATTR_PITCH]		=	STEP_DEF_PITCH;
	target_step->attribute[ATTR_LENGTH]		= 	STEP_DEF_LENGTH;
	target_step->attribute[ATTR_START]		= 	STEP_DEF_START;
	target_step->attribute[ATTR_AMOUNT]		= 	STEP_DEF_AMOUNT;
	target_step->attribute[ATTR_MIDICC]		= 	STEP_DEF_MIDICC;

	//	target_step->attribute[ATTR_POSITION]	= 	STEP_DEF_POSITION;
	//	target_step->attribute[ATTR_DIRECTION]	= 	STEP_DEF_DIRECTION;
}



//// Scaffold: Display the contents of a Step struct
//void Step_print(Stepstruct* target_step){
//	



// Returns a random value valid for the passed attribute index,
// depending on the value of the passed amount
signed char randomStepAttrValue( 	Stepstruct* target_step,
									unsigned char attrNdx, 
									unsigned char amount ){

	signed int maxvalue = 0;
	signed int minvalue = 0;
	signed int value = 0;
	signed int result = 0;

	switch( attrNdx ){

		case ATTR_VELOCITY:
			maxvalue = STEP_MAX_VELOCITY;
			minvalue = STEP_MIN_VELOCITY;
			value = target_step->attribute[ATTR_VELOCITY];
			break;
			
		case ATTR_PITCH:
			maxvalue = STEP_MAX_PITCH;
			minvalue = STEP_MIN_PITCH;
			value = target_step->attribute[ATTR_PITCH];
			break;

		case ATTR_LENGTH:
			// Do not allow Legato to be selected by RND
			maxvalue = ((unsigned char) STEP_MAX_LENGTH) - 1;
			minvalue = STEP_MIN_LENGTH;
			value = (unsigned char) target_step->attribute[ATTR_LENGTH];
			break;

		case ATTR_START:
			maxvalue = STEP_MAX_START;
			minvalue = STEP_MIN_START;
			value = target_step->attribute[ATTR_START];
			break;			

		case ATTR_GROOVE:
			maxvalue = STEP_MAX_GROOVE;
			minvalue = STEP_MIN_GROOVE;
			value = target_step->attribute[ATTR_GROOVE];
			break;			

		case ATTR_AMOUNT:
			maxvalue = STEP_MAX_AMOUNT;
			minvalue = STEP_MIN_AMOUNT;
			value = target_step->attribute[ATTR_AMOUNT];
			break;

		case ATTR_MIDICC:
			maxvalue = STEP_MAX_MIDICC;
			minvalue = STEP_MIN_MIDICC;
			value = target_step->attribute[ATTR_MIDICC];
			break;	
	}

	result = randomize_byamount( value, amount, minvalue, maxvalue );

	return result;
}



// Randomizes the parameter values of the given step
void Step_randomize( Stepstruct* target_step ){

	// Randomize Step parameters
	target_step->attribute[ATTR_VELOCITY] 	= 
		randomStepAttrValue( target_step, ATTR_VELOCITY, 	TRACK_DEF_AMOUNT 		);
		
	target_step->attribute[ATTR_PITCH] 		= 
		randomStepAttrValue( target_step, ATTR_PITCH, 		TRACK_DEF_AMOUNT 		);		

	target_step->attribute[ATTR_LENGTH]		= 
		randomStepAttrValue( target_step, ATTR_LENGTH, 		TRACK_DEF_AMOUNT		);

	target_step->attribute[ATTR_START]		= 
		randomStepAttrValue( target_step, ATTR_START, 		TRACK_DEF_AMOUNT 		);

	target_step->attribute[ATTR_AMOUNT]		= 
		randomStepAttrValue( target_step, ATTR_AMOUNT, 		TRACK_DEF_AMOUNT		);

	target_step->attribute[ATTR_MIDICC]		= 
		randomStepAttrValue( target_step, ATTR_MIDICC, 		TRACK_DEF_AMOUNT		);
}



// Copies the contents of source step structure to the target step structure
void Step_copy( Stepstruct* source_step, Stepstruct* target_step ){
	
	unsigned int i=0;
	// d_iag_printf( "step would have been copied now..\n" );

	// Copy the chord data	
	target_step->chord_data = source_step->chord_data;

	// Copy the status
	target_step->attribute[ATTR_STATUS] = source_step->attribute[ATTR_STATUS];
	
	// Copy the other attributes
	for (i=0; i<STEP_NROF_ATTRIBUTES; i++) {

		// d_iag_printf( "     step-attribute[%d] now..\n", i );
		target_step->attribute[i] = source_step->attribute[i];
	}
}







