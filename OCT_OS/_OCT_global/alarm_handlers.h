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







// Timeout alarm handler - used for response behavior of encoders
void alarm_handler( cyg_handle_t alarm_handle, cyg_addrword_t data ){

	// Reset the indicators
	MIX_TIMER 	= OFF;
	EDIT_TIMER 	= OFF;
	TEMPO_TIMER	= OFF;
	PHRASE_TIMER = OFF;

	PhraseMultiTweakReset();

	// d_iag_printf( "Alarm ringing..%d\n", data );

	// Disable the alarm - it will be enabled as needed somewhere else
	cyg_alarm_disable( alarm_hdl );
}


// Double click alarm handler - used to handle double click behavior
void doubleClickAlarm_handler( cyg_handle_t alarm_handle, cyg_addrword_t data ){

	// DOUBLE CLICK CONTROL
	if (DOUBLE_CLICK_TIMER != OFF) {
		// Increment the counter
		DOUBLE_CLICK_TIMER = (DOUBLE_CLICK_TIMER+1) % DOUBLE_CLICK_ALARM_RESOLUTION;

		// At the end of double click window
		if (DOUBLE_CLICK_TIMER == 0) {
			// Clear target
			DOUBLE_CLICK_TARGET = 0;
			// And disable the alarm
			cyg_alarm_disable( doubleClickAlarm_hdl );

			// Check if we need to send some PgmCh data from a page.
			// This is set in the GRID PLAY mode - Klaus Gstettner input
			if ( G_pgmch_pending_page != NULL ){
				// d_iag_printf( ">>> pgmch_pending page:%d\n", G_pgmch_pending_page->pageNdx );

				// Send the program changes
				send_program_changes( G_pgmch_pending_page );

				// Void the pgmch pending page so we don't repeat later
				G_pgmch_pending_page = NULL;
			}
			else{
				// d_iag_printf( "    pgmch_pending page:NULL\n" );
			}
		}
	}
}


// TV G_scanner alarm handler
void alarmTV_handler( cyg_handle_t alarmTV_handle, cyg_addrword_t data ){


	// This kicks off the G_scan keys activity in the readKeys_thread
	cyg_semaphore_post( &sem_readKeys );

}



// Rotary Quickturn alarm handler
void quickturnAlarm_handler( cyg_handle_t quickturnAlarm_handle, cyg_addrword_t data ){

	// Decrement the multiplier
	ROTARY_QUICKTURN_MULTIPLIER --;
	// d_iag_printf( "RQM:%d\n", ROTARY_QUICKTURN_MULTIPLIER );

	if ( ROTARY_QUICKTURN_MULTIPLIER == 1 ){
		// Disable the alarm
		cyg_alarm_disable( quickturnAlarm_hdl );
	}
}


// Turns off the light that indicates the MIDI overload
void MIDI1_overloadAlarm_handler( cyg_handle_t MIDI1_overloadAlarm_handle, cyg_addrword_t data ){

	// Show Green again..
	G_MIDI_load_monitor1 = MIR_GREEN;
}

// Turns off the light that indicates the MIDI overload
void MIDI2_overloadAlarm_handler( cyg_handle_t MIDI2_overloadAlarm_handle, cyg_addrword_t data ){

	// Show Green again..
	G_MIDI_load_monitor2 = MIR_GREEN;
}


// Turns off the light that indicates the MIDI overload
void CPU_overloadAlarm_handler( cyg_handle_t CPU_overloadAlarm_handle, cyg_addrword_t data ){

	// Show Green again..
	G_CPU_load_monitor = MIR_GREEN;
}


void page_refreshAlarm_handler( cyg_handle_t page_refreshAlarm_handle, cyg_addrword_t data ){

	// Refresh the page
	Page_requestRefresh();

	// Toggle the global blinker
	G_master_blinker ^= 1;
}
