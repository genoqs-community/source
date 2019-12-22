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





// Show the scale selection of the given page
void show_OCTAVE_CIRCLE_scale_selection( Pagestruct* target_page ){

	unsigned int 	i = 0,
					j = 0;

	unsigned char blink = (SOLO_rec_transpose == ON && SOLO_transpose_latch == ON);


	#ifdef FEATURE_SOLO_REC
	if ( SOLO_rec_transpose == OFF ){
	#endif
		
		// Show the SCL_align status - not relevant in GRID mode
		if ( target_page != GRID_assistant_page ){

			if ( target_page->SCL_align == OFF ){

				#ifdef FEATURE_SOLO_REC
				if ( G_zoom_level != zoomSOLOREC ){
					MIR_write_dot( LED_SCALE_CAD, MIR_GREEN );
				}
				#else
				MIR_write_dot( LED_SCALE_CAD, MIR_GREEN );
				#endif
			}
			else{

				MIR_write_dot( LED_SCALE_CAD, MIR_RED );
				MIR_write_dot( LED_SCALE_CAD, MIR_BLINK );
			}
		}

	#ifdef FEATURE_SOLO_REC
	}
	else {

		target_page = &Page_repository[ GRID_CURSOR ];
	}

	// show the transpose tracks if there are per-track assignments
	if ( G_run_bit == ON && ( G_zoom_level == zoomSOLOREC || G_zoom_level == zoomPAGE )){

		Pagestruct* temp_page = &Page_repository[ GRID_CURSOR ];
		short rec_pattern = Page_getTrackRecPattern(temp_page);
		int row = my_bit2ndx(rec_pattern);

		if ( has_track_scale(temp_page->scaleNotes, row) != FALSE ){
			target_page = &Page_repository[ GRID_CURSOR ];
		}

		if ( rec_pattern == OFF ) {
			SOLO_transpose_row = NOP;
		}
	}
	#endif

	j = track_scale_value(SOLO_transpose_row, target_page->scaleNotes);

	// ON fields showing first
	for (i=0; i<12; i++) {

		// If the note is selected as ON
		if ( j & (1 << (11-i)) ) {

			switch (i) {
				case 0:
					MIR_write_dot( LED_NOTE_C, 		MIR_GREEN );
					break;
				case 1:
					MIR_write_dot( LED_NOTE_Cis, 	MIR_GREEN );
					break;
				case 2:
					MIR_write_dot( LED_NOTE_D,	 	MIR_GREEN );
					break;
				case 3:
					MIR_write_dot( LED_NOTE_Dis, 	MIR_GREEN );
					break;
				case 4:
					MIR_write_dot( LED_NOTE_E,	 	MIR_GREEN );
					break;
				case 5:
					MIR_write_dot( LED_NOTE_F,	 	MIR_GREEN );
					break;
				case 6:
					MIR_write_dot( LED_NOTE_Fis, 	MIR_GREEN );
					break;
				case 7:
					MIR_write_dot( LED_NOTE_G,	 	MIR_GREEN );
					break;
				case 8:
					MIR_write_dot( LED_NOTE_Gis, 	MIR_GREEN );
					break;
				case 9:
					MIR_write_dot( LED_NOTE_A, 		MIR_GREEN );
					break;
				case 10:
					MIR_write_dot( LED_NOTE_Ais, 	MIR_GREEN );
					break;
				case 11:
					MIR_write_dot( LED_NOTE_B,	 	MIR_GREEN );
					break;
			}
		}
	} // ON field iterator

	// UP fields showing first. This shows the scale lead
	for (i=0; i<12; i++) {
		
		// If the note is selected as UP
		if ( track_scale_value(SOLO_transpose_row, target_page->scaleLead) & (1 << (11-i) ) ) {
			
			switch (i) {
				case 0:
					MIR_write_dot( LED_NOTE_C, 		MIR_RED );
					if (blink) MIR_write_dot( LED_NOTE_C, 		MIR_BLINK );
					break;
				case 1:
					MIR_write_dot( LED_NOTE_Cis, 	MIR_RED );
					if (blink) MIR_write_dot( LED_NOTE_Cis, 	MIR_BLINK );
					break;
				case 2:
					MIR_write_dot( LED_NOTE_D,	 	MIR_RED );
					if (blink) MIR_write_dot( LED_NOTE_D, 		MIR_BLINK );
					break;
				case 3:
					MIR_write_dot( LED_NOTE_Dis, 	MIR_RED );
					if (blink) MIR_write_dot( LED_NOTE_Dis, 	MIR_BLINK );
					break;
				case 4:
					MIR_write_dot( LED_NOTE_E,	 	MIR_RED );
					if (blink) MIR_write_dot( LED_NOTE_E, 		MIR_BLINK );
					break;
				case 5:
					MIR_write_dot( LED_NOTE_F,	 	MIR_RED );
					if (blink) MIR_write_dot( LED_NOTE_F, 		MIR_BLINK );
					break;
				case 6:
					MIR_write_dot( LED_NOTE_Fis, 	MIR_RED );
					if (blink) MIR_write_dot( LED_NOTE_Fis, 	MIR_BLINK );
					break;
				case 7:
					MIR_write_dot( LED_NOTE_G,	 	MIR_RED );
					if (blink) MIR_write_dot( LED_NOTE_G, 		MIR_BLINK );
					break;
				case 8:
					MIR_write_dot( LED_NOTE_Gis, 	MIR_RED );
					if (blink) MIR_write_dot( LED_NOTE_Gis, 	MIR_BLINK );
					break;
				case 9:
					MIR_write_dot( LED_NOTE_A, 		MIR_RED );
					if (blink) MIR_write_dot( LED_NOTE_A, 		MIR_BLINK );
					break;
				case 10:
					MIR_write_dot( LED_NOTE_Ais, 	MIR_RED );
					if (blink) MIR_write_dot( LED_NOTE_Ais, 	MIR_BLINK );
					break;
				case 11:
					MIR_write_dot( LED_NOTE_B,	 	MIR_RED );
					if (blink) MIR_write_dot( LED_NOTE_B, 		MIR_BLINK );
					break;									
			}
		}
	} // UP field for iterator

}


// Show the chord tone selection of the given page
void show_OCTAVE_CIRCLE_chord_tone_selection( Pagestruct* target_page ){

	unsigned int 	i = 0, j = 0;
	unsigned char 	hasArp = OFF;

	if ( SOLO_scale_chords_b == ON ){
		MIR_write_dot( LED_NOTE_Cup, MIR_RED ); // Display A/B toggle - 2nd C
	}
	else {
		MIR_write_dot( LED_NOTE_Cup, MIR_GREEN );
	}

	if ( TEMPO_TIMER == ON || ( SOLO_scale_chords_program == ON && SOLO_scale_chords_palette_ndx != NOP )){


		if ( SOLO_scale_chords_program == ON && SOLO_scale_chords_palette_ndx != NOP ){ // show the Arp notes

			for (i=0; i<MATRIX_NROF_COLUMNS; i++){

				if ( Note_get_status( Chord_palette_repository[SOLO_scale_chords_palette_ndx].Arp[i], STEPSTAT_TOGGLE ) == ON ){
					hasArp = ON;
					break;
				}
			}

			if ( SOLO_assistant_page->Track[1]->attr_STATUS == ON ){ // blink if there is an arp undo

				MIR_write_dot( LED_SELECT_MASTER, MIR_RED );
				MIR_write_dot( LED_SELECT_MASTER, MIR_BLINK );
			}

			MIR_write_dot( ROT_MCH, MIR_RED );
			if ( !hasArp ){

				MIR_write_dot( 20, 		MIR_GREEN );
				MIR_write_dot( 20, 		MIR_RED );
				MIR_write_dot( 31, 		MIR_RED );

			}
			if ( hasArp ){

				MIR_write_dot( LED_SELECT_MASTER, MIR_RED );

				unsigned int track_togglepattern 	= 	Page_get_trackpattern( target_page, 0 ); // first track holds the visible Arp steps
				unsigned int track_eventpattern 	=	Page_get_event_trackpattern( target_page, 0 );
				unsigned int track_skippattern		=	Page_get_skippattern( target_page, 0 );

				// These are the active steps - exempting the skipped steps
				MIR_write_trackpattern  ( 	(	(  	track_togglepattern	| track_eventpattern )
												^	track_skippattern
											)
											&	(  	track_togglepattern	| track_eventpattern )
											,
										9,
										MIR_GREEN);

				// .. the ones containing chords..
				MIR_write_trackpattern( Page_get_chord_trackpattern( target_page, 0 ), 9, MIR_RED);

				MIR_augment_trackpattern( Page_get_skippattern( target_page, 0 ), 9, MIR_RED );

				if ( GRID_CURSOR == SOLO_assistant_page->pageNdx ){

					MIR_write_lauflicht_track( 9, 9 ); // show the Arp chase light
				}
			}
		}

		for (i=0; i<OCTAVE; i++) {

			if ( CHECK_BIT(SOLO_scale_chords_last, i) != OFF && ( G_run_bit == OFF || SOLO_scale_chords_program == ON )){ // this pitch has a chord note

				// 33=C, 44=C#, 55=D
				MIR_write_dot( 33 + (i * 11), 		MIR_GREEN );
				if (( !hasArp && is_pressed_key(20) ) || ( hasArp == ON && is_pressed_key(33 + (i * 11))) ){
					MIR_write_dot( 33 + (i * 11), 		MIR_BLINK );
				}
				else {

					for (j=0; j<MATRIX_NROF_COLUMNS; j++) {

						if ( is_pressed_key( 20 + (j * 11)) == TRUE &&
							 Note_get_status( Chord_palette_repository[SOLO_scale_chords_palette_ndx].Arp[j], STEPSTAT_TOGGLE ) == ON
						   ){ // an Arp step is pressed

							if (( Chord_palette_repository[SOLO_scale_chords_palette_ndx].Arp[j]->attr_PIT % OCTAVE ) == i ){

								unsigned int chord_value = get_chord_projected(SOLO_assistant_page->Step[0][j]);
								unsigned int k=0;

								for (k = 0; k < 32; k++) {
									if (CHECK_BIT(chord_value, k)) {
										MIR_write_dot(33 + ((i + k + 1) * 11), MIR_BLINK);
									}
								}

								MIR_write_dot( 33 + (i * 11), 		MIR_BLINK ); // flash the corresponding note at the top of the page
								MIR_write_dot( 20 + (j * 11), 		MIR_BLINK ); // flash the pressed step
							}
							break;
						}
					}
				}
			}

			// Display the chord modulations
			if ( CHECK_BIT(SOLO_scale_chords_modulations, i) == OFF || SOLO_scale_chords_program == ON ){

				continue;
			}
			switch (i) {
				case 0:
					MIR_write_dot( LED_NOTE_C, 		MIR_GREEN );
					break;
				case 1:
					MIR_write_dot( LED_NOTE_Cis, 	MIR_GREEN );
					break;
				case 2:
					MIR_write_dot( LED_NOTE_D,	 	MIR_GREEN );
					break;
				case 3:
					MIR_write_dot( LED_NOTE_Dis, 	MIR_GREEN );
					break;
				case 4:
					MIR_write_dot( LED_NOTE_E,	 	MIR_GREEN );
					break;
				case 5:
					MIR_write_dot( LED_NOTE_F,	 	MIR_GREEN );
					break;
				case 6:
					MIR_write_dot( LED_NOTE_Fis, 	MIR_GREEN );
					break;
				case 7:
					MIR_write_dot( LED_NOTE_G,	 	MIR_GREEN );
					break;
				case 8:
					MIR_write_dot( LED_NOTE_Gis, 	MIR_GREEN );
					break;
				case 9:
					MIR_write_dot( LED_NOTE_A, 		MIR_GREEN );
					break;
				case 10:
					MIR_write_dot( LED_NOTE_Ais, 	MIR_GREEN );
					break;
				case 11:
					MIR_write_dot( LED_NOTE_B,	 	MIR_GREEN );
					break;
			}
		}
	}

	if ( SOLO_rec_transpose == ON ){

		// Show the selected notes in scale. Both in MOD and SEL
		show_OCTAVE_CIRCLE_scale_selection( target_page );
	}
	else {

		// UP fields showing first. This shows the scale lead
		for (i=0; i<12; i++) {

			if ( SOLO_scale_chords_program == ON ){

				if ( Chord_palette_repository[i].chord_id != NOP ){

					switch (i) {
						case 0:
							MIR_write_dot( LED_NOTE_C, 		MIR_GREEN );
							break;
						case 1:
							MIR_write_dot( LED_NOTE_Cis, 	MIR_GREEN );
							break;
						case 2:
							MIR_write_dot( LED_NOTE_D,	 	MIR_GREEN );
							break;
						case 3:
							MIR_write_dot( LED_NOTE_Dis, 	MIR_GREEN );
							break;
						case 4:
							MIR_write_dot( LED_NOTE_E,	 	MIR_GREEN );
							break;
						case 5:
							MIR_write_dot( LED_NOTE_F,	 	MIR_GREEN );
							break;
						case 6:
							MIR_write_dot( LED_NOTE_Fis, 	MIR_GREEN );
							break;
						case 7:
							MIR_write_dot( LED_NOTE_G,	 	MIR_GREEN );
							break;
						case 8:
							MIR_write_dot( LED_NOTE_Gis, 	MIR_GREEN );
							break;
						case 9:
							MIR_write_dot( LED_NOTE_A, 		MIR_GREEN );
							break;
						case 10:
							MIR_write_dot( LED_NOTE_Ais, 	MIR_GREEN );
							break;
						case 11:
							MIR_write_dot( LED_NOTE_B,	 	MIR_GREEN );
							break;
					}

					switch (i) {
						case 0:
							MIR_write_dot( LED_NOTE_C, 		MIR_RED );
							break;
						case 1:
							MIR_write_dot( LED_NOTE_Cis, 	MIR_RED );
							break;
						case 2:
							MIR_write_dot( LED_NOTE_D,	 	MIR_RED );
							break;
						case 3:
							MIR_write_dot( LED_NOTE_Dis, 	MIR_RED );
							break;
						case 4:
							MIR_write_dot( LED_NOTE_E,	 	MIR_RED );
							break;
						case 5:
							MIR_write_dot( LED_NOTE_F,	 	MIR_RED );
							break;
						case 6:
							MIR_write_dot( LED_NOTE_Fis, 	MIR_RED );
							break;
						case 7:
							MIR_write_dot( LED_NOTE_G,	 	MIR_RED );
							break;
						case 8:
							MIR_write_dot( LED_NOTE_Gis, 	MIR_RED );
							break;
						case 9:
							MIR_write_dot( LED_NOTE_A, 		MIR_RED );
							break;
						case 10:
							MIR_write_dot( LED_NOTE_Ais, 	MIR_RED );
							break;
						case 11:
							MIR_write_dot( LED_NOTE_B,	 	MIR_RED );
							break;
					}

					if ( SOLO_scale_chords_palette_ndx == i ){

						switch (i) {
							case 0:
								MIR_write_dot( LED_NOTE_C, 		MIR_BLINK );
								break;
							case 1:
								MIR_write_dot( LED_NOTE_Cis, 	MIR_BLINK );
								break;
							case 2:
								MIR_write_dot( LED_NOTE_D,	 	MIR_BLINK );
								break;
							case 3:
								MIR_write_dot( LED_NOTE_Dis, 	MIR_BLINK );
								break;
							case 4:
								MIR_write_dot( LED_NOTE_E,	 	MIR_BLINK );
								break;
							case 5:
								MIR_write_dot( LED_NOTE_F,	 	MIR_BLINK );
								break;
							case 6:
								MIR_write_dot( LED_NOTE_Fis, 	MIR_BLINK );
								break;
							case 7:
								MIR_write_dot( LED_NOTE_G,	 	MIR_BLINK );
								break;
							case 8:
								MIR_write_dot( LED_NOTE_Gis, 	MIR_BLINK );
								break;
							case 9:
								MIR_write_dot( LED_NOTE_A, 		MIR_BLINK );
								break;
							case 10:
								MIR_write_dot( LED_NOTE_Ais, 	MIR_BLINK );
								break;
							case 11:
								MIR_write_dot( LED_NOTE_B,	 	MIR_BLINK );
								break;
						}
					}
				}
			}

			// If the note is selected as UP
			else if ( target_page-> scaleLead[G_scale_ndx] & (1 << (11-i) ) ) {

				switch (i) {
					case 0:
						MIR_write_dot( LED_NOTE_C, 		MIR_RED );
						break;
					case 1:
						MIR_write_dot( LED_NOTE_Cis, 	MIR_RED );
						break;
					case 2:
						MIR_write_dot( LED_NOTE_D,	 	MIR_RED );
						break;
					case 3:
						MIR_write_dot( LED_NOTE_Dis, 	MIR_RED );
						break;
					case 4:
						MIR_write_dot( LED_NOTE_E,	 	MIR_RED );
						break;
					case 5:
						MIR_write_dot( LED_NOTE_F,	 	MIR_RED );
						break;
					case 6:
						MIR_write_dot( LED_NOTE_Fis, 	MIR_RED );
						break;
					case 7:
						MIR_write_dot( LED_NOTE_G,	 	MIR_RED );
						break;
					case 8:
						MIR_write_dot( LED_NOTE_Gis, 	MIR_RED );
						break;
					case 9:
						MIR_write_dot( LED_NOTE_A, 		MIR_RED );
						break;
					case 10:
						MIR_write_dot( LED_NOTE_Ais, 	MIR_RED );
						break;
					case 11:
						MIR_write_dot( LED_NOTE_B,	 	MIR_RED );
						break;
				}
			}
		} // UP field for iterator

		if ( SOLO_scale_chords_program == OFF && TEMPO_TIMER == OFF ){ // show the pitch

			switch (target_page->attr_PIT % OCTAVE) {
				case 0:
					// don't show C because it is a null offset - the natural pitch
//					MIR_write_dot( LED_NOTE_C, 		MIR_GREEN );
					break;
				case 1:
					MIR_write_dot( LED_NOTE_Cis, 	MIR_GREEN );
					break;
				case 2:
					MIR_write_dot( LED_NOTE_D,	 	MIR_GREEN );
					break;
				case 3:
					MIR_write_dot( LED_NOTE_Dis, 	MIR_GREEN );
					break;
				case 4:
					MIR_write_dot( LED_NOTE_E,	 	MIR_GREEN );
					break;
				case 5:
					MIR_write_dot( LED_NOTE_F,	 	MIR_GREEN );
					break;
				case 6:
					MIR_write_dot( LED_NOTE_Fis, 	MIR_GREEN );
					break;
				case 7:
					MIR_write_dot( LED_NOTE_G,	 	MIR_GREEN );
					break;
				case 8:
					MIR_write_dot( LED_NOTE_Gis, 	MIR_GREEN );
					break;
				case 9:
					MIR_write_dot( LED_NOTE_A, 		MIR_GREEN );
					break;
				case 10:
					MIR_write_dot( LED_NOTE_Ais, 	MIR_GREEN );
					break;
				case 11:
					MIR_write_dot( LED_NOTE_B,	 	MIR_GREEN );
					break;
			}
		}
	}
}


// Show the chord tone selection of the given page
void show_OCTAVE_CIRCLE_chord_octave_transpose_selection( signed char octave ){

	unsigned char blink = OFF;

	if ( SOLO_rec_transpose == ON ){

		Pagestruct* temp_page = &Page_repository[ GRID_CURSOR ];
		short rec_pattern = Page_getTrackRecPattern(temp_page);
		int row = my_bit2ndx(rec_pattern);

		if ( rec_pattern != OFF ){

			if ( has_track_scale(temp_page->scaleNotes, row) != FALSE ){

				if ( temp_page->Track[row]->lead_pitch_offset < 0 ){

					octave = (signed char) ( temp_page->Track[row]->lead_pitch_offset - 11 ) / OCTAVE;
				}
				else {
					octave = (signed char) temp_page->Track[row]->lead_pitch_offset / OCTAVE;
				}

				if ( SOLO_transpose_latch == ON ){

					blink = ON;
				}
			}
		}
	}

	switch ( octave ){

		case -3:
			MIR_write_dot (LED_SCALE_CAD, MIR_GREEN);
			break;
		case -2:
			MIR_write_dot (LED_SCALE_CAD, MIR_GREEN);
			MIR_write_dot (LED_SCALE_MOD, MIR_RED);
			break;
		case -1:
			MIR_write_dot (LED_SCALE_CAD, MIR_GREEN);
			MIR_write_dot (LED_SCALE_MOD, MIR_GREEN);
			MIR_write_dot (LED_SCALE_MOD, MIR_RED);
			break;
		case 0:
			MIR_write_dot (LED_SCALE_CAD, MIR_GREEN);
			MIR_write_dot (LED_SCALE_MOD, MIR_GREEN);
			break;
		case 1:
			MIR_write_dot (LED_SCALE_CAD, MIR_GREEN);
			MIR_write_dot (LED_SCALE_CAD, MIR_RED);
			MIR_write_dot (LED_SCALE_MOD, MIR_GREEN);
			break;
		case 2:
			MIR_write_dot (LED_SCALE_CAD, MIR_RED);
			MIR_write_dot (LED_SCALE_MOD, MIR_GREEN);
			break;
		case 3:
			MIR_write_dot (LED_SCALE_MOD, MIR_GREEN);
			break;
	}

	if (blink) MIR_write_dot (LED_SCALE_CAD, MIR_BLINK);
	if (blink) MIR_write_dot (LED_SCALE_MOD, MIR_BLINK);
}


