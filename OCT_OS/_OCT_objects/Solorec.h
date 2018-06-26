
#include "../_OCT_objects/Runtime.h"


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

		G_solo_page_play_along[i] = 255;
	 }
}
