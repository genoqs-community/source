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
	unsigned char temp=0;
			
// d_iag_printf( "rotNdx:%d dir:%d pgNdx:%d\n", rotNdx, direction, target_page->pageNdx );

	unsigned char quickturn = 0;
	
	// When only one track is selected, it is safe to use the quickturn
	switch ( my_bit_cardinality( target_page->trackSelection ) ){
		case 1:
			quickturn = ON;
			break;

		default:
			quickturn = OFF;
			break;
	}


	// EDIT_TIMER handling
	//
	// Reset timer on every turn, so it doesnt run out
	if (target_page->trackSelection != 0) {

		switch( target_page->editorMode ) {
		
			// No track selected and EDITOR is in Controller Mode -> send MIDI CC data
			case CONTROL:
	
				// For every track in selection modify the corresponding attribute
				if ( target_page->trackSelection == 0 ) 
					break;

				for ( i=0; i < 10; i++ ){

					// On all selected tracks
					if ((1<<i) & (target_page->trackSelection)) {
						
						// Short for MIDI channel
						temp = target_page->Track[i]->attr_MCH	+ target_page->Track[i]->event_offset[ATTR_MIDICH];
		
						// Remember the change
						modify_parameter 
							(&NEMO_CC_Sound_Control[ temp ][ rotNdx - 1 ], 
								0, 127, direction, ON, FIXED);	

						// Send MIDICC on the corresponding channel
						MIDI_send( 	MIDI_CC,
									temp,
									80 - rotNdx,
									NEMO_CC_Sound_Control[ temp ][ rotNdx - 1 ]
									);
					}
				}
				break;	

			default:
			
				EDIT_TIMER = ON;				

				// Setup alarm for the EDIT TIMER
				cyg_alarm_initialize(	alarm_hdl, 
										cyg_current_time() + TIMEOUT_VALUE, 
										0 );
		
				// Select the edit attribute
				switch( rotNdx ){
				
					case 1:	target_page->editAttribute = NEMO_ATTR_PITCH;		break;
					case 2:	target_page->editAttribute = NEMO_ATTR_VELOCITY;	break;
					case 3:	target_page->editAttribute = NEMO_ATTR_LENGTH;		break;
					case 4:	target_page->editAttribute = NEMO_ATTR_START;		break;
				}

				break;
		}
		
	} // Case where some track is selected

	

	// 
	// ATTRIBUTE MODIFY - EDITOR of trackSelection (hardwired)
	//
	switch (rotNdx) {
		
		case ROT_PIT:		// VELOCITY
			// For every track in selection modify the corresponding attribute
			for (i=0; i< 10; i++) {
				if ((1<<i) & (target_page->trackSelection)) {

					target_page->Track[i]->event_offset[NEMO_ATTR_PITCH] = 0;

					modify_parameter 
						(&target_page->Track[i]->attr_PIT, 
							TRACK_MIN_PITCH, TRACK_MAX_PITCH, direction, quickturn, FIXED);	
				}
			}
			break;


		case ROT_VEL:		// PITCH
			// For every track in selection modify the corresponding attribute
			for (i=0; i< 10; i++) {
				if ((1<<i) & (target_page->trackSelection)) {

					target_page->Track[i]->event_offset[NEMO_ATTR_VELOCITY] = 0;

					modify_parameter 
						(&target_page->Track[i]->attr_VEL, 
						 	TRACK_MIN_VELOCITY, TRACK_MAX_VELOCITY, direction, quickturn, FIXED);
				}
			}
			break;


		case ROT_LEN:		// LENGTH
			// For every track in selection modify the corresponding attribute
			for (i=0; i< 10; i++) {
				if ((1<<i) & (target_page->trackSelection)) {

					target_page->Track[i]->event_offset[NEMO_ATTR_LENGTH] = 0;

					modify_parameter(  
						&target_page->Track[i]->LEN_factor, 
						// 1, 16, direction, ON);
						TRACK_MIN_LENFACTOR, TRACK_MAX_LENFACTOR, direction, OFF, FIXED);
				}
			}
			break;
			

		case ROT_STA:		// START
			// For every track in selection modify the corresponding attribute
			for (i=0; i< 10; i++) {
				if ((1<<i) & (target_page->trackSelection)) {

					target_page->Track[i]->event_offset[NEMO_ATTR_START] = 0;

					modify_parameter( 
						&target_page->Track[i]->STA_factor, 
						TRACK_MIN_STAFACTOR, TRACK_MAX_STAFACTOR, direction, OFF, FIXED);
				}
			}
			break;
			
	} // switch (rotNdx)

}

//Selected Tracks and Mix->edit(secondary) knob
void rot_exe_MIX2EDIT	( 	unsigned char rotNdx,
							unsigned char direction,
							Pagestruct* target_page ) {
#ifdef FEATURE_ENABLE_KEYB_TRANSPOSE
	unsigned char i=0;
// d_iag_printf( "rotNdx:%d dir:%d pgNdx:%d\n", rotNdx, direction, target_page->pageNdx );

	unsigned char quickturn = 0;

	// When only one track is selected, it is safe to use the quickturn
	switch ( my_bit_cardinality( target_page->trackSelection ) ){
		case 1:
			quickturn = ON;
			break;

		default:
			quickturn = OFF;
			break;
	}

	// EDIT_TIMER handling
	//
	// Reset timer on every turn, so it doesnt run out
	if (target_page->trackSelection != 0) {

		EDIT_TIMER = ON;

		// Setup alarm for the EDIT TIMER
		cyg_alarm_initialize(	alarm_hdl,
								cyg_current_time() + TIMEOUT_VALUE,
								0 );

		switch (rotNdx) {
			// Transpose MIDICH
			case MIXTGT_EXP:
				target_page->editAttribute	= NEMO_ATTR_MIDITCH;
				// For every track in selection modify the corresponding attribute
				for (i=0; i< 10; i++) {
					if ((1<<i) & (target_page->trackSelection)) {

						target_page->Track[i]->event_offset[ATTR_MIDITCH] = 0;

						modify_parameter
							(&target_page->Track[i]->attr_STATUS,
								TRACK_MIN_MIDITCH, TRACK_MAX_MIDITCH, direction, quickturn, FIXED);
					}
				}
				break;
		}
	} // Case where some track is selected
#endif
}
