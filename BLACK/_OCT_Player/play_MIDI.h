
//
// MIDI OUT 1 thread
//
void UART0_OUT_thread( cyg_addrword_t index ) {

	// UART0 Base address assignment
	unsigned int E7T_UARTx_BASE 		= E7T_UART0_BASE;

	// UART0 mailbox assignment
	cyg_handle_t UARTx_OUT_mbox_handle 	= UART0_OUT_mbox_handle;
	
	// static unsigned char running_status_byte_UART0;
	unsigned char* running_status_byte = &running_status_byte_OUT_UART[0];
	
	// MIDI send logic 
	#include "play_UARTx_OUT.h"
}


//
// MIDI OUT 2 thread
//
void UART1_OUT_thread( cyg_addrword_t index ) {

	// UART1 Base address assignment
	unsigned int E7T_UARTx_BASE 		= E7T_UART1_BASE;

	// UART0 mailbox assignment
	cyg_handle_t UARTx_OUT_mbox_handle 	= UART1_OUT_mbox_handle;
	
	// static unsigned char running_status_byte_UART1;
	unsigned char* running_status_byte = &running_status_byte_OUT_UART[1];

	// MIDI send logic 
	#include "play_UARTx_OUT.h"
}


// MIDI SEND FUNCTION: assembles an internal MIDI message and puts it on send queue
void MIDI_send( 	unsigned char type, 
					unsigned char val0, 
					unsigned char val1, 
					unsigned char val2 ){
	
	// Initialize to something other than 0, where it can't do damage
	unsigned char i = 200;	
	unsigned char j = 200;
	
	// Assembly of the entire MIDI message
	unsigned int outMsg = 0;
	
	// Assume target_UART to be 0, may change as we go.
	unsigned char target_UART = 0;

	// UART selection depends on the type of MIDI message. Note does differently
	// ..than MIDI CLOCK for example.	
	switch( type ){

		case MIDI_NOTE:
		case MIDI_PGMCH:
		case MIDI_CC:
			// Val0 here indicates the channel.. so
			// ..select the correct UART and scale the channel absolute value accordingly
			if ( val0 <= 16 ){
				
				target_UART = 0;
			}
			else{
				
				target_UART = 1;
				val0 		= val0 - 16; 
			}
			break;

		case MIDI_CLOCK:
			// Val0 here indicates the message
			// Send the clock on both UARTs
			target_UART = 2;

			break;			
	}


	// Assemble the message according to the rules established
	outMsg = (		(type 		<< 24)		// type - as defined in defs_general.h
				| 	(val0 		<< 16)		// channel or mtc command
				|	(val1		<<  8)		// controller / value / pgmchg index
				|	(val2		<< 	0)	);	// value

	 if (		(type == MIDI_NOTE) 
	 		&& 	(val2 != OFF)
	 	) {

		// d_iag_printf( "   MIDI_NOTE underway:0x%x\n", outMsg );
	 }
				

	// Send message to its appropriate mailbox
	switch( target_UART ){
	
			case 0:
				i = cyg_mbox_tryput( UART0_OUT_mbox_handle, (void*) outMsg );
				// d_iag_printf( "MIDI 1 out\n" );
				break;

			case 1:
				// MIDI OUT 2 message
				j = cyg_mbox_tryput( UART1_OUT_mbox_handle, (void*) outMsg );
				// d_iag_printf( "MIDI 2 out\n" );
				break;

			case 2:
				// MIDI OUT 1 and 2 message
				i = cyg_mbox_tryput( UART0_OUT_mbox_handle, (void*) outMsg );
				i |= cyg_mbox_tryput( UART1_OUT_mbox_handle, (void*) outMsg );
				break;				
	}
		
	// Signal the full mailbox accordingly. This is used for load bounding.
	// It shows that the machine is under load, and drops MIDI messages
	if (	( target_UART == 0 )	
		&&	( i != 1 )
		){
	
		MIDI_LOAD_MONITOR1 = MIR_RED;
		
		// d_iag_printf( "MIDI_send(): returned:%d\n", i );
		// Set the alarm to make it GREEN again
		cyg_alarm_initialize(	MIDI1_overloadAlarm_hdl, 
								cyg_current_time() + MIDI_RED_INTERVAL,
								0 );			
	}

	if (	( target_UART == 1 )
		&&	( j != 1 )
		){
	
		MIDI_LOAD_MONITOR2 = MIR_RED;
		
		// d_iag_printf( "MIDI_send(): returned:%d\n", i );
		// Set the alarm to make it GREEN again
		cyg_alarm_initialize(	MIDI2_overloadAlarm_hdl, 
								cyg_current_time() + MIDI_RED_INTERVAL,
								0 );			
	}


}


// Plays the event on the MIDI port and pushes the consumed event on the stack
void MIDI_send_event( MIDIeventstruct* target_event ){

	if ( target_event == NULL ) return;

	MIDI_send( 	target_event->type, 
				target_event->val0,
				target_event->val1,
				target_event->val2   );
				
	MIDI_stack_push( target_event );
}

