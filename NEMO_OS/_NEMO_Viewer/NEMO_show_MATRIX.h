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


	switch (content) {

		case STRUM_VALS:
			#include "NEMO_show_MATRIX__STRUM_VALS.h"
			break;

		case PAGES:
			#include "NEMO_show_MATRIX__PAGES.h"
			break;

		case GRIDTRACK:
			#include "NEMO_show_MATRIX__GRIDTRACK.h"
			break;

		case SYSEX:
			#include "NEMO_show_MATRIX__SYSEX.h"
			break;

		case GRID:
			#include "NEMO_show_MATRIX__GRID.h"
			break;

		case STEP_TOGGLE:
			#include "NEMO_show_MATRIX__STEP_TOGGLE.h"
			break;

		case STEP_SELECTION:
			#include "NEMO_show_MATRIX__STEP_SELECTION.h"
			break;

		case STEP_SELECTED:
			#include "NEMO_show_MATRIX__STEP_SELECTED.h"
			break;

		case STEP_SKIPS:
			#include "NEMO_show_MATRIX__STEP_SKIPS.h"
			break;

		case TRACK_SELECTION:
			#include "NEMO_show_MATRIX__TRACK_SELECTION.h"
			break;

		case LAUFLICHT:
			#include "NEMO_show_MATRIX__LAUFLICHT.h"
			break;

		case LAUFLICHT_TRACK:
			#include "NEMO_show_MATRIX__LAUFLICHT_TRACK.h"
			break;

		case LAUFLICHT_BOTTOM:
			#include "NEMO_show_MATRIX__LAUFLICHT_BOTTOM.h"
			break;

		case MIX_ATTRIBUTE_VALUES:
			#include "NEMO_show_MATRIX__MIX_ATTRIBUTE_VALUES.h"
			break;

		case HARDWIRED_ATTRIBUTE_VALUES:
			#include "NEMO_show_MATRIX__HARDWIRED_ATTRIBUTE_VALUES.h"
			break;

		case EXTENDED_ATTRIBUTE_VALUES:
			#include "NEMO_show_MATRIX__EXTENDED_ATTRIBUTE_VALUES.h"
			break;

		case ATTR_MAP_VALUES:
			#include "NEMO_show_MATRIX__ATTR_MAP_VALUES.h"
			break;


		case GRID_CC_MAP_VALS:
			#include "NEMO_show_MATRIX__GRID_CC_MAP_VALS.h"
			break;

		case MIXAMT_VALS:
			#include "NEMO_show_MATRIX__MIXAMT_VALS.h"
			break;

		case EDIT_ATTRIBUTE:
			#include "../_NEMO_Viewer/NEMO_show_MATRIX__EDIT_ATTRIBUTE.h"
			break;

		case STEP_ATTRIBUTE_VALUES:
			#include "NEMO_show_MATRIX__STEP_ATTRIBUTE_VALUES.h"
			break;

		case PREVIEW_STEP:
			#include "NEMO_show_MATRIX__PREVIEW_STEP.h"
			break;

	} // switch MATRIX content


