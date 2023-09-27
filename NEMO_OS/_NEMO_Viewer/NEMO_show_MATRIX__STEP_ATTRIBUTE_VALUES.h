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


		// Shows the attributes of a selected and zoomed in STEP as bars
		// Applies to one selected step.
		row = target_page->stepSelectionSingleRow;
		col = target_page->stepSelectionSingleCol;
		// Extract the column of the selected step in the row.
		// Since we are in step mode this is safe
		// col = 15 - (my_bit2ndx( Page_get_selection_trackpattern( target_page, row ) ) );

		// ROW I-III
		// Handled separately in each mode
		// VALUE-EVENT-RANGE (+CHORD)
		switch( NEMO_step_VER ){

			case VER_VALUE:
				#include "NEMO_show_MATRIX__STEP_ATTRIBUTE_VALUES__VALUE.h"
				// ROW I	- ok
				// ROW II	- ok
				// ROW III 	- ok
				// ROW IV	- ok
				break;

			case VER_EVENT:
				#include "NEMO_show_MATRIX__STEP_ATTRIBUTE_VALUES__EVENT.h"
				// ROW I 	- ok
				// ROW II 	- ok
				// ROW III	- ok
				// ROW IV	- ok
				break;

			case VER_RANGE:
				#include "NEMO_show_MATRIX__STEP_ATTRIBUTE_VALUES__RANGE.h"
				// ROW I 	- ok
				// ROW II 	- ok
				// ROW III	- ok
				// ROW IV	- ok
				break;

			case VER_CHORD:
			#ifdef FEATURE_ENABLE_CHORD_OCTAVE
			case VER_CHORD_OCTAVE_FIRST:
			case VER_CHORD_OCTAVE_SECOND:
			case VER_CHORD_OCTAVE_THIRD:
			#endif
				#include "NEMO_show_MATRIX__STEP_ATTRIBUTE_VALUES__CHORD.h"
				// ROW I 	-
				// ROW II 	-
				// ROW III	-
				// ROW IV	- ok
				break;
	}

	// ROW IV
	// Same for all - show the status of the steps and act as step selector in track
	// Shows the actual track pattern and augments the skipped steps and events
	// These are the active steps - and the events
	MIR_write_trackpattern(   Page_get_trackpattern( target_page, row )
							| Page_get_event_trackpattern( target_page, row),
							NEMO_ROW_IV,	MIR_GREEN );

	// .. and here the event/chord mongers - are built into the above
	MIR_write_trackpattern(   Page_get_chord_trackpattern( target_page, row)
							| Page_get_event_trackpattern( target_page, row),
							NEMO_ROW_IV, 	MIR_RED   );
	MIR_augment_trackpattern( Page_get_skippattern(  target_page, row),
							NEMO_ROW_IV, 	MIR_RED   );

	#ifdef FEATURE_ZOOMSTEP_PLUS  // Includes FIX_SHOW_HYPER (modified)


	MIR_augment_trackpattern( Page_get_selection_trackpattern( target_page, row),
								NEMO_ROW_IV, MIR_BLINK );

	if (  (Step_get_status( target_page->Step[row][col], STEPSTAT_TOGGLE ) == OFF )
		&& (target_page->Step[row][col]->hyperTrack_ndx == 10 )  )   {
			// Selected step is off
			// Color it red if it is off anyway
			MIR_write_dot( Page_dotIndex( NEMO_ROW_IV, col ),  MIR_RED );
	}


	if (target_page->Step[row][col]->hyperTrack_ndx == 10 ) {
		MIR_augment_trackpattern( 	Page_get_hyperpattern(  target_page, row ),
								NEMO_ROW_IV, MIR_SHINE_GREEN   );
	}
	else {
		if ( Step_get_status( target_page->Step[row][col], STEPSTAT_SKIP ) == OFF ) {
			// Selected step is a hyperstep make it shine red
			MIR_write_dot( Page_dotIndex( NEMO_ROW_IV, col ),  MIR_SHINE_RED );
		}
		// Show any other hypersteps
		int hcol = 0;
		for (hcol=0; hcol<16; hcol++) {
			if (  (  (hcol != col) && ( target_page->Step[row][hcol]->hyperTrack_ndx != 10 )  )
					&& ( Step_get_status( target_page->Step[row][hcol], STEPSTAT_SKIP ) == OFF )  )  {

				MIR_write_dot( Page_dotIndex( NEMO_ROW_IV, hcol ),  MIR_SHINE_GREEN );
			}
		}
	}


	#else
	// Blink the selected step
	MIR_augment_trackpattern( Page_get_selection_trackpattern( target_page, row),
					NEMO_ROW_IV, 	MIR_BLINK );
	if (Step_get_status( target_page->Step[row][col], STEPSTAT_TOGGLE ) == OFF ){
		// Color it red if it is off anyway
		MIR_write_dot( Page_dotIndex(
					NEMO_ROW_IV, 	col ),  MIR_RED );
	}
	#endif



