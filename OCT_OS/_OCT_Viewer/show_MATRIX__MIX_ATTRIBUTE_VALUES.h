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




	// Display the value in each row, respecting the corresponding ranges					
	// Does not take into account the event offset values
	switch (target_page->mixAttribute) {



		// VEL
		case ATTR_VELOCITY:
			for (row=0; row<MATRIX_NROF_ROWS; row++) {

				// Head play mode of the chain
				if 	(	( target_page->CHAINS_PLAY_HEAD == TRUE )
					// Show the head tracks only
					&& 	( target_page->Track[row]->chain_data[HEAD] != target_page->Track[row] )
					){

					continue;
				}
	
				// Display all track values
				MIR_write_numeric_H 
					(	target_page->Track[row]->attr_VEL, row	);
			}
			break;



		// PIT
		case ATTR_PITCH:
			for (row=0; row < MATRIX_NROF_ROWS; row++) {

				// Head play mode of the chain
				if 	(	( target_page->CHAINS_PLAY_HEAD == TRUE )
					// Show the head tracks only
					&& 	( target_page->Track[row]->chain_data[HEAD] != target_page->Track[row] )
					){

					continue;
				}
				
				// Show the total pitch value, although change is done only to the track pitch
					j = target_page->Track[row]->attr_PIT
						+ target_page->Track[row]->scale_pitch_offset
						+ target_page->Track[row]->lead_pitch_offset
						#ifdef EVENTS_ABSOLUTE
						+ target_page->Track[row]->event_offset[ATTR_PITCH]
						#endif // EVENTS_ABSOLUTE
					;
				
				// Display it in the note representation
				MIR_write_pitch_H( normalize ( j, TRACK_MIN_PITCH, TRACK_MAX_PITCH ), row );
			}
			break;
		


		// LEN
		case ATTR_LENGTH:
			for (row=0; row < MATRIX_NROF_ROWS; row++) {

				// Head play mode of the chain
				if 	(	( target_page->CHAINS_PLAY_HEAD == TRUE )
					// Show the head tracks only
					&& 	( target_page->Track[row]->chain_data[HEAD] != target_page->Track[row] )
					){

					continue;
				}
				
				// Display all values
				MIR_fill_numeric( 1, 	target_page->Track[row]->LEN_factor +1, row, MIR_RED );
				MIR_fill_numeric( 1, 	target_page->Track[row]->LEN_factor +1, row, MIR_GREEN );
			}
			break;



		// STA
		case ATTR_START:
			for (row=0; row < MATRIX_NROF_ROWS; row++) {

				// Head play mode of the chain
				if 	(	( target_page->CHAINS_PLAY_HEAD == TRUE )
					// Show the head tracks only
					&& 	( target_page->Track[row]->chain_data[HEAD] != target_page->Track[row] )
					){

					continue;
				}
				
				MIR_fill_numeric( 1, 	target_page->Track[row]->STA_factor +1, row, MIR_RED );
				MIR_fill_numeric( 1, 	target_page->Track[row]->STA_factor +1, row, MIR_GREEN );
			}					
			break;
			
			

		// POS
		case ATTR_POSITION:
			show( ELE_MATRIX, STEP_TOGGLE );
			break;
		
		
		
		// DIR
		case ATTR_DIRECTION:
			for (row=0; row<MATRIX_NROF_ROWS; row++) {

				// Showing all directions since chaining does not apply and head value does not carry
				MIR_point_numeric 
					(	
//						target_page->Track[row]->attribute[target_page->mixAttribute] 
						*(ptr_of_track_attr_value( target_page, row, target_page->mixAttribute ))
						+ target_page->Track[row]->event_offset[ATTR_DIRECTION], row, MIR_RED
					);

				MIR_point_numeric 
					(	
//						target_page->Track[row]->attribute[target_page->mixAttribute] 
						*(ptr_of_track_attr_value( target_page, row, target_page->mixAttribute ))
						+ target_page->Track[row]->event_offset[ATTR_DIRECTION], row, MIR_GREEN
					);
			}
			break;



		// AMT
		case ATTR_AMOUNT:
			for (row=0; row<MATRIX_NROF_ROWS; row++) {
				
				// Head play mode of the chain
				if 	(	( target_page->CHAINS_PLAY_HEAD == TRUE )
					// Show the head tracks only
					&& 	( target_page->Track[row]->chain_data[HEAD] != target_page->Track[row] )
					){

					continue;
				}

				// Display all values
				MIR_write_numeric_H 
					(	target_page->Track[row]->attr_AMT 
						+ target_page->Track[row]->event_offset[ATTR_AMOUNT], row);
			}
			break;
		


		// GRV
		case ATTR_GROOVE:
			for (row=0; row<MATRIX_NROF_ROWS; row++) {

				// Head play mode of the chain
				if 	(	( target_page->CHAINS_PLAY_HEAD == TRUE )
					// Show the head tracks only
					&& 	( target_page->Track[row]->chain_data[HEAD] != target_page->Track[row] )
					){

					continue;
				}
				
				// Show all values
				MIR_fill_numeric (	1, 	
								target_page->Track[row]->attr_GRV +1,
								row, MIR_RED);
				MIR_fill_numeric (	1, 
								target_page->Track[row]->attr_GRV +1, 
								row, MIR_GREEN);
			}
			break;
		


		// MCC
		case ATTR_MIDICC:
			for( row=0; row<MATRIX_NROF_ROWS; row++ ){
						
				// Head play mode of the chain
				if 	(	( target_page->CHAINS_PLAY_HEAD == TRUE )
					// Show the head tracks only
					&& 	( target_page->Track[row]->chain_data[HEAD] != target_page->Track[row] )
					){

					continue;
				}

				// Show the MIDI_CC value
				switch( target_page->Track[row]->attr_MCC ){
				
					case MIDICC_NONE:

						MIR_write_trackpattern ( 0x0f, row, MIR_GREEN);
						break;

					case MIDICC_BENDER:

						MIR_write_trackpattern ( 0x01, row, MIR_RED);	
						break;	

					case MIDICC_PRESSURE:
						MIR_write_trackpattern ( 0x03, row, MIR_RED);	
						break;

					default:
						MIR_write_numeric_H( target_page->Track[row]->attr_MCC, row);
						break;
						
				} // MIDI_CC value switch
			} // Row iterator
			break;



		// MCH
		case ATTR_MIDICH: 
			for (row=0; row < MATRIX_NROF_ROWS; row++) {
				
				// Head play mode of the chain
				if 	(	( target_page->CHAINS_PLAY_HEAD == TRUE )
					// Show the head tracks only
					&& 	( target_page->Track[row]->chain_data[HEAD] != target_page->Track[row] )
					){

					continue;
				}

				// Display according to MIDI channel/port
				switch( 	target_page->Track[row]->attr_MCH
						+ 	target_page->Track[row]->event_offset[ATTR_MIDICH] ){
				
					// MIDI OUT 1 REAL - channels in GREEN
					case  1: case  2: case  3: case  4: case  5: case  6: case  7: case  8:
					case  9: case 10: case 11: case 12: case 13: case 14: case 15: case 16:

						MIR_point_numeric 
							(	target_page->Track[row]->attr_MCH, row, MIR_GREEN );
						break;

					// MIDI OUT 2 REAL - channels in RED
					case 17: case 18: case 19: case 20: case 21: case 22: case 23: case 24:
					case 25: case 26: case 27: case 28: case 29: case 30: case 31: case 32:

						MIR_point_numeric 
							(	target_page->Track[row]->attr_MCH - 16, row, MIR_RED );
						break;

					// MIDI OUT 1 VIRTUAL - channels in GREEN BLINK
					case 33: case 34: case 35: case 36: case 37: case 38: case 39: case 40:
					case 41: case 42: case 43: case 44: case 45: case 46: case 47: case 48:

						MIR_point_numeric 
							(	target_page->Track[row]->attr_MCH - 32, row, MIR_GREEN );
						MIR_point_numeric 
							(	target_page->Track[row]->attr_MCH - 32, row, MIR_BLINK );
						break;

					// MIDI OUT 2 VIRTUAL - channels in RED BLINK
					case 49: case 50: case 51: case 52: case 53: case 54: case 55: case 56:
					case 57: case 58: case 59: case 60: case 61: case 62: case 63: case 64:

						MIR_point_numeric 
							(	target_page->Track[row]->attr_MCH - 48, row, MIR_RED );
						MIR_point_numeric 
							(	target_page->Track[row]->attr_MCH - 48, row, MIR_BLINK );
						break;

				} // MIDI channel switch
			} // row iterator
			break;
	}



	// Blink the values for trackSelection
	for (i=0; i<MATRIX_NROF_ROWS; i++){

		if ((1<<i) & (target_page->trackSelection)){
			MIR_blink_track (i, 1, 16);
		}
	}







