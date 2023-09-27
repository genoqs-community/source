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


		// ROW I
		if ( MODE_OBJECT_SELECTION != BIRDSEYE ){

			MIR_write_numeric_H(
				target_page->Step 	[target_page->stepSelectionSingleRow]
						  		 	[target_page->stepSelectionSingleCol]->attr_AMT,	NEMO_ROW_I );
		}


		// ROW II
		// --> rather :: Show the available and the selected step event type
		if ( MODE_OBJECT_SELECTION != BIRDSEYE ){

			// Show available event attributes
			// All attributes are available
			MIR_write_trackpattern( 0xF758, NEMO_ROW_II, MIR_GREEN );

			// Show currently selected event attribute
			if ( Step_get_status( target_page->Step[ target_page->stepSelectionSingleRow ]
								  				   [ target_page->stepSelectionSingleCol ],
   								  STEPSTAT_EVENT ) == ON 
   				){
						
				// Fetch the event pointer - in Octopus format
				temp = target_page->Step [target_page->stepSelectionSingleRow]
								  		 [target_page->stepSelectionSingleCol]->event_data & 0x0F;
				unsigned char event_data_attr = temp + 1;

				// The data is in Octopus format, so transform to NEMO:
				switch( temp ){
					case 0: 	temp = 0;	break;	// PIT
					case 1: 	temp = 1;	break;	// VEL
					case 2: 	temp = 2;	break;	// LEN
					case 3: 	temp = 3;	break;	// STA
					case 4: 	temp = 11;	break;	// POS
					case 5: 	temp = 12;	break;	// DIR
					case 6: 	temp = 5;	break;	// AMT
					case 7: 	temp = 6;	break;	// GRV
					case 8: 	temp = 7;	break;	// MCC
					case 9: 	temp = 9;	break;	// MCH
					default:	temp = 20;	break;
				}

				if( temp != 20 ) {
					#ifdef FEATURE_STEP_EVENT_TRACKS
					// Finally point out the event in the row
					if( 	( ( event_data_attr == NEMO_ATTR_POSITION ) || ( event_data_attr == NEMO_ATTR_DIRECTION ) )
									&& 	( CHECK_BIT( target_page->Step[ target_page->stepSelectionSingleRow ]
						   	                              [ target_page->stepSelectionSingleCol ]->attr_STATUS, STEP_EVENT_TRACK_ALT_MODE + 5 ) ) ) {
						// POS OR DIR ALT MODE
						MIR_write_trackpattern( 1<<(15-temp), NEMO_ROW_II, MIR_SHINE_GREEN   );
					} else {
						MIR_augment_trackpattern( 1<<(15-temp), NEMO_ROW_II, MIR_RED   );
						MIR_augment_trackpattern( 1<<(15-temp), NEMO_ROW_II, MIR_BLINK );
					}
					#else
					// Finally point out the event in the row
					MIR_augment_trackpattern( 1<<(15-temp), NEMO_ROW_II, MIR_RED   );
					MIR_augment_trackpattern( 1<<(15-temp), NEMO_ROW_II, MIR_BLINK );
					#endif
				}
   			}
		}
		// Only flow attributes available
		MIR_write_trackpattern( 0xF758, NEMO_ROW_II, MIR_GREEN );
		// Indicate VFF status
		MIR_write_dot( LED_VER_EVENT, MIR_RED );



		// ROW III
		switch( NEMO_selectedStepAttribute ){
			
			case NEMO_ATTR_PITCH:
			case NEMO_ATTR_VELOCITY:
			case NEMO_ATTR_LENGTH:
			case NEMO_ATTR_START:
			
			case NEMO_ATTR_AMOUNT:
			case NEMO_ATTR_GROOVE:
			case NEMO_ATTR_MIDICC:

				// If an event is set, show also Range
				if ( Step_get_status( target_page->Step[ target_page->stepSelectionSingleRow ]
									  				   [ target_page->stepSelectionSingleCol ],
									 	   STEPSTAT_EVENT ) == ON ){
					MIR_write_trackpattern( 0x0118, 	NEMO_ROW_III, MIR_GREEN );							
				} 
				else{
					MIR_write_trackpattern( 0x0110, 	NEMO_ROW_III, MIR_GREEN );						
				}
		}		

		// Jump to Value is available
		MIR_write_dot( LED_VER_VALUE, MIR_GREEN );

		// Event marked as selected
		MIR_write_dot( LED_VER_EVENT, MIR_RED );

