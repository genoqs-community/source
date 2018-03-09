

void rot_exe_DIAG( 	Pagestruct* target_page, 
					unsigned char rotNdx,
					unsigned char direction ){

	// ROTARY ACTION in DIAG MODE
	
	// Modify the diag value of the rotary
	DIAG_rotary_current = rotNdx;

	modify_parameter( 	&DIAG_rotary_value[ DIAG_rotary_current ], 
						0, 
						255,
						direction, OFF, FIXED);
	
}

