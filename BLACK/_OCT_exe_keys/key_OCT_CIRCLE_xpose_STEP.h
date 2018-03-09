// Key execution snippet for the Octave circle. May be reused

void key_OCT_CIRCLE_xpose_STEP( unsigned int keyNdx ){

	unsigned char row = 0;
	unsigned char col = 0;
	unsigned char j   = 0;

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

				row = VIEWER_page->stepSelectionSingleRow;
				col = VIEWER_page->stepSelectionSingleCol;					
			}
			else{

				// Base coordinates of the step
				row = my_bit2ndx( VIEWER_page->trackSelection );
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
					if ((( VIEWER_page->Track[row]->attribute[ATTR_PITCH] 
						   + VIEWER_page->Step[row][col]->attribute[ATTR_PITCH] ) %12 ) == 0 ){

						// Make sure we stay within bounds
						if ( VIEWER_page->Step[row][col]->attribute[ATTR_PITCH] > (STEP_MIN_PITCH ) ){ 
							
							// Go down one octave and normalize it back
							VIEWER_page->Step[row][col]->attribute[ATTR_PITCH] -= 12;
							if ( VIEWER_page->Step[row][col]->attribute[ATTR_PITCH] < STEP_MIN_PITCH ){
								VIEWER_page->Step[row][col]->attribute[ATTR_PITCH] = STEP_MIN_PITCH;
							}
						}
					}
					else{
						// The general case of pitch selection
						if ( VIEWER_page->Track[row]->attribute[ATTR_PITCH]
							 + VIEWER_page->Step[row][col]->attribute[ATTR_PITCH] >= 0 ){

							VIEWER_page->Step[row][col]->attribute[ATTR_PITCH] =
//								( VIEWER_page->Step[row][col]->attribute[ATTR_PITCH] / 12 )*12 + j;

							(( VIEWER_page->Track[row]->attribute[ATTR_PITCH] +
							   + VIEWER_page->Step[row][col]->attribute[ATTR_PITCH] ) /12 ) *12 
							 + j
							 - VIEWER_page->Track[row]->attribute[ATTR_PITCH];

						}
						else{

							VIEWER_page->Step[row][col]->attribute[ATTR_PITCH] =
							
								(  (  -1*VIEWER_page->Track[row]->attribute[ATTR_PITCH] 
									+ VIEWER_page->Step[row][col]->attribute[ATTR_PITCH] - 11) / 12 ) * 12 + (j)
									+ VIEWER_page->Track[row]->attribute[ATTR_PITCH];
						}
					}

					break;

				case 12:
					// This means Cup was pressed
					// Transpose up by an octave if enough "headroom"
					if ( VIEWER_page->Step[row][col]->attribute[ATTR_PITCH] 
										<= (STEP_MAX_PITCH ) 
						){
											
						// Go up one octave
						VIEWER_page->Step[row][col]->attribute[ATTR_PITCH] += 12;
						if ( VIEWER_page->Step[row][col]->attribute[ATTR_PITCH] > STEP_MAX_PITCH ){

							VIEWER_page->Step[row][col]->attribute[ATTR_PITCH] = STEP_MAX_PITCH;
						}
					}
					break;
	
				default:
					// The general case of pitch selection

// d_iag_printf( "old PIT:%d\n", VIEWER_page->Step[row][col]->attribute[ATTR_PITCH] );

					// Dealing with positve general values
					if ((  VIEWER_page->Track[row]->attribute[ATTR_PITCH] 
						   + VIEWER_page->Step[row][col]->attribute[ATTR_PITCH] ) >= 0 ){

						// Work on the total sum and split the sum again.
						VIEWER_page->Step[row][col]->attribute[ATTR_PITCH] =
						
							(( VIEWER_page->Track[row]->attribute[ATTR_PITCH] +
							   + VIEWER_page->Step[row][col]->attribute[ATTR_PITCH] ) /12 ) *12 
							 + j
							 - VIEWER_page->Track[row]->attribute[ATTR_PITCH];
	
						// Max out the offset value
						if ( VIEWER_page->Step[row][col]->attribute[ATTR_PITCH] > STEP_MAX_PITCH ){
	
							VIEWER_page->Step[row][col]->attribute[ATTR_PITCH] = STEP_MAX_PITCH;
						}
					}
					// Dealing with negative values
					else{

						VIEWER_page->Step[row][col]->attribute[ATTR_PITCH] =

							(( -1* VIEWER_page->Track[row]->attribute[ATTR_PITCH]
							     + VIEWER_page->Step[row][col]->attribute[ATTR_PITCH] - 11) /12 ) *12 
							 + (j)
							 + VIEWER_page->Track[row]->attribute[ATTR_PITCH];
					}

// d_iag_printf( "new PIT:%d\n", VIEWER_page->Step[row][col]->attribute[ATTR_PITCH] );

					break;

			} // switch( j );
		
// d_iag_printf( "row:%d pitch old:%d new:%d\n", 
//	row, i, VIEWER_page->Track[row]->attribute[ATTR_PITCH] );

	} // Key is in the octave circle

}





