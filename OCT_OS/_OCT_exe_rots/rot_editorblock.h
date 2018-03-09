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






void rot_exe_EDIT( 	unsigned char rotNdx, 
					unsigned char direction, 
					Pagestruct* target_page ){

	unsigned char i=0;
			
	// d_iag_printf( "rotNdx:%d dir:%d pgNdx:%d\n", rotNdx, direction, target_page->pageNdx );


	// EDIT_TIMER handling
	//
	// Reset timer on every turn, so it doesnt run out
	if (target_page->trackSelection != 0) {

		EDIT_TIMER = ON;				
		// Setup alarm for the EDIT TIMER
		cyg_alarm_initialize(	alarm_hdl, 
								cyg_current_time() + TIMEOUT_VALUE,
								0 );

		// Select the edit attribute
		target_page->editAttribute = rotNdx;

	} // Case where some track is selected

	
	else {

		switch( target_page->editorMode == CONTROL ) {
		
			// No track selected and EDITOR is in Controller Mode -> send MIDI CC data
			case CONTROL:
	
				// Normalize i value to get to the track index
				i = rotNdx - 1;
	
				// Check before sending data that a valid CC is assigned (tmoravan)
				if ( (	target_page->Track[i]->attr_MCC ) != MIDICC_NONE ){
				
					// Modify the CC amount of track
					modify_signed_parameter 
						(&target_page->Track[i]->attr_CCAMT, 
							TRACK_MIN_CCAMNT, TRACK_MAX_CCAMNT, direction, ON, FIXED);
		
					// Send MIDICC on the corresponding channel
					MIDI_send( 	MIDI_CC,
								target_page->Track[i]->attr_MCH
								+ target_page->Track[i]->event_offset[ATTR_MIDICH],
								target_page->Track[i]->attr_MCC,					
								target_page->Track[i]->attr_CCAMT 	);
				} // Track is assigned a valid CC value
				break;
	
		}
	}



	// 
	// ATTRIBUTE MODIFY - EDITOR of trackSelection (hardwired)
	//
	switch (rotNdx) {
		
		case 1:		// VELOCITY
			// For every track in selection modify the corresponding attribute
			for (i=0; i< 10; i++) {
				if ((1<<i) & (target_page->trackSelection)) {

					target_page->Track[i]->event_offset[ATTR_VELOCITY] = 0;

					modify_parameter 
						(&target_page->Track[i]->attr_VEL, 
						 	TRACK_MIN_VELOCITY, TRACK_MAX_VELOCITY, direction, OFF, FIXED);
				}
			}
			// Start the quickturn timer after the first full track iteration
			// start_QUICKTURN_TIMER();
			break;


		case 2:		// PITCH
			// For every track in selection modify the corresponding attribute
			for (i=0; i< 10; i++) {
				if ((1<<i) & (target_page->trackSelection)) {

					target_page->Track[i]->event_offset[ATTR_PITCH] = 0;

					modify_parameter 
						(&target_page->Track[i]->attr_PIT, 
							TRACK_MIN_PITCH, TRACK_MAX_PITCH, direction, OFF, FIXED);	
				}
			}
			// Start the quickturn timer after the first full track iteration
			// start_QUICKTURN_TIMER();
			break;


		case 3:		// LENGTH
			// For every track in selection modify the corresponding attribute
			for (i=0; i< 10; i++) {
				if ((1<<i) & (target_page->trackSelection)) {

					target_page->Track[i]->event_offset[ATTR_LENGTH] = 0;

					modify_parameter(  
						&target_page->Track[i]->LEN_factor, 
						// 1, 16, direction, ON);
						TRACK_MIN_LENFACTOR, TRACK_MAX_LENFACTOR, direction, OFF, FIXED);
				}
			}
			// Start the quickturn timer after the first full track iteration
			// Disable here to prevent overshooting of length and therefore 
			// ..undefined results.
			// start_QUICKTURN_TIMER();
			break;
			

		case 4:		// START
			// For every track in selection modify the corresponding attribute
			for (i=0; i< 10; i++) {
				if ((1<<i) & (target_page->trackSelection)) {

					target_page->Track[i]->event_offset[ATTR_START] = 0;

					modify_parameter( 
						&target_page->Track[i]->STA_factor, 
						TRACK_MIN_STAFACTOR, TRACK_MAX_STAFACTOR, direction, OFF, FIXED);
				}
			}
			// Start the quickturn timer after the first full track iteration
			// Disable here to prevent overshooting of length and therefore 
			// ..undefined results.
			// start_QUICKTURN_TIMER();
			break;
			

		case 5:		// POSITION
			// For every track in selection modify the corresponding attribute
			for (i=0; i< 10; i++) {

				if ((1<<i) & (target_page->trackSelection)) {

					// Position computation done manually for now for every side
					Page_wrap_track( target_page, i, direction, 1 );

					modify_parameter 
						(&target_page->Track[i]->attr_POS, 
							TRACK_MIN_POSITION, TRACK_MAX_POSITION, direction, OFF, FIXED);
				}
			}
			break;
			

		case 6:		// DIRECTION
			// For every track in selection modify the corresponding attribute
			for (i=0; i< 10; i++) {
				if ((1<<i) & (target_page->trackSelection)) {

					target_page->Track[i]->event_offset[ATTR_DIRECTION] = 0;

					modify_parameter 
						(&target_page->Track[i]->attr_DIR, 
						 	TRACK_MIN_DIRECTION, TRACK_MAX_DIRECTION, direction, OFF, FIXED);
				}
			}
			break;
		

		case 7:		// AMOUNT
			// For every track in selection modify the corresponding attribute
			for (i=0; i< 10; i++) {

				if ((1<<i) & (target_page->trackSelection)) {

					target_page->Track[i]->event_offset[ATTR_AMOUNT] = 0;

					modify_parameter 
						(&target_page->Track[i]->attr_AMT, 
						 	TRACK_MIN_AMOUNT, TRACK_MAX_AMOUNT, direction, OFF, FIXED);
				}
			}
			break;
			

		case 8:		// GROOVE
			// For every track in selection modify the corresponding attribute
			for (i=0; i< 10; i++) {

				if ((1<<i) & (target_page->trackSelection)) {

					target_page->Track[i]->event_offset[ATTR_GROOVE] = 0;

					modify_parameter 
						(&target_page->Track[i]->attr_GRV, 
						 	TRACK_MIN_GROOVE, TRACK_MAX_GROOVE, direction, OFF, FIXED);
				}
			}
			// Update the max groove value for the page
			Page_update_maxGroove (target_page);
			break;
			

		case 9:		// MIDICC
			// For every track in selection modify the corresponding attribute
			for (i=0; i< 10; i++) {
				if ((1<<i) & (target_page->trackSelection)) {

					target_page->Track[i]->event_offset[ATTR_MIDICC] = 0;

					modify_signed_parameter 
						(&target_page->Track[i]->attr_MCC, 
						 	TRACK_MIN_MIDICC, TRACK_MAX_MIDICC, direction, OFF, FIXED);
				}
			}
			break;
			

		case 10:	// MIDICH	
			// For every track in selection modify the corresponding attribute
			for (i=0; i< 10; i++) {

				if ((1<<i) & (target_page->trackSelection)) {

					target_page->Track[i]->event_offset[ATTR_MIDICH] = 0;

					if ( G_prev_rec_page == target_page->pageNdx )
					{
						Page_setTrackRecPattern( target_page, G_prev_rec_page_pattern );
					}

					modify_parameter 
						(&target_page->Track[i]->attr_MCH, 
						 	TRACK_MIN_MIDICH, TRACK_MAX_MIDICH, direction, OFF, FIXED);
				}
			}
			break;
	} // switch (rotNdx)

}

//Selected Tracks and Mix->edit(secondary) knob
void rot_exe_MIX2EDIT( 	unsigned char rotNdx,
							unsigned char direction,
							Pagestruct* target_page ) {
	// EDIT_TIMER handling
	//
	// Reset timer on every turn, so it doesnt run out
	if (target_page->trackSelection != 0) {

		unsigned char i=0;

		EDIT_TIMER = ON;
		// Setup alarm for the EDIT TIMER
		cyg_alarm_initialize(	alarm_hdl,
								cyg_current_time() + TIMEOUT_VALUE,
								0 );

		// Select the edit attribute
		target_page->editAttribute = rotNdx;

		switch (rotNdx) {
			case ROT_0:	// Transpose MIDICH
				// For every track in selection modify the corresponding attribute
				for (i=0; i< 10; i++) {

					if ((1<<i) & (target_page->trackSelection)) {

						target_page->Track[i]->event_offset[ATTR_MIDITCH] = 0;

							modify_parameter
								(&*target_page->Track[i]->attr_TCH,
									TRACK_MIN_MIDITCH, TRACK_MAX_MIDITCH, direction, OFF, FIXED);

					}
				}
				break;
		}
	} // Case where some track is selected
}
