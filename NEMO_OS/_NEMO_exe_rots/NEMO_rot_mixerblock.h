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



void rot_exe_MIX( unsigned char rotNdx, unsigned char direction, Pagestruct* target_page ){

	signed int min = 0;
	signed int max = 0;
	unsigned char trackNdx = 0;
	unsigned int i=0, j=0, k=0;

	// x2 - Track row window shift
	unsigned char shiftTrackRow = track_get_window_shift( target_page );

	// Mixer block rotary passed
	if (	(rotNdx >= 11) 
		&& 	(rotNdx <= 20) 
		){

		// Restart MIX Timer - this enables the MIX per track view (mixed with page steps)
		// ..maybe one day. 
		// You can get the screen that you want if the target_page->mix MasterStatus is "red"
		// Needs more digging
		start_MIX_TIMER();

		// Normalized index
		trackNdx = shiftTrackRow + rotNdx - 11;

		// Keep the index for some other processing (in viewer)
		target_page->mixingTrack = trackNdx;


		switch( target_page->mixTarget ){
			
			case MIXTGT_ATR:
				#include "NEMO_rot_mixAttribute.h"
				break;


//			// 	///////////////////////
//			//	SOUND CONTROL SELECTION
//			//			
//			// Sound control 1
//			case MIXTGT_VOL:
//				modify_parameter( &target_page->MIXAMT_VOL[trackNdx], 
//					CC_MIN_AMOUNT, CC_MAX_AMOUNT, direction, ON, FIXED);
//											
//				MIDI_send( 	MIDI_CC, 
//							target_page->Track[trackNdx]->attr_MCH
//							+ target_page->Track[trackNdx]->event_offset[ATTR_MIDICH],
//							70,
//							target_page->MIXAMT_VOL[trackNdx] 	);					
//				break;
//
//
//			// Sound control 2
//			case MIXTGT_PAN:
//				modify_parameter( &target_page->MIXAMT_PAN[trackNdx], 
//					CC_MIN_AMOUNT, CC_MAX_AMOUNT, direction, ON, FIXED);
//				
//				MIDI_send( 	MIDI_CC, 
//							target_page->Track[trackNdx]->attr_MCH
//							+ target_page->Track[trackNdx]->event_offset[ATTR_MIDICH],
//							71,
//							target_page->MIXAMT_PAN[trackNdx] 	);					
//				break;
//
//
//			// Sound control 3
//			case MIXTGT_MOD:
//				modify_parameter( &target_page->MIXAMT_MOD[trackNdx], 
//					CC_MIN_AMOUNT, CC_MAX_AMOUNT, direction, ON, FIXED);
//				
//				MIDI_send( 	MIDI_CC, 
//							target_page->Track[trackNdx]->attr_MCH
//							+ target_page->Track[trackNdx]->event_offset[ATTR_MIDICH],
//							72,
//							target_page->MIXAMT_MOD[trackNdx] 	);					
//				break;
//
//
//			// Sound control 4
//			case MIXTGT_EXP:
//				modify_parameter( &target_page->MIXAMT_EXP[trackNdx], 
//					CC_MIN_AMOUNT, CC_MAX_AMOUNT, direction, ON, FIXED);
//				
//				MIDI_send( 	MIDI_CC, 
//							target_page->Track[trackNdx]->attr_MCH
//							+ target_page->Track[trackNdx]->event_offset[ATTR_MIDICH],
//							73,
//							target_page->MIXAMT_EXP[trackNdx] 	);					
//				break;


			// This implements the USR map functionality - NEMO CC Sound Control
			case MIXTGT_USR1:
			case MIXTGT_USR2:
			case MIXTGT_USR3:
			case MIXTGT_USR4:
				// Operate on the GRID assistant page
				GRID_assistant_page->mixTarget = target_page->mixTarget;
				target_page = GRID_assistant_page;

				// Edit the parameters of the CC map
				if ( G_zoom_level == zoomMIXMAP ){
					shiftTrackRow = track_get_window_shift( GRID_assistant_page );
					trackNdx = shiftTrackRow + rotNdx - 11;
					// This applies to all other modes
					switch( target_page->CC_MIXMAP_attribute ) {

						case CC_MIXMAP_AMT:
							modify_signed_parameter( 
									   	&target_page->CC_MIXMAP 	[target_page->mixTarget][trackNdx]
																	[CC_MIXMAP_AMT], 
										CC_MIN_AMOUNT, CC_MAX_AMOUNT, direction, ON, FIXED);
											
							// Send MIDICC unless there is no controller selected			
							if ( ( target_page->CC_MIXMAP	[target_page->mixTarget]
															[trackNdx][CC_MIXMAP_MCC]
										!= (unsigned char) MIDICC_NONE )
								){

								MIDI_send( 	MIDI_CC, 
										target_page->CC_MIXMAP  [target_page->mixTarget][trackNdx]
																[CC_MIXMAP_MCH],
										target_page->CC_MIXMAP	[target_page->mixTarget][trackNdx]
																[CC_MIXMAP_MCC],
										target_page->CC_MIXMAP	[target_page->mixTarget][trackNdx]
																[CC_MIXMAP_AMT]   );					
							}
							break;

						case CC_MIXMAP_MCC:
							modify_signed_parameter( 
									   	&target_page->CC_MIXMAP 	[target_page->mixTarget][trackNdx]
																	[CC_MIXMAP_MCC], 
											CC_MIN_MCC, CC_MAX_MCC, direction, ON, FIXED);
							break;

						case CC_MIXMAP_MCH:
							modify_signed_parameter( 
									   	&target_page->CC_MIXMAP 	[target_page->mixTarget][trackNdx]
																	[CC_MIXMAP_MCH], 
											CC_MIN_MCH, CC_MAX_MCH, direction, ON, FIXED);
							break;
					}
				}
				
				// Modes other than zoomMIXMAP
				else{
	
					// Modify the value of the controller internally
					modify_signed_parameter( 
							   	&target_page->CC_MIXMAP [target_page->mixTarget][trackNdx][CC_MIXMAP_AMT], 
								CC_MIN_AMOUNT, CC_MAX_AMOUNT, direction, ON, FIXED );
						
					// Send corresponding MIDICC if there is a valid controller selected			
					if ( ( target_page->CC_MIXMAP[target_page->mixTarget][trackNdx][CC_MIXMAP_MCC]
							!= (unsigned char) MIDICC_NONE )
						){
	
						MIDI_send( 	MIDI_CC, 
								target_page->CC_MIXMAP  [target_page->mixTarget][trackNdx][CC_MIXMAP_MCH],
								target_page->CC_MIXMAP	[target_page->mixTarget][trackNdx][CC_MIXMAP_MCC],
								target_page->CC_MIXMAP	[target_page->mixTarget][trackNdx][CC_MIXMAP_AMT]  );
					}
				}

				break;

		} // switch
	} // if rotNdx part of MIXER
	
}

