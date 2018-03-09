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
#include "NEMO_rot_functions.h"
#include "NEMO_rot_editorblock.h"
#include "NEMO_rot_DIAG.h"	
#include "NEMO_rot_GRID.h"
#include "NEMO_rot_GRIDTRACK.h"
#include "NEMO_rot_PAGE.h"
#include "NEMO_rot_TRACK.h"
// #include "NEMO_rot_MAP.h"
#include "NEMO_rot_STEP.h"
#include "NEMO_rot_STRUM.h"
#include "NEMO_rot_SCALE.h"
#include "NEMO_rot_SYSEX.h"
#include "NEMO_rot_mixerblock.h"

// EXECUTE ROTARY impulse data
void executeRot( unsigned int in_rotNdx ){
	
	// Work on the page under the grid cursor
	Pagestruct* target_page = &Page_repository[GRID_CURSOR];

	// Get rotary data: encodes both rotary address and direction
	unsigned char rotNdx 	= in_rotNdx >> 2;
	unsigned char direction = in_rotNdx & 0x03;
	
	// Transform the rotNdx of the mixer encoders to match the backend Octopus model
	switch( rotNdx ){	
		case 5:		rotNdx = 12;		break;
		case 6:		rotNdx = 11;		break;
		case 7:		rotNdx = 14;		break;
		case 8:		rotNdx = 13;		break;
	}

	// d_iag_printf( "rotNdx:%d, direction:%d\n", rotNdx, direction );

	// LAB SESSION
//	if ( rotNdx == 0 ){
//		modify_parameter( &NEMO_CHUNKSHIFT, 0, 8, direction, OFF, FIXED);
//	}
//	else if ( rotNdx == 1 ){
//		modify_parameter( &NEMO_REDSHIFT, 0, 8, direction, OFF, FIXED);
//	}
//	else if ( rotNdx == 2 ){
//		modify_parameter( &NEMO_GREENSHIFT, 0, 8, direction, OFF, FIXED);
//	}
//	else if ( rotNdx == 3 ){
//		modify_parameter( &NEMO_REDSHIFT, 0, 8, direction, OFF, FIXED);
//		modify_parameter( &NEMO_GREENSHIFT, 0, 8, direction, OFF, FIXED);
//	}
//
//	d_iag_printf( "rotNdx:%d, dir:%d shift:%d\n", 
//					rotNdx, direction, NEMO_CHUNKSHIFT );


	// Encoder operation depending on zoom level
	switch( G_zoom_level ){
	
		case zoomSYSEX:
		case zoomDEVICE:
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

		case zoomSTEP:
			NEMO_rot_exec_STEP( target_page, rotNdx, direction );
			break;

		case zoomSTRUM:
			rot_exe_STRUM( target_page, rotNdx, direction );
			break;

		case zoomSCALE:
			// rot_exe_SCALE( GRID_assistant_page, rotNdx, direction );
			break;
	}
		
	// Update HUI with changes performed by rotarys
	Page_requestRefresh();
}

