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




	// BIG KNOB
	if ( rotNdx == 0 ){

		// Change the value of the selected attribute of the track
		switch( NEMO_selectedTrackAttribute ){
		
			case NEMO_ATTR_VELOCITY:
				// Modify VEL factor
				target_page->Track[ my_bit2ndx( target_page->trackSelection )]->event_offset[NEMO_ATTR_VELOCITY] = 0;
				modify_parameter( 	&target_page->Track[ my_bit2ndx( target_page->trackSelection )]->VEL_factor, 
									TRACK_MIN_VELFACTOR, 
									TRACK_MAX_VELFACTOR, 
									direction, OFF, FIXED );
				break;

			case NEMO_ATTR_PITCH:
				// Modify PIT factor
				target_page->Track[ my_bit2ndx( target_page->trackSelection )]->event_offset[NEMO_ATTR_PITCH] = 0;
				modify_parameter( 	&target_page->Track[ my_bit2ndx( target_page->trackSelection )]->PIT_factor, 
									TRACK_MIN_PITFACTOR, 
									TRACK_MAX_PITFACTOR, 
									direction, OFF, FIXED );
				break;

			case NEMO_ATTR_LENGTH:
				// Modify LEN factor
				target_page->Track[ my_bit2ndx( target_page->trackSelection )]->event_offset[NEMO_ATTR_LENGTH] = 0;
				modify_parameter( 	&target_page->Track[ my_bit2ndx( target_page->trackSelection )]->LEN_factor, 
									TRACK_MIN_LENFACTOR, 
									TRACK_MAX_LENFACTOR, 
									direction, OFF, FIXED );				
				break;

			case NEMO_ATTR_START:
				target_page->Track[ my_bit2ndx( target_page->trackSelection )]->event_offset[NEMO_ATTR_START] = 0;
				modify_parameter( 	&target_page->Track[ my_bit2ndx( target_page->trackSelection )]->STA_factor, 
									TRACK_MIN_STAFACTOR, 
									TRACK_MAX_STAFACTOR, 
									direction, OFF, FIXED );
				break;

			case NEMO_ATTR_AMOUNT:
				// Modify AMT factor
				target_page->Track[ my_bit2ndx( target_page->trackSelection )]->event_offset[NEMO_ATTR_AMOUNT] = 0;
				modify_parameter( 	&target_page->Track[ my_bit2ndx( target_page->trackSelection )]->AMT_factor, 
									TRACK_MIN_AMTFACTOR, 
									TRACK_MAX_AMTFACTOR, 
									direction, OFF, FIXED );
				break;

			case NEMO_ATTR_GROOVE:
				// Modify GRV factor
				target_page->Track[ my_bit2ndx( target_page->trackSelection )]->event_offset[NEMO_ATTR_GROOVE] = 0;
				modify_parameter( 	&target_page->Track[ my_bit2ndx( target_page->trackSelection )]->GRV_factor, 
									TRACK_MIN_GRVFACTOR, 
									TRACK_MAX_GRVFACTOR, 
									direction, OFF, FIXED );
				break;

			case NEMO_ATTR_MIDICC:
				// Modify MCC range - note the relation to STEP
				target_page->Track[ my_bit2ndx( target_page->trackSelection )]->event_offset[ATTR_MIDICC] = 0;
				modify_parameter( 	&target_page->Track[ my_bit2ndx( target_page->trackSelection )]->MCC_factor, 
									TRACK_MIN_MCCFACTOR, 
									TRACK_MAX_MCCFACTOR, 
									direction, OFF, FIXED );
				break;
				
		} // switch on selected track attribute
	} // rotNdx == 0


	//	
	// EDIT rotary: editing the hardwired attributes
	//
	else if ((rotNdx >= 1) && (rotNdx <= 10)) {

		// Point the display to the changing attribute					
		NEMO_selectedTrackAttribute = rotNdx;

		// Modify hardwired EDIT Attribute for trackSelection
		switch( rotNdx ){
			
			case NEMO_ATTR_VELOCITY:
				// Modify VEL factor
				target_page->Track[ my_bit2ndx( target_page->trackSelection )]->event_offset[NEMO_ATTR_VELOCITY] = 0;
				modify_parameter( 	&target_page->Track[ my_bit2ndx( target_page->trackSelection )]->VEL_factor, 
									TRACK_MIN_VELFACTOR, 
									TRACK_MAX_VELFACTOR, 
									direction, OFF, FIXED );
				break;

			case NEMO_ATTR_PITCH:
				// Modify PIT factor
				target_page->Track[ my_bit2ndx( target_page->trackSelection )]->event_offset[NEMO_ATTR_PITCH] = 0;
				modify_parameter( 	&target_page->Track[ my_bit2ndx( target_page->trackSelection )]->PIT_factor, 
									TRACK_MIN_PITFACTOR, 
									TRACK_MAX_PITFACTOR, 
									direction, OFF, FIXED );
				break;

			case NEMO_ATTR_LENGTH:
				// Modify LEN factor
				target_page->Track[ my_bit2ndx( target_page->trackSelection )]->event_offset[NEMO_ATTR_LENGTH] = 0;
				modify_parameter( 	&target_page->Track[ my_bit2ndx( target_page->trackSelection )]->LEN_factor, 
									TRACK_MIN_LENFACTOR, 
									TRACK_MAX_LENFACTOR, 
									direction, OFF, FIXED );				
				break;

			case NEMO_ATTR_START:
				target_page->Track[ my_bit2ndx( target_page->trackSelection )]->event_offset[NEMO_ATTR_START] = 0;
				modify_parameter( 	&target_page->Track[ my_bit2ndx( target_page->trackSelection )]->STA_factor, 
									TRACK_MIN_STAFACTOR, 
									TRACK_MAX_STAFACTOR, 
									direction, OFF, FIXED );
				break;
		} // switch (rotNdx)
		
	} // rotNdx between 1 and 10
	

