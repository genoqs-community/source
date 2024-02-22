
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
 * Definitions of the page data and functions
 * 08.11.2003 genoQs Machines
 */


#include "../_OCT_objects/Runtime.h"



extern unsigned char how_selected_in_current_scale( 	Pagestruct* target_page,
														unsigned char target_note );

extern unsigned char offset_to_next_current_degree(		Pagestruct* target_page,
														unsigned char pitch );

extern 	void 			shiftAttributeMap( 		Pagestruct* target_page,
												unsigned int row,
												unsigned int in_attribute,
												unsigned char direction );
#ifdef FEATURE_STEP_EVENT_TRACKS
extern	unsigned char 	seek_unmasked_step_col( 	Pagestruct* target_page,
													unsigned int row,
													unsigned char start_pos,
													unsigned char end_pos,
													unsigned int skip_mask );

extern unsigned char 	seek_unmasked_skip_col(		Pagestruct* target_page,
													unsigned int row,
													unsigned char start_pos,
													unsigned char direction,
													unsigned int skip_mask );

extern void 			shiftSkipsFromPOS( 		Pagestruct* target_page,
												unsigned int row,
												unsigned char start_pos,
												unsigned char end_pos,
												unsigned char alt_skips,
												unsigned int skip_mask );


#endif


extern 	void 			shiftAttributeMapFromPOS( 	Pagestruct* target_page,
													unsigned int row,
													unsigned char start_pos,
													unsigned char end_pos,
													unsigned int in_attribute,
													unsigned char allow_skips,
													unsigned int skip_mask );

extern unsigned char 	row_of_track( Pagestruct* target_page, Trackstruct* target_track );
extern void 			NEMO_Page_RMX_track( Pagestruct* target_page, unsigned char row );

#ifdef FEATURE_ENABLE_DICE
extern Trackstruct* throw_dice( 				Pagestruct* target_page );

extern signed int dice_attr_flow_offset( Pagestruct * target_page, unsigned char attr, const card8 locator );
#endif

void Page_clear( Pagestruct* pagePt )
{
	int row;

	// Select all tracks in cursor page
	pagePt->trackSelection = 0x3FF;

	// Clear selected tracks in page
	Page_CLEAR_selected_tracks( pagePt );

	// Re-init the page.
	PAGE_init( pagePt, pagePt->pageNdx, False );

	// Initialize the page Track chain data
	for ( row=0; row < MATRIX_NROF_ROWS; row++ ){
		// Init the chain data for 10 unchained rows: each track is by itself
		pagePt->Track[row]->chain_data[HEAD] = pagePt->Track[row];
		pagePt->Track[row]->chain_data[NEXT] = pagePt->Track[row];
		pagePt->Track[row]->chain_data[PREV] = pagePt->Track[row];
		pagePt->Track[row]->chain_data[PLAY] = pagePt->Track[row];
	}

	// Assign the default pitches to the tracks
	for ( row=0; row<MATRIX_NROF_ROWS; row++ ){
		pagePt->Track[row]->attr_PIT = TRACK_DEFAULT_PITCH[row];
		// Reset track directions to normal
		pagePt->Track[row]->attr_DIR = TRACK_DEF_DIRECTION;
	}

	// Unselect all tracks in cursor page
	pagePt->trackSelection = OFF;

	// Mark page as cleared
	pagePt->page_clear = ON;

	pagePt->priv_track_REC_pattern = OFF;
}













unsigned short Page_getTrackRecPattern( const Pagestruct* pagePt )
{
	return pagePt->priv_track_REC_pattern;
}

int Page_getTrackRecCount( const Pagestruct* pagePt )
{
	return my_bit_cardinality( pagePt->priv_track_REC_pattern );
}

void Page_setTrackRecPattern( Pagestruct* pagePt, unsigned short pattern )
{
	int oldCount = my_bit_cardinality( pagePt->priv_track_REC_pattern );

	pagePt->priv_track_REC_pattern = pattern;

	int newCount = my_bit_cardinality( pagePt->priv_track_REC_pattern );
	G_tracks_in_record_count += (newCount - oldCount);
}

void Page_flipTrackRecPatternBit( Pagestruct* pagePt, int bitNm )
{
	int oldCount = my_bit_cardinality( pagePt->priv_track_REC_pattern );

	pagePt->priv_track_REC_pattern ^= (1<<bitNm);

	int newCount = my_bit_cardinality( pagePt->priv_track_REC_pattern );
	G_tracks_in_record_count += (newCount - oldCount);
}

void Page_setTrackRecPatternBit( Pagestruct* pagePt, int bitNm )
{
	int oldCount = my_bit_cardinality( pagePt->priv_track_REC_pattern );

	pagePt->priv_track_REC_pattern |= (1<<bitNm);

	int newCount = my_bit_cardinality( pagePt->priv_track_REC_pattern );
	G_tracks_in_record_count += (newCount - oldCount);
}

// Compute the cardinality of the step selection in the page
unsigned char get_stepSEL_cardinality( Pagestruct* target_page ){

	unsigned char row = 0;
	unsigned char result = 0;
	unsigned char ndx = 0;

	// Count cardinality of each row
	for ( row=0; row < MATRIX_NROF_ROWS; row++ ){

		if ( target_page->stepSELpattern[row] != 0 ){

			// Add the rowcount to the total count
			ndx = target_page->stepSELpattern_ndx;
			result = result + my_bit_cardinality( target_page->stepSELpattern[ndx][row] );
		}
	}

	return result;
}



// Exports the actual step selection in page to the step selection bit pattern
void export_stepSELpattern( Pagestruct* target_page ){

	unsigned char row = 0;
	unsigned char col = 0;

	// Iterate through all steps
	for ( row=0; row < MATRIX_NROF_ROWS; row++ ){

		// Init the selection pattern
		target_page->stepSELpattern[target_page->stepSELpattern_ndx][row] = 0;

		for ( col=0; col < MATRIX_NROF_COLUMNS; col++ ){

			// If the step is selected in the matrix
			if ( Step_get_status( target_page->Step[row][col], STEPSTAT_SELECT ) == ON ){

				// Set the corresponding bit in the selection array
				target_page->stepSELpattern[target_page->stepSELpattern_ndx][row] |= ( 1 << (15-col) );

				// Flag the step unselected
				Step_set_status( target_page->Step[row][col], STEPSTAT_SELECT, OFF );
			} // Step is selected in Matrix

		} // Col iterator

	} // Row iterator

	// Reset the counter of selected steps
	target_page->stepSelection = OFF;
	target_page->stepAttributeSelection = OFF;
}


// Imports the step selection bit pattern to an actual bit selection
void import_stepSELpattern( Pagestruct* target_page ){

	unsigned char row = 0;
	unsigned char col = 0;

	for ( row=0; row < MATRIX_NROF_ROWS; row++ ){

		for ( col=0; col < MATRIX_NROF_COLUMNS; col++ ){

			// If the step is selected in the bitpattern and is ON in reality
			if (	( (target_page->stepSELpattern[target_page->stepSELpattern_ndx][row] & ( 1 << (15-col) )) != 0 )
				&&	( Step_get_status( target_page->Step[row][col], STEPSTAT_TOGGLE ) == ON )
				){

				// Mark the step selected
				Step_set_status( target_page->Step[row][col], STEPSTAT_SELECT, ON );
			}
			else{

				// Mark the step unselected
				Step_set_status( target_page->Step[row][col], STEPSTAT_SELECT, OFF );
			}
		} // Column iterator
	} // Row iterator

	// Update the counter of selected steps
	target_page->stepSelection = get_stepSEL_cardinality( target_page );
	target_page->stepAttributeSelection = OFF;
}


// General purpose column pattern builder used by Player returns bitpattern of a column
// Incoming column value: between 1 and 16
unsigned int 	PAGE_get_bitpattern(	Pagestruct* target_page, unsigned int col){

	unsigned int bitpattern=0, row=0;

	for (row=0; row<MATRIX_NROF_ROWS; row++) {
		bitpattern |= (target_page->Step[row][col-1]->attr_STATUS & 1) << row;
	}

	return bitpattern;
}


#ifdef FEATURE_ZOOMSTEP_PLUS

// This renders a matrix row as bitpattern. More efficient that columns.
unsigned int 	Page_get_hyperpattern( Pagestruct* target_page, unsigned char row ){

	unsigned char col=0;
	unsigned int bitpattern=0;
	unsigned char i=0;

	for (col=0; col<16; col++) {
//		if ( ( target_page->Step[row][col]->hyperTrack_ndx != 10 )
//				&& ( Step_get_status( target_page->Step[row][col], STEPSTAT_SKIP ) == OFF ) ) {
//			bitpattern |= (1 << (15-col) );
//		}

		if ( ( target_page->Step[row][col]->hyperTrack_ndx != 10 )
				&& ( Step_get_status( target_page->Step[row][col], STEPSTAT_SKIP ) == OFF ) ) {

			// Do not show hyperstep if hyped-track selected
			// H-S will be shown shine_red in fill_PAGE_sel_TRACK.h
			if (  ( my_bit_cardinality( target_page->trackSelection ) == 1 )
						&& (G_zoom_level != zoomTRACK)  )  {

				i = my_bit2ndx( target_page->trackSelection );

				if ( i == target_page->Step[row][col]->hyperTrack_ndx )  {
					//Hyped Track sel button held
					continue;
				}
			}
			bitpattern |= (1 << (15-col) );
		}

	}
	return bitpattern;
}


// This renders a matrix row as bitpattern. More efficient that columns.
unsigned int 	Page_get_trackpattern( Pagestruct* target_page, unsigned char row ){

	unsigned char col=0;
	unsigned int bitpattern=0;

	for (col=0; col<16; col++) {
		if ( (Step_get_status( target_page->Step[row][col], STEPSTAT_TOGGLE ) == ON)
				&& ( Step_get_status( target_page->Step[row][col], STEPSTAT_SKIP ) == OFF )
				&& ( target_page->Step[row][col]->hyperTrack_ndx == 10 )  )  {
				// Do not add hyperstep if its Step is On

			bitpattern |= (1 << (15-col) );
		}
	}
	return bitpattern;
}


// This renders a matrix row as bitpattern. More efficient that columns.
unsigned int 	Page_get_event_trackpattern( Pagestruct* target_page, unsigned char row ) {

	unsigned char col=0;
	unsigned int bitpattern=0;

	for (col=0; col<16; col++) {
		if ( 	( Step_get_status( target_page->Step[row][col], STEPSTAT_EVENT  ) == ON )
			&&	( Step_get_status( target_page->Step[row][col], STEPSTAT_SKIP ) == OFF )  )  {

			bitpattern |= (1 << (15-col) );
		}
	}
	return bitpattern;
}


// This renders a matrix row as bitpattern. More efficient that columns.
unsigned int 	Page_get_chord_trackpattern( Pagestruct* target_page, unsigned char row ) {

	unsigned char col=0;
	unsigned int bitpattern=0;

	for (col=0; col<16; col++) {
		#ifdef FEATURE_ENABLE_CHORD_OCTAVE
		if ( 	( is_step_chord( target_page->Step[row][col] ) )
		#else
		if ( 	( my_bit_cardinality( target_page->Step[row][col]->chord_data & 0x7FF )>0 )
		#endif
			&&	( Step_get_status( target_page->Step[row][col], STEPSTAT_TOGGLE ) == ON )
			&&  ( Step_get_status( target_page->Step[row][col], STEPSTAT_SKIP ) == OFF )
			){

			bitpattern |= (1 << (15-col) );
		}
	}
	return bitpattern;
}

#else

// This renders a matrix row as bitpattern. More efficient that columns.
unsigned int 	Page_get_hyperpattern( Pagestruct* target_page, unsigned char row ){

	unsigned char col=0;
	unsigned int bitpattern=0;

	for (col=0; col<16; col++) {
		if ( target_page->Step[row][col]->hyperTrack_ndx != 10 ){
			bitpattern |= (1 << (15-col) );
		}
	}
	return bitpattern;
}


// This renders a matrix row as bitpattern. More efficient that columns.
unsigned int 	Page_get_trackpattern( Pagestruct* target_page, unsigned char row ){

	unsigned char col=0;
	unsigned int bitpattern=0;

	for (col=0; col<16; col++) {
		if (Step_get_status( target_page->Step[row][col], STEPSTAT_TOGGLE ) == ON) {
				bitpattern |= (1 << (15-col) );
		}
	}
	return bitpattern;
}


// This renders a matrix row as bitpattern. More efficient that columns.
unsigned int 	Page_get_event_trackpattern( Pagestruct* target_page, unsigned char row ) {

	unsigned char col=0;
	unsigned int bitpattern=0;

	for (col=0; col<16; col++) {
		if ( 	( Step_get_status( target_page->Step[row][col], STEPSTAT_EVENT  ) == ON )
//			&&	( Step_get_status( target_page->Step[row][col], STEPSTAT_TOGGLE ) == ON )
			){

			bitpattern |= (1 << (15-col) );
		}
	}
	return bitpattern;
}


// This renders a matrix row as bitpattern. More efficient that columns.
unsigned int 	Page_get_chord_trackpattern( Pagestruct* target_page, unsigned char row ) {

	unsigned char col=0;
	unsigned int bitpattern=0;

	for (col=0; col<16; col++) {
		#ifdef FEATURE_ENABLE_CHORD_OCTAVE
		if ( 	( is_step_chord( target_page->Step[row][col] ) )
		#else
		if ( 	( my_bit_cardinality( target_page->Step[row][col]->chord_data & 0x7FF )>0 )
		#endif
			&&	( Step_get_status( target_page->Step[row][col], STEPSTAT_TOGGLE ) == ON )
			){

			bitpattern |= (1 << (15-col) );
		}
	}
	return bitpattern;
}
#endif


// This renders a matrix row as bitpattern. More efficient that columns.
unsigned int 	Page_get_selection_trackpattern(Pagestruct* target_page, unsigned char row) {

	unsigned char col=0;
	unsigned int bitpattern=0;

	for (col=0; col < MATRIX_NROF_COLUMNS; col++) {

		if (Step_get_status( target_page->Step[row][col], STEPSTAT_SELECT ) == ON) {

				bitpattern |= (1 << (15-col) );
		}
	}
	return bitpattern;
}


// Returns the pattern of steps that are selected but whose activity is off
unsigned int 	Page_get_selectOff_trackpattern(Pagestruct* target_page, unsigned char row) {

	unsigned char col=0;
	unsigned int bitpattern=0;

	for (col=0; col < MATRIX_NROF_COLUMNS; col++) {
		if (   (Step_get_status( target_page->Step[row][col], STEPSTAT_SELECT ) == ON)
			&& (Step_get_status( target_page->Step[row][col], STEPSTAT_TOGGLE ) == OFF)  ) {

			bitpattern |= (1 << (15-col) );
		}
	}
	return bitpattern;

}

#ifdef FEATURE_STEP_SELECT_ALL

// Adds all ON Steps to the current step selection...
// ... If all Steps already in selection then Selection is erased

void all_ON_stepSELpattern( Pagestruct* target_page ){

	unsigned char row = 0;
	unsigned char col = 0;

	// Check if all ON Steps already Selected
	unsigned char stepons = 0;
	unsigned char seltons = 0;
	for ( row=0; row < MATRIX_NROF_ROWS; row++ ){

		for ( col=0; col < MATRIX_NROF_COLUMNS; col++ ){

			// Is the step ON in reality?
			if ( Step_get_status( target_page->Step[row][col], STEPSTAT_TOGGLE ) == ON ) {
				stepons = stepons + 1;
				// Is the Step Selected?
				if (Step_get_status( target_page->Step[row][col], STEPSTAT_SELECT ) == ON ) {
					seltons = seltons + 1;
				}
			}

		} // Column iterator
	} // Row iterator

	// If all ON Steps already Selected Unselect all steps in page
	if ( stepons == seltons ){

		for (row=0; row<MATRIX_NROF_ROWS; row++){

			// Clear the stepSELpattern in the page
			target_page->stepSELpattern[target_page->stepSELpattern_ndx][row] = 0;

			// Clear the selection flag for each step in page
			for (col=0; col<MATRIX_NROF_COLUMNS; col++){

				Step_set_status( target_page->Step[row][col], STEPSTAT_SELECT, OFF );
			}
		}
		// Reset the counter of selected steps
		target_page->stepSelection = OFF;
		target_page->stepAttributeSelection = OFF;
	}

	else {

		for ( row=0; row < MATRIX_NROF_ROWS; row++ ){

			for ( col=0; col < MATRIX_NROF_COLUMNS; col++ ){

				// If the step is ON in reality
				if ( Step_get_status( target_page->Step[row][col], STEPSTAT_TOGGLE ) == ON ) {

					// Set the corresponding bit in the selection array
					target_page->stepSELpattern[target_page->stepSELpattern_ndx][row] |= ( 1 << (15-col) );

					// Mark the step selected
					Step_set_status( target_page->Step[row][col], STEPSTAT_SELECT, ON );
				}

			} // Column iterator
		} // Row iterator

		// Update the counter of selected steps
		target_page->stepSelection = get_stepSEL_cardinality( target_page );
		target_page->stepAttributeSelection = OFF;

	}

}

#endif

#ifdef FEATURE_ENABLE_SONG_UPE
// Return the bitpattern of the skipped steps
unsigned int 	Page_get_locator_event_skippattern( 	Pagestruct* target_page,
									unsigned char row ){

	unsigned char col=0;
	unsigned int bitpattern=0;

	if ( GRID_p_selection[target_page->pageNdx % 10] != target_page ) {
		return 0;
	}

	for (col=0; col < MATRIX_NROF_COLUMNS; col++) {
		if ( Step_get_status( target_page->Step[row][col], STEPSTAT_SKIP ) == ON
				&& target_page->Step[row][col]->phrase_num == target_page->Track[row]->ctrl_offset) {
			bitpattern |= (1 << (15-col) );
		}
	}

	return bitpattern;
}
#endif

// Return the bitpattern of the skipped steps
unsigned int 	Page_get_skippattern( 	Pagestruct* target_page,
									unsigned char row ){

	unsigned char col=0;
	unsigned int bitpattern=0;

	for (col=0; col < MATRIX_NROF_COLUMNS; col++) {
		if ( Step_get_status( target_page->Step[row][col], STEPSTAT_SKIP ) == ON ) {
			bitpattern |= (1 << (15-col) );
		}
	}

	return bitpattern;
}


// This builds the status of a row according to a given bitpattern
void Page_set_skippattern(	Pagestruct* target_page,
							unsigned char row,
							unsigned int bitpattern) {

	unsigned char col=0;
	unsigned int pointer = 0;

	for (col=0;	col<16; col++) {

		pointer = 1 << (15-col);

		//	my_print_bits(pointer & bitpattern);

		// Set the status of the bit anew..
		Step_set_status( target_page->Step[row][col], STEPSTAT_SKIP, (pointer&bitpattern)>0 );
	}

	//	my_print_bits(Page_get_trackpattern(target_page, row));
}



// This builds the status of a row according to a given bitpattern
void Page_set_trackpattern(	Pagestruct* target_page,
							unsigned char row,
							unsigned int bitpattern) {

	unsigned char col=0;
	unsigned int pointer = 0;

	for (col=0;	col<16; col++) {
		pointer = 1 << (15-col);

		//	my_print_bits(pointer & bitpattern);

		// Set the status of the bit anew..
		Step_set_status( target_page->Step[row][col], STEPSTAT_TOGGLE, (pointer & bitpattern) > 0 );
	}

	//	my_print_bits(Page_get_trackpattern(target_page, row));
}



// Modify the column of the selected step in the page
void shiftStepSelectionCol( Pagestruct* target_page, unsigned char direction ){

	unsigned char result = 0;

	switch( direction ){

		case INC:
			switch( target_page->stepSelectionSingleCol ){

				case 15:	result = 0;											break;
				default:	result = target_page->stepSelectionSingleCol + 1;	break;
			}
			break;

		case DEC:
			switch( target_page->stepSelectionSingleCol ){

				case  0:	result = 15;										break;
				default:	result = target_page->stepSelectionSingleCol - 1;	break;
			}
			break;
	}

	// Finally make the value assignment
	target_page->stepSelectionSingleCol = result;
}



// Wraps the track "ndx" in the given direction over the given distance
void Page_wrap_track (	Pagestruct* target_page,
						unsigned int row,
						unsigned char direction,
						unsigned char distance) {

	// Get the trackpattern
	unsigned char i = 0;

	// ATTRIBUTE MAPS SECTION
	// Shift the corresponding step attribute maps along - now including the status information.
	for ( i=0; i < distance; i++ ){

		// Execute the actual map shifting
		shiftAttributeMap( target_page, row, ATTR_VELOCITY, direction );
		shiftAttributeMap( target_page, row, ATTR_PITCH, 	direction );
		shiftAttributeMap( target_page, row, ATTR_LENGTH, 	direction );
		shiftAttributeMap( target_page, row, ATTR_START, 	direction );
		shiftAttributeMap( target_page, row, ATTR_GROOVE, 	direction );
		shiftAttributeMap( target_page, row, ATTR_MIDICC, 	direction );
		shiftAttributeMap( target_page, row, ATTR_STATUS, 	direction );

		// Do not shift this, or the events will shift along!
		// shiftAttributeMap( target_page, row, ATTR_AMOUNT, 	direction );

		if ( 	( G_zoom_level == zoomSTEP )
			#ifdef NEMO
			&&	( ( NEMO_step_VER != VER_EVENT )
			&&	( NEMO_step_VER != VER_RANGE ) )
			#endif
			&&	( row == target_page->stepSelectionSingleRow ) ) {

			// Keep the coordinates of the selected step intact

			// Update the selected step column after the shift of the rest
			shiftStepSelectionCol( target_page, direction );
		}
	} // distance iterator

}




// Identifies and updates highest groove attribute in page
void Page_update_maxGroove (Pagestruct* target_page) {

	unsigned char row=0;
	unsigned char max=0;

	// Find the highest groove value
	for (row = 0; row < MATRIX_NROF_ROWS; row++) {
		if (target_page->Track[row]->attr_GRV	> max) {
			max = target_page->Track[row]->attr_GRV;
		}
	}
	// Store it in the page
	target_page->maxGroove = max;
}


// Requests that a page refresh be made as scheduling allows
void Page_requestRefresh() {

	// Check if there is a pending request for page refresh already
	cyg_count32 semValue = 0;
	cyg_semaphore_peek( &sem_showPage, &semValue );

	// If there is none, post a request
	if( semValue == 0 ){

		cyg_semaphore_post( &sem_showPage );
	}

}






//__________________________________________________________________________________________
// Makes a replica copy of the source track in the target track
void Track_copy( Pagestruct* source_page, unsigned char source_row,
				 Pagestruct* target_page, unsigned char target_row ){

	unsigned char i=0;

	// Copy hyperstep data
	target_page->Track[target_row]->hyper =
		source_page->Track[target_row]->hyper;

	// Copy program and bank change data
	target_page->Track[target_row]->program_change =
		source_page->Track[source_row]->program_change;

	target_page->Track[target_row]->bank_change =
		source_page->Track[source_row]->bank_change;

	// Copy the scale information data
	target_page->Track[target_row]->scale_pitch_offset =
		source_page->Track[source_row]->scale_pitch_offset;

	target_page->Track[target_row]->lead_pitch_offset =
		source_page->Track[source_row]->lead_pitch_offset;


	// ATTRIBUTES
	target_page->Track[target_row]->attr_STATUS			=	source_page->Track[source_row]->attr_STATUS;		// Not sure it is used anywhere..
	target_page->Track[target_row]->attr_VEL			=	source_page->Track[source_row]->attr_VEL;
	target_page->Track[target_row]->attr_PIT			=	source_page->Track[source_row]->attr_PIT;
	target_page->Track[target_row]->attr_LEN			=	source_page->Track[source_row]->attr_LEN;
	target_page->Track[target_row]->attr_STA			=	source_page->Track[source_row]->attr_STA;
	target_page->Track[target_row]->attr_POS			=	source_page->Track[source_row]->attr_POS;
	target_page->Track[target_row]->attr_DIR			=	source_page->Track[source_row]->attr_DIR;
	target_page->Track[target_row]->attr_AMT			=	source_page->Track[source_row]->attr_AMT;
	target_page->Track[target_row]->attr_GRV			=	source_page->Track[source_row]->attr_GRV;
	target_page->Track[target_row]->attr_MCC			=	source_page->Track[source_row]->attr_MCC;
	target_page->Track[target_row]->attr_MCH			=	source_page->Track[source_row]->attr_MCH;
	// Make sure we don't touch the locator attribute
	// target_page->Track[target_row]->attr_LOCATOR		=	source_page->Track[source_row]->attr_LOCATOR;
	target_page->Track[target_row]->attr_MISC			=	source_page->Track[source_row]->attr_MISC;
	target_page->Track[target_row]->attr_CCAMT			=	source_page->Track[source_row]->attr_CCAMT;
	target_page->Track[target_row]->attr_TEMPOMUL		=	source_page->Track[source_row]->attr_TEMPOMUL;
	target_page->Track[target_row]->attr_TEMPOMUL_SKIP	=	source_page->Track[source_row]->attr_TEMPOMUL_SKIP;


	// Copy the attribute event max values
	for ( i=0; i < TRACK_NROF_ATTRIBUTES; i++ ){

		target_page->Track[target_row]->event_max[i] =
			source_page->Track[source_row]->event_max[i] ;
	}


	// FACTORS
	target_page->Track[target_row]->VEL_factor = source_page->Track[source_row]->VEL_factor;
	target_page->Track[target_row]->PIT_factor = source_page->Track[source_row]->PIT_factor;
	target_page->Track[target_row]->LEN_factor = source_page->Track[source_row]->LEN_factor;
	target_page->Track[target_row]->STA_factor = source_page->Track[source_row]->STA_factor;
	target_page->Track[target_row]->AMT_factor = source_page->Track[source_row]->AMT_factor;
	target_page->Track[target_row]->GRV_factor = source_page->Track[source_row]->GRV_factor;
	target_page->Track[target_row]->MCC_factor = source_page->Track[source_row]->MCC_factor;

	// MCC resolution
	target_page->Track[target_row]->CC_resolution = source_page->Track[source_row]->CC_resolution;


	// STEP COPY - Copy the Steps in the tracks
	for (i=0; i<MATRIX_NROF_COLUMNS; i++){

		Step_copy( 	source_page->Step[source_row][i],
					target_page->Step[target_row][i],
					source_page != target_page  );
	}
}



//_____________________________________________________________________________________________
// Makes a copy of the source page in the target page
void Page_copy( Pagestruct* source_page, Pagestruct* target_page ) {

	unsigned char 	i=0,
					j=0,
					k=0;
	unsigned char temp = 0;

	// Copy the tracks and steps (in Track copy included) within the page
	for (i=0; i<MATRIX_NROF_ROWS; i++) {

		Track_copy( source_page, i, target_page, i );
	}

	// TRACK CHAIN CONFIGURATION COPY
	for ( i=0; i < MATRIX_NROF_ROWS; i++ ){

		// Calculate track index in source context, then make assignment
		temp = row_of_track( source_page, source_page->Track[i]->chain_data[ HEAD ] );
		target_page->Track[i]->chain_data[ HEAD ] = target_page->Track[ temp ];

		// Calculate track index in source context, then make assignment
		temp = row_of_track( source_page, source_page->Track[i]->chain_data[ PLAY] );
		target_page->Track[i]->chain_data[ PLAY ] = target_page->Track[ temp ];

		// Calculate track index in source context, then make assignment
		temp = row_of_track( source_page, source_page->Track[i]->chain_data[ PREV ] );
		target_page->Track[i]->chain_data[ PREV ] = target_page->Track[ temp ];

		// Calculate track index in source context, then make assignment
		temp = row_of_track( source_page, source_page->Track[i]->chain_data[ NEXT ] );
		target_page->Track[i]->chain_data[ NEXT ] = target_page->Track[ temp ];
	}


	// Copy all the other page attributes
	#ifdef NEMO
	target_page->track_window			= source_page->track_window;
	#endif
	target_page->locator 				= 0;
	target_page->editorMode				= source_page->editorMode;
	target_page->page_clear				= source_page->page_clear;
	target_page->chainMode				= source_page->chainMode;

	target_page->trackMutepattern		= source_page->trackMutepattern;
	target_page->trackMutepatternStored = source_page->trackMutepatternStored;
	target_page->trackSolopattern		= source_page->trackSolopattern;
	//target_page->track_REC_pattern		= source_page->track_REC_pattern;
	Page_setTrackRecPattern( target_page, Page_getTrackRecPattern(source_page));

	target_page->editAttribute			= source_page->editAttribute;
	target_page->mixAttribute			= source_page->mixAttribute;
	target_page->mixMode				= source_page->mixMode;
	target_page->mixMasterStatus		= source_page->mixMasterStatus;
	target_page->mixingTrack			= source_page->mixingTrack;

	target_page->force_to_scale			= source_page->force_to_scale;

	target_page->maxGroove				= source_page->maxGroove;
	target_page->mixTarget				= source_page->mixTarget;
	target_page->CC_MIXMAP_attribute	= source_page->CC_MIXMAP_attribute;
	target_page->scaleStatus			= source_page->scaleStatus;
	target_page->my_scale_signature		= source_page->my_scale_signature;

	// There are nine selectable user scales
	for ( i=0; i < 9; i++ ){

		target_page->scaleNotes[i]		= source_page->scaleNotes[i];
		target_page->scaleLead[i]		= source_page->scaleLead[i];
	}

	target_page->scaleLead_old			= source_page->scaleLead_old;
	target_page->scaleNotes_old			= source_page->scaleNotes_old;

	// Attribute copy
	target_page->attr_VEL 				= source_page->attr_VEL;
	target_page->attr_PIT 				= source_page->attr_PIT;
	target_page->attr_LEN 				= source_page->attr_LEN;
	target_page->attr_STA 				= source_page->attr_STA;


	for (i=0; i<10; i++) {
		target_page->MIXAMT_VOL[i] 		= source_page->MIXAMT_VOL[i];
		target_page->MIXAMT_PAN[i] 		= source_page->MIXAMT_PAN[i];
		target_page->MIXAMT_MOD[i] 		= source_page->MIXAMT_MOD[i];
		target_page->MIXAMT_EXP[i] 		= source_page->MIXAMT_EXP[i];
		target_page->MIXAMT_USR[i] 		= source_page->MIXAMT_USR[i];

		for (j=0; j<6; j++) {
			for (k=0; k<3; k++) {
				target_page->CC_MIXMAP[j][i][k] = source_page->CC_MIXMAP[j][i][k];
			}
		}
	}

	// target_page->OPS_mode 			= source_page->OPS_mode;

	target_page->attr_mix2edit 			= source_page->attr_mix2edit;
	target_page->attr_mix2edit_MIN		= source_page->attr_mix2edit_MIN;
	target_page->attr_mix2edit_MAX		= source_page->attr_mix2edit_MAX;

	target_page->CHAINS_PLAY_HEAD		= source_page->CHAINS_PLAY_HEAD;
}



//____________________________________________________________________________________________
// Used by PLAY mode to compute the pitch for a particular key press
unsigned char 	Page_getPitchOf( unsigned int keyNdx ){

	// Pitch return value
	unsigned int row = row_of		( keyNdx );
	unsigned int col = column_of	( keyNdx );

	// Make sure we are on the MIDI Scale and do not go negative..
	if (( row == 9 )&&( col < 3 )){
		return 0;
	}
	else {
		return ((9-row) * 12) + col  -2;
	}
}



//____________________________________________________________________________________________
// Returns the dot index of a button whose row and col coordinates are given
unsigned char 	Page_dotIndex( unsigned char row, unsigned char col ){

	return ( 11 +  (11 * col) + row );
}


// Clears all the steps in the given track
void Track_clear_steps( Pagestruct* target_page, unsigned char track_ndx ){

	unsigned char col = 0;

	// Row has been selected, so operate: init steps first
	for (col = 0; col < MATRIX_NROF_COLUMNS; col++) {

		// target_page->Step[row][col]->attr_STATUS = OFF;
		Step_init( target_page->Step[track_ndx][col] );
	}
}


// Clear the track fully
void Track_clear_full( Pagestruct* target_page, unsigned char track_ndx ){

	unsigned char MIDI_channel = target_page->Track[track_ndx]->attr_MCH;

	// Clear all steps in the track
	Track_clear_steps( target_page, track_ndx );

	// Clear any stored program changes
	target_page->Track[track_ndx]->program_change	= TRACK_DEF_PROGRAMCHANGE;
	target_page->Track[track_ndx]->bank_change		= TRACK_DEF_BANKCHANGE;

	// Soft_init the track
	Track_soft_init( target_page->Track[track_ndx] );

	// Re-assign MIDI channel from before
	target_page->Track[track_ndx]->attr_MCH = MIDI_channel;
}


//____________________________________________________________________________________________
// Used tp clear all steps in page by setting their status attribute to zero
// New: put all steps in a track into the init() state. Also the track attributes that apply.
void Page_CLEAR_selected_tracks( Pagestruct* target_page ) {

	unsigned char 	row = 0;
	unsigned int	all_tracks_mask = 0x3ff;
	#ifdef NEMO
	all_tracks_mask = 0xff;
	#endif
	// Applies to the selected rows
	for (row = 0; row < MATRIX_NROF_ROWS; row ++) {

		// Check track_selection if this applies to this row
		if ( target_page->trackSelection & (1 << row)) {

			// Clear the track fully
			Track_clear_full( target_page, row );
		}
	} // Iterate through the rows

	// Mark page as cleared, if all tracks have been selected
	if (target_page->trackSelection == all_tracks_mask) {

		// Mark the page as cleared
		target_page->page_clear = ON;
	}

	// Clear track Selection
	target_page->trackSelection = OFF;
}


//____________________________________________________________________________________________
// Used tp clear all steps in page by setting their status attribute to zero
// New: put all steps in a track into the init() state. Also the track attributes that apply.
void Page_CLEAR_recording_tracks( Pagestruct* target_page ) {

	unsigned char 	row = 0;

	// Applies to the selected rows
	for (row = 0; row < MATRIX_NROF_ROWS; row ++) {

		// Check track_selection if this applies to this row
		if ( target_page->trackSelection & (1 << row)) {

			// Clear the track steps
			Track_clear_steps( target_page, row );
		}
	}

	// Mark page as cleared, if all tracks have been selected
	if (target_page->trackSelection == 0x3ff) {

		target_page->page_clear = ON;
	}

	// Clear track Selection
	target_page->trackSelection = OFF;
}



// Boolean: returns true if the index is in the grid selection, false otherwise
unsigned char PAGE_is_pre_selected_in_GRID( Pagestruct* target_page ){

	return GRID_p_preselection[target_page->pageNdx % 10] == target_page;
}


//____________________________________________________________________________________________
// Randomizes the selected tracks in page
void Page_RNDMZ_selected_tracks( Pagestruct* target_page ) {

	unsigned char 	row = 0,
					col = 0;
//	unsigned char 	value_initial = 0;

	// Applies to step status, ONLY
	for (row = 0; row < MATRIX_NROF_ROWS; row++) {

		// Check track_selection if this applies to this row
		if ( target_page->trackSelection & (1 << row)) {

			// Row has been selected, so operate..
			for (col = 0; col < MATRIX_NROF_COLUMNS; col++) {

				// Do not touch skipped steps
				if ( ( Step_get_status( target_page->Step[row][col], STEPSTAT_SKIP) == ON )){

					continue;
				}

				// Set NOTE ONs
				target_page->Step[row][col]->attr_STATUS = ( ( rand() % 3) % 2 );
			}

			// Mark the page as edited
			target_page->page_clear = OFF;

		} // this is a selected trac, to which this applies

	} // row iterator looking for selected track
}


//____________________________________________________________________________________________
// Randomizes the selected tracks in page
void NEMO_Page_RNDMZ_selected_tracks( Pagestruct* target_page ) {

	unsigned char 	row = 0,
					col = 0;
//	unsigned char 	value_initial = 0;

	// Applies to step status, ONLY
	for (row = 0; row < MATRIX_NROF_ROWS; row++) {

		// Check track_selection if this applies to this row
		if ( target_page->trackSelection & (1 << row)) {

			// Empty tracks are being randomized. Non-empty ones remixed
			switch( Page_get_trackpattern( target_page, row ) ){

				// Track is empty so randomize
				case 0:
					// Row has been selected, so operate..
					for (col = 0; col < MATRIX_NROF_COLUMNS; col++) {

						// Do not touch skipped steps
						if ( ( Step_get_status( target_page->Step[row][col], STEPSTAT_SKIP) == ON )){

							continue;
						}

						// Set NOTE ONs
						target_page->Step[row][col]->attr_STATUS = ( ( rand() % 3) % 2 );
					}

					// Mark the page as edited
					target_page->page_clear = OFF;
					break;

				// Track has some contents so remix the track
				default:
					NEMO_Page_RMX_track( target_page, row );
					break;
			}

		} // this is a selected track, to which this applies

	} // row iterator looking for selected track
}




//____________________________________________________________________________________________
// Add step toggle status to the target track on an OR basis
void FLT_add_status( 	Pagestruct* target_page,
						unsigned char col,
						unsigned char dest_ndx ){

	unsigned char row = 0;

	// Iterate through the columns
	for ( row=0; row < MATRIX_NROF_ROWS; row++ ){

		// Skip tracks that are not selected, and the dest_ndx
		if ( 	(	( target_page->trackSelection & ( 1 << row )) == 0 )
			&&	(	row != dest_ndx )
			){

			continue;
		}

		// ON / OFF status
		if (	( Step_get_status( target_page->Step[row][col], STEPSTAT_TOGGLE) == ON )
			&&	( Step_get_status( target_page->Step[row][col], STEPSTAT_SKIP) != ON )
			){

			Step_set_status( target_page->Step[dest_ndx][col], STEPSTAT_TOGGLE, ON );
		}
	} // col iterator
}



//____________________________________________________________________________________________
// Add step toggle status to the target track on an OR basis
void FLT_add_LENSTA( 	Pagestruct* target_page,
						unsigned char col,
						unsigned char dest_ndx ){

	unsigned char row = 0;

	// Iterate through the columns
	for ( row=0; row < MATRIX_NROF_ROWS; row++ ){

		// Skip tracks that are not selected, and the dest_ndx
		if ( 	(	( target_page->trackSelection & ( 1 << row )) == 0 )
			&&	(	row != dest_ndx )
			){

			continue;
		}

		// ON / OFF status
		if (	( Step_get_status( target_page->Step[row][col], STEPSTAT_TOGGLE) == ON )
			&&	( Step_get_status( target_page->Step[row][col], STEPSTAT_SKIP) != ON )
			){

			// LEN takeover
			target_page->Step[dest_ndx][col]->attr_LEN =
				target_page->Step[row][col]->attr_LEN;

			// STA takeover
			target_page->Step[dest_ndx][col]->attr_STA =
				target_page->Step[row][col]->attr_STA;

			// VEL takeover - last minute request by Tom :-)
			target_page->Step[dest_ndx][col]->attr_VEL =
				target_page->Step[row][col]->attr_VEL;
		}
	} // col iterator
}



//____________________________________________________________________________________________
// Add step pitches to steps on the target track on a chord basis
void FLT_add_PIT( 		Pagestruct* target_page,
						unsigned char col,
						unsigned char dest_row ){

	unsigned char in_pitch = 0;

	unsigned char current_lowest_pitch = 200;
	unsigned char lowest_pitch_row = 0;
	unsigned char last_pitch = 0;

	unsigned char pass = 0;
	unsigned char row = 0;

	// Make sure to react to first valid pitch detected
	current_lowest_pitch = 200;

	// Take the 7 lowest pitches and build them into the target step
	for ( pass=0; pass < 7; pass ++ ){

		// Find the current lowest pitch that was not visited
		for ( row=0; row < MATRIX_NROF_ROWS; row++ ){

			// Only in the selected tracks, but not the destination row, and current step is ON
			if ( 	( ( target_page->trackSelection & ( 1<<row )) != 0 )
				&&	( row != dest_row )
				&& 	( Step_get_status( target_page->Step[row][col], STEPSTAT_TOGGLE) == ON )
				){

				// Compute the played pitch of the origin
				in_pitch = normalize(  	  target_page->Track[row]->attr_PIT
										+ target_page->Step[row][col]->attr_PIT,
										1, 127 );

// d_iag_printf( "in_pitch(%d):%d\n", row, in_pitch );

				// Pitch found is lowest unprocessed pitch
				if ( 	( in_pitch < current_lowest_pitch )
					&&	( in_pitch > last_pitch )
					){

					current_lowest_pitch = in_pitch;
					lowest_pitch_row = row;
				} // lowest pitch finder
			} // eligible track condition
		} // row iterator


		// Make sure we don't repeat operations
		if ( current_lowest_pitch != 200 ){

//			d_iag_printf( "current_lowest_pitch:%d lowest_pitch_row:%d\n", current_lowest_pitch, lowest_pitch_row );

			// Remember the last processed pitch
			last_pitch = current_lowest_pitch;

			// If the destination step is OFF, assign the new pitch to the step
			if ( Step_get_status( target_page->Step[dest_row][col], STEPSTAT_TOGGLE ) == OFF ){

				// No chord set for the step, and 0 is the neutral strum value
				target_page->Step[dest_row][col]->chord_data = 9 << 11;
				target_page->Step[dest_row][col]->phrase_num  = 0;
				target_page->Step[dest_row][col]->phrase_data  = PHRASE_DATA_INIT_VAL;

				// Fill the step pitch value simply.. after having erased traces of chords
				target_page->Step[dest_row][col]->attr_PIT =
					adjust_PIT_to_track( current_lowest_pitch, target_page->Track[dest_row] );

				// Turn the Step on
				Step_set_status( target_page->Step[dest_row][col], STEPSTAT_TOGGLE, ON );

			}

			// Destination step is ON, add pitch to its chord stack
			else{

//				d_iag_printf( "make_chord stepBase:%d trackBase:%d add:%d\n",
//						target_page->Step[dest_row][col]->attr_PIT,
//						target_page->Track[dest_row]->attr_PIT,
//						current_lowest_pitch
//				);

				// Add the incoming pitch to the chord structure of the step
				make_chord( target_page->Step[dest_row][col],
							target_page->Track[dest_row]->attr_PIT,
							current_lowest_pitch
				);
				}

			// Mark sonsumed the current lowest pitch
			current_lowest_pitch = 200;

		} // Operation repeat lock

	} // pass iterator
}



//____________________________________________________________________________________________
// Flat the track selection onto the lowest track
void Page_FLT_selected_tracks( Pagestruct* target_page ) {

	unsigned char	dest_ndx = 0;
	unsigned char 	row  	= 0;
	unsigned char 	col 	= 0;

	// No need to continue if no track selected
	if ( target_page->trackSelection == 0 ){
		return;
	}

	// Identify the destination track index
	for ( row = 0; row < MATRIX_NROF_ROWS; row ++ ){

		if ( target_page->trackSelection & (1 << row)) {

			dest_ndx = row;
		}
	}

	// Start FLT processing
	for ( col=0; col < MATRIX_NROF_COLUMNS; col++ ){

		// Add the pitch of steps to the steps in destination
		FLT_add_PIT( target_page, col, dest_ndx );

		// Take over the LEN of the last encountered active step in col
		FLT_add_LENSTA( target_page, col, dest_ndx );

		// Add the status of track steps to the destination - includes skips
		FLT_add_status( target_page, col, dest_ndx );

	} // Column iterator

	// Clear the track selection
	target_page->trackSelection = 0;
}




// Compute the Page the leftmost neighbor to current page
Pagestruct* Page_leftmost_neighbour( Pagestruct* target_page ) {

	signed short 	this_ndx = 0;
	unsigned char	j;
	Pagestruct* 	leftmost_page = target_page;

	this_ndx = target_page->pageNdx;

	if ( Page_repository[this_ndx].page_clear == OFF ) {

		for ( j = 0; j < MATRIX_NROF_COLUMNS; j++ ) {

			this_ndx -= 10;

			if (	( this_ndx < 0 )
				||	( Page_repository[this_ndx].page_clear == ON ) ) {

				leftmost_page = &Page_repository[this_ndx + 10];
				break;
			}

		}
	}

	return leftmost_page;
}



// Compute the Page the rightmost neighbor to current page
Pagestruct* Page_rightmost_neighbour( Pagestruct* target_page ) {

	unsigned short 	this_ndx = 0;
	unsigned char	j;
	Pagestruct* 	rightmost_page = target_page;

	this_ndx = target_page->pageNdx;
	if ( Page_repository[this_ndx].page_clear == OFF ) {

		for ( j = 0; j < MATRIX_NROF_COLUMNS; j++ ) {

			this_ndx += 10;

			if (	( ( this_ndx / 10 ) > MATRIX_NROF_COLUMNS - 1 )
				||	( Page_repository[this_ndx].page_clear == ON ) ) {

				rightmost_page = &Page_repository[this_ndx - 10];
				break;
			}

		}
	}

	return rightmost_page;
}



//____________________________________________________________________________________________
// Returns a random value according to the amount and interval passed
signed char	randomize_byamount( 		signed int in_value,
										signed int amount,
										signed int min_val,
										signed int max_val 		){

	signed int interval_offset = 0;
	signed int result		=	0;
	signed int amt_val		= 	0;
	signed int result_old 	=	0;
	signed char direction	= 0;

	// Make sure we are not getting malformed data
	if ( max_val < min_val ) return max_val;

	// Make sure we are not overshooting due to a bad amount
	if ( amount > 100 ) amount = 100;

	// Move the interval into the positive domain to operate on..
	if ( min_val < 0 ) {

		max_val = max_val - min_val;
		in_value = in_value - min_val;
		interval_offset = min_val;
		min_val = 0;
	}

	// Avoid multiplication and division by 0
	if ( in_value == 0) in_value = 1;

	// Calculus details on book page 234. Randomize the top amount percentage..
	// ..within a range of 3/2 (i.e. 1,5)
	amt_val 	=	( rand() % ((max_val*100) * amount )/100 ) / 100;
	amt_val		= 	amt_val / 2;

	// Decide whether to increase or decrease the value
	switch( rand()%2 ){

		case 0:
			// Increase the value and top it at maximum value
			result 	= 	in_value + amt_val;
			if ( (in_value + amt_val) > max_val ){
				result 	= 	max_val;
			}
	direction = POS;
			break;

		case 1:
			// Decrease the value and bootom it at minimum value
			result 	= 	in_value - amt_val;
			if (result < min_val) {
				result 	= 	min_val;
			}
	direction = NEG;
			break;
	}

	result_old = result;

	// Final check against the value bounds
	if ( result < min_val ){
		result = min_val;
	}
	if ( result > max_val ){
		result = max_val;
	}

//	 d_iag_printf("in_val:%d amt:%d _val:%d min:%d max:%d dir:%d res:%d\n",
//		in_value, amount, amt_val, min_val, max_val, direction, result );

	// Result now contains the return value
	return (signed char) ( result + interval_offset );
}



//____________________________________________________________________________________________
// Remix the selected tracks in page by operating on their attribute maps.
// See description below for what this means.
void NEMO_Page_RMX_track( Pagestruct* target_page, unsigned char row ){

	// Remix depends on the current track amount value and means:
	// MAP			Random Shift		Random values
	// -----------------------------------------------
	// Velocity		X					X
	// Pitch		X
	// Length		X					X
	// Start		X					X
	// Position		X

	unsigned char 	// row  	= 0,
					col 	= 0,
					temp	= 0,
					i		= 0;

	// Perform the shift operations on the track maps. Computationally intensive.
	// Do things more subtly: be active only in 50% of the cases
	// if ( rand()%2 ) continue;

	// VELOCITY Map shift
	temp = rand() % 16;
	for( i=0; i<temp; i++ ){

		shiftAttributeMap( target_page, row, ATTR_VELOCITY, INC );
	}

	// PITCH Map shift
	temp = rand() % 16;
	for( i=0; i<temp; i++ ){

		shiftAttributeMap( target_page, row, ATTR_PITCH, INC );
	}

	// LENGTH Map shift
	temp = rand() % 16;
	for( i=0; i<temp; i++ ){

		shiftAttributeMap( target_page, row, ATTR_LENGTH, INC );
	}

	// START Map shift
	temp = rand() % 16;
	for( i=0; i<temp; i++ ){

		shiftAttributeMap( target_page, row, ATTR_START, INC );
	}

	// POSITION Map shift
	temp = rand() % 16;
	for( i=0; i<temp; i++ ){

		modify_parameter ( &(target_page->Track[row]->attr_POS),
					TRACK_MIN_POSITION, TRACK_MAX_POSITION, INC, OFF, FIXED);
	}



	// Row has been selected, so do the remix operation on the steps (cols)
	for (col = 0; col < MATRIX_NROF_COLUMNS; col++) {

		// VELOCITY Random values
		target_page->Step[row][col]->attr_VEL =
			(signed char)
			randomize_byamount( target_page->Step[row][col]->attr_VEL,
								target_page->Track[row]->attr_AMT
								+ target_page->Track[row]->event_offset[ATTR_AMOUNT]
								,
								STEP_MIN_VELOCITY, STEP_MAX_VELOCITY );

		// LENGTH Random values
		target_page->Step[row][col]->attr_LEN =
			(signed char)
			randomize_byamount( (unsigned char)target_page->Step[row][col]->attr_LEN,
								(unsigned char) target_page->Track[row]->attr_AMT
								+ target_page->Track[row]->event_offset[ATTR_AMOUNT]
								,
// old									STEP_MIN_LENGTH, ((unsigned char)STEP_MAX_LENGTH) );
								STEP_MIN_LENGTH, STEP_MAX_LENGTH );

		// START Random values
		// Get the random delta to be applied
		temp =
			(signed char)
			randomize_byamount( 6 + target_page->Step[row][col]->attr_STA,
								target_page->Track[row]->attr_AMT
								+ target_page->Track[row]->event_offset[ATTR_AMOUNT]
								,
								STEP_MIN_START + 6, STEP_MAX_START + 6 );

		if ((temp >= STEP_MIN_START + 6) && (temp < STEP_MAX_START + 6)) {
			target_page->Step[row][col]->attr_STA = temp - 6;
		}

		// d_iag_printf( "temp:%d start value now:%d\n",
		//   temp, target_page->Step[row][col]->attr_STA );

	} // Column iterator
}


//____________________________________________________________________________________________
// Remix the selected tracks in page by operating on their attribute maps.
// See description below for what this means.
void Page_RMX_selected_tracks( Pagestruct* target_page ) {

	// Remix depends on the current track amount value and means:
	// MAP			Random Shift		Random values
	// -----------------------------------------------
	// Velocity		X					X
	// Pitch		X
	// Length		X					X
	// Start		X					X
	// Position		X

	unsigned char 	row  	= 0,
					col 	= 0,
					temp	= 0,
					i		= 0;

	// Iterate through rows to identify selected ones.
	for (row = 0; row < MATRIX_NROF_ROWS; row++) {

		// Check track_selection if this applies to this row
		if ( target_page->trackSelection & (1 << row)) {

			// Perform the shift operations on the track maps. Computationally intensive.
			// Do things more subtly: be active only in 50% of the cases
			// if ( rand()%2 ) continue;

			// VELOCITY Map shift
			temp = rand() % 16;
			for( i=0; i<temp; i++ ){

				shiftAttributeMap( target_page, row, ATTR_VELOCITY, INC );
			}

			// PITCH Map shift
			temp = rand() % 16;
			for( i=0; i<temp; i++ ){

				shiftAttributeMap( target_page, row, ATTR_PITCH, INC );
			}

			// LENGTH Map shift
			temp = rand() % 16;
			for( i=0; i<temp; i++ ){

				shiftAttributeMap( target_page, row, ATTR_LENGTH, INC );
			}

			// START Map shift
			temp = rand() % 16;
			for( i=0; i<temp; i++ ){

				shiftAttributeMap( target_page, row, ATTR_START, INC );
			}

			// POSITION Map shift
			temp = rand() % 16;
			for( i=0; i<temp; i++ ){

				modify_parameter ( &(target_page->Track[row]->attr_POS),
							TRACK_MIN_POSITION, TRACK_MAX_POSITION, INC, OFF, FIXED);
			}



			// Row has been selected, so do the remix operation on the steps (cols)
			for (col = 0; col < MATRIX_NROF_COLUMNS; col++) {

				// VELOCITY Random values
				target_page->Step[row][col]->attr_VEL =
					(signed char)
					randomize_byamount( target_page->Step[row][col]->attr_VEL,
										target_page->Track[row]->attr_AMT
										+ target_page->Track[row]->event_offset[ATTR_AMOUNT]
										,
										STEP_MIN_VELOCITY, STEP_MAX_VELOCITY );

				// LENGTH Random values
				target_page->Step[row][col]->attr_LEN =
					(signed char)
					randomize_byamount( (unsigned char)target_page->Step[row][col]->attr_LEN,
										(unsigned char) target_page->Track[row]->attr_AMT
										+ target_page->Track[row]->event_offset[ATTR_AMOUNT]
										,
// old									STEP_MIN_LENGTH, ((unsigned char)STEP_MAX_LENGTH) );
										STEP_MIN_LENGTH, STEP_MAX_LENGTH );

				// START Random values
				// Get the random delta to be applied
				temp =
					(signed char)
					randomize_byamount( 6 + target_page->Step[row][col]->attr_STA,
										target_page->Track[row]->attr_AMT
										+ target_page->Track[row]->event_offset[ATTR_AMOUNT]
										,
										STEP_MIN_START + 6, STEP_MAX_START + 6 );

				if ((temp >= STEP_MIN_START + 6) && (temp < STEP_MAX_START + 6)) {
					target_page->Step[row][col]->attr_STA = temp - 6;
				}

				// d_iag_printf( "temp:%d start value now:%d\n",
				//   temp, target_page->Step[row][col]->attr_STA );

			} // Column iterator

		} // Row is that of a selcted track

	} // Row iterator
}







//____________________________________________________________________________________________
// Compute the active step playcoordinate which is the absolute position of the step trigger
// Function of step ATTR_START and track ATTR_GROOVE.
// Incorporates trigger of the next column, which needs to be played earlier that its beat(0) point
unsigned char 	get_track_GRV_offset(	Pagestruct* target_page,
									unsigned char row,
									unsigned char col,
									unsigned char which_col ){

	unsigned char result = 0;
	unsigned char delay = 0;

	// TR909 groove implementation: delay the even numbered 1/16th notes. All others byebye.
	if ( (col+1) % 2 != 0){
		return 0;
	}

	signed char dice_groove_offset = 0;

	#ifdef FEATURE_ENABLE_DICE
	#ifdef NEMO
	dice_groove_offset = dice_attr_flow_offset( target_page, NEMO_ATTR_GROOVE, col );
	#else
	dice_groove_offset = dice_attr_flow_offset( target_page, ATTR_GROOVE, col );
	#endif
	#endif

	// Compute the absolute delay according to GRV - and convert it to a value in [0..12]
	if ( target_page->Track[row]->attr_GRV + dice_groove_offset > 0 ){

		switch( normalize(target_page->Track[row]->attr_GRV + dice_groove_offset + 1, STEP_MIN_GROOVE, STEP_MAX_GROOVE) % 2 ){

		case 0:
			// Real ODD values: Use the value / 2
			delay = normalize(target_page->Track[row]->attr_GRV + dice_groove_offset + 1, STEP_MIN_GROOVE, STEP_MAX_GROOVE) / 2;
			break;

		case 1:
			// Real EVEN values: Use the value of previous odd +/- 1
			delay = normalize(target_page->Track[row]->attr_GRV + dice_groove_offset + 1, STEP_MIN_GROOVE, STEP_MAX_GROOVE) / 2;
			switch( rand() % 3 ){
				case 0: 			break;
				case 1:	delay --;	break;
				case 2: delay ++;	break;
			}
			break;
		}
	} // Non-zero track GRV value

	result = delay;

	// Return the value to compare G_TTC_abs_value against
	return result;
}



#if GROEF
//____________________________________________________________________________________________
// Compute the active step playcoordinate which is the absolute position of the step trigger
// Function of step ATTR_START and track ATTR_GROOVE.
// Incorporates trigger of the next column, which needs to be played earlier that its beat(0) point
unsigned char 	get_step_GRV_offset(	Pagestruct* target_page,
										unsigned char row,
										unsigned char col,
										unsigned char which_col ){

	unsigned char result = 0;

	// Heisenberg groove implementation: delay the step by a random amount defined by groove.
	if ( target_page->Step[row][col]->attr_GRV == 0){
		return result;
	}

	// Compute a random value between 0 and STEP_MAX_GROOVE
	result = rand() % (target_page->Step[row][col]->attr_GRV + 1);

	// Return the computed value, which is really a delay in TTC units (1/12)
	return result;
}
#endif


// Init the given CC MIXMAP in the given page
void init_CC_MIXMAP( Pagestruct* target_page, unsigned char map_ndx ){

	unsigned char j = 0;
	unsigned char k = 0;

	// Init the CC_MIXMAP data
	for (j=0; j<10; j++) {
		for (k=0; k<3; k++) {

			switch( k ){
				case CC_MIXMAP_MCH:
					target_page->CC_MIXMAP[map_ndx][j][k] =  1;
					break;

				case CC_MIXMAP_AMT:
					target_page->CC_MIXMAP[map_ndx][j][k] =  64;
					break;

				default:
					target_page->CC_MIXMAP[map_ndx][j][k] =  MIDICC_NONE;
					break;
			}

		}
	}

#ifdef NEMO
	// ..do nothing
#else
	// Set the default position for the CC mixmap
	target_page->CC_MIXMAP_attribute = CC_MIXMAP_AMT;
#endif
}




//____________________________________________________________________________________________
// Clears the given CC mixmap in the given page
void clear_CC_MIXMAP( Pagestruct* target_page, unsigned char in_ndx ){

	unsigned char map_ndx = 0;

	// Identify which map to operate on - transform to an array index
	switch( in_ndx ){

		case MIXTGT_USR0:	map_ndx = 0;	break;
		case MIXTGT_USR1:	map_ndx = 1;	break;
		case MIXTGT_USR2:	map_ndx = 2;	break;
		case MIXTGT_USR3:	map_ndx = 3;	break;
		case MIXTGT_USR4:	map_ndx = 4; 	break;
		case MIXTGT_USR5:	map_ndx = 5;	break;
	}

	// Final consistency check for the right index
	if (	( map_ndx < 1 )
		||	( map_ndx > 5 )
		){
			return;
		}

	// Perform the clear operation on the CC map
	init_CC_MIXMAP( target_page, map_ndx );
}



extern void export_stepSELpattern( Pagestruct* target_page );

// Swaps the full content of step a with the content of Step b
void Step_swap( Pagestruct* target_page, Stepstruct* a_step, Stepstruct* b_step ){

	Stepstruct buffer;

	// Copy step A to buffer
	Step_copy( a_step, &buffer, true );

	// Copy step B to step A
	Step_copy( b_step, a_step, true );

	// Copy step A to step B
	Step_copy( &buffer, b_step, true );

	// Update the step selection pattern
	export_stepSELpattern( target_page );
	import_stepSELpattern( target_page );
}


// Move the step in the given direction
void Step_move( Pagestruct* target_page, unsigned char row, unsigned char col, unsigned char direction ){

	switch( direction ){

		case POS:
			col = (col + 1) % 16;
			break;

		case NEG:
			// Swap step with the left neighbour
			if ( col == 0 ){
				col = 15;
			}
			else{
				col = col - 1;
			}
			break;
	}

	// Do the step swap
	Step_swap( target_page, target_page->Step[row][col], target_page->Step[row][col] );
}

void Step_zoom( 	Pagestruct* target_page,
					unsigned char row,
					unsigned char col ){

	unsigned char rowctr = 0;
	unsigned char colctr = 0;

	Stepstruct *target_step = target_page->Step[row][col];

	// Un-SELECT all potentially selected steps in page
	if (target_page->stepSelection != 0) {

		for (rowctr=0; rowctr<MATRIX_NROF_ROWS; rowctr++){
			for (colctr=0; colctr<MATRIX_NROF_COLUMNS; colctr++){

				Step_set_status( target_page->Step[rowctr][colctr], STEPSTAT_SELECT, OFF );
			}
		}
		// Reset the counter of selected steps
		target_page->stepSelection = 0;
		target_page->stepAttributeSelection = OFF;
	}

	// Select the current step formally
	Step_set_status( target_step, STEPSTAT_SELECT, ON );

	// Update the step selection counter in page
	target_page->stepSelection = 1;

	// Remember STEP COORDINATES
	target_page->stepSelectionSingleRow = row;
	target_page->stepSelectionSingleCol = col;

	// d_iag_printf( "zooming into step row:%d col:%d ts:%d\n",
	//		row, col, target_page->trackSelection );

	// Zoom into the step: Switch the mode
	G_zoom_level = zoomSTEP;
}

#ifdef FEATURE_STEP_EVENT_TRACKS

void Step_rotate(Pagestruct* target_page, unsigned char current_row, unsigned char row, unsigned char start_pos, unsigned char end_pos ) {

	unsigned char direction = start_pos < end_pos ? INC : DEC;

	// Execute the actual map shifting
	shiftAttributeMapFromPOS( target_page, row, end_pos, start_pos, ATTR_VELOCITY, 	 TRUE, OFF );
	shiftAttributeMapFromPOS( target_page, row, end_pos, start_pos, ATTR_PITCH, 	 TRUE, OFF );
	shiftAttributeMapFromPOS( target_page, row, end_pos, start_pos, ATTR_LENGTH, 	 TRUE, OFF );
	shiftAttributeMapFromPOS( target_page, row, end_pos, start_pos, ATTR_START, 	 TRUE, OFF );
	shiftAttributeMapFromPOS( target_page, row, end_pos, start_pos, ATTR_GROOVE, 	 TRUE, OFF );
	shiftAttributeMapFromPOS( target_page, row, end_pos, start_pos, ATTR_MIDICC, 	 TRUE, OFF );
	shiftAttributeMapFromPOS( target_page, row, end_pos, start_pos, ATTR_AMOUNT, 	 TRUE, OFF );
	shiftAttributeMapFromPOS( target_page, row, end_pos, start_pos, ATTR_STATUS, 	 TRUE, OFF );

	if ( 	( G_zoom_level == zoomSTEP )
		&&	( row == target_page->stepSelectionSingleRow ) ) {
		// Keep the coordinates of the selected step intact
		// Update the selected step column after the shift of the rest
		shiftStepSelectionCol( target_page, direction );
	}
}


void Skip_rotate(Pagestruct* target_page, unsigned char current_row, unsigned char row, unsigned char start_pos, unsigned char end_pos ) {

	//unsigned char direction = start_pos < end_pos ? INC : DEC;

	// Execute the actual skip shifting
	shiftSkipsFromPOS( target_page, row, end_pos, start_pos, TRUE, OFF );

}

#endif
