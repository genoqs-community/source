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



//
// Functions pertaining to general encoder operation
//
// genoQs Machines
// 01.10.2005



// QUICKTURN TIMER HANDLING
void start_QUICKTURN_TIMER(){

	// Refill multiplier
	ROTARY_QUICKTURN_MULTIPLIER = RQM_MAX;

	// Setup alarm to decrement it
	cyg_alarm_initialize(	quickturnAlarm_hdl,
							cyg_current_time() + (RQM_INTERVAL),
							(RQM_INTERVAL) );
}

// New version of modify_parameter with QUICKTURN
void modify_parameter(	  signed char* 	in_parameter,
						unsigned int 	min_val,
						unsigned int 	max_val,
						unsigned char 	direction,
						unsigned char 	QTT_status,
						unsigned char 	WRAP_status	) {

	unsigned char i = 0;
	unsigned char temp = 0;

//d_iag_printf( "rot &:%x, val:%d, min:%d max:%d dir:%d RQM:%d\n",
//					(int) in_parameter,
//					(unsigned int) *in_parameter,
//					min_val,
//					max_val,
//					direction,
//					ROTARY_QUICKTURN_MULTIPLIER
//					);

	// Repeat according to quickturn indicator
	for (i=0; i < ROTARY_QUICKTURN_MULTIPLIER; i++) {

		// Refresh the temp value
		temp = (unsigned char) *in_parameter;

		// Execute rotary action
		switch (direction) {

			case INC:

				if (temp < max_val) {
//				if (*in_parameter < max_val) {
					*in_parameter = *in_parameter + 1;
				}
				else if ( WRAP_status == WRAPPED ){
					*in_parameter = min_val;
				}
				break;

			case DEC:

				if (temp > min_val) {
//				if (*in_parameter > min_val) {
					*in_parameter = *in_parameter - 1;
				}
				else if ( WRAP_status == WRAPPED ){
					*in_parameter = max_val;
				}
				break;
		}
	}

	// ACTIVATE THE MULTIPLIER - but not in the negative modify mode
	if (QTT_status == ON) {
		start_QUICKTURN_TIMER();
	}
}


// New version of modify_parameter with QUICKTURN
void modify_signed_parameter(	signed char* 	in_parameter,
								signed int 		min_val,
								signed int 		max_val,
								unsigned char 	direction,
								unsigned char 	QTT_status,
								unsigned char 	WRAP_status	) {

	unsigned char i = 0;
	signed char temp = 0;

//	d_iag_printf( "Srot &:%x, val:%d, min:%d max:%d dir:%d\n",
//						(int) in_parameter,
//						*in_parameter,
//						min_val,
//						max_val,
//						direction
//						);

	// Repeat according to quickturn indicator
	for (i=0; i < ROTARY_QUICKTURN_MULTIPLIER; i++) {

		// Refresh the temp value
		temp =  *in_parameter;

		// Execute rotary action
		switch (direction) {

			case INC:

				if (temp < max_val) {
//				if (*in_parameter < max_val) {
					*in_parameter = *in_parameter + 1;
				}
				else if ( WRAP_status == WRAPPED ){
					*in_parameter = min_val;
				}
				break;

			case DEC:

				if (temp > min_val) {
//				if (*in_parameter > min_val) {
					*in_parameter = *in_parameter - 1;
				}
				else if ( WRAP_status == WRAPPED ){
					*in_parameter = max_val;
				}
				break;
		}
	}

	// ACTIVATE THE MULTIPLIER - but not in the negative modify mode
	if (QTT_status == ON) {
		start_QUICKTURN_TIMER();
	}
}







//
// SHIFT MAP of target_track for in_attribute in the given direction
//
void shiftAttributeMap( 	Pagestruct* target_page,
							unsigned int row,
							unsigned int in_attribute,
							unsigned char direction ){

	// Shifts once and wraps around track boundaries [1,16]
	unsigned int col=0;
	unsigned int buffer=0;
	unsigned int 	buffer_chord_data=0,
					buffer_chord_up=0,
					buffer_phrase_num=0,
					buffer_phrase_data=0,
					buffer_event_data=0,
					buffer_hyperTrackNdx=0x10;

	Stepstruct* thisStepPt;
	Stepstruct* nextStepPt;

	// Direction of shift movement
	switch( direction ){

		case DEC:
			// Store the first value in a buffer, to be later filled in the last position.
			buffer 				= *(ptr_of_step_attr_value( target_page, row, 0, in_attribute ));
			buffer_chord_data 	= target_page->Step[row][0]->chord_data;
			buffer_chord_up 	= target_page->Step[row][0]->chord_up;
			buffer_phrase_num 	= target_page->Step[row][0]->phrase_num;
			buffer_phrase_data 	= target_page->Step[row][0]->phrase_data;
			buffer_event_data 	= target_page->Step[row][0]->event_data;
			buffer_hyperTrackNdx = target_page->Step[row][0]->hyperTrack_ndx;

			// Copy everything to one column earlier.. except for last column
			for (col=0; col < MATRIX_NROF_COLUMNS-1; col++) {

				thisStepPt 	= target_page->Step[row][col];
				nextStepPt	= target_page->Step[row][col+1];

				switch( in_attribute ){

					case ATTR_STATUS:
						// Make sure we don't shift the event bit in status
						thisStepPt->attr_STATUS =
							 	( thisStepPt->attr_STATUS & (1 << 4) )
							|	( nextStepPt->attr_STATUS & 0xEF );
						break;

					case ATTR_PITCH:
						// Shift the chord data separately
						thisStepPt->chord_data 	= nextStepPt->chord_data;
						thisStepPt->chord_up 	= nextStepPt->chord_up;

						// Shift the phrase data
						// thisStepPt->phrase_num 	= nextStepPt->phrase_num;
						// thisStepPt->phrase_data = nextStepPt->phrase_data;

						// Shift the hyperTrack_ndx: needs both Step and Track update
						thisStepPt->hyperTrack_ndx = nextStepPt->hyperTrack_ndx;
						if ( thisStepPt->hyperTrack_ndx < MATRIX_NROF_ROWS ){
							// Make sure we don't touch invalid track indices
							target_page->Track[thisStepPt->hyperTrack_ndx]->hyper = (row << 4) | col;
						}

						// thisStepPt->event_data = nextStepPt->event_data;
						// Make sure we only shift the stepLenMul in event_data, no event attr
						thisStepPt->event_data =
							 	( thisStepPt->event_data & 0x0F )
							|	( nextStepPt->event_data & 0xF0 );

						// No break, fallthrough needed

					default:
						// Shift the attribute value with no changes
						*(ptr_of_step_attr_value( target_page, row, col, in_attribute )) =
							*(ptr_of_step_attr_value( target_page, row, col+1, in_attribute ));
						break;
				} // attribute switch
			} // Iterator columns 0-14;

			// Last column handler: put buffer value into the last place
			col = MATRIX_NROF_COLUMNS - 1;
			thisStepPt 	= target_page->Step[row][col];

			switch( in_attribute ){

				case ATTR_STATUS:
					// Make sure we don't shift the event bit in status
					thisStepPt->attr_STATUS =
							( thisStepPt->attr_STATUS & (1 << 4) ) 	// Old status bit
						|	( buffer & 0xEF );						// New other bits
					break;

				case ATTR_PITCH:
					// Shift the chord data separately
					thisStepPt->chord_data 	= 	buffer_chord_data;
					thisStepPt->chord_up 	=	buffer_chord_up;

					// Shift the phrase data
					// thisStepPt->phrase_num 	=	buffer_phrase_num;
					// thisStepPt->phrase_data =	buffer_phrase_data;

					// Shift the hypertrack data
					thisStepPt->hyperTrack_ndx = buffer_hyperTrackNdx;
					if ( thisStepPt->hyperTrack_ndx < MATRIX_NROF_ROWS ){
						// Make sure we don't touch invalid track indices
						target_page->Track[thisStepPt->hyperTrack_ndx]->hyper = (row << 4) | col;
					}

					// target_page->Step[row][col]->event_data = buffer_event_data;
					// Make sure we only shift the stepLenMul in event_data, no event attr
					thisStepPt->event_data =
						 	( thisStepPt->event_data  	& 0x0F )
						|	( buffer_event_data 		& 0xF0 );

					// No break, fallthrough needed

				default:
					// Shift the attribute value with no changes
					*(ptr_of_step_attr_value( target_page, row, col, in_attribute )) = buffer;
					break;
			} // attribute switch
			break;



		case INC:
			// Store last value in the buffer, to be filled into the first position
			buffer = *(ptr_of_step_attr_value( target_page, row, 15, in_attribute ));
			buffer_chord_data 	= target_page->Step[row][15]->chord_data;
			buffer_chord_up 	= target_page->Step[row][15]->chord_up;
			buffer_phrase_num 	= target_page->Step[row][15]->phrase_num;
			buffer_phrase_data 	= target_page->Step[row][15]->phrase_data;
			buffer_event_data 	= target_page->Step[row][15]->event_data;
			buffer_hyperTrackNdx = target_page->Step[row][15]->hyperTrack_ndx;

			// Shift all contents to the right
			for ( col=MATRIX_NROF_COLUMNS-1; col > 0; col-- ){

				thisStepPt 	= target_page->Step[row][col];
				nextStepPt	= target_page->Step[row][col-1];

				switch( in_attribute ){

					case ATTR_STATUS:
						thisStepPt->attr_STATUS =
							 	( thisStepPt->attr_STATUS & (1 << 4) )
							|	( target_page->Step[row][col-1]->attr_STATUS & 0xEF );
						break;

					case ATTR_PITCH:
						thisStepPt->chord_data 	=	nextStepPt->chord_data;
						thisStepPt->chord_up 	=	nextStepPt->chord_up;

						// thisStepPt->phrase_num 	=	nextStepPt->phrase_num;
						// thisStepPt->phrase_data =	nextStepPt->phrase_data;

						thisStepPt->hyperTrack_ndx = nextStepPt->hyperTrack_ndx;
						if ( thisStepPt->hyperTrack_ndx < MATRIX_NROF_ROWS ){
							// Make sure we don't touch invalid track indices
							target_page->Track[thisStepPt->hyperTrack_ndx]->hyper = (row << 4) | col;
						}

						// target_page->Step[row][col]->event_data =
						// target_page->Step[row][col-1]->event_data;
						// Make sure we only shift the stepLenMul in event_data, no event attr
						thisStepPt->event_data =
							 	( thisStepPt->event_data & 0x0F )
							|	( nextStepPt->event_data & 0xF0 );

						// No break, fallthrough needed

					default:
						*(ptr_of_step_attr_value( target_page, row, col, in_attribute ))
							= *(ptr_of_step_attr_value( target_page, row, col-1, in_attribute ));
						break;
				}
			} // Column iterator

			// Fill in the buffer value into the first place
			col = 0;
			thisStepPt 	= target_page->Step[row][col];

			switch( in_attribute ){

				case ATTR_STATUS:
					// Make sure we don't shift the event bit in status
					target_page->Step[row][0]->attr_STATUS =
							( buffer & 0xEF )
						|	( target_page->Step[row][0]->attr_STATUS & (1 << 4) );
					break;

				case ATTR_PITCH:
					// Only chord data here
					target_page->Step[row][0]->chord_data 	= buffer_chord_data;
					target_page->Step[row][0]->chord_up 	= buffer_chord_up;

					// Shift the phrase data - obsolote - done in another place.
					// target_page->Step[row][0]->phrase_num 	= buffer_phrase_num;
					// target_page->Step[row][0]->phrase_data 	= buffer_phrase_data;

					target_page->Step[row][0]->hyperTrack_ndx = buffer_hyperTrackNdx;
					if ( thisStepPt->hyperTrack_ndx < MATRIX_NROF_ROWS ){
						// Make sure we don't touch invalid track indices
						target_page->Track[thisStepPt->hyperTrack_ndx]->hyper = (row << 4) | col;
					}

					// target_page->Step[row][0]->event_data 	= buffer_event_data;
					// Make sure we only shift the stepLenMul in event_data, no event attr
					target_page->Step[row][0]->event_data =
						 	( target_page->Step[row][0]->event_data   & 0x0F )
						|	( buffer_event_data & 0xF0 );

					// No break, fallthrough intended

				default:
					// Default attribute value shift
					*(ptr_of_step_attr_value( target_page, row, 0, in_attribute )) = buffer;
					break;
			} // attribute switch

			break;

	} // Direction switch
}




// Apply the given rotary to the given Step
void apply_rotary_to_step( 	unsigned char rotNdx,
							unsigned char direction,
							Pagestruct* target_page,
							unsigned char row,
							unsigned char col 	){

	// Syntactic simplification
	Stepstruct* target_step = target_page->Step[row][col];

	// Modify the attribute value; normalize the rotNdx
	switch( rotNdx-1 ) {

		case VELOCITY:
			modify_signed_parameter(
				&target_step->attr_VEL,
				STEP_MIN_VELOCITY, STEP_MAX_VELOCITY, direction, OFF, FIXED );
			break;

		case PITCH:
			modify_signed_parameter(
				&target_step->attr_PIT,
				STEP_MIN_PITCH, STEP_MAX_PITCH, direction, OFF, FIXED );
			break;

		case LENGTH:
			modify_parameter(
				&target_step->attr_LEN,
// old			STEP_MIN_LENGTH, ((unsigned char)STEP_MAX_LENGTH), direction, OFF, FIXED );
				STEP_MIN_LENGTH, STEP_MAX_LENGTH, direction, OFF, FIXED );
			break;

		case START:
			modify_parameter(
				&target_step->attr_STA,
				STEP_MIN_START, STEP_MAX_START, direction, OFF, FIXED );
			break;

		case POSITION:
			PhraseEditStepFactor( target_step, direction );
			break;

		case DIRECTION:
//			PhraseEditStepType( target_step, direction );
			break;

		case AMOUNT:
			modify_signed_parameter(
				&target_step->attr_AMT,
				STEP_MIN_AMOUNT, STEP_MAX_AMOUNT, direction, OFF, FIXED );
			break;

		case GROOVE:
			PhraseEditStepNumber( target_step, direction );
			break;

		case MIDICC:
			modify_signed_parameter(
				&target_step->attr_MCC,
				STEP_MIN_MIDICC, STEP_MAX_MIDICC, direction, OFF, FIXED );
			break;

	} // switch( rotNDx-1 )

}


