
#include "../_OCT_objects/Runtime.h"


// Fine tune acts like a band pass filter that opens wider starting equal distance between two steps.
// Notes found within this band will be quantize-flipped so that the note is placed in the opposite direction.
unsigned char SOLO_quantize_fine_tune_center	= 1; // attract to center (STA_START)
unsigned char SOLO_quantize_fine_tune_edge		= 9; // switch polarity from the edge
unsigned char SOLO_quantize_fine_tune_drop_edge	= OFF; // drop edge notes that would otherwise switch polarity
unsigned char SOLO_quantize_note 				= 0; // 0=OFF, 1=STA4, 2=STA3, 3=STA2, 4=STA1, 5=STA0
signed char	  SOLO_strum						= 9; // 9=OFF
unsigned char SOLO_slow_tempo					= OFF;
Pagestruct*	  SOLO_rec_page						= NULL;
unsigned char SOLO_midi_ch						= 1;
unsigned char SOLO_normalize_pitch				= OFF;
unsigned char SOLO_normalize_len				= OFF;
unsigned char SOLO_has_rec						= OFF;
unsigned char SOLO_rec_finalized				= OFF;
unsigned char SOLO_edit_buffer_volatile			= OFF;
unsigned char SOLO_overdub						= OFF;
unsigned char SOLO_rec_pressed_col				= OFF;
unsigned char SOLO_rec_freeflow					= OFF;
unsigned char SOLO_rec_freeflow_trim			= OFF;
unsigned char SOLO_rec_ending_flash				= ON;
unsigned char SOLO_rec_continue_recording		= ON;
unsigned char SOLO_rec_quantize_first_beat		= ON;
unsigned char SOLO_rec_legato					= OFF;
unsigned char SOLO_rec_measure_hold				= OFF;
unsigned char SOLO_rec_bank						= OFF;
unsigned char SOLO_rec_rehersal					= OFF;
unsigned char SOLO_rec_track_preview			= SOLOPAGE;
unsigned char SOLO_rec_has_MCC					= OFF;
unsigned char SOLO_undo_note					= NOP;
unsigned char SOLO_undo_note_all				= OFF;
unsigned char SOLO_undo_note_page_col			= NOP;
unsigned char SOLO_page_play_along[10];
unsigned char SOLO_page_play_along_toggle[10];
unsigned short SOLO_rec_save_playmodes			= OFF;
unsigned short SOLO_rec_measure_count			= OFF;
unsigned short SOLO_rec_freeflow_measures		= OFF;
unsigned short SOLO_rec_measure_pos				= OFF;
unsigned short SOLO_pos_marker_in				= OFF; // left cut -  SOLO_rec_measure_pos
unsigned short SOLO_pos_marker_out				= OFF; // right cut - SOLO_rec_measure_pos
Pagestruct*   SOLO_pos_in						= NULL;
Pagestruct*   SOLO_pos_out						= NULL;



void initNote(Notestruct* note){

	note->status = OFF;
	note->chord_up = 0;
	note->chord_data = 0;
	note->attr_VEL = 0;
	note->attr_STA = 0;
	note->attr_PIT = 0;
	note->attr_LEN = 0;
}

// Solo Recordings: Initalize the original note recording repository
void Solorec_init(){

	unsigned char i=0, j=0;
	unsigned short ndx=0;
	Recstruct* target_rec = NULL;

	for (i=0; i<MATRIX_NROF_COLUMNS; i++){

		target_rec = &Rec_repository[i];
		target_rec->measure_count = 0;
		SOLO_page_play_along[i] = NOP;
		SOLO_page_play_along_toggle[i] = NOP;
	}

	for (i=0; i<MAX_NROF_PAGES; i++){

		for (j=0; j<MATRIX_NROF_COLUMNS; j++){

			ndx = (i * MATRIX_NROF_COLUMNS) + j;
			initNote( &Note_repository[ndx] );
			initNote( &Note_undo_repository[ndx] );
			Rec_repository[j].Note[i] = &Note_repository[ndx];
			Rec_undo_repository[j].Note[i] = &Note_undo_repository[ndx];
		}
	}
}

void copyNote(Notestruct* src, Notestruct* dest){

	dest->status = src->status;
	dest->chord_up = src->chord_up;
	dest->chord_data = src->chord_data;
	dest->attr_VEL = src->attr_VEL;
	dest->attr_STA = src->attr_STA;
	dest->attr_PIT = src->attr_PIT;
	dest->attr_LEN = src->attr_LEN;
}

void noteToStep(Notestruct* note, Stepstruct* step){

	Step_set_status( step, STEPSTAT_TOGGLE, note->status );
	step->chord_up = note->chord_up;
	step->chord_data = note->chord_data;
	step->attr_VEL = note->attr_VEL;
	step->attr_STA = note->attr_STA;
	step->attr_PIT = note->attr_PIT;
	step->attr_LEN = note->attr_LEN;
}

void stepToNote(Stepstruct* step, Notestruct* note){

	note->status = Step_get_status(step, STEPSTAT_TOGGLE);
	note->chord_up = step->chord_up;
	note->chord_data = step->chord_data;
	note->attr_VEL = step->attr_VEL;
	note->attr_STA = step->attr_STA;
	note->attr_PIT = step->attr_PIT;
	note->attr_LEN = step->attr_LEN;
}

void recPageCopy( unsigned char source_col, unsigned char dest_col ){

	Recstruct* source_rec = &Rec_repository[source_col];
	Recstruct* dest_rec = &Rec_repository[dest_col];
	Recstruct* undo_dest_rec = &Rec_undo_repository[dest_col];
	undo_dest_rec->measure_count = dest_rec->measure_count;
	dest_rec->measure_count = source_rec->measure_count;
	unsigned char j=0;
	for (j = 0; j < MAX_NROF_REC_NOTES; j++) {
		copyNote(dest_rec->Note[j], undo_dest_rec->Note[j]); // undo
		copyNote(source_rec->Note[j], dest_rec->Note[j]);
	}
}

void undoAllNotes(){

	signed short this_ndx = first_page_in_cluster(SOLO_rec_page->pageNdx);
	int i, col;
	Notestruct* target_note;
	Notestruct* undo_note;
	Stepstruct* target_step;
	Pagestruct* target_page;

	// For each page in the record chain
	// track forward
	while ( 	(this_ndx < MAX_NROF_PAGES) &&
			(Page_repository[this_ndx].page_clear == OFF)
	){

		target_page = &Page_repository[this_ndx];
		col = grid_col(target_page->pageNdx);

		for (i=0; i<MAX_NROF_PAGE_NOTES; i++){
			// copy up from undo
			target_note = Rec_repository[col].Note[i];
			undo_note = Rec_undo_repository[col].Note[i];
			target_step = target_page->Step[grid_row(i)][grid_col(i)];
			copyNote(undo_note, target_note);
			noteToStep(target_note, target_step);
		}
		this_ndx += 10;
	}

	SOLO_undo_note_all = OFF;
}

void pageClusterEnterSoloRec(unsigned char pageNdx){

	unsigned char this_ndx = first_page_in_cluster(pageNdx);
	unsigned char start, col;
	Pagestruct* target_page;

	// For each page in the record chain
	// track forward
	while ( 	(this_ndx < MAX_NROF_PAGES) &&
			(Page_repository[this_ndx].page_clear == OFF)
	){

		target_page = &Page_repository[this_ndx];
		col = grid_col(target_page->pageNdx);
		start = find_record_track_chain_start(target_page);
		Rec_repository[col].measure_count = MATRIX_NROF_ROWS - start;

		if ( SOLO_rec_page == NULL ){
			GRID_CURSOR = target_page->pageNdx;
			GRID_p_selection[ SOLO_rec_bank ] = target_page;
			GRID_p_preselection[ SOLO_rec_bank ] = target_page;
			GRID_p_clock_presel[ SOLO_rec_bank ] = target_page;
			SOLO_rec_page = target_page;
			SOLO_has_rec = ON;
			SOLO_rec_finalized = ON;
			SOLO_rec_bank = grid_row(pageNdx);
			SOLO_midi_ch = target_page->Track[start]->attr_MCH;
		}
		this_ndx += 10;
	}

	copy_page_cluster_to_recording();
}

void breakSoloRecordingMeasureHold(){

	if ( SOLO_rec_page != NULL &&
		 G_run_bit == ON &&
		 SOLO_rec_measure_hold == ON &&
		 SOLO_rec_rehersal == OFF
	   ){

		SOLO_rec_measure_pos = 1;
		SOLO_has_rec = ON;
		SOLO_rec_measure_hold = OFF;
	}
}

void freeflowOff( unsigned char trim ){

	if ( trim == TRUE ){

		unsigned char last_page = last_page_in_cluster(SOLO_rec_page->pageNdx);
		Pagestruct* target_page = &Page_repository[ last_page ];
		trim_freeflow_track_chain(target_page, (target_page->attr_STA - target_page->repeats_left));
		copy_page_cluster_to_recording();
	}
	SOLO_rec_freeflow_trim = OFF;
	SOLO_rec_freeflow = OFF;
}

void quantizeStep(Stepstruct* target_step, Notestruct* noteRec){

	if ( noteRec->attr_STA > STEP_MAX_START - SOLO_quantize_note ){

		target_step->attr_STA = STEP_MAX_START - SOLO_quantize_note;
	}
	else if ( noteRec->attr_STA < STEP_MIN_START + SOLO_quantize_note ){

		target_step->attr_STA = STEP_MIN_START + SOLO_quantize_note;
	}
	else {
		target_step->attr_STA = noteRec->attr_STA;
	}
}

void capture_note_event(
		Stepstruct* target_step,
		Pagestruct* target_page,
		unsigned char row,
		unsigned char step_col ){


	unsigned char col = grid_col(target_page->pageNdx);
	unsigned char idx = grid_ndx(row, step_col);
	Notestruct* noteRec = Rec_repository[col].Note[idx];

	stepToNote(target_step, noteRec);
	if ( SOLO_rec_finalized == OFF ){
		copyNote(noteRec, Rec_undo_repository[col].Note[idx]); // undo
	}

	// Quantize notes as they are recorded
	quantizeStep(target_step, noteRec);
}

void quantize(Pagestruct* target_page){

	// move to beginning of page cluster
	unsigned short this_ndx = first_page_in_cluster(target_page->pageNdx);
	unsigned char step_row, step_col = 0;
	unsigned int i=0;
	Notestruct* target_note;

	// track forward
	while ((this_ndx < MAX_NROF_PAGES) && (Page_repository[this_ndx].page_clear == OFF)
	){
		target_page = &Page_repository[this_ndx];

		// loop for each note
		for (i=0; i<MAX_NROF_PAGE_NOTES; i++){

			// line up the recording note with the actual step
			step_row = i / MATRIX_NROF_COLUMNS;
			step_col = i % MATRIX_NROF_COLUMNS;
			target_note = Rec_repository[grid_col(target_page->pageNdx)].Note[grid_ndx(step_row, step_col)];

			if ( target_page->Step[step_row][step_col]->attr_STATUS == ON ){

				quantizeStep(target_page->Step[step_row][step_col], target_note);
			}
		}

		this_ndx += 10;
	}
}

int measure_of_page_by_locator_vector (
		unsigned char pageNdx,
		int measure_count_vector /* 1 based */,
		unsigned char single_page ){

	int col = grid_col(pageNdx);
	int first_col = grid_col(first_page_in_cluster(pageNdx));
	unsigned char measure_count = Rec_repository[col].measure_count;

	measure_count_vector = ( single_page == TRUE ) ?
			measure_count_vector :
			// subtract the count of the first page so align the vector to the grid rows
		   (measure_count_vector - Rec_repository[first_col].measure_count);

	return (( MATRIX_NROF_ROWS - measure_count ) +
		   (( measure_count_vector -1 /* to zero based */) % MATRIX_NROF_ROWS ));
}

void cut_by_pos_markers() {

	int count_in, count_out;

	unsigned char single_page_in = ( first_page_in_cluster(SOLO_pos_in->pageNdx) == SOLO_pos_in->pageNdx ) ? TRUE : FALSE;
	unsigned char single_page_out = ( SOLO_pos_in->pageNdx == SOLO_pos_out->pageNdx ) ? TRUE : FALSE;

	int last_cut_measure = measure_of_page_by_locator_vector( SOLO_pos_in->pageNdx, SOLO_pos_marker_in, single_page_in );
	int last_shift_measure = measure_of_page_by_locator_vector( SOLO_pos_out->pageNdx, SOLO_pos_marker_out,
															 /* we care about multiple pages even if in/out are for a single page */
																single_page_in );
	int first_col = grid_col(first_page_in_cluster(SOLO_pos_in->pageNdx));
	int cnt = Rec_repository[first_col].measure_count;

	if ( single_page_out == TRUE ){

		count_in = (( SOLO_pos_marker_out - cnt ) - ( SOLO_pos_marker_in - cnt )) + 1;
		count_out = count_in;
	}
	else {

		if ( single_page_in == TRUE ){

			count_in = MATRIX_NROF_ROWS - last_cut_measure;
		}
		else {
			count_in = last_cut_measure - (( MATRIX_NROF_ROWS - 1 ) - cnt );
		}

		count_in = MATRIX_NROF_ROWS - ( last_cut_measure -
				 ( MATRIX_NROF_ROWS - Rec_repository[grid_col(SOLO_pos_in->pageNdx)].measure_count ));

		count_out = last_shift_measure -
				 (( MATRIX_NROF_ROWS - 1 ) - Rec_repository[grid_col(SOLO_pos_out->pageNdx)].measure_count );
	}

	cut_freeflow_track_chain(SOLO_pos_in, last_cut_measure, count_in);
	shift_down_freeflow_track_chain(SOLO_pos_out, last_shift_measure, count_out);
}

