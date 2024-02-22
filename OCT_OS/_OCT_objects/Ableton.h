

// This is the implementation of the Ableton Live Client.
// It contains a struct that holds a virtual stateful controller.
// The UI interacts with this controller, which generates MIDI signals.
// Synchronization with the Ableton Host (Live) takes place via MIDI.


// Implements a clip inside the client
typedef struct clientclipstruct{

	// State data
	unsigned char state;

	// Control data
	unsigned char note;

} Clientclipstruct;


// Implements a track inside the client
typedef struct clienttrackstruct{

	// A track has 9 clips
	Clientclipstruct Clip[9];
	
	// State data
	unsigned char state_active;
	unsigned char state_solo;
	unsigned char state_arm;
	unsigned char state_VOL;
	unsigned char state_PAN;
	unsigned char state_SNDA;
	unsigned char state_SNDB;
	
	// Control data
	unsigned char active_note;
	unsigned char solo_note;
	unsigned char arm_note;
	unsigned char VOL_MIDICC;
	unsigned char PAN_MIDICC;
	unsigned char SNDA_MIDICC;
	unsigned char SNDB_MIDICC;	

	unsigned char MIDIchannel;

} Clienttrackstruct;


// Implements the master structure
typedef struct abletonstruct{

	// An Ableton Client has 16 tracks
	Clienttrackstruct Track[16];

	// The selected track is the one to operate on with encoders
	unsigned char selected_track;

	// State data
	unsigned char state_runbit;
	unsigned char state_follow;
	unsigned char state_recbit;
	unsigned char state_VOL;
	unsigned char state_PAN;
	
	// Control data
	unsigned char runbit_note;
	unsigned char follow_note;
	unsigned char recbit_note;
	unsigned char VOL_MIDICC;
	unsigned char PAN_MIDICC;

	unsigned char MIDIchannel;

} Abletonstruct;





