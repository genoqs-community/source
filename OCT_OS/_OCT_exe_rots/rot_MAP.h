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




void rot_exec_MAP( 	Pagestruct* target_page,
								unsigned char rotNdx,
								unsigned char direction ){


		unsigned char i = 0;
		unsigned char row = 0;
		unsigned char col = 0;
		unsigned int stepSelection = 0;

		// Compute step selection for the selected track
		stepSelection =
			Page_get_selection_trackpattern
				(target_page, my_bit2ndx( target_page->trackSelection ));

		// BIG KNOB
		if ( rotNdx == 0 ){

			if ( is_pressed_rowzero() != 0 ) return;

			// Determine the row of the edited track
			row = my_bit2ndx( target_page->trackSelection );

			switch( target_page->trackAttributeSelection ){


				case ( 1 << VELOCITY ):

					target_page->Track[row]->event_offset[ATTR_VELOCITY] = 0;

					modify_parameter(
						&target_page->Track[row]->VEL_factor,
						TRACK_MIN_VELFACTOR, TRACK_MAX_VELFACTOR,
						direction, OFF, FIXED );
					break;


				case ( 1 << PITCH ):

					target_page->Track[row]->event_offset[ATTR_PITCH] = 0;

					modify_parameter(
						&target_page->Track[row]->PIT_factor,
						TRACK_MIN_PITFACTOR, TRACK_MAX_PITFACTOR,
						direction, OFF, FIXED );
					break;


				case ( 1 << LENGTH ):

					target_page->Track[row]->event_offset[ATTR_LENGTH] = 0;

					modify_parameter(
						&target_page->Track[row]->LEN_factor,
						TRACK_MIN_LENFACTOR, TRACK_MAX_LENFACTOR,
						direction, OFF, FIXED );
					break;


				case ( 1 << START ):

					target_page->Track[row]->event_offset[ATTR_START] = 0;

					modify_parameter(
						&target_page->Track[row]->STA_factor,
						TRACK_MIN_STAFACTOR, TRACK_MAX_STAFACTOR,
						direction, OFF, FIXED );
					break;


				case ( 1 << DIRECTION ):

					// Modify the certaint_next parameter for selected trigger set. But not in directions 1-5
					if ( target_page->Track[row]->attr_DIR > 5 ){

						modify_parameter(
							&direction_repository[ target_page->Track[row]->attr_DIR ]
							.trigger_set[ current_trigger_set ]
							.certainty_next,
							0, 100, direction, ON, FIXED );
					}
					break;


				case ( 1 << AMOUNT ):

					target_page->Track[row]->event_offset[ATTR_AMOUNT] = 0;

					modify_parameter(
						&target_page->Track[row]->AMT_factor,
						TRACK_MIN_AMTFACTOR, TRACK_MAX_AMTFACTOR,
						direction, OFF, FIXED );
					break;


				case ( 1 << GROOVE ):

					target_page->Track[row]->event_offset[ATTR_GROOVE] = 0;

					modify_parameter(
						&target_page->Track[row]->GRV_factor,
						TRACK_MIN_GRVFACTOR, TRACK_MAX_GRVFACTOR,
						direction, OFF, FIXED );
					break;


				case ( 1 << MIDICC ):

					target_page->Track[row]->event_offset[ATTR_MIDICC] = 0;

					modify_parameter(
						&target_page->Track[row]->MCC_factor,
						TRACK_MIN_MCCFACTOR, TRACK_MAX_MCCFACTOR,
						direction, OFF, FIXED );
					break;
			}
		}




		//
		// MIXER
		//
		if ((rotNdx >= 11) && (rotNdx <= 20)) {

			// Done now via the BIRDSEYE view
			// Jump to the track of the rotary by changing the track selection in page
			target_page->trackSelection = ( 1 << (rotNdx - 11) );
		} // rotNdx between 11 and 20



		//
		// EDITOR
		//
		// No step is selected, the rotary is not Position, no key is pressed, or EDITOR in control mode
		if (	(	((stepSelection == 0) 	&& 	(rotNdx-1 != POSITION))
				&&	( G_key_pressed == 0 ) )
			|| 	(target_page->editorMode == CONTROL)
			){

			return;
		}


		// Execute on the finger_on_a_step mode before anything else
		if ( G_key_pressed != 0 ){

			// Linear G_scan through the buttons in question: Selectors and Steps.
			for( i=0; i <= 185; i++ ){

				// There is a key pressed - in the Step Range
				if (	( G_pressed_keys[i] >=  11 )
					&&	( G_pressed_keys[i] <= 185 )

					// ..and in row zero
					&& 	( ((G_pressed_keys[i] - 9) % 11) == 0 )
					){

					// Compute the step coordinates. Row is that of the track selection in page
					row = my_bit2ndx( target_page->trackSelection );
					col = column_of	( G_pressed_keys[i] );

					// Just to avoid any bad pointing down the line.
					if ( row > 10 ) break;

					// The first knob click activates the step if it is not active
					if ( Step_get_status( target_page->Step[row][col], STEPSTAT_TOGGLE ) == OFF ){

 						// If we are not in PREVIEW mode
 						if (	( target_page->editorMode != PREVIEW )
 								&& ( target_page->editorMode != PREVIEW_PERFORM )
 							){

	 						// Make sure the step gets turned on as we turn the knob
							Step_set_status( target_page->Step[row][col], STEPSTAT_TOGGLE, ON );
						}
					}
					else {

						// Finally apply the rotary to the step in question.
						apply_rotary_to_step( rotNdx, direction, target_page, row, col );

						// If we are in PREVIEW mode, play the step as well
						if (	( target_page->editorMode == PREVIEW )
							){

							PLAYER_preview_step( target_page, row, col );
						}

					}
				} // key pressed in step range
			} // button iterator
		} // G_key_pressed != 0


		// Implicitly one or more steps are selected, or the rotary was POSITION
		if ((rotNdx >= 1) && (rotNdx <= 10)) {

			// Normalize rotNdx value
			rotNdx --;

			// If this is not a relevant rotary, break
			if ( 	(	( 	(1 << rotNdx)
						&	PANEL_getAttributeMap( STEP_S, MODIFY )) == 0 )
					&&	( rotNdx != POSITION )
				){

				return;
			}

			// Identify the row to operate on
			row = my_bit2ndx( target_page->trackSelection );

			// MAP SHIFT inplementation: POSITION rotary
			if ( rotNdx == POSITION ) {

			if ( 	(stepSelection == 0 )
				&&	( target_page->trackAttributeSelection != ( 1 << DIRECTION ) )
				) {

					shiftAttributeMap(
						target_page,
						row,
						my_bit2ndx( target_page->trackAttributeSelection ) +1,
						direction
						);

					// d_iag_printf( "shifting attribute:%d\n",
					//		my_bit2ndx( target_page->trackAttributeSelection ) );

				}
				else {
					// do nothing
				}
			}

			else {

				target_page->trackAttributeSelection = 1 << rotNdx;
			}

			// MODIFY Editor attribute for selected Steps only
			switch (rotNdx) {

				case VELOCITY:
					// For selected steps
					for (i=0; i<MATRIX_NROF_COLUMNS; i++) {
						if ((1<<(15-i)) & (stepSelection)) {
							modify_signed_parameter
								(&target_page->Step[row][i]->attr_VEL,
									STEP_MIN_VELOCITY, STEP_MAX_VELOCITY, direction, OFF, FIXED);
						}
					}
					break;

				case PITCH:
					// For selected steps
					for (i=0; i<MATRIX_NROF_COLUMNS; i++) {
						if ((1<<(15-i)) & (stepSelection)) {
							modify_signed_parameter
								(&target_page->Step[row][i]->attr_PIT,
									STEP_MIN_PITCH, STEP_MAX_PITCH, direction, OFF, FIXED);
						}
					}
					break;

				case LENGTH:
					// For selected steps
					for (i=0; i<MATRIX_NROF_COLUMNS; i++) {
						if ((1<<(15-i)) & (stepSelection)) {
							modify_parameter
								(&target_page->Step[row][i]->attr_LEN,
// old								STEP_MIN_LENGTH, ((unsigned char)STEP_MAX_LENGTH), direction, OFF, FIXED);
									STEP_MIN_LENGTH, STEP_MAX_LENGTH, direction, OFF, FIXED);
						}
					}
					break;

				case START:
					// Works via the STEP->>ATTR_START parameter
					// For selected steps
					for (i=0; i<MATRIX_NROF_COLUMNS; i++) {
						if ((1<<(15-i)) & (stepSelection)) {
							modify_parameter
								(&target_page->Step[row][i]->attr_STA,
									STEP_MIN_START, STEP_MAX_START, direction, OFF, FIXED);
						}
					}
					break;

#if GROEF || 1
				case GROOVE:
					// For selected steps
					for (i=0; i<MATRIX_NROF_COLUMNS; i++) {
						if ((1<<(15-i)) & (stepSelection)) {
							modify_parameter
								(&target_page->Step[row][i]->phrase_num,
									STEP_MIN_GROOVE, STEP_MAX_GROOVE, direction, OFF, FIXED);
						}
					}
					break;
#endif

				case MIDICC:
					// For selected steps
					for (i=0; i<MATRIX_NROF_COLUMNS; i++) {
						if ((1<<(15-i)) & (stepSelection)) {
							modify_parameter
								(&target_page->Step[row][i]->attr_MCC,
									STEP_MIN_MIDICC, STEP_MAX_MIDICC, direction, OFF, FIXED);
						}
					}
					break;

			} // switch (rotNdx)
		} // rotNdx <= 10

}

