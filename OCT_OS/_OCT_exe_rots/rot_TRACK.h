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






void rot_exec_TRACK( 	Pagestruct* target_page, 
						unsigned char rotNdx, 
						unsigned char direction ){


		unsigned char i = 0;
		unsigned char row = 0;
		unsigned char col = 0;


	// EDIT TIMER - Reset on every turn	
	if (EDIT_TIMER == ON) {
		
		EDIT_TIMER = ON;
		// Setup alarm for the EDITOR TIMER
		cyg_alarm_initialize(	alarm_hdl, 
								cyg_current_time() + TIMEOUT_VALUE,
								0 );

		if (rotNdx != ATTR_START) {
			// IF.. because I like the bar display better than the START dot.
			target_page->editAttribute = rotNdx;
		}
	}

	
	// BIG KNOB
	if ( rotNdx == 0 ){

		// Program Change-er or individualized modifier
		switch( target_page->trackAttributeSelection ){

			case 0:
				// Act as program changer
				i = my_bit2ndx( target_page->trackSelection );

				switch( CHANGE_BANK_OR_PGM ){
				
					case PGM:
						modify_parameter( 	&target_page->Track[i]->program_change, 
											TRACK_MIN_PROGRAMCHANGE, TRACK_MAX_PROGRAMCHANGE, direction, ON, FIXED );
		
						// If the Program button is selected, send the actual program change as well
						if ( is_pressed_key( KEY_PROGRAM ) ){
						
							MIDI_send( 	MIDI_PGMCH, 
								target_page->Track[i]->attr_MCH
								+ target_page->Track[i]->event_offset[ATTR_MIDICH], 
								target_page->Track[i]->program_change - 1, 
								0 );

							// Volume level as dedicated message
							if (target_page->MIXAMT_VOL[i] != 64) {
								MIDI_send( 	MIDI_CC,
										target_page->Track[i]->attr_MCH
										+ target_page->Track[i]->event_offset[ATTR_MIDICH],
										7,
										target_page->MIXAMT_VOL[i] 	);
							}
						}
						break;

					case BANK:
						modify_parameter( 	&target_page->Track[i]->bank_change, 
											TRACK_MIN_PROGRAMCHANGE, TRACK_MAX_PROGRAMCHANGE, direction, ON, FIXED );
						break;
				}
				break;


			case ( 1 << 0 ):
				// Modify VEL factor
				target_page->Track[ my_bit2ndx( target_page->trackSelection )]->event_offset[ATTR_VELOCITY] = 0;
				modify_parameter( 	&target_page->Track[ my_bit2ndx( target_page->trackSelection )]->VEL_factor, 
									TRACK_MIN_VELFACTOR, 
									TRACK_MAX_VELFACTOR, 
									direction, OFF, FIXED );
				break;

			case ( 1 << 1 ):
				// Modify PIT factor
				target_page->Track[ my_bit2ndx( target_page->trackSelection )]->event_offset[ATTR_PITCH] = 0;
				modify_parameter( 	&target_page->Track[ my_bit2ndx( target_page->trackSelection )]->PIT_factor, 
									TRACK_MIN_PITFACTOR, 
									TRACK_MAX_PITFACTOR, 
									direction, OFF, FIXED );
				break;

			case ( 1 << 2 ):
				// Modify LEN factor
				target_page->Track[ my_bit2ndx( target_page->trackSelection )]->event_offset[ATTR_LENGTH] = 0;
				modify_parameter( 	&target_page->Track[ my_bit2ndx( target_page->trackSelection )]->LEN_factor, 
									TRACK_MIN_LENFACTOR, 
									TRACK_MAX_LENFACTOR, 
									direction, OFF, FIXED );
				break;

			case ( 1 << 3 ):
				// Modify STA factor
				target_page->Track[ my_bit2ndx( target_page->trackSelection )]->event_offset[ATTR_START] = 0;
				modify_parameter( 	&target_page->Track[ my_bit2ndx( target_page->trackSelection )]->STA_factor, 
									TRACK_MIN_STAFACTOR, 
									TRACK_MAX_STAFACTOR, 
									direction, OFF, FIXED );
				break;

			case ( 1 << 6 ):
				// Modify AMT factor
				target_page->Track[ my_bit2ndx( target_page->trackSelection )]->event_offset[ATTR_AMOUNT] = 0;
				modify_parameter( 	&target_page->Track[ my_bit2ndx( target_page->trackSelection )]->AMT_factor, 
									TRACK_MIN_AMTFACTOR, 
									TRACK_MAX_AMTFACTOR, 
									direction, OFF, FIXED );
				break;

			case ( 1 << 7 ):
				// Modify GRV factor
				target_page->Track[ my_bit2ndx( target_page->trackSelection )]->event_offset[ATTR_GROOVE] = 0;
				modify_parameter( 	&target_page->Track[ my_bit2ndx( target_page->trackSelection )]->GRV_factor, 
									TRACK_MIN_GRVFACTOR, 
									TRACK_MAX_GRVFACTOR, 
									direction, OFF, FIXED );
				break;

			case ( 1 << 8 ):
				// Modify MCC range - note the relation to STEP
				target_page->Track[ my_bit2ndx( target_page->trackSelection )]->event_offset[ATTR_MIDICC] = 0;
				modify_parameter( 	&target_page->Track[ my_bit2ndx( target_page->trackSelection )]->MCC_factor, 
									TRACK_MIN_MCCFACTOR, 
									TRACK_MAX_MCCFACTOR, 
									direction, OFF, FIXED );
				break;
		}

	}


	//	
	// EDIT rotary: editing the hardwired attributes
	//
	else if ((rotNdx >= 1) && (rotNdx <= 10)) {

		// Execute on the finger_on_a_step mode before anything else
		if ( G_key_pressed != 0 ){

			// Linear G_scan through the buttons in question: Selectors and Steps.
			for( i=0; i <= 185; i++ ){
				
				// There is a key pressed - in the Step Range
				if (	( G_pressed_keys[i] >=  11 )
					&&	( G_pressed_keys[i] <= 185 ) 

					// And precisely in the position row
					&& 	( ((G_pressed_keys[i]-4) % 11) == 0 ) 
					){

					// Compute the step coordinates. Row is that of the track selection in page
					row = my_bit2ndx( target_page->trackSelection );
					col = column_of	( G_pressed_keys[i] );							
					//d_iag_printf( "rotary /step: row:%d col%d\n", row, col);

					// Just to avoid any bad pointing down the line.
					if ( row > 10 ) break;

					// d_iag_printf( "%d row:%d col:%d  ", G_pressed_keys[i], row, col );

					// The first knob click activates the step if it is not active
					if ( Step_get_status( target_page->Step[row][col], STEPSTAT_TOGGLE ) == OFF ){

						// Make sure the step gets turned on as we turn the knob
						Step_set_status( target_page->Step[row][col], STEPSTAT_TOGGLE, ON );
					}
					else {
						
						// Finally apply the rotary to the step in question.
						apply_rotary_to_step( rotNdx, direction, target_page, row, col );
					}
				} // key pressed in step range
			} // button iterator

		} // G_key_pressed != 0


		// There is no key pressed
		else {

			// Modify hardwired EDIT Attribute for trackSelection
			switch (rotNdx-1) {
				
				case VELOCITY:
				
					// For selected track modify the corresponding attribute
					for (i=0; i< 10; i++) {
						if ((1<<i) & (target_page->trackSelection)) {

							target_page->Track[i]->event_offset[ATTR_VELOCITY] = 0;

							modify_parameter 
								(&target_page->Track[i]->attr_VEL, 
									TRACK_MIN_VELOCITY, TRACK_MAX_VELOCITY, direction, ON, FIXED);
						}
					}
					break;
					
					
				case PITCH:
				
					// For every track in selection modify the corresponding attribute
					for (i=0; i< 10; i++) {
						if ((1<<i) & (target_page->trackSelection)) {

							target_page->Track[i]->event_offset[ATTR_PITCH] = 0;

							modify_signed_parameter 
								(&target_page->Track[i]->attr_PIT, 
									TRACK_MIN_PITCH, TRACK_MAX_PITCH, direction, ON, FIXED);					
						}
					}
					break;
					
					
				case LENGTH:
				
					// For every track in selection modify the corresponding attribute
					for (i=0; i < MATRIX_NROF_ROWS; i++) {
						
						if ((1<<i) & (target_page->trackSelection)) {

							target_page->Track[i]->event_offset[ATTR_LENGTH] = 0;

							modify_parameter(  
								&target_page->Track[i]->LEN_factor, 
								TRACK_MIN_LENFACTOR, TRACK_MAX_LENFACTOR, direction, OFF, FIXED);
						}
					}
					break;	
					
								
				case START:
				
					// For every track in selection modify the corresponding attribute
					for (i=0; i< 10; i++) {
						if ((1<<i) & (target_page->trackSelection)) {

							target_page->Track[i]->event_offset[ATTR_START] = 0;

							modify_parameter( 
								&target_page->Track[i]->STA_factor, 
								TRACK_MIN_STAFACTOR, TRACK_MAX_STAFACTOR, direction, OFF, FIXED);
						}
					}
					break;


				case POSITION:

					// For every track in selection modify the corresponding attribute
					for (i=0; i< 10; i++) {
						if ((1<<i) & (target_page->trackSelection)) {
							// Position computation done manually for now for every side
							Page_wrap_track( target_page, i, direction, 1);								

							modify_parameter 
								(&target_page->Track[i]->attr_POS, 
									// 1, 16, direction, ON);
									TRACK_MIN_POSITION, TRACK_MAX_POSITION, direction, OFF, FIXED);
						}
					}
					break;


				case DIRECTION:

					// For every track in selection modify the corresponding attribute
					for (i=0; i< 10; i++) {
						if ((1<<i) & (target_page->trackSelection)) {

							target_page->Track[i]->event_offset[ATTR_DIRECTION] = 0;

							modify_parameter 
								(&target_page->Track[i]->attr_DIR, 
									// 1, DIRECTION_MODES, direction, ON);
									TRACK_MIN_DIRECTION, TRACK_MAX_DIRECTION, direction, OFF, FIXED);
						}
					}
					break;
					
					
				case AMOUNT:
				
					// For every track in selection modify the corresponding attribute
					for (i=0; i< 10; i++) {
						if ((1<<i) & (target_page->trackSelection)) {

							target_page->Track[i]->event_offset[ATTR_AMOUNT] = 0;

							modify_parameter 
								(&target_page->Track[i]->attr_AMT, 
									// 0, 127, direction, ON);
									TRACK_MIN_AMOUNT, TRACK_MAX_AMOUNT, direction, ON, FIXED);										
						}
					}
					break;
					
					
				case GROOVE:

					// For every track in selection modify the corresponding attribute
					for (i=0; i< 10; i++) {
						if ((1<<i) & (target_page->trackSelection)) {

							target_page->Track[i]->event_offset[ATTR_GROOVE] = 0;

							modify_parameter 
								(&target_page->Track[i]->attr_GRV, 
									// 0, 16, direction, ON);
									TRACK_MIN_GROOVE, TRACK_MAX_GROOVE, direction, OFF, FIXED);
						}
					}
					break;
				
				
				case MIDICC:
				
					// For every track in selection modify the corresponding attribute
					for (i=0; i< 10; i++) {
						if ((1<<i) & (target_page->trackSelection)) {
							
							target_page->Track[i]->event_offset[ATTR_MIDICC] = 0;
							
							modify_signed_parameter 
								(&target_page->Track[i]->attr_MCC, 
									TRACK_MIN_MIDICC, TRACK_MAX_MIDICC, direction, ON, FIXED);
						}
					}
					break;						


				case MIDICH:

					// For every track in selection modify the corresponding attribute
					for (i=0; i< 10; i++) {
						if ((1<<i) & (target_page->trackSelection)) {

							target_page->Track[i]->event_offset[ATTR_MIDICH] = 0;
							
							modify_parameter 
								(&target_page->Track[i]->attr_MCH, 
									TRACK_MIN_MIDICH, TRACK_MAX_MIDICH, direction, OFF, FIXED);
						}
					}
					break;
			} // switch (rotNdx)
		
		} // else -> there was no key pressed
		
	} // rotNdx between 1 and 10
	

	// MIX rotarys
	if ((rotNdx >= 11) && (rotNdx <= 20)) {

		// Jump to the track of the rotary by changing the track selection in page
		target_page->trackSelection = ( 1 << (rotNdx - 11) );

	} // rotNdx between 11 and 20

			
}


