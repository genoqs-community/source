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



// Take a page, a step pointer and a row, and return the col index of the given step
unsigned char col_of_step( Pagestruct* target_page, Stepstruct* target_step, unsigned char row ){
	
	unsigned char col=0;

	for ( col=0; col < MATRIX_NROF_COLUMNS; col++ ){

		if ( target_step == target_page->Step[row][col] ){
			break;
		}
	}
	return col;
}


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
	show( ELE_ZOOMLEVEL, G_zoom_level );

	switch (G_zoom_level) {
		
		case zoomSYSEX:
			#include "filler_SYSEX.h"
			break;

		case zoomAMANDA:
			#include "filler_AMANDA.h"
			break;

		case zoomDEVICE:
			#include "filler_DEVICE.h"
			break;
			
		case zoomABLETON:
//			#include "filler_ABLETON.h"
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

		case zoomDIAG:
			#include "filler_DIAG.h"
			break;

	} // switch ( target_page->zoomlevel );

} // void VIEWER_fill_MIR()


