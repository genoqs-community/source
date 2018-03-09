

		switch( MODE_OBJECT_SELECTION ){
			
			case BIRDSEYE: 
				// If the zoom key is not pressed anymore, return to default
				if (	!(is_pressed_key( KEY_ZOOM_TRACK ))
					){

					// Enter interactive mode
					MODE_OBJECT_SELECTION = INTERACTIVE;
				}
				else{
					// Nothing to do as of yet..
				}
				break;


			default:
				// Check for pressed track_selectors - which are really MAP selectors here
				my_pressed_key = is_pressed_keyrange( 1, 10 );

				// If a map is held down selected
				switch( my_pressed_key ){
				
					case 0:
					
						VIEWER_page->trackAttributeSelection = 0;
						MODE_OBJECT_SELECTION = INTERACTIVE;
						break;
		
					case 1: case 2: case 3: case 4: case 6:	case 8: case 9:
		
						MODE_OBJECT_SELECTION = PASSIVE;
						break;				
				}

				break;
		}


		if ( (is_pressed_key( KEY_ZOOM_TRACK )) 
			){

			// Enter the Birds Eye view
			MODE_OBJECT_SELECTION = BIRDSEYE;
		}

		



