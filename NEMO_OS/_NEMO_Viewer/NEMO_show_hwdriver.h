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






//
// INFRASTRUCTURE CODE below..
//
// Main VIEWER Function: Shows the content of the MIR

void VIEWER_show_MIR(){

	// Displays the VIEWER page on the Panel
	unsigned int  shiftNdx		=	0;
	unsigned char bitpattern 	= 	0;
	unsigned char color 		= 	0;
	unsigned char mirrorsize 	= 	0;
	unsigned char NEMOchunkNdx	= 	0;


	if ( G_zoom_level != zoomDEVICE ){

		// Lock the Data port
		cyg_mutex_lock ( &mutex_data_port );
	}

	// Set IO Ports for all write
	HAL_WRITE_UINT32(E7T_IOPMOD, 0x3f8ff);

	// Init the shift sequence
	Viewer_initShift();

	// Then display every chunk in the page, shift by shift
	for (shiftNdx=1; shiftNdx <= 26; shiftNdx++){

		if ( (shiftNdx % 2) == 0 ){
			// Even shift index
			color = MIR_RED;
		} else {
			// Odd shift index
			color = MIR_GREEN;
		}

		// Compute the chunk number. +1 is an increment so /2 works
		NEMOchunkNdx = (shiftNdx+1) / 2;

		// Generate the bitpattern corresponding to the chunk of Octopus MIR
		bitpattern = NEMO_getBitpattern( color, NEMOchunkNdx );

		// Blink bit/s application to the color bitpattern
		if ( !G_master_blinker ){

			// Turn off the bits that are common to both bitpattern and blinkpattern
			bitpattern ^= (bitpattern & ( NEMO_getBitpattern (MIR_BLINK, NEMOchunkNdx) ) );
		}

		// Chunk 13 has some special needs - equally for red and green coloring
		if ( NEMOchunkNdx == 13 ){
			bitpattern = (bitpattern << 2 );
		}


		// Augment bitpattern with the RAINBOW_GREEN bits. Green on OFF and Orange on ON
		if ( color == MIR_GREEN ){
			bitpattern |= NEMO_getBitpattern( MIR_SHINE_GREEN, NEMOchunkNdx );
		}
		else if ( ( color == MIR_RED ) && ( G_master_blinker == ON ) ){
			bitpattern |= NEMO_getBitpattern( MIR_SHINE_GREEN, NEMOchunkNdx );
		}

		// Augment bitpattern with the RAINBOW_RED bits. Red on OFF and Orange on ON
		if ( color == MIR_RED ){
			bitpattern |= NEMO_getBitpattern( MIR_SHINE_RED, NEMOchunkNdx );
		}
		else if ( ( color == MIR_GREEN ) && ( G_master_blinker == ON ) ){
			bitpattern |= NEMO_getBitpattern( MIR_SHINE_RED, NEMOchunkNdx );
		}


		// Inversion necessary for the green patterns, once mirrorsize is clear
		if (color == MIR_GREEN) {

			switch ( NEMOchunkNdx ) {

				case 12:
					mirrorsize = 5;
					break;

				case 11:
					bitpattern = (bitpattern << 1);
					mirrorsize = 0;
					break;

				// ..and 8 bits for the others
				default:
					mirrorsize = 8;
					break;
			}

			bitpattern = mirror( bitpattern, mirrorsize );

		} // if MIR_GREEN

		// Final bitpattern sent to the HUI
		Viewer_writeShift( bitpattern );
	} // for shiftNdx..

	// Don't listen to the Schlusskommentar of the hardware!!
	HAL_WRITE_UINT32(E7T_IOPDATA , SELECT_NOP_DEVICE | OCTDEF_UART2_MODE);

	// In flashops we are having some trouble
	if ( G_zoom_level != zoomDEVICE ){

		// Unlock the Data port
		cyg_mutex_unlock ( &mutex_data_port );
	}
}




