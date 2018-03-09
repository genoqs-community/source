         /* OCTOPUS
 * Implementation of the player functions
 * 07.11.2003 genoQs Machines
 * 
 * The first beat is slightly shorter because of the time needed
 * to operate the timer interrupt in player_start().
 */

extern void stop_playing_page( 						Pagestruct* target_page, 
													unsigned char in_TTC_ABS_VALUE );
	
extern void set_track_locators( 					Pagestruct* target_page,
													unsigned char target_LOC,
													unsigned char target_TTC	 );
	
extern unsigned char offset_to_next_degree(			Pagestruct* target_page, 
													unsigned char pitch );

extern unsigned char Step_active_playcoordinate(	Pagestruct* target_page, 
													unsigned char row, 
													unsigned char col, 
													unsigned char which );

extern unsigned char get_next_tracklocator( 		Trackstruct* target_track, 
													unsigned char curr_locator );

extern void 		PLAYER_play_page(				Pagestruct* target_page,
													unsigned char in_TTC_ABS_VALUE );

//extern unsigned char jump_skipped_steps( 			Pagestruct* target_page,
//													unsigned char row, 
//													unsigned char oldLocator,
//													unsigned char offset,
//													unsigned char start,
//													unsigned char length,
//													unsigned char direction	);
													
extern unsigned char jump_skipped_steps( 			Pagestruct* target_page,
													unsigned char row, 
													unsigned char direction,
													unsigned char offset	);													

extern void set_page_locators( 						Pagestruct* target_page, 
													unsigned char target_LOC, 
													unsigned char target_TTC );


// General Player functions
#include "play_functions.h"

// MIDI related functions, includes the MIDI out thread
#include "play_MIDI.h"

// The main play function
#include "play_play.h"

// Functions for locator update
#include "play_updlocators.h"



// Advances the global locator and execute associated functions
void advance_global_locator(){

	// Advance global locator. This is hardwired to 16 (columns)
	GLOBAL_LOCATOR = (GLOBAL_LOCATOR % MATRIX_NROF_COLUMNS) +1;
}


// Used to advance the locator in the target_page and in its tracks as well
// according to TTC_ABS_VALUE and other conditions.See logic on page 190 book.
// This used to be TEMPO_TRACKER driven.
void advance_page_locators( Pagestruct* target_page ){
	
	switch( TTC_ABS_VALUE ) {

		// Affected pages: multipliers: 1  2  4 (all pages)
		case 1:
			// Advance page locator: modulo operation acc. to page length
			target_page->locator = 
				( target_page->locator % target_page->attribute[ATTR_LENGTH] ) + 1;	
			
			// Execute cadence code
			// execute_cadence( target_page );
	
			// d_iag_printf( "advanced locator: page[%d]->locator:%d\n", 
			//	target_page->pageNdx, target_page->locator );

			break;

			
		// Nothing to do otherwise.
		default:
			break;

	} // switch (TEMPO_TRACKER
}




// Play the MIDI events queued up for the given timestamp
void play_MIDI_queue( unsigned int in_MIDI_timestamp ){

	unsigned int i = 0;
	MIDIeventstruct* queue_event = NULL;
	
	// MIDI OFF - Play currently relevant note OFF data from the MIDI queue
	for( i=0; i < MIDI_queue_counter; i++ ){

		// Get the next event at current timestamp
		queue_event = MIDI_queue_remove( in_MIDI_timestamp );

		// d_iag_printf( "MIDI off event at ts:%d\n", MIDI_timestamp );
		// MIDIevent_print( off_event );

		// No off events left, so move on.
		if ( queue_event == NULL ){ break; 	}

		// Send the MIDI OFF message to the MIDI port
		MIDI_send_event( queue_event );
	} // queue iterator	
}




//
// PLAYER entry point
//
void PLAYER_dispatch( unsigned char in_TTC_ABS_VALUE ) {
	
	// Counter variable
	unsigned int i=0;

	// Play MIDI queue elements which are due at current timestamp
	play_MIDI_queue( MIDI_timestamp );
	
	// d_iag_printf( "player dispatched\n" );

	// Advance page locators ..on every FOURTH TICK  ( 12/4 == 3 ):
	if ( (in_TTC_ABS_VALUE % 3) == 1 ){
		
		// Check if selected in GRID! ..as opposed to advancing ALL pages 
		for( i=0; i<GRID_NROF_BANKS; i++ ) {

			// Skip inactive banks, btw- the empty flag is GRID_SELECTION_EMPTY
			if ( GRID_p_selection[i] != NULL ){	
				
				// d_iag_printf( "pageNdx:%d in_TTC:%d\n", 
				//	GRID_selection[i], in_TTC_ABS_VALUE );
			
				// Advance page locator of the page, having its length..
				advance_page_locators( GRID_p_selection[i] );
	
				// d_iag_printf( "___length:%d loc:%d\n", 
				//	(&Page_repository[ GRID_selection[i] ])->attribute[ATTR_LENGTH], 
				//	(&Page_repository[ GRID_selection[i] ])->locator );

			} // GRID selection is not NULL			
		} // GRID bank iterator
	} // (TTC_ABS_VALUE) % 3 == 1

	// d_iag_printf( "player STEP1\n" );

	// ..on the BEAT TICK
	if ( in_TTC_ABS_VALUE == 1 ){

		// Advance the global locator - normal speed
		advance_global_locator();

		// PAGE PRESELECTION
		// Preselect pages as appropriate in every bank ..supports variable page lengths.
		for( i=0; i < GRID_NROF_BANKS; i++ ){

			// First time we are executing this after a start..
			if ( SEQUENCER_JUST_STARTED == TRUE ) {

				// Do not switch anything just upon sequencer start
				break;
			}

			// Locator of ONE in the playing page, or otherwise GLOBAL_LOCATOR == 1 ..
			// ..prepare the page switch accordingly
			if (	
					// The grid selection is NOT empty and.. selected page at locator 1
					(	( 	GRID_p_selection[i] != NULL )
					&&	( 	GRID_p_selection[i]->locator == 1 ) )
				||	
					// The grid selection is empty and.. global locator at oclock
					( 	( 	GRID_p_selection[i] == NULL ) 
					&& 	(	GLOBAL_LOCATOR == 1 ) )
				){
					
				// d_iag_printf( "target_bank:%d\n", GRID_p_selection[i]->pageNdx % 10 );
				// d_iag_printf( "--switching..\n" );
					
				// Clock_presel added the natural neighbours.
				compute_chain_presel( i );
		
				// Clock presel input (from key presses) moved into preselection. Page 239 book.
				preselect_clock_presel( i );

				// d_iag_printf( "_______loc:%d len:%d GRID_presel[0]:%d sel[0]:%d\n", 
				//	GRID_p_selection[i]->locator,
				//	GRID_p_selection[i]->attribute[ATTR_LENGTH],
				//	GRID_p_preselection[0]->pageNdx, GRID_p_selection[0]->pageNdx );
			}

		} // GRID_BANK_ITERATOR

		// Select the preselections on every beat!
		select_page_preselections();

	// d_iag_printf( "___pageNdx:%d length:%d loc:%d\n",
 	// 	(&Page_repository[ GRID_selection[0] ])->pageNdx,
	//	(&Page_repository[ GRID_selection[0] ])->attribute[ATTR_LENGTH], 
	//	(&Page_repository[ GRID_selection[0] ])->locator );

	} // in_TTC_ABS_VALUE  == 1 ..on the BEAT TICK
	
	// d_iag_printf( "player STEP2\n" );


	SEQUENCER_JUST_STARTED = FALSE;


	// PAGE PLAY from grid selection: each bank has potentially one active page
	for (i=0; i<GRID_NROF_BANKS; i++) { 		// PRODUCTIVE CODE
//	for (i=0; i<1; i++){					// 1st bank only - Test mode -do not use for production
		
		// d_iag_printf( "bank:%d: %x ", i, (unsigned int) GRID_p_selection[i] );
		
		// Skip the banks that are not currently active
		if ( GRID_p_selection[i] != NULL ){	
			
			// Play the page selected in current GRID bank
			PLAYER_play_page_new( GRID_p_selection[i], in_TTC_ABS_VALUE );

			// d_iag_printf( "played:%d\n", i );
		}
		else{
			//d_iag_printf( "skipped:%d\n", i );
		}
	} // GRID bank iterator

	// d_iag_printf( "player STEP3\n" );

} // end --- void PLAYER_dispatch()


