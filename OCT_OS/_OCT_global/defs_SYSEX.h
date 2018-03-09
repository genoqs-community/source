


// SYSEX defines - for STEP 
#define		def_SYSEX_STEP_ID_code 				105
#define		def_SYSEX_STEP_NROF_TOKENS			12

#define 	def_SYSEX_STEP_chord_up_ID 			1
#define		def_SYSEX_STEP_chord_data_ID		2
#define		def_SYSEX_STEP_event_data_ID		3
#define		def_SYSEX_STEP_attr_STATUS_ID		4
#define		def_SYSEX_STEP_attr_VEL_ID			5
#define		def_SYSEX_STEP_attr_PIT_ID			6
#define		def_SYSEX_STEP_attr_LEN_ID			7
#define		def_SYSEX_STEP_attr_STA_ID			8
#define		def_SYSEX_STEP_attr_POS_ID			9
#define		def_SYSEX_STEP_attr_AMT_ID			10
#define		def_SYSEX_STEP_attr_GRV_ID			11
#define		def_SYSEX_STEP_attr_MCC_ID			12



// SYSEX defines - for TRACK 
#define		def_SYSEX_TRACK_ID_code 					104
#define		def_SYSEX_TRACK_NROF_TOKENS					35

#define 	def_SYSEX_TRACK_attr_STATUS_ID				1
#define 	def_SYSEX_TRACK_attr_VEL_ID					2
#define 	def_SYSEX_TRACK_attr_PIT_ID					3
#define 	def_SYSEX_TRACK_attr_LEN_ID					4
#define 	def_SYSEX_TRACK_attr_STA_ID					5
#define 	def_SYSEX_TRACK_attr_POS_ID					6
#define 	def_SYSEX_TRACK_attr_DIR_ID					7
#define 	def_SYSEX_TRACK_attr_AMT_ID					8
#define 	def_SYSEX_TRACK_attr_GRV_ID					9
#define 	def_SYSEX_TRACK_attr_MCC_ID					10
#define 	def_SYSEX_TRACK_attr_MCH_ID					11
#define 	def_SYSEX_TRACK_attr_LOCATOR_ID				12
#define 	def_SYSEX_TRACK_attr_MISC_ID				13
#define 	def_SYSEX_TRACK_attr_CCAMT_ID				14
#define 	def_SYSEX_TRACK_attr_TEMPOMUL_ID			15
#define 	def_SYSEX_TRACK_attr_TEMPOMUL_SKIP_ID		16
#define 	def_SYSEX_TRACK_program_change_ID			17
#define 	def_SYSEX_TRACK_bank_change_ID				18
#define 	def_SYSEX_TRACK_frame_ndx_ID				19
#define 	def_SYSEX_TRACK_trigger_ID					20
#define 	def_SYSEX_TRACK_chain_data_ID				21	
#define 	def_SYSEX_TRACK_VEL_factor_ID				22
#define 	def_SYSEX_TRACK_PIT_factor_ID				23
#define 	def_SYSEX_TRACK_LEN_factor_ID				24
#define 	def_SYSEX_TRACK_STA_factor_ID				25
#define 	def_SYSEX_TRACK_AMT_factor_ID				26
#define 	def_SYSEX_TRACK_GRV_factor_ID				27
#define 	def_SYSEX_TRACK_MCC_factor_ID				28
#define 	def_SYSEX_TRACK_event_offset_ID				29
#define 	def_SYSEX_TRACK_event_max_ID				30
#define 	def_SYSEX_TRACK_TTC_ID						31
#define 	def_SYSEX_TRACK_scale_pitch_offset_ID		32
#define 	def_SYSEX_TRACK_lead_pitch_offset_ID		33
#define 	def_SYSEX_TRACK_CC_resolution_ID			34
// #ifdef NEMO
	#define def_SYSEX_TRACK_flow_shape_ID				35
// #endif



// SYSEX defines - for PAGE 
#define		def_SYSEX_PAGE_ID_code 						103
#define		def_SYSEX_PAGE_NROF_TOKENS					32

// empty	def_SYSEX_PAGE_pageNdx_ID					1
#define		def_SYSEX_PAGE_CHAINS_PLAY_HEAD_ID			2	
// empty						 						3	
#define		def_SYSEX_PAGE_chainMode_ID					4	
#define		def_SYSEX_PAGE_editorMode_ID				5		
#define		def_SYSEX_PAGE_editAttribute_ID				6		
#define		def_SYSEX_PAGE_force_to_scale_ID 			7	
#define		def_SYSEX_PAGE_repeats_left_ID				8
#define		def_SYSEX_PAGE_mixTarget_ID					9	
#define		def_SYSEX_PAGE_MIXAMT_VOL_ID				10	
#define		def_SYSEX_PAGE_MIXAMT_PAN_ID				11
#define		def_SYSEX_PAGE_MIXAMT_MOD_ID				12
#define		def_SYSEX_PAGE_MIXAMT_EXP_ID				13
#define		def_SYSEX_PAGE_MIXAMT_USR_ID				14
#define		def_SYSEX_PAGE_CC_MIXMAP_attribute_ID		15	
#define		def_SYSEX_PAGE_CC_MIXMAP_ID					16	
#define		def_SYSEX_PAGE_scaleStatus_ID				17	
#define		def_SYSEX_PAGE_scaleNotes_ID				18
#define		def_SYSEX_PAGE_scaleLead_ID					19
#define		def_SYSEX_PAGE_scaleLead_old_ID				20
#define		def_SYSEX_PAGE_scaleNotes_old_ID			21
#define		def_SYSEX_PAGE_my_scale_signature_ID		22	
// empty												23
#define		def_SYSEX_PAGE_OPS_mode_ID					24
#define		def_SYSEX_PAGE_SCL_align_ID					25
#define		def_SYSEX_PAGE_attr_mix2edit_ID				26
#define		def_SYSEX_PAGE_attr_mix2edit_MIN_ID			27
#define		def_SYSEX_PAGE_attr_mix2edit_MAX_ID			28
#define		def_SYSEX_PAGE_attr_VEL_ID					29
#define		def_SYSEX_PAGE_attr_PIT_ID					30
#define		def_SYSEX_PAGE_attr_LEN_ID					31
#define		def_SYSEX_PAGE_attr_STA_ID					32
#define		def_SYSEX_PAGE_mixAttribute_ID				33	
#define		def_SYSEX_PAGE_trackMutepattern_ID			34	
#define		def_SYSEX_PAGE_trackMutepatternStored_ID	35 	



// SYSEX defines - for GRID 
#define		def_SYSEX_GRID_ID_code 							102
#define		def_SYSEX_GRID_NROF_TOKENS						23

#define		def_SYSEX_GRID_ID_G_master_tempo_ID				1
#define		def_SYSEX_GRID_ID_G_clock_source_ID				2									
#define		def_SYSEX_GRID_ID_GRID_scene_ID					3
#define		def_SYSEX_GRID_ID_GRID_switch_mode_ID			4
// empty	was: GRID_bank_triggermode_ID					5
#define		def_SYSEX_GRID_ID_GRID_CURSOR_ID				6
#define		def_SYSEX_GRID_ID_GRID_mixAttribute_ID			7
#define		def_SYSEX_GRID_ID_GRID_play_mode_ID				8
#define		def_SYSEX_GRID_ID_GRID_bank_playmodes_ID		9
#define		def_SYSEX_GRID_ID_GRID_set_switchmode_ID		10
#define		def_SYSEX_GRID_ID_current_GRID_set_ID			11
#define		def_SYSEX_GRID_ID_GRID_p_set_ID					12
#define		def_SYSEX_GRID_ID_GRID_OPS_mode_ID				13
#define		def_SYSEX_GRID_ID_GRIDTRACK_OPS_mode_ID			14
// empty	was: stepDefaultAttrValue						15
// empty	was: TRACK_COPY_BUFFER							16
#define		def_SYSEX_GRID_ID_follow_flag_ID				17
// empty	was: CHAINS_PLAY_HEAD_ID						18
#define		def_SYSEX_GRID_ID_GRID_p_selection_ID 			19
#define		def_SYSEX_GRID_ID_OCT_step_strum_VEL_ID			20
#define		def_SYSEX_GRID_ID_OCT_step_strum_PIT_ID			21
#define		def_SYSEX_GRID_ID_OCT_step_strum_LEN_ID			22
#define		def_SYSEX_GRID_ID_OCT_step_strum_STA_ID			23
#define		def_SYSEX_GRID_ID_SYSEX_dump_speed_ID			24
#define		def_SYSEX_GRID_ID_GRID_editmode_ID				25
#define		def_SYSEX_GRID_ID_GRIDTRACK_editmode_ID			26
#define		def_SYSEX_GRID_ID_direction_repository			27
//#define		def_SYSEX_GRID_ID_assistant_page				28






