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



	for (i=0; i<MATRIX_NROF_ROWS; i++) {


		// If the track is in the active selection
		if ( 	( (target_page->trackSelection & (1<<i)) > 0 )
			&&	( EDIT_TIMER == ON )
			){

			// Show its edited attribute value in its row
			switch( target_page->editAttribute ) {

				case ATTR_VELOCITY:
					MIR_write_numeric_H(
						target_page->Track[i]->attr_VEL, 	i);
					break;

				case ATTR_PITCH:
					MIR_write_pitch_H(
						target_page->Track[i]->attr_PIT, i);
					break;

				case ATTR_LENGTH:
					// New school: showing the LEN factor
					MIR_fill_numeric(
						1,
						target_page->Track[i]->LEN_factor + 1
						+ target_page->Track[i]->event_offset[ATTR_LENGTH],
						i,	MIR_GREEN);
					MIR_fill_numeric(
						1,
						target_page->Track[i]->LEN_factor + 1
						+ target_page->Track[i]->event_offset[ATTR_LENGTH],
						i,	MIR_RED);
					break;

				case ATTR_START:
					// New school: showing the STA factor
					MIR_fill_numeric(
						1,
						target_page->Track[i]->STA_factor + 1
						+ target_page->Track[i]->event_offset[ATTR_START],
						i,	MIR_RED);
					MIR_fill_numeric(
						1,
						target_page->Track[i]->STA_factor + 1
						+ target_page->Track[i]->event_offset[ATTR_START],
						i,	MIR_GREEN);
					break;

				case ATTR_POSITION:
					MIR_write_trackpattern (Page_get_trackpattern(target_page, i), 			i, MIR_RED);
					break;

				case ATTR_DIRECTION:
					MIR_point_numeric(
						target_page->Track[i]->attr_DIR
						+ target_page->Track[i]->event_offset[ATTR_DIRECTION],
						i, 	MIR_RED);
					MIR_point_numeric(
						target_page->Track[i]->attr_DIR
						+ target_page->Track[i]->event_offset[ATTR_DIRECTION],
						i, 	MIR_GREEN);
					break;

				case ATTR_AMOUNT:
					MIR_write_numeric_H(
						target_page->Track[i]->attr_AMT
						+ target_page->Track[i]->event_offset[ATTR_AMOUNT],
						i);
					break;

				case ATTR_GROOVE:
					MIR_fill_numeric(
						1,
						target_page->Track[i]->attr_GRV+1,
						i,	MIR_RED);
					MIR_fill_numeric(
						1,
						target_page->Track[i]->attr_GRV+1,
						i,	MIR_GREEN);
					break;;

				case ATTR_MIDICC:
					if ( (	target_page->Track[i]->attr_MCC )
								== MIDICC_NONE ){

						// Track is set to not send MIDICC, show MIDICC_NONE flag
						MIR_write_trackpattern ( 0x0f, i, MIR_GREEN);
					}
					else if ( (	target_page->Track[i]->attr_MCC )
								== MIDICC_BENDER
						){

						// Signal the bender as selected
						MIR_write_trackpattern ( 0x01, i, MIR_RED);
					}
					else if ( (	target_page->Track[i]->attr_MCC )
								== MIDICC_PRESSURE
						){

						// Signal the pressure as selected
						MIR_write_trackpattern ( 0x03, i, MIR_RED);
					}
					else {
						// MIDICC has a valid value
						MIR_write_numeric_H(
							target_page->Track[i]->attr_MCC, i);
					}
					break;

				case ATTR_MIDICH:
					if ( 	target_page->Track[i]->attr_MCH
							+ target_page->Track[i]->event_offset[ATTR_MIDICH] <= 16
						){

						MIR_point_numeric(
							target_page->Track[i]->attr_MCH-0
							+ target_page->Track[i]->event_offset[ATTR_MIDICH],
							i,	MIR_GREEN);
					}

					else if ( 	target_page->Track[i]->attr_MCH
							+ target_page->Track[i]->event_offset[ATTR_MIDICH] <= 32
						){

						MIR_point_numeric(
							target_page->Track[i]->attr_MCH-16
							+ target_page->Track[i]->event_offset[ATTR_MIDICH],
							i,	MIR_RED);
					}

					else if ( 	target_page->Track[i]->attr_MCH
							+ target_page->Track[i]->event_offset[ATTR_MIDICH] <= 48
						){

						MIR_point_numeric(
							target_page->Track[i]->attr_MCH-32
							+ target_page->Track[i]->event_offset[ATTR_MIDICH],
							i,	MIR_GREEN);
						MIR_point_numeric(
							target_page->Track[i]->attr_MCH-32
							+ target_page->Track[i]->event_offset[ATTR_MIDICH],
							i,	MIR_BLINK);
					}

					else if ( 	target_page->Track[i]->attr_MCH
							+ target_page->Track[i]->event_offset[ATTR_MIDICH] <= 64
						){

						MIR_point_numeric(
							target_page->Track[i]->attr_MCH-48
							+ target_page->Track[i]->event_offset[ATTR_MIDICH],
							i,	MIR_RED );
						MIR_point_numeric(
							target_page->Track[i]->attr_MCH-48
							+ target_page->Track[i]->event_offset[ATTR_MIDICH],
							i,	MIR_BLINK );
					}
					break;

				case ATTR_MIDITCH:
					if ( 	*target_page->Track[i]->attr_TCH
							+ target_page->Track[i]->event_offset[ATTR_MIDITCH] == 0 ){

						MIR_point_numeric(
							*target_page->Track[i]->attr_TCH+1
							+ target_page->Track[i]->event_offset[ATTR_MIDITCH],
							i,	MIR_RED );
						MIR_point_numeric(
							*target_page->Track[i]->attr_TCH+1
							+ target_page->Track[i]->event_offset[ATTR_MIDITCH],
							i,	MIR_BLINK );
					}
					else if ( 	*target_page->Track[i]->attr_TCH
							+ target_page->Track[i]->event_offset[ATTR_MIDITCH] <= 16
						){

						MIR_point_numeric(
							*target_page->Track[i]->attr_TCH-0
							+ target_page->Track[i]->event_offset[ATTR_MIDITCH],
							i,	MIR_GREEN);
					}

					else if ( 	*target_page->Track[i]->attr_TCH
							+ target_page->Track[i]->event_offset[ATTR_MIDITCH] <= 32
						){

						MIR_point_numeric(
							*target_page->Track[i]->attr_TCH-16
							+ target_page->Track[i]->event_offset[ATTR_MIDITCH],
							i,	MIR_RED);
					}

					else if ( 	*target_page->Track[i]->attr_TCH
							+ target_page->Track[i]->event_offset[ATTR_MIDITCH] <= 48
						){

						MIR_point_numeric(
							*target_page->Track[i]->attr_TCH-32
							+ target_page->Track[i]->event_offset[ATTR_MIDITCH],
							i,	MIR_GREEN);
						MIR_point_numeric(
							*target_page->Track[i]->attr_TCH-32
							+ target_page->Track[i]->event_offset[ATTR_MIDITCH],
							i,	MIR_BLINK);
					}

					else if ( 	*target_page->Track[i]->attr_TCH
							+ target_page->Track[i]->event_offset[ATTR_MIDITCH] <= 64
						){

						MIR_point_numeric(
							*target_page->Track[i]->attr_TCH-48
							+ target_page->Track[i]->event_offset[ATTR_MIDITCH],
							i,	MIR_RED );
						MIR_point_numeric(
							*target_page->Track[i]->attr_TCH-48
							+ target_page->Track[i]->event_offset[ATTR_MIDITCH],
							i,	MIR_BLINK );
					}
					break;
			}
		}




		// If the track is in the active selection, or if we are mixing something..
		else if (
//						( (target_page->trackSelection & (1<<i)) > 0 )
//					||
			( MIX_TIMER == ON )
			){

			if ( MIX_TIMER == ON ){
				i = target_page->mixingTrack;
				MIR_write_dot( LED_MIX_INDICATOR, MIR_BLINK );
				temp = target_page->mixAttribute;
			}
			else if ( EDIT_TIMER == ON ){
				temp = target_page->editAttribute;
			}

			// Clear the displayed track contents
			MIR_write_trackpattern( 0, i, MIR_RED   );
			MIR_write_trackpattern( 0, i, MIR_GREEN );
			MIR_write_trackpattern( 0, i, MIR_BLINK );

			switch( target_page->mixTarget ){

				// An attribute is being mixed..
				case MIXTGT_ATR:

					// Show its edited attribute value in its row
					switch( temp ) {

						case ATTR_VELOCITY:
							MIR_write_numeric_H(
								target_page->Track[i]->attr_VEL, 	i);
							break;

						case ATTR_PITCH:
							MIR_write_pitch_H(
								target_page->Track[i]->attr_PIT, 		i);
							break;

						case ATTR_LENGTH:
							// New school: showing the LEN factor
							MIR_fill_numeric(
								1,
								target_page->Track[i]->LEN_factor + 1
								+ target_page->Track[i]->event_offset[ATTR_LENGTH],
								i,	MIR_RED);
							MIR_fill_numeric(
								1,
								target_page->Track[i]->LEN_factor + 1
								+ target_page->Track[i]->event_offset[ATTR_LENGTH],
								i,	MIR_GREEN);
							break;

						case ATTR_START:
							MIR_fill_numeric(
								1,
								target_page->Track[i]->STA_factor + 1
								+ target_page->Track[i]->event_offset[ATTR_START],
								i,	MIR_RED);
							MIR_fill_numeric(
								1,
								target_page->Track[i]->STA_factor + 1
								+ target_page->Track[i]->event_offset[ATTR_START],
								i,	MIR_GREEN);
							break;

						case ATTR_POSITION:
							MIR_write_trackpattern (Page_get_trackpattern(target_page, i), 			i, MIR_RED);
							break;

						case ATTR_DIRECTION:
							MIR_point_numeric(
								target_page->Track[i]->attr_DIR
								+ target_page->Track[i]->event_offset[ATTR_DIRECTION],
								i, 	MIR_RED);
							MIR_point_numeric(
								target_page->Track[i]->attr_DIR
								+ target_page->Track[i]->event_offset[ATTR_DIRECTION],
								i, 	MIR_GREEN);
							break;

						case ATTR_AMOUNT:
							MIR_write_numeric_H(
								target_page->Track[i]->attr_AMT
								+ target_page->Track[i]->event_offset[ATTR_AMOUNT], 	i);
							break;

						case ATTR_GROOVE:
							MIR_fill_numeric(
								1,
								target_page->Track[i]->attr_GRV+1,
								i,	MIR_RED);
							MIR_fill_numeric(
								1, target_page->Track[i]->attr_GRV+1,
								i,	MIR_GREEN);
							break;;

						case ATTR_MIDICC:
							if ( (	target_page->Track[i]->attr_MCC )
										== MIDICC_NONE ){

								// Track is set to not send MIDICC, show MIDICC_NONE flag
								MIR_write_trackpattern ( 0x0f, i, MIR_GREEN);
							}
							else if ( (	target_page->Track[i]->attr_MCC ) == MIDICC_BENDER
								){

								// Signal the bender as selected
								MIR_write_trackpattern ( 0x01, i, MIR_RED);
							}
							else if ( (	target_page->Track[i]->attr_MCC ) == MIDICC_PRESSURE
								){

								// Signal the pressure as selected
								MIR_write_trackpattern ( 0x03, i, MIR_RED);
							}
							else {
								// MIDICC has a valid value
								MIR_write_numeric_H(
									target_page->Track[i]->attr_MCC, 	i);
							}
							break;

						case ATTR_MIDICH:
							if ( 	target_page->Track[i]->attr_MCH
									+ target_page->Track[i]->event_offset[ATTR_MIDICH] <= 16 ){

								MIR_point_numeric(
									target_page->Track[i]->attr_MCH
									+ target_page->Track[i]->event_offset[ATTR_MIDICH],
									i,	MIR_GREEN );
							}

							else if ( 	target_page->Track[i]->attr_MCH
									+ target_page->Track[i]->event_offset[ATTR_MIDICH] <= 32 ){

								MIR_point_numeric(
									target_page->Track[i]->attr_MCH-16
									+ target_page->Track[i]->event_offset[ATTR_MIDICH],
									i,	MIR_RED );
							}

							else if ( 	target_page->Track[i]->attr_MCH
									+ target_page->Track[i]->event_offset[ATTR_MIDICH] <= 48 ){

								MIR_point_numeric(
									target_page->Track[i]->attr_MCH-32
									+ target_page->Track[i]->event_offset[ATTR_MIDICH],
									i,	MIR_GREEN );
								MIR_point_numeric(
									target_page->Track[i]->attr_MCH-32
									+ target_page->Track[i]->event_offset[ATTR_MIDICH],
									i,	MIR_BLINK );
							}

							else if ( 	target_page->Track[i]->attr_MCH
									+ target_page->Track[i]->event_offset[ATTR_MIDICH] <= 64 ){

								MIR_point_numeric(
									target_page->Track[i]->attr_MCH-32
									+ target_page->Track[i]->event_offset[ATTR_MIDICH],
									i,	MIR_RED );
								MIR_point_numeric(
									target_page->Track[i]->attr_MCH-32
									+ target_page->Track[i]->event_offset[ATTR_MIDICH],
									i,	MIR_BLINK );
							}
							break;
					}
					break;


				case MIXTGT_USR1:
				case MIXTGT_USR2:
				case MIXTGT_USR3:
				case MIXTGT_USR4:
				case MIXTGT_USR5:
					// Check which page to use as source
					// if ( GRID_MAP0_status == ON ){
						temp_page = GRID_assistant_page;
					// }
					// else{
					//	temp_page = target_page;
					// }
					// Show the available attributes of the CC_MIXMAP
					show( ELE_TRACK_SELECTORS, CC_MIXMAP_ATTRIBUTES );
					MIR_write_numeric_H( temp_page->CC_MIXMAP [target_page->mixTarget][i][CC_MIXMAP_AMT], i );
					break;

				// These dont show anything for now
				case MIXTGT_VOL:
					MIR_write_numeric_H( target_page->MIXAMT_VOL[i], i );
					break;

				case MIXTGT_PAN:
					MIR_write_numeric_H( target_page->MIXAMT_PAN[i], i );
					break;

				case MIXTGT_MOD:
					MIR_write_numeric_H( target_page->MIXAMT_MOD[i], i );
					break;

				case MIXTGT_EXP:
					MIR_write_numeric_H( target_page->MIXAMT_EXP[i], i );
					break;
			}

			// Exit the for loop because we are done
			if ( MIX_TIMER == ON ){
				i = MATRIX_NROF_ROWS;
			}

		} // Found the right track to process data from..
	}

