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

#ifndef RUNTIME_H_
#define RUNTIME_H_

#include "../_OCT_global/includes-declarations.h"


// ===============================================================================================
// 	                                      DECLARATIONS ONLY!
// ===============================================================================================






// =====================================================================================
// Runtime step struct
// =====================================================================================

typedef struct stepstruct{

	unsigned int chord_up;
	// Holds the bit offset for chords together with aux notes.
	// Chord range is therefore 32+11 = 43 notes. That's 3+ octaves!
	// The two halves hold the info about transposition of auxiliary notes (in chord)
	// The top three bits hold info on the chord stack size. 0 is default, 6 is max.
	// |__|__|__|__|__|__|..|__|__|__|__|__|__|__|__|__|..|__|__|__|
	//  31 30 29 28 27 26 .. 18 17 16 15 14 13 12 11 10 ..  2  1  0
	//   |  |  |  |  |  |  |  |  |  |  |  |  |  |  |  |  |  |  |  |
	//   |  |  |  |  |  |  |  |  |  |  |  |  |  |  |  +--+--+--+--+------> first 11 bits: chord up		(FEATURE_ENABLE_CHORD_OCTAVE first 12 bits)
	//   |  |  |  |  |  |  |  |  |  |  +--+--+--+--+---------------------> 5 EMPTY
	//   |  |  |  |  |  +--+--+--+--+------------------------------------> second 11 bits: chord up up 	(FEATURE_ENABLE_CHORD_OCTAVE first 12 bits)
	//   |  |  |  +--+---------------------------------------------------> 2 EMPTY
	//   +--+--+---------------------------------------------------------> chord stack size (max 6)


	unsigned short chord_data;
	// |__|__|__|__|__|__|__|__|__|__|__|__|__|__|__|__|
	//  15 14 13 12 11 10  9  8  7  6  5  4  3  2  1  0
	//   |  |  |  |  |  |  |  |  |  |  |  |  |  |  |  |
	//   |  |  |  |  |  +--+--+--+--+--+--+--+--+--+--+------> 11 bits of auxiliary notes
	//   |  |  |  |  |
	//   +--+--+--+--+--------------------------------------->  5 bits free.


	// Phrase number. Default is 0 for no-phrase.
	unsigned char phrase_num;


	unsigned char phrase_data;
	// |__|__|__|__|__|__|__|__|
	//   7  6  5  4  3  2  1  0
	//   |  |  |  |  |  |  |  |
	//   |  |  |  |  +--+--+--+------------------------------>  4 bits for groove factor [0, 15], where 7 is neutral.
	//	 |  |  |  |
	//   +--+--+--+------------------------------------------>  4 bits for phrase type.


	unsigned char event_data;
	// |__|__|__|__|__|__|__|__|
	//   7  6  5  4  3  2  1  0
	//   |  |  |  |  |  |  |  |
	//   |  |  |  |  +--+--+--+------------------------------>  4 bits of event attribute index
	// Step LEN multiplier fields
	//	 |  |  |  |
	//   +--+--+--+------------------------------------------>  4 bits of step LEN multiplier ??
	// Holds the LEN multiplier of the step. Possible states: 1, 2, 4, ... ??


	/* Step status: 8 bits hold the step status (counting right to left):
	 * ..contents of attr_STATUS;
	 *	+--+--+--+--+--+--+--+--+
	 *	| 7| 6| 5| 4| 3| 2| 1| 0|
	 * 	+--+--+--+--+--+--+--+--+
	 *
	 * 						 |-0|
	 * 						   0  OFF					(TOGGLE)
	 *                         1  ON 					(TOGGLE)
	 *                    |-1|
	 * 					    0  	  UN-SELECTED			(SELECT)
	 * 				      	1     SELECTED				(SELECT)
	 *                 |-2|
	 * 					 0 		  UN-SKIPPED			(SKIP)
	 * 				  	 1		  SKIPPED				(SKIP)
	 * 				|-3|
	 * 				  0			  Step always played	(PROVISORY)
	 * 				  1			  STEP PROVISORY		(PROVISORY)
	 * 			 |-4|
	 * 			   0			  EVENT OFF				(EVENT)
	 * 			   1			  EVENT ON				(EVENT)
	 * 		  |-5|
	 * 			0				  empty..
	 * 			1
	 *
	 * 	   |-6|
	 *       0                    ROLL plays all noteoffs
	 *       1                    ROLL plays only last noteoff
	 *  |-7|
	 *    0						  ROLL is OFF
	 *    1 					  ROLL is ON
	 *
	 */

	// The step attributes are offsets relative to the values of the corresponding
	// track attributes of the tracks they belong to.

	// currently the first 10 are used
	// Source 		Meaning					Array index

	// 	manual		ATTR_STATUS				0			0 tgl 1 sel 2 skip 4 event
	unsigned char 	attr_STATUS;
	  signed char	attr_VEL;
	  signed char	attr_PIT;
	unsigned char	attr_LEN;
	unsigned char	attr_STA;

	unsigned char	attr_POS;				// Holds the second 7-bit nibble for the bender value

	  signed char	attr_AMT;
	// unsigned char	attr_GRV;
	  signed char 	attr_MCC;
    unsigned char	hyperTrack_ndx;			// Holds the index of the hyped track, or 10 if void

    // unsigned char buffer;
	// The index of auxiliary notes is index into the parts of the bit array (max 7).
	// With 2 bits each, we can describe 4 octaves max! However only 3 are LED showable.
	// Base octave orange (base note blinks), oct up green, oct upup red.

} Stepstruct;







// =====================================================================================
// Runtime track struct
// =====================================================================================



/* OCTOPUS
 * Definitions of the TRACK data and functions
 * 20.11.2003 genoQs Machines
 */

	//
	// TRACK ATTRIBUTES LEGEND - An Overview:
	//

	// Source 		Meaning					Array index
	// ----------   ----------------       -------------
	// Attributes
	//	#define 	[ATTR_STATUS]			0		// Not seen in use anywhere..

	//	#define		[ATTR_VELOCITY]			1

	//	#define 	[ATTR_PITCH]			2

	//	#define 	[ATTR_LENGTH]			3

	//	#define 	[ATTR_START]			4

	//	#define 	[ATTR_POSITION]			5

	//	#define		[ATTR_DIRECTION]		6

	//	#define		[ATTR_AMOUNT]			7

	//	#define 	[ATTR_GROOVE]			8

	//	#define 	[ATTR_MIDICC]			9

	//	#define 	[ATTR_MIDICH]			10

	//	#define 	[ATTR_LOCATOR]			11

	//	#define 	[ATTR_MISC]				12		// Collection of bits, keeping all sorts of info
													// |_|_|_|_|_|_|_|_|
													//	7 6 5 4 3 2 1 0
													//  | | | | | | | +------> DIRECTION :: RETURN
													//  | / | \ | / |          OFF : current direction: increasing
													//  |   |   |   |          ON  : current direction: decreasing
													//  |   |   |   |
													//  |   |   |   +--------> DIRECTION :: SINGLE
													//	|	|	|			   OFF : play, track end not reached
													//	|	|	|			   ON  : track end reached
													//	|	|	|
													//	|	|	+------------> CHORD
													//	|	|				   Stores one of the 7 track chord flags
													//	|	|				   as a numeric value -> 0-6.
													//	|	|
													//	|	+----------------> REC
													//						   1 means that recording is enabled on this track
													//	|
													//	+--------------------> EFF VALUE ::
													//						   OFF 	   : EFF not active
													//						   SEND    : Send track values into EFF pool
													//						   RECEIVE : be influenced by the EFF pool

	//	#define		[ATTR_CCAMT]				13		// This seems to be a duplicate of AMOUNT, hence empty space

	//	#define 	[ATTR_G_master_tempoMUL]				14		// Stores the track tempo multiplier as numeric value.
													// Valid values are as follows:
													// 0	- 	Track is paused
													// 1	-	1x Master tempo
													// 2	-	2x
													// 3	-	3x
													// 4	-	4x
													// 5	-	5x
													// 12	-	1/2x
													// 14	-	1/4x

	//	#define		[ATTR_G_master_tempoMUL_SKIP]		15		// Counter used by the fractional G_master_tempoMULs
													// |_|_|_|_|_|_|_|_|
													//	7 6 5 4 3 2 1 0
													//  | | | | + + + +------> SET SKIPPER VALUE
													//  + + + +--------------> CURRENT SKIPPER VALUE

typedef struct trackstruct {

	// Unique track ID.
	trackid_t trackId;

	// Makes track attributes accessible by #defined labels. See legend above
//	signed char attribute[ TRACK_NROF_ATTRIBUTES - 9 ];

	unsigned char attr_STATUS;
	unsigned char attr_VEL;
	unsigned char attr_PIT;
	unsigned char attr_LEN;
	unsigned char attr_STA;
	unsigned char attr_POS;
	unsigned char attr_DIR;
	unsigned char attr_AMT;
	unsigned char attr_GRV;
	  signed char attr_MCC;
	unsigned char attr_MCH;
	unsigned char attr_LOCATOR;
	unsigned char attr_MISC;
	unsigned char attr_CCAMT;
	unsigned char attr_TEMPOMUL;
	unsigned char attr_TEMPOMUL_SKIP;

	// A value of 0 means none is active, the rest is 1-128
	unsigned char program_change;
	unsigned char bank_change;

	// Holds the trigger data for the track: (pg. 331 book)
	// A direction consists of frames. Each frame contains 0-9 position triggers.
	// The track frame index points to the current frame for the track
	unsigned char frame_ndx;

	// Copy of the trigger set from the direction array
	unsigned char trigger[10];

	// Holds the chain data in array: HEAD (0), NEXT (1), PREV (2), PLAY(4)
	// PLAY indicates which track in the chain is playing.
	struct trackstruct* chain_data[4];


	// Factors for the track - factoring the step attributes maps
	unsigned char VEL_factor;
	unsigned char PIT_factor;
	unsigned char LEN_factor;
	unsigned char STA_factor;
	unsigned char AMT_factor;
	unsigned char GRV_factor;
	unsigned char MCC_factor;

	// Event offset and range (max) values for the track - acting on the factors
	unsigned char event_offset	[ TRACK_NROF_ATTRIBUTES ];
	unsigned char event_max		[ TRACK_NROF_ATTRIBUTES ];

	// Each track has a TTC enabling it to basically run its own clock.
	unsigned char TTC;

	// Stores the pitch offsets, which get produced by changing the scale and the lead
	signed char scale_pitch_offset;
	signed char lead_pitch_offset;

	unsigned char CC_resolution;

	// Coordinates of the hyperstep: Row (MS 4 bits) Col (LS 4 bits)
	unsigned char hyper;
	// The gate size is the length of the hyperstep.
	// The gatePosition is the number of gate ticks already executed.
	// NOT PERSISTENT, runtime only member.
	unsigned char gatePosition;

	// Holds the shapeindex of the flows per attribute
	unsigned char flow_shape[10];
	#ifdef FEATURE_ENABLE_KEYB_TRANSPOSE
	unsigned char attr_EMISC;			//ephemeral misc
	unsigned char attr_GST; 			//PIT ghost (on track select) attribute
	// attr_STATUS is transpose in channel
	#endif
	
	#ifdef FEATURE_ENABLE_SONG_UPE
	unsigned char ctrl_offset;		//used for control track event offset
	#endif
} Trackstruct;








// =====================================================================================
// Runtime page struct
// =====================================================================================

typedef struct pagestruct{

 	// Bitpattern that allows tracks to be visible and accessible in Nemo pages
	unsigned char	track_window;
	// Shift distance for the track window 1 = Rows (1-4) 2 = (5-8)
	unsigned char 	track_window_shift;


	// Implement the morph block in the page
	unsigned char 	morph_level;		// Level is selected by the user - between 0 and 7
	unsigned char	morph_stage;		// Stage is computed as we go along, between 0 and level
	unsigned char 	morph_direction;	// Single trigger, ping pong
	unsigned char 	morph_source_ndx;
	unsigned char 	morph_target_ndx;
	unsigned char 	morph_source_VEL;
	unsigned char 	morph_source_PIT;

	// Indicates the playmode of the chains in the page
 	unsigned char 	CHAINS_PLAY_HEAD;

	// PAGE contains an array of pointers to Track structures - mainly attribute collections
	Trackstruct* 	Track[MATRIX_NROF_ROWS];

	// MATRIX: Page contains an array of Step structures grouped in the Matrix
	Stepstruct* 	Step[MATRIX_NROF_ROWS][MATRIX_NROF_COLUMNS];

	// This holds the locator value. Int because of higher resolution scenarios
	unsigned int 	locator;

	// CONTROL, MIX2EDIT or EDIT - indicates what the Edit knobs are doing..
	unsigned char 	editorMode;

	// Holds the index of the page in the page repository
	unsigned char 	pageNdx;

	// indicates whether the page is cleared or contains some data. ON means cleared, OFF is dirty
	unsigned char 	page_clear;

	// Defines the track chains and splits in the page (book pg. 146)
	unsigned char 	chainMode;

	unsigned short 	trackSelection;		// Keeps the bitmap of selected tracks pattern
	unsigned short 	trackSelectionStored; // bitmap of previous trackSelection (one step rollback)

	unsigned char 	stepSelection; 			// Keeps count of the number of steps that are selected in the page
	unsigned char 	stepSelectionSingleRow;	// Keeps row coordinate of a selected single step
	unsigned char 	stepSelectionSingleCol;	// Keeps col coordinate of a selected single step
	Stepstruct* 	selected_step;

	unsigned short 	trackMutepattern;	// Keeps the bitmap the muted tracks pattern
	unsigned short 	trackMutepatternStored; // one step rollback - previous track Selection

	unsigned short 	trackSolopattern;	// Keeps the bitmap of the soloed tracks

	unsigned short 	priv_track_REC_pattern; 	// Keeps the bitmap of tracks set to REC mode
	unsigned char	REC_bit;			// Bit flag for step recording Duncan style, and other additionals..

	unsigned char 	editAttribute;		// Attribute selected for edit with the EDIT rotary knobs
	unsigned char 	mixAttribute;		// Attribute selected for edit with the MIX rotary knobs
	unsigned char 	mixMode;			// INC: mixer knob increases value of its track. DEC: decreases value of all other tracks
	unsigned char 	mixMasterStatus;	// POS or NEG: defines behavior of the MIXER
	unsigned char 	mixingTrack;		// Stores the index of the track that has been mixed last

	unsigned int 	trackAttributeSelection; // The track attribute selected in TRACK zoom mode - input for map - bitmap!!

	unsigned int 	stepAttributeSelection;	// The step attribute selected in STEP zoom mode - input for map - bitmap!!

	// Force-to-scale flag variables
	unsigned char 	force_to_scale;		// Indicates whether page is to be forced to its scale or not. May be reG_scan_cycled as force_to_scale bit

	unsigned char 	maxGroove;			// Indicates the highest groove value in Page

	unsigned char 	mixTarget;			// This can be one of MIXTGT_ATTRIBUTE, MIXTGT_VOLUME, MIXTGT_PAN, etc..

	// Store Preset MCC Amount for each track
	unsigned char 	MIXAMT_VOL[10];
	unsigned char 	MIXAMT_PAN[10];
	unsigned char 	MIXAMT_MOD[10];
	unsigned char 	MIXAMT_EXP[10];
	unsigned char 	MIXAMT_USR[10];

	// This stores the CC_MIXMAP data
	// Array built like this: [NROF_MAPS][NROF_ROWS][NROF_ATTRIBUTES]
	// The values are:         6 maps (for now) 10 (rows -easy)  3(0-AMT, 1-MCC, 2-MCH - may be expanded later)
	unsigned char 	CC_MIXMAP[6][10][3];
	unsigned char 	CC_MIXMAP_attribute;		// Indicates which attribute is shown for CC_MIXMAPS

	// Keeps the scale status of the page:
	// OFF: 		don't worry about scales
	// ON:			scale mode is active
	// SELECTED: 	scale is defined (cardinality and degrees
	// ACTIVE:		scale applies to the page
	unsigned char 	scaleStatus;

	// Stores the selected notes that make up the scale. Needs 12x2 = 24 bits of storage.
	// These are BITPATTERNS. Should work Faster and clearer
	// There are nine pairs that may be used - and eventually selected via numeric quadrant
	unsigned int 	scaleNotes[9];
	unsigned int 	scaleLead[9];

	// Temporary infrastructure variable - stores the scale lead temporarily, used for
	// ..double click scenario, where the lead needs to be reverted to previous one.
	unsigned int 	scaleLead_old;
	unsigned int 	scaleNotes_old;

	// Keeps the signature of the last selected, non-preset scale
	unsigned int 	my_scale_signature;

	// Holds the number of repetitions in GRID mode
	unsigned char 	repeats_left;

	// Store page specific attributes. The four key ones.
	unsigned char 	attr_VEL;
	unsigned char 	attr_PIT;
	unsigned char 	attr_LEN;
	unsigned char 	attr_STA;

	// Store the page length in steps. This is the master lenght for the size,
	// as it indicates how many steps we stay in page for before an oclock switch
	// takes place.

	// This is the mode in which the page reacts to key presses in the selector block.
	// Pretty ghetto for now I would think, but somehow working.
	unsigned char 	OPS_mode;

	// EFF pool - this is the modulation value pool to be built by the EFF send tracks..
	// ..to influence the EFF receive tracks. Pool is actualized with each page iteration
	signed char 	EFF_pool[ STEP_NROF_ATTRIBUTES ];

	// mix2edit attribute[] index and corresponding min and max values of stored attribute
	unsigned char 	attr_mix2edit;
	signed char 	attr_mix2edit_MIN;
	signed char		attr_mix2edit_MAX;

	// Indicator of the Program mode in SCL
	unsigned char SCL_align;

	// Indicator of the SEL LOCK status
	unsigned char SEL_LOCK;

	// There are 5 step selection patterns per page that may be selected
	unsigned char stepSELpattern_ndx;

	// Store of the step selection patterns in the page. 16 bits per track.
	unsigned short stepSELpattern[5][ MATRIX_NROF_ROWS ];
	#ifdef FEATURE_ENABLE_KEYB_TRANSPOSE
	// Transpose pitch absolute mode toggle
	unsigned char pitch_abs;
	#endif


} Pagestruct;


// =====================================================================================
// Runtime note recording struct
// =====================================================================================

typedef struct notestruct{

	unsigned int 	chord_up;
	unsigned short  chord_data;
	  signed char	attr_VEL;
	  signed char	attr_PIT;
	unsigned char	attr_LEN;
	unsigned char	attr_STA;

} Notestruct;

typedef struct recstruct{

	Notestruct*		Note[MAX_NROF_REC_MEASURES];
	unsigned char	measure_count;

} Recstruct;



#ifdef FEATURE_ENABLE_DICE
extern unsigned char 	SEL_DICE_BANK;
extern Pagestruct* 		DICE_bank;
#endif

// =====================================================================================
// Runtime data structure repositories
// =====================================================================================

extern Stepstruct 			Step_repository[MAX_NROF_STEPS];
extern Trackstruct 			Track_repository[MAX_NROF_TRACKS];
extern Pagestruct 			Page_repository[MAX_NROF_PAGES];
extern Track_Direction 		direction_repository[];
extern Recstruct			Rec_repository[16];




// =====================================================================================
// Declarations that represent the global Grid 'struct'
// =====================================================================================

// From grid.h.
extern unsigned char 	Page_GRID[2][10][3];
extern unsigned char 	GRID_status;
extern unsigned char	GRID_scene;
extern unsigned char 	GRID_target_scene;
extern unsigned char	GRID_switch_mode;
extern unsigned short	GRID_bank_playmodes;
extern Pagestruct* 	GRID_p_selection 		[GRID_NROF_BANKS];
extern Pagestruct* 	GRID_p_preselection	  	[GRID_NROF_BANKS];
extern Pagestruct* 	GRID_p_clock_presel	  	[GRID_NROF_BANKS];
extern Pagestruct* 	GRID_p_set				[GRID_NROF_SETS ][GRID_NROF_BANKS]; // Stores the GRID sets
extern unsigned char   GRID_p_set_note_offsets	[GRID_NROF_SETS ];
extern unsigned char   GRID_p_set_midi_ch;
extern unsigned char   GRID_p_selection_cluster;
#ifdef FEATURE_ENABLE_SONG_UPE
extern char			GRID_p_set_note_presel;
#endif
#ifdef FEATURE_ENABLE_SONG_UPE
extern Pagestruct* 	GRID_p_selection_buffer [GRID_NROF_BANKS];
#else
extern Pagestruct* 	GRID_p_selection_buffer [MATRIX_NROF_ROWS];
#endif
extern unsigned char 	GRID_CURSOR;
extern unsigned short 	GRID_mixAttribute;
extern unsigned char 	GRID_play_mode;
extern unsigned char 	GRID_bigknob_SELECT;
extern unsigned int GRID_set_switchmode;
extern unsigned char current_GRID_set;
extern unsigned char GRID_OPS_mode;
extern unsigned char GRIDTRACK_OPS_mode;
extern unsigned char GRID_MAP0_status;
extern unsigned char SEQUENCER_JUST_STARTED;
extern unsigned char stepDefaultAttrValue[STEP_NROF_ATTRIBUTES];
extern unsigned int TRACK_COPY_BUFFER;
extern unsigned int PAGE_COPY_BUFFER;
extern unsigned short ATTR_MAP_COPY_BUFFER[11];
extern unsigned char CLOCK_MASTER_UART;
extern unsigned char GRID_rowzero_pagelength;
extern Trackstruct* Track_playlist[ MATRIX_NROF_ROWS ];
extern unsigned char CCMAP_learner;
extern unsigned char CHANGE_BANK_OR_PGM;
extern unsigned char SYSEX_dump_speed;
extern Stepstruct* page_preview_step;
extern unsigned char page_preview_step_col;
extern unsigned char page_preview_step_row;
extern unsigned char track_tempo_mul;
extern unsigned char track_tempo_div;
extern Trackstruct*  track_tempo_ptr;
extern Pagestruct*   track_tempo_page;
extern unsigned short GRID_mutepattern;
extern unsigned char GRID_editmode;
extern unsigned char GRIDTRACK_editmode;
extern unsigned char CC_resolution_delay[8][6];
extern unsigned char MIDICLOCK_PASSTHROUGH;
#ifdef NEMO
extern unsigned short NEMO_selectedTrackAttribute;
extern unsigned short NEMO_selectedStepAttribute;
extern unsigned char NEMO_track_VFF;
extern unsigned char NEMO_step_VER;
#endif
#ifdef FEATURE_ENABLE_DICE
extern unsigned char dice_synced_attr;
extern unsigned char dice_synced_value;
#endif
// From variables.h
extern unsigned char follow_flag;
extern unsigned char G_master_tempo;
extern unsigned char G_clock_source;
extern unsigned int G_TIMER_REFILL;
extern unsigned char G_zoom_level;
extern signed char SOLO_strum;

extern unsigned char G_midi_map_controller_mode;
extern unsigned char G_MIDI_B_priority;
extern unsigned char G_TT_external_latency_offset;
extern unsigned char G_flashgridheadersonly_flag;
extern unsigned char G_initZoom;

// From init_memory.h
extern Pagestruct* GRID_assistant_page;

// Shift distance for the track window 1 = Rows (1-4) 2 = (5-8)
unsigned char	page_window;

// =====================================================================================
// End of grid declarations
// =====================================================================================






#endif /*RUNTIME_H_*/
