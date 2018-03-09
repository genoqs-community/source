/* OCTOPUS
 * 
 * Executing the Interrupt received via EXT1
 * genoQs Machines, 07.11.2003
 */

// General functions pertaining to rotary execution
#include "rot_functions.h"
#include "rot_editorblock.h"
#include "rot_DIAG.h"	
#include "rot_GRID.h"
#include "rot_GRIDTRACK.h"
#include "rot_PAGE.h"
#include "rot_TRACK.h"
#include "rot_MAP.h"
#include "rot_STEP.h"
#include "rot_STRUM.h"

// EXECUTE ROTARY impulse data
void executeRot( unsigned int in_rotNdx ){
	
	// Work on the page under the grid cursor
	Pagestruct* target_page = &Page_repository[GRID_CURSOR];

	// Get rotary data: encodes both rotary address and direction
	unsigned char rotNdx 	= in_rotNdx >> 2;
	unsigned char direction = in_rotNdx & 0x03;
	
	// Encoder operation depending on zoom level
	switch( zoom_level ){
	
		case zoomDIAG:
			rot_exe_DIAG( 	target_page, rotNdx, direction );
			break;

		case zoomAMANDA:
			// Allow tempo modification
			if ( rotNdx == 0 ){
				
				// When not running on external clock
				if ( CLOCK_SOURCE != EXT ){
	
					modify_parameter( &TEMPO, MIN_TEMPO, MAX_TEMPO, direction, ON, FIXED);
					TIMER_REFILL_update();
				}
			}
			break;

		case zoomGRID:
			rot_exec_GRID( target_page, rotNdx, direction );			
			break;

		case zoomGRIDTRACK:
			rot_exec_GRIDTRACK( target_page, rotNdx, direction );
			break;	

		case zoomPAGE:
			rot_exec_PAGE( target_page, rotNdx, direction );
			break;

		case zoomMIXMAP:
			rot_exec_MIXMAP( target_page, rotNdx, direction );
			break;

		case zoomTRACK:
			rot_exec_TRACK( target_page, rotNdx, direction );
			break;

		case zoomMAP:
			rot_exec_MAP( target_page, rotNdx, direction );
			break;

		case zoomSTEP:
			rot_exec_STEP( target_page, rotNdx, direction );
			break;

		case zoomSTRUM:
			rot_exe_STRUM( target_page, rotNdx, direction );
			break;
	}
		
	// Update HUI with changes performed by rotarys
	Page_requestRefresh();
}

