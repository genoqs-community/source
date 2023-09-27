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

		row = target_page->stepSelectionSingleRow;
		col = target_page->stepSelectionSingleCol;


		// ROW I
		if ( MODE_OBJECT_SELECTION != BIRDSEYE ){

			// Show value according to selected attribute
			switch( NEMO_selectedStepAttribute ){

				case ( NEMO_ATTR_PITCH ):
					// Show the chord note stack assigned to the step
					// << 1 due to some historics..
					#ifdef FEATURE_ENABLE_CHORD_OCTAVE
					if ( NEMO_step_VER >= VER_CHORD_OCTAVE_FIRST ) {
						unsigned char chord_octave = get_current_chord_octave();

						// Show the chord octave layers
						show_chord_octave(
							target_page->Step[row][col],
							( normalize( 	target_page->Track[row]->attr_PIT
										+ target_page->Step[row][col]->attr_PIT,
										0, 127 ) ) % 12,
							chord_octave
						);
					} else {
						// Show the chord octave positions orange, green and red
						show_chord_octave_first(
							target_page->Step[row][col],
							( normalize( 	target_page->Track[row]->attr_PIT
										+ target_page->Step[row][col]->attr_PIT,
										0, 127 ) ) % 12
						);
					}
					#else
					// Show absolute pitch in orange
					show_pitch_in_line(
						normalize( 	target_page->Track[row]->attr_PIT
									+ target_page->Step[row][col]->attr_PIT,
									0, 127 ),
						OFF );

					show_chord_in_line(
						target_page->Step[row][col],
						( normalize( 	target_page->Track[row]->attr_PIT
									+ target_page->Step[row][col]->attr_PIT,
									0, 127 ) )%12
						);
					#endif
					break;

				case ( NEMO_ATTR_START ):
					// Show the strum level
					show_strum_in_line( ( target_page->Step[row][col]->chord_data & 0xF800) >> 11 );
					break;

				case ( NEMO_ATTR_AMOUNT ):
					// Show the number of notes the step is playing
					#ifdef FEATURE_ENABLE_CHORD_OCTAVE
					MIR_fill_numeric 	(	0, get_chord_cardinality( target_page->Step[row][col], CHORD_OCTAVE_ALL ) + 2, NEMO_ROW_I,	MIR_GREEN );
					#else
					MIR_fill_numeric 	(	0, (my_bit_cardinality( target_page->Step[row][col]->chord_data & 0x7FF )) +1 +1, NEMO_ROW_I,	MIR_GREEN);
					#endif
					// Show current step polyphony
//					MIR_fill_numeric 	(	0, ((target_page->Step[row][col]->chord_up >> 29)) +1 +1, NEMO_ROW_I,	MIR_GREEN);
					MIR_fill_numeric 	(	0, ((target_page->Step[row][col]->chord_up >> 29)) +1 +1, NEMO_ROW_I,	MIR_RED);
					break;
			}
		} // Non-birdseye mode



		// ROW II
		// --> rather :: Show the available and the selected step event type

		if ( MODE_OBJECT_SELECTION != BIRDSEYE ){

			// Show available attributes:
			// PIT holds the pitch stack, VEL holds the strum level, AMT the polyphony
			MIR_write_trackpattern( 0x9400, NEMO_ROW_II, MIR_GREEN );

			// Show selected attribute: PIT, VEL, AMT are available
			switch( NEMO_selectedStepAttribute ){

				case ( NEMO_ATTR_PITCH ):
					MIR_augment_trackpattern( 0x8000, NEMO_ROW_II, MIR_RED );
					MIR_augment_trackpattern( 0x8000, NEMO_ROW_II, MIR_BLINK );
					break;

				case ( NEMO_ATTR_START ):
					MIR_augment_trackpattern( 0x1000, NEMO_ROW_II, MIR_RED );
					MIR_augment_trackpattern( 0x1000, NEMO_ROW_II, MIR_BLINK );
					break;

				case ( NEMO_ATTR_AMOUNT ):
					MIR_augment_trackpattern( 0x0400, NEMO_ROW_II, MIR_RED );
					MIR_augment_trackpattern( 0x0400, NEMO_ROW_II, MIR_BLINK );
					break;
			}
		}



		// ROW III
		//
		// Show the chord jump options: value or event
		MIR_write_trackpattern( 0x0110, 	NEMO_ROW_III, MIR_GREEN );

		// VALUE-EVENT-RANGE
		// Indicate VER status
		#ifdef FEATURE_ENABLE_CHORD_OCTAVE
		switch( NEMO_step_VER ){
			case VER_CHORD:
				MIR_write_dot( LED_VER_CHORD, MIR_RED 	);
				MIR_write_dot( LED_VER_CHORD, MIR_GREEN );
			break;

			case VER_CHORD_OCTAVE_FIRST:
				MIR_write_dot( LED_VER_CHORD, MIR_RED 	);
				MIR_write_dot( LED_VER_CHORD, MIR_GREEN );
				MIR_write_dot( LED_VER_CHORD, MIR_BLINK );
				break;

			case VER_CHORD_OCTAVE_SECOND:
				MIR_write_dot( LED_VER_CHORD, MIR_GREEN );
				MIR_write_dot( LED_VER_CHORD, MIR_BLINK );
				break;

			case VER_CHORD_OCTAVE_THIRD:
				MIR_write_dot( LED_VER_CHORD, MIR_RED );
				MIR_write_dot( LED_VER_CHORD, MIR_BLINK );
				break;
		}
		#else
		MIR_write_dot( LED_VER_CHORD, MIR_RED 	);
		MIR_write_dot( LED_VER_CHORD, MIR_GREEN );
		#endif


