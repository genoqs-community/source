
extern unsigned char row_of_track( 				Pagestruct* target_page, 
												Trackstruct* target_track );


// 
// SHOW - Contains the workhorse code for MIR refill. Puts the content into the target display element (ELE)
//
void show (unsigned int target, unsigned int content) {
	
	unsigned int i=0, j=0;
	signed char my_pitch = 0;
	unsigned int row=0;
	unsigned int col=0;
	unsigned int attribute=0;
	unsigned int bitpattern=0;
	unsigned char temp = 0;

	Pagestruct* temp_page = NULL;
	Trackstruct* current_track = NULL;

	Pagestruct* target_page = &Page_repository[GRID_CURSOR];

	switch (target) {

		// 
		// ZOOM LEVEL (a.k.a. Inner Circle)
		//
		case ELE_ZOOMLEVEL:		
			#include "show_ZOOM_LEVEL.h"
			break;	


		//
		// MIX MASTER
		//
		case ELE_MIX_MASTER:
			switch (content) {
				case GREEN:
					MIR_write_dot (LED_MIX_MASTER, MIR_GREEN);
					break;
				case RED:
					MIR_write_dot (LED_MIX_MASTER, MIR_RED);
					break;
				case BLINK:
					MIR_write_dot (LED_MIX_MASTER, MIR_BLINK);
				case OFF:
					break;				
			}
			break;


		//
		// MIX INDICATOR (LED for the MIX rotarys)
		//
		case ELE_MIX_INDICATOR:
			switch (content) {
				case GREEN:
					MIR_write_dot (LED_MIX_INDICATOR, MIR_GREEN);
					break;
				case RED:
					MIR_write_dot (LED_MIX_INDICATOR, MIR_RED);
					break;
				case BLINK:
					MIR_write_dot (LED_MIX_INDICATOR, MIR_BLINK);
				case OFF:
					break;				
			}
			break;

		//
		// MIX TARGET INDICATOR
		//
		case ELE_MIXTGT:
		
			// Depending on the zoom mode:
			switch( zoom_level ){

				case zoomPAGE:
				case zoomMIXMAP:
				
					i = target_page->mixTarget;
					
					if ( content == OPTIONS ) {
		
						// Show what's available
						MIR_write_dot( LED_MIXTGT_ATR, 	MIR_GREEN );
						MIR_write_dot( LED_MIXTGT_VOL, 	MIR_GREEN );
						MIR_write_dot( LED_MIXTGT_PAN, 	MIR_GREEN );
						MIR_write_dot( LED_MIXTGT_MOD,	MIR_GREEN );
						MIR_write_dot( LED_MIXTGT_EXP, 	MIR_GREEN );
						// MIR_write_dot( LED_MIXTGT_USR0, MIR_GREEN );
						MIR_write_dot( LED_MIXTGT_USR1, MIR_GREEN );
						MIR_write_dot( LED_MIXTGT_USR2, MIR_GREEN );
						MIR_write_dot( LED_MIXTGT_USR3, MIR_GREEN );
						MIR_write_dot( LED_MIXTGT_USR4, MIR_GREEN );
						MIR_write_dot( LED_MIXTGT_USR5, MIR_GREEN );
					}
					break;

				case zoomGRIDTRACK:
				case zoomGRID:
					i = GRID_assistant_page->mixTarget;

					if ( content == OPTIONS ) {
		
						// Show what's available
						MIR_write_dot( LED_MIXTGT_USR1, MIR_GREEN );
						MIR_write_dot( LED_MIXTGT_USR2, MIR_GREEN );
						MIR_write_dot( LED_MIXTGT_USR3, MIR_GREEN );
						MIR_write_dot( LED_MIXTGT_USR4, MIR_GREEN );
						MIR_write_dot( LED_MIXTGT_USR5, MIR_GREEN );
					}
					break;
			}


			switch( i ) {
				
				case MIXTGT_ATR:
					j = LED_MIXTGT_ATR;
					break;				
				case MIXTGT_VOL:
					j = LED_MIXTGT_VOL;
					break;					
				case MIXTGT_PAN:
					j = LED_MIXTGT_PAN;
					break;
				case MIXTGT_MOD:
					j = LED_MIXTGT_MOD;
					break;				
				case MIXTGT_EXP:
					j = LED_MIXTGT_EXP;
					break;					
//				case MIXTGT_USR0:
//					j = LED_MIXTGT_USR0;
//					break;
				case MIXTGT_USR1:
					j = LED_MIXTGT_USR1;
					break;
				case MIXTGT_USR2:
					j = LED_MIXTGT_USR2;
					break;
				case MIXTGT_USR3:
					j = LED_MIXTGT_USR3;
					break;
				case MIXTGT_USR4:
					j = LED_MIXTGT_USR4;
					break;
				case MIXTGT_USR5:
					j = LED_MIXTGT_USR5;
					break;
			}
			
			
			// Show selection
			MIR_write_dot (j, MIR_RED);
			MIR_write_dot (j, MIR_GREEN);

			// Blink to show current selection
			if ( 	( zoom_level == zoomMIXMAP ) 
				||	( 	( ( zoom_level == zoomGRID ) || ( zoom_level == zoomGRIDTRACK ) )
					&&	( content == OPTIONS ) )
				){

				MIR_write_dot (j, MIR_BLINK);
			}

			// MAP0 LED
			if ( 	( i == MIXTGT_USR1 )
				||	( i == MIXTGT_USR2 )
				||	( i == MIXTGT_USR3 )
				||	( i == MIXTGT_USR4 )
				||	( i == MIXTGT_USR5 )
				){

				if (	( zoom_level == zoomPAGE )
					&&	( target_page->keyMode == KMOD_PAGE )
					){

					if ( GRID_MAP0_status == ON ){
						// Show grid map operation
						MIR_write_dot( LED_MIXTGT_USR0, MIR_GREEN );
					}
					else{
						// Show local operation
						MIR_write_dot( LED_MIXTGT_USR0, MIR_RED );
					}
				}
			}
			break;


		//
		// EDIT INDICATOR (LED for the EDIT rotarys)
		//
		case ELE_EDIT_INDICATOR:
			switch (content) {
				case GREEN:
					MIR_write_dot (LED_EDIT_INDICATOR, MIR_GREEN);
					break;
				case RED:
					MIR_write_dot (LED_EDIT_INDICATOR, MIR_RED);
					break;
				case BLINK:
					MIR_write_dot (LED_EDIT_INDICATOR, MIR_BLINK);
				case OFF:
					break;				
			}
			break;


		//
		// TRACK SELECTORS
		//
		case ELE_TRACK_SELECTORS:
			#include "show_TRACK_SELECTORS.h"
			break;
			
		// 	
		// SELECT MASTER
		//
		case ELE_SELECT_MASTER:
			switch (content) {
				case GREEN:
					MIR_write_dot (LED_SELECT_MASTER, MIR_GREEN);
					break;
				case RED:
					MIR_write_dot (LED_SELECT_MASTER, MIR_RED);
					break;
				case BLINK:
					MIR_write_dot (LED_SELECT_MASTER, MIR_BLINK);
					break;
			}
			break;

		//
		// TEMPO MULTIPLIER
		//
		case ELE_TEMPO_MULTIPLIER:
			// All are green.. and the selected one is red.
//			MIR_write_dot( LED_TEMPO_MULTIPLIER_1, MIR_GREEN );
//			MIR_write_dot( LED_TEMPO_MULTIPLIER_2, MIR_GREEN );
//			MIR_write_dot( LED_TEMPO_MULTIPLIER_4, MIR_GREEN );
//
//			switch (target_page->tempoMultiplier) {
//				case 1:
//					j = LED_TEMPO_MULTIPLIER_1;
//					break;				
//				case 2:
//					j = LED_TEMPO_MULTIPLIER_2;
//					break;					
//				case 4:
//					j = LED_TEMPO_MULTIPLIER_4;
//					break;
//			}
//
//			MIR_write_dot (j, MIR_RED);

			break;

		//
		// CHAIN INDICATOR
		//
		case ELE_CHAIN_INDICATOR:

			switch( content ){
	
				case ON:
					switch (target_page->chainMode) {
						case 0:
							j = LED_CHAINMODE_1;
							break;				
						case 1:
							j = LED_CHAINMODE_2;
							break;					
						case 2:
							j = LED_CHAINMODE_3;
							break;
						case 3:
							j = LED_CHAINMODE_4;
							break;
						case 4:
							j = LED_CHAINER;
							break;
					}
					break;


				case CADENCE_STATUS:
						
					// For now it is just linear. Later it may change
					j = LED_CHAINMODE_4; 
					break;
			}
			
			// Light the actual LED
			MIR_write_dot (j, MIR_RED);

			if ( CHAINS_PLAY_HEAD == ON ){
				MIR_write_dot (j, MIR_GREEN);
			}

			break;
		


		// M A T R I X
		//
		case ELE_MATRIX:
			#include "show_MATRIX.h"
			break;


		// TRACK MUTATORS
		//
		case ELE_TRACK_MUTATORS:
			#include "show_TRACK_MUTATORS.h"
			break; // ELE_TRACK_MUTATORS


		case ELE_MUTE_MASTER:
			switch (content) {
				case GREEN:
					MIR_write_dot (LED_MUTE_MASTER, MIR_GREEN);
					break;
				case RED:
					MIR_write_dot (LED_MUTE_MASTER, MIR_RED);
					break;
				case BLINK:
					MIR_write_dot (LED_MUTE_MASTER, MIR_BLINK);
					break;
			}
			break;
			

		case ELE_EDIT_MASTER:
			switch (content) {
				case OFF:
					break;				
				case RED:
					MIR_write_dot (LED_EDIT_MASTER, MIR_RED);
					break;
				case GREEN:
					MIR_write_dot (LED_EDIT_MASTER, MIR_GREEN);
					break;
				case BLINK:
					MIR_write_dot (LED_EDIT_MASTER, MIR_BLINK);
					break;				
			}
			break;
			

		case ELE_RETURN:
			switch (content) {
				case OFF:
					break;
				case GREEN:
					MIR_write_dot (LED_RETURN, MIR_GREEN);
					break;
				case RED:
					MIR_write_dot (LED_RETURN, MIR_RED);
					break;
				case BLINK:
					MIR_write_dot (LED_RETURN, MIR_BLINK);
					break;
			}
			break;
			
			
		case ELE_GLOBALS:
			#include "show_ELE_GLOBALS.h"
			break;	


		case ELE_OCTAVE_CIRCLE:
			#include "show_OCTAVE_CIRCLE.h"		
			break;
			

		// Show the type of scale selected 
		case ELE_SCALE_SELECTOR:

			switch( content ){		
					
				case SELECTED_CADENCE_KEY:

					// Show all scale selectors green, as they are available
					for (i=244; i<=249; i++){
	
						MIR_write_dot( i, MIR_GREEN );
					}
					break;
				
			}
			break; // ELE_SCALE_SELECTOR
						



		// Show the type of chord currently selected
		case ELE_CHORD_SELECTOR:

			switch( content ){


				case CHORD_SIZE:
					// This is a bit deep but efficient. See LED index map. 
					// MIR_write_dot( 258 - target_page->chordSize + 1, MIR_RED   );
					// MIR_write_dot( 258 - target_page->chordSize + 1, MIR_GREEN );

					break;


				case CHORD_SIZE_TRACK:
					// Get the value of the chord bit for the selected track
					i = Track_get_MISC( target_page->Track[ my_bit2ndx( target_page->trackSelection ) ],
						CHORD_BIT );
					MIR_write_dot( 258 - i + 1, MIR_RED   );
					MIR_write_dot( 258 - i + 1, MIR_GREEN );

					break;


				case SELECTED_CADENCE_KEY:
					// Show all chord selectors green as they are all available
					for (i = 252; i<=258; i++){
						MIR_write_dot( i, MIR_GREEN );
					}
					
					// Shows the key of the selected cadence step.
//					MIR_write_dot( target_page->cadence->key[ target_page->cadence->locator ], MIR_GREEN );
//					MIR_write_dot( target_page->cadence->key[ target_page->cadence->locator ], MIR_RED );
					break;
			}
			break;

	} // switch (target)
}


