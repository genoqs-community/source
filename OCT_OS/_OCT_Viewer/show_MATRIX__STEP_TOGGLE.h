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

		#ifdef FEATURE_ENABLE_SONG_UPE
		if (is_pressed_key(KEY_ZOOM_PAGE)) {
			return;
		}
		#endif
		// MATRIX fields with track bitpatterns (Rows 0-9) of step toggle
		for (i=0; i<MATRIX_NROF_ROWS; i++) {

			track_togglepattern = 	Page_get_trackpattern( target_page, i );
			track_eventpattern 	=	Page_get_event_trackpattern( target_page, i );
			track_skippattern	=	Page_get_skippattern( target_page, i );

			// These are the active steps - exempting the skipped steps
			MIR_write_trackpattern  ( 	(	(  	track_togglepattern	| track_eventpattern )
											^	track_skippattern
										)
										&	(  	track_togglepattern	| track_eventpattern )
										,
									i,
									MIR_GREEN);

			// .. the ones containing chords..
			//	MIR_write_trackpattern( Page_get_chord_trackpattern( target_page, i ), i, MIR_RED);

			// .. and here the event mongers - are built into the above
			MIR_write_trackpattern(   Page_get_chord_trackpattern( target_page, i )
									| track_eventpattern,
									i,
									MIR_RED   );
			// MIR_write_trackpattern( Page_get_event_trackpattern( target_page, i ), i, MIR_GREEN );

			// HYPER: Show the hypersteps in RAINBOW_GREEN
			MIR_write_trackpattern( Page_get_hyperpattern(target_page, i),
									i,
									MIR_SHINE_GREEN   );
			
			#ifdef FEATURE_ENABLE_SONG_UPE
			if (Track_get_MISC(target_page->Track[i], CONTROL_BIT) ){
				MIR_write_trackpattern ( track_skippattern, i, MIR_GREEN );

				if (is_pressed_key(KEY_ZOOM_TRACK)) {
					MIR_write_dot( LED_ZOOM_TRACK, MIR_GREEN );
					track_skippattern = Page_get_locator_event_skippattern( target_page, i );
					MIR_blink_trackpattern ( track_skippattern, i );
				}
			}
			#endif
		}


