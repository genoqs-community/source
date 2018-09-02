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



	
		// Display the value of each attribute for the SELECTED TRACK	

		// Find first the selected track
		for (i=0; i<MATRIX_NROF_ROWS; i++) {

			if ( (target_page->trackSelection & (1<<i)) > 0 ) {
				//  row is index of the first selected track
				row = i;
			}
		}
		// Now fill the matrix rows with the attribute values of track row



		// VELOCITY
		MIR_write_numeric_H( 	target_page->Track[row]->attr_VEL
							#ifdef EVENTS_ABSOLUTE
							+	target_page->Track[row]->event_offset[ATTR_VELOCITY]
							#endif // EVENTS_ABSOLUTE
							, 0);



		// PITCH
		MIR_write_pitch_H( target_page->Track[row]->attr_PIT
							#ifdef EVENTS_ABSOLUTE
							+ target_page->Track[row]->event_offset[ATTR_PITCH]
							#endif // EVENTS_ABSOLUTE
							, 1);



		// LENGTH - New school: showing the LEN factor
		MIR_fill_numeric 	(	1, 
								target_page->Track[row]->LEN_factor + 1
								+ target_page->Track[row]->event_offset[ATTR_LENGTH], 
								2,	MIR_GREEN);
		MIR_fill_numeric 	(	1, 
								target_page->Track[row]->LEN_factor + 1
								+ target_page->Track[row]->event_offset[ATTR_LENGTH], 
								2,	MIR_RED);


		
		// START - New school: showing the STA factor
		MIR_fill_numeric 	(	1, 
								target_page->Track[row]->STA_factor + 1
								+ target_page->Track[row]->event_offset[ATTR_START], 
								3,	MIR_RED);
		MIR_fill_numeric 	(	1, 
								target_page->Track[row]->STA_factor + 1
								+ target_page->Track[row]->event_offset[ATTR_START], 
								3,	MIR_GREEN);
		


		// POSITION
		// Position shows the actual track pattern and augments the skipped steps and events

		// These are the active steps - and the events
		MIR_write_trackpattern(   Page_get_trackpattern( target_page, row ) 
								| Page_get_event_trackpattern( target_page, row ),
								4, 
								MIR_GREEN );						

		// .. and here the event/chord mongers - are built into the above
		MIR_write_trackpattern(   Page_get_chord_trackpattern( target_page, row )
								| Page_get_event_trackpattern( target_page, row ), 
								4, 
								MIR_RED   );	

		MIR_augment_trackpattern( Page_get_skippattern(  target_page, row ), 4, MIR_RED   );
		MIR_augment_trackpattern( Page_get_selection_trackpattern( target_page, row ), 4, MIR_BLINK );



		// DIRECTION
		MIR_point_numeric 	(	target_page->Track[row]->attr_DIR
								+ target_page->Track[row]->event_offset[ATTR_DIRECTION], 	5, 	MIR_RED );
		MIR_point_numeric 	(	target_page->Track[row]->attr_DIR 	
								+ target_page->Track[row]->event_offset[ATTR_DIRECTION],	5, 	MIR_GREEN);



		// AMOUNT
		MIR_write_numeric_H (	target_page->Track[row]->attr_AMT
								+ target_page->Track[row]->event_offset[ATTR_AMOUNT]
								,6);


		// GROOVE
		MIR_fill_numeric 	(	1, 
								target_page->Track[row]->attr_GRV + 1, 	7,	MIR_RED);
		MIR_fill_numeric 	(	1, 
								target_page->Track[row]->attr_GRV + 1, 	7,	MIR_GREEN);



		// MIDICC
		if ( (	target_page->Track[row]->attr_MCC ) == MIDICC_NONE ){

			// Step is set to play legato, show legato flag
			MIR_write_trackpattern ( 0x0f, 8, MIR_GREEN);
		}
		else if ( (	target_page->Track[row]->attr_MCC ) == MIDICC_BENDER ){

			// Signal the bender as selected
			MIR_write_trackpattern ( 0x01, 8, MIR_RED);	
		}
		else if ( (	target_page->Track[row]->attr_MCC ) == MIDICC_PRESSURE ){
		
			// Signal the pressure as selected
			MIR_write_trackpattern ( 0x03, 8, MIR_RED);	
		}
		else {
			// MIDICC has a valid value 
			MIR_write_numeric_H (	target_page->Track[row]->attr_MCC, 	8);
		}


		// MIDICH
		j = target_page->Track[row]->attr_MCH 
			+ target_page->Track[row]->event_offset[ATTR_MIDICH];

		if ( j <= 16 ){
			MIR_point_numeric(	j, 	9,	MIR_GREEN );
		} 
		else if ( j <= 32 ){
			MIR_point_numeric(	j-16, 9,	MIR_RED );
		}
		else if ( j <= 48 ){
			MIR_point_numeric(	j-32, 9,	MIR_GREEN );
			MIR_point_numeric(	j-32, 9,	MIR_BLINK );
		}
		else if ( j <= 64 ){
			MIR_point_numeric(	j-48, 9,	MIR_RED );
			MIR_point_numeric(	j-48, 9,	MIR_BLINK );
		}
		



