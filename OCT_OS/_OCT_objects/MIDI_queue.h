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



// Implements the priority queue used for MIDI operations.
// Implements the stack used to keep the free MIDI event slots.



// Type of the events in the MIDI queue
typedef struct MIDIevent{

	unsigned int 		timestamp;
	unsigned char		type;
	unsigned char		val0;
	unsigned char		val1;
	unsigned char		val2;
	struct MIDIevent* 	prev;
	struct MIDIevent*	next;

} MIDIeventstruct;




// The production value should be 2000!
#define 	 MIDI_NROF_EVENTS		2000

// MIDI event repository, used as memory fence around the MIDI event resources.
MIDIeventstruct 	MIDIevent_repository[ MIDI_NROF_EVENTS ];
extern void 		MIDIevent_init( 	MIDIeventstruct* in_event );
extern 	void		MIDI_send_event(	MIDIeventstruct* );



//// Prints out the content of the given MIDI Event
//void MIDIevent_print( MIDIeventstruct* in_event ){
//
//	d_iag_printf( "___ME ID:%d ts:%d ty:%d v0:%d v1:%d v2:%d ne:%d\n",
//		(unsigned int) in_event,
//		in_event->timestamp,
//		in_event->type,
//		in_event->val0,
//		in_event->val1,
//		in_event->val2,
//		(unsigned int) in_event->next
//		);
//}


// Fills a given MIDI event with the passed data
void MIDIevent_fill( 	MIDIeventstruct* target_event,
						unsigned int in_timestamp,
						unsigned char in_type,
						unsigned char in_val0,
						unsigned char in_val1,
						unsigned char in_val2				){

	target_event->timestamp = in_timestamp;
	target_event->type		= in_type;
	target_event->val0		= in_val0;
	target_event->val1		= in_val1;
	target_event->val2     	= in_val2;
}


///////////////////////
// STACK IMPLEMENTATION

// MIDI stack main pointer to be used as main reference.
MIDIeventstruct* 	MIDI_stack = NULL;


// Pops an empty event (by definition) from the MIDI_stack.
// If none left available, NULL returned.
MIDIeventstruct* 	MIDI_stack_pop()
{
	cyg_mutex_lock( &mutex_midi_output_stack );

	// Remember the element at stack top
	MIDIeventstruct* result = MIDI_stack;

	// Point stack top to the next event
	if ( MIDI_stack != NULL ){
		MIDI_stack = MIDI_stack->next;
	}

	if ( result != NULL ){
		// Disconnect result from the stack
		result->next = NULL;

		MIDI_stack_counter --;
	}

	cyg_mutex_unlock( &mutex_midi_output_stack );

	return result;
}


// Pushes the passed event onto the stack, but cleans it first
void MIDI_stack_push( MIDIeventstruct* in_event )
{
	// Do not deal with NULL arguments.
	if ( in_event == NULL ) return;

	// Clean old event content
	MIDIevent_init( in_event );

	cyg_mutex_lock( &mutex_midi_output_stack );

	// Put the event at top of stack
	in_event->next = MIDI_stack;

	// Point stack top to it.
	MIDI_stack = in_event;

	MIDI_stack_counter ++;

	cyg_mutex_unlock( &mutex_midi_output_stack );
}



///////////////////////
// QUEUE IMPLEMENTATION

// MIDI queue main pointer, to be used as the main reference.
MIDIeventstruct* 	MIDI_queue = NULL;


// Adds event to the queue, according to its priority which is timestamp based
void MIDI_queue_insert( MIDIeventstruct* in_event )
{
	unsigned int i = 0;

	// Validate input
	if ( in_event == NULL ) return;

	cyg_mutex_lock( &mutex_midi_output_queue );

	MIDIeventstruct* pointer = MIDI_queue;

	// No elements in the queue yet - just an empty lead pointer
	if (MIDI_queue == NULL){

		// Simply add the element to the queue, as the head.
		MIDI_queue 	= in_event;
		in_event->next = NULL;
	}
	else {

		// Insert event at the head of the queue
		if ( MIDI_queue->timestamp > in_event->timestamp ){

			in_event->next 	= MIDI_queue;
			MIDI_queue 		= in_event;
		}
		else{

			// Search for the insert position.
			// Hard bound (vs. while-loop) to avoid infinite loop
			for( i=0; i < MIDI_queue_counter + 1 ; i++ ){

				// Last element in the queue reached or insert position found
				if (	(pointer->next == NULL)
					||	(pointer->next->timestamp > in_event->timestamp)
					){

					// pointer->next is now the insert position for in_event.
					in_event->next 	= pointer->next;
					pointer->next 	= in_event;

					break;
				}

				// Advance the pointer
				pointer = pointer->next;
			} // event iterator
		} // inserting element in the non-head part of queue
	} // MIDI queue is not empty

	// Increment the queue counter
	MIDI_queue_counter ++;

	cyg_mutex_unlock( &mutex_midi_output_queue );
}


//// Prints the contents of the MIDI queue
//void MIDI_queue_print(){
//
//	unsigned int i = 0;
//	MIDIeventstruct* pointer = NULL;
//
//	d_iag_printf( "_____MIDI_queue:%d\n", MIDI_queue_counter );
//
//	pointer = MIDI_queue;
//	i = 0;
//	if (pointer == NULL){
//		d_iag_printf( "..empty\n" );
//		return;
//	}
//
//	pointer = MIDI_queue;
//	while( pointer != NULL ){
//
//		i ++;
//		d_iag_printf("\n\nndx:%d", i );
//		MIDIevent_print( pointer );
//
//		pointer = pointer->next;
//	}
//
//	d_iag_printf( "_____MIDI_STACK:%d\n", MIDI_stack_counter );
//
//	pointer = MIDI_stack;
//	i = 0;
//	if (pointer == NULL){
//		d_iag_printf( "..empty\n" );
//		return;
//	}
//
//	pointer = MIDI_stack;
//	while( pointer != NULL ){
//
//		i ++;
//		d_iag_printf("\n\nndx:%d", i );
//		MIDIevent_print( pointer );
//
//		pointer = pointer->next;
//	}
//}


// Return the first event with the corresponding or smaller timestamp
// ..or otherwise NULL if no event available..
MIDIeventstruct* 	MIDI_queue_remove( unsigned int in_timestamp )
{
	// ..removing it from the queue.
	// Event should be then placed back on the stack by the consumer.

	MIDIeventstruct* result 	= NULL;

	cyg_mutex_lock( &mutex_midi_output_queue );

	// No elements in the queue yet - just an empty lead pointer
	if (MIDI_queue == NULL){

		// Return the bitter truth
		result = NULL;
	}
	// Remove event at the head of the queue
	else if ( MIDI_queue->timestamp <= in_timestamp ){

			result  		= MIDI_queue;
			MIDI_queue 		= MIDI_queue->next;
	}

	// Decrement the queue counter
	if (result != NULL) {

		MIDI_queue_counter --;

		// d_iag_printf( "queue remove:%d\n", MIDI_queue_counter );
		// MIDIevent_print( result );
	}

	cyg_mutex_unlock( &mutex_midi_output_queue );

	// Result contains the return value
	return result;
}


// Checks whether input is a NOTE OFF message. If not, returns NUL
MIDIeventstruct* validate_MIDIevent_NOTEOFF( MIDIeventstruct* target_event ){

	MIDIeventstruct* result = NULL;

	// Do not operate on NULLs
	if ( target_event == NULL ){

		result = NULL;
	}
	// Execute the type checking: NOTE with vel 0
	else if (	( target_event->type == MIDI_NOTE )
			&&	( target_event->val2 == 0 )
		){

		result = target_event;
	}

	return result;
}



// Removes the first event in the queue
// NO LOCKS HERE! Called from within protected MIDI_queue_flush().
MIDIeventstruct* MIDI_queue_remove_head()
{
	MIDIeventstruct* result = MIDI_queue;

	if ( result != NULL ) {

		// Adjust the queue head
		MIDI_queue = MIDI_queue->next;
		MIDI_queue_counter --;
	}

	return result;
}



// Simply play all off notes that are pending in the MIDI queue
// and drop any possible ON Notes
void MIDI_queue_flush(){

	unsigned char i = 0;
	unsigned char queue_size = 0; // Stores the initial MIDI queue size

	MIDIeventstruct* eventPt = NULL;
	MIDIeventstruct* offEventPt = NULL;

	cyg_mutex_lock( &mutex_midi_output_queue );

	// Play all still pending MIDI OFF data
	queue_size = MIDI_queue_counter;	// Initial queue size
	for( i=0; i < queue_size; i++ ){

		// Get the head of the MIDI queue
		eventPt = MIDI_queue_remove_head();

		//d_iag_printf( "\ncurrent event from queue:\n" );
		//MIDIevent_print( off_event );

		// Make sure we don't continue on NULL pointers..
		if ( eventPt == NULL ) continue;

		cyg_mutex_unlock( &mutex_midi_output_queue );

		// Otherwise need a check here that we are not playing additional ON notes
		// ..and that only the OFF notes are played.
		offEventPt = validate_MIDIevent_NOTEOFF( eventPt );

		// Send the MIDI off event to the MIDI port.
		// The send function will push the event back on the stack, when done.

		if ( offEventPt  ){
			MIDI_send_event( offEventPt );
		}

		// Return all other (ignored) events to the stack right away.
		else {
			MIDI_stack_push( eventPt );
		}

		cyg_mutex_lock( &mutex_midi_output_queue );

	} // MIDI OFF queue iterator

	cyg_mutex_unlock( &mutex_midi_output_queue );
}




