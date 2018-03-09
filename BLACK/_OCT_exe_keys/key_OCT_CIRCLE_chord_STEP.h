// Key execution for the Octave circle. 


// Take a step and a bit offset and apply bit offset to the chord structure
void build_step_chord( 	Stepstruct* target_step, 
						unsigned char bit_offset ){

	// Toggle the roll and nOFF status
	// Bit offset 0 or Cup means same pitch selected -> means STEP ROLL
	if (	( bit_offset == 0 )
		){

		// If ROLL OFF, turn it on with ALL notes
		// IF ROLL ON and ALL OFFs make ONE OFF
		// IF ROLL ON and ONE OFF make ROLL OFF 
		switch( (target_step->attribute[ATTR_STATUS] & (1<<7)) >> 7 ){

			case OFF:
				// ROLL bit ON in the Step's status byte
				target_step->attribute[ATTR_STATUS] |= (1 << 7);

				// NOTE OFF - ALL
				target_step->attribute[ATTR_STATUS] &= (0xFF ^ (1<<6));
				break;

			case ON:
				if ( (target_step->attribute[ATTR_STATUS] & (1<<6)) == 0 ){

					// Turn on the nOFF bit
					target_step->attribute[ATTR_STATUS] |= (1 << 6);
				}
				else{

					// Turn off ROLL and nOFF bits
					target_step->attribute[ATTR_STATUS] &= (0xFF ^ (3<<6));
				}
				break;
		}
	}
	// Toggle the bit of the offset in the step chord data byte, if non-zero
	else if ( bit_offset > 0 ){

		// Make sure to use every bit in the pattern
		bit_offset --;

		// Toggle the bit in question
		target_step->chord_data ^= ( 1 << bit_offset );

		// Restrict cardinality of the chord to 7, i.e. 6 aux notes allowed
		if ( my_bit_cardinality( target_step->chord_data & 0x7FF ) > 6 ){

			// Reverse the operation..
			target_step->chord_data ^= ( 1 << bit_offset );
		}
	}
}



void key_OCT_CIRCLE_chord_STEP( keyNdx ){
	
	unsigned char key_offset = 0;
	unsigned char bit_offset = 0;
	unsigned char row = 0;
	unsigned char col = 0;
	unsigned char step_abs_pitch = 0;

	// If the key is in the Octave circle:
	if ( 	(keyNdx == KEY_NOTE_C	)
	 	|| 	(keyNdx == KEY_NOTE_Cup	)
		||	(keyNdx == KEY_NOTE_Cis	)
		||	(keyNdx == KEY_NOTE_D	)
		||	(keyNdx == KEY_NOTE_Dis	)
		||	(keyNdx == KEY_NOTE_E	)
		||	(keyNdx == KEY_NOTE_F	)
		||	(keyNdx == KEY_NOTE_Fis	)
		||	(keyNdx == KEY_NOTE_G	)
		||	(keyNdx == KEY_NOTE_Gis	)
		||	(keyNdx == KEY_NOTE_A	)
		||	(keyNdx == KEY_NOTE_Ais	)
		||	(keyNdx == KEY_NOTE_B	)
		){

		// Depending on who is asking the coordinates come from different sources
		if (	( zoom_level == zoomSTEP )
			||	( zoom_level == zoomSTRUM )
			){

			row = VIEWER_page->stepSelectionSingleRow;
			col = VIEWER_page->stepSelectionSingleCol;					
		}
		else{

			// Base coordinates of the step
			row = my_bit2ndx( VIEWER_page->trackSelection );
			col = is_pressed_rowzero() - 1;
		}
		
		// Establish the key offset depending on the pressed octave key
		switch( keyNdx ){

			case KEY_NOTE_C:		key_offset = 0;		break;
			case KEY_NOTE_Cis:		key_offset = 1;		break;
			case KEY_NOTE_D:		key_offset = 2;		break;
			case KEY_NOTE_Dis:		key_offset = 3;		break;
			case KEY_NOTE_E:		key_offset = 4;		break;
			case KEY_NOTE_F:		key_offset = 5;		break;
			case KEY_NOTE_Fis:		key_offset = 6;		break;
			case KEY_NOTE_G:		key_offset = 7;		break;
			case KEY_NOTE_Gis:		key_offset = 8;		break;
			case KEY_NOTE_A:		key_offset = 9;		break;
			case KEY_NOTE_Ais:		key_offset = 10;	break;
			case KEY_NOTE_B:		key_offset = 11;	break;
			case KEY_NOTE_Cup:		key_offset = 100; 	break;
		}

		if ( key_offset != 100 ){

			// Get the absolute pitch of the step
			step_abs_pitch = ( VIEWER_page->Track[row]->attribute[ATTR_PITCH]
							 + VIEWER_page->Step[row][col]->attribute[ATTR_PITCH] ) % 12;
						 
			// Calculate the offset into the bit array
			bit_offset = ( 12 + key_offset - step_abs_pitch ) % 12;
		}
		else if ( key_offset == 100 ){
		
			// Special Cup condition
			bit_offset = 0;
		}

		// Build the actual step chord structure
		build_step_chord( VIEWER_page->Step[row][col], bit_offset );


//		// Toggle the roll and nOFF status
//		// Bit offset 0 or Cup means same pitch selected -> means STEP ROLL
//		if (	( bit_offset == 0 )
//			||	( key_offset == 100 )
//			){
//
//			// If ROLL OFF, turn it on with ALL notes
//			// IF ROLL ON and ALL OFFs make ONE OFF
//			// IF ROLL ON and ONE OFF make ROLL OFF 
//			switch( (VIEWER_page->Step[row][col]->attribute[ATTR_STATUS] & (1<<7)) >> 7 ){
//
//				case OFF:
//					// ROLL bit ON in the Step's status byte
//					VIEWER_page->Step[row][col]->attribute[ATTR_STATUS] |= (1 << 7);
//					// NOTE OFF - ALL
//					VIEWER_page->Step[row][col]->attribute[ATTR_STATUS] &= (0xFF ^ (1<<6));
//					break;
//
//				case ON:
//					if ( (VIEWER_page->Step[row][col]->attribute[ATTR_STATUS] & (1<<6)) == 0 ){
//						// Turn on the nOFF bit
//						VIEWER_page->Step[row][col]->attribute[ATTR_STATUS] |= (1 << 6);
//					}
//					else{
//						// Turn off ROLL and nOFF bits
//						VIEWER_page->Step[row][col]->attribute[ATTR_STATUS] &= (0xFF ^ (3<<6));
//					}
//					break;
//			}
//		}
//		// Toggle the bit of the offset in the step chord data byte, if non-zero
//		else if ( bit_offset > 0 ){
//
//			// Make sure to use every bit in the pattern
//			bit_offset --;
//
//			// Toggle the bit in question
//			VIEWER_page->Step[row][col]->chord_data ^= ( 1 << bit_offset );
//
//			// Restrict cardinality of the chord to 7, i.e. 6 aux notes allowed
//			if ( my_bit_cardinality( VIEWER_page->Step[row][col]->chord_data & 0x7FF ) > 6 ){
//				// Reverse the operation..
//				VIEWER_page->Step[row][col]->chord_data ^= ( 1 << bit_offset );
//			}
//		}
		
	} // Key is in the octave circle

}



