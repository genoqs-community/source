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



				
			// Per track: Mix Attribute Value
			for (i=0; i<MATRIX_NROF_ROWS; i++) {
				
				j = GRID_assistant_page->CC_MIXMAP[ GRID_assistant_page->mixTarget ][ i ][ GRID_assistant_page->CC_MIXMAP_attribute];

				// Account for the different display needs of the attributes
				switch( GRID_assistant_page->CC_MIXMAP_attribute ){

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

