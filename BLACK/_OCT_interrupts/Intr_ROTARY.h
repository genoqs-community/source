/* OCTOPUS
 * 
 * Interrupt handling of EXT1
 * genoQs Machines, 07.11.2003
 */


// EXT1 ISR: Rotary Interrupt handling
cyg_uint32 Ext1_ISR(cyg_vector_t vector, cyg_addrword_t data){
	
	// Prevent this interrupt from occuring until DSR completes
	cyg_interrupt_mask (vector);

	// Acknowledge interrupt so other interrupts can be serviced
	cyg_interrupt_acknowledge(vector);

	// Proceed to executing the DSR
	return CYG_ISR_CALL_DSR;
}



// EXT1 DSR: Rotary Deferred Interrupt Handling
void Ext1_DSR(cyg_vector_t vector, cyg_ucount32 count, cyg_addrword_t data){

//	unsigned int rotNdx = scanRots();
	
	// Put the keyNdx into the mailbox for further execution
// 	cyg_mbox_tryput ( rotNdx_mbox_handle, (void*) rotNdx);
	
	// Post the semaphore, thread will take care of the rest
	cyg_semaphore_post( &sem_scanRots );
	
	// Allow the interrupt to occur again
	cyg_interrupt_unmask(vector);
}



//
// ROTARY SCAN 
//
unsigned int scanRots() {
	
	// Scan the rotary index and direction and pack it into one number to be passed on..
	// int port_value = 0;
	// int brute_port_value = 0;
	unsigned int my_port = 0; 
	// int row=0;
	unsigned char direction=0;

	// Some counter variables. Rotary_ndx used for keeping track of the read rotary.
	unsigned int i=0, j=0;
	unsigned int rotary_ndx=0;

	// Get the dataport mutex
	cyg_mutex_lock (&mutex_data_port);

	// Set IO Ports for read/write config
// Old Keydevice Driver
//	HAL_WRITE_UINT32(E7T_IOPMOD, 0x3ff00);

	// Set IO Ports for read/write config
// New Keydevice Driver
	HAL_WRITE_UINT32(E7T_IOPMOD, 0x3f800);

	// Arm the hardware so it can trigger an interrupt by placing an impulse everywhere
	for (i=0; i<11; i++) {
		HAL_WRITE_UINT32(E7T_IOPDATA , SELECT_ROTARY_DEVICE | IMPULSE_HIGH | CLOCK_LR_LOW | OCTDEF_UART2_MODE);	
 		HAL_WRITE_UINT32(E7T_IOPDATA , SELECT_ROTARY_DEVICE | IMPULSE_HIGH | CLOCK_LR_HIGH | OCTDEF_UART2_MODE);
	}

	// Initial step: select first column
	HAL_WRITE_UINT32(E7T_IOPDATA , SELECT_ROTARY_DEVICE | IMPULSE_LOW | CLOCK_LR_LOW | OCTDEF_UART2_MODE);	
 	HAL_WRITE_UINT32(E7T_IOPDATA , SELECT_ROTARY_DEVICE | IMPULSE_LOW | CLOCK_LR_HIGH | OCTDEF_UART2_MODE);


	// Follow-up scan sequence for the rotary encoder pairs
	for (j=0; j<=11; j++) {		
		// Read the Rotary
	 	HAL_WRITE_UINT32(E7T_IOPDATA , SELECT_ROTARY_DEVICE | IMPULSE_HIGH | CLOCK_LR_LOW | OCTDEF_UART2_MODE);
	 	HAL_READ_UINT32(E7T_IOPDATA , my_port);


		// BIG KNOB: Check the MSB (which is the second rotary_ndx in pair)
		if (j == 0) {
			if ((my_port & 0xf0) == 0xf0) {
				// CW increases
				direction = INC;
			}
			if ((my_port & 0xf0) == 0x10) {
				// CCW decreases	
				direction = DEC;
			}
			if (direction) {
				// ROT detected 
				rotary_ndx = 0;
				break;
			}
		}
					

		// R H S: Scanning and writing the value in the array.
		if ( (j > 0) && (j <= 5) ) {
			// LSB: Check the LSB (which is the first rotary_ndx in pair)
			if ((my_port & 0x0f) == 0x0f) {
				// CW increases
				direction = INC;
			}
			if ((my_port & 0x0f) == 0x01) {
				// CCW decreases
 				direction = DEC;
			}			
			if (direction) {
				// ROT detected 
				rotary_ndx = (j*2) - 1;
				break;			
			}
								
			// MSB: Check the MSB (which is the second rotary_ndx in pair)
			if ((my_port & 0xf0) == 0xf0) {
				// CW increases
				direction = INC;
			}
			if ((my_port & 0xf0) == 0x10) {
				// CCW decreases	
				direction = DEC;
			}
			if (direction) {
				// ROT detected 
				rotary_ndx = (j*2);
				break;
			}
		}

		
		
		// L H S: Scannig and writing the value into the array
		if (j >= 6) {
			// LSB: Check the LSB (which is the first rotary_ndx in pair)
			if ((my_port & 0xf0) == 0xf0) { 
				// CW increases
				direction = INC;
			}
			if ((my_port & 0xf0) == 0x10) {
				// CCW decreases
				direction = DEC;
			}
			if (direction) {
				// ROT detected 
				rotary_ndx = (j*2) - 1;
				break;
			} 

			// MSB: Check the MSB (which is the second rotary_ndx in pair)
			if ((my_port & 0x0f) == 0x0f) {
				// CW increases
				direction = INC;
			}
			if ((my_port & 0x0f) == 0x01) {
				// CCW decreases
				direction = DEC;
			}
			if (direction) {
				// ROT detected 
				rotary_ndx = (j*2);
				break;
			} 
		}

		// Shift to next rotary encoder pair
	 	if (j<11) 
	 		HAL_WRITE_UINT32(E7T_IOPDATA , SELECT_ROTARY_DEVICE | IMPULSE_HIGH | CLOCK_LR_HIGH | OCTDEF_UART2_MODE);
	}


	// Reset the rotary counters
	HAL_WRITE_UINT32(E7T_IOPDATA , RESET_ROTARY_COUNTERS | OCTDEF_UART2_MODE);
	
	// Re-init the data lines to avoid confusion
	HAL_WRITE_UINT32(E7T_IOPDATA , SELECT_NOP_DEVICE | OCTDEF_UART2_MODE);

	// Release the dataport mutex
	cyg_mutex_unlock( &mutex_data_port );
		
	return ((rotary_ndx << 2) | direction);
}


//
// ROTARY EXECUTE THREAD
//
void rotExecute_thread (cyg_addrword_t index) {
//	int* rotNdx=0;
	unsigned int rotNdx = 0;
	
	// Wait for messages(i.e. rotary values) to arrive
	while (1){

		cyg_semaphore_wait( &sem_scanRots );

		// Rot interrupt signaled, read source		
		rotNdx = scanRots();

		// Make sure the message was received before attempting to process it
//		if ( rotNdx != NULL ){
		
//		cyg_mutex_lock( &mutex_sequencer_data );
		cyg_scheduler_lock();

			executeRot( (int) rotNdx );

		cyg_scheduler_unlock();		
//		}

	}
}




