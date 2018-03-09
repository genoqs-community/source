

		switch( MODE_OBJECT_SELECTION ){
			
			case BIRDSEYE: 
				// If the zoom key is not pressed anymore, return to default
				if (	!(is_pressed_key( KEY_ZOOM_STEP ))
					){

					// Enter interactive mode
					MODE_OBJECT_SELECTION = INTERACTIVE;
				}
				else{
					// Nothing to do as of yet..
				}
				break;

			case CHORDEYE:
				// If no more chord button pressed anymore, return to default
				if ( is_pressed_keyrange( 252, 258 ) == 0
					){

					// Enter interactive mode
					MODE_OBJECT_SELECTION = INTERACTIVE;
				}

				break;
		}

		// STEP Mode button pressed
		if ( (is_pressed_key( KEY_ZOOM_STEP )) 
			){

			// Enter the Birds Eye view
			MODE_OBJECT_SELECTION = BIRDSEYE;
		}
		// Some chord button is pressed
		else if ( is_pressed_keyrange( 252, 258 ) != 0 
			){

			// Enter the CHORDEYE view
			MODE_OBJECT_SELECTION = CHORDEYE;					
		}



