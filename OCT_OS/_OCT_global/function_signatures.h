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

extern void 			show_strum_in_circle(	signed char my_strum );

extern 	void 			Octopus_recall_flash();
extern 	void			my_executable_dump(		unsigned int in_length );
extern 	void			my_OS_dump();

extern	unsigned char	sysex_to_byte( 			unsigned char high_nibble,
												unsigned char low_nibble );

extern 	void			my_OS_program_to_flash( unsigned char* data_source );
extern 	void			my_clear_flash();
extern 	void			my_configfile_program_to_flash( unsigned char* data_source );

extern	void			read_my_flash();
extern 	void 			send_sysex( 			unsigned char in_byte );

extern 	void			driveSequencer();
extern  unsigned int	kickSequencerThread( void );

extern 	void 			midi_byte_interpret( 	unsigned char midi_byte );
extern 	void 			sequencer_STOP( bool midi_send_stop );
extern 	void 			sequencer_command_STOP();
extern 	void 			sequencer_START();
extern 	void 			sequencer_HALT();
#ifdef FEATURE_ENABLE_SONG_UPE
extern	void			sequencer_command_PAUSE(unsigned char measure_scrolling);
extern	void 			select_page_preselections();
extern  unsigned char 	is_pre_selected_in_GRID( Pagestruct* target_page );

extern 	void 			copy_ctrl_step_to_track( Pagestruct* page, Trackstruct* track, Stepstruct* step );
extern	void 			copy_ctrl_track_to_step( Pagestruct* page, Trackstruct* track, Stepstruct* step );
extern  void 			ctrl_event_set_target_page( Pagestruct* target_page );
extern	void 			quick_assign_control_track ( Pagestruct* target_page, unsigned char trackIdx );
extern	void 			make_control_track ( Pagestruct* target_page, unsigned char trackIdx );
extern	void 			align_measure_locators();
extern	void 			drivePageCursor(Pagestruct* target_page, unsigned int measures);
#endif
extern 	void 			sequencer_RESET( unsigned char force_stop );

extern unsigned int 	computeKeyNdx(			unsigned char reg,
												unsigned char sigByte,
												unsigned char portValue);

extern 	void 			executeKey( 			unsigned int keyNdx );
extern 	void 			updateMIR();
extern 	void 			G_TIMER_REFILL_update();
extern 	unsigned char	scan_keys();

extern 	void 			executeRot(				unsigned int rotNdx);
extern 	void 			G_TIMER_REFILL_update();
extern 	unsigned int 	G_scanRots();


extern 	unsigned char 	selection_change_request_pending();
extern 	void 			show( 					unsigned int target,
												unsigned int content );

extern 	void 			VIEWER_fill_MIR();

extern 	void 			VIEWER_write_column( 	unsigned int column,
												unsigned char color,
												unsigned int bits);

extern 	unsigned char 	getBitpattern( 			unsigned char color,
												unsigned char chunkNdx);
extern 	unsigned char 	NEMO_getBitpattern( 			unsigned char color,
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
extern 	void 			Page_repository_init();

// ASSIGNMENT functions
extern 	void 			Page_repository_assign_Steps();
extern 	void 			Page_repository_assign_Tracks();
extern 	void 			Page_repository_assign_NoteOFF_queues();
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
extern void 			show_pitch_in_line( 	signed char my_pitch, unsigned char status );
extern void 			show_velocity_in_circle(signed char my_velocity );
extern void 			show_strum_in_line(		signed char my_strum );


// extern void 			show_chord_in_line( 	unsigned short chord_pattern,
//												unsigned char offset );

unsigned char 			offset_to_chord_aux(		unsigned short in_scale,
													unsigned char degree );
unsigned char 			offset_to_next_chord_aux(		unsigned short in_scale,
														unsigned char pitch );
extern void 			import_GRID_set( 		unsigned int source_set );

extern unsigned char 	map_to_8bit( 			unsigned short _14bit );
extern void 			key_exe_chainselectors( unsigned short keyNdx );

#ifdef NEMO
	extern void 		NEMO_key_OCT_CIRCLE_chord_STEP( unsigned char keyNdx );
#endif


extern unsigned short 	Page_getTrackRecPattern( const Pagestruct* pagePt );
extern int				Page_getTrackRecCount( const Pagestruct* pagePt );
extern void 			Page_setTrackRecPattern( Pagestruct* pagePt, unsigned short pattern );
extern void 			Page_clear( Pagestruct* pagePt );
extern void 			Page_CLEAR_selected_tracks( Pagestruct* target_page );
extern void 			PAGE_init(Pagestruct* target_page, pageid_t pageId, booln firstInitBo );

extern void 			MIDI_init();
extern void 			NOTE_IN_init();
extern unsigned short 	extract_current_scale( Pagestruct* target_page );
extern void 			grid_select( Pagestruct* target_page, unsigned char target_status );
extern void 			extract_scale_and_lead( Pagestruct* target_page );
extern void 			export_scale_degrees( Pagestruct* target_page );
extern void 			export_scale_lead( Pagestruct* target_page );
extern void 			Step_init( Stepstruct* target_step );
extern unsigned int 	length2_ticks( unsigned int length );
extern void 			build_step_chord( Stepstruct* target_step, unsigned char bit_offset );
extern signed char 		adjust_PIT_to_track( unsigned char in_PIT, Trackstruct* target_track );
extern void 			make_chord( Stepstruct* target_step, signed char track_pitch, signed char in_pitch );

extern void 			key_ScaleSelector_mini_functions( unsigned int keyNdx, Pagestruct* target_page );
extern void 			key_ScaleSelector_functions( unsigned int keyNdx, Pagestruct* target_page );
extern void 			my_sysex_dump( unsigned char dump_type, unsigned char my_grid_cursor, int objectIx, int objectCt );

extern void 			show_progress_bar( unsigned char part, unsigned char color );

extern unsigned int 	is_pressed_keyrange( unsigned int min, unsigned int max );
extern void 			MIR_write_base12_H( int val, unsigned char target_row );
extern void 			MIR_write_numeric_C (unsigned char number);
extern unsigned char 	Step_get_status ( Stepstruct* target_step, unsigned char target_bit);
extern int 				scale_pitch( Pagestruct* pagePt, int pitch );

#ifdef FEATURE_ENABLE_CHORD_OCTAVE
extern unsigned char 	get_chord_cardinality( Stepstruct* target_step, unsigned char octave_mask );
extern void 			toggle_chord_octave( Stepstruct* target_step, unsigned char note_offset, unsigned char chord_octave_mask );
extern bool 			is_step_chord( Stepstruct* target_step );
extern void 			show_chord_octave( Stepstruct* target_step, unsigned char note_offset, unsigned char chord_octave );
extern void 			show_chord_octave_first( Stepstruct* target_step, unsigned char note_offset );
extern unsigned char 	get_current_chord_octave();
#endif
