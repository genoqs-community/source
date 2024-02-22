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

#ifndef PERSISTENT_H_
#define PERSISTENT_H_


#include "../_OCT_global/includes-declarations.h"
#include "../_OCT_objects/Runtime.h"
#include "../_OCT_objects/PersistentV1.h"
#include "../_OCT_objects/PersistentV2.h"

// ===============================================================================================
// 	                                      DECLARATIONS ONLY!
// ===============================================================================================

 // Current persistent data format version number.
#define			PERSISTENT_CUR_DATA_VERSION		2

#define 		PERSISTENT_CUR_GRID_TP 			GridPersistentV2
#define 		PERSISTENT_CUR_PAGE_TP			PagePersistentV2
#define 		PERSISTENT_CUR_SCENE_TP			ScenePersistentV2

#define			PERSISTENT_LARGEST_GRID_TP		GridPersistentV2
#define			PERSISTENT_LARGEST_PAGE_TP		PagePersistentV2
#define			PERSISTENT_LARGEST_SCENE_TP		ScenePersistentV2


 // Persistent page object magic. - JAVA
 #define		PERSISTENT_PAGE_MAGIC1			0xcafe
 #define		PERSISTENT_PAGE_MAGIC2			0xbabe

 // Persistent grid object magic. - IBM
 #define		PERSISTENT_GRID_MAGIC1			0xdead
 #define		PERSISTENT_GRID_MAGIC2			0xbeaf

 // Persistent scene object magic. - IRIX
 #define		PERSISTENT_SCENE_MAGIC1			0xdead
 #define		PERSISTENT_SCENE_MAGIC2			0xbabe

typedef struct {
	card16 		magic1;
	card16 		magic2;
	card16 		version;
	card16 		sizeBt;
	card16 		checkSum;			// Card16 checksum (simply adding) of all struct bytes, with this field initially set to 0.
} PersistentHeaderTp;





// Function prototypes.
void PersGridExport( card8* tarBufPt, size_t tarBufSizeBt );
void PersGridImport( const card8* srcBufPt, size_t srcBufSizeBt );
void PersPageExport( const Pagestruct* srcPagePt, card8* tarBufPt, size_t tarBufSizeBt );
void PersPageImport( const card8* srcBufPt, size_t srcBufSizeBt, Pagestruct* tarPagePt );
void PersSceneExport( card8* tarBufPt, size_t tarBufSizeBt );
void PersSceneImport( const card8* srcBufPt, size_t srcBufSizeBt );


#endif /*PERSISTENT_H_*/
