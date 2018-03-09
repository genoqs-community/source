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



	// Blink the EXC LED in orange
	MIR_write_dot( LED_ALIGN, MIR_GREEN );
	MIR_write_dot( LED_ALIGN, MIR_RED );
	MIR_write_dot( LED_ALIGN, MIR_BLINK );
	
	// NUMERIC QUADRANT
	// MIR_write_tempo( SYSEX_dump_speed, 0 );

	// Show the ESC LED - leaving the scale mode of the GRID asstn page
	MIR_write_dot( LED_RETURN, MIR_GREEN );

	// MATRIX - shows all pages in the matrix
	show( ELE_MATRIX, SYSEX );

	// TRACK SELECTORS - all lit - these are the bank triggers
	MIR_write_buttool( LHS, 0x1FF, 	MIR_GREEN );
	
	// MIX master is the GRID - This is the global setting trigger
	MIR_write_dot( LED_MIX_MASTER, MIR_GREEN );
	
	// SEL master in the GRID - This is the ALL PAGES trigger
	MIR_write_dot( LED_SELECT_MASTER, MIR_GREEN );
		
	// EDIT master - EDIT is the ALL data trigger
	MIR_write_dot( LED_EDIT_MASTER, MIR_GREEN );

	




