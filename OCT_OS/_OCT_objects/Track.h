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



#include "../_OCT_objects/Runtime.h"






// Used to clear the track soft factors, used at runtime
void Track_soft_init( Trackstruct* target_track ){

	target_track->attr_VEL 	= TRACK_DEF_VELOCITY;
//	target_track->attr_PIT 	= TRACK_DEF_PITCH;
	target_track->attr_LEN 	= TRACK_DEF_LENGTH;
	target_track->attr_STA 	= TRACK_DEF_START;
	target_track->attr_POS	= TRACK_DEF_POSITION;
	target_track->attr_DIR	= TRACK_DEF_DIRECTION;
	target_track->attr_AMT	= TRACK_DEF_AMOUNT;
	target_track->attr_GRV	= TRACK_DEF_GROOVE;
	target_track->attr_MCC	= TRACK_DEF_MIDICC;
	target_track->attr_MCH	= TRACK_DEF_MIDICH;

	target_track->scale_pitch_offset			= 0;
	target_track->lead_pitch_offset				= 0;

	target_track->attr_CCAMT		= 0;
	target_track->attr_TEMPOMUL			= 1;
	target_track->attr_TEMPOMUL_SKIP 	= 0;
	target_track->attr_MISC      	= (1 << 2);		// sets the chord bit
	#ifdef FEATURE_ENABLE_KEYB_TRANSPOSE
	target_track->attr_EMISC		= 0;
	target_track->attr_STATUS		= TRACK_DEF_MIDITCH;
	target_track->attr_GST 			= TRACK_DEF_PITCH;
	#endif
	
	#ifdef FEATURE_ENABLE_SONG_UPE
	target_track->ctrl_offset = 0;
	#endif
		#ifdef EVENTS_FACTORIZED
	target_track->event_max[ATTR_VELOCITY]	= TRACK_MAX_VELFACTOR;
		#endif // EVENTS_FACTORIZED
		#ifdef EVENTS_ABSOLUTE
	target_track->event_max[ATTR_VELOCITY]	= TRACK_MAX_VELOCITY;
		#endif // EVENTS_ABSOLUTE
	target_track->event_offset[ATTR_VELOCITY] = 0;

		#ifdef EVENTS_FACTORIZED
	target_track->event_max[ATTR_PITCH]		= TRACK_MAX_PITFACTOR;
		#endif // EVENTS_FACTORIZED
		#ifdef EVENTS_ABSOLUTE
	target_track->event_max[ATTR_PITCH]		= TRACK_MAX_PITCH;
		#endif // EVENTS_ABSOLUTE

	target_track->event_offset[ATTR_PITCH]	= 0;


	target_track->event_max[ATTR_LENGTH]	= TRACK_DEF_RANGE_LEN;
	target_track->event_offset[ATTR_LENGTH]	= 0;

	target_track->event_max[ATTR_START]		= TRACK_DEF_RANGE_STA;
	target_track->event_offset[ATTR_START]	= 0;

	target_track->event_max[ATTR_DIRECTION]	= TRACK_DEF_RANGE_DIR;
	target_track->event_offset[ATTR_DIRECTION]	= 0;

	target_track->event_max[ATTR_POSITION]	= TRACK_DEF_RANGE_POS;
	target_track->event_offset[ATTR_POSITION]	= 0;

	target_track->event_max[ATTR_AMOUNT]	= TRACK_DEF_RANGE_AMT;
	target_track->event_offset[ATTR_AMOUNT]	= 0;

	target_track->event_max[ATTR_GROOVE]	= TRACK_DEF_RANGE_GRV;
	target_track->event_offset[ATTR_GROOVE]	= 0;

	target_track->event_max[ATTR_MIDICC]	= TRACK_DEF_RANGE_MCC;
	target_track->event_offset[ATTR_MIDICC]	= 0;

	target_track->event_max[ATTR_MIDICH]	= TRACK_DEF_RANGE_MCH;
	target_track->event_offset[ATTR_MIDICH]	= 0;


	// For the MAP Attributes only: Index into the factoring array
	target_track->VEL_factor				= TRACK_DEF_VELFACTOR;
	target_track->PIT_factor				= TRACK_DEF_PITFACTOR;
	target_track->LEN_factor				= TRACK_DEF_LENFACTOR;
	target_track->STA_factor				= TRACK_DEF_STAFACTOR;
	target_track->AMT_factor				= TRACK_DEF_AMTFACTOR;
	target_track->GRV_factor				= TRACK_DEF_GRVFACTOR;
	target_track->MCC_factor				= TRACK_DEF_MCCFACTOR;

	target_track->program_change			= TRACK_DEF_PROGRAMCHANGE;
	target_track->bank_change				= TRACK_DEF_BANKCHANGE;

	target_track->CC_resolution				= 1;

	// Set the default void value for hyper
	target_track->hyper						= 0xF0;
	target_track->gatePosition				= 0;

// Initialise the flow shape indexes
#ifdef NEMO
	target_track->flow_shape[NEMO_ATTR_PITCH] 		= 0;
	target_track->flow_shape[NEMO_ATTR_VELOCITY] 	= 0;
	target_track->flow_shape[NEMO_ATTR_LENGTH] 		= 0;
	target_track->flow_shape[NEMO_ATTR_START] 		= 0;
	target_track->flow_shape[NEMO_ATTR_AMOUNT] 		= 0;
	target_track->flow_shape[NEMO_ATTR_GROOVE] 		= 0;
	target_track->flow_shape[NEMO_ATTR_MIDICC] 		= 0;
#endif
}


//__________________________________________________________________________________________
//
// This implements the construction function for Track
void Track_hard_init( Trackstruct* target_track, trackid_t trackId ){

	unsigned char i=0;

	target_track->trackId = trackId;

	// Init the trigger repository
	for ( i=0; i<10; i++ ){
		target_track->trigger[i] = 0;
	}
	target_track->frame_ndx			= 0;

	// Init the chain data for 10 unchained rows: each track is by itself
	target_track->chain_data[HEAD] = target_track;
	target_track->chain_data[NEXT] = target_track;
	target_track->chain_data[PREV] = target_track;
	target_track->chain_data[PLAY] = target_track;

	// Initialization sequence
	#ifdef FEATURE_ENABLE_KEYB_TRANSPOSE
	target_track->attr_STATUS 		= TRACK_DEF_MIDITCH;
	#else
	target_track->attr_STATUS 		= OFF;
	#endif
	target_track->attr_LOCATOR		= 0;
	target_track->TTC				= 0;


	// Initialize the rest of the track parameters
	Track_soft_init( target_track );
}



//__________________________________________________________________________________________
//
// Gets the data from the MISC attribute, according to given flag
unsigned char Track_get_MISC( 	Trackstruct* target_track,
								unsigned char target_flag 	){
	unsigned char result=0;
	#ifdef FEATURE_ENABLE_SONG_UPE
	unsigned char ctrl=target_track->attr_MISC & 1;

	switch( target_flag ){

		case CONTROL_BIT:
			return ctrl;

		case CHORD_BIT:
			// Return the value of the CHORD value in the target track
			result = (target_track->attr_MISC & 0x1C) >> 2;
			break;

		case EFF_BIT:
			// Check the status of the EFF bits in the target track
			result = (target_track->attr_MISC & 0xC0) >> 6;
			break;
	}
	if (ctrl == ON) return OFF;	
	#else
	switch( target_flag ){

		case CHORD_BIT:
			// Return the value of the CHORD value in the target track
			result = (target_track->attr_MISC & 0x1C) >> 2;
			break;

		case EFF_BIT:
			// Check the status of the EFF bits in the target track
			result = (target_track->attr_MISC & 0xC0) >> 6;
			break;
	}	
	#endif
	return result;
}



//__________________________________________________________________________________________
//
// Sets the data into the MISC attribute, according to given flag
void Track_set_MISC( 			Trackstruct* target_track,
								unsigned char target_flag,
								unsigned char in_value 		){

	unsigned char temp = target_track->attr_MISC;

	switch( target_flag ){

		case CHORD_BIT:
			// Erase the three chord bits
			temp &= 0xE3;
			// Set the value of the CHORD value in the target track
			target_track->attr_MISC = ( temp | (in_value << 2) );
			break;

		case EFF_BIT:
			// Erase the EFF bit
			temp &= 0x3F;
			// Set the new EFF bit
			target_track->attr_MISC = ( temp | (in_value << 6) );
			break;
	}
}


