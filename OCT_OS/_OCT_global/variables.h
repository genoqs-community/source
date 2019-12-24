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



/* OCTOPUS
 * Miscellaneous global variable definitions
 * 12.04.2004 genoQs Machines
 *
 */


//unsigned int MAX_NROF_PAGES = 160;
//// unsigned int MAX_NROF_TRACKS = 10 * MAX_NROF_PAGES;
//unsigned int MAX_NROF_TRACKS = 1600;
//// unsigned int MAX_NROF_STEPS	= 16 * MAX_NROF_TRACKS;
//unsigned int MAX_NROF_STEPS	= 25600;



// ************************
// GLOBAL VARIABLES SECTION
// ************************

unsigned char 		G_flashload_flag = LOAD_ON_POWERON;
unsigned char		G_flashgridheadersonly_flag = FALSE;

// SPAGHETTI from scan_keys
unsigned int 		G_scan[3][26];
unsigned int 		G_pressed_keys[261];

// Global variable indicating that a finger is on a key
unsigned int 		G_key_pressed = 0;
unsigned int 		G_scan_cycle = 0;

//
// CLOCK_FREQUENCY: 50 000 000 corresponds to 1 second
//
unsigned char 	 	G_master_tempo 		= DEF_TEMPO; 	//120 default
unsigned char		G_clock_source		= OFF;	// Can be any of OFF, INT(ernal), EXT(ernal)

unsigned char G_midi_map_controller_mode = ON;

// G_master_blinker is used to control LED blinking activities
unsigned char 		G_master_blinker		= 0;
unsigned char 		G_master_blinker2		= 0; // Used for the one current ZOOM LED showing TEMPO
unsigned char		G_MIDI_load_monitor1	= MIR_GREEN;
unsigned char 		G_MIDI_load_monitor2	= MIR_GREEN;
unsigned char 		G_CPU_load_monitor		= MIR_GREEN;

unsigned char		G_MIDI_B_priority		= FALSE;
unsigned char		G_initZoom				= FALSE;
unsigned char		G_LED_metronome			= FALSE;

// CPU load percentage, between 0-100%. We have overload at 100%.
unsigned char  		G_cpu_load_perc = 0;

// Max CPU load percentage built up over the current sequencer run.
unsigned char		G_cpu_load_max_perc = 0;

// The value (in timer ticks) that was used to start the timer, which is G_TIMER_REFILL, or when in external clock mode, a constant value.
unsigned int		G_cpu_load_timer_start_ticks = 1;

// The timer period, in timer ticks. This is G_TIMER_REFILL, or when in external clock mode, a measured value.
unsigned int		G_cpu_load_timer_period_ticks = 1;

// This counter indicates that driveSequencer() is running when > 0. Used to detect and protect sequencer overload.
int					G_sequencer_is_busy = 0;

// Used to count down to normal tempo -> G_master_tempo
unsigned char 		G_tempo_tracker		= 1;

// Absolute value of the TIMER_TICK_CTR in the 1xT and 2xT modes
unsigned char 		G_TTC_abs_value		= 0;

// STA amount subtracted from the timing of incoming MIDI notes to account for external hardware latency.
unsigned char		G_TT_external_latency_offset = 0;

// Total number of tracks currently set in record select. If just 1, auto rechanneling for record is enabled.
int					G_tracks_in_record_count = 0;

// 32 pitch segment from 0 to 3 NOTE_OFF messages
unsigned char		G_pitch_segment = 0;

// Assign data buffers for incoming MIDI data, plus counter to current data
#define 			def_MIDI1_IN_BUFFER_SIZE		100
unsigned int 		G_in_MIDI1[def_MIDI1_IN_BUFFER_SIZE];
unsigned char 		G_in_MIDI1_ndx = 0;

#define 			def_MIDI2_IN_BUFFER_SIZE		100
unsigned int 		G_in_MIDI2[def_MIDI2_IN_BUFFER_SIZE];
unsigned char 		G_in_MIDI2_ndx = 0;


// Assign data buffers for outgoing MIDI data, plus counter to current data
#define 			def_MIDI_OUT_BUFFER_SIZE		100
unsigned int 		G_out_MIDI[def_MIDI_OUT_BUFFER_SIZE];
unsigned char 		G_out_MIDI_ndx = 0;

#define 			def_MIDI2_OUT_BUFFER_SIZE		100
unsigned int 		G_out_MIDI2[def_MIDI2_OUT_BUFFER_SIZE];
unsigned char 		G_out_MIDI2_ndx = 0;


// Assign data buffers for incoming KEY data, plus counter to current data
#define 			def_KEY_IN_BUFFER_SIZE			20
unsigned int 		G_in_KEY[def_KEY_IN_BUFFER_SIZE];
unsigned char 		G_in_KEY_ndx = 0;


//Thread handles
// cyg_handle_t 	handle_thread_player_run;
cyg_handle_t 	handle_timer1_isr;
cyg_handle_t 	handle_ext0_isr;
cyg_handle_t 	handle_ext1_isr;
cyg_handle_t	handle_ext2_isr;
cyg_handle_t	handle_UART0_IN_isr;
cyg_handle_t	handle_UART1_IN_isr;

// TIMER interrupt
cyg_interrupt 	isr;

// KEY RAISING interrupt on EXT0
cyg_interrupt 	isr2;

// ROTARY interrupt on EXT1
cyg_interrupt 	isr3;

// KEY FALLING interrupt on EXT2
cyg_interrupt 	isr4;

// UART0-IN interrupt
cyg_interrupt	isr5;

// UART1-IN interrupt
cyg_interrupt	isr6;



//
// ALARM used for MIXER and EDITOR timeouts
//
cyg_handle_t		counter_hdl;
cyg_handle_t		sys_clk;
cyg_tick_count_t	ticks;

cyg_handle_t		alarm_hdl;
cyg_alarm_t			alarm_handler;
cyg_alarm			alarm_obj;
unsigned long 		alarm_index;

cyg_handle_t		doubleClickAlarm_hdl;
cyg_alarm_t			doubleClickAlarm_handler;
cyg_alarm			doubleClickAlarm_obj;
unsigned long 		doubleClickAlarm_index;

cyg_handle_t		alarmTV_hdl;
cyg_alarm_t			alarmTV_handler;
cyg_alarm			alarmTV_obj;
unsigned long 		alarmTV_index;

cyg_handle_t		quickturnAlarm_hdl;
cyg_alarm_t			quickturnAlarm_handler;
cyg_alarm			quickturnAlarm_obj;
unsigned long 		quickturnAlarm_index;

cyg_handle_t		MIDI1_overloadAlarm_hdl;
cyg_alarm_t			MIDI1_overloadAlarm_handler;
cyg_alarm			MIDI1_overloadAlarm_obj;
unsigned long 		MIDI1_overloadAlarm_index;

cyg_handle_t		MIDI2_overloadAlarm_hdl;
cyg_alarm_t			MIDI2_overloadAlarm_handler;
cyg_alarm			MIDI2_overloadAlarm_obj;
unsigned long 		MIDI2_overloadAlarm_index;

cyg_handle_t		CPU_overloadAlarm_hdl;
cyg_alarm_t			CPU_overloadAlarm_handler;
cyg_alarm			CPU_overloadAlarm_obj;
unsigned long 		CPU_overloadAlarm_index;

cyg_handle_t		page_refreshAlarm_hdl;
cyg_alarm_t			page_refreshAlarm_handler;
cyg_alarm			page_refreshAlarm_obj;
unsigned long 		page_refreshAlarm_index;

//
// SEMAPHORES
//
cyg_sem_t		sem_showPage;
cyg_sem_t		sem_timerInterrupt;
cyg_sem_t		sem_readKeys;
cyg_sem_t		sem_driveSequencer;
cyg_sem_t		sem_scanRots;
cyg_sem_t		sem_flashOps;


//
// MAILBOX HANDLES
//
cyg_handle_t 	keyNdx_mbox_handle;
cyg_handle_t 	rotNdx_mbox_handle;

// UART handles
cyg_handle_t	UART0_OUT_mbox_handle;
cyg_handle_t	UART0_IN_mbox_handle;

cyg_handle_t	UART1_OUT_mbox_handle;
cyg_handle_t	UART1_IN_mbox_handle;


// This indicates globally whether Octopus is running or not, or in Step record mode
unsigned char G_run_bit 		= OFF;
unsigned char G_pause_bit 		= OFF;
unsigned char G_track_page_chain_mod_bit = OFF;
unsigned char G_on_the_measure_operation = OFF;
unsigned short G_on_the_measure_trackMutepattern = 0;
unsigned char G_on_the_measure_trackMutepattern_pageNdx = 0;
Trackstruct* G_on_the_measure_track[MATRIX_NROF_ROWS];

unsigned char G_stop_bit		= OFF;

#ifdef FEATURE_ENABLE_SONG_UPE
unsigned char G_scroll_bit 		= OFF;
unsigned char prev_G_pause_bit  = OFF;
unsigned char prev_G_stop_bit 	= OFF;
unsigned char force_stop_bit	= OFF;
unsigned char G_align_bit		= OFF;
#endif
// This indicates the global track record mode.
unsigned char G_track_rec_bit = OFF;

// This is the midi counter flag
unsigned char G_MIDI_tick_counter = 0;

// Heartbeat flag
// unsigned char heart = 0;

// Define the panel status for the page
unsigned int G_PANEL_matrix[PANEL_NROF_OBJECTS][PANEL_NROF_ATTRIBUTES+1];

// Refill for 16th steps: (50Mhz/4steps_per_beat * 60Secs)/G_master_tempo
unsigned int G_TIMER_REFILL = 0;

// Mutex for the data pipe
cyg_mutex_t 	mutex_data_port;

// Mutex for print operations
cyg_mutex_t 	mutex_play_TTC;

// Mutex for the page refresh request variable
cyg_mutex_t 	mutex_pageRefreshReq;

// Mutex for write acccess to page data
cyg_mutex_t		mutex_PageWrite;

// Mutex for flash operations
cyg_mutex_t		mutex_FlashOperations;

// Mutex for read/write ops of sequencer data
cyg_mutex_t		mutex_sequencer_data;

// Set of mutexes to protect output MIDI queue and stack.
cyg_mutex_t		mutex_midi_output_stack;
cyg_mutex_t		mutex_midi_output_queue;


// Matrix Intermediate Representation. The first dimension (2) represents left and right.
// The second dimension (17) is the scaling factor. See MIR scheme table outside the code.
// The third dimension is for BLINK (0), RED (1), GREEN (2), RAINBOW_GREEN (3), ...
// ... and RAINBOW_RED (4) using RED and GREEN defines
unsigned char MIR  [2][17][5];
unsigned char DIAG [261];
unsigned char DIAG_rotary_current = 200;
unsigned char DIAG_rotary_value[21];

// This probably should be zoomGRID but may use zoomPAGE for convenience
unsigned char G_zoom_level = zoomPAGE;


// Edit timer used to temporarily switch between default and track EDIT mode.
unsigned char EDIT_TIMER 			= OFF;
unsigned char MIX_TIMER				= OFF;
unsigned char TEMPO_TIMER			= OFF; // used in nemo to display tempo
unsigned char PHRASE_TIMER			= OFF; // Used to show phrase number in outer circle.
unsigned char ROT_INDEX				= OFF;

// Double click control
unsigned char DOUBLE_CLICK_TIMER	= OFF;
unsigned int  DOUBLE_CLICK_TARGET	= 0;		//needs to be larger than 8 bits

// Track default pitches
unsigned char TRACK_DEFAULT_PITCH[10];


// MIDI OFF in PLAY mode
unsigned char PLAY_MIDI_OFF[3];

// Circle number field: hardwired position for now
unsigned int circleNrField[11] = { 201, 200, 199, 198, 197, 207, 206, 216, 215, 224, 233 };

// Values empirically established - see scaffold code in execute rotary code
unsigned char DOUBLE_CLICK_ALARM_TIME = 2;
unsigned char DOUBLE_CLICK_ALARM_RESOLUTION =12;	// 10 // Specifies the duration of double click
unsigned char DOUBLE_CLICK_ALARM_SENSITIVITY = 5;

// Values empirically established - see scaffold code in execute rotary code
unsigned char ROTARY_QUICKTURN_MULTIPLIER = 1;
unsigned char RQM_MAX = 5;
unsigned char RQM_INTERVAL = 1;

// Scaffold variable
unsigned char midi_clock_in_counter = 0;

// Global locator counter. Value range: [0..16]. 0 means OFF, others are running positions.
unsigned char G_global_locator = 0;



// FOLLOW variables - finally reduced to a radio button: FOLLOW_GRID, FOLLOW_PAGE, FOLLOW_TRACK
#ifdef NEMO
	unsigned char follow_flag = FOLLOW_TRACK;
#else
	unsigned char follow_flag = OFF;
#endif

// MIDI event counter for events that are in the MIDI queue at any time
unsigned int MIDI_queue_counter = 0;
unsigned int NOTE_queue_counter = 0;

// MIDI stack counter for events that are in the MIDI stack at any time
unsigned int MIDI_stack_counter = 0;
// unsigned int NOTE_stack_counter = 0;


// This is the timestamp keeper for MIDI events.
unsigned int G_MIDI_timestamp = 0;

// This is a flag to signal the first run of the sequencer thread
unsigned char G_manualStartBo = TRUE;


// The max value an unsigned int may carry - 0xFFFFFFFF - ours is smaller b/c something..
#define def_TIMESTAMP_MAX						 429496729

// This indicates the operation mode: INTERACTIVE or PASSIVE
// unsigned char OPS_mode = INTERACTIVE;


// MIDI reception status variables
unsigned char MIDI_RECEIVE_SYSEX = FALSE;
unsigned char MIDI_SYSEX_HEADER_SEEN = FALSE;


// Describes the memory space needed to accomodate incoming fresh code
// #define				SMALL_MEM_FILLER_SIZE 	327680 // 0x50000

// Code keeper will be used in zoomDEVICE mode and override the step repository memory space.
// CODE KEEPER SIZE IS THE SIZE REQUIRED BY REDBOOT AFTER HYPERTERM TRANSFER
unsigned char* 		G_sysex_stream_keeper;
unsigned char 		my_buffer[10000];
 // Size of stream-keeper buffer.
 unsigned int		G_sysex_stream_keeper_size = 0;

 // Index in stream-keeper buffer. Cannot be more than G_sysex_stream_keeper_size - 1.
 unsigned int		G_sysex_stream_keeper_index = 0;


// #ifdef NEMO
	#define				def_OSFILE_SIZE						0x40000
// #else
//	#define 			def_OSFILE_SIZE						0x40000
// #endif

#define 			def_CONFIGFILE_SIZE					0x10000
#define 			def_SYSEX_STREAM_KEEPER_SIZE		( def_CONFIGFILE_SIZE + def_OSFILE_SIZE )

// MIDI running status bytes
unsigned char G_running_status_byte_OUT_UART[2] = {0, 0};
unsigned char G_running_status_byte_IN_UART[2]  = {0, 0};

// MIDI IN BUFFER - handling incoming MIDI data
unsigned char G_midi_data_ndx 					= 0;		// Used as a pointer to the IN BUFFER
unsigned char G_midi_note_IN_UART[2][3] 		= { {0xff, 0xff, 0xff}, {0xff, 0xff, 0xff} };
unsigned char G_midi_controller_ndx				= 0;
unsigned char G_midi_controller_IN_UART[2][3] 	= { {0xff, 0xff, 0xff}, {0xff, 0xff, 0xff} };
unsigned char G_midi_bender_ndx					= 0;
unsigned char G_midi_bender_IN_UART[2][3] 		= { {0xff, 0xff, 0xff}, {0xff, 0xff, 0xff} };
unsigned char G_midi_pressure_ndx				= 0;
unsigned char G_midi_pressure_IN_UART[2][3] 	= { {0xff, 0xff, 0xff}, {0xff, 0xff, 0xff} };

// MIDI IN STUFF - used to buffer in data so we can print it.
// Printing it directly will not work, it gets scrambled.
unsigned char G_midi_in[3];
// unsigned char G_midi_in_ctr =0;

// stores the high and low nibbles of a byte coming in via SYSEX
unsigned char G_midi_in_sysex[2] = {0, 0};
unsigned char G_midi_in_sysex_nibble = HIGH;



// Page refresh signal
unsigned char PAGE_REFRESH_IN_PROGRESS 		= FALSE;
unsigned char SEQUENCER_DRIVING 			= FALSE;
unsigned char SEQUENCER_PLAYING_TTC 		= FALSE;

unsigned char MIDI_RED_INTERVAL 	= 	10;  	// 25 // 20-OK   // 15-OK    // 10 - OK
unsigned char CPU_RED_INTERVAL		=  	10;		// 10 - OK
unsigned char PAGE_REFRESH_INTERVAL = 	15;

unsigned int G_sysex_byte_count = 0;
// unsigned char SOFTWARE_UPDATE_RECEIVED = FALSE;
// unsigned char CONFIGFILE_UPDATE_RECEIVED = FALSE;

// Indicates whether we are pressing the current mode's object button
unsigned char MODE_OBJECT_SELECTION = INTERACTIVE;

unsigned char RAND_SEEDED = FALSE;
unsigned char POWERON_LOADED = FALSE;

// Default play mode status
unsigned char PLAY_MODE_STATUS = OFF;

#define FACTOR_INTERVAL_TYPE_DEFAULT 	0
#define FACTOR_INTERVAL_TYPE_SEMITONE 	1

// Multiples of 16
unsigned char Track_LEN_factor[ TRACK_MAX_LENFACTOR + 1 ] =
					{  	0, 1, 4, 8, 10, 12, 14, 15, 16,
						17, 18, 19, 20, 22, 24, 28, 32 };
#define LEN_FACTOR_NEUTRAL_VALUE 	16


unsigned char Track_STA_factor[ TRACK_MAX_STAFACTOR + 1 ] =
					{  	0, 20,  40,  50, 60,  70, 80, 90, 100,
					 	110, 120, 140, 160, 180, 200, 220, 255 };
#define STA_FACTOR_NEUTRAL_VALUE 	100


unsigned char Track_VEL_factor[ TRACK_MAX_VELFACTOR + 1 ] =
					{	0, 1, 3, 7, 12, 18, 26, 42, 50,
						61, 72, 85, 98, 112, 128, 162, 200 };
#define VEL_FACTOR_NEUTRAL_VALUE 	50


unsigned char Track_PIT_factor[ TRACK_MAX_PITFACTOR + 1 ] =
					{	0, 1, 3, 7, 12, 18, 26, 42, 50,
						61, 72, 85, 98, 112, 128, 162, 200 };

unsigned char Track_PIT_factor_semitone[ TRACK_MAX_PITFACTOR + 1 ] =
					{	0, 6, 12, 18, 24, 30, 36, 42, 48,
						54, 60, 66, 72, 78, 84, 90, 96 };

#define PIT_FACTOR_NEUTRAL_VALUE 	50


unsigned char Track_GRV_factor[ TRACK_MAX_GRVFACTOR + 1 ] =
					{	0, 1, 3, 7, 12, 18, 26, 42, 50,
						61, 72, 85, 98, 112, 128, 162, 200 };
#define GRV_FACTOR_NEUTRAL_VALUE 	50


unsigned char Track_AMT_factor[ TRACK_MAX_AMTFACTOR + 1 ] =
					{	0, 1, 3, 7, 12, 18, 26, 42, 50,
						61, 72, 85, 98, 112, 128, 162, 200 };
#define AMT_FACTOR_NEUTRAL_VALUE 	50


unsigned char Track_MCC_factor[ TRACK_MAX_MCCFACTOR + 1 ] =
					{	0, 1, 3, 7, 12, 18, 26, 42, 50,
						61, 72, 85, 98, 112, 128, 162, 200 };
#define MCC_FACTOR_NEUTRAL_VALUE 	50


// This is the trigger set index - used to show a single triggerset in DIR map mode
unsigned char current_trigger_set = 1;


// SYSEX parsing state registry
unsigned int 	G_sysex_header 			= 0;	// genoQs OS, CONFIG, GRID, PAGE
unsigned char	G_sysex_machine_type 	= 0;	// Type of machine that produced the sysex dump
unsigned short	G_sysex_OS_version 		= 0;	// OS version that produced the sysex dump
//unsigned char 	G_sysex_armed_object 	= 0;	// One of STEP_ID, TRACK_ID, PAGE_ID, GRID_ID, or 0
//unsigned char 	G_sysex_armed_ID 		= 0;	// The ID we are currently receiving data for
//unsigned short	G_sysex_armed_ID_size 	= 0;	// Nr. of bytes to receive before accepting ID value

// SYSEX incoming checksum
unsigned char 	G_in_stream_checksum 	= 0; 	// Running checksum for sysex dump



// Used as an internal flagging mechanism for various purposes
unsigned char 	DEVICE_STATE = 0;

// Used for finger step recording
unsigned char 	STEP_RECORD_FINGER = 0;

// Message code used for error state display
unsigned char 	GRID_message_code = 0;



// Variables used in the NEMO viewer lab
unsigned char NEMO_CHUNKSHIFT = 1;
unsigned char NEMO_REDSHIFT = 0;
unsigned char NEMO_GREENSHIFT = 0;

// Value sets behind the flow shapes on the NEMO front panel
unsigned char NEMO_flow_shape[10][16]	=
		{ 	{	0, 0, 0, 0, 		0, 0, 0, 0, 		0, 0, 0, 0, 		0, 0, 0, 0			},
			{	8, 16, 24, 32,		40, 48, 56, 64,		72, 80, 88, 96, 	104, 112, 118, 120	},
			{	120, 118, 112, 104, 96, 88, 80, 72, 	64, 56, 48, 40, 	32, 24, 16, 8		},
			{	8, 14, 24, 40, 		72, 104, 110, 120, 	120, 110, 104, 72,	40, 24, 14, 8		},
			{	64, 64, 56, 24, 	4, 40, 64, 88, 		104, 118, 120, 120, 104, 88, 72, 64		},
			{	64, 64, 72, 104, 	120, 120, 88, 64, 	40, 18, 24, 40, 	48, 56, 60, 62		},
			{	24, 40, 56, 64,		72, 88, 104, 120,	24, 40, 56, 64, 	72, 88, 104, 120	},
			{	64, 120, 120, 120, 	8, 8, 8, 88,		88, 88, 40, 40,		40, 72, 72, 56		},
			{	1, 56, 104, 120, 	10, 64, 69, 72, 	56, 35, 60, 64, 	52, 58, 62, 64		},
			{	1, 104, 120, 122, 	120, 100, 72, 88,	64, 24, 104, 40,	78, 60, 58, 64		}	};

unsigned char NEMO_CC_Sound_Control[ 32 ][ 4 ];
unsigned char G_last_cursor = 0;	// Used as a return address for navigation
unsigned char G_scale_ndx	= 1;	// Index of current scale. Has to be between 1 and 4 for NEMO!
unsigned char G_last_zoom_level	= zoomPAGE; // remembers the previous zoom level
unsigned int  G_measure_locator = 0;

Pagestruct* G_pgmch_pending_page = NULL;
#ifdef FEATURE_ENABLE_DICE
unsigned char G_dice_run_bit 		= OFF;
#endif

unsigned char 	G_save_song_pos = OFF;
// Allow MCH change w/o loosing track record status
unsigned short G_prev_rec_page_pattern = 0;
unsigned short G_prev_rec_page = 0;


#ifdef FEATURE_ENABLE_SONG_UPE
unsigned char 	G_repeats_delay = ON;
unsigned char 	G_repeats_interval_idx = 0;
unsigned int	Repeats_Intervals[] = { 0xFFFFFFFF, 64, 32, 24, 16, 8, 4 };
unsigned char	G_measure_indicator_value = 0; // 5 Measure Nibbles
unsigned char	G_measure_indicator_part = 0; // 1 Measure - 1/5 Nibble
unsigned char	G_silent = OFF;
unsigned char	G_delay = 0;
unsigned char	G_reset = OFF;
unsigned char	G_unarm_ctrl = OFF;
Trackstruct*	G_rec_ctrl_track = NULL;
Pagestruct*		G_rec_ctrl_page = NULL;
Pagestruct*		G_last_ctrl_page[9] = { NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL };
unsigned short	G_last_ctrl_offset[9] = { 0, 0, 0, 0, 0, 0, 0, 0, 0 };
#endif
