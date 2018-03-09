//
// This is a flat form of the page, i.e. without repository pointers, 
// but with the real data copied in
// 



typedef struct page_flashbuffer_struct{
 
	// PAGE contains an array of pointers to Track structures - mainly attribute collections
	Trackstruct Track[MATRIX_NROF_ROWS];	

	// MATRIX: Page contains an array of Step structures grouped in the Matrix
	Stepstruct Step[MATRIX_NROF_ROWS][MATRIX_NROF_COLUMNS];

	// Zoom level and info mode used to know what to display..
	unsigned char editorMode;

	// Showing something is done by knowing the mode and of course the target
	unsigned char bigKnobAmt;
	
	// Holds the index of the page in the page repository
	unsigned char pageNdx;

	// indicates whether the page is cleared or contains some data. ON means cleared, OFF is dirty
	unsigned char page_clear;

	// Flag to tell in which keymode the page is in
	unsigned char keyMode;

	// Defines the track chains and splits in the page (book pg. 146)
	unsigned char chainMode;
	
	// Keeps track of the progress in the track chains: ranges from 0 to 7
	unsigned int chainCounter;

	unsigned short trackSelection;		// Keeps the bitmap of selected tracks pattern
	unsigned short trackSelectionStored; // bitmap of previous trackSelection (one step rollback)

	unsigned char stepSelection; 	// Keeps count of the number of steps that are selected in the page
	unsigned char stepSelectionSingleRow;	// Keeps row coordinate of a selected single step
	unsigned char stepSelectionSingleCol;	// Keeps col coordinate of a selected single step

	unsigned short trackMutepattern;	// Keeps the bitmap the muted tracks pattern
	unsigned short trackMutepatternStored; // one step rollback - previous track Selection

	unsigned short trackSolopattern;	// Keeps the bitmap of the soloed tracks
	
	unsigned short track_REC_pattern;	// Kepps the bitmap of the REC track status

	unsigned char trackZoomAttribute;

	unsigned char editAttribute;		// Attribute selected for edit with the EDIT rotary knobs
	unsigned char mixAttribute;		// Attribute selected for edit with the MIX rotary knobs
	unsigned char mixMode;			// INC: mixer knob increases value of its track. DEC: decreases value of all other tracks
	unsigned char mixMasterStatus;	// POS or NEG: defines behavior of the MIXER
	unsigned char mixingTrack;		// Stores the index of the track that has been mixed last
	
	unsigned int trackAttributeSelection; // The track attribute selected in TRACK zoom mode - input for map - bitmap!!

	unsigned int stepAttributeSelection;

	unsigned char force_to_scale;	// Page is to be forced to its scale?
	unsigned char grid_FTS_exempt;	// Force to, or exempt from grid scale?
	
	// Indicates the highest groove value in Page
	unsigned char maxGroove;
	
	unsigned char mixTarget;			// This can be one of MIXTGT_ATTRIBUTE, MIXTGT_VOLUME, MIXTGT_PAN, etc..

	// Store Preset MCC Amount for each track
	unsigned char MIXAMT_VOL[10];
	unsigned char MIXAMT_PAN[10];
	unsigned char MIXAMT_MOD[10];
	unsigned char MIXAMT_EXP[10];
	unsigned char MIXAMT_USR[10];

	// This stores the CC_MIXMAP data
	// Array built like this: [NROF_MAPS][NROF_ROWS][NROF_ATTRIBUTES]
	// The values are:         6(for now) 10 (easy)  3(0-AMT, 1-MCC, 2-MCH - can be expanded later)
	unsigned char CC_MIXMAP[6][10][3];
	unsigned char CC_MIXMAP_attribute;		// Indicates which attribute is shown for CC_MIXMAPS


	// Keeps the scale status of the page:
	// OFF: 		don't worry about scales
	// ON:			scale mode is active
	// SELECTED: 	scale is defined (cardinality and degrees
	// ACTIVE:		scale applies to the page
	unsigned char scaleStatus;

	// Stores the selected notes that make up the scale. Needs 12x2 = 24 bits of storage.
	// BITPATTERN
	// unsigned int scaleSelection;
	
	// Store the same data as scaleSelection, but separately. 
	// These are BITPATTERNS. Should work Faster and clearer
	unsigned int scaleNotes[9];
	unsigned int scaleLead[9];
	// Temporary infrastructure variable - stores the scale lead temporarily, used for 
	// ..double click scenario, where the lead needs to be reverted to previous one.
	unsigned int scaleLead_old;
	unsigned int scaleNotes_old;
	
	// Keeps the last non-standard scale selected
	unsigned int my_scale_signature;

	// Stores the size of the chord selected in page
	unsigned short current_scale;
	
	// Holds the mode that EDIT knobs operate on: OFF (default) Step preview, or MCC.
	unsigned char editMode;
	
	// Can be ON, OFF, or PROVISORY. ON is the usual. OFF triggers or locks. Havent decided.
	// Provisory sets a step but then only plays it once and erases it.
	unsigned char recordMode;
	
	unsigned char repeats;

	// CADENCE status can be OFF, ON (playing), or RECORDING. Indicates the obvious.
	unsigned char cadenceStatus;

	// this is the real cadence
	// Cadencestruct cadence;

	// Store page specific attributes. I.e. velocity and pitch
	unsigned char attribute[PAGE_NROF_ATTRIBUTES];
	
	unsigned char OPS_mode;
		
	// EFF pool - this is the modulation value pool to be built by the EFF send tracks..
	// ..to influence the EFF receive tracks. Pool is actualized with each page iteration
	// signed char 	EFF_pool[ STEP_NROF_ATTRIBUTES ];

	// mix2edit attribute[] index and corresponding min and max values of stored attribute
	unsigned char 	attr_mix2edit;
	signed char 	attr_mix2edit_MIN;
	signed char		attr_mix2edit_MAX;
	
	unsigned char SCL_align;

} Page_flashimage_struct;



// Freezes the source_row in source_page in its place in target Page_flashbuffer
void Track_freeze( 	Pagestruct* source_page, 
					unsigned int source_row, 
					Page_flashimage_struct* target ){

	unsigned int i=0; 
	
	// PROGRAM CHANGE
	target->Track[source_row].program_change = source_page->Track[source_row]->program_change;

	// CHAIN DATA
	target->Track[source_row].chain_data[0] = source_page->Track[source_row]->chain_data[0];
	target->Track[source_row].chain_data[1] = source_page->Track[source_row]->chain_data[1];
	target->Track[source_row].chain_data[2] = source_page->Track[source_row]->chain_data[2];
	target->Track[source_row].chain_data[3] = source_page->Track[source_row]->chain_data[3];

	// LEN, STA, VEL, PIT factors
	target->Track[source_row].LEN_factor = source_page->Track[source_row]->LEN_factor;
	target->Track[source_row].STA_factor = source_page->Track[source_row]->STA_factor;	
	target->Track[source_row].VEL_factor = source_page->Track[source_row]->VEL_factor;
	target->Track[source_row].PIT_factor = source_page->Track[source_row]->PIT_factor;

	// ATTRIBUTES
	for (i=0; i<TRACK_NROF_ATTRIBUTES; i++) {
		target->Track[source_row].attribute[i] = 
			source_page->Track[source_row]->attribute[i] ;
	}
	
	// VEL, PIT, MCC event modulation ranges
	target->Track[source_row].range_VEL = source_page->Track[source_row]->range_VEL;
	target->Track[source_row].range_PIT = source_page->Track[source_row]->range_PIT;
	target->Track[source_row].range_MCC = source_page->Track[source_row]->range_MCC;
				
	// STEPS
	for (i=0; i<MATRIX_NROF_COLUMNS; i++){
		Step_copy( 	source_page->Step[source_row][i], 
					&((*target).Step[source_row][i])   );
	}
}




// Unfreezes the tracks in the flashpage buffer into the tracks of target_page
void Track_unfreeze( 	Pagestruct* target_page, 
						unsigned int source_row,
						Page_flashimage_struct* source ){

	unsigned int i=0; 
	
	// PROGRAM CHANGE
	target_page->Track[source_row]->program_change = source->Track[source_row].program_change;

	// CHAIN DATA
	target_page->Track[source_row]->chain_data[0] = source->Track[source_row].chain_data[0];
	target_page->Track[source_row]->chain_data[1] = source->Track[source_row].chain_data[1];
	target_page->Track[source_row]->chain_data[2] = source->Track[source_row].chain_data[2];
	target_page->Track[source_row]->chain_data[3] = source->Track[source_row].chain_data[3];

	// LEN, STA, VEL, PIT factors
	target_page->Track[source_row]->LEN_factor = source->Track[source_row].LEN_factor;
	target_page->Track[source_row]->STA_factor = source->Track[source_row].STA_factor;	
	target_page->Track[source_row]->VEL_factor = source->Track[source_row].VEL_factor;
	target_page->Track[source_row]->PIT_factor = source->Track[source_row].PIT_factor;

	// ATTRIBUTES
	for (i=0; i<TRACK_NROF_ATTRIBUTES; i++) {
		target_page->Track[source_row]->attribute[i] = 
			source->Track[source_row].attribute[i] ;
	}
			
	// VEL, PIT, MCC event modulation ranges
	target_page->Track[source_row]->range_VEL = source->Track[source_row].range_VEL;
	target_page->Track[source_row]->range_PIT = source->Track[source_row].range_PIT;
	target_page->Track[source_row]->range_MCC = source->Track[source_row].range_MCC;

	// STEPS
	for (i=0; i<MATRIX_NROF_COLUMNS; i++){
		Step_copy( 	&((*source).Step[source_row][i]), 
					target_page->Step[source_row][i] 	);	
	}
}


// The COPY operation from a real page to the flat version, which is a buffer in RAM:
// Steps, tracks and cadence need to be copied in separately.
void Page_export_to_flashbuffer( 	Pagestruct* 				source_page, 
									Page_flashimage_struct* 	target_page ) {

	unsigned char 	i=0,
					j=0,
					k=0;

	// Copy the tracks and steps. Target page is not really a page, but the flash buffer
	for (i=0; i<MATRIX_NROF_ROWS; i++) {
		Track_freeze( source_page, i, target_page);
	}

	// Copy the cadence
	// Cadence_copy( source_page->cadence, target_page->cadence );

	#include "Page_flat_fields.h"

	// d_iag_printf( "X:pageNdx:%d page_clear:%d\n", source_page->pageNdx, source_page->page_clear );

}


//
// I M P O R T  FROM FLASHBUFFER
//
void Page_import_from_flashbuffer( 	Page_flashimage_struct* 	source_page, 
									Pagestruct* 				target_page ) {

	unsigned char 	i=0,
					j=0,
					k=0;

	// Copy the tracks and steps. Source page is not really a page, but the flash buffer
	for (i=0; i<MATRIX_NROF_ROWS; i++) {
		Track_unfreeze( target_page, i, source_page);
	}

	// Copy the cadence
	// Cadence_copy( source_page->cadence, target_page->cadence );

	#include "Page_flat_fields.h"
	
	// d_iag_printf( "I:pageNdx:%d flash page_clear:%d\n", target_page->pageNdx, source_page->page_clear );
}







