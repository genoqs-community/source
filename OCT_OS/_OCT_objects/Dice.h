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

/**
 * Dice is a performance Zoom layer that exists above the Grid allowing you to resample / modulate / chop-repeat the global TCC
 * to affect your page(s)  in the selected gridrows.
 */

#include "../_OCT_objects/Runtime.h"

unsigned char DICE_BANK;
unsigned char dice_synced_attr = 0;
unsigned char dice_synced_value = 0;

// Dice type definitions
typedef struct DiceFlowAttributes {
	unsigned char attr_SCALE;
	unsigned char attr_MAX;
	signed char attr_NEUTRAL;
	unsigned char attr_INTERVAL;
} DiceFlowAttributes;


// Updated track clock applied dice offset
typedef struct DiceOffsetClock {
	unsigned char attr_TEMPOMUL;
	unsigned char attr_TEMPOMUL_SKIP;
} DiceOffsetClock;


// Used to clear the dice soft factors, used at runtime
void Dice_init() {

	unsigned char i;

	DICE_bank->trackSelection 		= 0;
	DICE_bank->trackSelectionStored	= 0;
	DICE_bank->trackSolopattern		= 0;
	DICE_bank->REC_bit				= OFF;

	// Init Dice bank sessions
	for ( i=0; i < MAX_NROF_DICE; i++ ) {
		Trackstruct* target_dice = DICE_bank->Track[i];
		target_dice->attr_LEN 			= i + 1;
		target_dice->attr_STA 			= 0;

		// Dice flow TEMPOMUL / TEMPOMUL_SKIP
		target_dice->attr_TEMPOMUL		= 1;
		target_dice->attr_TEMPOMUL_SKIP = 0;

		// NEMO_selectedDiceAttribute
		target_dice->attr_STATUS 		= NEMO_ATTR_PITCH;

		// Dice Flags
		target_dice->attr_MISC      	= 0;

		// For the MAP Attributes only: Index into the factoring array
		target_dice->VEL_factor				= 0;
		target_dice->PIT_factor				= 0;
		target_dice->LEN_factor				= 0;
		target_dice->STA_factor				= 0;
		target_dice->AMT_factor				= 0;
		target_dice->GRV_factor				= 0;
		target_dice->MCC_factor				= 0;

		// Initialise the flow shape indexes
		#ifdef NEMO
		target_dice->flow_shape[NEMO_ATTR_PITCH] 		= 0;
		target_dice->flow_shape[NEMO_ATTR_VELOCITY] 	= 0;
		target_dice->flow_shape[NEMO_ATTR_LENGTH] 		= 0;
		target_dice->flow_shape[NEMO_ATTR_START] 		= 0;
		target_dice->flow_shape[NEMO_ATTR_AMOUNT] 		= 0;
		target_dice->flow_shape[NEMO_ATTR_GROOVE] 		= 0;
		target_dice->flow_shape[NEMO_ATTR_MIDICC] 		= 0;
		#else
		target_dice->flow_shape[ATTR_PITCH] 		= 0;
		target_dice->flow_shape[ATTR_VELOCITY] 		= 0;
		target_dice->flow_shape[ATTR_LENGTH] 		= 0;
		target_dice->flow_shape[ATTR_START] 		= 0;
		target_dice->flow_shape[ATTR_AMOUNT] 		= 0;
		target_dice->flow_shape[ATTR_GROOVE] 		= 0;
		target_dice->flow_shape[ATTR_MIDICC] 		= 0;
		#endif

		unsigned char col = 0;
		// Row has been selected, so operate: init steps first
		for (col = 0; col < MATRIX_NROF_COLUMNS; col++) {
			Step_init( DICE_bank->Step[i][col] );
			DICE_bank->Step[i][col]->attr_AMT = STEP_MAX_AMOUNT;
		}
	}
}

// Makes a copy of the source dice to target
void Dice_copy( Trackstruct* source_dice, Trackstruct* target_dice ) {

	// ATTRIBUTES
	target_dice->attr_PIT 			=	source_dice->attr_PIT;
	target_dice->attr_VEL			=	source_dice->attr_VEL;
	target_dice->attr_LEN			=	source_dice->attr_LEN;
	target_dice->attr_STA			=	source_dice->attr_STA;
	target_dice->attr_POS			=	source_dice->attr_POS;
	target_dice->attr_AMT			=	source_dice->attr_AMT;
	target_dice->attr_GRV			=	source_dice->attr_GRV;
	target_dice->attr_MCC			=	source_dice->attr_MCC;
	target_dice->attr_TEMPOMUL		= 	source_dice->attr_TEMPOMUL;
	target_dice->attr_TEMPOMUL_SKIP =	source_dice->attr_TEMPOMUL_SKIP;
	target_dice->attr_MISC      	= 	source_dice->attr_MISC;

	// FACTORS
	target_dice->VEL_factor = source_dice->VEL_factor;
	target_dice->PIT_factor = source_dice->PIT_factor;
	target_dice->LEN_factor = source_dice->LEN_factor;
	target_dice->STA_factor = source_dice->STA_factor;
	target_dice->AMT_factor = source_dice->AMT_factor;
	target_dice->GRV_factor = source_dice->GRV_factor;
	target_dice->MCC_factor = source_dice->MCC_factor;

	#ifdef NEMO
	target_dice->flow_shape[NEMO_ATTR_PITCH] 		= source_dice->flow_shape[NEMO_ATTR_PITCH];
	target_dice->flow_shape[NEMO_ATTR_VELOCITY] 	= source_dice->flow_shape[NEMO_ATTR_VELOCITY];
	target_dice->flow_shape[NEMO_ATTR_LENGTH] 		= source_dice->flow_shape[NEMO_ATTR_LENGTH];
	target_dice->flow_shape[NEMO_ATTR_START] 		= source_dice->flow_shape[NEMO_ATTR_START] ;
	target_dice->flow_shape[NEMO_ATTR_AMOUNT] 		= source_dice->flow_shape[NEMO_ATTR_AMOUNT];
	target_dice->flow_shape[NEMO_ATTR_GROOVE] 		= source_dice->flow_shape[NEMO_ATTR_GROOVE];
	target_dice->flow_shape[NEMO_ATTR_MIDICC] 		= source_dice->flow_shape[NEMO_ATTR_MIDICC];
	#else
	target_dice->flow_shape[ATTR_PITCH] 		= source_dice->flow_shape[ATTR_PITCH];
	target_dice->flow_shape[ATTR_VELOCITY] 		= source_dice->flow_shape[ATTR_VELOCITY];
	target_dice->flow_shape[ATTR_LENGTH] 		= source_dice->flow_shape[ATTR_LENGTH];
	target_dice->flow_shape[ATTR_START] 		= source_dice->flow_shape[ATTR_START] ;
	target_dice->flow_shape[ATTR_AMOUNT] 		= source_dice->flow_shape[ATTR_AMOUNT];
	target_dice->flow_shape[ATTR_GROOVE] 		= source_dice->flow_shape[ATTR_GROOVE];
	target_dice->flow_shape[ATTR_MIDICC] 		= source_dice->flow_shape[ATTR_MIDICC];
	#endif
}

// Get the currently selected Dice
Trackstruct * Dice_get() {
	return DICE_bank->Track[SEL_DICE_BANK];
}

// Gets the data into the MISC attribute, according to given flag
unsigned char Dice_get_MISC( Trackstruct* target_dice, unsigned char target_flag ) {

	switch( target_flag ) {

		case DICE_TRACK_CLOCK:
		case DICE_GLOBAL_CLOCK:
		case DICE_GLOBAL_CLOCK | DICE_TRACK_CLOCK:
			return CHECK_MASK(target_dice->attr_MISC, target_flag);
	}

	return OFF;
}


// Sets the data into the MISC attribute, according to given flag
void Dice_set_MISC( Trackstruct* target_dice,
					 unsigned char target_flag,
					 unsigned char in_value ) {

	// Toggle set clock bits.
	unsigned char holdout_mask;
	unsigned char target_mask;

	switch( target_flag ) {

		case DICE_GLOBAL_CLOCK | DICE_TRACK_CLOCK:
		target_mask = holdout_mask = target_dice->attr_MISC;

		CLEAR_MASK( holdout_mask, target_flag ); SET_APPLY_MASK( target_mask, in_value ); TOGGLE_MASK( target_mask, in_value );
		target_dice->attr_MISC = SET_MASK( target_mask, holdout_mask );
		break;
	}
}



// Gets the selected clock type
Trackstruct * Dice_get_global_clock_track() {

	return DICE_bank->Track[MATRIX_NROF_ROWS - 1];
}


// Gets the selected clock type
unsigned char Dice_get_selected_clock_type( Trackstruct* target_dice ) {

	return Dice_get_MISC( target_dice, DICE_TRACK_CLOCK ) ? DICE_TRACK_CLOCK : DICE_GLOBAL_CLOCK;
}


// Get a pointer to selected dice TEMPOMUL helper
unsigned char * Dice_get_TEMPOMUL_ptr( Trackstruct* target_dice, unsigned char clock ) {

	return clock == DICE_TRACK_CLOCK ?  &target_dice->attr_TEMPOMUL : &Dice_get_global_clock_track( DICE_bank )->attr_TEMPOMUL;
}


// Get value of dice TEMPOMUL
unsigned char Dice_get_TEMPOMUL( Trackstruct* target_dice, unsigned char clock ) {

	unsigned char * TEMPOMUL_ptr = Dice_get_TEMPOMUL_ptr( target_dice, clock );
	return TEMPOMUL_ptr ? *TEMPOMUL_ptr : 0;
}


// Set value of dice TEMPOMUL
void Dice_set_TEMPOMUL( Trackstruct* target_dice, const unsigned char value, unsigned char clock ) {

	unsigned char * TEMPOMUL_ptr = Dice_get_TEMPOMUL_ptr( target_dice, clock );
	if( TEMPOMUL_ptr ) {
		*TEMPOMUL_ptr = value;
	}
}


// Get a pointer to selected dice TEMPOMUL_SKIP helper
unsigned char * Dice_get_TEMPOMUL_SKIP_ptr( Trackstruct* target_dice, unsigned char clock ) {

	return clock == DICE_TRACK_CLOCK ? &target_dice->attr_TEMPOMUL_SKIP : &Dice_get_global_clock_track( DICE_bank )->attr_TEMPOMUL_SKIP;
}


// Get value of dice TEMPOMUL_SKIP
unsigned char Dice_get_TEMPOMUL_SKIP( Trackstruct* target_dice, unsigned char clock ) {

	unsigned char * TEMPOMUL_SKIP_ptr = Dice_get_TEMPOMUL_SKIP_ptr( target_dice, clock );
	return TEMPOMUL_SKIP_ptr ? *TEMPOMUL_SKIP_ptr : 0;
}


// Set value of dice TEMPOMUL_SKIP
void Dice_set_TEMPOMUL_SKIP( Trackstruct* target_dice, const unsigned char value, unsigned char clock ) {

	unsigned char * TEMPOMUL_SKIP_ptr = Dice_get_TEMPOMUL_SKIP_ptr( target_dice, clock );
	if( TEMPOMUL_SKIP_ptr ) {
		*TEMPOMUL_SKIP_ptr = value;
	}
}


// Get a pointer to Dice attribute helper
signed int * Dice_get_attribute_ptr( Trackstruct* target_dice, const unsigned char attribute ) {

	#ifdef NEMO
	// Get  pointer.
	switch( attribute ) {
		case NEMO_ATTR_PITCH: 	return (signed int *)&target_dice->attr_PIT;
		case NEMO_ATTR_VELOCITY:return (signed int *)&target_dice->attr_VEL;
		case NEMO_ATTR_LENGTH: 	return (signed int *)&target_dice->attr_LEN;
		case NEMO_ATTR_START: 	return (signed int *)&target_dice->attr_STA;
		case NEMO_ATTR_AMOUNT: 	return (signed int *)&target_dice->attr_AMT;
		case NEMO_ATTR_GROOVE: 	return (signed int *)&target_dice->attr_GRV;
		case NEMO_ATTR_MIDICC: 	return (signed int *)&target_dice->attr_MCC;
	}
	#else
	switch( attribute ) {
		case ATTR_PITCH: 	return (signed int *)&target_dice->attr_PIT;
		case ATTR_VELOCITY: return (signed int *)&target_dice->attr_VEL;
		case ATTR_LENGTH: 	return (signed int *)&target_dice->attr_LEN;
		case ATTR_START: 	return (signed int *)&target_dice->attr_STA;
		case ATTR_AMOUNT: 	return (signed int *)&target_dice->attr_AMT;
		case ATTR_GROOVE: 	return (signed int *)&target_dice->attr_GRV;
		case ATTR_MIDICC: 	return (signed int *)&target_dice->attr_MCC;
	}
	#endif
	return NULL;
}


// Get a pointer to Dice attribute by selected attribute helper
signed int * Dice_get_selected_attribute_ptr( Trackstruct* target_dice ) {

	return Dice_get_attribute_ptr( target_dice, target_dice->attr_STATUS );
}


// Get value of Dice attribute by attribute
signed int Dice_get_attribute( Trackstruct* target_dice, const unsigned char attribute ) {

	signed int * attribute_ptr = Dice_get_attribute_ptr( target_dice, attribute );
	return attribute_ptr ? *attribute_ptr : 0;
}


// Get a value of Dice selected attribute
signed int Dice_get_selected_attribute( Trackstruct* target_dice ) {

	return Dice_get_attribute( target_dice, target_dice->attr_STATUS );
}


// Set value of Dice attribute by attribute
void Dice_set_attribute( Trackstruct* target_dice, const unsigned char attribute, const signed int value ) {

	signed int * attribute_ptr = Dice_get_attribute_ptr( target_dice, attribute );
	if( attribute_ptr ) {
		*attribute_ptr = value;
	}
}


// Set a value of Dice selected attribute
void Dice_set_selected_attribute( Trackstruct* target_dice, const signed int value ) {

	Dice_set_attribute( target_dice, target_dice->attr_STATUS, value );
}


// Get a pointer to Dice factor by attribute helper
unsigned char * Dice_get_factor_ptr( Trackstruct* target_dice, const unsigned char attribute ) {

	#ifdef NEMO
	// Get factor pointer.
	switch( attribute ) {
		case NEMO_ATTR_PITCH: 	return &target_dice->PIT_factor;
		case NEMO_ATTR_VELOCITY:return &target_dice->VEL_factor;
		case NEMO_ATTR_LENGTH: 	return &target_dice->LEN_factor;
		case NEMO_ATTR_START: 	return &target_dice->STA_factor;
		case NEMO_ATTR_AMOUNT: 	return &target_dice->AMT_factor;
		case NEMO_ATTR_GROOVE: 	return &target_dice->GRV_factor;
		case NEMO_ATTR_MIDICC: 	return &target_dice->MCC_factor;
	}
	#else
	switch( attribute ) {
		case ATTR_PITCH: 	return &target_dice->PIT_factor;
		case ATTR_VELOCITY: return &target_dice->VEL_factor;
		case ATTR_LENGTH: 	return &target_dice->LEN_factor;
		case ATTR_START: 	return &target_dice->STA_factor;
		case ATTR_AMOUNT: 	return &target_dice->AMT_factor;
		case ATTR_GROOVE: 	return &target_dice->GRV_factor;
		case ATTR_MIDICC: 	return &target_dice->MCC_factor;
	}
	#endif
	return NULL;
}


// Get a pointer to Dice factor by selected attribute helper
unsigned char * Dice_get_selected_factor_ptr( Trackstruct* target_dice ) {

	return Dice_get_factor_ptr( target_dice, target_dice->attr_STATUS );
}


// Get value of Dice factor by attribute
unsigned char Dice_get_factor( Trackstruct* target_dice, const unsigned char attribute ) {

	signed char * factor_ptr = Dice_get_factor_ptr( target_dice, attribute );
	return factor_ptr ? *factor_ptr : 0;
}


// Get value of Dice factor by selected attribute
unsigned char Dice_get_selected_factor( Trackstruct* target_dice ) {

	return Dice_get_factor( target_dice, target_dice->attr_STATUS );
}


// Set value of Dice factor by attribute
void Dice_set_factor( Trackstruct* target_dice, const unsigned char attribute, const unsigned char value ) {

	unsigned char * factor_ptr = Dice_get_factor_ptr( target_dice, attribute );
	if( factor_ptr ) {
		*factor_ptr = value;
	}
}


// Set value of Dice factor by selected attribute
void Dice_set_selected_factor( Trackstruct* target_dice, const unsigned char value ) {

	return Dice_set_factor( target_dice, target_dice->attr_STATUS, value );
}

// Get value if Dice flow by attribute
DiceFlowAttributes Dice_get_flow_attributes( const unsigned char attribute ) {

	DiceFlowAttributes  flow_attributes;

	flow_attributes.attr_MAX = 0;
	flow_attributes.attr_SCALE = 1;
	flow_attributes.attr_NEUTRAL = 0;
	flow_attributes.attr_INTERVAL = FACTOR_INTERVAL_TYPE_DEFAULT;

	#ifdef NEMO
	switch( attribute ) {
		case NEMO_ATTR_PITCH:
			flow_attributes.attr_MAX = STEP_MAX_PITCH;
			flow_attributes.attr_NEUTRAL = PIT_FACTOR_NEUTRAL_VALUE;
			flow_attributes.attr_INTERVAL = FACTOR_INTERVAL_TYPE_SEMITONE;
			break;

		case NEMO_ATTR_VELOCITY:
			flow_attributes.attr_MAX = STEP_MAX_VELOCITY;
			flow_attributes.attr_NEUTRAL = VEL_FACTOR_NEUTRAL_VALUE;
			break;

		case NEMO_ATTR_LENGTH:
			flow_attributes.attr_MAX = STEP_MAX_LENGTH;
			flow_attributes.attr_NEUTRAL = LEN_FACTOR_NEUTRAL_VALUE;
			break;

		case NEMO_ATTR_START:
			flow_attributes.attr_MAX = STEP_MAX_START;
			flow_attributes.attr_SCALE = 8;
			flow_attributes.attr_NEUTRAL = STA_FACTOR_NEUTRAL_VALUE;
			break;

		case NEMO_ATTR_AMOUNT:
			flow_attributes.attr_MAX = STEP_MAX_AMOUNT;
			flow_attributes.attr_NEUTRAL = AMT_FACTOR_NEUTRAL_VALUE;
			break;

		case NEMO_ATTR_GROOVE:
			flow_attributes.attr_MAX = STEP_MAX_GROOVE;
			flow_attributes.attr_NEUTRAL = GRV_FACTOR_NEUTRAL_VALUE;
			break;

		case NEMO_ATTR_MIDICC:
			flow_attributes.attr_MAX = STEP_MAX_MIDICC;
			flow_attributes.attr_NEUTRAL = MCC_FACTOR_NEUTRAL_VALUE;
			break;
	}

	#else
	switch( attribute ) {

		case ATTR_PITCH:
			flow_attributes.attr_MAX = STEP_MAX_PITCH;
			flow_attributes.attr_NEUTRAL = PIT_FACTOR_NEUTRAL_VALUE;
			flow_attributes.attr_INTERVAL = FACTOR_INTERVAL_TYPE_SEMITONE;
			break;

		case ATTR_VELOCITY:
			flow_attributes.attr_MAX = STEP_MAX_VELOCITY;
			flow_attributes.attr_NEUTRAL = VEL_FACTOR_NEUTRAL_VALUE;
			break;

		case ATTR_LENGTH:
			flow_attributes.attr_MAX = STEP_MAX_LENGTH;
			flow_attributes.attr_NEUTRAL = LEN_FACTOR_NEUTRAL_VALUE;
			break;

		case ATTR_START:
			flow_attributes.attr_MAX = STEP_MAX_START;
			flow_attributes.attr_NEUTRAL = STA_FACTOR_NEUTRAL_VALUE;
			break;

		case ATTR_AMOUNT:
			flow_attributes.attr_MAX = STEP_MAX_AMOUNT;
			flow_attributes.attr_NEUTRAL = AMT_FACTOR_NEUTRAL_VALUE;
			break;

		case ATTR_GROOVE:
			flow_attributes.attr_MAX = STEP_MAX_GROOVE;
			flow_attributes.attr_NEUTRAL = GRV_FACTOR_NEUTRAL_VALUE;
			break;

		case ATTR_MIDICC:
			flow_attributes.attr_MAX = STEP_MAX_MIDICC;
			flow_attributes.attr_NEUTRAL = MCC_FACTOR_NEUTRAL_VALUE;
			break;
	}
	#endif
	return flow_attributes;
}


// Get value if Dice flow by attribute
signed char Dice_get_flow_attribute_value( const unsigned char attribute, const unsigned char col ) {

	signed char flow_value = 0;

	#ifdef NEMO
	switch( attribute ) {
		case NEMO_ATTR_PITCH:
			flow_value = DICE_bank->Step[SEL_DICE_BANK][col]->attr_PIT; break;
		case NEMO_ATTR_VELOCITY:
			flow_value = DICE_bank->Step[SEL_DICE_BANK][col]->attr_VEL; break;
		case NEMO_ATTR_LENGTH:
			flow_value = DICE_bank->Step[SEL_DICE_BANK][col]->attr_LEN; break;
		case NEMO_ATTR_START:
			flow_value = DICE_bank->Step[SEL_DICE_BANK][col]->attr_STA; break;
		case NEMO_ATTR_AMOUNT:
			flow_value = DICE_bank->Step[SEL_DICE_BANK][col]->attr_AMT; break;
		case NEMO_ATTR_GROOVE:
			flow_value = DICE_bank->Step[SEL_DICE_BANK][col]->phrase_num; break;
		case NEMO_ATTR_MIDICC:
			flow_value = DICE_bank->Step[SEL_DICE_BANK][col]->attr_MCC; break;
	}

	#else

	switch( attribute ) {

		case ATTR_PITCH:
			flow_value = DICE_bank->Step[SEL_DICE_BANK][col]->attr_PIT; break;
		case ATTR_VELOCITY:
			flow_value = DICE_bank->Step[SEL_DICE_BANK][col]->attr_VEL; break;
		case ATTR_LENGTH:
			flow_value = DICE_bank->Step[SEL_DICE_BANK][col]->attr_LEN; break;
		case ATTR_START:
			flow_value = DICE_bank->Step[SEL_DICE_BANK][col]->attr_STA; break;
		case ATTR_AMOUNT:
			flow_value = DICE_bank->Step[SEL_DICE_BANK][col]->attr_AMT; break;
		case ATTR_GROOVE:
			flow_value = DICE_bank->Step[SEL_DICE_BANK][col]->phrase_num; break;
		case ATTR_MIDICC:
			flow_value = DICE_bank->Step[SEL_DICE_BANK][col]->attr_MCC; break;
	}
	#endif
	return flow_value;
}


// Is the flow attribute a flat offset
bool Dice_is_flow_attribute_flat( const unsigned char attribute ) {

	signed char a = Dice_get_flow_attribute_value( attribute, 0 );
	signed char b = Dice_get_flow_attribute_value( attribute, 7 );
	signed char c = Dice_get_flow_attribute_value( attribute, 15 );

	return !( ( a ^ b ) | ( b ^ c) );
}


// Is Dice tempo selected
bool Dice_is_edit_tempo( Trackstruct* target_dice ) {

	return target_dice->attr_MISC & 0x3;
}


// Is grid row diced
bool Dice_is_grid_row_diced( Trackstruct* target_dice, unsigned char row ) {

	return CHECK_BIT( DICE_bank->trackSelection, row );
}


// Is dice loop used to quantize Dice actions i.e. switch slice
bool Dice_is_quantizer( Trackstruct* target_dice ) {
	return CHECK_BIT( DICE_bank->editorMode, DICE_QUANT );
}
