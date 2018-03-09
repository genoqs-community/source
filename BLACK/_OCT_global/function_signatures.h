

extern 	void 			my_flash_101( 			unsigned int in_length );
extern 	void			my_executable_dump(		unsigned int in_length );
extern	unsigned char	sysex_to_byte( 			unsigned char high_nibble, 
												unsigned char low_nibble );
extern 	void			my_executable_program_to_flash();
extern	void			read_my_flash();
extern 	void 			send_sysex( unsigned char in_byte );

extern 	void			driveSequencer();

extern 	void 			midi_byte_interpret( 	unsigned char midi_byte );
extern 	void 			sequencer_STOP();
extern 	void 			sequencer_START();
extern 	void 			sequencer_HALT();
extern 	void 			sequencer_RESET();

extern unsigned int 	computeKeyNdx(			unsigned char reg, 
												unsigned char sigByte, 
												unsigned char portValue);

extern 	void 			executeKey( 			unsigned int keyNdx );
extern 	void 			updateMIR();
extern 	void 			TIMER_REFILL_update();
extern 	unsigned char	scanKeys();

extern 	void 			executeRot(				unsigned int rotNdx);
extern 	void 			TIMER_REFILL_update();
extern 	unsigned int 	scanRots();


extern 	unsigned char 	selection_change_request_pending();
extern 	void 			show( 					unsigned int target, 
												unsigned int content );

extern 	void 			VIEWER_fill_MIR();

extern 	void 			VIEWER_write_column( 	unsigned int column, 
												unsigned char color, 
												unsigned int bits);
extern 	unsigned char 	getBitpattern( 			unsigned char color, 
												unsigned char chunkNdx);
extern 	void 			Viewer_initShift();
extern 	void 			Viewer_writeShift(		unsigned int bitpattern);
extern 	unsigned int 	mirror (				unsigned int input, 
												unsigned char length);
extern 	void 			VIEWER_show_MIR();
extern 	void 			initMIR();
extern 	void 			PLAYER_updateTrackLocators();
extern 	void			MIR_write_GRID();
extern 	void 			GRID_write_dot( 		unsigned int ndx, 
												unsigned char color);
extern 	void 			initGRID();

extern 	void 			Page_printStatusVector();
extern 	void 			VIEWER_fill_MIR();
extern 	unsigned int 	row_of( 				unsigned int keyNdx );
extern 	unsigned int 	column_of( 				unsigned int keyNdx );
extern 	void 			VIEWER_clear_HUI();
extern 	void 			VIEWER_show_page( 		unsigned char lauflicht);

extern 	unsigned int 	normalize (				signed int in_value, 
												signed int min_value,
												signed int max_value );


// Solve Compilation dependencies  
extern 	void 			PANEL_build();  


// REPOSITORY init functions
extern 	void 			Step_repository_init();
extern 	void 			Track_repository_init();
extern 	void 			NoteOFF_repository_init();
extern 	void 			Cadence_repository_init();
extern 	void 			Page_repository_init();
extern 	void 			Page_flashbuffer_repository_init();
 
// ASSIGNMENT functions
extern 	void 			Page_repository_assign_Steps();
extern 	void 			Page_repository_assign_Tracks();
extern 	void 			Page_repository_assign_NoteOFF_queues();
extern 	void 			Page_repository_assign_Cadences();
extern 	void 			Page_repository_assign_Tools();

// INITIALIZATION functions
// extern void PAGE_init(Pagestruct* target_page, int ndx);
extern 	void 			MIR_init();
extern 	void 			Page_GRID_init();
extern 	void 			VIEWER_show_MIR();
extern 	void 			MIR_write_dot( 			unsigned int, 
												unsigned char);

extern 	void 			modify_parameter( 		signed char* in_parameter, 
												unsigned int min_val, 
												unsigned int max_val, 
												unsigned char direction,
												unsigned char QTT_status,
												unsigned char WRAP_status	);
												
extern 	void 			modify_signed_parameter(signed char* in_parameter, 
												signed int min_val, 
												signed int max_val, 
												unsigned char direction,
												unsigned char QTT_status,
												unsigned char WRAP_status	);

extern unsigned char 	get_content_GRID_set( 	unsigned char target_set );

extern unsigned char 	is_pressed_key( 		unsigned int target_key );

extern void 			export_GRID_set( 		unsigned int target_GRID_set );

extern unsigned char 	is_actual_GRID( 		unsigned char target_set );

extern unsigned char 	pitch_to_noteNdx( 		unsigned char in_pitch );

extern signed char		randomize_byamount( 	signed int in_value,
												signed int amount,
												signed int min_val,
												signed int max_val 		);
												
extern unsigned char 	cursor_to_dot( 			unsigned char in_cursor );
extern unsigned int 	is_pressed_rowzero();
extern void 			show_pitch_in_circle( 	signed char my_pitch, unsigned char status );
extern void 			show_velocity_in_circle(signed char my_velocity );
extern void 			show_strum_in_circle(	signed char my_strum );

extern void 			midi_note_execute( 		unsigned char UART_ndx,
												unsigned char status_byte, 
												unsigned char data_byte_1,
												unsigned char data_byte_2	);
												
extern void 			show_chord_in_circle( 	unsigned short chord_pattern, 
												unsigned char offset );
unsigned char 			offset_to_next_chord_aux(		unsigned short in_scale, 
														unsigned char pitch );
extern void 			import_GRID_set( 		unsigned int source_set );
