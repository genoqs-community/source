/* OCTOPUS main.c file
 */

#include "_OCT_global/includes.h"

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

	// CHECK KEY PRESSES ON POWER-ON
	// First key scan - need to fill the scan array, so we can proceed from here
	scanKeys();


	// Enter the device maintenance mode
	zoom_level = zoomDEVICE;
	FLASH_LOAD_FLAG = FALSE;
	
	// Code keeper array now uses the memory reserved for the step repository.
	// sysex_stream_keeper = (unsigned char*) Step_repository;
	sysex_stream_keeper = (void*) 0x00098000;
	
	return;
	

	// Check whether there is a pressed key
	//if ( TRUE ){ // used only for development purposes - zoom DEVICE
	if ( scanKeys() == TRUE ) {
	
		// Start the sequencer without loading the memory content
		if ( is_pressed_key( KEY_CLEAR ) ){

			// Seed the randomizer
			srand( cyg_current_time() );

			// Pretend data was loaded, without actually doing it
			POWERON_LOADED = TRUE;

			// Inhibit data loading further below
			FLASH_LOAD_FLAG = FALSE;		
		}

		else if ( is_pressed_key( KEY_RETURN ) ){

			// Enter the device maintenance mode
			zoom_level = zoomDEVICE;
			FLASH_LOAD_FLAG = FALSE;
			
			// Code keeper array now uses the memory reserved for the step repository.
			sysex_stream_keeper = (unsigned char*) Step_repository;
		}
	}
	// No key was pressed on power-on
	else {
		
		// Code keeper array points to a sysex_in buffer.
		sysex_stream_keeper = (unsigned char*) SYSEX_IN_flatrecord;
	}


	// Finally, recall data stored in flash - includes the randomizer seed
	if ( FLASH_LOAD_FLAG == TRUE ){

		Octopus_recall_flash();
		POWERON_LOADED = TRUE;
	}

	// Start with a fresh display
	Page_requestRefresh();	

	// From here execution is interrupt-driven. Enjoy!
}


