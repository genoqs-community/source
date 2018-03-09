

// Holds the GRID-relevant data to be stored with a memory image and imported likewise
typedef struct grid_image_struct{
		
	unsigned char 	TEMPO; 	//120 default
	unsigned char	CLOCK_SOURCE;	// Can be any of OFF, INT(ernal), EXT(ernal)
		
	// Refill for 16th steps: (50Mhz/4steps_per_beat * 60Secs)/TEMPO
	unsigned int 	TIMER_REFILL;
				
	// This probably should be zoomGRID but may use zoomPAGE for convenience
	unsigned char 	zoom_level;
			
	// G R I D implementation
	unsigned char	GRID_scene;		// Currently selected grid scene for play or storage
	
	// Mode of selection. Either GRID_SWITCH_DIRECT or GRID_SWITCH_OCLOCK.
	unsigned char	GRID_switch_mode;		

	// Keeps the triggermodes for the individual grid lines. Bit set means on	
	// Per default, all banks are in the RED trigger mode - i.e. manual
	unsigned short 	GRID_bank_triggermode[GRID_NROF_BANKS];
	
	// This is the point of reference for GRID operations and Pages selections. Defaults to 1.
	unsigned int 	CURSOR;
	
	// The attribute selected in Grid for MIX modification
	unsigned short 	GRID_mixAttribute;
	
	// GRID is by default in GRID_EDIT mode, but this flag can be changed and is static
	// The other option is GRID_MIX
	unsigned char 	GRID_play_mode;
	
	unsigned short	GRID_bank_playmodes;		// either _SIMPLE or _CHAIN
		
	// One of RECALL or CLEAR. Variable has to be int because of the KEY_xxx #defs.
	// Semantics: current GRID selection kept in a set, may be recalled or cleared - i.e. overwritten by the current grid selection.
	unsigned int 	GRID_set_switchmode;
	
	// Default is the first set
	unsigned char 	current_GRID_set;
	
	// Specifies the ops mode of the GRID/TRACK: INTERACTIVE or PASSIVE
	unsigned char 	GRID_OPS_mode;
	unsigned char 	GRIDTRACK_OPS_mode;
	
	unsigned char 	stepDefaultAttrValue[STEP_NROF_ATTRIBUTES];
		
	unsigned int 	TRACK_COPY_BUFFER;			// Stores selected tracks in a page as follows:
	// first ten bits keeps the track selection binary, then follows the source page ndx as decimal
	
	unsigned int 	PAGE_COPY_BUFFER;	// Stores the index of the page to be copied
	// ..uses the GRID_SELECTION_EMPTY flag for empty
	
	// ATTR MAP copy buffer - values 0-9 correspond to the track values, value 10 is the attribute (semantic flag)
	unsigned short ATTR_MAP_COPY_BUFFER [11];
	// ..uses the GRID_SELECTION_EMPTY flag for empty
						
	// FOLLOW variables - finally reduced to a radio button: FOLLOW_GRID, FOLLOW_PAGE, FOLLOW_TRACK
	unsigned char 	follow_flag;
				
	// Lauflicht status holder - ON means Lauflicht is visible, OFF otherwise
	unsigned char LAUFLICHT_DISPLAY;	

	Pagestruct* 	GRID_p_selection 		[GRID_NROF_BANKS];  // Pointers of the pages that are active in the GRID at any time, one per GRID bank
	Pagestruct* 	GRID_p_preselection		[GRID_NROF_BANKS];  // Pointers of the pages that are preselected in the GRID at any time, one per GRID bank
	Pagestruct* 	GRID_p_clock_presel		[GRID_NROF_BANKS];  // Pointers of the pages that are preselected in the GRID at any time, one per GRID bank
	Pagestruct* 	GRID_p_selection_buffer [MATRIX_NROF_ROWS]; // Temporary buffer for solo operations and other things


	// Define the level of strum to display and edit - basically an array index to the below
	unsigned char OCT_strum_view_level;
	unsigned char OCT_strum_attribute;

	// Values for the step strum; Hardwired [chordsize]x[strumlevel]
	signed char OCT_step_strum_VEL	[7][MATRIX_NROF_ROWS];
	signed char OCT_step_strum_PIT	[7][MATRIX_NROF_ROWS];
	signed char OCT_step_strum_LEN	[7][MATRIX_NROF_ROWS];
	signed char OCT_step_strum_STA	[7][MATRIX_NROF_ROWS];
	
	// This is the direction repository
	Track_Direction direction_repository[17];

} Grid_flashimage_struct;


