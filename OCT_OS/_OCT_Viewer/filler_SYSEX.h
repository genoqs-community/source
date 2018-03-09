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
//	MIR_write_numeric_C( SYSEX_dump_speed );
	MIR_write_numeric_C( G_master_tempo );

	// Show the ESC LED - leaving the scale mode of the GRID asstn page
	MIR_write_dot( LED_RETURN, MIR_GREEN );

	// MATRIX - shows all pages in the matrix
	show( ELE_MATRIX, SYSEX );

	// TRACK SELECTORS - all lit - these are the bank triggers
	MIR_write_buttool( LHS, 0x1FF, 	MIR_GREEN );

	// SCENE SELECTOR - lit
	MIR_write_buttool( 	LHS, 1 << 9, MIR_RED );
	MIR_write_buttool( 	LHS, 1 << 9, MIR_GREEN );

	// MIX master is the GRID - This is the global setting trigger
	MIR_write_dot( LED_MIX_MASTER, MIR_GREEN );

	// SEL master in the GRID - This is the ALL PAGES trigger
	MIR_write_dot( LED_SELECT_MASTER, MIR_GREEN );

	// EDIT indicator - EDIT is the ALL data trigger
	MIR_write_dot( LED_EDIT_INDICATOR, MIR_GREEN );


//	// This is the rotating picture.. Progress of dumps
//	switch ( SYSEX_dump_progress ){
//
//		// Fallthrough overall intended - to get the packman effect
//		case 16:		MIR_write_dot( LED_NOTE_F, 		MIR_GREEN );
//		case 15:		MIR_write_dot( LED_NOTE_E, 		MIR_GREEN );
//		case 14:		MIR_write_dot( LED_NOTE_Dis, 	MIR_GREEN );
//		case 13:		MIR_write_dot( LED_NOTE_D, 		MIR_GREEN );
//		case 12:		MIR_write_dot( LED_NOTE_Cis, 	MIR_GREEN );
//		case 11:		MIR_write_dot( LED_NOTE_C, 		MIR_GREEN );
//		case 10:		MIR_write_dot( LED_SCALE_MOD,	MIR_GREEN );
//		case 9:			MIR_write_dot( LED_SCALE_SEL,	MIR_GREEN );
//		case 8:			MIR_write_dot( LED_SCALE_CAD,	MIR_GREEN );
//		case 7:			MIR_write_dot( LED_NOTE_Cup, 	MIR_GREEN );
//		case 6:			MIR_write_dot( LED_NOTE_B, 		MIR_GREEN );
//		case 5:			MIR_write_dot( LED_NOTE_Ais, 	MIR_GREEN );
//		case 4:			MIR_write_dot( LED_NOTE_A, 		MIR_GREEN );
//		case 3:			MIR_write_dot( LED_NOTE_Gis, 	MIR_GREEN );
//		case 2:			MIR_write_dot( LED_NOTE_G, 		MIR_GREEN );
//		case 1:			MIR_write_dot( LED_NOTE_Fis, 	MIR_GREEN );
//		case 0:			// Show nothing
//			break;
//	} // switch( progress )






