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



// Sends MIDI note corresponding to given clip
void Clip_trigger( unsigned char track_ndx, unsigned char clip_ndx ){

	unsigned char MIDIchannel = 0;
	unsigned char note  = 0;
	
	MIDIchannel 	= ABLETON_client.Track[ track_ndx ].MIDIchannel;
	note 			= ABLETON_client.Track[ track_ndx ].Clip[ clip_ndx ].note;
	
	// NOTE send: channel, pitch, velocity
	MIDI_send(	MIDI_NOTE, MIDIchannel, note, 127 );
	MIDI_send(	MIDI_NOTE, MIDIchannel, note,   0 );
}


// Interpret the track switch which is row 0
void Client_track_switch( unsigned char track_ndx ){

	unsigned char MIDIchannel = 0;
	unsigned char note  = 0;
	
	MIDIchannel 	= ABLETON_client.Track[ track_ndx ].MIDIchannel;

	// Check the colours of the row 0 key
	// ARM
	if ( is_pressed_key( KEY_RECORD ) ){
	
		note = ABLETON_client.Track[ track_ndx ].arm_note;
	}
	// SOLO
	else if ( is_pressed_key( KEY_SELECT_MASTER ) ){
	
		note = ABLETON_client.Track[ track_ndx ].solo_note;
	}
	// ACTIVE
	else if ( is_pressed_key( KEY_MUTE_MASTER ) ) {
		
		note = ABLETON_client.Track[ track_ndx ].active_note;
	}
	else {
	
		// Mark the track selected in anticipation for rotary input
		ABLETON_client.selected_track = track_ndx;
	}

	// NOTE ON send: channel, pitch, velocity
	MIDI_send(	MIDI_NOTE, MIDIchannel, note, 127 );
	MIDI_send(	MIDI_NOTE, MIDIchannel, note,   0 );
}


// Master key interpreter in ABLETON Client mode
void key_exec_ABLETON( unsigned int keyNdx ){

	unsigned char row = 0;
	unsigned char col = 0;

	//
	// MATRIX 
	//
	// Make sure we are within the real matrix range
	if ( 	( keyNdx >= 11 ) && ( keyNdx <= 185 )
		&& 	( ( (keyNdx-10) % 11 ) != 0 ) 
		){ 

		// This is the track index for the clip
		col = column_of(keyNdx);
		// Compute the clip index in the track
		row = row_of(keyNdx);

		// Sends a trigger message for the given clip
		switch( row ){

			case 9:
				// Row 0 deals with client track switches
				Client_track_switch( col );
				break;

			default:
				// All other rows deal with clip switches
				Clip_trigger( col, row);				
				break;
		}
	}	
}	


