

// Holds the GRID-relevant data to be stored with a memory image and imported likewise
typedef struct grid_flatrecord_struct{
		
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
	unsigned char CHAINS_PLAY_HEAD;	

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

	// Flat record of the grid assistant page
	Page_flatrecord_struct assistant_page;

} Grid_flatrecord_struct;


// This holds the full memory data
typedef struct memory_image_struct{
	
	Page_flatrecord_struct	 Page_flatrecord_repository	[MAX_NROF_PAGES];	
	Grid_flatrecord_struct   Grid_flatrecord_buffer;

} Memory_image_struct;

Memory_image_struct memory_image;



//______________________________________________________________________________________________
// Imports the grid data into the memory image
void Grid_import_from_flatrecord_buffer(){
	
	TEMPO = memory_image.Grid_flatrecord_buffer.TEMPO; 	//120 default
	// Refill for 16th steps: (50Mhz/4steps_per_beat * 60Secs)/TEMPO
	TIMER_REFILL = memory_image.Grid_flatrecord_buffer.TIMER_REFILL;

	CLOCK_SOURCE = memory_image.Grid_flatrecord_buffer.CLOCK_SOURCE;	// Can be any of OFF, INT(ernal), EXT(ernal)
						
	// This probably should be zoomGRID but may use zoomPAGE for convenience
	zoom_level = memory_image.Grid_flatrecord_buffer.zoom_level;
			
	// G R I D implementation
	GRID_scene = memory_image.Grid_flatrecord_buffer.GRID_scene;		// Currently selected grid scene for play or storage
	
	// Mode of selection. Either GRID_SWITCH_DIRECT or GRID_SWITCH_OCLOCK.
	GRID_switch_mode = memory_image.Grid_flatrecord_buffer.GRID_switch_mode;		

	// Keeps the triggermodes for the individual grid lines. Bit set means on	
	memcpy( GRID_bank_triggermode,
			memory_image.Grid_flatrecord_buffer.GRID_bank_triggermode, 
			sizeof( unsigned char ) * GRID_NROF_BANKS );
	
	// This is the point of reference for GRID operations and Pages selections. Defaults to 1.
	GRID_CURSOR = memory_image.Grid_flatrecord_buffer.CURSOR;
	
	// The attribute selected in Grid for MIX modification
	 GRID_mixAttribute = memory_image.Grid_flatrecord_buffer.GRID_mixAttribute;
	
	// GRID is by default in GRID_EDIT mode, but this flag can be changed and is static
	// The other option is GRID_MIX
	GRID_play_mode = memory_image.Grid_flatrecord_buffer.GRID_play_mode;
	
	GRID_bank_playmodes = memory_image.Grid_flatrecord_buffer.GRID_bank_playmodes;		// either _SIMPLE or _CHAIN
		
	// One of RECALL or CLEAR. Variable has to be int because of the KEY_xxx #defs.
	// Semantics: current GRID selection kept in a set, may be recalled or cleared - i.e. overwritten by the current grid selection.
	GRID_set_switchmode = memory_image.Grid_flatrecord_buffer.GRID_set_switchmode;
	
	// Default is the first set
	current_GRID_set = memory_image.Grid_flatrecord_buffer.current_GRID_set;
	
	// These are the GRID CC MAPS - they are generally assignable
	// GRID_CC_MIXMAP[6][10][3];
//	memcpy( GRID_CC_MIXMAP, memory_image.Grid_flatrecord_buffer.GRID_CC_MIXMAP, 180 );
	
//	GRID_CC_MIXMAP_attribute = memory_image.Grid_flatrecord_buffer.GRID_CC_MIXMAP_attribute;	// Indicates which attribute is shown for GRID_CC_MIXMAPS
//	GRID_mixTarget = memory_image.Grid_flatrecord_buffer.GRID_mixTarget;				// Specifies the mix knob target in GRID mode.
	GRID_OPS_mode = memory_image.Grid_flatrecord_buffer.GRID_OPS_mode;
	GRIDTRACK_OPS_mode = memory_image.Grid_flatrecord_buffer.GRIDTRACK_OPS_mode;
	
	// unsigned char 	stepDefaultAttrValue[STEP_NROF_ATTRIBUTES];
	memcpy( &stepDefaultAttrValue, 
			memory_image.Grid_flatrecord_buffer.stepDefaultAttrValue, 
			sizeof( unsigned char ) * STEP_NROF_ATTRIBUTES );
		
	TRACK_COPY_BUFFER = memory_image.Grid_flatrecord_buffer.TRACK_COPY_BUFFER;			// Stores selected tracks in a page as follows:
	// first ten bits keeps the track selection binary, then follows the source page ndx as decimal
	
	PAGE_COPY_BUFFER = memory_image.Grid_flatrecord_buffer.PAGE_COPY_BUFFER;	// Stores the index of the page to be copied
	// ..uses the GRID_SELECTION_EMPTY flag for empty
						
	// FOLLOW variables - finally reduced to a radio button: FOLLOW_GRID, FOLLOW_PAGE, FOLLOW_TRACK
	follow_flag = memory_image.Grid_flatrecord_buffer.follow_flag;
				
	// Lauflicht status holder - ON means Lauflicht is visible, OFF otherwise
	CHAINS_PLAY_HEAD = memory_image.Grid_flatrecord_buffer.CHAINS_PLAY_HEAD;	
	
	// Grid snapshot copy
	memcpy( GRID_p_selection,
			memory_image.Grid_flatrecord_buffer.GRID_p_selection,
			sizeof( Pagestruct* ) * GRID_NROF_BANKS );

	memcpy( GRID_p_preselection,
			memory_image.Grid_flatrecord_buffer.GRID_p_preselection,
			sizeof( Pagestruct* ) * GRID_NROF_BANKS );

	memcpy( GRID_p_clock_presel,
			memory_image.Grid_flatrecord_buffer.GRID_p_clock_presel,
			sizeof( Pagestruct* ) * GRID_NROF_BANKS );

	memcpy( GRID_p_selection_buffer,
			memory_image.Grid_flatrecord_buffer.GRID_p_selection_buffer,
			sizeof( Pagestruct* ) * MATRIX_NROF_ROWS );
	

	// Level of strum to display and edit - array index to the below
	OCT_strum_view_level 	= memory_image.Grid_flatrecord_buffer.OCT_strum_view_level;
	OCT_strum_attribute		= memory_image.Grid_flatrecord_buffer.OCT_strum_attribute;

	// Values for the step strum
	memcpy( OCT_step_strum_VEL,
			memory_image.Grid_flatrecord_buffer.OCT_step_strum_VEL,
			sizeof( char ) * 7 * MATRIX_NROF_ROWS );

	memcpy( OCT_step_strum_PIT,
			memory_image.Grid_flatrecord_buffer.OCT_step_strum_PIT,
			sizeof( char ) * 7 * MATRIX_NROF_ROWS );

	memcpy( OCT_step_strum_LEN,
			memory_image.Grid_flatrecord_buffer.OCT_step_strum_LEN,
			sizeof( char ) * 7 * MATRIX_NROF_ROWS );

	memcpy( OCT_step_strum_STA,
			memory_image.Grid_flatrecord_buffer.OCT_step_strum_STA,
			sizeof( char ) * 7 * MATRIX_NROF_ROWS );

	// Direction data
	memcpy( direction_repository,
			memory_image.Grid_flatrecord_buffer.direction_repository,
			sizeof( Track_Direction ) * 17 );

	// GRID ASSISTANT PAGE export to grid structure
	Page_import_from_flatrecord_buffer(	&memory_image.Grid_flatrecord_buffer.assistant_page, 
										GRID_assistant_page );
}



//______________________________________________________________________________________________
// Exports the grid data into the memory image
void Grid_export_to_flatrecord_buffer(){
	
	memory_image.Grid_flatrecord_buffer.TEMPO = TEMPO; 	//120 default

	// Refill for 16th steps: (50Mhz/4steps_per_beat * 60Secs)/TEMPO
	memory_image.Grid_flatrecord_buffer.TIMER_REFILL = TIMER_REFILL;
	
	memory_image.Grid_flatrecord_buffer.CLOCK_SOURCE = CLOCK_SOURCE;	// Can be any of OFF, INT(ernal), EXT(ernal)
						
	// This probably should be zoomGRID but may use zoomPAGE for convenience
	memory_image.Grid_flatrecord_buffer.zoom_level = zoom_level;
			
	// G R I D implementation
	memory_image.Grid_flatrecord_buffer.GRID_scene = GRID_scene;		// Currently selected grid scene for play or storage
	
	// Mode of selection. Either GRID_SWITCH_DIRECT or GRID_SWITCH_OCLOCK.
	memory_image.Grid_flatrecord_buffer.GRID_switch_mode = GRID_switch_mode;		

	// Keeps the triggermodes for the individual grid lines. Bit set means on	
	memcpy( memory_image.Grid_flatrecord_buffer.GRID_bank_triggermode, 
			GRID_bank_triggermode, 
			sizeof( unsigned char ) * GRID_NROF_BANKS );
	
	// This is the point of reference for GRID operations and Pages selections. Defaults to 1.
	memory_image.Grid_flatrecord_buffer.CURSOR = GRID_CURSOR;
	
	// The attribute selected in Grid for MIX modification
	memory_image.Grid_flatrecord_buffer.GRID_mixAttribute = GRID_mixAttribute;
	
	// GRID is by default in GRID_EDIT mode, but this flag can be changed and is static
	// The other option is GRID_MIX
	memory_image.Grid_flatrecord_buffer.GRID_play_mode = GRID_play_mode;
	
	memory_image.Grid_flatrecord_buffer.GRID_bank_playmodes = GRID_bank_playmodes;		// either _SIMPLE or _CHAIN
		
	// One of RECALL or CLEAR. Variable has to be int because of the KEY_xxx #defs.
	// Semantics: current GRID selection kept in a set, may be recalled or cleared - i.e. overwritten by the current grid selection.
	memory_image.Grid_flatrecord_buffer.GRID_set_switchmode = GRID_set_switchmode;
	
	// Default is the first set
	memory_image.Grid_flatrecord_buffer.current_GRID_set = current_GRID_set;
	
	memory_image.Grid_flatrecord_buffer.GRID_OPS_mode = GRID_OPS_mode;
	memory_image.Grid_flatrecord_buffer.GRIDTRACK_OPS_mode = GRIDTRACK_OPS_mode;
	
	// unsigned char 	stepDefaultAttrValue[STEP_NROF_ATTRIBUTES];
	memcpy( memory_image.Grid_flatrecord_buffer.stepDefaultAttrValue, 
			&stepDefaultAttrValue, 
			sizeof( unsigned char ) * STEP_NROF_ATTRIBUTES );
		
	memory_image.Grid_flatrecord_buffer.TRACK_COPY_BUFFER = TRACK_COPY_BUFFER;			// Stores selected tracks in a page as follows:
	// first ten bits keeps the track selection binary, then follows the source page ndx as decimal
	
	memory_image.Grid_flatrecord_buffer.PAGE_COPY_BUFFER = PAGE_COPY_BUFFER;	// Stores the index of the page to be copied
	// ..uses the GRID_SELECTION_EMPTY flag for empty
						
	// FOLLOW variables - finally reduced to a radio button: FOLLOW_GRID, FOLLOW_PAGE, FOLLOW_TRACK
	memory_image.Grid_flatrecord_buffer.follow_flag = follow_flag;
				
	// Lauflicht status holder - ON means Lauflicht is visible, OFF otherwise
	memory_image.Grid_flatrecord_buffer.CHAINS_PLAY_HEAD = CHAINS_PLAY_HEAD;	

	// Grid snapshot copy into the flashbuffer
	memcpy( &(memory_image.Grid_flatrecord_buffer.GRID_p_selection),
			&GRID_p_selection,
			sizeof( Pagestruct* ) * GRID_NROF_BANKS );

	memcpy( &(memory_image.Grid_flatrecord_buffer.GRID_p_preselection),
			&GRID_p_preselection,
			sizeof( Pagestruct* ) * GRID_NROF_BANKS );

	memcpy( &(memory_image.Grid_flatrecord_buffer.GRID_p_clock_presel),
			&GRID_p_clock_presel,
			sizeof( Pagestruct* ) * GRID_NROF_BANKS );

	memcpy( memory_image.Grid_flatrecord_buffer.GRID_p_selection_buffer,
			&GRID_p_selection_buffer,
			sizeof( Pagestruct* ) * MATRIX_NROF_ROWS );


	// Level of strum to display and edit - array index to the below
	memory_image.Grid_flatrecord_buffer.OCT_strum_view_level 	= OCT_strum_view_level;
	memory_image.Grid_flatrecord_buffer.OCT_strum_attribute		= OCT_strum_attribute;
	
	// Values for the step strum -> 7*10 character array
	memcpy( memory_image.Grid_flatrecord_buffer.OCT_step_strum_VEL,
			&OCT_step_strum_VEL,
			sizeof( char ) * 7 * MATRIX_NROF_ROWS );

	memcpy( memory_image.Grid_flatrecord_buffer.OCT_step_strum_PIT,
			&OCT_step_strum_PIT,
			sizeof( char ) * 7 * MATRIX_NROF_ROWS );

	memcpy( memory_image.Grid_flatrecord_buffer.OCT_step_strum_LEN,
			&OCT_step_strum_LEN,
			sizeof( char ) * 7 * MATRIX_NROF_ROWS );

	memcpy( memory_image.Grid_flatrecord_buffer.OCT_step_strum_STA,
			&OCT_step_strum_STA,
			sizeof( char ) * 7 * MATRIX_NROF_ROWS );

	// Direction data
	memcpy( memory_image.Grid_flatrecord_buffer.direction_repository,
			&direction_repository,
			sizeof( Track_Direction ) * 17 );

	// GRID ASSISTANT PAGE export to grid structure
	Page_export_to_flatrecord_buffer( 	GRID_assistant_page, 
										&memory_image.Grid_flatrecord_buffer.assistant_page );
}

