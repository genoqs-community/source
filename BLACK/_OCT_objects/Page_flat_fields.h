

	// Copy all the other page attributes
	// target_page->locator 				= OFF;
	target_page->editorMode					= source_page->editorMode;

//	target_page->bigKnobAmt					= source_page->bigKnobAmt;
	
	target_page->page_clear					= source_page->page_clear;
	
	// target_page->page_select				= OFF;
	// target_page->page_preselect			= OFF;
	target_page->keyMode					= source_page->keyMode;
	target_page->chainMode					= source_page->chainMode;
	target_page->chainCounter				= source_page->chainCounter;
	target_page->trackSelection				= source_page->trackSelection;
	target_page->trackSelectionStored		= source_page->trackSelectionStored;
	target_page->stepSelection				= source_page->stepSelection;
	target_page->stepSelectionSingleRow 	= source_page->stepSelectionSingleRow;
	target_page->stepSelectionSingleCol		= source_page->stepSelectionSingleCol;
	target_page->trackMutepattern			= source_page->trackMutepattern;
	target_page->trackMutepatternStored 	= source_page->trackMutepatternStored;
	target_page->trackSolopattern			= source_page->trackSolopattern;
	target_page->track_REC_pattern			= source_page->track_REC_pattern;
//	target_page->trackZoomAttribute			= source_page->trackZoomAttribute;
	target_page->editAttribute				= source_page->editAttribute;
	target_page->mixAttribute				= source_page->mixAttribute;
	target_page->mixMode					= source_page->mixMode;
	target_page->mixMasterStatus			= source_page->mixMasterStatus;
	target_page->mixingTrack				= source_page->mixingTrack;
	target_page->trackAttributeSelection	= source_page->trackAttributeSelection;
	target_page->stepAttributeSelection 	= source_page->stepAttributeSelection;
	target_page->force_to_scale				= source_page->force_to_scale;
	target_page->maxGroove					= source_page->maxGroove;
	target_page->mixTarget					= source_page->mixTarget;
	target_page->grid_FTS_exempt			= source_page->grid_FTS_exempt;
	target_page->CC_MIXMAP_attribute		= source_page->CC_MIXMAP_attribute;
	target_page->scaleStatus				= source_page->scaleStatus;
	target_page->current_scale				= source_page->current_scale;
	target_page->editMode					= source_page->editMode;
	target_page->recordMode					= source_page->recordMode;
//	target_page->cadenceStatus				= source_page->cadenceStatus;

	target_page->repeats					= source_page->repeats;
	

	// There are nine selectable user scales
	for ( i=0; i < 9; i++ ){

		target_page->scaleNotes[i]		= source_page->scaleNotes[i];
		target_page->scaleLead[i]		= source_page->scaleLead[i];
	}

	target_page->scaleLead_old				= source_page->scaleLead_old;
	target_page->scaleNotes_old				= source_page->scaleNotes_old;
	
	for (i=0; i<PAGE_NROF_ATTRIBUTES; i++) {
		target_page->attribute[i] 			= source_page->attribute[i];
	}

	for (i=0; i<10; i++) {
		target_page->MIXAMT_VOL[i] 			= source_page->MIXAMT_VOL[i];
		target_page->MIXAMT_PAN[i] 			= source_page->MIXAMT_PAN[i];
		target_page->MIXAMT_MOD[i] 			= source_page->MIXAMT_MOD[i];
		target_page->MIXAMT_EXP[i] 			= source_page->MIXAMT_EXP[i];
		target_page->MIXAMT_USR[i] 			= source_page->MIXAMT_USR[i];		

		for (j=0; j<6; j++) {
			for (k=0; k<3; k++) {
				target_page->CC_MIXMAP[j][i][k] = source_page->CC_MIXMAP[j][i][k];
			}
		}
	}

	target_page->attr_mix2edit 				= source_page->attr_mix2edit;
	target_page->attr_mix2edit_MIN			= source_page->attr_mix2edit_MIN;
	target_page->attr_mix2edit_MAX			= source_page->attr_mix2edit_MAX;
	
	target_page->SCL_align					= source_page->SCL_align;
	












