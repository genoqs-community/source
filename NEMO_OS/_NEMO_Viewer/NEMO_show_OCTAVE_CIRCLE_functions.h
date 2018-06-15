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


// Show the 4 user scales in the scale mode
void show_scale( 	unsigned short scale_pattern, 
					unsigned short scale_lead, 
					unsigned char target_row ){

	unsigned char i = 0;
	unsigned char offset = 33 + target_row;

	// ON fields showing first
	for (i=0; i<12; i++) {
		
		// If the note is selected as ON and not as LEAD
		if (	( scale_pattern & (1 << (11-i)) ) 
			&&	(!( scale_lead & (1 << (11-i))) )
			){

			// Write the green dots in the matrix
			MIR_write_dot( offset + (i*11), MIR_GREEN );
		}
	} // ON field iterator
	

	// UP fields showing first. This shows the scale lead
	for (i=0; i<12; i++) {
		
		// If the note is selected as UP
		if ( scale_lead & (1 << (11-i) ) ) {							
			
			// Write the red dot in the matrix
			MIR_write_dot( offset + (i*11), MIR_RED );
		}
	} // UP field for iterator
}



// Show the scale selection of the given page
void show_OCTAVE_CIRCLE_scale_selection( Pagestruct* target_page ){

	unsigned int 	i = 0,
					j = 0;

	for (i=0; i<4; i++){
	
		show_scale( 	target_page->scaleNotes[i+1],
						target_page->scaleLead [i+1],
						i );
	}
	return;


	// Determine which scale to show
	if ( target_page->scaleStatus == OFF ){
		j = target_page->scaleNotes[G_scale_ndx];
	}
	else{
		j = target_page->scaleNotes[G_scale_ndx];
	}
 

	// Show the SCL_align status - not relevant in GRID mode
	if ( target_page != GRID_assistant_page ){
		
		if ( target_page->SCL_align == OFF ){
		
			MIR_write_dot( LED_SCALE_CAD, MIR_GREEN );
		}
		else{

			MIR_write_dot( LED_SCALE_CAD, MIR_RED );
			MIR_write_dot( LED_SCALE_CAD, MIR_BLINK );
		}
	}


	// ON fields showing first
	for (i=0; i<12; i++) {
		
		// If the note is selected as ON and not as LEAD
		if (	( j & (1 << (11-i)) ) 
			&&	(!( target_page-> scaleLead[G_scale_ndx] & (1 << (11-i))) )
			){

			switch (i) {
				case 0:		MIR_write_dot( LED_NOTE_C, 		MIR_GREEN );	break;
				case 1:		MIR_write_dot( LED_NOTE_Cis, 	MIR_GREEN );	break;
				case 2:		MIR_write_dot( LED_NOTE_D,	 	MIR_GREEN );	break;
				case 3:		MIR_write_dot( LED_NOTE_Dis, 	MIR_GREEN );	break;
				case 4:		MIR_write_dot( LED_NOTE_E,	 	MIR_GREEN );	break;
				case 5:		MIR_write_dot( LED_NOTE_F,	 	MIR_GREEN );	break;
				case 6:		MIR_write_dot( LED_NOTE_Fis, 	MIR_GREEN );	break;
				case 7:		MIR_write_dot( LED_NOTE_G,	 	MIR_GREEN );	break;
				case 8:		MIR_write_dot( LED_NOTE_Gis, 	MIR_GREEN );	break;
				case 9:		MIR_write_dot( LED_NOTE_A, 		MIR_GREEN );	break;
				case 10:	MIR_write_dot( LED_NOTE_Ais, 	MIR_GREEN );	break;
				case 11:	MIR_write_dot( LED_NOTE_B,	 	MIR_GREEN );	break;
			}
		}
	} // ON field iterator

	// UP fields showing first. This shows the scale lead
	for (i=0; i<12; i++) {
		
		// If the note is selected as UP
		if ( target_page-> scaleLead[G_scale_ndx] & (1 << (11-i) ) ) {							
			
			switch (i) {
				case 0:		MIR_write_dot( LED_NOTE_C, 		MIR_RED );		break;
				case 1:		MIR_write_dot( LED_NOTE_Cis, 	MIR_RED );		break;
				case 2:		MIR_write_dot( LED_NOTE_D,	 	MIR_RED );		break;
				case 3:		MIR_write_dot( LED_NOTE_Dis, 	MIR_RED );		break;
				case 4:		MIR_write_dot( LED_NOTE_E,	 	MIR_RED );		break;
				case 5:		MIR_write_dot( LED_NOTE_F,	 	MIR_RED );		break;
				case 6:		MIR_write_dot( LED_NOTE_Fis, 	MIR_RED );		break;
				case 7:		MIR_write_dot( LED_NOTE_G,	 	MIR_RED );		break;
				case 8:		MIR_write_dot( LED_NOTE_Gis, 	MIR_RED );		break;
				case 9:		MIR_write_dot( LED_NOTE_A, 		MIR_RED );		break;
				case 10:	MIR_write_dot( LED_NOTE_Ais, 	MIR_RED );		break;
				case 11:	MIR_write_dot( LED_NOTE_B,	 	MIR_RED );		break;
			}
		}
	} // UP field for iterator

}



