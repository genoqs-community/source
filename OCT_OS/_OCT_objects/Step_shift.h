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


// Team_CE_OS 26.10.2019
// Routines for DIR Rotary pretaining to FEATURE_STEP_SHIFT
// Includes Vertical Step Swap function
// Updated 26.02.2021
// Matched Overrun's commented out 01.09.2023

#include "../_OCT_objects/Runtime.h"

// Horizontal Step Shift when in Preview_Perform mode
void StepShiftRot( Pagestruct* target_pageSft, intn direction )
{
	unsigned int heldcol = 0;
	unsigned int heldrow = 0;
	unsigned int heldmax = 15; // Last Step of Held Row
	int i = 0;
	int copycol = 0; // Copy Column Held Row
	int orcopycol = 0; // Copy Column Overrun Row
	int pastecol = 0; // Paste Column
	int last_copycol = 0; // hypersteps at start of overrun track (DEC) or end of heldrow (INC)
	int target_row = 0; // Current Overrun Row


	if ( G_zoom_level == zoomSTEP ) {
		heldrow = target_pageSft->stepSelectionSingleRow;
		heldcol = target_pageSft->stepSelectionSingleCol;
	}
	else {
		// Preview Perform mode
		heldrow = page_preview_step_row;
		heldcol = page_preview_step_col;
	}

	switch( direction ) {

		case DEC:
			// Is held step the last step of the last row
			if (  ( heldrow == MATRIX_NROF_ROWS - 1 ) && ( heldcol == 15 )  ) break;

			// Get Status of Step adjacent to held step
			if (   ( ( Step_get_status( target_pageSft->Step[heldrow][heldcol+1], STEPSTAT_SKIP )) == ON  )
						|| ( Step_get_status( target_pageSft->Step[heldrow][heldcol+1], STEPSTAT_EVENT ) == ON )
						|| ( Step_get_status( target_pageSft->Step[heldrow][heldcol+1], STEPSTAT_TOGGLE ) == ON )
						|| ( target_pageSft->Step[heldrow][heldcol+1]->hyperTrack_ndx != 10 )  )  {
				// Shift not possible
				break;
			}

			// Start with check for last copycol & hyperstep(s)
			last_copycol = 15;
			while (target_pageSft->Step[heldrow][last_copycol]->hyperTrack_ndx != 10 ) {
				// Find the last (non-hyper) step to be copied before end of the track
				last_copycol --;
			}


			if ( last_copycol > heldcol+1 ) {
				// Shift possible (subject to hyperstep check)
				copycol = heldcol +2;
				pastecol = heldcol +1;

				while ( copycol <= last_copycol )
				{
					while  (  (target_pageSft->Step[heldrow][copycol]->hyperTrack_ndx != 10 )
								&& ( copycol <= last_copycol )  )   {
						// Do not copy hyperstep
						copycol++;
					}
					if ( copycol <= last_copycol ) {
						STEP_COPY_BUFFER = target_pageSft->Step[heldrow][copycol];
					}
					while ( target_pageSft->Step[heldrow][pastecol]->hyperTrack_ndx != 10 ) {
						// Do not paste to hyperstep
						pastecol++;
					}
					if ( pastecol < last_copycol )  {
						Step_copy( STEP_COPY_BUFFER, target_pageSft->Step[heldrow][pastecol], 0 );
					}
					copycol++;
					pastecol++;
				} // while
			} // last_copycol > heldcol+1

			if ( heldrow == MATRIX_NROF_ROWS - 1) {
				// heldrow is last row hence no overrun
				// Initialize last copied step
				Step_init (target_pageSft->Step[heldrow][last_copycol]);
				break;
			}

			// Check if Track is set to Overrun to next track lower in matrix
			if ( target_pageSft->Track[heldrow]->attr_AMT == 100 ) {
				target_row = heldrow + 1;
				orcopycol = 0;
				pastecol = 0;
				// Check if Step 16 is the held step
				if ( heldcol == heldmax ) {
					if (   ( ( Step_get_status( target_pageSft->Step[target_row][orcopycol], STEPSTAT_SKIP )) == ON  )
							|| ( Step_get_status( target_pageSft->Step[target_row][orcopycol], STEPSTAT_EVENT ) == ON )
							|| ( Step_get_status( target_pageSft->Step[target_row][orcopycol], STEPSTAT_TOGGLE ) == ON )
							|| ( target_pageSft->Step[target_row][orcopycol]->hyperTrack_ndx != 10 )  )  {
						// Shift not possible
						break;
					}
				}

				while (  ( target_pageSft->Track[target_row]->attr_AMT == 100 )  && ( target_row < MATRIX_NROF_ROWS )  )
				{
					orcopycol = 0;
					// Copy & Paste between rows
					// Check for Hyperstep on nextrow step 1 onwards
					while (target_pageSft->Step[target_row][orcopycol]->hyperTrack_ndx != 10 )  {
						// Do not copy hyperstep
						orcopycol++;
					}
					STEP_COPY_BUFFER = target_pageSft->Step[target_row][orcopycol];

					// last_copycol resolved on heldrow (or previous row)
					pastecol = last_copycol;

					Step_copy( STEP_COPY_BUFFER, target_pageSft->Step[target_row-1][pastecol], 0 );

					orcopycol++;
					pastecol = 0;

					last_copycol = 15;
					while (target_pageSft->Step[target_row][last_copycol]->hyperTrack_ndx != 10 ) {
						// Find the last (non-hyper) step to be copied before end of the overrun track
						last_copycol --;
					}
					// Shift overrun row
					while ( orcopycol <= last_copycol )
					{
						while  (  (target_pageSft->Step[target_row][orcopycol]->hyperTrack_ndx != 10 )
									&& ( orcopycol <= last_copycol )  )   {
							// Do not copy hyperstep
							orcopycol++;
						}
						if ( orcopycol <= last_copycol ) {
							STEP_COPY_BUFFER = target_pageSft->Step[target_row][orcopycol];
						}
						while (target_pageSft->Step[target_row][pastecol]->hyperTrack_ndx != 10 )  {
							// Do not paste to hyperstep
							pastecol++;
						}
						if ( pastecol < last_copycol )  {
							Step_copy( STEP_COPY_BUFFER, target_pageSft->Step[target_row][pastecol], 0 );
						}
						orcopycol++;
						pastecol++;
					} // while
					target_row++;
				} // while ( target_pageSft->Track[target_row]->attr_AMT == 100 )

				if ( target_row < MATRIX_NROF_ROWS ) {
					// Shift row below Track with AMT == 100
					orcopycol = 0;
					// Copy & Paste between rows
					// Check for Hyperstep on nextrow step 1 onwards
					while (target_pageSft->Step[target_row][orcopycol]->hyperTrack_ndx != 10 )  {
						// Do not copy hyperstep
						orcopycol++;
					}
					STEP_COPY_BUFFER = target_pageSft->Step[target_row][orcopycol];

					// last_copycol resolved on heldrow (or previous row)
					pastecol = last_copycol;

					Step_copy( STEP_COPY_BUFFER, target_pageSft->Step[target_row-1][pastecol], 0 );

					orcopycol++;
					pastecol = 0;

					last_copycol = 15;
					while (target_pageSft->Step[target_row][last_copycol]->hyperTrack_ndx != 10 ) {
						// Find the last (non-hyper) step to be copied before end of the overrun track
						last_copycol --;
					}
					// Shift overrun row
					while ( orcopycol <= last_copycol )
					{
						while  (  (target_pageSft->Step[target_row][orcopycol]->hyperTrack_ndx != 10 )
									&& ( orcopycol <= last_copycol )  )   {
							// Do not copy hyperstep
							orcopycol++;
						}
						if ( orcopycol <= last_copycol ) {
							STEP_COPY_BUFFER = target_pageSft->Step[target_row][orcopycol];
						}
						while (target_pageSft->Step[target_row][pastecol]->hyperTrack_ndx != 10 )  {
							// Do not paste to hyperstep
							pastecol++;
						}
						if ( pastecol < last_copycol )  {
							Step_copy( STEP_COPY_BUFFER, target_pageSft->Step[target_row][pastecol], 0 );
						}
						orcopycol++;
						pastecol++;
					} // while
					// Initialize last copied step of overrun row
					Step_init (target_pageSft->Step[target_row][last_copycol]);
				}
				else {
					// Initialize last copied step of overrun row
					Step_init (target_pageSft->Step[target_row-1][last_copycol]);
				}
			} // AMT == 100
			else {
				target_row = 0;
				if ( target_pageSft->Track[heldrow]->attr_AMT != 50 ) {
					//for ( i = MATRIX_NROF_ROWS-1; i > heldrow; i -- ) {
					for ( i = heldrow+1; i < MATRIX_NROF_ROWS; i ++ ) {
						if (  (target_pageSft->Track[heldrow]->attr_AMT - target_pageSft->Track[i]->attr_AMT ) == 0 ) {
							// Match Track AMT's for overrun (lowest matched track in matrix will be designated the overrun)
							target_row = i;
						}
					}
				}
				if ( target_row > heldrow ) {
					// Overrun Track
					orcopycol = 0;
					pastecol = 0;
					// Check if Step 16 is the held step
					if ( heldcol == heldmax ) {
						if (   ( ( Step_get_status( target_pageSft->Step[target_row][orcopycol], STEPSTAT_SKIP )) == ON  )
								|| ( Step_get_status( target_pageSft->Step[target_row][orcopycol], STEPSTAT_EVENT ) == ON )
								|| ( Step_get_status( target_pageSft->Step[target_row][orcopycol], STEPSTAT_TOGGLE ) == ON )
								|| ( target_pageSft->Step[target_row][target_row]->hyperTrack_ndx != 10 )  )  {
							// Shift not possible
							break;
						}
					}

					// Check for Hyperstep on overrunrow step 1 onwards
					while (target_pageSft->Step[target_row][orcopycol]->hyperTrack_ndx != 10 )  {
						// Do not copy hyperstep
						orcopycol++;
					}
					STEP_COPY_BUFFER = target_pageSft->Step[target_row][orcopycol];

					// last_copycol resolved on heldrow
					pastecol = last_copycol;

					Step_copy( STEP_COPY_BUFFER, target_pageSft->Step[heldrow][pastecol], 0 );

					orcopycol++;
					pastecol = 0;

					last_copycol = 15;
					while (target_pageSft->Step[target_row][last_copycol]->hyperTrack_ndx != 10 ) {
						// Find the last (non-hyper) step to be copied before end of overrun track
						last_copycol --;
					}

					// Shift overrun row
					while ( orcopycol <= last_copycol )
					{
						while  (  (target_pageSft->Step[target_row][orcopycol]->hyperTrack_ndx != 10 )
									&& ( orcopycol <= last_copycol )  )   {
							// Do not copy hyperstep
							orcopycol++;
						}
						if ( orcopycol <= last_copycol ) {
							STEP_COPY_BUFFER = target_pageSft->Step[target_row][orcopycol];
						}
						while (target_pageSft->Step[target_row][pastecol]->hyperTrack_ndx != 10 )  {
							// Do not paste to hyperstep
							pastecol++;
						}
						if ( pastecol < last_copycol )  {
							Step_copy( STEP_COPY_BUFFER, target_pageSft->Step[target_row][pastecol], 0 );
						}
						orcopycol++;
						pastecol++;

					} // while

					// Initialize last copied step of overrunrow
					Step_init (target_pageSft->Step[target_row][last_copycol]);

				} // matched overrun track
				else {
					if ( last_copycol > heldcol+1 )  {
						// No overrun hence Initialize last copied step of heldrow
						Step_init (target_pageSft->Step[heldrow][last_copycol]);
					}
				}
			} // else AMT

			break;


		case INC:
			// Check if heldcol is step 16 or heldcol plus any H-S adds up to heldmax
			if ( heldcol == heldmax ) {
				last_copycol = 0;
			}
			else {
				last_copycol = heldcol+1;
				while (  (target_pageSft->Step[heldrow][last_copycol]->hyperTrack_ndx != 10 ) && ( last_copycol <= heldmax )  ) {
					last_copycol++;
				}
				if ( ( last_copycol == 15 ) && ( target_pageSft->Step[heldrow][last_copycol]->hyperTrack_ndx != 10 ) ) {
					// Hyperstep(s) from heldcol to end of track
					last_copycol = 0;
				}
			}
			target_row = heldrow + 1;
			copycol = 14;
			pastecol = 15;

			if (  ( target_pageSft->Track[heldrow]->attr_AMT == 100 ) && ( heldrow != MATRIX_NROF_ROWS -1 )  )  {
				// Find last row of shift
				while (  ( target_pageSft->Track[target_row]->attr_AMT == 100 )  && ( target_row < MATRIX_NROF_ROWS -1 )  )
				{
				 	target_row++;
				}

				while ( target_row > heldrow  )
				{
					pastecol = 15;
					while (target_pageSft->Step[target_row][pastecol]->hyperTrack_ndx != 10 )
					{
						// Check for hyperstep(s) at end of last overrun track
						pastecol--;
					}
					// Set first copy column
		 			orcopycol = pastecol-1;


					// Shift nextrow
					while ( orcopycol >= 0 )
					{
						while (target_pageSft->Step[target_row][orcopycol]->hyperTrack_ndx != 10 )  {
							// Do not copy hyperstep
							orcopycol--;
						}
						if ( orcopycol >= 0 ) {
							STEP_COPY_BUFFER = target_pageSft->Step[target_row][orcopycol];
						}
						while (target_pageSft->Step[target_row][pastecol]->hyperTrack_ndx != 10 )  {
							// Do not paste to hyperstep
							pastecol--;
						}
						if ( pastecol > 0 )  {
							Step_copy( STEP_COPY_BUFFER, target_pageSft->Step[target_row][pastecol], 0 );
						}
						orcopycol--;
						pastecol--;
					} // while

					if ( target_row != heldrow+1 ) {
						// Copy & Paste between rows (if not row below heldrow)
						orcopycol = 15;
						// Check for Hyperstep on nextrow-1 step 16 (& earlier)
						while (target_pageSft->Step[target_row-1][orcopycol]->hyperTrack_ndx != 10 )  {
							// Do not copy hyperstep
							orcopycol--;
						}
						STEP_COPY_BUFFER = target_pageSft->Step[target_row-1][orcopycol];
						pastecol = 0;
						// Check for Hyperstep on step 1(& later) of previous row
						while (target_pageSft->Step[target_row][pastecol]->hyperTrack_ndx != 10 )  {
							pastecol++;
						}
						Step_copy( STEP_COPY_BUFFER, target_pageSft->Step[target_row][pastecol], 0 );
					}
					else {
						// If Step 16 is the held step
						if ( last_copycol == 0 ) {
							pastecol = 0;
							// Check for Hyperstep on step 1(& later) of previous row
							while (target_pageSft->Step[target_row][pastecol]->hyperTrack_ndx != 10 )  {
								pastecol++;
							}
							Step_init (target_pageSft->Step[heldrow+1][pastecol]);
						}
						else  {
							// Copy & Paste between rows
							orcopycol = 15;
							// Check for Hyperstep on nextrow-1 step 16 (& earlier)
							while (target_pageSft->Step[target_row-1][orcopycol]->hyperTrack_ndx != 10 )  {
								// Do not copy hyperstep
								orcopycol--;
							}
							STEP_COPY_BUFFER = target_pageSft->Step[target_row-1][orcopycol];
							pastecol = 0;
							// Check for Hyperstep on step 1(& later) of previous row
							while (target_pageSft->Step[target_row][pastecol]->hyperTrack_ndx != 10 )  {
								pastecol++;
							}
							Step_copy( STEP_COPY_BUFFER, target_pageSft->Step[target_row][pastecol], 0 );
						}
					}
					pastecol = orcopycol;
					orcopycol--;

					target_row--;

				} // while (target_row > heldrow)

			} // attr_AMT == 100 & not heldrow
			else {
				target_row = 0;
				if ( target_pageSft->Track[heldrow]->attr_AMT != 50 ) {
					//for ( i = MATRIX_NROF_ROWS-1; i > heldrow; i -- ) {
					for ( i = heldrow+1; i < MATRIX_NROF_ROWS; i ++ ) {
						if (  (target_pageSft->Track[heldrow]->attr_AMT - target_pageSft->Track[i]->attr_AMT ) == 0 ) {
							// Match Track AMT's for overrun (lowest matched track in matrix will be designated the overrun)
							target_row = i;
						}
					}
				}

				if ( target_row > heldrow ) {
					pastecol = 15;
					// Check from hyperstep at end of track
					while (target_pageSft->Step[target_row][pastecol]->hyperTrack_ndx != 10 )  {
						pastecol--;
					}
					orcopycol = pastecol-1;
					// Shift overrun row
					while ( orcopycol >= 0 )
					{
						while (target_pageSft->Step[target_row][orcopycol]->hyperTrack_ndx != 10 )  {
							// Do not copy hyperstep
							orcopycol--;
						}
						if ( orcopycol >= 0 ) {
							STEP_COPY_BUFFER = target_pageSft->Step[target_row][orcopycol];
						}
						while (target_pageSft->Step[target_row][pastecol]->hyperTrack_ndx != 10 )  {
							// Do not paste to hyperstep
							pastecol--;
						}
						if ( pastecol > 0 )  {
							Step_copy( STEP_COPY_BUFFER, target_pageSft->Step[target_row][pastecol], 0 );
						}
						orcopycol--;
						pastecol--;
					} // while

					// If Step 16 is the held step
					if ( last_copycol == 0 ) {
						pastecol = 0;
						// Check for Hyperstep on step 1(& later) of previous row
						while (target_pageSft->Step[target_row][pastecol]->hyperTrack_ndx != 10 )  {
							pastecol++;
						}
						Step_init (target_pageSft->Step[target_row][pastecol]);
					}
					else {
						// Copy & Paste between rows
						copycol = 15;
						// Check for Hyperstep on nextrow step 16 (& earlier)
						while (target_pageSft->Step[heldrow][copycol]->hyperTrack_ndx != 10 )  {
							// Do not copy hyperstep
							copycol--;
						}
						STEP_COPY_BUFFER = target_pageSft->Step[heldrow][copycol];
						pastecol = 0;
						// Check for Hyperstep on step 1(& later) of previous row
						while (target_pageSft->Step[target_row][pastecol]->hyperTrack_ndx != 10 )  {
							pastecol++;
						}
						Step_copy( STEP_COPY_BUFFER, target_pageSft->Step[target_row][pastecol], 0 );
					}

				}  // target_row > heldrow
			}  // else


			// Check for hyperstep(s) at end of track
			pastecol = 15;
			while (target_pageSft->Step[heldrow][pastecol]->hyperTrack_ndx != 10 )  {
				pastecol--;
			}
			copycol = pastecol-1;
			// Copy & Paste Track of Held Step
			while ( copycol > heldcol )
			{
				while (target_pageSft->Step[heldrow][copycol]->hyperTrack_ndx != 10 )  {
					// Do not copy hyperstep
					copycol--;
				}
				if ( copycol > heldcol ) {
					STEP_COPY_BUFFER = target_pageSft->Step[heldrow][copycol];
				}
				while (target_pageSft->Step[heldrow][pastecol]->hyperTrack_ndx != 10 )  {
					// Do not paste to hyperstep
					pastecol--;
				}
				if ( pastecol <= heldmax )    {
					Step_copy( STEP_COPY_BUFFER, target_pageSft->Step[heldrow][pastecol], 0 );
				}
				copycol--;
				pastecol--;
			} // while
			// Turn off step next to held step
			if ( last_copycol != 0 ) {
				pastecol = heldcol+1;
				// Check for Hyperstep next to heldcol
				while (target_pageSft->Step[heldrow][pastecol]->hyperTrack_ndx != 10 )  {
					pastecol++;
				}
				Step_init (target_pageSft->Step[heldrow][pastecol]);
			}
		break;

	}  //switch direction

} //function



// Vertical Step Swap - if a Step Selection has all Selected Step(s) in the same row
// Step will be automatically removed from the Selection if a Hyperstep
// 								is either the Source or Destination of the Swap
// If more than one Track has AMT_attr=0 they will be designated swap tracks
void StepSwapVertical( Pagestruct* target_pageSwp, intn direction )
{
	int selectrow = 0; // Row containing Selected Steps
	int swaprow; // Swap destination
	int nrofswaps = 0; // Number of designated swap rows
	int searchrow = 0; // Search for swap row
	int rows_with_sel_steps = 0;
	int cols_with_sel_steps = 0;

	// Check if Selected Step(s) are in the same row?
	unsigned char row = 0;
	unsigned char col = 0;

	for ( row=0; row < MATRIX_NROF_ROWS; row++ )  {

		for ( col=0; col < MATRIX_NROF_COLUMNS; col++ ){

			// Is the step ON in reality?
			if ( Step_get_status( target_pageSwp->Step[row][col], STEPSTAT_TOGGLE ) == ON ) {

				// Is the Step Selected?
				if (Step_get_status( target_pageSwp->Step[row][col], STEPSTAT_SELECT ) == ON ) {
					cols_with_sel_steps = cols_with_sel_steps + 1;
				}
			}

		} // Column iterator
		if ( cols_with_sel_steps > 0 ) {
			rows_with_sel_steps = rows_with_sel_steps + 1;
			selectrow = row;
		}
		cols_with_sel_steps = 0;
		if ( rows_with_sel_steps > 1 ) {
			// Selection Steps in more than one row
			break;
		}
	} // Row iterator

	if ( rows_with_sel_steps == 1 ) {
		// Find number of swap rows - if any
		for ( row=0; row < MATRIX_NROF_ROWS; row++ )  {
			if ( target_pageSwp->Track[row]->attr_AMT == 0 ) {
				nrofswaps = nrofswaps +1;
			}
		} // Row iterator

		switch( direction ) {

			case DEC:
				if ( selectrow >0 ) {
					// Do swap with all selected steps in the row
					// Swap Upward
					swaprow = selectrow -1;
					if ( ( target_pageSwp->Track[selectrow]->attr_AMT == 0 )
							&& ( nrofswaps >1 ) ) {
						// Search for a swaprow higher in the matrix
						swaprow = 99; // No swaps on Upward rows
						searchrow = selectrow -1;
						while ( searchrow >= 0 )
						{
							if ( target_pageSwp->Track[searchrow]->attr_AMT == 0 ) {
								swaprow = searchrow;
								break;
							}
							else  {
								searchrow --;
							}
						}

					}
					if ( ( target_pageSwp->Track[selectrow]->attr_AMT != 0 )
							&& ( nrofswaps >1 ) ) {
						// Search for a swaprow higher in the matrix
						swaprow = 99; // No swaps on Upward rows
						searchrow = selectrow -1;
						while ( searchrow >= 0 )
						{
							if ( target_pageSwp->Track[searchrow]->attr_AMT != 0 ) {
								swaprow = searchrow;
								break;
							}
							else  {
								searchrow --;
							}
						}

					}
					if ( swaprow != 99 ) {
						// Swap row available
						for ( col=0; col < MATRIX_NROF_COLUMNS; col++ ){

							// Is the step ON in reality?
							if ( Step_get_status( target_pageSwp->Step[selectrow][col], STEPSTAT_TOGGLE ) == ON ) {

								// Check if a Hyperstep is the source or destination
								if ( (target_pageSwp->Step[selectrow][col]->hyperTrack_ndx != 10 )
										|| (target_pageSwp->Step[swaprow][col]->hyperTrack_ndx != 10 ) ) {
									Step_set_status( target_pageSwp->Step[selectrow][col], STEPSTAT_SELECT, OFF );
								}
								// Is the Step Selected?
								if ( ( Step_get_status( target_pageSwp->Step[selectrow][col], STEPSTAT_SELECT ) == ON )
										&& (target_pageSwp->Step[selectrow][col]->hyperTrack_ndx == 10 )  )   {
									Step_swap( target_pageSwp, target_pageSwp->Step[selectrow][col], target_pageSwp->Step[swaprow][col] );
								}
							}

						} // Column iterator
					} // swaprow
				}
				break;


			case INC:
				if ( selectrow < MATRIX_NROF_ROWS - 1 ) {
					// Do swap with all selected steps in the row
					// Swap Downward
					swaprow = selectrow +1;
					if ( ( target_pageSwp->Track[selectrow]->attr_AMT == 0 )
							&& ( nrofswaps >1 ) ) {
						// Search for a swaprow higher in the matrix
						swaprow = 99; // No swaps on Downward rows
						searchrow = selectrow +1;
						while ( searchrow <= MATRIX_NROF_ROWS - 1 )
						{
							if ( target_pageSwp->Track[searchrow]->attr_AMT == 0 ) {
								swaprow = searchrow;
								break;
							}
							else  {
								searchrow ++;
							}
						}
					}
					if ( ( target_pageSwp->Track[selectrow]->attr_AMT != 0 )
							&& ( nrofswaps >1 ) ) {
						// Search for a swaprow higher in the matrix
						swaprow = 99; // No swaps on Downward rows
						searchrow = selectrow +1;
						while ( searchrow <= MATRIX_NROF_ROWS - 1 )
						{
							if ( target_pageSwp->Track[searchrow]->attr_AMT != 0 ) {
								swaprow = searchrow;
								break;
							}
							else  {
								searchrow ++;
							}
						}
					}
					if ( swaprow != 99 ) {
						for ( col=0; col < MATRIX_NROF_COLUMNS; col++ ){

							// Is the step ON in reality?
							if ( Step_get_status( target_pageSwp->Step[selectrow][col], STEPSTAT_TOGGLE ) == ON ) {

								// Check if a Hyperstep is the source or destination
								if ( (target_pageSwp->Step[selectrow][col]->hyperTrack_ndx != 10 )
										|| (target_pageSwp->Step[swaprow][col]->hyperTrack_ndx != 10 ) ) {
									Step_set_status( target_pageSwp->Step[selectrow][col], STEPSTAT_SELECT, OFF );
								}

								// Is the Step Selected?
								if ( ( Step_get_status( target_pageSwp->Step[selectrow][col], STEPSTAT_SELECT ) == ON )
										&& (target_pageSwp->Step[selectrow][col]->hyperTrack_ndx == 10 )  )   {
									Step_swap( target_pageSwp, target_pageSwp->Step[selectrow][col], target_pageSwp->Step[swaprow][col] );
								}
							}
						} // Column iterator
					} // Swaprow
				}
				break;

		} // Switch

	} // rows_with_sel_steps == 1
} // function

