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

// Indicates a page cluster selection by blinking
void page_cluster_selection( unsigned char grid_cursor ){

	Pagestruct* temp_page = &Page_repository[ grid_cursor ];
	signed short 	next_ndx = 0,
					this_ndx = 0;
	unsigned char	is_solo_rec = OFF;

	// Compute the index of the right neighbor
	this_ndx = temp_page->pageNdx;
	next_ndx = this_ndx + 10;

	// SET PREV PAGE
	GRID_write_dot (this_ndx - 10, MIR_BLINK);
	GRID_write_dot (this_ndx, MIR_BLINK);

	if ( G_run_bit == OFF ){
		is_solo_rec = has_valid_record_cluster_format( &Page_repository[ this_ndx ], OFF );
	}

	// RIGHT neighbor exists and has some content
	while ( 	(next_ndx < MAX_NROF_PAGES) &&
			(Page_repository[next_ndx].page_clear == OFF)
	){
		GRID_write_dot (next_ndx, MIR_BLINK);
		temp_page = &Page_repository[ next_ndx ];
		this_ndx = temp_page->pageNdx;
		next_ndx = this_ndx + 10;
	}

	if ( G_run_bit == OFF && is_solo_rec == ON ){
		MIR_write_dot( LED_PAUSE, MIR_GREEN );
		MIR_write_dot( LED_PAUSE, MIR_BLINK );
	}
}

// Utility function to the one below
void fill_length_bits( unsigned char index, unsigned char row, unsigned char col, unsigned char length ){

	unsigned char i=0;
	unsigned char position = 0;

	// Start with green bits at col and continue for length around the matrix
	position = col;
	for ( i=0; i < length; i++ ){

		// Compute the index of the next green dot
		MIR_write_dot(  (position + 1) * 11 + (row), 	MIR_RED );

		// Increment position and wrap around..
		position = (position + 1) % 16;
	}
}


// Show the given length at the position at hand in the matrix row
void show_preview_stepLEN( unsigned int index, unsigned char row, unsigned char col, unsigned char length ){

	// Use only the 12 multiples, since we are only coarse-grained for now
	switch( length / 12 ){

		case 0:
			// Show step blink orange
			MIR_write_dot( index, MIR_RED );
			MIR_write_dot( index, MIR_GREEN );
			MIR_write_dot( index, MIR_BLINK );
			break;

		case 1:
			// Show the full step green
			MIR_write_dot( index, MIR_GREEN );
			MIR_write_dot( index, MIR_BLINK );
			break;

		default:
			// Show the full step green
			MIR_write_dot( index, MIR_GREEN );
			MIR_write_dot( index, MIR_BLINK );

			// Show the next length
			fill_length_bits( index, row, col, length/12 );
			break;
	}
}



// Show the given velocity in the outter circle
void show_velocity_in_circle( signed char my_velocity ){

	// Write the numeric value into the circle
	MIR_write_numeric_C( my_velocity );
}



// Show the strum value into the outter circle
void show_strum_in_circle( signed char my_strum ){

	unsigned char i=0;

	if ( my_strum < 9  ){
		// Write the red dots (NEG strum)

		MIR_write_numeric_C( (9 - my_strum) * 10 );
	}
	else{
		// Write the green dots (POS strum)
		for ( i=0; i < (my_strum - 8); i++ ){

			MIR_write_numeric_C( i );
		}
	}
}




// Takes a note index and returns the note address in the inner circle
unsigned int ndx_to_note( unsigned char ndx ){

	unsigned int result = 0;

	switch( ndx ){
		case 0: 	result = LED_NOTE_C  ; 		break;
		case 1: 	result = LED_NOTE_Cis; 		break;
		case 2: 	result = LED_NOTE_D  ; 		break;
		case 3: 	result = LED_NOTE_Dis; 		break;
		case 4: 	result = LED_NOTE_E  ; 		break;
		case 5: 	result = LED_NOTE_F  ; 		break;
		case 6: 	result = LED_NOTE_Fis; 		break;
		case 7: 	result = LED_NOTE_G  ; 		break;
		case 8: 	result = LED_NOTE_Gis; 		break;
		case 9: 	result = LED_NOTE_A  ; 		break;
		case 10: 	result = LED_NOTE_Ais; 		break;
		case 11: 	result = LED_NOTE_B  ; 		break;
		case 12: 	result = LED_NOTE_Cup; 		break;
	}

	return result;
}

unsigned int note_to_ndx( unsigned char note ){

	unsigned int result = 0;

	switch( note ){
		case NOTE_C: 	result = 0; 		break;
		case NOTE_Cis: 	result = 1; 		break;
		case NOTE_D: 	result = 2; 		break;
		case NOTE_Dis: 	result = 3; 		break;
		case NOTE_E: 	result = 4; 		break;
		case NOTE_F: 	result = 5; 		break;
		case NOTE_Fis: 	result = 6; 		break;
		case NOTE_G: 	result = 7; 		break;
		case NOTE_Gis: 	result = 8; 		break;
		case NOTE_A: 	result = 9; 		break;
		case NOTE_Ais: 	result = 10; 		break;
		case NOTE_B: 	result = 11; 		break;
	}

	return result;
}


#ifndef FEATURE_ENABLE_CHORD_OCTAVE
// Return the chord up status of the step
unsigned char get_chord_up_print( Stepstruct* target_step, unsigned char bit_offset ){

	unsigned int aux_flag	= 0;
	unsigned int LSb		= 0;
	unsigned int MSb		= 0;
	unsigned char result 	= 0;

// d_iag_printf( "   0get_chord:ndx:%d chord_up>", bit_offset );
// my_print_bits( target_step->chord_up );

	// Compute the actual flag we are looking for in the bit array
	aux_flag	= ( 1 << bit_offset );

	// Check if LSb is saying anything to us, and put it in the last bit
	LSb 		= (aux_flag & ( target_step->chord_up & 0x0000FFFF ) ) >> bit_offset;

	// Check if MSb is saying anything to us, and put it in the last bit.
	MSb			= (aux_flag & ( (target_step->chord_up & 0xFFFF0000) >> 16 )) >> bit_offset;

	// Assemble the result value: 0, 1, 2, or 3, MSb is still greater
	result = ( (MSb << 1) | LSb );

// d_iag_printf( "   2get_chord:ndx:%d result:%d\n", bit_offset, result );
// my_print_bits32( target_step->chord_up );
// d_iag_printf( "   result:%d\n", result );

	return result;
}



// Sets the chord-up of a step to the given value
void set_chord_up( 	Stepstruct* target_step,
					unsigned char bit_offset,
					unsigned char in_value ){

	unsigned int pattern = 0;

//d_iag_printf( "   set_chord_up:%d ndx:%d", in_value, bit_offset );
//my_print_bits32( target_step->chord_up );

	switch( in_value ){

		case 0:
			pattern = ( ( 0x00010001 << bit_offset ) ^ 0xFFFFFFFF );
			target_step->chord_up &= pattern;
			break;

		case 1:
			// Set to 0 first
			set_chord_up( target_step, bit_offset, 0 );
			// Set to 1 finally
			pattern = ( 0x00000001 << bit_offset );
			target_step->chord_up |= pattern;
			break;

		case 2:
			// Set to 0 first
			set_chord_up( target_step, bit_offset, 0 );
			// Set to 2 finally
			pattern = ( 0x00010000 << bit_offset );
			target_step->chord_up |= pattern;
			break;
	}

// d_iag_printf( "   set_chord_up:%d ndx:%d", in_value, bit_offset );
// my_print_bits32( target_step->chord_up );
// get_chord_up_print( target_step, bit_offset );

}



// Return the chord_up status of the step at the given index
unsigned char get_chord_up( Stepstruct* target_step, unsigned char bit_offset ){

	unsigned int aux_flag	= 0;
	unsigned int LSb		= 0;
	unsigned int MSb		= 0;
	unsigned char result 	= 0;

	// Compute the actual flag we are looking for in the bit array
	aux_flag	= ( 1 << bit_offset );

	// Check if LSb is saying anything to us, and put it in the last bit
	LSb 		= (aux_flag & ( target_step->chord_up & 0x0000FFFF ) ) >> bit_offset;

	// Check if MSb is saying anything to us, and put it in the last bit.
	MSb			= (aux_flag & ( (target_step->chord_up & 0xFFFF0000) >> 16 )) >> bit_offset;

	// Assemble the result value: 0, 1, 2, or 3, MSb is still greater
	result = ( (MSb << 1) | LSb );

//d_iag_printf( "x:%d r:%d\n", bit_offset, result );

	return result;
}




// Shows the chord notes in the circle. Needs 12 bits for the offset bitpattern
void show_chord_in_circle( Stepstruct* target_step, unsigned char offset ){

	unsigned char i = 0;
	unsigned short chord_pattern = 0;

	// Compute the actual pattern - << 1 is due to some historics..
	chord_pattern = (target_step->chord_data & 0x7FF) << 1;

	// Shows the bitpattern of the chord assigned to a step
	for ( i=0; i < 12; i++ ){

		// Bit in the pattern was found
		if ( (chord_pattern & ( 1 << (i+1) )) != 0 ){

			// Check the step's chord_up status at the aux counter position
			switch( get_chord_up( target_step, i )){

				case 0:
					MIR_write_dot( ndx_to_note( (offset + i+1)%12 ),	MIR_GREEN 	);
					MIR_write_dot( ndx_to_note( (offset + i+1)%12 ),	MIR_RED 	);
					break;

				case 1:
					MIR_write_dot( ndx_to_note( (offset + i+1)%12 ),	MIR_GREEN 	);
					break;

				case 2:
					MIR_write_dot( ndx_to_note( (offset + i+1)%12 ),	MIR_RED 	);
					break;
			}

		} // Bit in the pattern was found
	}
}
#endif


// Show the given pitch in the inner circle.
// Status ON means show everything including octave bars, status OFF means pitch only.
// Latter (status OFF) used especially in step chord mode.
void show_pitch_in_circle( signed char my_pitch, unsigned char status ){

	signed char i=0, j=0;
	unsigned char pitch_sign = POS;
	unsigned char note = 0;

	// Determine the sign of the pitch to be written and the abs value of pitch
	if ( my_pitch < 0 ) {
		pitch_sign = NEG;
		my_pitch = abs( my_pitch ) ;
	}

	// Depending on the sign of the pitch..
	switch( pitch_sign ){

		case POS:

			// Note value
			i = (my_pitch % 12);

			// Octave value
			j = (my_pitch / 12);

			// Show the octave as red bar.
			// If clauses to turn off the red under the greed LED

			// Shows everything.. including the octave as red bar..
			if ( status == ON ){

				switch( j ){
					case 10:
						if ( (i == (j -1)) || (i != 9) )
							MIR_write_dot( LED_NOTE_A,		MIR_RED	);
					case 9:
						if ( (i == (j -1)) || (i != 8) )
							MIR_write_dot( LED_NOTE_Gis,	MIR_RED	);
					case 8:
						if ( (i == (j -1)) || (i != 7) )
							MIR_write_dot( LED_NOTE_G,		MIR_RED	);
					case 7:
						if ( (i == (j -1)) || (i != 6) )
							MIR_write_dot( LED_NOTE_Fis,	MIR_RED	);
					case 6:
						if ( (i == (j -1)) || (i != 5) )
							MIR_write_dot( LED_NOTE_F,		MIR_RED	);
					case 5:
						if ( (i == (j -1)) || (i != 4) )
							MIR_write_dot( LED_NOTE_E, 		MIR_RED	);
					case 4:
						if ( (i == (j -1)) || (i != 3) )
							MIR_write_dot( LED_NOTE_Dis,	MIR_RED	);
					case 3:
						if ( (i == (j -1)) || (i != 2) )
							MIR_write_dot( LED_NOTE_D,		MIR_RED	);
					case 2:
						if ( (i == (j -1)) || (i != 1) )
							MIR_write_dot( LED_NOTE_Cis,	MIR_RED	);
					case 1:
						if ( (i == (j -1)) || (i != 0) )
							MIR_write_dot( LED_NOTE_C, 		MIR_RED	);
						break;
				} // switch( my_pitch / 12 )
			}

			// Show the note (as opposed to the octave)
			switch( i ){
				case 0:
					// Consider the "headroom reached case"
					if ( my_pitch < 120 ){
						note = LED_NOTE_C;
					}
					else {
						note = LED_NOTE_Cup;
					}
					break;
				case 1:		note = LED_NOTE_Cis;	break;
				case 2: 	note = LED_NOTE_D;		break;
				case 3:		note = LED_NOTE_Dis;	break;
				case 4:		note = LED_NOTE_E; 		break;
				case 5:		note = LED_NOTE_F;		break;
				case 6:		note = LED_NOTE_Fis;	break;
				case 7:		note = LED_NOTE_G;		break;
				case 8:		note = LED_NOTE_Gis;	break;
				case 9:		note = LED_NOTE_A;		break;
				case 10:	note = LED_NOTE_Ais;	break;
				case 11:	note = LED_NOTE_B;		break;
			} // switch( my_pitch % 12)

			// The actual write operation for the note.
			MIR_write_dot( note, MIR_GREEN 	);

			// When status is OFF, show the pitch blinking in orange
			if ( status == OFF ){
				MIR_write_dot( note, MIR_RED 	);
				MIR_write_dot( note, MIR_BLINK 	);
			}
			break; // pitch_sign is POS


		case NEG:
			// Adapt the pitch value
			my_pitch --;

			// Note value
			i = (my_pitch % 12);

			// Octave value
			j = (my_pitch / 12);

			// Show the octave as red bar
			// ..only when the status is ON:
			if ( status == ON ){

				switch( j ){
					case 10:
						if ( (i == (j -1)) || (i != 9) )
							MIR_write_dot( LED_NOTE_D, 		MIR_RED	);
					case 9:
						if ( (i == (j -1)) || (i != 8) )
							MIR_write_dot( LED_NOTE_Dis,	MIR_RED	);
					case 8:
						if ( (i == (j -1)) || (i != 7) )
							MIR_write_dot( LED_NOTE_E,		MIR_RED	);
					case 7:
						if ( (i == (j -1)) || (i != 6) )
							MIR_write_dot( LED_NOTE_F,		MIR_RED	);
					case 6:
						if ( (i == (j -1)) || (i != 5) )
							MIR_write_dot( LED_NOTE_Fis,	MIR_RED	);
					case 5:
						if ( (i == (j -1)) || (i != 4) )
							MIR_write_dot( LED_NOTE_G,		MIR_RED	);
					case 4:
						if ( (i == (j -1)) || (i != 3) )
							MIR_write_dot( LED_NOTE_Gis,	MIR_RED	);
					case 3:
						if ( (i == (j -1)) || (i != 2) )
							MIR_write_dot( LED_NOTE_A,		MIR_RED	);
					case 2:
						if ( (i == (j -1)) || (i != 1) )
							MIR_write_dot( LED_NOTE_Ais,	MIR_RED	);
					case 1:
						if ( (i == (j -1)) || (i != 0) )
							MIR_write_dot( LED_NOTE_B,		MIR_RED	);
					case 0:
							MIR_write_dot( LED_NOTE_Cup,	MIR_RED );
						break;
				} // switch( my_pitch / 12 )
			}

			// Show the note (as opposed to the octave)
			switch( 11 - i ){
				case 0:
					// Consider the "headroom reached case"
					if ( my_pitch < 120 ){
						MIR_write_dot( LED_NOTE_C, 		MIR_GREEN 	);
					}
					else {
						MIR_write_dot( LED_NOTE_Cup, 	MIR_GREEN	);
					}
					break;
				case 1:
					MIR_write_dot( LED_NOTE_Cis, 	MIR_GREEN	);
					break;
				case 2:
					MIR_write_dot( LED_NOTE_D, 		MIR_GREEN	);
					break;
				case 3:
					MIR_write_dot( LED_NOTE_Dis, 	MIR_GREEN	);
					break;
				case 4:
					MIR_write_dot( LED_NOTE_E, 		MIR_GREEN	);
					break;
				case 5:
					MIR_write_dot( LED_NOTE_F, 		MIR_GREEN	);
					break;
				case 6:
					MIR_write_dot( LED_NOTE_Fis, 	MIR_GREEN	);
					break;
				case 7:
					MIR_write_dot( LED_NOTE_G, 		MIR_GREEN	);
					break;
				case 8:
					MIR_write_dot( LED_NOTE_Gis,	MIR_GREEN	);
					break;
				case 9:
					MIR_write_dot( LED_NOTE_A, 		MIR_GREEN	);
					break;
				case 10:
					MIR_write_dot( LED_NOTE_Ais, 	MIR_GREEN	);
					break;
				case 11:
					MIR_write_dot( LED_NOTE_B, 		MIR_GREEN	);
					break;
			} // switch( my_pitch % 12)

			// Show pitch in orange when status is off
			if ( status == OFF ){

				// Show the note (as opposed to the octave)
				switch( 11 - i ){
					case 0:
						// Consider the "headroom reached case"
						if ( my_pitch < 120 ){
							MIR_write_dot( LED_NOTE_C, 		MIR_RED 	);
						}
						else {
							MIR_write_dot( LED_NOTE_Cup, 	MIR_RED	);
						}
						break;
					case 1:
						MIR_write_dot( LED_NOTE_Cis, 	MIR_RED	);
						break;
					case 2:
						MIR_write_dot( LED_NOTE_D, 		MIR_RED	);
						break;
					case 3:
						MIR_write_dot( LED_NOTE_Dis, 	MIR_RED	);
						break;
					case 4:
						MIR_write_dot( LED_NOTE_E, 		MIR_RED	);
						break;
					case 5:
						MIR_write_dot( LED_NOTE_F, 		MIR_RED	);
						break;
					case 6:
						MIR_write_dot( LED_NOTE_Fis, 	MIR_RED	);
						break;
					case 7:
						MIR_write_dot( LED_NOTE_G, 		MIR_RED	);
						break;
					case 8:
						MIR_write_dot( LED_NOTE_Gis,	MIR_RED	);
						break;
					case 9:
						MIR_write_dot( LED_NOTE_A, 		MIR_RED	);
						break;
					case 10:
						MIR_write_dot( LED_NOTE_Ais, 	MIR_RED	);
						break;
					case 11:
						MIR_write_dot( LED_NOTE_B, 		MIR_RED	);
						break;
				} // switch( my_pitch % 12)
			}

			break; // pitch_sign is NEG
	} // switch( pitch_sign )


}







// Map chunkNdx to MIR index. takes a chunkNdx and returns its MIR pattern.
unsigned char getBitpattern (	unsigned char color,
								unsigned char chunkNdx) {

	unsigned char bitpattern = 0;

	// BUT TOOL LEFT
	// 1/2 : top part
	if (chunkNdx == 1) {
		bitpattern = MIR[LHS][11][color];
	}
	// 2/2 : bottom part
	else if (chunkNdx == 2) {
		bitpattern = MIR[LHS][12][color];
	}


	// MATRIX
	// LEFT: chunk 13 is explicitly out
	else if ((chunkNdx > 2) && (chunkNdx < 13)) {
		bitpattern = MIR[LHS][chunkNdx-3][color];
	}
	// RIGHT: side
	else if ((chunkNdx > 13) && (chunkNdx < 24)) {
		bitpattern = MIR[RHS][chunkNdx-14][color];
	}


	// BUT TOOL RIGHT
	// 1/2 : top part
	else if (chunkNdx == 25) {
		// Take the first 6 bits part
		bitpattern = MIR[RHS][11][color];
	}
	// 2/2 : bottom part
	else if (chunkNdx == 26) {
		// Take the second 6 bits part
		bitpattern = MIR[RHS][12][color];
	}


	// HORIZONTAL
	// LEFT
	else if (chunkNdx == 13) {
		bitpattern = MIR[LHS][10][color];
	}
	// RIGHT
	else if (chunkNdx == 24) {
		bitpattern = MIR[RHS][10][color];
	}


	// FUNCTIONS
		// Top Left
	else if (chunkNdx == 27) {
		bitpattern = MIR[LHS][13][color];
	}
		// Bottom Left
	else if (chunkNdx == 28) {
		bitpattern = MIR[LHS][14][color];
	}
		// Top Right
	else if (chunkNdx == 35) {
		bitpattern = MIR[RHS][13][color];
	}
		// Bottom Right
	else if (chunkNdx == 29) {
		bitpattern = MIR[RHS][14][color];
	}


	// ATTRIBUTES
		// Left
	else if (chunkNdx == 32) {
		bitpattern = MIR[LHS][15][color];
	}
		// Right
	else if (chunkNdx == 33) {
		bitpattern = MIR[RHS][15][color];
	}

	// MODES BUTTONS
	else if (chunkNdx == 30) {
		bitpattern = MIR[RHS][16][color];
	}

	// OBJECTS: for now test the lightpipes
	else if (chunkNdx == 34) {
		bitpattern = MIR[LHS][16][color];
	}

	return bitpattern;
}

// Initialization sequence for the shift register. DONT TOUCH!!
void Viewer_initShift(){
	// int i=0;

	// Generate the impulse
	HAL_WRITE_UINT32(E7T_IOPDATA , SELECT_LED_DEVICE | IMPULSE_HIGH | CLOCK_LR_LOW | OCTDEF_UART2_MODE);
	// !!! Need a break after this or the Page will not display
	//	for (i=0; i<1000; i++){}
	// wait Not needed anymore.

	// Move to the first shift and take it from there..
	HAL_WRITE_UINT32(E7T_IOPDATA , SELECT_LED_DEVICE | IMPULSE_HIGH | CLOCK_LR_HIGH | OCTDEF_UART2_MODE);
}



void Viewer_writeShift( unsigned int bitpattern ){

   // Write bitpattern of shift n (in heft 1.)
   HAL_WRITE_UINT32(E7T_IOPDATA , SELECT_LED_DEVICE | IMPULSE_LOW | CLOCK_LR_LOW  | bitpattern | OCTDEF_UART2_MODE);

   // Write bitpattern of shift n and increment the shift index to n+1 (in heft 2.)
   HAL_WRITE_UINT32(E7T_IOPDATA , SELECT_LED_DEVICE | IMPULSE_LOW | CLOCK_LR_HIGH | bitpattern | OCTDEF_UART2_MODE);

     // Turn off rusische Diso
   HAL_WRITE_UINT32(E7T_IOPDATA , SELECT_LED_DEVICE | IMPULSE_LOW | CLOCK_LR_LOW  | 0 | OCTDEF_UART2_MODE);
}




// Mirrors the bitpattern found in the first -length- bits within an integer
unsigned int mirror ( unsigned int input, unsigned char length) {
	unsigned int i=0;
	unsigned int output=0;

	for (i=1; i<length+1; i++) {
		// if a 1 is found in a bit
		if (1<<(length-i) & input)
			output |=  1 << (i-1);
	}
	return output;
}




#ifdef FEATURE_ENABLE_CHORD_OCTAVE
// Helper return the selected chord octave
unsigned char get_current_chord_octave() {

	unsigned char chord_octave = 0;

	switch ( MODE_OBJECT_SELECTION ) {
		case CHORDEYE_OCTAVE_FIRST:
			chord_octave = CHORD_OCTAVE_FIRST;
			break;

		case CHORDEYE_OCTAVE_SECOND:
			chord_octave = CHORD_OCTAVE_SECOND;
			break;
		case CHORDEYE_OCTAVE_THIRD:
			chord_octave = CHORD_OCTAVE_THIRD;
			break;
	}

	return chord_octave;
}


// Get chord bit pattern projected onto a single octave
unsigned int get_chord_projected( Stepstruct* target_step ) {

	return ( target_step->chord_data & 0x7FF ) | ( target_step->chord_up & 0xFFF ) | ( ( target_step->chord_up & 0xFFF0000 ) >> 16 );
}


// Get cardinality of chord octave constituents
unsigned char get_chord_cardinality( Stepstruct* target_step, unsigned char octave_mask ) {

	switch ( octave_mask ) {
		case CHORD_OCTAVE_FIRST:
			return my_bit_cardinality( target_step->chord_data & 0x7FF );
		case CHORD_OCTAVE_SECOND:
			return my_bit_cardinality( target_step->chord_up & 0xFFF );
		case CHORD_OCTAVE_THIRD:
			return my_bit_cardinality( target_step->chord_up & 0xFFF0000 );
		case CHORD_OCTAVE_FIRST | CHORD_OCTAVE_SECOND:
			return my_bit_cardinality( ( target_step->chord_data & 0x7FF ) | ( ( target_step->chord_up & 0xFFF ) << 11 ) );
		case CHORD_OCTAVE_FIRST | CHORD_OCTAVE_THIRD:
			return my_bit_cardinality( ( target_step->chord_data & 0x7FF ) | ( target_step->chord_up & 0xFFF0000 ) );
		case CHORD_OCTAVE_SECOND | CHORD_OCTAVE_THIRD:
			return my_bit_cardinality( target_step->chord_up & 0xFFF0FFF );
		case CHORD_OCTAVE_ALL:
			return my_bit_cardinality( target_step->chord_data & 0x7FF ) + my_bit_cardinality( target_step->chord_up & 0xFFF0FFF );
	}

	return 0;
}


// Get the chord octave(s) mask for the step at the given note index
unsigned char get_chord_octave_mask( Stepstruct* target_step, unsigned char note_offset ) {

	unsigned int note_flag	= 0;
	unsigned int chord_data = target_step->chord_data & 0x7FF;
	unsigned int chord_up = target_step->chord_up & 0xFFF0FFF;

	unsigned char octave_mask = 0;

	note_flag = 1 << ( note_offset - 1 );
	if ( ( note_flag & chord_data ) ) {
		SET_MASK( octave_mask, CHORD_OCTAVE_FIRST );
	}

	note_flag = 1 << note_offset;
	if ( note_flag & ( chord_up & 0xFFF ) ) {
		SET_MASK( octave_mask, CHORD_OCTAVE_SECOND );
	}

	chord_up >>= 16;
	if ( note_flag & chord_up ) {
		SET_MASK( octave_mask, CHORD_OCTAVE_THIRD );
	}

	return octave_mask;
}


// Get the first chord octave(s) matching given mask
unsigned char get_chord_octave_first( unsigned char octave_mask ) {

	unsigned char octave = 0;

	if ( CHECK_MASK( octave_mask, CHORD_OCTAVE_FIRST ) ) {
		octave = CHORD_OCTAVE_FIRST;
	} else if ( CHECK_MASK( octave_mask, CHORD_OCTAVE_SECOND ) ) {
		octave = CHORD_OCTAVE_SECOND;
	} else if ( CHECK_MASK( octave_mask, CHORD_OCTAVE_THIRD ) ) {
		octave = CHORD_OCTAVE_THIRD;
	}

	return octave;
}


// Get the status of the chord octave mask applied to the current octave
unsigned char get_chord_octave_status( unsigned char octave_mask, unsigned char octave ) {

	unsigned char status = 0;
	unsigned char other_mask = octave_mask;

	CLEAR_MASK( other_mask, octave );

	if ( CHECK_MASK( octave_mask, CHORD_OCTAVE_ALL ) ) {
		status = CHORD_OCTAVE_STATUS_ALL;
	} else {
		if ( CHECK_MASK( octave_mask, octave ) ) {
			status = CHORD_OCTAVE_STATUS_CURRENT;
		}

		if( other_mask ) {
			SET_MASK( status, CHORD_OCTAVE_STATUS_OTHER );
		}
	}

	return status;
}


// Check if step contains a given note in a given octave
bool is_set_chord_octave( Stepstruct* target_step, unsigned char note_offset, unsigned char chord_octave_mask ) {

	return CHECK_MASK( get_chord_octave_mask( target_step, note_offset ), chord_octave_mask );
}


// Check if step contains any aux chord note data
bool is_step_chord( Stepstruct* target_step ) {

	return ( target_step->chord_data & 0x7FF ) || ( target_step->chord_up & 0xFFF0FFF );
}

// Check if note contains any aux chord note data
bool is_note_chord( Notestruct* target_note ) {

	return ( target_note->chord_data & 0x7FF ) || ( target_note->chord_up & 0xFFF0FFF );
}


// Show step chord note data for given note offset and viewing octave layer (CHORD_OCTAVE_FIRST, CHORD_OCTAVE_SECOND or CHORD_OCTAVE_THIRD)
// ORANGE = Note set only in currently viewed Octave
// GREEN  = Note set in currently viewed Octave AND also in another Octave(s)
// RED    = Note NOT set in current Octave, but IS set in one of the another Octave(s)
// SHINE  = Note set in ALL 3 Octaves (full stack)
void show_chord_octave( Stepstruct* target_step, unsigned char note_offset, unsigned char chord_octave ) {

	unsigned char i	= 0;
	unsigned char note_ndx = 0;
	unsigned char octave_mask = 0;
	unsigned char root_note = ndx_to_note( note_offset );

	MIR_write_dot( root_note, MIR_BLINK );

	if( is_step_chord( target_step ) ) {
		for ( i = 0; i < 12; i++ ) {
			octave_mask = get_chord_octave_mask( target_step, i );

			if ( i == 0 ) {
				octave_mask |= CHORD_OCTAVE_FIRST;
			} else if ( !octave_mask ) {
				continue;
			}

			note_ndx = ndx_to_note( ( note_offset + i ) % 12 );

			switch ( get_chord_octave_status( octave_mask, chord_octave ) ) {

				case CHORD_OCTAVE_STATUS_CURRENT:
					MIR_write_dot( note_ndx, MIR_GREEN );
					MIR_write_dot( note_ndx, MIR_RED );
					break;

				case ( CHORD_OCTAVE_STATUS_CURRENT | CHORD_OCTAVE_STATUS_OTHER ):
					MIR_write_dot( note_ndx, MIR_GREEN );
					break;

				case CHORD_OCTAVE_STATUS_OTHER:
					MIR_write_dot( note_ndx, MIR_RED );
					break;

				case CHORD_OCTAVE_STATUS_ALL:
					if( i == 0 ) {
						// Full stack on root
						MIR_write_dot( note_ndx, MIR_SHINE_RED );
					} else {
						// Full stack on aux
						MIR_write_dot( note_ndx, MIR_SHINE_GREEN );
					}
					break;
			}
		}
	} else {
		if( chord_octave == CHORD_OCTAVE_FIRST ) {
			MIR_write_dot( root_note, MIR_GREEN );
		}
		MIR_write_dot( root_note, MIR_RED );
	}
}


// Show step chord note data for given note offset and viewing octave layer (CHORD_OCTAVE_FIRST, CHORD_OCTAVE_SECOND or CHORD_OCTAVE_THIRD)
// ORANGE = Note in first octave
// GREEN  = Note in first octave
// RED    = Note in third octave
void show_chord_octave_first( Stepstruct* target_step, unsigned char note_offset ) {

	unsigned char i	= 0;
	unsigned char note_ndx = 0;
	unsigned char octave_mask = 0;
	unsigned char root_note = ndx_to_note( note_offset );

	MIR_write_dot( root_note, MIR_GREEN );
	MIR_write_dot( root_note, MIR_RED );
	MIR_write_dot( root_note, MIR_BLINK );

	if( is_step_chord( target_step ) ) {
		for ( i = 1; i < 12; i++ ) {
			octave_mask = get_chord_octave_mask( target_step, i );

			if ( i == 0 ) {
				octave_mask |= CHORD_OCTAVE_FIRST;
			} else if ( !octave_mask ) {
				continue;
			}

			note_ndx = ndx_to_note( ( note_offset + i ) % 12 );

			switch ( get_chord_octave_first( octave_mask ) ) {

				case CHORD_OCTAVE_FIRST:
					MIR_write_dot( note_ndx, MIR_GREEN );
					MIR_write_dot( note_ndx, MIR_RED );
					break;

				case ( CHORD_OCTAVE_SECOND ):
					MIR_write_dot( note_ndx, MIR_GREEN );
					break;

				case CHORD_OCTAVE_THIRD:
					MIR_write_dot( note_ndx, MIR_RED );
					break;
			}
		}
	}
}
#endif
