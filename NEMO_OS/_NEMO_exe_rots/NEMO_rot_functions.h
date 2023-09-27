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







////
//// SHIFT MAP of target_track for in_attribute in the given direction
////
//void shiftAttributeMap( 	Pagestruct* target_page,
//							unsigned int row,
//							unsigned int in_attribute,
//							unsigned char direction ){
//
//	// Shifts once and wraps around track boundaries [1,16]
//	unsigned int col=0;
//	unsigned int buffer=0;
//	unsigned int buffer_chord=0;
//
//	// Direction of shift movement
//	switch( direction ){
//
//		case DEC:
//			// Store the first value in a buffer, to be later filled in the last position.
//			buffer 			= *(ptr_of_step_attr_value( target_page, row, 0, in_attribute ));
//			buffer_chord 	= target_page->Step[row][0]->chord_data;
//
//			// Copy everything to one column earlier.. except for last column
//			for (col=0; col < MATRIX_NROF_COLUMNS-1; col++) {
//
//				switch( in_attribute ){
//
//					case NEMO_ATTR_STATUS:
//						// Make sure we don't shift the event bit in status
//						target_page->Step[row][col]->attr_STATUS =
//							 	( target_page->Step[row][col]->attr_STATUS & status_hold_mask )
//							|	( target_page->Step[row][col+1]->attr_STATUS & status_apply_mask );
//						break;
//
//					case NEMO_ATTR_PITCH:
//						// Shift the chord data separately
//						target_page->Step[row][col]->chord_data =
//							target_page->Step[row][col+1]->chord_data;
//						// No break, fallthrough needed
//
//					default:
//						// Shift the attribute value with no changes
//						*(ptr_of_step_attr_value( target_page, row, col, in_attribute )) =
//							*(ptr_of_step_attr_value( target_page, row, col+1, in_attribute ));
//						break;
//				} // attribute switch
//			} // Iterator columns 0-14;
//
//			// Last column handler: put buffer value into the last place
//			col = MATRIX_NROF_COLUMNS - 1;
//
//			switch( in_attribute ){
//
//				case NEMO_ATTR_STATUS:
//					// Make sure we don't shift the event bit in status
//					target_page->Step[row][col]->attr_STATUS =
//							( target_page->Step[row][col]->attr_STATUS & status_hold_mask ) // Old status bit
//						|	( buffer & status_apply_mask );										// New other bits
//					break;
//
//				case NEMO_ATTR_PITCH:
//					// Shift the chord data separately
//					target_page->Step[row][col]->chord_data = buffer_chord;
//					// No break, fallthrough needed
//
//				default:
//					// Shift the attribute value with no changes
//					*(ptr_of_step_attr_value( target_page, row, col, in_attribute )) = buffer;
//					break;
//			} // attribute switch
//			break;
//
//
//
//		case INC:
//			// Store last value in the buffer, to be filled into the first position
//			buffer = *(ptr_of_step_attr_value( target_page, row, 15, in_attribute ));
//			buffer_chord = target_page->Step[row][15]->chord_data;
//
//			// Shift all contents to the right
//			for ( col=MATRIX_NROF_COLUMNS-1; col > 0; col-- ){
//
//				switch( in_attribute ){
//
//					case NEMO_ATTR_STATUS:
//						target_page->Step[row][col]->attr_STATUS =
//							 	( target_page->Step[row][col]->attr_STATUS & status_hold_mask )
//							|	( target_page->Step[row][col-1]->attr_STATUS & status_apply_mask );
//						break;
//
//					case NEMO_ATTR_PITCH:
//						target_page->Step[row][col]->chord_data =
//							target_page->Step[row][col-1]->chord_data;
//						// No break, fallthrough needed
//
//					default:
//						*(ptr_of_step_attr_value( target_page, row, col, in_attribute ))
//							= *(ptr_of_step_attr_value( target_page, row, col-1, in_attribute ));
//						break;
//				}
//			} // Column iterator
//
//			// Fill in the buffer value into the first place
//			col = 0;
//
//			switch( in_attribute ){
//
//				case NEMO_ATTR_STATUS:
//					// Make sure we don't shift the event bit in status
//					target_page->Step[row][0]->attr_STATUS =
//							( buffer & status_apply_mask )
//						|	( target_page->Step[row][0]->attr_STATUS & status_hold_mask );
//					break;
//
//				case NEMO_ATTR_PITCH:
//					// Only chord data here
//					target_page->Step[row][0]->chord_data = buffer_chord;
//					// No break, fallthrough intended
//
//				default:
//					// Default attribute value shift
//					*(ptr_of_step_attr_value( target_page, row, 0, in_attribute )) = buffer;
//					break;
//			} // attribute switch
//
//			break;
//
//	} // Direction switch
//}

#ifdef FEATURE_STEP_EVENT_TRACKS
unsigned char is_skipped_rotate_step( Stepstruct *target_step ) {
	return (	( target_step->attr_AMT == -127 )
			|| 	( target_step->hyperTrack_ndx != 10 )
			||	( Step_get_status( target_step, STEPSTAT_SKIP ) == ON )
			// Step Mask is also a Step Event with a VEL==-127 (JB addition)
			||  ( ( Step_get_status( target_step, STEPSTAT_EVENT == ON ) )
			&& ( target_step->attr_VEL == -127 )  )   );

}

// Seek valid step (not Step Mask)
unsigned char seek_unmasked_step_col( Pagestruct* target_page, unsigned int row, unsigned char start_pos, unsigned char direction, unsigned int skip_mask ) {

	if( 	!( CHECK_BIT( skip_mask, start_pos ) )
		&&	!( is_skipped_rotate_step( target_page->Step[row][start_pos] ) ) ) {
		return start_pos;
	}

	unsigned char col = 0;

	switch( direction ) {
		case DEC:
			col = ( start_pos == 0 ) ? MATRIX_NROF_COLUMNS - 1: start_pos - 1;
			while(	(  start_pos != col )
				&& ( ( CHECK_BIT( skip_mask, col ) )
				||	 ( is_skipped_rotate_step( target_page->Step[row][col] ) ) ) )
			{
				col = ( col == 0 ) ? MATRIX_NROF_COLUMNS - 1: col - 1;
			}
		break;
		case INC:
			col = ( start_pos + 1 ) % MATRIX_NROF_COLUMNS;
			while(	(  start_pos != col )
				&& ( ( CHECK_BIT( skip_mask, col ) )
				||	 ( is_skipped_rotate_step( target_page->Step[row][col] ) ) ) )
			{
				col = ( col + 1 ) % MATRIX_NROF_COLUMNS;
			}
		break;
	}

	return col;
}

unsigned char is_skipped_rotate_skip( Stepstruct *target_step ) {
	return (  ( target_step->attr_AMT == -127 )
			||  ( ( Step_get_status( target_step, STEPSTAT_EVENT == ON ) )
								&& ( target_step->attr_VEL == -127 )  )   );
}

// Seek valid skip (not Step Mask)
unsigned char seek_unmasked_skip_col( Pagestruct* target_page, unsigned int row, unsigned char start_pos, unsigned char direction, unsigned int skip_mask ) {

	if( 	!( CHECK_BIT( skip_mask, start_pos ) )
		&&	!( is_skipped_rotate_skip( target_page->Step[row][start_pos] ) ) ) {
		return start_pos;
	}

	unsigned char col = 0;

	switch( direction ) {
		case DEC:
			col = ( start_pos == 0 ) ? MATRIX_NROF_COLUMNS - 1: start_pos - 1;
			while(	(  start_pos != col )
				&& ( ( CHECK_BIT( skip_mask, col ) )
				||	 ( is_skipped_rotate_skip( target_page->Step[row][col] ) ) ) )
			{
				col = ( col == 0 ) ? MATRIX_NROF_COLUMNS - 1: col - 1;
			}
		break;
		case INC:
			col = ( start_pos + 1 ) % MATRIX_NROF_COLUMNS;
			while(	(  start_pos != col )
				&& ( ( CHECK_BIT( skip_mask, col ) )
				||	 ( is_skipped_rotate_skip( target_page->Step[row][col] ) ) ) )
			{
				col = ( col + 1 ) % MATRIX_NROF_COLUMNS;
			}
		break;
	}

	return col;
}

//
// SHIFT Step SKIPS of target_track in the given direction
//
void shiftSkipsFromPOS( 	Pagestruct* target_page,
							unsigned int row,
							unsigned char start_pos,
							unsigned char end_pos,
							unsigned char alt_skips,
							unsigned int skip_mask ){

	// Shifts once and wraps around track boundaries [1,16]
	unsigned int 	col=0;
	unsigned char 	buffer_skip = 0;

	unsigned char direction = start_pos < end_pos ? DEC : INC;


	if( alt_skips ) {
		start_pos = seek_unmasked_skip_col( target_page, row, start_pos, direction == INC ? DEC : INC, skip_mask );
		end_pos = seek_unmasked_skip_col( target_page, row, end_pos, direction, skip_mask );
	}


	unsigned char next_col;

	// Direction of shift movement
	switch( direction ){

		case DEC:
			// Copy everything to one column earlier.. except for last column
			if( start_pos == end_pos ) {
				break;
			}

			for (col = start_pos; col < end_pos; col++) {

				if ( col == start_pos ) {
					// Store the first value in a buffer, to be later filled in the last position.
					buffer_skip = Step_get_status( target_page->Step[row][col], STEPSTAT_SKIP );
				}

				next_col = col + 1;

				if( alt_skips ) {
					col = seek_unmasked_skip_col( target_page, row, col, INC, skip_mask );
					if ( col == end_pos ) {
						break;
					}
					next_col = seek_unmasked_skip_col( target_page, row, col + 1, INC, skip_mask );
				}

				if (  (  Step_get_status( target_page->Step[row][next_col], STEPSTAT_SKIP )  ) == ON )  {
					// Turn on Skip
					Step_set_status( target_page->Step[row][col], STEPSTAT_SKIP, ON );
					// Turn off Skip
					Step_set_status( target_page->Step[row][next_col], STEPSTAT_SKIP, OFF );
				}
				else {
					Step_set_status( target_page->Step[row][col], STEPSTAT_SKIP, OFF );
				}
			} // Iterator columns 0-14;


			// Last column handler: put buffer value into the last place
			col = end_pos;

			Step_set_status( target_page->Step[row][col], STEPSTAT_SKIP, buffer_skip );

			break;



		case INC:
			// Shift all contents to the right
			if( start_pos == end_pos ) {
				break;
			}

			for ( col = start_pos; col > end_pos; col-- ){

				if ( col == start_pos ) {
					// Store last value in the buffer, to be filled into the first position
					buffer_skip = Step_get_status( target_page->Step[row][col], STEPSTAT_SKIP );
				}

				next_col = col - 1;

				if( alt_skips ) {
					col = seek_unmasked_skip_col( target_page, row, col, DEC, skip_mask );
					if ( col == end_pos ) {
						break;
					}
					next_col = seek_unmasked_skip_col( target_page, row, col - 1, DEC, skip_mask );
				}

				if (  (  Step_get_status( target_page->Step[row][next_col], STEPSTAT_SKIP )  ) == ON )  {
					// Turn on Skip
					Step_set_status( target_page->Step[row][col], STEPSTAT_SKIP, ON );
					// Turn off Skip
					Step_set_status( target_page->Step[row][next_col], STEPSTAT_SKIP, OFF );
				}
				else {
					Step_set_status( target_page->Step[row][col], STEPSTAT_SKIP, OFF );
				}
			} // Column iterator

			// Fill in the buffer value into the first place
			col = end_pos;

			Step_set_status( target_page->Step[row][col], STEPSTAT_SKIP, buffer_skip );

			break;


	} // Direction switch
}


//
// SHIFT Skip condition of the Step in the given direction
//
void shiftSkips( 	Pagestruct* target_page,
						unsigned int row,
						unsigned char direction ){

	unsigned char start_pos = direction == DEC ? 0 : MATRIX_NROF_COLUMNS - 1;
	unsigned char end_pos = direction == DEC ? MATRIX_NROF_COLUMNS - 1 : 0;

	shiftSkipsFromPOS( target_page, row, start_pos, end_pos, FALSE, OFF );
}

#endif //FEATURE_STEP_EVENT_TRACKS


//
// SHIFT MAP of target_track for in_attribute in the given direction with start col
//
void shiftAttributeMapFromPOS( 	Pagestruct* target_page,
							unsigned int row,
							unsigned char start_pos,
							unsigned char end_pos,
							unsigned int in_attribute,
							unsigned char alt_skips,
							unsigned int skip_mask ){

	// Shifts once and wraps around track boundaries [1,16]
	unsigned int 	col=0;
	unsigned int 	buffer=0;
	unsigned int 	buffer_chord_data=0,
					buffer_chord_up=0,
					buffer_phrase_num=0,
					buffer_phrase_data=0,
					buffer_event_data=0,
					buffer_hyperTrackNdx=0x10;

	Stepstruct* thisStepPt;
	Stepstruct* nextStepPt;

	unsigned char direction = start_pos < end_pos ? DEC : INC;

	#ifdef FEATURE_STEP_EVENT_TRACKS
	if( alt_skips ) {
		start_pos = seek_unmasked_step_col( target_page, row, start_pos, direction == INC ? DEC : INC, skip_mask );
		end_pos = seek_unmasked_step_col( target_page, row, end_pos, direction, skip_mask );
	}
	#endif

	unsigned char next_col;

	unsigned char status_hold_mask = 0;
	unsigned char status_apply_mask = 0xFF;

	if( !alt_skips ) {
		// exclude step events from rotation
		status_hold_mask = 0x30;
		status_apply_mask = 0xCF;
		if( G_zoom_level == zoomSTEP ) {
			// When in zoomSTEP Don't shift focused step selection (in STEP mode all Pages Step selections are OFF except for the focused step)
			SET_BIT( status_hold_mask, 1 );
			CLEAR_BIT( status_apply_mask, 1 );
		}
	}


	// Direction of shift movement
	switch( direction ){

		case DEC:
			// Copy everything to one column earlier.. except for last column
			if( start_pos == end_pos ) {
				break;
			}

			for (col = start_pos; col < end_pos; col++) {

				if ( col == start_pos ) {
					// Store the first value in a buffer, to be later filled in the last position.
					buffer 				 = *(ptr_of_step_attr_value( target_page, row, col, in_attribute ));
					buffer_chord_data 	 = target_page->Step[row][col]->chord_data;
					buffer_chord_up 	 = target_page->Step[row][col]->chord_up;
					buffer_phrase_num 	 = target_page->Step[row][col]->phrase_num;
					buffer_phrase_data 	 = target_page->Step[row][col]->phrase_data;
					buffer_event_data 	 = target_page->Step[row][col]->event_data;
					buffer_hyperTrackNdx = target_page->Step[row][col]->hyperTrack_ndx;
				}

				next_col = col + 1;
				#ifdef FEATURE_STEP_EVENT_TRACKS
				if( alt_skips ) {
					col = seek_unmasked_step_col( target_page, row, col, INC, skip_mask );
					if ( col == end_pos ) {
						break;
					}
					next_col = seek_unmasked_step_col( target_page, row, col + 1, INC, skip_mask );
				}
				#endif
				thisStepPt 	= target_page->Step[row][col];
				nextStepPt	= target_page->Step[row][next_col];

				switch( in_attribute ){

					case NEMO_ATTR_STATUS:
						// Make sure we don't shift the event bit in status
						thisStepPt->attr_STATUS =
							 	( thisStepPt->attr_STATUS & status_hold_mask )
							|	( nextStepPt->attr_STATUS & status_apply_mask );

						if ( CHECK_BIT( nextStepPt->attr_STATUS, STEPSTAT_EVENT ) ) {
							// Rotate the event data
							thisStepPt->event_data =
									( thisStepPt->event_data & 0xF0 )
								|	( nextStepPt->event_data & 0x0F );
						}
						break;

					case ATTR_AMOUNT:
						thisStepPt->attr_AMT = nextStepPt->attr_AMT;
						break;

					case ATTR_PITCH:
						// Shift the chord data separately
						thisStepPt->chord_data 	= nextStepPt->chord_data;
						thisStepPt->chord_up 	= nextStepPt->chord_up;

						// Shift the phrase data
						// thisStepPt->phrase_num 	= nextStepPt->phrase_num;
						thisStepPt->phrase_data = nextStepPt->phrase_data;

						// Shift the hyperTrack_ndx: needs both Step and Track update
						thisStepPt->hyperTrack_ndx = nextStepPt->hyperTrack_ndx;
						if ( thisStepPt->hyperTrack_ndx < MATRIX_NROF_ROWS ){
							target_page->Track[thisStepPt->hyperTrack_ndx]->hyper =
								(row << 4) | col;
						}
						// Make sure we only shift the stepLenMul in event_data, no event attr
						thisStepPt->event_data =
							 	( thisStepPt->event_data & 0x0F )
							|	( nextStepPt->event_data & 0xF0 );
						// No break, fallthrough needed

					default:
						// Shift the attribute value with no changes
						*(ptr_of_step_attr_value( target_page, row, col, in_attribute )) =
							*(ptr_of_step_attr_value( target_page, row, next_col, in_attribute ));
						break;
				} // attribute switch
			} // Iterator columns 0-14;

			// Last column handler: put buffer value into the last place
			col = end_pos;
			thisStepPt 	= target_page->Step[row][col];

			// According to selected attribute
			switch( in_attribute ){

				case NEMO_ATTR_STATUS:
					// Make sure we don't shift the event bit in status
					thisStepPt->attr_STATUS =
							( thisStepPt->attr_STATUS & status_hold_mask ) 	// Old status bit
						|	( buffer & status_apply_mask );						// New other bits

					if ( CHECK_BIT( buffer_event_data, STEPSTAT_EVENT ) ) {
						// Rotate the event data
						thisStepPt->event_data =
							 	( thisStepPt->event_data  	& 0xF0 )
							|	( buffer_event_data 		& 0x0F );
					}
					break;

				case ATTR_AMOUNT:
					thisStepPt->attr_AMT = buffer;
					break;

				case ATTR_PITCH:
					// Shift the chord data separately
					thisStepPt->chord_data 	= 	buffer_chord_data;
					thisStepPt->chord_up 	=	buffer_chord_up;

					// Shift the phrase data
					// thisStepPt->phrase_num 	=	buffer_phrase_num;
					thisStepPt->phrase_data =	buffer_phrase_data;

					// Shift the hypertrack data
					thisStepPt->hyperTrack_ndx = buffer_hyperTrackNdx;
					if ( thisStepPt->hyperTrack_ndx < MATRIX_NROF_ROWS ){
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
			// Shift all contents to the right
			if( start_pos == end_pos ) {
				break;
			}

			for ( col = start_pos; col > end_pos; col-- ){

				if ( col == start_pos ) {
					// Store last value in the buffer, to be filled into the first position
					buffer = *(ptr_of_step_attr_value( target_page, row, col, in_attribute ));
					buffer_chord_data 	 = target_page->Step[row][col]->chord_data;
					buffer_chord_up 	 = target_page->Step[row][col]->chord_up;
					buffer_phrase_num 	 = target_page->Step[row][col]->phrase_num;
					buffer_phrase_data 	 = target_page->Step[row][col]->phrase_data;
					buffer_event_data 	 = target_page->Step[row][col]->event_data;
					buffer_hyperTrackNdx = target_page->Step[row][col]->hyperTrack_ndx;
				}

				next_col = col - 1;
				#ifdef FEATURE_STEP_EVENT_TRACKS
				if( alt_skips ) {
					col = seek_unmasked_step_col( target_page, row, col, DEC, skip_mask );
					if ( col == end_pos ) {
						break;
					}
					next_col = seek_unmasked_step_col( target_page, row, col - 1, DEC, skip_mask );
				}
				#endif

				thisStepPt 	= target_page->Step[row][col];
				nextStepPt	= target_page->Step[row][next_col];

				switch( in_attribute ){

					case NEMO_ATTR_STATUS:
						thisStepPt->attr_STATUS =
							 	( thisStepPt->attr_STATUS & status_hold_mask )
							|	( nextStepPt->attr_STATUS & status_apply_mask );

						if ( CHECK_BIT( nextStepPt->attr_STATUS, STEPSTAT_EVENT ) ) {
							// Rotate the event data
							thisStepPt->event_data =
									( thisStepPt->event_data & 0xF0 )
								|	( nextStepPt->event_data & 0x0F );
						}
						break;

					case ATTR_AMOUNT:
						thisStepPt->attr_AMT = nextStepPt->attr_AMT;
						break;

					case ATTR_PITCH:
						thisStepPt->chord_data 	=	nextStepPt->chord_data;
						thisStepPt->chord_up 	=	nextStepPt->chord_up;

						//thisStepPt->phrase_num =	nextStepPt->phrase_num;
						thisStepPt->phrase_data =	nextStepPt->phrase_data;

						thisStepPt->hyperTrack_ndx = nextStepPt->hyperTrack_ndx;
						if ( thisStepPt->hyperTrack_ndx < MATRIX_NROF_ROWS ){
							target_page->Track[thisStepPt->hyperTrack_ndx]->hyper =
								(row << 4) | col;
						}
						// target_page->Step[row][col]->event_data =
						// target_page->Step[row][next_col]->event_data;
						// Make sure we only shift the stepLenMul in event_data, no event attr
						thisStepPt->event_data =
							 	( thisStepPt->event_data & 0x0F )
							|	( nextStepPt->event_data & 0xF0 );

						// No break, fallthrough needed

					default:
						*(ptr_of_step_attr_value( target_page, row, col, in_attribute ))
							= *(ptr_of_step_attr_value( target_page, row, next_col, in_attribute ));
						break;
				}
			} // Column iterator

			// Fill in the buffer value into the first place
			col = end_pos;
			thisStepPt 	= target_page->Step[row][col];

			switch( in_attribute ){

				case NEMO_ATTR_STATUS:
					// Make sure we don't shift the event bit in status
					thisStepPt->attr_STATUS =
							( buffer & status_apply_mask )
						|	( thisStepPt->attr_STATUS & status_hold_mask );

					if ( CHECK_BIT( buffer_event_data, STEPSTAT_EVENT ) ) {
						thisStepPt->event_data =
								( thisStepPt->event_data	& 0xF0 )
							|	( buffer_event_data 		& 0x0F );
					}
					break;

				case ATTR_AMOUNT:
					thisStepPt->attr_AMT = buffer;
					break;

				case ATTR_PITCH:
					// Only chord data here
					thisStepPt->chord_data 	= buffer_chord_data;
					thisStepPt->chord_up 	= buffer_chord_up;

					// Shift the phrase data - obsolote - done in another place.
					// thisStepPt->phrase_num 	= buffer_phrase_num;
					thisStepPt->phrase_data 	= buffer_phrase_data;

					thisStepPt->hyperTrack_ndx = buffer_hyperTrackNdx;
					if ( thisStepPt->hyperTrack_ndx < MATRIX_NROF_ROWS ){
						target_page->Track[thisStepPt->hyperTrack_ndx]->hyper =
							(row << 4) | col;
					}
					// thisStepPt->event_data 	= buffer_event_data;
					// Make sure we only shift the stepLenMul in event_data, no event attr
					thisStepPt->event_data =
						 	( thisStepPt->event_data   & 0x0F )
						|	( buffer_event_data & 0xF0 );

					// No break, fallthrough intended

				default:
					// Default attribute value shift
					*(ptr_of_step_attr_value( target_page, row, col, in_attribute )) = buffer;
					break;
			} // attribute switch

			break;

	} // Direction switch
}



//
// SHIFT MAP of target_track for in_attribute in the given direction
//
void shiftAttributeMap( 	Pagestruct* target_page,
							unsigned int row,
							unsigned int in_attribute,
							unsigned char direction ){

	unsigned char start_pos = direction == DEC ? 0 : MATRIX_NROF_COLUMNS - 1;
	unsigned char end_pos = direction == DEC ? MATRIX_NROF_COLUMNS - 1 : 0;

	shiftAttributeMapFromPOS( target_page, row, start_pos, end_pos, in_attribute, FALSE, OFF );
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
	switch( rotNdx ) {

		case ROT_PIT:
			modify_signed_parameter(
				&target_step->attr_PIT,
				STEP_MIN_PITCH, STEP_MAX_PITCH, direction, OFF, FIXED );
			break;

		case ROT_VEL:
			modify_signed_parameter(
				&target_step->attr_VEL,
				STEP_MIN_VELOCITY, STEP_MAX_VELOCITY, direction, OFF, FIXED );
			break;

		case ROT_LEN:
			modify_parameter(
				&target_step->attr_LEN,
				STEP_MIN_LENGTH, STEP_MAX_LENGTH, direction, OFF, FIXED );
			break;

		case ROT_STA:
			modify_parameter(
				&target_step->attr_STA,
				STEP_MIN_START, STEP_MAX_START, direction, OFF, FIXED );
			break;

	} // switch( rotNDx-1 )
}


// Modify the selected track attribute value - normally by big knob
void modify_selectedTrackAttribute_VALUE( 	Pagestruct* target_page,
											unsigned char trackNdx,
											unsigned char direction,
											unsigned char quickturn ){

		// Change the value of the selected attribute of the track
		switch( NEMO_selectedTrackAttribute ){

			case NEMO_ATTR_VELOCITY:
				modify_parameter( 	&target_page->Track[trackNdx]->attr_VEL,
									TRACK_MIN_VELOCITY, TRACK_MAX_VELOCITY, direction, quickturn, FIXED );
				break;

			case NEMO_ATTR_PITCH:
				modify_signed_parameter( 	&target_page->Track[trackNdx]->attr_PIT,
									TRACK_MIN_PITCH, TRACK_MAX_PITCH, direction, quickturn, FIXED );
				break;

			case NEMO_ATTR_LENGTH:
				modify_parameter( 	&target_page->Track[trackNdx]->LEN_factor,
									TRACK_MIN_LENGTH, TRACK_MAX_LENGTH, direction, quickturn, FIXED );
				break;

			case NEMO_ATTR_START:
				modify_parameter( 	&target_page->Track[trackNdx]->STA_factor,
									TRACK_MIN_STAFACTOR, TRACK_MAX_STAFACTOR, direction, quickturn, FIXED );
				break;

			case NEMO_ATTR_POSITION:
				modify_parameter( 	&target_page->Track[trackNdx]->attr_POS,
									TRACK_MIN_POSITION, TRACK_MAX_POSITION, direction, quickturn, WRAPPED );
				// Special handling here
				Page_wrap_track( target_page, trackNdx, direction, 1);
				break;

			case NEMO_ATTR_DIRECTION:
				modify_parameter( 	&target_page->Track[trackNdx]->attr_DIR,
									TRACK_MIN_DIRECTION, TRACK_MAX_DIRECTION, direction, quickturn, FIXED );
				break;

			case NEMO_ATTR_AMOUNT:
				modify_parameter( 	&target_page->Track[trackNdx]->attr_AMT,
									TRACK_MIN_AMOUNT, TRACK_MAX_AMOUNT, direction, quickturn, FIXED );
				break;

			case NEMO_ATTR_GROOVE:
				modify_parameter( 	&target_page->Track[trackNdx]->attr_GRV,
									TRACK_MIN_GROOVE, TRACK_MAX_GROOVE, direction, quickturn, FIXED );
				break;

			case NEMO_ATTR_MIDICC:
				modify_signed_parameter( 	&target_page->Track[trackNdx]->attr_MCC,
									TRACK_MIN_MIDICC, TRACK_MAX_MIDICC, direction, quickturn, FIXED );
				break;

			case ATTR_MIDICH:
				modify_parameter( 	&target_page->Track[trackNdx]->attr_MCH,
									TRACK_MIN_MIDICH, TRACK_MAX_MIDICH, direction, quickturn, FIXED );
				break;

			case NEMO_ATTR_PGMCH:
				modify_parameter( 	&target_page->Track[trackNdx]->program_change,
									TRACK_MIN_PROGRAMCHANGE, TRACK_MAX_PROGRAMCHANGE, direction, quickturn, FIXED );
				MIDI_send( 	MIDI_PGMCH,
							target_page->Track[trackNdx]->attr_MCH
							+ target_page->Track[trackNdx]->event_offset[NEMO_ATTR_MIDICH],
							target_page->Track[trackNdx]->program_change - 1,
							0 );
				break;
		}
}


// Modify the selected track attribute value - normally by big knob
void modify_selectedTrackAttribute_FACTOR( 	Pagestruct* target_page,
											unsigned char trackNdx,
											unsigned char direction,
											unsigned char quickturn ){

		// Change the value of the selected attribute of the track
		switch( NEMO_selectedTrackAttribute ){

			case NEMO_ATTR_VELOCITY:
				// Modify VEL factor
				target_page->Track[ trackNdx ]->event_offset[NEMO_ATTR_VELOCITY] = 0;
				modify_parameter( 	&target_page->Track[ trackNdx ]->VEL_factor,
									TRACK_MIN_VELFACTOR,
									TRACK_MAX_VELFACTOR,
									direction, quickturn, FIXED );
				break;

			case NEMO_ATTR_PITCH:
				// Modify PIT factor
				target_page->Track[ trackNdx ]->event_offset[NEMO_ATTR_PITCH] = 0;
				modify_parameter( 	&target_page->Track[ trackNdx ]->PIT_factor,
									TRACK_MIN_PITFACTOR,
									TRACK_MAX_PITFACTOR,
									direction, quickturn, FIXED );
				break;

			case NEMO_ATTR_LENGTH:
				// Modify LEN factor
				target_page->Track[ trackNdx ]->event_offset[NEMO_ATTR_LENGTH] = 0;
				modify_parameter( 	&target_page->Track[ trackNdx ]->LEN_factor,
									TRACK_MIN_LENFACTOR,
									TRACK_MAX_LENFACTOR,
									direction, quickturn, FIXED );
				break;

			case NEMO_ATTR_START:
				target_page->Track[ trackNdx ]->event_offset[NEMO_ATTR_START] = 0;
				modify_parameter( 	&target_page->Track[ trackNdx ]->STA_factor,
									TRACK_MIN_STAFACTOR,
									TRACK_MAX_STAFACTOR,
									direction, quickturn, FIXED );
				break;

			case NEMO_ATTR_AMOUNT:
				// Modify AMT factor
				target_page->Track[ trackNdx ]->event_offset[NEMO_ATTR_AMOUNT] = 0;
				modify_parameter( 	&target_page->Track[ trackNdx ]->AMT_factor,
									TRACK_MIN_AMTFACTOR,
									TRACK_MAX_AMTFACTOR,
									direction, quickturn, FIXED );
				break;

			case NEMO_ATTR_GROOVE:
				// Modify GRV factor
				target_page->Track[ trackNdx ]->event_offset[NEMO_ATTR_GROOVE] = 0;
				modify_parameter( 	&target_page->Track[ trackNdx ]->GRV_factor,
									TRACK_MIN_GRVFACTOR,
									TRACK_MAX_GRVFACTOR,
									direction, quickturn, FIXED );
				break;

			case NEMO_ATTR_MIDICC:
				// Modify MCC range - note the relation to STEP
				target_page->Track[ trackNdx ]->event_offset[ATTR_MIDICC] = 0;
				modify_parameter( 	&target_page->Track[ trackNdx ]->MCC_factor,
									TRACK_MIN_MCCFACTOR,
									TRACK_MAX_MCCFACTOR,
									direction, quickturn, FIXED );
				break;

		} // switch on selected track attribute
}
