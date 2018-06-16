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

#include "../_OCT_objects/PersistentV2.h"


void PersistentV2_SceneExport( ScenePersistentV2* targetScenePt )
{
	int setIx, bankIx;

	//diag_printf( "scene-export to %p size=%ld ", targetScenePt, sizeof(ScenePersistentV2));

	// Init target scene buffer.
	PersistentV2_SceneInit( targetScenePt );


	// Set meta data fields.
	targetScenePt->magic1 		= PERSISTENT_SCENE_MAGIC1;
	targetScenePt->magic2 		= PERSISTENT_SCENE_MAGIC2;
	targetScenePt->sizeBt 		= sizeof(ScenePersistentV2);
	targetScenePt->version 		= 2;
	targetScenePt->sceneCheckSum = 0;
	#ifdef FEATURE_ENABLE_SONG_UPE
	targetScenePt->midiCh = GRID_p_set_midi_ch;
	#endif

	// Grid set repository - stores the actual grid sets
	for ( setIx = 0; setIx < GRID_NROF_SETS; setIx++ ) {
		#ifdef FEATURE_ENABLE_SONG_UPE
		targetScenePt->noteOffsets[setIx] = GRID_p_set_note_offsets[setIx];
		#endif
		for ( bankIx = 0; bankIx < GRID_NROF_BANKS; bankIx++ ) {
			pageid_t pageId = ~0;
			if ( GRID_p_set[setIx][bankIx] != NULL ) {
				pageId = GRID_p_set[setIx][bankIx]->pageNdx;
			}
			targetScenePt->GRID_p_id_set[setIx][bankIx] = pageId;
		}
	}

	// Calc and set checksum.
	card8* curPt = (card8*) targetScenePt;
	card8* endPtEx = curPt + sizeof(ScenePersistentV2);
	card16 checkSum = 0;
	while ( curPt < endPtEx ) {
		checkSum += *curPt++;
	}
	targetScenePt->sceneCheckSum = checkSum;

}



/**
 * Imports from given persistent Scene object and updates the runtime Grid data members.
 * This is used for reading the scene object from flash or from incoming sysex data.
 *
 * @author Gabriel Seher, Robert van der Kamp
 */
void PersistentV2_SceneImport( const ScenePersistentV2* sourceScenePt )
{
	int setIx, bankIx;

	//diag_printf( "scene-import from %p m1=%x m2=%x ", sourceScenePt, sourceScenePt->magic1, sourceScenePt->magic2 );

	// Check scene magic values.
	if ( sourceScenePt->magic1 != PERSISTENT_GRID_MAGIC1 || sourceScenePt->magic2 != PERSISTENT_SCENE_MAGIC2 ) {
		return;
	}

	// Check object size.
	if ( sourceScenePt->sizeBt != sizeof(ScenePersistentV2)) {
		return;
	}

	// Check format version number.
	if ( sourceScenePt->version != 2 ) {
		return;
	}


	// Do the checksum test.
	const card8* curPt 		= (const card8*) sourceScenePt;
	const card8* endPtEx 	= curPt + sizeof(ScenePersistentV2);
	card16 myCheckSum 		= 0;

	while ( curPt < endPtEx ) {
		myCheckSum += *curPt++;
	}
	curPt = (const card8*) &sourceScenePt->sceneCheckSum;
	myCheckSum -= *curPt++;
	myCheckSum -= *curPt++;

	if ( myCheckSum != sourceScenePt->sceneCheckSum ) {
		return;
	}

	#ifdef FEATURE_ENABLE_SONG_UPE
	GRID_p_set_midi_ch = sourceScenePt->midiCh;
	#endif

	// Grid set repository - stores the actual grid sets
	for ( setIx = 0; setIx < GRID_NROF_SETS; setIx++ ) {
		#ifdef FEATURE_ENABLE_SONG_UPE
		GRID_p_set_note_offsets[setIx] = sourceScenePt->noteOffsets[setIx];
		#endif
		for ( bankIx = 0; bankIx < GRID_NROF_BANKS; bankIx++ ) {
			GRID_p_set[setIx][bankIx] = NULL;
			pageid_t pageId = sourceScenePt->GRID_p_id_set[setIx][bankIx];
			if ( pageId < MAX_NROF_PAGES ) {
				GRID_p_set[setIx][bankIx] = &Page_repository[pageId];
			}
		}
	}

}




/**
 * Exports the runtime Grid data members into the given persistent grid object.
 * This is used when writing the Grid object to flash or for outgoing sysex data.
 *
 * @author Gabriel Seher, Robert van der Kamp
 */
void PersistentV2_GridExport( GridPersistentV2* targetGridPt )
{
	int setIx, bankIx, rowIx;

	//d_iag_printf( "grid-export to %p size=%ld ", targetGridPt, sizeof(GridPersistentV2));

	// Init target grid buffer.
	PersistentV2_GridInit( targetGridPt );


	// Set meta data fields.
	targetGridPt->magic1 		= PERSISTENT_GRID_MAGIC1;
	targetGridPt->magic2 		= PERSISTENT_GRID_MAGIC2;
	targetGridPt->sizeBt 		= sizeof(GridPersistentV2);
	targetGridPt->version 		= 2;
	targetGridPt->gridCheckSum 	= 0;


	// Export grid data members.
	targetGridPt->G_master_tempo = G_master_tempo; 	//120 default
	targetGridPt->G_TIMER_REFILL = G_TIMER_REFILL;
	targetGridPt->G_clock_source = G_clock_source;	// Can be any of OFF, INT(ernal), EXT(ernal)
	// Because zoom level is not used, I will override it to set the CC controller mode - Synth knobs, etc. or MIDI controller.
	targetGridPt->G_zoom_level = G_midi_map_controller_mode;
	targetGridPt->GRID_scene = GRID_scene;		// Currently selected grid scene for play or storage
	#ifdef FEATURE_ENABLE_SONG_UPE
	targetGridPt->midiCh = GRID_p_set_midi_ch;
	#endif

	// Grid set repository - stores the actual grid sets
	for ( setIx = 0; setIx < GRID_NROF_SETS; setIx++ ) {
		#ifdef FEATURE_ENABLE_SONG_UPE
		targetGridPt->noteOffsets[setIx] = GRID_p_set_note_offsets[setIx];
		#endif
		for ( bankIx = 0; bankIx < GRID_NROF_BANKS; bankIx++ ) {
			pageid_t pageId = ~0;
			if ( GRID_p_set[setIx][bankIx] != NULL ) {
				pageId = GRID_p_set[setIx][bankIx]->pageNdx;
			}
			targetGridPt->GRID_p_id_set[setIx][bankIx] = pageId;
		}
	}

	targetGridPt->GRID_switch_mode = GRID_switch_mode;
	targetGridPt->GRID_CURSOR = GRID_CURSOR;
	targetGridPt->GRID_mixAttribute = GRID_mixAttribute;
	targetGridPt->GRID_play_mode = GRID_play_mode;
	targetGridPt->GRID_bank_playmodes = GRID_bank_playmodes;		// either _SIMPLE or _CHAIN
	targetGridPt->GRID_set_switchmode = GRID_set_switchmode;
	targetGridPt->current_GRID_set = current_GRID_set;
	targetGridPt->GRID_OPS_mode = GRID_OPS_mode;
	targetGridPt->GRIDTRACK_OPS_mode = GRIDTRACK_OPS_mode;

	memcpy( targetGridPt->stepDefaultAttrValue,	&stepDefaultAttrValue, sizeof( unsigned char ) * STEP_NROF_ATTRIBUTES );

	targetGridPt->TRACK_COPY_BUFFER = TRACK_COPY_BUFFER;			// Stores selected tracks in a page as follows:
	targetGridPt->PAGE_COPY_BUFFER = PAGE_COPY_BUFFER;	// Stores the index of the page to be copied
	targetGridPt->follow_flag = follow_flag;

	for ( bankIx = 0; bankIx < GRID_NROF_BANKS; bankIx++ ) {
		pageid_t pageId = ~0;
		if ( GRID_p_selection[bankIx] != NULL ) {
			pageId = GRID_p_selection[bankIx]->pageNdx;
		}
		targetGridPt->GRID_p_id_selection[bankIx] = pageId;
	}

	for ( bankIx = 0; bankIx < GRID_NROF_BANKS; bankIx++ ) {
		pageid_t pageId = ~0;
		if ( GRID_p_preselection[bankIx] != NULL ) {
			pageId = GRID_p_preselection[bankIx]->pageNdx;
		}
		targetGridPt->GRID_p_id_preselection[bankIx] = pageId;
	}

	for ( bankIx = 0; bankIx < GRID_NROF_BANKS; bankIx++ ) {
		pageid_t pageId = ~0;
		if ( GRID_p_clock_presel[bankIx] != NULL ) {
			pageId = GRID_p_clock_presel[bankIx]->pageNdx;
		}
		targetGridPt->GRID_p_id_clock_presel[bankIx] = pageId;
	}

	for ( rowIx = 0; rowIx < MATRIX_NROF_ROWS; rowIx++ ) {
		pageid_t pageId = ~0;
		if ( GRID_p_selection_buffer[rowIx] != NULL ) {
			pageId = GRID_p_selection_buffer[rowIx]->pageNdx;
		}
		targetGridPt->GRID_p_id_selection_buffer[rowIx] = pageId;
	}

	// Export the runtime user phrase slots and remember how many phrases were actually exported.
	memcpy( targetGridPt->phraseRepos, PhraseRepos, PHRASE_COUNT_USER_RUNTIME * sizeof(PhraseStruct) );
	targetGridPt->phraseStoredCt = PHRASE_COUNT_USER_RUNTIME;

	// Export current phrase and note index for the editor.
	targetGridPt->phraseCurPhraseIx = PhraseCurPhraseIx;
	targetGridPt->phraseCurNoteIx = PhraseCurNoteIx;

	memcpy( targetGridPt->direction_repository,
			&direction_repository,
			sizeof( Track_Direction ) * 17 );

	targetGridPt->SYSEX_dump_speed = SYSEX_dump_speed;
	targetGridPt->GRID_editmode = GRID_editmode;
	targetGridPt->GRIDTRACK_editmode = GRIDTRACK_editmode;

	// Export grid assistent page, embedded in persistent grid object.
	PersistentV2_PageExport( GRID_assistant_page, &targetGridPt->assistant_page );



	// Calc and set checksum.
	card8* curPt = (card8*) targetGridPt;
	card8* endPtEx = curPt + sizeof(GridPersistentV2);
	card16 checkSum = 0;
	while ( curPt < endPtEx ) {
		checkSum += *curPt++;
	}
	targetGridPt->gridCheckSum = checkSum;


}




/**
 * Imports from given persistent Grid object and updates the runtime Grid data members.
 * This is used for reading the grid object from flash or from incoming sysex data.
 *
 * @author Gabriel Seher, Robert van der Kamp
 */
void PersistentV2_GridImport( const GridPersistentV2* sourceGridPt )
{
	int setIx, bankIx, rowIx;

	//d_iag_printf( "grid-import from %p m1=%x m2=%x ", sourceGridPt, sourceGridPt->magic1, sourceGridPt->magic2 );

	// Check page magic values.
	if ( sourceGridPt->magic1 != PERSISTENT_GRID_MAGIC1 || sourceGridPt->magic2 != PERSISTENT_GRID_MAGIC2 ) {
		return;
	}

	// Check object size.
	if ( sourceGridPt->sizeBt != sizeof(GridPersistentV2)) {
		return;
	}

	// Check format version number.
	if ( sourceGridPt->version != 2 ) {
		return;
	}


	// Do the checksum test.
	const card8* curPt 		= (const card8*) sourceGridPt;
	const card8* endPtEx 	= curPt + sizeof(GridPersistentV2);
	card16 myCheckSum 		= 0;

	while ( curPt < endPtEx ) {
		myCheckSum += *curPt++;
	}
	curPt = (const card8*) &sourceGridPt->gridCheckSum;
	myCheckSum -= *curPt++;
	myCheckSum -= *curPt++;

	if ( myCheckSum != sourceGridPt->gridCheckSum ) {
		return;
	}

	// Import grid data members.
	G_master_tempo = sourceGridPt->G_master_tempo; 	//120 default
	G_TIMER_REFILL = sourceGridPt->G_TIMER_REFILL;
	G_clock_source = sourceGridPt->G_clock_source;	// Can be any of OFF, INT(ernal), EXT(ernal)

	// Maintain current zoom level.
	// G_zoom_level = sourceGridPt->G_zoom_level;
	// Because zoom level is not used, I will override it to set the CC controller mode - Synth knobs, etc. or MIDI controller.
	G_midi_map_controller_mode = sourceGridPt->G_zoom_level;

	GRID_scene = sourceGridPt->GRID_scene;		// Currently selected grid scene for play or storage
	GRID_switch_mode = sourceGridPt->GRID_switch_mode;
	GRID_CURSOR = sourceGridPt->GRID_CURSOR;
	GRID_mixAttribute = sourceGridPt->GRID_mixAttribute;
	GRID_play_mode = sourceGridPt->GRID_play_mode;
	GRID_bank_playmodes = sourceGridPt->GRID_bank_playmodes;		// either _SIMPLE or _CHAIN
	GRID_set_switchmode = sourceGridPt->GRID_set_switchmode;
	current_GRID_set = sourceGridPt->current_GRID_set;
	#ifdef FEATURE_ENABLE_SONG_UPE
	GRID_p_set_midi_ch = sourceGridPt->midiCh;
	#endif

	// Grid set repository - stores the actual grid sets
	for ( setIx = 0; setIx < GRID_NROF_SETS; setIx++ ) {
		#ifdef FEATURE_ENABLE_SONG_UPE
		GRID_p_set_note_offsets[setIx] = sourceGridPt->noteOffsets[setIx];
		#endif
		for ( bankIx = 0; bankIx < GRID_NROF_BANKS; bankIx++ ) {
			GRID_p_set[setIx][bankIx] = NULL;
			pageid_t pageId = sourceGridPt->GRID_p_id_set[setIx][bankIx];
			if ( pageId < MAX_NROF_PAGES ) {
				GRID_p_set[setIx][bankIx] = &Page_repository[pageId];
			}
		}
	}

	GRID_OPS_mode = sourceGridPt->GRID_OPS_mode;
	GRIDTRACK_OPS_mode = sourceGridPt->GRIDTRACK_OPS_mode;

	memcpy( &stepDefaultAttrValue, sourceGridPt->stepDefaultAttrValue, sizeof( unsigned char ) * STEP_NROF_ATTRIBUTES );

	TRACK_COPY_BUFFER = sourceGridPt->TRACK_COPY_BUFFER;			// Stores selected tracks in a page as follows:
	PAGE_COPY_BUFFER = sourceGridPt->PAGE_COPY_BUFFER;	// Stores the index of the page to be copied
	follow_flag = sourceGridPt->follow_flag;

	// Grid snapshot copy
	for ( bankIx = 0; bankIx < GRID_NROF_BANKS; bankIx++ ) {
		GRID_p_selection[bankIx] = NULL;
		pageid_t pageId = sourceGridPt->GRID_p_id_selection[bankIx];
		if ( pageId < MAX_NROF_PAGES ) {
			GRID_p_selection[bankIx] = &Page_repository[pageId];
		}
	}

	for ( bankIx = 0; bankIx < GRID_NROF_BANKS; bankIx++ ) {
		GRID_p_preselection[bankIx] = NULL;
		pageid_t pageId = sourceGridPt->GRID_p_id_preselection[bankIx];
		if ( pageId < MAX_NROF_PAGES ) {
			GRID_p_preselection[bankIx] = &Page_repository[pageId];
		}
	}

	for ( bankIx = 0; bankIx < GRID_NROF_BANKS; bankIx++ ) {
		GRID_p_clock_presel[bankIx] = NULL;
		pageid_t pageId = sourceGridPt->GRID_p_id_clock_presel[bankIx];
		if ( pageId < MAX_NROF_PAGES ) {
			GRID_p_clock_presel[bankIx] = &Page_repository[pageId];
		}
	}

	for ( rowIx = 0; rowIx < MATRIX_NROF_ROWS; rowIx++ ) {
		GRID_p_selection_buffer[rowIx] = NULL;
		pageid_t pageId = sourceGridPt->GRID_p_id_selection_buffer[rowIx];
		if ( pageId < MAX_NROF_PAGES ) {
			GRID_p_selection_buffer[rowIx] = &Page_repository[pageId];
		}
	}

	// Get the number of phrases that can be imported. We may have stored more than this OS current can handle at runtime.
	card16 phraseCopyCt = min( sourceGridPt->phraseStoredCt, PHRASE_COUNT_USER_RUNTIME );

	// Import phraseCopyCt stored phrases into the user phrase repository.
	memcpy( PhraseRepos, sourceGridPt->phraseRepos, sizeof(PhraseStruct) * phraseCopyCt );

	// Import current phrase and phrase note index for phrase editor.
	PhraseCurPhraseIx = sourceGridPt->phraseCurPhraseIx;
	PhraseCurNoteIx = sourceGridPt->phraseCurNoteIx;

	memcpy( direction_repository,
			sourceGridPt->direction_repository,
			sizeof( Track_Direction ) * 17 );

	SYSEX_dump_speed = sourceGridPt->SYSEX_dump_speed;
	GRID_editmode 	= sourceGridPt->GRID_editmode;
	GRIDTRACK_editmode = sourceGridPt->GRIDTRACK_editmode;

	// Import assistent page embedded in persistent grid object.
	PersistentV2_PageImport( &sourceGridPt->assistant_page, GRID_assistant_page );
}





/**
 * Exports runtime page sourcePagePt into persistent targetPagePt.
 * This is used to write a page to flash or for outgoing sysex.
 *
 * @author Gabriel Seher, Robert van der Kamp
 */

void PersistentV2_PageExport( const Pagestruct* sourcePagePt, PagePersistentV2* targetPagePt)
{
	unsigned char i=0, j=0, k=0;

	PersistentV2_PageInit( targetPagePt );

	// Set meta data fields.
	targetPagePt->magic1 		= PERSISTENT_PAGE_MAGIC1;
	targetPagePt->magic2 		= PERSISTENT_PAGE_MAGIC2;
	targetPagePt->sizeBt 		= sizeof(PagePersistentV2);
	targetPagePt->version 		= 2;
	targetPagePt->pageCheckSum 	= 0;

	// Export the tracks and steps.
	for (i=0; i<MATRIX_NROF_ROWS; i++) {
		PersistentV2_TrackAndStepExport(sourcePagePt, i, targetPagePt);
	}

	// Export direct page data fields.
	targetPagePt->pageNdx					= sourcePagePt->pageNdx;
	targetPagePt->editorMode				= sourcePagePt->editorMode;
	targetPagePt->page_clear				= sourcePagePt->page_clear;
	targetPagePt->chainMode					= sourcePagePt->chainMode;
	targetPagePt->trackSelection			= sourcePagePt->trackSelection;
	targetPagePt->trackSelectionStored		= sourcePagePt->trackSelectionStored;
	targetPagePt->stepSelection				= sourcePagePt->stepSelection;
	targetPagePt->stepSelectionSingleRow 	= sourcePagePt->stepSelectionSingleRow;
	targetPagePt->stepSelectionSingleCol	= sourcePagePt->stepSelectionSingleCol;
	targetPagePt->trackMutepattern			= sourcePagePt->trackMutepattern;
	targetPagePt->trackMutepatternStored 	= sourcePagePt->trackMutepatternStored;
	targetPagePt->trackSolopattern			= sourcePagePt->trackSolopattern;
	targetPagePt->track_REC_pattern   		= Page_getTrackRecPattern(sourcePagePt);
	targetPagePt->editAttribute				= sourcePagePt->editAttribute;
	targetPagePt->mixAttribute				= sourcePagePt->mixAttribute;
	targetPagePt->mixMode					= sourcePagePt->mixMode;
	targetPagePt->mixMasterStatus			= sourcePagePt->mixMasterStatus;
	targetPagePt->mixingTrack				= sourcePagePt->mixingTrack;
	targetPagePt->trackAttributeSelection	= sourcePagePt->trackAttributeSelection;
	targetPagePt->stepAttributeSelection 	= sourcePagePt->stepAttributeSelection;
	targetPagePt->force_to_scale			= sourcePagePt->force_to_scale;
	targetPagePt->maxGroove					= sourcePagePt->maxGroove;
	targetPagePt->mixTarget					= sourcePagePt->mixTarget;
	targetPagePt->CC_MIXMAP_attribute		= sourcePagePt->CC_MIXMAP_attribute;
	targetPagePt->scaleStatus				= sourcePagePt->scaleStatus;

	for ( i=0; i < 9; i++ ){
		targetPagePt->scaleNotes[i]			= sourcePagePt->scaleNotes[i];
		targetPagePt->scaleLead[i]			= sourcePagePt->scaleLead[i];
	}

	targetPagePt->scaleLead_old				= sourcePagePt->scaleLead_old;
	targetPagePt->scaleNotes_old			= sourcePagePt->scaleNotes_old;
	targetPagePt->attr_VEL					= sourcePagePt->attr_VEL;
	targetPagePt->attr_PIT					= sourcePagePt->attr_PIT;
	targetPagePt->attr_LEN					= sourcePagePt->attr_LEN;
	targetPagePt->attr_STA					= sourcePagePt->attr_STA;
	targetPagePt->repeats_left				= sourcePagePt->repeats_left;

	for (i=0; i<10; i++) {
		targetPagePt->MIXAMT_VOL[i] 		= sourcePagePt->MIXAMT_VOL[i];
		targetPagePt->MIXAMT_PAN[i] 		= sourcePagePt->MIXAMT_PAN[i];
		targetPagePt->MIXAMT_MOD[i] 		= sourcePagePt->MIXAMT_MOD[i];
		targetPagePt->MIXAMT_EXP[i] 		= sourcePagePt->MIXAMT_EXP[i];
		targetPagePt->MIXAMT_USR[i] 		= sourcePagePt->MIXAMT_USR[i];

		for (j=0; j<6; j++) {
			for (k=0; k<3; k++) {
				targetPagePt->CC_MIXMAP[j][i][k] = sourcePagePt->CC_MIXMAP[j][i][k];
			}
		}
	}

	targetPagePt->attr_mix2edit 			= sourcePagePt->attr_mix2edit;
	targetPagePt->attr_mix2edit_MIN			= sourcePagePt->attr_mix2edit_MIN;
	targetPagePt->attr_mix2edit_MAX			= sourcePagePt->attr_mix2edit_MAX;
	targetPagePt->SCL_align					= sourcePagePt->SCL_align;
	targetPagePt->CHAINS_PLAY_HEAD			= sourcePagePt->CHAINS_PLAY_HEAD;


	// Calc and set checksum.
	card8* curPt = (card8*) targetPagePt;
	card8* endPtEx = curPt + sizeof(PagePersistentV2);
	card16 checkSum = 0;
	while ( curPt < endPtEx ) {
		checkSum += *curPt++;
	}
	targetPagePt->pageCheckSum = checkSum;
}




/**
 * Imports persisent sourcePagePt into the associated runtime targetPagePt.
 * This is used when reading from flash or from incoming sysex.
 *
 * @author Gabriel Seher, Robert van der Kamp
 */
void PersistentV2_PageImport(const PagePersistentV2* sourcePagePt, Pagestruct* targetPagePt)
{
	unsigned char i=0, j=0, k=0;

	//d_iag_printf( "page %d import ", sourcePagePt->pageNdx );

	// First off, clear the target page (includes tracks).
	if ( targetPagePt ) {
		Page_clear( targetPagePt );
	}

	// Check page magic values.
	if ( sourcePagePt->magic1 != PERSISTENT_PAGE_MAGIC1 || sourcePagePt->magic2 != PERSISTENT_PAGE_MAGIC2 ) {
		return;
	}

	// Check object size.
	if ( sourcePagePt->sizeBt != sizeof(PagePersistentV2)) {
		return;
	}

	// Check format version number.
	if ( sourcePagePt->version != 2 ) {
		return;
	}

	if ( sourcePagePt->pageNdx >= MAX_NROF_PAGES ) {
		return;
	}


	// Do the checksum test.
	const card8* curPt 		= (const card8*) sourcePagePt;
	const card8* endPtEx 	= curPt + sizeof(PagePersistentV2);
	card16 myCheckSum 		= 0;

	while ( curPt < endPtEx ) {
		myCheckSum += *curPt++;
	}
	curPt = (const card8*) &sourcePagePt->pageCheckSum;
	myCheckSum -= *curPt++;
	myCheckSum -= *curPt++;

	if ( myCheckSum != sourcePagePt->pageCheckSum ) {
		return;
	}



	if ( !targetPagePt ) {
		targetPagePt = &Page_repository[ sourcePagePt->pageNdx ];
		Page_clear( targetPagePt );
	}



	// Import the tracks and steps.
	for (i=0; i<MATRIX_NROF_ROWS; i++) {
		PersistentV2_TrackAndStepImport(targetPagePt, i, sourcePagePt);
	}

	// Import direct page data fields.
	targetPagePt->pageNdx					= sourcePagePt->pageNdx;
	targetPagePt->editorMode				= sourcePagePt->editorMode;
	targetPagePt->page_clear				= sourcePagePt->page_clear;
	targetPagePt->chainMode					= sourcePagePt->chainMode;
	targetPagePt->trackSelection			= sourcePagePt->trackSelection;
	targetPagePt->trackSelectionStored		= sourcePagePt->trackSelectionStored;
	targetPagePt->stepSelection				= sourcePagePt->stepSelection;
	targetPagePt->stepSelectionSingleRow 	= sourcePagePt->stepSelectionSingleRow;
	targetPagePt->stepSelectionSingleCol	= sourcePagePt->stepSelectionSingleCol;
	targetPagePt->trackMutepattern			= sourcePagePt->trackMutepattern;
	targetPagePt->trackMutepatternStored 	= sourcePagePt->trackMutepatternStored;
	targetPagePt->trackSolopattern			= sourcePagePt->trackSolopattern;
	Page_setTrackRecPattern(targetPagePt, sourcePagePt->track_REC_pattern);
	targetPagePt->editAttribute				= sourcePagePt->editAttribute;
	targetPagePt->mixAttribute				= sourcePagePt->mixAttribute;
	targetPagePt->mixMode					= sourcePagePt->mixMode;
	targetPagePt->mixMasterStatus			= sourcePagePt->mixMasterStatus;
	targetPagePt->mixingTrack				= sourcePagePt->mixingTrack;
	targetPagePt->trackAttributeSelection	= sourcePagePt->trackAttributeSelection;
	targetPagePt->stepAttributeSelection 	= sourcePagePt->stepAttributeSelection;
	targetPagePt->force_to_scale			= sourcePagePt->force_to_scale;
	targetPagePt->maxGroove					= sourcePagePt->maxGroove;
	targetPagePt->mixTarget					= sourcePagePt->mixTarget;
	targetPagePt->CC_MIXMAP_attribute		= sourcePagePt->CC_MIXMAP_attribute;
	targetPagePt->scaleStatus				= sourcePagePt->scaleStatus;

	for ( i=0; i < 9; i++ ){
		targetPagePt->scaleNotes[i]			= sourcePagePt->scaleNotes[i];
		targetPagePt->scaleLead[i]			= sourcePagePt->scaleLead[i];
	}

	targetPagePt->scaleLead_old				= sourcePagePt->scaleLead_old;
	targetPagePt->scaleNotes_old			= sourcePagePt->scaleNotes_old;
	targetPagePt->attr_VEL					= sourcePagePt->attr_VEL;
	targetPagePt->attr_PIT					= sourcePagePt->attr_PIT;
	targetPagePt->attr_LEN					= sourcePagePt->attr_LEN;
	targetPagePt->attr_STA					= sourcePagePt->attr_STA;
	targetPagePt->repeats_left				= sourcePagePt->repeats_left;

	for (i=0; i<10; i++) {
		targetPagePt->MIXAMT_VOL[i] 		= sourcePagePt->MIXAMT_VOL[i];
		targetPagePt->MIXAMT_PAN[i] 		= sourcePagePt->MIXAMT_PAN[i];
		targetPagePt->MIXAMT_MOD[i] 		= sourcePagePt->MIXAMT_MOD[i];
		targetPagePt->MIXAMT_EXP[i] 		= sourcePagePt->MIXAMT_EXP[i];
		targetPagePt->MIXAMT_USR[i] 		= sourcePagePt->MIXAMT_USR[i];

		for (j=0; j<6; j++) {
			for (k=0; k<3; k++) {
				targetPagePt->CC_MIXMAP[j][i][k] = sourcePagePt->CC_MIXMAP[j][i][k];
			}
		}
	}

	targetPagePt->attr_mix2edit 			= sourcePagePt->attr_mix2edit;
	targetPagePt->attr_mix2edit_MIN			= sourcePagePt->attr_mix2edit_MIN;
	targetPagePt->attr_mix2edit_MAX			= sourcePagePt->attr_mix2edit_MAX;
	targetPagePt->SCL_align					= sourcePagePt->SCL_align;
	targetPagePt->CHAINS_PLAY_HEAD			= sourcePagePt->CHAINS_PLAY_HEAD;
}






/**
 * Inits persistent scene object scenePt to a known state.
 */
void PersistentV2_SceneInit( ScenePersistentV2* scenePt )
{
	memset( scenePt, 0, sizeof( ScenePersistentV2 ) );
}




/**
 * Inits persistent grid object gridPt to a known state.
 *
 * @author Robert van der Kamp
 */
void PersistentV2_GridInit( GridPersistentV2* gridPt )
{
	memset( gridPt, 0, sizeof( GridPersistentV2 ) );
}




/**
 * Inits persistent page object pagePt to a known state.
 *
 * @author Gabriel Seher, Robert van der Kamp
 */
void PersistentV2_PageInit( PagePersistentV2* pagePt )
{
	unsigned char i=0, j=0;

	memset( pagePt, 0, sizeof(PagePersistentV2) );

	// Init the tracks and the steps
	for (i=0; i<MATRIX_NROF_ROWS; i++) {

		// Init the tracks.
		PersistentV2_TrackInit( &pagePt->Track[i] );

		// Init the steps
		for (j=0; j<MATRIX_NROF_COLUMNS; j++) {
			PersistentV2_StepInit( &pagePt->Step[i][j]);
		}
	}

	pagePt->page_clear = ON;
	pagePt->force_to_scale = FALSE;
	pagePt->attr_VEL = PAGE_DEF_VEL;
	pagePt->attr_PIT = PAGE_DEF_PIT;
	pagePt->attr_LEN = PAGE_DEF_LEN;
	pagePt->attr_STA = PAGE_DEF_STA;
	pagePt->OPS_mode = INTERACTIVE;
}



/**
 * Exports the track and step data from runtime sourcePagePt into persistent targetPagePt.
 * Used when writing a page to flash or for outgoing sysex data.
 *
 * @author Gabriel Seher, Robert van der Kamp
 */
void PersistentV2_TrackAndStepExport(const Pagestruct* sourcePagePt, card32 source_row, PagePersistentV2* targetPagePt)
{
	unsigned int i=0;

	const Trackstruct* sourceTrackPt = sourcePagePt->Track[source_row];
	TrackPersistentV2* targetTrackPt = &targetPagePt->Track[source_row];
	if ( !sourceTrackPt || !targetTrackPt ) {
		return;
	}

	// TRACK ID
	targetTrackPt->trackId = sourceTrackPt->trackId;

	// PROGRAM CHANGE
	targetTrackPt->program_change = sourceTrackPt->program_change;

	// BANK CHANGE
	targetTrackPt->bank_change = sourceTrackPt->bank_change;

	// CHAIN DATA
	targetTrackPt->chain_id_data[0]	= sourceTrackPt->chain_data[0] ? sourceTrackPt->chain_data[0]->trackId : ~0;
	targetTrackPt->chain_id_data[1] = sourceTrackPt->chain_data[1] ? sourceTrackPt->chain_data[1]->trackId : ~0;
	targetTrackPt->chain_id_data[2]	= sourceTrackPt->chain_data[2] ? sourceTrackPt->chain_data[2]->trackId : ~0;
	targetTrackPt->chain_id_data[3]	= sourceTrackPt->chain_data[3] ? sourceTrackPt->chain_data[3]->trackId : ~0;

	// Attribute factors
	targetTrackPt->VEL_factor = sourceTrackPt->VEL_factor;
	targetTrackPt->PIT_factor = sourceTrackPt->PIT_factor;
	targetTrackPt->LEN_factor = sourceTrackPt->LEN_factor;
	targetTrackPt->STA_factor = sourceTrackPt->STA_factor;
	targetTrackPt->AMT_factor = sourceTrackPt->AMT_factor;
	targetTrackPt->GRV_factor = sourceTrackPt->GRV_factor;
	targetTrackPt->MCC_factor = sourceTrackPt->MCC_factor;

	// ATTRIBUTES
	targetTrackPt->attr_STATUS = sourceTrackPt->attr_STATUS; // Not sure it is used anywhere..
	targetTrackPt->attr_VEL = sourceTrackPt->attr_VEL;
	targetTrackPt->attr_PIT = sourceTrackPt->attr_PIT;
	targetTrackPt->attr_LEN = sourceTrackPt->attr_LEN;
	targetTrackPt->attr_STA = sourceTrackPt->attr_STA;
	targetTrackPt->attr_POS = sourceTrackPt->attr_POS;
	targetTrackPt->attr_DIR = sourceTrackPt->attr_DIR;
	targetTrackPt->attr_AMT = sourceTrackPt->attr_AMT;
	targetTrackPt->attr_GRV = sourceTrackPt->attr_GRV;
	targetTrackPt->attr_MCC = sourceTrackPt->attr_MCC;
	targetTrackPt->attr_MCH = sourceTrackPt->attr_MCH;
	targetTrackPt->attr_LOCATOR = sourceTrackPt->attr_LOCATOR;
	targetTrackPt->attr_MISC = sourceTrackPt->attr_MISC;
	targetTrackPt->attr_CCAMT = sourceTrackPt->attr_CCAMT;
	targetTrackPt->attr_TEMPOMUL = sourceTrackPt->attr_TEMPOMUL;
	targetTrackPt->attr_TEMPOMUL_SKIP = sourceTrackPt->attr_TEMPOMUL_SKIP;
	targetTrackPt->CC_resolution = sourceTrackPt->CC_resolution;
	targetTrackPt->hyper = sourceTrackPt->hyper;

	// Attribute offset ranges
	for (i=0; i < TRACK_NROF_ATTRIBUTES; i++) {
		targetTrackPt->event_max[i] = sourceTrackPt->event_max[i];
	}

	#ifdef NEMO
	for ( i = 0; i < 10; i++ ) targetTrackPt->flow_shape[i] = sourceTrackPt->flow_shape[i];
	#else
	for ( i = 0; i < 10; i++ ) targetTrackPt->flow_shape[i] = 0;
	#endif

	// STEPS
	for (i=0; i<MATRIX_NROF_COLUMNS; i++) {
		PersistentV2_StepExport( sourcePagePt->Step[source_row][i],	&targetPagePt->Step[source_row][i]);
	}
}





/**
 * Imports the track and step data from persistent sourcePagePt into runtime targetPagePt.
 * Used when reading a page to flash or for incoming sysex data.
 *
 * @author Gabriel Seher, Robert van der Kamp
 */
void PersistentV2_TrackAndStepImport( Pagestruct* targetPagePt, card32 source_row, const PagePersistentV2* sourcePagePt )
{
	unsigned int i=0;

	const TrackPersistentV2* sourceTrackPt = &sourcePagePt->Track[source_row];
	Trackstruct* targetTrackPt = targetPagePt->Track[source_row];
	if ( !sourceTrackPt || !targetTrackPt) {
		return;
	}

	// Unique track IDs of soure and target should match. Note that target points into the track repository.
	if (targetTrackPt->trackId != sourceTrackPt->trackId) {
		return;
	}

	// PROGRAM CHANGE
	targetTrackPt->program_change = sourceTrackPt->program_change;

	// BANK CHANGE
	targetTrackPt->bank_change = sourceTrackPt->bank_change;

	// CHAIN DATA
	for (i = 0; i < 4; i++) {
		targetTrackPt->chain_data[i] = targetTrackPt;
		trackid_t trackId = sourceTrackPt->chain_id_data[i];
		if (trackId < MAX_NROF_TRACKS) {
			targetTrackPt->chain_data[i] = &Track_repository[trackId];
		}
	}

	// LEN, STA, VEL, PIT factors
	targetTrackPt->VEL_factor = sourceTrackPt->VEL_factor;
	targetTrackPt->PIT_factor = sourceTrackPt->PIT_factor;
	targetTrackPt->LEN_factor = sourceTrackPt->LEN_factor;
	targetTrackPt->STA_factor = sourceTrackPt->STA_factor;
	targetTrackPt->AMT_factor = sourceTrackPt->AMT_factor;
	targetTrackPt->GRV_factor = sourceTrackPt->GRV_factor;
	targetTrackPt->MCC_factor = sourceTrackPt->MCC_factor;

	// ATTRIBUTES
	targetTrackPt->attr_STATUS = sourceTrackPt->attr_STATUS; // Not sure it is used anywhere..
	targetTrackPt->attr_VEL = sourceTrackPt->attr_VEL;
	targetTrackPt->attr_PIT = sourceTrackPt->attr_PIT;
	targetTrackPt->attr_LEN = sourceTrackPt->attr_LEN;
	targetTrackPt->attr_STA = sourceTrackPt->attr_STA;
	targetTrackPt->attr_POS = sourceTrackPt->attr_POS;
	targetTrackPt->attr_DIR = sourceTrackPt->attr_DIR;
	targetTrackPt->attr_AMT = sourceTrackPt->attr_AMT;
	targetTrackPt->attr_GRV = sourceTrackPt->attr_GRV;
	targetTrackPt->attr_MCC = sourceTrackPt->attr_MCC;
	targetTrackPt->attr_MCH = sourceTrackPt->attr_MCH;
	targetTrackPt->attr_LOCATOR = sourceTrackPt->attr_LOCATOR;
	targetTrackPt->attr_MISC = sourceTrackPt->attr_MISC;
	targetTrackPt->attr_CCAMT = sourceTrackPt->attr_CCAMT;
	targetTrackPt->attr_TEMPOMUL = sourceTrackPt->attr_TEMPOMUL;
	targetTrackPt->attr_TEMPOMUL_SKIP = sourceTrackPt->attr_TEMPOMUL_SKIP;
	#ifdef FEATURE_ENABLE_DICE
	targetTrackPt->attr_pack_TEMPO = ( targetTrackPt->attr_TEMPOMUL == 25 ? 14 : targetTrackPt->attr_TEMPOMUL ) | ( targetTrackPt->attr_TEMPOMUL_SKIP << 4 );
	#endif
	targetTrackPt->CC_resolution = sourceTrackPt->CC_resolution;
	targetTrackPt->hyper = sourceTrackPt->hyper;
	targetTrackPt->gatePosition = 0; // Runtime data member.
	#ifdef FEATURE_ENABLE_KB_TRANSPOSE
	targetTrackPt->attr_GST = sourceTrackPt->attr_PIT;
	#endif
	// Attribute offset ranges
	for (i=0; i < TRACK_NROF_ATTRIBUTES; i++) {
		targetTrackPt->event_max[i] = sourceTrackPt->event_max[i];
	}

	// STEPS
	for (i=0; i<MATRIX_NROF_COLUMNS; i++) {
		PersistentV2_StepImport( &sourcePagePt->Step[source_row][i], targetPagePt->Step[source_row][i]);
	}

	#if NEMO
	for ( i = 0; i < 10; i++ ) targetTrackPt->flow_shape[i] = sourceTrackPt->flow_shape[i];
	#endif

}


/**
 * Inits persistent track targetTrackPt to a known state.
 *
 * @author Robert van der Kamp
 */
void PersistentV2_TrackInit( TrackPersistentV2* targetTrackPt )
{
	memset( targetTrackPt, 0, sizeof(TrackPersistentV2) );
	targetTrackPt->hyper = 0xF0;
}



/**
 * Inits persistent step object targetStepPt to a known state.
 *
 * @author Gabriel Seher, Robert van der Kamp
 */
void PersistentV2_StepInit( StepPersistentV2* targetStepPt )
{
	memset( targetStepPt, 0, sizeof(StepPersistentV2) );

	// Length multiplier of step (Dieter Herten)
	targetStepPt->event_data	= 	1 << 4;

	// Initialization sequence
	targetStepPt->attr_STATUS 	=	OFF;
	targetStepPt->attr_VEL 		=	STEP_DEF_VELOCITY;
	targetStepPt->attr_PIT		=	STEP_DEF_PITCH;
	targetStepPt->attr_LEN		= 	STEP_DEF_LENGTH;
	targetStepPt->attr_STA		= 	STEP_DEF_START;
	targetStepPt->attr_AMT		= 	STEP_DEF_AMOUNT;
	targetStepPt->attr_MCC		= 	STEP_DEF_MIDICC;

	// Step position attribute holds the LSB 7-bit nibble for the bender value, next to MIDICC
	targetStepPt->attr_POS	= 	STEP_DEF_POSITION;
	targetStepPt->hyperTrack_ndx =	10;
}



/**
 * Exports runtime step sourceStepPt into persistent step targetStepPt.
 *
 * @author Gabriel Seher, Robert van der Kamp
 */
void PersistentV2_StepExport( const Stepstruct* sourceStepPt, StepPersistentV2* targetStepPt ){

	targetStepPt->event_data = sourceStepPt->event_data;

	targetStepPt->chord_data = sourceStepPt->chord_data;
	targetStepPt->phrase_num = sourceStepPt->phrase_num;
	targetStepPt->phrase_data = sourceStepPt->phrase_data;
	targetStepPt->chord_up 	= sourceStepPt->chord_up;

	targetStepPt->attr_STATUS = sourceStepPt->attr_STATUS;

	targetStepPt->attr_VEL		= 	sourceStepPt->attr_VEL;
	targetStepPt->attr_PIT		= 	sourceStepPt->attr_PIT;
	targetStepPt->attr_LEN		=	sourceStepPt->attr_LEN;
	targetStepPt->attr_STA		=	sourceStepPt->attr_STA;
	targetStepPt->attr_POS		= 	sourceStepPt->attr_POS;
	targetStepPt->attr_AMT		=	sourceStepPt->attr_AMT;
	targetStepPt->attr_MCC		=	sourceStepPt->attr_MCC;

	targetStepPt->hyperTrack_ndx =	sourceStepPt->hyperTrack_ndx;
}


/**
 * Imports persistent step sourceStepPt into runtime step targetStepPt.
 *
 * @author Gabriel Seher, Robert van der Kamp
 */
void PersistentV2_StepImport( const StepPersistentV2* sourceStepPt, Stepstruct* targetStepPt )
{
	targetStepPt->event_data = sourceStepPt->event_data;

	targetStepPt->chord_data = sourceStepPt->chord_data;
	targetStepPt->phrase_num = sourceStepPt->phrase_num;
	targetStepPt->phrase_data = sourceStepPt->phrase_data;
	targetStepPt->chord_up 	= sourceStepPt->chord_up;

	targetStepPt->attr_STATUS = sourceStepPt->attr_STATUS;

	targetStepPt->attr_VEL		= 	sourceStepPt->attr_VEL;
	targetStepPt->attr_PIT		= 	sourceStepPt->attr_PIT;
	targetStepPt->attr_LEN		=	sourceStepPt->attr_LEN;
	targetStepPt->attr_STA		=	sourceStepPt->attr_STA;
	targetStepPt->attr_POS		= 	sourceStepPt->attr_POS;
	targetStepPt->attr_AMT		=	sourceStepPt->attr_AMT;
	targetStepPt->attr_MCC		=	sourceStepPt->attr_MCC;

	targetStepPt->hyperTrack_ndx =	sourceStepPt->hyperTrack_ndx;

}

