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
// DEFINES FOR ATTRIBUTE VALUES
//

#define PAGE_MIN_VEL			0
#define PAGE_MAX_VEL			16
#define PAGE_DEF_VEL			8		// This is really a factor

#define PAGE_DEF_PIT			MIDDLE_C
#define PAGE_MIN_PIT			0
#define PAGE_MAX_PIT			127

#define PAGE_DEF_LEN			16
#define PAGE_MIN_LEN			1
#define PAGE_MAX_LEN			128

#define PAGE_DEF_STA			1
#define	PAGE_MIN_STA			0
#ifdef FEATURE_ENABLE_SONG_UPE
#define	PAGE_MAX_STA			128
#else
#define	PAGE_MAX_STA			16
#endif

#define TRACK_DEF_MUTE			0
#define TRACK_DEF_SELECT		0




#define TRACK_DEF_VELOCITY		64		// 64 Is the MIDI protocol default value for VEL
#define TRACK_MIN_VELOCITY		0
#define TRACK_MAX_VELOCITY		127

#define TRACK_MIN_VELFACTOR		0
#define TRACK_MAX_VELFACTOR		16
#define TRACK_DEF_VELFACTOR		8

#ifdef EVENTS_FACTORIZED
#define TRACK_DEF_RANGE_VEL		TRACK_MAX_VELFACTOR		// Track Modulation RANGE values
#define TRACK_MIN_RANGE_VEL		TRACK_MIN_VELFACTOR
#define TRACK_MAX_RANGE_VEL		TRACK_MAX_VELFACTOR
#endif // EVENTS_FACTORIZED

#ifdef EVENTS_ABSOLUTE
#define TRACK_DEF_RANGE_VEL		TRACK_MAX_VELOCITY		// Track Modulation RANGE values
#define TRACK_MIN_RANGE_VEL		TRACK_MIN_VELOCITY
#define TRACK_MAX_RANGE_VEL		TRACK_MAX_VELOCITY
#endif // EVENTS_ABSOLUTE



#define TRACK_DEF_PITCH			60		// Handled in the code - assignment ongoing
#define TRACK_MIN_PITCH			0
#define TRACK_MAX_PITCH			127

#define TRACK_MIN_PITFACTOR		0
#define TRACK_MAX_PITFACTOR		16
#define TRACK_DEF_PITFACTOR		8

#ifdef EVENTS_FACTORIZED
#define TRACK_DEF_RANGE_PIT		TRACK_MAX_PITFACTOR	// Track Modulation RANGE values
#define TRACK_MIN_RANGE_PIT		TRACK_MIN_PITFACTOR
#define TRACK_MAX_RANGE_PIT		TRACK_MAX_PITFACTOR
#endif // EVENTS_FACTORIZED

#ifdef EVENTS_ABSOLUTE
#define TRACK_DEF_RANGE_PIT		TRACK_MAX_PITCH	// Track Modulation RANGE values
#define TRACK_MIN_RANGE_PIT		TRACK_MIN_PITCH
#define TRACK_MAX_RANGE_PIT		TRACK_MAX_PITCH
#endif // EVENTS_ABSOLUTE



#define STEP_DEF_RANGE_MCC		127
#define STEP_MIN_RANGE_MCC		0
#define STEP_MAX_RANGE_MCC		127



#define TRACK_MIN_LENGTH		0
#define TRACK_MAX_LENGTH		16
#define TRACK_DEF_LENGTH		16

#define TRACK_MIN_LENFACTOR		0
#define TRACK_MAX_LENFACTOR		16
#define TRACK_DEF_LENFACTOR		8

#define	TRACK_MIN_RANGE_LEN		TRACK_MIN_LENFACTOR
#define	TRACK_MAX_RANGE_LEN		TRACK_MAX_LENFACTOR
#define TRACK_DEF_RANGE_LEN		TRACK_MAX_LENFACTOR		// Track Modulation RANGE values




#define TRACK_MIN_START			1
#define TRACK_MAX_START			16
#define TRACK_DEF_START			1

#define TRACK_MIN_STAFACTOR		0
#define TRACK_MAX_STAFACTOR		16
#define TRACK_DEF_STAFACTOR		8

#define	TRACK_MIN_RANGE_STA		TRACK_MIN_STAFACTOR
#define	TRACK_MAX_RANGE_STA		TRACK_MAX_STAFACTOR
#define TRACK_DEF_RANGE_STA		TRACK_MAX_STAFACTOR		// Track Modulation RANGE values




#define TRACK_MIN_POSITION		1
#define TRACK_MAX_POSITION		16
#define TRACK_DEF_POSITION		1

#define TRACK_MIN_RANGE_POS		0
#define TRACK_MAX_RANGE_POS		16
#define TRACK_DEF_RANGE_POS		16



#define TRACK_MIN_DIRECTION		1
#ifdef NEMO
	#define TRACK_MAX_DIRECTION		5 // DIRECTION_MODES - 16 in total, index into the direction array
#else
	#define TRACK_MAX_DIRECTION		16 // DIRECTION_MODES - 16 in total, index into the direction array
#endif
#define TRACK_DEF_DIRECTION		1

#define TRACK_MIN_RANGE_DIR		0
#define TRACK_MAX_RANGE_DIR		TRACK_MAX_DIRECTION
#define TRACK_DEF_RANGE_DIR		TRACK_MAX_DIRECTION




#define TRACK_MIN_AMOUNT		0
#define TRACK_MAX_AMOUNT		100
#define TRACK_DEF_AMOUNT		50		// influences the randomness range or RND

#define TRACK_MIN_AMTFACTOR		0
#define TRACK_MAX_AMTFACTOR		16
#define TRACK_DEF_AMTFACTOR		8

#define TRACK_MIN_RANGE_AMT		TRACK_MIN_AMTFACTOR
#define TRACK_MAX_RANGE_AMT		TRACK_MAX_AMTFACTOR
#define TRACK_DEF_RANGE_AMT		TRACK_MAX_AMTFACTOR




#define TRACK_MIN_GROOVE		0
#define TRACK_MAX_GROOVE		16
#define TRACK_DEF_GROOVE		0

#define TRACK_MIN_GRVFACTOR		0
#define TRACK_MAX_GRVFACTOR		16
#define TRACK_DEF_GRVFACTOR		8

#define TRACK_DEF_RANGE_GRV		TRACK_MAX_GRVFACTOR	// Track Modulation RANGE values
#define TRACK_MIN_RANGE_GRV		TRACK_MIN_GRVFACTOR
#define TRACK_MAX_RANGE_GRV		TRACK_MAX_GRVFACTOR




#define TRACK_MIN_MIDICC		-3		// -1 = NONE; -2 = MIDI BENDER; -3 = CHANNEL PRESSURE
#define TRACK_MAX_MIDICC		127		// 0-127 true controller numbers, 128 means _NONE
#define TRACK_DEF_MIDICC		-1

#define TRACK_MIN_MCCFACTOR		0
#define TRACK_MAX_MCCFACTOR		16
#define TRACK_DEF_MCCFACTOR		8

#define TRACK_DEF_RANGE_MCC		TRACK_MAX_MCCFACTOR	// Track Modulation RANGE values
#define TRACK_MIN_RANGE_MCC		TRACK_MIN_MCCFACTOR
#define TRACK_MAX_RANGE_MCC		TRACK_MAX_MCCFACTOR




#define	MIDICC_NONE				-1		// 0-127 true controller numbers, 128 means _NONE
#define MIDICC_BENDER			-2		// -2 means BENDER
#define MIDICC_PRESSURE			-3		// -3 means CHANNEL PRESSURE

#define TRACK_MIN_MIDITCH		0		//0 disabled keyboard transpose channel
#define TRACK_MAX_MIDITCH		32
#define TRACK_DEF_MIDITCH		0

#define TRACK_MIN_MIDICH		1
#define TRACK_MAX_MIDICH		32		// 1-16 are MIDI 1, 17-32 are MIDI 2, 33-64 are virtual channels
// Virtual channels are used to send data from any track to the in-ports directly.
// VMCH 33-48 send to MIDI IN 1, VMC 49-64 send to MIDI IN 2. VMCH tracks do not record from MIDI.
#define TRACK_DEF_MIDICH		1
#define TRACK_MIN_RANGE_MCH		TRACK_MIN_MIDICH
#define TRACK_MAX_RANGE_MCH		TRACK_MAX_MIDICH -1 // TRACK_MAX_MIDICH // Allow virtual midi channels
#define TRACK_DEF_RANGE_MCH		31 // TRACK_MAX_MIDICH // Do not go into virtual midi channels



#define	TRACK_MIN_CCAMNT		0
#define TRACK_MAX_CCAMNT		127
#define TRACK_DEF_CCAMNT		0		// This is the CC amount stored in the track structure.

#define	TRACK_MIN_PROGRAMCHANGE	0		// 0 is internal flag for no PC
#define TRACK_MAX_PROGRAMCHANGE	128		// The PC will be offset by one when showing and sending
#define	TRACK_DEF_PROGRAMCHANGE	0
#define	TRACK_DEF_BANKCHANGE	0



// STEP - the attributes values are offsets to the track attributes
#define MAX_NROF_STEPS			16 * MAX_NROF_TRACKS
#define STEP_NROF_ATTRIBUTES	10 		// normally 10

#define STEP_DEF_ACTIVITY		0
#define STEP_DEF_BLINKER		0

#define STEP_MIN_VELOCITY		-127	// worked with -40
#define STEP_MAX_VELOCITY		127		// worked with 40
#define STEP_DEF_VELOCITY		0

#define STEP_MIN_PITCH			-127	// worked with -40
#define STEP_MAX_PITCH			127		// worked with 40
#define STEP_DEF_PITCH			0

#define	LEGATO					0
#define STEP_MIN_LENGTH			1
#define STEP_MAX_LENGTH 		192		// 1-192 are normal length values, 0 means legato.
#define STEP_DEF_LENGTH			12		// See book p.169

#define STEP_MIN_START			1		// 0 will overlap with the 12 of the previous step
#define STEP_MAX_START			11
#define STEP_DEF_START			6		// (OCTDEF_TTC_PER_16TH/2) This is the neutral point. See Book 158, 162ff for details.

#define STEP_MIN_AMOUNT			-127	// worked with -40
#define STEP_MAX_AMOUNT			127 	// worked with 40
#define STEP_DEF_AMOUNT			0		// used in event applications

#define STEP_MIN_GROOVE			0
#define STEP_MAX_GROOVE			(PHRASE_COUNT_USER_RUNTIME-1)		// For control over phrase count, see Phrase.h
#define STEP_DEF_GROOVE			0

#define STEP_MIN_MIDICC			MIDICC_NONE
#define	STEP_MAX_MIDICC			127					// 0-127 true controller numbers, -1 means NONE
#define STEP_DEF_MIDICC			MIDICC_NONE		// To be used in MIDICC Applications

#define STEP_DEF_POSITION		0	// Step position attribute holds the second 7-bit nibble for the bender value


// These are hardcoded to the step structure, do not modify without consulting the struct
#define	STEPSTAT_TOGGLE			0
#define	STEPSTAT_SELECT			1
#define	STEPSTAT_SKIP			2
#define STEPSTAT_EVENT			4

#ifdef FEATURE_ENABLE_DICE
//DICE definitions
#define MAX_NROF_DICE			4
#define DICE_DEF_SOLO			0
#define DICE_DEF_SELECT			0

#define DICE_MIN_VELFACTOR		0
#define DICE_MAX_VELFACTOR		16
#define DICE_DEF_VELFACTOR		0

#define DICE_MIN_PITFACTOR		0
#define DICE_MAX_PITFACTOR		16
#define DICE_DEF_PITFACTOR		0

#define DICE_MIN_LENGTH			1
#define DICE_MAX_LENGTH			8
#define DICE_DEF_LENGTH			1

#define DICE_MIN_LENFACTOR		0
#define DICE_MAX_LENFACTOR		16
#define DICE_DEF_LENFACTOR		0

#define	DICE_MIN_RANGE_LEN		DICE_MIN_LENFACTOR
#define	DICE_MAX_RANGE_LEN		DICE_MAX_LENFACTOR
#define DICE_DEF_RANGE_LEN		DICE_MAX_LENFACTOR

#define DICE_MIN_START			1
#define DICE_MAX_START			16
#define DICE_DEF_START			1

#define DICE_MIN_STAFACTOR		0
#define DICE_MAX_STAFACTOR		16
#define DICE_DEF_STAFACTOR		0

#define	DICE_MIN_RANGE_STA		DICE_MIN_STAFACTOR
#define	DICE_MAX_RANGE_STA		DICE_MAX_STAFACTOR
#define DICE_DEF_RANGE_STA		DICE_MAX_STAFACTOR

#define DICE_MIN_RANGE_DIR		0
#define DICE_MAX_RANGE_DIR		DICE_MAX_DIRECTION
#define DICE_DEF_RANGE_DIR		DICE_MAX_DIRECTION

#define DICE_MIN_AMTFACTOR		0
#define DICE_MAX_AMTFACTOR		16
#define DICE_DEF_AMTFACTOR		0

#define DICE_MIN_GRVFACTOR		0
#define DICE_MAX_GRVFACTOR		16
#define DICE_DEF_GRVFACTOR		0

#define DICE_MIN_MCCFACTOR		0
#define DICE_MAX_MCCFACTOR		16
#define DICE_DEF_MCCFACTOR		0
#endif

// CC_MIXMAP DEFINES
#define CC_MIN_AMOUNT			0
#define	CC_MAX_AMOUNT			127

#define	CC_MIN_MCC				-1
#define	CC_MAX_MCC				127

#define CC_MIN_MCH				TRACK_MIN_MIDICH
#define CC_MAX_MCH				TRACK_MAX_MIDICH



// These are the attributes for the objects

// OBJECTS definitions
#define STEP_S					8
#define STEP_M					7
#define TRACK					1
#define TRACK_S					5
#define TRACK_M					2
#define FX						6
#define PAGE					3
#define SEQUENCE				4

// When this bit is set or used, we mean the object as entity
#define ENTITY					PANEL_NROF_ATTRIBUTES


// ATTRIBUTES definition used for shifts in bit matrix representations
#define VELOCITY				0
#define PITCH					1
#define LENGTH					2
#define START					3
#define POSITION				4
#define DIRECTION				5
#define AMOUNT					6
#define GROOVE					7
#define MIDICC					8
#define MIDICH					9
#define MIDITCH					10

#define NEMO_ROW_I				0
#define NEMO_ROW_II				1
#define NEMO_ROW_III			2
#define NEMO_ROW_IV				3



// Attributes definitions used in array indexes etc.
#define 	ATTR_STATUS				0
#define		ATTR_VELOCITY			1
#define 	ATTR_PITCH				2
#define 	ATTR_LENGTH				3
#define 	ATTR_START				4
#define 	ATTR_POSITION			5
#define		ATTR_DIRECTION			6
#define		ATTR_AMOUNT				7
#define 	ATTR_GROOVE				8
#define 	ATTR_MIDICC				9
#define 	ATTR_MIDICH				10
#define 	ATTR_LOCATOR			11
#define 	ATTR_MISC				12		// Collection of bits, see Track.h for details
#define		ATTR_CCAMNT				13
#define 	ATTR_G_master_tempoMUL			14
#define 	ATTR_G_master_tempoMUL_SKIP		15		// Counter used by fractional G_master_tempoMULs
#define 	ATTR_CLOCK				16	// only used in NEMO
#ifdef FEATURE_ENABLE_KEYBOARD_TRANSPOSE
#define 	ATTR_MIDITCH			20	// transpose midi channel
#endif
#ifdef FEATURE_ENABLE_DICE
#define		ATTR_DICE_BANK			21
#endif
#define 	NEMO_ATTR_STATUS			0
#define		NEMO_ATTR_VELOCITY			2
#define 	NEMO_ATTR_PITCH				1
#define 	NEMO_ATTR_LENGTH			3
#define 	NEMO_ATTR_START				4
#define 	NEMO_ATTR_POSITION			5
#define		NEMO_ATTR_DIRECTION			6
#define		NEMO_ATTR_AMOUNT			7
#define 	NEMO_ATTR_GROOVE			8
#define 	NEMO_ATTR_MIDICC			9
#define 	NEMO_ATTR_MIDICH			10
#define 	NEMO_ATTR_LOCATOR			11
#define 	NEMO_ATTR_MISC				12		// Collection of bits, see Track.h for details
#define		NEMO_ATTR_CCAMNT			13
#define 	NEMO_ATTR_G_master_tempoMUL			14
#define 	NEMO_ATTR_G_master_tempoMUL_SKIP		15		// Counter used by fractional G_master_tempoMULs
#define 	NEMO_ATTR_CLOCK				16	// only used in NEMO
#define		NEMO_ATTR_PGMCH				17  // only used in NEMO
#ifdef FEATURE_ENABLE_KEYBOARD_TRANSPOSE
#define		NEMO_ATTR_MIDITCH			20	// transpose channel
#endif
