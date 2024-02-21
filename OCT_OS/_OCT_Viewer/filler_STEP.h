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
// S T E P  MAP MODE
//

		// TRACK SELECTORS
		//
		// Show the available and the selected step attribute/s
		// --> rather :: Show the available and the selected step event type
		if ( MODE_OBJECT_SELECTION != BIRDSEYE ){

			// Switched to the new model inside..
			// Show available event attributes
			show( ELE_TRACK_SELECTORS, STEP_ATTRIBUTES_ALL );

			// Show currently selected event attribute
			if ( Step_get_status( target_page->Step[ target_page->stepSelectionSingleRow ]
								  				   [ target_page->stepSelectionSingleCol ],
   								  STEPSTAT_EVENT ) == ON
   				){

				show( ELE_TRACK_SELECTORS, STEP_ATTRIBUTE_SELECTION );
   			}

			#ifdef FEATURE_STEP_EVENT_TRACKS
			show( ELE_MIXTGT, OPTIONS );
			#endif
		}


		// MATRIX
		//
		// Blinks the selected step and shows its attribute values..
		// ..which are editable via editor
		if ( MODE_OBJECT_SELECTION == BIRDSEYE ){

			show( ELE_MATRIX, STEP_TOGGLE 	);
			show( ELE_MATRIX, STEP_SELECTED );
		}
		else{
			show( ELE_MATRIX, STEP_ATTRIBUTE_VALUES );
		}

		// Show the lauflicht of the row the Step is belonging to in bottom row
		MIR_write_lauflicht_track( target_page->stepSelectionSingleRow, 9 );

		// TRACK MUTATORS
		//
		if (target_page->stepSelection != 0) {

			// Step rotarys shown
			if ( MODE_OBJECT_SELECTION != BIRDSEYE ){

				show( ELE_TRACK_MUTATORS, STEP_FUNCTIONS );
			}
		}


		// EDIT INDICATOR
		//
		// Active when a step is selected - show " ' "
		show( ELE_EDIT_INDICATOR, GREEN );

		if (EDIT_TIMER != OFF){
			show( ELE_EDIT_INDICATOR, BLINK );
		}


		// RETURN
		//
		show (ELE_RETURN, GREEN);



		// Compute coordinates
		row = target_page->stepSelectionSingleRow;	// Keeps row coordinate of a selected single step
		col = target_page->stepSelectionSingleCol;	// Keeps col coordinate of a selected single step

		#ifdef FEATURE_ZOOMSTEP_PLUS
		// If selected step is skipped then the underlying step status is shown as follows:
		// Step On - Main Mute: Flash Green
		// Chord - Main Mute: Flash Orange
		// Event & Step On - Main Mute: Flash Orange
		// Event & Step Off - Main Mute: Orange
		// Hyperstep - Main Mute: Shine_Green

		if (  (  Step_get_status( target_page->Step[row][col], STEPSTAT_SKIP )  ) == ON ) {

			if (  (  (  Step_get_status( target_page->Step[row][col], STEPSTAT_TOGGLE )  ) == OFF )
					&&  ( Step_get_status( target_page->Step[row][col], STEPSTAT_EVENT  ) == ON ) )  {
					//Event & Step Off - Main Mute: Orange
					show( ELE_MUTE_MASTER, GREEN );
					show( ELE_MUTE_MASTER, RED );
			}

			if (  (  Step_get_status( target_page->Step[row][col], STEPSTAT_TOGGLE )  ) == ON ) {
				//Step On - Main Mute: Flash Green
				show( ELE_MUTE_MASTER, GREEN );
				if  ( Step_get_status( target_page->Step[row][col], STEPSTAT_EVENT  ) == ON ) {
					//Event & Step On - Main Mute: Flash Orange
					show( ELE_MUTE_MASTER, RED );
				}
				#ifdef FEATURE_ENABLE_CHORD_OCTAVE
					if ( 	( is_step_chord( target_page->Step[row][col] ) )
				#else
					if ( 	( my_bit_cardinality( target_page->Step[row][col]->chord_data & 0x7FF )>0 )
				#endif
					)  {
						// Chord - Main Mute: Flash Orange
						show( ELE_MUTE_MASTER, RED );
					}
				show( ELE_MUTE_MASTER, BLINK );

			}

			if ( target_page->Step[row][col]->hyperTrack_ndx != 10 )  {
				// Hyperstep - Main Mute: Shine_Green
				MIR_write_dot( LED_MUTE_MASTER,	MIR_SHINE_RED);
			}

			if (  (  (  Step_get_status( target_page->Step[row][col], STEPSTAT_TOGGLE )  ) == OFF )
					&& ( target_page->Step[row][col]->hyperTrack_ndx == 10 )  )  {
				//Step Off - Main Mute: Red
				show( ELE_MUTE_MASTER, RED );
			}
		}

		#endif

		// GLOBALS
		//
		// show (ELE_GLOBALS, TRANSPORT);
		// TRANSPORT: Step LEN multiplier
		display_stepLEN_multiplier( target_page->Step[row][col] );


		// RECORDING LED
		if ( Page_getTrackRecPattern(target_page) == (1<<row) ) {
			MIR_write_dot( LED_RECORD, MIR_RED 	 );
		}
		else if ( Page_getTrackRecPattern(target_page) ) {
			MIR_write_dot( LED_RECORD, MIR_GREEN );
		}
		if ( G_track_rec_bit == ON ) {
			if ( NOTE_queue == NULL ) {
				MIR_write_dot( LED_RECORD, MIR_BLINK );
			}
		}


		// CIRCLES: INNER and OUTER CIRCLE
		if ( MODE_OBJECT_SELECTION >= CHORDEYE ){
			#ifdef FEATURE_ENABLE_CHORD_OCTAVE
			// Show chord and strum values in the two circles
			// ..
			show_strum_in_circle( ( target_page->Step[row][col]->chord_data & 0xF800) >> 11 );

			// Signal the big knob select LED
			MIR_write_dot( LED_SCALE_MYSEL, MIR_RED   );
			MIR_write_dot( LED_SCALE_MYSEL, MIR_GREEN );


			// Show Chord
			if ( MODE_OBJECT_SELECTION >= CHORDEYE_OCTAVE_FIRST ) {
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
			// Show chord and strum values in the two circles
			// ..
			// Show absolute pitch in inner circle - orange - status is OFF
			show_pitch_in_circle(
				target_page->Track[row]->attr_PIT +
				target_page->Step[row][col]->attr_PIT, OFF );

			// Show the chord notes assigned to the step in the circle.
			show_chord_in_circle( target_page->Step[row][col],
								  ( target_page->Track[row]->attr_PIT +
				  					target_page->Step[row][col]->attr_PIT )%12 	);


//			// Show the ROLL bit in the octave circle - when roll bit is set
//			if ( (target_page->Step[row][col]->attr_STATUS & (1 << 7)) != 0 ){
//				MIR_write_dot( LED_NOTE_Cup, MIR_RED );
//
//				if ( (target_page->Step[row][col]->attr_STATUS & (1 << 6)) == 0 ){
//					MIR_write_dot( LED_NOTE_Cup, MIR_GREEN );
//				}
//			}
			// Show step strum value in the outer circle
//			PhraseShowPhraseNum( target_page->Step[row][col]->phrase_num );
			show_strum_in_circle( ( target_page->Step[row][col]->chord_data & 0xF800) >> 11 );

			// Signal the big knob select LED
			MIR_write_dot( LED_SCALE_MYSEL, MIR_RED   );
			MIR_write_dot( LED_SCALE_MYSEL, MIR_GREEN );
			#endif
		}
		else{

			// If an event is set show the range defined for set event.
			if ( Step_get_status( target_page->Step[row][col], STEPSTAT_EVENT ) == ON
   				){
				#ifdef FEATURE_STEP_EVENT_TRACKS
				unsigned char temp = ( target_page->Step[row][col]->event_data & 0x0F ) + 1;
				#endif
				// Depending on which event is set, show range
				switch( (target_page->Step[row][col]->event_data & 0x0F) + 1 ){

					case ATTR_VELOCITY:

						MIR_write_numeric_C( target_page->Track[row]->event_max[ATTR_VELOCITY] );
						MIR_write_dot( LED_SCALE_MYSEL, MIR_RED   );
						MIR_write_dot( LED_SCALE_MYSEL, MIR_GREEN );
						break;

					case ATTR_PITCH:

						MIR_write_numeric_C( target_page->Track[row]->event_max[ATTR_PITCH] );
						MIR_write_dot( LED_SCALE_MYSEL, MIR_RED   );
						MIR_write_dot( LED_SCALE_MYSEL, MIR_GREEN );
						break;

					case ATTR_LENGTH:

						MIR_write_numeric_C( target_page->Track[row]->event_max[ATTR_LENGTH] );
						MIR_write_dot( LED_SCALE_MYSEL, MIR_RED   );
						MIR_write_dot( LED_SCALE_MYSEL, MIR_GREEN );
						break;

					case ATTR_START:

						MIR_write_numeric_C( target_page->Track[row]->event_max[ATTR_START] );
						MIR_write_dot( LED_SCALE_MYSEL, MIR_RED   );
						MIR_write_dot( LED_SCALE_MYSEL, MIR_GREEN );
						break;
					#ifdef FEATURE_STEP_EVENT_TRACKS
					case ATTR_POSITION:

						MIR_write_numeric_C( target_page->Track[row]->event_max[ATTR_POSITION] );
						MIR_write_dot( LED_SCALE_MYSEL, MIR_RED   );
						MIR_write_dot( LED_SCALE_MYSEL, MIR_GREEN );
						break;
					#endif
					case ATTR_DIRECTION:

						MIR_write_numeric_C( target_page->Track[row]->event_max[ATTR_DIRECTION] );
						MIR_write_dot( LED_SCALE_MYSEL, MIR_RED   );
						MIR_write_dot( LED_SCALE_MYSEL, MIR_GREEN );
						break;

					case ATTR_AMOUNT:

						MIR_write_numeric_C( target_page->Track[row]->event_max[ATTR_AMOUNT] );
						MIR_write_dot( LED_SCALE_MYSEL, MIR_RED   );
						MIR_write_dot( LED_SCALE_MYSEL, MIR_GREEN );
						break;

					case ATTR_GROOVE:

						MIR_write_numeric_C( target_page->Track[row]->event_max[ATTR_GROOVE] );
						MIR_write_dot( LED_SCALE_MYSEL, MIR_RED   );
						MIR_write_dot( LED_SCALE_MYSEL, MIR_GREEN );
						break;

					case ATTR_MIDICC:

						MIR_write_numeric_C( target_page->Track[row]->event_max[ATTR_MIDICC] );
						MIR_write_dot( LED_SCALE_MYSEL, MIR_RED   );
						MIR_write_dot( LED_SCALE_MYSEL, MIR_GREEN );
						break;

					case ATTR_MIDICH:

						MIR_write_numeric_C( target_page->Track[row]->event_max[ATTR_MIDICH] );
						MIR_write_dot( LED_SCALE_MYSEL, MIR_RED   );
						MIR_write_dot( LED_SCALE_MYSEL, MIR_GREEN );
						break;
					#ifdef FEATURE_STEP_EVENT_TRACKS
					default:
						if ( temp >= EVENT_FLAG_TRACK_MUTE ) {
							MIR_write_numeric_C( target_page->Track[row]->event_max[temp] );
							MIR_write_dot( LED_SCALE_MYSEL, MIR_RED   );
							MIR_write_dot( LED_SCALE_MYSEL, MIR_GREEN );
						}
						break;
					#endif
				} // switch on selected event type
			}

			else{
				// Show the absolute PIT and VEL of the step in the circle - status is ON
				show_pitch_in_circle(
					target_page->Track[row]->attr_PIT +
					target_page->Step[row][col]->attr_PIT, ON );

				show_velocity_in_circle(
						normalize(
								target_page->Track[row]->attr_VEL +
								target_page->Step[row][col]->attr_VEL,
								0, 127 ) );
			}
		}


		// CHORD ARRAY
		// Show the real STEP CHORD cardinality in the chord indicator in red.
		#ifdef FEATURE_ENABLE_CHORD_OCTAVE
		if( MODE_OBJECT_SELECTION > CHORDEYE ) {
			// Chord mode toggle state (blink in stack chord mode)
			MIR_write_dot( LED_SCALE_MYSEL, MIR_BLINK );

			// Chord octave button 1 - 3 octave green unselected
			MIR_write_dot( LED_SCALE_MOD, MIR_GREEN );
			MIR_write_dot( LED_SCALE_SEL, MIR_GREEN );
			MIR_write_dot( LED_SCALE_CAD, MIR_GREEN );

			// Selected chord octave orange blink
			switch( MODE_OBJECT_SELECTION ) {

				case CHORDEYE_OCTAVE_FIRST:

					MIR_write_dot( LED_SCALE_MOD, MIR_RED );
					MIR_write_dot( LED_SCALE_MOD, MIR_BLINK );
					break;

				case CHORDEYE_OCTAVE_SECOND:

					MIR_write_dot( LED_SCALE_SEL, MIR_RED );
					MIR_write_dot( LED_SCALE_SEL, MIR_BLINK );
					break;

				case CHORDEYE_OCTAVE_THIRD:

					MIR_write_dot( LED_SCALE_CAD, MIR_RED );
					MIR_write_dot( LED_SCALE_CAD, MIR_BLINK );
					break;
			}
		}

		// Show number of notes in chord
		MIR_write_dot( 258 - get_chord_cardinality( target_page->Step[row][col], CHORD_OCTAVE_ALL ), MIR_RED );
		#else
		MIR_write_dot( 258 -
			my_bit_cardinality( target_page->Step[row][col]->chord_data & 0x7FF )+ 0, MIR_RED   );
		#endif
		// Show chord_size of the step - current and editable, in green.
//		MIR_write_dot( 258 -
//			3, MIR_GREEN );

		MIR_write_dot( 258 -
			(target_page->Step[row][col]->chord_up >> 29 )+ 0, MIR_GREEN );
