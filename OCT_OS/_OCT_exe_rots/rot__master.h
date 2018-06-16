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
#include "rot_SYSEX.h"
#include "rot_mixerblock.h"

// EXECUTE ROTARY impulse data
void executeRot( unsigned int in_rotNdx ){
	
	// Work on the page under the grid cursor
	Pagestruct* target_page = &Page_repository[GRID_CURSOR];

	// Get rotary data: encodes both rotary address and direction
	unsigned char rotNdx 	= in_rotNdx >> 2;
	unsigned char direction = in_rotNdx & 0x03;
	
	
	// Encoder operation depending on zoom level
	switch( G_zoom_level ){
	
		case zoomSYSEX:
			rot_exe_SYSEX( 	target_page, rotNdx, direction );
			break;

		case zoomDIAG:
			rot_exe_DIAG( 	target_page, rotNdx, direction );
			break;

		case zoomAMANDA:
			// Allow tempo modification
			if ( rotNdx == 0 ){
				
				// When not running on external clock
				if ( G_clock_source != EXT ){
	
					modify_parameter( &G_master_tempo, MIN_TEMPO, MAX_TEMPO, direction, ON, FIXED);
					G_TIMER_REFILL_update();
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

