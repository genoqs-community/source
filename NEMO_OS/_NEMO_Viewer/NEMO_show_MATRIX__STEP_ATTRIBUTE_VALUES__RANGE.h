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
		
			// Shows the range value, which is relevant for the event attribute!
			// Fetch the event pointer - in Octopus format
			temp = target_page->Step [target_page->stepSelectionSingleRow]
							  		 [target_page->stepSelectionSingleCol]->event_data & 0x0F;
			// The data is in Octopus format..
			switch( temp ){	
				case 0: 	// VEL
					i = NEMO_ATTR_VELOCITY;
					break;
				case 1:		// PIT
					i = NEMO_ATTR_PITCH;
					break;
				case 2:		// LEN
					i = NEMO_ATTR_LENGTH;
					break;
				case 3:		// STA
					i = NEMO_ATTR_START;
					break;
				case 4:		// POS 	
					i = NEMO_ATTR_POSITION;
					break;
				case 5:		// DIR
					i = NEMO_ATTR_DIRECTION;
					break;
				case 6:		// AMT
					i = NEMO_ATTR_AMOUNT;
					break;
				case 7:		// GRV
					i = NEMO_ATTR_GROOVE;
					break;
				case 8:		// MCC
					i = NEMO_ATTR_MIDICC;
					break;
				case 9:		// MCH
					i = NEMO_ATTR_MIDICH;
					break;
			}
			// Write the actual value out
			if ( i == NEMO_ATTR_MIDICH ){
				// The MIDICH range is a natural number
				MIR_write_numeric_H( target_page->Track[row]->event_max[i],	NEMO_ROW_I );
			}
			else{
				// All other ranges are 1-16 as factors of the maps are 1-16
				MIR_fill_numeric( 1, target_page->Track[row]->event_max[i],	NEMO_ROW_I,	MIR_RED		);
				MIR_fill_numeric( 1, target_page->Track[row]->event_max[i],	NEMO_ROW_I,	MIR_GREEN	);
			}
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

				// The data is in Octopus format, so transform to NEMO:
				switch( temp ){

					case 0: 	temp = 1;	break;	// VEL
					case 1: 	temp = 0;	break;	// PIT
					case 2: 	temp = 2;	break;	// LEN
					case 3: 	temp = 3;	break;	// STA
					case 4: 	temp = 11;	break;	// POS
					case 5: 	temp = 12;	break;	// DIR
					case 6: 	temp = 5;	break;	// AMT
					case 7: 	temp = 6;	break;	// GRV
					case 8: 	temp = 7;	break;	// MCC
					case 9: 	temp = 9;	break;	// MCH
				}

				// Finally point out the event in the row
				MIR_augment_trackpattern( 1<<(15-temp), NEMO_ROW_II, MIR_RED   );
				MIR_augment_trackpattern( 1<<(15-temp), NEMO_ROW_II, MIR_BLINK );
   			}
		}

		
		// ROW III
		switch( NEMO_selectedStepAttribute ){
			
			case NEMO_ATTR_PITCH:
			case NEMO_ATTR_VELOCITY:
			case NEMO_ATTR_LENGTH:
			case NEMO_ATTR_START:
			
			case NEMO_ATTR_AMOUNT:
			case NEMO_ATTR_GROOVE:
			case NEMO_ATTR_MIDICC:
				MIR_write_trackpattern( 0x0118, 	NEMO_ROW_III, MIR_GREEN );
				MIR_write_dot( LED_VER_RANGE, MIR_RED );
				break;

			default:
				MIR_write_trackpattern( 0x3100, 	NEMO_ROW_III, MIR_GREEN );
				break;
		}

		
		


