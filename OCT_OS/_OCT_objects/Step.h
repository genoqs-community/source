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



/* OCTOPUS
 * Definitions of the step data and functions
 * This will later be the STEP object
 * 08.11.2003 genoQs Machines
 */


#include "../_OCT_objects/Runtime.h"




// Stores the pointer to a selected step. Only one at a time may be selected.
Stepstruct* STEP_COPY_BUFFER = NULL;



// Get the step MCC activity
unsigned char Step_get_MCC_ACTIVITY (Stepstruct* target_step){

	// Check whether we have a valid CC amount or not
	if ( target_step->attr_MCC != MIDICC_NONE ) {

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

	if ( target_step->attr_STATUS & (1 << target_bit) ) {

		return ON;
	}
	else {

		return OFF;
	}
}



//
// STEP STATUS SETTER (universal). Flags are STEPSTAT_TOGGLE, _SELECT, _SKIP, _EVENT
//
void Step_set_status (			Stepstruct* target_step,
								unsigned char target_bit,
								unsigned char in_value		) {
	// Set target bit to 0
	target_step->attr_STATUS &= (0xFF ^ (1 << target_bit));

	// Apply the in_value
	if (in_value == ON) {
		target_step->attr_STATUS |= (1 << target_bit);
	}
}



// This implementes the construction function for Step
void Step_init( Stepstruct* target_step ){

	// No event for the step initially (in ATTR_STATUS), but default is ATTR_VELOCITY (0);
	target_step->event_data		= 	0;

	// Length multiplier of step (Dieter Herten)
	target_step->event_data		= 	1 << 4;
	// target_step->LEN_multiplier	=	1;

	// No chord set for the step, and 9 is the neutral strum value.
	target_step->chord_data		= 	9 << 11;
	target_step->phrase_num     =   0;
	target_step->phrase_data    =   PHRASE_DATA_INIT_VAL;
	target_step->chord_up		= 	0;

	// Initialization sequence
	target_step->attr_STATUS 	=	OFF;
	target_step->attr_VEL 		=	STEP_DEF_VELOCITY;
	target_step->attr_PIT		=	STEP_DEF_PITCH;
	target_step->attr_LEN		= 	STEP_DEF_LENGTH;
	target_step->attr_STA		= 	STEP_DEF_START;
	target_step->attr_AMT		= 	STEP_DEF_AMOUNT;
	target_step->attr_MCC		= 	STEP_DEF_MIDICC;

	// Step position attribute holds the second 7-bit nibble for the bender value, next to MIDICC
	target_step->attr_POS		= 	STEP_DEF_POSITION;

	// Set the hyperTrackNdx to 10, which means "void". Valid values are 0-9, as track indices
	target_step->hyperTrack_ndx =	10;

	//	target_step->attr_DIR	= 	STEP_DEF_DIRECTION;
}



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
			value = target_step->attr_VEL;
			break;

		case ATTR_PITCH:
			maxvalue = STEP_MAX_PITCH;
			minvalue = STEP_MIN_PITCH;
			value = target_step->attr_PIT;
			break;

		case ATTR_LENGTH:
			// Do not allow Legato to be selected by RND
			maxvalue = STEP_DEF_LENGTH * 3;
			minvalue = STEP_MIN_LENGTH;
			value = (unsigned char) target_step->attr_LEN;
			break;

		case ATTR_START:
			maxvalue = STEP_MAX_START;
			minvalue = STEP_MIN_START;
			value = target_step->attr_STA;
			break;
#if GROEF || 1
		case ATTR_GROOVE:
			maxvalue = STEP_MAX_GROOVE;
			minvalue = STEP_MIN_GROOVE;
			value = target_step->phrase_num;
			break;
#endif
		case ATTR_AMOUNT:
			maxvalue = STEP_MAX_AMOUNT;
			minvalue = STEP_MIN_AMOUNT;
			value = target_step->attr_AMT;
			break;

		case ATTR_MIDICC:
			maxvalue = STEP_MAX_MIDICC;
			minvalue = STEP_MIN_MIDICC;
			value = target_step->attr_MCC;
			break;
	}

	result = randomize_byamount( value, amount, minvalue, maxvalue );

	return result;
}



// Randomizes the parameter values of the given step
void Step_randomize( Stepstruct* target_step ){

	// Randomize Step parameters
	target_step->attr_VEL 	=
		randomStepAttrValue( target_step, ATTR_VELOCITY, 	TRACK_DEF_AMOUNT 		);

	target_step->attr_PIT 		=
		randomStepAttrValue( target_step, ATTR_PITCH, 		TRACK_DEF_AMOUNT 		);

	target_step->attr_LEN		=
		randomStepAttrValue( target_step, ATTR_LENGTH, 		TRACK_DEF_AMOUNT		);

	target_step->attr_STA		=
		randomStepAttrValue( target_step, ATTR_START, 		TRACK_DEF_AMOUNT 		);

	target_step->attr_AMT		=
		randomStepAttrValue( target_step, ATTR_AMOUNT, 		TRACK_DEF_AMOUNT		);

	target_step->attr_MCC		=
		randomStepAttrValue( target_step, ATTR_MIDICC, 		TRACK_DEF_AMOUNT		);
}




// Copies the contents of source step structure to the target step structure
void Step_copy( const Stepstruct* source_step, Stepstruct* target_step, bool copy_hyper_data ){

	// Copy the event information
	// Copy the length multiplier implicitly
	target_step->event_data = source_step->event_data;

	// Copy the chord data
	target_step->chord_data  = source_step->chord_data;
	target_step->phrase_num  = source_step->phrase_num;
	target_step->phrase_data = source_step->phrase_data;
	target_step->chord_up 	 = source_step->chord_up;


	// Copy the status
	target_step->attr_STATUS = source_step->attr_STATUS;


	// Copy the regular attributes
	target_step->attr_VEL		= 	source_step->attr_VEL;
	target_step->attr_PIT		= 	source_step->attr_PIT;
	target_step->attr_LEN		=	source_step->attr_LEN;
	target_step->attr_STA		=	source_step->attr_STA;
	target_step->attr_POS		= 	source_step->attr_POS;
//	target_step->attr_DIR		= 	source_step->attr_DIR;
	target_step->attr_AMT		=	source_step->attr_AMT;
	target_step->attr_MCC		=	source_step->attr_MCC;

	// Copy hyper data
	if( copy_hyper_data ) {
		target_step->hyperTrack_ndx =	source_step->hyperTrack_ndx;
	}
}




