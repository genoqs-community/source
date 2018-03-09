
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


#include "../_OCT_objects/Phrase.h"




#define DEFSTA 		{0, 0, 0, 0, 0, 0, 0, 0}
#define DEFVEL 		{0, 0, 0, 0, 0, 0, 0, 0}
#define DEFPIT 		{0, 0, 0, 0, 0, 0, 0, 0}
#define DEFLEN 		{0, 0, 0, 0, 0, 0, 0, 0}
#define POLY1		(1<<4)
#define POLY2		(2<<4)
#define POLY3		(3<<4)
#define POLY4		(4<<4)
#define POLY5		(5<<4)
#define POLY6		(6<<4)
#define POLY7		(7<<4)
#define FILLER 		{0, 0, 0}


PhraseStruct PhrasePresets[PHRASE_COUNT_PRESET] = {

		// PHRASE #0, READ-ONLY

		/* 000: */  { DEFSTA, DEFVEL, DEFPIT, DEFLEN, PhraseTypePhraseFwd, FILLER },

		// BANK GREEN: DELAYS and RHYTHMIC DELAYS
		/* 001: */  { {   0,  24,   0,   0,   0,   0,   0,   0}, {   0, -10,   0,   0,   0,   0,   0,   0}, DEFPIT, DEFLEN, PhraseTypePhraseFwd, FILLER },
		/* 002: */  { {   0,  24,  48,   0,   0,   0,   0,   0}, {   0, -10, -20,   0,   0,   0,   0,   0}, DEFPIT, DEFLEN, PhraseTypePhraseRndPit, FILLER },
		/* 003: */  { {   0,  24,  48,  72,   0,   0,   0,   0}, {   0, -10, -20, -30,   0,   0,   0,   0}, DEFPIT, DEFLEN, PhraseTypePhraseFwd, FILLER },
		/* 004: */  { {   0,  12,  26,  42,  60,  80, 102,   0}, {   0, -10, -20, -30, -40,   0,   0,   0}, DEFPIT, DEFLEN, PhraseTypePhraseFwd, FILLER },
		/* 005: */  { {   0,  12,  24,  36,  48,  60,  72,  84}, {   0, -10, -20, -30, -40, -50,   0,   0}, DEFPIT, DEFLEN, PhraseTypePhraseFwd, FILLER },
		/* 006: */  { {   0,  12,  36,  48,   0,   0,   0,   0}, {   0, -39, -30, -20,   0,   0,   0,   0}, DEFPIT, {  26,   0,  14,   0,   0,   0,   0,   0}, PhraseTypePhraseFwd, FILLER },
		/* 007: */  { {   0,  16,  48,  72,   0,   0,   0,   0}, {   0, -39, -30, -20,   0,   0,   0,   0}, DEFPIT, {  26,   0,  14,   0,   0,   0,   0,   0}, PhraseTypePhraseFwd, FILLER },
		/* 008: */  { {   0,  12,  36,  60,  80, 104,   0,   0}, {   0, -39, -30, -17, -20, -40,   0,   0}, DEFPIT, {  22,   0,  14,   0,   0,   0,   0,   0}, PhraseTypePhraseFwd, FILLER },

		/* 009: */  { {   0,  15,  72,  36,   0,   0,   0,   0}, { -30,   0, -40,   0,   0,   0,   0,   0}, DEFPIT, DEFLEN, PhraseTypePhraseFwd, FILLER },
		/* 010: */  { {   0,  24,  36,  72,  84, 120,   0,   0}, {   0, -10, -20, -30, -40, -50,   0,   0}, DEFPIT, DEFLEN, PhraseTypePhraseFwd, FILLER },
		/* 011: */  { {   0,  12,  48,  60,  96, 120,   0,   0}, {   0, -10, -20, -30, -40, -50,   0,   0}, DEFPIT, DEFLEN, PhraseTypePhraseFwd, FILLER },
		/* 012: */  { {   0,  32,  48,  80,  96, 128,   0,   0}, {   0, -10, -20, -30, -40, -50,   0,   0}, DEFPIT, DEFLEN, PhraseTypePhraseFwd, FILLER },
		/* 013: */  { {   0,  12,  24,  72,  84, 108,   0,   0}, {   0, -10, -20, -30, -40, -50,   0,   0}, DEFPIT, DEFLEN, PhraseTypePhraseFwd, FILLER },
		/* 014: */  { {   0,  12,  24,  96, 108, 120,   0,   0}, {   0, -10, -20, -30, -34, -40,   0,   0}, DEFPIT, DEFLEN, PhraseTypePhraseFwd, FILLER },
		/* 015: */  { {   0,  24,  60,  72,  96, 120,   0,   0}, {   0, -10, -20, -30, -40, -50,   0,   0}, DEFPIT, DEFLEN, PhraseTypePhraseFwd, FILLER },
		/* 016: */  { {   0,  12,  24,  36,  84, 120,   0,   0}, {   0, -10, -20, -30, -40, -50,   0,   0}, DEFPIT, DEFLEN, PhraseTypePhraseFwd, FILLER },


		// PITCHED DELAYS and ARPS
		/* 017: */  { {   0,  24,  48,  72,   0,   0,   0,   0}, {   0, -20, -40, -50,   0,   0,   0,   0}, {  12,   0,   7,  12,   0,   0,   0,   0}, DEFLEN, PhraseTypePhraseFwd, FILLER },
		/* 018: */  { {   0,  24,  48,  72,  96,   0,   0,   0}, {   0, -20, -30, -40, -37,   0,   0,   0}, {   0,   5,   7,  12,   7,   0,   0,   0}, DEFLEN, PhraseTypePhraseFwd, FILLER },
		/* 019: */  { {   0,  24,  96, 120,   0,   0,   0,   0}, {   0, -20, -30, -40,   0,   0,   0,   0}, {   0,   9,  12,   7,   0,   0,   0,   0}, DEFLEN, PhraseTypePhraseRndPit, FILLER },
		/* 020: */  { {   0,  12,  48,  60,  96, 108,   0,   0}, {   0, -20, -30, -40, -36, -39,   0,   0}, {   0,   2,   5,   7,  12,   0,   0,   0}, DEFLEN, PhraseTypePhraseRndPit, FILLER },
		/* 021: */  { {   0,  24,  48,  72,  16,   0,   0,   0}, {   0, -20, -40, -50, -27,   0,   0,   0}, {   3,  -2,   5,   7,   8,   0,   0,   0}, DEFLEN, PhraseTypePhraseRndPit, FILLER },
		/* 022: */  { {   0,  36,  72, 108,  21,  75,   0,   0}, {   0, -20, -40, -46, -27, -43,   0,   0}, {   0,  12,   0, -12,   0,  24,   0,   0}, DEFLEN, PhraseTypePhraseFwd, FILLER },
		/* 023: */  { {   0,  12,  24,  36,   6,  24,  60,   0}, {   0, -20, -40, -50, -27, -43, -49,   0}, {   7,  -7,   5,  12,   0,   0,   0,   0}, DEFLEN, PhraseTypePhraseRev, FILLER },
		/* 024: */  { {   0,  32,  37,  64,  80,   0,   0,   0}, {   0, -30, -21,   0, -40,   0,   0,   0}, {  -7,   4,  12,   5,   0,   0,   0,   0}, DEFLEN, PhraseTypePhraseRev, FILLER },

		/* 025: */  { {   0,  24,  48,  72,  96,   0,   0,   0}, {   0, -10, -36, -30, -40,   0,   0,   0}, {   2,   5,   5,   7,  12,   0,   0,   0}, DEFLEN, PhraseTypePhraseRndPit, FILLER },
		/* 026: */  { {   0,  36,  72, 108,  48,   0,   0,   0}, {   0, -20, -40, -36, -33,   0,   0,   0}, {  12,  12,   3,  12,  12,   0,   0,   0}, DEFLEN, PhraseTypePhraseRndPit, FILLER },
		/* 027: */  { {   0,  24,  48,  72,  12,  48, 120,   0}, {   0, -20, -40, -50, -27, -43, -49,   0}, {   7,  -7,   5,  12,   0,   0,   0,   0}, DEFLEN, PhraseTypePhraseRndPit, FILLER },
		/* 028: */  { {   0,  64,  78, 128, 160, 144,   0,   0}, {   0, -30, -21,   0, -40, -32,   0,   0}, {  -7,   4,  12,   5,   5,   3,   0,   0}, DEFLEN, PhraseTypePhraseRev, FILLER },
		/* 029: */  { {   0,  12,  24,  36,   0,   0,   0,   0}, {   0, -20,   0,   0,   0,   0,   0,   0}, {   0,  12,  28,  12,   0,   0,   0,   0}, DEFLEN, PhraseTypePhraseFwd, FILLER },
		/* 030: */  { {   0,  12,  24,  36,   0,   0,   0,   0}, {   0, -20,   0,   0,   0,   0,   0,   0}, {   0,  12,  24,  12,  19,   0,   0,   0}, DEFLEN, PhraseTypePhraseFwd, FILLER },
		/* 031: */  { {   0,  12,  24,  36,  48,   0,   0,   0}, {   0, -20,   0,   0,   0,   0,   0,   0}, {   0,  12,  24,  12,  19,   0,   0,   0}, DEFLEN, PhraseTypePhraseFwd, FILLER },
		/* 032: */  { {   0,  12,  24,  36,  48,   0,   0,   0}, {   0, -20,   0,   0,   0,   0,   0,   0}, {   0,  12,  24,  12,  20,   0,   0,   0}, DEFLEN, PhraseTypePhraseFwd, FILLER },

//		/* 038: */  { {   0,  12,  24,  36,  48,  60,  72,  84}, {   0, -20,   0,   0,   0,   0,   0,   0}, {   0,  16,  24,  17,  19,  17,  12,  19}, DEFLEN, PhraseTypePhraseFwd, FILLER },
//		/* 041: */  { {   0,  12,  24,  36,  48,  60,  72,  84}, {  30,   0,  30,   0,  30,   0,  30,   0}, {   0,  24,  12,  12,  24,  12,  12,  12}, DEFLEN, PhraseTypePhraseFwd, FILLER },
//		/* 043: */  { {   0,  12,  24,  36,  48,  60,  72,  84}, {  30,   0,  30,   0,  30,   0,  30,   0}, {   0,  24,  12,  12,  19,  12,  12,   7}, DEFLEN, PhraseTypePhraseFwd, FILLER },
//		/* 044: */  { {   0,  12,  24,  36,  48,  60,  72,  84}, {  30,   0,  30,   0,  30,   0,  30,   0}, {   0,   7,  12,  12,  15,  12,  12,   0}, DEFLEN, PhraseTypePhraseFwd, FILLER },

		// STA WOBBLES
		/* 033: */  { {   0,   1,   0,   0,   0,   0,   0,   0}, DEFVEL, DEFPIT, DEFLEN, POLY1 | PhraseTypePhraseRndAll, FILLER },
		/* 034: */  { {   0,   1,   2,   0,   0,   0,   0,   0}, DEFVEL, DEFPIT, DEFLEN, POLY1 | PhraseTypePhraseRndAll, FILLER },
		/* 035: */  { {   0,   1,   2,   3,   0,   0,   0,   0}, DEFVEL, DEFPIT, DEFLEN, POLY1 | PhraseTypePhraseRndAll, FILLER },
		/* 036: */  { {   0,   1,   2,   3,   4,   5,   7,   9}, DEFVEL, DEFPIT, DEFLEN, POLY1 | PhraseTypePhraseRndAll, FILLER },
		// VEL WOBBLES
		/* 037: */  { DEFSTA, {   0,   10,  -10,   0,   0,   0,   0,   0}, DEFPIT, DEFLEN, POLY1 | PhraseTypePhraseRndAll, FILLER },
		/* 038: */  { DEFSTA, {   0,   10,  -10,  20, -20,   0,   0,   0}, DEFPIT, DEFLEN, POLY1 | PhraseTypePhraseRndAll, FILLER },
		/* 039: */  { DEFSTA, {   0,   10,  -10,  20, -20,  35, -35,   0}, DEFPIT, DEFLEN, POLY1 | PhraseTypePhraseRndAll, FILLER },
		/* 040: */  { DEFSTA, {   0,   20,  -20,  35, -35,  50, -50,   0}, DEFPIT, DEFLEN, POLY1 | PhraseTypePhraseRndAll, FILLER },
		// STA + VEL WOBBLES
		/* 041: */  { {   0,   1,   0,   0,   0,   0,   0,   0}, {   0,   10,  -10,   0,   0,   0,   0,   0}, DEFPIT, DEFLEN, POLY1 | PhraseTypePhraseRndAll, FILLER },
		/* 042: */  { {   0,   1,   2,   0,   0,   0,   0,   0}, {   0,   10,  -10,  20, -20,   0,   0,   0}, DEFPIT, DEFLEN, POLY1 | PhraseTypePhraseRndAll, FILLER },
		/* 043: */  { {   0,   1,   2,   3,   0,   0,   0,   0}, {   0,   10,  -10,  20, -20,  35, -35,   0}, DEFPIT, DEFLEN, POLY1 | PhraseTypePhraseRndAll, FILLER },
		/* 044: */  { {   0,   1,   2,   3,   4,   5,   7,   9}, {   0,   20,  -20,  35, -35,  50, -50,   0}, DEFPIT, DEFLEN, POLY1 | PhraseTypePhraseRndAll, FILLER },
		// STA + VEL + PIT WOBBLES
		/* 045: */  { {   0,   1,   0,   0,   0,   0,   0,   0}, {   0,   10,  -10,   0,   0,   0,   0,   0}, {   0,  12,  24,  12,  19,  17,  12,  19}, DEFLEN, POLY1 | PhraseTypePhraseRndAll, FILLER },
		/* 046: */  { {   0,   1,   2,   0,   0,   0,   0,   0}, {   0,   10,  -10,  20, -20,   0,   0,   0}, {   0,  12,  24,  12,  19,  17,  12,  19}, DEFLEN, POLY1 | PhraseTypePhraseRndAll, FILLER },
		/* 047: */  { {   0,   1,   2,   3,   0,   0,   0,   0}, {   0,   10,  -10,  20, -20,  35, -35,   0}, {   0,  12,  24,  12,  19,  17,  12,  19}, DEFLEN, POLY1 | PhraseTypePhraseRndAll, FILLER },
		/* 048: */  { {   0,   1,   2,   3,   4,   5,   7,   9}, {   0,   20,  -20,  35, -35,  50, -50,   0}, {   0,  12,  24,  12,  19,  17,  12,  19}, DEFLEN, POLY1 | PhraseTypePhraseRndAll, FILLER },


		//		// STA WOBBLES
//		/* 049: */  { {   0,   1,   0,   0,   0,   0,   0,   0}, DEFVEL, DEFPIT, DEFLEN, POLY1 | PhraseTypePhraseRndAll, FILLER },
//		/* 050: */  { {   0,   1,   2,   0,   0,   0,   0,   0}, DEFVEL, DEFPIT, DEFLEN, POLY1 | PhraseTypePhraseRndAll, FILLER },
//		/* 051: */  { {   0,   1,   2,   3,   0,   0,   0,   0}, DEFVEL, DEFPIT, DEFLEN, POLY1 | PhraseTypePhraseRndAll, FILLER },
//		/* 052: */  { {   0,   1,   2,   3,   4,   0,   0,   0}, DEFVEL, DEFPIT, DEFLEN, POLY1 | PhraseTypePhraseRndAll, FILLER },
//		/* 053: */  { {   0,   1,   2,   3,   4,   5,   0,   0}, DEFVEL, DEFPIT, DEFLEN, POLY1 | PhraseTypePhraseRndAll, FILLER },
//		/* 054: */  { {   0,   1,   2,   3,   4,   5,   6,   0}, DEFVEL, DEFPIT, DEFLEN, POLY1 | PhraseTypePhraseRndAll, FILLER },
//		/* 055: */  { {   0,   1,   2,   3,   4,   5,   6,   7}, DEFVEL, DEFPIT, DEFLEN, POLY1 | PhraseTypePhraseRndAll, FILLER },
//		/* 056: */  { {   0,   1,   2,   3,   4,   5,   7,   9}, DEFVEL, DEFPIT, DEFLEN, POLY1 | PhraseTypePhraseRndAll, FILLER },
//
//		// VEL WOBBLES
//		/* 057: */  { DEFSTA, {   0,   10,  -10,   0,   0,   0,   0,   0}, DEFPIT, DEFLEN, POLY1 | PhraseTypePhraseRndAll, FILLER },
//		/* 058: */  { DEFSTA, {   0,   10,  -10,  20, -20,   0,   0,   0}, DEFPIT, DEFLEN, POLY1 | PhraseTypePhraseRndAll, FILLER },
//		/* 059: */  { DEFSTA, {   0,   10,  -10,  20, -20,  30, -30,   0}, DEFPIT, DEFLEN, POLY1 | PhraseTypePhraseRndAll, FILLER },
//		/* 060: */  { DEFSTA, {   0,   10,  -10,  20, -20,  30, -30,   0}, DEFPIT, DEFLEN, POLY1 | PhraseTypePhraseRndAll, FILLER },
//		/* 061: */  { DEFSTA, {   0,   10,  -10,  20, -20,  35, -35,   0}, DEFPIT, DEFLEN, POLY1 | PhraseTypePhraseRndAll, FILLER },
//		/* 062: */  { DEFSTA, {   0,   15,  -15,  25, -25,  40, -40,   0}, DEFPIT, DEFLEN, POLY1 | PhraseTypePhraseRndAll, FILLER },
//		/* 063: */  { DEFSTA, {   0,   15,  -15,  30, -30,  45, -45,   0}, DEFPIT, DEFLEN, POLY1 | PhraseTypePhraseRndAll, FILLER },
//		/* 064: */  { DEFSTA, {   0,   20,  -20,  35, -35,  50, -50,   0}, DEFPIT, DEFLEN, POLY1 | PhraseTypePhraseRndAll, FILLER },


		// STRUMS
		// If the index of the strum changes, need modification of PHRASE_STRUM_PRESET_NUM.
		/* 065: */  { {   0,   0,   1,   1,   2,   2,   3,   0}, DEFVEL, DEFPIT, DEFLEN, PhraseTypeStrumFwd, FILLER },
		/* 066: */  { {   0,   1,   2,   2,   3,   3,   6,   0}, DEFVEL, DEFPIT, DEFLEN, PhraseTypeStrumFwd, FILLER },
		/* 067: */  { {   0,   1,   3,   4,   5,   6,   9,   0}, DEFVEL, DEFPIT, DEFLEN, PhraseTypeStrumFwd, FILLER },
		/* 068: */  { {   0,   2,   4,   6,   9,  12,  16,   0}, DEFVEL, DEFPIT, DEFLEN, PhraseTypeStrumFwd, FILLER },
		/* 069: */  { {   0,   2,   5,   8,  11,  15,  19,   0}, DEFVEL, DEFPIT, DEFLEN, PhraseTypeStrumFwd, FILLER },
		/* 070: */  { {   0,   3,   6,   9,  13,  18,  24,   0}, DEFVEL, DEFPIT, DEFLEN, PhraseTypeStrumFwd, FILLER },
		/* 071: */  { {   0,   3,   7,  10,  15,  21,  29,   0}, DEFVEL, DEFPIT, DEFLEN, PhraseTypeStrumFwd, FILLER },
		/* 072: */  { {   0,   4,   8,  13,  19,  27,  36,   0}, DEFVEL, DEFPIT, DEFLEN, PhraseTypeStrumFwd, FILLER },
		/* 073: */  { {   0,   5,  10,  17,  23,  30,  45,   0}, DEFVEL, DEFPIT, DEFLEN, PhraseTypeStrumFwd, FILLER },
		/* 074: */  { {   0,   5,  13,  21,  31,  48,  61,   0}, DEFVEL, DEFPIT, DEFLEN, PhraseTypeStrumFwd, FILLER },

};





