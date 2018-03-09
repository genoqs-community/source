

// Wires the values from lead and scale pitch offsets into track pitch attributes
void wire_scale_data( Pagestruct* target_page ){
	
	unsigned char i = 0;

	// Get the current picture of the actual scale in page
//	target_page->scaleNotes[0] = 0;
//	extract_scale_and_lead( target_page );

	// Sometimes the lead is off the scale, align it to the lowest note in scale
	if ((target_page->scaleLead[0] & target_page->scaleNotes[0]) == 0){

		// Iterate through the scale
		for (i=0; i < 12; i++) {

			// First note in scale found
			if ( 1<<(11-i) & target_page->scaleNotes[0] ) {

				// Build the scaleLead
				target_page->scaleLead[0] = 1 << (11-i);

				// Exit the for loop
				i = 12;
			}
		}
	}

	// Normalize the track pitch offsets and..
	// .. transform the pitch info into track pitches.
	for ( i=0; i < MATRIX_NROF_ROWS; i++ ){

		// Move the lead pitch offsets to the scale pitch offsets
		target_page->Track[i]->scale_pitch_offset += 
				target_page->Track[i]->lead_pitch_offset;

		// Empty the lead pitch offset 
		target_page->Track[i]->lead_pitch_offset = 0;

		// THIS IS CORRECT BUT HAS MUCH GREATER IMPLICATIONS. NEEDS BIG REVISION.
		// Copy this data into the track pitch fields
		target_page->Track[i]->attribute[ATTR_PITCH] 
			=	36 + target_page->Track[i]->scale_pitch_offset;

		// Empty the scale_pitch_offset;
		target_page->Track[i]->scale_pitch_offset = 0;
	}

}


//
// Key execution snippet for the Octave circle in Page mode. May be reused as necessary
// Includes the creation/selection of scales
//
void key_ScaleSelector_functions( unsigned int keyNdx, Pagestruct* target_page ){

	unsigned int 	i = 0, 
					k = 0;

	// RETURN / ESCAPE: clear the scale and lead offsets and 
	// exit the scale mode. Track pitch attributes are not affected by this.
	if (keyNdx == KEY_RETURN) {

		// Iterate through all tracks
		for (i=0; i<MATRIX_NROF_ROWS; i++){

			// Clear their scale and lead offsets
			target_page->Track[i]->scale_pitch_offset = 0;
			target_page->Track[i]->lead_pitch_offset = 0;
		}

		// Clear everything thats left in the page and exit the scale mode.
		target_page->scaleStatus = OFF;

		target_page->force_to_scale = OFF;
	}


	// 
	// Control the Scale functionality in the page.
	//

	// SCALE SELECTION control
	
	if ( keyNdx == KEY_SCALE_SEL ) {

		// D O U B L E - C L I C K  C O N S T R U C T
		// DOUBLE CLICK SCENARIO
		if ((DOUBLE_CLICK_TARGET == keyNdx) 
				&& (DOUBLE_CLICK_TIMER > DOUBLE_CLICK_ALARM_SENSITIVITY)) {
	
			// Double click code
			// ...
			// Enter the Scale selection mode

			switch( target_page->scaleStatus ){
	
				case SCALE_MOD:
				case OFF:			

					// Switch to the scale selection mode.
					target_page->scaleStatus = SCALE_SEL;

					// Make sure force to scale is ON 
					target_page->force_to_scale = ON;

					// This fixes the problem of not being able to start with the CHR scale as default
					target_page->scaleNotes[0] = target_page->current_scale;
					break;
			} // switch( target_page->scaleStatus )	
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
			
			// When not in scale mode, just toggle force to scale
			if ( target_page->scaleStatus == OFF ){

				// Turn on force to scale
				target_page->force_to_scale = ON;

				// Activate the MOD scale mode
				target_page->scaleStatus = SCALE_MOD;
			}
			else{
				
				// RETURN / ESCAPE: clear the scale and lead offsets and 
				// exit the scale mode. Track pitch attributes are not affected by this.

				// Iterate through all tracks
				for (i=0; i<MATRIX_NROF_ROWS; i++){
		
					// Clear their scale and lead offsets
					target_page->Track[i]->scale_pitch_offset = 0;
				}
		
				// Get out of the scale selection mode
				target_page->scaleStatus = OFF;

				// Lock the current scale
				target_page->force_to_scale = OFF;
			}
		}

		// No further execution required
		return;

	} // key is SCALE_SEL



	// SCALE MODE 
	
	// Basically dos
	if (keyNdx == KEY_SCALE_MOD) {

//		// CADENCE recording 
//		if (target_page->cadenceStatus == RECORDING){
//			target_page->cadence->key[ target_page->cadence->locator ] = keyNdx;
//		}

		// Do not react when force to scale is on
		if ( target_page->scaleStatus == SCALE_SEL ){

			target_page->scaleStatus = SCALE_MOD;
			// return;
		}
	} // keyNdx == SCALE_MOD



	// 
	// CADENCE Status selector / Toggler
	// 
	if (keyNdx == KEY_SCALE_CAD) {
	
	// Disabled for the time being!!! - the return ignores the rest of the code
	return;

		switch( target_page->scaleStatus ){
			
			case OFF:
				// Turn on the cadence edit mode
				target_page->scaleStatus = SCALE_CAD;
				target_page->keyMode = KMOD_CADENCE_EDIT;
				break;
				

			case SCALE_SEL:	
				// No break, fallthrough intended, as these are similar from this p.o.v.

			case SCALE_MOD:			
				// Select one of OFF (green) / PLAYING (orange) / RECORDING (red)
				//
				// D O U B L E - C L I C K
				// DOUBLE CLICK SCENARIO
				if ((DOUBLE_CLICK_TARGET == keyNdx) 
						&& (DOUBLE_CLICK_TIMER > DOUBLE_CLICK_ALARM_SENSITIVITY)) {
	
					// Double click code
					// ...

					// Set the cadence to RECORDING
//					target_page->cadenceStatus = RECORDING;
							
				}		
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
					// ... empty
					
				}
				break;
			

			case SCALE_CAD:
				// Turn off the cadence edit mode
				target_page->scaleStatus = OFF;
				target_page->keyMode = KMOD_PAGE;
				break;
		}
	} // keyNdx == KEY_SCALE_CAD





	//
	// OCTAVE CIRCLE: Key is in the octave circle
	//

	// The mode of operation is as follows:
	// Keys have two states: OFF, ON, and if ON, a note can also be LEAD.
	// OFF is obvious. ON means step is selected in the scale, 
	// LEAD means this is the lead tone of the scale - i.e. the counting reference for intervals.
	// First note selected or double click selects the lead. Others toggle the notes.
	// However, restrictions do apply, such as to make for a meaningful scale.

	if ( 	(keyNdx == KEY_NOTE_C	) ||
		 	// (keyNdx == KEY_NOTE_Cup	) ||
			(keyNdx == KEY_NOTE_Cis	) ||
			(keyNdx == KEY_NOTE_D	) ||
			(keyNdx == KEY_NOTE_Dis	) ||
			(keyNdx == KEY_NOTE_E	) ||
			(keyNdx == KEY_NOTE_F	) ||
			(keyNdx == KEY_NOTE_Fis	) ||
			(keyNdx == KEY_NOTE_G	) ||
			(keyNdx == KEY_NOTE_Gis	) ||
			(keyNdx == KEY_NOTE_A	) ||
			(keyNdx == KEY_NOTE_Ais	) ||
			(keyNdx == KEY_NOTE_B	) 		
		){
		

		// Only apply the following chunk of code if Scale mode ON. 
		// This has a lock function.
		if (target_page->scaleStatus == OFF) {

			// key_ScaleSelector_mini_functions( keyNdx, target_page );
			// return;
		}

	
		// CADENCE recording the NOTE KEY presses
		// if (target_page->cadenceStatus == RECORDING){
		//	target_page->cadence->key[ target_page->cadence->locator ] = keyNdx;
		// }

		// Transform key Index to a note index;
		k = keyNdx_to_noteNdx( keyNdx );

// d_iag_printf( "key_SclSel_funs key:%d, sclStat:%d\n", k, target_page->scaleStatus );

		switch( target_page->scaleStatus ){
			
			//
			// SEL MODE
			//
			case SCALE_SEL:
	
				// Toggle the notes of the scale
				switch( how_selected_in_scale( target_page, k ) ){
	
					// Note selected is LEAD
					case LEAD:
					
						// DECIDED NOT DO DO ANYTHING TO THE LEAD!! 
						// DON'T ALLOW TURNING THE LEAD OFF
						break;					
										
					// Note selected is OFF or ON
					case OFF:
						// NO BREAK - Fallthrough intended and needed
					case ON:

						// Add or remove note in scale ^1 corresponds to ON/OFF
						select_in_scale( target_page, k, how_selected_in_scale( target_page, k )^1 );
						
						// If the SCALE is set to align, proceed as follows:						
						if ( target_page->SCL_align == ON ){

							// Export the scale degrees
							export_scale_degrees( target_page );
							
							// Refetch the old lead
							target_page->scaleLead[0] = target_page->scaleLead_old;
							
							// Export the lead
							export_scale_lead( target_page );

							// Wire the changes into the track pitches
							wire_scale_data( target_page );
						}
							
						break;
				
				} // switch( how_selected_in_scale( k )
				break;
	
	
	
			//
			// MOD MODE
			//
			case SCALE_MOD:
				// Change the LEAD - used for scale bound trans-position
				
				// The book says to Select clicked note as the lead, 
				// ..only if the note is in the scale.
				if ( how_selected_in_scale( target_page, k ) != ON  ) {
					
					// May be interesting to Experiment a bit with this..
					// ..but not intuitive and not musical
					break;
				}

				// Store current lead so we know how much to shift pitches
				target_page->scaleLead_old = target_page->scaleLead[0];
	
				// d_iag_printf( "..MOD key:oldL=%d newL=%d\n", 
				// 		target_page->scaleLead_old, target_page->scaleLead );
				// my_print_bits32( target_page->scaleLead_old );
				// my_print_bits32( target_page->scaleLead );
	
				// Select the new scale lead
				select_in_scale( target_page, k, LEAD );
	
				//	d_iag_printf( "..executing key: old\n" ) ;
				//	my_print_bits32( target_page->scaleLead );
				//	my_print_bits32( target_page->scaleLead_old );
	
	
				// If the SCALE is set to align, proceed as follows:						
				if ( target_page->SCL_align == ON ){

					// Export the scale degrees
					export_scale_degrees( target_page );
					
					// Refetch the old lead
//					target_page->scaleLead[0] = target_page->scaleLead_old;
					
					// Export the lead
					export_scale_lead( target_page );

					// Wire the changes into the track pitches
					wire_scale_data( target_page );
				}

				// Export the new lead to the track pitch offsets
				// export_scale_lead( target_page );
	
				// d_iag_printf( "*LEAD modified: scale/lead/_old\n" );
				// my_print_bits32( target_page->scaleNotes[0] );
				// my_print_bits32( target_page->scaleLead[0] );
				// my_print_bits32( target_page->scaleLead_old );

				break;
	

			case OFF:
				// Doesn't do anything while in page mode.
				break;
	
		} // switch( scale status )


		// Simply update the current scale with what is selected in the octave circle
		target_page->current_scale = target_page->scaleNotes[0];


	} // KEY is in the Octave circle



	// 
	// SCALE TYPE BUTTONS. Apply scale interval structures to tracks
	// 
	if ( 	(keyNdx == KEY_SCALE_MYSEL) 
		||	(keyNdx == KEY_SCALE_PEN)
		||	(keyNdx == KEY_SCALE_WHL)	
		||	(keyNdx == KEY_SCALE_MAJ)	
		||	(keyNdx == KEY_SCALE_MIN)	
		||	(keyNdx == KEY_SCALE_DIM)	
		||	(keyNdx == KEY_SCALE_CHR)		
		){
	
		// // CADENCE recording 
		// if (	(target_page->cadenceStatus == RECORDING) 
		//	||	(target_page->keyMode == KMOD_CADENCE_EDIT)	){
		//	target_page->cadence->key[ target_page->cadence->locator ] = keyNdx;
		// }

		// Clear the scale and lead offsets in the tracks
		for (i=0; i<MATRIX_NROF_ROWS; i++){
			target_page->Track[i]->scale_pitch_offset = 0;
//			target_page->Track[i]->lead_pitch_offset = 0;
		}

		// Act according to selected scale
		switch( keyNdx ){
			
			
			case KEY_SCALE_MYSEL:
				// Recall the last non-standard scale
				target_page->scaleNotes[0] = target_page->my_scale_signature;
				break;

			case KEY_SCALE_PEN:
				target_page->my_scale_signature = target_page->scaleNotes[0];
				target_page->scaleNotes[0] = SCALE_SIG_PEN;
				break;
				
			case KEY_SCALE_WHL:
				target_page->my_scale_signature = target_page->scaleNotes[0];
				target_page->scaleNotes[0] = SCALE_SIG_WHL;
				break;
				
			case KEY_SCALE_MAJ:
				target_page->my_scale_signature = target_page->scaleNotes[0];
				target_page->scaleNotes[0] = SCALE_SIG_MAJ;		
				break;
				
			case KEY_SCALE_MIN:
				target_page->my_scale_signature = target_page->scaleNotes[0];
				target_page->scaleNotes[0] = SCALE_SIG_MIN;
				break;

			case KEY_SCALE_DIM:
				target_page->my_scale_signature = target_page->scaleNotes[0];
				target_page->scaleNotes[0] = SCALE_SIG_DIM;		
				break;

			case KEY_SCALE_CHR:
				target_page->my_scale_signature = target_page->scaleNotes[0];
				target_page->scaleNotes[0] = SCALE_SIG_CHR;
				break;

		} // switch( keyNdx )
		
		//		d_iag_printf( "SCL SEL: scale Notes shift:%d:\n", 11 - my_bit2ndx(target_page->scaleLead) );
		//		my_print_bits32( target_page->scaleNotes );
		
		// Shift the signature according to the lead
		target_page->scaleNotes[0] = 
			my_shift_bitpattern( 	target_page->scaleNotes[0], 
									12, 
									INC, 
									(11 - my_bit2ndx(target_page->scaleLead[0] ) ) 
								);

		//	d_iag_printf( "SCL SEL: shifted/lead:%d/defaultPEN/adjusted:%d/\n", 
		//									my_bit2ndx( target_page->scaleLead )+1 );
		//	my_print_bits32( target_page->scaleNotes );
		//	my_print_bits32( target_page->scaleLead );
		//	my_print_bits32( SCALE_SIG_PEN );	
		//	my_print_bits32( my_shift_bitpattern( 	target_page->scaleNotes, 12, INC,
		//												my_bit2ndx( target_page->scaleLead )+1
		//												)
		//					);


		// Export the changes to the tracks
		target_page->scaleLead_old = target_page->scaleLead[0];
		target_page->scaleNotes_old = target_page->scaleNotes[0];

		// Simply update the current scale with what is selected in the octave circle
		target_page->current_scale = target_page->scaleNotes[0];
		
		// If the SCALE is set to align, proceed as follows:						
		if ( target_page->SCL_align == ON ){

			// Export the scale degrees
			export_scale_degrees( target_page );
			
			// Refetch the old lead
//			target_page->scaleLead[0] = target_page->scaleLead_old;
			
			// Export the lead
			export_scale_lead( target_page );

			// Wire the changes into the track pitches
			wire_scale_data( target_page );
		}

	} // Key is one of the preset scales
}


