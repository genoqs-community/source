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



//
// Defines for the front pannel: KEY and LEDs
// Some may be outdated. Needs revision at some point.



// LEDs Keys pairs
#define		LED_ZOOM_GRID			218
#define 	KEY_ZOOM_GRID			218

#define 	LED_ZOOM_PAGE			219
#define		KEY_ZOOM_PAGE			219

#define 	LED_ZOOM_TRACK			220
#define		KEY_ZOOM_TRACK			220

#define 	LED_ZOOM_MAP			228
#define 	KEY_ZOOM_MAP			228

//#ifdef NEMO
//	#define 	LED_ZOOM_PLAY			186
//	#define 	KEY_ZOOM_PLAY			186
//#else
	#define 	LED_ZOOM_PLAY			229
	#define 	KEY_ZOOM_PLAY			229
//#endif

#define		LED_ZOOM_STEP			227
#define		KEY_ZOOM_STEP			227

#define		LED_MIX_MASTER			21
#define 	KEY_MIX_MASTER			21

#define 	LED_SELECT_MASTER		32
#define 	KEY_SELECT_MASTER		32


#define 	LED_TEMPO				234
#define		KEY_TEMPO				234


#ifdef NEMO
	#define		LED_PROGRAM				231
	#define		KEY_PROGRAM				223
#else
	#define		LED_PROGRAM				242
	#define		KEY_PROGRAM				242
#endif

#define		LED_EDIT_INDICATOR		259		
			// No key available

#define 	LED_MIX_INDICATOR		260		
			// No key available

#define 	LED_CLOCK				233 // 213	
#define		KEY_CLOCK				233 // 213

#define 	LED_CHAINER				213
#define		KEY_CHAINER				213

#define		LED_FOLLOW				214
#define		KEY_FOLLOW				214

#define 	LED_MIXTGT_USR0			98
#define 	KEY_MIXTGT_USR0			98


#ifdef NEMO
	#define 	LED_MIXTGT_USR1			43
	#define 	KEY_MIXTGT_USR1			43
	#define 	LED_MIXTGT_USR2			54
	#define 	KEY_MIXTGT_USR2			54
	#define 	LED_MIXTGT_USR3			65
	#define 	KEY_MIXTGT_USR3			65
	#define 	LED_MIXTGT_USR4			76
	#define 	KEY_MIXTGT_USR4			76
#else
	#define 	LED_MIXTGT_USR1			109
	#define 	KEY_MIXTGT_USR1			109
	#define 	LED_MIXTGT_USR2			120
	#define 	KEY_MIXTGT_USR2			120
	#define 	LED_MIXTGT_USR3			131
	#define 	KEY_MIXTGT_USR3			131
	#define 	LED_MIXTGT_USR4			142
	#define 	KEY_MIXTGT_USR4			142
#endif

#define 	LED_MCC_STREAM1			146
#define 	KEY_MCC_STREAM1			146
#define 	LED_MCC_STREAM2			157
#define 	KEY_MCC_STREAM2			157
#define 	LED_MCC_STREAM3			168
#define 	KEY_MCC_STREAM3			168
#define 	LED_MCC_STREAM4			179
#define 	KEY_MCC_STREAM4			179

#define 	LED_MIXTGT_USR5			153
#define 	KEY_MIXTGT_USR5			153

#define 	LED_MUTE_MASTER			164
#define		KEY_MUTE_MASTER			164

#define		LED_EDIT_MASTER			175
#define		KEY_EDIT_MASTER			175


//#ifdef NEMO
//	#define		LED_RETURN				229
//	#define 	KEY_RETURN				229
//#else
	#define		LED_RETURN				186
	#define 	KEY_RETURN				186
//#endif


#ifdef NEMO
	// Pointed to MIX MASTER
	#define 	LED_MIXTGT_ATR			21
	#define 	KEY_MIXTGT_ATR			21
	
	// Sound control buttons
	#define 	LED_MIXTGT_VOL			43
	#define 	KEY_MIXTGT_VOL			43
	
	#define 	LED_MIXTGT_PAN			54
	#define 	KEY_MIXTGT_PAN			54
	
	#define 	LED_MIXTGT_MOD			65
	#define 	KEY_MIXTGT_MOD			65
	
	#define 	LED_MIXTGT_EXP			76
	#define 	KEY_MIXTGT_EXP			76
#else
	#define 	LED_MIXTGT_ATR			43
	#define 	KEY_MIXTGT_ATR			43
	
	#define 	LED_MIXTGT_VOL			54
	#define 	KEY_MIXTGT_VOL			54
	
	#define 	LED_MIXTGT_PAN			65
	#define 	KEY_MIXTGT_PAN			65
	
	#define 	LED_MIXTGT_MOD			76
	#define 	KEY_MIXTGT_MOD			76
	
	#define 	LED_MIXTGT_EXP			87
	#define 	KEY_MIXTGT_EXP			87
#endif



#ifdef NEMO
	#define		LED_RECORD				231
	#define		KEY_RECORD				223

	#define 	KEY_STOP				231
	#define		LED_STOP				223
#else
	#define		LED_RECORD				223
	#define		KEY_RECORD				223

	#define 	KEY_STOP				231
	#define		LED_STOP				231
#endif


#define		KEY_PAUSE				232
#define 	LED_PAUSE				232

#define		KEY_PLAY1				241
#define		LED_PLAY1				241

#define		KEY_PLAY2				240
#define		LED_PLAY2				240

#define		KEY_PLAY4				250
#define		LED_PLAY4				250

#define 	LED_QUANTIZE_LOW		252
#define 	KEY_QUANTIZE_LOW		252
#define 	LED_QUANTIZE_HIGH		258
#define 	KEY_QUANTIZE_HIGH		258

//	#define 	LED_MIDI_OFF				213
// 	#define		KEY_MIDI_OFF				300 // ->This key will therefore never be registered

// KEYS
#define 	KEY_ZOOM_PLUS			208
#define 	KEY_ZOOM_MINUS			217

#ifdef NEMO
	#define 	KEY_ALIGN				241
	#define 	LED_ALIGN				241
#else
	#define 	KEY_ALIGN				251
	#define 	LED_ALIGN				251
 #endif


// MUTATOR KEYS - far from complete..
#define		LED_ZOOM				194
#define		KEY_ZOOM				194





// OCTAVE CIRCLE KEYS
#ifdef NEMO
	#define 	LED_NOTE_C				33
	#define		KEY_NOTE_C				33
	
	#define		LED_NOTE_Cis			44
	#define 	KEY_NOTE_Cis			44
	
	#define		LED_NOTE_D				55
	#define		KEY_NOTE_D				55
	
	#define		LED_NOTE_Dis			66
	#define		KEY_NOTE_Dis			66
	
	#define		LED_NOTE_E				77
	#define		KEY_NOTE_E				77
	
	#define		LED_NOTE_F				88
	#define		KEY_NOTE_F				88
	
	#define		LED_NOTE_Fis			99
	#define		KEY_NOTE_Fis			99
	
	#define		LED_NOTE_G				110
	#define		KEY_NOTE_G				110
	
	#define		LED_NOTE_Gis			121
	#define		KEY_NOTE_Gis			121
	
	#define		LED_NOTE_A				132
	#define		KEY_NOTE_A				132
	
	#define		LED_NOTE_Ais			143
	#define		KEY_NOTE_Ais			143
	
	#define		LED_NOTE_B				154
	#define		KEY_NOTE_B				154
	
	#define		LED_NOTE_Cup			24
	#define		KEY_NOTE_Cup			24
#else
	#define 	LED_NOTE_C				212
	#define		KEY_NOTE_C				212
	
	#define		LED_NOTE_Cis			211
	#define 	KEY_NOTE_Cis			211
	
	#define		LED_NOTE_D				210
	#define		KEY_NOTE_D				210
	
	#define		LED_NOTE_Dis			209
	#define		KEY_NOTE_Dis			209
	
	#define		LED_NOTE_E				208
	#define		KEY_NOTE_E				208
	
	#define		LED_NOTE_F				217
	#define		KEY_NOTE_F				217
	
	#define		LED_NOTE_Fis			225
	#define		KEY_NOTE_Fis			225
	
	#define		LED_NOTE_G				226
	#define		KEY_NOTE_G				226
	
	#define		LED_NOTE_Gis			235
	#define		KEY_NOTE_Gis			235
	
	#define		LED_NOTE_A				236
	#define		KEY_NOTE_A				236
	
	#define		LED_NOTE_Ais			237
	#define		KEY_NOTE_Ais			237
	
	#define		LED_NOTE_B				238
	#define		KEY_NOTE_B				238
	
	#define		LED_NOTE_Cup			239
	#define		KEY_NOTE_Cup			239
#endif


// SCALE relevant keys
#define 	LED_SCALE_SEL			222
#define 	KEY_SCALE_SEL			222


#define 	LED_SCALE_MOD			221
#define		KEY_SCALE_MOD			221

#define 	LED_SCALE_CAD			230
#define		KEY_SCALE_CAD			230

#define 	LED_SCALE_MYSEL			243
#define		KEY_SCALE_MYSEL			243

#define		LED_SCALE_PEN			244
#define		KEY_SCALE_PEN			244

#define		LED_SCALE_WHL			245
#define		KEY_SCALE_WHL			245

#define 	LED_SCALE_MAJ			246
#define 	KEY_SCALE_MAJ			246

#define		LED_SCALE_MIN			247
#define		KEY_SCALE_MIN			247

#define		LED_SCALE_DIM			248
#define		KEY_SCALE_DIM			248

#define		LED_SCALE_CHR			249
#define		KEY_SCALE_CHR			249


// KEYS of the BIG KNOB
#define		KEY_BK1					201
#define		KEY_BK2					200
#define		KEY_BK3					199
#define		KEY_BK4					198
#define		KEY_BK5					197
#define		KEY_BK6					207
#define		KEY_BK7					206
#define		KEY_BK8					216
#define		KEY_BK9					215
#define		KEY_BK100				224
#define		LED_BK100				254
#define		KEY_BK200				233



// ROTARY definition - naming according to frontpanel
#define		ROT_BIGKNOB				0
#ifdef NEMO
	#define		ROT_PIT					1
	#define		ROT_VEL					2
#else
	#define		ROT_VEL					1
	#define		ROT_PIT					2
#endif

#define		ROT_LEN					3
#define		ROT_STA					4
#define		ROT_POS					5
#define		ROT_DIR					6
#define		ROT_AMT					7
#define		ROT_GRV					8
#define		ROT_MCC					9
#define		ROT_MCH					10
#define		ROT_9					11
#define		ROT_8					12
#define		ROT_7					13
#define		ROT_6					14
#define		ROT_5					15
#define 	ROT_4					16
#define		ROT_3					17
#define 	ROT_2					18
#define		ROT_1					19
#define		ROT_0					20


// MUT keys
#define 	LED_EFF					193
#define		KEY_EFF					193
