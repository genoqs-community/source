
	// toggle the bits in the DIAG array for diag purposes

	// Forget about rotarys
	DIAG_rotary_current = 200;
	
	// Toggle the key color
	DIAG[ keyNdx ] = 
		( DIAG[ keyNdx ] +1 ) % 4;
	



