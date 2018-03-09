

//_________________________________________________________________________________________
//
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


//_________________________________________________________________________________________
//
// This is the function that exports the selected scale pitches into the tracks
void program_scale_pitches( Pagestruct* target_page ){
	
	// Export the scale degrees
	export_scale_degrees( target_page );
	
	// Refetch the old lead
	target_page->scaleLead[0] = target_page->scaleLead_old;
	
	// Export the lead
	export_scale_lead( target_page );

	// Wire the changes into the track pitches
	wire_scale_data( target_page );
}



//_________________________________________________________________________________________
//
// Key execution snippet for the Octave circle in Page mode. May be reused as necessary
// Includes the creation/selection of scales
void key_ScaleSelector_functions( unsigned int keyNdx, Pagestruct* target_page ){

//	unsigned int 	i = 0;
//
//	// RETURN / ESCAPE: clear the scale and lead offsets and 
//	// exit the scale mode. Track pitch attributes are not affected by this.
//	if( keyNdx == KEY_RETURN ){
//
//		// Iterate through all tracks
//		for (i=0; i<MATRIX_NROF_ROWS; i++){
//
//			// Clear their scale and lead offsets
//			target_page->Track[i]->scale_pitch_offset = 0;
//			target_page->Track[i]->lead_pitch_offset = 0;
//		}
//
//		// Turn off the scale mode.
//		target_page->scaleStatus = OFF;
//	}
//
//
//	if ( keyNdx == KEY_SCALE_SEL ) {
//
//		// D O U B L E - C L I C K  C O N S T R U C T
//		// DOUBLE CLICK SCENARIO
//		if ((DOUBLE_CLICK_TARGET == keyNdx) 
//				&& (DOUBLE_CLICK_TIMER > DOUBLE_CLICK_ALARM_SENSITIVITY)) {
//	
//			// Double click code
//			// ...
//			// Enter the Scale selection mode
//
//			switch( target_page->scaleStatus ){
//	
//				case SCALE_MOD:
//				case OFF:			
//
//					// Switch to the scale selection mode.
////					target_page->scaleStatus = SCALE_SEL;
//
//					// Make sure force to scale is ON 
////					target_page->force_to_scale = ON;
//
//					// This fixes the problem of not being able to start with the CHR scale as default
////					target_page->scaleNotes[0] = target_page->current_scale;
//					break;
//			} // switch( target_page->scaleStatus )	
//		} // end of double click scenario 
//		
//	
//		// SINGLE CLICK SCENARIO
//		else if (DOUBLE_CLICK_TARGET == 0) {
//			
//				DOUBLE_CLICK_TARGET = keyNdx;
//				DOUBLE_CLICK_TIMER = ON;
//				// Start the Double click Alarm
//				cyg_alarm_initialize(	
//						doubleClickAlarm_hdl, 
//						cyg_current_time() + DOUBLE_CLICK_ALARM_TIME,
//						DOUBLE_CLICK_ALARM_TIME );
//	
//			// Single click code
//			// ...
//			
//			// When not in scale mode, just toggle force to scale
//			if ( target_page->scaleStatus == OFF ){
//
//				// Turn on force to scale
//				target_page->force_to_scale = ON;
//
//				// Activate the MOD scale mode
//				target_page->scaleStatus = SCALE_MOD;
//			}
//			else{
//				
//				// RETURN / ESCAPE: clear the scale and lead offsets and 
//				// exit the scale mode. Track pitch attributes are not affected by this.
//
//				// Iterate through all tracks
//				for (i=0; i<MATRIX_NROF_ROWS; i++){
//		
//					// Clear their scale and lead offsets
//					target_page->Track[i]->scale_pitch_offset = 0;
//				}
//		
//				// Get out of the scale selection mode
//				target_page->scaleStatus = OFF;
//
//				// Lock the current scale
//				target_page->force_to_scale = OFF;
//			}
//		}
//
//		// No further execution required
//		return;
//
//	} // key is SCALE_SEL
//
//
//
//	if (keyNdx == KEY_SCALE_MOD) {
//
//		// Do not react when force to scale is on
//		if ( target_page->scaleStatus == SCALE_SEL ){
//
//			target_page->scaleStatus = SCALE_MOD;
//		}
//	} // keyNdx == SCALE_MOD
//
//
//
////	if (keyNdx == KEY_SCALE_CAD) {
////	
////		if ( target_page->scaleStatus == SCALE_MOD ){
////
////			target_page->scaleStatus = SCALE_SEL;
////		}		
////	} // keyNdx == KEY_SCALE_CAD
//
//	if (keyNdx == KEY_SCALE_MYSEL) {
//	
//		if ( target_page->scaleStatus == SCALE_MOD ){
//
//			target_page->scaleStatus = SCALE_SEL;
//		}		
//	} // keyNdx == KEY_SCALE_CAD
//
//
//
//
//	//
//	// OCTAVE CIRCLE: Key is in the octave circle
//	//
//	if ( 	(keyNdx == KEY_NOTE_C	) ||
//			(keyNdx == KEY_NOTE_Cis	) ||
//			(keyNdx == KEY_NOTE_D	) ||
//			(keyNdx == KEY_NOTE_Dis	) ||
//			(keyNdx == KEY_NOTE_E	) ||
//			(keyNdx == KEY_NOTE_F	) ||
//			(keyNdx == KEY_NOTE_Fis	) ||
//			(keyNdx == KEY_NOTE_G	) ||
//			(keyNdx == KEY_NOTE_Gis	) ||
//			(keyNdx == KEY_NOTE_A	) ||
//			(keyNdx == KEY_NOTE_Ais	) ||
//			(keyNdx == KEY_NOTE_B	) 		
//		){
//		 
//		// Transform key Index to a note index and send it to the scale modifier
//		modify_scale_composition( target_page, keyNdx_to_noteNdx( keyNdx ) );
//	} 
//
//
//
//	// 
//	// SCALE TYPE BUTTONS. Apply scale interval structures to tracks
//	// 
//	if ( 	
////			(keyNdx == KEY_SCALE_MYSEL) 
//			(keyNdx == KEY_SCALE_PEN)
//		||	(keyNdx == KEY_SCALE_WHL)	
//		||	(keyNdx == KEY_SCALE_MAJ)	
//		||	(keyNdx == KEY_SCALE_MIN)	
//		||	(keyNdx == KEY_SCALE_DIM)	
//		||	(keyNdx == KEY_SCALE_CHR)		
//		){
//	
//		// Clear the scale and lead offsets in the tracks
//		for (i=0; i<MATRIX_NROF_ROWS; i++){
//			target_page->Track[i]->scale_pitch_offset = 0;
//			// target_page->Track[i]->lead_pitch_offset = 0;
//		}
//
//		// Act according to selected scale
//		switch( keyNdx ){
//			
////			case KEY_SCALE_MYSEL:
////				// Recall the last non-standard scale, but apply to current lead
////				target_page->scaleStatus = SCALE_SEL;
////				target_page->scaleNotes[0] = 				
////						my_shift_bitpattern( 	target_page->my_scale_signature, 12, INC,
////												my_bit2ndx( target_page->scaleLead[0] )+1);
////				break;
//
//			case KEY_SCALE_PEN:
//				target_page->my_scale_signature = target_page->scaleNotes[0];
//				target_page->scaleNotes[0] = SCALE_SIG_PEN;
//				break;
//				
//			case KEY_SCALE_WHL:
//				target_page->my_scale_signature = target_page->scaleNotes[0];
//				target_page->scaleNotes[0] = SCALE_SIG_WHL;
//				break;
//				
//			case KEY_SCALE_MAJ:
//				target_page->my_scale_signature = target_page->scaleNotes[0];
//				target_page->scaleNotes[0] = SCALE_SIG_MAJ;		
//				break;
//				
//			case KEY_SCALE_MIN:
//				target_page->my_scale_signature = target_page->scaleNotes[0];
//				target_page->scaleNotes[0] = SCALE_SIG_MIN;
//				break;
//
//			case KEY_SCALE_DIM:
//				target_page->my_scale_signature = target_page->scaleNotes[0];
//				target_page->scaleNotes[0] = SCALE_SIG_DIM;		
//				break;
//
//			case KEY_SCALE_CHR:
//				target_page->my_scale_signature = target_page->scaleNotes[0];
//				target_page->scaleNotes[0] = SCALE_SIG_CHR;
//				break;
//
//		} // switch( keyNdx )
//		
//		//		d_iag_printf( "SCL SEL: scale Notes shift:%d:\n", 11 - my_bit2ndx(target_page->scaleLead) );
//		//		my_print_bits32( target_page->scaleNotes );
//		
//		// Shift the signature according to the lead
//		target_page->scaleNotes[0] = 
//			my_shift_bitpattern( 	target_page->scaleNotes[0], 
//									12, 
//									INC, 
//									(11 - my_bit2ndx(target_page->scaleLead[0] ) ) 
//								);
//
//		//	d_iag_printf( "SCL SEL: shifted/lead:%d/defaultPEN/adjusted:%d/\n", 
//		//									my_bit2ndx( target_page->scaleLead )+1 );
//		//	my_print_bits32( target_page->scaleNotes );
//		//	my_print_bits32( target_page->scaleLead );
//		//	my_print_bits32( SCALE_SIG_PEN );	
//		//	my_print_bits32( my_shift_bitpattern( 	target_page->scaleNotes, 12, INC,
//		//												my_bit2ndx( target_page->scaleLead )+1
//		//												)
//		//					);
//
//		// Export the changes to the tracks
//		target_page->scaleLead_old = target_page->scaleLead[0];
//		target_page->scaleNotes_old = target_page->scaleNotes[0];
//
//		// Simply update the current scale with what is selected in the octave circle
////		target_page->current_scale = target_page->scaleNotes[0];
//		
//		// Program the new scale into the page		
//		if ( target_page->SCL_align == ON ){
//
//			program_scale_pitches( target_page );
//		}
//
//	} // Key is one of the preset scales
}


