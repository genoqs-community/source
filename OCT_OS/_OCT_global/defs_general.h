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



/* OCTOPUS
 * Miscellaneous defines used in the designated code parts
 * 14.11.2003 genoQs Machines
 */

// ***************
// DEFINES SECTION
// ***************

// #define		NEMO							TRUE

// SOFTWARE VERSION DISPLAYED
#include "defs_version.h"

// G_master_tempo RANGE DEFINITION
#define		MIN_TEMPO						 10		// MIN selectable tempo value
#define 	MAX_TEMPO						199		// MAX selectable tempo value 199
#define		DEF_TEMPO						120		// Default tempo value - 120

 // Number of timer ticks per second.
 #define 	TIMER_TICKS_PER_SEC 			50000000LL
 #define 	TIMER_TICKS_PER_MSEC 			50000LL


 // EVENTS INFRASTRUCTURE:
// CAUTION!! -> has to define one of: EVENTS_FACTORIZED or EVENTS_ABSOLUTE
#define 	EVENTS_FACTORIZED				1

// Clear the copy buffer after a paste operation (or not!)
// #define		COPY_BUFFER_FRESH				1

// INFRASTRUCTURE defines, mainly dealing with hardware issues
#include "defs_infrastructure.h"

// FRONT PANEL DEFINES
#include "defs_frontpanel.h"

// This regulates the amount of groove delay. Should not be greater than 4.
#define 	OCTDEF_GROOVE_RESOLUTION 		OCTDEF_TIMER_TICKS_PER_16TH_AT4X / (OCTDEF_MIDI_TICKS_PER_QUARTERNOTE / 4)

#include "defs_MIDI.h"
#include "defs_SYSEX.h"

// Standard Octopus matrix;
#define 	MATRIX_NROF_COLUMNS				16
#define 	MATRIX_NROF_ROWS				10
#define		MATRIX_NROF_STEPS				MATRIX_NROF_ROWS * MATRIX_NROF_COLUMNS

// Standard Control panel for now
#define 	PANEL_NROF_OBJECTS				10
#define 	PANEL_NROF_ATTRIBUTES			10
#define 	PANEL_NROF_FUNCTIONS			10
#define 	PANEL_NROF_AUX					10
#define 	PANEL_NROF_GLOBALS				10

// Defines for color coding and status
#define 	RED								100	// b01
#define 	GREEN							200	// b10
#define 	BLINK							10

#define 	MIR_BLINK						0
#define 	MIR_RED							1
#define 	MIR_GREEN						2
#define		MIR_SHINE_GREEN				3	// Switches colors between green and orange
#define		MIR_SHINE_RED					4	// Switches colors between red and orange

#define 	LHS 							0
#define 	RHS								1

// GLOBAL SYNTAX DEFINES
#define		NOP								255
#define 	OFF								0
#define 	ON								1
#define		QBIT							3

#define 	INT								1	// used to denote internal clock source
#define 	EXT								2	// used to denote external clock source.
												// Third one to go with this is "OFF"

#define 	TRUE							1	// used in boolean functions
#define		FALSE							0	// used in boolean functions

#define 	HIGH				0	// used in handling of byte nibbles in SYSEX application
#define		LOW					1

#define		PROVISORY			2	// used to indicate the provisory record mode
#define		MIDI_IN				3	// used to indicate MIDI_IN recording mode

#ifdef FEATURE_ENABLE_CHORD_OCTAVE
#define		CHORDEYE_OCTAVE_THIRD	7	// Used in Step mode to display a chord 3rd octave
#define		CHORDEYE_OCTAVE_SECOND	6	// Used in Step mode to display a chord 2nd octave
#define		CHORDEYE_OCTAVE_FIRST	5	// Used in Step mode to display a chord 1st octave
#endif
#define		CHORDEYE			4	// Used in Step mode to display a chord
#define		EMBEDDED			3	// Used to show the pitches of steps selected in map
#define 	BIRDSEYE			2	// Used for the mode where pages ops are available from within the page
#define		INTERACTIVE			1	// Used to define the interaction mode
#define 	PASSIVE				0	// Used to define the passive mode

#define 	SCALE_SEL			1
#define 	SCALE_MOD			2
#define		SCALE_CAD			3

#define 	CLUSTER_MOD			0	// Cluster operation applied immediately flag
#define		ON_THE_MEASURE_MOD	1	// Operation applied on the measure flag

#define 	OPERATION_MUTE		0 	// On the measure mute operation
#define 	OPERATION_SOLO		1 	// On the measure solo operation
#define 	OPERATION_MASK		2 	// On the measure mute / solo mask operation


#define 	LEAD				2	// used when a note in scale selection is the lead tone

#define		CURRENT				0	// Used in player bitpattern creation to differentiate
#define		NEXT				1	// between the current and next column we need info about.

#define 	SEND				1	// EFF flag - Track sending its data into EFF pool
#define		RECEIVE				2	// EFF flag - Track being influenced by EFF pool
#define		RECEIVESEND			3	// EFF flag - Track is sending and receiveing EFF pool data

#define		SOLOMCC				2
#define		SOLOPAGE			1
#define		SOLOGRID			0

// FUNCTION RELATED DEFINES
#include "defs_functions.h"


// NUMBER OF PAGES
// Should never be changed down from 160 because the invisible pages (index 144-159)
// are used for shadow operations like grid assistancy, freeze and revert, etc.
#define MAX_NROF_PAGES			160
#define PAGE_NROF_ATTRIBUTES	4	// means: STATUS, VELOCITY, PITCH, LENGTH.

#define MAX_NROF_PAGE_NOTES		160  // all notes for a full page
#define MAX_NROF_REC_NOTES		16 * 160  // all notes for a full page cluster
#define MAX_NROF_PALETTE_CHORDS 12

#define REC_MEASURES_IDX		22
#define REC_MEASURES_SPLIT		23

// TRACK
#define MAX_NROF_TRACKS			10 * MAX_NROF_PAGES
#define TRACK_NROF_ATTRIBUTES	16	// Normally 16


// ATTRIBUTE VALUES
#include "defs_attribute_values.h"


// Defines for the Zoomlevel
#define 	zoomSYSEX				0
#define 	zoomDEVICE				1
#define 	zoomGRID				2
#define 	zoomPAGE				3
#define		zoomTRACK				4
#define 	zoomMAP					5
#define 	zoomSTEP				6
#define 	zoomPLAY				7
#define 	zoomGRIDTRACK			8
#define 	zoomDIAG				9
#define		zoomSTRUM				10
#define		zoomMIXMAP				11
#define 	zoomAMANDA				12	// Locked out mode
#define		zoomABLETON				13	// Ableton controller mode
#ifdef FEATURE_ENABLE_DICE
#define		zoomDICE				14
#endif
#ifdef FEATURE_SOLO_REC
#define		zoomSOLOREC				15
#endif

#define 	NOTHING					0
#define 	MATRIX					1
#define 	PAGES					2
#define		SYSEX					29
#define 	GRID					21
#define 	GRIDTRACK				22
#define 	STEP_TOGGLE				3
#define 	STEP_SELECTION			30
#define		STEP_SELECTED			130
#define		STEP_SKIPS				32
// 			empty					33
#define 	LAUFLICHT				4
#define 	LAUFLICHT_TRACK			40
#define 	LAUFLICHT_BOTTOM		41
#define 	HANDLE					5
#define 	TRACK_SELECTORS			6
#define 	ROTARY_LEFT				7
#define 	TRACK_MUTATORS			8
#define		ROTARY_RIGHT			9
#define		GLOBALS					10
#define		TRANSPORT				11
#define		NUMERIC_QUADRANT		110
#define 	HANDLE_SELECTION		12
#define 	BIG_KNOB				120
#define		CLOCK					13
#define 	FOLLOW					14
#ifdef FEATURE_ENABLE_DICE
#define 	DICE					15
#endif
#define 	TRACK_SELECTION				13
#define 	TRACK_MUTEPATTERN			130
#define		TRACK_SOLOPATTERN			131
#define 	TRACK_FUNCTIONS				14
#define		TRACK_ATTRIBUTE_FUNCTIONS 	140
#define 	TRACK_ATTRIBUTES_RED		15
#define		TRACK_ATTRIBUTES_ALL		150
#define 	TRACK_ATTRIBUTES_MAPS		151
#define		TRACK_ATTRIBUTES_MAP_FUNCTIONS 152

#define 	STEP_ATTRIBUTES_RED			110
#define 	STEP_ATTRIBUTES_GREEN		111
#define 	STEP_ATTRIBUTE_SELECTION	112
#define 	STEP_ATTRIBUTES_MAPS		2
#define		STEP_ATTRIBUTES_ALL			200
#define 	STEP_ATTRIBUTE_VALUES		16
#define		STEP_FUNCTIONS				52
#define 	PAGE_FUNCTIONS				53
#define 	GRID_MIX_ATTRIBUTE			60

#define		MIX_ATTRIBUTE				16
#define 	CC_MIXMAP_AMT				0
#define		CC_MIXMAP_MCC				1
#define 	CC_MIXMAP_MCH				2

#define 	CC_MIXMAP_ATTRIBUTES		160
#define		TRACK_EFF_STATUS			161
#define		TRACK_REC_STATUS			162

#define 	HARDWIRED_ATTRIBUTE_VALUES	17
#define 	EXTENDED_ATTRIBUTE_VALUES	170
#define 	PLAY_CROSSBARS				18
#define 	MIX_ATTRIBUTE_VALUES		19

#define 	EDIT_ATTRIBUTE				20
#define 	TRACK_ATTRIBUTE_SELECTION 	21
#define 	MIX_ATTRIBUTE_SELECTED 		22
#define 	EDIT_ATTRIBUTE_VALUES		29

#define 	ATTR_MAP_VALUES				50
#define		CHAIN_INDICATOR				51
#define		PREVIEW_STEP				52

// VIEWER defines
#define 	MIXAMT_VALS					100
#define		GRID_CC_MAP_VALS			101
#define		STRUM_VALS					102


// Display Elements
#define		ELE_EDIT_MASTER			0
#define		ELE_TRACK_SELECTORS		1
// #define 	ELE_THIS_TRACK			2
#define 	ELE_MATRIX				3
// #define 	ELE_THIS_MATRIX			4
#define 	ELE_TRACK_MUTATORS		5
#define		ELE_SELECT_MASTER		6
#define 	ELE_MUTE_MASTER			61
#define 	ELE_MIX_MASTER			7
#define 	ELE_RETURN				8
#define 	ELE_THIS_STEP			9
#define		ELE_GLOBALS				10
#define 	ELE_ZOOMLEVEL			11
#define 	ELE_G_master_tempo_MULTIPLIER	12
#define		ELE_CHAIN_INDICATOR		13
#define		ELE_MIX_INDICATOR		14
#define		ELE_EDIT_INDICATOR		15
#define 	ELE_MIXTGT				16
#define 	ELE_OCTAVE_CIRCLE		17
#define 	ELE_SCALE_SELECTOR		170
#define		ELE_CHORD_SELECTOR		171
#define 	PAGE_TRANSPOSITION		18
#define		TRACK_TRANSPOSITION		180
#define 	G_global_locator_PICTURE	19
#define 	SCALE_MASTER			20
#define		SCALE_SELECTION			21
#define 	CHORD_SIZE				22
#define		CHORD_SIZE_TRACK		32
// 			empty					23
// #define		GRID_TRIGGERMODES		30
#define		GRID_SET_SWITCHMODE		44
#define 	GRID_BANK_PLAYMODES		45
#ifdef FEATURE_ENABLE_DICE
#define 	DICE_GRID_SELECTION		46
#endif
// CHAIN MOODE DEFS
//#define 	TEN_OF_ONE				0	// MODE 1: TEN OF ONE
#define 	CHAINMODE_1				0
#define 	LED_CHAINMODE_1			205
#define 	KEY_CHAINMODE_1			205

//#define 	FIVE_OF_TWO				1	// MODE 2: FIVE OF TWO
#define 	CHAINMODE_2				1
#define 	LED_CHAINMODE_2			204
#define 	KEY_CHAINMODE_2			204

//#define		TWO_OF_FOUR			2	// MODE 3: TWO OF FOUR
#define 	CHAINMODE_3				2
#define 	LED_CHAINMODE_3			203
#define 	KEY_CHAINMODE_3			203

//#define 	ONE_OF_EIGHT			3	// MODE 4: ONE OF EIGHT
#define 	CHAINMODE_4				3
#define 	LED_CHAINMODE_4			202
#define 	KEY_CHAINMODE_4			202


// SCALE RELEVANT INDEXES - index of notes in the selection bit array
#define		NOTE_C					23
#define		NOTE_Cis				22
#define		NOTE_D					21
#define		NOTE_Dis				20
#define		NOTE_E					19
#define		NOTE_F					18
#define		NOTE_Fis				17
#define		NOTE_G					16
#define		NOTE_Gis				15
#define		NOTE_A					14
#define		NOTE_Ais				13
#define		NOTE_B					12

// THE SCALE SIGNATURES - computed to hex from binary. See book p. 120ff
#define		SCALE_SIG_PEN			0xA94		// C D E G A 			- Major Pentatonic 	- https://www.scales-chords.com/scaleinfo.php?skey=C&sname=major%20pentatonic
#define		SCALE_SIG_WHL			0xAAA		// C D E F# G# Bb 		- Whole Tone 		- https://www.scales-chords.com/scaleinfo.php?skey=C&sname=whole%20tone
#define		SCALE_SIG_MAJ			0xAD5		// C D E F G A B		- Major Scale		- https://www.scales-chords.com/scaleinfo.php?skey=C&sname=major
#define		SCALE_SIG_MIN			0xB5A		// C D Eb F G Ab Bb		- Natual Minor		- https://www.scales-chords.com/scaleinfo.php?skey=C&sname=natural%20minor
#define 	SCALE_SIG_DIM			0xB35		// C D Eb F# G A B		- Diminished Lydian - https://www.scales-chords.com/scaleinfo.php?skey=C&sname=diminished%20lydian
#define		SCALE_SIG_CHR			0xFFF		// CHR is the chromatic scale - All Keys


// GRID SELECTION EMPTY flag - 0 is a valid index so need something greater than 160
#define 	GRID_SELECTION_EMPTY			200
#define		GRID_SELECTION_BUFFER_EMPTY		201	// Different from GRID_SELECTION_EMPTY, used as solo status flag.
#define 	GRID_DEFAULT			0
#define		GRID_SELECT				1
#define		GRID_EXPORT				2
#define 	GRID_NROF_SETS			16		// Size of the scene storage: buttons of row zero
#define 	GRID_NROF_BANKS			9		// Possible concurrent pages in the GRID.

#define		GRID_SWITCH_DIRECT		0		// status for GRID_switch_mode etc.
#define 	GRID_SWITCH_OCLOCK		1		// status for GRID_switch_mode etc.
#define		GRID_SWITCH_TAKEOVER	2		// Like direct, but locator positions are taken over

#define 	GRID_MIX				0		// GRID play mode, performance oriented, indicated by the SEL LED
#define		GRID_EDIT				1		// GRID play mode, edit oriented
#define 	GRID_CCMAP				2		// GRID play mode for CC Map editing

#define 	GRID_BANK_SIMPLE		0		// Bank play mode, performance oriented
#define 	GRID_BANK_CHAIN			1		// Bank play mode, performance oriented


// This is a flag used to indicate in the note off queue that a pitch is not to be played
#define		PITCH_EMPTY				0 		//Some flag outside the playable range: 227

// DIRECTION MODES definitions:
#define		DIRECTION_MODES			5 		// Regulates the rotary value interval

#define 	DIRECTION_LINEAR		1
#define		DIRECTION_REVERSE		2
#define 	DIRECTION_RETURN		3
#define 	DIRECTION_RANDOM		4
#define 	DIRECTION_GAME			5
#define 	DIRECTION_SINGLE		6
#define		DIRECTION_DOMINO		7

// Used to set the alarm for MIXER and EDITOR Timeouts
#define 	TIMEOUT_VALUE			140
#define 	EDIT					0		// This is the regular mode - edit attributes of selection
#define		CONTROL					1		// Edit as MCC source - send MCC data
#define 	PREVIEW					2		// Same as regular mode, but preview the steps being edited
#define		MIX2EDIT				3		// Copy of MIX functionality with selected attribute (mix2edit)
#define		PREVIEW_PERFORM			4		// Same as PREVIEW mode but outputs no MIDI during edit

// MIX targets: specify what MIXER does - and used for VIEWER defines..
#define		MIXTGT_ATR				10
#define		MIXTGT_VOL				11
#define		MIXTGT_PAN				12
#define 	MIXTGT_MOD				13
#define 	MIXTGT_EXP				14

// Dont change these define values - assumed to be like this in the init sequence.
#define 	MIXTGT_USR0				0
#define 	MIXTGT_USR1				1
#define 	MIXTGT_USR2				2
#define 	MIXTGT_USR3				3
#define 	MIXTGT_USR4				4
#define 	MIXTGT_USR5				5

#define 	OPTIONS					127 // flag to indicate that we want to see available options of the above

// MIDI handling
#define		MIDI_NOTE				0
#define 	MIDI_PGMCH				1
#define 	MIDI_CC					2
#define		MIDI_CLOCK				3
#define		MIDI_BENDER				4
#define 	MIDI_PRESSURE			6
#define 	MIDI_LOAD				5		// Used in Viewer for MIDI mbox peek
#define		MIDI_START				0xFA
#define		MIDI_STOP				0xFC

// MIX MASTER STATUS
#define		NEG						0
#define		POS						1

// This is the pitch offset of the middle C. Used at various occasoins
#define 	MIDDLE_C				60


// This defines the states that the follow variable may take
#define		FOLLOW_GRID				1
#define		FOLLOW_PAGE				2
#define		FOLLOW_TRACK			3

#ifdef FEATURE_ENABLE_DICE
// This defines the dice page repository location
#define		DICE_PAGE				159

// This defines the dice attrMISC flags
// This defines the dice attrMISC flags
#define		DICE_TRACK_CLOCK		1 // Track Dice multiplier / divider clock flag [*******x]
#define		DICE_GLOBAL_CLOCK		2 // Track Dice multiplier / divider clock flag [******x*]

// This defines dice bank editorMode
#define		DICE_QUANT				1 // Quantise user to dice grid
#define		DICE_ALIGN				2 // Align dice pages to global
#endif
// MIDI EVENTS: There can be at most this many events in the MIDI QUEUE.
// Make sure this is not below 10 or so, otherwise something bad may happen.. :-(
// #define 	 MIDI_NROF_EVENTS		2000

#define 	FIXED					0		// Used for rotary value wrap
#define		WRAPPED					1		// Used for rotary value wrap

// Used for GRID_set_switchmode
#define 	SEL_RECALL				0
#define		SEL_CLEAR				1

// Used in the Track MISC attribute as flags
#ifdef FEATURE_ENABLE_SONG_UPE
#define 	CONTROL_BIT				0
#endif
#define 	CHORD_BIT				2
#define		EFF_BIT					7

// Used in the chain data array (in the tracks)
#define 	HEAD					0
#define 	NEXT					1
#define		PREV					2
#define 	PLAY					3

#define 	EMPTY_TRIGGER_SET		20
#define 	EMPTY					200

#define		def_GENOQS_SYSEX_BASE		0x00206400
#define		def_OS_SYSEX_DUMP			0x00
#define		def_CONFIGFILE_SYSEX_DUMP	0x01
#define 	def_PAGE_SYSEX_DUMP			0x10
#define 	def_BANK_SYSEX_DUMP			0x20
#define		def_GRID_SYSEX_DUMP			0x30
#define 	def_FULL_SYSEX_DUMP			0x40
#define		def_SCENE_SYSEX_DUMP		0x50

#define 	PGM						0
#define 	BANK					1

#define 	SYSEX_DUMP_SPEED_MIN 	10
#define 	SYSEX_DUMP_SPEED_MAX 	100

#define 	CONSTANT_BLINK			TRUE


#define min(a, b)	((a)<(b)?(a):(b))
#define max(a, b)	((a)>(b)?(a):(b))

#define 	HYPERSTEPS_PLAY_NOTES	TRUE

#ifdef FEATURE_ENABLE_CHORD_OCTAVE
#define		CHORD_OCTAVE_FIRST		1
#define		CHORD_OCTAVE_SECOND		2
#define		CHORD_OCTAVE_THIRD		4
#define		CHORD_OCTAVE_ALL		0x7

#define		CHORD_OCTAVE_STATUS_CURRENT	1
#define		CHORD_OCTAVE_STATUS_OTHER	2
#define		CHORD_OCTAVE_STATUS_ALL		4
#endif

#ifdef FEATURE_ENABLE_SONG_UPE
#define		TRK_CTRL_PGTGL			1
#define		TRK_CTRL_MUT_PGTGL		2
#define		TRK_CTRL_EXT_PLAY		3
#define		TRK_CTRL_EXT_STOP		4
#define		TRK_CTRL_CLEAR			5
#define		TRK_CTRL_MIX			6
#define		TRK_CTRL_MOVE			7
#define		TRK_CTRL_TEMPO			0
#endif

#define		PGM_CLST_CLR			1
#define		PGM_CLST_CPY			2

#ifdef FEATURE_ENABLE_KEYBOARD_TRANSPOSE
#define		GST_TOGGLE				0
#endif
