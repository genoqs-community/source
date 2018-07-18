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





	switch (content) {
		
		// Show the pitch of the selected track, only when one selected
		case TRACK_TRANSPOSITION:
		
			if ( G_zoom_level == zoomGRIDTRACK ){

				for ( i=0; i < GRID_NROF_BANKS; i++ ){
			
					// A page is active in the bank
					if ( GRID_p_selection[i] != NULL ){
					
						if (	( GRID_p_selection[i]->trackSelection != 0 )
							&&	( my_bit_cardinality( GRID_p_selection[i]->trackSelection ) == 1 )
							){
			
							// Get the pitch of the selected track
							my_pitch = 
								GRID_p_selection[i]->Track[ my_bit2ndx( GRID_p_selection[i]->trackSelection ) ]->attr_PIT;
							
							// Display it in the inner circle
							show_pitch_in_circle( my_pitch, ON );
		
							// Terminate the loop
							i = GRID_NROF_BANKS;
						}
					} 
				} // Bank iterator	
			} // GRIDTRACK mode is active

			else{

				// Depending on how many tracks are selected in the page
				switch( my_bit_cardinality( target_page->trackSelection ) ){
	
					// Only one track is selected
					case 1:
						// Get the pitch of the selected track
						my_pitch = 
							target_page->Track[ my_bit2ndx( target_page->trackSelection ) ]->attr_PIT;
						
						// Display it in the inner circle
						show_pitch_in_circle( my_pitch, ON );
						break;
	
					// More than one track is selected
					default:
						// For now nothing to do
						break;
				} // switch( cardinality of trackselection )
			}

			break;

		
			
		case PAGE_TRANSPOSITION:
		
			// Only show it if there is no track selection in page
			if ( target_page->trackSelection != 0 ) {
				// Will have to evaluate what makes most sense here
				// break;
			}
		
			// This is the octave-wise trans-position : RED
			// The middle octave (case 5) is always lit
			MIR_write_dot( LED_NOTE_Fis,	MIR_RED		);					

			switch ( Page_repository[GRID_CURSOR].attr_PIT / 12 ){
				case 0:
					MIR_write_dot( LED_NOTE_Cis,	MIR_RED		);

				case 1:
					MIR_write_dot( LED_NOTE_D,		MIR_RED		);

				case 2:
					MIR_write_dot( LED_NOTE_Dis,	MIR_RED		);

				case 3:
					MIR_write_dot( LED_NOTE_E,		MIR_RED		);

				case 4:
					MIR_write_dot( LED_NOTE_F,		MIR_RED		);
					break;						

				// Scale up: fallthrough intended for Balken
				case 10:
					// This LED shlould never light red
					// MIR_write_dot( LED_NOTE_B,		MIR_RED		);

				case 9:
					MIR_write_dot( LED_NOTE_Ais,	MIR_RED		);

				case 8:
					MIR_write_dot( LED_NOTE_A,		MIR_RED		);

				case 7:
					MIR_write_dot( LED_NOTE_Gis,	MIR_RED		);			

				case 6:
					MIR_write_dot( LED_NOTE_G,		MIR_RED		);
					break;	
			}

			// This is the note-wise trans-position: GREEN
			if ( Page_repository[GRID_CURSOR].attr_PIT == 127) {
				// Light Cup only when the transpose limit is reached
				MIR_write_dot( LED_NOTE_Cup, 	MIR_GREEN	);
			}
			switch ( Page_repository[GRID_CURSOR].attr_PIT % 12 ) {
				case 0:
					// Consider the "headroom reached case"
					if ( Page_repository[GRID_CURSOR].attr_PIT < 120 ){
						MIR_write_dot( LED_NOTE_C, 		MIR_GREEN 	);
					}
					else {
						MIR_write_dot( LED_NOTE_Cup, 	MIR_GREEN	);
					}
					break;
				case 1:
					MIR_write_dot( LED_NOTE_Cis, 	MIR_GREEN	);
					break;
				case 2:
					MIR_write_dot( LED_NOTE_D, 		MIR_GREEN	);
					break;
				case 3:
					MIR_write_dot( LED_NOTE_Dis, 	MIR_GREEN	);
					break;
				case 4:
					MIR_write_dot( LED_NOTE_E, 		MIR_GREEN	);
					break;
				case 5:
					MIR_write_dot( LED_NOTE_F, 		MIR_GREEN	);
					break;
				case 6:
					MIR_write_dot( LED_NOTE_Fis, 	MIR_GREEN	);
					break;
				case 7:
					MIR_write_dot( LED_NOTE_G, 		MIR_GREEN	);
					break;
				case 8:
					MIR_write_dot( LED_NOTE_Gis,	MIR_GREEN	);
					break;
				case 9:
					MIR_write_dot( LED_NOTE_A, 		MIR_GREEN	);
					break;
				case 10:
					MIR_write_dot( LED_NOTE_Ais, 	MIR_GREEN	);
					break;
				case 11:
					MIR_write_dot( LED_NOTE_B, 		MIR_GREEN	);
					break;
			}
			break; // case PAGE_TRANSPOSITION:
			
		
		
		// Show the picture of the global locator, color to be determined
		case G_global_locator_PICTURE:

			// Determine the color to be used: DIRECT->GREEN // OCLOCK->RED
			j = MIR_RED;

			if ( GRID_switch_mode == GRID_SWITCH_DIRECT ){

				// This is the DIRECT mode
				j = MIR_GREEN;
			}

			#ifdef FEATURE_SOLO_REC

			if ( G_zoom_level == zoomSOLOREC ){
				// This is the rotating picture..
				switch ( G_global_locator ) {

					// Fallthrough overall intended - to get the packman effect
					case 16:		MIR_write_dot( LED_SCALE_CAD, 		j );
					case 15:		MIR_write_dot( LED_NOTE_Cup, 		j );
					case 14:		MIR_write_dot( LED_NOTE_B, 			j );
					case 13:		MIR_write_dot( LED_NOTE_Ais, 		j );
					case 12:		MIR_write_dot( LED_NOTE_A, 			j );
					case 11:		MIR_write_dot( LED_NOTE_Gis, 		j );
					case 10:		MIR_write_dot( LED_NOTE_G,			j );
					case 9:			MIR_write_dot( LED_NOTE_Fis,		j );
					case 8:			MIR_write_dot( LED_NOTE_F,			j );
					case 7:			MIR_write_dot( LED_NOTE_E, 			j );
					case 6:			MIR_write_dot( LED_NOTE_Dis, 		j );
					case 5:			MIR_write_dot( LED_NOTE_D, 			j );
					case 4:			MIR_write_dot( LED_NOTE_Cis, 		j );
					case 3:			MIR_write_dot( LED_NOTE_C, 			j );
					case 2:			MIR_write_dot( LED_SCALE_MOD, 		j );
					case 1:			MIR_write_dot( LED_SCALE_SEL, 		j );
					case 0:			// Show nothing
						break;

				} // switch (G_global_locator)
			}
			else {

				// This is the rotating picture..
				switch ( G_global_locator ) {

					// Fallthrough overall intended - to get the packman effect
					case 16:		MIR_write_dot( LED_NOTE_F, 		j );
					case 15:		MIR_write_dot( LED_NOTE_E, 		j );
					case 14:		MIR_write_dot( LED_NOTE_Dis, 	j );
					case 13:		MIR_write_dot( LED_NOTE_D, 		j );
					case 12:		MIR_write_dot( LED_NOTE_Cis, 	j );
					case 11:		MIR_write_dot( LED_NOTE_C, 		j );
					case 10:		MIR_write_dot( LED_SCALE_MOD,	j );
					case 9:			MIR_write_dot( LED_SCALE_SEL,	j );
					case 8:			MIR_write_dot( LED_SCALE_CAD,	j );
					case 7:			MIR_write_dot( LED_NOTE_Cup, 	j );
					case 6:			MIR_write_dot( LED_NOTE_B, 		j );
					case 5:			MIR_write_dot( LED_NOTE_Ais, 	j );
					case 4:			MIR_write_dot( LED_NOTE_A, 		j );
					case 3:			MIR_write_dot( LED_NOTE_Gis, 	j );
					case 2:			MIR_write_dot( LED_NOTE_G, 		j );
					case 1:			MIR_write_dot( LED_NOTE_Fis, 	j );
					case 0:			// Show nothing
						break;

				} // switch (G_global_locator)
			}

			// Solo Recording - Show pink when measure hold is released by first key press
			if ( G_zoom_level == zoomSOLOREC && SOLO_rec_measure_hold == OFF && G_track_rec_bit == ON ){

				switch ( G_global_locator ) {

					case 16:		MIR_write_dot( LED_SCALE_CAD, 		MIR_GREEN );
					case 15:		MIR_write_dot( LED_NOTE_Cup, 		MIR_GREEN );
					case 14:		MIR_write_dot( LED_NOTE_B, 			MIR_GREEN );
					case 13:		MIR_write_dot( LED_NOTE_Ais, 		MIR_GREEN );
					case 12:		MIR_write_dot( LED_NOTE_A, 			MIR_GREEN );
					case 11:		MIR_write_dot( LED_NOTE_Gis, 		MIR_GREEN );
					case 10:		MIR_write_dot( LED_NOTE_G,			MIR_GREEN );
					case 9:			MIR_write_dot( LED_NOTE_Fis,		MIR_GREEN );
					case 8:			MIR_write_dot( LED_NOTE_F,			MIR_GREEN );
					case 7:			MIR_write_dot( LED_NOTE_E, 			MIR_GREEN );
					case 6:			MIR_write_dot( LED_NOTE_Dis, 		MIR_GREEN );
					case 5:			MIR_write_dot( LED_NOTE_D, 			MIR_GREEN );
					case 4:			MIR_write_dot( LED_NOTE_Cis, 		MIR_GREEN );
					case 3:			MIR_write_dot( LED_NOTE_C, 			MIR_GREEN );
					case 2:			MIR_write_dot( LED_SCALE_MOD, 		MIR_GREEN );
					case 1:			MIR_write_dot( LED_SCALE_SEL, 		MIR_GREEN );
					case 0:			// Show nothing
						break;
				}
			}
			#else

			// This is the rotating picture..
			switch ( G_global_locator ) {

				// Fallthrough overall intended - to get the packman effect
				case 16:		MIR_write_dot( LED_NOTE_F, 		j );
				case 15:		MIR_write_dot( LED_NOTE_E, 		j );
				case 14:		MIR_write_dot( LED_NOTE_Dis, 	j );
				case 13:		MIR_write_dot( LED_NOTE_D, 		j );
				case 12:		MIR_write_dot( LED_NOTE_Cis, 	j );
				case 11:		MIR_write_dot( LED_NOTE_C, 		j );
				case 10:		MIR_write_dot( LED_SCALE_MOD,	j );
				case 9:			MIR_write_dot( LED_SCALE_SEL,	j );
				case 8:			MIR_write_dot( LED_SCALE_CAD,	j );
				case 7:			MIR_write_dot( LED_NOTE_Cup, 	j );
				case 6:			MIR_write_dot( LED_NOTE_B, 		j );
				case 5:			MIR_write_dot( LED_NOTE_Ais, 	j );
				case 4:			MIR_write_dot( LED_NOTE_A, 		j );
				case 3:			MIR_write_dot( LED_NOTE_Gis, 	j );
				case 2:			MIR_write_dot( LED_NOTE_G, 		j );
				case 1:			MIR_write_dot( LED_NOTE_Fis, 	j );
				case 0:			// Show nothing
					break;

			} // switch (G_global_locator)

			#endif

			break; 
			

	} // case ELE_OCTAVE_CIRCLE / switch( content )			
