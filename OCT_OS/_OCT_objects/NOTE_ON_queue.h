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




// Implements the priority queue used for NOTE operations.
// Implements the stack used to keep the free NOTE event slots.

extern signed char adjust_PIT_to_track( unsigned char in_PIT, Trackstruct* target_track );
extern signed char adjust_VEL_to_track( unsigned char in_VEL, Trackstruct* target_track );

// Type of the events in the NOTE queue
typedef struct NOTE_event{

	Stepstruct*  			target_step;
	Trackstruct*			target_track;
	unsigned int 			timestamp;
	signed char				velocity;
	signed char				pitch;
	unsigned char			length;
	unsigned char			start;
	struct NOTE_event* 		prev;
	struct NOTE_event*		next;

} NOTEeventstruct;


// The production value should be 200 - eventually!
#define 	 NOTE_NROF_EVENTS		50

// NOTE event repository, used as memory fence around the NOTE event resources.
NOTEeventstruct 	NOTEevent_repository[ NOTE_NROF_EVENTS ];
extern void 		NOTEevent_init( NOTEeventstruct* in_event );


//// Print the contents of a queue element
//void NOTEevent_print( NOTEeventstruct* in_event ){
//
//	// Make sure not not hang up on port misconfig
//	if ( OCTDEF_UART2_MODE != USB ) return;
//
//	d_iag_printf( "__ME ID:%d ts:%d vel:%d pit:%d len:%d sta:%d pv:%d nx:%d\n",
//		(unsigned int) in_event,
//		in_event->timestamp,
//		in_event->velocity,
//		in_event->pitch,
//		in_event->length,
//		in_event->start,
//		(unsigned int) in_event->prev,
//		(unsigned int) in_event->next
//		);
//}


// Fills a given NOTE event with the passed data
void NOTEevent_fill(
						NOTEeventstruct* target_event,
						Stepstruct* 	in_step,
						Trackstruct* 	in_track,
						unsigned int  	in_timestamp,
						unsigned char   in_velocity,
						unsigned char   in_pitch,
						unsigned char 	in_length,
						unsigned char 	in_start			){

	// Prevent lockup on NULL pointer
	if ( target_event == NULL ) return;

	target_event->target_step 	= in_step;
	target_event->target_track	= in_track;
	target_event->timestamp		= in_timestamp;
	target_event->velocity		= in_velocity;
	target_event->pitch 		= in_pitch;
	target_event->length     	= in_length;
	target_event->start			= in_start;
}



///////////////////////
// STACK IMPLEMENTATION

// NOTE stack main pointer to be used as main reference.
NOTEeventstruct* 	NOTE_stack = NULL;


// Pops an empty event (by definition) from the NOTE_stack. If none left, NULL returned.
NOTEeventstruct* 	NOTE_stack_pop(){

	// Remember the element at stack top
	NOTEeventstruct* result = NOTE_stack;

	// Point stack top to the next event
	if ( NOTE_stack != NULL ){
		NOTE_stack = NOTE_stack->next;
	}

	return result;
}



// Pushes the passed event onto the stack after cleaning it
void NOTE_stack_push( NOTEeventstruct* in_event ){

	// Do not deal with NULL arguments.
	if ( in_event == NULL ) return;

	// Clean old event content
	NOTEevent_init( in_event );

	// Put the event at top of stack
	in_event->next = NOTE_stack;

	// Point stack top to it.
	NOTE_stack = in_event;
}



///////////////////////
// QUEUE IMPLEMENTATION

// NOTE queue main pointer, to be used as the main reference.
NOTEeventstruct* 	NOTE_queue = NULL;

// Adds event to the queue - always at head position.
void NOTE_queue_insert( NOTEeventstruct* in_event ){

	if ( in_event == NULL )	return;

	// Insert event as queue head
	in_event->prev = NULL;
	in_event->next = NOTE_queue;
	NOTE_queue = in_event;

	// Set the prev link on the old head
	if ( in_event->next != NULL ){
		(in_event->next)->prev = in_event;
	}

	// Increment the queue counter
	NOTE_queue_counter ++;
}



//// Prints the contents of the NOTE queue
//void NOTE_queue_print(){
//
//	// Make sure not not hang up on port mis-config
//	if ( OCTDEF_UART2_MODE != USB ) return;
//
//	unsigned int i = 0;
//	NOTEeventstruct* pointer = NOTE_queue;
//
//	d_iag_printf( "_____print NOTE_queue length:%d\n", NOTE_queue_counter );
//
//	if (pointer == NULL){
//
//		d_iag_printf( "..empty\n" );
//		return;
//	}
//
//	// Show the real contents of the NOTE queue
//	for( i=0; i < NOTE_queue_counter; i++ ){
//
//		NOTEevent_print( pointer );
//		if ( pointer != NULL ){
//			pointer = pointer->next;
//		}
//	}
//
//	d_iag_printf( "..WHILE NOTE_QUEUE\n" );
//
//	// And now while-loop wise:
//	pointer = NOTE_queue;
//	i=0;
//	while( pointer != NULL ){
//
//		// Show the event
//		NOTEevent_print( pointer );
//
//		// Advance the pointer
//		pointer = pointer->next;
//
//		i++;
//		if ( i>NOTE_NROF_EVENTS ) break;
//	}
//
//	// And now the stack
//	d_iag_printf( "..WHILE NOTE STACK\n" );
//
//	// And now while-loop wise:
//	pointer = NOTE_stack;
//	i=0;
//	while( pointer != NULL ){
//
//		// Show the event
//		NOTEevent_print( pointer );
//
//		// Advance the pointer
//		pointer = pointer->next;
//
//		i++;
//		if ( i>NOTE_NROF_EVENTS ) break;
//	}
//}



// Return the first event with the given pitch ..or otherwise NULL if no event available
NOTEeventstruct* NOTE_queue_remove( signed char in_pitch ) {

	unsigned int i = 0;

	// Cursor G_scanning through the queue
	NOTEeventstruct* cursor = NOTE_queue;
	NOTEeventstruct* result = NULL;

	// Scan queue..
	for ( i=0; i < NOTE_queue_counter; i++ ){

		// Required pitch was not found in queue
		if ( cursor == NULL ) break;

		// Desired pitch was found at cursor position
		// Take the element out of the queue and mark it as result
		if ( cursor->pitch == in_pitch ){

			// Found the wanted element
			result = cursor;

			// Remove the element from the queue
			//
			// Wanted element is the queue head
			if ( result == NOTE_queue ){

				// Move the queue head, because old head will be returned
				NOTE_queue = NOTE_queue->next;

				// There is no prev element for the head, so mark it null
				if ( NOTE_queue != NULL ){
					NOTE_queue->prev = NULL;
				}
			}
			// Wanted element is in the tail of the queue
			else{
				// Remove the element under cursor from the queue
				(result->prev)->next = result->next;

				if ( result->next != NULL ){
					(result->next)->prev = result->prev;
				}
			}

			// Decrement the queue counter
			NOTE_queue_counter --;

			break;
		} // cursor hit wanted element

		// Move cursor to the next element in the queue
		cursor = cursor->next;

	} //  iterate through the queue

	// Clean the queue references from the result element
	result->prev = NULL;
	result->next = NULL;

	return result;
}







