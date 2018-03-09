

void rot_exe_STRUM( 	Pagestruct* target_page,
						unsigned char rotNdx,
						unsigned char direction ){

	unsigned char row = 0;
	unsigned char col = 0;
	unsigned char temp = 0;

	//
	// STRUM level regulation
	//	

	switch( rotNdx ){
	
		// BIG KNOB
		case 0:
			// Only one step is selected
			if ( VIEWER_page->stepSelection == 1 ){
			
				// Enter the step strum level
				row = VIEWER_page->stepSelectionSingleRow;
				col = VIEWER_page->stepSelectionSingleCol;
	
				temp = ( VIEWER_page->Step[row][col]->chord_data & 0xF800 ) >> 11;
				modify_parameter( &temp, 0, 18, direction, OFF, FIXED);
	
				VIEWER_page->Step[row][col]->chord_data = ( temp << 11 )
					| ( VIEWER_page->Step[row][col]->chord_data & 0x7FF );
			}
			break;
			
		// MIXER
		case 11: case 12: case 13: case 14: case 15: 
		case 16: case 17: case 18: case 19: case 20:

			switch( OCT_strum_attribute ){
	
				case ATTR_VELOCITY:
					modify_signed_parameter 
						(&OCT_step_strum_VEL	[OCT_strum_view_level][ 9-(rotNdx-11)], 	
							STEP_MIN_VELOCITY, STEP_MAX_VELOCITY, direction, ON, FIXED); 
					break;
				case ATTR_PITCH:
					modify_signed_parameter 
						(&OCT_step_strum_PIT	[OCT_strum_view_level][ 9-(rotNdx-11)], 	
							STEP_MIN_PITCH, STEP_MAX_PITCH, direction, ON, FIXED); 
					break;
				case ATTR_LENGTH:
					modify_signed_parameter 
						(&OCT_step_strum_LEN	[OCT_strum_view_level][ 9-(rotNdx-11)], 	
							-40, 40, direction, ON, FIXED); 
					break;
				case ATTR_START:
					modify_parameter 
						(&OCT_step_strum_STA	[OCT_strum_view_level][ 9-(rotNdx-11)], 	
							0, 127, direction, ON, FIXED); 
					break;
			}
			break;
	}

}



