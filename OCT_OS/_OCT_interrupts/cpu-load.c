#ifndef CPULOADCODE_H_
#define CPULOADCODE_H_


#include "../_OCT_interrupts/cpu-load.h"


/**
 * Returns true if the CPU load for the current sequencer tick reached the maximum load, or false otherwise.
 * For safety, max load is set at a value below 100%.
 * When we reach max CPU load, driveSequencer() should stop processing tracks.
 *
 * @return True at max load, false otherwise.
 * @author Robert van der Kamp
 */
int cpu_load_at_max( void )
{
	return (G_cpu_load_perc >= 85);
}


/**
 * Returns true if the CPU load for the current sequencer tick reached a nearly-max load, or false otherwise.
 *
 * @author Robert van der Kamp
 */
int cpu_load_nearly_at_max( void )
{
	return ((G_cpu_load_perc >= 80) && (G_cpu_load_perc < 85));
}



/**
 * Resets the CPU load measuring fields. This sets the cpu-load indicator to 0%.
 * If the previous sequencer tick reached (near-)max CPU load, the overload indicator
 * LEDs are cleared by setting the dedicated alarm.
 *
 * @note To be called at begin of driveSequencer().
 *
 * @author Robert van der Kamp
 */
void cpu_load_reset( unsigned int timerStartTicks, unsigned int timerPeriodTicks )
{
	// Reset load indicator, and get the count-down timer start value.
	// G_TIMER_REFILL may change during the sequencer tick.

	G_cpu_load_perc = 0;
	G_cpu_load_max_perc = 0;
	G_cpu_load_timer_start_ticks = timerStartTicks;
	G_cpu_load_timer_period_ticks = timerPeriodTicks;
}


/**
 * Measures the CPU load for the sequencer tick currently being processed.
 * This results in global G_cpu_load_perc to be updated. It indicates the amount of CPU spent
 * on the current sequencer tick as an percentage. At values > 100% we reach CPU overload status.
 *
 * @see cpu_load_at_max(), cpu_load_nearly_at_max()
 * @author Robert van der Kamp
 */
void cpu_load_measure( void )
{
	// Read the timer count-down register, the lower 32-bit part.
	// This register starts with value G_cpu_load_timer_refill, and then counts down.
	// If it reaches zero we know G_cpu_load_timer_refill timer ticks have passed and we're at max CPU load.

	unsigned int ticksLeft, ticksDone;
	HAL_READ_UINT32( E7T_TCNT1, ticksLeft );
	ticksDone = G_cpu_load_timer_start_ticks - ticksLeft;

	// G_cpu_load_perc is a percentage value indicating CPU load. > 100% is serious overload.
	G_cpu_load_perc = (ticksDone * 100) / G_cpu_load_timer_period_ticks;

	// Update the max percentage reached in this sequencer run.
	if ( G_cpu_load_perc > G_cpu_load_max_perc ) G_cpu_load_max_perc = G_cpu_load_perc;
}


/**
 * Provides CPU load statistics for debugging and measuring means only.
 * It prints current, average and max CPU load over a number of sequencer ticks.
 *
 * @note To be called at the end of driveSequencer().
 * @note DO NOT CALL THIS METHOD for a final version.
 *
 * @author Robert van der Kamp
 */
void cpu_load_stats( void )
{
	static int cnt = 0;
	static unsigned int avgPerc = 0;
	static unsigned int avgMax = 0;

	avgPerc += G_cpu_load_perc;
	if ( G_cpu_load_perc > avgMax ) avgMax = G_cpu_load_perc;

	if ( ++cnt == 300 ) {
		//d_iag_printf("cpu=%03d avg=%03d max=%03d %d %d\n", G_cpu_load_perc, avgPerc/300, avgMax, G_cpu_load_timer_period_ticks, G_master_tempo );
		cnt = 0;
		avgPerc = 0;
		avgMax = 0;
	}
}



#endif /*CPULOADCODE_H_*/
