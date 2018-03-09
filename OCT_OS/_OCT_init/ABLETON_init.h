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


#define 	CLIENT_NROF_TRACKS		16
#define 	CLIENTTRACK_NROF_CLIPS	 9
#define 	CLIENT_DEF_VOL			64
#define		CLIENT_DEF_PAN			64
#define		CLIENTTRACK_DEF_VOL		64
#define		CLIENTTRACK_DEF_PAN		64
#define		CLIENTTRACK_DEF_SNDA	64
#define		CLIENTTRACK_DEF_SNDB	64


// Makes the assignment for a given clip
void init_clientclip( 		Clientclipstruct* target_clip,
							unsigned char in_state,
							unsigned char in_note ){
							
	target_clip->state = in_state;
	target_clip->note = in_note;	
}



// Makes the assignement for one given track
void init_clienttrack( 	unsigned char track_ndx ){

	unsigned char i;
	unsigned char base_ndx 	= 0;
	unsigned char base_note = 0;
	unsigned char base_cc	= 0;
	Clienttrackstruct* target_clienttrack = NULL;

	// Init the Clienttrack data
	target_clienttrack = &ABLETON_client.Track[track_ndx];

	// State data
	target_clienttrack->state_active 	= 	ON;
	target_clienttrack->state_solo		=	OFF;
	target_clienttrack->state_arm		=	OFF;
	target_clienttrack->state_VOL		= 	CLIENTTRACK_DEF_VOL;
	target_clienttrack->state_PAN		= 	CLIENTTRACK_DEF_PAN;
	target_clienttrack->state_SNDA		= 	CLIENTTRACK_DEF_SNDA;
	target_clienttrack->state_SNDB		= 	CLIENTTRACK_DEF_SNDB;

	// Make the control data assignment depending on the track index
	base_ndx = (track_ndx+1) / 8;

	// Assign MIDI Channel for track
	switch( base_ndx ){
	
		// Host tracks 1-8
		case 0: 
			target_clienttrack->MIDIchannel = 1;
			break;
			
		// Host tracks 9-16
		case 1:
			target_clienttrack->MIDIchannel = 2;
			break;
	}

	// Control data
	base_note 	= (track_ndx % 8) * 12;
	base_cc		= (track_ndx % 8) * 12;

	// Assign note controls for track
	target_clienttrack->active_note		= base_note + 0;
	target_clienttrack->solo_note		= base_note + 1;
	target_clienttrack->arm_note		= base_note + 2;
	
	// Assign CC controls for track
	target_clienttrack->VOL_MIDICC		= base_cc + 0;
	target_clienttrack->PAN_MIDICC		= base_cc + 1;
	target_clienttrack->SNDA_MIDICC		= base_cc + 2;
	target_clienttrack->SNDB_MIDICC		= base_cc + 3;
		
	// Assign note controls for clips in track
	for ( i=0; i < CLIENTTRACK_NROF_CLIPS; i++ ){		
		init_clientclip( &target_clienttrack->Clip[i], OFF, base_note + 3 + i );
	}
}



// Makes the necessary assignments for the ABLETON mode - such as pitches
void ABLETON_init( Abletonstruct* ableton_client ){

	unsigned char i = 0;

	ableton_client->selected_track 	= 0;

	// State data
	ableton_client->state_runbit 	= OFF;
	ableton_client->state_follow	= OFF;
	ableton_client->state_recbit	= OFF;
	ableton_client->state_VOL 		= CLIENT_DEF_VOL;
	ableton_client->state_PAN		= CLIENT_DEF_PAN;
	
	// Control data
	ableton_client->runbit_note		= 0;
	ableton_client->follow_note		= 1;
	ableton_client->recbit_note		= 2;
	ableton_client->VOL_MIDICC		= 0;
	ableton_client->PAN_MIDICC		= 1;

	// MIDI channel
	ableton_client->MIDIchannel = 3;
	
	// Initialize the client tracks
	for ( i=0; i < CLIENT_NROF_TRACKS; i++ ){
	
		// Init the Ableton client track with computed data
		init_clienttrack( i );
	}
}


