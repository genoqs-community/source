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

	// Post the semaphore, thread will take care of the rest
	cyg_semaphore_post( &sem_scanRots );
	
	// Allow the interrupt to occur again
	cyg_interrupt_unmask(vector);
}



//
// ROTARY SCAN 
//
unsigned int G_scanRots() {
	
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


	// Follow-up G_scan sequence for the rotary encoder pairs
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

	unsigned int rotNdx = 0;
	
	// Wait for messages(i.e. rotary values) to arrive
	while (1){

		cyg_semaphore_wait( &sem_scanRots );
		
		// Rot interrupt signaled, read source		
		rotNdx = G_scanRots();

		// Execute associated command
		cyg_scheduler_lock();

		executeRot( (int) rotNdx );

		cyg_scheduler_unlock();	
	}
}




