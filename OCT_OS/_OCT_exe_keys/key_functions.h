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



#include "key_tap_operation.h"
#include "key_chainer_operation.h"
#include "key_cluster_operation.h"

unsigned short get_otm_track_pattern() {

	unsigned char i;
	unsigned short otm_track_pattern = 0;

	for ( i = 0; i < MATRIX_NROF_ROWS; i++ ){

			if ( G_on_the_measure_track[i] != NULL ){
				SET_BIT(otm_track_pattern, i);
			}
	}

	return otm_track_pattern;
}

void apply_page_mute_pattern_operation( Pagestruct* target_page, unsigned short pattern, unsigned char operation ){

	unsigned char j = 0;
	G_on_the_measure_operation = OFF;
	for (j = 0; j < MATRIX_NROF_ROWS; j++){
		G_on_the_measure_track[j] = NULL;
	}

	if ( CHECK_BIT( G_track_page_chain_mod_bit, ON_THE_MEASURE_MOD ) ) {
		SET_BIT( G_on_the_measure_operation, operation );
		SET_BIT( G_on_the_measure_operation, OPERATION_MASK );
		G_on_the_measure_trackMutepattern = pattern;
		G_on_the_measure_trackMutepattern_pageNdx = target_page->pageNdx;
	} else if ( CHECK_BIT( G_track_page_chain_mod_bit, CLUSTER_MOD ) ) {
		apply_page_cluster_mute_pattern( target_page, pattern, 1 << operation );
	} else if ( operation == OPERATION_MUTE ) {
		target_page->trackMutepattern = pattern;
		if (pattern) {
			target_page->trackMutepatternStored = target_page->trackMutepattern;
		}
	} else if ( operation == OPERATION_SOLO ) {
		target_page->trackSolopattern = pattern;
	}
}

void apply_page_track_mute_toggle_operation( Pagestruct* target_page, Trackstruct* current_track, unsigned char operation ){

	G_on_the_measure_operation = OFF;
	G_on_the_measure_trackMutepattern = OFF;

	if ( CHECK_BIT( G_track_page_chain_mod_bit, ON_THE_MEASURE_MOD ) ) {
		apply_page_track_mute_toggle( target_page, current_track, &G_on_the_measure_trackMutepattern );
		SET_BIT( G_on_the_measure_operation, operation );
		G_on_the_measure_trackMutepattern_pageNdx = target_page->pageNdx;
		G_on_the_measure_track[row_of_track( target_page, current_track )] = current_track;
	} else if ( CHECK_BIT( G_track_page_chain_mod_bit, CLUSTER_MOD ) ) {
		apply_page_cluster_track_mute_toggle( target_page, current_track, 1 << operation );
	} else if ( operation == OPERATION_MUTE ) {
		apply_page_track_mute_toggle( target_page, current_track, &target_page->trackMutepattern );
		target_page->trackMutepatternStored = target_page->trackMutepattern;
	} else if ( operation == OPERATION_SOLO ) {
		apply_page_track_mute_toggle( target_page, current_track, &target_page->trackSolopattern );
	}
}

// Modifies the length of the previewed step in the matrix
void adjust_preview_stepLEN( unsigned char col ){

	unsigned char result = 0;

	// Length entry without wrap
	if ( col > page_preview_step_col ){

		result = ( col - page_preview_step_col) * 12;
	}

	// Length entry with wrap
	else{

		result = ( 16 - (page_preview_step_col - col)) * 12;
	}

	// Make the final length assignment
	page_preview_step->attr_LEN = result;
}




// Return the pointer of the step attribute value specified by given coordinates
char* ptr_of_step_attr_value( 	Pagestruct* target_page,
								unsigned char row,
								unsigned char col,
								unsigned char attr_ndx ){

	char* result = NULL;

	switch( attr_ndx ){

		case ATTR_STATUS:
			result = &target_page->Step[row][col]->attr_STATUS;
			break;

		case ATTR_VELOCITY:
			result = &target_page->Step[row][col]->attr_VEL;
			break;

		case ATTR_PITCH:
			result = &target_page->Step[row][col]->attr_PIT;
			break;

		case ATTR_LENGTH:
			result = &target_page->Step[row][col]->attr_LEN;
			break;

		case ATTR_START:
			result = &target_page->Step[row][col]->attr_STA;
			break;

		case ATTR_POSITION:
			result = &target_page->Step[row][col]->attr_POS;
			break;

//		case ATTR_DIRECTION:
//			result = &target_page->Step[row][col]->attr_DIR;
//			break;

		case ATTR_AMOUNT:
			result = &target_page->Step[row][col]->attr_AMT;
			break;

		case ATTR_GROOVE:
			result = &target_page->Step[row][col]->phrase_num;
			break;

		case ATTR_MIDICC:
			result = &target_page->Step[row][col]->attr_MCC;
			break;
	}

	// Send to Amanda mode if encountered NULL pointer!
	if ( result == NULL ){

		G_zoom_level = zoomAMANDA;
		GRID_message_code = 101;
		result = &target_page->Step[row][col]->attr_VEL;
	}

	return result;
}


// Return the pointer of the track attribute value specified by given coordinates
char* ptr_of_track_attr_value( 	Pagestruct* target_page,
								unsigned char track_ndx,
								unsigned char attr_ndx ){

	char* result = NULL;

	switch( attr_ndx ){

		case ATTR_STATUS:
			result = &target_page->Track[track_ndx]->attr_STATUS;
			break;

		case ATTR_VELOCITY:
			result = &target_page->Track[track_ndx]->attr_VEL;
			break;

		case ATTR_PITCH:
			result = &target_page->Track[track_ndx]->attr_PIT;
			break;

		case ATTR_LENGTH:
//			result = &target_page->Track[track_ndx]->attr_LEN;
			result = &target_page->Track[track_ndx]->LEN_factor;
			break;

		case ATTR_START:
//			result = &target_page->Track[track_ndx]->attr_STA;
			result = &target_page->Track[track_ndx]->STA_factor;
			break;

		case ATTR_POSITION:
			result = &target_page->Track[track_ndx]->attr_POS;
			break;

		case ATTR_DIRECTION:
			result = &target_page->Track[track_ndx]->attr_DIR;
			break;

		case ATTR_AMOUNT:
			result = &target_page->Track[track_ndx]->attr_AMT;
			break;

		case ATTR_GROOVE:
			result = &target_page->Track[track_ndx]->attr_GRV;

			break;
		case ATTR_MIDICC:
			result = &target_page->Track[track_ndx]->attr_MCC;

			break;
		case ATTR_MIDICH:
			result = &target_page->Track[track_ndx]->attr_MCH;
			break;
	}

	// Send to Amanda mode if encountered NULL pointer!
	if ( result == NULL ){
		G_zoom_level = zoomAMANDA;
		GRID_message_code = 102;
		result = &target_page->Track[track_ndx]->attr_VEL;
	}

	return result;
}



// Translate key presses in numeric quadrant to indexes
unsigned char BK_KEY_to_xdx( unsigned char keyNdx ){

	unsigned char result = 0;

	switch( keyNdx ){

		case 201:	result =  1; 	break;
		case 200:	result =  2; 	break;
		case 199:	result =  3; 	break;
		case 198:	result =  4; 	break;
		case 197:	result =  5; 	break;
		case 207:	result =  6; 	break;
		case 206:	result =  7; 	break;
		case 216:	result =  8; 	break;
		case 215:	result =  9; 	break;
		case 224:	result = 10; 	break;
	}

	return result;
}




// Resets all Track locators in the given page, unless a non-NULL in_track is passed.
// In that case the target track only is being processed. Used when changing track clocks
// on the fly to keep them well aligned to the global locator
void set_track_locators(	Pagestruct* target_page,
							Trackstruct* in_track,
							unsigned char target_LOC,
							unsigned char target_TTC	){

	unsigned char row=0;
	unsigned char i=0;
	Trackstruct* target_track = NULL;

	// Needs at least 10 bits to store all track flags
	unsigned short new_REC_pattern = 0;


	// This is the set-to-zero scenario and applies to all tracks
	if ( 	(target_LOC == 0) && (target_TTC == 0)
		){

		// Iterate through rows
		for ( row=0; row < MATRIX_NROF_ROWS; row++ ){

			if ( in_track != NULL ){
				target_track = in_track;
			}
			else{
				// Assign target - syntax simplification really..
				target_track = target_page->Track[row];
			}

			// If this is a hyped track, set its PLAY to NULL
			if ( target_track->hyper < 0xF0 ){
				target_track->chain_data[PLAY] = NULL;
			}

			// Set the TTC and LOC values
			target_track->TTC 				= 0;
			target_track->attr_LOCATOR 		= 0;
			target_track->frame_ndx 		= 0;

			// Empty the track trigger sets
			for (i=1; i<MATRIX_NROF_ROWS; i++ ){
				target_track->trigger[i] 	= 0;
			}

			// Reset the track clock divisor skipper status (higher nibble)
			target_track->attr_TEMPOMUL_SKIP &= 0x0F;

			// Do not continue if non-NULL in_track
			if ( in_track != NULL ){
				break;
			}

			// Set the PLAY to HEAD in order to reset the chains in page.
			// Do not touch the hyped tracks though.
			if ( target_track->chain_data[PLAY] != NULL ){
				target_track->chain_data[PLAY] = target_track->chain_data[HEAD];
			}

		} // Row iterator
	}

	// Preserve some old code in the first part - this one has the PLAY check condition..
	else{

		// Iterate through rows
		for ( row=0; row < MATRIX_NROF_ROWS; row++ ){

			if ( in_track != NULL ){
				target_track = in_track;
			}
			else{
				// Assign target - syntax simplification really..
				target_track = target_page->Track[row];
			}

			// Align play tracks to the given coordinates
			if ( target_track->chain_data[PLAY] == target_track ){

				// Set the TTC and LOC values
				target_track->TTC 			= target_TTC;
				target_track->frame_ndx 	= target_LOC;
				target_track->attr_LOCATOR 	= target_LOC;

				// Adjust the trigger sets for the new position
				for (i=1; i < MATRIX_NROF_ROWS; i++ ){

					// Empty the track trigger sets
					target_track->trigger[i] = 0;
				}

				// REC FLAG CARRY - carry REC flag over with the cursor
				if ( recording_active_in_chainof( target_page, row )
					){

					// Build the new REC pattern for the page
					new_REC_pattern |= (1 << row);
				}
			}
			// The tracks that are not playing should be set to 0 coordinates
			else{

				// Set the TTC and LOC values
				target_track->TTC 			= 0;
				target_track->attr_LOCATOR 	= 0;
				target_track->frame_ndx 	= 0;
			}

			// Do not continue if non-NULL in_track
			if ( in_track != NULL ){
				break;
			}

		} // Track iterator

		// Update the REC pattern in the page
		Page_setTrackRecPattern(target_page, new_REC_pattern );

	} // This is the non-zero set scenario
}




// Resets all locators associated with a given page to given LOC and TTC
void set_page_locators( 	Pagestruct* target_page,
							unsigned char target_LOC,
							unsigned char target_TTC ){

	// Reset page master locator
	target_page->locator = target_LOC;

	// Reset all track locators (including chain counters)
	set_track_locators( target_page, NULL, target_LOC, target_TTC );
}



#ifdef FEATURE_ENABLE_SONG_UPE
// Send the MIDI OFF notes belonging to a page
void stop_playing_page( 	Pagestruct* target_page,
							unsigned char in_G_TTC_abs_value,
							unsigned char pause ){

	unsigned int i = 0;
	unsigned char row = 0;

	if( target_page == NULL ) return;

	// Set all rebound bits in the page to default state
	for ( i=0; i < MATRIX_NROF_ROWS; i++ ){

		if ( Track_get_MISC(target_page->Track[i], CONTROL_BIT) == ON ) break;
		target_page->Track[i]->attr_MISC =
			( target_page->Track[i]->attr_MISC & 0xFE );
	}

	if ( G_stop_bit ) {
		// Refill the STA values in the page
		target_page->repeats_left = target_page->attr_STA;

		for ( row=0; row < MATRIX_NROF_ROWS; row++ ){

			if ( Track_get_MISC(target_page->Track[row], CONTROL_BIT ) ) {
				target_page->Track[row]->ctrl_offset = 0;
			}
		}
	}
}
#else
// Send the MIDI OFF notes belonging to a page
void stop_playing_page( 	Pagestruct* target_page,
							unsigned char in_G_TTC_abs_value ){

	unsigned int i = 0;

	if( target_page == NULL ) return;

	// Set all rebound bits in the page to default state
	for ( i=0; i < MATRIX_NROF_ROWS; i++ ){

		target_page->Track[i]->attr_MISC =
			( target_page->Track[i]->attr_MISC & 0xFE );
	}

	// Refill the STA values in the page
	target_page->repeats_left = target_page->attr_STA;
}
#endif


//
// Boolean: returns true if there is a change request pending, false otherwise
//
unsigned char selection_change_request_pending() {

	unsigned char i=0;

	for (i=0; i<GRID_NROF_BANKS; i++) {

		if (GRID_p_selection[i] != GRID_p_preselection[i]) {

			return TRUE;
		}
	}
	return FALSE;
}



//
// Boolean: returns true if the index is in the grid selection, false otherwise
//
unsigned char is_selected_in_GRID( Pagestruct* target_page ){

	unsigned char i=0;

	for ( i=0; i<GRID_NROF_BANKS; i++ ){

		if ( GRID_p_selection[i] == target_page ){

			return TRUE;
		}
	}
	return FALSE;
}


#ifdef FEATURE_ENABLE_SONG_UPE
//
// Boolean: returns true if the index is in the grid selection, false otherwise
//
unsigned char is_pre_selected_in_GRID( Pagestruct* target_page ){

	if ( G_pause_bit == OFF ) return FALSE;

	unsigned char i=0;

	for ( i=0; i<GRID_NROF_BANKS; i++ ){

		if ( GRID_p_preselection[i] == target_page ){

			return TRUE;
		}
	}
	return FALSE;
}


void ctrl_event_pack_step( Pagestruct* target_page, Trackstruct* target_track, char overwrite ){

	unsigned char i;

	if (overwrite){
		for ( i = 0; i < MATRIX_NROF_COLUMNS; i++) { // start from the right to find the first set step
			if ( Step_get_status( target_page->Step[target_track->trackId % 10][i], STEPSTAT_SKIP ) == ON ){
				copy_ctrl_track_to_step( target_page, target_track, target_page->Step[target_track->trackId % 10][i] );
				return;
			}
		}
	}
	else
	{
		for ( i = MATRIX_NROF_COLUMNS -1; i != 255; --i) { // start from the left to find an opening
			if ( Step_get_status( target_page->Step[target_track->trackId % 10][i], STEPSTAT_SKIP ) == OFF ){
				copy_ctrl_track_to_step( target_page, target_track, target_page->Step[target_track->trackId % 10][i] );
				return;
			}
		}
	}
}

unsigned char ctrl_event_assign_target_page( Pagestruct* target_page ){

	unsigned char i, j;
	for ( i = 0; i < MATRIX_NROF_ROWS; i++) {
		if ( !Track_get_MISC( target_page->Track[i], CONTROL_BIT ) ){
			return i;
		}
		if ( Track_get_MISC( target_page->Track[i], CONTROL_BIT )){
			// find next free step
			for ( j = MATRIX_NROF_COLUMNS -1; j != 255; --j) {
				if ( Step_get_status( target_page->Step[i][j], STEPSTAT_SKIP ) == OFF ){
					return i;
				}
			}
		}
	}
	return 255;
}

void ctrl_event_page_toggle( Pagestruct* target_page ){
	if ( G_rec_ctrl_track == NULL ){
		return;
	}
	G_rec_ctrl_track->attr_STATUS = 127;
	G_rec_ctrl_track->attr_DIR = 1;
	unsigned char trackIdx = ctrl_event_assign_target_page(G_rec_ctrl_page);
	if (trackIdx == 255) return;
	if ( &G_rec_ctrl_track != &G_rec_ctrl_page->Track[trackIdx] ){
		quick_assign_control_track(G_rec_ctrl_page, trackIdx);
	}
	ctrl_event_set_target_page( target_page );
}

void ctrl_event_set_target_page( Pagestruct* target_page ){
	G_rec_ctrl_track->program_change = 1 + (target_page->pageNdx / 10);
	G_rec_ctrl_track->attr_MCH = 9 - (target_page->pageNdx % 10);
	G_rec_ctrl_track->attr_VEL = G_rec_ctrl_page->attr_STA - G_rec_ctrl_page->repeats_left;
	ctrl_event_pack_step(G_rec_ctrl_page, G_rec_ctrl_track, OFF);
}

void ctrl_event_mute_target_track( Pagestruct* target_page, Trackstruct* target_track ){
	if (G_last_ctrl_page[G_rec_ctrl_page->pageNdx % 10] != G_rec_ctrl_page)
	{
		// this page is not active in this measure offset
		return;
	}
	ctrl_event_set_target_page( target_page );
	if ( G_rec_ctrl_track == NULL ){
		return;
	}
	G_rec_ctrl_track->attr_DIR = 1;
	G_rec_ctrl_track->STA_factor = (target_track->trackId % 10);
	// set the measure offset
	G_rec_ctrl_track->attr_VEL = G_last_ctrl_offset[G_rec_ctrl_page->pageNdx % 10];
	SET_BIT(G_rec_ctrl_track->attr_MISC, TRK_CTRL_MUT_PGTGL);
	ctrl_event_pack_step(G_rec_ctrl_page, G_rec_ctrl_track, ON);
	target_page->trackMutepatternStored = 0;
}

void unarm_ctrl(){
	G_rec_ctrl_track = NULL;
	G_rec_ctrl_page = NULL;
	GRID_editmode = ON;
	G_unarm_ctrl = OFF;
}

void clear_ctrl_track( Pagestruct* target_page, Trackstruct* target_track ){
	if ( target_track == NULL || Track_get_MISC( target_track, CONTROL_BIT ) == OFF ) {
		return;
	}
	CLEAR_BIT(target_track->attr_CCAMT, TRK_CTRL_TEMPO);
	CLEAR_BIT(target_track->attr_MISC, CONTROL_BIT);
	CLEAR_BIT(target_track->attr_MISC, TRK_CTRL_MOVE);
	CLEAR_BIT(target_track->attr_MISC, TRK_CTRL_MIX);
	CLEAR_BIT(target_track->attr_MISC, TRK_CTRL_CLEAR);
	CLEAR_BIT(target_track->attr_MISC, TRK_CTRL_PGTGL);
	CLEAR_BIT(target_track->attr_MISC, TRK_CTRL_MUT_PGTGL);
	Track_clear_full( target_page, target_track->trackId % 10 );
	target_track->attr_MCH = 0;
}

#endif

//
// Select the target_page to be played by the grid.
// Automatically unselect the rest of the bank. Bank is horizontal
void grid_select( 	Pagestruct* target_page,
					unsigned char target_status ){

	unsigned char 	j = 0;
	unsigned char   k = GRID_switch_mode;

// d_iag_printf( "..grid_select page:%d status:%d\n", target_page->pageNdx, target_status );

	// Account for the stopped mode
	if ( G_run_bit == OFF ){
		GRID_switch_mode = GRID_SWITCH_DIRECT;
	}

	// Refill the page repeats value
	target_page->repeats_left = target_page->attr_STA;

	// Find the first page in the bank of the target page (from L to R)
	j = target_page->pageNdx % 10;


	// If page is to be selected from its row, select it now
	switch( target_status ) {

		case ON:
			// Page needs to be turned ON

			// Do the actual switching
			switch( GRID_switch_mode ){

				case GRID_SWITCH_OCLOCK:

					// Add page to the GRID_clock_presel to be switched into presel later on
					GRID_p_clock_presel[ j%10 ] = target_page;

					break;

				case GRID_SWITCH_DIRECT:

					// Add page to the GRID_preselection directly
					GRID_p_preselection[ j%10 ] = target_page;

					// Propagate the change over to clock presel
					GRID_p_clock_presel[ j%10 ] = GRID_p_preselection[ j%10 ];
					break;
			}
			break;


		case OFF:
			// Page needs to be turned OFF
			switch( GRID_switch_mode ){

				case GRID_SWITCH_OCLOCK:

					// Add page to the GRID_clock_presel to be switched into presel later on
					GRID_p_clock_presel[ j%10 ] = NULL;

					break;

				case GRID_SWITCH_DIRECT:

					// Remove page from GRID_preselection.
					GRID_p_preselection[ j%10 ] = NULL;

					// Propagate the change over to clock presel
					GRID_p_clock_presel[ j%10 ] = GRID_p_preselection[ j%10 ];
					break;
			}
			break;

	} // switch( target status

	// Quick and dirty fix
	if ( G_run_bit == OFF ){

		GRID_switch_mode = k;
	}

	// If the sequencer is stopped, act immediately.
	#ifdef FEATURE_ENABLE_SONG_UPE
	if (	(G_run_bit == OFF)  &&  (G_pause_bit == OFF || G_scroll_bit)	){
	#else
	if (	(G_run_bit == OFF)  	){
	#endif
		// Direct selection of the preselections, because not done by the player
		select_page_preselections();
	}
}



// Set the TTCs of all tracks in page to in_value
void set_track_TTCs( Pagestruct* target_page, unsigned char in_value ){

	unsigned char row = 0;

	if ( target_page == NULL ) return;

	for( row = 0; row < MATRIX_NROF_ROWS; row++ ){

		// Make sure we don't touch hyper tracks
		if ( target_page->Track[row]->chain_data[PLAY] != NULL ){
			target_page->Track[row]->TTC = in_value;
		}
	}
}



// Jumpstart the sequencer by setting all conditions
void sequencer_START() {

	unsigned char i=0;

	#warning GS: q+d>track TTCs need clear b/c they get changed somehow..
	// Clean all tracks in active pages, prepare them for play
	// Stop playing all pages of the grid selection
	for (i=0; i < GRID_NROF_BANKS; i++) {

		// Store the page address in the pointer buffer
		if ( GRID_p_selection[i] != NULL ){

			// Stop playing the target page and do not reset its track locators
			#ifdef FEATURE_ENABLE_SONG_UPE
			stop_playing_page( GRID_p_selection[i], G_TTC_abs_value, prev_G_pause_bit );
			prev_G_pause_bit = OFF;
			#else
			stop_playing_page( GRID_p_selection[i], G_TTC_abs_value );			
			#endif
			// Set the TTC values of all tracks to 0
			set_track_TTCs( GRID_p_selection[i], 0 );
		}
	} // Iterate through the playing pages of the grid


	// Reset the G_MIDI_timestamp;
	G_MIDI_timestamp = 0;

	// Play start condition
	G_TTC_abs_value 	= 0;
	G_tempo_tracker	= 1;
	G_MIDI_tick_counter 	= 1;

	// Flag for the player in chain mode
	SEQUENCER_JUST_STARTED = ON;

	// Recalc the timer refill value.
	G_TIMER_REFILL_update();

	// Raise the flag for timer refill
	G_manualStartBo = TRUE;

	// Turn the blinker on
	G_master_blinker = ON;


	// Set the sequencer running conditions
	G_pause_bit = OFF;
	G_run_bit = ON;
	#ifdef FEATURE_ENABLE_SONG_UPE
	prev_G_stop_bit = (G_run_bit == ON);
	G_measure_indicator_part--;
	#endif
	// MIDI CLOCK: START - only when clock is active or machine is slave
	if (	( G_clock_source == INT )
			||	(	( G_clock_source == EXT )
				&&	MIDICLOCK_PASSTHROUGH == TRUE )
		){

		#ifdef FEATURE_SOLO_REC
		if ( G_zoom_level == zoomSOLOREC && SOLO_rec_record_OTM == ON && SOLO_has_rec == OFF ){
			SOLO_rec_CLOCKSTART_OTM = ON;
		}
		else {
			// Start external device
			MIDI_send( MIDI_CLOCK, MIDICLOCK_START, 0, 0);
		}
		#else
		// Start external device
		MIDI_send( MIDI_CLOCK, MIDICLOCK_START, 0, 0);
		#endif
	}

	// Start the sequencer thread.
	kickSequencerThread();
}



// Halts the sequencer, disabling running activities,
// ..but leaving locators untouched
void sequencer_HALT() {

	unsigned char 	i 			= 0;
	Pagestruct* 	target_page = NULL;

	#ifdef FEATURE_ENABLE_SONG_UPE
	prev_G_stop_bit = (G_run_bit == OFF);
	prev_G_pause_bit = !force_stop_bit;
	force_stop_bit = OFF;
	#endif
	
	// Turn of running bit first
	G_run_bit 		= OFF;

	// Set the pause bit for now, preparing MTC continue
	G_pause_bit 	= ON;

	// Flush the MIDI queue - play all note OFFs and drop any ONs
	MIDI_queue_flush();

	// Stop playing all pages of the grid selection
	for (i=0; i < GRID_NROF_BANKS; i++) {

		// Store the page address in the pointer buffer
		if ( GRID_p_selection[i] != NULL ){

			target_page = GRID_p_selection[i];

			// Stop playing the target page and do not reset its track locators
			#ifdef FEATURE_ENABLE_SONG_UPE
			stop_playing_page( target_page, G_TTC_abs_value, prev_G_pause_bit );
			#else
			stop_playing_page( target_page, G_TTC_abs_value );
			#endif
			// Set the TTC values of all tracks to 0
			set_track_TTCs( target_page, 0 );
		}

	} // Iterate through the playing pages of the grid

}



// Un-halt the sequencer - typically after a pause state
void sequencer_UNHALT(){

	// Play start condition
	G_TTC_abs_value 	= 0;
	G_tempo_tracker		= 1;
	G_MIDI_tick_counter = 1;

	// Reset the timer
	G_TIMER_REFILL_update();

	// Set the sequencer running conditions
	#ifdef FEATURE_ENABLE_SONG_UPE
	prev_G_stop_bit = (G_run_bit == ON);
	#endif
	G_run_bit = ON;
	G_pause_bit = OFF;

	// Get the sequencer going.
	kickSequencerThread();
}


// Init the event offsets for the page
void init_event_offsets( Pagestruct* target_page ){

	unsigned char row = 0;

	// Row iterator
	for ( row=0; row < MATRIX_NROF_ROWS; row++ ){

		target_page->Track[row]->event_offset[ATTR_VELOCITY] 	= 0;
		target_page->Track[row]->event_offset[ATTR_PITCH] 		= 0;
		target_page->Track[row]->event_offset[ATTR_LENGTH] 		= 0;
		target_page->Track[row]->event_offset[ATTR_START] 		= 0;
		target_page->Track[row]->event_offset[ATTR_DIRECTION] 	= 0;
		target_page->Track[row]->event_offset[ATTR_AMOUNT] 		= 0;
		target_page->Track[row]->event_offset[ATTR_GROOVE] 		= 0;
		target_page->Track[row]->event_offset[ATTR_MIDICC] 		= 0;
		target_page->Track[row]->event_offset[ATTR_MIDICH] 		= 0;
	}
}


// All tracks in page point to their HEAD tracks as PLAY tracks
void rewind_page_chains( Pagestruct* target_page ){

	unsigned char i=0;
	for ( i=0; i < MATRIX_NROF_ROWS; i++ ){

		// Make sure we do not touch the hyped tracks.
		if ( target_page->Track[i]->chain_data[PLAY] != NULL ){

			// Rewind the chains to play the HEAD track.
			(target_page->Track[i])->chain_data[PLAY] =
				(target_page->Track[i])->chain_data[HEAD];
		}
	}
}


// Brings all sequencer locators into start position -> inlcuding rewinding all chains in page
void sequencer_RESET( unsigned char force_stop ){

	unsigned char 	i 	= 0;

	#ifdef FEATURE_SOLO_REC
	if ( G_pause_bit == ON ){
		G_measure_locator = OFF;
		SOLO_rec_measure_count = OFF;
		SOLO_rec_measure_pos = OFF;
	}
	#endif

	// Remove the pause	state for good;
	G_pause_bit 	= OFF;

	// Reset the global locator
	G_global_locator = 0;

	// RESET LOCATORS AND CHAIN COUNTERS FOR THE PLAYING PAGES

	// All pages in the grid selection reset to zero position
	for (i=0; i < GRID_NROF_BANKS; i++) {

		// Store the page address in the pointer buffer
		if (GRID_p_selection[i] != NULL) {

			// Reset all locators in target page
			set_page_locators( GRID_p_selection[i], 0, 0 );

			// Rewind all chains in page to their respective HEAD tracks
			rewind_page_chains( GRID_p_selection[i] );

			#ifdef FEATURE_ENABLE_SONG_UPE
			if ( !prev_G_pause_bit ) {
				// Reset the repeates values for the target page
				GRID_p_selection[i]->repeats_left = GRID_p_selection[i]->attr_STA;
			}
			#else
			GRID_p_selection[i]->repeats_left = GRID_p_selection[i]->attr_STA;
			#endif
			// Init event offsets
			init_event_offsets( GRID_p_selection[i] );
		}
	}

	if ( force_stop ) {
		unsigned char j	= 0;

		// Bring all chained banks to the first page in chain.
		for ( i=0; i < GRID_NROF_BANKS; i++ ){

			// Set the chain start in the clock preselection
			if ( GRID_p_selection[ i ] != NULL ) {
				compute_chain_startpoints( i );

			} else {

				// Find the first page with data
				for (j=0; j < MATRIX_NROF_COLUMNS; j++) {
					if ( Page_repository[ i + (10 * j) ].page_clear == OFF ) {
						GRID_p_selection[ i ] = &Page_repository[ i + (10 * j) ];
						GRID_p_clock_presel[ i ] = &Page_repository[ i + (10 * j) ];
						break;
					}
				}
			}

			// Move the clock preselection into plain preselection
			preselect_clock_presel( i );

			GRID_p_preselection[ i ] = NULL;

			// Switch the gridbank
			switch_gridbank( i );
		}
	}

	// Reset the running status bytes
	G_running_status_byte_OUT_UART[0] 	= 0;
	G_running_status_byte_OUT_UART[1] 	= 0;
	G_running_status_byte_IN_UART[0]	= 0;
	G_running_status_byte_IN_UART[1]	= 0;

	// Init the recording event queue
	NOTE_IN_init();
}

void align_measure_locators(){

	unsigned int	n=0,
					row=0;
	unsigned char 	j=0;
	unsigned int 	i=0;
	signed short 	next_ndx = 0,
					sel_ndx  = 0,
					this_ndx = 0;
	Trackstruct* target_track = NULL;

	// [INIT] Reset the locators
	sequencer_RESET(ON);
	G_pause_bit 				= ON;
//	G_scroll_bit 				= ON;
//	prev_G_stop_bit				= ON;
	G_MIDI_timestamp			= 0;
	G_global_locator			= 0;
	G_TTC_abs_value				= 0;


	for (i=0; i <= GRID_NROF_BANKS; i++) { // for each grid bank

//		G_last_ctrl_offset[i] = 0;
//		G_last_ctrl_page[i] = NULL;

		// Reset all repeats and offsets
		for (j=0; j < MATRIX_NROF_COLUMNS; j++) {
			next_ndx = i + (10 * (j));

			Page_repository[next_ndx].repeats_left = Page_repository[next_ndx].attr_STA;
			Page_repository[next_ndx].locator = 1;
			Page_repository[next_ndx].trackMutepattern = 0;
			for ( row=0; row < MATRIX_NROF_ROWS; row++ ){

//				if ( Track_get_MISC(Page_repository[next_ndx].Track[row], CONTROL_BIT ) ) {
//					Page_repository[next_ndx].Track[row]->ctrl_offset = 0;
//					Page_repository[next_ndx].Track[row]->attr_LOCATOR = 0;
//				}
			}
		}

		// In given bank: select the leftmost page
		for (j=0; j < MATRIX_NROF_COLUMNS; j++) {

			next_ndx = i + (10 * (j));

			// If the left neighbor is either empty or out of the bounds - break,
			// else implicitly continue.
			if ( next_ndx >= 150 )
			{
				next_ndx = i;
			}
			if ( Page_repository[next_ndx].page_clear == OFF ) {
//				GRID_p_clock_presel[ i ] = &Page_repository[next_ndx];
//				GRID_p_preselection[ i ] = &Page_repository[next_ndx];
//				GRID_p_selection[ i ] = &Page_repository[next_ndx];
				GRID_p_clock_presel[ i ] = NULL;
				GRID_p_preselection[ i ] = NULL;
				GRID_p_selection[ i ] = NULL;
				break;
			}
		}

//		G_last_ctrl_page[ i ] = &Page_repository[next_ndx];
	}

//	// fire pre-play events
//	for (i=0; i <= GRID_NROF_BANKS; i++) { // for each grid bank
//		for ( row=0; row < MATRIX_NROF_ROWS; row ++ ){
//			if ( (GRID_bank_playmodes & ( 1 << i )) == 0 ) continue;
//			if ( GRID_p_selection[i] != NULL ){
//				target_track = GRID_p_selection[i]->Track[row];
//				send_track_program_change( target_track, GRID_p_selection[i] );
//			}
//		}
//	}

	next_ndx = 0;
	// Reset the G_MIDI_timestamp;
	G_MIDI_timestamp = 0;
	cpu_load_reset( G_TIMER_REFILL, G_TIMER_REFILL );

	for (n=1; n <= G_measure_locator; n++) { // for each measure

		// Update LEDS in inner circle for status report.
		show_progress_bar( 1 + ((n * 8) / G_measure_locator), MIR_GREEN );
		VIEWER_show_MIR();

		for (i=0; i < GRID_NROF_BANKS; i++) { // for each grid bank

			if ( (GRID_bank_playmodes & ( 1 << i )) == 0 ) continue;

			if ( GRID_p_selection[i] != NULL ){
//				G_last_ctrl_offset[i]++;
			}

			// Skip if we are pointing to a null page in the bank..
			if ( GRID_p_clock_presel[ i ] == NULL ) continue;

			// Remember the starting index
			this_ndx = GRID_p_clock_presel[ i ]->pageNdx;
			sel_ndx = GRID_p_selection[ i ]->pageNdx;
			GRID_p_selection[ i ] = &Page_repository[this_ndx];
			GRID_p_preselection[ i ] = &Page_repository[this_ndx];

			// Iterate page tracks to fire explicit control track events
			for ( row=0; row < MATRIX_NROF_ROWS; row ++ ){
				target_track = GRID_p_selection[i]->Track[row];
				// control track
//				if ( Track_get_MISC(target_track, CONTROL_BIT ) && target_track->trackId != 65535 ) {
//					target_track->ctrl_offset++;
//					send_track_program_change( target_track, GRID_p_selection[i] );
//				}
			}

			// Move the page locator position
			if ( Page_repository[sel_ndx].repeats_left-- > 0 ) {

				next_ndx = this_ndx + 10;

				if (	( next_ndx > 150 ) ||
						( Page_repository[next_ndx].page_clear == ON )	){
					// Walk to beginning of chain
					for (j=0; j <= (this_ndx / 10); j++) {
						next_ndx = this_ndx - (10 * (j));
						if ( next_ndx < 0 || Page_repository[next_ndx].page_clear == ON ) {
							next_ndx = next_ndx + 10;
							break; // don't walk off the edge
						}
					}
				}
				if ( next_ndx < 161 ){ // there are no active pages in this grid track
					if ( &Page_repository[next_ndx].page_clear == OFF )
					{
						GRID_p_selection[ i ] = &Page_repository[next_ndx];
						GRID_p_clock_presel[ i ] = &Page_repository[next_ndx];
						GRID_p_preselection[ i ] = NULL;
					}
					else {
						GRID_p_clock_presel[ i ] = &Page_repository[sel_ndx];
						GRID_p_preselection[ i ] = NULL;
					}
				}

				// [PAGE CHANGE] Otherwise enable the next page in the cluster
			} else {

				next_ndx = this_ndx + 10;

				if (	( next_ndx > 150 ) ||
						( Page_repository[next_ndx].page_clear == ON )	){
					// Walk to beginning of chain
					for (j=0; j <= (this_ndx / 10); j++) {
						next_ndx = this_ndx - (10 * (j));
						if ( next_ndx < 0 || Page_repository[next_ndx].page_clear == ON ) {
							next_ndx = next_ndx + 10;
							break; // don't walk off the edge
						}
					}
				}

				if ( next_ndx < 161 ){ // there are no active pages in this grid track

					// Iterate page tracks to fire explicit control track events
					for ( row=0; row < MATRIX_NROF_ROWS; row ++ ){
						target_track = Page_repository[next_ndx].Track[row];
//						target_track->ctrl_offset = OFF;

						// control track
//						if ( Track_get_MISC(target_track, CONTROL_BIT) && target_track->trackId != 65535
//						){
//							send_track_program_change( target_track, &Page_repository[next_ndx] );
//						}
					}

//					G_last_ctrl_page[ i ] = &Page_repository[next_ndx];
//					G_last_ctrl_offset[ i ] = 0;

					GRID_p_selection[ i ] = &Page_repository[next_ndx];
					GRID_p_preselection[ i ] = &Page_repository[next_ndx];
					GRID_p_clock_presel[ i ] = &Page_repository[next_ndx];

					Page_repository[sel_ndx].repeats_left = Page_repository[sel_ndx].attr_STA;
					Page_repository[this_ndx].repeats_left = Page_repository[this_ndx].attr_STA;
					Page_repository[next_ndx].repeats_left = Page_repository[next_ndx].attr_STA;
				}
			}
		}
	}
	// First time didn't work second time did
	G_run_bit = ON;
//	G_align_bit = ON;
	if (G_measure_locator > 0){
		for (i=0; i < GRID_NROF_BANKS; i++) { // for each grid bank
			if (GRID_p_selection[ i ] != NULL && GRID_p_selection[ i ]->page_clear == OFF && (GRID_bank_playmodes & ( 1 << i )) != 0){
				// move the page cursor to the correct track position adjusting for track chains and tempo multipliers
				drivePageCursor(GRID_p_selection[ i ], GRID_p_selection[ i ]->attr_STA - GRID_p_selection[ i ]->repeats_left);
			}
		}
	}

	for (i=0; i < GRID_NROF_BANKS; i++) { // for each grid bank
		// clear all pre-selections
		GRID_p_preselection[ i ] = NULL;
		if (GRID_p_clock_presel[ i ] == GRID_p_selection[ i ]) {
			GRID_p_preselection[ i ] = GRID_p_selection[ i ];
		}
	}
//	G_align_bit = OFF;
	G_run_bit = OFF;
//	G_reset = ON;
}

void copy_steps_to_recording(Pagestruct* target_page, unsigned char undo){
	int col, i;

	Notestruct* target_note;
	Notestruct* undo_note;
	Stepstruct* target_step;
	col = grid_col(target_page->pageNdx);

	for (i=0; i<MAX_NROF_PAGE_NOTES; i++){

		target_note = Rec_repository[col].Note[i];
		undo_note = Rec_undo_repository[col].Note[i];
		target_step = target_page->Step[grid_row(i)][grid_col(i)];
		stepToNote(target_step, target_note);
		if ( undo == ON ){
			copyNote(target_note, undo_note);
		}
	}
}

void copy_page_cluster_to_recording(){

	signed short    	this_ndx = first_page_in_cluster(SOLO_rec_page->pageNdx);

	// For each page in the record chain
	// track forward
	while ( 	(this_ndx < MAX_NROF_PAGES) &&
			(Page_repository[this_ndx].page_clear == OFF)
	){

		copy_steps_to_recording( &Page_repository[this_ndx], ON );
		this_ndx += 10;
	}
}

void create_next_freeflow_page_cluster(unsigned char next_ndx){

	Pagestruct* next_page = &Page_repository[ next_ndx ];
	create_page_record_track_chain(next_page, MATRIX_NROF_ROWS);
	SOLO_rec_freeflow_trim = ON; // last page requires trim()
}

void cut_freeflow_track_chain(Pagestruct* target_page, unsigned char last_measure, unsigned char count, unsigned char clear_pages){
	int row, col, prev, measureNdx, len;

	col = grid_col(target_page->pageNdx);
	prev = Rec_repository[col].measure_count;
	measureNdx = last_measure;

	SOLO_rec_freeflow_measures -= ( prev - count );
	SOLO_rec_measure_count = SOLO_rec_freeflow_measures;
	len = MATRIX_NROF_ROWS - count;

	// clear steps before the cut
	for ( row=0; row < len; row++ ){

		Track_clear_steps( target_page, row );
	}

	target_page->attr_STA = count;
	target_page->repeats_left = target_page->attr_STA;
	Rec_repository[col].measure_count = target_page->attr_STA;
	copy_steps_to_recording(target_page, OFF);

	if ( clear_pages == ON ){
		clear_pages_left( target_page );
	}
	SOLO_pos_marker_in = OFF;
	SOLO_pos_in = NULL;
}

void shift_down_freeflow_track_chain(Pagestruct* target_page, unsigned char last_measure, unsigned char count){
	int row, col, j, idx;

	col = grid_col(target_page->pageNdx);
	idx = last_measure;
	j = (MATRIX_NROF_ROWS -1);

	if (idx != j) {
		// shift down by inserting
		for ( row=idx; row >= 0; row-- ){

			Track_hard_init( target_page->Track[j], target_page->Track[j]->trackId ); // clear the end most track
			// insert the last original track in the chain to the new end from the bottom up
			Track_copy( target_page, row, target_page, j-- ); // Execute the copy operation
			Track_clear_steps( target_page, row );
		}
	}

	target_page->attr_STA = count;
	target_page->repeats_left = target_page->attr_STA;
	Rec_repository[col].measure_count = target_page->attr_STA;

	clear_pages_right( target_page );
}


void trim_freeflow_track_chain(Pagestruct* target_page, unsigned char measureNdx){
	int row, j;
	unsigned char measureCnt;

	measureCnt = measureNdx + 1;

	if ( measureCnt == MATRIX_NROF_ROWS ){
		return;
	}

	SOLO_rec_freeflow_measures -= (MATRIX_NROF_ROWS - measureNdx);
	SOLO_rec_measure_count = SOLO_rec_freeflow_measures;

	// shift down by inserting
	j = (MATRIX_NROF_ROWS -1);
	for ( row=measureNdx; row >= 0; row-- ){
		Track_hard_init( target_page->Track[j], target_page->Track[j]->trackId ); // clear the end most track
		// insert the last original track in the chain to the new end from the bottom up
		Track_copy( target_page, row, target_page, j-- ); // Execute the copy operation
		Track_clear_steps( target_page, row );
	}

	target_page->attr_STA = measureCnt;
	target_page->repeats_left = measureCnt;
	Rec_repository[grid_col(target_page->pageNdx)].measure_count = measureCnt;
	Rec_undo_repository[grid_col(target_page->pageNdx)].measure_count = measureCnt;
}

unsigned char has_valid_record_cluster_format(Pagestruct* target_page, unsigned char press){

	if ( press == OFF && GRID_p_selection_cluster == OFF ){

		return FALSE;
	}

	unsigned char this_ndx = first_page_in_cluster(target_page->pageNdx);

	// For each page in the record chain
	// track forward
	while ( 	(this_ndx < MAX_NROF_PAGES) &&
			(Page_repository[this_ndx].page_clear == OFF)
	){

		target_page = &Page_repository[this_ndx];
		if ( find_record_track_chain_start(target_page) == NOP ){

			return FALSE;
		}
		this_ndx += 10;
	}

	return TRUE;
}

unsigned char find_record_track_chain_start(Pagestruct* target_page){
	int row;

	unsigned char start = 0, in_chain = FALSE, chain_count = 0;
	Trackstruct* current_track;

	current_track = target_page->Track[0];

	// Compute the green pattern - all tracks in chain
	for ( row=0; row < MATRIX_NROF_ROWS; row++ ){

		if ( current_track->chain_data[NEXT] != target_page->Track[row] ){
			in_chain = FALSE;
		}

		// Find head chains larger than one track
		if ( current_track != current_track->chain_data[NEXT] ){ // not a standalone track

			if ( in_chain == FALSE ){

				start = row_of_track( target_page, current_track );
				chain_count++;
			}
			in_chain = TRUE;
		}
		else if ( row == 9 ){ // last track is not part of a chain

			if ( chain_count > 0 ){

				return NOP;
			}
		}
		else {

			in_chain = FALSE;
		}
		current_track = target_page->Track[row];
	}

	if ( chain_count == 0 ){

		return MATRIX_NROF_ROWS -1; // last row index
	}
	else if ( chain_count > 1 ){ // more than one track chain

		return NOP;
	}

	return start;
}

void duplicate_record_track_chain(Pagestruct* target_page){
	int row, col, idx, j, count, start;

	col = grid_col(target_page->pageNdx);
	idx = find_record_track_chain_start(target_page);
	j = idx;
	start = idx - Rec_repository[col].measure_count;
	count = idx - start;

	if (idx == NOP || count > 5){
		return;
	}

	for ( row=start; row < (start + count); row++ ){

		Track_hard_init( target_page->Track[row], target_page->Track[row]->trackId );
		Track_copy( target_page, j, target_page, row );

		assign_track_scale_value( track_scale_value( j, target_page->scaleLead ), row, target_page->scaleLead );
		assign_track_scale_value( track_scale_value( j, target_page->scaleNotes ), row, target_page->scaleNotes );

		j++;
	}

	target_page->attr_STA = target_page->attr_STA * 2;
	target_page->repeats_left = target_page->attr_STA;
	Rec_repository[col].measure_count = target_page->attr_STA;
	Rec_undo_repository[col].measure_count = target_page->attr_STA;
	copy_steps_to_recording(target_page, OFF);
	reset_page_cluster( target_page );
}

void create_page_record_track_chain(Pagestruct* target_page, unsigned int measures){
	int n, m, row, col;

	col = grid_col(target_page->pageNdx);

	PAGE_init(target_page, target_page->pageNdx, false);
	for ( row=0; row < MATRIX_NROF_ROWS; row++ ){
		Track_hard_init( target_page->Track[row], target_page->Track[row]->trackId );
		target_page->Track[row]->attr_MCH = SOLO_midi_ch;
		target_page->Track[row]->attr_PIT = MIDDLE_C;
	}

	row = grid_row(target_page->pageNdx);
	SOLO_rec_bank = row;

	target_page->page_clear = OFF;
	target_page->attr_STA = measures;
	target_page->repeats_left = measures;
	Rec_repository[grid_col(target_page->pageNdx)].measure_count = target_page->attr_STA;
	Rec_undo_repository[grid_col(target_page->pageNdx)].measure_count = target_page->attr_STA;

	GRID_bank_playmodes = 0;
	GRID_bank_playmodes |= 1 << row;
	SOLO_rec_save_playmodes |= 1 << row;

	if ( GRID_p_selection[ row ] == NULL ){

		GRID_p_selection[ row ] = target_page;
		GRID_p_preselection[ row ] = target_page;
		GRID_p_clock_presel[ row ] = target_page;
	}

	// reset first
	remove_track_chain(target_page);
	Page_setTrackRecPattern(target_page, 0);
	target_page->trackSelection = 0;

	m = MATRIX_NROF_ROWS - measures; // from the bottom up

	for (n=9; n >= m; --n) { // each measure
		target_page->trackSelection |= 1 << n;
	}

	chain_selected_tracks( target_page );
	Page_setTrackRecPatternBit( target_page, (n+1) );
}

void clear_page_record_track_chain(Pagestruct* target_page){
	int row, col, ndx;

	signed short this_ndx = first_page_in_cluster(target_page->pageNdx);
	unsigned char page_col;

	clearAllTranspose();

	// For each page in the record chain
	// track forward
	while ( 	(this_ndx < MAX_NROF_PAGES) &&
			(Page_repository[this_ndx].page_clear == OFF)
	){

		page_col = grid_col(Page_repository[this_ndx].pageNdx);

		// Init each step in each page
		for ( row=0; row < MATRIX_NROF_ROWS; row++ ){
			for ( col=0; col < MATRIX_NROF_COLUMNS; col++ ){

				ndx = grid_ndx(row, col);
				Step_init(Page_repository[this_ndx].Step[row][col]);
				copyNote(Rec_repository[page_col].Note[ndx], Rec_undo_repository[page_col].Note[ndx]);
				initNote(Rec_repository[page_col].Note[ndx]);
			}
		}
		this_ndx += 10;
	}
}

void checkpoint_save_undo_track_chain(Pagestruct* target_page){
	int row, col, ndx;

	signed short this_ndx = first_page_in_cluster(target_page->pageNdx);
	unsigned char page_col;

	// For each page in the record chain
	// track forward
	while ( 	(this_ndx < MAX_NROF_PAGES) &&
			(Page_repository[this_ndx].page_clear == OFF)
	){

		page_col = grid_col(Page_repository[this_ndx].pageNdx);

		// Init each step in each page
		for ( row=0; row < MATRIX_NROF_ROWS; row++ ){
			for ( col=0; col < MATRIX_NROF_COLUMNS; col++ ){

				ndx = grid_ndx(row, col);
				stepToNote(Page_repository[this_ndx].Step[row][col], Rec_undo_repository[page_col].Note[ndx]);
			}
		}
		this_ndx += 10;
	}

	SOLO_edit_buffer_volatile = ON;
	MIX_TIMER = ON;
	// Setup alarm for the MIX TIMER
	cyg_alarm_initialize(	alarm_hdl,
							cyg_current_time() + TIMEOUT_VALUE,
							0 );
}

void clear_page_record_mcc_data(Pagestruct* target_page){
	int row, col;

	signed short    	this_ndx = first_page_in_cluster(target_page->pageNdx);

	// For each page in the record chain
	// track forward
	while ( 	(this_ndx < MAX_NROF_PAGES) &&
			(Page_repository[this_ndx].page_clear == OFF)
	){
		// Init each step in each page
		for ( row=0; row < MATRIX_NROF_ROWS; row++ ){
			for ( col=0; col < MATRIX_NROF_COLUMNS; col++ ){

				Page_repository[this_ndx].Step[row][col]->attr_MCC		= 	STEP_DEF_MIDICC;
			}
		}

		this_ndx += 10;
	}
}

void drivePageCursor(Pagestruct* target_page, unsigned int measures){
	unsigned int n, i, res;

	for (n=0; n < measures; n++) { // each measure
		res = (n == (measures -1)) ? 179 : 192;
		for (i=0; i < res; i++) { // 192 / 12 = 16 = sequencer step resolution (12 ticks per step)

			PLAYER_play_page(target_page, (i % 12) + 1);
		}
	}
}

// Send an ALL NOTES OFF message on all channels
void send_ALL_NOTES_OFF(){

	unsigned char channel = 0;

	// Send on all 32 channels. Make sure to count from 1!
	for ( channel=1; channel <= 32; channel++ ){

		MIDI_send( MIDI_CC, channel, 123, 0 );
	} // channel iterator
}


// Send an ALL CONTROLLERS OFF message on all channels
void send_RESET_ALL_CONTROLLERS(){

	unsigned char channel = 0;

	// Send on all 32 channels. Make sure to count from 1!
	for ( channel=1; channel <= 32; channel++ ){

		MIDI_send( MIDI_CC, channel, 121, 0 );
	} // channel iterator
}


// STOPs the sequencer bringing it in start position
void sequencer_STOP( bool midi_send_stop ) {

	// Remember the initial G_run_bit status to send all notes off accordingly.
	unsigned char initial_G_run_bit = G_run_bit;
	#ifdef FEATURE_ENABLE_SONG_UPE
	unsigned char force_stop = force_stop_bit;
	unsigned char stop = G_stop_bit;
	G_stop_bit = OFF;
	#endif
	// Halt the sequencer - turns G_run_bit OFF
	sequencer_HALT();

	// MIDI CLOCK: STOP
	if (	( G_clock_source == INT )
			||	(	( G_clock_source == EXT )
				&&	MIDICLOCK_PASSTHROUGH == TRUE )
		){

		if ( midi_send_stop ) {
			// Only active when the clock is active or machine is slave
			MIDI_send( MIDI_CLOCK, MIDICLOCK_STOP, 0, 0 );
		}
		// If Stop was pressed in Stop state
		if ( initial_G_run_bit == OFF ){

			// Send the ALL NOTES OFF message
			send_ALL_NOTES_OFF();

			// Possibly: send the ALL CONTROLLERS OFF message
			// send_RESET_ALL_CONTROLLERS();
		}
	}

	// Reset the locators
	#ifdef FEATURE_ENABLE_SONG_UPE
	sequencer_RESET(force_stop);
	#else
	sequencer_RESET(OFF);
	#endif
	// Reset the G_MIDI_timestamp;
	G_MIDI_timestamp = 0;

	cpu_load_reset( G_TIMER_REFILL, G_TIMER_REFILL );
}


// Just a wrapper to stay call-name consistent
void sequencer_command_STOP(){

	// DOUBLE CLICK SCENARIO
	if (	( DOUBLE_CLICK_TARGET == KEY_STOP )
		&& 	( DOUBLE_CLICK_TIMER   > DOUBLE_CLICK_ALARM_SENSITIVITY ) ) {

		// Double click code
		// ...
//		G_measure_indicator_value = 0;
//		G_measure_indicator_part = 0;
//		prev_G_pause_bit = OFF;
		if ( SEQUENCER_JUST_RESTARTED == ON || G_pause_bit == ON ) // double click at startup or in grid scroll (paused) view
		{
//			G_measure_locator = 0;
			align_measure_locators(); // mute all pages in the grid
//			unsigned int	row=0;
//			unsigned int 	i=0;
//			Trackstruct* target_track = NULL;
//			for (i=0; i <= GRID_NROF_BANKS; i++) { // for each grid bank
//				for ( row=0; row < MATRIX_NROF_ROWS; row ++ ){
//					if ( (GRID_bank_playmodes & ( 1 << i )) == 0 ) continue;
//					if ( GRID_p_selection[i] != NULL ){
//						target_track = GRID_p_selection[i]->Track[row];
//						send_track_program_change( target_track, GRID_p_selection[i] );
//					}
//				}
//			}
		}

	} // end of double click scenario

	// SINGLE CLICK SCENARIO
	else if (DOUBLE_CLICK_TARGET == 0) {

		if ( G_save_song_pos == ON ) // the stop key was pressed rather than a function call
		{
			DOUBLE_CLICK_TARGET = KEY_STOP;
			DOUBLE_CLICK_TIMER = ON;
			// Start the Double click Alarm
			cyg_alarm_initialize(
					doubleClickAlarm_hdl,
					cyg_current_time() + DOUBLE_CLICK_ALARM_TIME,
					DOUBLE_CLICK_ALARM_TIME );
		}

		// Single click code
		sequencer_STOP( true );
#ifdef FEATURE_ENABLE_SONG_UPE
		if ( G_pause_bit == ON ) // in grid scroll (paused) view
		{
			RESET_measure_locator(ON);
		}
#endif
	}
}


// Toggles the pause state of the sequencer
#ifdef FEATURE_ENABLE_SONG_UPE
void sequencer_command_PAUSE(unsigned char measure_scrolling){

	if ( measure_scrolling && G_run_bit == OFF ) {

		sequencer_HALT();
		return;
	}
#else
void sequencer_command_PAUSE(){
#endif
	switch (G_run_bit) {

		case ON:

			sequencer_HALT();

			// MIDI CLOCK: STOP
			if (	( G_clock_source == INT )
					||	(	( G_clock_source == EXT )
						&&	MIDICLOCK_PASSTHROUGH == TRUE )
				){

				// Only active when the clock is active or machine is slave
				MIDI_send( MIDI_CLOCK, MIDICLOCK_STOP, 0, 0 );
			}
			break;

		case OFF:

			// Prevent from acting when not in pause mode
			if( G_pause_bit == ON ){

				sequencer_UNHALT();

				// MIDI CLOCK: CONTINUE
				if ( G_clock_source == INT ){

					// Only active when the clock is active
					MIDI_send( MIDI_CLOCK, MIDICLOCK_CONTINUE, 0, 0 );
				}
			}
			break;
	} // G_run_bit SWITCH
}



// Activate the sequencer play as needed
void sequencer_command_PLAY(){

	// React only if the sequencer is stopped
	switch( G_run_bit ){

		// Sequencer is IDLE
		case OFF:
			switch( G_pause_bit ){

				// Sequencer is PAUSED
				case ON:
					// Stop the sequencer - clear anything left in the pipe to start fresh (do not send MIDICLOCK_STOP fallthrough to sequencer_START)
					#ifdef FEATURE_ENABLE_SONG_UPE
					sequencer_UNHALT();
					#else
					sequencer_STOP( false );
					#endif

					// no BREAK - fallthrough intended - recovering from pause state

				// Sequencer is STOPPED
					/* no break */
				case OFF:
					// Start the sequencer
					sequencer_START();
					break;
			}
			break;

		// Sequencer is ACTIVE
		case ON:
			// Nothing to do
			break;

	}// switch( G_run_bit )

}




// Evaluate the selection status of the target note and return it.
// It can be any of LEAD, ON, OFF
unsigned char how_selected_in_current_scale( 	Pagestruct* target_page,
												unsigned char target_note ){

	unsigned char result = 0;
	// This implies the bit shift pattern defined as in the external table.
	// ..Book page 116.ff

	//d_iag_printf( "target note:%d vs. scale\n", target_note );
	//my_print_bits32( 1 << (target_note - 12) );
	//my_print_bits32( target_page->current_scale );

	// Note is selected;
	if ( target_page-> scaleNotes[G_scale_ndx] & (1 << (target_note - 12)) ){

		// When selected it also could be lead, check condition
		if ( target_page-> scaleLead[G_scale_ndx] & (1 << (target_note - 12)) ){

			// LEAD has the highest importance as it implies the rest
			result = LEAD;
		}
		else {

			result = ON;
		}
	}

	else {

		// Note is not selected
		result = OFF;
	}

	// d_iag_printf( "returning result:%d\n", result );
	return result;
}


// Evaluate the selection status of the target note and return it.
// It can be any of LEAD, ON, OFF
unsigned char NEMO_how_selected_in_scale( 	Pagestruct* target_page,
											unsigned char target_note,
											unsigned char scale_ndx ){

	// This implies the bit shift pattern defined as in the external table.
	// ..Book page 116.ff

	// Note is selected;
	if ( target_page-> scaleNotes[scale_ndx] & (1 << (target_note - 12)) ){

		// When selected it also could be lead, check condition
		if ( target_page-> scaleLead[scale_ndx] & (1 << (target_note - 12)) ){

			// LEAD has the highest importance as it implies the rest
			return LEAD;
		}
		else {

			return ON;
		}
	}

	else {

		// Note is not selected
		return OFF;
	}
}

// Evaluate the selection status of the target note and return it.
// It can be any of LEAD, ON, OFF
unsigned char how_selected_in_scale( 	Pagestruct* target_page,
										unsigned char target_note,
										unsigned char G_scale_ndx ){

	// This implies the bit shift pattern defined as in the external table.
	// ..Book page 116.ff

	// Note is selected;
	if ( target_page-> scaleNotes[G_scale_ndx] & (1 << (target_note - 12)) ){

		// When selected it also could be lead, check condition
		if ( target_page-> scaleLead[G_scale_ndx] & (1 << (target_note - 12)) ){

			// LEAD has the highest importance as it implies the rest
			return LEAD;
		}
		else {

			return ON;
		}
	}

	else {

		// Note is not selected
		return OFF;
	}
}



// Select a note in the scale selection according to the status passed along.
// Applies to target_page directly. The status can be ON, LEAD, OFF
// Remember that LEAD means this is the LEAD tone of the scale
// Automation can be done here to several levels and quality.
void select_in_scale( 	Pagestruct* target_page,
						unsigned char target_note,
						unsigned char target_state,
						unsigned char scale_ndx ){

	// Syntactic sugar: new_flag has to be at least 12 bits long!
	unsigned short new_flag 	= 	1 << (target_note - 12);

	switch( target_state ){

		case LEAD:
			// Set the LEAD flag and NOTE flag - LEAD always in scale
			target_page-> scaleLead[ scale_ndx ] 	= 	new_flag;
			target_page-> scaleNotes[ scale_ndx ] 	|= 	new_flag;
			break;

		case ON:
			// Set the NOTE flag - add note to the scale
			target_page-> scaleNotes[ scale_ndx ] 	|= 	new_flag;
			break;

		case OFF:
			// OFF: Deselect both NOTE and LEAD flag
			target_page-> scaleNotes[ scale_ndx ] 	&= 	( 0xFFF ^ new_flag );
			target_page-> scaleLead[ scale_ndx ] 	&= 	( 0xFFF ^ new_flag );
			break;
	}
}



//
// Convert the index of a key in the Octave circle to a key Ndx to be used
// in further operations
unsigned char keyNdx_to_noteNdx( unsigned int keyNdx ){

	unsigned char i = 0;
	// Transform keyNdx to note index
	switch (keyNdx) {
		case KEY_NOTE_C:	i = NOTE_C;			break;
		case KEY_NOTE_Cis:	i = NOTE_Cis;		break;
		case KEY_NOTE_D:	i = NOTE_D;			break;
		case KEY_NOTE_Dis:	i = NOTE_Dis;		break;
		case KEY_NOTE_E:	i = NOTE_E;			break;
		case KEY_NOTE_F:	i = NOTE_F;			break;
		case KEY_NOTE_Fis:	i = NOTE_Fis;		break;
		case KEY_NOTE_G:	i = NOTE_G;			break;
		case KEY_NOTE_Gis:	i = NOTE_Gis;		break;
		case KEY_NOTE_A:	i = NOTE_A;			break;
		case KEY_NOTE_Ais:	i = NOTE_Ais;		break;
		case KEY_NOTE_B:	i = NOTE_B;			break;
		case KEY_NOTE_Cup:	i = NOTE_C;			break;
	}
	return i;
}



unsigned char keyNdx_to_ndx( unsigned int keyNdx ){

	unsigned char i = 0;
	// Transform keyNdx to note index
	switch (keyNdx) {
		case KEY_NOTE_C:	i = 0;			break;
		case KEY_NOTE_Cis:	i = 1;			break;
		case KEY_NOTE_D:	i = 2;			break;
		case KEY_NOTE_Dis:	i = 3;			break;
		case KEY_NOTE_E:	i = 4;			break;
		case KEY_NOTE_F:	i = 5;			break;
		case KEY_NOTE_Fis:	i = 6;			break;
		case KEY_NOTE_G:	i = 7;			break;
		case KEY_NOTE_Gis:	i = 8;			break;
		case KEY_NOTE_A:	i = 9;			break;
		case KEY_NOTE_Ais:	i = 10;			break;
		case KEY_NOTE_B:	i = 11;			break;
		case KEY_NOTE_Cup:	i = NOP;		break;
	}
	return i;
}



// Convert a pitch value to a key Ndx to be used in further operations
unsigned char pitch_to_noteNdx( unsigned char in_pitch ){

	unsigned char i = 0;

	// Transform keyNdx to note index
	switch ( in_pitch % 12 ) {

		case 0:		i = NOTE_C;			break;
		case 1:		i = NOTE_Cis;		break;
		case 2:		i = NOTE_D;			break;
		case 3:		i = NOTE_Dis;		break;
		case 4:		i = NOTE_E;			break;
		case 5:		i = NOTE_F;			break;
		case 6:		i = NOTE_Fis;		break;
		case 7:		i = NOTE_G;			break;
		case 8:		i = NOTE_Gis;		break;
		case 9:		i = NOTE_A;			break;
		case 10:	i = NOTE_Ais;		break;
		case 11:	i = NOTE_B;			break;
	}

	return i;
}


// Extracts the scale bitpattern from the tracks of the current page
unsigned short extract_current_scale( Pagestruct* target_page ){

	unsigned char i = 0;
	unsigned char pitch = 0;
	unsigned short result = 0;

	// Build the scale - track by track
	for ( i=0; i<MATRIX_NROF_ROWS; i++ ){

		if ( target_page->scaleStatus == OFF ){

			pitch = target_page->Track[i]->attr_PIT ;
		}
		else {
			pitch =
//					target_page->Track[i]->attr_PIT +
					target_page->Track[i]->scale_pitch_offset +
					target_page->Track[i]->lead_pitch_offset ;
		}

		// Mark the note in the scale selection. This is the first 12 bits of the field.
//		pitch =
//					target_page->Track[i]->attr_PIT +
//					target_page->Track[i]->scale_pitch_offset +
//					target_page->Track[i]->lead_pitch_offset ;

		result |=  1 << (11 - ( pitch % 12 ));

		// if ( target_page->pageNdx == 0 ){
		// d_iag_printf( "i:%d extracted:%d scaleNotes:\n", i, pitch );
		// my_print_bits32( target_page-> scaleNotes[G_scale_ndx] );
		// }
	}

	// d_iag_printf( "current scale extracted\n" );

	return result;
}



//
// Extracts the current pitches of the page into the scale selection variable
// to be used and changed in the scale mode. It only works with track pitch offset values.
// Steps may have additional pitch offsets which are not considered at the moment.
//
// The result will be as follows: lowest absolute note found makes up the lead;
// The other notes will be added to the lead scale as if they were in the same octave,
// hence disregarding any octave trans-position of the tracks.
//
void extract_scale_and_lead( Pagestruct* target_page ){

	unsigned char 	i = 0,
					j = 127;

	signed char pitch = 0;


	if ( target_page->force_to_scale == ON ){

//		target_page-> scaleNotes[G_scale_ndx] = target_page->current_scale;
		target_page-> scaleLead[G_scale_ndx] = target_page->scaleLead_old;
		return;
	}

	// Clear the scaleNotes to start with..
	target_page-> scaleNotes[G_scale_ndx] = 0;

	// Find the lowest absolute note value. This is the lead.
	for (i=0; i<MATRIX_NROF_ROWS; i++) {

		pitch = 	target_page->Track[i]->attr_PIT
					+
					target_page->Track[i]->scale_pitch_offset +
					target_page->Track[i]->lead_pitch_offset ;

		if ( pitch < j ) {

			// Always remember the lowest encountered value
			j = pitch;
		}
	}

//	 if ( target_page->pageNdx == 0){
//		d_iag_printf( "*extract_scale: lowest pitch:%d\n", j );
//	 }

	// Keep the lead
	target_page-> scaleLead[G_scale_ndx] = 1 << ( 11 - (j%12) );

	// Set it as the scaleLead_old as well, and remember the scale
	target_page->scaleLead_old = target_page-> scaleLead[G_scale_ndx];
	target_page->scaleNotes_old = target_page-> scaleNotes[G_scale_ndx];

	// Build the scale - track by track
	for ( i=0; i<MATRIX_NROF_ROWS; i++ ){

		// Mark the note in the scale selection. This is the first 12 bits of the field.
		pitch =
					target_page->Track[i]->attr_PIT +
					target_page->Track[i]->scale_pitch_offset +
					target_page->Track[i]->lead_pitch_offset ;

		target_page-> scaleNotes[G_scale_ndx] |=  1 << (11 - ( pitch % 12 ));

		// if ( target_page->pageNdx == 0 ){
		// d_iag_printf( "i:%d extracted:%d scaleNotes:\n", i, pitch );
		// my_print_bits32( target_page-> scaleNotes[G_scale_ndx] );
		// }
	}

		// if ( target_page->pageNdx == 0 ){
		// d_iag_printf( "*extract_scale: scale/lead/_old\n" );
		// my_print_bits32( target_page-> scaleNotes[G_scale_ndx] );
		// my_print_bits32( target_page-> scaleLead[G_scale_ndx] );
		// my_print_bits32( target_page->scaleLead_old );
		// }
}



//
// Advances a bit in the given direction over an interval of 12
// This can be made more universal, but not right this moment please.
//
unsigned int advance_scale_search_bit( 	unsigned int search_bit,
										unsigned char direction ){

	unsigned int output = 0;

	switch( direction ){

		case INC:
			if (search_bit == 1) {
				// Wrap condition: start over on the left side
				output = (1 << 11);
			}
			else {
				// Advance the search bit towards right
				output = search_bit >> 1;
			}
			break;

		case DEC:
			if (search_bit == (1 << 11)) {
				// Wrap condition: start over on the right side
				output = 1;
			}
			else{
				// Advance search bit towards left
				output = search_bit << 1;
			}
			break;
	}

	return output;
}



// Used to compute step chord aux distance
unsigned int advance_chord_search_bit( 	unsigned int search_bit,
										unsigned char direction ){

	unsigned int output = 0;

	switch( direction ){

		case DEC:
			if (search_bit == 1) {
				// Wrap condition: start over on the left side
				output = (1 << 11);
			}
			else {
				// Advance the search bit towards right
				output = search_bit >> 1;
			}
			break;

		case INC:
			if (search_bit == (1 << 11)) {
				// Wrap condition: start over on the right side
				output = 1;
			}
			else{
				// Advance search bit towards left
				output = search_bit << 1;
			}
			break;
	}

	return output;
}




// In target page, modify the scale_pitch_offset of tracks
// ..according to the passed parameters.
void shift_track_pitches( 	Pagestruct* target_page,
							unsigned char degrees,
							unsigned char direction ){

	// Holds the bit indicating the current track pitch
	unsigned int track_pitch_bit = 0;
	unsigned int search_bit		= 0;
	signed char offset	= 0;
	unsigned char
					d = 0,					// keeps tracks of found degrees
					i = 0,					// counter variable
					row = 0;				// row iterator


	// d_iag_printf( "      *shifting Pitches: deg:%d dir:%d\n", degrees, direction);

	// Iterate through the tracks
	for (row=0; row<MATRIX_NROF_ROWS; row++) {

		#ifdef FEATURE_SOLO_REC
		if ( SOLO_rec_transpose == ON && SOLO_transpose_row != row ){
			continue;
		}
		#endif

		// Find out the pitch of the track relative to the scale..
		track_pitch_bit =
			1 << (11 - ( (
//								target_page->Track[row]->attr_PIT
//							+
								target_page->Track[row]->scale_pitch_offset
							+	target_page->Track[row]->lead_pitch_offset
							)

							% 12 )   ) ;

		// Starting position of the degree counter (d)
		search_bit = track_pitch_bit;

		// Iterate through the scale with cardinality to compute offset
		d = 0;

		// Compute the required pitch offset
		// Iterate through the scale. Max 12 degrees.
		for (i=0; i<12; i++) {

			// Number of degrees has not been reached yet
			if ( d < degrees ) {

				// Advance the search bit
				search_bit = advance_scale_search_bit( search_bit, direction );

				// Increment or decrement the offset
				offset ++;
				// if 		(direction == INC) 	{	offset ++; }
				// else if (direction == DEC) 	{	offset --; }

				// If degree found, also increment d
				// if ( ((target_page->scaleSelection >> 12) & search_bit) != 0 ){
				if ( (target_page-> scaleNotes[G_scale_ndx] & search_bit) != 0 ) {

					// d_iag_printf( "      degree %d found at off:%d\n", d, offset);
					d ++;
				}
			}
			else {

				// Nr. of needed degrees found, so exit
				break;
			}
		} // Scale selection iterator

		// if (direction == INC){   leave the offset as is  }
		if (direction == DEC) 	{	offset = -offset; 		}

		// d_iag_printf( "        tr:%d exit with offset:%d\n", row, offset);

		// Apply the offset to the track pitch offset variable
		target_page->Track[row]->lead_pitch_offset += offset;

		// Reset the offset variable
		offset = 0;

	} // Track iterator
}



// Take a pitch and return the positive offset to next degree in the scale passed along
unsigned char offset_to_chord_aux( unsigned short in_scale, unsigned char degree ){

	unsigned char 	i = 0;
	unsigned char 	offset = 0;
	unsigned char 	bits_seen = 0;

	// Iterate through the 12 half step scale and keep count..
	for ( i=0; i < 12; i++ ){

		// If we find a set bit, check if we are done
		if ( ( (1<<i) & in_scale) != 0 ){

			if ( bits_seen == degree ){
				i=12;
			}
			bits_seen ++;
		}

 		// Count the half step offset
		offset ++;
	}

	// The obvious
	return offset;
}



// Take a pitch and return the positive offset to the next degree in the scale passed along
unsigned char offset_to_next_chord_aux(		unsigned short in_scale,
											unsigned char  pitch ){

	unsigned char 	i = 0;
	unsigned char 	offset = 0;
	unsigned int 	pitch_bit = 1 << ((pitch % 12));

	// Iterate through the 12 half step scale
	for (i=0; i<12; i++) {

		// Increment the pitch_bit position first.
		pitch_bit = advance_chord_search_bit( pitch_bit, INC );

 		// Count the half step offset
		offset ++;
		//	my_print_bits32( pitch_bit );

		// If the next degree has been reached, break the loop
		if ( (pitch_bit & in_scale) != 0 ){
			break;
		}
	}

	// The obvious
	return offset;
}



// Take a pitch and return the positive offset to the next degree in the scale of its page
unsigned char offset_to_next_current_degree(	Pagestruct* target_page,
												unsigned char pitch ){

	unsigned char
					i = 0,
					offset = 0;
	unsigned int
					pitch_bit = 1 << (11 - (pitch % 12));

//		d_iag_printf( "   off_2next: Scale + pitch_bit(pi:%d):\n", pitch );
//		my_print_bits32( target_page->scaleNotes );

	// Iterate through the 12 half step scale
	for (i=0; i<12; i++) {

		// Increment the pitch_bit position first.
		pitch_bit = advance_scale_search_bit( pitch_bit, INC );

 		// Count the half step offset
		offset ++;

//		my_print_bits32( pitch_bit );

		// If the next degree has been reached, break the loop
//		if ( (pitch_bit & target_page->current_scale) != 0 ){
		if ( (pitch_bit & target_page-> scaleNotes[G_scale_ndx]) != 0 ){

			// Found the next degree
			break;
		}
	}

//	d_iag_printf( "   returning offset_:%d\n", offset );

	// The obvious
	return offset;
}




// Take a pitch and return the positive offset to the next degree in the scale of its page
unsigned char offset_to_next_degree(	Pagestruct* target_page,
										unsigned char pitch ){

	unsigned char
					i = 0,
					offset = 0;
	unsigned int
					pitch_bit = 1 << (11 - (pitch % 12));

//		d_iag_printf( "   off_2next: Scale + pitch_bit(pi:%d):\n", pitch );
//		my_print_bits32( target_page->scaleNotes );

	// Iterate through the 12 half step scale
	for (i=0; i<12; i++) {

		// Increment the pitch_bit position first.
		pitch_bit = advance_scale_search_bit( pitch_bit, INC );

 		// Count the half step offset
		offset ++;

//		my_print_bits32( pitch_bit );

		// If the next degree has been reached, break the loop
		if ( (pitch_bit & target_page-> scaleNotes[G_scale_ndx]) != 0 ){

			// Found the next degree
			break;
		}
	}

//	d_iag_printf( "   returning offset_:%d\n", offset );

	// The obvious
	return offset;
}




// Modifies the track lead offset according to the new lead,
// without modifying the pitch intervals.
void export_scale_lead ( Pagestruct* target_page ){

	unsigned char
					direction 	= 0,
					newLeadNdx 	= 0,
					row = 0,
					oldLeadNdx 	= 0,
					degrees 	= 0;

	unsigned int 	bitmask 	= 0;

	// Start over with a fresh old scale lead which goes with
	// the cleared lead pitch offsets..
	target_page->scaleLead_old = 1 << 11;

	// Index of newLead
	newLeadNdx = my_bit2ndx( target_page-> scaleLead[G_scale_ndx] );

	// Index of oldLead
	oldLeadNdx = my_bit2ndx( target_page->scaleLead_old );

	// Clear the lead offsets in tracks
	for (row=0; row<MATRIX_NROF_ROWS; row++) {

		#ifdef FEATURE_SOLO_REC
		if ( SOLO_rec_transpose == ON && SOLO_transpose_row != row ){
			continue;
		}
		#endif

		target_page->Track[row]->lead_pitch_offset = 0;
		// d_iag_printf( "%d  ", target_page->Track[row]->lead_pitch_offset );
	}

	// d_iag_printf( "   *export_scale_LEAD: NL=%d OL=%d\n", newLeadNdx, oldLeadNdx);

	// my_print_bits32( target_page-> scaleLead[G_scale_ndx] );
	// my_print_bits32( target_page->scaleLead_old );

	// Generate the bitmask of the interval in the scale
	bitmask = (0xfff >> (11 - newLeadNdx)) ^ (0xfff >> (11 - oldLeadNdx)) ;

	// Produce the scale selection and its cardinality
	degrees = my_bit_cardinality( bitmask & target_page-> scaleNotes[G_scale_ndx] );

	// Find trans-position direction. Somewhat twisted, oh well.. it possibly works.
	// Notice: the == case missing and does not make any changes here
	if 		(newLeadNdx < oldLeadNdx) 		{	direction = INC;	}
	else if	(newLeadNdx > oldLeadNdx)		{	direction = DEC;	}


//		d_iag_printf( "  pitching for SCALE LEAD. offsets before:\n" );
//		d_iag_printf( "track: scale_pitch_offsets INITIAL:\n" );
//		for (row=0; row<MATRIX_NROF_ROWS; row++) {
//			d_iag_printf( "%d  ", target_page->Track[row]->scale_pitch_offset );
//		}
//		d_iag_printf( "\n" );
//		for (row=0; row<MATRIX_NROF_ROWS; row++) {
//			d_iag_printf( "%d  ", target_page->Track[row]->lead_pitch_offset );
//		}
//		d_iag_printf( "\n" );
//		for (row=0; row<MATRIX_NROF_ROWS; row++) {
//			d_iag_printf( "%d  ", target_page->Track[row]->scale_pitch_offset +
//			                     target_page->Track[row]->lead_pitch_offset );
//		}
//		d_iag_printf( "\n" );
//		d_iag_printf( "    nL:%d oL:%d bm:%d deg:%d dir:%d\n", newLeadNdx, oldLeadNdx, bitmask, degrees, direction);
//		d_iag_printf( "\n" );

	// Now apply the pitch shift to the tracks - this has an effect if k>0
	shift_track_pitches( target_page, degrees, direction );


	// d_iag_printf( "lead shift- i:%d j:%d dir:%d k:%d bitmask:\n", i, j, direction, k );
	// my_print_bits32( bitmask );
	// ..so far so good;


//		d_iag_printf( "*export_scale_LEAD - current offsets (scale/lead/sum):\n" );
//		d_iag_printf( "track: scale_pitch_offsets SHIFTED:\n" );
//		for (row=0; row<MATRIX_NROF_ROWS; row++) {
//			d_iag_printf( "%d  ", target_page->Track[row]->scale_pitch_offset );
//		}
//		d_iag_printf( "\n" );
//		for (row=0; row<MATRIX_NROF_ROWS; row++) {
//			d_iag_printf( "%d  ", target_page->Track[row]->lead_pitch_offset );
//		}
//		d_iag_printf( "\n" );
//		for (row=0; row<MATRIX_NROF_ROWS; row++) {
//			d_iag_printf( "%d  ", target_page->Track[row]->scale_pitch_offset +
//			                     target_page->Track[row]->lead_pitch_offset );
//		}
//		d_iag_printf( "\n" );
//		d_iag_printf( "   exScLd- nL:%d oL:%d bm:%d deg:%d dir:%d\n",
//				newLeadNdx, oldLeadNdx, bitmask, degrees, direction);

	// Since the shift has taken place, no need to remember the old lead anymore
	target_page->scaleLead_old = target_page-> scaleLead[G_scale_ndx];
	target_page->scaleNotes_old = target_page-> scaleNotes[G_scale_ndx];
}


// Returns pitch in halfsteps of the given degree in the scaleSelection
unsigned char pitch_of( 	unsigned char degree,
							unsigned int scaleSelection ){

	unsigned char
					half_steps = 0,
					i = 0;

//	 d_iag_printf( "    --pit of deg:%d in:%d\n", degree, scaleSelection );
//	 my_print_bits32( scaleSelection );

	// Offset needed because of the -- condition.
	// Mechanism may be revised later.
	degree++;

	// Iterate through the scaleSelection counting half steps and degrees
	for (i=0; i<12; i++){

		// Count the degrees down when one found
		if ( (1 << (11-i)) & (scaleSelection) ) {

			// d_iag_printf( "    --degree found at i:%d\n", i );

			// Degree count down
			degree--;

//			d_iag_printf( "        --degree:%d\n", degree );

		}

		// Break when degree counter is used up
		if (degree == 0) {
			break;
		}

		// Count the half steps on every iteration
		half_steps ++;
	}

//	 d_iag_printf( "    --half_steps:%d\n", half_steps );

	return half_steps;
}




// Applies the selected scale degrees to the page track pitches.
// Projects the selected scale pitches onto the track pitches.
// Called after a degree has been added to or removed from the scale.
// This makes a direct assignment of pitches to tracks, no more lower first etc..
void export_scale_degrees( Pagestruct* target_page ){

	unsigned char
					scaleNdx 	= 0,	// points to scale step being processed
					j 			= 127,
					lowPiNdx 	= 0,
					carry 		= 0;	// indicates nr of scales passed so far

	signed char 	row			= 0;
	unsigned int	track_visited = 0x3ff;	// Stores the bits corresponding to the tracks to be visited

	// d_iag_printf( "*export_scale_degrees()\n" );

	// Start processing at the lowest (natural) note in scale, then apply later the scale lead offset.
	scaleNdx = 0;

	// Iterate through the tracks of the page and assign pitch by pith to the tracks
	for ( row = 9; row >= 0; row-- ) {

		#ifdef FEATURE_SOLO_REC
		if ( SOLO_rec_transpose == ON && SOLO_transpose_row != row ){
			continue;
		}
		#endif

//		 //d_iag_printf( "*export_scale_degrees: scale/Ndx:%d\n", scaleNdx );
//		 //my_print_bits32( target_page->scaleNotes );

		// This used to be a more sophisticated assignment logic, but KISS
		lowPiNdx = row;

		// When a scale has been completed, the carry gets increased..
		// ..so that pitch shifts carry to the next octave
		carry = 	( 10 - my_bit_cardinality( track_visited ) )
				/	(  0 + my_bit_cardinality( target_page-> scaleNotes[G_scale_ndx] ) );

		// Assign the lowest scale degree to the lowest pitched track.
		target_page->Track[lowPiNdx]->scale_pitch_offset =
				  pitch_of( scaleNdx, target_page-> scaleNotes[G_scale_ndx] )
				+ (12 * carry)
				;

		// Addendum to the assignment: if the scale consists of only one note,
		if ( my_bit_cardinality( target_page-> scaleNotes[G_scale_ndx] ) == 1 ){

			// then assign that pitch to all tracks - do not transpose away on one note..
			target_page->Track[lowPiNdx]->scale_pitch_offset =
						pitch_of( scaleNdx, target_page-> scaleNotes[G_scale_ndx] );
		}

		// d_iag_printf( "   --new pitchoffset:%d\n", target_page->Track[lowPiNdx]->scale_pitch_offset);

		// Advance the scale ndx, according to the length of the selected scale
		scaleNdx = (scaleNdx + 1) % (my_bit_cardinality( target_page-> scaleNotes[G_scale_ndx] ));

		// Mark track visited
		track_visited ^= (1 << lowPiNdx);

		// Re-init value of j
		j = 127;

	} // iterator through the rows of the page

}





// Imports the contents of a set into the preselection
void import_GRID_set( unsigned int source_set ){

	// Import the contents of the required scene into the grid preselection.
	unsigned char i=0;

	for (i=0; i < GRID_NROF_BANKS; i++){

		// Copy GRID Selection contents to the target scene
		GRID_p_clock_presel[i] = GRID_p_set[source_set][i];

		// Affect the choice immediately
		if ( GRID_switch_mode == GRID_SWITCH_DIRECT ){

			GRID_p_preselection[i] = GRID_p_set[source_set][i];
		}
	}

	if (G_run_bit == OFF){

		// Direct selection of the preselections
		select_page_preselections();
	}

	GRID_p_set_note_presel = source_set;
}



// Indicates whether a grid set is empty (FALSE) or has content (TRUE)
unsigned char get_content_GRID_set( unsigned char target_set ){

	unsigned char i = 0;
	unsigned char result = FALSE;

	for (i=0; i < GRID_NROF_BANKS; i++ ){

		// Show only when contained pages are not empty
		if ( 	( GRID_p_set[target_set][i] != NULL )
			&& 	( GRID_p_set[target_set][i]->page_clear == OFF )
			){

			result = TRUE;
			break;
		}
	}

	return result;
}


// Checks whether the target set is the same as what is in GRID
unsigned char is_actual_GRID( unsigned char target_set ){

	unsigned char i=0;
	unsigned char result=TRUE;

	for ( i=0; i < GRID_NROF_BANKS; i++){

		// Check if they are pointing to the same page
		if ( GRID_p_set[target_set][i] != GRID_p_preselection[i] ){

			result = FALSE;
			break;
		}
	}

	return result;
}


// Exports the current Grid selection into the given set
void export_GRID_set( unsigned int target_set ){

	unsigned char i=0;

	// d_iag_printf( "export_GRID_set():%d\n", target_set );

	for (i=0; i < GRID_NROF_BANKS; i++){

		// Copy GRID Selection contents to the target scene
		GRID_p_set[ target_set ][i] = GRID_p_preselection[i];

		// d_iag_printf( "i:%d ", GRID_set[ target_set ][i] );
	}
	// d_iag_printf( "\n" );
}

#ifdef FEATURE_ENABLE_SONG_UPE
void quick_assign_control_track ( Pagestruct* target_page, unsigned char trackIdx ) {
	if (trackIdx == 255) return;
	make_control_track(target_page, trackIdx);
	G_rec_ctrl_track = target_page->Track[trackIdx];
	G_rec_ctrl_page = target_page;
}


void make_control_track ( Pagestruct* target_page, unsigned char trackIdx ) {
	if (!Track_get_MISC(target_page->Track[trackIdx], CONTROL_BIT) ){
		// Clear all steps in the track
		Track_clear_steps( target_page, trackIdx );
	}
	// set the CONTROL_BIT
	target_page->Track[trackIdx]->attr_MISC = ON;
	target_page->Track[trackIdx]->attr_VEL = 0; // set the event offset
	target_page->Track[trackIdx]->STA_factor = 0; // set the target track offset
	GRID_editmode = OFF;
}


void copy_ctrl_track_to_step( Pagestruct* page, Trackstruct* track, Stepstruct* step )
{
	step->phrase_num = track->attr_VEL; // measure offset
	step->phrase_data = track->program_change;
	step->event_data = track->bank_change;
	step->attr_MCC = track->attr_MCH;
	step->attr_LEN = track->attr_DIR;
	step->attr_STA = track->STA_factor;
	step->attr_POS = track->attr_CCAMT;
	step->hyperTrack_ndx = track->attr_TEMPOMUL;
	step->attr_VEL = track->attr_TEMPOMUL_SKIP;
	step->attr_PIT = track->attr_MISC;
	step->attr_AMT = (page->trackMutepattern & (1<<row_of_track( page, track )));
	Step_set_status( step, STEPSTAT_SKIP, ON );
	track->attr_STATUS = 127;
}


void copy_ctrl_step_to_track( Pagestruct* page, Trackstruct* track, Stepstruct* step )
{
	track->attr_VEL = step->phrase_num; // measure offset
	track->program_change = step->phrase_data;
	track->bank_change = step->event_data;
	track->attr_MCH = step->attr_MCC;
	track->attr_DIR = step->attr_LEN;
	track->STA_factor = step->attr_STA;
	track->attr_CCAMT = step->attr_POS;
	track->attr_TEMPOMUL = step->hyperTrack_ndx;
	track->attr_TEMPOMUL_SKIP = step->attr_VEL;
	track->attr_MISC = step->attr_PIT;
	page->trackMutepattern & (step->attr_AMT<<row_of_track( page, track ));
}
#endif

// Interprets a key press dealing with changing the status of a step
void interpret_matrix_stepkey( 	unsigned char row,
								unsigned char col,
								Pagestruct* target_page ) {

	// Consolidate step pointer use
	Stepstruct* target_step = target_page->Step[row][col];

	// Absolute keyindex of the pressed button. Used to notice double click.
	unsigned int j = ((col+1) * 11) + (row);

	// Do not operate on step if step is hyper
	if ( target_step->hyperTrack_ndx != 10 ){
	//	return;
	}

	// Step status decides what to do
	switch( Step_get_status( target_step, STEPSTAT_TOGGLE ) ){


		// STEP TOGGLE is OFF
		case OFF:

			#ifdef FEATURE_SOLO_REC
			if ( G_zoom_level == zoomSOLOREC ){ // Steps can only be cleared by touch in SoloRec zoom
				// Turn the step simply ON
				Step_set_status( target_step, STEPSTAT_TOGGLE, ON );
				return;
			}
			#endif

			#ifdef FEATURE_ENABLE_SONG_UPE
			if (Track_get_MISC(target_page->Track[row], CONTROL_BIT) ){
				if ( Step_get_status( target_step, STEPSTAT_SKIP ) == ON ){
					// SKIP step
					Step_set_status( target_step, STEPSTAT_SKIP, OFF );
					target_page->Track[row]->attr_STATUS = 0;
					copy_ctrl_step_to_track(target_page, target_page->Track[row], target_step);
					Step_init(target_step);
				}
				else if (target_page->Track[row]->attr_STATUS == 0) {
					// unSKIP step
					Step_set_status( target_step, STEPSTAT_SKIP, ON );
					target_page->Track[row]->attr_STATUS = 127;
					copy_ctrl_track_to_step(target_page, target_page->Track[row], target_step);
				}
				break;
			}
			#endif
			// If STEP SKIPPED, unSKIP
			if ( Step_get_status( target_step, STEPSTAT_SKIP ) == ON ){

				// unSKIP step
				Step_set_status( target_step, STEPSTAT_SKIP, OFF );
			}

			// If MUT MASTER on, SKIP the step
			else if ( is_pressed_key( KEY_MUTE_MASTER ) == ON ){

				// SKIP the STEP
				Step_set_status( target_step, STEPSTAT_SKIP, ON );
			}

			// SEL modifier makes no sense if the step is OFF
			// None of the modifier buttons are pressed
			else if ( 	( is_pressed_key( KEY_MUTE_MASTER ) 	== OFF )
					&&	( is_pressed_key( KEY_SELECT_MASTER ) 	== OFF )
					&& 	( target_page->stepSelection == 0 )
					&&	( target_page->editorMode != PREVIEW )
					&&	( target_page->editorMode != PREVIEW_PERFORM )
				){

				// Turn the step simply ON
				Step_set_status( target_step, STEPSTAT_TOGGLE, ON );

				// DOUBLE CLICK was started in the ON toggle state section
				if (	( DOUBLE_CLICK_TARGET == j )
					&& 	( DOUBLE_CLICK_TIMER > DOUBLE_CLICK_ALARM_SENSITIVITY )
					){

					Step_zoom( target_page, row, col );
				}

				// Start thinking about double clicking
				else if ( DOUBLE_CLICK_TARGET == 0 ) {

					// Prepare double click timer for zooming into the step
					DOUBLE_CLICK_TARGET = j;
					DOUBLE_CLICK_TIMER = ON;

					// Start the Double click Alarm
					cyg_alarm_initialize(
						doubleClickAlarm_hdl,
						cyg_current_time() + DOUBLE_CLICK_ALARM_TIME,
						DOUBLE_CLICK_ALARM_TIME );
				}
			}
			break;



		// STEP TOGGLE is ON
		case ON:

			// SEL MODE: SEL master pressed, or selection active already..
			if 	(		( is_pressed_key( KEY_SELECT_MASTER ) == TRUE )
					||	( target_page->stepSelection > 0 )
				){

				// Remove any active track selection, so we can focus on the steps now
				if ( 	( target_page->trackSelection != 0 )
					&&	(	( G_zoom_level == zoomPAGE ) )
					){

					target_page->trackSelectionStored = target_page->trackSelection;
					target_page->trackSelection = 0;
				}

				// Toggle the step SELECT
				if (	( Step_get_status( target_step, STEPSTAT_SELECT ) == OFF )
					&&	(	( G_zoom_level == zoomPAGE )
						||	( G_zoom_level == zoomMAP ) )
					){

					Step_set_status( target_step, STEPSTAT_SELECT, ON );

					// Increment the counter of steps selected
					target_page->stepSelection ++;
				}
				else {

					Step_set_status( target_step, STEPSTAT_SELECT, OFF );

					// Increment the counter of steps selected
					target_page->stepSelection --;
				}
			}
			else if ( Step_get_status( target_step, STEPSTAT_SELECT) == ON) {

				// Un-SELECT step
				Step_set_status( target_step, STEPSTAT_SELECT, OFF );

				// Decrement the counter of steps selected
				target_page->stepSelection --;
			}

			// MUT MODE
			else if ( is_pressed_key( KEY_MUTE_MASTER ) == ON ){

				// SKIP or unskip Steps - toggle the SKIP state
				switch( Step_get_status( target_step, STEPSTAT_SKIP )){
				case ON:
					Step_set_status( target_step, STEPSTAT_SKIP, OFF  );
					break;
				case OFF:
					Step_set_status( target_step, STEPSTAT_SKIP, ON  );
					break;
				}
			}
			else if ( Step_get_status( target_step, STEPSTAT_SKIP) == ON ) {

				// Un-SKIP step
				Step_set_status( target_step, STEPSTAT_SKIP, OFF );
			}

			// STEP just ON, no SEL or MUT pressed, so turn it OFF
			else if (	( target_page->editorMode != PREVIEW )
					&&	( target_page->editorMode != PREVIEW_PERFORM )
					){

				// Turn the step simply OFF
				Step_set_status( target_step, STEPSTAT_TOGGLE, OFF );

				// DOUBLE CLICK was started in the ON toggle state section
				if (	( DOUBLE_CLICK_TARGET == j )
					&& 	( DOUBLE_CLICK_TIMER > DOUBLE_CLICK_ALARM_SENSITIVITY )
					){

					Step_zoom( target_page, row, col );
				}

				// Start thinking about double clicking
				else if ( DOUBLE_CLICK_TARGET == 0 ) {

					// Prepare double click timer for zooming into the step
					DOUBLE_CLICK_TARGET = j;
					DOUBLE_CLICK_TIMER = ON;

					// Start the Double click Alarm
					cyg_alarm_initialize(
						doubleClickAlarm_hdl,
						cyg_current_time() + DOUBLE_CLICK_ALARM_TIME,
						DOUBLE_CLICK_ALARM_TIME );
				}
			}

			// When there is only one step selected
			if ( target_page->stepSelection == 1 ){

				// Remember STEP COORDINATES
				target_page->stepSelectionSingleRow = row;
				target_page->stepSelectionSingleCol = col;
			}

			break;

	} // switch on step status
}



// Contains the solo operation for pages
void PAGE_toggle_solo(){

	unsigned char i=0, j=0;

	// Find out whether there is something in the solo buffer
	j = FALSE;
	for ( i=0; i < GRID_NROF_BANKS; i++ ){

		// Check content of current buffer position
		if ( (GRID_p_selection_buffer[i] != NULL) ) {

			j = TRUE;
		}
	}

	// Page solo condition: grid buffer is empty and page is active
	if ( ( j == FALSE ) ) {

		// Only active pages can be soloed. Break on inactive page.
		if (is_selected_in_GRID( &Page_repository[GRID_CURSOR] ) == OFF){

			return;
		}

		//d_iag_printf( "BUFFER EMPTY\n" );

		// Adjust the GRID selections: remember selections and preselection, then void
		for (i=0; i < GRID_NROF_BANKS; i++) {

			//d_iag_printf( "Row:%d   buffer:%d   sel:%d\n",
			//	i, GRID_selection_buffer[i], GRID_selection[i] );

			// Copy the original selection into the buffer
			GRID_p_selection_buffer[i] = GRID_p_selection[i];

			//d_iag_printf( "later:%d   buffer:%d   sel:%d   cursor:%d\n",
			//	i, GRID_selection_buffer[i], GRID_selection[i], GRID_CURSOR );

			// And mark it empty unless the page is under the cursor, or empty already
			if ( 	( GRID_p_selection[i]->pageNdx 	!= GRID_CURSOR 	)
				&&	( GRID_p_selection[i] 			!= NULL 		)
				){

				// Propagate the copy top down.
				// Copy the buffer into the clock preselection
				GRID_p_clock_presel[i] 			= NULL;

				if ( GRID_switch_mode == GRID_SWITCH_DIRECT ){

					// Clear grid selection and preselection accordingly
					GRID_p_preselection[i] 		= NULL;
					GRID_p_selection[i] 		= NULL;
					// Stop playing right now!
					#ifdef FEATURE_ENABLE_SONG_UPE
					stop_playing_page( GRID_p_selection[i], G_TTC_abs_value, OFF );
					#else
					stop_playing_page( GRID_p_selection[i], G_TTC_abs_value );
					#endif
					set_track_locators( GRID_p_selection[i], NULL, 0, 0 );
				}
			}
		}
	}

	// Page un-solo condition: GRID_selection_buffer[0] == GRID_SELECTION_BUFFER_EMPTY
	else {

		//d_iag_printf( "NEQ\n" );

		// Grid buffer is full, so transfer and empty
		for (i=0; i < GRID_NROF_BANKS; i++) {

			//d_iag_printf( "transfer+empty:%d\n", i );

			// Propagate the copy top down.
			// Copy the buffer into the clock selection
			GRID_p_clock_presel[i] 		= GRID_p_selection_buffer[i];

			if ( GRID_switch_mode == GRID_SWITCH_DIRECT ){

				GRID_p_preselection[i] 		= GRID_p_selection_buffer[i];
				GRID_p_selection[i] 		= GRID_p_selection_buffer[i];
			}

			// And clear the buffer as we go along
			GRID_p_selection_buffer[i] = NULL;
		}
	}
}


// Clear the target attribute of all tracks in page
void clear_tracks_by_ATR( Pagestruct* target_page, unsigned char target_attribute ){

	unsigned char row = 0;


	for ( row=0; row < MATRIX_NROF_ROWS; row++ ){

		// Clear the attribute offsets
		target_page->Track[row]->event_offset[target_attribute] = 0;

		switch( target_attribute ){

			case ATTR_VELOCITY:
				target_page->Track[row]->attr_VEL 	= TRACK_DEF_VELOCITY;
				break;

			case ATTR_PITCH:
				target_page->Track[row]->attr_PIT 		= TRACK_DEFAULT_PITCH[row];
				break;

			case ATTR_LENGTH:
				target_page->Track[row]->LEN_factor					= TRACK_DEF_LENFACTOR;
				break;

			case ATTR_START:
				target_page->Track[row]->STA_factor			 		= TRACK_DEF_STAFACTOR;
				break;

			case ATTR_POSITION:
				target_page->Track[row]->attr_POS 	= TRACK_DEF_POSITION;
				break;

			case ATTR_DIRECTION:
				target_page->Track[row]->attr_DIR 	= TRACK_DEF_DIRECTION;
				break;

			case ATTR_AMOUNT:
				target_page->Track[row]->attr_AMT 	= TRACK_DEF_AMOUNT;
				break;

			case ATTR_GROOVE:
				target_page->Track[row]->attr_GRV 	= TRACK_DEF_GROOVE;
				break;

			case ATTR_MIDICC:
				target_page->Track[row]->attr_MCC 	= TRACK_DEF_MIDICC;
				break;

			case ATTR_MIDICH:
				target_page->Track[row]->attr_MCH 	= TRACK_DEF_MIDICH;
				break;
		}
	}
}



// Randomize the target attribute of all tracks in page
void rand_tracks_by_ATR( Pagestruct* target_page, unsigned char target_attribute ){

	unsigned char row = 0;
	unsigned char min_val = 0;
	unsigned char max_val = 0;

	switch( target_attribute ){

		case ATTR_VELOCITY:
			min_val = TRACK_MIN_VELOCITY;
			max_val = TRACK_MAX_VELOCITY;
			break;

		case ATTR_PITCH:
			min_val = TRACK_MIN_PITCH;
			max_val = TRACK_MAX_PITCH;
			break;

		case ATTR_LENGTH:
			min_val = TRACK_MIN_LENFACTOR;
			max_val = TRACK_MAX_LENFACTOR;
			break;

		case ATTR_START:
			min_val = TRACK_MIN_STAFACTOR;
			max_val = TRACK_MAX_STAFACTOR;
			break;

		case ATTR_POSITION:
			min_val = TRACK_MIN_POSITION;
			max_val = TRACK_MAX_POSITION;
			break;

		case ATTR_DIRECTION:
			min_val = TRACK_MIN_DIRECTION;
			max_val = TRACK_MAX_DIRECTION;
			break;

		case ATTR_AMOUNT:
			min_val = TRACK_MIN_AMOUNT;
			max_val = TRACK_MAX_AMOUNT;
			break;

		case ATTR_GROOVE:
			min_val = TRACK_MIN_GROOVE;
			max_val = TRACK_MAX_GROOVE;
			break;

		case ATTR_MIDICC:
			min_val = TRACK_MIN_MIDICC;
			max_val = TRACK_MAX_MIDICC;
			break;

		case ATTR_MIDICH:
			min_val = TRACK_MIN_MIDICH;
			max_val = 32; // TRACK_MAX_MIDICH; // No virtual MIDI channels
			break;
	}

	// Randomize by amount the track values for the target attribute
	for ( row=0; row < MATRIX_NROF_ROWS; row++ ){

		// Clear the attribute offsets
		target_page->Track[row]->event_offset[target_attribute] = 0;

		switch( target_attribute ){

			case ATTR_START:
				target_page->Track[row]->STA_factor
						= 	(unsigned char)
							randomize_byamount(	target_page->Track[row]->STA_factor,
												target_page->Track[row]->attr_AMT
												+ target_page->Track[row]->event_offset[ATTR_AMOUNT]
												,
												min_val,
												max_val );
				break;

			case ATTR_LENGTH:
				target_page->Track[row]->LEN_factor
						= 	(unsigned char)
							randomize_byamount(	target_page->Track[row]->LEN_factor,
												target_page->Track[row]->attr_AMT
												+ target_page->Track[row]->event_offset[ATTR_AMOUNT]
												,
												min_val,
												max_val );
				break;

			default:

				*(ptr_of_track_attr_value( target_page, row, target_attribute ))
						= 	(unsigned char)
							randomize_byamount(	*(ptr_of_track_attr_value( target_page, row, target_attribute )),
												target_page->Track[row]->attr_AMT
												+ target_page->Track[row]->event_offset[ATTR_AMOUNT]
												,
												min_val,
												max_val );

//				target_page->Track[row]->attribute[target_attribute]
//						= 	(unsigned char)
//							randomize_byamount(	target_page->Track[row]->attribute[target_attribute],
//												target_page->Track[row]->attr_AMT
//												+ target_page->Track[row]->event_offset[ATTR_AMOUNT]
//												,
//												min_val,
//												max_val );
				break;
		} // switch( target_attribute )

	} // Row iterator

}


// Sorts the tracks of the given page in descending order (top to bottom) by target attribute
void sort_tracks_by_ATR( Pagestruct* target_page, unsigned char target_attribute ){

	unsigned char row = 0;
	unsigned char pass = 0;
	unsigned char top_ndx = 0;
	signed char top_val = -10;	// -10 accomodates the -1 from the MIDICC attribute
	unsigned short checklist = 0; // remember which tracks were visited

	// Basically copy the highest-valued track into the PAGE_assistant_page and back
	// d_iag_printf(" sorting ATR:%d\n", target_attribute );

	// Need to care about LEN and STA
	switch( target_attribute ){

		case ATTR_LENGTH:
			// Each pass corresponds to one track in target page
			for ( pass=0; pass < MATRIX_NROF_ROWS; pass++ ){

				// Find the index of the highest valued track
				for ( row=0; row < MATRIX_NROF_ROWS; row++ ){

					// Continue if the track was processed already
					if ( (checklist & (1<<row)) != 0 ){
						continue;
					}

					// Found a value higher than highest seen so far
					if ( ((signed char) target_page->Track[row]->LEN_factor) > top_val ){

						// Remember the value
						top_val = (signed char) target_page->Track[row]->LEN_factor;

						// Remember the index
						top_ndx = row;
					}
				} // Row iterator - making up one pass
				// d_iag_printf("top_val:%d at top_ndx:%d\n", top_val, top_ndx);

				// Now we know the index of the highest value, so copy the track
				Track_copy( target_page, top_ndx, PAGE_assistant_page, pass );

				// Mark the track processed - in the checklist
				checklist |= ( 1 << top_ndx );

				// Reset the top_val
				top_val = 0;
			} // pass iterator
			break;


		case ATTR_START:
			top_val = 0;
			// Each pass corresponds to one track in target page
			for ( pass=0; pass < MATRIX_NROF_ROWS; pass++ ){

				// Find the index of the highest valued track
				for ( row=0; row < MATRIX_NROF_ROWS; row++ ){

					// Continue if the track was processed already
					if ( (checklist & (1<<row)) != 0 ){
						continue;
					}

					// Found a value higher than highest seen so far
					if ( target_page->Track[row]->STA_factor > top_val ){

						// Remember the value
						top_val = target_page->Track[row]->STA_factor;

						// Remember the index
						top_ndx = row;
					}
				} // Row iterator - making up one pass

				// Now we know the index of the highest value, so copy the track
				Track_copy( target_page, top_ndx, PAGE_assistant_page, pass );

				// Mark the track processed - in the checklist
				checklist |= ( 1 << top_ndx );

				// Reset the top_val
				top_val = 0;
			} // pass iterator
			break;


		default:
			// Each pass corresponds to one track in target page
			for ( pass=0; pass < MATRIX_NROF_ROWS; pass++ ){

				// Find the index of the highest valued track
				for ( row=0; row < MATRIX_NROF_ROWS; row++ ){

					// Continue if the track was processed already
					if ( (checklist & (1<<row)) != 0 ){
						continue;
					}

					// Found a value higher than highest seen so far
					// if ( ((signed char) target_page->Track[row]->attribute[target_attribute]) > top_val ){
					if ( ( (signed char)*(ptr_of_track_attr_value( target_page, row, target_attribute )) ) > top_val ){

						// Remember the value
						// top_val = (signed char) target_page->Track[row]->attribute[target_attribute];
						top_val = (signed char)*(ptr_of_track_attr_value( target_page, row, target_attribute ));

						// Remember the index
						top_ndx = row;
					}
				} // Row iterator - making up one pass

				// Now we know the index of the highest value, so copy the track
				Track_copy( target_page, top_ndx, PAGE_assistant_page, pass );

				// Mark the track processed - in the checklist
				checklist |= ( 1 << top_ndx );

				// Reset the top_val
				top_val = -10;
			} // pass iterator

			break;
	}

	// Tracks sorted, now copy them back into the current page
	for ( row=0; row < MATRIX_NROF_ROWS; row++ ){

		// From PAGE_assistant_page to target_page
		Track_copy( PAGE_assistant_page, row, target_page, row );
	}

}


// Select all armed tracks with associated chained tracks as track selection
unsigned short page_get_armed_chain_selection( Pagestruct* target_page ) {

	unsigned short track_rec_pattern = Page_getTrackRecPattern(target_page);
	unsigned char i = 0, offset = 0, temp = 0;
	unsigned short track_selection = 0;
	Trackstruct* current_track = NULL;

	while( track_rec_pattern )
	{
		// Recording track index
		temp = my_bit2ndx( track_rec_pattern );

		// Starting track is the currently recording one
		current_track = target_page->Track[temp + offset];

		for ( i = 0; i < MATRIX_NROF_ROWS; i++ ){
			// Place current track into the selection
			track_selection |=
				( 1 << row_of_track( target_page, current_track ) );

			// Move the track pointer along the chain
			current_track = current_track->chain_data[NEXT];

			// End of chain
			if( i && current_track == target_page->Track[temp] ) {
				break;
			}
		}

		offset += temp + 1;
		track_rec_pattern >>= temp + 1;
	}

	return track_selection;
}


unsigned char is_matrix_key( unsigned char keyNdx ){
	return (( keyNdx >= 11 ) && ( keyNdx <= 185 ) && (((keyNdx-10) % 11 ) != 0 ));
}


#ifdef FEATURE_ENABLE_CHORD_OCTAVE
// Sets the the chord octave(s) mask of a step to the given octave
// chord_data 11 bits chord_data (excludes root)
// chord_up and chord_up up 12 bits (includes root upper octaves)
void toggle_chord_octave( Stepstruct* target_step, unsigned char note_offset, unsigned char chord_octave_mask ) {

	if( CHECK_MASK( chord_octave_mask, CHORD_OCTAVE_FIRST ) ) {
		TOGGLE_BIT( target_step->chord_data, note_offset - 1 );
	}

	if( CHECK_MASK( chord_octave_mask, CHORD_OCTAVE_SECOND ) ) {
		TOGGLE_BIT( target_step->chord_up, note_offset );
	}

	if( CHECK_MASK( chord_octave_mask, CHORD_OCTAVE_THIRD ) ) {
		TOGGLE_BIT( target_step->chord_up, note_offset + 16 );
	}
}


// Sets the the chord octave(s) mask of a step to the given octave
// chord_data 11 bits chord_data (excludes root)
// chord_up and chord_up up 12 bits (includes root upper octaves)
void set_chord_octave( Stepstruct* target_step, unsigned char note_offset, unsigned char chord_octave_mask, unsigned char value ) {

	if( CHECK_MASK( chord_octave_mask, CHORD_OCTAVE_FIRST ) ) {
		value == ON ? SET_BIT( target_step->chord_data, note_offset - 1 ) : CLEAR_BIT( target_step->chord_data, note_offset - 1 );
	}

	if( CHECK_MASK( chord_octave_mask, CHORD_OCTAVE_SECOND ) ) {
		value == ON ? SET_BIT( target_step->chord_up, note_offset ) : CLEAR_BIT( target_step->chord_up, note_offset );
	}

	if( CHECK_MASK( chord_octave_mask, CHORD_OCTAVE_THIRD ) ) {
		value == ON ? SET_BIT( target_step->chord_up, note_offset + 16 ) : CLEAR_BIT( target_step->chord_up, note_offset + 16 );
	}
}
#endif
