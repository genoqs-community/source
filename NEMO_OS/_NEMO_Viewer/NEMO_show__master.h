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
 * Viewer code for all kinds of occasions
 * genoQs Machines, 07.11.2003
 */
#include "../_NEMO_Viewer/NEMO_show_OCTAVE_CIRCLE_functions.h"
#include "../_NEMO_Viewer/NEMO_show_GLOBALS_scale_master.h"
#include "/home/genoqs/Desktop/Octopus-fork/OCT_OS_v1.60/_OCT_Viewer/show_SCALE_SELECTOR_scale_selection_function.h"

extern void show_chord_in_line( Stepstruct* target_step, unsigned char offset );
extern unsigned char is_selected_in_GRID( Pagestruct* target_page );
extern char* ptr_of_track_attr_value( 	Pagestruct* target_page,
								unsigned char track_ndx,
								unsigned char attr_ndx );

// This is the core routine to refresh the page - 
// so it can be used also after interrupts have been disabled
void Page_full_refresh() {

		// Refresh the page
		initMIR();

		initGRID();

		VIEWER_fill_MIR();

		VIEWER_show_MIR();
}


//
// SHOW PAGE THREAD - This is the monitor thread used to refresh the page
//
void showPage_thread (cyg_addrword_t index) {
	
//	unsigned int pvalue1=0;
//	unsigned int pvalue2=0;
	
	// Infinite loop for monitoring the request queues
	while (1){

		// Wait on the refresh semaphore
		cyg_semaphore_wait( &sem_showPage );
			
// Not locking the scheduler works only in conjuntion with giving it same prio as the MIDI OUT threads 
//		cyg_scheduler_lock();

			// Refresh the page	
			Page_full_refresh();
	
//		cyg_scheduler_unlock();

		cyg_semaphore_init( &sem_showPage, 0 );

	} // while( 1 )
}



// Return the Hardware address corresponding to the passed index of LED in the octave circle
unsigned char circle_address_of( unsigned char note_ndx ){
	
	unsigned char result = 0;

	switch( note_ndx ){
		case 0:		result = LED_NOTE_Fis;		break;
		case 1:		result = LED_NOTE_G;		break;
		case 2:		result = LED_NOTE_Gis;		break;
		case 3:		result = LED_NOTE_A;		break;
		case 4:		result = LED_NOTE_Ais;		break;
		case 5:		result = LED_NOTE_B;		break;
		case 6:		result = LED_NOTE_Cup;		break;
		case 7:		result = LED_SCALE_CAD;		break;
		case 8:		result = LED_SCALE_SEL;		break;
		case 9:		result = LED_SCALE_MOD;		break;
		case 10:	result = LED_NOTE_C;		break;
		case 11:	result = LED_NOTE_Cis;		break;
		case 12:	result = LED_NOTE_D;		break;
		case 13:	result = LED_NOTE_Dis;		break;
		case 14:	result = LED_NOTE_E;		break;
		case 15:	result = LED_NOTE_F;		break;
	}

	return result;
}


//
// FLASH OPS THREAD - This is the progress bar for the flash ops (program operation)
//
void flashOps_thread (cyg_addrword_t index) {
	
	unsigned char i = 0;
		
	// Infinite loop for monitoring the request queues
	while (1){

		// Wait on the semaphore
		cyg_semaphore_wait( &sem_flashOps );
			
	cyg_scheduler_lock();

		for( i=0; i<16; i++ ){

			MIR_write_dot( circle_address_of( i ),  MIR_RED);
			MIR_write_dot( circle_address_of( i ),  MIR_GREEN);
			
			VIEWER_show_MIR();			
			cyg_thread_delay( 1000000000 );
		}
		
	cyg_scheduler_unlock();

	} // while( 1 )
}






//
// GRID INIT - initialize the Pa
//
void initGRID() {
	
//	unsigned int side=0, ndx=0;
//		
//	for (side = 0; side < 2; side ++){
//		for (ndx=0; ndx<10; ndx++) {
//			Page_GRID[side][ndx][MIR_GREEN] = 0;
//			Page_GRID[side][ndx][MIR_RED] = 0;
//			Page_GRID[side][ndx][MIR_BLINK] = 0;	
//		}
//	}

	// unsigned char 	Page_GRID[2][10][3];
	memset( Page_GRID, 0, 60 );
}

//
// MIR -> GRID function - Write the contents of the GRID to the MIR
//
void MIR_write_GRID() {
	
	unsigned int row=0, side=0;

	// LHS
	side = LHS;	
	for (row=0; row<10; row++) {
		
		// Transfer the ON information
		MIR[side][row][MIR_RED] = Page_GRID[side][row][MIR_RED];
		// Transfer the Select information
		MIR[side][row][MIR_BLINK] = Page_GRID[side][row][MIR_BLINK];
		// Transfer the Select information
		MIR[side][row][MIR_GREEN] = Page_GRID[side][row][MIR_GREEN];


	}

	// RHS
	side = RHS;	
	for (row=0; row<10; row++) {
		
		// Transfer the ON information
		MIR[side][row][MIR_RED] = Page_GRID[side][row][MIR_RED];
		// Transfer the Select information
		MIR[side][row][MIR_BLINK] = Page_GRID[side][row][MIR_BLINK];
		// Transfer the Select information
		MIR[side][row][MIR_GREEN] = Page_GRID[side][row][MIR_GREEN];
	}

}

// The MIR filler functions
#include "../_NEMO_Viewer/NEMO_filler_functions.h"

// The main show function
#include "../_NEMO_Viewer/NEMO_show_show.h"

// Filling the MIR as bitmatrix
#include "../_NEMO_Viewer/NEMO_filler__master.h"

// Hardware driver code
#include "../_NEMO_Viewer/NEMO_show_hwdriver.h"

// General show functions
#include "../_NEMO_Viewer/NEMO_show_functions.h"

