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


#ifndef INCLUDESDEFINITIONS_H_
#define INCLUDESDEFINITIONS_H_


//#include "../_OCT_global/variables.h"
#include HEADER(_OCT_global/variables.h)

//#include "../_OCT_global/tools_general.h"
#include HEADER(_OCT_global/tools_general.h)

// #include "../_OCT_objects/Panel.h"
// #include "../_OCT_objects/Step.h"
// #include "../_OCT_objects/Track.h"
// #include "../_OCT_objects/Page.h"
#include HEADER(_OCT_objects/Panel.h)
#include HEADER(_OCT_objects/Step.h)
#include HEADER(_OCT_objects/Track.h)
#include HEADER(_OCT_objects/Page.h)
#ifdef FEATURE_ENABLE_DICE
#include HEADER(_OCT_objects/Dice.h)
#endif
// #include "../_OCT_objects/MIDI_queue.h"
// #include "../_OCT_objects/NOTE_ON_queue.h"
// #include "../_OCT_objects/Grid.h"
// #include "../_OCT_objects/Ableton.h"
#include HEADER(_OCT_objects/MIDI_queue.h)
#include HEADER(_OCT_objects/NOTE_ON_queue.h)
#include HEADER(_OCT_objects/Grid.h)
#include HEADER(_OCT_objects/Ableton.h)

// #include "../_OCT_init/Init_memory.h"
#include HEADER(_OCT_init/Init_memory.h)


// #include "../_OCT_Viewer/show__master.h"
#include "../_NEMO_Viewer/NEMO_show__master.h"

// #include "../_OCT_Player/play__master.h"
#include HEADER(_OCT_Player/play__master.h)

// #include "../_OCT_Player/MIDI_IN_interpreter.h"
#include HEADER(_OCT_Player/MIDI_IN_interpreter.h)

// #include "../_OCT_interrupts/Intr_KEY.h"
// #include "../_OCT_interrupts/Intr_ROTARY.h"
#include "../_NEMO_interrupts/NEMO_Intr_KEY.h"
#include "../_NEMO_interrupts/NEMO_Intr_ROTARY.h"

// #include "../_OCT_interrupts/Intr_TMR.h"
// #include "../_OCT_interrupts/Intr_IN_UARTs.h"
// #include "../_OCT_exe_keys/key_clock_select.h"
#include HEADER(_OCT_interrupts/Intr_TMR.h)
#include HEADER(_OCT_interrupts/Intr_IN_UARTs.h)
#include HEADER(_OCT_exe_keys/key_clock_select.h)

// #include "../_OCT_exe_keys/key__master.h"
// #include "../_OCT_exe_rots/rot__master.h"
#include "../_NEMO_exe_keys/NEMO_key__master.h"
#include "../_NEMO_exe_rots/NEMO_rot__master.h"

// #include "../_OCT_global/alarm_handlers.h"
// #include "../_OCT_init/OS_infrastructure.h"
// #include "../_OCT_global/sysex_functions.h"
// #include "../_OCT_global/flash_functions.h"
#include HEADER(_OCT_global/alarm_handlers.h)
#include HEADER(_OCT_init/OS_infrastructure.h)
#include HEADER(_OCT_global/sysex_functions.h)
#include HEADER(_OCT_global/flash_functions.h)


#endif /*INCLUDESDEFINITIONS_H_*/

// ###############################################################################
// ###############################################################################

