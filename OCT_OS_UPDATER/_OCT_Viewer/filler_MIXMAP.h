

		//
		// MIX INDICATOR - LED corresponding to the rotarys
		//
		show( ELE_MIX_INDICATOR, RED   );
		show( ELE_MIX_INDICATOR, GREEN );
		// show( ELE_MIX_INDICATOR, BLINK );

		//
		// MIX MASTER
		//
		switch (target_page->mixMode) {
			case INC:
				show (ELE_MIX_MASTER, GREEN);
				break;
				
			case DEC:
				show (ELE_MIX_MASTER, RED);
				break;
		}

		//
		// TRACK_SELECTORS
		//
		switch( target_page->mixTarget ){

			case MIXTGT_ATR:
				// Selectable Attribute Set
				show (ELE_TRACK_SELECTORS, TRACK_ATTRIBUTES_ALL);

				// Selected attribute
				show (ELE_TRACK_SELECTORS, MIX_ATTRIBUTE_SELECTED);		
				
				// ALIGN LED
				MIR_write_dot( LED_ALIGN, MIR_GREEN );		
				break;


			case MIXTGT_USR1:
			case MIXTGT_USR2:
			case MIXTGT_USR3:
			case MIXTGT_USR4:
			case MIXTGT_USR5:

				// Show the available attributes of the CC_MIXMAP
				// ..if no track is selected for learning
				switch( target_page->REC_bit ){
				
					case OFF:
						show( ELE_TRACK_SELECTORS, CC_MIXMAP_ATTRIBUTES );
						break;
					
					case ON:
						// Show the track selection - learning target blinking
						MIR_write_buttool( LHS, 1 << CCMAP_learner, MIR_RED 	);
						MIR_write_buttool( LHS, 1 << CCMAP_learner, MIR_BLINK 	);
						
						// Show the recording bit
						MIR_write_dot( LED_RECORD, MIR_RED 	 );
						MIR_write_dot( LED_RECORD, MIR_GREEN );
						MIR_write_dot( LED_RECORD, MIR_BLINK );
						
						// Show the currently selected attribute
						switch( target_page->CC_MIXMAP_attribute ){
							
							case CC_MIXMAP_AMT:
								i = 1<<6;
								break;
							case CC_MIXMAP_MCC:
								i = 1<<8;
								break;
							case CC_MIXMAP_MCH:
								i = 1<<9;
								break;
						}
						MIR_write_buttool( LHS, i, MIR_RED   );
						MIR_write_buttool( LHS, i, MIR_GREEN );
						
						break;
				}
				break;


			// These dont show anything for now
			case MIXTGT_VOL:
			case MIXTGT_PAN:
			case MIXTGT_MOD:
			case MIXTGT_EXP:
				break;
		}



		switch( GRID_MAP0_status ){
			case ON:
				// Points to the GRID map set (GRID is also lit green)
				MIR_write_dot( LED_MIXTGT_USR0, MIR_GREEN   );
				break;

			case OFF:
				// Points to the local grid set
				MIR_write_dot( LED_MIXTGT_USR0, MIR_RED );
				break;
		}
		

		// SELECT MASTER
		// 
		// see TRACK SELECTORS


		// MIX TARGET field
		//
		show( ELE_MIXTGT, OPTIONS );


		// MATRIX
		//
		show( ELE_MATRIX, MIXAMT_VALS );
							

		// EDIT MASTER
		switch( target_page->editorMode ){

			case CONTROL:		
				// Edit acts as MCC source - send MCC data
				show( ELE_EDIT_MASTER, GREEN );
				show( ELE_EDIT_MASTER, RED );
				break;

			case EDIT:
				// This is the regular mode - edit attributes of selection
				show( ELE_EDIT_MASTER, GREEN );
				break;

			case PREVIEW:
				// Same as regular mode, but preview the steps being edited
				show( ELE_EDIT_MASTER, RED );
				break;	

			case MIX2EDIT:
				show( ELE_EDIT_INDICATOR, GREEN );
				show( ELE_EDIT_INDICATOR, RED );
				break;
		}
			
		
		// MUTATORS - when ATR is selected
		if ( target_page->mixTarget == MIXTGT_ATR ){

			// RND
			MIR_write_dot( LED_RANDOMIZE, MIR_GREEN );
			MIR_write_dot( LED_RANDOMIZE, MIR_RED );
											
			// CLR
			MIR_write_dot( LED_CLEAR, MIR_GREEN );
			MIR_write_dot( LED_CLEAR, MIR_RED );

			// CPY
			MIR_write_dot( LED_COPY, MIR_GREEN );
			MIR_write_dot( LED_COPY, MIR_RED );
			
			// PST
			// Make sure we have a valid buffer on store..
			if ( ATTR_MAP_COPY_BUFFER[10] == target_page->mixAttribute ){
			
				MIR_write_dot( LED_PASTE, MIR_GREEN );
				MIR_write_dot( LED_PASTE, MIR_RED );			
			}
		}


		// RETURN
		show (ELE_RETURN, GREEN);


		// GLOBALS
		//
		show (ELE_GLOBALS, TRANSPORT);	
		show (ELE_GLOBALS, NUMERIC_QUADRANT);
		show (ELE_GLOBALS, BIG_KNOB);
		

		// SCALE MASTER
		show_GLOBALS_scale_master( target_page );

		// OCTAVE CIRCLE
		// In PAGE mode show the SCALE, trans-position is in GRID
		if (	( target_page->scaleStatus == SCALE_SEL )
			||	( target_page->scaleStatus == SCALE_MOD ) 
			){
			
			// Show the selected notes in scale. Both in MOD and SEL
			// show (ELE_OCTAVE_CIRCLE, SCALE_SELECTION);
			show_OCTAVE_CIRCLE_scale_selection( target_page );

			// if ( target_page->scaleStatus == SCALE_SEL ){
				// This allows for editing to be done. No need otherwise.
			show_SCALE_SELECTOR_scale_selection( target_page );
			// }

		}

		// When forced to scale, show the scale
		if ( target_page->force_to_scale == ON ){
			// Show the selected notes in scale. Both in MOD and SEL
			// show (ELE_OCTAVE_CIRCLE, SCALE_SELECTION);
			show_OCTAVE_CIRCLE_scale_selection( target_page );
			// show_OCTAVE_CIRCLE_scale_align( target_page );
		}		



