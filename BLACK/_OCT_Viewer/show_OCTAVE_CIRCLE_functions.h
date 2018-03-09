

// Show the scale selection of th given page
void show_OCTAVE_CIRCLE_scale_selection( Pagestruct* target_page ){

	unsigned int 	i = 0,
					j = 0;

	// Determine which scale to show
	if ( target_page->scaleStatus == OFF ){
		j = target_page->current_scale;
	}
	else{
		j = target_page->scaleNotes[0];
	}
 

	// Show the SCL_align status
	if ( target_page->SCL_align == OFF ){
		
		MIR_write_dot( LED_SCALE_CAD, MIR_GREEN );
	}
	else{

		MIR_write_dot( LED_SCALE_CAD, MIR_RED );
		// MIR_write_dot( LED_SCALE_CAD, MIR_BLINK );
	}




	// ON fields showing first
	for (i=0; i<12; i++) {
		
		// If the note is selected as ON
		if ( j & (1 << (11-i)) ) {

			switch (i) {
				case 0:
					MIR_write_dot( LED_NOTE_C, 		MIR_GREEN );
					break;
				case 1:
					MIR_write_dot( LED_NOTE_Cis, 	MIR_GREEN );
					break;
				case 2:
					MIR_write_dot( LED_NOTE_D,	 	MIR_GREEN );
					break;
				case 3:
					MIR_write_dot( LED_NOTE_Dis, 	MIR_GREEN );
					break;
				case 4:
					MIR_write_dot( LED_NOTE_E,	 	MIR_GREEN );
					break;
				case 5:
					MIR_write_dot( LED_NOTE_F,	 	MIR_GREEN );
					break;
				case 6:
					MIR_write_dot( LED_NOTE_Fis, 	MIR_GREEN );
					break;
				case 7:
					MIR_write_dot( LED_NOTE_G,	 	MIR_GREEN );
					break;
				case 8:
					MIR_write_dot( LED_NOTE_Gis, 	MIR_GREEN );
					break;
				case 9:
					MIR_write_dot( LED_NOTE_A, 		MIR_GREEN );
					break;
				case 10:
					MIR_write_dot( LED_NOTE_Ais, 	MIR_GREEN );
					break;
				case 11:
					MIR_write_dot( LED_NOTE_B,	 	MIR_GREEN );
					break;									
			}
		}
	} // ON field iterator

	// UP fields showing first. This shows the scale lead
	for (i=0; i<12; i++) {
		
		// If the note is selected as UP
		if ( target_page->scaleLead[0] & (1 << (11-i) ) ) {							
			
			switch (i) {
				case 0:
					MIR_write_dot( LED_NOTE_C, 		MIR_RED );
					break;
				case 1:
					MIR_write_dot( LED_NOTE_Cis, 	MIR_RED );
					break;
				case 2:
					MIR_write_dot( LED_NOTE_D,	 	MIR_RED );
					break;
				case 3:
					MIR_write_dot( LED_NOTE_Dis, 	MIR_RED );
					break;
				case 4:
					MIR_write_dot( LED_NOTE_E,	 	MIR_RED );
					break;
				case 5:
					MIR_write_dot( LED_NOTE_F,	 	MIR_RED );
					break;
				case 6:
					MIR_write_dot( LED_NOTE_Fis, 	MIR_RED );
					break;
				case 7:
					MIR_write_dot( LED_NOTE_G,	 	MIR_RED );
					break;
				case 8:
					MIR_write_dot( LED_NOTE_Gis, 	MIR_RED );
					break;
				case 9:
					MIR_write_dot( LED_NOTE_A, 		MIR_RED );
					break;
				case 10:
					MIR_write_dot( LED_NOTE_Ais, 	MIR_RED );
					break;
				case 11:
					MIR_write_dot( LED_NOTE_B,	 	MIR_RED );
					break;									
			}
		}
	} // UP field for iterator

}



