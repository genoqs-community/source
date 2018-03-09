

#include "key_tap_operation.h" 

#include "key_chainer_operation.h"

	
// Translate key presses in numeric quadrant to indexes
unsigned char BK_KEY_to_xdx( unsigned char keyNdx ){

	unsigned char result = 0;

	switch( keyNdx ){

		case 201:	result =  1; 	break;
		case 200:	result =  2; 	break;
		case 199:	result =  3; 	break;
		case 198:	result =  4; 	break;
		case 197:	result =  5; 	break;
		case 207:	result =  6; 	break;
		case 206:	result =  7; 	break;
		case 216:	result =  8; 	break;
		case 215:	result =  9; 	break;
		case 224:	result = 10; 	break;		
	}

	return result;
}



// Resets all Track locators in the given page.
void set_track_locators(	Pagestruct* target_page, 
							unsigned char target_LOC,
							unsigned char target_TTC	){
	
	unsigned char row=0;
	unsigned char i=0;
	
	// Iterate through rows and affect only the tracks that are playing.
	for ( row=0; row < MATRIX_NROF_ROWS; row++ ){

		if ( target_page->Track[row]->attribute[ATTR_LOCATOR] != 0 ){

			// Set the TTC value
			target_page->Track[row]->TTC = target_TTC;

			// Set the LOC value
			target_page->Track[row]->attribute[ATTR_LOCATOR] = target_LOC;
			target_page->Track[row]->trigger_set_ndx = target_LOC;

			// Empty the track trigger sets
			for (i=1; i<MATRIX_NROF_ROWS; i++ ){
				target_page->Track[row]->trigger[i]=0;
			}

		}

		// Reset the track clock divisor skipper status (higher nibble)
		target_page->Track[row]->attribute[ATTR_TEMPOMUL_SKIP] &= 0x0F;
	}

	// Reset the chain counter when we are zeroing all tracks
	if (	( target_LOC == 0 )
		&&	( target_TTC == 0 )
		){
	
		target_page->chainCounter = 0;

		// Iterate through the chain and set make the head play
		for ( row=0; row < MATRIX_NROF_ROWS; row++ ){
		
			// Make Head track the Play track
			target_page->Track[row]->chain_data[PLAY] = 
				target_page->Track[row]->chain_data[HEAD];
		}
	}
}



// Resets all locators associated with a given page to given LOC and TTC
void set_page_locators( 	Pagestruct* target_page, 
							unsigned char target_LOC, 
							unsigned char target_TTC ){

	// Reset page master locator
	target_page->locator = target_LOC;

	// Reset page Cadence locator
	// target_page->cadence->locator = target_LOC;

	// Reset all track locators (including chain counters)
	set_track_locators( target_page, target_LOC, target_TTC );
}



// Send the MIDI OFF notes belonging to a page 
void stop_playing_page( 	Pagestruct* target_page, 
							unsigned char in_TTC_ABS_VALUE ){
	
	unsigned int i = 0;

	if( target_page == NULL ) return;

	MIDIeventstruct* off_event = NULL;
	// d_iag_printf( "stopping page:%d\n", target_page->pageNdx );

	// Play all still pending MIDI OFF data
	for( i=0; i < MIDI_queue_counter; i++ ){

		// Get the head of the MIDI queue
		off_event = MIDI_queue_remove_head();

		if ( off_event != NULL ){
			
			// Send the MIDI event to the MIDI port
			MIDI_send_event( off_event );
		}
		else{

			break;
		}
	} // MIDI OFF queue iterator
	
	
	// Set all rebound bits in the page to default state
	for ( i=0; i<MATRIX_NROF_ROWS; i++ ){

		target_page->Track[i]->attribute[ATTR_MISC] =
			( target_page->Track[i]->attribute[ATTR_MISC] & 0xFE );
	}
}



//
// Boolean: returns true if there is a change request pending, false otherwise
//
unsigned char selection_change_request_pending() {

	unsigned char i=0;

	for (i=0; i<GRID_NROF_BANKS; i++) {

		if (GRID_p_selection[i] != GRID_p_preselection[i]) {

			return TRUE;
		}
	}
	return FALSE;
}



// 
// Boolean: returns true if the index is in the grid selection, false otherwise
//
unsigned char is_selected_in_GRID( Pagestruct* target_page ){

	unsigned char i=0;

	for ( i=0; i<GRID_NROF_BANKS; i++ ){

		if ( GRID_p_selection[i] == target_page ){

			return TRUE;
		}
	}
	return FALSE;
}



//
// Select the target_page to be played by the grid. 
// Automatically unselect the rest of the bank. Bank is horizontal 
void grid_select( 	Pagestruct* target_page, 
					unsigned char target_status ){

	unsigned char 	j=0;

	unsigned char   k = GRID_switch_mode;
	if ( RUNBIT == OFF ){
				
		GRID_switch_mode = GRID_SWITCH_DIRECT;
	}


	// Find the first page in the bank of the target page (from L to R)
	j = target_page->pageNdx % 10;

	//d_iag_printf( "->grid_select():%d to %d mode:%d\n", 
	//	target_page->pageNdx, target_status, GRID_switch_mode );
	
	// If page is to be selected from its row, select it now
	switch( target_status ) {

		case ON:			
			// Page needs to be turned ON
			switch( GRID_switch_mode ){
				
				case GRID_SWITCH_OCLOCK:

					// Add page to the GRID_clock_presel to be switched into presel later on
					GRID_p_clock_presel[ j%10 ] = target_page;

					break;

				case GRID_SWITCH_DIRECT:

					// Add page to the GRID_preselection directly
					GRID_p_preselection[ j%10 ] = target_page;

					// Propagate the change over to clock presel
					GRID_p_clock_presel[ j%10 ] = GRID_p_preselection[ j%10 ];
					break;
			}
			break;


		case OFF:
			// Page needs to be turned OFF
			switch( GRID_switch_mode ){

				case GRID_SWITCH_OCLOCK:			

					// Add page to the GRID_clock_presel to be switched into presel later on
					GRID_p_clock_presel[ j%10 ] = NULL;

					break;

				case GRID_SWITCH_DIRECT:

					// Remove page from GRID_preselection.	
					GRID_p_preselection[ j%10 ] = NULL;	

					// Propagate the change over to clock presel
					GRID_p_clock_presel[ j%10 ] = GRID_p_preselection[ j%10 ];
					break;
			}
			break;
	
	} // switch( target status 

	//d_iag_printf( "  [%d] G_presel:%d G_clk_pre:%d ->%d\n", 
	// 	j%10, 
	// 	GRID_p_preselection[j%10]->pageNdx, 
	// 	GRID_p_clock_presel[j%10]->pageNdx, 
	// 	target_status );

	// Quick and dirty fix
	if ( RUNBIT == OFF ){
				
		GRID_switch_mode = k;
	}

	// If the sequencer is stopped, act immediately.
	if (	(RUNBIT == OFF)  	){
		
		// Direct selection of the preselections, because not done by the player
		select_page_preselections();
	}
}





// Jumpstart the sequencer by setting all conditions
void sequencer_START() {
	
	// Reset the MIDI_timestamp;
	MIDI_timestamp = 0;

	// Play start condition
	TIMER_TICK_CNTR = 1;
	TTC_ABS_VALUE 	= 0;
	TEMPO_TRACKER	= 1;
	MIDI_TICK_CNTR 	= 1;

	// Flag for the player in chain mode
	SEQUENCER_JUST_STARTED = ON;


	// Reset the timer 
	TIMER_REFILL_update();		
	// Refill the timer counter
	HAL_WRITE_UINT32( E7T_TCNT1, TIMER_REFILL );
	// Turn the blinker on
	GLOBALBLINKER = ON;

			
	// Set the sequencer running conditions
	RUNBIT = ON;	
	PAUSEBIT = OFF;	

	// MIDI CLOCK: START - only when clock is active
	if ( CLOCK_SOURCE == INT ){

		// Start external device
		MIDI_send( MIDI_CLOCK, MIDICLOCK_START, 0, 0);

		// Send the first tick
		// MIDI_send( MIDI_CLOCK, MIDICLOCK_CLOCK, 0, 0 );
	}

	// Init the playload for the start
	PLAY_LOAD = 0;

	// Drive the sequencer forward: Jumpstart the play
	driveSequencer();
}


// Set the TTCs of all tracks in page to in_value
void set_track_TTCs( Pagestruct* target_page, unsigned char in_value ){

	unsigned char row = 0;
	
	if ( target_page == NULL ) return;

	for( row = 0; row < MATRIX_NROF_ROWS; row++ ){

		target_page->Track[row]->TTC = in_value;
	}
}



// Halts the sequencer, disabling running activities, 
// ..but leaving locators untouched
void sequencer_HALT() {
	
	unsigned char 	i 			= 0;
	Pagestruct* 	target_page = NULL;

	// Turn of running bit first
	RUNBIT 		= OFF;

	// Set the pause bit for now, preparing MTC continue
	PAUSEBIT 	= ON;

	// Stop playing all pages of the grid selection
	for (i=0; i<GRID_NROF_BANKS; i++) {
		
		// Store the page address in the pointer buffer
		if (GRID_p_selection[i] != NULL) {

			target_page = GRID_p_selection[i];

			// Stop playing the target page and do not reset its track locators
			stop_playing_page( target_page, TTC_ABS_VALUE );
			
			// Set the TTC values of all tracks to 0
			set_track_TTCs( target_page, 0 );
		}
		
	} // Iterate through the playing pages of the grid

}



// Un-halt the sequencer - typically after a pause state
void sequencer_UNHALT(){
	
	// Play start condition
	TIMER_TICK_CNTR = 1;
	TTC_ABS_VALUE 	= 0;
	TEMPO_TRACKER	= 1;
	MIDI_TICK_CNTR 	= 1;
	
	// Reset the timer 
	TIMER_REFILL_update();
			
	// Set the sequencer running conditions
	RUNBIT = ON;	
	PAUSEBIT = OFF;	

	driveSequencer();
}



// Brings all sequencer locators into start position -> inlcuding rewinding all chains in page
void sequencer_RESET(){

	unsigned char 	i 	= 0;
	Pagestruct* 	target_page = NULL;
	
	// Remove the pause	state for good;
	PAUSEBIT 	= OFF;

	// Reset the global locator
	GLOBAL_LOCATOR = 0;

	// RESET LOCATORS AND CHAIN COUNTERS FOR THE PLAYING PAGES

	// All pages in the grid selection - was the repository..
	for (i=0; i<GRID_NROF_BANKS; i++) {

		// Store the page address in the pointer buffer
		if (GRID_p_selection[i] != NULL) {

			target_page = GRID_p_selection[i];

			// Reset all locators in target page	
			set_page_locators( target_page, 0, 0 );
		
			// Reset the repeates values for the target page
			target_page->repeats_left = target_page->repeats;

			// Reset all chains in the target page
			// ..
		}
	}	


	// Bring all chained banks to the first page in chain.
	for ( i=0; i < GRID_NROF_BANKS; i++ ){

		// Set the chain start in the clock preselection
		compute_chain_startpoints( i );
	
		// Move the clock preselection into plain preselection
		preselect_clock_presel( i );

		// Switch the gridbank
		switch_gridbank( i );
	}

	// d_iag_printf( "sequencer RESET: STEP2\n" );

	// Reset the running status bytes
	running_status_byte_OUT_UART[0] = 0;
	running_status_byte_OUT_UART[1] = 0;
	running_status_byte_IN_UART[0]	= 0;
	running_status_byte_IN_UART[1]	= 0;
	
	// Init the recording event queue
	NOTE_IN_init();

}



// STOPs the sequencer bringing it in start position
void sequencer_STOP() {

	// Halt the sequencer
	sequencer_HALT();	

	// MIDI CLOCK: STOP
	if ( CLOCK_SOURCE == INT ){

		// Only active when the clock is active
		MIDI_send( MIDI_CLOCK, MIDICLOCK_STOP, 0, 0 );		
	}

	// Reset the locators
	sequencer_RESET();

	// Reset the MIDI_timestamp;
	MIDI_timestamp = 0;

	// Reset the PLAY load
	PLAY_LOAD = 0;
}


// Just a wrapper to stay call-name consistent
void sequencer_command_STOP(){
	
	sequencer_STOP();
}


// Toggles the pause state of the sequencer
void sequencer_command_PAUSE(){
	
	switch (RUNBIT) {

		case ON:

			sequencer_HALT();
			
			// MIDI CLOCK: STOP
			if ( CLOCK_SOURCE == INT ){

				// Only active when the clock is active
				MIDI_send( MIDI_CLOCK, MIDICLOCK_STOP, 0, 0 );
			}
			break;			

		case OFF:

			// Prevent from acting when not in pause mode
			if( PAUSEBIT == ON ){

				sequencer_UNHALT();								

				// MIDI CLOCK: CONTINUE
				if ( CLOCK_SOURCE == INT ){

					// Only active when the clock is active
					MIDI_send( MIDI_CLOCK, MIDICLOCK_CONTINUE, 0, 0 );
				}								
			}
			break;
	} // RUNBIT SWITCH
}



// Activate the sequencer play as needed
void sequencer_command_PLAY(){

	// React only if the sequencer is stopped
	switch( RUNBIT ){
		
		// Sequencer is IDLE
		case OFF:
			switch( PAUSEBIT ){
				
				// Sequencer is PAUSED
				case ON:
					// Stop the sequencer - clear anything left in the pipe to start fresh
					sequencer_STOP();	

					// no BREAK - fallthrough intended - recovering from pause state
	
				// Sequencer is STOPPED
				case OFF:	
					// Start the sequencer
					sequencer_START();	
					break;
			}			
			break;

		// Sequencer is ACTIVE
		case ON:
			// Nothing to do
			break;

	}// switch( RUNBIT )

}




// Evaluate the selection status of the target note and return it.
// It can be any of LEAD, ON, OFF
unsigned char how_selected_in_current_scale( 	Pagestruct* target_page, 
												unsigned char target_note ){

	unsigned char result = 0;
	// This implies the bit shift pattern defined as in the external table. 
	// ..Book page 116.ff

	//d_iag_printf( "target note:%d vs. scale\n", target_note );	
	//my_print_bits32( 1 << (target_note - 12) );
	//my_print_bits32( VIEWER_page->current_scale );

	// Note is selected;
	if ( target_page->current_scale & (1 << (target_note - 12)) ){

		// When selected it also could be lead, check condition
		if ( target_page->scaleLead[0] & (1 << (target_note - 12)) ){	

			// LEAD has the highest importance as it implies the rest
			result = LEAD;
		}
		else {

			result = ON;
		}
	}
	
	else {

		// Note is not selected 
		result = OFF;
	}

	// d_iag_printf( "returning result:%d\n", result );
	return result;
}


// Evaluate the selection status of the target note and return it.
// It can be any of LEAD, ON, OFF
unsigned char how_selected_in_scale( 	Pagestruct* target_page, 
										unsigned char target_note ){

	// This implies the bit shift pattern defined as in the external table. 
	// ..Book page 116.ff
	
	// Note is selected;
	if ( target_page->scaleNotes[0] & (1 << (target_note - 12)) ){

		// When selected it also could be lead, check condition
		if ( target_page->scaleLead[0] & (1 << (target_note - 12)) ){	

			// LEAD has the highest importance as it implies the rest
			return LEAD;
		}
		else {

			return ON;
		}
	}
	
	else {

		// Note is not selected 
		return OFF;
	}
}



// Select a note in the scale selection according to the status passed along.
// Applies to target_page directly. The status can be ON, LEAD, OFF
// Remember that LEAD means this is the LEAD tone of the scale
// Automation can be done here to several levels and quality.
void select_in_scale( 	Pagestruct* target_page,
						unsigned char target_note, 
						unsigned char target_state ){
	
	// d_iag_printf( "Selecting in scale: k:%d state:%d\n", 
	//									target_note, target_state );
	
	// Build the shift distance
	if (target_state == ON) {

		// Make selection of the regular note: add note to scale
		target_page->scaleNotes[0] |= 1 << (target_note - 12);

	} 
	else if (target_state == LEAD) {

		// Select the new scale lead
		target_page->scaleLead[0] = 1 << (target_note - 12);
	}
	else if (target_state == OFF) {

		// OFF: Deselect both regular and LEAD flag 
		// target_page->scaleSelection &= (0xFFFFFF ^ (1<<target_note)      );
		target_page->scaleNotes[0] &= (0xFFF ^ (1 << (target_note - 12)));

		// target_page->scaleSelection &= (0xFFFFFF ^ (1<<(target_note-12)) );		
		target_page->scaleLead[0] &= (0xFFF ^ (1 << (target_note - 12)));
	}
}



//
// Convert the index of a key in the Octave circle to a key Ndx to be used
// in further operations
unsigned char keyNdx_to_noteNdx( unsigned int keyNdx ){

	unsigned char i = 0;
	// Transform keyNdx to note index
	switch (keyNdx) {
		case KEY_NOTE_C:	i = NOTE_C;			break;
		case KEY_NOTE_Cis:	i = NOTE_Cis;		break;
		case KEY_NOTE_D:	i = NOTE_D;			break;
		case KEY_NOTE_Dis:	i = NOTE_Dis;		break;
		case KEY_NOTE_E:	i = NOTE_E;			break;
		case KEY_NOTE_F:	i = NOTE_F;			break;
		case KEY_NOTE_Fis:	i = NOTE_Fis;		break;
		case KEY_NOTE_G:	i = NOTE_G;			break;
		case KEY_NOTE_Gis:	i = NOTE_Gis;		break;
		case KEY_NOTE_A:	i = NOTE_A;			break;
		case KEY_NOTE_Ais:	i = NOTE_Ais;		break;
		case KEY_NOTE_B:	i = NOTE_B;			break;
		case KEY_NOTE_Cup:	i = NOTE_C;			break;								
	}
	return i;
}



// Convert a pitch value to a key Ndx to be used in further operations
unsigned char pitch_to_noteNdx( unsigned char in_pitch ){

	unsigned char i = 0;

	// Transform keyNdx to note index
	switch ( in_pitch % 12 ) {

		case 0:		i = NOTE_C;			break;
		case 1:		i = NOTE_Cis;		break;
		case 2:		i = NOTE_D;			break;
		case 3:		i = NOTE_Dis;		break;
		case 4:		i = NOTE_E;			break;
		case 5:		i = NOTE_F;			break;
		case 6:		i = NOTE_Fis;		break;
		case 7:		i = NOTE_G;			break;
		case 8:		i = NOTE_Gis;		break;
		case 9:		i = NOTE_A;			break;
		case 10:	i = NOTE_Ais;		break;
		case 11:	i = NOTE_B;			break;
	}

	return i;
}


// Extracts the scale bitpattern from the tracks of the current page
unsigned short extract_current_scale( Pagestruct* target_page ){

	unsigned char i = 0;
	unsigned char pitch = 0;
	unsigned short result = 0;
	
	// Build the scale - track by track
	for ( i=0; i<MATRIX_NROF_ROWS; i++ ){
						
		if ( target_page->scaleStatus == OFF ){

			pitch = target_page->Track[i]->attribute[ATTR_PITCH] ;
		}
		else {
			pitch = 	
//					target_page->Track[i]->attribute[ATTR_PITCH] +
					target_page->Track[i]->scale_pitch_offset +
					target_page->Track[i]->lead_pitch_offset ;
		}

		// Mark the note in the scale selection. This is the first 12 bits of the field.
//		pitch = 	
//					target_page->Track[i]->attribute[ATTR_PITCH] +
//					target_page->Track[i]->scale_pitch_offset +
//					target_page->Track[i]->lead_pitch_offset ;

		result |=  1 << (11 - ( pitch % 12 ));

		// if ( target_page->pageNdx == 0 ){
		// d_iag_printf( "i:%d extracted:%d scaleNotes:\n", i, pitch );
		// my_print_bits32( target_page->scaleNotes[0] );
		// }
	}
	
	// d_iag_printf( "current scale extracted\n" );

	return result;
}



// 
// Extracts the current pitches of the page into the scale selection variable 
// to be used and changed in the scale mode. It only works with track pitch offset values.
// Steps may have additional pitch offsets which are not considered at the moment.
// 
// The result will be as follows: lowest absolute note found makes up the lead;
// The other notes will be added to the lead scale as if they were in the same octave,
// hence disregarding any octave trans-position of the tracks.
//
void extract_scale_and_lead( Pagestruct* target_page ){
	
	unsigned char 	i = 0,
					j = 127;
	
	signed char pitch = 0;
	

	if ( target_page->force_to_scale == ON ){

		target_page->scaleNotes[0] = target_page->current_scale;
		target_page->scaleLead[0] = target_page->scaleLead_old;
		return;
	}

	// Clear the scaleNotes to start with..
	target_page->scaleNotes[0] = 0;

	// Find the lowest absolute note value. This is the lead.
	for (i=0; i<MATRIX_NROF_ROWS; i++) {

		pitch = 	target_page->Track[i]->attribute[ATTR_PITCH]
					+
					target_page->Track[i]->scale_pitch_offset +
					target_page->Track[i]->lead_pitch_offset ;

		if ( pitch < j ) {

			// Always remember the lowest encountered value
			j = pitch;
		}
	}
	
//	 if ( target_page->pageNdx == 0){
//		d_iag_printf( "*extract_scale: lowest pitch:%d\n", j );
//	 }
	
	// Keep the lead
	target_page->scaleLead[0] = 1 << ( 11 - (j%12) );

	// Set it as the scaleLead_old as well, and remember the scale
	target_page->scaleLead_old = target_page->scaleLead[0];
	target_page->scaleNotes_old = target_page->scaleNotes[0];

	// Build the scale - track by track
	for ( i=0; i<MATRIX_NROF_ROWS; i++ ){
						
		// Mark the note in the scale selection. This is the first 12 bits of the field.
		pitch = 	
					target_page->Track[i]->attribute[ATTR_PITCH] +
					target_page->Track[i]->scale_pitch_offset +
					target_page->Track[i]->lead_pitch_offset ;

		target_page->scaleNotes[0] |=  1 << (11 - ( pitch % 12 ));

		// if ( target_page->pageNdx == 0 ){
		// d_iag_printf( "i:%d extracted:%d scaleNotes:\n", i, pitch );
		// my_print_bits32( target_page->scaleNotes[0] );
		// }
	}
	
		// if ( target_page->pageNdx == 0 ){
		// d_iag_printf( "*extract_scale: scale/lead/_old\n" );
		// my_print_bits32( target_page->scaleNotes[0] );
		// my_print_bits32( target_page->scaleLead[0] );
		// my_print_bits32( target_page->scaleLead_old );
		// }
}



// 
// Advances a bit in the given direction over an interval of 12
// This can be made more universal, but not right this moment please.
//
unsigned int advance_scale_search_bit( 	unsigned int search_bit, 
										unsigned char direction ){

	unsigned int output = 0;

	switch( direction ){

		case INC:
			if (search_bit == 1) {
				// Wrap condition: start over on the left side
				output = (1 << 11);
			}
			else {
				// Advance the search bit towards right
				output = search_bit >> 1;				
			}
			break;

		case DEC:
			if (search_bit == (1 << 11)) {
				// Wrap condition: start over on the right side
				output = 1;
			}
			else{
				// Advance search bit towards left
				output = search_bit << 1;
			}
			break;
	}

	return output;
}



// Used to compute step chord aux distance
unsigned int advance_chord_search_bit( 	unsigned int search_bit, 
										unsigned char direction ){

	unsigned int output = 0;

	switch( direction ){

		case DEC:
			if (search_bit == 1) {
				// Wrap condition: start over on the left side
				output = (1 << 11);
			}
			else {
				// Advance the search bit towards right
				output = search_bit >> 1;				
			}
			break;

		case INC:
			if (search_bit == (1 << 11)) {
				// Wrap condition: start over on the right side
				output = 1;
			}
			else{
				// Advance search bit towards left
				output = search_bit << 1;
			}
			break;
	}

	return output;
}




// In target page, modify the scale_pitch_offset of tracks
// ..according to the passed parameters. 
void shift_track_pitches( 	Pagestruct* target_page, 
							unsigned char degrees, 
							unsigned char direction ){

	// Holds the bit indicating the current track pitch
	unsigned int track_pitch_bit = 0;
	unsigned int search_bit		= 0;
	signed char offset	= 0;	
	unsigned char
					d = 0,					// keeps tracks of found degrees
					i = 0,					// counter variable
					row = 0;				// row iterator


	// d_iag_printf( "      *shifting Pitches: deg:%d dir:%d\n", degrees, direction);	

	// Iterate through the tracks
	for (row=0; row<MATRIX_NROF_ROWS; row++) {
		
		// Find out the pitch of the track relative to the scale..
		track_pitch_bit =  	
			1 << (11 - ( ( 
//								target_page->Track[row]->attribute[ATTR_PITCH]
//							+	
								target_page->Track[row]->scale_pitch_offset
							+	target_page->Track[row]->lead_pitch_offset
							) 
							
							% 12 )   ) ;
				
		// Starting position of the degree counter (d)
		search_bit = track_pitch_bit;

		// Iterate through the scale with cardinality to compute offset
		d = 0;
	
		// d_iag_printf( "      ->track:%d deg:%d search_pos:%d\n", row, degrees, 
		//					my_bit2ndx(search_bit) );
		// my_print_bits32( search_bit );
		// my_print_bits32( VIEWER_page->scaleNotes[0] );
		
		// Compute the required pitch offset
		// Iterate through the scale. Max 12 degrees.
		for (i=0; i<12; i++) {

			// Number of degrees has not been reached yet
			if ( d < degrees ) {
				
				// Advance the search bit
				search_bit = advance_scale_search_bit( search_bit, direction );						

				// d_iag_printf( "      ->track:%d deg:%d current search bit pos:%d\n", row, degrees, 
				//					my_bit2ndx(search_bit) );
				// my_print_bits32( search_bit );

				// Increment or decrement the offset
				offset ++;
				// if 		(direction == INC) 	{	offset ++; }
				// else if (direction == DEC) 	{	offset --; }
				
				// If degree found, also increment d
				// if ( ((target_page->scaleSelection >> 12) & search_bit) != 0 ){
				if ( (target_page->scaleNotes[0] & search_bit) != 0 ) {

					// d_iag_printf( "      degree %d found at off:%d\n", d, offset);					
					d ++;
				}
			}
			else {

				// Nr. of needed degrees found, so exit
				break;
			}
		} // Scale selection iterator			

		// if (direction == INC){   leave the offset as is  }
		if (direction == DEC) 	{	offset = -offset; 		}

		// d_iag_printf( "        tr:%d exit with offset:%d\n", row, offset);

		// Apply the offset to the track pitch offset variable
		target_page->Track[row]->lead_pitch_offset += offset;

		// Reset the offset variable
		offset = 0;

	} // Track iterator
	
}



// Take a pitch and return the positive offset to the next degree in the scale passed along
unsigned char offset_to_next_chord_aux(		unsigned short in_scale, 
											unsigned char  pitch ){
	
	unsigned char 	
					i = 0, 
					offset = 0;
	unsigned int 	
					pitch_bit = 1 << ((pitch % 12));
	
	// d_iag_printf( "   off_2nextChordAux: Scale + pitch_bit(pi:%d):\n", pitch );
	// my_print_bits32( pitch_bit );	
	// my_print_bits32( in_scale );

	// Iterate through the 12 half step scale
	for (i=0; i<12; i++) {
	
		// Increment the pitch_bit position first.
		pitch_bit = advance_chord_search_bit( pitch_bit, INC );
 
 		// Count the half step offset
		offset ++;
		//	my_print_bits32( pitch_bit );

		// If the next degree has been reached, break the loop
		if ( (pitch_bit & in_scale) != 0 ){

			// Found the next degree
			break;
		}			
	}		
	
	// d_iag_printf( "   returning offset_:%d pitch/scale:\n", offset );
	// my_print_bits32( pitch_bit );	
	// my_print_bits32( in_scale );

	// The obvious
	return offset;
}



// Take a pitch and return the positive offset to the next degree in the scale of its page
unsigned char offset_to_next_current_degree(	Pagestruct* target_page, 
												unsigned char pitch ){
	
	unsigned char 	
					i = 0, 
					offset = 0;
	unsigned int 	
					pitch_bit = 1 << (11 - (pitch % 12));
	
//		d_iag_printf( "   off_2next: Scale + pitch_bit(pi:%d):\n", pitch );
//		my_print_bits32( target_page->scaleNotes );

	// Iterate through the 12 half step scale
	for (i=0; i<12; i++) {
	
		// Increment the pitch_bit position first.
		pitch_bit = advance_scale_search_bit( pitch_bit, INC );
 
 		// Count the half step offset
		offset ++;
		
//		my_print_bits32( pitch_bit );

		// If the next degree has been reached, break the loop
		if ( (pitch_bit & target_page->current_scale) != 0 ){

			// Found the next degree
			break;
		}			
	}
		
//	d_iag_printf( "   returning offset_:%d\n", offset );

	// The obvious
	return offset;
}




// Take a pitch and return the positive offset to the next degree in the scale of its page
unsigned char offset_to_next_degree(	Pagestruct* target_page, 
										unsigned char pitch ){
	
	unsigned char 	
					i = 0, 
					offset = 0;
	unsigned int 	
					pitch_bit = 1 << (11 - (pitch % 12));
	
//		d_iag_printf( "   off_2next: Scale + pitch_bit(pi:%d):\n", pitch );
//		my_print_bits32( target_page->scaleNotes );

	// Iterate through the 12 half step scale
	for (i=0; i<12; i++) {
	
		// Increment the pitch_bit position first.
		pitch_bit = advance_scale_search_bit( pitch_bit, INC );
 
 		// Count the half step offset
		offset ++;
		
//		my_print_bits32( pitch_bit );

		// If the next degree has been reached, break the loop
		if ( (pitch_bit & target_page->scaleNotes[0]) != 0 ){

			// Found the next degree
			break;
		}			
	}
		
//	d_iag_printf( "   returning offset_:%d\n", offset );

	// The obvious
	return offset;
}




// Modifies the track lead offset according to the new lead, 
// without modifying the pitch intervals.
void export_scale_lead ( Pagestruct* target_page ){

	unsigned char	
					direction 	= 0,
					newLeadNdx 	= 0,
					row = 0,
					oldLeadNdx 	= 0,
					degrees 	= 0;
	
	unsigned int 	bitmask 	= 0;

	// Start over with a fresh old scale lead which goes with
	// the cleared lead pitch offsets..
	target_page->scaleLead_old = 1 << 11;

	// Index of newLead
	newLeadNdx = my_bit2ndx( target_page->scaleLead[0] );

	// Index of oldLead
	oldLeadNdx = my_bit2ndx( target_page->scaleLead_old );

	// Clear the lead offsets in tracks
	for (row=0; row<MATRIX_NROF_ROWS; row++) {
		
		target_page->Track[row]->lead_pitch_offset = 0;
		// d_iag_printf( "%d  ", target_page->Track[row]->lead_pitch_offset );
	}
	
	// d_iag_printf( "   *export_scale_LEAD: NL=%d OL=%d\n", newLeadNdx, oldLeadNdx);
	
	// my_print_bits32( target_page->scaleLead[0] );
	// my_print_bits32( target_page->scaleLead_old );

	// Generate the bitmask of the interval in the scale	
	bitmask = (0xfff >> (11 - newLeadNdx)) ^ (0xfff >> (11 - oldLeadNdx)) ;

	// Produce the scale selection and its cardinality
	degrees = my_bit_cardinality( bitmask & target_page->scaleNotes[0] );
		
	// Find trans-position direction. Somewhat twisted, oh well.. it possibly works.
	// Notice: the == case missing and does not make any changes here
	if 		(newLeadNdx < oldLeadNdx) 		{	direction = INC;	}
	else if	(newLeadNdx > oldLeadNdx)		{	direction = DEC;	}


//		d_iag_printf( "  pitching for SCALE LEAD. offsets before:\n" );
//		d_iag_printf( "track: scale_pitch_offsets INITIAL:\n" );
//		for (row=0; row<MATRIX_NROF_ROWS; row++) {
//			d_iag_printf( "%d  ", target_page->Track[row]->scale_pitch_offset );
//		}
//		d_iag_printf( "\n" );		
//		for (row=0; row<MATRIX_NROF_ROWS; row++) {
//			d_iag_printf( "%d  ", target_page->Track[row]->lead_pitch_offset );
//		}
//		d_iag_printf( "\n" );		
//		for (row=0; row<MATRIX_NROF_ROWS; row++) {
//			d_iag_printf( "%d  ", target_page->Track[row]->scale_pitch_offset +
//			                     target_page->Track[row]->lead_pitch_offset );
//		}
//		d_iag_printf( "\n" );
//		d_iag_printf( "    nL:%d oL:%d bm:%d deg:%d dir:%d\n", newLeadNdx, oldLeadNdx, bitmask, degrees, direction);
//		d_iag_printf( "\n" );

	// Now apply the pitch shift to the tracks - this has an effect if k>0
	shift_track_pitches( target_page, degrees, direction );


	// d_iag_printf( "lead shift- i:%d j:%d dir:%d k:%d bitmask:\n", i, j, direction, k );
	// my_print_bits32( bitmask );
	// ..so far so good;
	
	
//		d_iag_printf( "*export_scale_LEAD - current offsets (scale/lead/sum):\n" );
//		d_iag_printf( "track: scale_pitch_offsets SHIFTED:\n" );
//		for (row=0; row<MATRIX_NROF_ROWS; row++) {
//			d_iag_printf( "%d  ", target_page->Track[row]->scale_pitch_offset );
//		}
//		d_iag_printf( "\n" );
//		for (row=0; row<MATRIX_NROF_ROWS; row++) {
//			d_iag_printf( "%d  ", target_page->Track[row]->lead_pitch_offset );
//		}
//		d_iag_printf( "\n" );
//		for (row=0; row<MATRIX_NROF_ROWS; row++) {
//			d_iag_printf( "%d  ", target_page->Track[row]->scale_pitch_offset +
//			                     target_page->Track[row]->lead_pitch_offset );
//		}
//		d_iag_printf( "\n" );
//		d_iag_printf( "   exScLd- nL:%d oL:%d bm:%d deg:%d dir:%d\n", 
//				newLeadNdx, oldLeadNdx, bitmask, degrees, direction);
	
	// Since the shift has taken place, no need to remember the old lead anymore
	target_page->scaleLead_old = target_page->scaleLead[0];
	target_page->scaleNotes_old = target_page->scaleNotes[0];
}


// Returns pitch in halfsteps of the given degree in the scaleSelection
unsigned char pitch_of( 	unsigned char degree, 
							unsigned int scaleSelection ){

	unsigned char
					half_steps = 0,
					i = 0;

//	 d_iag_printf( "    --pit of deg:%d in:%d\n", degree, scaleSelection );
//	 my_print_bits32( scaleSelection );

	// Offset needed because of the -- condition. 
	// Mechanism may be revised later.
	degree++;

	// Iterate through the scaleSelection counting half steps and degrees
	for (i=0; i<12; i++){

		// Count the degrees down when one found
		if ( (1 << (11-i)) & (scaleSelection) ) {
	
			// d_iag_printf( "    --degree found at i:%d\n", i ); 
	
			// Degree count down
			degree--;

//			d_iag_printf( "        --degree:%d\n", degree ); 

		}

		// Break when degree counter is used up
		if (degree == 0) {
			break;
		}
		
		// Count the half steps on every iteration
		half_steps ++;		
	}

//	 d_iag_printf( "    --half_steps:%d\n", half_steps );

	return half_steps;
}




// Applies the selected scale degrees to the page track pitches.
// Projects the selected scale pitches onto the track pitches.
// Called after a degree has been added to or removed from the scale. 
// This makes a direct assignment of pitches to tracks, no more lower first etc..
void export_scale_degrees( Pagestruct* target_page ){

	unsigned char 
					scaleNdx 	= 0,	// points to scale step being processed
					j 			= 127,
					lowPiNdx 	= 0,
					carry 		= 0;	// indicates nr of scales passed so far
	
	signed char 	row			= 0;
	unsigned int	track_visited = 0x3ff;	// Stores the bits corresponding to the tracks to be visited
		
	// d_iag_printf( "*export_scale_degrees()\n" );

	// Start processing at the lowest (natural) note in scale, then apply later the scale lead offset.
	scaleNdx = 0;

	// Iterate through the tracks of the page and assign pitch by pith to the tracks
	for ( row = 9; row >= 0; row-- ) {
		
//		 //d_iag_printf( "*export_scale_degrees: scale/Ndx:%d\n", scaleNdx );
//		 //my_print_bits32( target_page->scaleNotes );
		
		// This used to be a more sophisticated assignment logic, but KISS
		lowPiNdx = row;
		
		// When a scale has been completed, the carry gets increased..
		// ..so that pitch shifts carry to the next octave
		carry = 	( 10 - my_bit_cardinality( track_visited ) )
				/	(  0 + my_bit_cardinality( target_page->scaleNotes[0] ) );
				
		// Assign the lowest scale degree to the lowest pitched track.
		target_page->Track[lowPiNdx]->scale_pitch_offset = 
				  pitch_of( scaleNdx, target_page->scaleNotes[0] ) 
				+ (12 * carry) 
				;

		// Addendum to the assignment: if the scale consists of only one note, 
		if ( my_bit_cardinality( target_page->scaleNotes[0] ) == 1 ){
			
			// then assign that pitch to all tracks - do not transpose away on one note..			
			target_page->Track[lowPiNdx]->scale_pitch_offset = 
						pitch_of( scaleNdx, target_page->scaleNotes[0] );
		}
		
		// d_iag_printf( "   --new pitchoffset:%d\n", target_page->Track[lowPiNdx]->scale_pitch_offset);

		// Advance the scale ndx, according to the length of the selected scale
		scaleNdx = (scaleNdx + 1) % (my_bit_cardinality( target_page->scaleNotes[0] ));
		
		// Mark track visited
		track_visited ^= (1 << lowPiNdx);

		// Re-init value of j
		j = 127;

	} // iterator through the rows of the page

}





// Imports the contents of a set into the preselection
void import_GRID_set( unsigned int source_set ){
	
	// Import the contents of the required scene into the grid preselection.
	unsigned char i=0;

	for (i=0; i < GRID_NROF_BANKS; i++){
	
		// Copy GRID Selection contents to the target scene
		GRID_p_clock_presel[i] = GRID_p_set[source_set][i];

		// Affect the choice immediately
		if ( GRID_switch_mode == GRID_SWITCH_DIRECT ){

			GRID_p_preselection[i] = GRID_p_set[source_set][i];
		}
	}	

	if (RUNBIT == OFF){
		
		// Direct selection of the preselections
		select_page_preselections();
	}
}



// Indicates whether a grid set is empty (FALSE) or has content (TRUE)
unsigned char get_content_GRID_set( unsigned char target_set ){

	unsigned char i = 0;
	unsigned char result = FALSE;	
	
	for (i=0; i < GRID_NROF_BANKS; i++ ){

		// Show only when contained pages are not empty
		if ( 	( GRID_p_set[target_set][i] != NULL )
			&& 	( GRID_p_set[target_set][i]->page_clear == OFF )
			){

			result = TRUE;
			break;
		}
	}

	return result;
}


// Checks whether the target set is the same as what is in GRID
unsigned char is_actual_GRID( unsigned char target_set ){

	unsigned char i=0;
	unsigned char result=TRUE;
	
	for ( i=0; i < GRID_NROF_BANKS; i++){

		// Check if they are pointing to the same page
		if ( GRID_p_set[target_set][i] != GRID_p_preselection[i] ){
			
			result = FALSE;
			break;
		}
	}	

	return result;
}


// Exports the current Grid selection into the given set
void export_GRID_set( unsigned int target_set ){	

	unsigned char i=0;
	
	// d_iag_printf( "export_GRID_set():%d\n", target_set );

	for (i=0; i < GRID_NROF_BANKS; i++){
	
		// Copy GRID Selection contents to the target scene
		GRID_p_set[ target_set ][i] = GRID_p_preselection[i];				
		
		// d_iag_printf( "i:%d ", GRID_set[ target_set ][i] );
	}
	// d_iag_printf( "\n" );
}




// Interprets a key press dealing with changing the status of a step
void interpret_matrix_stepkey( 	unsigned char row, 
								unsigned char col,	
								Pagestruct* target_page ) {

	// Consolidate step pointer use
	Stepstruct* target_step = target_page->Step[row][col];
	
	// Absolute keyindex of the pressed button. Used to notice double click.
	unsigned int j = ((col+1) * 11) + (row);
	
	// Used to init step selection
	unsigned char rowctr = 0;
	unsigned char colctr = 0;
	
	// Step status decides what to do
	switch( Step_get_status( target_step, STEPSTAT_TOGGLE ) ){

		// STEP TOGGLE is OFF
		case OFF:
			
			// If STEP SKIPPED, unSKIP
			if ( Step_get_status( target_step, STEPSTAT_SKIP ) == ON ){
				
				// unSKIP step
				Step_set_status( target_step, STEPSTAT_SKIP, OFF );
			}

			// If MUT MASTER on, SKIP the step
			else if ( is_pressed_key( KEY_MUTE_MASTER ) == ON ){

				// SKIP the STEP
				Step_set_status( target_step, STEPSTAT_SKIP, ON );
			}

			// SEL modifier makes no sense if the step is OFF
			// None of the modifier buttons are pressed
			else if ( 	( is_pressed_key( KEY_MUTE_MASTER ) 	== OFF )
					&&	( is_pressed_key( KEY_SELECT_MASTER ) 	== OFF )
					&&	( target_page->editorMode != PREVIEW )
				){

				// Turn the step simply ON
				Step_set_status( target_step, STEPSTAT_TOGGLE, ON );
				
				// DOUBLE CLICK was started in the ON toggle state section
				if (	( DOUBLE_CLICK_TARGET == j )
					&& 	( DOUBLE_CLICK_TIMER > DOUBLE_CLICK_ALARM_SENSITIVITY )
					){
					
					// Un-SELECT all potentially selected steps in page
					if (target_page->stepSelection != 0) {

						for (rowctr=0; rowctr<MATRIX_NROF_ROWS; rowctr++){
							for (colctr=0; colctr<MATRIX_NROF_COLUMNS; colctr++){

								Step_set_status( target_page->Step[rowctr][colctr], STEPSTAT_SELECT, OFF );
							}
						}
						// Reset the counter of selected steps
						target_page->stepSelection = 0;
						target_page->stepAttributeSelection = OFF;
					}
					
					// Select the current step formally 
					Step_set_status( target_step, STEPSTAT_SELECT, ON );
										
					// Update the step selection counter in page
					target_page->stepSelection = 1;

					// Remember STEP COORDINATES
					target_page->stepSelectionSingleRow = row;
					target_page->stepSelectionSingleCol = col;
	
					// d_iag_printf( "zooming into step row:%d col:%d ts:%d\n", 
					//		row, col, target_page->trackSelection );

					// Zoom into the step: Switch the mode
					target_page->keyMode = KMOD_THIS_STEP;
					zoom_level = zoomSTEP;
//					VIEWER_step = target_page->Step	[VIEWER_page->stepSelectionSingleRow]
//													[VIEWER_page->stepSelectionSingleCol];
				}
				
				// Start thinking about double clicking 
				else if ( DOUBLE_CLICK_TARGET == 0 ) {
		
					// Prepare double click timer for zooming into the step
					DOUBLE_CLICK_TARGET = j;
					DOUBLE_CLICK_TIMER = ON;
		
					// Start the Double click Alarm
					cyg_alarm_initialize(
						doubleClickAlarm_hdl, 
						cyg_current_time() + DOUBLE_CLICK_ALARM_TIME,
						DOUBLE_CLICK_ALARM_TIME );
				}
			}
			break;


		// STEP TOGGLE is ON
		case ON:

//			// Do not react like that in REVIEW mode
//			if ( target_page->editorMode == PREVIEW ){
//				break;
//			}

			// SEL MODE
			if 	( is_pressed_key( KEY_SELECT_MASTER ) == ON ){

				// Remove any active track selection, so we can focus on the steps now
				if ( 	( target_page->trackSelection != 0 )
					&&	(	( zoom_level == zoomPAGE ) )
//						||	( zoom_level == zoomMAP ) )
					){

					target_page->trackSelectionStored = target_page->trackSelection;
					target_page->trackSelection = 0;
				}
				
				// Toggle the step SELECT
				if (	( Step_get_status( target_step, STEPSTAT_SELECT ) == OFF )
					&&	(	( zoom_level == zoomPAGE )
						||	( zoom_level == zoomMAP ) )
					){

					Step_set_status( target_step, STEPSTAT_SELECT, ON );

					// Increment the counter of steps selected
					target_page->stepSelection ++;
				}
				else {

					Step_set_status( target_step, STEPSTAT_SELECT, OFF );

					// Increment the counter of steps selected
					target_page->stepSelection --;
				}
			}
			else if ( Step_get_status( target_step, STEPSTAT_SELECT) == ON) {
				
				// Un-SELECT step							
				Step_set_status( target_step, STEPSTAT_SELECT, OFF );								
			
				// Decrement the counter of steps selected
				target_page->stepSelection --;
			}

			// MUT MODE
			else if ( is_pressed_key( KEY_MUTE_MASTER ) == ON ){

				// SKIP Step
				Step_set_status( target_step, STEPSTAT_SKIP, ON  );
				Step_set_status( target_step, STEPSTAT_TOGGLE, OFF );
			}
			else if ( Step_get_status( target_step, STEPSTAT_SKIP) == ON ) {
				
				// Un-SKIP step							
				Step_set_status( target_step, STEPSTAT_SKIP, OFF );								
			}

			// STEP just ON, no SEL or MUT pressed, so turn it OFF
			else if ( target_page->editorMode != PREVIEW ){

				// Turn the step simply OFF
				Step_set_status( target_step, STEPSTAT_TOGGLE, OFF );

				// DOUBLE CLICK was started in the ON toggle state section
				if (	( DOUBLE_CLICK_TARGET == j )
					&& 	( DOUBLE_CLICK_TIMER > DOUBLE_CLICK_ALARM_SENSITIVITY )
					){
					
					// Un-SELECT all potentially selected steps in page
					if (target_page->stepSelection != 0) {

						for (rowctr=0; rowctr<MATRIX_NROF_ROWS; rowctr++){
							for (colctr=0; colctr<MATRIX_NROF_COLUMNS; colctr++){

								Step_set_status( target_page->Step[rowctr][colctr], STEPSTAT_SELECT, OFF );
							}
						}
						// Reset the counter of selected steps
						target_page->stepSelection = 0;
						target_page->stepAttributeSelection = OFF;
					}
					
					// Select the current step formally 
					Step_set_status( target_step, STEPSTAT_SELECT, ON );
										
					// Update the step selection counter in page
					target_page->stepSelection = 1;

					// Remember STEP COORDINATES
					target_page->stepSelectionSingleRow = row;
					target_page->stepSelectionSingleCol = col;
	
					// d_iag_printf( "zooming into step row:%d col:%d ts:%d\n", 
					//		row, col, target_page->trackSelection );

					// Zoom into the step: Switch the mode
					target_page->keyMode = KMOD_THIS_STEP;
					zoom_level = zoomSTEP;
//					VIEWER_step = target_page->Step	[VIEWER_page->stepSelectionSingleRow]
//													[VIEWER_page->stepSelectionSingleCol];
				}				
				
				// Start thinking about double clicking 
				else if ( DOUBLE_CLICK_TARGET == 0 ) {
		
					// Prepare double click timer for zooming into the step
					DOUBLE_CLICK_TARGET = j;
					DOUBLE_CLICK_TIMER = ON;
		
					// Start the Double click Alarm
					cyg_alarm_initialize(
						doubleClickAlarm_hdl, 
						cyg_current_time() + DOUBLE_CLICK_ALARM_TIME,
						DOUBLE_CLICK_ALARM_TIME );
				}
			}

			// When there is only one step selected
			if ( target_page->stepSelection == 1 ){
			
				// Remember STEP COORDINATES
				target_page->stepSelectionSingleRow = row;
				target_page->stepSelectionSingleCol = col;
			}
			
			break;
			
	} // switch on step status
}



// Contains the solo operation for pages
void PAGE_toggle_solo(){
	
	unsigned char i=0, j=0;
	
	//d_iag_printf( "GRID_selection_buffer[0]:%d\n", GRID_selection_buffer[0] );

	// Solo the page under the cursor. Only One page may be solo-ed.
	// d_iag_printf( "GRID_CURSOR:%d is_sel:%d\n", 
	//	GRID_CURSOR, is_selected_in_GRID( &Page_repository[GRID_CURSOR] ));
	
	// Find out whether there is something in the solo buffer
	j = FALSE;
	for ( i=0; i < GRID_NROF_BANKS; i++ ){

		// Check content of current buffer position
		if ( (GRID_p_selection_buffer[i] != NULL) ) {

			j = TRUE;
		}
	}	

	// Page solo condition: grid buffer is empty and page is active				
	if ( ( j == FALSE ) ) {

		// Only active pages can be soloed. Break on inactive page.
		if (is_selected_in_GRID( &Page_repository[GRID_CURSOR] ) == OFF){

			return;
		}

		//d_iag_printf( "BUFFER EMPTY\n" );

		// Adjust the GRID selections: remember selections and preselection, then void
		for (i=0; i < GRID_NROF_BANKS; i++) {

			//d_iag_printf( "Row:%d   buffer:%d   sel:%d\n", 
			//	i, GRID_selection_buffer[i], GRID_selection[i] );

			// Copy the original selection into the buffer
			GRID_p_selection_buffer[i] = GRID_p_selection[i];

			//d_iag_printf( "later:%d   buffer:%d   sel:%d   cursor:%d\n", 
			//	i, GRID_selection_buffer[i], GRID_selection[i], GRID_CURSOR );

			// And mark it empty unless the page is under the cursor, or empty already
			if ( 	( GRID_p_selection[i]->pageNdx 	!= GRID_CURSOR 	)
				&&	( GRID_p_selection[i] 			!= NULL 		) 
				){

				// Propagate the copy top down.
				// Copy the buffer into the clock preselection
				GRID_p_clock_presel[i] 			= NULL;

				if ( GRID_switch_mode == GRID_SWITCH_DIRECT ){

					// Clear grid selection and preselection accordingly
					GRID_p_preselection[i] 		= NULL;
					GRID_p_selection[i] 		= NULL;

					// Stop playing right now!
					stop_playing_page( GRID_p_selection[i], TTC_ABS_VALUE );
					set_track_locators( GRID_p_selection[i], 0, 0 );
				}
			}
		}
	}

	// Page un-solo condition: GRID_selection_buffer[0] == GRID_SELECTION_BUFFER_EMPTY
	else {

		//d_iag_printf( "NEQ\n" );

		// Grid buffer is full, so transfer and empty
		for (i=0; i < GRID_NROF_BANKS; i++) {

			//d_iag_printf( "transfer+empty:%d\n", i );

			// Propagate the copy top down.
			// Copy the buffer into the clock selection
			GRID_p_clock_presel[i] 		= GRID_p_selection_buffer[i];

			if ( GRID_switch_mode == GRID_SWITCH_DIRECT ){

				GRID_p_preselection[i] 		= GRID_p_selection_buffer[i];
				GRID_p_selection[i] 		= GRID_p_selection_buffer[i];
			}
			
			// And clear the buffer as we go along
			GRID_p_selection_buffer[i] = NULL;
		}
	}
}


// Clear the target attribute of all tracks in page
void clear_tracks_by_ATR( Pagestruct* target_page, unsigned char target_attribute ){

	unsigned char row = 0;
	
	for ( row=0; row < MATRIX_NROF_ROWS; row++ ){
	
		switch( target_attribute ){
		
			case ATTR_VELOCITY:
				target_page->Track[row]->attribute[ATTR_VELOCITY] 	= TRACK_DEF_VELOCITY;
				break;

			case ATTR_PITCH:
				target_page->Track[row]->attribute[ATTR_PITCH] 		= TRACK_DEFAULT_PITCH[row];
				break;

			case ATTR_LENGTH:
				target_page->Track[row]->LEN_factor					= TRACK_DEF_LENFACTOR;
				break;

			case ATTR_START:
				target_page->Track[row]->STA_factor			 		= TRACK_DEF_STAFACTOR;
				break;
				
			case ATTR_POSITION:
				target_page->Track[row]->attribute[ATTR_POSITION] 	= TRACK_DEF_POSITION;
				break;

			case ATTR_DIRECTION:
				target_page->Track[row]->attribute[ATTR_DIRECTION] 	= TRACK_DEF_DIRECTION;
				break;

			case ATTR_AMOUNT:
				target_page->Track[row]->attribute[ATTR_AMOUNT] 	= TRACK_DEF_AMOUNT;
				break;

			case ATTR_GROOVE:
				target_page->Track[row]->attribute[ATTR_GROOVE] 	= TRACK_DEF_GROOVE;
				break;

			case ATTR_MIDICC:
				target_page->Track[row]->attribute[ATTR_MIDICC] 	= TRACK_DEF_MIDICC;
				break;

			case ATTR_MIDICH:
				target_page->Track[row]->attribute[ATTR_MIDICH] 	= TRACK_DEF_MIDICH;
				break;
		}
	}
}



// Randomize the target attribute of all tracks in page 
void rand_tracks_by_ATR( Pagestruct* target_page, unsigned char target_attribute ){

	unsigned char row = 0;
	unsigned char min_val = 0;
	unsigned char max_val = 0;
	
	switch( target_attribute ){
	
		case ATTR_VELOCITY:
			min_val = TRACK_MIN_VELOCITY;
			max_val = TRACK_MAX_VELOCITY;
			break;

		case ATTR_PITCH:
			min_val = TRACK_MIN_PITCH;
			max_val = TRACK_MAX_PITCH;
			break;

		case ATTR_LENGTH:
			min_val = TRACK_MIN_LENFACTOR;
			max_val = TRACK_MAX_LENFACTOR;
			break;

		case ATTR_START:
			min_val = TRACK_MIN_STAFACTOR;
			max_val = TRACK_MAX_STAFACTOR;
			break;

		case ATTR_POSITION:
			min_val = TRACK_MIN_POSITION;
			max_val = TRACK_MAX_POSITION;
			break;

		case ATTR_DIRECTION:
			min_val = TRACK_MIN_DIRECTION;
			max_val = TRACK_MAX_DIRECTION;
			break;

		case ATTR_AMOUNT:
			min_val = TRACK_MIN_AMOUNT;
			max_val = TRACK_MAX_AMOUNT;
			break;

		case ATTR_GROOVE:
			min_val = TRACK_MIN_GROOVE;
			max_val = TRACK_MAX_GROOVE;
			break;

		case ATTR_MIDICC:
			min_val = TRACK_MIN_MIDICC;
			max_val = TRACK_MAX_MIDICC;
			break;

		case ATTR_MIDICH:
			min_val = TRACK_MIN_MIDICH;
			max_val = TRACK_MAX_MIDICH;
			break;
	}

	// Randomize by amount the track values for the target attribute
	for ( row=0; row < MATRIX_NROF_ROWS; row++ ){

		switch( target_attribute ){

			case ATTR_START:
				target_page->Track[row]->STA_factor
						= 	(unsigned char)
							randomize_byamount(	VIEWER_page->Track[row]->STA_factor,
												VIEWER_page->Track[row]->attribute[ATTR_AMOUNT],
												min_val,
												max_val );
				break;		

			case ATTR_LENGTH:
				target_page->Track[row]->LEN_factor
						= 	(unsigned char) 
							randomize_byamount(	VIEWER_page->Track[row]->LEN_factor,
												VIEWER_page->Track[row]->attribute[ATTR_AMOUNT],
												min_val,
												max_val );
				break;		

			default:

				target_page->Track[row]->attribute[target_attribute] 
						= 	(unsigned char)
							randomize_byamount(	VIEWER_page->Track[row]->attribute[target_attribute],
												VIEWER_page->Track[row]->attribute[ATTR_AMOUNT],
												min_val,
												max_val );
				break;
		} // switch( target_attribute )

	} // Row iterator

}


// Sorts the tracks of the given page in descending order (top to bottom) by target attribute
void sort_tracks_by_ATR( Pagestruct* target_page, unsigned char target_attribute ){

	unsigned char row = 0;
	unsigned char pass = 0;
	unsigned char top_ndx = 0;
	signed char top_val = -10;	// -10 accomodates the -1 from the MIDICC attribute
	unsigned short checklist = 0; // remember which tracks were visited
	
	// Basically copy the highest-valued track into the PAGE_assistant_page and back
	// d_iag_printf(" sorting ATR:%d\n", target_attribute );

	// Need to care about LEN and STA
	switch( target_attribute ){

		case ATTR_LENGTH:
			// Each pass corresponds to one track in target page
			for ( pass=0; pass < MATRIX_NROF_ROWS; pass++ ){
			
				// Find the index of the highest valued track
				for ( row=0; row < MATRIX_NROF_ROWS; row++ ){
				
					// Continue if the track was processed already
					if ( (checklist & (1<<row)) != 0 ){
						continue;
					}
			
					// Found a value higher than highest seen so far
					if ( ((signed char) target_page->Track[row]->LEN_factor) > top_val ){
			
						// Remember the value
						top_val = (signed char) target_page->Track[row]->LEN_factor;
			
						// Remember the index
						top_ndx = row;
					}
				} // Row iterator - making up one pass
				// d_iag_printf("top_val:%d at top_ndx:%d\n", top_val, top_ndx);

				// Now we know the index of the highest value, so copy the track
				Track_copy( target_page, top_ndx, PAGE_assistant_page, pass );
				
				// Mark the track processed - in the checklist
				checklist |= ( 1 << top_ndx );
				
				// Reset the top_val
				top_val = 0;
			} // pass iterator
			break;


		case ATTR_START:		
			top_val = 0;
			// Each pass corresponds to one track in target page
			for ( pass=0; pass < MATRIX_NROF_ROWS; pass++ ){
			
				// Find the index of the highest valued track
				for ( row=0; row < MATRIX_NROF_ROWS; row++ ){
				
					// Continue if the track was processed already
					if ( (checklist & (1<<row)) != 0 ){
						continue;
					}
			
					// Found a value higher than highest seen so far
					if ( target_page->Track[row]->STA_factor > top_val ){
			
						// Remember the value
						top_val = target_page->Track[row]->STA_factor;
			
						// Remember the index
						top_ndx = row;
					}
				} // Row iterator - making up one pass
			
				// Now we know the index of the highest value, so copy the track
				Track_copy( target_page, top_ndx, PAGE_assistant_page, pass );
				
				// Mark the track processed - in the checklist
				checklist |= ( 1 << top_ndx );
				
				// Reset the top_val
				top_val = 0;
			} // pass iterator
			break;
			

		default:
			// Each pass corresponds to one track in target page
			for ( pass=0; pass < MATRIX_NROF_ROWS; pass++ ){
			
				// Find the index of the highest valued track
				for ( row=0; row < MATRIX_NROF_ROWS; row++ ){
				
					// Continue if the track was processed already
					if ( (checklist & (1<<row)) != 0 ){
						continue;
					}
			
					// Found a value higher than highest seen so far
					if ( ((signed char) target_page->Track[row]->attribute[target_attribute]) > top_val ){
			
						// Remember the value
						top_val = (signed char) target_page->Track[row]->attribute[target_attribute];
			
						// Remember the index
						top_ndx = row;
					}
				} // Row iterator - making up one pass

				// Now we know the index of the highest value, so copy the track
				Track_copy( target_page, top_ndx, PAGE_assistant_page, pass );
				
				// Mark the track processed - in the checklist
				checklist |= ( 1 << top_ndx );
				
				// Reset the top_val
				top_val = -10;				
			} // pass iterator

			break;
	}

	// Tracks sorted, now copy them back into the current page
	for ( row=0; row < MATRIX_NROF_ROWS; row++ ){

		// From PAGE_assistant_page to target_page
		Track_copy( PAGE_assistant_page, row, target_page, row );		
	}

}







