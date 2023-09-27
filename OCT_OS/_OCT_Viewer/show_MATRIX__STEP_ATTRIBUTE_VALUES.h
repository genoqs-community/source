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
		// Now fill the matrix rows with the attribute values of step


		// VELOCITY
		MIR_write_numeric_H( target_page->Step[row][col]->attr_VEL, 	0);

		// PITCH
		MIR_write_numeric_H( target_page->Step[row][col]->attr_PIT, 	1);

		// LENGTH
		#ifdef FEATURE_TEMPO_MULT_PLUS

		MIR_write_dot( LED_TEMPO, MIR_RED );
		MIR_write_dot( LED_TEMPO, MIR_GREEN );

		if ( is_pressed_key( KEY_TEMPO ))  {
			// If Tempo Key held show Step LEN Multiplier in POS row
			display_stepLEN_multiplier_At_Row( target_page->Step[row][col] );
		}
		else  {
			if ( target_page->Step[row][col]->attr_LEN == LEGATO ){
				// Step is set to play legato, show legato flag
				MIR_write_trackpattern ( 0x0f, 2, MIR_GREEN);
			}
			else {
				// The common case is when the length value is shown
				MIR_write_length_H( target_page->Step[row][col]->attr_LEN, 	2);
			}
		}
		#else
		if ( target_page->Step[row][col]->attr_LEN == LEGATO ){

			// Step is set to play legato, show legato flag
			MIR_write_trackpattern ( 0x0f, 2, MIR_GREEN);
		}
		else {
			// The common case is when the length value is shown
			MIR_write_length_H( target_page->Step[row][col]->attr_LEN, 	2);
		}
		#endif
		// START
		MIR_show_startbar_H (target_page->Step[row][col]->attr_STA, 		3);

		{
			intn phraseFactor   = ((target_page->Step[row][col]->phrase_data & 0x0f)) + 1;

//			intn phraseNeutral  = PHRASE_GRV_FACT_NEUTRAL + 1;
//			intn phraseType     = ((target_page->Step[row][col]->phrase_data & 0xf0) >> 4);

			intn phraseNum      = target_page->Step[row][col]->phrase_num;
			booln hundredBo     = False;

			if ( phraseNum >= 100 ) {
				phraseNum -= 100;
				hundredBo = True;
			}

			// POSITION
//			MIR_fill_numeric( 1, phraseFactor, 4, MIR_GREEN );
//			MIR_fill_numeric( phraseFactor, phraseFactor+1, 4, MIR_RED );
//			MIR_fill_numeric( phraseFactor+1, 17, 4, MIR_GREEN );
//			MIR_fill_numeric( phraseNeutral, phraseNeutral+1, 4, MIR_BLINK );

			// -> adjusted: show only when a phrase is active
			if ( phraseNum != 0 ){
				MIR_fill_numeric 	(	0, phraseFactor+1, 4,	MIR_GREEN);
				MIR_fill_numeric 	(	0, phraseFactor+1, 4,	MIR_RED);
			}

			// DIRECTION
//			MIR_fill_numeric( 1, phraseType+1, 5, MIR_GREEN );
//			MIR_fill_numeric( 1, phraseType+1, 5, MIR_GREEN );

			// Phrase type is inherent to the phrase - accessible via editor
//			MIR_point_numeric( phraseType, 5, MIR_GREEN );
//			MIR_point_numeric( phraseType, 5, MIR_RED );

			// GROOVE
			if ( PHRASE_COUNT_USER_RUNTIME > 65 ){
				MIR_write_numeric_H( phraseNum, 7 );
				if ( hundredBo ) MIR_fill_numeric( 10, 11, 7, MIR_RED );
			}
			else{
				if ( phraseNum <= 16 ){
					MIR_point_numeric(	phraseNum, 	  7,	MIR_GREEN );
				}
				else if ( phraseNum <= 32 ){
					MIR_point_numeric(	phraseNum-16, 7,	MIR_RED );
				}
				else if ( phraseNum <= 48 ){
					MIR_point_numeric(	phraseNum-32, 7,	MIR_GREEN );
					MIR_point_numeric(	phraseNum-32, 7,	MIR_RED );
				}
				else if ( phraseNum <= 64 ){
					MIR_point_numeric(	phraseNum-48, 7,	MIR_RED );
					MIR_point_numeric(	phraseNum-48, 7,	MIR_GREEN );
					MIR_point_numeric(	phraseNum-48, 7,	MIR_BLINK );
				}
			}
		}

		// AMOUNT
		MIR_write_numeric_H (target_page->Step[row][col]->attr_AMT,		6);


		// MIDICC
		if ( target_page->Step[row][col]->attr_MCC == MIDICC_NONE ){

			// Step is set to play legato, show legato flag
			MIR_write_trackpattern ( 0x0f, 8, MIR_GREEN);
		}
		else {
			// MIDICC has a valid value
			MIR_write_numeric_H (target_page->Step[row][col]->attr_MCC,		8);
		}


		// MIDICH
		// Shows the track pattern of the step's row and augments the skipped steps.
		MIR_write_trackpattern  ( Page_get_trackpattern( target_page, row ), 9, MIR_GREEN );

		// .. and here the event/chord mongers - are built into the above
		MIR_augment_trackpattern(   Page_get_chord_trackpattern( target_page, row )
								| Page_get_event_trackpattern( target_page, row ),
								9,
								MIR_RED   );
		// .. and here the event/chord mongers - are built into the above
		MIR_augment_trackpattern(   Page_get_chord_trackpattern( target_page, row )
								| Page_get_event_trackpattern( target_page, row ),
								9,
								MIR_GREEN   );

		MIR_augment_trackpattern( 	Page_get_skippattern(  target_page, row ),
									9,
									MIR_RED   );


		#ifdef FEATURE_ZOOMSTEP_PLUS  // Includes FIX_SHOW_HYPER (modified)

		MIR_augment_trackpattern( Page_get_selection_trackpattern( target_page, row),
									9, MIR_BLINK );

		if (  (Step_get_status( target_page->Step[row][col], STEPSTAT_TOGGLE ) == OFF )
			&& (target_page->Step[row][col]->hyperTrack_ndx == 10 )  )   {
				// Selected step is off
				// Color it red if it is off anyway
				MIR_write_dot( Page_dotIndex( 9, col ),  MIR_RED );
		}


		if (target_page->Step[row][col]->hyperTrack_ndx == 10 ) {
			MIR_augment_trackpattern( 	Page_get_hyperpattern(  target_page, row ),
									9, MIR_SHINE_GREEN   );
		}
		else {
			if ( Step_get_status( target_page->Step[row][col], STEPSTAT_SKIP ) == OFF ) {
				// Selected step is a hyperstep make it shine red
				MIR_write_dot( Page_dotIndex( 9, col ),  MIR_SHINE_RED );
			}
			// Show any other hypersteps
			int hcol = 0;
			for (hcol=0; hcol<16; hcol++) {
				if (  (  (hcol != col) && ( target_page->Step[row][hcol]->hyperTrack_ndx != 10 )  )
						&& ( Step_get_status( target_page->Step[row][hcol], STEPSTAT_SKIP ) == OFF )  )  {

					MIR_write_dot( Page_dotIndex( 9, hcol ),  MIR_SHINE_GREEN );
				}
			}
		}


		#else
		// Blink the selected step
		MIR_augment_trackpattern( Page_get_selection_trackpattern( target_page, row ),
								9, MIR_BLINK );
		if (Step_get_status( target_page->Step[row][col], STEPSTAT_TOGGLE ) == OFF ){
			// Color it red if it is off anyway
			MIR_write_dot( Page_dotIndex( 9, col ),  MIR_RED );
		}
		#endif






