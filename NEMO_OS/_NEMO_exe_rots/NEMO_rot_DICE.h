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


void rot_exec_DICE( 	unsigned char rotNdx,
						unsigned char direction ){

	extern void rot_exe_MIX( unsigned char rotNdx, unsigned char direction, Pagestruct* target_page );

	Trackstruct* target_dice = Dice_get();
	unsigned char * dice_factor_ptr = Dice_get_selected_factor_ptr( target_dice );

	// EDIT TIMER - Reset on every turn
	if (EDIT_TIMER == ON) {

		EDIT_TIMER = ON;
		// Setup alarm for the EDITOR TIMER
		cyg_alarm_initialize(	alarm_hdl,
								cyg_current_time() + TIMEOUT_VALUE,
								0 );
	}

	switch( rotNdx ){
		//
		// EDIT rotary: editing the hard-wired attributes
		//
		case 0: case ROT_PIT: case ROT_VEL: case ROT_LEN: case ROT_STA:

			// Modify the selected factor for the selected dice
			if( *dice_factor_ptr == 0  )
			{
				// Direction pivot
				*dice_factor_ptr = direction == INC ? 1 : 17;
			}
			else if( *dice_factor_ptr == 17  )
			{
				// Direction pivot
				*dice_factor_ptr = direction == INC ? 0 : 18;
			}
			else if( *dice_factor_ptr < 17 )
			{
				modify_parameter( 	dice_factor_ptr, 0, 16, direction, OFF, FIXED	);
			}
			else
			{
				modify_parameter( 	dice_factor_ptr, 0, 32, 3 - direction, OFF, FIXED	);
			}

			if ( rotNdx > 0 )
			{
				// Point the display to the changing attribute
				target_dice->attr_STATUS = rotNdx;
			}
		break;


		// MIX encoders
		case 11: case 12: case 13: case 14: case 15:
		case 16: case 17: case 18: case 19: case 20:

		// Send the corresponding CC data!
		rot_exe_MIX( rotNdx, direction, GRID_assistant_page );

		break;
	}
}


