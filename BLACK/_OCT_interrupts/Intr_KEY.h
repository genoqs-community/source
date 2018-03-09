
/* OCTOPUS
 * 
 * Interrupt handling of EXT0
 * genoQs Machines, 07.11.2003
 */


#include "Intr_KEY_functions.h"



//
// The thread takes the key value from readKeys and gets it executed
//
void keyExecute_thread (cyg_addrword_t index){

	int* keyNdx=0;
	
	// Wait for messages (i.e. keypresses) to arrive
	while (1){
		// Read in the next pressed key
		keyNdx = cyg_mbox_get( keyNdx_mbox_handle );

		// Make sure the message was received before attempting to process it
		if ( keyNdx != NULL ){

			cyg_scheduler_lock();

			// For now remove the info flag
			executeKey ( ((int) keyNdx & 0xffff) );

			cyg_scheduler_unlock();
		}
	}
}




//
// Alarm-triggered thread (TVstyle) - gets executed constantly, monitoring key press changes
//
void readKeys_thread (cyg_addrword_t index){

//	unsigned int 	temp = 0;
//	unsigned char 	row = 0,
//					col = 0;

	// Stores the currently pressed key
	unsigned int my_pressed_key		= 0;
	unsigned int my_pressed_page 	= 0;
	unsigned int my_pressed_mutator = 0;
	unsigned char row = 0, col = 0;
		
	// Indicates whether a new key was pressed recently
	unsigned char new_key = FALSE;

	while (1) {
		
		// Wait for key interrupt to post the semaphore
		cyg_semaphore_wait( &sem_readKeys );
		
		// Scan keys and tell whether a new press has occurred ( TRUE / FALSE )
		new_key = scanKeys();	
		
		//////////////////////////////////
		// INTERACTIVE MODE CODE -- BEGIN

		switch( zoom_level ){
			
			// DEVICE
			case zoomDEVICE:
				// no interactive code at this level
				break;
				
			// STEP
			case zoomSTEP:				
				#include "Intr_KEY_STEP.h"
				break;
				
			// MAP
			case zoomMAP:
				#include "Intr_KEY_MAP.h"
				break;

			// TRACK
			case zoomTRACK:			
				#include "Intr_KEY_TRACK.h"
				break;

			// PAGE
			case zoomPAGE:			
				#include "Intr_KEY_PAGE.h"
				break;
				
			// GRIDTRACK
			case zoomGRIDTRACK:
				#include "Intr_KEY_GRIDTRACK.h"
				break;

			// GRID
			case zoomGRID:
				#include "Intr_KEY_GRID.h"
				break;

		} // switch zoom_level
		
		// INTERACTIVE MODE CODE -- END
		////////////////////////////////

		// Refresh the Page to show any changes done by the key press.
		Page_requestRefresh();

	} // while (1) 

}





// Scans the data port and generates the index of the pressed key
unsigned char scanKeys() {
	
	unsigned int reg=0;
	unsigned int i=0; 
//	unsigned int j=0;
//	unsigned int k=0;
	unsigned int wait=15;
	unsigned int my_port=0;
	unsigned int keyIndx[8] = { 0, 0, 0, 0, 0, 0, 0, 0 };
	unsigned int keys = 0;
	unsigned int keys_new = 0;
	unsigned int keys_old = 0;
	unsigned int key_state = 0;
	unsigned int LSB_bus = 0;
	unsigned int MSB_bus = 0;

	unsigned char result = FALSE;

	// d_iag_printf( "scanKeys active\n" );

	// Disable all external interrupt pins
	HAL_WRITE_UINT32( E7T_IOPCON, 0);

	// New code:Set IO Ports for read/write config
	HAL_WRITE_UINT32(E7T_IOPMOD, 0x3f800);

	// Get the dataport mutex
	cyg_mutex_lock (&mutex_data_port);

	// Reset all key shift registers
	HAL_WRITE_UINT32( E7T_IOPDATA , RESET_KEY | OCTDEF_UART2_MODE );
	
	// Initial sequence:
	// Write impulse. This selects the first column
	HAL_WRITE_UINT32( E7T_IOPDATA, IMPULSE_HIGH 	| CLOCK_KEY_LOW 	| OCTDEF_UART2_MODE);
	HAL_WRITE_UINT32( E7T_IOPDATA, IMPULSE_HIGH 	| CLOCK_KEY_HIGH 	| OCTDEF_UART2_MODE);
		
	// Follow-up sequence: Check every one of 25 key shift-registers
	for (reg=0; reg < 25; reg++) {	

		// Prepare scan of the LSB
		
		// ??
		keys_old = 	scan[0][reg] & scan[1][reg] 	// & 	scan[2][reg]
						;
		// Marcels wait loop - hardware needs it
		my_wait( wait );
	
		// Read the LSB
		HAL_WRITE_UINT32( E7T_IOPDATA , 	SELECT_KEY_LSB_DEVICE 
										| 	IMPULSE_LOW 
										| 	CLOCK_KEY_LOW 
										| 	OCTDEF_UART2_MODE );
		// Read the port value
		HAL_READ_UINT32( E7T_IOPDATA, my_port );
		
		// Mask the Key_Bus an store the value. LSB_bus is a 5 bit bus.
		LSB_bus = my_port & 0x1f;
		
		// Store the new value for this register (column)
		scan[cycle][reg] = ( scan[cycle][reg] & 0xff00 ) | LSB_bus; 
		
		// ??
		keys_new = 	scan[0][reg] & scan[1][reg] 	// & 	scan[2][reg]
						;
						
		// Toggle if key is pressed or released
		keys = (keys_old & 0x00ff) ^ (keys_new & 0x00ff);

		// If there is a change in the port value - i.e. key pressed or released
		if ( keys != 0 ) {
			
			// At every bit position in the current register..
			for ( i=0; i < 8; i++ ){

				// ..compute the key index and store it for execution	
				keyIndx[i] = computeKeyNdx( reg, LSB, (keys & (1<<i) ));
			}
			
			// Iterate through the stored values from above
			for (i=0; i < 8; i++) {
				
				// If there is a valid keyindex - indicating a press..
				if ( keyIndx[i] != 0 ) {
					
					// ??
					if ( keys_new > keys_old ) {
						
						// Indicate that at least a key is pressed
						key_state = PRESSED; 

						// Put the scanned byteNdx into the mailbox for proper execution
						cyg_mbox_tryput ( keyNdx_mbox_handle, (void*) keyIndx[i] );	
						
						// Mark the pressed keys in an array
						pressed_keys[ keyIndx[i] ] = keyIndx[i];

						result = TRUE;
					}
					else if ( keys_new < keys_old ) {

						// Erase pressed key marker from the array
						pressed_keys[ keyIndx[i] ] = 0;
					}	
				} // keyIndx[i] != 0
			} // 8 keyIndx iterator
		} // keys != 0



		// Prepare scan of the MSB
		keys_old = scan[0][reg] & scan[1][reg] 		//	& 	scan[2][reg]
						;
		
		// Read the MSB
		HAL_WRITE_UINT32( E7T_IOPDATA, 		SELECT_KEY_MSB_DEVICE 
										| 	IMPULSE_LOW 
										| 	CLOCK_KEY_LOW 
										| 	OCTDEF_UART2_MODE );
		// Read the port again
		HAL_READ_UINT32( E7T_IOPDATA, my_port );

		// Mask the Key_Bus an store the value. MSB_bus is a 6 bit bus.
		MSB_bus = my_port & 0x3f;

		scan[cycle][reg] = ( scan[cycle][reg] & 0x00ff)  | (MSB_bus << 8); 
		
		keys_new = scan[0][reg] & scan[1][reg] // 	& 	scan[2][reg]
						;
		
		// Toggle if key is pressed or released
		keys = ((keys_old & 0xff00) ^ (keys_new & 0xff00)) >> 8;
	
		//if ( reg == 0 ){
		//	d_iag_printf( "scan[%d][0]:%d", cycle, scan[cycle][reg] );
		//	my_print_bits32( scan[cycle][reg] );
		//}
	
		if ( keys != 0 ) {
			
			// At every bit position in the current register..
			for ( i=0; i < 8; i++ ){

				// ..compute the key index and store it for execution	
				keyIndx[i] = computeKeyNdx( reg, MSB, (keys & (1<<i) ));
			}
			
			// Iterate through the keyIndx array looking for content
			for (i=0; i<8; i++) {
				
				// If content found..
				if ( keyIndx[i] != 0 ) {
					
					// ..the key was pressed
					if ( keys_new > keys_old ){
						
						key_state = PRESSED;

						// Put the scanned byteNdx into the mailbox for proper execution
						cyg_mbox_tryput ( keyNdx_mbox_handle, (void*) keyIndx[i] );
					
						// Mark pressed keys in the array
						pressed_keys[ keyIndx[i] ] = keyIndx[i];
						
						result = TRUE;
					}
					// ..the key was released
					else if ( keys_new < keys_old ){

						// Unmark the key from the pressed array
						pressed_keys[ keyIndx[i] ] = 0;
					}
					
					//d_iag_printf( "MSB byteNdx[%d][%d]: %d, keys: 0x%x, keys_old 0x%x, keys_new 0x%x, ,my_port 0x%x\n", cycle, reg, keyIndx[i], keys, keys_old, keys_new, my_port);
	
				} // if ( keyIndx[i] != 0 ) 
			} // keyIndx array iterator
		} // if ( keys != 0 )
	

		// Shift to next key column
		HAL_WRITE_UINT32(E7T_IOPDATA , IMPULSE_LOW | CLOCK_KEY_HIGH | OCTDEF_UART2_MODE);
		
	} // Shift register iterator
	

	// In this section: Refill the Shift registers, arming for the next interrupt trigger.
	for (i=0; i<25; i++) {	
		
		HAL_WRITE_UINT32( E7T_IOPDATA, IMPULSE_HIGH 	| CLOCK_KEY_LOW 	| OCTDEF_UART2_MODE);	
		HAL_WRITE_UINT32( E7T_IOPDATA, IMPULSE_HIGH 	| CLOCK_KEY_HIGH 	| OCTDEF_UART2_MODE);
	}	
	
	// Check the port again to see if the user still keeps some keys pressed	
	key_pressed = is_key_active();
	
	// Release the dataport mutex
	cyg_mutex_unlock( &mutex_data_port );
	
	// Incrementing the scan(?) cycle
//	cycle = (cycle + 1) % 3;
	cycle = (cycle + 1) % 2;


	// No more key is pressed on the panel
	if ( key_pressed == FALSE ){
		
		// Reset the key press aray
//		for (k=0; k < 261; k++) {
//		
//			pressed_keys[k] = 0;
//		}
		memset( pressed_keys, 0, 1044 );


		// Initialize the scan array
//		for (i=0; i < 3; i++){
//
//			for (j=0; j < 26; j++){
//
//				scan[i][j] = 0;
//				cycle = 0;
//			}
//		}
		cycle = 0;
		memset( scan, 0, 312 );

	} // No more keys are pressed on panel
	
	// Clear the Interrupt pending register for the external interrupt generated at detection time
	HAL_WRITE_UINT32(E7T_INTPND, 5);
 
	// Enable external interrupt pins for rotarys and keys
	HAL_WRITE_UINT32( E7T_IOPCON, 		OCTOPUS_IOPCON_KEYINTR_RISING 
									| 	OCTOPUS_IOPCON_ROTINTR_RISING  );
									

	// Seed the randomizer on the first click in the power lifecycle
	if ( RAND_SEEDED == FALSE ){

		// If a key was pressed (typically first key press in the power cycle)
		if ( result == TRUE ){

			// The memory contents were loaded from flash
			if ( POWERON_LOADED == TRUE ){

				srand( cyg_current_time() );
				RAND_SEEDED = TRUE;
			}
			else{
				// No good idea yet..
			}
		}		
	}

	// Indicate whether a new key press has occured				
	return result;
}



