/* OCTOPUS main.c file
 */


// 
// PLATFORM INDEPENDENT INCLUDE FILES
//
#include <stdio.h>
#include <stdlib.h>
#include <cyg/infra/diag.h> // needed for d_iag_printf()

//
// PLATFORM SPECIFIC INCLUDE FILES
//
// Latest working version: E7T_eA_34_1_*
#include <d:\cygwin\home\Root\ecos_work\ecos_builds\E7T\E7T_eA_34_1_install\include\cyg\kernel\kapi.h>
#include <d:\cygwin\home\Root\ecos_work\ecos_builds\E7T\E7T_eA_34_1_install\include\cyg\hal\hal_io.h>
#include <d:\cygwin\home\Root\ecos_work\ecos_builds\E7T\E7T_eA_34_1_install\include\cyg\io\flash.h>

#include "_OCT_global/function_signatures.h"

#include "_OCT_global/defs_general.h"
#include "_OCT_global/variables.h"
#include "_OCT_global/tools_general.h"

#include "_OCT_objects/Panel.h"

#include "_OCT_objects/Step.h"
#include "_OCT_objects/Track.h"
#include "_OCT_objects/Page.h"
#include "_OCT_objects/Page_flat.h"
#include "_OCT_objects/MIDI_queue.h"
#include "_OCT_objects/NOTE_ON_queue.h"
#include "_OCT_objects/Grid.h"

#include "_OCT_global/Grid_image_struct.h"

#include "_OCT_init/Init_memory.h"

#include "_OCT_Viewer/show__master.h"
#include "_OCT_Player/play__master.h"
#include "_OCT_Player/MIDI_interpreter.h"

#include "_OCT_interrupts/Intr_KEY.h"
#include "_OCT_interrupts/Intr_ROTARY.h"
#include "_OCT_interrupts/Intr_TMR.h"
#include "_OCT_interrupts/Intr_IN_UARTs.h"

#include "_OCT_exe_keys/key__master.h"
#include "_OCT_exe_rots/rot__master.h"

#include "_OCT_global/alarm_handlers.h"



// Flash IO Construction Site
// flash_info  my_flash_info;


// Universal thread stack size definition
#define	MY_THREAD_STACK_SIZE		12288 // 4096

#define TIMERINTERRUPT_THREAD_STACK_SIZE ( MY_THREAD_STACK_SIZE / sizeof(int) )
int timerInterrupt_thread_stack [TIMERINTERRUPT_THREAD_STACK_SIZE];
cyg_handle_t	timerInterrupt_thread_handle;
cyg_thread		timerInterrupt_thread_obj;

#define KEYEXECUTE_THREAD_STACK_SIZE ( MY_THREAD_STACK_SIZE / sizeof(int) )
int keyExecute_thread_stack [KEYEXECUTE_THREAD_STACK_SIZE];
cyg_handle_t	keyExecute_thread_handle;
cyg_thread		keyExecute_thread_obj;
cyg_mbox		keyNdx_mbox;

#define SCANKEYS_THREAD_STACK_SIZE ( MY_THREAD_STACK_SIZE / sizeof(int) )
int scanKeys_thread_stack [SCANKEYS_THREAD_STACK_SIZE];
cyg_handle_t	scanKeys_thread_handle;
cyg_thread		scanKeys_thread_obj;

#define ROTEXECUTE_THREAD_STACK_SIZE ( MY_THREAD_STACK_SIZE / sizeof(int) )
int rotExecute_thread_stack [ROTEXECUTE_THREAD_STACK_SIZE];
cyg_handle_t	rotExecute_thread_handle;
cyg_thread		rotExecute_thread_obj;
cyg_mbox		rotNdx_mbox;

#define SHOWPAGE_THREAD_STACK_SIZE ( MY_THREAD_STACK_SIZE / sizeof(int) )
int showPage_thread_stack [SHOWPAGE_THREAD_STACK_SIZE];
cyg_handle_t 	showPage_thread_handle;
cyg_thread 		showPage_thread_obj;

#define FLASHOPS_THREAD_STACK_SIZE ( MY_THREAD_STACK_SIZE / sizeof(int) )
int flashOps_thread_stack [FLASHOPS_THREAD_STACK_SIZE];
cyg_handle_t 	flashOps_thread_handle;
cyg_thread 		flashOps_thread_obj;



//
// MIDI PORT 1
//
#define UART0_OUT_THREAD_STACK_SIZE ( MY_THREAD_STACK_SIZE / sizeof(int) )
int 			UART0_OUT_thread_stack [UART0_OUT_THREAD_STACK_SIZE];
cyg_handle_t	UART0_OUT_thread_handle;
cyg_thread		UART0_OUT_thread_obj;
cyg_mbox		UART0_OUT_mbox;

#define UART0_IN_THREAD_STACK_SIZE ( MY_THREAD_STACK_SIZE / sizeof(int) )
int 			UART0_IN_thread_stack [UART0_IN_THREAD_STACK_SIZE];
cyg_handle_t	UART0_IN_thread_handle;	
cyg_thread		UART0_IN_thread_obj;
cyg_mbox		UART0_IN_mbox; 


//
// MIDI PORT 2 
//
#define UART1_OUT_THREAD_STACK_SIZE ( MY_THREAD_STACK_SIZE / sizeof(int) )
int 			UART1_OUT_thread_stack [UART1_OUT_THREAD_STACK_SIZE];
cyg_handle_t	UART1_OUT_thread_handle;
cyg_thread		UART1_OUT_thread_obj;
cyg_mbox		UART1_OUT_mbox;

#define UART1_IN_THREAD_STACK_SIZE ( MY_THREAD_STACK_SIZE / sizeof(int) )
int 			UART1_IN_thread_stack [UART0_IN_THREAD_STACK_SIZE];
cyg_handle_t	UART1_IN_thread_handle;	
cyg_thread		UART1_IN_thread_obj;
cyg_mbox		UART1_IN_mbox; 


//// Just some playground to figure out the flash story
// #include "my_flash_101.h"



void send_sysex( unsigned char in_byte ){

	//Send controller number
	HAL_WRITE_UINT32( E7T_UART0_BASE + E7T_UART_TXBUF, in_byte );
	Player_wait( E7T_UART0_BASE );
}


// Assembles one byte from two given nibbles.
unsigned char sysex_to_byte( unsigned char high_nibble, unsigned char low_nibble ){
	return ( (high_nibble << 4) | low_nibble );
}


// Sends a byte as MIDI sysex dividing it into two nibbles, returning the checksum
unsigned char byte_to_sysex( unsigned char in_byte ){

	unsigned char high_nibble = ( 0xf0 & in_byte ) >> 4;
	unsigned char low_nibble  = 0x0f & in_byte;
	
	send_sysex( high_nibble );
	send_sysex( low_nibble	);

	return ( high_nibble ^ low_nibble );
}



void read_my_flash(){

	// Make sure not not hang up on port misconfig
	if ( OCTDEF_UART2_MODE != USB ) return;

	unsigned int err = 0;
	unsigned int* err_address = &err;
	unsigned int i=0;

//	 d_iag_printf( "----------- SYSEX IN-DUMP END -----------\n" );
	 diag_printf( "            code_keeper_index:%d\n", code_keeper_index );
//	 d_iag_printf( "            checksum received:%x computed:%x\n", 
//			midi_in_sysex[ midi_in_sysex_nibble ], checksum );

	 for ( i=0; i < CODE_KEEPER_SIZE; i++ ){
	
		diag_printf( "code_keeper[%d]:%d\n", i, code_keeper[i] );
		code_keeper[i] = 0;
	 }
	diag_printf( "code_keeper reset to 0\n" );


	// Read the bytes back from flash
	err = flash_read(		(void *) MY_EXECUTABLE_BASE, 
							(void *) code_keeper,
							CODE_KEEPER_SIZE, 
							(void **) err_address		);

	// Show the codekeeper original
	diag_printf( "\nre-read code_keeper:\n" );
	for (i=0; i < CODE_KEEPER_SIZE; i++){
		diag_printf( "code_keeper[%d]:%d\n", i, code_keeper[i] );
		code_keeper[i] = 0;
	}

	diag_printf( "code_keeper now all 0.\n" );
}




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
//	show_progress_bar( 1, MIR_GREEN );
	VIEWER_show_MIR();
		
 	// ERASE the flash before rewriting it
	cyg_mutex_lock( &mutex_FlashOperations );
	
	err = flash_erase(		(void *) MY_EXECUTABLE_BASE, 
							CODE_KEEPER_SIZE,
							(void**) err_address  		);

	cyg_mutex_unlock( &mutex_FlashOperations );
//	d_iag_printf( "->flash_erase'd the target space for executable (err:%d)\n", err );	

	// Progress bar part 2/3
	show_progress_bar( 3, MIR_RED );
	show_progress_bar( 4, MIR_RED );
	VIEWER_show_MIR();

	// WRITE the bytes to the flash
	cyg_mutex_lock( &mutex_FlashOperations );

    err = flash_program(	(void *) MY_EXECUTABLE_BASE, 
							(void *) code_keeper, 
							CODE_KEEPER_SIZE, 
							(void **) err_address    	);

	cyg_mutex_unlock( &mutex_FlashOperations );
//	d_iag_printf( "->flash_program'd the target space for executable (err:%d)\n", err );	

	// Last third in progress bar
	show_progress_bar( 3, MIR_RED );
	show_progress_bar( 4, MIR_RED );
	VIEWER_show_MIR();
	
 	// Wait - which is just an optical gimmic
 	for (i=0; i<30000; i++) {}


//	// Show the codekeeper original
//	d_iag_printf( "written code_keeper:\n" );
//	for (i=0; i<CODE_KEEPER_SIZE; i++){
//		d_iag_printf( "code_keeper[%d]:%d\n", i, code_keeper[i] );
//	}
//	
//	// Read the bytes back from flash
//	err = flash_read(		(void *) MY_EXECUTABLE_BASE, 
//							(void *) code_keeper,
//							CODE_KEEPER_SIZE, 
//							(void **) err_address		);
//
//	// Show the codekeeper original
//	d_iag_printf( "\nre-read code_keeper:\n" );
//	for (i=0; i<CODE_KEEPER_SIZE; i++){
//		d_iag_printf( "code_keeper[%d]:%d\n", i, code_keeper[i] );
//	}

	// Enable interrupts
	//	cyg_interrupt_enable();
	
	// Clear the code keeper buffer array
	for ( i=0; i < CODE_KEEPER_SIZE; i++ ){

		code_keeper[i] = 0;
	}

	// Return to DEVICE level - which is the natural context of this function
	zoom_level = zoomDEVICE;
}



// Reads the binary and outputs it as MIDI sysex stream
void my_executable_dump( unsigned int in_length ){

	unsigned int i = 0; // caution: will take on very large values
	unsigned char checksum = 0; // Simple XOR operation over all data bytes

	// d_iag_printf( " SYSEX OUT-DUMP starting - checksum:%x\n", checksum );

	// Copy the executable binary into the holder array
	memcpy( (void*) code_keeper, (void*) MY_EXECUTABLE_BASE, in_length );

	// SYSEX INIT SEQUENCE
	send_sysex( 0xF0 );

	// Send the manufacturer ID
	send_sysex( 0x00 );
	send_sysex( 0x20 );
	send_sysex( 0x64 );

	// Send the manufacturer message type
	send_sysex( 0x00 );

	// Send the contents of the array
	for( i=0; i < in_length; i++ ){

		// Compute the running checksum
		checksum ^= byte_to_sysex( code_keeper[i] );
	}

	// Send the checksum
	send_sysex( checksum );

	// Send sysex terminator
	send_sysex( 0xF7 );

	// d_iag_printf( " SYSEX OUT-DUMP complete - checksum:%x\n", checksum );
}



// Recall the flash contents into the running sequencer
void Octopus_recall_flash(){

	unsigned int i = 0;

	// Read the flash contents into the memory image structure
	Flashbuffer_total_read_from_flash();		

	// Copy the Page data from the memory image
	for (i=0; i<MAX_NROF_PAGES; i++){
		
		Page_import_from_flashbuffer( 
			&(memory_image.Page_flashbuffer_repository[i]), 
			&Page_repository[i]               );
	}		

	// Copy the Grid data from the memory image
	Grid_import_from_flashbuffer();

	// Clear the flashbuffer - i.e. leave it empty
	Page_flashbuffer_repository_init();

	// Clear the grid part of the flashbuffer
	Grid_flashbuffer_init();
	
	// Enter the page mode - copied from pressing ESC!
	VIEWER_page = &Page_repository[ GRID_CURSOR];
	VIEWER_page->keyMode = KMOD_PAGE;		
	VIEWER_page->trackSelection = OFF;
	VIEWER_page->trackAttributeSelection = OFF;
	zoom_level = zoomPAGE;
	
	// Release the MIDI SYSEX state
	MIDI_RECEIVE_SYSEX = FALSE;
	MIDI_SYSEX_HEADER_SEEN = FALSE;
	code_keeper_index = 0;
}



// Initialize the Octopus hardware - called once upon startup
void init_hardware() {

	unsigned int i = 0;

	// my_flash_101( 10 );
	
	//	HAL_READ_UINT32( E7T_INTMOD, my_current_intmod );
	//	d_iag_printf( "main(): initial INTMOD:%x\n", my_current_intmod );
	//	HAL_READ_UINT32( E7T_INTPRI0, my_current_intmod_array[0] );
	//	HAL_READ_UINT32( E7T_INTPRI1, my_current_intmod_array[1] );
	//	HAL_READ_UINT32( E7T_INTPRI2, my_current_intmod_array[2] );
	//	HAL_READ_UINT32( E7T_INTPRI3, my_current_intmod_array[3] );
	//	HAL_READ_UINT32( E7T_INTPRI4, my_current_intmod_array[4] );
	//	HAL_READ_UINT32( E7T_INTPRI5, my_current_intmod_array[5] );
	//	for( i=0; i<6; i++ ){
	//		d_iag_printf("        INTMOD%d: %x\n", i, my_current_intmod_array[i] );
	//		my_print_bits32( my_current_intmod_array[i] );
	//	}

	// HAL_READ_UINT32( E7T_IOPDATA, value );
	//	printf("\tE7T_IOPDATA:%d\n", value);	
	//	my_print_bits32(value);
	
	// Fire up the memory initialization sequence
	// moved now to the bottom of the main function
	//	Octopus_memory_init();
	
	//	d_iag_printf("main(): Memory init OK\n");
	//	d_iag_printf("   Sizeof(char)   :%d\n", (int) sizeof(char));
	//	d_iag_printf("   Sizeof(char*)  :%d\n", (int) sizeof(char*));
	//
	//	d_iag_printf("   Sizeof(int)    :%d\n", (int) sizeof(int));
	//	d_iag_printf("   Sizeof(int*)   :%d\n", (int) sizeof(int*));
	//
	//	d_iag_printf("   Sizeof(STEP)   :%d\n", (int) sizeof(Stepstruct));
	//	d_iag_printf("   Sizeof(STEP*)  :%d\n", (int) sizeof(Stepstruct*));
	//
	//	d_iag_printf("   Sizeof(TRACK)  :%d\n", (int) sizeof(Trackstruct));
	//	d_iag_printf("   Sizeof(TRACK*) :%d\n", (int) sizeof(Trackstruct*));
	//
	//	d_iag_printf("   Sizeof(PAGE)   :%d\n", (int) sizeof(Pagestruct));
	//	d_iag_printf("   Sizeof(PAGE*)  :%d\n", (int) sizeof(Pagestruct*));
	//	d_iag_printf("   Pagesize total :%d\n", (int) (
	//			sizeof( Pagestruct  ) + 		sizeof( Pagestruct* ) +
	//			sizeof( Trackstruct )*10 +		sizeof( Trackstruct* )*10 +
	//			sizeof( Stepstruct  )*160 +   	sizeof( Stepstruct* )*160)    
	//													);
	//		
	//	d_iag_printf("   Sizeof(FLASHBUFFER):%d\n", (int) sizeof( Page_flashbuffer_struct ));	
	// 	d_iag_printf("   Sizeof(FB Repository):%d\n", (int) sizeof( Page_flashbuffer_repository ));

	//
	// PLATFORM SPECIFIC CODE SECTION
	//

	//
	// UART(0) for MIDI Operation writing the Divisor value
	//
	HAL_WRITE_UINT32(	E7T_UART0_BASE + E7T_UART_BRDIV, 
					 	MIDIBAUDCOUNTER 
					 	);			
	// Line control: b00000011: IR no, Int clock, No parity, 1 stopbit, 8bit words
	HAL_WRITE_UINT32(	E7T_UART0_BASE + E7T_UART_LCON, 
					 	E7T_UART_LCON_8_DBITS +
					 	E7T_UART_LCON_NO_PARITY +
					 	E7T_UART_LCON_1_SBITS 
					 	);
	// Control register: Interrupt request
	HAL_WRITE_UINT32(	E7T_UART0_BASE + E7T_UART_CON, 
					 	E7T_UART_CON_TXM_INT
					 |	E7T_UART_CON_RXM_INT
					 );
	// d_iag_printf("UART(0) CON_TXM init:         OK\n");

	//
	// UART(1) for MIDI Operation writing the Divisor value
	// 
	if ( OCTDEF_UART2_MODE == MIDI ){	

		// Do not do this when in USB mode, or things get confused
		// ..because this is where we are using printfs and stuff
		HAL_WRITE_UINT32(	E7T_UART1_BASE + E7T_UART_BRDIV, 
						 	MIDIBAUDCOUNTER 
						 	);			
		// Line control: b00000011: IR no, Int clock, No parity, 1 stopbit, 8bit words
		HAL_WRITE_UINT32(	E7T_UART1_BASE + E7T_UART_LCON, 
						 	E7T_UART_LCON_8_DBITS +
						 	E7T_UART_LCON_NO_PARITY +
						 	E7T_UART_LCON_1_SBITS 
						 	);
		// Control register: Interrupt request
		HAL_WRITE_UINT32(	E7T_UART1_BASE + E7T_UART_CON, 
							 	E7T_UART_CON_TXM_INT
							 |	E7T_UART_CON_RXM_INT
							 );
	}

	// d_iag_printf("UART(1) CON_TXM init:         OK\n");

	//	HAL_READ_UINT32(E7T_UART0_BASE+E7T_UART_BRDIV, value);
	//	printf("\tUART_BRDIV: %d\n", value);	
	//	my_print_bits(value);
	//
	//	HAL_READ_UINT32(E7T_UART0_BASE+E7T_UART_LCON, value);
	//	printf("\tUART_LCON: %d\n", value);	
	//	my_print_bits(value);
	//
	//	HAL_READ_UINT32(E7T_UART0_BASE+E7T_UART_CON, value);
	//	printf("\tUART_CON: %d\n", value);	
	//	my_print_bits(value);
	//
	//	HAL_READ_UINT32(E7T_UART0_BASE+E7T_UART_STAT, value);
	//	HAL_READ_UINT32(0x07FFD008, value1);
	//	
	//	printf("\tUART_STAT: %d\n", value);	
	//	my_print_bits(value);
	//	printf("\tUART_STAT1: %d\n", value1);	
	//	my_print_bits(value1);


	// Initialize IO Ports for all write
	HAL_WRITE_UINT32( E7T_IOPMOD, 0x3f8ff );

	// Enable TIMER1 via the TMOD register
	// ..without messing up the TIMER0 setup of eCos
	HAL_READ_UINT32	( E7T_TMOD, i );
  	HAL_WRITE_UINT32( E7T_TMOD, i | E7T_TMOD_TE1 | E7T_TMOD_TMD1 ); 

	// IO_ENABLE_INT0 and 1 | IO_ACTIVE_HIGH_INT0 and 1 | IO_RISING_EDGE_INT0 and 1	
	//
	// HAL_WRITE_UINT32(E7T_IOPCON, (1<<9 | 1<<8 | 1<<5) | (1<<4 | 1<<3 | 1<<0)); 		
	HAL_WRITE_UINT32( E7T_IOPCON, OCTOPUS_IOPCON_KEYINTR_RISING 
	//							| OCTOPUS_IOPCON_KEYINTR_FALLING
								| OCTOPUS_IOPCON_ROTINTR_RISING 
								);

	// In this section: Refill the Shift registers, in order to trigger an interrupt.
	for (i=0; i<25; i++) {	
		HAL_WRITE_UINT32( E7T_IOPDATA , IMPULSE_HIGH | CLOCK_KEY_LOW | OCTDEF_UART2_MODE);	
 		HAL_WRITE_UINT32( E7T_IOPDATA , IMPULSE_HIGH | CLOCK_KEY_HIGH | OCTDEF_UART2_MODE);
	}

	// d_iag_printf("start(): Shift registers refilled for interrupt trigger\n");
	
	// Reset the rotary counters
	HAL_WRITE_UINT32(E7T_IOPDATA , RESET_ROTARY_COUNTERS | OCTDEF_UART2_MODE);
	// Rewrite the data port to avoid confusion
	HAL_WRITE_UINT32(E7T_IOPDATA , 0 | OCTDEF_UART2_MODE);
}



// Interrupt init routine, setting up the interrupt infrastructure upon start
void init_interrupts(){
		
	// TIMER INTERRUPT - Unmask interrupt for Timer
	// d_iag_printf("start(): interrupt tmr1 unmasked\n");	

	// ROTARY INTERRUPT - Unmask interrupt for Rotary encoder
	// d_iag_printf("start(): interrupt ext1 unmasked\n");

	// UART0 (MIDI 1) - Unmask interrupt for UART0_IN
	// d_iag_printf("start(): interrupt UART0-IN unmasked\n");

	// UART1 (MIDI 2) - Unmask interrupt for UART1_IN
	// d_iag_printf("start(): interrupt UART0-IN unmasked\n");


	// TIMER INTERRUPT - Timer1 with prio 1 
	cyg_interrupt_unmask(	CYGNUM_HAL_INTERRUPT_TIMER1);
  	cyg_interrupt_create(	CYGNUM_HAL_INTERRUPT_TIMER1,
		       				1, 
		       				0, 
		       				Timer1_ISR, 
		       				Timer1_DSR, 
		       				&handle_timer1_isr, 
		       				&isr);
  	cyg_interrupt_attach(	 handle_timer1_isr );
  	// d_iag_printf("start(): interrupt timer1 created\n");
  
  
	// ROTARY INTERRUPT - rotary on Ext1 with prio 20
	cyg_interrupt_unmask(	CYGNUM_HAL_INTERRUPT_EXT1);
  	cyg_interrupt_create(	CYGNUM_HAL_INTERRUPT_EXT1,
		       				20,
							20,
		       				Ext1_ISR, 
		       				Ext1_DSR, 
		       				&handle_ext1_isr, 
		       				&isr3);
  	cyg_interrupt_attach(	 handle_ext1_isr );
  	// d_iag_printf("start(): interrupt ext1 created\n");
  	 	
 
 	// UART0 IN INTERRUPT - MIDI IN 1 
	cyg_interrupt_unmask(	CYGNUM_HAL_INTERRUPT_UART0_RX);
  	cyg_interrupt_create(	CYGNUM_HAL_INTERRUPT_UART0_RX,
		       				10, 
							20,
		       				UART0_IN_ISR, 
		       				UART0_IN_DSR, 
		       				&handle_UART0_IN_isr, 
		       				&isr5);
  	cyg_interrupt_attach(	 handle_UART0_IN_isr );
  	// d_iag_printf("start(): interrupt ext2 created\n");


 	// UART1 IN INTERRUPT - MIDI IN 2 
	cyg_interrupt_unmask(	CYGNUM_HAL_INTERRUPT_UART1_RX);
  	cyg_interrupt_create(	CYGNUM_HAL_INTERRUPT_UART1_RX,
		       				50, 
							20,
		       				UART1_IN_ISR, 
		       				UART1_IN_DSR, 
		       				&handle_UART1_IN_isr, 
		       				&isr6);
  	cyg_interrupt_attach(	 handle_UART1_IN_isr);


	// Set the Timer refill for default tempo;
	TIMER_REFILL_update();	

	// Enable interrupts
  	cyg_interrupt_enable();
	// d_iag_printf("main(): interrupts enabled\n");
}


// Setup the mailboxes used by the threads
void init_mailboxes(){

	// Key execution message box
	cyg_mbox_create(
		&keyNdx_mbox_handle,
		&keyNdx_mbox );

	// Rot execution message box
	cyg_mbox_create(
		&rotNdx_mbox_handle,
		&rotNdx_mbox );

	// MIDI 1 message boxes
	cyg_mbox_create(
		&UART0_OUT_mbox_handle,
		&UART0_OUT_mbox );

	cyg_mbox_create(
		&UART0_IN_mbox_handle,
		&UART0_IN_mbox );

	// MIDI 2 message boxes
	cyg_mbox_create(
		&UART1_OUT_mbox_handle,
		&UART1_OUT_mbox );

	cyg_mbox_create(
		&UART1_IN_mbox_handle,
		&UART1_IN_mbox );

	// d_iag_printf( "main(): message boxes created\n" );
}


// Set up the threads as used by the system
void init_threads(){

	// 
	// THREADS CREATION SECTION
	//

	// TIMER - MASTER CLOCK
	cyg_thread_create(
		1,
		timerInterrupt_thread,
		0,
		"Timer Interrupt Thread",
		&timerInterrupt_thread_stack,
		 TIMERINTERRUPT_THREAD_STACK_SIZE,
		&timerInterrupt_thread_handle,
		&timerInterrupt_thread_obj );
	// Let the thread run when the scheduler executes
	cyg_thread_resume (timerInterrupt_thread_handle);


	// MIDI OUT 1 thread
	cyg_thread_create(
		5,
		 UART0_OUT_thread,
		0,
		"UART0_OUT thread",
		&UART0_OUT_thread_stack,
		 UART0_OUT_THREAD_STACK_SIZE,
		&UART0_OUT_thread_handle,
		&UART0_OUT_thread_obj );
	cyg_thread_resume( UART0_OUT_thread_handle);		


	// MIDI OUT 2 thread
	cyg_thread_create(
		5,
		 UART1_OUT_thread,
		0,
		"UART1_OUT thread",
		&UART1_OUT_thread_stack,
		 UART1_OUT_THREAD_STACK_SIZE,
		&UART1_OUT_thread_handle,
		&UART1_OUT_thread_obj );
	cyg_thread_resume( UART1_OUT_thread_handle );		


	// MIDI IN 1 thread
	cyg_thread_create(
		7,
		UART0_IN_thread,
		0,
		"UART0_IN thread",
		&UART0_IN_thread_stack,
		 UART0_IN_THREAD_STACK_SIZE,
		&UART0_IN_thread_handle,
		&UART0_IN_thread_obj );
	cyg_thread_resume( UART0_IN_thread_handle );


	// MIDI IN 2 thread
	cyg_thread_create(
		8,
		UART1_IN_thread,
		0,
		"UART1_IN thread",
		&UART1_IN_thread_stack,
		 UART1_IN_THREAD_STACK_SIZE,
		&UART1_IN_thread_handle,
		&UART1_IN_thread_obj );
	cyg_thread_resume( UART1_IN_thread_handle );


	// Scan keys thread
	cyg_thread_create(
		12,
		readKeys_thread,
		0,
		"Scan Keys Thread",
		&scanKeys_thread_stack,
		 SCANKEYS_THREAD_STACK_SIZE,
		&scanKeys_thread_handle,
		&scanKeys_thread_obj );
	// Let the thread run when the scheduler executes
	cyg_thread_resume (scanKeys_thread_handle);


	// ROT execution thread
	cyg_thread_create(
		5,   //13
		rotExecute_thread,
		0,
		"Rot Execute Thread",
		&rotExecute_thread_stack,
		 ROTEXECUTE_THREAD_STACK_SIZE,
		&rotExecute_thread_handle,
		&rotExecute_thread_obj );
	// Let the thread run when the scheduler executes
	cyg_thread_resume (rotExecute_thread_handle);
	// d_iag_printf( "main(): Key Execute thread created\n" );


	// KEY execution thread
	cyg_thread_create(
		4,   //4
		keyExecute_thread,
		0,
		"Key Execute Thread",
		&keyExecute_thread_stack,
		 KEYEXECUTE_THREAD_STACK_SIZE,
		&keyExecute_thread_handle,
		&keyExecute_thread_obj );
	// Let the thread run when the scheduler executes
	cyg_thread_resume (keyExecute_thread_handle);
	// d_iag_printf( "main(): Key Execute thread created\n" );


	// Page refresh thread
	cyg_thread_create(
		5,
		showPage_thread,
		0,
		"Show Page Thread",
		&showPage_thread_stack,
		 SHOWPAGE_THREAD_STACK_SIZE,
		&showPage_thread_handle,
		&showPage_thread_obj );
	// Let the thread run when the scheduler executes	
	cyg_thread_resume (showPage_thread_handle);
	

	// Flashops thread
	cyg_thread_create(
		20,
		flashOps_thread,
		0,
		"Flash Ops Thread",
		&flashOps_thread_stack,
		 FLASHOPS_THREAD_STACK_SIZE,
		&flashOps_thread_handle,
		&flashOps_thread_obj );
	// Let the thread run when the scheduler executes	
	cyg_thread_resume (flashOps_thread_handle);
		
	// d_iag_printf( "main(): threads created\n" );
}


// Set up the alarms used by the system
void init_alarms(){

	// ALARM SETUP PREPARE
	sys_clk = cyg_real_time_clock();
	cyg_clock_to_counter( sys_clk, &counter_hdl );

	// ALARM: TIMEOUT	
	cyg_alarm_create(		counter_hdl,
							alarm_handler,
							(cyg_addrword_t)&alarm_index,
							&alarm_hdl,
							&alarm_obj );
	cyg_alarm_initialize(	alarm_hdl, 
							cyg_current_time() + 100,
							100 );	
	// d_iag_printf( "Timeout Alarm setup done\n" );

	// ALARM: DOUBLECLICK
	cyg_alarm_create(		counter_hdl,
							doubleClickAlarm_handler,
							(cyg_addrword_t)&doubleClickAlarm_index,
							&doubleClickAlarm_hdl,
							&doubleClickAlarm_obj );
	// Initialization done at potential double click sources

	// d_iag_printf( "DoubleClick Alarm setup done\n" );


	// ALARM: Key-TV
	cyg_alarm_create(		counter_hdl,
							alarmTV_handler,
							(cyg_addrword_t)&alarmTV_index,
							&alarmTV_hdl,
							&alarmTV_obj );	
							
	cyg_alarm_initialize(	alarmTV_hdl, 
							cyg_current_time() + TV_FREQUENCY,
							TV_FREQUENCY );														
	// d_iag_printf( "TV Alarm setup done\n" );



	// ALARM: ROTARY QUICKTURN
	cyg_alarm_create(		counter_hdl,
							quickturnAlarm_handler,
							(cyg_addrword_t)&quickturnAlarm_index,
							&quickturnAlarm_hdl,
							&quickturnAlarm_obj );	
	// d_iag_printf( "QuickTurn Alarm setup done\n" );



	// ALARM: MIDI_overload display
	cyg_alarm_create(		counter_hdl,
							MIDI1_overloadAlarm_handler,
							(cyg_addrword_t)&MIDI1_overloadAlarm_index,
							&MIDI1_overloadAlarm_hdl,
							&MIDI1_overloadAlarm_obj );	

	cyg_alarm_initialize(	MIDI1_overloadAlarm_hdl, 
							cyg_current_time() + MIDI_RED_INTERVAL,
							0 );


	cyg_alarm_create(		counter_hdl,
							MIDI2_overloadAlarm_handler,
							(cyg_addrword_t)&MIDI2_overloadAlarm_index,
							&MIDI2_overloadAlarm_hdl,
							&MIDI2_overloadAlarm_obj );	

	cyg_alarm_initialize(	MIDI2_overloadAlarm_hdl, 
							cyg_current_time() + MIDI_RED_INTERVAL,
							0 );


	// ALARM: CPU_overload display
	cyg_alarm_create(		counter_hdl,
							CPU_overloadAlarm_handler,
							(cyg_addrword_t)&CPU_overloadAlarm_index,
							&CPU_overloadAlarm_hdl,
							&CPU_overloadAlarm_obj );	

	cyg_alarm_initialize(	CPU_overloadAlarm_hdl, 
							cyg_current_time() + CPU_RED_INTERVAL,
							0 );

}


//
// MAIN ENTRY POINT INTO THE EXECUTION
//
void cyg_user_start(void) {

// 	unsigned int i=0;
//	unsigned int value=0;
	unsigned int pvalue=0;
	
//	unsigned char unsigned_temp=129;
//	signed char signed_temp = 0;	
//	signed char signed_temp2 = 200;
//	signed char signed_temp3 = 201;
//
//	d_iag_printf( "before: unsigned:%d signed:%d\n", unsigned_temp, signed_temp );
//	signed_temp = (signed char) unsigned_temp;
//	d_iag_printf( "after:  unsigned:%d signed:%d 2:%d 3:%d\n", 
//		unsigned_temp, signed_temp, signed_temp2, signed_temp3 );
//	my_print_bits32( unsigned_temp );
//	my_print_bits32( signed_temp  );
//	my_print_bits32( signed_temp2 );
//	my_print_bits32( signed_temp3 );

//	for (i=0; i<300; i++){
//
//		if (i%10 == 0) d_iag_printf( "\n" );
//		d_iag_printf("ndx:%d signed:%d unsigned:%d\n", 
//			i, (signed char) i, (unsigned char) i );
//	}
	// unsigned int my_current_intmod=0;
	// unsigned int my_current_intmod_array[6];


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
	// d_iag_printf("main(): rseed1: %d\n", pvalue);
	//	srand( pvalue );
	//	srand( cyg_current_time() );
	
	// Data storage / memory initialization sequence
	Octopus_memory_init();


	// Check whether the PROGRAM button is pressed and modify the number
	// of pages to be initalized
	// d_iag_printf( "checking for pressed keys\n" );
	

	// First key scan - need to fill the scan array, so we can proceed
	scanKeys();

	// Check whether there is a pressed key
//if ( TRUE ){ // used only for development purposes - zoom DEVICE
	if ( scanKeys() == TRUE ) {
	
		// d_iag_printf( "pressed key on startup detected\n" );

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
//else if ( TRUE ){ // used only for development purposes - zoom DEVICE

			// Enter the device maintenance mode
			zoom_level = zoomDEVICE;
			FLASH_LOAD_FLAG = FALSE;
			
			// Code keeper array now uses the memory reserved for the step repository.
			code_keeper = (unsigned char*) Step_repository;
		}
	}

	// No key was pressed on power-on
	else {
		// d_iag_printf( "no key pressed\n" );
	}

	// Finally, recall data stored in flash - includes the randomizer seed
	if ( FLASH_LOAD_FLAG == TRUE ){

		Octopus_recall_flash();
		POWERON_LOADED = TRUE;
	}

	// Point VIEWER_page to grid cursor
	VIEWER_page = &Page_repository[ GRID_CURSOR ];

	// diag_printf( "GRID_CURSOR:%d\n", GRID_CURSOR );

	// Start with a fresh display
	Page_requestRefresh();	
}


