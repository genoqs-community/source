/* OCTOPUS
 * Definitions of the page data and functions
 * 08.11.2003 genoQs Machines
 */

extern void Step_init( Stepstruct* target_step );
extern unsigned int length2_ticks( unsigned int length );
extern void build_step_chord( Stepstruct* target_step, unsigned char bit_offset );
extern signed char adjust_PIT_to_track( unsigned char in_PIT, Trackstruct* target_track );
extern void make_chord( unsigned char stackmode, Stepstruct* target_step, signed char in_pitch );												


typedef struct pagestruct{
 
 	unsigned short future;
 
	// PAGE contains an array of pointers to Track structures - mainly attribute collections
	Trackstruct* 	Track[MATRIX_NROF_ROWS];	

	// MATRIX: Page contains an array of Step structures grouped in the Matrix
	Stepstruct* 	Step[MATRIX_NROF_ROWS][MATRIX_NROF_COLUMNS];

	// This holds the locator value. Int because of higher resolution scenarios
	unsigned int 	locator; 

	// CONTROL, MIX2EDIT or EDIT - indicates what the Edit knobs are doing..
	unsigned char 	editorMode;
	
	// Holds the index of the page in the page repository
	unsigned char 	pageNdx;

	// indicates whether the page is cleared or contains some data. ON means cleared, OFF is dirty
	unsigned char 	page_clear;
		
	// Flag to tell in which keymode the page is in
	unsigned char 	keyMode;

	// Defines the track chains and splits in the page (book pg. 146)
	unsigned char 	chainMode;

	unsigned short 	trackSelection;		// Keeps the bitmap of selected tracks pattern
	unsigned short 	trackSelectionStored; // bitmap of previous trackSelection (one step rollback)

	unsigned short 	stepSelectionBits[10];	// Bit array of 10x16 indicating which steps are selected
	unsigned char 	stepSelection; 			// Keeps count of the number of steps that are selected in the page
	unsigned char 	stepSelectionSingleRow;	// Keeps row coordinate of a selected single step
	unsigned char 	stepSelectionSingleCol;	// Keeps col coordinate of a selected single step

	unsigned short 	trackMutepattern;	// Keeps the bitmap the muted tracks pattern
	unsigned short 	trackMutepatternStored; // one step rollback - previous track Selection

	unsigned short 	trackSolopattern;	// Keeps the bitmap of the soloed tracks
	
	unsigned short 	track_REC_pattern; 	// Keeps the bitmap of tracks set to REC mode
	unsigned char	REC_bit;			// Bit flag for step recording Duncan style, and other additionals..

	unsigned char 	editAttribute;		// Attribute selected for edit with the EDIT rotary knobs
	unsigned char 	mixAttribute;		// Attribute selected for edit with the MIX rotary knobs
	unsigned char 	mixMode;			// INC: mixer knob increases value of its track. DEC: decreases value of all other tracks
	unsigned char 	mixMasterStatus;	// POS or NEG: defines behavior of the MIXER
	unsigned char 	mixingTrack;		// Stores the index of the track that has been mixed last
	
	unsigned int 	trackAttributeSelection; // The track attribute selected in TRACK zoom mode - input for map - bitmap!!

	unsigned int 	stepAttributeSelection;	// The step attribute selected in STEP zoom mode - input for map - bitmap!!

	// Force-to-scale flag variables
	unsigned char 	force_to_scale;		// Indicates whether page is to be forced to its scale or not. May be recycled as force_to_scale bit
		
	unsigned char 	maxGroove;			// Indicates the highest groove value in Page
	
	unsigned char 	mixTarget;			// This can be one of MIXTGT_ATTRIBUTE, MIXTGT_VOLUME, MIXTGT_PAN, etc..

	// Store Preset MCC Amount for each track
	unsigned char 	MIXAMT_VOL[10];
	unsigned char 	MIXAMT_PAN[10];
	unsigned char 	MIXAMT_MOD[10];
	unsigned char 	MIXAMT_EXP[10];
	unsigned char 	MIXAMT_USR[10];

	// This stores the CC_MIXMAP data
	// Array built like this: [NROF_MAPS][NROF_ROWS][NROF_ATTRIBUTES]
	// The values are:         6 maps (for now) 10 (rows -easy)  3(0-AMT, 1-MCC, 2-MCH - may be expanded later)
	unsigned char 	CC_MIXMAP[6][10][3];
	unsigned char 	CC_MIXMAP_attribute;		// Indicates which attribute is shown for CC_MIXMAPS

	// Keeps the scale status of the page:
	// OFF: 		don't worry about scales
	// ON:			scale mode is active
	// SELECTED: 	scale is defined (cardinality and degrees
	// ACTIVE:		scale applies to the page
	unsigned char 	scaleStatus;

	// Stores the selected notes that make up the scale. Needs 12x2 = 24 bits of storage.
	// These are BITPATTERNS. Should work Faster and clearer
	// There are nine pairs that may be used - and eventually selected via numeric quadrant
	unsigned int 	scaleNotes[9];
	unsigned int 	scaleLead[9];
	
	// Temporary infrastructure variable - stores the scale lead temporarily, used for 
	// ..double click scenario, where the lead needs to be reverted to previous one.
	unsigned int 	scaleLead_old;
	unsigned int 	scaleNotes_old;
	
	// Keeps the signature of the last selected, non-preset scale
	unsigned int 	my_scale_signature;

	// Holds the number of repetitions in GRID mode
	unsigned char 	repeats;
	unsigned char 	repeats_left;

	// Store page specific attributes. I.e. velocity and pitch
	unsigned char 	attribute[ PAGE_NROF_ATTRIBUTES ];

	// Store the page length in steps. This is the master lenght for the size,
	// as it indicates how many steps we stay in page for before an oclock switch
	// takes place.

	// This is the mode in which the page reacts to key presses in the selector block.
	// Pretty ghetto for now I would think, but somehow working.
	unsigned char 	OPS_mode;
	
	// EFF pool - this is the modulation value pool to be built by the EFF send tracks..
	// ..to influence the EFF receive tracks. Pool is actualized with each page iteration
	signed char 	EFF_pool[ STEP_NROF_ATTRIBUTES ];
	
	// mix2edit attribute[] index and corresponding min and max values of stored attribute
	unsigned char 	attr_mix2edit;
	signed char 	attr_mix2edit_MIN;
	signed char		attr_mix2edit_MAX;
	
	// Indicator of the SEL LOCK status
	unsigned char SEL_LOCK;
	
	// Indicator of the Program mode in SCL
	unsigned char SCL_align;

} Pagestruct;



extern unsigned char how_selected_in_current_scale( 	Pagestruct* target_page, 
														unsigned char target_note );
														
extern unsigned char offset_to_next_current_degree(		Pagestruct* target_page, 
														unsigned char pitch );														

extern 	void 			shiftAttributeMap( 		Pagestruct* target_page,
												unsigned int row, 
												unsigned int in_attribute, 
												unsigned char direction );

extern unsigned char row_of_track( Pagestruct* target_page, Trackstruct* target_track );



// General purpose column pattern builder used by Player returns bitpattern of a column
// Incoming column value: between 1 and 16
unsigned int 	PAGE_get_bitpattern(	Pagestruct* target_page, unsigned int col){

	unsigned int bitpattern=0, row=0;
	
	for (row=0; row<MATRIX_NROF_ROWS; row++) {
		bitpattern |= (target_page->Step[row][col-1]->attribute[ATTR_STATUS] & 1) << row;
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
		if ( 	( my_bit_cardinality( target_page->Step[row][col]->chord_data & 0x7FF )>0 ) 
			&&	( Step_get_status( target_page->Step[row][col], STEPSTAT_TOGGLE ) == ON )
			){

			bitpattern |= (1 << (15-col) );
		}
	}
	return bitpattern;
}



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




// Wraps the track "ndx" in the given direction over the given distance
void Page_wrap_track (	Pagestruct* target_page, 
						unsigned int row, 
						unsigned char direction, 
						unsigned char distance) {
							
	// Get the trackpattern
	unsigned char i = 0;
	unsigned char temp_row=0, temp_col=0;

	// ATTRIBUTE MAPS SECTION
	// Shift the corresponding step attribute maps along - now including the status information.
	for (i=0; i < distance; i++){
		
		// If called while in step selection mode
		if ( zoom_level == zoomSTEP ){
			
			temp_row = target_page->stepSelectionSingleRow;
			temp_col = target_page->stepSelectionSingleCol;
				
			Step_set_status( target_page->Step[temp_row][temp_col], STEPSTAT_SELECT, OFF ) ;			
		}	
		
		shiftAttributeMap( target_page, row, VELOCITY, direction );
		shiftAttributeMap( target_page, row, PITCH	, direction );
		shiftAttributeMap( target_page, row, LENGTH	, direction );
		shiftAttributeMap( target_page, row, GROOVE	, direction );
		shiftAttributeMap( target_page, row, ATTR_STATUS-1	, direction ); // -1 is needed to align define spaces
	
		if ( zoom_level == zoomSTEP ){
				
			Step_set_status( target_page->Step[temp_row][temp_col], STEPSTAT_SELECT, ON ) ;			
		}	
	
	}

} 




// Identifies and updates highest groove attribute in page
void Page_update_maxGroove (Pagestruct* target_page) {

	unsigned char row=0;
	unsigned char max=0;
	
	// Find the highest groove value
	for (row = 0; row < MATRIX_NROF_ROWS; row++) {
		if (target_page->Track[row]->attribute[ATTR_GROOVE]	> max) {
			max = target_page->Track[row]->attribute[ATTR_GROOVE];
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
void Track_copy( Pagestruct* source_page, unsigned int source_row, 
				 Pagestruct* target_page, unsigned int target_row ){

	unsigned int i=0; 

	target_page->Track[target_row]->program_change = 
		source_page->Track[source_row]->program_change;

	// Copy the scale information data
	target_page->Track[target_row]->scale_pitch_offset 
		= source_page->Track[source_row]->scale_pitch_offset;		

	target_page->Track[target_row]->lead_pitch_offset	
		= source_page->Track[source_row]->lead_pitch_offset;

	// Copy the track attribute fields
	for ( i=0; i < TRACK_NROF_ATTRIBUTES; i++ ){

		// Make sure we don't touch the locator attribute
		if ( i != ATTR_LOCATOR ){
			
			target_page->Track[target_row]->attribute[i] = 
				source_page->Track[source_row]->attribute[i] ;
		}

		// Copy the attribute event max values
		target_page->Track[target_row]->event_max[i] = 
			source_page->Track[source_row]->event_max[i] ;
	}
			
	// Copy the factors
	target_page->Track[target_row]->VEL_factor = source_page->Track[source_row]->VEL_factor;
	target_page->Track[target_row]->PIT_factor = source_page->Track[source_row]->PIT_factor;
	target_page->Track[target_row]->LEN_factor = source_page->Track[source_row]->LEN_factor;
	target_page->Track[target_row]->STA_factor = source_page->Track[source_row]->STA_factor;
	target_page->Track[target_row]->AMT_factor = source_page->Track[source_row]->AMT_factor;
	target_page->Track[target_row]->GRV_factor = source_page->Track[source_row]->GRV_factor;
	target_page->Track[target_row]->MCC_factor = source_page->Track[source_row]->MCC_factor;
	
	
	// STEP COPY - Copy the Steps in the tracks
	for (i=0; i<MATRIX_NROF_COLUMNS; i++){
		
		Step_copy( 	source_page->Step[source_row][i], 
					target_page->Step[target_row][i]  );
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
	target_page->locator 				= 0;
	target_page->editorMode				= source_page->editorMode;
	target_page->page_clear				= source_page->page_clear;
	target_page->keyMode				= source_page->keyMode;
	target_page->chainMode				= source_page->chainMode;
	target_page->trackSelection			= source_page->trackSelection;
	target_page->trackSelectionStored	= source_page->trackSelectionStored;
	target_page->stepSelection			= source_page->stepSelection;
	target_page->stepSelectionSingleRow = source_page->stepSelectionSingleRow;
	target_page->stepSelectionSingleCol	= source_page->stepSelectionSingleCol;
	target_page->trackMutepattern		= source_page->trackMutepattern;
	target_page->trackMutepatternStored = source_page->trackMutepatternStored;
	target_page->trackSolopattern		= source_page->trackSolopattern;
	target_page->track_REC_pattern		= source_page->track_REC_pattern;
	target_page->editAttribute			= source_page->editAttribute;
	target_page->mixAttribute			= source_page->mixAttribute;
	target_page->mixMode				= source_page->mixMode;
	target_page->mixMasterStatus		= source_page->mixMasterStatus;
	target_page->mixingTrack			= source_page->mixingTrack;
	target_page->trackAttributeSelection= source_page->trackAttributeSelection;
	target_page->stepAttributeSelection = source_page->stepAttributeSelection;
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
	
	for (i=0; i<PAGE_NROF_ATTRIBUTES; i++) {
		target_page->attribute[i] 		= source_page->attribute[i];
	}

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

	target_page->OPS_mode 				= source_page->OPS_mode;

	target_page->attr_mix2edit 			= source_page->attr_mix2edit;
	target_page->attr_mix2edit_MIN		= source_page->attr_mix2edit_MIN;
	target_page->attr_mix2edit_MAX		= source_page->attr_mix2edit_MAX;
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



//____________________________________________________________________________________________
// Used tp clear all steps in page by setting their status attribute to zero
// New: put all steps in a track into the init() state. Also the track attributes that apply.
void Page_CLEAR_selected_tracks( Pagestruct* target_page ) {

	unsigned char 	row = 0,
					col = 0;
	
	// Applies to the selected rows
	for (row = 0; row < MATRIX_NROF_ROWS; row ++) {

		// Check track_selection if this applies to this row
		if ( target_page->trackSelection & (1 << row)) {										

			// Row has been selected, so operate: init steps first
			for (col = 0; col < MATRIX_NROF_COLUMNS; col++) {

				// target_page->Step[row][col]->attribute[ATTR_STATUS] = OFF;
				Step_init( target_page->Step[row][col] );
			}
		}
	}					

	// Mark page as cleared, if all tracks have been selected
	if (target_page->trackSelection == 0x3ff) { 

		target_page->page_clear = ON;
	}

	// Clear track Selection
	target_page->trackSelection = OFF;
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
				target_page->Step[row][col]->attribute[ATTR_STATUS] = ( ( rand() % 3) % 2 );
			}

			// Mark the page as edited
			target_page->page_clear = OFF;

		} // this is a selected trac, to which this applies

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
		
		// SKIP status - turn skipped steps OFF
		if ( Step_get_status( target_page->Step[row][col], STEPSTAT_SKIP) == ON ){

			Step_set_status( target_page->Step[dest_ndx][col], STEPSTAT_SKIP, ON );
			Step_set_status( target_page->Step[dest_ndx][col], STEPSTAT_TOGGLE, OFF );
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
				in_pitch = normalize(  	  target_page->Track[row]->attribute[ATTR_PITCH] 
										+ target_page->Step[row][col]->attribute[ATTR_PITCH],
										1, 127 );
	
//				d_iag_printf( "1selected pitch row:%d in col:%d pit:%d\n", row, col, in_pitch );
	
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

			// Remember the last processed pitch
			last_pitch = current_lowest_pitch;
	
	
			// If the destination step is OFF, assign the new pitch to the step
			if ( Step_get_status( target_page->Step[dest_row][col], STEPSTAT_TOGGLE ) != ON ){
	
//				d_iag_printf( "1set a note from row:%d in col:%d pl-it:%d\n", lowest_pitch_row, col, 
//					adjust_PIT_to_track( current_lowest_pitch, target_page->Track[dest_row] ) );
			
				// No chord set for the step, and 9 is the neutral strum value
				target_page->Step[dest_row][col]->chord_data	= 9 << 11;
				
				// Fill the step pitch value simply.. after having erased traces of chords
				target_page->Step[dest_row][col]->attribute[ATTR_PITCH] = 
					adjust_PIT_to_track( current_lowest_pitch, target_page->Track[dest_row] );
	
				// Turn the Step on
				Step_set_status( target_page->Step[dest_row][col], STEPSTAT_TOGGLE, ON );
			}

			// Destination step is ON, add pitch to its chord stack
			else{
					
//				d_iag_printf( "1make chord from row:%d in col:%d l-pit:%d\n", 
//					lowest_pitch_row, col, 
//					adjust_PIT_to_track( current_lowest_pitch, target_page->Track[dest_row] ) );
					
				// Add the incoming pitch to the chord structure of the step
				make_chord( INC, target_page->Step[dest_row][col], 
					adjust_PIT_to_track( current_lowest_pitch, target_page->Track[dest_row] ) );
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

		// Add the status of track steps to the destination - includes skips
		FLT_add_status( target_page, col, dest_ndx );

	} // Column iterator

	// Clear the track selection
	target_page->trackSelection = 0;
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
	
				modify_parameter ( &(target_page->Track[row]->attribute[ATTR_POSITION]), 
							TRACK_MIN_POSITION, TRACK_MAX_POSITION, INC, OFF, FIXED);
			}



			// Row has been selected, so do the remix operation on the steps (cols)
			for (col = 0; col < MATRIX_NROF_COLUMNS; col++) {

				// VELOCITY Random values
				target_page->Step[row][col]->attribute[ATTR_VELOCITY] = 
					(signed char)
					randomize_byamount( target_page->Step[row][col]->attribute[ATTR_VELOCITY],
										target_page->Track[row]->attribute[ATTR_AMOUNT]
										+ target_page->Track[row]->event_offset[ATTR_AMOUNT]
										, 
										STEP_MIN_VELOCITY, STEP_MAX_VELOCITY );
										
				// LENGTH Random values 
				target_page->Step[row][col]->attribute[ATTR_LENGTH] = 
					(signed char)
					randomize_byamount( (unsigned char)target_page->Step[row][col]->attribute[ATTR_LENGTH],
										(unsigned char) target_page->Track[row]->attribute[ATTR_AMOUNT]
										+ target_page->Track[row]->event_offset[ATTR_AMOUNT]
										, 
										STEP_MIN_LENGTH, ((unsigned char)STEP_MAX_LENGTH) );
				
				// START Random values 
				// Get the random delta to be applied
				temp =
					(signed char)
					randomize_byamount( 6 + target_page->Step[row][col]->attribute[ATTR_START],
										target_page->Track[row]->attribute[ATTR_AMOUNT]
										+ target_page->Track[row]->event_offset[ATTR_AMOUNT]
										, 
										STEP_MIN_START + 6, STEP_MAX_START + 6 );

				if ((temp >= STEP_MIN_START + 6) && (temp < STEP_MAX_START + 6)) {
					target_page->Step[row][col]->attribute[ATTR_START] = temp - 6;
				}

				// d_iag_printf( "temp:%d start value now:%d\n", 
				//   temp, target_page->Step[row][col]->attribute[ATTR_START] );

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

	// Compute the absolute delay according to GRV - and convert it to a value in [0..12]
	if ( target_page->Track[row]->attribute[ATTR_GROOVE] > 0 ){

		// The >0 condition is a specialty of length2_ticks.. We do need the 0 value here.
		delay = length2_ticks(	target_page->Track[row]->attribute[ATTR_GROOVE] );
	}
	// d_iag_printf( "get_GRV_offset which:%d delay:%d\n", which_col, delay );

	result = delay;
	
	// Return the value to compare TTC_ABS_VALUE against
	return result;
}



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
	if ( target_page->Step[row][col]->attribute[ATTR_GROOVE] == 0){
		return result;
	}

	// Compute a random value between 0 and STEP_MAX_GROOVE
	result = rand() % (target_page->Step[row][col]->attribute[ATTR_GROOVE] + 1);

	// Return the computed value, which is really a delay in TTC units (1/12)
	return result;
}



