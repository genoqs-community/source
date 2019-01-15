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





void show_SCALE_SELECTOR_scale_selection( Pagestruct* target_page ){

	#ifdef FEATURE_SOLO_REC
	if ( G_zoom_level == zoomSOLOREC ){

		if ( SOLO_scale_chords_program == ON ) return;
		if ( SOLO_rec_transpose == ON ){

			target_page = &Page_repository[ GRID_CURSOR ];
		}
	}
	if ( G_zoom_level != zoomSOLOREC ){
	#endif

		// Light all green because we can always use them
		MIR_write_dot( LED_SCALE_MYSEL,	MIR_GREEN	);
		MIR_write_dot( LED_SCALE_PEN, 	MIR_GREEN 	);
		MIR_write_dot( LED_SCALE_WHL, 	MIR_GREEN 	);
		MIR_write_dot( LED_SCALE_MAJ, 	MIR_GREEN 	);
		MIR_write_dot( LED_SCALE_MIN, 	MIR_GREEN	);
		MIR_write_dot( LED_SCALE_DIM, 	MIR_GREEN	);
		MIR_write_dot( LED_SCALE_CHR,	MIR_GREEN	);
	
	#ifdef FEATURE_SOLO_REC
	}

	unsigned int j = my_shift_bitpattern( target_page-> scaleNotes[G_scale_ndx], 12, INC,
							  my_bit2ndx( target_page-> scaleLead[G_scale_ndx] )+1);


	switch( j ) {

		case SCALE_SIG_PEN:
			MIR_write_dot( LED_SCALE_PEN,	MIR_GREEN );
			MIR_write_dot( LED_SCALE_PEN,	MIR_RED	);
			break;

		case SCALE_SIG_WHL:
			MIR_write_dot( LED_SCALE_WHL,	MIR_GREEN );
			MIR_write_dot( LED_SCALE_WHL,	MIR_RED	);
			break;

		case SCALE_SIG_MAJ:
			MIR_write_dot( LED_SCALE_MAJ,	MIR_GREEN );
			MIR_write_dot( LED_SCALE_MAJ,	MIR_RED	);
			break;

		case SCALE_SIG_MIN:
			MIR_write_dot( LED_SCALE_MIN,	MIR_GREEN );
			MIR_write_dot( LED_SCALE_MIN,	MIR_RED	);
			break;

		case SCALE_SIG_DIM:
			MIR_write_dot( LED_SCALE_DIM,	MIR_GREEN );
			MIR_write_dot( LED_SCALE_DIM,	MIR_RED	);
			break;

		case SCALE_SIG_CHR:
			MIR_write_dot( LED_SCALE_CHR,	MIR_GREEN );
			MIR_write_dot( LED_SCALE_CHR,	MIR_RED	);
			break;

		default:
			MIR_write_dot( LED_SCALE_MYSEL,	MIR_GREEN );
			MIR_write_dot( LED_SCALE_MYSEL,	MIR_RED	);
			break;
	}
	#else

	switch( j ) {

		case SCALE_SIG_PEN:
			MIR_write_dot( LED_SCALE_PEN,	MIR_RED	);
//			MIR_write_dot( LED_SCALE_PEN,	MIR_BLINK	);
			break;

		case SCALE_SIG_WHL:
			MIR_write_dot( LED_SCALE_WHL,	MIR_RED	);
//			MIR_write_dot( LED_SCALE_WHL,	MIR_BLINK	);
			break;

		case SCALE_SIG_MAJ:
			MIR_write_dot( LED_SCALE_MAJ,	MIR_RED	);
//			MIR_write_dot( LED_SCALE_MAJ,	MIR_BLINK	);
			break;

		case SCALE_SIG_MIN:
			MIR_write_dot( LED_SCALE_MIN,	MIR_RED	);
//			MIR_write_dot( LED_SCALE_MIN,	MIR_BLINK	);
			break;

		case SCALE_SIG_DIM:
			MIR_write_dot( LED_SCALE_DIM,	MIR_RED	);
//			MIR_write_dot( LED_SCALE_DIM,	MIR_BLINK	);
			break;

		case SCALE_SIG_CHR:
			MIR_write_dot( LED_SCALE_CHR,	MIR_RED	);
//			MIR_write_dot( LED_SCALE_CHR,	MIR_BLINK	);
			break;

		default:
			MIR_write_dot( LED_SCALE_MYSEL,	MIR_RED	);
//			MIR_write_dot( LED_SCALE_MYSEL,	MIR_BLINK	);
			break;
	}
	#endif
}


