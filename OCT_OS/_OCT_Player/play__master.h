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



         /* OCTOPUS
 * Implementation of the player functions
 * 07.11.2003 genoQs Machines
 *
 * The first beat is slightly shorter because of the time needed
 * to operate the timer interrupt in player_start().
 */

#ifdef FEATURE_ENABLE_SONG_UPE
extern void stop_playing_page( 						Pagestruct* target_page,
													unsigned char in_G_TTC_abs_value,
													unsigned char pause);
#else
extern void stop_playing_page( 						Pagestruct* target_page,
													unsigned char in_G_TTC_abs_value );
#endif

extern void set_track_locators( 					Pagestruct* target_page,
													Trackstruct* in_track,
													unsigned char target_LOC,
													unsigned char target_TTC	 );

extern unsigned char offset_to_next_degree(			Pagestruct* target_page,
													unsigned char pitch );

extern unsigned char Step_active_playcoordinate(	Pagestruct* target_page,
													unsigned char row,
													unsigned char col,
													unsigned char which );

extern unsigned char get_next_tracklocator( 		Trackstruct* target_track,
													unsigned char curr_locator );

extern unsigned char jump_skipped_steps( 			Pagestruct* target_page,
													unsigned char row,
													unsigned char direction,
													unsigned char offset	);

extern void set_page_locators( 						Pagestruct* target_page,
													unsigned char target_LOC,
													unsigned char target_TTC );


// General Player functions
#include "play_functions.h"

// MIDI related functions, includes the MIDI out thread
#include "play_MIDI.h"

// The main play function
#include "play_play.h"

// Functions for locator update
#include "play_updlocators.h"

// Functions for morphing
// # include "play_morph.h"


// Advances the global locator and execute associated functions
void advance_global_locator(){

	// Advance global locator. This is hardwired to 16 (columns)
	G_global_locator = (G_global_locator % MATRIX_NROF_COLUMNS) +1;
}


// Used to advance the locator in the target_page and in its tracks as well
// according to G_TTC_abs_value and other conditions.See logic on page 190 book.
// This used to be G_master_tempo_TRACKER driven.
void advance_page_locators( Pagestruct* target_page ){

	switch( G_TTC_abs_value ) {

		// Affected pages: multipliers: 1  2  4 (all pages)
		case 1:
			// Advance page locator: modulo operation acc. to page length

			target_page->locator = ( target_page->locator % target_page->attr_LEN ) + 1;

			break;


		// Nothing to do otherwise.
		default:
			break;

	} // switch (G_master_tempo_TRACKER
}



//____________________________________________________________________________________
// Play the MIDI events queued up for the given timestamp
void play_MIDI_queue( unsigned int in_G_MIDI_timestamp ){
	#ifdef FEATURE_ENABLE_SONG_UPE
	if (G_align_bit)
	{
		return;
	}
	#endif
	unsigned int i = 0;
	unsigned char queue_size = 0;	// Stores initial queue size
	MIDIeventstruct* queue_event = NULL;

	// MIDI OFF - Play currently relevant note OFF data from the MIDI queue
	queue_size = MIDI_queue_counter;

	// Bound on current queue size
	for( i=0; i < queue_size; i++ ){

		// Get the next event at current timestamp
		queue_event = MIDI_queue_remove( in_G_MIDI_timestamp );

		// d_iag_printf( "MIDI off event at ts:%d\n", G_MIDI_timestamp );
		// MIDIevent_print( off_event );

		// No off events left, so move on.
		if ( queue_event == NULL ){ break; 	}

		// Send the MIDI OFF message to the MIDI port
		MIDI_send_event( queue_event );
	} // queue iterator
}

#ifdef FEATURE_ENABLE_SONG_UPE
void RESET_measure_locator( unsigned char inStop ) {

	if ( !inStop ){
		sequencer_command_STOP();
	}
	G_measure_indicator_value = G_measure_locator / 5;
	G_measure_indicator_part = G_measure_locator % 5;
	align_measure_locators(); // set the start point
	if ( !inStop ){
		G_pause_bit 				= OFF;
		G_scroll_bit 				= OFF;
		prev_G_pause_bit  			= OFF;
		prev_G_stop_bit 			= OFF;
		force_stop_bit				= OFF;
		G_run_bit 					= ON;
	}
}
#endif
//
// PLAYER main entry point - called on each clock tick
//____________________________________________________________________________________
void PLAYER_dispatch( unsigned char in_G_TTC_abs_value ) {

	// Counter variable
	unsigned int i=0;
	#ifdef FEATURE_ENABLE_SONG_UPE
	unsigned int j=0;
	unsigned int row=0;
	Trackstruct* target_track = NULL;
	unsigned int measure = (G_measure_indicator_value * 5) + G_measure_indicator_part;
	unsigned int repeats = Repeats_Intervals[G_repeats_interval_idx];
	if ( repeats == 0xFFFFFFFF && measure >= 1279 )
	{
		G_measure_indicator_value = 0;
		G_measure_indicator_part = 0;
		measure = 0;
	}
	if ( (repeats != 0xFFFFFFFF && G_global_locator == 16 && ( measure == (G_measure_locator + repeats -1) )) || ( G_silent == ON )){
		if ( G_repeats_delay == OFF || ( G_silent == ON && G_delay == 0 )){
			MIDI_queue_flush();
			RESET_measure_locator(OFF);
			G_global_locator = 16;
			G_silent = OFF;
			G_reset = OFF;
			G_TTC_abs_value += 10;
			SEQUENCER_JUST_STARTED = OFF;
			return;
		}
		if ( G_repeats_delay == ON && G_silent == OFF ){
			G_silent = ON;
			G_delay = 16;
		}
	}
	if ( G_silent == ON ){
		if ( in_G_TTC_abs_value == 1 ){
			G_delay--;
		}
		if (G_delay < 16) return;
	}
	#endif
	// Play MIDI queue elements which are due at current timestamp
	play_MIDI_queue( G_MIDI_timestamp );

	// Advance page locators ..on every FOURTH TICK  ( 12/4 == 3 ):
	if ( (in_G_TTC_abs_value % 3) == 1 ){

		// Check if selected in GRID! ..as opposed to advancing ALL pages
		for( i=0; i<GRID_NROF_BANKS; i++ ) {

			// Skip inactive banks, btw- the empty flag is GRID_SELECTION_EMPTY
			if ( GRID_p_selection[i] != NULL ){

				// Advance page locator of the page, having its length..
				advance_page_locators( GRID_p_selection[i] );

			} // GRID selection is not NULL
		} // GRID bank iterator
	} // (G_TTC_abs_value) % 3 == 1


	// ..on the BEAT TICK
	if ( in_G_TTC_abs_value == 1 ){

		// Advance the global locator - normal speed master
		advance_global_locator();

		// on the measure
		if ( G_global_locator == 1 ) {
		#ifdef FEATURE_ENABLE_SONG_UPE

			for ( i=0; i < GRID_NROF_BANKS; i++ ){

				// Skip the banks that are not currently active
				if ( GRID_p_selection[i] != NULL ){

					// on the measure page mute
					if ( G_on_the_measure_trackMutepattern != 0 && selected_page_cluster_right_neighbor( GRID_p_selection[i], G_on_the_measure_trackMutepattern_pageNdx ) ) {
						for ( j=0; j < MATRIX_NROF_ROWS; j++ ){
							if ( G_on_the_measure_track[j] != NULL ){
								apply_page_cluster_track_mute_toggle( GRID_p_selection[i], G_on_the_measure_track[j] );
							}
						}
					}

					for ( row=0; row < MATRIX_NROF_ROWS; row ++ ){
						target_track = GRID_p_selection[i]->Track[row];

						// control track
						if ( Track_get_MISC(target_track, CONTROL_BIT ) ) {
							// skip because we call this on the bank change
							if ( target_track->ctrl_offset != 0 ){
								send_track_program_change( target_track, GRID_p_selection[i] );
							}
							target_track->ctrl_offset++;
						}
					}
				}
			}

			// reset the on the measure mute pattern
			if ( G_on_the_measure_trackMutepattern != 0 ){
				for (j=0; j<MATRIX_NROF_ROWS; j++){
					G_on_the_measure_track[j] = NULL;
				}
				G_on_the_measure_trackMutepattern = 0;
				G_on_the_measure_trackMutepattern_pageNdx = 0;
			}

			if (!G_align_bit){
				// Advance measure locator for pause measure scrolling
				if ( G_measure_indicator_part++ != 255 && G_measure_indicator_part % 5 == 0 ) {
					G_measure_indicator_part = 0;
					G_measure_indicator_value++;
				}
			}
		#endif

			// drum machine scene change
			if ( GRID_p_set_note_presel != 255) {
				MIDI_send(	MIDI_NOTE, GRID_p_set_midi_ch, GRID_p_set_note_offsets[GRID_p_set_note_presel], 127 );
				GRID_p_set_note_presel = 255;
			}
		}

		// PAGE PRESELECTION
		// Preselect pages as appropriate in every bank ..supports variable page lengths.
		for( i=0; i < GRID_NROF_BANKS; i++ ){

			// First time we are executing this after a start..
			if ( SEQUENCER_JUST_STARTED == TRUE ) {

				// Do not switch anything just upon sequencer start
				break;
			}

			// Locator of ONE in the playing page, or otherwise G_global_locator == 1 ..
			// ..prepare the page switch accordingly
			if (
					// The grid selection is NOT empty and.. selected page at locator 1
					(	( 	GRID_p_selection[i] != NULL )
					&&	( 	GRID_p_selection[i]->locator == 1 ) )
				||
					// The grid selection is empty and.. global locator at 1
					( 	( 	GRID_p_selection[i] == NULL )
					&& 	(	G_global_locator == 1 ) )
				){

				// Clock_presel added the natural neighbours.
				compute_chain_presel( i );

				// Clock preselect input (from key presses) moved into preselection. Page 239 book.
				// Do not switch pages when in the middle of a double click.
				// This fixes the issue with a potentially inconsistent double click in Grid..
				// ..but may introduce an issue at runtime, when several pages are queued.
				// This is bad for the nemo perform mode.. so we rule it out for Nemo

				if ( DOUBLE_CLICK_TARGET == 0 ){
					preselect_clock_presel( i );
				}
			}

		} // GRID_BANK_ITERATOR

		// Select the preselections on every beat!
		select_page_preselections();

		#ifdef FEATURE_ENABLE_DICE
		// Synchronize dice actions on beat.
		dice_synchronize();
		#endif
	} // in_G_TTC_abs_value  == 1 ..on the BEAT TICK


	SEQUENCER_JUST_STARTED = FALSE;

	// PAGE PLAY from grid selection: each bank has at most one active page
	for ( i=0; i < GRID_NROF_BANKS; i++ ){

		// Skip the banks that are not currently active
		if ( GRID_p_selection[i] != NULL ){

			// Play the page selected in current GRID bank
			PLAYER_play_page( GRID_p_selection[i], in_G_TTC_abs_value );
		}
	}

	#ifdef FEATURE_ENABLE_DICE
	// Advance DICE global clock (DICE_GLOBAL_CLOCK)
	Trackstruct* target_dice = throw_dice(NULL);
	if( target_dice && Dice_get_MISC( target_dice, DICE_GLOBAL_CLOCK ) ) {

		PLAYER_play_track( DICE_bank, row_of_track( DICE_bank, Dice_get_global_clock_track() ) );
	}
	#endif
}


