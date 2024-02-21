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
		// Press the MIX master to select the attribute of the CC Map
		show( ELE_MIX_MASTER, GREEN );
		if ( is_pressed_key( KEY_MIX_MASTER )){
			show( ELE_MIX_MASTER, MIR_BLINK );
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
				break;


			case MIXTGT_USR1:
			case MIXTGT_USR2:
			case MIXTGT_USR3:
			case MIXTGT_USR4:

				// The CC maps are global, therefore only showing the global page
				target_page = GRID_assistant_page;

				shiftTrackRow = track_get_window_shift( target_page );

				// Show the available attributes of the CC_MIXMAP
				// ..if no track is selected for learning
				switch( target_page->REC_bit ){

					case OFF:
						// Make sure MIX master is pressed - break if not pressed
						if ( is_pressed_key( KEY_MIX_MASTER ) ){
							show( ELE_TRACK_SELECTORS, CC_MIXMAP_ATTRIBUTES );
							}
						break;

					case ON:
						if ( is_pressed_key( KEY_MIX_MASTER )
							){
							// Show the selection of CC map attributes
							show( ELE_TRACK_SELECTORS, CC_MIXMAP_ATTRIBUTES );
						}
						else{
							// Show the track selection - learning target blinking
							MIR_write_buttool( LHS, 1 << ( CCMAP_learner - shiftTrackRow ), MIR_RED 	);
							MIR_write_buttool( LHS, 1 << ( CCMAP_learner - shiftTrackRow ), MIR_BLINK 	);
							// MIR_write_buttool( LHS, 1 << CCMAP_learner, MIR_GREEN 	);

							// Show the recording bit
							MIR_write_dot( LED_RECORD, MIR_RED 	 );
							// MIR_write_dot( LED_RECORD, MIR_GREEN );
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
						}
						break;
				}
				break;
		}


		// MIX TARGET field
		//
		// If we are looking at a CC map.. i.e. not at an attribute
		if ( target_page->mixTarget != MIXTGT_ATR ){
			show( ELE_MIXTGT, OPTIONS );
		}

		// MATRIX
		//
		if ( !is_pressed_key( KEY_MIX_MASTER ) ){
			show( ELE_MATRIX, MIXAMT_VALS );
		}
		else {

			if ( 	( target_page->mixTarget == MIXTGT_ATR )
				// &&	( is_pressed_key( KEY_MIX_MASTER ) )
				){

				if ( target_page->OPS_mode != BIRDSEYE ){

					// All NEMO attributes are available
					MIR_write_trackpattern( 0x0000, NEMO_ROW_I, MIR_GREEN );
					MIR_write_trackpattern( 0xF758, NEMO_ROW_II, MIR_GREEN );
					MIR_write_trackpattern( 0x0000, NEMO_ROW_III, MIR_GREEN );
					MIR_write_trackpattern( 0x0000, NEMO_ROW_IV, MIR_GREEN );

					// Blink the one assigned/selected - find out j
					switch( target_page->mixAttribute ){
						case NEMO_ATTR_PITCH:		j = 0;		break;
						case NEMO_ATTR_VELOCITY:	j = 1;		break;
						case NEMO_ATTR_LENGTH:		j = 2;		break;
						case NEMO_ATTR_START:		j = 3;		break;
						case NEMO_ATTR_AMOUNT:		j = 5;		break;
						case NEMO_ATTR_GROOVE:		j = 6;		break;
						case NEMO_ATTR_MIDICC:		j = 7;		break;
						case NEMO_ATTR_MIDICH:		j = 9;		break;
						case NEMO_ATTR_POSITION:	j = 11;		break;
						case NEMO_ATTR_DIRECTION:	j = 12;		break;
					}

					// Show selected attribute - one only
					MIR_write_trackpattern( 1 << (15 - j ), 	NEMO_ROW_II, MIR_RED );
					MIR_write_trackpattern( 1 << (15 - j ), 	NEMO_ROW_II, MIR_BLINK );
				}
			}
		}



		// MUTATORS - show appropriate mutators
		switch( target_page->mixTarget ){

			case MIXTGT_ATR:

				// RND
				MIR_write_dot( LED_RMX, MIR_GREEN );
				MIR_write_dot( LED_RMX, MIR_RED );

				// CLR
				MIR_write_dot( LED_CLEAR, MIR_GREEN );
				MIR_write_dot( LED_CLEAR, MIR_RED );

//				// CPY
//				MIR_write_dot( LED_COPY, MIR_GREEN );
//				MIR_write_dot( LED_COPY, MIR_RED );
//
//				// PST
//				// Make sure we have a valid buffer on store..
//				if ( ATTR_MAP_COPY_BUFFER[10] == target_page->mixAttribute ){
//
//					MIR_write_dot( LED_PASTE, MIR_GREEN );
//					MIR_write_dot( LED_PASTE, MIR_RED );
//				}
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


