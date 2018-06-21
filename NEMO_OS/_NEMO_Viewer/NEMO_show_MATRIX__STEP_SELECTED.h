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



		row = target_page->stepSelectionSingleRow;
		col = target_page->stepSelectionSingleCol;

		if( row_in_track_window( target_page, row ) ){
			if (Step_get_status( target_page->Step[row][col], STEPSTAT_TOGGLE ) == ON ){
	//		if (Step_get_status( VIEWER_step, STEPSTAT_TOGGLE ) == ON ){

				MIR_write_dot( Page_dotIndex( row, col ) - shiftTrackRow,  MIR_GREEN );
			}
			else {
				MIR_write_dot( Page_dotIndex( row, col ) - shiftTrackRow,  MIR_RED );
			}
			MIR_write_dot( Page_dotIndex( row, col ) - shiftTrackRow,  MIR_BLINK);
		}


