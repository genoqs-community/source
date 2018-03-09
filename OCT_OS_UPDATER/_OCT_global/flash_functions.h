

//___________________________________________________________________________________________
// Store the executable into flash
void my_configfile_program_to_flash_old(){
	
	unsigned int err = 0;
	unsigned int* err_address = &err;
	unsigned int i=0;
			
	// Make sure no lights go off as they should not
	GLOBALBLINKER = ON;

	// Enable interrupts
//	cyg_interrupt_disable();

	// Progress bar part 1/3
	show_progress_bar( 1, MIR_RED );
	show_progress_bar( 2, MIR_RED );
	VIEWER_show_MIR();
		
 	// ERASE the flash before rewriting it
	cyg_mutex_lock( &mutex_FlashOperations );
	
	err = flash_erase(		(void *) MY_CONFIGFILE_BASE, 
							CONFIGFILE_SIZE,
							(void**) err_address  		);

	cyg_mutex_unlock( &mutex_FlashOperations );

	// Progress bar part 2/3
	show_progress_bar( 3, MIR_RED );
	show_progress_bar( 4, MIR_RED );
	VIEWER_show_MIR();

	// WRITE the bytes to the flash
	cyg_mutex_lock( &mutex_FlashOperations );

    err = flash_program(	(void *) MY_CONFIGFILE_BASE, 
							(void *) sysex_stream_keeper, 
							CONFIGFILE_SIZE, 
							(void **) err_address    	);

	cyg_mutex_unlock( &mutex_FlashOperations );

	// Last third in progress bar
	show_progress_bar( 3, MIR_RED );
	show_progress_bar( 4, MIR_RED );
	VIEWER_show_MIR();
		
 	// Wait - which is just an optical gimmic
 	for (i=0; i<30000; i++) {}
	
//	// Clear the code keeper buffer array
//	for ( i=0; i < CONFIGFILE_SIZE; i++ ){
//
//		sysex_stream_keeper[i] = 0;
//	}

	// Point the sysex_stream_keeper to the OSFILE start, after the CONFIGFILE
	sysex_stream_keeper = &(sysex_stream_keeper[ CONFIGFILE_SIZE ]);

	// Enable interrupts
//	cyg_interrupt_enable();
}



//___________________________________________________________________________________________
// Store the executable into flash
void my_configfile_program_to_flash(){
	
	unsigned int err = 0;
	unsigned int* err_address = &err;
	unsigned int i=0;
		
	// Make sure no lights go off as they should not
	GLOBALBLINKER = ON;

	// Progress bar part 1/3
	show_progress_bar( 1, MIR_RED );
	show_progress_bar( 2, MIR_RED );

	VIEWER_show_MIR();
		
 	// ERASE the flash before rewriting it
	cyg_mutex_lock( &mutex_FlashOperations );
	
	err = flash_erase(		(void *) 	MY_CONFIGFILE_BASE, 
										CONFIGFILE_SIZE,
							(void**)	err_address  			);

	cyg_mutex_unlock( &mutex_FlashOperations );

	// Progress bar part 2/3
	show_progress_bar( 3, MIR_RED );
	show_progress_bar( 4, MIR_RED );
	VIEWER_show_MIR();

	// WRITE the bytes to the flash
	cyg_mutex_lock( &mutex_FlashOperations );

    err = flash_program(	(void *) 	MY_CONFIGFILE_BASE, 
							(void *) 	sysex_stream_keeper, 
										CONFIGFILE_SIZE, 
							(void **) 	err_address    			);

	cyg_mutex_unlock( &mutex_FlashOperations );

	// Last third in progress bar
	show_progress_bar( 3, MIR_RED );
	show_progress_bar( 4, MIR_RED );
	VIEWER_show_MIR();
	
 	// Wait - which is just an optical gimmic
 	for (i=0; i<30000; i++) {}
}



//___________________________________________________________________________________________
// Store the executable into flash
void my_executable_program_to_flash(){
	
	unsigned int err = 0;
	unsigned int* err_address = &err;
	unsigned int i=0;
	
	// Disable interrupts
//	cyg_interrupt_disable();
		
	// Make sure no lights go off as they should not
	GLOBALBLINKER = ON;

	// Progress bar part 1/3
	show_progress_bar( 1, MIR_RED );
	show_progress_bar( 2, MIR_RED );

	VIEWER_show_MIR();
		
 	// ERASE the flash before rewriting it
	cyg_mutex_lock( &mutex_FlashOperations );
	
	err = flash_erase(		(void *) MY_EXECUTABLE_BASE, 
							OSFILE_SIZE,
							(void**) err_address  		);

	cyg_mutex_unlock( &mutex_FlashOperations );

	// Progress bar part 2/3
	show_progress_bar( 3, MIR_RED );
	show_progress_bar( 4, MIR_RED );
	VIEWER_show_MIR();

	// WRITE the bytes to the flash
	cyg_mutex_lock( &mutex_FlashOperations );

    err = flash_program(	(void *) MY_EXECUTABLE_BASE, 
							(void *) sysex_stream_keeper, 
							OSFILE_SIZE, 
							(void **) err_address    	);

	cyg_mutex_unlock( &mutex_FlashOperations );

	// Last third in progress bar
	show_progress_bar( 3, MIR_RED );
	show_progress_bar( 4, MIR_RED );
	VIEWER_show_MIR();
	
 	// Wait - which is just an optical gimmic
 	for (i=0; i<30000; i++) {}

	// Enable interrupts
//	cyg_interrupt_enable();
	
//	// Clear the code keeper buffer array
//	for ( i=0; i < OSFILE_SIZE; i++ ){
//
//		sysex_stream_keeper[i] = 0;
//	}

	// Return to DEVICE level - which is the natural context of this function
	// zoom_level = zoomDEVICE;
}




// Recall the flash contents into the running sequencer
void Octopus_recall_flash(){

	unsigned int i = 0;

	Pagestruct* target_page = NULL;

	// Read the flash contents into the memory image structure
	Flashbuffer_total_read_from_flash();		

	// Copy the Page data from the memory image
	for (i=0; i<MAX_NROF_PAGES; i++){
		
		Page_import_from_flatrecord_buffer( 
			&(memory_image.Page_flatrecord_repository[i]), 
			&Page_repository[i]               );
	}		

	// Copy the Grid data from the memory image
	Grid_import_from_flatrecord_buffer();

	// Clear the flashbuffer - i.e. leave it empty
	Page_flatrecord_repository_init();

	// Clear the grid part of the flashbuffer
	Grid_flatrecord_buffer_init();
	
	// Enter the page mode - copied from pressing ESC!
	target_page = &Page_repository[ GRID_CURSOR];
	target_page->trackSelection = OFF;
	target_page->trackAttributeSelection = OFF;

	zoom_level = zoomPAGE;
	target_page->keyMode = KMOD_PAGE;		
	
	// Release the MIDI SYSEX state
	MIDI_RECEIVE_SYSEX = FALSE;
	MIDI_SYSEX_HEADER_SEEN = FALSE;
	sysex_stream_keeper_index = 0;
}




