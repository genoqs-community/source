


void key_clock_select( Pagestruct* target_page, unsigned short keyNdx ){

	// Do not listen to it when sequencer is running, DANGER!!
	// However allow it to operate when we are slave - to prevent lock..
	// ..in case the master suddenly disappears before sending a Stop command
	if ( 	( ( G_run_bit == OFF ) || ( G_clock_source == EXT ) )
		&& 	( target_page->trackSelection == 0)
		){

		// D O U B L E - C L I C K  C O N S T R U C T
		// DOUBLE CLICK SCENARIO
		if ((DOUBLE_CLICK_TARGET == keyNdx) 
				&& (DOUBLE_CLICK_TIMER > DOUBLE_CLICK_ALARM_SENSITIVITY)) {
	
			// Double click code
			// ...

			// Toggle the clock source
			switch ( G_clock_source )	{

				case INT:
					G_clock_source = OFF;
					break;

				case OFF:
					G_clock_source = INT;
					break;

				case EXT:
					G_clock_source = OFF;
					break;

			} // switch (G_clock_source)
			
			// Coming back from losing the master before having seen a Stop
			if ( G_run_bit == ON ){

				// Stop the sequencer properly
				sequencer_command_STOP();
				// Switch the clock source
				G_clock_source = OFF;
				// Make sure the CLOCK LED is lit
				G_master_blinker = ON;
			}
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
			
			// Toggle the clock source between INT and EXT
			switch ( G_clock_source )	{

				case OFF:
					G_clock_source = EXT;
					MIDICLOCK_PASSTHROUGH = 0;
					break;
				case INT:
					// Stop the sequencer properly, but fool it into not sending out ALL NOTES OFF
					#ifdef FEATURE_ENABLE_SONG_UPE
					prev_G_stop_bit = (G_run_bit == ON);
					#endif
					G_run_bit = ON;
					sequencer_command_STOP();
					// Switch the clock source
					G_clock_source = EXT;
					MIDICLOCK_PASSTHROUGH = 1;
					// Make sure the CLOCK LED is lit
					G_master_blinker = ON;
					break;

				case EXT:
					if ( !MIDICLOCK_PASSTHROUGH ){
						G_clock_source = OFF;
						break;
					}

					// Switch the clock source
					G_clock_source = INT;
					break;
					
			} // switch (G_clock_source)
		} //end of single click scenario

	} // G_run_bit == OFF
	
}



