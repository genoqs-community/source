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


//
//
//// Key execution code for DEFAULT keymode
//
//

void key_exec_DICE( unsigned short keyNdx ){

	unsigned char i = 0;
	unsigned char col = 0;
	unsigned char offset = 0;

	Trackstruct* target_dice = Dice_get();
	//
	// TRACK_SELECTORS
	//

	// Do the normal operation - legacy..
	if (	(( keyNdx >= 1 ) && ( keyNdx <= 10 ))){
		if ( G_run_bit == ON )
		{
			dice_synced_attr 	= ATTR_LOCATOR;
			dice_synced_value 	= keyNdx;
		}
		else
		{
			DICE_bank->trackSelection ^= 1 << (keyNdx-1);
		}

	} // key is a track selector


	//
	// SELECT MASTER
	//
	if (keyNdx == KEY_SELECT_MASTER) {
		// Clear trackSelection if exists
		if (DICE_bank->trackSelection != 0) {

			// Store the track selection
			DICE_bank->trackSelectionStored = DICE_bank->trackSelection;

			// Clear the track selection
			DICE_bank->trackSelection = 0;
		}

		// Fill the selection pattern from store
		else if (DICE_bank->trackSelectionStored != 0) {
			// Select it into the page
			DICE_bank->trackSelection = DICE_bank->trackSelectionStored;
		}

		// D O U B L E - C L I C K
		if (	(DOUBLE_CLICK_TARGET == keyNdx)
			&& 	(DOUBLE_CLICK_TIMER > DOUBLE_CLICK_ALARM_SENSITIVITY)
			){

			// This is a double click victim - Select all tracks - NEMO has 8
			DICE_bank->trackSelection = 0xFF;
			DICE_bank->trackSelectionStored = DICE_bank->trackSelection;
		}
	}

	//
	// MATRIX
	//
	// Make sure we are within the real matrix range
	if (	( 	( keyNdx >= 11 ) && ( keyNdx <= 185 )
			&& 	( ( (keyNdx-10) % 11 ) != 0 ) ) ) {


		// /////
		// ROW I - value entry, depending on selected attribute (clock divider / multiplier selector & flow factor control selector)
		//
		if ( (( keyNdx - NEMO_ROW_I ) % 11) == 0) {

			// Row of target track and column of target step
			col = (( keyNdx - NEMO_ROW_I ) / 11) - 1 ;

			if( Dice_is_edit_tempo( target_dice ) ) {
				unsigned char j;
				unsigned char dice_selected_clock_type = Dice_get_selected_clock_type( target_dice );
				// Double click selects the clock value as divisor
				if (	( DOUBLE_CLICK_TARGET == keyNdx )
					&& 	( DOUBLE_CLICK_TIMER > DOUBLE_CLICK_ALARM_SENSITIVITY )
					){

					// SINGLE CLICK: Set the tempo multiplier of the dice clock
					// Single click selects the clock value as multiplier
					switch( col ){

						// Multiplier offset
						case 0: case 1: case 2: case 3: case 4: case 5: case 6: //case 7: case 14: case 15:
							j = col + 1;

							// If the track is not playing, it is safe to adjust the multiplier immediately
							if ( G_run_bit == OFF ){
								// Update the multipler and clear the buffer
								Dice_set_TEMPOMUL( target_dice, j, dice_selected_clock_type );

								// Update the divisor
								Dice_set_TEMPOMUL_SKIP( target_dice, 0, dice_selected_clock_type );
							} // current Track is not playing
							// Track is playing so queue until the next global TTC1
							else{
								dice_synced_attr = NEMO_ATTR_G_master_tempoMUL;

								// Set the tempo multiplier of DICE.
								dice_synced_value = j;
							}
							break;
					}

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

					// DOUBLE CLICK CODE
					// Determine the DICE clock mult / div value..
					switch( col ){

						case 0:		break;
						case 1: case 2: case 3: case 4: case 5: case 6: //case 7: case 15:
							j = col;

							// If the track is not playing, it is safe to adjust the multiplier immediately
							if ( G_run_bit == OFF ){
								// Update the multipler and clear the buffer.
								Dice_set_TEMPOMUL( target_dice, 1, dice_selected_clock_type);

								// Update the divisor and clear the buffer.
								Dice_set_TEMPOMUL_SKIP( target_dice, j, dice_selected_clock_type );
							} // current Track is not playing
							else{
								dice_synced_attr = NEMO_ATTR_G_master_tempoMUL_SKIP;

								// Set the tempo multiplier of DICE. (pack byte)
								dice_synced_value = j;
							}
							break;
					}

				} // Single click, CLOCK
			}else{
				// Dice ATTR Factors
				unsigned char dice_factor = Dice_get_factor( target_dice, target_dice->attr_STATUS);
				// DOUBLE CLICK SCENARIO
				if ((DOUBLE_CLICK_TARGET == keyNdx)
						&& (DOUBLE_CLICK_TIMER > DOUBLE_CLICK_ALARM_SENSITIVITY)) {

					// Double click code
					// Shift pos / neg.
					Dice_set_factor( target_dice, target_dice->attr_STATUS, dice_factor < 17 ? dice_factor + 16 : dice_factor - 16 );
				} // end of double click scenario


				// SINGLE CLICK SCENARIO
				else if (DOUBLE_CLICK_TARGET == 0) {

						DOUBLE_CLICK_TARGET = keyNdx;
						DOUBLE_CLICK_TIMER = ON;
						// Start the Double click Alarm
						cyg_alarm_initialize(
								doubleClickAlarm_hdl,
								cyg_current_time() + DOUBLE_CLICK_ALARM_TIME,
								DOUBLE_CLICK_ALARM_TIME );

					// Single click code
					// Set factor...
				}

				// Set the new value of the PIT_factor
				Dice_set_factor( target_dice, target_dice->attr_STATUS, ( ( max( 1, dice_factor ) - 1 ) & 0x30 ) + col + 1 );
			}

		} // ROW I Key

		// /////
		// ROW II - CLOCK - flow and loop clock control & LEN value entry - selects dice subdivision
		//
		if ( (( keyNdx - NEMO_ROW_II ) % 11) == 0) {
			// Row of target track and column of target step
			col = (( keyNdx - NEMO_ROW_II ) / 11) - 1 ;

			// Select the track attribute
			switch( col ){

				// VEL PIT LEN
				case 0: 	target_dice->attr_STATUS = NEMO_ATTR_PITCH;			break;
				case 1: 	target_dice->attr_STATUS = NEMO_ATTR_VELOCITY;		break;
				case 2: 	target_dice->attr_STATUS = NEMO_ATTR_LENGTH;		break;
				case 3: 	target_dice->attr_STATUS = NEMO_ATTR_START;			break;
				case 5: 	target_dice->attr_STATUS = NEMO_ATTR_AMOUNT;		break;
				case 6: 	target_dice->attr_STATUS = NEMO_ATTR_GROOVE;		break;
				case 7: 	target_dice->attr_STATUS = NEMO_ATTR_MIDICC;		break;
			}

			if( col > 8 && col < 14 ){
				if ( G_run_bit == ON )
				{
					dice_synced_attr 	= ATTR_LENGTH;
					dice_synced_value 	= col;

					// D O U B L E - C L I C K  C O N S T R U C T
					// DOUBLE CLICK SCENARIO
					if ((DOUBLE_CLICK_TARGET == keyNdx)
							&& (DOUBLE_CLICK_TIMER > DOUBLE_CLICK_ALARM_SENSITIVITY)) {

						// Double click code
						SET_BIT( DICE_bank->editorMode, DICE_ALIGN );

					} // end of double click scenario


					// SINGLE CLICK SCENARIO
					else if (DOUBLE_CLICK_TARGET == 0) {

							DOUBLE_CLICK_TARGET = keyNdx;
							DOUBLE_CLICK_TIMER = ON;
							// Start the Double click Alarm
							cyg_alarm_initialize(
									doubleClickAlarm_hdl,
									cyg_current_time() + DOUBLE_CLICK_ALARM_TIME,
									DOUBLE_CLICK_ALARM_TIME );

					} // Single click scenario
				}else{
					target_dice->attr_LEN = col - 9;
					target_dice->attr_TEMPOMUL = 0;

					offset = max( 1, 1 << ( 4 - (  (target_dice->attr_LEN - 1 ) + 1 ) ) );
					target_dice->attr_STA = ROUNDUP( target_dice->attr_STA - ( offset - 1 ), offset );
				}
			}else if( col >= 14 ){
				// Tempo multiplier ( 15 = flow, 16 = grid ).
				Dice_set_MISC( target_dice, DICE_TRACK_CLOCK | DICE_GLOBAL_CLOCK, 16 - col );
			}
		}

		// /////
		// ROW III - STA value entry - selects loop position
		//
		if ( (( keyNdx - NEMO_ROW_III ) % 11 ) == 0 ){
			// Row of target track and column of target step
			col = (( keyNdx - NEMO_ROW_III ) / 11) - 1 ;

			if ( G_run_bit == ON )
			{
				dice_synced_attr 	= ATTR_START;
				dice_synced_value 	= col;

				// D O U B L E - C L I C K  C O N S T R U C T
				// DOUBLE CLICK SCENARIO
				if ((DOUBLE_CLICK_TARGET == keyNdx)
						&& (DOUBLE_CLICK_TIMER > DOUBLE_CLICK_ALARM_SENSITIVITY)) {

					SET_BIT( DICE_bank->editorMode, DICE_ALIGN );
				} // end of double click scenario


				// SINGLE CLICK SCENARIO
				else if (DOUBLE_CLICK_TARGET == 0) {

						DOUBLE_CLICK_TARGET = keyNdx;
						DOUBLE_CLICK_TIMER = ON;
						// Start the Double click Alarm
						cyg_alarm_initialize(
								doubleClickAlarm_hdl,
								cyg_current_time() + DOUBLE_CLICK_ALARM_TIME,
								DOUBLE_CLICK_ALARM_TIME );

				} // Single click scenario
			}
			else
			{
				offset = max( 1, 1 << ( 4 - (  (target_dice->attr_LEN - 1 ) + 1) ) );
				target_dice->attr_STA = ROUNDUP( col - ( offset - 1 ), offset );
			}
		}

		// //////
		// ROW IV allows flow selection
		//

		// Allows step toggle edit for the dice
		//
		if ( (( keyNdx - NEMO_ROW_IV ) % 11) == 0) {

			col = (( keyNdx - NEMO_ROW_IV ) / 11) - 1 ;

			if ( col < 10 ){
				// Params: shapeNdx, target_attribute, page, row;
				assign_dice_flow_shape( col, target_dice->attr_STATUS, DICE_bank, SEL_DICE_BANK );
				target_dice->flow_shape[ target_dice->attr_STATUS ] = col;

				if( SEL_DICE_BANK == 0 )
				{
					// Reset factor.
					Dice_set_factor(target_dice, target_dice->attr_STATUS, 0);
				}
			}
		}

		// Assignment of the MIX attribute
		if ( is_pressed_key( KEY_MIX_MASTER ) ){

			// Select the dice attribute
			switch( col ){

				// VEL PIT LEN STA
				case 0: 	target_dice->flow_shape[NEMO_ATTR_PITCH]	= 0;		break;
				case 1: 	target_dice->flow_shape[NEMO_ATTR_VELOCITY]	= 0;		break;
				case 2: 	target_dice->flow_shape[NEMO_ATTR_LENGTH]	= 0;		break;
				case 3:		target_dice->flow_shape[NEMO_ATTR_START]	= 0;		break;
				// AMT GRV MCC
				case 5: 	target_dice->flow_shape[NEMO_ATTR_AMOUNT]	= 0; 		break;
				case 6: 	target_dice->flow_shape[NEMO_ATTR_GROOVE]	= 0;		break;
				case 7: 	target_dice->flow_shape[NEMO_ATTR_MIDICC]	= 0;		break;

			}

			return;
		}

	} // Matrix key pressed


	//
	// MUTATORS
	//
	if (	(keyNdx >= 187)
		&& 	(keyNdx <= 195)
		){

		// New school: operate on the GRID mutepattern
		GRID_mutepattern ^= ( 1 << (keyNdx-187) );

	} // key is a mutator

	//
	// MUTE MASTER
	//
	if (keyNdx == KEY_MUTE_MASTER) {
		// Clear trackSelection if exists
		if (GRID_mutepattern != 0) {

			// Store the track selection
			DICE_bank->trackMutepatternStored = GRID_mutepattern;

			// Clear the track selection
			GRID_mutepattern = 0;
		}

		// Fill the selection pattern from store
		else if (DICE_bank->trackMutepatternStored != 0) {
			// Select it into the page
			GRID_mutepattern = DICE_bank->trackMutepatternStored;
		}

		// D O U B L E - C L I C K
		if (	(DOUBLE_CLICK_TARGET == keyNdx)
			&& 	(DOUBLE_CLICK_TIMER > DOUBLE_CLICK_ALARM_SENSITIVITY)
			){

			// This is a double click victim - Select all tracks - NEMO has 8
			GRID_mutepattern = 0xFF;
			DICE_bank->trackMutepatternStored = GRID_mutepattern;
		}
	}

	// Switch zoom level to zoomSCALE
	if ( keyNdx == KEY_SCALE_SEL ){

		G_last_zoom_level = zoomDICE;
		G_zoom_level = zoomSCALE;
		GRID_assistant_page->scaleStatus = SCALE_SEL;
		GRID_assistant_page->force_to_scale = ON;
		return;
	}



	//
	// MIX TARGET
	//
	if (
			( (keyNdx == KEY_MIXTGT_USR1) )
		||	( (keyNdx == KEY_MIXTGT_USR2) )
		||	( (keyNdx == KEY_MIXTGT_USR3) )
		||	( (keyNdx == KEY_MIXTGT_USR4) )
		){

		// D O U B L E - C L I C K  C O N S T R U C T
		// DOUBLE CLICK SCENARIO
		if ((DOUBLE_CLICK_TARGET == keyNdx)
				&& (DOUBLE_CLICK_TIMER > DOUBLE_CLICK_ALARM_SENSITIVITY)) {

			// Double click code
			// ...
			// Zoom into the clicked mix target

			// Save the cursor so we can return to its position
			G_zoom_level	= zoomMIXMAP;
			G_last_cursor 	= GRID_CURSOR;
			GRID_CURSOR 	= GRID_assistant_page->pageNdx;

		} // end of double click scenario


		// SINGLE CLICK SCENARIO
		else if (DOUBLE_CLICK_TARGET == 0) {

				DOUBLE_CLICK_TARGET = keyNdx;
				DOUBLE_CLICK_TIMER = ON;
				// Start the Double click Alarm
				cyg_alarm_initialize(
						doubleClickAlarm_hdl,
						cyg_current_time() + DOUBLE_CLICK_ALARM_TIME,
						DOUBLE_CLICK_ALARM_TIME );

			// Single click code
			// ...
			// Just simply switch to the chosen mix target.
			switch( keyNdx ){

				case KEY_MIXTGT_VOL: 	GRID_assistant_page->mixTarget = MIXTGT_USR1; 	break;
				case KEY_MIXTGT_PAN: 	GRID_assistant_page->mixTarget = MIXTGT_USR2; 	break;
				case KEY_MIXTGT_MOD: 	GRID_assistant_page->mixTarget = MIXTGT_USR3; 	break;
				case KEY_MIXTGT_EXP: 	GRID_assistant_page->mixTarget = MIXTGT_USR4; 	break;
			}
		} // Single click scenario
	} // MIX TARGET KEY pressed

	// EDIT MASTER KEY
	if (	( keyNdx == KEY_EDIT_MASTER ) ){
		// Toggles the quantise mode for DICE
		TOGGLE_BIT( DICE_bank->editorMode, DICE_QUANT );
	}

	// ALIGN KEY
	// Set locators of playing pages to the GLOBAL LOCATOR
	if ( keyNdx == KEY_ALIGN ){
		dice_synced_attr 	= ATTR_MISC;
		dice_synced_value 	= KEY_ALIGN;
	}

	//
	// TRANSPORT functions - universally valid
	//


	switch( keyNdx ){

		case KEY_STOP:
			if ( G_clock_source != EXT ){
				sequencer_command_STOP();
			}
			break;

		case KEY_PAUSE:
			if ( is_pressed_key( KEY_STOP ) ){
				switch( G_clock_source ){
					case OFF:	G_clock_source = EXT;		break;
					case INT:	G_clock_source = EXT;		break;
					case EXT:	G_clock_source = OFF;		break;
				}
				G_master_blinker = ON;
			}

			// Transport command
			else{
				if ( G_clock_source != EXT ){
					sequencer_command_PAUSE();
				}
			}
			break;

		case KEY_PLAY1:
		case KEY_PLAY2:
		case KEY_PLAY4:

			// Set the MIDI clock source
			if ( is_pressed_key( KEY_STOP ) ){
				switch( G_clock_source ){
					case OFF:	G_clock_source = INT;		break;
					case INT:	G_clock_source = OFF;		break;
					case EXT:	G_clock_source = INT;		break;
				}
				G_master_blinker = ON;
			}

			// Transport command
			else{
				if ( G_clock_source != EXT ){
					sequencer_command_PLAY();
				}
			}
			break;
	}

	if ( G_run_bit == OFF && keyNdx == KEY_PROGRAM ) {
		if( is_pressed_key( KEY_MCC_STREAM1 ) || is_pressed_key( KEY_MCC_STREAM2 ) ||
			is_pressed_key( KEY_MCC_STREAM3 ) || is_pressed_key( KEY_MCC_STREAM4 ) ) {
			// Write dice banks.
			Flash_write_page( DICE_PAGE );
			return;
		}
	}


	//
	// DICE bank selector buttons.
	//
	if( keyNdx == KEY_MCC_STREAM1 ||
		keyNdx == KEY_MCC_STREAM2 ||
		keyNdx == KEY_MCC_STREAM3 ||
		keyNdx == KEY_MCC_STREAM4 ){

		switch( keyNdx ){
			case KEY_MCC_STREAM1: i = 0; break;
			case KEY_MCC_STREAM2: i = 1; break;
			case KEY_MCC_STREAM3: i = 2; break;
			case KEY_MCC_STREAM4: i = 3; break;
		}

		if( is_pressed_key( KEY_MCC_STREAM1 ) ||
			is_pressed_key( KEY_MCC_STREAM2 ) ||
			is_pressed_key( KEY_MCC_STREAM3 ) ||
			is_pressed_key( KEY_MCC_STREAM4 )){

			Dice_copy(target_dice, DICE_bank->Track[i]);

		} else{
			// Switch dice bank.
			if ( G_run_bit == ON ){
				dice_synced_attr = ATTR_DICE_BANK;
				dice_synced_value = i;
			}
			else{
				SEL_DICE_BANK = i;
			}
		}
	}

	// GRID mode switch
	if (keyNdx == KEY_ZOOM_GRID) {

		G_zoom_level = zoomGRID;
	}
}



