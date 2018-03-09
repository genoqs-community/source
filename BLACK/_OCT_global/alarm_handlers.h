



// Timeout alarm handler - used for response behavior of rotarys
void alarm_handler( cyg_handle_t alarm_handle, cyg_addrword_t data ){

	// Returns to the default values
//	VIEWER_page->keyMode = KMOD_PAGE;
//	zoom_level = zoomPAGE;

	// Reset track selection - this is arguable still..
	//	VIEWER_page->trackSelection = 0;

	// Reset the indicators
	MIX_TIMER 	= OFF;
	EDIT_TIMER 	= OFF;

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
		}
	}
}


// TV scanner alarm handler
void alarmTV_handler( cyg_handle_t alarmTV_handle, cyg_addrword_t data ){


	// This kicks off the scan keys activity in the readKeys_thread
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
	MIDI_LOAD_MONITOR1 = MIR_GREEN;	
}

// Turns off the light that indicates the MIDI overload
void MIDI2_overloadAlarm_handler( cyg_handle_t MIDI2_overloadAlarm_handle, cyg_addrword_t data ){

	// Show Green again..
	MIDI_LOAD_MONITOR2 = MIR_GREEN;	
}


// Turns off the light that indicates the MIDI overload
void CPU_overloadAlarm_handler( cyg_handle_t CPU_overloadAlarm_handle, cyg_addrword_t data ){

	// Show Green again..
	CPU_LOAD_MONITOR = MIR_GREEN;
}


