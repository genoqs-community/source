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



		// Find first the selected track
		for (i=0; i<MATRIX_NROF_ROWS; i++) {
			if ( !row_in_track_window( target_page, i ) )
				continue;

			if ( (target_page->trackSelection & (1<<i)) > 0 ) {
				//  row is index of the first selected track
				row = i;
			}
		}


switch( G_zoom_level ){
	
	// Traditional track mode display, this is called by selecting a single track in page
	case zoomPAGE:
	
		// Display the value of each attribute for the SELECTED TRACK	

		// Now fill the matrix rows with the attribute values of track row



		// VELOCITY
		MIR_write_numeric_H( 	target_page->Track[row]->attr_VEL
							#ifdef EVENTS_ABSOLUTE
							+	target_page->Track[row]->event_offset[ATTR_VELOCITY]
							#endif // EVENTS_ABSOLUTE
							, 0);



		// PITCH
		MIR_write_pitch_H( target_page->Track[row]->attr_PIT
							#ifdef EVENTS_ABSOLUTE
							+ target_page->Track[row]->event_offset[ATTR_PITCH]
							#endif // EVENTS_ABSOLUTE
							, 1);



		// LENGTH - New school: showing the LEN factor
		MIR_fill_numeric 	(	1, 
								target_page->Track[row]->LEN_factor + 1
								+ target_page->Track[row]->event_offset[ATTR_LENGTH], 
								2,	MIR_GREEN);
		MIR_fill_numeric 	(	1, 
								target_page->Track[row]->LEN_factor + 1
								+ target_page->Track[row]->event_offset[ATTR_LENGTH], 
								2,	MIR_RED);


		
		// START - New school: showing the STA factor
		MIR_fill_numeric 	(	1, 
								target_page->Track[row]->STA_factor + 1
								+ target_page->Track[row]->event_offset[ATTR_START], 
								3,	MIR_RED);
		MIR_fill_numeric 	(	1, 
								target_page->Track[row]->STA_factor + 1
								+ target_page->Track[row]->event_offset[ATTR_START], 
								3,	MIR_GREEN);
		


		// POSITION
		// Position shows the actual track pattern and augments the skipped steps and events

		// These are the active steps - and the events
		MIR_write_trackpattern(   Page_get_trackpattern( target_page, row ) 
								| Page_get_event_trackpattern( target_page, row ),
								4, 
								MIR_GREEN );						

		// .. and here the event/chord mongers - are built into the above
		MIR_write_trackpattern(   Page_get_chord_trackpattern( target_page, row )
								| Page_get_event_trackpattern( target_page, row ), 
								4, 
								MIR_RED   );	

		MIR_augment_trackpattern( Page_get_skippattern(  target_page, row ), 4, MIR_RED   );
		MIR_augment_trackpattern( Page_get_selection_trackpattern( target_page, row ), 4, MIR_BLINK );



		// DIRECTION
		MIR_point_numeric 	(	target_page->Track[row]->attr_DIR
								+ target_page->Track[row]->event_offset[ATTR_DIRECTION], 	5, 	MIR_RED );
		MIR_point_numeric 	(	target_page->Track[row]->attr_DIR 	
								+ target_page->Track[row]->event_offset[ATTR_DIRECTION],	5, 	MIR_GREEN);



		// AMOUNT
		MIR_write_numeric_H (	target_page->Track[row]->attr_AMT
								+ target_page->Track[row]->event_offset[ATTR_AMOUNT]
								,6);


		// GROOVE
		MIR_fill_numeric 	(	1, 
								target_page->Track[row]->attr_GRV + 1, 	7,	MIR_RED);
		MIR_fill_numeric 	(	1, 
								target_page->Track[row]->attr_GRV + 1, 	7,	MIR_GREEN);



		// MIDICC
		if ( (	target_page->Track[row]->attr_MCC ) == MIDICC_NONE ){

			// Step is set to play legato, show legato flag
			MIR_write_trackpattern ( 0x0f, 8, MIR_GREEN);
		}
		else if ( (	target_page->Track[row]->attr_MCC ) == MIDICC_BENDER ){
		
			// Signal the bender as selected
			MIR_write_trackpattern ( 0x01, 8, MIR_RED);	
		}
		else if ( (	target_page->Track[row]->attr_MCC ) == MIDICC_PRESSURE ){
		
			// Signal the pressure as selected
			MIR_write_trackpattern ( 0x03, 8, MIR_RED);	
		}
		else {
			// MIDICC has a valid value 
			MIR_write_numeric_H (	target_page->Track[row]->attr_MCC, 	8);
		}


		// MIDICH
		j = target_page->Track[row]->attr_MCH 
			+ target_page->Track[row]->event_offset[ATTR_MIDICH];

		if ( j <= 16 ){
			MIR_point_numeric(	j, 	9,	MIR_GREEN );
		} 
		else if ( j <= 32 ){
			MIR_point_numeric(	j-16, 9,	MIR_RED );
		}
		else if ( j <= 48 ){
			MIR_point_numeric(	j-32, 9,	MIR_GREEN );
			MIR_point_numeric(	j-32, 9,	MIR_BLINK );
		}
		else if ( j <= 64 ){
			MIR_point_numeric(	j-48, 9,	MIR_RED );
			MIR_point_numeric(	j-48, 9,	MIR_BLINK );
		}
		
		break;
		
		
		
		
		
		

	// This is the track mode, where we deal with only one track
	case zoomTRACK:

		// ROW IV
		// POSITION or FLOW + resolution
		// FACTOR-VALUE-FLOW
		switch( NEMO_track_VFF ){

			// Factor on ROW I and flow on ROW IV
			case VFF_FACTOR:
				// Find out if we are seeing a known shape in the attribute
				i = target_page->Track[row]->flow_shape[ NEMO_selectedTrackAttribute ];

				// Show the flow value in the bottom row.
				MIR_write_trackpattern( 1 << (15-i), NEMO_ROW_IV, MIR_RED 	);
				MIR_write_trackpattern( 1 << (15-i), NEMO_ROW_IV, MIR_GREEN 	);		

				// Show the resolution if MCC
				if ( NEMO_selectedTrackAttribute == NEMO_ATTR_MIDICC ){
					switch( target_page->Track[row]->CC_resolution ){
					
						case 1:
							MIR_write_dot( 146, MIR_GREEN );
							MIR_write_dot( 146, MIR_GREEN );							
							break;
						
						case 3:
							MIR_write_dot( 157, MIR_GREEN );
							MIR_write_dot( 157, MIR_GREEN );							
							break;
						
						case 5:
							MIR_write_dot( 168, MIR_GREEN );
							MIR_write_dot( 168, MIR_GREEN );							
							break;

						case 7:
							MIR_write_dot( 179, MIR_GREEN );
							MIR_write_dot( 179, MIR_GREEN );							
							break;
					}
				}
				break;

			// Position
			case VFF_VALUE:
		
				// Position shows the actual track pattern and augments the skipped steps and events
				// These are the active steps - and the events
				MIR_write_trackpattern(   Page_get_trackpattern( target_page, row ) 
										| Page_get_event_trackpattern( target_page, row ),
										NEMO_ROW_IV,	MIR_GREEN );						
				// .. and here the event/chord mongers - are built into the above
				MIR_write_trackpattern(   Page_get_chord_trackpattern( target_page, row )
										| Page_get_event_trackpattern( target_page, row ), 
										NEMO_ROW_IV, 	MIR_RED   );			
				MIR_augment_trackpattern( Page_get_skippattern(  target_page, row ), NEMO_ROW_IV, MIR_RED   );
				MIR_augment_trackpattern( Page_get_selection_trackpattern( target_page, row ), NEMO_ROW_IV, MIR_BLINK );		
				break;
		}



		// ///////
		// ROW III
		//
		switch( NEMO_selectedTrackAttribute ){

			case NEMO_ATTR_PITCH:
			case NEMO_ATTR_VELOCITY:
			case NEMO_ATTR_LENGTH:
			case NEMO_ATTR_START:

			case NEMO_ATTR_AMOUNT:
			case NEMO_ATTR_GROOVE:
			case NEMO_ATTR_MIDICC:

				MIR_write_trackpattern( 0x3180, 	NEMO_ROW_III, MIR_GREEN );
				break;

			default:
				MIR_write_trackpattern( 0x3100, 	NEMO_ROW_III, MIR_GREEN );
				break;
		}

//			// SOLO
//			if (( target_page->trackSolopattern & ( 1 << row )) != 0 ){
//				MIR_write_dot( 13, MIR_BLINK );
//			}
			
			// EFFECTOR
			switch( Track_get_MISC( target_page->Track[row], EFF_BIT ) ){
			
				case 0:										break;
				case 1:		MIR_write_dot( 35, MIR_RED ); 	break;
				case 2:		MIR_write_dot( 46, MIR_RED );	break;
				case 3:		MIR_write_dot( 35, MIR_RED );
							MIR_write_dot( 46, MIR_RED );	break;
							
			}

			// FACTOR-VALUE-FLOW
			switch( NEMO_track_VFF ){
	
				case VFF_VALUE:
					// Indicate VFF status
					MIR_write_dot( LED_VFF_VALUE, MIR_RED );
					break;
	
				case VFF_FACTOR:
					// Indicate VFF status
					MIR_write_dot( LED_VFF_FACTOR, MIR_RED );
					break;

//				case VFF_FLOW:
//					// Indicate VFF status
//					MIR_write_dot( LED_VFF_FLOW, MIR_RED );
//					break;	
			}


//			// COPY-PASTE_CLEAR
//			// PASTE: Check if buffer is not empty
//			if (	(TRACK_COPY_BUFFER != 0) 
//				){
//				MIR_write_dot( 145, MIR_GREEN );
//			}			



		// ROW I
		// Value of the selected track attribute in the bottom line - and offset calculation for line II
		
		// Of the VALUE-FACTOR-FLOW trinity show the appropriate number
		// FACTOR-VALUE-FLOW
		switch( NEMO_track_VFF ){
			
			// VFF_VALUE
			case VFF_VALUE:
				#include "NEMO_show_MATRIX__HARDWIRED_ATTRIBUTE_VALUES__VALUE.h"
				break;

			// VFF_FLOW
			case VFF_FLOW:
				#include "NEMO_show_MATRIX__HARDWIRED_ATTRIBUTE_VALUES__FLOW.h"
				break;

			// VFF_FACTOR
			case VFF_FACTOR:
				#include "NEMO_show_MATRIX__HARDWIRED_ATTRIBUTE_VALUES__FACTOR.h"
				break;
		}



		// ROW II
		// Track attribute selector: VEL, PIT, LEN, STA, POS, DIR, AMT, GRV, MCC, MCH
		switch( NEMO_track_VFF ){

			case VFF_VALUE:
				// All attributes are available
				MIR_write_trackpattern( 0xF75D, NEMO_ROW_II, MIR_GREEN );
				// Indicate VFF status
				MIR_write_dot( LED_VFF_VALUE, MIR_RED );
				break;

			case VFF_FLOW:
				// Only flow attributes available
				MIR_write_trackpattern( 0xF700, NEMO_ROW_II, MIR_GREEN );
				// Indicate VFF status
				MIR_write_dot( LED_VFF_FLOW, MIR_RED );
				break;

			case VFF_FACTOR:
				// Only factor attributes available
				MIR_write_trackpattern( 0xF700, NEMO_ROW_II, MIR_GREEN );
				// Indicate VFF status
				MIR_write_dot( LED_VFF_FACTOR, MIR_RED );
				break;
		}

		// Show selected attribute - one only
		MIR_write_trackpattern( 1 << (15 - j ), 	NEMO_ROW_II, MIR_RED );
		MIR_write_trackpattern( 1 << (15 - j ), 	NEMO_ROW_II, MIR_BLINK );		

		break;
		#ifdef FEATURE_ENABLE_DICE
		// This is the dice mode, where we deal with only one track
		case zoomDICE:

			// ROW I
			if( Dice_is_edit_tempo( target_dice ) ){
				unsigned char dice_selected_clock = Dice_get_selected_clock_type( target_dice );
				unsigned char attr_TEMPOMUL = Dice_get_TEMPOMUL( target_dice, dice_selected_clock );
				unsigned char attr_TEMPOMUL_SKIP = Dice_get_TEMPOMUL_SKIP( target_dice, dice_selected_clock );

				// Determine color of the display: red for multiplier, green for divisor
				if ( attr_TEMPOMUL_SKIP == 0 ) {
					if ( attr_TEMPOMUL == 25 ) {
						MIR_point_numeric( 15, NEMO_ROW_I, MIR_GREEN );
					}
					else{
						// Multiplier indicator, no divisor active
						MIR_point_numeric( attr_TEMPOMUL, NEMO_ROW_I, MIR_RED );
					}
				}
				else{
					// Divisor indicator
					MIR_point_numeric( attr_TEMPOMUL_SKIP + 1, NEMO_ROW_I, MIR_GREEN );
				}
			}else{
				// Select the flow attribute
				unsigned char dice_factor = Dice_get_factor(target_dice, target_dice->attr_STATUS);

				if( dice_factor < 17 ){
					MIR_fill_numeric ( 1, dice_factor, NEMO_ROW_I, MIR_GREEN );
				}
				else{
					// Blink negative scale
					MIR_fill_numeric ( 1, dice_factor - 16, NEMO_ROW_I, MIR_GREEN );
					MIR_fill_numeric ( 1, dice_factor - 16, NEMO_ROW_I, MIR_RED );
				}
			}

			// ROW II
			// Dice flow attribute selector: VEL, PIT, LEN, STA, AMT, GRV, MCC
			j = ( target_dice->attr_STATUS > NEMO_ATTR_START ) ? target_dice->attr_STATUS - 2 : target_dice->attr_STATUS - 1;

			MIR_write_trackpattern( 0xF703, NEMO_ROW_II, MIR_GREEN );

			// Show selected attribute - one only
			MIR_write_trackpattern( ( 3 - ( target_dice->attr_MISC & 0x3) ) | 1 << (15 - j ), NEMO_ROW_II, MIR_RED );

			unsigned int rowII_blink_mask = 0;
			Trackstruct * dice_global_clock_track = Dice_get_global_clock_track();

			SET_BIT( rowII_blink_mask, 15 - j );

			if( target_dice->attr_TEMPOMUL > 1 || target_dice->attr_TEMPOMUL_SKIP & 0x0F ) {
				// Track multiplier applied
				SET_BIT( rowII_blink_mask, 0 );
			}

			if( ( dice_global_clock_track->attr_TEMPOMUL > 1 || dice_global_clock_track->attr_TEMPOMUL_SKIP & 0x0F ) && Dice_get_selected_clock_type( target_dice ) == DICE_GLOBAL_CLOCK ) {
				// Global flow clock applied
				SET_BIT( rowII_blink_mask, 1 );
			}

			MIR_write_trackpattern( rowII_blink_mask, NEMO_ROW_II, MIR_BLINK );

			// Flow on ROW IV
			j = target_dice->flow_shape[ target_dice->attr_STATUS ];

			// Show the flow value in the bottom row.
			MIR_write_trackpattern( 1 << ( 15 - j ), NEMO_ROW_IV, MIR_RED 	);
			MIR_write_trackpattern( 1 << ( 15 -  j ), NEMO_ROW_IV, MIR_GREEN );
			break;
		#endif
}


