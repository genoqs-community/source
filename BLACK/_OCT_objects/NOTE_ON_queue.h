
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
	
	if ( result != NULL ) NOTE_stack_counter --;

//	d_iag_printf( "stack pop:\n" );
//	if ( result == NULL ){ 
//		d_iag_printf( "NULL\n" ); 
//	}
//	else{
//		NOTEevent_print( result );
//	}
	
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
	
	NOTE_stack_counter ++;

	// d_iag_printf( "stack push:%d\n", NOTE_stack_counter );
	// NOTEevent_print( in_event );
}



///////////////////////
// QUEUE IMPLEMENTATION
 
// NOTE queue main pointer, to be used as the main reference.
NOTEeventstruct* 	NOTE_queue = NULL;

// Adds event to the queue, according to its priority
void NOTE_queue_insert( NOTEeventstruct* in_event ){
	
	if ( in_event == NULL )	return;

	// d_iag_printf( "queue:%d insert:%d\n", 
	// 	(unsigned int) NOTE_queue, (unsigned int) in_event );
	//NOTEevent_print( in_event );

	// Insert event as queue head
	in_event->next = NOTE_queue;
	in_event->prev = NULL;
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
//	// Make sure not not hang up on port misconfig
//	if ( OCTDEF_UART2_MODE != USB ) return;
//	
//	unsigned int i = 0;
//	NOTEeventstruct* pointer = NOTE_queue;
//	
//	d_iag_printf( "_____print NOTE_queue length:%d\n", NOTE_queue_counter );
//	if (pointer == NULL){
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
//}


// Return the first event with the given pitch ..or otherwise NULL if no event available
NOTEeventstruct* NOTE_queue_remove( signed char in_pitch ) {
	
	unsigned int i = 0;	
	
	// Step pointer to be returned
	// Stepstruct* result = NULL;

	// Cursor scanning through the queue
	NOTEeventstruct* cursor = NOTE_queue;
	NOTEeventstruct* result = NULL;

	// Scan queue..
	for ( i=0; i < NOTE_queue_counter; i++ ){

		if (	( cursor == NULL )
			||	( NOTE_queue_counter == 0 )
			){ 
				break;
		}
			
		// Look for pitch event in the queue
		if ( cursor->pitch == in_pitch ){

			// Take the element out of the queue and mark it as result
			if ( cursor == NOTE_queue ){
				NOTE_queue = cursor->next;
			}
			else{
				(cursor->prev)->next = cursor->next;
			}
			
			result = cursor;

			// Decrement the queue counter
			NOTE_queue_counter --;	

			break;
		} // cursor hit wanted element
		
		// Move cursor
		cursor = cursor->next;
		
	} //  iterate through the queue

	return result;
}


// Removes the first event in the queue
NOTEeventstruct* 	NOTE_queue_remove_head(){
	
	NOTEeventstruct* result = NULL;

	if ( NOTE_queue == NULL ) {
		
		return NULL;
	}
	else {

		result = NOTE_queue;
		NOTE_queue = NOTE_queue->next;
	
		return result;
	}
}






