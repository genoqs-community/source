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

void NEMO_key_OCT_CIRCLE_xpose_STEP( unsigned int keyNdx ){

//	unsigned char row = 0;
//	unsigned char col = 0;
	unsigned char j   = 0;

	// Work on the page under the grid cursor
	Pagestruct* target_page 	= &Page_repository[GRID_CURSOR];	
	Stepstruct* target_step 	= target_page->
									Step	[target_page->stepSelectionSingleRow]
											[target_page->stepSelectionSingleCol];
	Trackstruct* target_track 	= target_page->
									Track	[target_page->stepSelectionSingleRow];
	

//	// This is a regular scale key
//	if ( (col >= 2) && (col <= 13) ){						
//		if ( ( 		target_page->Track[row]->attr_PIT 
//				+ 	(target_step->attr_PIT / 12) * 12 
//				+ 	(col - 2) ) < STEP_MAX_PITCH 
//			){
//			// Adjust the pitch accordingly	by keypress
//			target_step->attr_PIT =
//					(target_step->attr_PIT / 12) * 12 
//				+ 	(col - 2);
//		}
//	}
//	// This is the octave- (8vb) button
//	else if (col == 14){
//		if ( 		target_page->Track[row]->attr_PIT 
//				+ target_step->attr_PIT >= (STEP_MIN_PITCH + 12) 
//			){
//			target_step->attr_PIT -= 12;
//		}
//	}
//	// This is the octave+ (8va) button
//	else if (col == 15){
//		if ( 		target_page->Track[row]->attr_PIT
//				+ target_step->attr_PIT <= (STEP_MAX_PITCH - 12) 
//			){
//			target_step->attr_PIT += 12;
//		}
//	}

	// Compute the col of the keypress
	j = (keyNdx / 11) - 1;


	switch( j ){

		case 14:
			// If the pitch is a C already, transpose octave down
			if ((( target_track->attr_PIT 
				   + target_step->attr_PIT ) %12 ) == 0 ){

				// Make sure we stay within bounds
				if ( target_step->attr_PIT > STEP_MIN_PITCH ){ 
					
					// Go down one octave and normalize it back
					target_step->attr_PIT -= 12;
					if ( target_step->attr_PIT < STEP_MIN_PITCH ){
						target_step->attr_PIT = STEP_MIN_PITCH;
					}
				}
			}
			else{
				// The general case of pitch selection
				if ( target_track->attr_PIT
					 + target_step->attr_PIT >= 0 ){

					target_step->attr_PIT =
//								( target_step->attr_PIT / 12 )*12 + j;

					(( target_track->attr_PIT +
					   + target_step->attr_PIT ) /12 ) *12 
					 + j
					 - target_track->attr_PIT;

				}
				else{

					target_step->attr_PIT =
					
						(  (  -1*target_track->attr_PIT 
							+ target_step->attr_PIT - 11) / 12 ) * 12 + (j)
							+ target_track->attr_PIT;
				}
			}

			break;


		case 15:
			// Go up one octave
			target_step->attr_PIT += 12;

			// Stay within bounds
			target_step->attr_PIT = 
				normalize( target_step->attr_PIT, STEP_MIN_PITCH, STEP_MAX_PITCH );

			break;


		default:
			// The general case of pitch selection

			// Dealing with positve general values
			if ((  target_track->attr_PIT 
				   + target_step->attr_PIT ) >= 0 ){

				// Work on the total sum and split the sum again.
				target_step->attr_PIT =
				
					(( target_track->attr_PIT +
					   + target_step->attr_PIT ) /12 ) *12 
					 + j
					 - target_track->attr_PIT;

				// Stay within bounds
				target_step->attr_PIT = 
					normalize( target_step->attr_PIT, STEP_MIN_PITCH, STEP_MAX_PITCH );
			}
			// Dealing with negative values
			else{

				target_step->attr_PIT =

					(( -1* target_track->attr_PIT
					     + target_step->attr_PIT - 11) /12 ) *12 
					 + (j)
					 + target_track->attr_PIT;
			}

			break;

	} // switch( j );

}





