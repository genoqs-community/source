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


#include "../_OCT_objects/PersistentV1.h"
#include "../_OCT_objects/PersistentV2.h"


/**
 * Exports the runtime Scene data members into the given persistent grid object.
 * This is used when writing the Scene object to outgoing sysex data.
 */
void PersSceneExport( card8* tarBufPt, size_t tarBufSizeBt )
{
	if ( !tarBufPt || (tarBufSizeBt < sizeof(PERSISTENT_CUR_SCENE_TP)) ) {
		return;
	}

	if ( tarBufSizeBt >= sizeof(ScenePersistentV2)) {
		PersistentV2_SceneExport( (ScenePersistentV2*) tarBufPt );
	}
}


/**
 * Imports from given persistent Scene object and updates the runtime Grid data members.
 * This is used for reading the scene object from incoming sysex data.
 */
void PersSceneImport( const card8* srcBufPt, size_t srcBufSizeBt )
{
	const PersistentHeaderTp* srcHdrPt = (const PersistentHeaderTp*) srcBufPt;
	if ( !srcHdrPt ) {
		return;
	}

	// Check persistent grid header magic values.
	if ( (srcHdrPt->magic1 != PERSISTENT_SCENE_MAGIC1) || (srcHdrPt->magic2 != PERSISTENT_SCENE_MAGIC2) ) {
		return;
	}

	// Check for data version younger than we can handle here.
	if ( srcHdrPt->version > PERSISTENT_CUR_DATA_VERSION ) {
		return;
	}

	if ( (srcHdrPt->version == 2) && (srcBufSizeBt >= sizeof(ScenePersistentV2)) ) {
		PersistentV2_SceneImport( (ScenePersistentV2*) srcBufPt );
	}

}



/**
 * Exports the runtime Grid data members into the given persistent grid object.
 * This is used when writing the Grid object to flash or for outgoing sysex data.
 *
 * @author Robert van der Kamp
 */
void PersGridExport( card8* tarBufPt, size_t tarBufSizeBt )
{
	if ( !tarBufPt || (tarBufSizeBt < sizeof(PERSISTENT_CUR_GRID_TP)) ) {
		return;
	}

	if ( tarBufSizeBt >= sizeof(GridPersistentV2)) {
		PersistentV2_GridExport( (GridPersistentV2*) tarBufPt );
	}
}


/**
 * Imports from given persistent Grid object and updates the runtime Grid data members.
 * This is used for reading the grid object from flash or from incoming sysex data.
 *
 * @author Robert van der Kamp
 */
void PersGridImport( const card8* srcBufPt, size_t srcBufSizeBt )
{
	const PersistentHeaderTp* srcHdrPt = (const PersistentHeaderTp*) srcBufPt;
	if ( !srcHdrPt ) {
		return;
	}

	// Check persistent grid header magic values.
	if ( (srcHdrPt->magic1 != PERSISTENT_GRID_MAGIC1) || (srcHdrPt->magic2 != PERSISTENT_GRID_MAGIC2) ) {
		return;
	}

	// Check for data version younger than we can handle here.
	if ( srcHdrPt->version > PERSISTENT_CUR_DATA_VERSION ) {
		return;
	}

	// Import persistent grid object with associated importer code.
	if ( (srcHdrPt->version == 1) && (srcBufSizeBt >= sizeof(GridPersistentV1)) ) {
		PersistentV1_GridImport( (GridPersistentV1*) srcBufPt );
	}
	else if ( (srcHdrPt->version == 2) && (srcBufSizeBt >= sizeof(GridPersistentV2)) ) {
		PersistentV2_GridImport( (GridPersistentV2*) srcBufPt );
	}
}



/**
 * Exports runtime page sourcePagePt into persistent targetPagePt.
 * This is used to write a page to flash or for outgoing sysex.
 *
 * @author Robert van der Kamp
 */
void PersPageExport( const Pagestruct* srcPagePt, card8* tarBufPt, size_t tarBufSizeBt )
{
	if ( !srcPagePt || !tarBufPt || (tarBufSizeBt < sizeof(PERSISTENT_CUR_PAGE_TP)) ) {
		return;
	}

	if ( tarBufSizeBt >= sizeof(PagePersistentV2) ) {
		PersistentV2_PageExport( srcPagePt, (PagePersistentV2*) tarBufPt );
	}
}


/**
 * Imports persistent sourcePagePt into the associated runtime targetPagePt.
 * This is used when reading from flash or from incoming sysex.
 *
 * @author Robert van der Kamp
 */
void PersPageImport( const card8* srcBufPt, size_t srcBufSizeBt, Pagestruct* tarPagePt )
{
	const PersistentHeaderTp* srcHdrPt = (const PersistentHeaderTp*) srcBufPt;
	if ( !srcHdrPt ) {
		return;
	}

	// Check page header magic.
	if ( (srcHdrPt->magic1 != PERSISTENT_PAGE_MAGIC1) || (srcHdrPt->magic2 != PERSISTENT_PAGE_MAGIC2) ) {
		return;
	}

	// Check for data version younger than we can handle here.
	if ( srcHdrPt->version > PERSISTENT_CUR_DATA_VERSION ) {
		return;
	}

	// Import persistent page object with associated imported code.
	if ( (srcHdrPt->version == 1) && (srcBufSizeBt >= sizeof(PagePersistentV1)) ) {
		PersistentV1_PageImport( (PagePersistentV1*) srcBufPt, tarPagePt );
	}
	else if ( (srcHdrPt->version == 2) && (srcBufSizeBt >= sizeof(PagePersistentV2)) ) {
		PersistentV2_PageImport( (PagePersistentV2*) srcBufPt, tarPagePt );
	}
}





