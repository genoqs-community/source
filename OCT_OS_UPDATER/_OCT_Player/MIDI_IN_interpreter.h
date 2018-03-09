

#include "MIDI_IN_functions.h"
#include "MIDI_IN_executer.h"


// Interpreter functions collect the data. Once they have a valid package,
// the package gets passed on for execution by the executor functions.



//_______________________________________________________________________________________ 
//
// Interprets the midi_byte as a CONTROL message
/* Status: 	0xB0 to 0xBF where the low nibble is the MIDI channel.
 * Data:	Two data bytes follow the Status.
 * 			First:  controller number (0 to 127). Indicates which controller is affected.
 * 			Second: value to which the controller should be set (0 to 127).
 */
void midi_interpret_CONTROL( unsigned char midi_byte, unsigned char UART_ndx ){

	// If data byte was received without prior status byte, fill in the running status
	if ( midi_controller_ndx == 0 ){

		// Take the runnning status byte into account
		midi_controller_IN_UART[UART_ndx][midi_controller_ndx] = 
			running_status_byte_IN_UART[ UART_ndx ];

		// Update running index of the data bytes
		midi_controller_ndx = 1;
	}

	// Interpret the byte as a DATA byte.
	midi_controller_IN_UART[UART_ndx][midi_controller_ndx] = midi_byte;
		
	// Point the index to the next data byte
	midi_controller_ndx++;
	
	// Check for the completeness of the note.
	// If the second data byte is not invalid (0xff) then we have a note.
	if ( midi_controller_IN_UART[UART_ndx][2] != 0xff ) {
		
		// Execute the midi note -> Record only when the sequencer is running
// Checked for later..
//		if ( RUNBIT == ON ){
			
			midi_controller_execute( 	midi_controller_IN_UART[UART_ndx][0], 
										midi_controller_IN_UART[UART_ndx][1], 
										midi_controller_IN_UART[UART_ndx][2] 	);	
		
			// Clear the note and the data byte index
			midi_controller_IN_UART[UART_ndx][1] = 0xff;
			midi_controller_IN_UART[UART_ndx][2] = 0xff;
		
			midi_controller_ndx = 0;
//		} // sequencer is running

	} 

}



//_______________________________________________________________________________________ 
//
// Interprets the midi_byte as a REALTIME messages
void midi_interpret_REALTIME( unsigned char midi_byte ){

	// MTC reference:
	// 0xf8   timing clock
	// 0xf9   undefined			->not implemented
	// 0xfa   start
	// 0xfb   continue
	// 0xfc   stop
	// 0xfd   undefined			->not implemented
	// 0xfe   active sensing	->not implemented
	// 0xff   system reset		->not implemented

	// unsigned int i = 0;

	// EXT clock is a must to continue..
	if ( CLOCK_SOURCE != EXT ) return;

	// REAL TIME MESSAGES
	switch ( midi_byte ) {

		// CLOCK
		case MIDICLOCK_CLOCK:
			
			// Drive the sequencer to play..
			// TWICE!! ..as required by the MIDI Clock resolution
			cyg_semaphore_post( &sem_driveSequencer );
			cyg_semaphore_post( &sem_driveSequencer );
			break;
			

		// START
		case MIDICLOCK_START:

			// Only react if running on external clock
			// Need to reset because ...
		 	sequencer_RESET();

			midi_clock_in_counter++;
		
			// Jumpstart the sequencer
			sequencer_START();
			break;
			

		// CONTINUE
		case MIDICLOCK_CONTINUE:

			RUNBIT 		= ON;
			PAUSEBIT 	= OFF;
			break;
			

		// STOP
		case MIDICLOCK_STOP:

			// First STOP causes a pause, second one stops slave for good.
			switch ( RUNBIT ){
				case ON:
					// This is not a full STOP command, can be pause as well
					sequencer_HALT();
					break;

				case OFF:
					// This is a full stop for the slave machine.
					sequencer_STOP();
					break;
			}
			midi_clock_in_counter = 0;

			// Just show the page to refresh the stop update
			Page_requestRefresh();

			break;
	}
}



//_______________________________________________________________________________________ 
//
// Interprets the midi_byte as part of a NOTE_ON (or NOTE_OFF) message
void midi_interpret_NOTE_ON( unsigned char midi_byte, unsigned char UART_ndx ){

	// Store the byte in the circular array	- just so it can be printed out.
	// DO NOT USE PRINTFS OR THE LIKE HERE, OR THE INPUT IS DELAYED AND SCRAMBLED

	// If data byte was received without prior status byte, 
	// .. or the status byte was not filled in yet, fill with in the running status
	if (	( midi_data_ndx == 0 ) 
		|| 	( midi_note_IN_UART[UART_ndx][0] == 0xff )
		){

		// Take the runnning status byte into account - can be one of NOTE_ON or NOTE_OFF
		midi_note_IN_UART[UART_ndx][0] = running_status_byte_IN_UART[ UART_ndx ];

		// Print buffer
		midi_in[0] = running_status_byte_IN_UART[ UART_ndx ];

		// Update running index of the data bytes
		midi_data_ndx = 1;		

// d_iag_printf( "got rst:0x%x\n", midi_note_IN_UART[UART_ndx][0] );
	}
						
	// Interpret the byte as a DATA byte.
	midi_note_IN_UART[UART_ndx][midi_data_ndx] = midi_byte;
	midi_in[midi_data_ndx] = midi_byte;
		
	// Point the index to the next data byte
	midi_data_ndx++;
	
	// Check for the completeness of the note.
	// If the second data byte is not invalid (0xff) then we have a note.
//	if ( midi_note_IN_UART[UART_ndx][2] != 0xff ) {
	if ( midi_data_ndx > 2 ) {
		
//		d_iag_printf( "assembled: st:0x%x pi:0x%x ve:0x%x\n", 
//									midi_note_IN_UART[UART_ndx][0], 
//									midi_note_IN_UART[UART_ndx][1], 
//									midi_note_IN_UART[UART_ndx][2]	);

		// Execute the midi note -> Record only when the sequencer is running
		switch( RUNBIT ){
			
			case OFF:
				// Take pitch over from played note into recording track
				// do step recording, or play through..
			case ON:			
				// Record midi note - pass the UART ndx along, for the pass through ops
				midi_note_execute( 	UART_ndx,
									midi_note_IN_UART[UART_ndx][0], 
									midi_note_IN_UART[UART_ndx][1], 
									midi_note_IN_UART[UART_ndx][2] 	);	
				break;				
		} // sequencer is running - or not
			
		// Clear the note and the data byte index
		midi_note_IN_UART[UART_ndx][0] = 0xff;
		midi_note_IN_UART[UART_ndx][1] = 0xff;
		midi_note_IN_UART[UART_ndx][2] = 0xff;
		
		midi_data_ndx = 0;
	}

}



//_______________________________________________________________________________________ 
//
// Interprets the midi_byte as a BENDER / PITCH WHEEL message
/* Status: 	0xE0 to 0xEF where the low nibble is the MIDI channel.
 * Data:	Two data bytes follow the Status. They are combined to a 14-bit value see above.
 */
void midi_interpret_BENDER( unsigned char midi_byte, unsigned char UART_ndx ){

	// If data byte was received without prior status byte, fill in the running status
	if ( midi_controller_ndx == 0 ){

		// Apply the runnning status byte
		midi_controller_IN_UART[ UART_ndx ][ midi_controller_ndx ] = 
			running_status_byte_IN_UART[ UART_ndx ];

		// Update running index of the data bytes
		midi_controller_ndx = 1;
	}

	// Interpret the byte as a DATA byte.
	midi_controller_IN_UART[UART_ndx][midi_controller_ndx] = midi_byte;
		
	// Point the index to the next data byte
	midi_controller_ndx++;
	
	// Check for the completeness of the message.
	// If the second data byte is not invalid (0xff) then we have a note.
	if ( midi_controller_IN_UART[ UART_ndx ][ 2 ] != 0xff ) {
		
		// Execute the midi note -> Record only when the sequencer is running
		if ( RUNBIT == ON ){
			
			midi_bender_execute( 	midi_controller_IN_UART[UART_ndx][0], 
									midi_controller_IN_UART[UART_ndx][1], 
									midi_controller_IN_UART[UART_ndx][2] 	);	
		
			// Clear the note and the data byte index
			midi_controller_IN_UART[UART_ndx][1] = 0xff;
			midi_controller_IN_UART[UART_ndx][2] = 0xff;
		
			midi_controller_ndx = 0;
		} // sequencer is running
	} 
}


//_______________________________________________________________________________________ 
//
// Interprets the input MIDI message - as it was sent by a virtual channel
void midi_interpret_VIRTUAL( 	unsigned char type, 
								unsigned char val0,
								unsigned char val1,
								unsigned char val2, 
								unsigned char UART_ndx ){
	
	unsigned int status_byte 	= 0;
	unsigned int data_byte1 	= 0;
	unsigned int data_byte2 	= 0;
	cyg_handle_t UART_VIRTUAL_mbox_handle;

	// UART mailbox assignment
	switch( UART_ndx ){
		case 1:
			UART_VIRTUAL_mbox_handle 	= UART0_IN_mbox_handle;
			break;
		case 2:
			UART_VIRTUAL_mbox_handle 	= UART1_IN_mbox_handle;
			break;
	}

	// Resend bytes to the IN mailboxes
	switch( type ){

		case MIDI_NOTE:		
			// Generate status byte: lower byte is the channel
			status_byte = MIDI_CMD_NOTE_ON | (val0 -1);
			// Note number (pitch)
			data_byte1	= val1;
			// Velocity
			data_byte2	= val2;

			// Place the MIDI message in the mailbox, for the interpreter
			cyg_mbox_tryput( UART_VIRTUAL_mbox_handle, (void*) status_byte );
			cyg_mbox_tryput( UART_VIRTUAL_mbox_handle, (void*) data_byte1 );
			cyg_mbox_tryput( UART_VIRTUAL_mbox_handle, (void*) data_byte2 );
			break;


		case MIDI_CC:		
			// Generate status byte: lower byte is the channel
			status_byte = MIDI_CMD_CONTROL | (val0 -1);
			// Controller number
			data_byte1	= val1;
			// Controller amount
			data_byte2	= val2;

			// Place the MIDI message in the mailbox, for the interpreter
			cyg_mbox_tryput( UART_VIRTUAL_mbox_handle, (void*) status_byte );
			cyg_mbox_tryput( UART_VIRTUAL_mbox_handle, (void*) data_byte1 );
			cyg_mbox_tryput( UART_VIRTUAL_mbox_handle, (void*) data_byte2 );
			break;

		
		case MIDI_BENDER:
			// Generate status byte: lower byte is the channel
			status_byte = MIDI_CMD_BENDER | (val0 -1);
			// First 7 bits
			data_byte1	= val1;
			// Second 7 bits
			data_byte2	= val2;

			// Place the MIDI message in the mailbox, for the interpreter
			cyg_mbox_tryput( UART_VIRTUAL_mbox_handle, (void*) status_byte );
			cyg_mbox_tryput( UART_VIRTUAL_mbox_handle, (void*) data_byte1 );
			cyg_mbox_tryput( UART_VIRTUAL_mbox_handle, (void*) data_byte2 );
			break;


		case MIDI_PGMCH:
			// Generate status byte: lower byte is the channel
			status_byte = MIDI_CMD_PGM_CHANGE | (val0 -1);
			// Program Number
			data_byte1	= val1;

			// Place the MIDI message in the mailbox, for the interpreter
			cyg_mbox_tryput( UART_VIRTUAL_mbox_handle, (void*) status_byte );
			cyg_mbox_tryput( UART_VIRTUAL_mbox_handle, (void*) data_byte1 );
			break;
	}

		
}



