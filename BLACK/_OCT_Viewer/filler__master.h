
//
// MASTER FILLER FUNCTION - prepares the MIR for display on the panel
//
// genoQs Machines 01.10.2005


// Transforms a grid cursor postion into a MIR index
unsigned char cursor_to_dot( unsigned char in_cursor ){

	return ( in_cursor + 11 + in_cursor/10 );
}


// This fills the MIR from a page perspective (Push)
void VIEWER_fill_MIR() {

	unsigned int i = 0;
	unsigned int j = 0;
	unsigned int row = 0;
	unsigned int col = 0;
	unsigned int temp = 0;
	
	// ZOOM LEVEL
	show( ELE_ZOOMLEVEL, zoom_level );

	switch (zoom_level) {
		
		case zoomDEVICE:
			// not defined yet
			break;
			
		case zoomSTRUM:
			#include "filler_STRUM.h"
			break;

		case zoomGRID:
			#include "filler_GRID.h"
			break;

		case zoomGRIDTRACK:
			#include "filler_GRIDTRACK.h"
			break;

		case zoomPAGE:
			#include "filler_PAGE.h"		
			break;

		case zoomTRACK:
			#include "filler_TRACK.h"
			break;
			
		case zoomMAP:
			#include "filler_MAP.h"
			break;	
		
		case zoomSTEP:
			#include "filler_STEP.h"
			break;

		case zoomPLAY:
			#include "filler_PLAY.h"
			break;

		case zoomDIAG:
			#include "filler_DIAG.h"
			break;

	} // case (VIEWER_page->zoomlevel);

} // void VIEWER_fill_MIR()


