
#include "../_OCT_objects/Runtime.h"


// Fine tune acts like a band pass filter that opens wider starting equal distance between two steps.
// Notes found within this band will be quantize-flipped so that the note is placed in the opposite direction.
unsigned char SOLO_quantize_fine_tune 			= 0; // Odd=switch polarity Even=drop polarized notes
unsigned char SOLO_quantize_note 				= 0; // 0=OFF, 1=STA4, 2=STA3, 3=STA2, 4=STA1, 5=STA0
signed char	  SOLO_strum						= 9; // 9=OFF
unsigned char SOLO_slow_tempo					= OFF;
Pagestruct*	  SOLO_rec_page						= NULL;
unsigned char SOLO_midi_ch						= 1;
unsigned char SOLO_normalize_pitch				= OFF;
unsigned char SOLO_normalize_len				= OFF;
unsigned char SOLO_has_rec						= OFF;
unsigned char SOLO_edit_buffer_volatile			= OFF;
unsigned char SOLO_overdub						= OFF;
unsigned char SOLO_rec_pressed_col				= OFF;
unsigned char SOLO_rec_freeflow					= OFF;
unsigned char SOLO_rec_freeflow_trim			= OFF;
unsigned char SOLO_rec_ending_flash				= ON;
unsigned char SOLO_rec_continue_recording		= OFF;
unsigned char SOLO_rec_quantize_first_beat		= ON;
unsigned char SOLO_rec_legato					= OFF;
unsigned char SOLO_rec_measure_hold				= OFF;
unsigned char SOLO_rec_bank						= OFF;
unsigned char SOLO_rec_rehersal					= OFF;
unsigned char SOLO_rec_track_preview			= SOLOPAGE;
unsigned char SOLO_rec_has_MCC					= FALSE;
unsigned char SOLO_page_play_along[10];
unsigned short SOLO_rec_save_playmodes			= OFF;
unsigned short SOLO_rec_measure_count			= OFF;
unsigned short SOLO_rec_freeflow_measures		= OFF;
unsigned short SOLO_rec_measure_pos				= OFF;
unsigned short SOLO_pos_marker_in				= OFF; // left cut -  SOLO_rec_measure_pos
unsigned short SOLO_pos_marker_out				= OFF; // right cut - SOLO_rec_measure_pos
Pagestruct*   SOLO_pos_in						= NULL;
Pagestruct*   SOLO_pos_out						= NULL;



// Solo Recordings: Initalize the original note recording repository
void Solorec_init(){

	unsigned int i=0;
	Recstruct* target_rec = NULL;

	for (i=0; i<10; i++){
		target_rec = &Rec_repository[i];

		target_rec->measure_count = 0;
		unsigned char j=0;
		for (j = 0; j < 16; ++j) {
			target_rec->Note[j]->attr_LEN = 0;
			target_rec->Note[j]->attr_PIT = 0;
			target_rec->Note[j]->attr_STA = 0;
			target_rec->Note[j]->attr_VEL = 0;
			target_rec->Note[j]->chord_data = 0;
			target_rec->Note[j]->chord_up = 0;
		}

		SOLO_page_play_along[i] = 255;
	 }
}

void freeflowOff( unsigned char trim ){

	if ( trim == TRUE ){

		unsigned char last_page = last_page_in_cluster(SOLO_rec_page->pageNdx);
		Pagestruct* target_page = &Page_repository[ last_page ];
		trim_freeflow_track_chain(target_page, (target_page->attr_STA - target_page->repeats_left));
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
		Trackstruct* target_track,
		unsigned char target_col ){

	Notestruct* noteRec = Rec_repository[grid_col(target_page->pageNdx)].Note[grid_ndx(grid_row(target_track->trackId), target_col)];
	noteRec->chord_up = target_step->chord_up;
	noteRec->chord_data = target_step->chord_data;
	noteRec->attr_VEL = target_step->attr_VEL;
	noteRec->attr_STA = target_step->attr_STA;
	noteRec->attr_PIT = target_step->attr_PIT;
	noteRec->attr_LEN = target_step->attr_LEN;

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

