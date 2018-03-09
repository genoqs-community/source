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


			// Show the edit attribute of selected track in top row for NEMO
			for ( i=0; i<MATRIX_NROF_ROWS; i++ ) {

				// Write the value unless it is POSITION					
				if ((target_page->trackSelection & (1<<i)) != 0) {

					if ( 	
							(target_page->editAttribute != ATTR_POSITION)  ){

						// Clear the track
						MIR_write_trackpattern( 0, 0, MIR_RED   );
						MIR_write_trackpattern( 0, 0, MIR_GREEN );
						MIR_write_trackpattern( 0, 0, MIR_BLINK );							

						show( ELE_MATRIX, EXTENDED_ATTRIBUTE_VALUES );
					}
				}
			}

