/* OCTOPUS
 * Builds the panel logic in the init phase
 * 08.03.2004 genoQs Machines
 */

//int PANEL_matrix[PANEL_NROF_OBJECTS][PANEL_NROF_ATTRIBUTES][PANEL_NROF_FUNCTIONS];

//	Available functions are:			|TGGL	|SOLO	|CLEAR	|RNDMZ	|FLT	|RMX	|EFF	|ZOOM	|COPY	|PASTE	|MODIFY
void PANEL_build(){	
	
	// STEP: Populate the Attributes with relevant functions
	PANEL_matrix[STEP_S][ENTITY] = 0	|TGGL			|CLEAR	|RNDMZ							|ZOOM	|COPY	        	   ;	
	PANEL_matrix[STEP_S][VELOCITY] = 0							|RNDMZ													|MODIFY;
	PANEL_matrix[STEP_S][PITCH] = 	0							|RNDMZ													|MODIFY;
	PANEL_matrix[STEP_S][LENGTH] = 	0							|RNDMZ													|MODIFY;
	PANEL_matrix[STEP_S][START] = 	0							|RNDMZ													|MODIFY;
	PANEL_matrix[STEP_S][POSITION] = 0														;
	PANEL_matrix[STEP_S][DIRECTION] = 0														;
	PANEL_matrix[STEP_S][AMOUNT] = 	0														;
	PANEL_matrix[STEP_S][GROOVE] =	0							 														|MODIFY;
	PANEL_matrix[STEP_S][MIDICC] = 	0					|CLEAR	|RNDMZ								|ZOOM	|COPY		|MODIFY;
	PANEL_matrix[STEP_S][MIDICH] = 	0							 							;

	PANEL_matrix[STEP_M][ENTITY] = 	0	|TGGL			|CLEAR	|RNDMZ						;	
	PANEL_matrix[STEP_M][VELOCITY] = 0							|RNDMZ													|MODIFY;
	PANEL_matrix[STEP_M][PITCH] = 	0							|RNDMZ													|MODIFY;
	PANEL_matrix[STEP_M][LENGTH] = 	0							|RNDMZ													|MODIFY;
	PANEL_matrix[STEP_M][START] = 	0							|RNDMZ						;
	PANEL_matrix[STEP_M][POSITION] = 0														;
	PANEL_matrix[STEP_M][DIRECTION] = 0														;
	PANEL_matrix[STEP_M][AMOUNT] = 		0													;
	PANEL_matrix[STEP_M][GROOVE] =	0														;
	PANEL_matrix[STEP_M][MIDICC] = 	0														;
	PANEL_matrix[STEP_M][MIDICH] = 	0														;

	// TRACK_S: Populate the attributes with the relevant functions for TRACK SINGLE SELECTION
//	PANEL_matrix[TRACK][ENTITY] = 	0	|TGGL	|SOLO	|CLEAR	|RNDMZ	|FLT	|RMX	|EFF	|ZOOM	|COPY	|PASTE	|MODIFY;
	PANEL_matrix[TRACK_S][ENTITY] = 0	|TGGL  	|SOLO	|CLEAR	|RNDMZ			|RMX			  		|ZOOM  	|COPY	|MODIFY;
	PANEL_matrix[TRACK_S][VELOCITY]=0					|CLEAR	|RNDMZ									|ZOOM			|MODIFY;
	PANEL_matrix[TRACK_S][PITCH] = 	0					|CLEAR	|RNDMZ									|ZOOM			|MODIFY;
	PANEL_matrix[TRACK_S][LENGTH] = 0					|CLEAR	|RNDMZ									|ZOOM			|MODIFY;
	PANEL_matrix[TRACK_S][START] = 	0					|CLEAR	|RNDMZ									|ZOOM			|MODIFY;
	PANEL_matrix[TRACK_S][POSITION] = 0					|CLEAR	|RNDMZ													|MODIFY;
	PANEL_matrix[TRACK_S][DIRECTION]= 0																	|ZOOM			|MODIFY;
	PANEL_matrix[TRACK_S][AMOUNT] = 	0				|CLEAR	|RNDMZ									|ZOOM			|MODIFY;
	PANEL_matrix[TRACK_S][GROOVE] = 	0				|CLEAR	|RNDMZ									|ZOOM			|MODIFY;
	PANEL_matrix[TRACK_S][MIDICC] = 	0				|CLEAR	|RNDMZ									|ZOOM			|MODIFY;
	PANEL_matrix[TRACK_S][MIDICH] = 	0																				|MODIFY;

	// TRACK_M: Populate the attributes with the relevant functions for TRACK MULTI SELECTION
//	PANEL_matrix[TRACK][ENTITY] = 	0	|TGGL	|SOLO	|CLEAR	|RNDMZ	|FLT	|RMX	|EFF	|ZOOM	|COPY	|PASTE	|MODIFY;
	PANEL_matrix[TRACK_M][ENTITY] = 0	|TGGL  	|SOLO	|CLEAR	|RNDMZ	|FLT	|RMX						  	|COPY	|MODIFY;
	PANEL_matrix[TRACK_M][VELOCITY]=0							|RNDMZ	|FLT											|MODIFY;
	PANEL_matrix[TRACK_M][PITCH] = 	0							|RNDMZ	|FLT											|MODIFY;
	PANEL_matrix[TRACK_M][LENGTH] = 0							|RNDMZ													|MODIFY;
	PANEL_matrix[TRACK_M][START] = 	0							|RNDMZ													|MODIFY;
	PANEL_matrix[TRACK_M][POSITION] = 0							|RNDMZ													|MODIFY;
	PANEL_matrix[TRACK_M][DIRECTION]= 0							|RNDMZ			|RMX									|MODIFY;
	PANEL_matrix[TRACK_M][AMOUNT] = 	0																				|MODIFY;
	PANEL_matrix[TRACK_M][GROOVE] = 0																					|MODIFY;
	PANEL_matrix[TRACK_M][MIDICH] = 0																					|MODIFY;
	
	
	// PAGE: Populate the attributes with the relevant functions
//	PANEL_matrix[PAGE][ENTITY] = 	0	|TGGL	|SOLO	|CLEAR	|RNDMZ	|FLT	|RMX	|EFF	|ZOOM	|COPY	;	
	PANEL_matrix[PAGE][ENTITY] = 	0	|TGGL			|CLEAR 	|RNDMZ			|RMX			|ZOOM 	|COPY;
	PANEL_matrix[PAGE][VELOCITY] = 	0					|CLEAR								;
	PANEL_matrix[PAGE][PITCH] = 	0														;
	PANEL_matrix[PAGE][LENGTH] = 	0					|CLEAR								;
	PANEL_matrix[PAGE][START] = 	0					|CLEAR								;
	PANEL_matrix[PAGE][POSITION] = 	0														;
	PANEL_matrix[PAGE][DIRECTION]= 	0														;
	PANEL_matrix[PAGE][AMOUNT]= 	0														;
	PANEL_matrix[PAGE][GROOVE]= 	0														;
	PANEL_matrix[PAGE][MIDICC] = 	0														;
	PANEL_matrix[PAGE][MIDICH] = 	0														;


	// FX: Populate the attributes with the relevant functions
	PANEL_matrix[FX][ENTITY]= 		0							|RNDMZ	|FLT	|RMX	;	
	PANEL_matrix[FX][VELOCITY]=		0							|RNDMZ	|FLT				;
	PANEL_matrix[FX][PITCH] = 		0														;
	PANEL_matrix[FX][LENGTH] = 		0							|RNDMZ	|FLT				;
	PANEL_matrix[FX][START] = 		0							|RNDMZ						;
	PANEL_matrix[FX][POSITION]=		0							|RNDMZ						;
	PANEL_matrix[FX][DIRECTION]=	0							|RNDMZ			|RMX	;
	PANEL_matrix[FX][AMOUNT]=			0														;
	PANEL_matrix[FX][GROOVE]=		0														;
	PANEL_matrix[FX][MIDICC] = 		0														;
	PANEL_matrix[FX][MIDICH]=		0							|RNDMZ						;


	// SEQUENCE: Populate the attributes with the relevant functions
	PANEL_matrix[SEQUENCE][ENTITY]= 0					|CLEAR	|RNDMZ						;	
	PANEL_matrix[SEQUENCE][VELOCITY]=0														;
	PANEL_matrix[SEQUENCE][PITCH] = 0														;
	PANEL_matrix[SEQUENCE][LENGTH] = 0														;
	PANEL_matrix[SEQUENCE][START] = 0														;
	PANEL_matrix[SEQUENCE][POSITION]=0														;
	PANEL_matrix[SEQUENCE][DIRECTION]=0														;
	PANEL_matrix[SEQUENCE][MIDICH] = 0														;
	PANEL_matrix[SEQUENCE][MIDICC]=	0														;
	
}


// Returns a bit vector with the attributes of the target object
unsigned int PANEL_get_attributes_all( unsigned char target ) {

	unsigned int bitpattern = 0;
	unsigned char i = 0;
	
	// Build the bitpattern
	for (i=0; i<MATRIX_NROF_ROWS; i++) {

		if ( PANEL_matrix[target][i] != 0 )

			bitpattern |=  1 << i;
	}

	return bitpattern;
}

// Only works for track right now
unsigned int PANEL_get_trackAttributeMap() {
	return  (	( 1 << VELOCITY 	)
			| 	( 1 << PITCH 		)
			|	( 1 << LENGTH 		)
			| 	( 1 << START 		)
			| 	( 1 << DIRECTION	)
			|	( 1 << GROOVE		)
			|	( 1 << MIDICC 		)
		    );
}


// Only works for step right now
unsigned int PANEL_get_stepAttributeMap() {
	return  (	( 1 << VELOCITY 	)
			| 	( 1 << PITCH  		)
			|	( 1 << LENGTH  		) 
			|	( 1 << START 		)
		    );
}



// Takes an object class and a function and returns bitpattern of relevant attributes
unsigned int PANEL_getAttributeMap( unsigned int object, unsigned int function) {
	
	unsigned int i=0, bitpattern=0;
	
	// Build the bitpattern
	for (i=0; i<MATRIX_NROF_ROWS; i++) {

		if ((PANEL_matrix[object][i] & function) != 0){

			bitpattern |= 1<<i;
		}
	}

	return bitpattern;
}

