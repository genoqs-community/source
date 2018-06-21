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

#include "../_OCT_global/flash-block.h"



//___________________________________________________________________________________________
// Store the executable into flash
void my_configfile_program_to_flash( unsigned char* data_source ){

	unsigned int err = 0;
	unsigned int* err_address = &err;
	unsigned int i=0;

	// Make sure no lights go off as they should not
	G_master_blinker = ON;

	// Enable interrupts
//	cyg_interrupt_disable();

	// Progress bar part 1/3
	show_progress_bar( 1, MIR_RED );
	show_progress_bar( 2, MIR_RED );
	show_progress_bar( 3, MIR_RED );
	VIEWER_show_MIR();

 	// ERASE the flash before rewriting it
	cyg_mutex_lock( &mutex_FlashOperations );

	err = flash_erase(		(void *) MY_CONFIGFILE_BASE,
							def_CONFIGFILE_SIZE,
							(void**) err_address  		);

	cyg_mutex_unlock( &mutex_FlashOperations );

	// Progress bar part 2/3
	show_progress_bar( 4, MIR_RED );
	show_progress_bar( 5, MIR_RED );
	show_progress_bar( 6, MIR_RED );
	VIEWER_show_MIR();

	// WRITE the bytes to the flash
	cyg_mutex_lock( &mutex_FlashOperations );

    err = flash_program(	(void *) MY_CONFIGFILE_BASE,
							(void *) data_source,
							def_CONFIGFILE_SIZE,
							(void **) err_address    	);

	cyg_mutex_unlock( &mutex_FlashOperations );

	// Last third in progress bar
	show_progress_bar( 7, MIR_RED );
	show_progress_bar( 8, MIR_RED );
	VIEWER_show_MIR();

 	// Wait - which is just an optical gimmick
 	for (i=0; i < 30000; i++) {}

	// Point the G_sysex_stream_keeper to the OSFILE start, after the CONFIGFILE
//	G_sysex_stream_keeper = &(G_sysex_stream_keeper[ def_CONFIGFILE_SIZE ]);

	// Enable interrupts
//	cyg_interrupt_enable();
}


#ifdef NEMO
//___________________________________________________________________________________________
// Store the executable into flash
void my_clear_flash(){

	unsigned int err = 0;
	unsigned int* err_address = &err;

	// Disable interrupts
	cyg_interrupt_disable();

	// Make sure no lights go off as they should not
	G_master_blinker = ON;

	// Last third in progress bar
	show_progress_bar( 3, MIR_RED );
	show_progress_bar( 4, MIR_RED );
	VIEWER_show_MIR();

 	// ERASE the flash between the OS file and 1900000 - presumably Nemo needs it
	cyg_mutex_lock( &mutex_FlashOperations );
	err = flash_erase(		(void *) MY_OSFILE_BASE + def_OSFILE_SIZE,
// E0000 is a much larger area than the OS covers, going to 1900000 - needed by Nemo apparently.. (??)
							0x01900000 - (MY_OSFILE_BASE + def_OSFILE_SIZE),
							(void**) err_address  		);
	cyg_mutex_unlock( &mutex_FlashOperations );

	// Enable interrupts
	cyg_interrupt_enable();

	// Return to DEVICE level - which is the natural context of this function
	G_zoom_level = zoomDEVICE;
}
#endif


//___________________________________________________________________________________________
// Store the executable into flash
void my_OS_program_to_flash( unsigned char* data_source ){

	unsigned int err = 0;
	unsigned int* err_address = &err;
	unsigned int i=0;

	// Disable interrupts
	cyg_interrupt_disable();

	// Make sure no lights go off as they should not
	G_master_blinker = ON;

	// Progress bar part 1/3
	show_progress_bar( 1, MIR_RED );
	show_progress_bar( 2, MIR_RED );
	show_progress_bar( 3, MIR_RED );
	VIEWER_show_MIR();

 	// ERASE the flash before rewriting it
	cyg_mutex_lock( &mutex_FlashOperations );
	err = flash_erase(		(void *) MY_OSFILE_BASE,
							def_OSFILE_SIZE,
							(void**) err_address  		);
	cyg_mutex_unlock( &mutex_FlashOperations );

	// Progress bar part 2/3
	show_progress_bar( 4, MIR_RED );
	show_progress_bar( 5, MIR_RED );
	show_progress_bar( 6, MIR_RED );
	VIEWER_show_MIR();

	// WRITE the bytes to the flash
	cyg_mutex_lock( &mutex_FlashOperations );
    err = flash_program(	(void *) MY_OSFILE_BASE,
							(void *) data_source,
							def_OSFILE_SIZE,
							(void **) err_address    	);
	cyg_mutex_unlock( &mutex_FlashOperations );

	// Last third in progress bar
	show_progress_bar( 7, MIR_RED );
	show_progress_bar( 8, MIR_RED );
	VIEWER_show_MIR();

 	// Wait - which is just an optical gimmic
 	for (i=0; i<30000; i++) {}

	// Enable interrupts
	cyg_interrupt_enable();

	// Return to DEVICE level - which is the natural context of this function
	G_zoom_level = zoomDEVICE;
}




// Recall the flash contents into the running sequencer
void Octopus_recall_flash()
{
	Pagestruct* target_page = NULL;

	// Read all pages and grid from flash into memory.
	Flash_read_all_pages();
	Flash_read_grid();

	// Enter the page mode - copied from pressing ESC!
	target_page = &Page_repository[ GRID_CURSOR];
	target_page->trackSelection = OFF;
	target_page->trackAttributeSelection = OFF;

	// Enter Page mode - although GRID would be more appropriate..
#ifdef FEATURE_SOLO_REC
	G_zoom_level = zoomGRID; // TODO: use persistent toggle bit from G_midi_map_controller_mode
#else
	G_zoom_level = zoomPAGE;
#endif

	// Release the MIDI SYSEX state
	MIDI_RECEIVE_SYSEX = FALSE;
	MIDI_SYSEX_HEADER_SEEN = FALSE;
	G_sysex_stream_keeper_index = 0;
}






