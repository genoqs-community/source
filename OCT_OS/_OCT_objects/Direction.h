
#ifndef DIRECTION_H_
#define DIRECTION_H_


// Trigger stack structure
typedef struct ts {

	// Trigger stack in the natural orde
	unsigned char trigger[10];

	// probability of moving fwd or bwd
	unsigned char certainty_next;

	// bit for fully random next position - overrides certainty
	unsigned char randomizer; 
} Trigger_Stack;


// Direction structure
typedef struct td {
	Trigger_Stack trigger_set[17];
} Track_Direction;



#endif /*DIRECTION_H_*/
