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

unsigned char FLASH_LOAD_FLAG = LOAD_ON_POWERON;

// SPAGHETTI from scanKeys
unsigned int scan[3][26];
unsigned int pressed_keys[261];

// Global variable indicating that a finger is on a key 
unsigned int key_pressed = 0;
unsigned int cycle = 0;


//
// CLOCK_FREQUENCY 50 000 000 corresponds to 1 second
//
unsigned char 	 	TEMPO 				= 120; 	//120 default
unsigned char		CLOCK_SOURCE		= OFF;	// Can be any of OFF, INT(ernal), EXT(ernal)

// GLOBALBLINKER is used to control LED blinking activities
unsigned char 		GLOBALBLINKER		= 0;
unsigned char		MIDI_LOAD_MONITOR1	= MIR_GREEN;
unsigned char 		MIDI_LOAD_MONITOR2	= MIR_GREEN;
unsigned char 		CPU_LOAD_MONITOR	= MIR_GREEN;

// Used to count down to normal tempo -> TEMPO
unsigned char 		TEMPO_TRACKER		= 1;

// Absolute value of the TIMER_TICK_CTR in the 1xT and 2xT modes
unsigned char 		TTC_ABS_VALUE		= 0;


// cyg_thread threads[TOTAL_THREADS];

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
unsigned char RUNBIT 	= OFF;
unsigned char PAUSEBIT 	= OFF;

// This enforces the beat resolution- This is the note indicator
unsigned char TIMER_TICK_CNTR = 0;

// This is the midi counter flag
unsigned char MIDI_TICK_CNTR = 0;

// Heartbeat flag
// unsigned char heart = 0;

// Define the panel status for the page
unsigned int PANEL_matrix[PANEL_NROF_OBJECTS][PANEL_NROF_ATTRIBUTES+1];

// Refill for 16th steps: (50Mhz/4steps_per_beat * 60Secs)/TEMPO
unsigned int TIMER_REFILL = 0;

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

// Global Repository for Rotary value information
// char rotary_value [22];


// Matrix Intermediate Representation. The first dimension (2) represents left and right.
// The second dimension (17) is the scaling factor. See MIR scheme table outside the code.
// The third dimension is for BLINK (0), RED (1) and GREEN (2) and uses the RED and GREEN defines
unsigned char MIR  [2][17][3];
unsigned char DIAG [258];
unsigned char DIAG_rotary_current = 200;
unsigned char DIAG_rotary_value[21];

// This probably should be zoomGRID but may use zoomPAGE for convenience
unsigned char zoom_level = zoomPAGE;



// This is a counter building the window of operation of a turnwheel
//unsigned char TURNWHEEL_TIMER 		= OFF;
//unsigned char TURNWHEEL_START 		= 0;
//unsigned char TURNWHEEL_DIRECTION 	= OFF;
//unsigned char TURNWHEEL_ROW			= 0;
//unsigned char TURNWHEEL_COL			= 0;
//unsigned char TURNWHEEL_COUNT		= 0;

// Edit timer used to temporarily switch between default and track EDIT mode.
unsigned char EDIT_TIMER 			= OFF;
unsigned char MIX_TIMER				= OFF;

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
unsigned char GLOBAL_LOCATOR = 0;



// FOLLOW variables - finally reduced to a radio button: FOLLOW_GRID, FOLLOW_PAGE, FOLLOW_TRACK
unsigned char follow_flag = OFF;


// MIDI event counter for events that are in the MIDI queue at any time
unsigned int MIDI_queue_counter = 0;
unsigned int NOTE_queue_counter = 0;

// MIDI stack counter for events that are in the MIDI stack at any time
unsigned int MIDI_stack_counter = 0;
unsigned int NOTE_stack_counter = 0;


// This is the timestamp keeper for MIDI events.
unsigned int MIDI_timestamp = 0;

// The max value an unsigned int may carry - 4294967295 - ours is smaller b/c something..
#define TIMESTAMP_MAX						 429496729	

// This indicates the operation mode: INTERACTIVE or PASSIVE
// unsigned char OPS_mode = INTERACTIVE;


// MIDI reception status variables
unsigned char MIDI_RECEIVE_SYSEX = FALSE;
unsigned char MIDI_SYSEX_HEADER_SEEN = FALSE;

// Lauflicht status holder - ON means Lauflicht is visible, OFF otherwise
unsigned char LAUFLICHT_DISPLAY = ON;
 

// Describes the memory space needed to accomodate incoming fresh code
// #define				SMALL_MEM_FILLER_SIZE 	327680 // 0x50000

// Code keeper will be used in zoomDEVICE mode and override the step repository memory space.
unsigned char* 		code_keeper;
#define 			CODE_KEEPER_SIZE		0x50000
unsigned int		code_keeper_index = 0;



// MIDI running status bytes
unsigned char running_status_byte_OUT_UART[2] = {0, 0};
unsigned char running_status_byte_IN_UART[2]  = {0, 0};

// MIDI IN BUFFER - handling incoming MIDI data
unsigned char midi_data_ndx 		= 0;		// Used as a pointer to the IN BUFFER
unsigned char midi_note_IN_UART[2][3];
unsigned char midi_controller_ndx	= 0;
unsigned char midi_controller_IN_UART[2][3];


// MIDI IN STUFF - used to buffer in data so we can print it.
// Printing it directly will not work, it gets scrambled.
unsigned char midi_in[3];
// unsigned char midi_in_ctr =0;

// stores the high and low nibbles of a byte coming in via SYSEX
unsigned char midi_in_sysex[2] = {0, 0};
unsigned char midi_in_sysex_nibble = HIGH;



// Page refresh signal
unsigned char PAGE_REFRESH_IN_PROGRESS 		= FALSE;
unsigned char SEQUENCER_DRIVING 			= FALSE;
unsigned char SEQUENCER_PLAYING_TTC 		= FALSE;
unsigned char PLAY_LOAD 					= 0;

//unsigned char PLAY_LIMIT = 30;		// 27 // 27-30 - Empirically established - see Lab Session III in rotBigKnob.h
unsigned char PLAY_LIMIT = 35;			// 30 / 45 / 35 This works well after the dedicated CPU alarm

unsigned char MIDI_RED_INTERVAL =  10;  // 25 // 20-OK   // 15-OK    // 10 - OK
unsigned char CPU_RED_INTERVAL	=  10;	// 10 - OK

unsigned int sysex_byte_count = 0;
unsigned char SOFTWARE_UPDATE_RECEIVED = FALSE;

// Indicates whether we are pressing the current mode's object button
unsigned char MODE_OBJECT_SELECTION = INTERACTIVE;

unsigned char RAND_SEEDED = FALSE;
unsigned char POWERON_LOADED = FALSE;

// Default play mode status
unsigned char PLAY_MODE_STATUS = OFF;

//// Multiples of 16
//unsigned char Track_LEN_factor[17] = {  0, 
//										1, 	4, 	8, 	10,	12, 14, 15, 16, 
//										17, 18, 19, 20, 22, 24, 28, 32 };
//// The values are finally divided by 100..
//unsigned char Track_STA_factor[17] = {  0, 	
//										20,  40, 50, 60, 70, 80, 90,100,
//										110,120,140,160,180,200,223,255 };

// Multiples of 16
unsigned char Track_LEN_factor[11] = {  0, 
										 1,  6, 10, 14, 16, 
										18, 20, 24, 28, 32 };

// The values are finally divided by 100..
unsigned char Track_STA_factor[11] = {  0, 	
										 20,  40,  60,  80, 100,
										120, 140, 170, 200, 255 };

// The values are finally divided by 50..
//unsigned char Track_VEL_factor[11] = {	0,
//										 2,   8,  18,  32,  50,  
//										72,  98, 128, 162, 200  };
unsigned char Track_VEL_factor[11] = {	0,
										 1,   4,  9,  16,  25,  
										36,  49, 64,  81, 100  };

// The values are finally divided by 50..
//unsigned char Track_PIT_factor[11] = {	0,
//										 2,   8,  18,  32,  50,  
//										72,  98, 128, 162, 200  };
unsigned char Track_PIT_factor[11] = {	0,
										 1,   4,  9,  16,  25,  
										36,  49, 64,  81, 100  };

unsigned char Track_GRV_factor[11] = {	0,
										 1,   4,  9,  16,  25,  
										36,  49, 64,  81, 100  };

unsigned char Track_MCC_factor[11] = {	0,
										 1,   4,  9,  16,  25,  
										36,  49, 64,  81, 100  };

// This is the trigger set index - used to show a single triggerset in DIR map mode
unsigned char current_trigger_set = 1;

