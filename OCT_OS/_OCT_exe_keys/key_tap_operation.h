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





// Tap a step into the target page
// Place a note in the selcted tracks at current locator coordinates.
// No quantization. More than one track may be selected. Only playing tracks are considered.
void tap_step( Pagestruct* target_page ){

	unsigned int current_TTC = 0;
	unsigned char 	row = 0,
					target_col = 0,
					target_start = 0,
					j = 0
					;

	if (target_page->trackSelection == 0) {
		// Needs at least one selected track
		return;
	}

	// Note TTC coordinate
	current_TTC = G_TTC_abs_value;

	// Iterate through selected tracks and activate the notes.
	// if hitting an active note, modify its START value acc. to TTC (j)
	for (row=0; row<MATRIX_NROF_ROWS; row++) {

		// Found a selected track, activate the corresponding step, if the track is playing
		if (	( (target_page->trackSelection & (1<<row) ) != 0 )
			&&	( target_page->Track[row]->attr_LOCATOR != 0 )
			&&	( target_page->Track[row]->chain_data[PLAY] == target_page->Track[row] )
			){

			// Compute the coordinates of the step to be activated

			// Adjust the step start value according to current TTC
			// Reasoning: see book p.189
			if ( current_TTC <= 6 ){

				// Place step in current column
				target_col 		= target_page->Track[row]->attr_LOCATOR -1;
				target_start 	= current_TTC-1 + 6;
			}
			else {

				// Place step in next column, which may have to wrap
				target_col 		=
					get_next_tracklocator( 	target_page->Track[row],
											target_page->Track[row]->attr_LOCATOR ) -1;
				target_start 	= current_TTC-1 -6;
			}

			// Adjust the step START value
			target_page->Step[row][target_col]->attr_STA = target_start;

			// Turn on Step under the locator (adjusted by 1)
			Step_set_status( target_page->Step[row][target_col], STEPSTAT_TOGGLE, ON );


			// d_iag_printf( "tapped: row:%d t_col%d t_start:%d\n",
			//							row, target_col, target_start );

			// Play the note that was just set, for a better feeling

			j = 		target_page->Step [row][target_col]->attr_PIT
	  				+ 	target_page->Track[row]->attr_PIT
					+ 	(target_page->attr_PIT - MIDDLE_C)
					+ 	(target_page->Track[row]->lead_pitch_offset);

			// NOTE OFF build
			MIDI_OFF_build_new( 	target_page->Track[row]->attr_MCH, j,
									target_page->Track[row]->attr_LEN );


			// SEND NOTE ON
			MIDI_send(		MIDI_NOTE,
							target_page->Track[row]->attr_MCH
							+ target_page->Track[row]->event_offset[ATTR_MIDICH],
							j,
							target_page->Track[row]->attr_VEL );

		} // Selected track found

	} // Row iterator

}


