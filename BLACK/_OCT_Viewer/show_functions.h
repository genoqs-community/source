
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


// Shows the chord notes in the circle. Needs 12 bits for the offset bitpattern
void show_chord_in_circle( unsigned short chord_pattern, unsigned char offset ){

	unsigned char i = 0;
	
	// Shows the bitpattern of the chord assigned to a step
	for ( i=0; i < 12; i++ ){

		// Bit in the pattern was found
		if ( (chord_pattern & ( 1 << i )) != 0 ){

			MIR_write_dot( ndx_to_note( (offset + i)%12 ),	MIR_GREEN );
		}		
	}
}



// Show the given pitch in the inner circle.
// Status ON means show everything including octave bars, status OFF means pitch only.
// Latter used especially in step chord mode.
void show_pitch_in_circle( signed char my_pitch, unsigned char status ){

	signed char i=0, j=0;
	unsigned char pitch_sign = POS;

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

			// Show the note pitch in orange when status is off..
			if ( status == OFF ){

				// Show the note (as opposed to the octave)
				switch( i ){
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

//// Writes the content of the status vector to the position of the chunk index
//void Viewer_writeShift( unsigned int bitpattern ){
//
//	// Write bitpattern of shift n (in heft 1.)
//	HAL_WRITE_UINT32(E7T_IOPDATA , SELECT_LED_DEVICE | IMPULSE_LOW | CLOCK_LR_LOW  | bitpattern | OCTDEF_UART2_MODE);
//
//	// Write bitpattern of shift n and increment the shift index to n+1 (in heft 2.)
//	HAL_WRITE_UINT32(E7T_IOPDATA , SELECT_LED_DEVICE | IMPULSE_LOW | CLOCK_LR_HIGH | bitpattern | OCTDEF_UART2_MODE);
//}


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


//// This shows the content of the matrix intermediate representation (MIR) layer
//void Page_printMatrix( Pagestruct* target_page ) {
//
//	// Make sure not not hang up on port misconfig
//	if ( OCTDEF_UART2_MODE != USB ) return;
//
//	unsigned char row=0;
//	// unsigned char col=0;
//	// int bitpattern=0;
//	
//	d_iag_printf("  M A T R I X:\n");	
//	d_iag_printf("\n\n  M I R red:\n");
//
//	// Show the contents of the red MIR
//	for (row=0; row<17; row++) {	
//		my_print_bits( ( ( mirror(MIR[LHS][row][MIR_RED], 8) <<8) + mirror(MIR[RHS][row][MIR_RED], 8) )  ); 
//	}
//
//	d_iag_printf("\n  M I R green:\n");
//	// Show the contents of the red MIR
//	for (row=0; row<17; row++) {	
//		my_print_bits( ( ( mirror(MIR[LHS][row][MIR_GREEN], 8) <<8) + mirror(MIR[RHS][row][MIR_GREEN], 8) )  ); 
//	}
//
//	d_iag_printf("\nMIR blink\n");
//	// Show the contents of the MIR as they arrive
//	for (row=0; row<17; row++) {	
////		my_print_bits (Page_get_trackpattern(VIEWER_page, row));
//		my_print_bits( ( ( mirror(MIR[LHS][row][MIR_BLINK], 8) <<8) + mirror(MIR[RHS][row][MIR_BLINK], 8) )  ); 
//	}
//}



