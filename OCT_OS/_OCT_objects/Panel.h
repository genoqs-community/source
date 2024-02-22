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
 * Builds the panel logic in the init phase
 * 08.03.2004 genoQs Machines
 */

//int G_PANEL_matrix[PANEL_NROF_OBJECTS][PANEL_NROF_ATTRIBUTES][PANEL_NROF_FUNCTIONS];

//	Available functions are:			|TGGL	|SOLO	|CLEAR	|RNDMZ	|FLT	|RMX	|EFF	|ZOOM	|COPY	|PASTE	|MODIFY
void PANEL_build(){	
	
	// STEP: Populate the Attributes with relevant functions
	G_PANEL_matrix[STEP_S][ENTITY] = 0	|TGGL			|CLEAR	|RNDMZ							|ZOOM	|COPY	        	   ;	
	G_PANEL_matrix[STEP_S][VELOCITY] = 0							|RNDMZ													|MODIFY;
	G_PANEL_matrix[STEP_S][PITCH] = 	0							|RNDMZ													|MODIFY;
	G_PANEL_matrix[STEP_S][LENGTH] = 	0							|RNDMZ													|MODIFY;
	G_PANEL_matrix[STEP_S][START] = 	0							|RNDMZ													|MODIFY;
	G_PANEL_matrix[STEP_S][POSITION] = 0														;
	G_PANEL_matrix[STEP_S][DIRECTION] = 0														;
	G_PANEL_matrix[STEP_S][AMOUNT] = 	0																					|MODIFY;		
	G_PANEL_matrix[STEP_S][GROOVE] =	0							 														|MODIFY;
	G_PANEL_matrix[STEP_S][MIDICC] = 	0					|CLEAR	|RNDMZ								|ZOOM	|COPY		|MODIFY;
	G_PANEL_matrix[STEP_S][MIDICH] = 	0							 							;

	G_PANEL_matrix[STEP_M][ENTITY] = 	0	|TGGL			|CLEAR	|RNDMZ						;	
	G_PANEL_matrix[STEP_M][VELOCITY] = 0							|RNDMZ													|MODIFY;
	G_PANEL_matrix[STEP_M][PITCH] = 	0							|RNDMZ													|MODIFY;
	G_PANEL_matrix[STEP_M][LENGTH] = 	0							|RNDMZ													|MODIFY;
	G_PANEL_matrix[STEP_M][START] = 	0							|RNDMZ						;
	G_PANEL_matrix[STEP_M][POSITION] = 0														;
	G_PANEL_matrix[STEP_M][DIRECTION] = 0														;
	G_PANEL_matrix[STEP_M][AMOUNT] = 		0													;
	G_PANEL_matrix[STEP_M][GROOVE] =	0														;
	G_PANEL_matrix[STEP_M][MIDICC] = 	0														;
	G_PANEL_matrix[STEP_M][MIDICH] = 	0														;

	// TRACK_S: Populate the attributes with the relevant functions for TRACK SINGLE SELECTION
//	G_PANEL_matrix[TRACK][ENTITY] = 	0	|TGGL	|SOLO	|CLEAR	|RNDMZ	|FLT	|RMX	|EFF	|ZOOM	|COPY	|PASTE	|MODIFY;
	G_PANEL_matrix[TRACK_S][ENTITY] = 0	|TGGL  	|SOLO	|CLEAR	|RNDMZ			|RMX			  		|ZOOM  	|COPY	|MODIFY;
	G_PANEL_matrix[TRACK_S][VELOCITY]=0					|CLEAR	|RNDMZ									|ZOOM			|MODIFY;
	G_PANEL_matrix[TRACK_S][PITCH] = 	0					|CLEAR	|RNDMZ									|ZOOM			|MODIFY;
	G_PANEL_matrix[TRACK_S][LENGTH] = 0					|CLEAR	|RNDMZ									|ZOOM			|MODIFY;
	G_PANEL_matrix[TRACK_S][START] = 	0					|CLEAR	|RNDMZ									|ZOOM			|MODIFY;
	G_PANEL_matrix[TRACK_S][POSITION] = 0					|CLEAR	|RNDMZ													|MODIFY;
	G_PANEL_matrix[TRACK_S][DIRECTION]= 0																	|ZOOM			|MODIFY;
	G_PANEL_matrix[TRACK_S][AMOUNT] = 	0				|CLEAR	|RNDMZ									|ZOOM			|MODIFY;
	G_PANEL_matrix[TRACK_S][GROOVE] = 	0				|CLEAR	|RNDMZ									|ZOOM			|MODIFY;
	G_PANEL_matrix[TRACK_S][MIDICC] = 	0				|CLEAR	|RNDMZ									|ZOOM			|MODIFY;
	G_PANEL_matrix[TRACK_S][MIDICH] = 	0																				|MODIFY;

	// TRACK_M: Populate the attributes with the relevant functions for TRACK MULTI SELECTION
//	G_PANEL_matrix[TRACK][ENTITY] = 	0	|TGGL	|SOLO	|CLEAR	|RNDMZ	|FLT	|RMX	|EFF	|ZOOM	|COPY	|PASTE	|MODIFY;
	G_PANEL_matrix[TRACK_M][ENTITY] = 0	|TGGL  	|SOLO	|CLEAR	|RNDMZ	|FLT	|RMX						  	|COPY	|MODIFY;
	G_PANEL_matrix[TRACK_M][VELOCITY]=0							|RNDMZ	|FLT											|MODIFY;
	G_PANEL_matrix[TRACK_M][PITCH] = 	0							|RNDMZ	|FLT											|MODIFY;
	G_PANEL_matrix[TRACK_M][LENGTH] = 0							|RNDMZ													|MODIFY;
	G_PANEL_matrix[TRACK_M][START] = 	0							|RNDMZ													|MODIFY;
	G_PANEL_matrix[TRACK_M][POSITION] = 0							|RNDMZ													|MODIFY;
	G_PANEL_matrix[TRACK_M][DIRECTION]= 0							|RNDMZ			|RMX									|MODIFY;
	G_PANEL_matrix[TRACK_M][AMOUNT] = 	0																				|MODIFY;
	G_PANEL_matrix[TRACK_M][GROOVE] = 0																					|MODIFY;
	G_PANEL_matrix[TRACK_M][MIDICH] = 0																					|MODIFY;
	
	
	// PAGE: Populate the attributes with the relevant functions
//	G_PANEL_matrix[PAGE][ENTITY] = 	0	|TGGL	|SOLO	|CLEAR	|RNDMZ	|FLT	|RMX	|EFF	|ZOOM	|COPY	;	
	G_PANEL_matrix[PAGE][ENTITY] = 	0	|TGGL			|CLEAR 	|RNDMZ			|RMX			|ZOOM 	|COPY;
	G_PANEL_matrix[PAGE][VELOCITY] = 	0					|CLEAR								;
	G_PANEL_matrix[PAGE][PITCH] = 	0														;
	G_PANEL_matrix[PAGE][LENGTH] = 	0					|CLEAR								;
	G_PANEL_matrix[PAGE][START] = 	0					|CLEAR								;
	G_PANEL_matrix[PAGE][POSITION] = 	0														;
	G_PANEL_matrix[PAGE][DIRECTION]= 	0														;
	G_PANEL_matrix[PAGE][AMOUNT]= 	0														;
	G_PANEL_matrix[PAGE][GROOVE]= 	0														;
	G_PANEL_matrix[PAGE][MIDICC] = 	0														;
	G_PANEL_matrix[PAGE][MIDICH] = 	0														;


	// FX: Populate the attributes with the relevant functions
	G_PANEL_matrix[FX][ENTITY]= 		0							|RNDMZ	|FLT	|RMX	;	
	G_PANEL_matrix[FX][VELOCITY]=		0							|RNDMZ	|FLT				;
	G_PANEL_matrix[FX][PITCH] = 		0														;
	G_PANEL_matrix[FX][LENGTH] = 		0							|RNDMZ	|FLT				;
	G_PANEL_matrix[FX][START] = 		0							|RNDMZ						;
	G_PANEL_matrix[FX][POSITION]=		0							|RNDMZ						;
	G_PANEL_matrix[FX][DIRECTION]=	0							|RNDMZ			|RMX	;
	G_PANEL_matrix[FX][AMOUNT]=			0														;
	G_PANEL_matrix[FX][GROOVE]=		0														;
	G_PANEL_matrix[FX][MIDICC] = 		0														;
	G_PANEL_matrix[FX][MIDICH]=		0							|RNDMZ						;


	// SEQUENCE: Populate the attributes with the relevant functions
	G_PANEL_matrix[SEQUENCE][ENTITY]= 0					|CLEAR	|RNDMZ						;	
	G_PANEL_matrix[SEQUENCE][VELOCITY]=0														;
	G_PANEL_matrix[SEQUENCE][PITCH] = 0														;
	G_PANEL_matrix[SEQUENCE][LENGTH] = 0														;
	G_PANEL_matrix[SEQUENCE][START] = 0														;
	G_PANEL_matrix[SEQUENCE][POSITION]=0														;
	G_PANEL_matrix[SEQUENCE][DIRECTION]=0														;
	G_PANEL_matrix[SEQUENCE][MIDICH] = 0														;
	G_PANEL_matrix[SEQUENCE][MIDICC]=	0														;
	
}


// Returns a bit vector with the attributes of the target object
unsigned int PANEL_get_attributes_all( unsigned char target ) {

	unsigned int bitpattern = 0;
	unsigned char i = 0;
	
	// Build the bitpattern
	for (i=0; i<MATRIX_NROF_ROWS; i++) {

		if ( G_PANEL_matrix[target][i] != 0 )

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
			|	( 1 << AMOUNT		)
			|	( 1 << GROOVE		)
			|	( 1 << MIDICC 		)
		    );
}


//// Only works for step right now
//unsigned int PANEL_get_stepAttributeMap() {
//	return  (	( 1 << VELOCITY 	)
//			| 	( 1 << PITCH  		)
//			|	( 1 << LENGTH  		) 
//			|	( 1 << START 		)
//		    );
//}



// Takes an object class and a function and returns bitpattern of relevant attributes
unsigned int PANEL_getAttributeMap( unsigned int object, unsigned int function) {
	
	unsigned int i=0, bitpattern=0;
	
	// Build the bitpattern
	for (i=0; i<MATRIX_NROF_ROWS; i++) {

		if ((G_PANEL_matrix[object][i] & function) != 0){

			bitpattern |= 1<<i;
		}
	}

	return bitpattern;
}

