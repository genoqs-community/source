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





// Returns the value of the new event offset based on given parameters
unsigned char compute_event_offset( unsigned char current_value,
									unsigned char min_value,
									unsigned char max_value,
									unsigned char current_offset,
									unsigned char max_offset,
									signed char event_value,
									unsigned char event_val_rnd ){

	unsigned char 	headroom = 0;
	unsigned char 	result = 0;
	signed short 	new_offset = 0;

	// Calculate the headroom for the operation
	headroom = my_min_value( max_offset, max_value - current_value );

	// Attempt to calculate new offset
	new_offset = current_offset + event_value;

	// Can happen if the event value is negative - recalculate the new offset
	if ( new_offset < 0 ){

		// Recalculate the new offset: subtract the event from the max offset
		new_offset = max_offset	- abs( current_offset + event_value ) +1;
	}

	// Make sure the result falls withing valid value bounds
	result = normalize( new_offset % (headroom + 1), min_value, max_value );

	// Randomize result if needed - return a vaue between 0 and the actual result.
	if ( event_val_rnd == TRUE ){

		result = rand() % (result+1);
	}

	return result;
}




// Perform the event programmed in the in step onto the given track
void perform_step_event( 	Stepstruct* in_step,
							Trackstruct* target_track,
							Pagestruct* target_page,
							unsigned char in_row,
							unsigned char locator){

	unsigned char direction = 0;

	unsigned char* target_val = NULL;
	unsigned char current_value 	= 0;
	unsigned char min_value		 	= 0;
	unsigned char max_value			= 0;
	unsigned char current_offset	= 0;
	unsigned char max_offset		= 0;
	signed char event_value			= 0;
	unsigned char event_val_rnd		= FALSE;

	signed char dice_amount_offset = 0;

	#ifdef FEATURE_ENABLE_DICE
	#ifdef NEMO
	dice_amount_offset = dice_attr_flow_offset( target_page, NEMO_ATTR_AMOUNT, in_row );
	#else
	dice_amount_offset = dice_attr_flow_offset( target_page, ATTR_AMOUNT, in_row );
	#endif
	#endif

	event_value =
		normalize( ( ( in_step->attr_AMT
			  * Track_AMT_factor[	target_track->AMT_factor
				 					+ target_track->event_offset[ATTR_AMOUNT] ] )
			/ AMT_FACTOR_NEUTRAL_VALUE ) + dice_amount_offset, STEP_MIN_AMOUNT, STEP_MAX_AMOUNT );

	if( event_value == 0 ) {
		// Track fractions of event value
	}
	// Continue with 0 / void events, because they set the event offset to 0.

	// POSITION attribute requires special handling
	if (	(( in_step->event_data & 0x0F) + 1 ) == ATTR_POSITION ){

		// Set the direction
		direction = INC;
		if ( event_value < 0 ) direction = DEC;

		Page_wrap_track( 	target_page,
							in_row,
							direction,
							abs( event_value ) %16 );

		// We are done for POSITION event, so return - other attributes are handled below
		return;
	}


	// All other attribute events - make the value assignments for final number crunching..
	switch( (in_step->event_data & 0x0F) + 1 ){

		#ifdef NEMO
			case NEMO_ATTR_VELOCITY:
		#else
			case ATTR_VELOCITY:
		#endif
			target_val 		= &target_track->event_offset[ATTR_VELOCITY];

			#ifdef EVENTS_FACTORIZED
			current_value	= target_track->VEL_factor;
			min_value		= TRACK_MIN_VELFACTOR;
			max_value		= TRACK_MAX_VELFACTOR;
			#endif // EVENTS_FACTORIZED

			#ifdef EVENTS_ABSOLUTE
			current_value	= target_track->attr_VEL;
			min_value		= TRACK_MIN_VELOCITY;
			max_value		= TRACK_MAX_VELOCITY;
			#endif // EVENTS_ABSOLUTE

			current_offset	= target_track->event_offset[ATTR_VELOCITY];
			max_offset		= target_track->event_max[ATTR_VELOCITY];
			break;

		#ifdef NEMO
			case NEMO_ATTR_PITCH:
		#else
			case ATTR_PITCH:
		#endif
			target_val 		= &target_track->event_offset[ATTR_PITCH];

			#ifdef EVENTS_FACTORIZED
			current_value	= target_track->PIT_factor;
			min_value		= TRACK_MIN_PITFACTOR;
			max_value		= TRACK_MAX_PITFACTOR;
			#endif // EVENTS_FACTORIZED

			#ifdef EVENTS_ABSOLUTE
			current_value	= target_track->attr_PIT;
			min_value		= TRACK_MIN_PITCH;
			max_value		= TRACK_MAX_PITCH;
			#endif // EVENTS_ABSOLUTE

			current_offset	= target_track->event_offset[ATTR_PITCH];
			max_offset		= target_track->event_max[ATTR_PITCH];
			break;



		case ATTR_LENGTH:

			target_val 		= &target_track->event_offset[ATTR_LENGTH];
			current_value	= target_track->LEN_factor;
			min_value		= TRACK_MIN_LENFACTOR;
			max_value		= TRACK_MAX_LENFACTOR;
			current_offset	= target_track->event_offset[ATTR_LENGTH];
			max_offset		= target_track->event_max[ATTR_LENGTH];

			break;



		case ATTR_START:

			target_val 		= &target_track->event_offset[ATTR_START];
			current_value	= target_track->STA_factor;
			min_value		= TRACK_MIN_STAFACTOR;
			max_value		= TRACK_MAX_STAFACTOR;
			current_offset	= target_track->event_offset[ATTR_START];
			max_offset		= target_track->event_max[ATTR_START];

			break;



		case ATTR_DIRECTION:

			target_val 		= &target_track->event_offset[ATTR_DIRECTION];
			current_value	= target_track->attr_DIR;
			min_value		= TRACK_MIN_DIRECTION;
			max_value		= TRACK_MAX_DIRECTION;
			current_offset	= target_track->event_offset[ATTR_DIRECTION];
			max_offset		= target_track->event_max[ATTR_DIRECTION];

			break;



		case ATTR_AMOUNT:

			target_val 		= &target_track->event_offset[ATTR_AMOUNT];
			current_value	= target_track->AMT_factor;
			min_value		= TRACK_MIN_AMTFACTOR;
			max_value		= TRACK_MAX_AMTFACTOR;
			current_offset	= target_track->event_offset[ATTR_AMOUNT];
			max_offset		= target_track->event_max[ATTR_AMOUNT];

			break;



		case ATTR_GROOVE:

			target_val 		= &target_track->event_offset[ATTR_GROOVE];
			current_value	= target_track->GRV_factor;
			min_value		= TRACK_MIN_GRVFACTOR;
			max_value		= TRACK_MAX_GRVFACTOR;
			current_offset	= target_track->event_offset[ATTR_GROOVE];
			max_offset		= target_track->event_max[ATTR_GROOVE];

			break;



		case ATTR_MIDICC:

			target_val 		= &target_track->event_offset[ATTR_MIDICC];
			current_value	= target_track->MCC_factor;
			min_value		= TRACK_MIN_MCCFACTOR;
			max_value		= TRACK_MAX_MCCFACTOR;
			current_offset	= target_track->event_offset[ATTR_MIDICC];
			max_offset		= target_track->event_max[ATTR_MIDICC];

			break;



		case ATTR_MIDICH:

			target_val 		= &target_track->event_offset[ATTR_MIDICH];
			current_value	= 0;
			min_value		= TRACK_MIN_MIDICH - 1;
			max_value		= TRACK_MAX_MIDICH - 1;
			current_offset	= target_track->event_offset[ATTR_MIDICH];
			max_offset		= target_track->event_max[ATTR_MIDICH];

			break;
	}

#if GROEF
	// Steps that have a non-zero groove parameter experience a random event value
	if ( in_step->attr_GRV != 0 ){
		event_val_rnd = TRUE;
	}
#endif
//	// Compute the event value considering its factorization
//	if ( in_step->attr_AMT == def_RANDOM_EVENT ){
//			// Steps experience a random event value
//			event_val_rnd = TRUE;
//		}
//	}

	// An event value of 0 resets the event offset to 0.
	if ( ( event_value == 0 )
	&& ( ( ( in_step->event_data & 0x0F) + 1 ) != ATTR_AMOUNT ) ){

		// As suggested by Pieter
		*target_val = 0;
	}
	else{

		// Number-crunch the new offset value and update the event offset.
		*target_val = compute_event_offset(	current_value, min_value, max_value,
										current_offset, max_offset, event_value, event_val_rnd );
	}
}




