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





	// TRACK Selectors
	switch( MODE_OBJECT_SELECTION ){

		// Select change track selection
		case BIRDSEYE:
			// Blink the currently selected track
			i = my_bit2ndx( target_page->trackSelection ) + 1;
			MIR_write_dot( i, MIR_RED   );
			MIR_write_dot( i, MIR_GREEN );
			MIR_write_dot( i, MIR_BLINK );
			break;

		case EMBEDDED:
			// CIRCLE OF PITCHES
			// If a step is pressed, show its absolute pitch in the inner circle
			i = is_pressed_rowzero();

			// Do not show this when working with the direction map
			if (	( i != 0 )
				&&	( target_page->trackAttributeSelection != ( 1 << DIRECTION ) )
				){

				// Compute coordinates
				row = my_bit2ndx( target_page->trackSelection );
				col = i - 1;

				// d_iag_printf( "step selected:%d\n", i );
				show_pitch_in_circle(
					target_page->Track[row]->attr_PIT +
					target_page->Step[row][col]->attr_PIT, ON );

				// Show the value of the current map attribute
				switch( target_page->trackAttributeSelection ){

					case ( 1 << VELOCITY ):
					case ( 1 << PITCH ):
						show_velocity_in_circle(
							normalize(
								target_page->Track[row]->attr_VEL +
								target_page->Step[row][col]->attr_VEL,
								0, 127 ) );
						break;

					case ( 1 << LENGTH ):
						show_velocity_in_circle(
							target_page->Step[row][col]->attr_LEN );
						break;

					case ( 1 << START ):
						show_velocity_in_circle(
							target_page->Step[row][col]->attr_STA );
						break;

					case ( 1 << AMOUNT ):
						show_velocity_in_circle(
							target_page->Step[row][col]->attr_AMT );
						break;

#if GROEF || 1
					case ( 1 << GROOVE ):
						PhraseShowPhraseNum( target_page->Step[row][col]->phrase_num );
						break;
#endif

					case ( 1 << MIDICC ):
						show_velocity_in_circle(
							target_page->Step[row][col]->attr_MCC );
						break;
				}
			}
			// break;

		default:
			// TRACK Selectors
			show (ELE_TRACK_SELECTORS, TRACK_ATTRIBUTES_MAPS);
			show (ELE_TRACK_SELECTORS, TRACK_ATTRIBUTE_SELECTION);
			break;
	}


	// MATRIX
	show( ELE_MATRIX, LAUFLICHT_BOTTOM ) ;
	show( ELE_MATRIX, ATTR_MAP_VALUES  ) ;


	// TRACK MUTATORS
	show( ELE_TRACK_MUTATORS, TRACK_ATTRIBUTES_MAP_FUNCTIONS );


	// EDIT MASTER
	switch( target_page->editorMode ){

		case CONTROL:
			// Edit acts as MCC source - send MCC data
			show( ELE_EDIT_MASTER, RED );
			break;

		case EDIT:
			// This is the regular mode - edit attributes of selection
			show( ELE_EDIT_MASTER, GREEN );
			break;

		case PREVIEW:
			// Same as regular mode, but preview the steps being edited
			show( ELE_EDIT_MASTER, RED );
			show( ELE_EDIT_MASTER, GREEN );
			show( ELE_EDIT_MASTER, BLINK );
			break;

		case PREVIEW_PERFORM:
			// Same as PREVUEW mode, but the MIDI stuff is not sent to out
			show( ELE_EDIT_MASTER, GREEN );
			show( ELE_EDIT_MASTER, BLINK );

			// This is the JohnK preview mode
			show( ELE_MATRIX, PREVIEW_STEP );
			break;

		case MIX2EDIT:
			// Edit acts as MCC source - send MCC data
			show( ELE_EDIT_INDICATOR, GREEN );
			show( ELE_EDIT_INDICATOR, RED );
			break;
	}


	// RETURN
	show (ELE_RETURN, GREEN);


	// GLOBALS
	show( ELE_CHAIN_INDICATOR, ON );
	show( ELE_GLOBALS, TRANSPORT );

	show( ELE_GLOBALS, FOLLOW );
	show( ELE_GLOBALS, CLOCK );


	// PLAY mode indicator
	switch( PLAY_MODE_STATUS ){

		case ON:
			MIR_write_dot( LED_ZOOM_PLAY, MIR_RED );
			MIR_write_dot( LED_ZOOM_PLAY, MIR_GREEN );
			MIR_write_dot( LED_ZOOM_PLAY, MIR_BLINK );

			// Here no PROGRAM LED because the button has a different function
			break;

		case OFF:
			MIR_write_dot( LED_ZOOM_PLAY, MIR_GREEN );
			break;
	}


	// G_master_tempoMUL of track
	// If one track (only) is selected in page
	if ( my_bit_cardinality( target_page->trackSelection ) == 1 ){

		// Display its tempo multiplier value
		display_Track_G_master_tempoMUL(
			target_page->Track[ my_bit2ndx( target_page->trackSelection ) ] );
	}


	// RECORDING LED
	if ( Page_getTrackRecPattern(target_page) == target_page->trackSelection ) {
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



	// CC resolution of the track - in CC map mode only..
	if ( target_page->trackAttributeSelection == ( 1 << MIDICC )
		){

//		MIR_write_dot( 252 + (7-target_page->Track[ my_bit2ndx( target_page->trackSelection ) ]->CC_resolution), MIR_RED   );
		MIR_write_dot( 252 + (7-target_page->Track[ my_bit2ndx( target_page->trackSelection ) ]->CC_resolution), MIR_GREEN );
	}




