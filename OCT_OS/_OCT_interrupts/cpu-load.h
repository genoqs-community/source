#ifndef CPULOADHDR_H_
#define CPULOADHDR_H_

#include "../_OCT_global/includes-declarations.h"


extern unsigned char G_cpu_load_perc;
extern unsigned char G_cpu_load_max_perc;
extern unsigned int	G_cpu_load_timer_start_ticks;
extern unsigned int	G_cpu_load_timer_period_ticks;





// Prototypes.
extern void cpu_load_measure( void );
extern void cpu_load_reset( unsigned int timerStartTicks, unsigned int timerPeriodTicks );
extern void cpu_load_stats( void );
extern int cpu_load_at_max( void );
extern int cpu_load_nearly_at_max( void );


#endif /*CPULOADHDR_H_*/
