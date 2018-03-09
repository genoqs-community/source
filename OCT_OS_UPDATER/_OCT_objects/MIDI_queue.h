
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
extern void 		MIDIevent_init( MIDIeventstruct* in_event );


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

// Pops an empty event (by definition) from the MIDI_stack. If none left, NULL returned.
MIDIeventstruct* 	MIDI_stack_pop(){

	// Remember the element at stack top
	MIDIeventstruct* result = MIDI_stack;
	
	// Point stack top to the next event
	MIDI_stack = MIDI_stack->next;

	if ( result != NULL ){ 
		
		MIDI_stack_counter --;
	}
	
//	// d_iag_printf( "stack pop:\n" );
//	if ( result == NULL ){ 
//		// d_iag_printf( "NULL\n" ); 
//	}
//	else{
//		// MIDIevent_print( result );
//	}
	
	return result;
}

// Pushes the passed event onto the stack after cleaning it
void MIDI_stack_push( MIDIeventstruct* in_event ){

	// Do not deal with NULL arguments.
	if ( in_event == NULL ) return;

	// Clean old event content
	MIDIevent_init( in_event );
	
	// Put the event at top of stack
	in_event->next = MIDI_stack;

	// Point stack top to it.
	MIDI_stack = in_event;
	
	MIDI_stack_counter ++;

	// d_iag_printf( "stack push:%d\n", MIDI_stack_counter );
	// MIDIevent_print( in_event );
}



///////////////////////
// QUEUE IMPLEMENTATION
 
// MIDI queue main pointer, to be used as the main reference.
MIDIeventstruct* 	MIDI_queue = NULL;


// Adds event to the queue, according to its priority
void MIDI_queue_insert( MIDIeventstruct* in_event ){
	
	unsigned int i = 0;
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
			for( i=0; i<MIDI_queue_counter+1; i++ ){
				
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

	// d_iag_printf( "queue insert:\n" );
	// MIDIevent_print( in_event );
	
	// Increment the queue counter
	MIDI_queue_counter ++;	
}


//// Prints the contents of the MIDI queue
//void MIDI_queue_print(){
//
//	unsigned int i = 0;
//	MIDIeventstruct* pointer = MIDI_queue;
//	
//	d_iag_printf( "_____MIDI_queue:%d\n", MIDI_queue_counter );
//	if (pointer == NULL){
//		d_iag_printf( "..empty\n" );	
//		return;
//	}
//
//	// Show the real contents of the MIDI queue
//	for( i=0; i<MIDI_queue_counter; i++ ){
//
//		MIDIevent_print( pointer );
//		if ( pointer != NULL ){
//			pointer = pointer->next;
//		}
//	}
//}


// Return the first event with the corresponding or smaller timestamp 
// ..or otherwise NULL if no event available..
MIDIeventstruct* 	MIDI_queue_remove( unsigned int in_timestamp ) {
	
	// ..removing it from the queue. 
	// Event should be then placed back on the stack by the consumer.
	
	MIDIeventstruct* result 	= NULL;
	
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
		
	// Result contains the return value
	return result;
}


//// Removes the first event in the queue
//MIDIeventstruct* 	MIDI_queue_remove_head(){
//	
//	MIDIeventstruct* result = NULL;
//
//	if ( MIDI_queue == NULL ) {
//		
//		return NULL;
//	}
//	else {
//
//		result = MIDI_queue;
//		MIDI_queue = MIDI_queue->next;
//	
//		MIDI_queue_counter --;
//		
//		return result;
//	}
//}
//


// Removes the first event in the queue
MIDIeventstruct* 	MIDI_queue_remove_head(){
	
	MIDIeventstruct* result = MIDI_queue;

	if ( result != NULL ) {
		
		MIDI_queue = MIDI_queue->next;	
		MIDI_queue_counter --;
	}

	return result;
}




