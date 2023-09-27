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
			#ifdef FEATURE_STEP_EVENT_TRACKS
			if ( 	( NEMO_step_VER == VER_EVENT )
				||	( NEMO_step_VER == VER_RANGE ) ){
				show( ELE_TRACK_SELECTORS, STEP_ATTRIBUTE_SELECTION );
			}
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

		// Show the lauflicht of the row the Step is belonging to
		MIR_write_lauflicht_track( target_page->stepSelectionSingleRow, 3 );


		// TRACK MUTATORS
		//
		if (target_page->stepSelection != 0) {

			// Step rotarys shown
			if ( MODE_OBJECT_SELECTION != BIRDSEYE ){

				show( ELE_TRACK_MUTATORS, STEP_FUNCTIONS );
			}

			// Copy and Paste
			MIR_write_dot( LED_COPY, MIR_GREEN 	);
			MIR_write_dot( LED_COPY, MIR_RED 	);

			// PASTE: Check if buffer and target selection have the same cardinality
			if ( (STEP_COPY_BUFFER != NULL) ){

				MIR_write_dot( LED_PASTE, MIR_GREEN );
				MIR_write_dot( LED_PASTE, MIR_RED );
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
		show (ELE_GLOBALS, TRANSPORT);
		// TRANSPORT: Step LEN multiplier -> in NEMO it could be on the MCC stream bar ??
		// display_stepLEN_multiplier( target_page->Step[row][col] );


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


//		// Show the ROLL bit in the octave circle - when roll bit is set
//		if ( (target_page->Step[row][col]->attr_STATUS & (1 << 7)) != 0 ){
//			MIR_write_dot( LED_NOTE_Cup, MIR_RED );
//
//			if ( (target_page->Step[row][col]->attr_STATUS & (1 << 6)) == 0 ){
//				MIR_write_dot( LED_NOTE_Cup, MIR_GREEN );
//			}
//		}






