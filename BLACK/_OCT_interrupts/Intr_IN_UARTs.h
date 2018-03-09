/* OCTOPUS
 * 
 * Interrupt handling of EXT2 - KEY Falling
 */


// unsigned int sysex_byte_count = 0; // exported to global variables
unsigned int sysex_header = 0;
unsigned char in_stream_checksum = 0;


#include "Intr_IN_UARTs_functions.h"



// MIDI1 IN ISR 
cyg_uint32 UART0_IN_ISR( cyg_vector_t vector, cyg_addrword_t data ){
	// Prevent this interrupt from occuring until DSR completes
	cyg_interrupt_mask (vector);

	// Acknowledge interrupt so other interrupts can be serviced
	cyg_interrupt_acknowledge(vector);
	
	// Proceed to executing the DSR
	return CYG_ISR_CALL_DSR;
}


// MIDI2 IN ISR 
cyg_uint32 UART1_IN_ISR( cyg_vector_t vector, cyg_addrword_t data ){
	// Prevent this interrupt from occuring until DSR completes
	cyg_interrupt_mask (vector);

	// Acknowledge interrupt so other interrupts can be serviced
	cyg_interrupt_acknowledge(vector);

	// Proceed to executing the DSR
	return CYG_ISR_CALL_DSR;
}



// MIDI1 IN DSR 
void UART0_IN_DSR( cyg_vector_t vector, cyg_ucount32 count, cyg_addrword_t data ){
	
	unsigned int 	midi_byte = 0;
	
	HAL_READ_UINT32( E7T_UART0_BASE + E7T_UART_RXBUF, midi_byte);

	// Place the MIDI message in the mailbox, for the interpreter
	cyg_mbox_tryput( UART0_IN_mbox_handle, (void*) midi_byte );
			
	// Allow the interrupt to occur again
	cyg_interrupt_unmask(vector);
}



// MIDI2 IN DSR 
void UART1_IN_DSR( cyg_vector_t vector, cyg_ucount32 count, cyg_addrword_t data ){
	
	unsigned int 	midi_byte = 0;
	
	HAL_READ_UINT32( E7T_UART1_BASE + E7T_UART_RXBUF, midi_byte);

	// Place the MIDI message in the mailbox, for the interpreter
	cyg_mbox_tryput( UART1_IN_mbox_handle, (void*) midi_byte );
			
	// Allow the interrupt to occur again
	cyg_interrupt_unmask(vector);
}




// Monitors the UARTRX0 mailbox
void UART0_IN_thread( cyg_addrword_t index ){
	
	unsigned int* midi_byte	= 0;
	unsigned char UART_ndx 	= 0;		// This is UART0
		
	// Wait for messages (i.e. MIDI bytes) to arrive
	while (1){
		
		// Read in the next MIDI byte
		midi_byte = cyg_mbox_get( UART0_IN_mbox_handle );			

		// d_iag_printf( "UART0_IN rec %x\n", (unsigned int) midi_byte );

		// differentiate between status and data bytes. Pass the UART index along
		switch( ((unsigned char) (unsigned int) midi_byte) & 0x80 ){

			case MIDI_STATUS_BYTE:  // 0x80 i.e. highest bit is set

				read_MIDI_STATUS_BYTE( (unsigned char) (unsigned int) midi_byte, UART_ndx );				
				break;
				
			case MIDI_DATA_BYTE:

				read_MIDI_DATA_BYTE(  (unsigned char) (unsigned int) midi_byte, UART_ndx );
				break;
		}
	}
}



// Monitors the UARTRX0 mailbox
void UART1_IN_thread( cyg_addrword_t index ){
	
	unsigned int* midi_byte	= 0;
	unsigned char UART_ndx 	= 1;		// This is UART1
		
	// Wait for messages (i.e. MIDI bytes) to arrive
	while (1){

		// Read in the next MIDI byte
		midi_byte = cyg_mbox_get( UART1_IN_mbox_handle );

		// differentiate between status and data bytes. Pass the UART index along
		switch( ((unsigned char) (unsigned int) midi_byte) & 0x80 ){

			case MIDI_STATUS_BYTE:	// 0x80 i.e. highest bit is set

				read_MIDI_STATUS_BYTE( (unsigned char) (unsigned int) midi_byte, UART_ndx );				
				break;
				
			case MIDI_DATA_BYTE:

				read_MIDI_DATA_BYTE(  (unsigned char) (unsigned int) midi_byte, UART_ndx );
				break;
		}
	}
}



