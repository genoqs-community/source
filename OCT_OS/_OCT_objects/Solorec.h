
#include "../_OCT_objects/Runtime.h"


// Fine tune acts like a band pass filter that opens wider starting equal distance between two steps.
// Notes found within this band will be quantize-flipped so that the note is placed in the opposite direction.
unsigned char SOLO_quantize_fine_tune 			= 0; // Odd=switch polarity Even=drop polarized notes
unsigned char SOLO_quantize_note 				= 1; // 0=OFF, 1=1/64, 2=1/32, 3=1/16, 4=1/8, 5=1/4, 6=1/2, 7=1/1-Note
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
unsigned char SOLO_rec_ending_flash				= ON;
unsigned char SOLO_rec_continue_recording		= OFF;
unsigned char SOLO_rec_legato					= OFF;
unsigned char SOLO_rec_measure_hold				= OFF;
unsigned char SOLO_rec_bank						= OFF;
unsigned char SOLO_rec_rehersal					= OFF;
unsigned char SOLO_page_play_along[10];
unsigned short SOLO_rec_save_playmodes			= OFF;
unsigned short SOLO_rec_measure_count			= OFF;
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
