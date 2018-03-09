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

	// Determine what to do based on the clock source
	switch( G_clock_source ){

		case EXT:
			// Clock is external so nothing to do
			break;

		case INT:
		case OFF:

			// Drive the sequencer to play.
			kickSequencerThread();
			break;
	}

	// Allow this interrupt to occur again
	cyg_interrupt_unmask (vector);
}



/**
 * Starts the sequencer thread by posting to its semaphore one or more times.
 * This is the only official way to get the sequencer running in its own thread.
 *
 * Reads the hires timer registers and restarts it for the next round.
 *
 * Resets the CPU load protection code, allowing the sequencer to process tracks until
 * near-max load is reached.
 *
 * Protects sequencer against overrun, where the sequencer would be kicked while it is still running.
 * This would starve the other treads that run at lower priority, and the machine would
 * appear to be frozen without being able to change anything.
 *
 * @return			Number of hires clock ticks elapsed since last call.
 * @author 			Robert van der Kamp
 */
unsigned int kickSequencerThread( void )
{
	int ix, kickCt;
	unsigned int timerStartTicks, orgTimerStartTicks, timerAvailTicks, timerLeftTicks, timerDoneTicks;

	// Read start- and countdown registers from timer.
	// Calc number of ticks elapsed since last call and return that value later on.

	HAL_READ_UINT32( E7T_TDATA1, timerStartTicks );
	HAL_READ_UINT32( E7T_TCNT1, timerLeftTicks );
	timerDoneTicks = timerStartTicks - timerLeftTicks;
	orgTimerStartTicks = timerStartTicks;

	// Set the new timer start value, number of ticks available for the next sequencer run,
	// and the number of kicks (posts) to the sequencer sem.

	timerStartTicks = G_TIMER_REFILL;
	timerAvailTicks = G_TIMER_REFILL;
	kickCt          = 1;

	// In external clock mode we set the timer start value to max and don't expect it to run down all to zero.
	// Instead we just measure the number of ticks elapsed (in timerDoneTicks) and start again.
	// In external clock mode we run at half the internal resolution (24 PPQN instead of 48 PPQN), so we must
	// kick the sequencer twice, and we have twice the amount of timer ticks available, which averages to
	// G_TIMER_REFILL * 2. The timerDoneTicks value however more closely follows any tempo changes.

	if ( G_clock_source == EXT ) {
		timerStartTicks = 0xFFFFFFFF;
		timerAvailTicks = timerDoneTicks;
		kickCt = 2;
	}

	// Write both data and count down timer registers. Only do this if a new timer value is needed due to
	// changed tempo. The timer restarts automatically, and left untouched, we get the most accurate time.
	// Also write a new value if this is the first call after the sequencer was started.

	if ( 	(G_clock_source == EXT)
		|| 	(timerStartTicks != orgTimerStartTicks)
		||	(G_manualStartBo == TRUE)
		){

		HAL_WRITE_UINT32( E7T_TDATA1, timerStartTicks );
		HAL_WRITE_UINT32( E7T_TCNT1,  timerStartTicks );

		// Lower the manual start flag
		G_manualStartBo = FALSE;
	}

	// Prevent sequencer overruns by checking the sequencer-is-busy flag.
	// We only want to give it new work if it's ready with the current workload.
	// Not doing so could keep the sequencer thread running forever, and starve the other threads.

	if ( G_sequencer_is_busy > 0 ) {

		// Signal overload
		G_CPU_load_monitor   = MIR_RED;
		G_MIDI_load_monitor1 = MIR_RED;
		G_MIDI_load_monitor2 = MIR_RED;

		// Start the interval for overload clearing
		cyg_alarm_initialize( CPU_overloadAlarm_hdl,   cyg_current_time() + CPU_RED_INTERVAL,  0 );
		cyg_alarm_initialize( MIDI1_overloadAlarm_hdl, cyg_current_time() + MIDI_RED_INTERVAL, 0 );
		cyg_alarm_initialize( MIDI2_overloadAlarm_hdl, cyg_current_time() + MIDI_RED_INTERVAL, 0 );

		return timerDoneTicks;
	}

	// Set the sequencer busy flag to the number of runs we want it to do.
	// The flag will be decremented by one in driveSequencer().

	G_sequencer_is_busy = kickCt;

	// Reset the CPU load code and tell it we start a new sequencer run of kickCt kicks,
	// which will be allowed to take timerAvailTicks ticks.
	// This resets the CPU load indicator to 0%.
	// Tracks will stop being processed when we reach a near 100% figure.

	cpu_load_reset( timerStartTicks, timerAvailTicks );

	// Kick the sequencer semaphore with the given number of runs.
	// This will wake up the sequencer thread and make it call driveSequencer().

	for ( ix = 0; ix < kickCt; ix++ ) {
		cyg_semaphore_post( &sem_driveSequencer );
	}

	// Return number of timer ticks elapsed since last call.
	return timerDoneTicks;
}




////
//// TIMER EXECUTION THREAD
////
void timerInterrupt_thread( cyg_addrword_t index ){

	// Infinite loop for monitoring the request queues
	while (1){

		// Wait on the refresh semaphore
		cyg_semaphore_wait( &sem_driveSequencer );

		// Drive the sequencer.
		driveSequencer();
	}
}





// Drives the sequencer - interprets the timer tick
void driveSequencer(){

	// Timestamp update. Wraps after approx. 310 days @200 bpm, with a glitch in the NOTE OFF.
	G_MIDI_timestamp ++;

	// G_TTC_abs_value: The master time keeper between two matrix columns
	// Generate the absolute value of TIMER_TICK_COUNTER - which is the master time keeper
	// Moving this operation here solves the MIDI clock drift problem in master mode
	// TIMER_TICK_COUNTER: INCREMENT - wrap depends on TIMER_TICKS_PER_16TH - resolution
	G_TTC_abs_value = (G_TTC_abs_value % 12) + 1;

	// Send MIDI CLOCK on every other TTC starting at 1
	if ( (G_TTC_abs_value) % 2 == 1) {

		// Consider the clocksource setting - and throughput the clock when slaved - and told to do so
		if (	( G_clock_source == INT )
				||	(	( G_clock_source == EXT )
					&&	MIDICLOCK_PASSTHROUGH == TRUE )
			){

			MIDI_send( MIDI_CLOCK, MIDICLOCK_CLOCK, 0, 0 );
		}
	}


	// Interpret according to the G_run_bit of the sequencer
	switch( G_run_bit ){

		// G_run_bit is ON
		case ON:

			// Trigger Player at current G_TTC_abs_value position
			PLAYER_dispatch( G_TTC_abs_value );

#ifdef CONSTANT_BLINK
			// Nothing to do because done someplace else
#else
			// Global Blinker trigger
			if ( G_TTC_abs_value == 1 ){

				// G_master_blinker TOGGLE
				G_master_blinker ^= 1;
			}
#endif
			break;


		// G_run_bit is OFF
		case OFF:

			// G_master_tempo_TRACKER - tracks the tempo index
			if (G_TTC_abs_value == 1){

				G_tempo_tracker = (G_tempo_tracker % OCTDEF_MAX_TEMPO_MULTIPLIER) + 1;
				// d_iag_printf( "G_master_tempo_TRACKER:%d\n", G_master_tempo_TRACKER );

// Define blink behavior
#ifdef CONSTANT_BLINK
				// Nothing to do because done someplace else
#else
				// Show the update
				Page_requestRefresh();

				// Toggle the global blinker
				G_master_blinker ^= 1;
#endif

				// Play the pending MIDI OFFs at current timestamp -
				// - important in the EDIT mode, when notes are being sent out
				play_MIDI_queue( G_MIDI_timestamp );
			}
			break;
	} // G_run_bit switch

	// Capture CPU load of previous driveSequencer() run and print statistics every N runs.
	// ONLY FOR TESTING!! Should be commented out by default.
	// if ( G_sequencer_is_busy == 1 ) {
	// 	cpu_load_measure();
	// 	cpu_load_stats();
	// }

	if ( G_sequencer_is_busy > 0 ) {
		G_sequencer_is_busy--;
	}
}



// Updates the Timer refill for the current tempo value
void G_TIMER_REFILL_update(){

	G_TIMER_REFILL =

		// This outlines the logic behind the numbers.
		// ( CLOCK_FREQUENCY / (QUARTERNOTES_PER_ROW * 4 * TIMER_TICKS_PER_16TH) )  *  60 / G_master_tempo;
		// ( CLOCK_FREQUENCY / (OCTDEF_TIMER_TICKS_PER_16TH_AT4X * 16) )  *  60 / G_master_tempo;
		// ( 50000000 / (3 * 16) )  *  60 / G_master_tempo;
		62500000 / G_master_tempo;

	// Fill up Timer1: countdown duration -
	// Done in the Timer DSR - in order to not disrupt the ticking when using the rotary knobs.
}


// MIX TIMER HANDLING
void start_MIX_TIMER(){

	// Start MIX Timer
	MIX_TIMER 		= ON;
	EDIT_TIMER 		= OFF;
	TEMPO_TIMER 	= OFF;

	// Setup alarm for the MIX TIMER
	cyg_alarm_initialize(	alarm_hdl,
							cyg_current_time() + TIMEOUT_VALUE,
							0 );
}
void restart_MIX_TIMER(){
	// ..if it was started somewhere else
	if ( MIX_TIMER == ON ){
		start_MIX_TIMER();
	}
}


// EDIT TIMER HANDLING - this is not used anywhere just yet..
void start_EDIT_TIMER(){

	// Start MIX Timer
	EDIT_TIMER 		= ON;
	MIX_TIMER 		= OFF;
	TEMPO_TIMER		= OFF;

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


// G_master_tempo TIMER HANDLING - this is only used in NEMO
void start_TEMPO_TIMER(){

	// Start G_master_tempo Timer
	EDIT_TIMER 		= OFF;
	MIX_TIMER 		= OFF;
	TEMPO_TIMER		= ON;

	// Setup alarm for the G_master_tempo TIMER
	cyg_alarm_initialize(	alarm_hdl,
							cyg_current_time() + TIMEOUT_VALUE,
							0 );
}
void restart_TEMPO_TIMER() {
	// ..if it was started somewhere else
	if (TEMPO_TIMER != OFF) {
		start_TEMPO_TIMER();
	}
}

