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


		// Display for each step the value of selected attribute

		// Get row of the selected track
		row = my_bit2ndx( target_page->trackSelection );

		// Get the selected attribute
		attribute = my_bit2ndx( target_page->trackAttributeSelection );

		// Display type depends on the attribute itself
		switch( attribute ){



			case VELOCITY:
				// Interval mapping taking place - centerline is 4, range -49 +49
				MIR_write_numeric_V_zeroline_VEL( 	4, 
												//	-49, 49, 
													row, attribute );

				// Show the VEL_factor of the current track
				MIR_write_factor_C( 
						target_page->Track[row]->VEL_factor 
						#ifdef EVENTS_FACTORIZED						
						+ target_page->Track[row]->event_offset[ATTR_VELOCITY] 
						#endif // EVENTS_FACTORIZED
					);

				// Show the active Big Knob - choosing certainty_next
				MIR_write_dot( LED_SCALE_MYSEL, MIR_GREEN );
				MIR_write_dot( LED_SCALE_MYSEL, MIR_RED );					
				break;



			case PITCH:
				// Interval mapping taking place - centerline is 4, range -49 +49
				MIR_write_numeric_V_zeroline_PIT( 	4, 
												//	-49, 49, 
													row );

				// Show the PIT_factor of the current track
				MIR_write_factor_C( 
						target_page->Track[row]->PIT_factor 
						#ifdef EVENTS_FACTORIZED
						+ target_page->Track[row]->event_offset[ATTR_PITCH] 
						#endif // EVENTS_FACTORIZED
					);

				// Show the active Big Knob - choosing certainty_next
				MIR_write_dot( LED_SCALE_MYSEL, MIR_GREEN );
				MIR_write_dot( LED_SCALE_MYSEL, MIR_RED );					
				break;



			case LENGTH:
				// The length has another special display mode. It also approximates
				// ..but under different conditions.
				MIR_show_lengthbar_V_LEN( row );

				// Show the LEN_factor of the current track
				MIR_write_factor_C( 
					target_page->Track[row]->LEN_factor 
					+ target_page->Track[row]->event_offset[ATTR_LENGTH] );

				// Show the active Big Knob - choosing certainty_next
				MIR_write_dot( LED_SCALE_MYSEL, MIR_GREEN );
				MIR_write_dot( LED_SCALE_MYSEL, MIR_RED );					
				break;



			case START: 
				// The start has a special display mode
				MIR_show_startbar_V( row );

				// Show the STA_factor of the current track
				MIR_write_factor_C( 
					target_page->Track[row]->STA_factor 
					+ target_page->Track[row]->event_offset[ATTR_START] );

				// Show the active Big Knob - choosing certainty_next
				MIR_write_dot( LED_SCALE_MYSEL, MIR_GREEN );
				MIR_write_dot( LED_SCALE_MYSEL, MIR_RED );					
				break;



			case DIRECTION:
				// This is the direction editing mode
				// Show the currently selected direction
				MIR_point_numeric( 
					target_page->Track[row]->attr_DIR, 9, MIR_RED );
				MIR_point_numeric( 
					target_page->Track[row]->attr_DIR, 9, MIR_GREEN );

				// Show the currently selected trigger set
				MIR_point_numeric( 
					current_trigger_set, 9, MIR_GREEN );
				MIR_point_numeric( 
					current_trigger_set, 9, MIR_BLINK );
					
				// Show the current triggers in the selected trigger set
				for (i=0; i < MATRIX_NROF_ROWS; i++){
					
					MIR_point_numeric( 
						direction_repository[ target_page->Track[row]->attr_DIR]
						.trigger_set[ current_trigger_set ].trigger[i], 
						MATRIX_NROF_ROWS - (i+1), MIR_RED );
				}
				
				// Show the "certainty next" parameter of the selected trigger set
				MIR_write_numeric_C( 
						direction_repository[ target_page->Track[row]->attr_DIR]
						.trigger_set[ current_trigger_set ]
						.certainty_next );

				// Show the active Big Knob - choosing certainty_next
				MIR_write_dot( LED_SCALE_MYSEL, MIR_GREEN );
				MIR_write_dot( LED_SCALE_MYSEL, MIR_RED );					
				break;



			case AMOUNT:
				// Interval mapping taking place - centerline is 4, range -49 +49
				MIR_write_numeric_V_zeroline_VEL( 	4, 
												// -49, 49, 
													row, attribute );

				// Show the VEL_factor of the current track
				MIR_write_factor_C( 
					target_page->Track[row]->AMT_factor 
					+ target_page->Track[row]->event_offset[ATTR_AMOUNT] );

				// Show the active Big Knob - choosing certainty_next
				MIR_write_dot( LED_SCALE_MYSEL, MIR_GREEN );
				MIR_write_dot( LED_SCALE_MYSEL, MIR_RED );					
				break;



			case GROOVE:
#if GROEF || 1
				// Nothing special here - the usual
				MIR_show_groovebar_V( row );
#endif
				
				// Show the GRV_factor of the current track
				MIR_write_factor_C( 
					target_page->Track[row]->GRV_factor 
					+ target_page->Track[row]->event_offset[ATTR_GROOVE] );

				// Show the active Big Knob - choosing certainty_next
					MIR_write_dot( LED_SCALE_MYSEL, MIR_GREEN );
					MIR_write_dot( LED_SCALE_MYSEL, MIR_RED );					
					break;

 

			case MIDICC:
					// Choose the most convenience display method
				switch( target_page->Track[row]->attr_MCC ){

					case MIDICC_BENDER:
						MIR_write_numeric_V_zeroline_MCC_BENDER( 	4, 
																// -63, 64, 
																	row );
						break;

					default:
						// Interval mapping taking place 
						MIR_write_numeric_V_MCC( target_page, row );		
						break;
				}

				// Show the MCC_factor of the current track
				MIR_write_factor_C( 
					target_page->Track[row]->MCC_factor 
					+ target_page->Track[row]->event_offset[ATTR_MIDICC] );

				// Show the active Big Knob - factor selector
				MIR_write_dot( LED_SCALE_MYSEL, MIR_GREEN );
				MIR_write_dot( LED_SCALE_MYSEL, MIR_RED );
				break;	
		}




