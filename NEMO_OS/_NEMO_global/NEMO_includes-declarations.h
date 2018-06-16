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

#ifndef INCLUDESDECLARATIONS_H_
#define INCLUDESDECLARATIONS_H_

// ===============================================================================================
// 	                                      DECLARATIONS ONLY!
// ===============================================================================================


//
// PLATFORM INDEPENDENT INCLUDE FILES
//
#include <stdio.h>
#include <stdlib.h>
#include <cyg/infra/diag.h> // needed for d_iag_printf()

//
// PLATFORM SPECIFIC INCLUDE FILES
//
#include <cyg/kernel/kapi.h>
#include <cyg/hal/hal_io.h>
#include <cyg/io/flash.h>

#include HEADER(_OCT_global/sts.h)
#include HEADER(_OCT_global/types-general.h)
#include "../_NEMO_global/NEMO_defs_general.h"

#include HEADER(_OCT_objects/Direction.h)
#include HEADER(_OCT_objects/Runtime.h)
#include HEADER(_OCT_global/function_signatures.h)
#include HEADER(_OCT_global/variables-declarations.h)
#include HEADER(_OCT_global/flash-block.h)
#include HEADER(_OCT_interrupts/cpu-load.h)
#include HEADER(_OCT_objects/Phrase.h)

#endif /*INCLUDESDECLARATIONS_H_*/



