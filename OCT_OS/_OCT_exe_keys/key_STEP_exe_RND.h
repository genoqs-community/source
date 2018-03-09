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
// RND function application on STEP object
//

void Step_exe_randomize( Pagestruct* target_page, Stepstruct* target_step ){

	unsigned char row=0;

	// Check the stepAttributeSelection
	switch( target_page->stepAttributeSelection ){

		case OFF:
			// Randomize all Step parameters
			Step_randomize( target_step );
			break;

		// Randomize individual attributes
		case (1 << VELOCITY):
			target_step->attr_VEL
				= randomStepAttrValue( 	target_step,
										ATTR_VELOCITY,
										target_page->Track[row]->attr_AMT
										+ target_page->Track[row]->event_offset[ATTR_AMOUNT]
										);
			break;

		case (1 << PITCH):
			target_step->attr_PIT
				= randomStepAttrValue( 	target_step,
										ATTR_PITCH,
										target_page->Track[row]->attr_AMT
										+ target_page->Track[row]->event_offset[ATTR_AMOUNT]
										);
			break;

		case (1 << LENGTH):
			target_step->attr_LEN
				= randomStepAttrValue( 	target_step,
										ATTR_LENGTH,
										target_page->Track[row]->attr_AMT
										+ target_page->Track[row]->event_offset[ATTR_AMOUNT]
										);
			break;

		case (1 << START):
			target_step->attr_STA
				= randomStepAttrValue( 	target_step,
										ATTR_START,
										target_page->Track[row]->attr_AMT
										+ target_page->Track[row]->event_offset[ATTR_AMOUNT]
										);
			break;

		case (1 << AMOUNT):
			target_step->attr_AMT
				= randomStepAttrValue( 	target_step,
										ATTR_AMOUNT,
										target_page->Track[row]->attr_AMT
										+ target_page->Track[row]->event_offset[ATTR_AMOUNT]
										);
			break;

		case (1 << MIDICC):
			target_step->attr_MCC
				= randomStepAttrValue( 	target_step,
										ATTR_MIDICC,
										target_page->Track[row]->attr_AMT
										+ target_page->Track[row]->event_offset[ATTR_AMOUNT]
										);
			break;

	} // stepAttributeSelection switch
}


