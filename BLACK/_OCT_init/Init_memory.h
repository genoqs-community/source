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

// Solve Compilation dependencies  
extern void PAGE_init(Pagestruct* target_page, unsigned int ndx);
extern void MIDI_init();
extern void NOTE_IN_init();
extern unsigned short extract_current_scale( Pagestruct* target_page );
extern void grid_select( Pagestruct* target_page, unsigned char target_status );

//extern unsigned char offset_to_next_current_degree(		Pagestruct* target_page, 
//														unsigned char pitch );


// UTILITY FUNCTIONS
extern void extract_scale_and_lead( 		Pagestruct* target_page );
extern void export_scale_degrees( 			Pagestruct* target_page );
extern void export_scale_lead( 				Pagestruct* target_page );


// **********************************************************************
// MEMORY ALLOCATION for the entire object repository
// **********************************************************************

// STEPS
Stepstruct 			Step_repository		[MAX_NROF_STEPS];

// TRACKS
Trackstruct 		Track_repository	[MAX_NROF_TRACKS];

// CADENCES - one per page
// Cadencestruct		Cadence_repository	[MAX_NROF_PAGES];

// PAGES
Pagestruct 			Page_repository		[MAX_NROF_PAGES];

// Used to be the manager of page pointer assignments
Pagestruct* 		VIEWER_page;

// Used to see the step even when changing positions around..
// Stepstruct* VIEWER_step = NULL;


// Used to keep some PAGE like information to be used by the GRID
Pagestruct* GRID_assistant_page = &Page_repository[159];

// Used mainly as a copy buffer for sorting of tracks (ALN) in MIX MAP (ATR) mode
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



// This is a utility variable for the GRID mode. Q+D - Can do better than this.
// Pagestruct* GRID_focus_page = NULL;

// Buffer for save and retrieve from FLASH storage
// Page_flashimage_struct	 Page_flashbuffer_repository	[MAX_NROF_PAGES];

// This holds the full memory data
typedef struct memory_image_struct{
	
	Page_flashimage_struct	 Page_flashbuffer_repository	[MAX_NROF_PAGES];
	Grid_flashimage_struct   Grid_flashbuffer;
} Memory_image_struct;

Memory_image_struct memory_image;


// Initialie the Grid flashbuffer to all zeroes
void Grid_flashbuffer_init(){

	memset( &(memory_image.Grid_flashbuffer), 0, sizeof( Grid_flashimage_struct ) );
}




// Exports the grid data into the memory image
void Grid_import_from_flashbuffer(){
	
	TEMPO = memory_image.Grid_flashbuffer.TEMPO; 	//120 default
	// Refill for 16th steps: (50Mhz/4steps_per_beat * 60Secs)/TEMPO
	TIMER_REFILL = memory_image.Grid_flashbuffer.TIMER_REFILL;

	CLOCK_SOURCE = memory_image.Grid_flashbuffer.CLOCK_SOURCE;	// Can be any of OFF, INT(ernal), EXT(ernal)
						
	// This probably should be zoomGRID but may use zoomPAGE for convenience
	zoom_level = memory_image.Grid_flashbuffer.zoom_level;
			
	// G R I D implementation
	GRID_scene = memory_image.Grid_flashbuffer.GRID_scene;		// Currently selected grid scene for play or storage
	
	// Mode of selection. Either GRID_SWITCH_DIRECT or GRID_SWITCH_OCLOCK.
	GRID_switch_mode = memory_image.Grid_flashbuffer.GRID_switch_mode;		

	// Keeps the triggermodes for the individual grid lines. Bit set means on	
	// Per default, all banks are in the RED trigger mode - i.e. manual
//	for ( i=0; i<GRID_NROF_BANKS; i++ ){
//		memory_image.Grid_flashbuffer.GRID_bank_triggermode[i] = GRID_bank_triggermode[i];
//	}
	
	memcpy( GRID_bank_triggermode,
			memory_image.Grid_flashbuffer.GRID_bank_triggermode, 
			sizeof( unsigned char ) * GRID_NROF_BANKS );
	
	// This is the point of reference for GRID operations and Pages selections. Defaults to 1.
	GRID_CURSOR = memory_image.Grid_flashbuffer.CURSOR;
	
	// The attribute selected in Grid for MIX modification
	 GRID_mixAttribute = memory_image.Grid_flashbuffer.GRID_mixAttribute;
	
	// GRID is by default in GRID_EDIT mode, but this flag can be changed and is static
	// The other option is GRID_MIX
	GRID_play_mode = memory_image.Grid_flashbuffer.GRID_play_mode;
	
	GRID_bank_playmodes = memory_image.Grid_flashbuffer.GRID_bank_playmodes;		// either _SIMPLE or _CHAIN
		
	// One of RECALL or CLEAR. Variable has to be int because of the KEY_xxx #defs.
	// Semantics: current GRID selection kept in a set, may be recalled or cleared - i.e. overwritten by the current grid selection.
	GRID_set_switchmode = memory_image.Grid_flashbuffer.GRID_set_switchmode;
	
	// Default is the first set
	current_GRID_set = memory_image.Grid_flashbuffer.current_GRID_set;
	
	// These are the GRID CC MAPS - they are generally assignable
	// GRID_CC_MIXMAP[6][10][3];
//	memcpy( GRID_CC_MIXMAP, memory_image.Grid_flashbuffer.GRID_CC_MIXMAP, 180 );
	
//	GRID_CC_MIXMAP_attribute = memory_image.Grid_flashbuffer.GRID_CC_MIXMAP_attribute;	// Indicates which attribute is shown for GRID_CC_MIXMAPS
//	GRID_mixTarget = memory_image.Grid_flashbuffer.GRID_mixTarget;				// Specifies the mix knob target in GRID mode.
	GRID_OPS_mode = memory_image.Grid_flashbuffer.GRID_OPS_mode;
	GRIDTRACK_OPS_mode = memory_image.Grid_flashbuffer.GRIDTRACK_OPS_mode;
	
	// unsigned char 	stepDefaultAttrValue[STEP_NROF_ATTRIBUTES];
	memcpy( &stepDefaultAttrValue, 
			memory_image.Grid_flashbuffer.stepDefaultAttrValue, 
			sizeof( unsigned char ) * STEP_NROF_ATTRIBUTES );
		
	TRACK_COPY_BUFFER = memory_image.Grid_flashbuffer.TRACK_COPY_BUFFER;			// Stores selected tracks in a page as follows:
	// first ten bits keeps the track selection binary, then follows the source page ndx as decimal
	
	PAGE_COPY_BUFFER = memory_image.Grid_flashbuffer.PAGE_COPY_BUFFER;	// Stores the index of the page to be copied
	// ..uses the GRID_SELECTION_EMPTY flag for empty
						
	// FOLLOW variables - finally reduced to a radio button: FOLLOW_GRID, FOLLOW_PAGE, FOLLOW_TRACK
	follow_flag = memory_image.Grid_flashbuffer.follow_flag;
				
	// Lauflicht status holder - ON means Lauflicht is visible, OFF otherwise
	LAUFLICHT_DISPLAY = memory_image.Grid_flashbuffer.LAUFLICHT_DISPLAY;	
	
		// Grid snapshot copy
	memcpy( GRID_p_selection,
			memory_image.Grid_flashbuffer.GRID_p_selection,
			sizeof( Pagestruct* ) * GRID_NROF_BANKS );

	memcpy( GRID_p_preselection,
			memory_image.Grid_flashbuffer.GRID_p_preselection,
			sizeof( Pagestruct* ) * GRID_NROF_BANKS );

	memcpy( GRID_p_clock_presel,
			memory_image.Grid_flashbuffer.GRID_p_clock_presel,
			sizeof( Pagestruct* ) * GRID_NROF_BANKS );

	memcpy( GRID_p_selection_buffer,
			memory_image.Grid_flashbuffer.GRID_p_selection_buffer,
			sizeof( Pagestruct* ) * MATRIX_NROF_ROWS );
	

	// Level of strum to display and edit - array index to the below
	OCT_strum_view_level 	= memory_image.Grid_flashbuffer.OCT_strum_view_level;
	OCT_strum_attribute		= memory_image.Grid_flashbuffer.OCT_strum_attribute;

	// Values for the step strum
	memcpy( OCT_step_strum_VEL,
			memory_image.Grid_flashbuffer.OCT_step_strum_VEL,
			sizeof( char ) * 7 * MATRIX_NROF_ROWS );

	memcpy( OCT_step_strum_PIT,
			memory_image.Grid_flashbuffer.OCT_step_strum_PIT,
			sizeof( char ) * 7 * MATRIX_NROF_ROWS );

	memcpy( OCT_step_strum_LEN,
			memory_image.Grid_flashbuffer.OCT_step_strum_LEN,
			sizeof( char ) * 7 * MATRIX_NROF_ROWS );

	memcpy( OCT_step_strum_STA,
			memory_image.Grid_flashbuffer.OCT_step_strum_STA,
			sizeof( char ) * 7 * MATRIX_NROF_ROWS );


	// Direction data
	memcpy( direction_repository,
			memory_image.Grid_flashbuffer.direction_repository,
			sizeof( Track_Direction ) * 17 );
}



// Exports the grid data into the memory image
void Grid_export_to_flashbuffer(){
	
	memory_image.Grid_flashbuffer.TEMPO = TEMPO; 	//120 default
	// Refill for 16th steps: (50Mhz/4steps_per_beat * 60Secs)/TEMPO
	memory_image.Grid_flashbuffer.TIMER_REFILL = TIMER_REFILL;
	
	memory_image.Grid_flashbuffer.CLOCK_SOURCE = CLOCK_SOURCE;	// Can be any of OFF, INT(ernal), EXT(ernal)
						
	// This probably should be zoomGRID but may use zoomPAGE for convenience
	memory_image.Grid_flashbuffer.zoom_level = zoom_level;
			
	// G R I D implementation
	memory_image.Grid_flashbuffer.GRID_scene = GRID_scene;		// Currently selected grid scene for play or storage
	
	// Mode of selection. Either GRID_SWITCH_DIRECT or GRID_SWITCH_OCLOCK.
	memory_image.Grid_flashbuffer.GRID_switch_mode = GRID_switch_mode;		

	// Keeps the triggermodes for the individual grid lines. Bit set means on	
	// Per default, all banks are in the RED trigger mode - i.e. manual
//	for ( i=0; i<GRID_NROF_BANKS; i++ ){
//		memory_image.Grid_flashbuffer.GRID_bank_triggermode[i] = GRID_bank_triggermode[i];
//	}
	
	memcpy( memory_image.Grid_flashbuffer.GRID_bank_triggermode, 
			GRID_bank_triggermode, 
			sizeof( unsigned char ) * GRID_NROF_BANKS );
	
	// This is the point of reference for GRID operations and Pages selections. Defaults to 1.
	memory_image.Grid_flashbuffer.CURSOR = GRID_CURSOR;
	
	// The attribute selected in Grid for MIX modification
	memory_image.Grid_flashbuffer.GRID_mixAttribute = GRID_mixAttribute;
	
	// GRID is by default in GRID_EDIT mode, but this flag can be changed and is static
	// The other option is GRID_MIX
	memory_image.Grid_flashbuffer.GRID_play_mode = GRID_play_mode;
	
	memory_image.Grid_flashbuffer.GRID_bank_playmodes = GRID_bank_playmodes;		// either _SIMPLE or _CHAIN
		
	// One of RECALL or CLEAR. Variable has to be int because of the KEY_xxx #defs.
	// Semantics: current GRID selection kept in a set, may be recalled or cleared - i.e. overwritten by the current grid selection.
	memory_image.Grid_flashbuffer.GRID_set_switchmode = GRID_set_switchmode;
	
	// Default is the first set
	memory_image.Grid_flashbuffer.current_GRID_set = current_GRID_set;
	
	// These are the GRID CC MAPS - they are generally assignable
	// GRID_CC_MIXMAP[6][10][3];
//	memcpy( memory_image.Grid_flashbuffer.GRID_CC_MIXMAP, GRID_CC_MIXMAP, 180 );
	
//	memory_image.Grid_flashbuffer.GRID_CC_MIXMAP_attribute = GRID_CC_MIXMAP_attribute;	// Indicates which attribute is shown for GRID_CC_MIXMAPS
//	memory_image.Grid_flashbuffer.GRID_mixTarget = GRID_mixTarget;				// Specifies the mix knob target in GRID mode.
	memory_image.Grid_flashbuffer.GRID_OPS_mode = GRID_OPS_mode;
	memory_image.Grid_flashbuffer.GRIDTRACK_OPS_mode = GRIDTRACK_OPS_mode;
	
	// unsigned char 	stepDefaultAttrValue[STEP_NROF_ATTRIBUTES];
	memcpy( memory_image.Grid_flashbuffer.stepDefaultAttrValue, 
			&stepDefaultAttrValue, 
			sizeof( unsigned char ) * STEP_NROF_ATTRIBUTES );
		
	memory_image.Grid_flashbuffer.TRACK_COPY_BUFFER = TRACK_COPY_BUFFER;			// Stores selected tracks in a page as follows:
	// first ten bits keeps the track selection binary, then follows the source page ndx as decimal
	
	memory_image.Grid_flashbuffer.PAGE_COPY_BUFFER = PAGE_COPY_BUFFER;	// Stores the index of the page to be copied
	// ..uses the GRID_SELECTION_EMPTY flag for empty
						
	// FOLLOW variables - finally reduced to a radio button: FOLLOW_GRID, FOLLOW_PAGE, FOLLOW_TRACK
	memory_image.Grid_flashbuffer.follow_flag = follow_flag;
				
	// Lauflicht status holder - ON means Lauflicht is visible, OFF otherwise
	memory_image.Grid_flashbuffer.LAUFLICHT_DISPLAY = LAUFLICHT_DISPLAY;	

	// Grid snapshot copy into the flashbuffer
	memcpy( &(memory_image.Grid_flashbuffer.GRID_p_selection),
			&GRID_p_selection,
			sizeof( Pagestruct* ) * GRID_NROF_BANKS );

	memcpy( &(memory_image.Grid_flashbuffer.GRID_p_preselection),
			&GRID_p_preselection,
//			&GRID_p_selection,
			sizeof( Pagestruct* ) * GRID_NROF_BANKS );

	memcpy( &(memory_image.Grid_flashbuffer.GRID_p_clock_presel),
			&GRID_p_clock_presel,
//			&GRID_p_selection,
			sizeof( Pagestruct* ) * GRID_NROF_BANKS );

	memcpy( memory_image.Grid_flashbuffer.GRID_p_selection_buffer,
			&GRID_p_selection_buffer,
//			&GRID_p_selection,
			sizeof( Pagestruct* ) * MATRIX_NROF_ROWS );


	// Level of strum to display and edit - array index to the below
	memory_image.Grid_flashbuffer.OCT_strum_view_level 	= OCT_strum_view_level;
	memory_image.Grid_flashbuffer.OCT_strum_attribute	= OCT_strum_attribute;
	
	// Values for the step strum -> 7*10 character array
	memcpy( memory_image.Grid_flashbuffer.OCT_step_strum_VEL,
			&OCT_step_strum_VEL,
			sizeof( char ) * 7 * MATRIX_NROF_ROWS );

	memcpy( memory_image.Grid_flashbuffer.OCT_step_strum_PIT,
			&OCT_step_strum_PIT,
			sizeof( char ) * 7 * MATRIX_NROF_ROWS );

	memcpy( memory_image.Grid_flashbuffer.OCT_step_strum_LEN,
			&OCT_step_strum_LEN,
			sizeof( char ) * 7 * MATRIX_NROF_ROWS );

	memcpy( memory_image.Grid_flashbuffer.OCT_step_strum_STA,
			&OCT_step_strum_STA,
			sizeof( char ) * 7 * MATRIX_NROF_ROWS );


	// Direction data
	memcpy( memory_image.Grid_flashbuffer.direction_repository,
			&direction_repository,
			sizeof( Track_Direction ) * 17 );
}



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
	
				direction_repository[ dir ].trigger_set[ set ].trigger[1] = 17 - set;
				direction_repository[ dir ].trigger_set[ set ].certainty_next = 100;
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
			// Iterate through the sets in dir 1 and set all triggers to 0
			for ( set=1; set < 17; set++ ){
	
				// Init all triggers to 0 - which produces random positions all over..
				for (i=0; i<10; i++){
					direction_repository[ dir ].trigger_set[ set ].trigger[i] = 0;
				}			
			}
			break;
			
	
		case 5:
			// Iterate through the sets in dir 1 and set all triggers to 0
			for ( set=1; set < 17; set++ ){
	
				// Triggers can stay the same as for LINEAR, only certainty_next adjusted
				direction_repository[ dir ].trigger_set[ set ].certainty_next = 66;
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

	//
	// PROGRAMMATIC SETTING OF THE DIRECTIONS
	//

	// DIR 0: not used
	
	// DIR 1: DIRECTION LINEAR - stays as it is, no need to modify
	dir = 1;
	direction_init( dir );

//		// Iterate through the sets in dir 1
//		for ( set=1; set < 17; set++ ){
//
//			direction_repository[ dir ].trigger_set[ set ].trigger[1] = set;
//			direction_repository[ dir ].trigger_set[ set ].certainty_next = 100;
//		}


	// DIR 2: DIRECTION_REVERSE on slot 2, i.e. dir = 1
	dir = 2;
	direction_init( dir );
	
//		// Iterate through the sets in dir 1
//		for ( set=1; set < 17; set++ ){
//
//			direction_repository[ dir ].trigger_set[ set ].trigger[1] = 17 - set;
//			direction_repository[ dir ].trigger_set[ set ].certainty_next = 100;
//		}


	// DIR 3: DIRECTION_PINGPONG
	dir = 3;
	direction_init( dir );

//		// Iterate through the sets in dir 2
//		for ( set=1; set < 9; set++ ){
//
//			direction_repository[ dir ].trigger_set[ set ].trigger[1] = (set * 2) - 1;
//			direction_repository[ dir ].trigger_set[ set ].trigger[2] =  set * 2;
//
//			direction_repository[ dir ].trigger_set[ set ].certainty_next = 100;
//		}
//		for ( set=9; set < 17; set++ ){
//
//			direction_repository[ dir ].trigger_set[ set ].trigger[1] =  (17 - set) * 2;
//			direction_repository[ dir ].trigger_set[ set ].trigger[2] = ((17 - set) * 2) - 1;
//
//			direction_repository[ dir ].trigger_set[ set ].certainty_next = 100;
//		}


	// DIR 4: DIRECTION_RANDOM on slot 3
	dir = 4;
	direction_init( dir );

//		// Iterate through the sets in dir 1 and set all triggers to 0
//		for ( set=1; set < 17; set++ ){
//
//			// Init all triggers to 0 - which produces random positions all over..
//			for (i=0; i<10; i++){
//				direction_repository[ dir ].trigger_set[ set ].trigger[i] = 0;
//			}			
//		}

		
	// DIR 5: DIRECTION_BROWNIAN on slot 4 - 66% fwd, 34% rev
	dir = 5;
	direction_init( dir );

//		// Iterate through the sets in dir 1 and set all triggers to 0
//		for ( set=1; set < 17; set++ ){
//
//			// Triggers can stay the same as for LINEAR, only certainty_next adjusted
//			direction_repository[ dir ].trigger_set[ set ].certainty_next = 66;
//		}


	// DIR 6-ff: DIRECTION LINEAR - default setting
	for ( dir=6; dir < 17; dir++ ){

		direction_init( dir );

//		// Iterate through the sets in dir 1
//		for ( set=1; set < 17; set++ ){
//
//			direction_repository[ dir ].trigger_set[ set ].trigger[1] = set;
//			direction_repository[ dir ].trigger_set[ set ].certainty_next = 100;
//		}
	}
}



// Initializes the GRID variables
void Grid_init(){

	unsigned int i=0, j=0;

	// Init the grid;
	Page_GRID_init();

	// Initialize the direction repository
	direction_repository_init();

	// Init the GRID_selection, _selection_buffer and _preselection
	for (i=0; i < GRID_NROF_BANKS; i++){
		
		GRID_p_selection_buffer[i]	= NULL;
	
		// Same function but different data models
		GRID_p_selection[i]			= NULL;
		GRID_p_preselection [i] 	= NULL;
		GRID_p_clock_presel [i]		= NULL;
	}
	
	// Init the GRID sets as empty
	for (i=0; i < GRID_NROF_SETS; i++){
		
		for (j=0; j < GRID_NROF_BANKS; j++){
			
			GRID_p_set[i][j]  = NULL;
		}
	}

	// Set the grid cursor to page 1 of bank 1
	GRID_CURSOR = 8;

	// This is the default starting page, in row 9, which is selected. 
	GRID_p_selection[8] 	= &Page_repository[GRID_CURSOR];
	GRID_p_preselection[8] 	= &Page_repository[GRID_CURSOR];
	GRID_p_clock_presel[8]	= &Page_repository[GRID_CURSOR];	
	
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
	GRID_assistant_page->mixTarget = MIXTGT_USR1;
	
	//
	// Define initial values of track pitches
	//
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

	// Copy the strum arrray contents from preset memory
	//	memcpy( 	OCT_step_strum_STA, 
	//				&OCT_step_strum_STA_preset, 
	//				sizeof( char ) * 7 * MATRIX_NROF_ROWS );

	// LINEAR running direction ( 16 16 10 )
	for ( i=0; i < MATRIX_NROF_COLUMNS; i++ ){	
		for ( j=0; j< MATRIX_NROF_ROWS; j++ ){
			if ( i == 0 ){
				OCT_direction[0][0][j] = j+1;
			}
			else{
				OCT_direction[0][0][j] = 0;
			}
		}
	}

	// REVERSE running direction
	for ( i=0; i < MATRIX_NROF_COLUMNS; i++ ){	
		for ( j=0; j< MATRIX_NROF_ROWS; j++ ){
			if ( i == 0 ){
				OCT_direction[0][0][j] = MATRIX_NROF_ROWS - j;
			}
			else{
				OCT_direction[0][0][j] = 0;
			}
		}
	}

}


// Initialize the strum levels
void strumlevels_init(){

	unsigned char i=0;
	unsigned char j=0;

	// Iterate through all OCT_strum_view_levels
	for ( j=0; j < 7; j++ ){

		// Make  VEL entries neutral				
		for ( i=0; i<MATRIX_NROF_ROWS; i++ ){
			OCT_step_strum_VEL	[j][i] = 0;
		}
	
		// Make all PIT entries neutral				
		for ( i=0; i<MATRIX_NROF_ROWS; i++ ){
			OCT_step_strum_PIT	[j][i] = 0;
		}
	
		for ( i=0; i<MATRIX_NROF_ROWS; i++ ){
			OCT_step_strum_LEN	[j][i] = 0;
		}
	
		for ( i=0; i<MATRIX_NROF_ROWS; i++ ){
			OCT_step_strum_STA	[j][i] = OCT_step_strum_STA_preset  [j][i];
		}
	}

}


// Init the step, track and page data
void Octopus_memory_CLR(){

	unsigned int i = 0;

	Step_repository_init();
//	d_iag_printf("STEP repository init:     OK\n");

	Track_repository_init();
//	d_iag_printf("TRACK repository init:    OK\n");

	Page_repository_init();
//	d_iag_printf("PAGE repository init:     OK\n");
	
	Page_flashbuffer_repository_init();
//	d_iag_printf("PAGE_flashbuffer init:    OK\n");	

	Grid_init();
//	d_iag_printf("GRID init:    			   OK\n");	

	Grid_flashbuffer_init();
//	d_iag_printf("GRID_flashbuffer init:    OK\n");	

	MIR_init();
//	d_iag_printf("MIR init:                 OK\n");

	// Init the DIAG field
	for ( i=0; i < 258; i++ ){

		DIAG[i] = OFF;
	}
	for ( i=0; i < 21; i++){

		DIAG_rotary_value[i] = 0;
	}

	// MIDI INIT
	midi_note_IN_UART[0][0] = 0xff;
	midi_note_IN_UART[0][1] = 0xff;
	midi_note_IN_UART[0][2] = 0xff;
	midi_note_IN_UART[1][0] = 0xff;
	midi_note_IN_UART[1][1] = 0xff;
	midi_note_IN_UART[1][2] = 0xff;

	midi_controller_IN_UART[0][0] = 0xff;
	midi_controller_IN_UART[0][1] = 0xff;
	midi_controller_IN_UART[0][2] = 0xff;
	midi_controller_IN_UART[1][0] = 0xff;
	midi_controller_IN_UART[1][1] = 0xff;
	midi_controller_IN_UART[1][2] = 0xff;

	// Init the strum levels
	strumlevels_init();
}



// This is the memory initialization sequence
void Octopus_memory_init(){
	
	unsigned int i=0;

	//
	// INIT/CLR the repositories first
	// 
	Octopus_memory_CLR();

	// 
	// ASSIGN entities to each other
	// 
	Page_repository_assign_Steps();
//	d_iag_printf("Page assign STEPS:        OK\n");

	Page_repository_assign_Tracks();
//	d_iag_printf("Page assign TRACKS:       OK\n");

//	Page_repository_assign_Cadences();
// 	d_iag_printf("Page assign CADENCES:	   OK\n");

	// Build the panel logic here
	PANEL_build();
//	d_iag_printf("Panel build:              OK\n");
	
	// Init the GRID
	Grid_init();

	// Finally extract the initial scale data
	// And export it into the page scale variables	
	for (i=0; i<MAX_NROF_PAGES; i++) {
	
		// extract_scale_and_lead( &Page_repository[i] );
		// export_scale_degrees( &Page_repository[i] );
		// export_scale_lead( &Page_repository[i] );
	}

	// Init the MIDI event repository, get it ready to play
	MIDI_init();
	// Init the NOTE_IN event repository, get it ready to receive
	NOTE_IN_init();
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

		Step_init(&Step_repository[i]);
	}
	// d_iag_printf("initialized %d Steps..\n",i);
}


// TRACKS: Initalize the entire Track repository
void Track_repository_init(){

	unsigned int i=0;
	
	for (i=0; i<MAX_NROF_TRACKS; i++){

		Track_init(&Track_repository[i]);
	}
	// d_iag_printf("initialized %d Tracks..\n",i);
}



//
// PAGES
//
void Page_repository_init(){
	unsigned int i=0;
	
	for (i=0; i<MAX_NROF_PAGES; i++){
		
		PAGE_init( &Page_repository[i], i );
		
//		d_iag_printf("init done Page:%d at:%x q:%x pi[0][0]=%d\n",
//					i, 
//					&Page_repository[i],
//					&Page_repository[i].noteOFF_queue[0],
//					&Page_repository[i].noteOFF_queue[0]->pitch[0][0]
//					);
	}

//	d_iag_printf("initialized %d Pages..\n",i);

}




void Page_flashbuffer_init( Page_flashimage_struct* target_page ){
	
	unsigned char 	i=0,
					j=0,
					k=0;

	// Init the tracks and the steps
	for (i=0; i<MATRIX_NROF_ROWS; i++) {
		
		// Track_init( &target_page->Track[i] );
		
		// Init the steps
		for (j=0; j<MATRIX_NROF_COLUMNS; j++) {
			Step_init( &target_page->Step[i][j] );
		}		
	}


	// Copy all the other page attributes
//	target_page->locator 				= 0;
	target_page->editorMode				= 0;
	target_page->bigKnobAmt				= 0;
	target_page->page_clear				= ON;
	target_page->keyMode				= 0;
	target_page->chainMode				= 0;
	target_page->chainCounter			= 0;
	target_page->trackSelection			= 0;
	target_page->trackSelectionStored	= 0;
	target_page->stepSelection			= 0;
	target_page->stepSelectionSingleRow = 0;
	target_page->stepSelectionSingleCol	= 0;
	target_page->trackMutepattern		= 0;
	target_page->trackMutepatternStored = 0;
	target_page->trackSolopattern		= 0;
	target_page->track_REC_pattern		= 0;
	target_page->trackZoomAttribute		= 0;
	target_page->editAttribute			= 0;
	target_page->mixAttribute			= 0;
	target_page->mixMode				= 0;
	target_page->mixMasterStatus		= 0;
	target_page->mixingTrack			= 0;
	target_page->trackAttributeSelection= 0;
	target_page->stepAttributeSelection = 0;

	target_page->force_to_scale			= FALSE;
	target_page->grid_FTS_exempt		= FALSE;

	target_page->maxGroove				= 0;
	target_page->mixTarget				= 0;
	target_page->CC_MIXMAP_attribute	= 0;
	target_page->scaleStatus			= OFF;
	target_page->current_scale			= SCALE_SIG_CHR;		// 
	target_page->editMode				= 0;
	target_page->recordMode				= ON;
	target_page->cadenceStatus			= 0;
	
	// 9 User scales in page
	for ( i=0; i < 9; i++ ){

		target_page->scaleNotes[i]		= 0;
		target_page->scaleLead[i]		= 0;
	}

	target_page->scaleLead_old			= 0;
	target_page->scaleNotes_old			= 0;
	

	for (i=0; i<PAGE_NROF_ATTRIBUTES; i++) {
		target_page->attribute[i] 		= 0;
	}
	target_page->attribute[ATTR_LENGTH]	= PAGE_DEF_LENGTH;
	target_page->attribute[ATTR_PITCH]	= PAGE_DEF_PITCH;


	for (i=0; i<10; i++) {
		target_page->MIXAMT_VOL[i] 		= 0;
		target_page->MIXAMT_PAN[i] 		= 0;
		target_page->MIXAMT_MOD[i] 		= 0;
		target_page->MIXAMT_EXP[i] 		= 0;
		target_page->MIXAMT_USR[i] 		= 0;		

		for (j=0; j<6; j++) {
			for (k=0; k<3; k++) {
				target_page->CC_MIXMAP[j][i][k] = 0;
			}
		}
	}
	target_page->OPS_mode				= INTERACTIVE;
}



// Init the flashbuffer repository
void Page_flashbuffer_repository_init() {
	
	unsigned int i=0;
	
	// Init all the page flashbuffers
	for (i=0; i<MAX_NROF_PAGES; i++){
		
		// Init the Page_flashbuffers
		Page_flashbuffer_init( &(memory_image.Page_flashbuffer_repository[i]) );
	}
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
			Page_repository[i].Track[j]->attribute[ATTR_PITCH] = TRACK_DEFAULT_PITCH[j];

			// Track default pitch assignment
//			Page_repository[i].Track[j]->attribute[ATTR_PITCH] = TRACK_DEF_PITCH;

		}
		// Keep the running counter (explicit)
		offset += j;			
	}
	// d_iag_printf("Assigned %d Tracks and OFFqueues to their Pages..\n", i*MATRIX_NROF_ROWS);
}



void Page_repository_assign_Steps(){
// STEPS->PAGES: Assign chunks of 160 Step addresses to the page Step container
	unsigned int page=0, row=0, col=0, offset=0;

	// For every page
	for (page=0; page<MAX_NROF_PAGES; page++){

		// Assign the Steps to the page in a 2dim array
		// For every row in the page
		for (row=0; row<MATRIX_NROF_ROWS; row++){

			// For every step in the row
			for (col=0; col<MATRIX_NROF_COLUMNS; col++){
				Page_repository[page].Step[row][col] = &Step_repository[col+offset];				
				// Page_repository[page].Step[row][col] = &Step_repository[col+offset];				
			}
			offset += col;
		}
	}
}


// Page initialization function
void PAGE_init (Pagestruct* target_page, unsigned int ndx){
	
	unsigned int row=0, i=0, j=0, k=0;

	target_page->locator 					= 0;	
	target_page->editorMode					= EDIT;
	target_page->pageNdx 					= ndx;
	target_page->page_clear 				= ON;
	
	// Initialize the page Tracks
	for ( row=0; row < MATRIX_NROF_ROWS; row++ ){
		
//		Track_init( target_page->Track[row] );

		// Make the default pitch assignment
		target_page->Track[row]->attribute[ATTR_PITCH] = TRACK_DEFAULT_PITCH[row];
	}	

	target_page->keyMode 					= KMOD_PAGE;
	target_page->trackSelection 			= 0;
	target_page->trackSelectionStored 		= 0;
	target_page->trackMutepattern 			= 0;
	target_page->trackMutepatternStored 	= 0;
	target_page->track_REC_pattern			= 0;
	target_page->REC_bit					= 0;
	target_page->mixAttribute 				= ATTR_VELOCITY;
	target_page->editAttribute				= ATTR_VELOCITY;

	// 	StepSelection will be computed on the fly, flag in the STEP object
	target_page->stepSelection 				= 0;
	target_page->stepSelectionSingleRow		= 0;
	target_page->stepSelectionSingleCol		= 0;
		
	target_page->trackAttributeSelection 	= 0;
	target_page->stepAttributeSelection 	= 0;
	
	target_page->chainMode					= CHAINMODE_1;
	target_page->chainCounter				= 0;
	target_page->force_to_scale				= OFF;
	target_page->grid_FTS_exempt			= FALSE;
	
	target_page->maxGroove					= 0;
	target_page->mixMode					= INC;
	
//	target_page->bigKnobAmt					= 1;

	target_page->scaleStatus				= OFF;
	
	for ( i=0; i< 9; i++){
		target_page->scaleNotes[i]			= SCALE_SIG_CHR;
		target_page->scaleLead[i]			= 1 << 11;  // this is equivalent to C
	}

	target_page->scaleLead_old				= 1 << 11;
	target_page->current_scale				= SCALE_SIG_CHR;
	target_page->editMode					= 0;
	target_page->recordMode					= ON;
//	target_page->cadenceStatus				= OFF;
	
	target_page->mixTarget					= MIXTGT_ATR;

	target_page->repeats					= 1;
	target_page->repeats_left 				= target_page->repeats ;

	// Init the page attribute data
	for (i=0; i<PAGE_NROF_ATTRIBUTES; i++) {
		target_page->attribute[i] 			= 0;
	}	
	target_page->attribute[ATTR_LENGTH] 	= PAGE_DEF_LENGTH;
	target_page->attribute[ATTR_PITCH]		= PAGE_DEF_PITCH;

		
	// Preset CC values
	for (i=0; i<MATRIX_NROF_ROWS; i++){
		target_page->MIXAMT_VOL[i] = 64;
		target_page->MIXAMT_PAN[i] = 64;
		target_page->MIXAMT_MOD[i] = 64;
		target_page->MIXAMT_EXP[i] = 64;		
		target_page->MIXAMT_USR[i] = 64;		
	}

	// Force to GRID scale	
	target_page->grid_FTS_exempt = FALSE;

	target_page->mixMasterStatus			= NEG;
	target_page->mixingTrack				= 0;
	
	// Init the CC_MIXMAP data 
	for (i=0; i<6; i++) {
		for (j=0; j<10; j++) {
			for (k=0; k<3; k++) {

				switch( k ){
					case CC_MIXMAP_MCH:
						target_page->CC_MIXMAP[i][j][k] =  1;
						break;

					case CC_MIXMAP_AMT:
						target_page->CC_MIXMAP[i][j][k] = 64;
						break;
					
					default:
						target_page->CC_MIXMAP[i][j][k] =  MIDICC_NONE;
						break;												
				}

			}
		}
	}	
	target_page->CC_MIXMAP_attribute = CC_MIXMAP_AMT;
	target_page->OPS_mode = INTERACTIVE;
	
	// Extract the current track pitches into the current scale bitpattern.
	// target_page->current_scale = extract_current_scale( target_page );	

	// Init the EFF_pool values;
	for ( i=0; i < STEP_NROF_ATTRIBUTES; i++ ){
	
		target_page->EFF_pool[ i ] = 0;
	}


	target_page->scaleLead[0] = 1 << (NOTE_C - 12);
	target_page->scaleNotes[0] = SCALE_SIG_CHR;

// IMPORTED FROM THE KEY PRESS SECTION

//		// Select the new scale lead
//		target_page->scaleLead[0] = 1 << (NOTE_C - 12);
//
//		// Shift the signature according to the lead
//		target_page->scaleNotes[0] = 
//			my_shift_bitpattern( 	target_page->scaleNotes[0], 
//									12, 
//									INC, 
//									(11 - my_bit2ndx(target_page->scaleLead[0] ) ) 
//								);
//
//		//	d_iag_printf( "SCL SEL: shifted/lead:%d/defaultPEN/adjusted:%d/\n", 
//		//									my_bit2ndx( VIEWER_page->scaleLead )+1 );
//		//	my_print_bits32( VIEWER_page->scaleNotes );
//		//	my_print_bits32( VIEWER_page->scaleLead );
//		//	my_print_bits32( SCALE_SIG_PEN );	
//		//	my_print_bits32( my_shift_bitpattern( 	VIEWER_page->scaleNotes, 12, INC,
//		//												my_bit2ndx( VIEWER_page->scaleLead )+1
//		//												)
//		//					);
//
//
//		// Export the changes to the tracks
//		target_page->scaleLead_old = target_page->scaleLead[0];
//		target_page->scaleNotes_old = target_page->scaleNotes[0];
//
//		// Simply update the current scale with what is selected in the octave circle
//		target_page->current_scale = target_page->scaleNotes[0];
//

	// Default assignment of mix2edit
	target_page->attr_mix2edit 		= ATTR_PITCH;
	target_page->attr_mix2edit_MIN 	= TRACK_MIN_PITCH;
	target_page->attr_mix2edit_MAX	= TRACK_MAX_PITCH;
	
	target_page->SEL_LOCK = OFF;
	
	target_page->SCL_align = OFF;
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


// Initilaize the passed MIDI event
void MIDIevent_init( MIDIeventstruct* in_event ){
	
	in_event->timestamp = (unsigned int) TIMESTAMP_MAX;
	in_event->type		= 0;
	in_event->val0		= 0;
	in_event->val1		= 0;
	in_event->val2		= 0;
	in_event->prev		= NULL;
	in_event->next 		= NULL;
}

// Initilaize the passed MIDI event
void NOTEevent_init( NOTEeventstruct* in_event ){
	
	in_event->target_step 	= NULL;
	in_event->target_track	= NULL;
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
	MIDI_stack = &MIDIevent_repository[ 0 ];

	// Push all events onto the stack
	for( i=0; i<MIDI_NROF_EVENTS; i++ ){
		
		// First init the event 
		MIDIevent_init( &MIDIevent_repository[i] );

		// Push the event on the stack
		MIDI_stack_push( &MIDIevent_repository[i] );
	}

	//Queue now is empty.
	MIDI_queue_counter = 0;

	// Queue is virtually inexistant.
	MIDI_queue = NULL;
}



// Initializes all events to be in the stack and the queue to be empty
void NOTE_IN_init(){
	
	unsigned int i=0;

	// Setup the stack pointer
	NOTE_stack = &NOTEevent_repository[ 0 ];

	// Push all events onto the stack
	for( i=0; i<NOTE_NROF_EVENTS; i++ ){
		
		// First init the event 
		NOTEevent_init( &NOTEevent_repository[i] );

		// Push the event on the stack
		NOTE_stack_push( &NOTEevent_repository[i] );
	}

	//Queue now is empty.
	NOTE_queue_counter = 0;

	// Queue is virtually inexistant.
	NOTE_queue = NULL;
}




// Progress bar
void show_progress_bar( unsigned char part, unsigned char color ){
	
	switch( part ){
		
		case 4:
			MIR_write_dot( LED_NOTE_F,		color);						
			MIR_write_dot( LED_NOTE_E,		color);
			MIR_write_dot( LED_NOTE_Dis,	color);	
			break;

		case 3:
			// Last third in the progress bar
			MIR_write_dot( LED_NOTE_D,  	color);				
			MIR_write_dot( LED_NOTE_Cis,	color);			
			MIR_write_dot( LED_NOTE_C,		color);	
			MIR_write_dot( LED_SCALE_MOD,	color);
			break;

		case 2:
			// Second third in the progress bar
			MIR_write_dot( LED_SCALE_SEL,	color);
			MIR_write_dot( LED_SCALE_CAD,	color);
			MIR_write_dot( LED_NOTE_Cup,	color);
			MIR_write_dot( LED_NOTE_B,		color);	
			break;		

		case 1: 
			// First third in the progress bar
			MIR_write_dot( LED_NOTE_Ais,	color);
			MIR_write_dot( LED_NOTE_A,		color);
			MIR_write_dot( LED_NOTE_Gis,	color);
			MIR_write_dot( LED_NOTE_G, 		color);			
			MIR_write_dot( LED_NOTE_Fis, 	color);
			break;		
	}
}



// Write the flashbuffer to flash
void Flashbuffer_total_read_from_flash(){

	// Temporarily stay here..
	unsigned int 	err 		= 	0;
	unsigned int* 	err_address = 	&err;

	// Size of flash image to be read
	unsigned int mem_image_size = sizeof( memory_image );

	// Disable interrupts
	cyg_interrupt_disable();

	// Make sure no lights go off as they should not
	GLOBALBLINKER = ON;

	// Read the flashbuffer from flash		
	cyg_mutex_lock( &mutex_FlashOperations );
	
	err = flash_read(		(void *) MY_FLASH_BASE, 
							(void *) &memory_image,
							mem_image_size,
							(void**) err_address  		);

	cyg_mutex_unlock( &mutex_FlashOperations );
	
	// Enable interrupts
	cyg_interrupt_enable();
}



// Write the flashbuffer to flash. Single flashbuffer handling
void Flashbuffer_total_write_to_flash(){

	unsigned int err = 0;
	unsigned int* err_address = &err;
	unsigned int i=0;

	// Size of flash image to be read
	unsigned int mem_image_size = sizeof( memory_image );
	
	// Disable interrupts
	//	cyg_interrupt_disable();
	//	cyg_scheduler_lock();

	// Make sure no lights go off as they should not
	GLOBALBLINKER = ON;
	// Progress bar part 1/4
	show_progress_bar( 2, MIR_RED );
	VIEWER_show_MIR();

	// ***
 	// *** ERASE the flash before rewriting it
	cyg_mutex_lock( &mutex_FlashOperations );
	err = flash_erase( 		(void *) MY_FLASH_BASE, 
							mem_image_size, 
							(void**) err_address );
	cyg_mutex_unlock( &mutex_FlashOperations );
	// Progress bar part 2/4
	show_progress_bar( 3, MIR_RED );
	VIEWER_show_MIR();
	
	// ***
	// *** WRITE the bytes to the flash
	cyg_mutex_lock( &mutex_FlashOperations );
    err = flash_program(	(void *) MY_FLASH_BASE, 
							(void*) &memory_image,
							mem_image_size, 
							(void **) err_address );
	cyg_mutex_unlock( &mutex_FlashOperations );
	// Progress bar part 4/4
	show_progress_bar( 4, MIR_RED );
	VIEWER_show_MIR();
	
	// ***
 	// *** WAIT - which is just an optical gimmic
 	for (i=0; i<30000; i++) {}

	// Enable interrupts
	//	cyg_interrupt_enable();
	//	cyg_scheduler_unlock();
}

