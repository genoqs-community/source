// Key execution snippet for the Octave circle. Can be reused

	if (keyNdx == KEY_NOTE_C) {
	
		if ( (Page_repository[GRID_CURSOR].attribute[ATTR_PITCH] % 12) == 0 ) {
			// Check whether C was already selected so move one octave down
			if (Page_repository[GRID_CURSOR].attribute[ATTR_PITCH] >= 24 ) {
				Page_repository[GRID_CURSOR].attribute[ATTR_PITCH] =
					Page_repository[GRID_CURSOR].attribute[ATTR_PITCH] - 12;			
			}
		}
		else {
			// C was not selected
			Page_repository[GRID_CURSOR].attribute[ATTR_PITCH] =
				(Page_repository[GRID_CURSOR].attribute[ATTR_PITCH] / 12)*12 + 0;
		}

	}
	if (keyNdx == KEY_NOTE_Cis) {
		Page_repository[GRID_CURSOR].attribute[ATTR_PITCH] = 
			normalize ( (Page_repository[GRID_CURSOR].attribute[ATTR_PITCH] / 12)*12 + 1, 0, 109);
	}
	if (keyNdx == KEY_NOTE_D) {
		Page_repository[GRID_CURSOR].attribute[ATTR_PITCH] = 
			normalize ( (Page_repository[GRID_CURSOR].attribute[ATTR_PITCH] / 12)*12 + 2, 0, 110);
	}
	if (keyNdx == KEY_NOTE_Dis) {
		Page_repository[GRID_CURSOR].attribute[ATTR_PITCH] = 
			normalize ( (Page_repository[GRID_CURSOR].attribute[ATTR_PITCH] / 12)*12 + 3, 0, 111);
	}
	if (keyNdx == KEY_NOTE_E) {
		Page_repository[GRID_CURSOR].attribute[ATTR_PITCH] = 
			normalize ( (Page_repository[GRID_CURSOR].attribute[ATTR_PITCH] / 12)*12 + 4, 0, 112);
	}
	if (keyNdx == KEY_NOTE_F) {
		Page_repository[GRID_CURSOR].attribute[ATTR_PITCH] = 
			normalize ( (Page_repository[GRID_CURSOR].attribute[ATTR_PITCH] / 12)*12 + 5, 0, 113);
	}
	if (keyNdx == KEY_NOTE_Fis) {
		Page_repository[GRID_CURSOR].attribute[ATTR_PITCH] = 
			normalize ( (Page_repository[GRID_CURSOR].attribute[ATTR_PITCH] / 12)*12 + 6, 0, 114);
	}
	if (keyNdx == KEY_NOTE_G) {
		Page_repository[GRID_CURSOR].attribute[ATTR_PITCH] = 
			normalize ( (Page_repository[GRID_CURSOR].attribute[ATTR_PITCH] / 12)*12 + 7, 0, 115);
	}
	if (keyNdx == KEY_NOTE_Gis) {
		Page_repository[GRID_CURSOR].attribute[ATTR_PITCH] = 
			normalize ( (Page_repository[GRID_CURSOR].attribute[ATTR_PITCH] / 12)*12 + 8, 0, 116);
	}
	if (keyNdx == KEY_NOTE_A) {
		Page_repository[GRID_CURSOR].attribute[ATTR_PITCH] = 
			normalize ( (Page_repository[GRID_CURSOR].attribute[ATTR_PITCH] / 12)*12 + 9, 0, 117);
	}
	if (keyNdx == KEY_NOTE_Ais) {
		Page_repository[GRID_CURSOR].attribute[ATTR_PITCH] = 
			normalize ( (Page_repository[GRID_CURSOR].attribute[ATTR_PITCH] / 12)*12 + 10, 0, 118);
	}
	if (keyNdx == KEY_NOTE_B) {
		Page_repository[GRID_CURSOR].attribute[ATTR_PITCH] = 
			normalize ( (Page_repository[GRID_CURSOR].attribute[ATTR_PITCH] / 12)*12 + 11, 0, 119);
	}
	if (keyNdx == KEY_NOTE_Cup) {
		// Transpose up by an octave if enough "headroom"
		if ( (Page_repository[GRID_CURSOR].attribute[ATTR_PITCH] + 12) < 119) {
			Page_repository[GRID_CURSOR].attribute[ATTR_PITCH] = 
				normalize ( Page_repository[GRID_CURSOR].attribute[ATTR_PITCH] + 12, 0, 120 );
		}
		// This is the maxixum transpose-up value
		else {
			Page_repository[GRID_CURSOR].attribute[ATTR_PITCH] = 120;
		}
	}
	




