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
			if ( G_zoom_level == zoomSTEP ){

				row = target_page->stepSelectionSingleRow;
				col = target_page->stepSelectionSingleCol;
			}
			else if (	( G_zoom_level == zoomPAGE )
					&&	(	( target_page->editorMode == PREVIEW )
						||	( target_page->editorMode == PREVIEW_PERFORM )
						)
					&&	( page_preview_step != NULL )
				){

				row = page_preview_step_row;
				col = page_preview_step_col;
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
					if ((( target_page->Track[row]->attr_PIT
						   + target_page->Step[row][col]->attr_PIT ) %12 ) == 0 ){

						// Make sure we stay within bounds
						if ( target_page->Step[row][col]->attr_PIT > STEP_MIN_PITCH ){

							// Go down one octave and normalize it back
							target_page->Step[row][col]->attr_PIT -= 12;
							if ( target_page->Step[row][col]->attr_PIT < STEP_MIN_PITCH ){
								target_page->Step[row][col]->attr_PIT = STEP_MIN_PITCH;
							}
						}
					}
					else{
						// The general case of pitch selection
						if ( target_page->Track[row]->attr_PIT
							 + target_page->Step[row][col]->attr_PIT >= 0 ){

							target_page->Step[row][col]->attr_PIT =
//								( target_page->Step[row][col]->attr_PIT / 12 )*12 + j;

							(( target_page->Track[row]->attr_PIT +
							   + target_page->Step[row][col]->attr_PIT ) /12 ) *12
							 + j
							 - target_page->Track[row]->attr_PIT;

						}
						else{

							target_page->Step[row][col]->attr_PIT =

								(  (  -1*target_page->Track[row]->attr_PIT
									+ target_page->Step[row][col]->attr_PIT - 11) / 12 ) * 12 + (j)
									+ target_page->Track[row]->attr_PIT;
						}
					}

					break;

				case 12:
					// This means Cup was pressed
					// Transpose up by an octave if enough "headroom"
//					if ( target_page->Step[row][col]->attr_PIT <= (STEP_MAX_PITCH )
//						){

						// Go up one octave
						target_page->Step[row][col]->attr_PIT += 12;

						// Stay within bounds
						target_page->Step[row][col]->attr_PIT =
							normalize( target_page->Step[row][col]->attr_PIT, STEP_MIN_PITCH, STEP_MAX_PITCH );
//					}
					break;

				default:
					// The general case of pitch selection

// d_iag_printf( "old PIT:%d\n", target_page->Step[row][col]->attr_PIT );

					// Dealing with positve general values
					if ((  target_page->Track[row]->attr_PIT
						   + target_page->Step[row][col]->attr_PIT ) >= 0 ){

						// Work on the total sum and split the sum again.
						target_page->Step[row][col]->attr_PIT =

							(( target_page->Track[row]->attr_PIT +
							   + target_page->Step[row][col]->attr_PIT ) /12 ) *12
							 + j
							 - target_page->Track[row]->attr_PIT;

						// Stay within bounds
						target_page->Step[row][col]->attr_PIT =
							normalize( target_page->Step[row][col]->attr_PIT, STEP_MIN_PITCH, STEP_MAX_PITCH );
					}
					// Dealing with negative values
					else{

						target_page->Step[row][col]->attr_PIT =

							(( -1* target_page->Track[row]->attr_PIT
							     + target_page->Step[row][col]->attr_PIT - 11) /12 ) *12
							 + (j)
							 + target_page->Track[row]->attr_PIT;
					}

// d_iag_printf( "new PIT:%d\n", target_page->Step[row][col]->attr_PIT );

					break;

			} // switch( j );

// d_iag_printf( "row:%d pitch old:%d new:%d\n",
//	row, i, target_page->Track[row]->attr_PIT );

	} // Key is in the octave circle

}





