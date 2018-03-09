

		switch( MODE_OBJECT_SELECTION ){
			
			case BIRDSEYE: 
				// If the zoom key is not pressed anymore, return to default
				if (	!(is_pressed_key( KEY_ZOOM_MAP ))
					){

					// Enter interactive mode
					MODE_OBJECT_SELECTION = INTERACTIVE;
				}
				else{
					// Nothing to do as of yet..
				}
				break;

			case EMBEDDED:
				if ( is_pressed_rowzero() == 0 ){
					
					MODE_OBJECT_SELECTION = INTERACTIVE;
				}
		}

		if ( (is_pressed_key( KEY_ZOOM_MAP )) 
			){

			// Enter the Birds Eye view
			MODE_OBJECT_SELECTION = BIRDSEYE;
		}
		else if ( is_pressed_rowzero() != 0 ){

			// CIRCLE OF PITCHES
			// If a step is pressed, show its absolute pitch in the inner circle
			MODE_OBJECT_SELECTION = EMBEDDED;
			// d_iag_printf( "embedded\n" );
		}





