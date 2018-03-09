


// Makes the necessary assignments for the ABLETON mode - such as pitches
void ABLETON_init( Pagestruct* target_page ){

	unsigned char row = 0;
	unsigned char col = 0;

	// MATRIX FIELD - sending out note values
	//
	// Assign tracks 0-4 to channel 1
	for ( row=0; row <= 4; row++ ){

		// MIDICH
		target_page->Track[row]->attribute[ATTR_MIDICH] = 1;

		// PITCH
		target_page->Track[row]->attribute[ATTR_PITCH] = 16 * ( row%5 );
		for ( col=0; col < MATRIX_NROF_COLUMNS; col++ ){
		
			target_page->Step[row][col]->attribute[ATTR_PITCH] = col;
		}
	}
	
	// Assign tracks 5-9 to channel 2, etc
	for ( row=5; row <= 9; row++ ){

		// MIDICH
		target_page->Track[row]->attribute[ATTR_MIDICH] = 2;

		// PITCH
		target_page->Track[row]->attribute[ATTR_PITCH] = 16 * ( row%5 );
		for ( col=0; col < MATRIX_NROF_COLUMNS; col++ ){
		
			target_page->Step[row][col]->attribute[ATTR_PITCH] = col;
		}
	}
		

}


