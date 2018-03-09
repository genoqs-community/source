

	switch (content) {
		
		// Show the pitch of the selected track, only when one selected
		case TRACK_TRANSPOSITION:
		
			if ( zoom_level == zoomGRIDTRACK ){

				for ( i=0; i < GRID_NROF_BANKS; i++ ){
			
					// A page is active in the bank
					if ( GRID_p_selection[i] != NULL ){
					
						if (	( GRID_p_selection[i]->trackSelection != 0 )
							&&	( my_bit_cardinality( GRID_p_selection[i]->trackSelection ) == 1 )
							){
			
							// Get the pitch of the selected track
							my_pitch = 
								GRID_p_selection[i]->Track[ my_bit2ndx( GRID_p_selection[i]->trackSelection ) ]->attribute[ATTR_PITCH] 
								+ GRID_p_selection[i]->Track[ my_bit2ndx( GRID_p_selection[i]->trackSelection ) ]->event_PIT ;
							
							// Display it in the inner circle
							show_pitch_in_circle( my_pitch, ON );
		
							// Terminate the loop
							i = GRID_NROF_BANKS;
						}
					} 
				} // Bank iterator	
			} // GRIDTRACK mode is active

			else{

				// Depending on how many tracks are selected in the page
				switch( my_bit_cardinality( VIEWER_page->trackSelection ) ){
	
					// Only one track is selected
					case 1:
						// Get the pitch of the selected track
						my_pitch = 
							VIEWER_page->Track[ my_bit2ndx( VIEWER_page->trackSelection ) ]->attribute[ATTR_PITCH] 
							+ VIEWER_page->Track[ my_bit2ndx( VIEWER_page->trackSelection ) ]->event_PIT ;
						
						// Display it in the inner circle
						show_pitch_in_circle( my_pitch, ON );
						break;
	
					// More than one track is selected
					default:
						// For now nothing to do
						break;
				} // switch( cardinality of trackselection )
			}

			break;

		
			
		case PAGE_TRANSPOSITION:
		
			// Only show it if there is no track selection in page
			if ( VIEWER_page->trackSelection != 0 ) {
				// Will have to evaluate what makes most sense here
				// break;
			}
		
			// This is the octave-wise trans-position : RED
			// The middle octave (case 5) is always lit
			MIR_write_dot( LED_NOTE_Fis,	MIR_RED		);					

			switch ( Page_repository[GRID_CURSOR].attribute[ATTR_PITCH] / 12 ){
				case 0:
					MIR_write_dot( LED_NOTE_Cis,	MIR_RED		);

				case 1:
					MIR_write_dot( LED_NOTE_D,		MIR_RED		);

				case 2:
					MIR_write_dot( LED_NOTE_Dis,	MIR_RED		);

				case 3:
					MIR_write_dot( LED_NOTE_E,		MIR_RED		);

				case 4:
					MIR_write_dot( LED_NOTE_F,		MIR_RED		);
					break;						

				// Scale up: fallthrough intended for Balken
				case 10:
					// This LED shlould never light red
					// MIR_write_dot( LED_NOTE_B,		MIR_RED		);

				case 9:
					MIR_write_dot( LED_NOTE_Ais,	MIR_RED		);

				case 8:
					MIR_write_dot( LED_NOTE_A,		MIR_RED		);

				case 7:
					MIR_write_dot( LED_NOTE_Gis,	MIR_RED		);			

				case 6:
					MIR_write_dot( LED_NOTE_G,		MIR_RED		);
					break;	
			}

			// This is the note-wise trans-position: GREEN
			if ( Page_repository[GRID_CURSOR].attribute[ATTR_PITCH] == 127) {
				// Light Cup only when the transpose limit is reached
				MIR_write_dot( LED_NOTE_Cup, 	MIR_GREEN	);
			}
			switch ( Page_repository[GRID_CURSOR].attribute[ATTR_PITCH] % 12 ) {
				case 0:
					// Consider the "headroom reached case"
					if ( Page_repository[GRID_CURSOR].attribute[ATTR_PITCH] < 120 ){
						MIR_write_dot( LED_NOTE_C, 		MIR_GREEN 	);
					}
					else {
						MIR_write_dot( LED_NOTE_Cup, 	MIR_GREEN	);
					}
					break;
				case 1:
					MIR_write_dot( LED_NOTE_Cis, 	MIR_GREEN	);
					break;
				case 2:
					MIR_write_dot( LED_NOTE_D, 		MIR_GREEN	);
					break;
				case 3:
					MIR_write_dot( LED_NOTE_Dis, 	MIR_GREEN	);
					break;
				case 4:
					MIR_write_dot( LED_NOTE_E, 		MIR_GREEN	);
					break;
				case 5:
					MIR_write_dot( LED_NOTE_F, 		MIR_GREEN	);
					break;
				case 6:
					MIR_write_dot( LED_NOTE_Fis, 	MIR_GREEN	);
					break;
				case 7:
					MIR_write_dot( LED_NOTE_G, 		MIR_GREEN	);
					break;
				case 8:
					MIR_write_dot( LED_NOTE_Gis,	MIR_GREEN	);
					break;
				case 9:
					MIR_write_dot( LED_NOTE_A, 		MIR_GREEN	);
					break;
				case 10:
					MIR_write_dot( LED_NOTE_Ais, 	MIR_GREEN	);
					break;
				case 11:
					MIR_write_dot( LED_NOTE_B, 		MIR_GREEN	);
					break;
			}
			break; // case PAGE_TRANSPOSITION:
			
		
		
		// Show the picture of the global locator, color to be determined
		case GLOBAL_LOCATOR_PICTURE:

			// Determine the color to be used: DIRECT->GREEN // OCLOCK->RED
			j = MIR_RED;

//			if ( GRID_bank_triggermodes == OFF ){
			if ( GRID_switch_mode == GRID_SWITCH_DIRECT ){
				// This is the DIRECT mode
				j = MIR_GREEN;
			}

			// This is the rotating picture..
			switch ( GLOBAL_LOCATOR ) {
				
				// Fallthrough overall intended - to get the packman effect
				case 16:
					MIR_write_dot( LED_NOTE_F, 		j );
//					MIR_write_dot( LED_SCALE_CAD,	j );
				case 15:
					MIR_write_dot( LED_NOTE_E, 		j );
//					MIR_write_dot( LED_NOTE_Cup, 	j );
				case 14:
					MIR_write_dot( LED_NOTE_Dis, 	j );
//					MIR_write_dot( LED_NOTE_B, 		j );
				case 13:
					MIR_write_dot( LED_NOTE_D, 		j );
//					MIR_write_dot( LED_NOTE_Ais, 	j );
				case 12:
					MIR_write_dot( LED_NOTE_Cis, 	j );
//					MIR_write_dot( LED_NOTE_A, 		j );
				case 11:
					MIR_write_dot( LED_NOTE_C, 		j );
//					MIR_write_dot( LED_NOTE_Gis, 	j );
				case 10:
					MIR_write_dot( LED_SCALE_MOD,	j );
//					MIR_write_dot( LED_NOTE_G, 		j );
				case 9:
					MIR_write_dot( LED_SCALE_SEL,	j );
//					MIR_write_dot( LED_NOTE_Fis, 	j );
				case 8:
					MIR_write_dot( LED_SCALE_CAD,	j );
//					MIR_write_dot( LED_NOTE_F, 		j );
				case 7:
					MIR_write_dot( LED_NOTE_Cup, 	j );
//					MIR_write_dot( LED_NOTE_E, 		j );
				case 6:
					MIR_write_dot( LED_NOTE_B, 		j );
//					MIR_write_dot( LED_NOTE_Dis,	j );
				case 5:
					MIR_write_dot( LED_NOTE_Ais, 	j );
//					MIR_write_dot( LED_NOTE_D, 		j );
				case 4:
					MIR_write_dot( LED_NOTE_A, 		j );
//					MIR_write_dot( LED_NOTE_Cis, 	j );
				case 3:
					MIR_write_dot( LED_NOTE_Gis, 	j );
//					MIR_write_dot( LED_NOTE_C, 		j );
				case 2:
					MIR_write_dot( LED_NOTE_G, 		j );
//					MIR_write_dot( LED_SCALE_MOD,	j );
				case 1:
					MIR_write_dot( LED_NOTE_Fis, 	j );
//					MIR_write_dot( LED_SCALE_SEL,	j );
				case 0:
					// Show nothing
					break;
			} // switch (GLOBAL_LOCATOR)

//			if ( RUNBIT == OFF ){
//				// Always show the Trigger status, also in stop mode.
//				MIR_write_dot( LED_SCALE_SEL,	j );
//			}

//			if ( RUNBIT == OFF ){
				// Always show the Trigger status, also in stop mode.
				MIR_write_dot( LED_SCALE_SEL,	MIR_RED   );
				MIR_write_dot( LED_SCALE_SEL,	MIR_GREEN );
//			}

			break; // GLOBAL_LOCATOR_PICTURE
			

		
//		// Show the selected scale for the VIEWER_page				
//		case SCALE_SELECTION:
//
//			// Determine which scale to show
//			if ( VIEWER_page->scaleStatus == OFF ){
//				j = VIEWER_page->current_scale;
//			}
//			else{
//				j = VIEWER_page->scaleNotes[0];
//			}
// 
//			// ON fields showing first
//			for (i=0; i<12; i++) {
//				
//				// If the note is selected as ON
//				if ( j & (1 << (11-i)) ) {
//
//					switch (i) {
//						case 0:
//							MIR_write_dot( LED_NOTE_C, 		MIR_GREEN );
//							break;
//						case 1:
//							MIR_write_dot( LED_NOTE_Cis, 	MIR_GREEN );
//							break;
//						case 2:
//							MIR_write_dot( LED_NOTE_D,	 	MIR_GREEN );
//							break;
//						case 3:
//							MIR_write_dot( LED_NOTE_Dis, 	MIR_GREEN );
//							break;
//						case 4:
//							MIR_write_dot( LED_NOTE_E,	 	MIR_GREEN );
//							break;
//						case 5:
//							MIR_write_dot( LED_NOTE_F,	 	MIR_GREEN );
//							break;
//						case 6:
//							MIR_write_dot( LED_NOTE_Fis, 	MIR_GREEN );
//							break;
//						case 7:
//							MIR_write_dot( LED_NOTE_G,	 	MIR_GREEN );
//							break;
//						case 8:
//							MIR_write_dot( LED_NOTE_Gis, 	MIR_GREEN );
//							break;
//						case 9:
//							MIR_write_dot( LED_NOTE_A, 		MIR_GREEN );
//							break;
//						case 10:
//							MIR_write_dot( LED_NOTE_Ais, 	MIR_GREEN );
//							break;
//						case 11:
//							MIR_write_dot( LED_NOTE_B,	 	MIR_GREEN );
//							break;									
//					}
//				}
//			}
//
//
////			// Show lead only when outside of scale
////			if ( VIEWER_page->scaleStatus != OFF ){
////				break;
////			}
//
//
//			// UP fields showing first. This shows the scale lead
//			for (i=0; i<12; i++) {
//				
//				// If the note is selected as UP
//				if ( VIEWER_page->scaleLead[0] & (1 << (11-i) ) ) {							
//					
//					switch (i) {
//						case 0:
//							MIR_write_dot( LED_NOTE_C, 		MIR_RED );
//							break;
//						case 1:
//							MIR_write_dot( LED_NOTE_Cis, 	MIR_RED );
//							break;
//						case 2:
//							MIR_write_dot( LED_NOTE_D,	 	MIR_RED );
//							break;
//						case 3:
//							MIR_write_dot( LED_NOTE_Dis, 	MIR_RED );
//							break;
//						case 4:
//							MIR_write_dot( LED_NOTE_E,	 	MIR_RED );
//							break;
//						case 5:
//							MIR_write_dot( LED_NOTE_F,	 	MIR_RED );
//							break;
//						case 6:
//							MIR_write_dot( LED_NOTE_Fis, 	MIR_RED );
//							break;
//						case 7:
//							MIR_write_dot( LED_NOTE_G,	 	MIR_RED );
//							break;
//						case 8:
//							MIR_write_dot( LED_NOTE_Gis, 	MIR_RED );
//							break;
//						case 9:
//							MIR_write_dot( LED_NOTE_A, 		MIR_RED );
//							break;
//						case 10:
//							MIR_write_dot( LED_NOTE_Ais, 	MIR_RED );
//							break;
//						case 11:
//							MIR_write_dot( LED_NOTE_B,	 	MIR_RED );
//							break;									
//					}
//				}
//			}
//			break; // case SCALE_SELECTION



		// Light up everything green, except for active one which is red
		case SELECTED_CADENCE_KEY:
		
			// i = VIEWER_page->cadence->key[ VIEWER_page->cadence->locator ];
			j = MIR_GREEN;
		
			MIR_write_dot( LED_NOTE_C, 		j );
			MIR_write_dot( LED_NOTE_Cis, 	j );
			MIR_write_dot( LED_NOTE_D,	 	j );
			MIR_write_dot( LED_NOTE_Dis, 	j );
			MIR_write_dot( LED_NOTE_E,	 	j );
			MIR_write_dot( LED_NOTE_F,	 	j );
			MIR_write_dot( LED_NOTE_Fis, 	j );
			MIR_write_dot( LED_NOTE_G,	 	j );
			MIR_write_dot( LED_NOTE_Gis, 	j );
			MIR_write_dot( LED_NOTE_A, 		j );
			MIR_write_dot( LED_NOTE_Ais, 	j );
			MIR_write_dot( LED_NOTE_B,	 	j );					

			MIR_write_dot( LED_SCALE_MOD, 	j );
			MIR_write_dot( LED_SCALE_SEL,	j );

//			MIR_write_dot( i, MIR_BLINK );

			// The Cup key is the eraser
//			if ( VIEWER_page->cadence->key[ VIEWER_page->cadence->locator ] != 0 ){
//				MIR_write_dot( LED_NOTE_Cup, MIR_RED );
//			}

			break;


	} // case ELE_OCTAVE_CIRCLE / switch( content )			
