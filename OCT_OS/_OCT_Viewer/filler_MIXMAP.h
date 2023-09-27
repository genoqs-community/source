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
		// MIX INDICATOR - LED corresponding to the rotarys
		//
		show( ELE_MIX_INDICATOR, RED   );
		show( ELE_MIX_INDICATOR, GREEN );

		//
		// MIX MASTER
		//
		switch (target_page->mixMode) {
			case INC:
				show (ELE_MIX_MASTER, GREEN);
				break;

			case DEC:
				show (ELE_MIX_MASTER, RED);
				break;
		}

		//
		// TRACK_SELECTORS
		//
		switch( target_page->mixTarget ){

			case MIXTGT_ATR:
				// Selectable Attribute Set
				show( ELE_TRACK_SELECTORS, TRACK_ATTRIBUTES_ALL );

				// Selected attribute
				show( ELE_TRACK_SELECTORS, MIX_ATTRIBUTE_SELECTED );

				#ifdef FEATURE_STEP_SHIFT
				// SHIFT_SKIPS
				if (  (  target_page->mixAttribute == ATTR_POSITION )
							&& ( G_MixShiftSkips == 1 )  )   {

					MIR_write_dot( target_page->mixAttribute,	MIR_SHINE_RED);
				}
				#endif

				// ALIGN LED
				MIR_write_dot( LED_ALIGN, MIR_GREEN );
				break;

			case MIXTGT_USR0:
			case MIXTGT_USR1:
			case MIXTGT_USR2:
			case MIXTGT_USR3:
			case MIXTGT_USR4:
			case MIXTGT_USR5:

				// The CC maps are global, therefore only showing the global page
				target_page = GRID_assistant_page;

				// Show the available attributes of the CC_MIXMAP
				// ..if no track is selected for learning
				// RvdK Work this one out.
				switch( target_page->REC_bit ){

					case OFF:
						show( ELE_TRACK_SELECTORS, CC_MIXMAP_ATTRIBUTES );
						break;

					case ON:
						// Show the track selection - learning target blinking
						MIR_write_buttool( LHS, 1 << CCMAP_learner, MIR_RED 	);
						MIR_write_buttool( LHS, 1 << CCMAP_learner, MIR_BLINK 	);

						// Show the recording bit
						MIR_write_dot( LED_RECORD, MIR_RED 	 );
						MIR_write_dot( LED_RECORD, MIR_GREEN );
						MIR_write_dot( LED_RECORD, MIR_BLINK );

						// Show the currently selected attribute
						switch( target_page->CC_MIXMAP_attribute ){

							case CC_MIXMAP_AMT:
								i = 1<<6;
								break;
							case CC_MIXMAP_MCC:
								i = 1<<8;
								break;
							case CC_MIXMAP_MCH:
								i = 1<<9;
								break;
						}
						MIR_write_buttool( LHS, i, MIR_RED   );
						MIR_write_buttool( LHS, i, MIR_GREEN );

						break;
				}
				break;


			// These dont show anything for now
			case MIXTGT_VOL:
			case MIXTGT_PAN:
			case MIXTGT_MOD:
			case MIXTGT_EXP:
				break;
		}



		// SELECT MASTER
		//
		// see TRACK SELECTORS


		// MIX TARGET field
		//
		show( ELE_MIXTGT, OPTIONS );


		// MATRIX
		//
		show( ELE_MATRIX, MIXAMT_VALS );


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
				show( ELE_EDIT_INDICATOR, GREEN );
				show( ELE_EDIT_INDICATOR, RED );
				break;
		}



		// MUTATORS - show appropriate mutators
		switch( target_page->mixTarget ){

			case MIXTGT_ATR:

				// RND
				MIR_write_dot( LED_RANDOMIZE, MIR_GREEN );
				MIR_write_dot( LED_RANDOMIZE, MIR_RED );

				// CLR
				MIR_write_dot( LED_CLEAR, MIR_GREEN );
				MIR_write_dot( LED_CLEAR, MIR_RED );

				// CPY
				MIR_write_dot( LED_COPY, MIR_GREEN );
				MIR_write_dot( LED_COPY, MIR_RED );

				// PST
				// Make sure we have a valid buffer on store..
				if ( ATTR_MAP_COPY_BUFFER[10] == target_page->mixAttribute ){

					MIR_write_dot( LED_PASTE, MIR_GREEN );
					MIR_write_dot( LED_PASTE, MIR_RED );
				}
				break;


			case MIXTGT_USR1:
			case MIXTGT_USR2:
			case MIXTGT_USR3:
			case MIXTGT_USR4:
			case MIXTGT_USR5:

				// CLR
				MIR_write_dot( LED_CLEAR, MIR_GREEN );
				MIR_write_dot( LED_CLEAR, MIR_RED );
				break;

				// COPY
				// ..

				// PASTE
				// ..
		}



		// RETURN
		show (ELE_RETURN, GREEN);


		// GLOBALS
		//
		show (ELE_GLOBALS, TRANSPORT);
		show (ELE_GLOBALS, NUMERIC_QUADRANT);
		show (ELE_GLOBALS, BIG_KNOB);
		show( ELE_GLOBALS, CLOCK );
		show( ELE_GLOBALS, FOLLOW );

		// TRACK CHAINING
		show( ELE_CHAIN_INDICATOR, ON );

		// SCALE MASTER
		show_GLOBALS_scale_master( target_page );

		// OCTAVE CIRCLE
		// In PAGE mode show the SCALE, trans-position is in GRID
		if (	( target_page->scaleStatus == SCALE_SEL )
			||	( target_page->scaleStatus == SCALE_MOD )
			){

			// Show the selected notes in scale. Both in MOD and SEL
			// show (ELE_OCTAVE_CIRCLE, SCALE_SELECTION);
			show_OCTAVE_CIRCLE_scale_selection( target_page );

			// if ( target_page->scaleStatus == SCALE_SEL ){
				// This allows for editing to be done. No need otherwise.
			show_SCALE_SELECTOR_scale_selection( target_page );
			// }

		}

		// When forced to scale, show the scale
		if ( target_page->force_to_scale == ON ){
			// Show the selected notes in scale. Both in MOD and SEL
			// show (ELE_OCTAVE_CIRCLE, SCALE_SELECTION);
			show_OCTAVE_CIRCLE_scale_selection( target_page );
			// show_OCTAVE_CIRCLE_scale_align( target_page );
		}


		// RECORD
		// Global track record mode is red.
		if ( G_track_rec_bit == ON ) {
			MIR_write_dot( LED_RECORD, MIR_RED );
		}
		// Local page record mode is orange.
		else if ( target_page->REC_bit == ON ) {
			MIR_write_dot( LED_RECORD, MIR_GREEN );
			MIR_write_dot( LED_RECORD, MIR_RED );
		}
		// Blink when no ON notes are pending..
		if ( NOTE_queue == NULL ){
			MIR_write_dot( LED_RECORD, MIR_BLINK );
		}



