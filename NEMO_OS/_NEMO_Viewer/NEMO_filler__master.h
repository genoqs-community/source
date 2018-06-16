//
// Copyright (C) 2004, 2005, 2006, 2007 genoQs Machines
// This file is part of Octopus_OS - 
// the firmware of the genoQs Machines Octopus, MIDI Control Sequencer
//
// The source code Octopus_OS is available for use in accordance with the 
// GNU General Public License (GPL). For more information about Open Source 
// licensing, please visit http://www.opensource.org/
//
// Octopus_OS is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation; either version 2 of the License, or
// (at your option) any later version.
//
// Octopus_OS is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with Octopus_OS; if not, write to the Free Software
// Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
//




//
// MASTER FILLER FUNCTION - prepares the MIR for display on the panel
//
// genoQs Machines 01.10.2005

extern void MIR_write_tempo( unsigned char tempo, unsigned char row );

extern bool page_is_chain_follow( Pagestruct* target_page );


// Transforms a grid cursor postion into a MIR index
unsigned char cursor_to_dot( unsigned char in_cursor ){

	return ( in_cursor + 11 + in_cursor/10 );
}



// Transforms a grid cursor postion into a MIR index
unsigned char NEMO_cursor_to_dot( unsigned char in_cursor ){

	unsigned char result = 0;
	unsigned char row0 = 0;

	row0 	= ((in_cursor / 4) +1 ) * 11;	// This gives 11, 22, 33, etc.. the row 0 dot indexes
	result 	= row0 + (in_cursor % 4);
	
//	switch( in_cursor ){
//	
//		case 0:		result = 11;	break;
//		case 1:		result = 12;	break;
//		case 2:		result = 13;	break;
//		case 3:		result = 14;	break;
//		
//		case 4:		result = 22;	break;
//		case 5:		result = 23;	break;
//		case 6:		result = 24;	break;
//		case 7:		result = 25;	break;
//
//		case 8:		result = 33;	break;
//		case 9:		result = 34;	break;
//		case 10:	result = 35;	break;
//		case 11:	result = 36;	break;
//	}

	// return ( in_cursor + 11 + in_cursor/10 );
	return result;
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
	#ifdef FEATURE_ENABLE_DICE
	Trackstruct* target_dice = Dice_get();
	#endif
	// x2 - Track row window shift
	unsigned char shiftTrackRow = track_get_window_shift( target_page );;
	unsigned char shiftPageRow	= page_get_window_shift();

	// ZOOM LEVEL
	show( ELE_ZOOMLEVEL, G_zoom_level );

	switch (G_zoom_level) {
		
		case zoomSYSEX:
			#include "NEMO_filler_SYSEX.h"
			break;

		case zoomAMANDA:
			#include "NEMO_filler_AMANDA.h"
			break;

		case zoomDEVICE:
			#include "NEMO_filler_DEVICE.h"
			break;
			
		case zoomABLETON:
//			#include "NEMO_filler_ABLETON.h"
			break;

		case zoomSTRUM:
			#include "NEMO_filler_STRUM.h"
			break;

		case zoomGRID:
			#include "NEMO_filler_GRID.h"
			break;

		case zoomGRIDTRACK:
			#include "NEMO_filler_GRIDTRACK.h"
			break;

		case zoomPAGE:
			#include "NEMO_filler_PAGE.h"		
			break;

		case zoomMIXMAP:
			#include "NEMO_filler_MIXMAP.h"
			break;

		case zoomTRACK:
			#include "NEMO_filler_TRACK.h"
			break;
			
//		case zoomMAP:
//			#include "NEMO_filler_MAP.h"
//			break;	
		
		case zoomSTEP:
			#include "NEMO_filler_STEP.h"
			break;

		case zoomDIAG:
			#include "NEMO_filler_DIAG.h"
			break;

		case zoomSCALE:
			#include "NEMO_filler_SCALE.h"
			break;

		#ifdef FEATURE_ENABLE_DICE
		case zoomDICE:
			#include "NEMO_filler_DICE.h"
			break;
		#endif
	} // switch ( target_page->zoomlevel );

} // void VIEWER_fill_MIR()


