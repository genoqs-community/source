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


#if PHRASE_EDIT_VIEW_OCTO
	if ( keyNdx >= 1 && keyNdx <= 4 ) {
		PhraseCurAttrib = keyNdx - 1;
	}
#endif

	// Do not react on key presses for the 0 index.
	if ( PhraseCurPhraseIx == 0) return;

	// CLEAR CONTENTS
	if ( keyNdx == KEY_CLEAR ){

		intn pressedKey = is_pressed_keyrange( 1, 10 );

		// Clear the currently shown values
		switch( pressedKey ) {
		case 0:	PhraseClear( PhraseCurPhraseIx ); 	 break;
		case 1:	PhraseClearVel( PhraseCurPhraseIx ); break;
		case 2:	PhraseClearPit( PhraseCurPhraseIx ); break;
		case 3:	PhraseClearLen( PhraseCurPhraseIx ); break;
		case 4: PhraseClearSta( PhraseCurPhraseIx ); break;
		}
	}



	// RANDOMIZE CONTENTS
	if ( keyNdx == KEY_RANDOMIZE ){
		intn pressedKey = is_pressed_keyrange( 1, 10 );

		// Clear the currently shown values
		switch( pressedKey ) {
		case 0: PhraseRandomize( PhraseCurPhraseIx ); 	 break;
		case 1: PhraseRandomizeVel( PhraseCurPhraseIx ); break;
		case 2:	PhraseRandomizePit( PhraseCurPhraseIx ); break;
		case 3:	PhraseRandomizeLen( PhraseCurPhraseIx ); break;
		case 4:	PhraseRandomizeSta( PhraseCurPhraseIx ); break;
		}
	}

	if ( keyNdx == KEY_COPY ) {
		PhraseCopy();
	}

	if ( keyNdx == KEY_PASTE ) {
		PhrasePaste();
	}

//	if ( keyNdx == KEY_MUTE_MASTER ) {
//		PhraseUndo();
//	}

//	if  ( keyNdx == KEY_ZOOM ) {
//		PhrasePrintRepos();
//	}


	// Allow STEP context related operations
	if ( target_page->stepSelection == 1 ){

		// Enter the chord aux notes via inner circle
// No need for this because the display is off anyway
//		key_OCT_CIRCLE_chord_STEP( keyNdx );

		// Switch back to STEP mode
		if ( ( keyNdx == KEY_ZOOM_STEP) ) {
			G_zoom_level = zoomSTEP;
		}
	}



	// ZOOM MODE
	//
	// GRID zoom mode
	if (	(keyNdx == KEY_ZOOM_GRID)
		||	(keyNdx == KEY_ZOOM_PAGE)
		|| 	(keyNdx == KEY_ZOOM_TRACK) ){

		switch( keyNdx ){

			case KEY_ZOOM_GRID:
				G_zoom_level = zoomGRID;
				break;

			case KEY_ZOOM_PAGE:
				G_zoom_level = zoomPAGE;
				break;

			case KEY_ZOOM_TRACK:
				G_zoom_level = zoomTRACK;
				break;
		}

		// Unselect all potentially selected steps in the track
		// ..to prevent conflicting state of track and step selected
		if ( target_page->stepSelection != 0 ){

			// Calculate the row of the selected track
			row = my_bit2ndx( target_page->trackSelection );

			// Iterate through all steps in that row (selected track)
			for (col=0; col<MATRIX_NROF_COLUMNS; col++) {

				// If step is selected
				if ( Step_get_status( target_page->Step[row][col], STEPSTAT_SELECT ) == ON ){

					// Mark step unselected
					Step_set_status( target_page->Step[row][col], STEPSTAT_SELECT, OFF );

					// And decrement step selection counter
					target_page->stepSelection --;
				}
			}
		}

	} // keyNdx is a KEY_ZOOM_xxx



	//
	// TRANSPORT functions - when nothing is selected in page
	//
	if ( G_clock_source != EXT ){

		switch( keyNdx ){

			case KEY_STOP:
				sequencer_command_STOP();
				break;


			case KEY_PAUSE:
				sequencer_command_PAUSE(OFF);
				break;


			case KEY_PLAY1:
			case KEY_PLAY2:
			case KEY_PLAY4:

				// Make sure the sequencer is running
				sequencer_command_PLAY();

				break;
		}
	}

