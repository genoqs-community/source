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


//#include "../_OCT_global/defs_general.h"



// Holds GRID relevant variables
//
// G R I D implementation
//
unsigned char 	Page_GRID[2][10][3];						// The Page GRID. Used by the VIEWER
	
unsigned char 	GRID_status = GRID_DEFAULT; 				// One of DEFAULT, SELECT, EXPORT
unsigned char	GRID_scene = 0;								// Currently selected grid scene for play or storage
unsigned char 	GRID_target_scene = 0;						// Target grid scene index for export operation

// Mode of selection. Either GRID_SWITCH_DIRECT or GRID_SWITCH_OCLOCK.
unsigned char	GRID_switch_mode = GRID_SWITCH_OCLOCK;		
// Keeps the triggermodes for the individual grid lines. Bit set means on

//// Per default, all banks are in the RED trigger mode - i.e. manual
//unsigned short 	GRID_bank_triggermode[GRID_NROF_BANKS] 
//					= { RED, RED, RED, RED, RED, RED, RED, RED, RED };
					
// Bank mutepattern - not used anymore
unsigned short	GRID_bank_playmodes = GRID_BANK_SIMPLE;		// either _SIMPLE or _CHAIN


// GRID STRUCTURES
Pagestruct* 	GRID_p_selection 		[GRID_NROF_BANKS]; // Pointers of the pages that are active in the GRID at any time, one per GRID bank
Pagestruct* 	GRID_p_preselection	  	[GRID_NROF_BANKS];	// Stores the page indexes of the pages to be selected at the next oclock
Pagestruct* 	GRID_p_clock_presel	  	[GRID_NROF_BANKS];
Pagestruct* 	GRID_p_set				[GRID_NROF_SETS ][GRID_NROF_BANKS]; // Stores the GRID sets
unsigned char	GRID_p_set_note_offsets	[GRID_NROF_SETS ];
unsigned char   GRID_p_set_midi_ch = 0; // Stores the global midi channel for note events
char			GRID_p_set_note_presel = 255; // Stores the set index of the note to send on the measure


// Page cluster selection active flag
unsigned char	GRID_p_selection_cluster	= OFF;

// Hacky way to index a move target using a char
// Rule: if pgmch is 0 bankch = row index counting from top to bottom
// Rule: else bankch = map value
// Exclusions: col 16 is not a valid move target, row 0 is not a valid move target
unsigned char move_map[127];
unsigned char move_page_map[127];
unsigned char page_cluster_op 				= 0;

// Used for page cluster selection criteria
unsigned char 	PREV_GRID_CURSOR			= 8;

#ifdef FEATURE_ENABLE_SONG_UPE
Pagestruct* 	GRID_p_selection_buffer [GRID_NROF_BANKS]; 	// Temporary buffer for solo operations and other things

Trackstruct*	MIX_TRACK					= NULL;
#else
Pagestruct* 	GRID_p_selection_buffer [MATRIX_NROF_ROWS]; 	// Temporary buffer for solo operations and other things
#endif

// This is the point of reference for GRID operations and Pages selections. 
// Defaults to 8, which is the first page of row 1.
unsigned char 	GRID_CURSOR		= 8;

// The attribute selected in Grid for MIX modification
unsigned short 	GRID_mixAttribute = 1 << VELOCITY; // initialized to VELOCITY

// GRID is by default in GRID_EDIT mode, but this flag can be changed and is static
// The other option is GRID_MIX
unsigned char 	GRID_play_mode	= GRID_MIX;

// Flag that indicates whether we are showing G_master_tempo or SELECT
unsigned char 	GRID_bigknob_SELECT 	= OFF;

// One of RECALL or CLEAR. 
// Variable has to be int because of the KEY_xxx #defs.
// Semantics: current GRID selection kept in a set, may be recalled 
// or cleared - i.e. overwritten by the current grid selection.
unsigned int GRID_set_switchmode	= SEL_RECALL;

// Default is the first set
unsigned char current_GRID_set = 0;

// GRID CC MAPS ARE HANDLED BY THE GRID ASSISTANT PAGE - which is in the page repository.

unsigned char GRID_OPS_mode = INTERACTIVE;
unsigned char GRIDTRACK_OPS_mode = INTERACTIVE;

// Flag indicates whether we are using the grid maps globally or only in the GRID.
unsigned char GRID_MAP0_status = ON;

// Needed for Track chainning mute in the player
unsigned char SEQUENCER_JUST_STARTED = OFF;

// Set to false after play is started the first time
unsigned char SEQUENCER_JUST_RESTARTED = ON;

// Stores the default values of the step attributes
unsigned char stepDefaultAttrValue[STEP_NROF_ATTRIBUTES];

// Stores selected tracks in a page as follows:
// first ten bits keeps the track selection binary, then follows the source page ndx as decimal
unsigned int TRACK_COPY_BUFFER 	= 0;	

// Stores the index of the page to be copied
// ..uses the GRID_SELECTION_EMPTY flag for empty
unsigned int PAGE_COPY_BUFFER	= GRID_SELECTION_EMPTY;	


// Contains 10 values for the tracks and one semantic value. 20 means empty.
// ..uses the GRID_SELECTION_EMPTY / EMPTY flag for empty
unsigned short ATTR_MAP_COPY_BUFFER[11] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, EMPTY };


// Default listening to UART 0 for clock signals.
unsigned char CLOCK_MASTER_UART = 0;

// Indicated whether rowzero shows/edits the page repetition or lenght value
unsigned char GRID_rowzero_pagelength = FALSE;

// This is the direction repository
Track_Direction direction_repository[17];

// Playlist used by the player as a temporary buffer
Trackstruct* Track_playlist[ MATRIX_NROF_ROWS ];

// CCMAP_learner - the index of the learner track in a CC map learning mode
unsigned char CCMAP_learner = 0;

// Tells whether the tracks are showing program or bank change - global flag
unsigned char CHANGE_BANK_OR_PGM = PGM;

// NOT USED! -> substituted by a link to the global tempo.
unsigned char SYSEX_dump_speed = 15; // Normal setting is 50

// Temp variable to store the preview step pointer
Stepstruct* page_preview_step = NULL;
unsigned char page_preview_step_col = 0;
unsigned char page_preview_step_row = 0;


// Buffer for track tempo multiplier changes
unsigned char track_tempo_mul = 0;
unsigned char track_tempo_div = 0;
Trackstruct*  track_tempo_ptr = NULL;
Pagestruct*   track_tempo_page = NULL;


// GRID mutepattern as suggested by John Kimble
unsigned short GRID_mutepattern = 0;

// GRID editor mode, to support GRID edit mode a la page edit mode
unsigned char GRID_editmode = ON;

// GRID editor mode, to support GRIDTRACK edit mode a la page edit mode
unsigned char GRIDTRACK_editmode = ON;

unsigned char CC_resolution_delay	[8][6] =
	{	{ 0, 0, 0, 0, 0, 0 },
		{ 0, 0, 0, 0, 0, 0 },
		{ 6, 0, 0, 0, 0, 0 },
		{ 4, 8, 0, 0, 0, 0 },
		{ 3, 6, 9, 0, 0, 0 },
		{ 2, 5, 8, 10, 0,0 },
		{ 2, 4, 6, 8, 10,0 },
		{ 2, 4, 5, 7, 8,10 }	};

unsigned char MIDICLOCK_PASSTHROUGH = FALSE;




#ifdef NEMO
	// This is the pointer to the editable attribute in NEMO Track mode
	unsigned short NEMO_selectedTrackAttribute = NEMO_ATTR_PITCH;
	unsigned short NEMO_selectedStepAttribute  = NEMO_ATTR_PITCH;

	// Indicates what is currently selected as a track modification target
	unsigned char NEMO_track_VFF = VFF_VALUE;
	unsigned char NEMO_step_VER =  VER_VALUE;
#endif
