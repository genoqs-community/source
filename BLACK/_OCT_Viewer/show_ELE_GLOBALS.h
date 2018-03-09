

switch (content) {
	
	// NUMERIC INDICATOR RELEVANT FOR BIG KNOB
	case NUMERIC_QUADRANT:
	
		switch( zoom_level ){						
			
			case zoomGRIDTRACK:
				// fallthrough

			case zoomGRID:

				switch( GRID_status ){
					case GRID_SELECT:
						
						// Show selection
						// MIR_write_dot_C( GRID_scene, MIR_GREEN );
						MIR_write_dot_C( GRID_scene, MIR_RED   );
						MIR_write_dot_C( GRID_scene, MIR_BLINK );							 
						break;


					case GRID_EXPORT:
						MIR_write_dot_C( GRID_scene, MIR_RED   );
						MIR_write_dot_C( GRID_scene, MIR_BLINK );
						break;									


					case GRID_DEFAULT: 
						if ( CLOCK_SOURCE != EXT ){

							MIR_write_numeric_C( TEMPO );
						}
						break;
					}
				break;
				

			default:
				if ( VIEWER_page->trackSelection == 0 ){

					switch( GRID_bigknob_SELECT ){

						case ON:
							// Only relevant in page mode
							if ( zoom_level != zoomPAGE ){
								break;
							}

							// Green Tempo LED
							MIR_write_dot( LED_TEMPO, MIR_GREEN );
							
							// Orange Select LED
							MIR_write_dot( LED_SELECT, MIR_RED   );
							MIR_write_dot( LED_SELECT, MIR_GREEN );
	
							// Show the currently active banks
							for( i=0; i<GRID_NROF_BANKS; i++ ){
	
								// There is a page playing in the bank
								if ( GRID_p_selection[i] != NULL ){
	
									// All active pages in green
									MIR_write_dot( circleNrField[8-i], MIR_GREEN );
	
									// Current page blinking orange
									if ( GRID_p_selection[i] == VIEWER_page ){
	
										MIR_write_dot( circleNrField[8-i], MIR_RED   );
										MIR_write_dot( circleNrField[8-i], MIR_BLINK );
									}
								}
							} // bank iterator
							break;
						
						case OFF:
							// Orange Tempo LED and TEMPO value
							MIR_write_dot( LED_TEMPO, MIR_RED   );
							MIR_write_dot( LED_TEMPO, MIR_GREEN );

							if ( CLOCK_SOURCE != EXT ){
								// Write the TEMPO in the numeric quadrant of the Circle
								MIR_write_numeric_C( TEMPO );
							}
	
							// Green Select LED
							if ( zoom_level == zoomPAGE ){
								MIR_write_dot( LED_SELECT, MIR_GREEN );
							}
							break;
					}

				}
				break;

		}
		break;



	// TRANSPORT AREA	
	case TRANSPORT:

		if ( RUNBIT == ON ) {
			
			i = LED_PLAY1;
			// MIDI LOAD MONITOR					
			if (MIDI_LOAD_MONITOR1 == MIR_RED){
				
				MIR_write_dot (258, MIR_RED);
				// MIR_write_dot (i, MIR_BLINK);
			}
			if (MIDI_LOAD_MONITOR2 == MIR_RED){
				
				MIR_write_dot (257, MIR_RED);
				// MIR_write_dot (i, MIR_BLINK);
			}
			else {

				if ( zoom_level != zoomTRACK ){
					MIR_write_dot (i, MIR_GREEN);
				}
//				MIR_write_dot (i, MIR_BLINK);				
			}


			i = LED_PAUSE;

			// CPU LOAD MONITOR					
			if ( CPU_LOAD_MONITOR == MIR_RED ){
				
//				MIR_write_dot (i, MIR_RED);
				// MIR_write_dot (i, MIR_BLINK);
				MIR_write_dot (256, MIR_RED);
				MIR_write_dot (255, MIR_RED);
				MIR_write_dot (254, MIR_RED);
				MIR_write_dot (253, MIR_RED);
				MIR_write_dot (252, MIR_RED);
			}
			// Almost overrunning
			else if ( PLAY_LOAD > (PLAY_LIMIT - 7) ){
				MIR_write_dot (256, MIR_GREEN);
				MIR_write_dot (255, MIR_GREEN);				
				MIR_write_dot (254, MIR_GREEN);				

				MIR_write_dot (256, MIR_RED);
				MIR_write_dot (255, MIR_RED);
				MIR_write_dot (254, MIR_RED);				
			}

		}

		else {
			// STOP condition
			MIR_write_dot (LED_STOP, MIR_RED);		
			// Blink when running on internal clock
			if ( CLOCK_SOURCE == INT ) {
				// MIR_write_dot (LED_STOP, MIR_BLINK);
			}
		}

		// Pause condition
		if (PAUSEBIT == ON) {
			// Show the pausebit
			MIR_write_dot( LED_PAUSE, MIR_RED   );
			MIR_write_dot( LED_PAUSE, MIR_GREEN );
			// Blink when running on internal clock
			if ( CLOCK_SOURCE == INT ){
				// MIR_write_dot( LED_PAUSE, MIR_BLINK );
			}
		}

		break; // TRANSPORT
		
		
	// FOLLOW_?? state indicator
	case FOLLOW:
		switch( zoom_level ){
			
			case zoomGRIDTRACK:
			case zoomGRID:

				break;

			case zoomPAGE:
				if ( follow_flag == FOLLOW_PAGE ){

					MIR_write_dot( LED_FOLLOW, MIR_GREEN   );
				}
				else {
					MIR_write_dot( LED_FOLLOW, MIR_RED     );
				}
				break;

			case zoomTRACK:
			case zoomMAP:
				if ( follow_flag == FOLLOW_TRACK ){

					MIR_write_dot( LED_FOLLOW, MIR_GREEN );
				}
				else {
					MIR_write_dot( LED_FOLLOW, MIR_RED );
				}
				break;
		}
		break; // TRANSPORT
		
		
		

	// Buttons of Big Knob
	case BIG_KNOB:
	
		switch( zoom_level ){

			//
			// GRID zoom 7 mode
			//
			case zoomGRIDTRACK:
			case zoomGRID:

				switch( GRID_status ){

					case GRID_DEFAULT:
						
						// TEMPO shows switchmode for the GRID
						if ( GRID_switch_mode == GRID_SWITCH_OCLOCK ){
							MIR_write_dot( LED_TEMPO, MIR_RED   );
						}
						else{
							MIR_write_dot( LED_TEMPO, MIR_GREEN );
						}
											
						//ALIGN is available
						MIR_write_dot( LED_ALIGN, MIR_GREEN );
						break;


					case GRID_SELECT:

						// Show the selected scene
						MIR_write_dot( LED_SELECT, MIR_RED   );
						MIR_write_dot( LED_SELECT, MIR_GREEN );

						// TEMPO is available
						MIR_write_dot( LED_TEMPO, MIR_GREEN );	
						break;


					case GRID_EXPORT:
						// EXPORT is active
						MIR_write_dot( LED_ALIGN, MIR_RED 	);
						MIR_write_dot( LED_ALIGN, MIR_GREEN );
						MIR_write_dot( LED_ALIGN, MIR_BLINK );

						// SELECT is available
						MIR_write_dot( LED_SELECT, MIR_GREEN );
						
						// TEMPO is available
						MIR_write_dot( LED_TEMPO, MIR_GREEN );	
						break;
				}
				
				break;

			
			case zoomPAGE:

				switch( GRID_bigknob_SELECT ){
					case ON:
						break;
				}


			// 
			// Modes other than GRID or PAGE
			// 
			default:
				// Program change numbers in track selection mode
				if ( my_bit_cardinality( VIEWER_page->trackSelection ) == 1 ){

					// If in track zoom mode
					if ( zoom_level == zoomTRACK ){

						switch( VIEWER_page->trackAttributeSelection ){

							// VEL
							case ( 1 << 0 ):
								MIR_write_factor_C( VIEWER_page->Track[ my_bit2ndx( VIEWER_page->trackSelection )]->VEL_factor );
								MIR_write_dot( LED_SELECT, MIR_RED   );
								MIR_write_dot( LED_SELECT, MIR_GREEN );
								break;

							// PIT 
							case ( 1 << 1 ):
								MIR_write_factor_C( VIEWER_page->Track[ my_bit2ndx( VIEWER_page->trackSelection )]->PIT_factor );
								MIR_write_dot( LED_SELECT, MIR_RED   );
								MIR_write_dot( LED_SELECT, MIR_GREEN );
								break;

							// LEN 
							case ( 1 << 2 ):
								MIR_write_factor_C( VIEWER_page->Track[ my_bit2ndx( VIEWER_page->trackSelection )]->LEN_factor );
								MIR_write_dot( LED_SELECT, MIR_RED   );
								MIR_write_dot( LED_SELECT, MIR_GREEN );
								break;

							// STA 
							case ( 1 << 3 ):
								MIR_write_factor_C( VIEWER_page->Track[ my_bit2ndx( VIEWER_page->trackSelection )]->STA_factor );
								MIR_write_dot( LED_SELECT, MIR_RED   );
								MIR_write_dot( LED_SELECT, MIR_GREEN );
								break;

							// GRV 
							case ( 1 << 7 ):
								MIR_write_factor_C( VIEWER_page->Track[ my_bit2ndx( VIEWER_page->trackSelection )]->GRV_factor );
								MIR_write_dot( LED_SELECT, MIR_RED   );
								MIR_write_dot( LED_SELECT, MIR_GREEN );
								break;

							// MCC
							case ( 1 << 8 ):
								MIR_write_factor_C( VIEWER_page->Track[ my_bit2ndx( VIEWER_page->trackSelection )]->MCC_factor );
								MIR_write_dot( LED_SELECT, MIR_RED   );
								MIR_write_dot( LED_SELECT, MIR_GREEN );
								break;
							
							case 0:
//							default:
								// Write the Program change number in the numeric quadrant of the Circle
								// ..used to be offset by +1, removed offset based on feedback from Tom
								MIR_write_numeric_C( VIEWER_page->Track[ my_bit2ndx( VIEWER_page->trackSelection )]->program_change );
										MIR_write_dot( LED_PROGRAM, MIR_RED );
								MIR_write_numeric_C( VIEWER_page->Track[ my_bit2ndx( VIEWER_page->trackSelection )]->program_change );
										MIR_write_dot( LED_PROGRAM, MIR_GREEN );
								break;


//							// VEL
//							case ( 1 << 0 ):
//								// Show the VEL range for track
//								MIR_write_numeric_C( VIEWER_page->Track[ my_bit2ndx( VIEWER_page->trackSelection )]->range_VEL );
//								MIR_write_dot( LED_SELECT, MIR_RED   );
//								MIR_write_dot( LED_SELECT, MIR_GREEN );
//								break;
//
//							// PIT 
//							case ( 1 << 1 ):
//								// Show the VEL range for track
//								MIR_write_numeric_C( VIEWER_page->Track[ my_bit2ndx( VIEWER_page->trackSelection )]->range_PIT );
//								MIR_write_dot( LED_SELECT, MIR_RED   );
//								MIR_write_dot( LED_SELECT, MIR_GREEN );
//								break;
//
//							// MCC
//							case ( 1 << 8 ):
//								// Show the VEL range for track
//								MIR_write_numeric_C( VIEWER_page->Track[ my_bit2ndx( VIEWER_page->trackSelection )]->range_MCC );
//								MIR_write_dot( LED_SELECT, MIR_RED   );
//								MIR_write_dot( LED_SELECT, MIR_GREEN );
//								break;
//							
//							case 0:
////							default:
//								// Write the Program change number in the numeric quadrant of the Circle
//								// ..used to be offset by +1, removed offset based on feedback from Tom
//								MIR_write_numeric_C( VIEWER_page->Track[ my_bit2ndx( VIEWER_page->trackSelection )]->program_change );
//										MIR_write_dot( LED_PROGRAM, MIR_RED );
//								MIR_write_numeric_C( VIEWER_page->Track[ my_bit2ndx( VIEWER_page->trackSelection )]->program_change );
//										MIR_write_dot( LED_PROGRAM, MIR_GREEN );
//								break;
						}

					}
					else{

						// Write the Program change number in the numeric quadrant of the Circle
						// ..used to be offset by +1, removed offset based on feedback from Tom
						MIR_write_numeric_C( VIEWER_page->Track[ my_bit2ndx( VIEWER_page->trackSelection )]->program_change );

						MIR_write_dot( LED_PROGRAM, MIR_RED );
						MIR_write_dot( LED_PROGRAM, MIR_GREEN );
					}
				}

				else if ( VIEWER_page->trackSelection == 0 ) {
				}
				break;
		}	
		break;



	// Clock Source Indicator
	case CLOCK:
	
		switch( CLOCK_SOURCE ){
	
			case EXT:
	
				// Indicate the UART whose clock we are listening to
				switch( CLOCK_MASTER_UART ){

					case 0:
						MIR_write_dot( LED_CLOCK, MIR_GREEN 	);
						break;
					
					case 1:
						MIR_write_dot( LED_CLOCK, MIR_RED		);
						break;
				}					

				// Blink when sequencer running
				 if (RUNBIT == ON) {
					// MIR_write_dot( LED_CLOCK, MIR_BLINK );
					MIR_write_dot( LED_TEMPO, MIR_BLINK );
				 }
				break;
				
			case INT:
				// Visible/available only when the sequencer is not running.
				// if (RUNBIT == OFF) {
					MIR_write_dot( LED_CLOCK, MIR_GREEN );
					MIR_write_dot( LED_CLOCK, MIR_RED 	);
				// }
				break;										

			case OFF:
				// Visible/available only when the sequencer is not running.
				if (RUNBIT == OFF) {
					// MIR_write_dot( LED_CLOCK, MIR_GREEN );
					// ..nothing really to do
				}
				break;
		}
		
		break;


} // switch (content)






