

// 
// PLATFORM INDEPENDENT INCLUDE FILES
//
#include <stdio.h>
#include <stdlib.h>
#include <cyg/infra/diag.h> // needed for d_iag_printf()

//
// PLATFORM SPECIFIC INCLUDE FILES
//
// Latest working version: E7T_eA_34_1_*
#include <d:\cygwin\home\Root\ecos_work\ecos_builds\E7T\E7T_eA_34_1_install\include\cyg\kernel\kapi.h>
#include <d:\cygwin\home\Root\ecos_work\ecos_builds\E7T\E7T_eA_34_1_install\include\cyg\hal\hal_io.h>
#include <d:\cygwin\home\Root\ecos_work\ecos_builds\E7T\E7T_eA_34_1_install\include\cyg\io\flash.h>

#include "../_OCT_global/function_signatures.h"

#include "../_OCT_global/defs_general.h"
#include "../_OCT_global/variables.h"
#include "../_OCT_global/tools_general.h"

#include "../_OCT_objects/Panel.h"

#include "../_OCT_objects/Step.h"
#include "../_OCT_objects/Track.h"
#include "../_OCT_objects/Page.h"
#include "../_OCT_objects/Page_flatrecord.h"
#include "../_OCT_objects/MIDI_queue.h"
#include "../_OCT_objects/NOTE_ON_queue.h"
#include "../_OCT_objects/Grid.h"

// #include "../_OCT_objects/Grid_flatrecord.h"
#include "../_OCT_init/Init_memory.h"

#include "../_OCT_Viewer/show__master.h"
#include "../_OCT_Player/play__master.h"
#include "../_OCT_Player/MIDI_IN_interpreter.h"

#include "../_OCT_interrupts/Intr_KEY.h"
#include "../_OCT_interrupts/Intr_ROTARY.h"
#include "../_OCT_interrupts/Intr_TMR.h"
#include "../_OCT_interrupts/Intr_IN_UARTs.h"

#include "../_OCT_exe_keys/key__master.h"
#include "../_OCT_exe_rots/rot__master.h"

#include "../_OCT_global/alarm_handlers.h"
#include "../_OCT_init/OS_infrastructure.h"
#include "../_OCT_global/sysex_functions.h"
#include "../_OCT_global/flash_functions.h"




