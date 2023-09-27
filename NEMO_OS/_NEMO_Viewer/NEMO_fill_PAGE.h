//
// Copyright (C) 2004, 2005, 2006, 2007 genoQs Machines
// This file is part of Octopus_OS -
// the firmware of the genoQs Machines Octopus, MIDI Control Sequencer
//
// The source code Octopus_OS is available for use in accordance with the
// GNU General Public License (GPL). For more information about Open Source
// licensing, please visit http://www.opensource.org/
//
// Octopus_OS is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation; either version 2 of the License, or
// (at your option) any later version.
//
// Octopus_OS is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with Octopus_OS; if not, write to the Free Software
// Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
//




			// MATRIX - a track selection means we are showing track parameters
			// in all cases but when only one track is selected
// 			if (	( target_page->trackSelection != 1 )
//				){

			if ( target_page->OPS_mode != BIRDSEYE ){

				if (!( 		(( target_page->editorMode == PREVIEW )
						||	( target_page->editorMode == PREVIEW_PERFORM ))
						&& 	is_pressed_steprange()
					)){

					switch( target_page->trackSelection ){

						// No tracks selected
						case 0:
						// Some tracks are selected
						default:
							// The following is the normal (OCT) matrix display
							show ( ELE_MATRIX, STEP_TOGGLE );

							// Show selected steps
							show ( ELE_MATRIX, STEP_SELECTION );

							// Show step skips
							show (ELE_MATRIX, STEP_SKIPS);

							show( ELE_MATRIX, LAUFLICHT );
							break;
					}
				}

				#ifdef FEATURE_STEP_SHIFT
					if ( ( target_page->editorMode == PREVIEW_PERFORM )
							&& ( ShiftHeldStep == 1 ) )  {
						// The following is the normal (OCT) matrix display
						show ( ELE_MATRIX, STEP_TOGGLE );
						// Show selected steps
						show ( ELE_MATRIX, STEP_SELECTION );
						// Show step skips
						show (ELE_MATRIX, STEP_SKIPS);
						// Show Chaselight
						show( ELE_MATRIX, LAUFLICHT );
					}
				#endif
			}

			// If the editor is just being used
			if ( EDIT_TIMER == ON ) {
				show (ELE_MATRIX, EDIT_ATTRIBUTE);
				MIR_write_dot( LED_RETURN, MIR_GREEN );
			}

			// If the MIXER is just being used
			else if ( MIX_TIMER == ON ) {

				show( ELE_MATRIX, EXTENDED_ATTRIBUTE_VALUES );
				MIR_write_dot( LED_RETURN, MIR_GREEN );
			}


			// RETURN
			if (
					( target_page->stepSelection  	!= 0 )
				|| 	( target_page->scaleStatus 		!= OFF )
				){

				show (ELE_RETURN, GREEN);
			}


			// ALIGN - Indicate that ALIGN is available
//			MIR_write_dot( LED_ALIGN, MIR_GREEN );

			// In track mode this is orange
			if ( target_page->trackSelection != 0 ){

				MIR_write_dot( LED_ALIGN, MIR_RED 	);
				MIR_write_dot( LED_ALIGN, MIR_BLINK );
			}


			// PLAY mode
			switch( PLAY_MODE_STATUS ){

				case ON:
					MIR_write_dot( LED_ZOOM_PLAY, MIR_RED );
					MIR_write_dot( LED_ZOOM_PLAY, MIR_GREEN );
					MIR_write_dot( LED_ZOOM_PLAY, MIR_BLINK );
					break;

				case OFF:
					MIR_write_dot( LED_ZOOM_PLAY, MIR_GREEN );
					break;
			}

