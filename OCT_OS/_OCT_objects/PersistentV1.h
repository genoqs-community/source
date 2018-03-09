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

#ifndef PERSISTENTV1_H_
#define PERSISTENTV1_H_


#include "../_OCT_global/includes-declarations.h"
#include "../_OCT_objects/Runtime.h"
#include "../_OCT_objects/Persistent.h"


// ===============================================================================================
// 	                                      DECLARATIONS ONLY!
// ===============================================================================================


typedef struct {
	card32 	chord_up;
	card16 	chord_data;
	card8	phrase_num;
	card8	phrase_data;
	card8 	event_data;
	card8 	attr_STATUS;
	int8	attr_VEL;
	int8	attr_PIT;
	card8	attr_LEN;
	card8	attr_STA;
	card8	attr_POS;
	int8	attr_AMT;
	int8 	attr_MCC;
} StepPersistentV1;



typedef struct {
	trackid_t 	trackId;

	card8 		attr_STATUS;		// Not sure it is used anywhere..
	card8 		attr_VEL;
	card8 		attr_PIT;
	card8 		attr_LEN;
	card8 		attr_STA;
	card8 		attr_POS;
	card8 		attr_DIR;
	card8 		attr_AMT;
	card8 		attr_GRV;
	int8  		attr_MCC;
	card8 		attr_MCH;
	card8 		attr_LOCATOR;
	card8 		attr_MISC;
	card8 		attr_CCAMT;
	card8 		attr_TEMPOMUL;
	card8 		attr_TEMPOMUL_SKIP;

	card8 		program_change;
	card8 		bank_change;
	card8 		frame_ndx;

	card8 		trigger[10];
	trackid_t 	chain_id_data[4];

	card8 		VEL_factor;
	card8 		PIT_factor;
	card8 		LEN_factor;
	card8 		STA_factor;
	card8 		AMT_factor;
	card8 		GRV_factor;
	card8 		MCC_factor;

	card8 		event_offset[ TRACK_NROF_ATTRIBUTES ];
	card8 		event_max[ TRACK_NROF_ATTRIBUTES ];

	card8 		TTC;
	int8 		scale_pitch_offset;
	int8 		lead_pitch_offset;
	card8 		CC_resolution;

	card8 		flow_shape[10];		// NEMO only.
	card8		fillerVc[32];

} TrackPersistentV1;



// Structure holds the data of a page. The indexes are sysex relevant
typedef struct {

	// Meta data.
	card16 				magic1;				// Must be PERSISTENT_PAGE_MAGIC1.
	card16 				magic2;				// Must be PERSISTENT_PAGE_MAGIC2.
	card16 				version;			// Must be 1.
	card16 				sizeBt;				// Size of struct PagePersistentV1.
	card16 				pageCheckSum;		// Card16 checksum (simply adding) of all struct bytes, with this field initially set to 0.

	TrackPersistentV1 	Track[ MATRIX_NROF_ROWS ];
	StepPersistentV1 	Step[ MATRIX_NROF_ROWS ][ MATRIX_NROF_COLUMNS ];

	card8 				pageNdx;
	card8 				page_clear;
	card8 				chainMode;
	card8 				editorMode;
	card8 				editAttribute;		// Attribute selected for edit with the EDIT rotary knobs
	card8 				force_to_scale;		// Page is to be forced to its scale?
	card8 				mixTarget;			// This can be one of MIXTGT_ATTRIBUTE, MIXTGT_VOLUME, MIXTGT_PAN, etc..

	card8 				MIXAMT_VOL[10];
	card8 				MIXAMT_PAN[10];
	card8 				MIXAMT_MOD[10];
	card8 				MIXAMT_EXP[10];
	card8 				MIXAMT_USR[10];

	card8 				CC_MIXMAP_attribute;		// Indicates which attribute is shown for CC_MIXMAPS
	card8 				CC_MIXMAP[6][10][3];

	card8 				scaleStatus;
	card32 				scaleNotes[9];
	card32 				scaleLead[9];
	card32 				scaleLead_old;
	card32 				scaleNotes_old;
	card32 				my_scale_signature;

	card8 				OPS_mode;

	card8 				attr_mix2edit;
	int8 				attr_mix2edit_MIN;
	int8				attr_mix2edit_MAX;

	card8 				SCL_align;

	card8 				attr_VEL;
	card8 				attr_PIT;
	card8 				attr_LEN;
	card8 				attr_STA;

	card8 				repeats_left;

	card8 				maxGroove;					// Indicates the highest groove value in Page
	card8 				mixAttribute;				// Attribute selected for edit with the MIX rotary knobs
	card8 				mixMode;					// INC: mixer knob increases value of its track. DEC: decreases value of all other tracks
	card8 				mixMasterStatus;			// POS or NEG: defines behavior of the MIXER
	card8 				mixingTrack;				// Stores the index of the track that has been mixed last
	card32 				trackAttributeSelection; 	// The track attribute selected in TRACK zoom mode - input for map - bitmap!!
	card32 				stepAttributeSelection;
	card16 				trackSelection;				// Keeps the bitmap of selected tracks pattern
	card16 				trackSelectionStored; 		// bitmap of previous trackSelection (one step rollback)
	card8 				stepSelection; 				// Keeps count of the number of steps that are selected in the page
	card8 				stepSelectionSingleRow;		// Keeps row coordinate of a selected single step
	card8 				stepSelectionSingleCol;		// Keeps col coordinate of a selected single step
	card16 				trackMutepattern;			// Keeps the bitmap the muted tracks pattern
	card16 				trackMutepatternStored; 	// one step rollback - previous track Selection
	card16 				trackSolopattern;			// Keeps the bitmap of the soloed tracks
	card16 				track_REC_pattern;			// Keeps the bitmap of the REC track status
	card8 				CHAINS_PLAY_HEAD;			// Determines play behaviour of track chains

	card8				fillerVc[100];

} PagePersistentV1;




// Trigger stack structure
typedef struct {
	card8 trigger[10];
	card8 certainty_next;
	card8 randomizer;
} TriggerStackPersistentV1;


// Direction structure
typedef struct {
	TriggerStackPersistentV1 trigger_set[17];
} TrackDirectionPersistentV1;




typedef struct {

	// Meta data.
	card16 		magic1;				// Must be PERSISTENT_GRID_MAGIC1.
	card16 		magic2;				// Must be PERSISTENT_GRID_MAGIC2.
	card16 		version;			// Must be 1.
	card16 		sizeBt;				// Size of struct GridPersistentV1.
	card16 		gridCheckSum;		// Card16 checksum (simply adding) of all struct bytes, with this field initially set to 0.

	card8 		G_master_tempo; 	//120 default
	card8		G_clock_source;		// Can be any of OFF, INT(ernal), EXT(ernal)
	card32 		G_TIMER_REFILL;
	card8 		G_zoom_level;
	card8		GRID_scene;			// Currently selected grid scene for play or storage
	card8		GRID_switch_mode;
	card16 		GRID_bank_triggermode[GRID_NROF_BANKS];
	card16 		GRID_CURSOR;
	card16 		GRID_mixAttribute;
	card8 		GRID_play_mode;
	card16		GRID_bank_playmodes;		// either _SIMPLE or _CHAIN
	card32 		GRID_set_switchmode;
	card8 		current_GRID_set;
	pageid_t 	GRID_p_id_set[GRID_NROF_SETS ][GRID_NROF_BANKS];
	card8 		GRID_OPS_mode;
	card8 		GRIDTRACK_OPS_mode;
	card8 		stepDefaultAttrValue[STEP_NROF_ATTRIBUTES];
	card32 		TRACK_COPY_BUFFER;			// Stores selected tracks in a page as follows:
	card32 		PAGE_COPY_BUFFER;	// Stores the index of the page to be copied
	card16 		ATTR_MAP_COPY_BUFFER[11];
	card8 		follow_flag;
	card8 		CHAINS_PLAY_HEAD;

	pageid_t 	GRID_p_id_selection[GRID_NROF_BANKS];  // Pointers of the pages that are active in the GRID at any time, one per GRID bank
	pageid_t 	GRID_p_id_preselection[GRID_NROF_BANKS];  // Pointers of the pages that are preselected in the GRID at any time, one per GRID bank
	pageid_t 	GRID_p_id_clock_presel[GRID_NROF_BANKS];  // Pointers of the pages that are preselected in the GRID at any time, one per GRID bank
	pageid_t 	GRID_p_id_selection_buffer[MATRIX_NROF_ROWS]; // Temporary buffer for solo operations and other things

	PhraseStruct	phraseRepos[PHRASE_COUNT_USER_PERSISTENT];
	card16			phraseStoredCt;
	card8			phraseCurPhraseIx;
	card8			phraseCurNoteIx;

	TrackDirectionPersistentV1 	direction_repository[17];
	PagePersistentV1 			assistant_page;

	card8 		SYSEX_dump_speed;
	card8 		GRID_editmode;
	card8 		GRIDTRACK_editmode;

	card8		fillerVc[1024];

} GridPersistentV1;









// Function prototypes.
extern void PersistentV1_GridExport( GridPersistentV1* targetGridPt );
extern void PersistentV1_GridImport( const GridPersistentV1* sourceGridPt );
extern void PersistentV1_PageExport( const Pagestruct* sourcePagePt, PagePersistentV1* targetPagePt);
extern void PersistentV1_PageImport(const PagePersistentV1* sourcePagePt, Pagestruct* targetPagePt);
extern void PersistentV1_GridInit( GridPersistentV1* gridPt );
extern void PersistentV1_PageInit( PagePersistentV1* pagePt );
extern void PersistentV1_TrackInit( TrackPersistentV1* targetTrackPt );
extern void PersistentV1_StepInit( StepPersistentV1* targetStepPt );
extern void PersistentV1_TrackAndStepExport(const Pagestruct* sourcePagePt, card32 source_row, PagePersistentV1* targetPagePt);
extern void PersistentV1_TrackAndStepImport( Pagestruct* targetPagePt, card32 source_row, const PagePersistentV1* sourcePagePt );
extern void PersistentV1_TrackAndStepImport( Pagestruct* targetPagePt, card32 source_row, const PagePersistentV1* sourcePagePt );
extern void PersistentV1_StepExport( const Stepstruct* sourceStepPt, StepPersistentV1* targetStepPt );
extern void PersistentV1_StepImport( const StepPersistentV1* sourceStepPt, Stepstruct* targetStepPt );




#endif /*PERSISTENTV1_H_*/
