/* OCTOPUS
 * 
 * Interrupt handling and refill of TIM1
 * genoQs Machines, 07.11.2003
 */


// ISR of Timer1: Interrupt handling - This is the heart of OCTOPUS
cyg_uint32 Timer1_ISR(cyg_vector_t vector, cyg_addrword_t data){ 
	
 	// Block this interrupt from occurring until DSR completed
	cyg_interrupt_mask (vector);

  	// Ack the interrupt so other routines can be serviced
  	cyg_interrupt_acknowledge(vector);	

  	// Proceed to DSR
	return CYG_ISR_CALL_DSR;
}


// DSR of Timer1: Deferred Interrupt Routine that handles timer ticks.
void Timer1_DSR( 	cyg_vector_t vector, 
					cyg_ucount32 count, 
					cyg_addrword_t data		){

	unsigned int i = 0;

	// TIMER REFILL - determine if it is needed
  	HAL_READ_UINT32( E7T_TDATA1, i );
	
	// Refill the timer if the tempo has been changed
	if ( i != TIMER_REFILL ){
	
	  	HAL_WRITE_UINT32( E7T_TDATA1, TIMER_REFILL );
	}

	// Timestamp update. Wraps after approx. 310 days @200 bpm, with a glitch in the NOTE OFF.
	// MIDI_timestamp ++;
	// -> moved into drive_Sequencer()
	
	// Determine what to do based on the clock source
	switch( CLOCK_SOURCE ){

		case EXT:
			// Clock is external so nothing to do
			break;

		case INT:
		case OFF:

			// Drive the sequencer to play
			cyg_semaphore_post( &sem_driveSequencer );
			break;
	}

	// Allow this interrupt to occur again
	cyg_interrupt_unmask (vector);
}



////
//// TIMER EXECUTION THREAD
////
void timerInterrupt_thread( cyg_addrword_t index ){
	
	// Infinite loop for monitoring the request queues
	while (1){

		// Wait on the refresh semaphore
		cyg_semaphore_wait( &sem_driveSequencer );

		// Drive the sequencer
		driveSequencer();
	}
}




// Drives the sequencer - interprets the timer tick
void driveSequencer(){

	// Timestamp update. Wraps after approx. 310 days @200 bpm, with a glitch in the NOTE OFF.
	MIDI_timestamp ++;

	// Interpret according to the RUNBIT of the sequencer
	switch( RUNBIT ){

		// RUNBIT is ON
		case ON:
		
			// TTC_ABS_VALUE: Master time keeper, between two matrix columns
			// Generate the absolute value of TIMER_TICK_COUNTER - which is the master time keeper
			TTC_ABS_VALUE = (TTC_ABS_VALUE % 12) + 1;
				
			// Send MIDI CLOCK on every other TTC starting at 1
			if ( (TTC_ABS_VALUE) % 2 == 1) {
	
				// Consider the clocksource setting
				if ( CLOCK_SOURCE == INT ){

					MIDI_send( MIDI_CLOCK, MIDICLOCK_CLOCK, 0, 0 );
				}
			}
				
				
			// Refresh PLAYLOAD indicator	
			if ( TTC_ABS_VALUE == 1 ){
							
				if ( PLAY_LOAD > PLAY_LIMIT ){
		
					CPU_LOAD_MONITOR  = MIR_RED;
											
					// Set the alarm to make it GREEN again
					cyg_alarm_initialize(	CPU_overloadAlarm_hdl,
											cyg_current_time() + CPU_RED_INTERVAL,
											0 );
				}
			
				PLAY_LOAD = 0;
			}		

			// Trigger Player at current TTC_ABS_VALUE position
			PLAYER_dispatch( TTC_ABS_VALUE );

			// Global Blinker trigger
			if ( TTC_ABS_VALUE == 1 ){

				// GLOBALBLINKER TOGGLE
				GLOBALBLINKER ^= 1;

				// Refresh the Page 
				// Page_requestRefresh();
			}
			break;
			

		// RUNBIT is OFF
		case OFF:
		
// d_iag_printf( "driving sequencer first\n" );

			// TIMER_TICK_COUNTER: INCREMENT - wrap depends on TIMER_TICKS_PER_16TH - resolution
			TIMER_TICK_CNTR = ( TIMER_TICK_CNTR % OCTDEF_TIMER_TICKS_PER_16TH_AT4X ) + 1;		
			// d_iag_printf( "TTC:%d ", TIMER_TICK_CNTR );
	
			// TEMPO_TRACKER - tracks the tempo index
			if (TIMER_TICK_CNTR == 1){
		
				TEMPO_TRACKER = (TEMPO_TRACKER % OCTDEF_MAX_TEMPO_MULTIPLIER) + 1;
				// d_iag_printf( "TEMPO_TRACKER:%d\n", TEMPO_TRACKER );
		
				// Show the update
				Page_requestRefresh();

// d_iag_printf( "page refreshed\n" );

				// Toggle the global blinker
				if (TEMPO_TRACKER == 1) {
	
					// GLOBALBLINKER TOGGLE
					GLOBALBLINKER ^= 1;

					// Play the pending MIDI OFFs at current timestamp -
					// - important in the EDIT mode, when notes are being sent out 
					play_MIDI_queue( MIDI_timestamp );
// d_iag_printf( "MIDI queue played\n" );
				}
			}
			break;
	}
}



// Updates the Timer refill for the current tempo value
void TIMER_REFILL_update(){

	TIMER_REFILL = 
	
		// This outlines the logic behind the numbers.
		// ( CLOCK_FREQUENCY / (QUARTERNOTES_PER_ROW * 4 * TIMER_TICKS_PER_16TH) )  *  60 / TEMPO;	
		// ( CLOCK_FREQUENCY / (OCTDEF_TIMER_TICKS_PER_16TH_AT4X * 16) )  *  60 / TEMPO;
		// ( 50000000 / (3 * 16) )  *  60 / TEMPO;
		62500000 / TEMPO;

	//	d_iag_printf("TIMER_REFILL_update: %d\n", TIMER_REFILL);
	
	// Fill up Timer1: countdown duration - 
	// Done in the Timer DSR - in order to not disrupt the ticking when using the rotary knobs.
}


// MIX TIMER HANDLING
//
void start_MIX_TIMER(){

	// Start MIX Timer
	MIX_TIMER = ON;	
	EDIT_TIMER = OFF;
	
	// Setup alarm for the MIX TIMER
	cyg_alarm_initialize(	alarm_hdl, 
							cyg_current_time() + TIMEOUT_VALUE,
							0 );
		
	// d_iag_printf( "mix timer started\n" );
}


void restart_MIX_TIMER(){
	// ..if it was started somewhere else 
	
	if ( MIX_TIMER == ON ){

		start_MIX_TIMER();
	}
}


// EDIT TIMER HANDLING - this is not used anywhere just yet..
//
void start_EDIT_TIMER(){
	
	// Start MIX Timer
	EDIT_TIMER = ON;		
	MIX_TIMER = OFF;

	// Setup alarm for the EDIT TIMER
	cyg_alarm_initialize(	alarm_hdl, 
							cyg_current_time() + TIMEOUT_VALUE,
							0 );
}

void restart_EDIT_TIMER() {
	// ..if it was started somewhere else 
	if (EDIT_TIMER != OFF) {
		start_EDIT_TIMER();
	}
}


