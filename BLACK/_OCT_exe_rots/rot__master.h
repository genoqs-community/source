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
	
	// Get rotary data: encodes both rotary address and direction
	unsigned char rotNdx 	= in_rotNdx >> 2;
	unsigned char direction = in_rotNdx & 0x03;
	
	// Encoder operation depending on zoom level
	switch( zoom_level ){
	
		case zoomDIAG:
			rot_exe_DIAG( 	VIEWER_page, rotNdx, direction );
			break;

		case zoomGRID:
			rot_exec_GRID( VIEWER_page, rotNdx, direction );			
			break;

		case zoomGRIDTRACK:
			rot_exec_GRIDTRACK( VIEWER_page, rotNdx, direction );
			break;	

		case zoomPAGE:
			rot_exec_PAGE( VIEWER_page, rotNdx, direction );
			break;

		case zoomTRACK:
			rot_exec_TRACK( VIEWER_page, rotNdx, direction );
			break;

		case zoomMAP:
			rot_exec_MAP( VIEWER_page, rotNdx, direction );
			break;

		case zoomSTEP:
			rot_exec_STEP( VIEWER_page, rotNdx, direction );
			break;

		case zoomSTRUM:
			rot_exe_STRUM( VIEWER_page, rotNdx, direction );
			break;
	}
		
	// Update HUI with changes performed by rotarys
	Page_requestRefresh();
}

