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



extern unsigned char row_of_track( 				Pagestruct* target_page,
												Trackstruct* target_track );
extern unsigned int is_pressed_steprange();
extern void show_preview_stepLEN( unsigned int index, unsigned char row, unsigned char col, unsigned char length );
extern void	page_cluster_selection( unsigned char grid_cursor );
//
// SHOW - Contains the workhorse code for MIR refill. Puts the content into the target display element (ELE)
//
void show (unsigned int target, unsigned int content) {

	unsigned int i=0, j=0;
	signed char my_pitch = 0;
	unsigned int row=0;
	unsigned int col=0;
	unsigned int attribute=0;
	unsigned int bitpattern=0;
	unsigned char temp = 0;
	unsigned int my_pressed_step = 0;

	short track_togglepattern = 0;
	short track_eventpattern = 0;
	short track_skippattern = 0;


	Pagestruct* temp_page = NULL;
	Trackstruct* current_track = NULL;

	Pagestruct* target_page = &Page_repository[GRID_CURSOR];

	switch (target) {

		//
		// ZOOM LEVEL (a.k.a. Inner Circle)
		//
		case ELE_ZOOMLEVEL:
			#include "show_ZOOM_LEVEL.h"
			break;


		//
		// MIX MASTER
		//
		case ELE_MIX_MASTER:
			switch (content) {
				case GREEN:
					MIR_write_dot (LED_MIX_MASTER, MIR_GREEN);
					break;
				case RED:
					MIR_write_dot (LED_MIX_MASTER, MIR_RED);
					break;
				case BLINK:
					MIR_write_dot (LED_MIX_MASTER, MIR_BLINK);
					break;
				case OFF:
					break;
			}
			break;


		//
		// MIX INDICATOR (LED for the MIX rotarys)
		//
		case ELE_MIX_INDICATOR:
			switch (content) {
				case GREEN:
					MIR_write_dot (LED_MIX_INDICATOR, MIR_GREEN);
					break;
				case RED:
					MIR_write_dot (LED_MIX_INDICATOR, MIR_RED);
					break;
				case BLINK:
					MIR_write_dot (LED_MIX_INDICATOR, MIR_BLINK);
					break;
				case OFF:
					break;
			}
			break;

		//
		// MIX TARGET INDICATOR
		//
		case ELE_MIXTGT:

			// Depending on the zoom mode:
			switch( G_zoom_level ){

				case zoomPAGE:
				case zoomMIXMAP:

					i = target_page->mixTarget;

					if ( content == OPTIONS ) {

						// Show what's available
						MIR_write_dot( LED_MIXTGT_ATR, 	MIR_GREEN );
						MIR_write_dot( LED_MIXTGT_VOL, 	MIR_GREEN );
						MIR_write_dot( LED_MIXTGT_PAN, 	MIR_GREEN );
						MIR_write_dot( LED_MIXTGT_MOD,	MIR_GREEN );
						MIR_write_dot( LED_MIXTGT_EXP, 	MIR_GREEN );

						MIR_write_dot( LED_MIXTGT_USR0, MIR_GREEN );
						MIR_write_dot( LED_MIXTGT_USR1, MIR_GREEN );
						MIR_write_dot( LED_MIXTGT_USR2, MIR_GREEN );
						MIR_write_dot( LED_MIXTGT_USR3, MIR_GREEN );
						MIR_write_dot( LED_MIXTGT_USR4, MIR_GREEN );
						MIR_write_dot( LED_MIXTGT_USR5, MIR_GREEN );
					}
					break;

				case zoomGRIDTRACK:
				case zoomGRID:
					i = GRID_assistant_page->mixTarget;

					#ifdef FEATURE_SOLO_REC
					if ( GRID_CC_events == ON ) {

						if ( SOLO_assistant_page->Track[4 + MIXTGT_USR1]->attr_STATUS == ON ) MIR_write_dot( LED_MIXTGT_USR1, MIR_GREEN );
						if ( SOLO_assistant_page->Track[4 + MIXTGT_USR2]->attr_STATUS == ON ) MIR_write_dot( LED_MIXTGT_USR2, MIR_GREEN );
						if ( SOLO_assistant_page->Track[4 + MIXTGT_USR3]->attr_STATUS == ON ) MIR_write_dot( LED_MIXTGT_USR3, MIR_GREEN );
						if ( SOLO_assistant_page->Track[4 + MIXTGT_USR4]->attr_STATUS == ON ) MIR_write_dot( LED_MIXTGT_USR4, MIR_GREEN );
						if ( SOLO_assistant_page->Track[4 + MIXTGT_USR5]->attr_STATUS == ON ) MIR_write_dot( LED_MIXTGT_USR5, MIR_GREEN );

						if ( SOLO_assistant_page->Track[4 + MIXTGT_USR1]->attr_VEL == ON ) MIR_write_dot( LED_MIXTGT_USR1, MIR_RED );
						if ( SOLO_assistant_page->Track[4 + MIXTGT_USR2]->attr_VEL == ON ) MIR_write_dot( LED_MIXTGT_USR2, MIR_RED );
						if ( SOLO_assistant_page->Track[4 + MIXTGT_USR3]->attr_VEL == ON ) MIR_write_dot( LED_MIXTGT_USR3, MIR_RED );
						if ( SOLO_assistant_page->Track[4 + MIXTGT_USR4]->attr_VEL == ON ) MIR_write_dot( LED_MIXTGT_USR4, MIR_RED );
						if ( SOLO_assistant_page->Track[4 + MIXTGT_USR5]->attr_VEL == ON ) MIR_write_dot( LED_MIXTGT_USR5, MIR_RED );

						if ( EDIT_TIMER == ON || OTM_CC_type != OFF ){

							if ( SOLO_assistant_page->Track[4 + MIXTGT_USR1]->attr_STATUS == ON ) MIR_write_dot( LED_MIXTGT_USR1, MIR_BLINK );
							if ( SOLO_assistant_page->Track[4 + MIXTGT_USR2]->attr_STATUS == ON ) MIR_write_dot( LED_MIXTGT_USR2, MIR_BLINK );
							if ( SOLO_assistant_page->Track[4 + MIXTGT_USR3]->attr_STATUS == ON ) MIR_write_dot( LED_MIXTGT_USR3, MIR_BLINK );
							if ( SOLO_assistant_page->Track[4 + MIXTGT_USR4]->attr_STATUS == ON ) MIR_write_dot( LED_MIXTGT_USR4, MIR_BLINK );
							if ( SOLO_assistant_page->Track[4 + MIXTGT_USR5]->attr_STATUS == ON ) MIR_write_dot( LED_MIXTGT_USR5, MIR_BLINK );
						}

						MIR_write_dot( LED_CLEAR, MIR_GREEN );
						MIR_write_dot( LED_CLEAR, MIR_RED );
						if ( is_pressed_key( KEY_CLEAR ) ){

							MIR_write_dot( LED_CLEAR, MIR_BLINK );
						}

						break;
					}
					#endif

					if ( content == OPTIONS ) {

						// Show what's available
						MIR_write_dot( LED_MIXTGT_USR0, MIR_GREEN );
						MIR_write_dot( LED_MIXTGT_USR1, MIR_GREEN );
						MIR_write_dot( LED_MIXTGT_USR2, MIR_GREEN );
						MIR_write_dot( LED_MIXTGT_USR3, MIR_GREEN );
						MIR_write_dot( LED_MIXTGT_USR4, MIR_GREEN );
						MIR_write_dot( LED_MIXTGT_USR5, MIR_GREEN );
					}
					break;
			}

			#ifdef FEATURE_SOLO_REC
			if ( GRID_CC_events == ON ) break;
			#endif

			switch( i ) {

				case MIXTGT_ATR:
					j = LED_MIXTGT_ATR;
					break;
				case MIXTGT_VOL:
					j = LED_MIXTGT_VOL;
					break;
				case MIXTGT_PAN:
					j = LED_MIXTGT_PAN;
					break;
				case MIXTGT_MOD:
					j = LED_MIXTGT_MOD;
					break;
				case MIXTGT_EXP:
					j = LED_MIXTGT_EXP;
					break;

				case MIXTGT_USR0:
					j = LED_MIXTGT_USR0;
					break;
				case MIXTGT_USR1:
					j = LED_MIXTGT_USR1;
					break;
				case MIXTGT_USR2:
					j = LED_MIXTGT_USR2;
					break;
				case MIXTGT_USR3:
					j = LED_MIXTGT_USR3;
					break;
				case MIXTGT_USR4:
					j = LED_MIXTGT_USR4;
					break;
				case MIXTGT_USR5:
					j = LED_MIXTGT_USR5;
					break;
			}

			// Show selection
			MIR_write_dot (j, MIR_RED);
			MIR_write_dot (j, MIR_GREEN);

			// Blink to show current selection
			if ( 	( G_zoom_level == zoomMIXMAP )
				||	( 	( ( G_zoom_level == zoomGRID ) || ( G_zoom_level == zoomGRIDTRACK ) )
					&&	( content == OPTIONS ) )
				){

				MIR_write_dot (j, MIR_BLINK);
			}
			break;


		//
		// EDIT INDICATOR (LED for the EDIT rotarys)
		//
		case ELE_EDIT_INDICATOR:
			switch (content) {
				case GREEN:
					MIR_write_dot (LED_EDIT_INDICATOR, MIR_GREEN);
					break;
				case RED:
					MIR_write_dot (LED_EDIT_INDICATOR, MIR_RED);
					break;
				case BLINK:
					MIR_write_dot (LED_EDIT_INDICATOR, MIR_BLINK);
					break;
				case OFF:
					break;
			}
			break;


		//
		// TRACK SELECTORS
		//
		case ELE_TRACK_SELECTORS:
			#include "show_TRACK_SELECTORS.h"
			break;

		//
		// SELECT MASTER
		//
		case ELE_SELECT_MASTER:
			switch (content) {
				case GREEN:
					MIR_write_dot (LED_SELECT_MASTER, MIR_GREEN);
					break;
				case RED:
					MIR_write_dot (LED_SELECT_MASTER, MIR_RED);
					break;
				case BLINK:
					MIR_write_dot (LED_SELECT_MASTER, MIR_BLINK);
					break;
			}
			break;

		//
		// G_master_tempo MULTIPLIER
		//
		case ELE_G_master_tempo_MULTIPLIER:
			break;

		//
		// CHAIN INDICATOR
		//
		case ELE_CHAIN_INDICATOR:

			switch( content ){

				case ON:
					switch (target_page->chainMode) {
						case 0:
							j = LED_CHAINMODE_1;
							break;
						case 1:
							j = LED_CHAINMODE_2;
							break;
						case 2:
							j = LED_CHAINMODE_3;
							break;
						case 3:
							j = LED_CHAINMODE_4;
							break;
						case 4:
							j = LED_CHAINER;
							break;
					}
					break;

			}

			// Light the actual LED
			MIR_write_dot (j, MIR_RED);

			if ( target_page->CHAINS_PLAY_HEAD == FALSE ){

				// If tracks play on their own, light orange
				MIR_write_dot (j, MIR_GREEN);
			}

			break;



		// M A T R I X
		//
		case ELE_MATRIX:
			#include "show_MATRIX.h"
			break;


		// TRACK MUTATORS
		//
		case ELE_TRACK_MUTATORS:
			#include "show_TRACK_MUTATORS.h"
			break; // ELE_TRACK_MUTATORS


		case ELE_MUTE_MASTER:
			switch (content) {
				case GREEN:
					MIR_write_dot (LED_MUTE_MASTER, MIR_GREEN);
					break;
				case RED:
					MIR_write_dot (LED_MUTE_MASTER, MIR_RED);
					break;
				case BLINK:
					MIR_write_dot (LED_MUTE_MASTER, MIR_BLINK);
					break;
			}
			break;


		case ELE_EDIT_MASTER:
			switch (content) {
				case OFF:
					break;
				case RED:
					MIR_write_dot (LED_EDIT_MASTER, MIR_RED);
					break;
				case GREEN:
					MIR_write_dot (LED_EDIT_MASTER, MIR_GREEN);
					break;
				case BLINK:
					MIR_write_dot (LED_EDIT_MASTER, MIR_BLINK);
					break;
			}
			break;


		case ELE_RETURN:
			switch (content) {
				case OFF:
					break;
				case GREEN:
					MIR_write_dot (LED_RETURN, MIR_GREEN);
					break;
				case RED:
					MIR_write_dot (LED_RETURN, MIR_RED);
					break;
				case BLINK:
					MIR_write_dot (LED_RETURN, MIR_BLINK);
					break;
			}
			break;


		case ELE_GLOBALS:
			#include "show_ELE_GLOBALS.h"
			break;


		case ELE_OCTAVE_CIRCLE:
			#include "show_OCTAVE_CIRCLE.h"
			break;


		// Show the type of scale selected
		case ELE_SCALE_SELECTOR:

			switch( content ){

			}
			break; // ELE_SCALE_SELECTOR




		// Show the type of chord currently selected
		case ELE_CHORD_SELECTOR:

			switch( content ){


				case CHORD_SIZE:
					// This is a bit deep but efficient. See LED index map.
					// MIR_write_dot( 258 - target_page->chordSize + 1, MIR_RED   );
					// MIR_write_dot( 258 - target_page->chordSize + 1, MIR_GREEN );

					break;


				case CHORD_SIZE_TRACK:
					// Get the value of the chord bit for the selected track
					i = Track_get_MISC( target_page->Track[ my_bit2ndx( target_page->trackSelection ) ],
						CHORD_BIT );
					MIR_write_dot( 258 - i + 1, MIR_RED   );
					MIR_write_dot( 258 - i + 1, MIR_GREEN );

					break;

			}
			break;

		#ifdef FEATURE_NOTE_DRUM_CTRL
		case ELE_DRUM_CTRL:
		if ( 	( G_zoom_level == zoomGRID )
			&&	( CHECK_BIT( GRID_p_set_mode, GRID_SET_NOTE_CTRL_ENABLE ) ) ) {

			// Show the current grid set track if a note is set
			MIR_write_dot( 10, MIR_RED );
			MIR_write_dot( 10, MIR_BLINK );
			if ( 	( is_pressed_key( 10 ) )
				||	( GRID_p_set_note_offsets[current_GRID_set] != 255 ) ) {
				MIR_write_dot( 10, MIR_GREEN );
			}

			if ( is_pressed_key( 10 ) ) {

				if ( GRID_p_set_midi_ch <= 16 ) {
					MIR_point_numeric(
						GRID_p_set_midi_ch,
						9,	MIR_GREEN);
				} else if ( GRID_p_set_midi_ch <= 32 ) {

					MIR_point_numeric(
						GRID_p_set_midi_ch - 16,
						9,	MIR_RED);
				}

				if ( GRID_p_set_note_offsets[current_GRID_set] != 255 ) {
					show_pitch_in_circle( GRID_p_set_note_offsets[current_GRID_set], ON );
				}
			}
		}
		break;
		#endif
	} // switch (target)
}


