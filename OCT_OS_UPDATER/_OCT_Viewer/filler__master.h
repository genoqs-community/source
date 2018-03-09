
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
	unsigned char color = 0;
	
	// Work on the page under the grid cursor
	Pagestruct* target_page = &Page_repository[ GRID_CURSOR ];

	// ZOOM LEVEL
	show( ELE_ZOOMLEVEL, zoom_level );

	switch (zoom_level) {
		
		case zoomAMANDA:
			// Don't show anything, and don't react.
			MIR_write_dot( LED_TEMPO, MIR_GREEN );
			MIR_write_dot( LED_TEMPO, MIR_RED   );
			
			if ( CLOCK_SOURCE != EXT ){
				MIR_write_dot( LED_TEMPO, MIR_BLINK );
			}
			break;

		case zoomDEVICE:
			// not defined yet
			#include "filler_DEVICE.h"
			break;
			
		case zoomABLETON:
			#include "filler_ABLETON.h"
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

		case zoomMIXMAP:
			#include "filler_MIXMAP.h"
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

	} // switch ( target_page->zoomlevel );

} // void VIEWER_fill_MIR()


