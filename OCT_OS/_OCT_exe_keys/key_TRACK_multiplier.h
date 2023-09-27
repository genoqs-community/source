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






#ifdef FEATURE_TEMPO_MULT_PLUS
// Set the Track Tempo Multiplier using the MCH row Matrix keys
if ( is_pressed_key( KEY_TEMPO ) ) {

	//if ( (keyNdx > 10) && (keyNdx <= 185) && (((keyNdx-10) % 11) != 0) ) {

		//if ( ((keyNdx - MIDICH) % 11) == 0) {

	unsigned char j_dblclk = 0;

	if (	( DOUBLE_CLICK_TARGET == keyNdx )
			&& 	( DOUBLE_CLICK_TIMER > DOUBLE_CLICK_ALARM_SENSITIVITY )
			) {


		// DOUBLE CLICK CODE
		// Determine the wanted divisor / skip value..
		// ..and store it into the track attribute

		j_dblclk = ((keyNdx - MIDICH) / 11);


			switch (j_dblclk )  {

				case 2: case 3: case 4: case 5: case 6: case 7: case 8: case 16:
					j = j_dblclk - 1;
					break;

				case 15:
					// 1/1.5 divisor
					// Set target_track->attr_TEMPOMUL to 25
					j = 25;
					break;


				default:
					j_dblclk = 0;
					break;

			}

			if ( j_dblclk != 0 )  {
				// Only set tempo multiplier if valid case

				// Set the tempo multiplier of the selected tracks
				for ( i=0; i < MATRIX_NROF_ROWS; i++ ){

					// If Track[i] is selected
					if ( target_page->trackSelection & (1<<i) ){

						// If the track is not playing, it is safe to adjust the multiplier immediately
						if ( target_page->Track[i]->attr_LOCATOR == 0 )  {

							if ( j == 25 ) {
								// Special case ( 1/1.5 divisor)
								// target_page->Track[i]->attr_TEMPOMUL = 25;
								target_page->Track[i]->attr_TEMPOMUL 	= j;
								target_page->Track[i]->attr_TEMPOMUL_SKIP	= 0;
							}
							else {
								// Update the divisor and clear the buffer
								target_page->Track[i]->attr_TEMPOMUL 	= 1;

								// Update the divisor and clear the buffer
								target_page->Track[i]->attr_TEMPOMUL_SKIP	= j;
							}
						} // current Track is not playing

						else {

							if ( j == 25 ) {
								// Special case ( 1/1.5 divisor)
								// target_page->Track[i]->attr_TEMPOMUL = 25;
								track_tempo_mul = 25;
								track_tempo_div = 0;
							}
							else {

								// Set the tempo multiplier of selected tracks
								track_tempo_div = j;

								// Set the tempo multiplier of selected tracks
								// target_page->Track[i]->attr_TEMPOMUL = 1;
								track_tempo_mul = 1;

							}
							// Remember the track for the clock change
							track_tempo_ptr = target_page->Track[i];

							// Remember the page for the clock change
							track_tempo_page = target_page;
						}
					} // selection
				} // row iterator
			}	// j_dblclk != 0
		}

		// Start thinking about double clicking
		else if ( DOUBLE_CLICK_TARGET == 0 ) {

			// Prepare double click timer for zooming into the step
			DOUBLE_CLICK_TARGET = keyNdx;
			DOUBLE_CLICK_TIMER = ON;

			// Start the Double click Alarm
			cyg_alarm_initialize(
				doubleClickAlarm_hdl,
				cyg_current_time() + DOUBLE_CLICK_ALARM_TIME,
				DOUBLE_CLICK_ALARM_TIME );

			// SINGLE CLICK CODE:
			// Determine the wanted multiplicator value..
			// ..and store it into the track attribute

			j = ((keyNdx - MIDICH) / 11);

			if ( ( j < 9 ) || ( j > 14 ) ) {
				// Only set tempo multiplier if valid case

				// Set the tempo multiplier of the selected tracks
				for ( i=0; i < MATRIX_NROF_ROWS; i++ ){

					// If Track[i] is selected
					if ( target_page->trackSelection & (1<<i) ){

						// If the track is not playing, it is safe to adjust the multiplier immediately
						if ( target_page->Track[i]->attr_LOCATOR == 0 )  {

							// Update the multipler and clear the buffer
							target_page->Track[i]->attr_TEMPOMUL 		= j;

							// Update the divisor and clear the buffer
							target_page->Track[i]->attr_TEMPOMUL_SKIP	= 0;
						} // current Track is not playing

						// Track is playing so queue until the next global TTC1
						else {

							// Set the tempo multiplier of selected tracks
							track_tempo_mul = j;

							// Make sure to get rid of any skip values
							track_tempo_div = 0;

							// Remember the track for the clock change
							track_tempo_ptr = target_page->Track[i];

							// Remember the page for the clock change
							track_tempo_page = target_page;
						}
					}
				} // row iterator
			} // ( ( j < 9 ) || ( i > 14 ) )
		} // single click scenario


}  //( is_pressed_key( KEY_TEMPO ) )



else {

	if (	( DOUBLE_CLICK_TARGET == keyNdx )
		&& 	( DOUBLE_CLICK_TIMER > DOUBLE_CLICK_ALARM_SENSITIVITY )
		) {

	// DOUBLE CLICK was started in the ON toggle state section


		// DOUBLE CLICK CODE
		// Determine the wanted multiplicator / skip value..
		// ..and store it into the track attribute
		switch( keyNdx ){

			case KEY_PLAY1:
				if ( 	is_pressed_key( KEY_PLAY4 )
					&&	is_pressed_key( KEY_PLAY2 )
					){
					// Divisor 16
					j = 15;
				}

				// Inverse triplet multiplier - including special treatment
				else if ( is_pressed_key( KEY_PLAY2 )){

					j = 25;
/*
					// Set the tempo multiplier of the selected tracks
					for ( i=0; i < MATRIX_NROF_ROWS; i++ ){

						// If Track[i] is selected
						if ( target_page->trackSelection & (1<<i) ){

							// If the track is not playing, it is safe to adjust the multiplier immediately
							if ( target_page->Track[i]->attr_LOCATOR == 0 ){
////////////////////////////////// Bug Fix ( PLUS_X Spreadsheet case 101)
								// Special case ( 1/1.5 divisor)
								// target_page->Track[i]->attr_TEMPOMUL = 25;
								target_page->Track[i]->attr_TEMPOMUL 	= j;
								target_page->Track[i]->attr_TEMPOMUL_SKIP	= 0;
							}
							else  {
								// Set the tempo multiplier of selected tracks
								track_tempo_div = 0;

								// Set the tempo multiplier of selected tracks
								// target_page->Track[i]->attr_TEMPOMUL = j;
								track_tempo_mul = j;

								// Remember the track for the clock change
								track_tempo_ptr = target_page->Track[i];

								// Remember the track for the clock change
								track_tempo_page = target_page;
							}
						}
					} // row iterator
					return;
*/
				}
				break;

			case KEY_PLAY2:
				// Divisor 2
				j = 1;

				if ( is_pressed_key( KEY_PLAY1 )){
					// Divisor 3
					j = 2;
				}
				else if ( is_pressed_key( KEY_PLAY4 )){
					// Divisor 8
					j = 7;
				}
				break;

			case KEY_PLAY4:
				// Divisor 4
				j = 3;

				if ( is_pressed_key( KEY_PLAY1 )){
					// Divisor 5
					j = 4;
					if ( is_pressed_key( KEY_PLAY2 )){
						// Divisor 7
						j = 6;
					}
				}
				else if ( is_pressed_key( KEY_PLAY2 )){
					// Divisor 6
					j = 5;
				}
				break;
		}

		// Set the tempo multiplier of the selected tracks
		for ( i=0; i < MATRIX_NROF_ROWS; i++ ){

			// If Track[i] is selected
			if ( target_page->trackSelection & (1<<i) ){

				// If the track is not playing, it is safe to adjust the multiplier immediately
				if ( target_page->Track[i]->attr_LOCATOR == 0 ){

//////////////////// Bug Fix ( PLUS_X Spreadsheet case 101)
					if ( j == 25 ) {
						// Special case ( 1/1.5 divisor)
						// target_page->Track[i]->attr_TEMPOMUL = 25;
						target_page->Track[i]->attr_TEMPOMUL 	= j;
						target_page->Track[i]->attr_TEMPOMUL_SKIP	= 0;
					}
					else {
						// Update the multipler and clear the buffer
						target_page->Track[i]->attr_TEMPOMUL 		= 1;

						// Update the divisor and clear the buffer
						target_page->Track[i]->attr_TEMPOMUL_SKIP	= j;
					} // current Track is not playing
				}
				else {

					if ( j == 25 ) {
						// Special case ( 1/1.5 divisor)
						// target_page->Track[i]->attr_TEMPOMUL = 25;
						track_tempo_mul = 25;
						track_tempo_div = 0;
					}
					else {

						// Set the tempo multiplier of selected tracks
						// target_page->Track[i]->attr_TEMPOMUL = 1;
						track_tempo_mul = 1;

						// Set the tempo divisor of selected tracks
						track_tempo_div = j;

					}

					// Remember the track for the clock change
						track_tempo_ptr = target_page->Track[i];

					// Remember the track for the clock change
					track_tempo_page = target_page;

				} // current Track is not playing

			} // track selection
		} // row iterator
	}


	// Start thinking about double clicking
	else if ( DOUBLE_CLICK_TARGET == 0 ) {

		// Prepare double click timer for zooming into the step
		DOUBLE_CLICK_TARGET = keyNdx;
		DOUBLE_CLICK_TIMER = ON;

		// Start the Double click Alarm
		cyg_alarm_initialize(
			doubleClickAlarm_hdl,
			cyg_current_time() + DOUBLE_CLICK_ALARM_TIME,
			DOUBLE_CLICK_ALARM_TIME );

		// SINGLE CLICK CODE:
		// Determine the wanted multiplicator value..
		// ..and store it into the track attribute
		switch( keyNdx ){

			case KEY_PLAY1:
				j = 1;

				// Add one to the keypress
				if ( 	is_pressed_key( KEY_PLAY2 )
					&&	!is_pressed_key( KEY_PLAY4 )
					){
					// Triplet multiplier, so 1,5
					j = 15;
				}
				else if (	is_pressed_key( KEY_PLAY4 )
						&&	is_pressed_key( KEY_PLAY2 )
						){
					// Real 16!!
					j = 16;
				}
				break;

			case KEY_PLAY2:
				j = 2;

				// Add one to the keypress
				if ( is_pressed_key( KEY_PLAY1 )){
					j = 3;
				}
				else if ( is_pressed_key( KEY_PLAY4 )){
					j = 8;
				}
				break;

			case KEY_PLAY4:
				j = 4;

				// Add one to the keypress
				if ( is_pressed_key( KEY_PLAY1 )){
					j = 5;
					// Add two additional to the keypress
					if ( is_pressed_key( KEY_PLAY2 )){
						j = 7;
					}
				}
				else if ( is_pressed_key( KEY_PLAY2 )){
						j = 6;
				}
				break;
		}

		// Set the tempo multiplier of the selected tracks
		for ( i=0; i < MATRIX_NROF_ROWS; i++ ){

			// If Track[i] is selected
			if ( target_page->trackSelection & (1<<i) ){

				// If the track is not playing, it is safe to adjust the multiplier immediately
				if ( target_page->Track[i]->attr_LOCATOR == 0 ){

					// Update the multipler and clear the buffer
					target_page->Track[i]->attr_TEMPOMUL 		= j;

					// Update the divisor and clear the buffer
					target_page->Track[i]->attr_TEMPOMUL_SKIP	= 0;
				} // current Track is not playing

				// Track is playing so queue until the next global TTC1
				else{

					// Set the tempo multiplier of selected tracks
					track_tempo_mul = j;

					// Make sure to get rid of any skip values
					track_tempo_div = 0;

					// Remember the track for the clock change
					track_tempo_ptr = target_page->Track[i];

					// Remember the page for the clock change
					track_tempo_page = target_page;
				}
			}
		} // row iterator
	} // single click scenario

} //else


#else

if (	( DOUBLE_CLICK_TARGET == keyNdx )
	&& 	( DOUBLE_CLICK_TIMER > DOUBLE_CLICK_ALARM_SENSITIVITY )
	) {

// DOUBLE CLICK was started in the ON toggle state section


	// DOUBLE CLICK CODE
	// Determine the wanted multiplicator / skip value..
	// ..and store it into the track attribute
	switch( keyNdx ){

		case KEY_PLAY1:
			if ( 	is_pressed_key( KEY_PLAY4 )
				&&	is_pressed_key( KEY_PLAY2 )
				){
				// Divisor 16
				j = 15;
			}

			// Inverse triplet multiplier - including special treatment
			else if ( is_pressed_key( KEY_PLAY2 )){

				j = 25;

				// Set the tempo multiplier of the selected tracks
				for ( i=0; i < MATRIX_NROF_ROWS; i++ ){

					// If Track[i] is selected
					if ( target_page->trackSelection & (1<<i) ){

						// Set the tempo multiplier of selected tracks
						track_tempo_div = 0;

						// Set the tempo multiplier of selected tracks
						// target_page->Track[i]->attr_TEMPOMUL = j;
						track_tempo_mul = j;

						// Remember the track for the clock change
						track_tempo_ptr = target_page->Track[i];

						// Remember the track for the clock change
						track_tempo_page = target_page;
					}
				} // row iterator
				return;

			}
			break;

		case KEY_PLAY2:
			// Divisor 2
			j = 1;

			if ( is_pressed_key( KEY_PLAY1 )){
				// Divisor 3
				j = 2;
			}
			else if ( is_pressed_key( KEY_PLAY4 )){
				// Divisor 8
				j = 7;
			}
			break;

		case KEY_PLAY4:
			// Divisor 4
			j = 3;

			if ( is_pressed_key( KEY_PLAY1 )){
				// Divisor 5
				j = 4;
				if ( is_pressed_key( KEY_PLAY2 )){
					// Divisor 7
					j = 6;
				}
			}
			else if ( is_pressed_key( KEY_PLAY2 )){
				// Divisor 6
				j = 5;
			}
			break;
	}

	// Set the tempo multiplier of the selected tracks
	for ( i=0; i < MATRIX_NROF_ROWS; i++ ){

		// If Track[i] is selected
		if ( target_page->trackSelection & (1<<i) ){

			// If the track is not playing, it is safe to adjust the multiplier immediately
			if ( target_page->Track[i]->attr_LOCATOR == 0 ){

				// Update the multipler and clear the buffer
				target_page->Track[i]->attr_TEMPOMUL 	= 1;

				// Update the divisor and clear the buffer
				target_page->Track[i]->attr_TEMPOMUL_SKIP	= j;
			} // current Track is not playing

			else{
				// Set the tempo multiplier of selected tracks
				track_tempo_div = j;

				// Set the tempo multiplier of selected tracks
				// target_page->Track[i]->attr_TEMPOMUL = 1;
				track_tempo_mul = 1;

				// Remember the track for the clock change
				track_tempo_ptr = target_page->Track[i];

				// Remember the page for the clock change
				track_tempo_page = target_page;
			}
		}
	} // row iterator
}


// Start thinking about double clicking
else if ( DOUBLE_CLICK_TARGET == 0 ) {

	// Prepare double click timer for zooming into the step
	DOUBLE_CLICK_TARGET = keyNdx;
	DOUBLE_CLICK_TIMER = ON;

	// Start the Double click Alarm
	cyg_alarm_initialize(
		doubleClickAlarm_hdl,
		cyg_current_time() + DOUBLE_CLICK_ALARM_TIME,
		DOUBLE_CLICK_ALARM_TIME );

	// SINGLE CLICK CODE:
	// Determine the wanted multiplicator value..
	// ..and store it into the track attribute
	switch( keyNdx ){

		case KEY_PLAY1:
			j = 1;

			// Add one to the keypress
			if ( 	is_pressed_key( KEY_PLAY2 )
				&&	!is_pressed_key( KEY_PLAY4 )
				){
				// Triplet multiplier, so 1,5
				j = 15;
			}
			else if (	is_pressed_key( KEY_PLAY4 )
					&&	is_pressed_key( KEY_PLAY2 )
					){
				// Real 16!!
				j = 16;
			}
			break;

		case KEY_PLAY2:
			j = 2;

			// Add one to the keypress
			if ( is_pressed_key( KEY_PLAY1 )){
				j = 3;
			}
			else if ( is_pressed_key( KEY_PLAY4 )){
				j = 8;
			}
			break;

		case KEY_PLAY4:
			j = 4;

			// Add one to the keypress
			if ( is_pressed_key( KEY_PLAY1 )){
				j = 5;
				// Add two additional to the keypress
				if ( is_pressed_key( KEY_PLAY2 )){
					j = 7;
				}
			}
			else if ( is_pressed_key( KEY_PLAY2 )){
					j = 6;
			}
			break;
	}

	// Set the tempo multiplier of the selected tracks
	for ( i=0; i < MATRIX_NROF_ROWS; i++ ){

		// If Track[i] is selected
		if ( target_page->trackSelection & (1<<i) ){

			// If the track is not playing, it is safe to adjust the multiplier immediately
			if ( target_page->Track[i]->attr_LOCATOR == 0 ){

				// Update the multipler and clear the buffer
				target_page->Track[i]->attr_TEMPOMUL 		= j;

				// Update the divisor and clear the buffer
				target_page->Track[i]->attr_TEMPOMUL_SKIP	= 0;
			} // current Track is not playing

			// Track is playing so queue until the next global TTC1
			else{

				// Set the tempo multiplier of selected tracks
				track_tempo_mul = j;

				// Make sure to get rid of any skip values
				track_tempo_div = 0;

				// Remember the track for the clock change
				track_tempo_ptr = target_page->Track[i];

				// Remember the page for the clock change
				track_tempo_page = target_page;
			}
		}
	} // row iterator
} // single click scenario

#endif
