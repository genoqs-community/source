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





	// Status of Scale selection. Book pg. 110..
void show_forcetoscale_status( Pagestruct* target_page ){
	
		switch( target_page->force_to_scale ){
		
			case ON:
				MIR_write_dot( LED_SCALE_SEL, MIR_RED 	);
				break;
			
			case OFF:
				MIR_write_dot( LED_SCALE_SEL, MIR_GREEN );
				break;
		}				

//		switch( target_page->scaleStatus ) {
//
//			case OFF:
//			
//				// The available options
//				if ( target_page->force_to_scale == OFF ){
//		
//					MIR_write_dot( LED_SCALE_SEL, MIR_GREEN );
//				}
//				else{
//		
//					MIR_write_dot( LED_SCALE_SEL, MIR_RED 	);
//					
////					MIR_write_dot( LED_SCALE_SEL, MIR_GREEN );
////					MIR_write_dot( LED_SCALE_SEL, MIR_BLINK );
//		
////					if ( target_page->scaleStatus != OFF ){
////						MIR_write_dot( LED_SCALE_MOD, MIR_GREEN );
////						MIR_write_dot( LED_SCALE_MOD, MIR_RED );
////					}
//				}
//			
//				break;
//
//
//			case SCALE_MOD:
//				// Indicate that SCALE_MOD is selected
//				MIR_write_dot( LED_SCALE_MOD, MIR_GREEN );
//				MIR_write_dot( LED_SCALE_MOD, MIR_RED   );
//				MIR_write_dot( LED_SCALE_MOD, MIR_BLINK );
//				// Indicate force-to-scale state
//				MIR_write_dot( LED_SCALE_SEL, MIR_RED );
//				break;
//
//								
//			case SCALE_SEL:
//				// SEL is active - now shown in the CAD LED - case is outdated
//				MIR_write_dot( LED_SCALE_MYSEL, MIR_GREEN );
//				MIR_write_dot( LED_SCALE_MYSEL, MIR_RED	);
//				MIR_write_dot( LED_SCALE_MYSEL, MIR_BLINK );
//
//				// MOD is selectable
//				MIR_write_dot( LED_SCALE_MOD, MIR_GREEN	);
//
//				// Indicate force-to-scale state
//				MIR_write_dot( LED_SCALE_SEL, MIR_RED );
//
//				break;
//							
//		} // switch( target_page->scaleStatus );
		
}

