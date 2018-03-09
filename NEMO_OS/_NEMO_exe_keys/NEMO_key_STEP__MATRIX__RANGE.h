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



		// /////
		// ROW I - value entry, depending on selected attribute
		// 
		if ( (( keyNdx - NEMO_ROW_I ) % 11) == 0 ){
			
			// Target coordinates
			row = target_page->stepSelectionSingleRow;
			col = (( keyNdx - NEMO_ROW_I ) / 11) - 1 ;

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

			// Behave according to selected attribute
			switch( i ){
	
				case NEMO_ATTR_PITCH:
					step_enter_range( 
						&(target_page->Track[row]->event_max[NEMO_ATTR_PITCH]), 
						TRACK_MAX_RANGE_PIT, TRACK_DEF_RANGE_PIT, keyNdx, row, col );
					break;
			
				case NEMO_ATTR_VELOCITY:
					step_enter_range( 
						&(target_page->Track[row]->event_max[NEMO_ATTR_VELOCITY]), 
						TRACK_MAX_RANGE_VEL, TRACK_DEF_RANGE_VEL, keyNdx, row, col );
					break;
	
				case NEMO_ATTR_LENGTH:
					step_enter_range( 
						&(target_page->Track[row]->event_max[NEMO_ATTR_LENGTH]), 
						TRACK_MAX_RANGE_LEN, TRACK_DEF_RANGE_LEN, keyNdx, row, col );
					break;
				
				case NEMO_ATTR_START:
					step_enter_range( 
						&(target_page->Track[row]->event_max[NEMO_ATTR_START]), 
						TRACK_MAX_RANGE_STA, TRACK_DEF_RANGE_STA, keyNdx, row, col );
					break;
	
				case NEMO_ATTR_AMOUNT:
					step_enter_range( 
						&(target_page->Track[row]->event_max[NEMO_ATTR_AMOUNT]), 
						TRACK_MAX_RANGE_AMT, TRACK_DEF_RANGE_AMT, keyNdx, row, col );
					break;
	
				case NEMO_ATTR_GROOVE:
					step_enter_range( 
						&(target_page->Track[row]->event_max[NEMO_ATTR_GROOVE]), 
						TRACK_MAX_RANGE_GRV, TRACK_DEF_RANGE_GRV, keyNdx, row, col );
					break;

				case NEMO_ATTR_MIDICC:
//					step_enter_range( 
//						&(target_page->Track[row]->event_max[NEMO_ATTR_MIDICC]), 
//						TRACK_MAX_RANGE_MCC, TRACK_DEF_RANGE_MCC, keyNdx, row, col );
					step_enter_range( 
						&(target_page->Track[row]->event_max[NEMO_ATTR_MIDICC]), 
						TRACK_MAX_RANGE_MCC, TRACK_DEF_RANGE_MCC, keyNdx, row, col );
					break;
	
				case NEMO_ATTR_MIDICH:
					// avoid the negative marker here - not relevant.
					if ( col == 15 ) col = 14;
					step_enter_value_PITVELAMT( 
						&(target_page->Track[row]->event_max[NEMO_ATTR_MIDICH]), 
						TRACK_MAX_RANGE_MCH, TRACK_DEF_RANGE_MCH, keyNdx, row, col );
					break;

				case NEMO_ATTR_POSITION:
					step_enter_range( 
						&(target_page->Track[row]->event_max[NEMO_ATTR_POSITION]), 
						TRACK_MAX_RANGE_POS, TRACK_DEF_RANGE_POS, keyNdx, row, col );
					break;

				case NEMO_ATTR_DIRECTION:
					step_enter_range( 
						&(target_page->Track[row]->event_max[NEMO_ATTR_DIRECTION]), 
						TRACK_MAX_RANGE_DIR, TRACK_DEF_RANGE_DIR, keyNdx, row, col );
					break;

			} // switch( NEMO_selectedTrackAttribute )
		} // ROW I Key





		// //////
		// ROW II selects the track attribute as well as some others..
		//
		if ( (( keyNdx - NEMO_ROW_II ) % 11) == 0 ){

			// Target coordinates
			row = target_page->stepSelectionSingleRow;
			col = (( keyNdx - NEMO_ROW_II ) / 11) - 1 ;

			// Select the step event attribute
			// Initialize the event_offsets
			target_page->Track[ target_page->stepSelectionSingleRow ]->event_offset[NEMO_ATTR_VELOCITY] 	= 0;
			target_page->Track[ target_page->stepSelectionSingleRow ]->event_offset[NEMO_ATTR_PITCH] 	= 0;
			target_page->Track[ target_page->stepSelectionSingleRow ]->event_offset[NEMO_ATTR_LENGTH] 	= 0;
			target_page->Track[ target_page->stepSelectionSingleRow ]->event_offset[NEMO_ATTR_START] 	= 0;
			target_page->Track[ target_page->stepSelectionSingleRow ]->event_offset[NEMO_ATTR_DIRECTION] = 0;
			target_page->Track[ target_page->stepSelectionSingleRow ]->event_offset[NEMO_ATTR_AMOUNT] 	= 0;
			target_page->Track[ target_page->stepSelectionSingleRow ]->event_offset[NEMO_ATTR_GROOVE] 	= 0;
			target_page->Track[ target_page->stepSelectionSingleRow ]->event_offset[NEMO_ATTR_MIDICC] 	= 0;
			target_page->Track[ target_page->stepSelectionSingleRow ]->event_offset[NEMO_ATTR_MIDICH] 	= 0;
	
			// Now set the event status..
			if (target_page->stepSelection == 1){
				
				// Transform the keypress into to the Octopus model
				temp = ( keyNdx / 11 ) - 1;
				switch( temp ){
					case 0:		temp = 1;	break;	// PIT
					case 1:		temp = 0;	break;	// VEL
					case 2:		temp = 2;	break;	// LEN
					case 3:		temp = 3;	break;	// STA
					case 5:		temp = 6;	break;	// AMT
					case 6:		temp = 7;	break;	// GRV
					case 7:		temp = 8;	break;	// MCC
					case 9:		temp = 9;	break;	// MCH
					case 11:	temp = 4;	break;	// POS
					case 12:	temp = 5;	break;	// DIR
					default:	temp = 20;	break;	// not a valid key press
				}

				// Do not continue if the pressed key was not valid
				if ( temp == 20 ) break;

				// Click on an active event - turn the event OFF
				if (	( ( temp ) == 
					 	  (target_page->Step[ target_page->stepSelectionSingleRow ]
							  		        [ target_page->stepSelectionSingleCol ]->event_data & 0x0F) )
					&&	( Step_get_status( target_page->Step[ target_page->stepSelectionSingleRow ]
									  				        [ target_page->stepSelectionSingleCol ],
									 	   STEPSTAT_EVENT ) == ON )
					&& 	( NEMO_step_VER != VER_RANGE )
					){
	
					Step_set_status( target_page->Step[ target_page->stepSelectionSingleRow ]
									  				  [ target_page->stepSelectionSingleCol ],
									 STEPSTAT_EVENT, 
									 OFF );
				}
				else{
					
					// Remember the pressed attribute..
					target_page->Step[ target_page->stepSelectionSingleRow ]
							  		 [ target_page->stepSelectionSingleCol ]->event_data
						= 	(target_page->Step[ target_page->stepSelectionSingleRow ]
							  		   	 	  [ target_page->stepSelectionSingleCol ]->event_data & 0xF0) 
							| ( temp );

					// ..and turn the event status bit ON
					Step_set_status( target_page->Step[ target_page->stepSelectionSingleRow ]
									  				  [ target_page->stepSelectionSingleCol ],
									 STEPSTAT_EVENT, 
									 ON );
				}
			}
		} // ROW II Key
	


		// ////////////
		// NEMO_ROW_III - the value specifiers: Value, Event, Range
		//
		if ( (( keyNdx - NEMO_ROW_III ) % 11) == 0) {

			// Target coordinates
			row = target_page->stepSelectionSingleRow;
			col = (( keyNdx - NEMO_ROW_III ) / 11) - 1 ;

			switch( col ){

				// VER Selection
				case 7:	
					NEMO_step_VER = VER_VALUE;
					break;					

				case 11:
					NEMO_step_VER = VER_EVENT;
					break;
			}			
		} // ROW III Key

