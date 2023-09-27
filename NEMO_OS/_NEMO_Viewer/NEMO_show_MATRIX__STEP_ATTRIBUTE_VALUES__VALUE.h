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


		// ROW I
		// Behave according to selected attribute
		switch( NEMO_selectedStepAttribute ){

			case ( NEMO_ATTR_PITCH ):
				// Numeric offset for the step
				MIR_write_numeric_H( target_page->Step[row][col]->attr_PIT, NEMO_ROW_I );
				j = 0;
				break;

			case ( NEMO_ATTR_VELOCITY ):
				MIR_write_numeric_H( target_page->Step[row][col]->attr_VEL, NEMO_ROW_I );
				j = 1;
				break;

			case ( NEMO_ATTR_LENGTH ):
				#ifdef FEATURE_TEMPO_MULT_PLUS
				// Show Sel led flash green
				MIR_write_dot( LED_SELECT_MASTER, MIR_GREEN );
				MIR_write_dot( LED_SELECT_MASTER, MIR_BLINK );

				if  ( is_pressed_key ( KEY_SELECT_MASTER ) )  {
					// If LEN attribute key is held show Step LEN Multiplier in ROW I
					display_stepLEN_multiplier_At_Row( target_page->Step[row][col] );
				}
				else  {
					if ( target_page->Step[row][col]->attr_LEN == LEGATO ){
						// Step is set to play legato, show legato flag
						MIR_write_trackpattern ( 0x0f, 0, MIR_GREEN);
					}
					else {
						// The common case is when the length value is shown
						MIR_write_length_H( target_page->Step[row][col]->attr_LEN, NEMO_ROW_I );
					}
					j = 2;
				}
				#else
				if ( target_page->Step[row][col]->attr_LEN == LEGATO ){
					// Step is set to play legato, show legato flag
					MIR_write_trackpattern ( 0x0f, 0, MIR_GREEN);
				}
				else {
					// The common case is when the length value is shown
					MIR_write_length_H( target_page->Step[row][col]->attr_LEN, NEMO_ROW_I );
				}
				j = 2;
				#endif
				break;

			case ( NEMO_ATTR_START ):
				MIR_show_startbar_H (target_page->Step[row][col]->attr_STA, NEMO_ROW_I );
				j = 3;
				break;


			case ( NEMO_ATTR_AMOUNT ):
				MIR_write_numeric_H (target_page->Step[row][col]->attr_AMT,	NEMO_ROW_I );
				j = 5;
				break;

			case ( NEMO_ATTR_GROOVE ):
				if ( STEP_MAX_GROOVE > 48 ){
					MIR_write_numeric_H	( target_page->Step[row][col]->phrase_num,	NEMO_ROW_I);
				}
				else{
					if ( target_page->Step[row][col]->phrase_num <= 16 ){
						MIR_point_numeric(	target_page->Step[row][col]->phrase_num, 	  NEMO_ROW_I,	MIR_GREEN );
					}
					else if ( target_page->Step[row][col]->phrase_num <= 32 ){
						MIR_point_numeric(	target_page->Step[row][col]->phrase_num-16, NEMO_ROW_I,	MIR_RED );
					}
					else if ( target_page->Step[row][col]->phrase_num <= 48 ){
						MIR_point_numeric(	target_page->Step[row][col]->phrase_num-32, NEMO_ROW_I,	MIR_GREEN );
						MIR_point_numeric(	target_page->Step[row][col]->phrase_num-32, NEMO_ROW_I,	MIR_RED );
					}
					else if ( target_page->Step[row][col]->phrase_num <= 64 ){
						MIR_point_numeric(	target_page->Step[row][col]->phrase_num-48, NEMO_ROW_I,	MIR_RED );
						MIR_point_numeric(	target_page->Step[row][col]->phrase_num-48, NEMO_ROW_I,	MIR_BLINK );
					}
				}
				j = 6;
				break;

			case ( NEMO_ATTR_MIDICC ):
				if ( target_page->Step[row][col]->attr_MCC == MIDICC_NONE ){
					// Step is set to play legato, show legato flag
					MIR_write_trackpattern ( 0x0f, NEMO_ROW_I, MIR_GREEN);
				}
				else {
					// MIDICC has a valid value
					MIR_write_numeric_H (target_page->Step[row][col]->attr_MCC,	NEMO_ROW_I );
				}
				j = 7;
				break;

			case ( NEMO_ATTR_POSITION ):
				if ( target_page->Step[row][col]->phrase_num != 0 ){
					MIR_fill_numeric 	(	0, ((target_page->Step[row][col]->phrase_data & 0x0f)) +1 +1, NEMO_ROW_I,	MIR_GREEN);
					MIR_fill_numeric 	(	0, ((target_page->Step[row][col]->phrase_data & 0x0f)) +1 +1, NEMO_ROW_I,	MIR_RED);
				}
				j = 11;
				break;

			#ifdef FEATURE_STEP_SHIFT
				case ( NEMO_ATTR_DIRECTION ):
					j = 12;
					break;
			#endif

		}



		// ROW II
		// All attributes are available

		#ifdef FEATURE_STEP_SHIFT
			if ( target_page->Step[row][col]->phrase_num != 0 ){
				// Including POS bit
				MIR_write_trackpattern( 0xF718, 			NEMO_ROW_II, MIR_GREEN );
			}
			else{
				// Without POS bit
				MIR_write_trackpattern( 0xF708, 			NEMO_ROW_II, MIR_GREEN );
			}
		#else
			if ( target_page->Step[row][col]->phrase_num != 0 ){
				// Including POS bit
				MIR_write_trackpattern( 0xF710, 			NEMO_ROW_II, MIR_GREEN );
			}
			else{
				// Without POS bit
				MIR_write_trackpattern( 0xF700, 			NEMO_ROW_II, MIR_GREEN );
			}
		#endif

		#ifdef FEATURE_TEMPO_MULT_PLUS
		// TEMPO_PLUS - if.. is_pressed_key(Selct_Master) attr_LEN then flash green (not orange)
							// and show Step LEN multiplier on ROW I
		if ( ( NEMO_selectedStepAttribute == NEMO_ATTR_LENGTH )
			&& ( is_pressed_key ( KEY_SELECT_MASTER ) )  )  {

			// Show selected attribute as blink green (hence Step LEN multiplier)
			MIR_write_dot( 34, MIR_BLINK );
		}
		else {
			// Show selected attribute - one only
			MIR_write_trackpattern( 1 << (15 - j ), 	NEMO_ROW_II, MIR_RED );
			MIR_write_trackpattern( 1 << (15 - j ), 	NEMO_ROW_II, MIR_BLINK );
		}
		#else
		// Show selected attribute - one only
		MIR_write_trackpattern( 1 << (15 - j ), 	NEMO_ROW_II, MIR_RED );
		MIR_write_trackpattern( 1 << (15 - j ), 	NEMO_ROW_II, MIR_BLINK );
		#endif

		// ROW III
		switch( NEMO_selectedStepAttribute ){

			case NEMO_ATTR_PITCH:
			case NEMO_ATTR_START:
				// Show including the chord option
				MIR_write_trackpattern( 0x8110, 	NEMO_ROW_III, MIR_GREEN );
				break;

			case NEMO_ATTR_VELOCITY:
			case NEMO_ATTR_LENGTH:
			case NEMO_ATTR_AMOUNT:
			case NEMO_ATTR_GROOVE:
			case NEMO_ATTR_MIDICC:
				// Show without the chord option
				MIR_write_trackpattern( 0x0110, 	NEMO_ROW_III, MIR_GREEN );
				break;

			case NEMO_ATTR_POSITION:
				// Show without chord and event options
				MIR_write_trackpattern( 0x0100, 	NEMO_ROW_III, MIR_GREEN );
		}

		#ifdef FEATURE_STEP_SHIFT
			if ( NEMO_selectedStepAttribute != NEMO_ATTR_DIRECTION )  {
				// Indicate VER attribute
				MIR_write_dot( LED_VER_VALUE, MIR_RED );
			}
		#else
			// Indicate VER attribute
			MIR_write_dot( LED_VER_VALUE, MIR_RED );
		#endif




