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


		// Depends on the selected mix target
		switch( target_page->mixTarget ){

			case MIXTGT_ATR:
				// If mixMasterStatus wants to show values
				if (target_page->mixMasterStatus == NEG) {

					// Per track: Mix Attribute Value
					show( ELE_MATRIX, MIX_ATTRIBUTE_VALUES );
				}
				else {
					// DEFAULT MATRIX DISPLAY
					show( ELE_MATRIX, STEP_TOGGLE );
					show( ELE_MATRIX, LAUFLICHT );

					// Write the edited value in track
					// ..unless it is LENGTH, START or POSITION
					if ( 	(target_page->mixAttribute != ATTR_LENGTH)
						 &&	(target_page->mixAttribute != ATTR_START)
						 &&	(target_page->mixAttribute != ATTR_POSITION)
						 ){

						// Clear the track
						i = target_page->mixingTrack;
						MIR_write_trackpattern( 0, i, MIR_RED   );
						MIR_write_trackpattern( 0, i, MIR_GREEN );
						MIR_write_trackpattern( 0, i, MIR_BLINK );

						// Write attribute value in the mixing track
						// j = target_page->Track[i]->attribute[target_page->mixAttribute];
						j =	*(ptr_of_track_attr_value( target_page, i, target_page->mixAttribute ));

						MIR_write_numeric_H( j, i );
					}
				}
				break;


			case MIXTGT_VOL:
				// If mixMasterStatus wants to show values
				if (target_page->mixMasterStatus == NEG) {
					// Per track: Mix Attribute Value
					for (i=0; i<MATRIX_NROF_ROWS; i++) {
						MIR_write_numeric_H( target_page->MIXAMT_VOL[i], i );
					}
				}
				else {
					// DEFAULT MATRIX DISPLAY
					show( ELE_MATRIX, STEP_TOGGLE );
					show( ELE_MATRIX, LAUFLICHT );

					// Write the edited value in track:

					// Clear the track
					i = target_page->mixingTrack;
					MIR_write_trackpattern( 0, i, MIR_RED   );
					MIR_write_trackpattern( 0, i, MIR_GREEN );
					MIR_write_trackpattern( 0, i, MIR_BLINK );

					// Write attribute value in the mixing track
					j = target_page->MIXAMT_VOL[i];
					MIR_write_numeric_H( j, i );
				}
				break;


			case MIXTGT_PAN:
				// If mixMasterStatus wants to show values
				if (target_page->mixMasterStatus == NEG) {
					// Per track: Mix Attribute Value
					for (i=0; i<MATRIX_NROF_ROWS; i++) {
						MIR_write_numeric_H( target_page->MIXAMT_PAN[i], i );
					}
				}
				else {
					// DEFAULT MATRIX DISPLAY
					show( ELE_MATRIX, STEP_TOGGLE );
					show( ELE_MATRIX, LAUFLICHT );

					// Write the edited value in track:

					// Clear the track
					i = target_page->mixingTrack;
					MIR_write_trackpattern( 0, i, MIR_RED   );
					MIR_write_trackpattern( 0, i, MIR_GREEN );
					MIR_write_trackpattern( 0, i, MIR_BLINK );

					// Write attribute value in the mixing track
					j = target_page->MIXAMT_PAN[i];
					MIR_write_numeric_H( j, i );
				}
				break;


			case MIXTGT_MOD:
				// If mixMasterStatus wants to show values
				if (target_page->mixMasterStatus == NEG) {
					// Per track: Mix Attribute Value
					for (i=0; i<MATRIX_NROF_ROWS; i++) {
						MIR_write_numeric_H( target_page->MIXAMT_MOD[i], i );
					}
				}
				else {
					// DEFAULT MATRIX DISPLAY
					show( ELE_MATRIX, STEP_TOGGLE );
					show( ELE_MATRIX, LAUFLICHT );

					// Write the edited value in track:

					// Clear the track
					i = target_page->mixingTrack;
					MIR_write_trackpattern( 0, i, MIR_RED   );
					MIR_write_trackpattern( 0, i, MIR_GREEN );
					MIR_write_trackpattern( 0, i, MIR_BLINK );

					// Write attribute value in the mixing track
					j = target_page->MIXAMT_MOD[i];
					MIR_write_numeric_H( j, i );
				}
				break;

			case MIXTGT_EXP:
				// If mixMasterStatus wants to show values
				if (target_page->mixMasterStatus == NEG) {
					// Per track: Mix Attribute Value
					for (i=0; i<MATRIX_NROF_ROWS; i++) {
						MIR_write_numeric_H( target_page->MIXAMT_EXP[i], i );
					}
				}
				else {
					// DEFAULT MATRIX DISPLAY
					show( ELE_MATRIX, STEP_TOGGLE );
					show( ELE_MATRIX, LAUFLICHT );

					// Write the edited value in track:

					// Clear the track
					i = target_page->mixingTrack;
					MIR_write_trackpattern( 0, i, MIR_RED   );
					MIR_write_trackpattern( 0, i, MIR_GREEN );
					MIR_write_trackpattern( 0, i, MIR_BLINK );

					// Write attribute value in the mixing track
					j = target_page->MIXAMT_EXP[i];
					MIR_write_numeric_H( j, i );
				}
				break;

			// The indexes are 0-5
			case MIXTGT_USR0:
			case MIXTGT_USR1:
			case MIXTGT_USR2:
			case MIXTGT_USR3:
			case MIXTGT_USR4:
			case MIXTGT_USR5:

				// if ( GRID_MAP0_status == ON ){
					i = target_page->mixTarget;
					target_page = GRID_assistant_page;
					target_page->mixTarget = i;
				// }

				// If mixMasterStatus wants to show values
				if (target_page->mixMasterStatus == NEG) {

					// Per track: Mix Attribute Value
					for (i=0; i<MATRIX_NROF_ROWS; i++) {

						// Buffer for the actual value
						j = target_page->CC_MIXMAP[target_page->mixTarget][i][target_page->CC_MIXMAP_attribute];

						// Account for the different display needs of the attributes
						switch( target_page->CC_MIXMAP_attribute ){

							case CC_MIXMAP_MCC:
								if ( j == (unsigned char) MIDICC_NONE ){
									// Track is set to not send MIDICC, show MIDICC_NONE flag
									MIR_write_trackpattern ( 0x0f, i, MIR_GREEN);
								}
								else {
									// MIDICC has a valid value
									MIR_write_numeric_H ( j, i );
								}
								break;

							case CC_MIXMAP_MCH:
								if ( j <= 16 ){
									MIR_point_numeric( j, 		i, MIR_GREEN);
								}
								else if ( j <= 32 ){
									MIR_point_numeric( j-16,	i,	MIR_RED);
								}
								else if ( j <= 48 ){
									MIR_point_numeric( j-32,	i,	MIR_GREEN);
									MIR_point_numeric( j-32,	i,	MIR_BLINK);
								}
								else if ( j <= 64 ){
									MIR_point_numeric( j-48,	i,	MIR_RED);
									MIR_point_numeric( j-48,	i,	MIR_BLINK);
								}
								break;

							case CC_MIXMAP_AMT:
								MIR_write_numeric_H( j, i );
								break;
						}
					}
				}
				else {

					// DEFAULT MATRIX DISPLAY
					show( ELE_MATRIX, STEP_TOGGLE );
					show( ELE_MATRIX, LAUFLICHT );

					// Write the edited value in track:

					// Clear the track
					i = target_page->mixingTrack;
					MIR_write_trackpattern( 0, i, MIR_RED   );
					MIR_write_trackpattern( 0, i, MIR_GREEN );
					MIR_write_trackpattern( 0, i, MIR_BLINK );

					// Write attribute value in the mixing track
					j = temp_page->MIXAMT_USR[i];
					MIR_write_numeric_H( j, i );
				}
				break;
		}


