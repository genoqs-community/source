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


/* OCTOPUS main.c file - containing the entry point to execution.
 */
#define HEADER(FILE) <BASELINE_PATH/FILE>

#define BASELINE_PATH /home/genoqs/workspace/OCT_OS

#include "_NEMO_global/NEMO_includes.h"
#include HEADER(_OCT_global/flash-block.h)

//
// MAIN ENTRY POINT INTO THE EXECUTION
//
void cyg_user_start( void ){

	unsigned int pvalue=0;

	// Mandatory init function of flash.
	// Needed for subsequent flash operations
	flash_init( diag_printf );

	// Initialize the hardware
	init_hardware();

	// Set up the interrupts used by system
	init_interrupts();

	// Set up the mailboxes used by threads
	init_mailboxes();

	// Set up the threads used by the system
	init_threads();

	// Set up the alarms used by the system
	init_alarms();

	// Seed the randomizer
	HAL_CLOCK_READ( &pvalue );

	// Data storage / memory initialization sequence
	Octopus_memory_init();


//  /////////////////////////////////////////////////
//	// Make this code an OS updater!
//	d_iag_printf( "&Step_rep:%p 0x%X -> 0x%lX \n 0x%lX\n",
//		&Step_repository,
//		(unsigned int) &Step_repository,
//		((unsigned int) &Step_repository) + (MAX_NROF_STEPS * sizeof(Stepstruct)),
//		sizeof(Stepstruct) * MAX_NROF_STEPS
//		);
//
//	// Make this code an OS updater!
//	d_iag_printf( "&Track_rep:%p 0x%X - 0x%lX \n",
//		&Track_repository,
//		(unsigned int) &Track_repository,
//		((unsigned int) &Track_repository) + (MAX_NROF_TRACKS * sizeof(Trackstruct))
//		);
//
//	// Make this code an OS updater!
//	d_iag_printf( "&Page_rep:%p 0x%X - 0x%lX \n",
//		&Page_repository,
//		(unsigned int) &Page_repository,
//		((unsigned int) &Page_repository) + (MAX_NROF_PAGES * sizeof(Pagestruct))
//		);
//
//	d_iag_printf( "&Track_rep: 0x%X - 0x%X\n  #%d * %d = %d (0x%X)\n",
//		(unsigned int) &Track_repository,
//		(unsigned int) &Track_repository + sizeof(Trackstruct) * MAX_NROF_TRACKS,
//		MAX_NROF_TRACKS,
//		sizeof(Trackstruct),
//		sizeof(Trackstruct) * MAX_NROF_TRACKS,
//		sizeof(Trackstruct) * MAX_NROF_TRACKS 	);
//
//	d_iag_printf( "&Page_rep: 0x%X - 0x%X\n  #%d * %d = %d (0x%X)\n",
//		(unsigned int) &Page_repository,
//		(unsigned int) &Page_repository + sizeof(Pagestruct) * MAX_NROF_PAGES,
//		MAX_NROF_PAGES,
//		sizeof(Pagestruct),
//		sizeof(Pagestruct) * MAX_NROF_PAGES,
//		sizeof(Pagestruct) * MAX_NROF_PAGES 	);

//	// Enter the device maintenance mode
//	G_zoom_level = zoomDEVICE;
//	G_flashload_flag = FALSE;
//
//	// Code keeper array now uses the memory reserved for the step repository.
//	G_sysex_stream_keeper = (void*) Step_repository;
//
//	return;


		// First key scan - need to fill the scan array, so we can proceed from here
	scan_keys();

	// CHECK KEY PRESSES ON POWER-ON
	if ( scan_keys() == TRUE ) {

		// Start the sequencer without loading the memory content
		if ( is_pressed_key( KEY_CLEAR ) ){

			// Seed the randomizer
			srand( cyg_current_time() );

			// Pretend data was loaded, without actually doing it
			POWERON_LOADED = TRUE;

			// Inhibit data loading further below
			G_flashload_flag = FALSE;

			static card32 AlignedSysexStreamBuffer[ (FLASH_BLOCK_SIZE + 32) / sizeof(card32) ];

			// Code keeper array points to a sysex_in buffer.
			G_sysex_stream_keeper = (card8*) &AlignedSysexStreamBuffer;
			G_sysex_stream_keeper_size = FLASH_BLOCK_SIZE;
		}

		else if ( is_pressed_key( KEY_RETURN ) ){

			// Enter the device maintenance mode
			G_zoom_level = zoomDEVICE;
			G_flashgridheadersonly_flag = TRUE; // only load the grid

			// Sysex stream keeper array now uses the memory reserved for the step repository.
			G_sysex_stream_keeper = (unsigned char*) Step_repository;
			G_sysex_stream_keeper_size = sizeof(Step_repository);
		}
	}
	// No key was pressed on power-on. This normal runtime mode.
	else {
		static card32 AlignedSysexStreamBuffer[ (FLASH_BLOCK_SIZE + 32) / sizeof(card32) ];

		// Code keeper array points to a sysex_in buffer.
		G_sysex_stream_keeper = (card8*) &AlignedSysexStreamBuffer;
		G_sysex_stream_keeper_size = FLASH_BLOCK_SIZE;
	}


	// Finally, recall data stored in flash - includes the randomizer seed
	if ( G_flashload_flag == TRUE ){

		Octopus_recall_flash();

		POWERON_LOADED = TRUE;
	}

	MIDICLOCK_PASSTHROUGH = TRUE;


	// Start with a fresh display
	Page_requestRefresh();

	// From here on the execution is interrupt-driven. Enjoy!
}


