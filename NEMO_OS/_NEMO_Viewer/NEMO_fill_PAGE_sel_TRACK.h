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





			// Show GREEN Select master option
			show (ELE_SELECT_MASTER, GREEN);

			// Show the current step selection set
			if ( is_pressed_key( KEY_SELECT_MASTER )
				){
				switch( target_page->stepSELpattern_ndx ){

					case 0:	MIR_write_dot( LED_MIXTGT_USR1, MIR_GREEN 	);
							MIR_write_dot( LED_MIXTGT_USR1, MIR_BLINK 	);
						break;
					case 1:	MIR_write_dot( LED_MIXTGT_USR2, MIR_GREEN 	);
							MIR_write_dot( LED_MIXTGT_USR2, MIR_BLINK 	);
						break;
					case 2:	MIR_write_dot( LED_MIXTGT_USR3, MIR_GREEN 	);
							MIR_write_dot( LED_MIXTGT_USR3, MIR_BLINK 	);
						break;
					case 3:	MIR_write_dot( LED_MIXTGT_USR4, MIR_GREEN 	);
							MIR_write_dot( LED_MIXTGT_USR4, MIR_BLINK 	);
						break;
					case 4:	MIR_write_dot( LED_MIXTGT_USR5, MIR_GREEN 	);
							MIR_write_dot( LED_MIXTGT_USR5, MIR_BLINK 	);
						break;
				} // switch
			}


			// TRACK MUTATORS
			// Track functions shown
			show ( ELE_TRACK_MUTATORS, TRACK_FUNCTIONS );

			// Solo
			MIR_write_dot( LED_MUTE_MASTER, MIR_GREEN 	);
			MIR_write_dot( LED_MUTE_MASTER, MIR_RED 	);

			// Copy and Paste
			MIR_write_dot( LED_COPY, MIR_GREEN 	);
			MIR_write_dot( LED_COPY, MIR_RED 	);

			// PASTE: Check if buffer and target selection have the same cardinality
			if ((TRACK_COPY_BUFFER != 0) &&
				(my_bit_cardinality( TRACK_COPY_BUFFER & 0x3FF ) ==
					my_bit_cardinality( target_page->trackSelection ) )
				){

				MIR_write_dot( LED_PASTE, MIR_GREEN );
				MIR_write_dot( LED_PASTE, MIR_RED );
			}

			// CHAINER LED
			MIR_write_dot( LED_CHAINER, MIR_GREEN );

			// Show the chains play head status in the MIX MASTER
			// Make it dependant on whether the track is head in its chain??
			show( ELE_CHAIN_INDICATOR, ON );

			// EDIT INDICATOR
			// Show activation status: all are active
			show( ELE_EDIT_INDICATOR, RED   );
			show( ELE_EDIT_INDICATOR, GREEN );

			// Only one track is selected
			if (	( my_bit_cardinality( target_page->trackSelection ) == 1 )
				&&	( my_bit2ndx( target_page->trackSelection ) !=
						target_page->attr_mix2edit - 1 )
				){

				show( ELE_EDIT_INDICATOR, BLINK );

				// Show the EDIT MASTER in green
				show( ELE_EDIT_MASTER, GREEN );
			}

			if ( EDIT_TIMER != OFF ){
				show( ELE_EDIT_INDICATOR, BLINK );
			}


			// ESC
			MIR_write_dot( LED_RETURN, MIR_GREEN );


			// RECORD is available to all track selections
			// Show the recording status for the track
			// RECORDING LED
			if ( Page_getTrackRecPattern(target_page) == target_page->trackSelection ){
				MIR_write_dot( LED_RECORD, MIR_RED 	);
			}
			else if ( Page_getTrackRecPattern(target_page) ) {
				MIR_write_dot( LED_RECORD, MIR_GREEN );
			}
			if ( G_track_rec_bit == ON ) {
				if ( NOTE_queue_counter == 0 ) {
					MIR_write_dot( LED_RECORD, MIR_BLINK );
				}
			}

			// If tracks are in record select, allow the Stop button to un-arm all tracks.
			MIR_write_dot( LED_STOP, MIR_GREEN );


			// Show the PAGE REC bit while track is selected - BerndK mode transparent..
			if ( target_page->REC_bit == ON ){
				MIR_write_dot( LED_RECORD, MIR_GREEN 	);
				MIR_write_dot( LED_RECORD, MIR_RED 		);
				MIR_write_dot( LED_RECORD, MIR_BLINK 	);
			}
			#ifdef FEATURE_ENABLE_KEYBOARD_TRANSPOSE
			// x2 - Toggle transpose abs pitch mode
			if (	( target_page->pitch_abs == TRUE )	){

				MIR_write_dot( LED_SCALE_SEL, MIR_GREEN 	);
				MIR_write_dot( LED_SCALE_SEL, MIR_BLINK 	);

			} else {

				MIR_write_dot( LED_SCALE_SEL, MIR_RED 	);
				MIR_write_dot( LED_SCALE_SEL, MIR_BLINK 	);

			}
			#endif

			#ifdef FEATURE_ZOOMSTEP_PLUS
			// If a single track selected is a hyper-track then show the associated hyperstep in the matrix
			if (  ( my_bit_cardinality( target_page->trackSelection ) == 1 )
				 && (G_zoom_level != zoomTRACK)  )  {
				// Only if not in zoomTrack

					i = my_bit2ndx( target_page->trackSelection );

					if ( (target_page->Track[i]->hyper >> 4) != 0x0F ) {

						// Extract the coordinates of the responsible hyperstep
						row = target_page->Track[i]->hyper >> 4;
						col = target_page->Track[i]->hyper & 0x0F;

						// Check if H-S is in the same window as the hyped-track
						if ( row_in_track_window( target_page, row )  )   {
							// Show Hyperstep in matrix shine_red
							MIR_write_dot( Page_dotIndex( row, col ),  MIR_SHINE_RED );
						}
					}
			}
			#endif
