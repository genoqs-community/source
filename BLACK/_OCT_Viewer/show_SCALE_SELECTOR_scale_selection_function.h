

void show_SCALE_SELECTOR_scale_selection( Pagestruct* target_page ){

	unsigned int j = my_shift_bitpattern( 	target_page->scaleNotes[0], 12, INC,
								my_bit2ndx( target_page->scaleLead[0] )+1);

	// Light all green because we can always use them
	MIR_write_dot( LED_SCALE_MYSEL,	MIR_GREEN	);
	MIR_write_dot( LED_SCALE_PEN, 	MIR_GREEN 	);
	MIR_write_dot( LED_SCALE_WHL, 	MIR_GREEN 	);
	MIR_write_dot( LED_SCALE_MAJ, 	MIR_GREEN 	);
	MIR_write_dot( LED_SCALE_MIN, 	MIR_GREEN	);
	MIR_write_dot( LED_SCALE_DIM, 	MIR_GREEN	);
	MIR_write_dot( LED_SCALE_CHR,	MIR_GREEN	);
	
	switch( j ) {

		case SCALE_SIG_PEN:
			MIR_write_dot( LED_SCALE_PEN,	MIR_RED	);
//			MIR_write_dot( LED_SCALE_PEN,	MIR_BLINK	);
			break;

		case SCALE_SIG_WHL:
			MIR_write_dot( LED_SCALE_WHL,	MIR_RED	);
//			MIR_write_dot( LED_SCALE_WHL,	MIR_BLINK	);
			break;

		case SCALE_SIG_MAJ:
			MIR_write_dot( LED_SCALE_MAJ,	MIR_RED	);
//			MIR_write_dot( LED_SCALE_MAJ,	MIR_BLINK	);
			break;

		case SCALE_SIG_MIN:
			MIR_write_dot( LED_SCALE_MIN,	MIR_RED	);
//			MIR_write_dot( LED_SCALE_MIN,	MIR_BLINK	);
			break;

		case SCALE_SIG_DIM:
			MIR_write_dot( LED_SCALE_DIM,	MIR_RED	);
//			MIR_write_dot( LED_SCALE_DIM,	MIR_BLINK	);
			break;

		case SCALE_SIG_CHR:
			MIR_write_dot( LED_SCALE_CHR,	MIR_RED	);
//			MIR_write_dot( LED_SCALE_CHR,	MIR_BLINK	);
			break;

		default:
			MIR_write_dot( LED_SCALE_MYSEL,	MIR_RED	);
//			MIR_write_dot( LED_SCALE_MYSEL,	MIR_BLINK	);
			break;
	}

}


