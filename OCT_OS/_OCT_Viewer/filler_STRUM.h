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
	// S T R U M  MAP MODE
	//

	//
	// MIX INDICATOR - LED corresponding to the rotarys
	//
	show( ELE_MIX_INDICATOR, RED   );
	show( ELE_MIX_INDICATOR, GREEN );

	// SEL Column  - show the available Strum parameters
#if PHRASE_EDIT_VIEW_NEMO
	MIR_write_dot( 1, 					MIR_GREEN );
	MIR_write_dot( 2, 					MIR_GREEN );
	MIR_write_dot( 3, 					MIR_GREEN );
	MIR_write_dot( 4, 					MIR_GREEN );
	MIR_write_dot( 6, 					MIR_GREEN );
#endif

#if PHRASE_EDIT_VIEW_OCTO
	MIR_write_dot( 1, 					MIR_GREEN );
	MIR_write_dot( 2, 					MIR_GREEN );
	MIR_write_dot( 3, 					MIR_GREEN );
	MIR_write_dot( 4, 					MIR_GREEN );
// The visual clue is given during usage. May remove the green LEDs in SEL column
//	MIR_write_dot( 5, 					MIR_GREEN );
//	MIR_write_dot( 6, 					MIR_GREEN );
//	MIR_write_dot( 7, 					MIR_GREEN );
//	MIR_write_dot( 8, 					MIR_GREEN );
//	MIR_write_dot( 9, 					MIR_GREEN );

	MIR_write_dot( 1, 					MIR_RED );
	MIR_write_dot( 2, 					MIR_RED );
	MIR_write_dot( 3, 					MIR_RED );
	MIR_write_dot( 4, 					MIR_RED );

	MIR_write_dot( 1 + PhraseCurAttrib, MIR_BLINK );
#endif


	if ( PhraseCurPhraseIx > 0 ) {

		// MUTATORS
		MIR_write_dot( LED_COPY, 			MIR_RED   );
		MIR_write_dot( LED_COPY, 			MIR_GREEN );

		switch( is_pressed_keyrange(1, 10) ) {
			case 1: case 2: case 3: case 4:
				MIR_write_dot( LED_CLEAR, 			MIR_RED   );
				MIR_write_dot( LED_CLEAR, 			MIR_GREEN );
				MIR_write_dot( LED_RANDOMIZE, 		MIR_RED   );
				MIR_write_dot( LED_RANDOMIZE, 		MIR_GREEN );
				break;
			default:
				MIR_write_dot( LED_CLEAR, 			MIR_RED   );
				MIR_write_dot( LED_CLEAR, 			MIR_GREEN );

				MIR_write_dot( LED_RANDOMIZE, 		MIR_RED   );
				MIR_write_dot( LED_RANDOMIZE, 		MIR_GREEN );
		}

		MIR_write_dot( LED_PASTE, 			MIR_RED   );
		MIR_write_dot( LED_PASTE, 			MIR_GREEN );
	}

	// MATRIX
	show( ELE_MATRIX, STRUM_VALS );


	// RETURN
	show (ELE_RETURN, GREEN);


	// GLOBALS
	show (ELE_GLOBALS, TRANSPORT);
	show (ELE_GLOBALS, BIG_KNOB);


	// Show the Strum view level in the chord indicator. Deep but efficient
	for ( i=0; i < 7; i++ ){
		MIR_write_dot( 258 - i, MIR_GREEN );
	}

#if PHRASE_EDIT_VIEW_NEMO
	MIR_write_dot( 258 - PhraseCurNoteIx, MIR_RED   );
	MIR_write_dot( 258 - PhraseCurNoteIx, MIR_BLINK );
#endif

#if PHRASE_EDIT_VIEW_OCTO
	for ( i=0; i < 7; i++ ){
		MIR_write_dot( 258 - i, MIR_GREEN );
	}

	if ( PhraseGetPoly(PhraseCurPhraseIx) ) {
		MIR_write_dot( 258 - (PhraseGetPoly(PhraseCurPhraseIx)-1), MIR_RED );
		MIR_write_dot( 258 - (PhraseGetPoly(PhraseCurPhraseIx)-1), MIR_BLINK );
	}
#endif


	// Signal the big knob select LED
	MIR_write_dot( LED_SCALE_MYSEL, MIR_RED   );
	MIR_write_dot( LED_SCALE_MYSEL, MIR_GREEN );
	MIR_write_dot( LED_SCALE_MYSEL, MIR_BLINK );

	// Update the step selection counter in page
	if ( target_page->stepSelection == 1 ){

		// Show the STEP LED - because you can go back to the step..
		MIR_write_dot( LED_ZOOM_STEP, MIR_GREEN );

//		row = target_page->stepSelectionSingleRow;
//		col = target_page->stepSelectionSingleCol;
//
//		// Show chord and strum values in the two circles
//		// ..
//		// Show absolute pitch in inner circle - orange - status is OFF
//		show_pitch_in_circle(
//			target_page->Track[row]->attr_PIT +
//			target_page->Step[row][col]->attr_PIT, OFF );
//
//		// Show the chord notes assigned to the step in the circle.
//		show_chord_in_circle( target_page->Step[row][col],
//			( target_page->Track[row]->attr_PIT +
//			  target_page->Step[row][col]->attr_PIT )%12 );
//
//		// Show the ROLL bit in the octave circle - when roll bit is set
//		if ( (target_page->Step[row][col]->attr_STATUS & (1 << 7)) != 0 ){
//			MIR_write_dot( LED_NOTE_Cup, MIR_RED );
//
//			if ( (target_page->Step[row][col]->attr_STATUS & (1 << 6)) == 0 ){
//				MIR_write_dot( LED_NOTE_Cup, MIR_GREEN );
//			}
//		}
	}
	else {
		show (ELE_GLOBALS, NUMERIC_QUADRANT);
	}

	// Show phrase index in row 0
	// PhraseShowPhraseNum( PhraseCurPhraseIx );
	unsigned char phraseNum = PhraseCurPhraseIx;
	if ( phraseNum <= 16 ){
		MIR_point_numeric(	phraseNum, 	  9,	MIR_GREEN );
		MIR_point_numeric(	phraseNum, 	  9,	MIR_BLINK );
	}
	else if ( phraseNum <= 32 ){
		MIR_point_numeric(	phraseNum-16, 9,	MIR_RED   );
		MIR_point_numeric(	phraseNum-16, 9,	MIR_BLINK );
	}
	else if ( phraseNum <= 48 ){
		MIR_point_numeric(	phraseNum-32, 9,	MIR_GREEN );
		MIR_point_numeric(	phraseNum-32, 9,	MIR_RED   );
		MIR_point_numeric(	phraseNum-32, 9,	MIR_BLINK );
	}
	else if ( phraseNum <= 64 ){
		MIR_point_numeric(	phraseNum-48, 9,	MIR_RED   );
		MIR_point_numeric(	phraseNum-48, 9,	MIR_BLINK );
	}

