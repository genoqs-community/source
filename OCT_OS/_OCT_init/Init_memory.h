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
 *
 * Init code for the startup phase and for object re-initialization.
 *
 * STARTUP SEQUENCE: Creation and Assignment.
 *
 * Init the entire repository (160 pages)
 * 1.	 Create Button objects
 * 2.	 Create Step objects
 * 3. 	 Create Track objects
 * 3.1	 Create Tool Objects
 * 3.2	 Create Collection Objects
 * 4.	 Create Page objects
 *
 * Once the Objects are created link them to each other:
 * 5. 	 Assign Tracks to Pages
 * 6.	 Assign Buttons to Pages
 * 7.	 Assign Steps to Pages
 * 8. 	 Assign Tools to Pages
 * 9. 	 Assign Collections to Pages
 *
 * genoQs Machines, 15.04.2004
 */


// Abletonstruct ABLETON_client;

// #include "ABLETON_init.h"
#include "../_OCT_objects/Persistent.h"
#include "../_OCT_objects/Phrase.h"



// **********************************************************************
// MEMORY ALLOCATION for the entire object repository
// **********************************************************************

// STEPS
Stepstruct 			Step_repository		[MAX_NROF_STEPS];

// TRACKS
Trackstruct 		Track_repository	[MAX_NROF_TRACKS];

// PAGES
Pagestruct 			Page_repository		[MAX_NROF_PAGES];
#ifdef FEATURE_ENABLE_DICE
// DICE performance modes
Pagestruct*	DICE_bank		=	&Page_repository[DICE_PAGE];
#endif
Pagestruct* BUFFER_page 	= 	&Page_repository[149];

// Used to keep some PAGE like information to be used by the GRID
Pagestruct* GRID_assistant_page = &Page_repository[109];

// Used as a copy buffer for sorting of tracks (ALN) in MIX MAP (ATR) mode
Pagestruct* PAGE_assistant_page = &Page_repository[99];

// Array of play mode pages - for indexes, see below
Pagestruct* PLAY_MODE_page[9];

// Later on, we do the following assignment:
//	PLAY_MODE_page[8] = &Page_repository[89];
//	PLAY_MODE_page[7] = &Page_repository[79];
//	PLAY_MODE_page[6] = &Page_repository[69];
//	PLAY_MODE_page[5] = &Page_repository[59];
//	PLAY_MODE_page[4] = &Page_repository[49];
//	PLAY_MODE_page[3] = &Page_repository[39];
//	PLAY_MODE_page[2] = &Page_repository[29];
//	PLAY_MODE_page[1] = &Page_repository[19];
//	PLAY_MODE_page[0] = &Page_repository[9];


// Initializes a given direction to its initial state
void direction_init( unsigned char dir ){

	unsigned char set=0;
	unsigned char i = 0;


	// Trigger set iterator - clear all triggers in direction before rewriting it
	for (set=0; set < 17; set++){

		// Init all triggers to 0
		for (i=0; i<10; i++){
			direction_repository[ dir ].trigger_set[ set ].trigger[i] = 0;
		}

		direction_repository[ dir ].trigger_set[ set ].certainty_next = 100;
	}


	switch( dir ){

		case 1:
			// Iterate through the sets in dir 1
			for ( set=1; set < 17; set++ ){

				direction_repository[ dir ].trigger_set[ set ].trigger[1] = set;
				direction_repository[ dir ].trigger_set[ set ].certainty_next = 100;
			}
			break;


		case 2:
			// Iterate through the sets in dir 1
			for ( set=1; set < 17; set++ ){

//				direction_repository[ dir ].trigger_set[ set ].trigger[1] = 17 - set;
//				direction_repository[ dir ].trigger_set[ set ].certainty_next = 100;

				direction_repository[ dir ].trigger_set[ set ].trigger[1] = set;
				direction_repository[ dir ].trigger_set[ set ].certainty_next = 0;
			}
			break;


		case 3:
			// Iterate through the sets in dir 2
			for ( set=1; set < 9; set++ ){

				direction_repository[ dir ].trigger_set[ set ].trigger[1] = (set * 2) - 1;
				direction_repository[ dir ].trigger_set[ set ].trigger[2] =  set * 2;

				direction_repository[ dir ].trigger_set[ set ].certainty_next = 100;
			}
			for ( set=9; set < 17; set++ ){

				direction_repository[ dir ].trigger_set[ set ].trigger[1] =  (17 - set) * 2;
				direction_repository[ dir ].trigger_set[ set ].trigger[2] = ((17 - set) * 2) - 1;

				direction_repository[ dir ].trigger_set[ set ].certainty_next = 100;
			}
			break;


		case 4:
			// Iterate through sets in dir 4, set triggers to linear, certainty_next to 66%
			for ( set=1; set < 17; set++ ){

				direction_repository[ dir ].trigger_set[ set ].trigger[1] = set;
				direction_repository[ dir ].trigger_set[ set ].certainty_next = 66;
			}
			break;


		case 5:
			// Iterate through the sets in dir 4 and set all triggers to 0
			for ( set=1; set < 17; set++ ){

				// Triggers can stay the same as for LINEAR, only certainty_next adjusted
				direction_repository[ dir ].trigger_set[ set ].trigger[1] = 0;
				direction_repository[ dir ].trigger_set[ set ].certainty_next = 100;
			}
			break;


		default:
			// Iterate through the sets in dir 1
			for ( set=1; set < 17; set++ ){

				direction_repository[ dir ].trigger_set[ set ].trigger[1] = set;
				direction_repository[ dir ].trigger_set[ set ].certainty_next = 100;
			}
			break;
	}
}



// Initialize the direction repository
void direction_repository_init(){

	unsigned char dir=0;
	unsigned char set=0;
	unsigned char i = 0;

	// ALL: First init all directions to LINEAR FORWARD
	// Direction iterator - there are 16 directions to work with
	for ( dir=0; dir < 17; dir++ ){

		// Trigger set iterator - each direction has 16 trigger sets
		for (set=0; set < 17; set++){

			// Init all triggers to 0
			for (i=0; i<10; i++){
				direction_repository[ dir ].trigger_set[ set ].trigger[i] = 0;
			}

			direction_repository[ dir ].trigger_set[ set ].certainty_next = 100;
		}
	}

	// PROGRAMMATIC SETTING OF THE DIRECTIONS

	// DIR 0: not used

	// DIR 1: DIRECTION LINEAR - stays as it is, no need to modify
	dir = 1;
	direction_init( dir );


	// DIR 2: DIRECTION_REVERSE on slot 2, i.e. dir = 1
	dir = 2;
	direction_init( dir );


	// DIR 3: DIRECTION_PINGPONG
	dir = 3;
	direction_init( dir );


	// DIR 4: DIRECTION_RANDOM on slot 3
	dir = 4;
	direction_init( dir );


	// DIR 5: DIRECTION_BROWNIAN on slot 4 - 66% fwd, 34% rev
	dir = 5;
	direction_init( dir );


	// DIR 6-ff: DIRECTION LINEAR - default setting
	for ( dir=6; dir < 17; dir++ ){

		direction_init( dir );
	}
}



// FIXME: this gets called twice at startup
// Initializes the GRID variables
void Grid_init(){

	unsigned int i=0, j=0;

	// Init the grid;
	Page_GRID_init();

	// Initialize the direction repository
	direction_repository_init();

	// Init the GRID_selection, _selection_buffer and _preselection
	for (i=0; i < GRID_NROF_BANKS; i++){

		// #warning RvdK: bad index: GRID_p_selection_buffer is declared as a MATRIX_NROF_ROWS deep vector.
		// GS: solved the above.
		GRID_p_selection_buffer[i]	= NULL;

		#ifdef FEATURE_ENABLE_SONG_UPE
		GRID_p_selection_cluster	= OFF;
		page_cluster_op				= 0;		
		#endif
		// Same function but different data models
		GRID_p_selection[i]			= NULL;
		GRID_p_preselection [i] 	= NULL;
		GRID_p_clock_presel [i]		= NULL;
	}

	// Init the GRID sets as empty
	for (i=0; i < GRID_NROF_SETS; i++){

		#ifdef FEATURE_ENABLE_SONG_UPE
		GRID_p_set_note_offsets[i] = 255;
		#endif

		for (j=0; j < GRID_NROF_BANKS; j++){

			GRID_p_set[i][j]  = NULL;
		}
	}

	// Set the grid cursor to page 1 of bank 1
#ifdef NEMO
	GRID_CURSOR = 0;
	#ifdef FEATURE_ENABLE_SONG_UPE
	PREV_GRID_CURSOR = 0;
	#endif
	// This is the default starting page, in row 0, which is selected.
	GRID_p_selection[0] 	= &Page_repository[GRID_CURSOR];
	GRID_p_preselection[0] 	= &Page_repository[GRID_CURSOR];
	GRID_p_clock_presel[0]	= &Page_repository[GRID_CURSOR];
//	Page_repository[GRID_CURSOR].page_clear = OFF;
#else
	GRID_CURSOR = 8;
	#ifdef FEATURE_ENABLE_SONG_UPE
	for (j=0; j<MATRIX_NROF_ROWS; j++){
		G_on_the_measure_track[j] = NULL;
	}
	PREV_GRID_CURSOR = 8;
	#endif	
	// This is the default starting page, in row 9, which is selected.
	GRID_p_selection[8] 	= &Page_repository[GRID_CURSOR];
	GRID_p_preselection[8] 	= &Page_repository[GRID_CURSOR];
	GRID_p_clock_presel[8]	= &Page_repository[GRID_CURSOR];
	Page_repository[GRID_CURSOR].page_clear = OFF;
#endif

	// Assign the PLAY_MODE_page pointers to real pages
	// these are the pages in the bottom matrix row..
	PLAY_MODE_page[8] = &Page_repository[89];
	PLAY_MODE_page[7] = &Page_repository[79];
	PLAY_MODE_page[6] = &Page_repository[69];
	PLAY_MODE_page[5] = &Page_repository[59];
	PLAY_MODE_page[4] = &Page_repository[49];
	PLAY_MODE_page[3] = &Page_repository[39];
	PLAY_MODE_page[2] = &Page_repository[29];
	PLAY_MODE_page[1] = &Page_repository[19];
	PLAY_MODE_page[0] = &Page_repository[9];


	// MIX TARGET of GRID assistant page
	GRID_assistant_page->mixTarget = MIXTGT_USR0;

	//
	// Define initial values of track pitches
	//
	#ifdef NEMO
	TRACK_DEFAULT_PITCH[0]	= 69;
	TRACK_DEFAULT_PITCH[1]	= 67;
	TRACK_DEFAULT_PITCH[2]	= 64;
	TRACK_DEFAULT_PITCH[3]	= 62;
	TRACK_DEFAULT_PITCH[4]	= 45;
	TRACK_DEFAULT_PITCH[5]	= 43;
	TRACK_DEFAULT_PITCH[6]	= 40;
	TRACK_DEFAULT_PITCH[7]	= 38;
	#else
	TRACK_DEFAULT_PITCH[0]	= 69;
	TRACK_DEFAULT_PITCH[1]	= 67;
	TRACK_DEFAULT_PITCH[2]	= 64;
	TRACK_DEFAULT_PITCH[3]	= 62;
	TRACK_DEFAULT_PITCH[4]	= 60;
	TRACK_DEFAULT_PITCH[5]	= 45;
	TRACK_DEFAULT_PITCH[6]	= 43;
	TRACK_DEFAULT_PITCH[7]	= 40;
	TRACK_DEFAULT_PITCH[8]	= 38;
	TRACK_DEFAULT_PITCH[9]	= 36;
	#endif
	
	#ifdef FEATURE_ENABLE_SONG_UPE
	// Allocate a 127 grid_cursor mapping table for special control track move operations
	unsigned short idx = 0;
	i = j = 0;
	for ( i=10; i < 160; i+=10 ){
		for ( j=0; j <= 8; j++ ){
//			diag_printf("idx:%d val:%d\n", idx, i+j);
			move_map[idx++] = i + j;
			if (idx == 128) goto out1;
		}
	}
	out1:
	i = j = idx = 0;
	for ( i=10; i <= 160; i+=10 ){
		for ( j=0; j <= 8; j++ ){
//			diag_printf("pidx:%d pval:%d\n", i+j, idx);
			move_page_map[i+j] = idx++;
			if (idx == 128) goto out2;
		}
	}
	out2:	
	#endif
//	// Copy the strum arrray contents from preset memory
//	//	memcpy( 	OCT_step_strum_STA,
//	//				&OCT_step_strum_STA_preset,
//	//				sizeof( char ) * 7 * MATRIX_NROF_ROWS );
//
//	// LINEAR running direction ( 16 16 10 )
//	for ( i=0; i < MATRIX_NROF_COLUMNS; i++ ){
//		for ( j=0; j< MATRIX_NROF_ROWS; j++ ){
//			if ( i == 0 ){
//				OCT_direction[0][0][j] = j+1;
//			}
//			else{
//				OCT_direction[0][0][j] = 0;
//			}
//		}
//	}
//
//	// REVERSE running direction
//	for ( i=0; i < MATRIX_NROF_COLUMNS; i++ ){
//		for ( j=0; j< MATRIX_NROF_ROWS; j++ ){
//			if ( i == 0 ){
//				OCT_direction[0][0][j] = MATRIX_NROF_ROWS - j;
//			}
//			else{
//				OCT_direction[0][0][j] = 0;
//			}
//		}
//	}

	// Initialise matrixScroll index
	page_window				= 0xF;
}



// Init the step, track and page data
void Octopus_memory_CLR(){

	unsigned int i = 0;

	Step_repository_init();

	Track_repository_init();

	Page_repository_init();

	Grid_init();

	#ifdef FEATURE_ENABLE_DICE
	Dice_init( DICE_bank );
	#endif

	MIR_init();

	// Init the DIAG field
	for ( i=0; i < 258; i++ ){

		DIAG[i] = OFF;
	}
	for ( i=0; i < 21; i++){

		DIAG_rotary_value[i] = 0;
	}

	// MIDI INIT
	G_midi_note_IN_UART[0][0] = 0xff;
	G_midi_note_IN_UART[0][1] = 0xff;
	G_midi_note_IN_UART[0][2] = 0xff;
	G_midi_note_IN_UART[1][0] = 0xff;
	G_midi_note_IN_UART[1][1] = 0xff;
	G_midi_note_IN_UART[1][2] = 0xff;

	G_midi_controller_IN_UART[0][0] = 0xff;
	G_midi_controller_IN_UART[0][1] = 0xff;
	G_midi_controller_IN_UART[0][2] = 0xff;
	G_midi_controller_IN_UART[1][0] = 0xff;
	G_midi_controller_IN_UART[1][1] = 0xff;
	G_midi_controller_IN_UART[1][2] = 0xff;

	// Init the strum levels
	PhraseInit();
}



// This is the memory initialization sequence
void Octopus_memory_init(){

	// INIT/CLR the repositories first
	Octopus_memory_CLR();

	// ASSIGN entities to each other
	Page_repository_assign_Steps();
	//	d_iag_printf("Page assign STEPS:        OK\n");
	Page_repository_assign_Tracks();
	//	d_iag_printf("Page assign TRACKS:       OK\n");

	// Build the panel logic here
	PANEL_build();
	//	d_iag_printf("Panel build:              OK\n");

	// Init the GRID
	Grid_init();

	// Init the MIDI event repository, get it ready to play
	MIDI_init();

	// Init the NOTE_IN event repository, get it ready to receive
	NOTE_IN_init();

	#ifdef FEATURE_ENABLE_DICE
	// Init Dice performance mode
	Dice_init( DICE_bank );
	#endif
	// ABLETON page init
//	ABLETON_init( &ABLETON_client );
}


// **********************************************************************
// REPOSITORY INIT functions
// **********************************************************************


// STEPS: Initialize the entire Step repository
void Step_repository_init(){

	unsigned int i=0;

	// STEP DEFAULT ATTRIBUTE VALUES - populate global catalog
	stepDefaultAttrValue[ATTR_STATUS]	= 	OFF;
	stepDefaultAttrValue[ATTR_VELOCITY] =	STEP_DEF_VELOCITY;
	stepDefaultAttrValue[ATTR_PITCH] 	= 	STEP_DEF_PITCH;
	stepDefaultAttrValue[ATTR_LENGTH] 	= 	STEP_DEF_LENGTH;
	stepDefaultAttrValue[ATTR_START]	= 	STEP_DEF_START;
	stepDefaultAttrValue[ATTR_AMOUNT]	=	STEP_DEF_AMOUNT;
	stepDefaultAttrValue[ATTR_GROOVE]	= 	STEP_DEF_GROOVE;
	stepDefaultAttrValue[ATTR_MIDICC]	=	STEP_DEF_MIDICC;


	// Initialize Steps as objects
	for (i=0; i<MAX_NROF_STEPS; i++){

		Step_init( &Step_repository[i] );
	}
	// d_iag_printf("initialized %d Steps..\n",i);
}


// TRACKS: Initalize the entire Track repository
void Track_repository_init(){

	unsigned int i=0;

  	for (i=0; i<MAX_NROF_TRACKS; i++){
 		Track_hard_init( &Track_repository[i], i );
  	}
}



//
// PAGES
//
void Page_repository_init(){
	unsigned int i=0;

	for (i=0; i<MAX_NROF_PAGES; i++){
 		PAGE_init( &Page_repository[i], i, True );
  	}

//	d_iag_printf("initialized %d Pages..\n",i);

}





// **********************************************************************
// OBJECT ASSIGNMENT functions
// **********************************************************************
void Page_repository_assign_Tracks(){

	// TRACKS->PAGES: Assign chunks of 16 Step Addresses to Track containers
	unsigned int i=0, j=0, offset=0;

	// Iterate through Pages
	for (i=0; i<MAX_NROF_PAGES; i++){

		// And assign the Tracks to the Page
		for (j=0; j<MATRIX_NROF_ROWS; j++){

			// Track assignment
			Page_repository[i].Track[j] 		= &Track_repository[j + offset];

//			// Track default pitch assignment - the defined scales
			Page_repository[i].Track[j]->attr_PIT = TRACK_DEFAULT_PITCH[j];

			// Track default pitch assignment
//			Page_repository[i].Track[j]->attr_PIT = TRACK_DEF_PITCH;

		}
		// Keep the running counter (explicit)
		offset += j;
	}
	// d_iag_printf("Assigned %d Tracks and OFFqueues to their Pages..\n", i*MATRIX_NROF_ROWS);
}



// STEPS->PAGES: Assign chunks of 160 Step addresses to the page Step container
void Page_repository_assign_Steps(){

	unsigned int page=0, row=0, col=0, offset=0;

	// For every page assign the Steps to the page in a 2dim array
	for (page=0; page<MAX_NROF_PAGES; page++){

		// For every row in the page
		for (row=0; row<MATRIX_NROF_ROWS; row++){

			// For every step in the row
			for (col=0; col<MATRIX_NROF_COLUMNS; col++){
				Page_repository[page].Step[row][col] = &Step_repository[col+offset];
			}
			offset += col;
		}
	}
}


// Page initialization function
void PAGE_init (Pagestruct* target_page, pageid_t pageId, booln firstInitBo ){

	unsigned int row=0;
	unsigned int i=0;

	target_page->locator 					= 0;
	target_page->editorMode					= EDIT;
	target_page->pageNdx 					= pageId;
	target_page->page_clear 				= ON;

	// Initialize the page Tracks
	for ( row=0; row < MATRIX_NROF_ROWS; row++ ){

		// Make the default pitch assignment
		target_page->Track[row]->attr_PIT = TRACK_DEFAULT_PITCH[row];
	}

	// Init the step selection pattern
	for ( i=0; i < 5; i++ ){

		for ( row=0; row < MATRIX_NROF_ROWS; row++ ){

			target_page->stepSELpattern[i][row] = 0;
		}
	}

	// Init the step selection index
	target_page->stepSELpattern_ndx = 0;

	#ifdef NEMO
	target_page->track_window				= 0xF;
	#endif
	target_page->trackSelection 			= 0;
	target_page->trackSelectionStored 		= 0;
	target_page->trackMutepattern 			= 0;
	target_page->trackMutepatternStored 	= 0;

	// Since this is the page constructor, we don't want to use the Page_setTrackRecPattern() function to update the global tracks-in-record counter.
	// So as an exception, use the private data member directly here.

	if ( firstInitBo ) {
		target_page->priv_track_REC_pattern	= 0;
	}
	else {
		Page_setTrackRecPattern( target_page, 0 );
	}

	target_page->REC_bit					= OFF;
	target_page->mixAttribute 				= ATTR_VELOCITY;
	target_page->editAttribute				= ATTR_VELOCITY;

	// 	StepSelection will be computed on the fly, flag in the STEP object
	target_page->stepSelection 				= 0;
	target_page->stepSelectionSingleRow		= 0;
	target_page->stepSelectionSingleCol		= 0;

	target_page->trackAttributeSelection 	= 0;
	target_page->stepAttributeSelection 	= 0;


	target_page->chainMode					= CHAINMODE_1;

//	// Initialize the page Track chain data
//	for ( row=0; row < MATRIX_NROF_ROWS; row++ ){
//
//		// Init the chain data for 10 unchained rows: each track is by itself
//		target_page->Track[row]->chain_data[HEAD] = target_page->Track[row];
//		target_page->Track[row]->chain_data[NEXT] = target_page->Track[row];
//		target_page->Track[row]->chain_data[PREV] = target_page->Track[row];
//		target_page->Track[row]->chain_data[PLAY] = target_page->Track[row];
//	}


	target_page->force_to_scale				= OFF;

	target_page->maxGroove					= 0;
	target_page->mixMode					= INC;

	target_page->scaleStatus				= OFF;

	for ( i=0; i< 9; i++){
		target_page->scaleNotes[i]			= SCALE_SIG_CHR;
		target_page->scaleLead[i]			= 1 << 11;  // this is equivalent to C
	}

	target_page->scaleLead_old				= 1 << 11;
	target_page->my_scale_signature			= 1;

	target_page->mixTarget					= MIXTGT_ATR;

	target_page->attr_STA					= 1;
	target_page->repeats_left 				= target_page->attr_STA ;

	// Init the page attribute data
	target_page->attr_VEL 	= PAGE_DEF_VEL;
	target_page->attr_PIT	= PAGE_DEF_PIT;
	target_page->attr_LEN 	= PAGE_DEF_LEN;
	target_page->attr_STA	= PAGE_DEF_STA;

	// Preset CC values
	for (i=0; i<MATRIX_NROF_ROWS; i++){
		target_page->MIXAMT_VOL[i] = 64;
		target_page->MIXAMT_PAN[i] = 64;
		target_page->MIXAMT_MOD[i] = 64;
		target_page->MIXAMT_EXP[i] = 64;
		target_page->MIXAMT_USR[i] = 64;
	}


	target_page->mixMasterStatus			= NEG;
	target_page->mixingTrack				= 0;

	// Init the CC_MIXMAP data
	for (i=0; i<6; i++) {

		init_CC_MIXMAP( target_page, i );
	}
	target_page->CC_MIXMAP_attribute = CC_MIXMAP_AMT;
	target_page->OPS_mode = INTERACTIVE;

	// Extract the current track pitches into the current scale bitpattern.
	// target_page->current_scale = extract_current_scale( target_page );

	// Init the EFF_pool values;
	for ( i=0; i < STEP_NROF_ATTRIBUTES; i++ ){

		target_page->EFF_pool[ i ] = 0;
	}


	target_page-> scaleLead[G_scale_ndx] = 1 << (NOTE_C - 12);
	target_page-> scaleNotes[G_scale_ndx] = SCALE_SIG_CHR;


	// Default assignment of mix2edit
	target_page->attr_mix2edit 		= ATTR_PITCH;
	target_page->attr_mix2edit_MIN 	= TRACK_MIN_PITCH;
	target_page->attr_mix2edit_MAX	= TRACK_MAX_PITCH;

	target_page->SEL_LOCK = OFF;

	target_page->SCL_align = OFF;

// #ifdef NEMO
//	target_page->CHAINS_PLAY_HEAD = TRUE;
// #else
	target_page->CHAINS_PLAY_HEAD = FALSE;
// #endif
	#ifdef FEATURE_ENABLE_KB_TRANSPOSE
	target_page->pitch_abs = FALSE;
	#endif
}



// Initialize the Page_GRID
void Page_GRID_init() {

	unsigned char side=0, ndx=0, color=0;

	for (side=0; side<2; side++) {
		for (ndx=0; ndx < 10; ndx++) {
			for (color=0; color<3; color++) {
				Page_GRID[side][ndx][color] = 0;
			}
		}
	}
}


// Initialize the MIR
void MIR_init() {

	unsigned int side=0, ndx=0, color=0;

	for (side=0; side<2; side++) {
		for (ndx=0; ndx < 18; ndx++) {
			for (color=0; color<3; color++) {
				MIR[side][ndx][color] = 0;
			}
		}
	}
}


// Initialize the passed MIDI event
void MIDIevent_init( MIDIeventstruct* in_event ){

	in_event->timestamp = (unsigned int) def_TIMESTAMP_MAX;
	in_event->type		= 0;
	in_event->val0		= 0;
	in_event->val1		= 0;
	in_event->val2		= 0;
	in_event->prev		= NULL;
	in_event->next 		= NULL;
}

// Initialize the passed MIDI event
void NOTEevent_init( NOTEeventstruct* in_event ){

	in_event->target_step 	= NULL;
	in_event->target_track	= NULL;
	in_event->timestamp		= 0;
	in_event->velocity		= 0;
	in_event->pitch			= 0;
	in_event->length		= 0;
	in_event->start			= 0;
	in_event->prev			= NULL;
	in_event->next 			= NULL;
}



// Initializes all events to be in the stack and the queue to be empty
void MIDI_init(){

	unsigned int i=0;

	// Setup the stack pointer
	MIDI_stack = NULL;

	// Push all events onto the stack
	for( i=0; i<MIDI_NROF_EVENTS; i++ ){

		// First init the event
		MIDIevent_init( &MIDIevent_repository[i] );

		// Push the event on the stack
		MIDI_stack_push( &MIDIevent_repository[i] );
	}

	//Queue now is empty.
	MIDI_queue_counter = 0;

	// Queue is virtually inexistent.
	MIDI_queue = NULL;
}



// Initializes all events to be in the stack and the queue to be empty
void NOTE_IN_init(){

	unsigned int i=0;

	// Setup the stack pointer to NULL -> start position!
	NOTE_stack = NULL;

	// Now push all events onto the stack
	for( i=0; i<NOTE_NROF_EVENTS; i++ ){

		// First init the event
		NOTEevent_init( &NOTEevent_repository[i] );

		// Push the event on the stack
		NOTE_stack_push( &NOTEevent_repository[i] );
	}

	//Queue now is empty.
//	NOTE_queue_counter = 0;

	// Queue is virtually inexistant.
	NOTE_queue = NULL;
}




// Progress bar
void show_progress_bar( unsigned char part, unsigned char color ){


#ifdef NEMO
	switch( part ){

		case 8:
			MIR_write_dot( 176,		color);
			MIR_write_dot( 165,		color);
			break;

		case 7:
			MIR_write_dot( 154,		color);
			MIR_write_dot( 143,		color);
			break;

		case 6:
			MIR_write_dot( 132,  	color);
			MIR_write_dot( 121,		color);
			break;

		case 5:
			MIR_write_dot( 110,		color);
			MIR_write_dot( 99,		color);
			break;

		case 4:
			MIR_write_dot( 88,		color);
			MIR_write_dot( 77,		color);
			break;

		case 3:
			MIR_write_dot( 66,		color);
			MIR_write_dot( 55,		color);
			break;

		case 2:
			MIR_write_dot( 44,		color);
			MIR_write_dot( 33,		color);
			break;

		case 1:
			MIR_write_dot( 22, 		color);
			MIR_write_dot( 11, 		color);
			break;
	}
#else
	switch( part ){

		case 8:
			MIR_write_dot( LED_NOTE_F,		color);
			MIR_write_dot( LED_NOTE_E,		color);
			break;

		case 7:
			MIR_write_dot( LED_NOTE_Dis,	color);
			MIR_write_dot( LED_NOTE_D,  	color);
			break;

		case 6:
			MIR_write_dot( LED_NOTE_Cis,	color);
			MIR_write_dot( LED_NOTE_C,		color);
			break;

		case 5:
			MIR_write_dot( LED_SCALE_MOD,	color);
			MIR_write_dot( LED_SCALE_SEL,	color);
			break;

		case 4:
			MIR_write_dot( LED_SCALE_CAD,	color);
			MIR_write_dot( LED_NOTE_Cup,	color);
			break;

		case 3:
			MIR_write_dot( LED_NOTE_B,		color);
			MIR_write_dot( LED_NOTE_Ais,	color);
			break;

		case 2:
			MIR_write_dot( LED_NOTE_A,		color);
			MIR_write_dot( LED_NOTE_Gis,	color);
			break;

		case 1:
			MIR_write_dot( LED_NOTE_G, 		color);
			MIR_write_dot( LED_NOTE_Fis, 	color);
			break;
	}
#endif

}
