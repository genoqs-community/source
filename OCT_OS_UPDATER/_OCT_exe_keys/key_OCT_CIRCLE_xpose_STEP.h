// Key execution snippet for the Octave circle. May be reused

void key_OCT_CIRCLE_xpose_STEP( unsigned int keyNdx ){

	unsigned char row = 0;
	unsigned char col = 0;
	unsigned char j   = 0;

	// Work on the page under the grid cursor
	Pagestruct* target_page = &Page_repository[GRID_CURSOR];

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
			if ( zoom_level == zoomSTEP ){

				row = target_page->stepSelectionSingleRow;
				col = target_page->stepSelectionSingleCol;					
			}
			else{

				// Base coordinates of the step
				row = my_bit2ndx( target_page->trackSelection );
				col = is_pressed_rowzero() - 1;
			}
			
			// Establish the offset depending on the pressed octave key
			switch( keyNdx ){
	
				case KEY_NOTE_C:		j = 0;		break;
				case KEY_NOTE_Cis:		j = 1;		break;
				case KEY_NOTE_D:		j = 2;		break;
				case KEY_NOTE_Dis:		j = 3;		break;
				case KEY_NOTE_E:		j = 4;		break;
				case KEY_NOTE_F:		j = 5;		break;
				case KEY_NOTE_Fis:		j = 6;		break;
				case KEY_NOTE_G:		j = 7;		break;
				case KEY_NOTE_Gis:		j = 8;		break;
				case KEY_NOTE_A:		j = 9;		break;
				case KEY_NOTE_Ais:		j = 10;		break;
				case KEY_NOTE_B:		j = 11;		break;
				case KEY_NOTE_Cup:		j = 12; 	break;
			}
	

			switch( j ){

				case 0:
					// If the pitch is a C already, transpose octave down
					if ((( target_page->Track[row]->attribute[ATTR_PITCH] 
						   + target_page->Step[row][col]->attribute[ATTR_PITCH] ) %12 ) == 0 ){

						// Make sure we stay within bounds
						if ( target_page->Step[row][col]->attribute[ATTR_PITCH] > (STEP_MIN_PITCH ) ){ 
							
							// Go down one octave and normalize it back
							target_page->Step[row][col]->attribute[ATTR_PITCH] -= 12;
							if ( target_page->Step[row][col]->attribute[ATTR_PITCH] < STEP_MIN_PITCH ){
								target_page->Step[row][col]->attribute[ATTR_PITCH] = STEP_MIN_PITCH;
							}
						}
					}
					else{
						// The general case of pitch selection
						if ( target_page->Track[row]->attribute[ATTR_PITCH]
							 + target_page->Step[row][col]->attribute[ATTR_PITCH] >= 0 ){

							target_page->Step[row][col]->attribute[ATTR_PITCH] =
//								( target_page->Step[row][col]->attribute[ATTR_PITCH] / 12 )*12 + j;

							(( target_page->Track[row]->attribute[ATTR_PITCH] +
							   + target_page->Step[row][col]->attribute[ATTR_PITCH] ) /12 ) *12 
							 + j
							 - target_page->Track[row]->attribute[ATTR_PITCH];

						}
						else{

							target_page->Step[row][col]->attribute[ATTR_PITCH] =
							
								(  (  -1*target_page->Track[row]->attribute[ATTR_PITCH] 
									+ target_page->Step[row][col]->attribute[ATTR_PITCH] - 11) / 12 ) * 12 + (j)
									+ target_page->Track[row]->attribute[ATTR_PITCH];
						}
					}

					break;

				case 12:
					// This means Cup was pressed
					// Transpose up by an octave if enough "headroom"
					if ( target_page->Step[row][col]->attribute[ATTR_PITCH] 
										<= (STEP_MAX_PITCH ) 
						){
											
						// Go up one octave
						target_page->Step[row][col]->attribute[ATTR_PITCH] += 12;
						if ( target_page->Step[row][col]->attribute[ATTR_PITCH] > STEP_MAX_PITCH ){

							target_page->Step[row][col]->attribute[ATTR_PITCH] = STEP_MAX_PITCH;
						}
					}
					break;
	
				default:
					// The general case of pitch selection

// d_iag_printf( "old PIT:%d\n", target_page->Step[row][col]->attribute[ATTR_PITCH] );

					// Dealing with positve general values
					if ((  target_page->Track[row]->attribute[ATTR_PITCH] 
						   + target_page->Step[row][col]->attribute[ATTR_PITCH] ) >= 0 ){

						// Work on the total sum and split the sum again.
						target_page->Step[row][col]->attribute[ATTR_PITCH] =
						
							(( target_page->Track[row]->attribute[ATTR_PITCH] +
							   + target_page->Step[row][col]->attribute[ATTR_PITCH] ) /12 ) *12 
							 + j
							 - target_page->Track[row]->attribute[ATTR_PITCH];
	
						// Max out the offset value
						if ( target_page->Step[row][col]->attribute[ATTR_PITCH] > STEP_MAX_PITCH ){
	
							target_page->Step[row][col]->attribute[ATTR_PITCH] = STEP_MAX_PITCH;
						}
					}
					// Dealing with negative values
					else{

						target_page->Step[row][col]->attribute[ATTR_PITCH] =

							(( -1* target_page->Track[row]->attribute[ATTR_PITCH]
							     + target_page->Step[row][col]->attribute[ATTR_PITCH] - 11) /12 ) *12 
							 + (j)
							 + target_page->Track[row]->attribute[ATTR_PITCH];
					}

// d_iag_printf( "new PIT:%d\n", target_page->Step[row][col]->attribute[ATTR_PITCH] );

					break;

			} // switch( j );
		
// d_iag_printf( "row:%d pitch old:%d new:%d\n", 
//	row, i, target_page->Track[row]->attribute[ATTR_PITCH] );

	} // Key is in the octave circle

}





