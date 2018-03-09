
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

// Per default, all banks are in the RED trigger mode - i.e. manual
unsigned short 	GRID_bank_triggermode[GRID_NROF_BANKS] 
					= { RED, RED, RED, RED, RED, RED, RED, RED, RED };
					
// Bank mutepattern - not used anymore
unsigned short	GRID_bank_playmodes = GRID_BANK_SIMPLE;		// either _SIMPLE or _CHAIN


// GRID STRUCTURES
Pagestruct* 	GRID_p_selection 		[GRID_NROF_BANKS]; // Pointers of the pages that are active in the GRID at any time, one per GRID bank
Pagestruct* 	GRID_p_preselection	  	[GRID_NROF_BANKS];	// Stores the page indexes of the pages to be selected at the next oclock
Pagestruct* 	GRID_p_clock_presel	  	[GRID_NROF_BANKS];
Pagestruct* 	GRID_p_set				[GRID_NROF_SETS ][GRID_NROF_BANKS]; // Stores the GRID sets
Pagestruct* 	GRID_p_selection_buffer [MATRIX_NROF_ROWS]; 	// Temporary buffer for solo operations and other things


// This is the point of reference for GRID operations and Pages selections. 
// Defaults to 8, which is the first page of row 1.
unsigned int 	GRID_CURSOR		= 8;

// The attribute selected in Grid for MIX modification
unsigned short 	GRID_mixAttribute = 1 << VELOCITY; // initialized to VELOCITY

// GRID is by default in GRID_EDIT mode, but this flag can be changed and is static
// The other option is GRID_MIX
unsigned char 	GRID_play_mode	= GRID_MIX;

// Flag that indicates whether we are showing TEMPO or SELECT
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
unsigned char GRID_MAP0_status = OFF;

// Needed for Track chainning mute in the player
unsigned char SEQUENCER_JUST_STARTED = OFF;

// This is a buffer that stores pointers to MIDI off data - returned from track and to be played
// unsigned char* noteOFFdataBuffer [16];

unsigned char stepDefaultAttrValue[STEP_NROF_ATTRIBUTES];

// Stores the pointed to a selected step. Only one at a time may be selected.
// Stepstruct* STEP_COPY_BUFFER = NULL;
// --> moved to Step.h, after Stepstruct has been defined.


unsigned int TRACK_COPY_BUFFER 	= 0;	// Stores selected tracks in a page as follows:
// first ten bits keeps the track selection binary, then follows the source page ndx as decimal


// Stores the index of the page to be copied
// ..uses the GRID_SELECTION_EMPTY flag for empty
unsigned int PAGE_COPY_BUFFER	= GRID_SELECTION_EMPTY;	


// Contains 10 values for the tracks and one semantic value. 20 means empty.
// ..uses the GRID_SELECTION_EMPTY / EMPTY flag for empty
unsigned short ATTR_MAP_COPY_BUFFER[11] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
											EMPTY };


// Default listening to UART 0 for clock signals.
unsigned char CLOCK_MASTER_UART = 0;

// Indicated whether rowzero shows/edits the page repetition or lenght value
unsigned char GRID_rowzero_pagelength = FALSE;

// Define the level of strum to display and edit - array index to the below
unsigned char OCT_strum_view_level 	= 0;

// Can be one of ATTR_VELOCITY, ATTR_PITCH, ATTR_LENGTH, ATTR_START - relevant to Strum
unsigned char OCT_strum_attribute 	= ATTR_START;

// Preset values for the START of notes - cannot be changed by user
// Filled by copy operation from preset, which takes place in the memory init routine..
signed char OCT_step_strum_STA_preset		[7][MATRIX_NROF_ROWS] = 
				{		{0,  0,  0,  0,  0,  0,  0,  0,  0,  0},
						{0,  1,  1,  1,  1,  1,  2,  2,  2,  3},
						{0,  1,  1,  2,  2,  3,  4,  5,  6,  8},
						{0,  2,  2,  3,  4,  5,  7, 10, 13, 16},
						{0,  2,  3,  4,  6,  8, 12, 17, 23, 28},
						{0,  3,  4,  6,  9, 12, 19, 26, 36, 43},
						{0,  4,  5,  8, 13, 19, 25, 38, 48, 61}   };
signed char OCT_step_strum_STA		[7][MATRIX_NROF_ROWS] = 
				{		{0,  0,  0,  0,  0,  0,  0,  0,  0,  0},
						{0,  1,  1,  1,  1,  1,  2,  2,  2,  3},
						{0,  1,  1,  2,  2,  3,  4,  5,  6,  8},
						{0,  2,  2,  3,  4,  5,  7, 10, 13, 16},
						{0,  2,  3,  4,  6,  8, 12, 17, 23, 28},
						{0,  3,  4,  6,  9, 12, 19, 26, 36, 43},
						{0,  4,  5,  8, 13, 19, 25, 38, 48, 61}   };

// Values for the step strum; Hardwired [chordsize]x[strumlevel]
signed char OCT_step_strum_VEL		[7][MATRIX_NROF_ROWS] = 
				{		{0,  0,  0,  0,  0,  0,  0,  0,  0,  0},
						{0,  0,  0,  0,  0,  0,  0,  0,  0,  0},
						{0,  0,  0,  0,  0,  0,  0,  0,  0,  0},
						{0,  0,  0,  0,  0,  0,  0,  0,  0,  0},
						{0,  0,  0,  0,  0,  0,  0,  0,  0,  0},
						{0,  0,  0,  0,  0,  0,  0,  0,  0,  0},
						{0,  0,  0,  0,  0,  0,  0,  0,  0,  0}   };

signed char OCT_step_strum_PIT		[7][MATRIX_NROF_ROWS] = 
				{		{0,  0,  0,  0,  0,  0,  0,  0,  0,  0},
						{0,  0,  0,  0,  0,  0,  0,  0,  0,  0},
						{0,  0,  0,  0,  0,  0,  0,  0,  0,  0},
						{0,  0,  0,  0,  0,  0,  0,  0,  0,  0},
						{0,  0,  0,  0,  0,  0,  0,  0,  0,  0},
						{0,  0,  0,  0,  0,  0,  0,  0,  0,  0},
						{0,  0,  0,  0,  0,  0,  0,  0,  0,  0}   };

signed char OCT_step_strum_LEN		[7][MATRIX_NROF_ROWS] = 
				{		{0,  0,  0,  0,  0,  0,  0,  0,  0,  0},
						{0,  0,  0,  0,  0,  0,  0,  0,  0,  0},
						{0,  0,  0,  0,  0,  0,  0,  0,  0,  0},
						{0,  0,  0,  0,  0,  0,  0,  0,  0,  0},
						{0,  0,  0,  0,  0,  0,  0,  0,  0,  0},
						{0,  0,  0,  0,  0,  0,  0,  0,  0,  0},
						{0,  0,  0,  0,  0,  0,  0,  0,  0,  0}   };

// Holds global direction data: [direction_ndx:16][direction_position:16][trigger:10]
unsigned char OCT_direction [16][16][10]; 

// Trigger stack structure
typedef struct ts {

	// Trigger stack in the natural orde
	unsigned char trigger[10];

	// probability of moving fwd or bwd
	unsigned char certainty_next;

	// bit for fully random next position - overrides certainty
	unsigned char randomizer; 
} Trigger_Stack;


// Direction structure
typedef struct td {

	Trigger_Stack trigger_set[17];
} Track_Direction;

// This is the direction repository
Track_Direction direction_repository[17];

// Playlist used by the player as a temporary buffer
Trackstruct* Track_playlist[ MATRIX_NROF_ROWS ];

// CCMAP_learner - the index of the learner track in a CC map learning mode
unsigned char CCMAP_learner = 0;

