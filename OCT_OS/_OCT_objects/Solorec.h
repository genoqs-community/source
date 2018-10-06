
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
unsigned char SOLO_pos_marker_in				= OFF;
unsigned char SOLO_pos_marker_out				= OFF;
unsigned char SOLO_rec_freeflow					= OFF;
unsigned char SOLO_rec_freeflow_trim			= ON;
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

	Notestruct* noteRec = Rec_repository[row_of(target_page->pageNdx)].Note[row_of(target_track->trackId) * target_col];
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
	unsigned char this_ndx = first_page_in_cluster(target_page->pageNdx);
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
			target_note = Rec_repository[row_of(target_page->pageNdx)].Note[step_row * step_col];

			if ( target_page->Step[step_row][step_col]->attr_STATUS == ON ){

				quantizeStep(target_page->Step[step_row][step_col], target_note);
			}
		}

		this_ndx += 10;
	}
}


