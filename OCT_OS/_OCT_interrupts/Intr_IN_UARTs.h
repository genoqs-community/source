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
 * Interrupt handling of EXT2 - KEY Falling
 */



#include "Intr_IN_UARTs_functions.h"



// MIDI1 IN ISR 
cyg_uint32 UART0_IN_ISR( cyg_vector_t vector, cyg_addrword_t data ){

	// Prevent this interrupt from occuring until DSR completes
	cyg_interrupt_mask (vector);

	// Update the counter for the buffer, the place to read the data in
	G_in_MIDI1_ndx = ( (G_in_MIDI1_ndx + 1) % def_MIDI1_IN_BUFFER_SIZE );

	// Acknowledge interrupt so other interrupts can be serviced
	cyg_interrupt_acknowledge(vector);
	
	// Proceed to executing the DSR
	return CYG_ISR_CALL_DSR;
}


// MIDI2 IN ISR 
cyg_uint32 UART1_IN_ISR( cyg_vector_t vector, cyg_addrword_t data ){

	// Prevent this interrupt from occuring until DSR completes
	cyg_interrupt_mask (vector);

	// Update the counter for the buffer, the place to read the data in
	G_in_MIDI2_ndx = ( (G_in_MIDI2_ndx + 1) % def_MIDI2_IN_BUFFER_SIZE );

	// Acknowledge interrupt so other interrupts can be serviced
	cyg_interrupt_acknowledge(vector);

	// Proceed to executing the DSR
	return CYG_ISR_CALL_DSR;
}


// MIDI1 IN DSR 
void UART0_IN_DSR( cyg_vector_t vector, cyg_ucount32 count, cyg_addrword_t data ){
		
	// HAL_READ_UINT32( E7T_UART0_BASE + E7T_UART_RXBUF, midi_byte);
	// Read the data from the register into the MIDI read buffer
	HAL_READ_UINT32( E7T_UART0_BASE + E7T_UART_RXBUF, G_in_MIDI1[G_in_MIDI1_ndx] );

	// Place the MIDI message in the mailbox, for the interpreter
	cyg_mbox_tryput( UART0_IN_mbox_handle, &(G_in_MIDI1[G_in_MIDI1_ndx]) );

	// Allow the interrupt to occur again
	cyg_interrupt_unmask(vector);
}



// MIDI2 IN DSR 
void UART1_IN_DSR( cyg_vector_t vector, cyg_ucount32 count, cyg_addrword_t data ){
	
	// HAL_READ_UINT32( E7T_UART1_BASE + E7T_UART_RXBUF, midi_byte);
	// Read the data from the register into the MIDI read buffer
	HAL_READ_UINT32( E7T_UART1_BASE + E7T_UART_RXBUF, G_in_MIDI2[G_in_MIDI2_ndx] );

	// Place the MIDI message in the mailbox, for the interpreter
	cyg_mbox_tryput( UART1_IN_mbox_handle, &(G_in_MIDI2[G_in_MIDI2_ndx]) );
			
	// Allow the interrupt to occur again
	cyg_interrupt_unmask(vector);
}




// Monitors the UARTRX0 mailbox
void UART0_IN_thread( cyg_addrword_t index ){
	
	unsigned char UART_ndx 	= 0;		// This is UART0

	unsigned int* message = NULL;
	unsigned char midi_byte	= 0;
		
	// Wait for messages (i.e. MIDI bytes) to arrive
	while (1){
		
		// Read in the next MIDI byte
		message = (unsigned int*) cyg_mbox_get( UART0_IN_mbox_handle );			
		if ( message == NULL ) continue;

		// Extract the actual data from the message, in char format (first 8 bits)
		midi_byte = (unsigned char) *( message );
		
		// differentiate between status and data bytes. Pass the UART index along
		switch( midi_byte & 0x80 ){

			case MIDI_STATUS_BYTE:  // 0x80 i.e. highest bit is set

				read_MIDI_STATUS_BYTE( midi_byte, UART_ndx );				
				break;
				
			case MIDI_DATA_BYTE:

				read_MIDI_DATA_BYTE(   midi_byte, UART_ndx );
				break;
		}
	}
}



// Monitors the UARTRX0 mailbox
void UART1_IN_thread( cyg_addrword_t index ){
	
	unsigned char UART_ndx 	= 1;		// This is UART1

	unsigned int* message = NULL;
	unsigned char midi_byte	= 0;
		
	// Wait for messages (i.e. MIDI bytes) to arrive
	while (1){

		// Read in the next MIDI byte
		message = (unsigned int*) cyg_mbox_get( UART1_IN_mbox_handle );
		if ( message == NULL ) continue;

		// Extract the actual data from the message - in char format (first 8 bits)
		midi_byte = (unsigned char) *( message );

		// differentiate between status and data bytes. Pass the UART index along
		switch( midi_byte & 0x80 ){

			case MIDI_STATUS_BYTE:	// 0x80 i.e. highest bit is set

				read_MIDI_STATUS_BYTE( midi_byte, UART_ndx );				
				break;
				
			case MIDI_DATA_BYTE:

				read_MIDI_DATA_BYTE(   midi_byte, UART_ndx );
				break;
		}
	}
}



