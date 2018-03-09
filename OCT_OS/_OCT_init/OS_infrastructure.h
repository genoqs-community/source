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






// Universal thread stack size definition
#define	MY_THREAD_STACK_SIZE		12288 // 16384 // 4096

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
int scan_keys_thread_stack [SCANKEYS_THREAD_STACK_SIZE];
cyg_handle_t	scan_keys_thread_handle;
cyg_thread		scan_keys_thread_obj;

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




// Initialize the Octopus hardware - called once upon startup
void init_hardware() {

	unsigned int i = 0;

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

//		d_iag_printf("main(): Memory init OK\n");
//		d_iag_printf("   Sizeof(char)   :%d\n", (int) sizeof(char));
//		d_iag_printf("   Sizeof(char*)  :%d\n", (int) sizeof(char*));
//
//		d_iag_printf("   Sizeof(int)    :%d\n", (int) sizeof(int));
//		d_iag_printf("   Sizeof(int*)   :%d\n", (int) sizeof(int*));
//
//		d_iag_printf("   Sizeof(STEP)   :%d\n", (int) sizeof(Stepstruct));
//		d_iag_printf("   Sizeof(STEP*)  :%d\n", (int) sizeof(Stepstruct*));
//
//		d_iag_printf("   Sizeof(TRACK)  :%d\n", (int) sizeof(Trackstruct));
//		d_iag_printf("   Sizeof(TRACK*) :%d\n", (int) sizeof(Trackstruct*));
//
//		d_iag_printf("   Sizeof(PAGE)   :%d\n", (int) sizeof(Pagestruct));
//		d_iag_printf("   Sizeof(PAGE*)  :%d\n", (int) sizeof(Pagestruct*));
//		d_iag_printf("   Pagesize total :%d\n", (int) (
//				sizeof( Pagestruct  ) + 		sizeof( Pagestruct* ) +
//				sizeof( Trackstruct )*10 +		sizeof( Trackstruct* )*10 +
//				sizeof( Stepstruct  )*160 +   	sizeof( Stepstruct* )*160)
//														);
//
//	 	d_iag_printf("   Sizeof(Memory_image_struct):%d\n", (int) sizeof( Memory_image_struct ));


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
	G_TIMER_REFILL_update();

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
		&scan_keys_thread_stack,
		 SCANKEYS_THREAD_STACK_SIZE,
		&scan_keys_thread_handle,
		&scan_keys_thread_obj );
	// Let the thread run when the scheduler executes
	cyg_thread_resume (scan_keys_thread_handle);


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


	// ALARM: Page refresh call
#ifdef CONSTANT_BLINK
	cyg_alarm_create(		counter_hdl,
							page_refreshAlarm_handler,
							(cyg_addrword_t)&page_refreshAlarm_index,
							&page_refreshAlarm_hdl,
							&page_refreshAlarm_obj );

	cyg_alarm_initialize(	page_refreshAlarm_hdl,
							cyg_current_time() + PAGE_REFRESH_INTERVAL + 2,
							PAGE_REFRESH_INTERVAL + 2 );
#endif

}


void init_muxes( void )
{
	// Mutex set for the MIDI output buffer structure
	cyg_mutex_init( &mutex_midi_output_stack );
	cyg_mutex_init( &mutex_midi_output_queue );

	// Mutex for the data pipe
	cyg_mutex_init ( &mutex_data_port );

	// Mutex for print operations
	cyg_mutex_init ( &mutex_play_TTC );

	// Mutex for the page refresh request variable
	cyg_mutex_init ( &mutex_pageRefreshReq );

	// Mutex for write acccess to page data
	cyg_mutex_init ( &mutex_PageWrite );

	// Mutex for flash operations
	cyg_mutex_init ( &mutex_FlashOperations );

	// Mutex for read/write ops of sequencer data
	cyg_mutex_init ( &mutex_sequencer_data );

	// Set of mutexes to protect output MIDI queue and stack.
	cyg_mutex_init ( &mutex_midi_output_stack );
	cyg_mutex_init ( &mutex_midi_output_queue );
}


///// Check and show bad threads if stack overflow.
//void CheckAndShowBadThreads( unsigned char bShowNormalInfo ){
//
//	cyg_handle_t thread = 0;
//	cyg_uint16 id 		= 0;
//
//	if ( bShowNormalInfo ){
//		d_iag_printf("Threads running:\n");
//	}
//
//	while( cyg_thread_get_next( &thread, &id ) ){
//
//		cyg_thread_info info;
//
//		if( !cyg_thread_get_info( thread, id, &info ) ) break;
//
//		// Check statck overflow with some reserve space
//		if ((info.stack_used + 512) > info.stack_size){
//
//			d_iag_printf("\nSTACK OVERFLOW: ID=%02d \"%26s\" Priority=%02d Stack=%4u/%4u\n",
//				info.id, (info.name ? info.name : "----"),
//				info.set_pri, info.stack_used, info.stack_size );
//		}
//		else if ( bShowNormalInfo ){
//
//			d_iag_printf(" ID=%02d \"%26s\" Priority=%02d Stack=%4u/%4u\n",
//			info.id, (info.name ? info.name : "----"),
//			info.set_pri, info.stack_used, info.stack_size );
//		}
//	}
//}
