
#include "../_OCT_objects/Runtime.h"


// Fine tune acts like a band pass filter that opens wider starting equal distance between two steps.
// Notes found within this band will be quantize-flipped so that the note is placed in the opposite direction.
unsigned char SOLO_quantize_fine_tune_center	= 1; // attract to center (STA_START)
unsigned char SOLO_quantize_fine_tune_edge		= 9; // switch polarity from the edge
unsigned char SOLO_quantize_fine_tune_drop_edge	= OFF; // drop edge notes that would otherwise switch polarity
unsigned char SOLO_quantize_note 				= 0; // 0=OFF, 1=STA4, 2=STA3, 3=STA2, 4=STA1, 5=STA0
signed char	  SOLO_strum						= 9; // 9=OFF
unsigned char SOLO_apply_effects_alarm			= OFF;
unsigned char SOLO_last_controller_mode			= NOP;
unsigned int  SOLO_last_note_scale				= SCALE_SIG_CHR;
unsigned char SOLO_scale_chords					= OFF;
unsigned short SOLO_scale_chords_modulations	= OFF;
unsigned short SOLO_scale_chords_last			= OFF;
signed char   SOLO_scale_chords_octave			= OFF;
signed char   SOLO_scale_chords_program_octave	= OFF;
unsigned char SOLO_scale_chords_program			= OFF;
unsigned char SOLO_scale_chords_program_keys	= OFF;
unsigned char SOLO_scale_chords_program_armed	= OFF;
unsigned char SOLO_scale_chords_arp_cursor		= NOP;
unsigned char SOLO_scale_chords_palette_ndx		= NOP;
unsigned char SOLO_scale_chords_prev_palette_ndx = NOP;
unsigned char SOLO_scale_chords_prev_on_ndx		= NOP;
unsigned char SOLO_scale_chords_pitch_recall	= OFF;
unsigned char SOLO_scale_chords_pitch_prev		= OFF;
unsigned char SOLO_scale_chords_pitch_recall_octave	   = OFF;
unsigned char SOLO_scale_chords_pitch_prev_octave      = OFF;
unsigned int SOLO_scale_chords_pitch_recall_scaleLead  = OFF;
unsigned int SOLO_scale_chords_pitch_prev_scaleLead    = OFF;
unsigned int SOLO_scale_chords_pitch_recall_scaleNotes = OFF;
unsigned int SOLO_scale_chords_pitch_prev_scaleNotes   = OFF;
unsigned char SOLO_scale_chords_b				= OFF;
unsigned char SOLO_has_scale					= OFF;
unsigned char SOLO_slow_tempo					= OFF;
Pagestruct*	  SOLO_rec_page						= NULL;
unsigned char SOLO_midi_ch						= 1;
unsigned char SOLO_normalize_vel				= OFF;
unsigned char SOLO_normalize_len				= OFF;
signed char SOLO_len_adjust						= OFF;
unsigned char SOLO_has_rec						= OFF;
unsigned char SOLO_rec_finalized				= OFF;
unsigned char SOLO_edit_buffer_volatile			= OFF;
unsigned char SOLO_overdub						= OFF;
unsigned char SOLO_rec_pressed_col				= OFF;
unsigned char SOLO_rec_is_tape					= OFF;
unsigned char SOLO_rec_freeflow					= OFF; // This was renamed to Tape Recording in the manual
unsigned char SOLO_rec_freeflow_trim			= OFF;
unsigned char SOLO_rec_ending_flash				= ON;
unsigned char SOLO_rec_continue_recording		= OFF;
unsigned char SOLO_rec_quantize_first_beat		= ON;
unsigned char SOLO_rec_legato					= OFF;
int 		 SOLO_transpose_row				   	= NOP;
unsigned char SOLO_transpose_latch				= OFF;
unsigned char SOLO_transpose_GRID_CURSOR		= NOP;
unsigned char SOLO_rec_transpose				= OFF;
signed char   SOLO_rec_transpose_octave			= OFF;
signed char   SOLO_rec_transpose_prev_pitch		= 127;
unsigned char SOLO_rec_measure_hold				= OFF;
unsigned char SOLO_rec_measure_hold_OTM			= OFF;
unsigned char SOLO_rec_measure_hold_latch		= OFF;
unsigned char SOLO_rec_show_strum				= OFF;
unsigned char SOLO_rec_strum_latch				= OFF;
unsigned char SOLO_rec_bank						= OFF;
unsigned char SOLO_rec_rehearsal				= OFF;
unsigned char SOLO_rec_record_OTM				= OFF;
unsigned char SOLO_rec_record_OTM_latch			= OFF;
unsigned char SOLO_rec_CLOCKSTART_OTM			= OFF;
unsigned char SOLO_rec_track_preview			= SOLOPAGE;
unsigned char SOLO_rec_has_MCC					= OFF;
unsigned char SOLO_rec_MCC_enabled				= OFF;
unsigned char SOLO_undo_note					= NOP;
unsigned char SOLO_undo_note_all				= OFF;
unsigned char SOLO_undo_note_page_col			= NOP;
unsigned char SOLO_undo_page_col				= NOP;
unsigned char SOLO_undo_page_len				= OFF;
unsigned char SOLO_page_play_along[10];
unsigned char SOLO_page_play_along_toggle[10];
unsigned short SOLO_rec_save_playmodes			= OFF;
unsigned short SOLO_rec_measure_count			= OFF;
unsigned short SOLO_rec_undo_measure_count		= OFF;
unsigned short SOLO_rec_freeflow_measures		= OFF;
unsigned short SOLO_rec_measure_pos				= OFF;
unsigned short SOLO_pos_marker_in				= OFF; // left cut -  SOLO_rec_measure_pos
unsigned short SOLO_pos_marker_out				= OFF; // right cut - SOLO_rec_measure_pos
unsigned char SOLO_orig_GRID_CURSOR				= NOP;
unsigned char SOLO_orig_G_clock_source			= NOP;
unsigned char SOLO_prev_stop					= ON;
unsigned char SOLO_mute							= OFF;

Pagestruct*   SOLO_pos_in						= NULL;
Pagestruct*   SOLO_pos_out						= NULL;
Chordstruct*  SOLO_last_chord					= NULL;
Pagestruct*   SOLO_p_selection 		 [GRID_NROF_BANKS];

unsigned char OTM_CC_type						= OFF;
unsigned char OTM_CC_val0						= OFF;
unsigned char OTM_CC_val1						= OFF;
unsigned char OTM_CC_val2						= OFF;

unsigned char delay_CC_type						= OFF;
unsigned char delay_CC_val0						= OFF;
unsigned char delay_CC_val1						= OFF;
unsigned char delay_CC_val2						= OFF;



void initNote(Notestruct* note){

	note->status = OFF;
	note->chord_up = 0;
	note->chord_data = 0;
	note->attr_VEL = STEP_DEF_VELOCITY;
	note->attr_STA = STEP_DEF_START;
	note->attr_PIT = STEP_DEF_PITCH;
	note->attr_LEN = STEP_DEF_LENGTH;
	note->event_data = 1 << 4;
	note->attr_MCC = 0;
}

void clearStepNote(Stepstruct* step){ // don't clear MCC

	step->attr_STATUS = OFF;
	step->chord_up = 0;
	step->chord_data = 0;
	step->event_data = 1 << 4;
	step->attr_VEL = STEP_DEF_VELOCITY;
	step->attr_STA = STEP_DEF_START;
	step->attr_PIT = STEP_DEF_PITCH;
	step->attr_LEN = STEP_DEF_LENGTH;
}

void initChord(Chordstruct* chord, unsigned char col){

	unsigned char i;
	Notestruct* target_note = NULL;

	chord->chord_id = NOP;
	chord->octave = 0;
	chord->pitch = 0;
	chord->scale = 0;
	chord->strum = 9;
	chord->tone = 0;
	chord->attr_LEN = STEP_DEF_LENGTH;
	chord->attr_VEL = 0;

	for (i=0; i<MATRIX_NROF_COLUMNS; i++){

		target_note = &Arp_pattern_repository[(col * MAX_NROF_PALETTE_CHORDS) + i];
		initNote(target_note);
		chord->Arp[i] = target_note;
	}
}

// Solo Recordings: Initalize the original note recording repository
void Solorec_init(){

	unsigned char i=0, j=0, row;
	unsigned short ndx=0;
	Recstruct* target_rec = NULL;
	Recstruct* undo_rec = NULL;

	for (i=0; i<MATRIX_NROF_COLUMNS; i++){

		target_rec = &Rec_repository[i];
		undo_rec = &Rec_undo_repository[i];
		target_rec->measure_count = 0;
		undo_rec->measure_count = 0;
		SOLO_page_play_along[i] = NOP;
		SOLO_page_play_along_toggle[i] = NOP;

		for ( row=0; row<MATRIX_NROF_ROWS; row++ ){
			target_rec->track_pitch[row] = TRACK_DEF_PITCH;
			undo_rec->track_pitch[row] = TRACK_DEF_PITCH;
		}
	}

	for (i=0; i<MAX_NROF_PAGES; i++){ // for each track of each record page

		for (j=0; j<MATRIX_NROF_COLUMNS; j++){ // create the track notes

			ndx = (i * MATRIX_NROF_COLUMNS) + j;
			initNote( &Note_repository[ndx] );
			initNote( &Note_undo_repository[ndx] );
			Rec_repository[j].Note[i] = &Note_repository[ndx];
			Rec_undo_repository[j].Note[i] = &Note_undo_repository[ndx];
		}
	}
}

void chord_palette_init(){

	unsigned int i=0;

	for (i=0; i < (MAX_NROF_PALETTE_CHORDS + 1); i++){

		initChord( &Chord_palette_repository[i], i );
	}
	//	validatePitches();
}

void initAssistantPage(){
	unsigned char row;

	SOLO_assistant_page->force_to_scale = ON;
	SOLO_assistant_page->scaleStatus = SCALE_MOD;
	SOLO_assistant_page->Track[0 /* ARP_TRACK */]->attr_PIT = MIDDLE_C;

	for ( row=1; row<MATRIX_NROF_ROWS; row++ ){ // row zero will play arp patterns
		// mute all other tracks
		SET_BIT(SOLO_assistant_page->trackMutepattern, row);
	}
}

void muteAssistantPage(){

	SET_BIT(SOLO_assistant_page->trackMutepattern, 0); // mute the Arp track
}

void enterSoloRec(){
	int i;

	SOLO_mute = OFF;
	G_measure_locator = OFF;
	SOLO_rec_measure_count = OFF;
	SOLO_rec_measure_pos = OFF;
	G_skip_step = NULL;
	SOLO_rec_CLOCKSTART_OTM = OFF;
	G_track_rec_bit = OFF;

	SOLO_orig_G_clock_source = NOP;
	if ( G_clock_source == EXT ){

		SOLO_orig_G_clock_source = G_clock_source;
		G_clock_source = OFF;
	}

	SOLO_orig_GRID_CURSOR = GRID_CURSOR;

	if ( SOLO_has_scale == OFF ){

		Page_copy(GRID_assistant_page, SOLO_assistant_page);
		initAssistantPage();
	}
	SOLO_assistant_page->attr_PIT = OFF;

	if ( SOLO_rec_page == NULL ){

		GRID_CURSOR = SOLO_assistant_page->pageNdx;
	}

	for ( i=0; i < GRID_NROF_BANKS; i++ ){
		SOLO_p_selection[ i ] = GRID_p_selection[ i ];
	}

	for ( i=0; i< 9; i++){
		SOLO_assistant_page->scaleNotes[i]			= SCALE_SIG_MAJ;
		SOLO_assistant_page->scaleLead[i]			= OFF;
		SOLO_assistant_page->scaleLead[i]			= 1 << 11;  // this is equivalent to C
	}
	SOLO_scale_chords_pitch_recall_scaleLead = SOLO_assistant_page->scaleLead[ G_scale_ndx ];
	SOLO_scale_chords_pitch_recall_scaleNotes = SOLO_assistant_page->scaleNotes[ G_scale_ndx ];
	SOLO_scale_chords_pitch_recall = SOLO_assistant_page->attr_PIT;

	CLEAR_BIT(SOLO_assistant_page->trackMutepattern, 0); // un-mute the Arp track
	SOLO_has_scale = ON;
	G_zoom_level = zoomSOLOREC;
}

void saveRec(){

	if ( SOLO_rec_finalized == OFF ) return;

	G_measure_locator = 0;
	selected_page_cluster_copy(GRID_CURSOR + 20, GRID_CURSOR);
	SOLO_rec_page = &Page_repository[GRID_CURSOR + 20];
	if ( G_run_bit == OFF ){
		GRID_p_selection[ SOLO_rec_bank ] = SOLO_rec_page;
		GRID_p_preselection[ SOLO_rec_bank ] = SOLO_rec_page;
	}
	GRID_p_clock_presel[ SOLO_rec_bank ] = SOLO_rec_page;
	pageClusterEnterSoloRec(GRID_CURSOR + 20);
	SOLO_page_play_along[grid_row(SOLO_rec_page->pageNdx)] = NOP;
}

void clearRec(){
	if ( SOLO_has_rec == ON ){

		SOLO_edit_buffer_volatile = ON; // toggle
		SOLO_has_rec = OFF;
		SOLO_rec_legato = OFF;
		freeflowOff(FALSE);
		SOLO_pos_marker_in = OFF;
		SOLO_pos_marker_out = OFF;
		SOLO_rec_finalized = OFF;
		SOLO_rec_transpose = OFF;
		if ( SOLO_rec_has_MCC == ON && SOLO_rec_track_preview == SOLOMCC ){
			SOLO_rec_track_preview = SOLOPAGE;
		}
		SOLO_rec_has_MCC = OFF;
		SOLO_rec_measure_hold = ON;
		SOLO_rec_rehearsal = OFF;
		// Clear the pages
		clear_page_record_track_chain(SOLO_rec_page);

		if ( SOLO_rec_is_tape == ON ){

			selected_page_cluster_clear(SOLO_rec_page->pageNdx);
			SOLO_has_rec 				= OFF;
			Solorec_init();
			unsigned char col = grid_col(GRID_CURSOR);

			// Free Flow!
			GRID_CURSOR = SOLO_rec_page->pageNdx;
			col = grid_col(GRID_CURSOR);
			Rec_repository[col].measure_count = MATRIX_NROF_ROWS;
			Rec_undo_repository[col].measure_count = MATRIX_NROF_ROWS;
			create_next_freeflow_page_cluster(GRID_CURSOR);
			SOLO_rec_freeflow_measures = count_to_last_page_in_grid_row(col) * MATRIX_NROF_ROWS;
			reset_page_cluster( SOLO_rec_page );
			SOLO_rec_freeflow = ON;
			SOLO_rec_is_tape = ON;
		}

		MIX_TIMER = ON;
		// Setup alarm for the MIX TIMER
		cyg_alarm_initialize(	alarm_hdl,
								cyg_current_time() + TIMEOUT_VALUE,
								0 );
	}
	// Clear the MCC data
	else if ( G_run_bit == ON && SOLO_rec_has_MCC == ON ){
		clear_page_record_mcc_data(SOLO_rec_page);
		SOLO_rec_has_MCC = OFF;
		SOLO_rec_track_preview = SOLOPAGE;
	}
}

void externalMIDI_PGMCH(){

	if ( SOLO_rec_freeflow == ON || G_PGMCH_foot_control == OFF ) return;

	if ( G_prev_PGMCH_val == G_PGMCH_val + 1 ){ // down

		if ( G_track_rec_bit ){ // running, stop
			stop_solo_rec(FALSE, OFF);
		}
		else if ( SOLO_edit_buffer_volatile == OFF ){ // has undo - clear data
			clearRec();
		}
		else {
			undoRec(NOP); // undo
		}
	}

	if (( G_prev_PGMCH_val == G_PGMCH_val - 1 ) && G_run_bit == ON ){ // up
		if ( G_track_rec_bit == OFF ){
			if ( SOLO_rec_finalized == OFF ){
				SOLO_rec_measure_hold = ON;
			}
			G_track_rec_bit = ON;
			SOLO_rec_track_preview = SOLOPAGE;
			SOLO_rec_rehearsal = OFF;
		}
		else { // recording, overdub / punch - toggle
			if ( SOLO_rec_measure_hold == ON ){
				SOLO_rec_rehearsal = ON;
				G_track_rec_bit = OFF;
			}
			else {
				SOLO_overdub ^= 1; // toggle
			}
		}
	}
}

void undoRec( unsigned char keyNdx ){
	if ( SOLO_rec_transpose == ON ){

		// DOUBLE CLICK SCENARIO
		if (	( DOUBLE_CLICK_TARGET == keyNdx )
			&& 	( DOUBLE_CLICK_TIMER   > DOUBLE_CLICK_ALARM_SENSITIVITY ) ) {

			// Double click code
			// ...
			clearAllTranspose();

		} // end of double click scenario

		// SINGLE CLICK SCENARIO
		else if (DOUBLE_CLICK_TARGET == 0) {

			DOUBLE_CLICK_TARGET = KEY_EDIT_MASTER;
			DOUBLE_CLICK_TIMER = ON;
			// Start the Double click Alarm
			cyg_alarm_initialize(
					doubleClickAlarm_hdl,
					cyg_current_time() + DOUBLE_CLICK_ALARM_TIME,
					DOUBLE_CLICK_ALARM_TIME );

			undoAllNotes();
		}
	}
	else if ( SOLO_edit_buffer_volatile == ON ){

		if ( SOLO_undo_page_col != NOP ){

			unsigned char j=0;
			for (j = 0; j < SOLO_undo_page_len; j++) {

				Page_repository[grid_ndx(SOLO_rec_bank, ( SOLO_undo_page_col + j ))].page_clear = OFF;
				Rec_repository[SOLO_undo_page_col + j].measure_count = Rec_undo_repository[SOLO_undo_page_col + j].measure_count;
			}
			SOLO_rec_measure_count = SOLO_rec_undo_measure_count;
			SOLO_rec_undo_measure_count = OFF;
			SOLO_undo_page_col = NOP;
			SOLO_undo_page_len = OFF;;
		}
		undoAllNotes();
		SOLO_has_rec = ON;
		SOLO_rec_finalized = ON;
		reset_page_cluster( SOLO_rec_page );
		SOLO_edit_buffer_volatile ^= 1; // toggle
	}
	else if ( SOLO_undo_note != NOP ){

		Notestruct* undoNote = Rec_undo_repository[SOLO_undo_note_page_col].Note[SOLO_undo_note];
		Notestruct* note = Rec_repository[SOLO_undo_note_page_col].Note[SOLO_undo_note];
		copyNote(undoNote, note);
		Pagestruct* page = &Page_repository[grid_ndx(SOLO_rec_bank, SOLO_undo_note_page_col)];
		Stepstruct* step = page->Step[grid_row(SOLO_undo_note)][grid_col(SOLO_undo_note)];
		noteToStep(note, step);
		Step_set_status( step, STEPSTAT_TOGGLE, ON );
		SOLO_undo_note = NOP;
		SOLO_undo_note_page_col = NOP;
	}
	else if ( SOLO_undo_note_all == ON ){

		undoAllNotes();
	}
}

void exitSoloRec(){
	int i;
	unsigned char row = NOP;
	Pagestruct* target_page = NULL;

	if ( SOLO_rec_page != NULL ){

		row = grid_row(SOLO_rec_page->pageNdx);
		target_page = SOLO_rec_page;
		reset_page_cluster( SOLO_rec_page );
		SOLO_orig_GRID_CURSOR = SOLO_rec_page->pageNdx;
	}

	if ( SOLO_orig_G_clock_source != NOP ){

		G_clock_source = SOLO_orig_G_clock_source;
		SOLO_orig_G_clock_source = NOP;
	}

	// Reset most of the global variables
	SOLO_quantize_fine_tune_center  	= 1;
	SOLO_quantize_fine_tune_edge		= 9;
	SOLO_quantize_fine_tune_drop_edge	= OFF;
	SOLO_quantize_note 					= OFF;
	SOLO_strum							= 9; // 9=OFF
	SOLO_apply_effects_alarm			= OFF;
	SOLO_slow_tempo						= OFF;
	SOLO_rec_page						= NULL;
	SOLO_rec_rehearsal					= OFF;
//	SOLO_midi_ch						= 1;
	SOLO_normalize_vel					= OFF;
	SOLO_normalize_len					= OFF;
	SOLO_has_rec						= OFF;
	SOLO_rec_finalized					= OFF;
	SOLO_undo_page_col					= NOP;
	SOLO_undo_page_len					= OFF;
	SOLO_edit_buffer_volatile			= OFF;
	SOLO_overdub						= OFF;
	SOLO_rec_pressed_col				= OFF;
	SOLO_pos_marker_in					= OFF;
	SOLO_pos_marker_out					= OFF;
	SOLO_rec_freeflow					= OFF;
//	SOLO_rec_ending_flash				= OFF;
	SOLO_rec_legato						= OFF;
	SOLO_rec_transpose					= OFF;
//	SOLO_page_play_along[10];
	G_measure_locator					= OFF;
	SOLO_rec_measure_count				= OFF;
	SOLO_rec_freeflow_measures			= OFF;
	SOLO_rec_measure_hold				= OFF;
	GRID_bank_playmodes 				= SOLO_rec_save_playmodes;
	SOLO_rec_save_playmodes				= OFF;
	SOLO_rec_has_MCC					= OFF;
	SOLO_undo_note						= NOP;
	SOLO_undo_note_page_col				= NOP;
	SOLO_transpose_row				   	= NOP;
	SOLO_transpose_latch				= OFF;
	SOLO_transpose_GRID_CURSOR			= NOP;
	SOLO_rec_transpose					= OFF;
	SOLO_rec_transpose_octave			= OFF;
	SOLO_rec_show_strum					= OFF;
	SOLO_rec_strum_latch				= OFF;
	SOLO_len_adjust						= OFF;
	SOLO_rec_is_tape					= OFF;
	SOLO_rec_measure_hold_latch			= OFF;
	SOLO_scale_chords = OFF;
	SOLO_scale_chords_program = OFF;
	SOLO_scale_chords_program_keys = OFF;
	SOLO_scale_chords_program_armed = OFF;
	SOLO_scale_chords_octave = OFF;
	SOLO_mute = OFF;
	G_skip_step = NULL;
	SOLO_rec_CLOCKSTART_OTM = OFF;

	for (i=0; i<MATRIX_NROF_ROWS; i++){
		if ( SOLO_page_play_along_toggle[i] != NOP ){
			grid_select( &Page_repository[SOLO_page_play_along_toggle[i]], OFF );
		}
	}
	if ( SOLO_last_controller_mode != NOP ) {

		G_midi_map_controller_mode = SOLO_last_controller_mode;
	}

	Solorec_init();
	muteAssistantPage();
	GRID_p_selection_cluster = OFF;

	for ( i=0; i < GRID_NROF_BANKS; i++ ){

		GRID_p_selection[ i ] = SOLO_p_selection[ i ];
		GRID_p_preselection[ i ] = SOLO_p_selection[ i ];
		GRID_p_clock_presel[ i ] = SOLO_p_selection[ i ];
	}
	if ( row != NOP ){

		GRID_p_selection[ row ] = target_page;
		GRID_p_preselection[ row ] = target_page;
		GRID_p_clock_presel[ row ] = target_page;
	}

	GRID_CURSOR = SOLO_orig_GRID_CURSOR;

	G_zoom_level = zoomGRID; // exit the Solo Recording view
}

unsigned char Note_get_status (	Notestruct* target_note,
								unsigned char target_bit){

	if ( target_note->status & (1 << target_bit) ) {

		return ON;
	}
	else {

		return OFF;
	}
}

void Note_set_status (			Notestruct* target_note,
								unsigned char target_bit,
								unsigned char in_value		) {
	// Set target bit to 0
	target_note->status &= (0xFF ^ (1 << target_bit));

	// Apply the in_value
	if (in_value == ON) {
		target_note->status |= (1 << target_bit);
	}
}

void tracksToRec(Pagestruct* page, Recstruct* rec){
	unsigned char row;

	for ( row=0; row<MATRIX_NROF_ROWS; row++ ){
		rec->track_pitch[row] = page->Track[row]->attr_PIT;
	}
}

void recToTracks(Recstruct* rec, Pagestruct* page){
	unsigned char row;

	for ( row=0; row<MATRIX_NROF_ROWS; row++ ){
		page->Track[row]->attr_PIT = rec->track_pitch[row];
	}
}

void copyTracks(Recstruct* src, Recstruct* dest){
	unsigned char row;

	for ( row=0; row<MATRIX_NROF_ROWS; row++ ){
		dest->track_pitch[row] = src->track_pitch[row];
	}
}

void copyNote(Notestruct* src, Notestruct* dest){

	dest->status = src->status;
	dest->event_data = src->event_data;
	dest->chord_up = src->chord_up;
	dest->chord_data = src->chord_data;
	dest->attr_VEL = src->attr_VEL;
	dest->attr_STA = src->attr_STA;
	dest->attr_PIT = src->attr_PIT;
	dest->attr_LEN = src->attr_LEN;
	dest->attr_MCC = src->attr_MCC;
}

void noteToStep(Notestruct* note, Stepstruct* step){

	step->attr_STATUS = note->status;
	step->event_data = note->event_data;
	step->chord_up = note->chord_up;
	step->chord_data = note->chord_data;
	step->attr_VEL = note->attr_VEL;
	step->attr_STA = note->attr_STA;
	step->attr_PIT = note->attr_PIT;
	step->attr_LEN = note->attr_LEN;
	step->attr_MCC = note->attr_MCC;
}

void stepToNote(Stepstruct* step, Notestruct* note){

	note->status = step->attr_STATUS;
	note->event_data = step->event_data;
	note->chord_up = step->chord_up;
	note->chord_data = step->chord_data;
	note->attr_VEL = step->attr_VEL;
	note->attr_STA = step->attr_STA;
	note->attr_PIT = step->attr_PIT;
	note->attr_LEN = step->attr_LEN;
	note->attr_MCC = step->attr_MCC;
}

void recPageCopy( unsigned char source_col, unsigned char dest_col ){

	Recstruct* source_rec = &Rec_repository[source_col];
	Recstruct* dest_rec = &Rec_repository[dest_col];
	Recstruct* undo_dest_rec = &Rec_undo_repository[dest_col];
	undo_dest_rec->measure_count = dest_rec->measure_count;
	dest_rec->measure_count = source_rec->measure_count;
	copyTracks(source_rec, dest_rec);
	copyTracks(dest_rec, undo_dest_rec);

	unsigned char j=0;

	for (j = 0; j < MAX_NROF_PAGE_NOTES; j++) {

		copyNote(source_rec->Note[j], dest_rec->Note[j]);
		copyNote(dest_rec->Note[j], undo_dest_rec->Note[j]); // undo
	}
}

void undoAllNotes(){

	signed short this_ndx = first_page_in_cluster(SOLO_rec_page->pageNdx);
	int i, col;
	Notestruct* target_note;
	Notestruct* undo_note;
	Stepstruct* target_step;
	Pagestruct* target_page;
	Recstruct* rec;
	unsigned char start_row = NOP;

	// For each page in the record chain
	// track forward
	while ( 	(this_ndx < MAX_NROF_PAGES) &&
			(Page_repository[this_ndx].page_clear == OFF)
	){

		target_page = &Page_repository[this_ndx];
		col = grid_col(target_page->pageNdx);
		rec = &Rec_undo_repository[ col ]; // undo
		if ( SOLO_undo_note_all == ON ){
			start_row = find_record_track_chain_start(target_page);
		}

		copyTracks(&Rec_undo_repository[col], &Rec_repository[col]);
		recToTracks(&Rec_repository[col], target_page);

		for (i=0; i<MAX_NROF_PAGE_NOTES; i++){

			if ( start_row != NOP && grid_row(i) < start_row ) continue;

			// copy up from undo
			target_note = Rec_repository[col].Note[i];
			undo_note = Rec_undo_repository[col].Note[i];
			target_step = target_page->Step[grid_row(i)][grid_col(i)];
			copyNote(undo_note, target_note);
			noteToStep(target_note, target_step);
		}

		this_ndx += 10;
	}

	if ( SOLO_rec_transpose == ON ){

		resetTransposeRecTrack();
	}
	else {
		SOLO_undo_note_all = OFF;
		SOLO_rec_legato = OFF;
		SOLO_has_rec = ON;
		SOLO_rec_measure_hold = OFF;
		SOLO_rec_finalized = ON;
	}
}

void rebuild_undo_using_rec_notes(){
	signed short this_ndx = first_page_in_cluster(SOLO_rec_page->pageNdx);
	int i, col;
	Notestruct* target_note;
	Notestruct* undo_note;
	Pagestruct* target_page;
	Recstruct* rec;
	unsigned char start_row = NOP;

	// For each page in the record chain
	// track forward
	while ( 	(this_ndx < MAX_NROF_PAGES) &&
			(Page_repository[this_ndx].page_clear == OFF)
	){

		target_page = &Page_repository[this_ndx];
		col = grid_col(target_page->pageNdx);
		rec = &Rec_undo_repository[ col ]; // undo
		start_row = find_record_track_chain_start(target_page);

		copyTracks(&Rec_repository[col], &Rec_undo_repository[col]);

		for (i=0; i<MAX_NROF_PAGE_NOTES; i++){

			if ( start_row != NOP && grid_row(i) < start_row ) continue;

			// copy up from undo
			target_note = Rec_repository[col].Note[i];
			undo_note = Rec_undo_repository[col].Note[i];
			copyNote(target_note, undo_note);
		}

		this_ndx += 10;
	}
}

void resetTransposeRecTrack(){

	signed short this_ndx = first_page_in_cluster(SOLO_rec_page->pageNdx);
	Pagestruct* target_page = &Page_repository[this_ndx];
	SOLO_transpose_row = my_bit2ndx(Page_getTrackRecPattern(target_page));
	unsigned char col;
	Recstruct* rec;
	int i;

	// For each page in the record chain
	// track forward
	while ( 	(this_ndx < MAX_NROF_PAGES) &&
			(Page_repository[this_ndx].page_clear == OFF)
	){

		target_page = &Page_repository[this_ndx];
		col = grid_col(target_page->pageNdx);
		rec = &Rec_undo_repository[ col ]; // undo

		for (i=0; i<MATRIX_NROF_ROWS; i++){

			// Clear their scale and lead offsets
			target_page->Track[i]->scale_pitch_offset = rec->scale_pitch_offset[i];
			target_page->Track[i]->lead_pitch_offset = rec->lead_pitch_offset[i];

			target_page->scaleLead[i] = rec->scaleLead[i];
			target_page->scaleNotes[i] = rec->scaleNotes[i];

		}

		this_ndx += 10;
	}
}

void saveTransposeRecTrack( Pagestruct* target_page ){

	if ( SOLO_transpose_row == NOP ){

		SOLO_transpose_row = my_bit2ndx(Page_getTrackRecPattern(target_page));
	}

	// set the circle section
	assign_track_scale_value( target_page->scaleLead[G_scale_ndx], SOLO_transpose_row, target_page->scaleLead );
	assign_track_scale_value( target_page->scaleNotes[G_scale_ndx], SOLO_transpose_row, target_page->scaleNotes );
}

void copyTransposeTrack( Pagestruct* target_page, Pagestruct* prev_page, int target_row, int prev_row ){

	target_page->Track[ target_row ]->scale_pitch_offset = prev_page->Track[ prev_row ]->scale_pitch_offset;
	target_page->Track[ target_row ]->lead_pitch_offset = prev_page->Track[ prev_row ]->lead_pitch_offset;

	// set the circle section
	assign_track_scale_value( track_scale_value( prev_row, prev_page->scaleLead ), target_row, target_page->scaleLead );
	assign_track_scale_value( track_scale_value( prev_row, prev_page->scaleNotes ), target_row, target_page->scaleNotes );
}


void captureTransposeRecTrackUndo(){

	signed short this_ndx = first_page_in_cluster(SOLO_rec_page->pageNdx);
	int i, col;
	Pagestruct* target_page;
	Recstruct* rec;

	// For each page in the record chain
	// track forward
	while ( 	(this_ndx < MAX_NROF_PAGES) &&
			(Page_repository[this_ndx].page_clear == OFF)
	){

		target_page = &Page_repository[this_ndx];
		col = grid_col(target_page->pageNdx);
		rec = &Rec_undo_repository[ col ]; // undo

		for (i=0; i<MATRIX_NROF_ROWS; i++){

			// Clear their scale and lead offsets
			rec->scale_pitch_offset[i] = target_page->Track[i]->scale_pitch_offset;
			rec->lead_pitch_offset[i] = target_page->Track[i]->lead_pitch_offset;

			// scaleLead and scaleNotes are always assigned together
			if ( has_track_scale(target_page->scaleLead, i) == FALSE ){

				assign_track_scale_value( SOLO_assistant_page->scaleLead[G_scale_ndx], i, target_page->scaleLead );
				assign_track_scale_value( SOLO_assistant_page->scaleNotes[G_scale_ndx], i, target_page->scaleNotes );
			}

			rec->scaleLead[i] = target_page->scaleLead[i];
			rec->scaleNotes[i] = target_page->scaleNotes[i];
		}

		this_ndx += 10;
	}
}

void clearAllTranspose(){

	signed short this_ndx = first_page_in_cluster(SOLO_rec_page->pageNdx);
	int i;
	Pagestruct* target_page;

	// For each page in the record chain
	// track forward
	while ( 	(this_ndx < MAX_NROF_PAGES) &&
			(Page_repository[this_ndx].page_clear == OFF)
	){

		target_page = &Page_repository[this_ndx];

		for (i=0; i<MATRIX_NROF_ROWS; i++){

			// Clear their scale and lead offsets
			target_page->Track[i]->scale_pitch_offset = OFF;
			target_page->Track[i]->lead_pitch_offset = OFF;
		}

		for (i=0; i<9; i++){ // clear the transpose flag and upper/lower parts

			target_page->scaleLead[i] = OFF;
			target_page->scaleLead[i] = 1 << 11;  // this is equivalent to C
			target_page->scaleNotes[i] = SCALE_SIG_CHR;
		}

		this_ndx += 10;
	}

	target_page = &Page_repository[GRID_CURSOR];
	target_page->scaleLead[G_scale_ndx] = SOLO_assistant_page->scaleLead[G_scale_ndx];
	target_page->scaleNotes[G_scale_ndx] = SOLO_assistant_page->scaleNotes[G_scale_ndx];

	captureTransposeRecTrackUndo();

	SOLO_rec_transpose_octave = OFF;
	SOLO_undo_note_all = ON;
}

void saveOrUndoTranspose(){

	if ( SOLO_rec_transpose == ON ){

		if ( !G_track_rec_bit ){
			resetTransposeRecTrack();
		}
		else {
			captureTransposeRecTrackUndo();
		}
	}

	SOLO_transpose_row = NOP;
}

void commitMix(){

	SOLO_undo_page_col = NOP;
	SOLO_undo_page_len = OFF;

	// copy all notes to undo_notes
	signed short this_ndx = first_page_in_cluster(SOLO_rec_page->pageNdx);
	int i, col;
	Notestruct* target_note;
	Notestruct* undo_note;
	Pagestruct* target_page;

	// For each page in the record chain
	// track forward
	while ( 	(this_ndx < MAX_NROF_PAGES) &&
			(Page_repository[this_ndx].page_clear == OFF)
	){

		target_page = &Page_repository[this_ndx];
		col = grid_col(target_page->pageNdx);

		tracksToRec(target_page, &Rec_repository[col]);
		copyTracks(&Rec_repository[col], &Rec_undo_repository[col]);
		Rec_repository[col].measure_count = target_page->attr_STA;
		Rec_undo_repository[col].measure_count = target_page->attr_STA;

		for (i=0; i<MAX_NROF_PAGE_NOTES; i++){

			target_note = Rec_repository[col].Note[i];
			undo_note = Rec_undo_repository[col].Note[i];
			copyNote(target_note, undo_note);
		}
		this_ndx += 10;
	}

	SOLO_edit_buffer_volatile = OFF;
	SOLO_undo_note_all = OFF;
}

void applyStrumToPageCluster(){

	SOLO_undo_page_col = NOP;
	SOLO_undo_page_len = OFF;

	// copy all notes to undo_notes
	signed short this_ndx = first_page_in_cluster(SOLO_rec_page->pageNdx);
	int i;
	Stepstruct* target_step;
	Pagestruct* target_page;

	// For each page in the record chain
	// track forward
	while ( 	(this_ndx < MAX_NROF_PAGES) &&
			(Page_repository[this_ndx].page_clear == OFF)
	){

		target_page = &Page_repository[this_ndx];

		for (i=0; i<MAX_NROF_PAGE_NOTES; i++){

			target_step = target_page->Step[grid_row(i)][grid_col(i)];
			target_step->chord_data = ( SOLO_strum << 11 ) | ( target_step->chord_data & 0x7FF );
			stepToNote(target_step, Rec_repository[grid_col(target_page->pageNdx)].Note[i]);
		}
		this_ndx += 10;
	}
}

void playSoloRecCluster(){

	unsigned int pressed = is_pressed_pagerange();
	unsigned char pressedNdx, cursor_col, col, offset, i;
	Pagestruct* target_page;

	if ( pressed != FALSE ){

		cursor_col = grid_col(GRID_CURSOR);
		pressedNdx = grid_ndx_from_key(pressed);
		if ( selected_page_cluster( pressedNdx, SOLO_rec_page->pageNdx ) != NOP ){

			target_page = &Page_repository[pressedNdx];
			GRID_p_selection[ SOLO_rec_bank ] = target_page;
			GRID_p_preselection[ SOLO_rec_bank ] = target_page;
			GRID_p_clock_presel[ SOLO_rec_bank ] = target_page;
			GRID_CURSOR = target_page->pageNdx;
			SOLO_transpose_GRID_CURSOR = GRID_CURSOR;
			col = grid_col(target_page->pageNdx);
			offset = col - cursor_col;

			for (i=0; i<offset; i++){
				SOLO_rec_measure_pos += Rec_repository[col + i].measure_count;
			}
		}
	}

	G_skip_step = NULL;
	SOLO_prev_stop = SOLO_rec_finalized;

	if ( G_clock_source != EXT ){
		sequencer_command_PLAY();
	}
}

void pageClusterEnterSoloRec(unsigned char pageNdx){

	unsigned char this_ndx = first_page_in_cluster(pageNdx);
	unsigned char start, col;
	Pagestruct* target_page;

	if ( G_run_bit == OFF ){
		GRID_bank_playmodes = 0;
		GRID_bank_playmodes |= 1 << grid_row(pageNdx);
		SOLO_rec_save_playmodes |= 1 << grid_row(pageNdx);
		SOLO_rec_has_MCC = ON; // we don't know if there is MCC data so make the matrix view available
	}

	// For each page in the record chain
	// track forward
	while ( 	(this_ndx < MAX_NROF_PAGES) &&
			(Page_repository[this_ndx].page_clear == OFF)
	){

		target_page = &Page_repository[this_ndx];
		col = grid_col(target_page->pageNdx);
		start = find_record_track_chain_start(target_page);
		Rec_repository[col].measure_count = MATRIX_NROF_ROWS - start;
		Rec_undo_repository[col].measure_count = MATRIX_NROF_ROWS - start;

		tracksToRec(target_page, &Rec_repository[col]);
		copyTracks(&Rec_repository[col], &Rec_undo_repository[col]);

		if ( SOLO_rec_page == NULL ){
			SOLO_rec_bank = grid_row(pageNdx);
			GRID_CURSOR = target_page->pageNdx;
			GRID_p_selection[ SOLO_rec_bank ] = target_page;
			GRID_p_preselection[ SOLO_rec_bank ] = target_page;
			GRID_p_clock_presel[ SOLO_rec_bank ] = target_page;
			SOLO_rec_page = target_page;
			SOLO_has_rec = ON;
			SOLO_rec_finalized = ON;
			SOLO_midi_ch = target_page->Track[start]->attr_MCH;
			SOLO_assistant_page->Track[0]->attr_MCH = SOLO_midi_ch;
		}
		this_ndx += 10;
	}

	copy_page_cluster_to_recording();
}

void breakSoloRecordingMeasureHold(){

	if ( SOLO_rec_page != NULL &&
		 G_run_bit == ON &&
		 ( G_track_rec_bit == ON || G_track_rec_bit_latch == ON ) &&
		 SOLO_rec_measure_hold != OFF &&
		 SOLO_rec_measure_hold_OTM == OFF &&
		 SOLO_rec_rehearsal == OFF
	   ){

		SOLO_has_rec = ON;
		SOLO_rec_measure_hold = OFF;
		SOLO_rec_measure_hold_OTM = ON;
		//SOLO_rec_has_MCC = ON;
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
}

void capture_note_event(
		Stepstruct* target_step,
		Pagestruct* target_page,
		unsigned char row,
		unsigned char step_col ){

	unsigned char col = grid_col(target_page->pageNdx);
	unsigned char idx = grid_ndx(row, step_col);
	Notestruct* noteRec = Rec_repository[col].Note[idx];

	Rec_repository[col].track_pitch[row] = target_page->Track[row]->attr_PIT;
	Rec_undo_repository[col].track_pitch[row] = target_page->Track[row]->attr_PIT;

	// apply the strum value
	target_step->chord_data = ( SOLO_strum << 11 ) | ( target_step->chord_data & 0x7FF );

	stepToNote(target_step, noteRec);

//	diag_printf("cap r:%d c:%d l:%d off:%d\n", row, step_col, target_step->attr_LEN, target_step->attr_VEL);
	if ( SOLO_rec_finalized == OFF ){
//		diag_printf("undo\n");
		copyNote(noteRec, Rec_undo_repository[col].Note[idx]); // undo
	}

	// Quantize notes as they are recorded
	quantizeStep(target_step, noteRec);

	SOLO_has_rec = ON;
}

void capture_mcc_event(
		Stepstruct* target_step,
		Pagestruct* target_page,
		unsigned char row,
		unsigned char step_col ){


	unsigned char col = grid_col(target_page->pageNdx);
	unsigned char idx = grid_ndx(row, step_col);
	Rec_repository[col].Note[idx]->attr_MCC = target_step->attr_MCC;

	SOLO_has_rec = ON;
}

// return TRUE if note was created
unsigned char pivotStep(
				 Stepstruct* pivot_step,
				 Notestruct* target_note,
				 signed char pivot_track_pitch,
				 unsigned char sta_offset ){

	if ( pivot_step == NULL ){
		return FALSE;
	}

	if ( Step_get_status( pivot_step, STEPSTAT_TOGGLE ) == ON ){ // we need to combine

		make_chord(pivot_step, pivot_track_pitch, target_note->attr_PIT);
		return FALSE;
	}
	else {
		clearStepNote(pivot_step);
		signed char mcc = pivot_step->attr_MCC;
//		diag_printf("N:%d %d S:%d %d\n", target_note->attr_LEN, target_note->attr_PIT, pivot_step->attr_LEN, pivot_step->attr_PIT);
		noteToStep(target_note, pivot_step);
		pivot_step->attr_STA = sta_offset;
		pivot_step->attr_MCC = mcc; // save the original MCC back
		return TRUE;
	}
}

unsigned char fineQuantizeStep(
				   Stepstruct* prev_step,
				   Stepstruct* target_step,
				   Notestruct* target_note,
				   Stepstruct* next_step,
				   unsigned char* combine_next_step,
				   signed char prev_track_pitch,
				   signed char next_track_pitch,
				   double* total_len,
				   double* total_vel){

	unsigned char center = SOLO_quantize_fine_tune_center -1;
	unsigned char edge = SOLO_quantize_fine_tune_edge -1;
	unsigned char sta_offset = 0;

	if ( center == OFF && edge == 8 ){

		return TRUE; // no fine tune filters applied
	}

	// Is the STA distance close enough to be caught by the center filter?
	if ( center > OFF ){ // bottom filter

		if ( target_note->attr_STA < STEP_DEF_START ){ // negative STA

			if ( target_note->attr_STA > center ){ // negative STA distance is too far from center

//				1 > 1 FALSE for STA 1
//				2 > 1 TRUE for STA 2
//				3 > 1 TRUE for STA 3
//				1 > 2 FALSE for STA 1
//				2 > 2 FALSE for STA 2
//				3 > 2 TRUE for STA 3

				return TRUE; // this is an include from the center out
			}
		}
		else { // positive STA

			if ( target_note->attr_STA - STEP_DEF_START < STEP_DEF_START - center ){ // positive STA distance is too far from center

//				11 - 6 = 5 < 6 - 1 = 5 FALSE for STA 5
//				10 - 6 = 4 < 6 - 1 = 5 TRUE for STA 4
//				09 - 6 = 3 < 6 - 1 = 5 TRUE for STA 3
//				11 - 6 = 5 < 6 - 2 = 4 FALSE for STA 5
//				10 - 6 = 4 < 6 - 2 = 4 FALSE for STA 4
//				09 - 6 = 3 < 6 - 2 = 4 TRUE for STA 3

				return TRUE; // this is an include from the center out
			}
		}
	}

	// Are we going to handle the large distance STA steps?
	if ( SOLO_quantize_fine_tune_edge < 9 ){

		if ( target_note->attr_STA < STEP_DEF_START ){ // negative STA

			if ( target_note->attr_STA <= (8 - edge) ){ // negative STA distance is too far from center

//					1 <= 8 - 7 = 1 TRUE for STA 1
//					2 <= 8 - 7 = 1 FALSE for STA 2
//					3 <= 8 - 7 = 1 FALSE for STA 3
//					1 <= 8 - 6 = 2 TRUE for STA 1
//					2 <= 8 - 6 = 2 TRUE for STA 2
//					3 <= 8 - 6 = 2 FALSE for STA 3

				if ( SOLO_quantize_fine_tune_drop_edge == OFF ){ // pivot

					sta_offset = STEP_MAX_START - ((8 - edge) - target_note->attr_STA);

					if ( pivotStep(prev_step, target_note, prev_track_pitch, sta_offset) == TRUE ){

						// adjust the LEN since we're moving one column back
						prev_step->attr_LEN = normalize(prev_step->attr_LEN + STEP_DEF_LENGTH,
														STEP_MIN_LENGTH,
														STEP_MAX_LENGTH);

						// the previous note was created so we need to update the totals
						*total_len += prev_step->attr_LEN;
						*total_vel += prev_step->attr_VEL;
					}
				}
				clearStepNote(target_step); // always delete the current step

				return FALSE; // do not quantize
			}
			else {
				return TRUE; // not in the filter - do quantize
			}
		}
		else { // positive STA

			if ( STEP_DEF_START - ( target_note->attr_STA - STEP_DEF_START ) <= (8 - edge) ){ // positive STA distance is too far from center

//				6 - (11 - 6 = 5) = 1 <= 8 - 7 = 1 TRUE for STA 11
//				6 - (10 - 6 = 4) = 2 <= 8 - 7 = 1 FALSE for STA 10
//				6 - (9 - 6 = 3) = 3 <= 8 - 7 = 1 FALSE for STA 9
//				6 - (11 - 6 = 5) = 1 <= 8 - 6 = 2 TRUE for STA 11
//				6 - (10 - 6 = 4) = 2 <= 8 - 6 = 2 TRUE for STA 10
//				6 - (9 - 6 = 3) = 3 <= 8 - 6 = 2 FALSE for STA 9

				if ( SOLO_quantize_fine_tune_drop_edge == OFF ){ // pivot

					sta_offset = STEP_MIN_START + ((8 - edge) - (STEP_DEF_START - ( target_note->attr_STA - STEP_DEF_START )));

					pivotStep(next_step, target_note, next_track_pitch, sta_offset);
					*combine_next_step = ON;
				}
				clearStepNote(target_step); // always delete the current step

				return FALSE; // do not quantize
			}
			else {
				return TRUE; // not in the filter - do quantize
			}
		}
	}

	return FALSE;
}

void lengthAdjust(Stepstruct* target_step){

	signed short len_adjust_steps;

	if ( SOLO_rec_legato == OFF ){

		// LEN Adjust
		if ( SOLO_len_adjust > 10 ){ // after 10 we increase by whole steps

			len_adjust_steps = (( SOLO_len_adjust * 3 ) * 10) + (( SOLO_len_adjust - 10 ) * STEP_DEF_LENGTH);
		}
		else if ( SOLO_len_adjust < -10 ){

			len_adjust_steps = (( SOLO_len_adjust * 3 ) * 10) + (( SOLO_len_adjust + 10 ) * STEP_DEF_LENGTH);
		}
		else {
			len_adjust_steps = (SOLO_len_adjust * 3); // 1/4 step
		}

		if ( len_adjust_steps >= STEP_MAX_LENGTH - target_step->attr_LEN ){

			target_step->attr_LEN = STEP_MAX_LENGTH;
		}
		else if ( target_step->attr_LEN + len_adjust_steps < STEP_MIN_LENGTH ){

			target_step->attr_LEN = STEP_MIN_LENGTH;
		}
		else {
			target_step->attr_LEN += len_adjust_steps;
		}
	}
}

Stepstruct* nextStep(Pagestruct* target_page, unsigned char track_row, unsigned char step_col){

	signed short 	this_ndx = 0;

	if (step_col == MATRIX_NROF_COLUMNS - 1){

		this_ndx = target_page->pageNdx;

		// track forward
		while ( 	(this_ndx < MAX_NROF_PAGES) &&
				(Page_repository[this_ndx].page_clear == OFF)
		){

			if ( track_row == MATRIX_NROF_ROWS -1 ){ // last track so flip to the next page

				track_row = -1;
				this_ndx += 10;
				continue;
			}

			return Page_repository[this_ndx].Step[track_row++][0];
		}

		return NULL;
	}
	else {

		return target_page->Step[track_row][step_col + 1];
	}
}

Trackstruct* nextTrack(Pagestruct* target_page, unsigned char track_row){

	signed short 	this_ndx = 0;

	this_ndx = target_page->pageNdx;

	// track forward
	while ( 	(this_ndx < MAX_NROF_PAGES) &&
			(Page_repository[this_ndx].page_clear == OFF)
	){

		if ( track_row == MATRIX_NROF_ROWS -1 ){ // last track so flip to the next page

			track_row = -1;
			this_ndx += 10;
			continue;
		}

		return Page_repository[this_ndx].Track[track_row++];
	}

	return NULL;
}

void applyEffects(){

	Trackstruct* target_track;
	Trackstruct* next_track;
	Stepstruct* target_step = NULL;
	Stepstruct* prev_step = NULL;
	Stepstruct* next_step = NULL;
	Stepstruct temp_step;
	Notestruct* target_note;
	Notestruct* first_note = NULL;
	Pagestruct* target_page = SOLO_rec_page;
	unsigned int i, j, legato_step_len = 0;
	double step_cnt = 0, total_vel = 0, total_len = 0;
	unsigned int last_step = NOP, first_step = NOP;
	unsigned short first_ndx = first_page_in_cluster(target_page->pageNdx);
	unsigned short this_ndx = first_ndx;
	unsigned char track_has_step, start_row, step_distance, step_row, prev_STA = 0, first_STA = 0;
	unsigned char track_cnt = 0;
	unsigned char skip_quantize = OFF;
	unsigned char combine_next_step = OFF;
	signed char STA_offset = 0, prev_col = 0, step_col = 0;
	signed char prev_track_pitch = 0;

	Step_init(&temp_step);

	// Use the recorded Note values to apply the effects to the Step
	//
	// ==== EFX ORDER ====
	//
	// -- FIRST PASS --
	//
	//  1. if (!LEGATO) LEN Adjust
	//  2. Fine Quantize
	// 	3. Quantize
	//  4. Sum LEN and VEL
	//
	// -- SECOND PASS --
	//
	//  5. LEGATO
	//  ||
	//  6. LEN Normalize
	//  &&
	//  7. VEL Normalize

	// track forward
	while ((this_ndx < MAX_NROF_PAGES) && (Page_repository[this_ndx].page_clear == OFF)
	){
		target_page = &Page_repository[this_ndx];
		start_row = find_record_track_chain_start(target_page);

		for (i=start_row; i<MATRIX_NROF_ROWS; i++){ // Rec rows

			// line up the recording note with the actual step
			step_row = i;

			for (j=0; j<MATRIX_NROF_COLUMNS; j++){ // Step columns

				step_col = j;
				skip_quantize = OFF;
				target_note = Rec_repository[grid_col(target_page->pageNdx)].Note[grid_ndx(step_row, step_col)];
				target_step = target_page->Step[step_row][step_col];
				target_track = target_page->Track[step_row];

				noteToStep(target_note, target_step); // copy from the recording

				if ( SOLO_rec_page == NULL || SOLO_rec_finalized == OFF ){

					continue;
				}

				if ( combine_next_step == ON ){

					Step_copy(&temp_step, target_step, TRUE); // delayed write of next step happens now
					skip_quantize = ON; // don't apply quantize again
				}

				combine_next_step = OFF;

				if ( Step_get_status( target_step, STEPSTAT_TOGGLE ) == ON ){

					step_cnt++;

					if ( first_note == NULL ){

						first_note = target_note;
						prev_track_pitch = target_track->lead_pitch_offset;
					}

					next_step = nextStep(target_page, step_row, step_col);

					if ( next_step != NULL ){

						Step_copy(next_step, &temp_step, TRUE);

						if (step_col == MATRIX_NROF_COLUMNS - 1){ // current step is at the end of the track

							next_track = nextTrack(target_page, step_row);
						}
						else {
							next_track = target_track;
						}
					}
					else {
						// we are at the end of the recording and this nextTrack will be ignored anyway
						next_track = target_track;
					}

					lengthAdjust(target_step); // adjust step length by offset

					if ( skip_quantize == OFF ){

						if ( fineQuantizeStep(prev_step,
											  target_step,
											  target_note,
											  next_step != NULL ? &temp_step : NULL,
											  &combine_next_step,
											  prev_track_pitch,
											  next_track->lead_pitch_offset,
											  &total_len,
											  &total_vel) == TRUE ){

							quantizeStep(target_step, target_note); // apply Quantize
						}
					}

					// Fine Quantize may have removed the current step so check again
					if ( Step_get_status( target_step, STEPSTAT_TOGGLE ) == ON ){

						total_len += target_step->attr_LEN;
						total_vel += target_step->attr_VEL;
					}
				}

				prev_step = target_step;
				prev_track_pitch = target_track->lead_pitch_offset;
			}
		}

		this_ndx += 10;
	}

	this_ndx = first_ndx; // reset to first page

	// track forward
	while ((this_ndx < MAX_NROF_PAGES) && (Page_repository[this_ndx].page_clear == OFF)
	){
		target_page = &Page_repository[this_ndx];
		start_row = find_record_track_chain_start(target_page);

		for (i=start_row; i<MATRIX_NROF_ROWS; i++){ // Rec rows

			// line up the recording note with the actual step
			step_row = i;
			track_has_step = OFF;

			for (j=0; j<MATRIX_NROF_COLUMNS; j++){ // Step columns

				step_col = j;
				target_note = Rec_repository[grid_col(target_page->pageNdx)].Note[grid_ndx(step_row, step_col)];

				if ( Step_get_status( target_page->Step[step_row][step_col], STEPSTAT_TOGGLE ) == ON ){

					target_step = target_page->Step[step_row][step_col];

					if ( SOLO_rec_legato == ON ){ // LEGATO

						if ( last_step != NOP ){

							if ( track_has_step == OFF && first_step != prev_col ){
								step_distance = ((MATRIX_NROF_COLUMNS * track_cnt) + ( MATRIX_NROF_COLUMNS - prev_col ) + step_col);
							}
							else {
								step_distance = ((MATRIX_NROF_COLUMNS * track_cnt) + ( step_col - prev_col ));
							}
							STA_offset = ( target_step->attr_STA - STEP_DEF_START ) - ( prev_STA - STEP_DEF_START );
							legato_step_len = (step_distance * STEP_DEF_LENGTH) + STA_offset;
							prev_step->attr_LEN = (legato_step_len <= STEP_MAX_LENGTH) ? legato_step_len : STEP_MAX_LENGTH;
						}
						else {

							last_step = ON;
							prev_STA = target_step->attr_STA;
							prev_step = target_step;
							prev_col = step_col;
							first_step = step_col;
							first_STA = target_step->attr_STA;
							continue;
						}
					}

					prev_STA = target_step->attr_STA;
					prev_step = target_step;
					prev_col = step_col;
					track_cnt = 0;
					track_has_step = ON;
				}
				else {
					// Normalize LEN
					target_step->attr_LEN += ((( total_len/step_cnt ) - target_step->attr_LEN ) / MATRIX_NROF_COLUMNS ) * SOLO_normalize_len;
				}
				// Normalize VEL
				target_step->attr_VEL += ((( total_vel/step_cnt ) - target_step->attr_VEL ) / MATRIX_NROF_COLUMNS ) * SOLO_normalize_vel;
			}

			if ( track_has_step == OFF ){
				track_cnt++;
			}
		}

		this_ndx += 10;
	}

	// LEGATO
	// last note of legato rolls over to the first note of the first page
	if ( SOLO_rec_legato == ON ){

		if ( last_step != NOP ){

			step_distance = (MATRIX_NROF_COLUMNS * track_cnt) +
							(MATRIX_NROF_COLUMNS - prev_col) +
							first_step;
			STA_offset = ( first_STA - STEP_DEF_START ) - ( prev_STA - STEP_DEF_START );
			legato_step_len = (step_distance * STEP_DEF_LENGTH) + STA_offset;
			target_step->attr_LEN = (legato_step_len <= STEP_MAX_LENGTH) ? legato_step_len : STEP_MAX_LENGTH;
		}
	}
}

void SoloRecRotEffects(){

	if ( G_zoom_level == zoomSOLOREC ){

		if ( SOLO_rec_strum_latch == ON && G_run_bit == ON && G_track_rec_bit == ON ){

			SOLO_strum = 9; // reset
		}

		if ( SOLO_apply_effects_alarm == ON ){

			SOLO_apply_effects_alarm = OFF;
			applyEffects();
		}
	}

	if ( delay_CC_type != OFF ){

		MIDI_send( delay_CC_type, delay_CC_val0, delay_CC_val1, delay_CC_val2 );
		delay_CC_type = OFF;
		delay_CC_val0 = OFF;
		delay_CC_val1 = OFF;
		delay_CC_val2 = OFF;
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

	SOLO_undo_page_col = grid_col(first_page_in_cluster(SOLO_pos_in->pageNdx));
	SOLO_undo_page_len = grid_col(last_page_in_cluster(SOLO_pos_in->pageNdx)) - SOLO_undo_page_col + 1;
	SOLO_rec_undo_measure_count = SOLO_rec_measure_count;

	shift_down_freeflow_track_chain(SOLO_pos_out, last_shift_measure, count_out);
	cut_freeflow_track_chain(SOLO_pos_in, last_cut_measure, count_in, ON);

	if ( SOLO_pos_in != SOLO_pos_out ){

		cut_freeflow_track_chain(SOLO_pos_out, last_shift_measure, count_out, OFF);
	}

	SOLO_pos_marker_out = OFF;
	SOLO_pos_in = NULL;

	SOLO_edit_buffer_volatile = ON;

	MIX_TIMER = ON;
	// Setup alarm for the MIX TIMER
	cyg_alarm_initialize(	alarm_hdl,
							cyg_current_time() + TIMEOUT_VALUE,
							0 );
}

