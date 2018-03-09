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



			// Get index of selected track
			j = my_bit2ndx( target_page->trackSelection );

			// Write contents of selected track to rowzero
			MIR_write_trackpattern( Page_get_trackpattern( target_page, j ), 9, MIR_GREEN	    );
			MIR_write_trackpattern( Page_get_skippattern ( target_page, j ), 9, MIR_RED			);


			MIR_write_trackpattern  ( Page_get_trackpattern( target_page, j ), 9, MIR_GREEN );
			// .. and here the event/chord mongers - are built into the above
			MIR_write_trackpattern(   Page_get_chord_trackpattern( target_page, j )
									| Page_get_event_trackpattern( target_page, j ),
									9,
									MIR_RED   );

			MIR_augment_trackpattern( Page_get_skippattern(  target_page, j ), 9, MIR_RED   );
			MIR_augment_trackpattern( Page_get_selection_trackpattern( target_page, j ), 9, MIR_BLINK );


			// Blink the Steps whose selection bit is set
			MIR_blink_trackpattern (Page_get_selection_trackpattern(target_page, j), 9);

			// Track contents of selected Track in rowzero, lauflicht in the selected track
			if (G_run_bit) {

				MIR_write_lauflicht_track( j, j );
			}

