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



// Holds the phrase number currently being edited.
card8 PhraseCurPhraseIx = 0;

// Holds the note number of the phrase currently being edited.
card8 PhraseCurNoteIx = 0;

int16 PhraseMultiVal = -1;

// Specifies the currently shown phrase attribute in the Octo view of the editor.
card8 PhraseCurAttrib = PhraseAttribSta;

booln PhraseShowPhraseBo = False;

// The phrase repository.
PhraseStruct PhraseRepos[PHRASE_COUNT_USER_RUNTIME];

// Clipboard for copy/paste operations on phrase objects.
PhraseStruct PhraseClipBoard;

// Undo buffer for current phrase in the phrase editor.
PhraseStruct PhraseUndoBuf;


#if PHRASE_PRINT
	#define PRINT		// d_iag_printf
#else
	#define PRINT		#error
#endif





void PhraseInit( void )
{
	intn ix;

	memset( PhraseRepos, 0, sizeof(PhraseRepos) );
	for ( ix = 0; ix < PHRASE_COUNT_USER_RUNTIME; ix++ ) {
		PhraseSetType( ix, PhraseTypePhraseFwd );
	}

	memset( &PhraseClipBoard, 0, sizeof(PhraseClipBoard) );
	PhraseClipBoard.polyType = PhraseTypePhraseFwd;

	memset( &PhraseUndoBuf, 0, sizeof(PhraseUndoBuf) );
	PhraseUndoBuf.polyType = PhraseTypePhraseFwd;

	PhraseReposRestore();
}

void PhraseReposRestore( void )
{
	// It's natural to have more presets than user slots. Make sure not to copy too many into the user slots.
	size_t copySzBt = min( sizeof(PhraseRepos), sizeof(PhrasePresets) );
	memcpy( PhraseRepos, PhrasePresets, copySzBt );
}

void PhrasePrintRepos( void )
{
#if PHRASE_PRINT
	card8 ix;
	PRINT( "\n" );
	for ( ix = 0; ix < PHRASE_COUNT_USER_RUNTIME; ix++ ) {
		PhrasePrint( ix );
	}
#endif
}


void PhrasePtPrint( PhraseStruct* phrasePt )
{
#if PHRASE_PRINT

	// Print phrase number.
	PRINT( "/* Phrase from Pt */  {" );

	// Print each of the attribute vectors.
	PhrasePrintVec( phrasePt->sta, "DEFSTA", True );
	PhrasePrintVec( phrasePt->vel, "DEFVEL", False );
	PhrasePrintVec( phrasePt->pit, "DEFPIT", False );
	PhrasePrintVec( phrasePt->len, "DEFLEN", False );

//	// Print the phrase polyphony, if set.
//	switch( (phrasePt->polyType & 0xf0) ) {
//	case POLY1: PRINT( "POLY1 | "); break;
//	case POLY2: PRINT( "POLY2 | "); break;
//	case POLY3: PRINT( "POLY3 | "); break;
//	case POLY4: PRINT( "POLY4 | "); break;
//	case POLY5: PRINT( "POLY5 | "); break;
//	case POLY6: PRINT( "POLY6 | "); break;
//	case POLY7: PRINT( "POLY7 | "); break;
//	}

	/*
	PhraseTypeStrumFwd  	= 1,
	PhraseTypeStrumRev  	= 2,
	PhraseTypeStrumRndPit  	= 3,
	PhraseTypePhraseFwd 	= 4,
	PhraseTypePhraseRev 	= 5,
	PhraseTypePhraseRndPit 	= 6,	// Shuffle pitch only.
	PhraseTypePhraseRndAll	= 7,	// Shuffle all attribs, wobble.
	 */

	// Print the phrase type.
	switch( (PhraseTypeEnum) (phrasePt->polyType & 0x0f) ) {
	case PhraseTypeStrumFwd: 	PRINT("PhraseTypeStrumFwd, "); break;
	case PhraseTypeStrumRev: 	PRINT("PhraseTypeStrumRev, "); break;
	case PhraseTypeStrumRndPit: 	PRINT("PhraseTypeStrumRndPit, "); break;
	case PhraseTypePhraseFwd: 	PRINT("PhraseTypePhraseFwd, "); break;
	case PhraseTypePhraseRev: 	PRINT("PhraseTypePhraseRev, "); break;
	case PhraseTypePhraseRndPit:	PRINT("PhraseTypePhraseRndPit, "); break;
	case PhraseTypePhraseRndAll:	PRINT("PhraseTypePhraseRndAll, "); break;
	}

	PRINT( "FILLER },\n");
#endif
}

void PhrasePrint( card8 phraseIx )
{
#if PHRASE_PRINT
	PhraseStruct* phrasePt = PhraseRepos + phraseIx;

	// Print phrase number.
	PRINT( "/* %03d: */  { ", phraseIx );

	// Print each of the attribute vectors.
	PhrasePrintVec( phrasePt->sta, "DEFSTA", True );
	PhrasePrintVec( phrasePt->vel, "DEFVEL", False );
	PhrasePrintVec( phrasePt->pit, "DEFPIT", False );
	PhrasePrintVec( phrasePt->len, "DEFLEN", False );

//	// Print the phrase polyphony, if set.
//	switch( (phrasePt->polyType & 0xf0) ) {
//	case POLY1: PRINT( "POLY1 | "); break;
//	case POLY2: PRINT( "POLY2 | "); break;
//	case POLY3: PRINT( "POLY3 | "); break;
//	case POLY4: PRINT( "POLY4 | "); break;
//	case POLY5: PRINT( "POLY5 | "); break;
//	case POLY6: PRINT( "POLY6 | "); break;
//	case POLY7: PRINT( "POLY7 | "); break;
//	}

	/*
	PhraseTypeStrumFwd  	= 1,
	PhraseTypeStrumRev  	= 2,
	PhraseTypeStrumRndPit  	= 3,
	PhraseTypePhraseFwd 	= 4,
	PhraseTypePhraseRev 	= 5,
	PhraseTypePhraseRndPit 	= 6,	// Shuffle pitch only.
	PhraseTypePhraseRndAll	= 7,	// Shuffle all attribs, wobble.
	 */

	// Print the phrase type.
	switch( (PhraseTypeEnum) (phrasePt->polyType & 0x0f) ) {
	case PhraseTypeStrumFwd: 	PRINT("PhraseTypeStrumFwd, "); break;
	case PhraseTypeStrumRev: 	PRINT("PhraseTypeStrumRev, "); break;
	case PhraseTypeStrumRndPit: 	PRINT("PhraseTypeStrumRndPit, "); break;
	case PhraseTypePhraseFwd: 	PRINT("PhraseTypePhraseFwd, "); break;
	case PhraseTypePhraseRev: 	PRINT("PhraseTypePhraseRev, "); break;
	case PhraseTypePhraseRndPit:	PRINT("PhraseTypePhraseRndPit, "); break;
	case PhraseTypePhraseRndAll:	PRINT("PhraseTypePhraseRndAll, "); break;
	}

	PRINT( "FILLER },\n");
#endif
}

void PhrasePrintVec( int8* vecPt, const char8* textSt, booln staBo )
{
#if PHRASE_PRINT
	intn ix;

	// See if any non-zero value is defined in the vector.
	for ( ix = 0; ix < PHRASE_NOTE_COUNT; ix++ ) {
		if ( vecPt[ix] != 0 ) break;
	}

	// Nothing defined. Print the DEFxxx string. */
	if ( ix == PHRASE_NOTE_COUNT ) {
		PRINT( "%s, ", textSt );
		return;
	}

	// Explicit values defined. Print vector in full detail.
	PRINT( "{");
	for (ix = 0; ix < PHRASE_NOTE_COUNT; ix++ ) {
		intn val = staBo ? ((card8) (vecPt[ix])) : vecPt[ix];
		PRINT( "%4d", val );
		if ( ix < PHRASE_NOTE_COUNT-1 ) PRINT(",");
	}
	PRINT( "}, ");
#endif
}




void PhraseSetCurrent( card8 phraseIx )
{
	if ( phraseIx < PHRASE_COUNT_USER_RUNTIME ) {
		PhraseCurPhraseIx = phraseIx;
		PhraseUndoBuf = PhraseRepos[phraseIx];
	}
}


void PhraseCopy( void )
{
	if ( PhraseCurPhraseIx < PHRASE_COUNT_USER_RUNTIME ) {
		PhraseClipBoard = PhraseRepos[PhraseCurPhraseIx];
	}
}

void PhrasePaste( void )
{
	if ( (PhraseCurPhraseIx > 0) && (PhraseCurPhraseIx < PHRASE_COUNT_USER_RUNTIME) ) {
		PhraseUndoBuf = PhraseRepos[PhraseCurPhraseIx];
		PhraseRepos[PhraseCurPhraseIx] = PhraseClipBoard;
	}
}

void PhraseUndo( void )
{
	if ( (PhraseCurPhraseIx > 0) && (PhraseCurPhraseIx < PHRASE_COUNT_USER_RUNTIME) ) {
		PhraseRepos[PhraseCurPhraseIx] = PhraseUndoBuf;
	}
}



#if 0
booln PhraseIsStrum( card8 phraseIx )
{
	card8 type = PhraseGetType( phraseIx );
	return (type == PhraseTypeStrumFwd) || (type == PhraseTypeStrumRev) || (type == PhraseTypeStrumRndPit);
}

booln PhraseIsPhrase( card8 phraseIx )
{
	return !PhraseIsStrum(phraseIx);
}

booln PhraseIsFwd( card8 phraseIx )
{
	card8 type = PhraseGetType( phraseIx );
	return (type == PhraseTypePhraseFwd) || (type == PhraseTypeStrumFwd);
}

booln PhraseIsRev( card8 phraseIx )
{
	card8 type = PhraseGetType( phraseIx );
	return (type == PhraseTypePhraseRev) || (type == PhraseTypeStrumRev);
}

booln PhraseIsRndPit( card8 phraseIx )
{
	card8 type = PhraseGetType( phraseIx );
	return (type == PhraseTypePhraseRndPit) || (type == PhraseTypeStrumRndPit);
}

booln PhraseIsRndAll( card8 phraseIx )
{
	card8 type = PhraseGetType( phraseIx );
	return (type == PhraseTypePhraseRndAll);
}
#endif



booln PhraseIsEmpty( card8 phraseIx )
{
	intn ix;
	for ( ix = 0; ix < PHRASE_NOTE_COUNT; ix++ ) {
		if ( PhraseRepos[phraseIx].sta[ix] != 0 ) return False;
		if ( PhraseRepos[phraseIx].vel[ix] != 0 ) return False;
		if ( PhraseRepos[phraseIx].pit[ix] != 0 ) return False;
		if ( PhraseRepos[phraseIx].len[ix] != 0 ) return False;
	}
	if ( PhraseRepos[phraseIx].polyType != PhraseTypePhraseFwd ) {
		return False;
	}

	return True;
}

booln PhrasePtIsStrum( PhraseStruct* phrasePt )
{
	card8 type = PhrasePtGetType( phrasePt );
	return (type == PhraseTypeStrumFwd) || (type == PhraseTypeStrumRev) || (type == PhraseTypeStrumRndPit);
}

booln PhrasePtIsFwd( PhraseStruct* phrasePt )
{
	card8 type = PhrasePtGetType( phrasePt );
	return (type == PhraseTypePhraseFwd) || (type == PhraseTypeStrumFwd);
}

booln PhrasePtIsRev( PhraseStruct* phrasePt )
{
	card8 type = PhrasePtGetType( phrasePt );
	return (type == PhraseTypePhraseRev) || (type == PhraseTypeStrumRev);
}

booln PhrasePtIsRndPit( PhraseStruct* phrasePt )
{
	card8 type = PhrasePtGetType( phrasePt );
	return (type == PhraseTypePhraseRndPit) || (type == PhraseTypeStrumRndPit);
}

booln PhrasePtIsRndAll( PhraseStruct* phrasePt )
{
	card8 type = PhrasePtGetType( phrasePt );
	return (type == PhraseTypePhraseRndAll);
}











card8 PhraseGetNoteCount( card8 phraseIx )
{
	return PhrasePtGetNoteCount( PhraseRepos+phraseIx );
}

card8 PhrasePtGetNoteCount( PhraseStruct* phrasePt )
{
	intn ix;

	for ( ix = PHRASE_NOTE_COUNT-1; ix >= 0; ix-- ) {
		if ( phrasePt->sta[ix] != 0 ) break;
		if ( phrasePt->pit[ix] != 0 ) break;
		if ( phrasePt->vel[ix] != 0 ) break;
		if ( phrasePt->len[ix] != 0 ) break;
	}
	return ix+1;
}



void PhraseClear( card8 phraseIx )
{
	if ( (phraseIx > 0) && (phraseIx < PHRASE_COUNT_USER_RUNTIME) ) {
		if ( !PhraseIsEmpty(phraseIx) ) {
			memset( &PhraseRepos[phraseIx], 0, sizeof(PhraseStruct) );
			PhraseRepos[phraseIx].polyType = PhraseTypePhraseFwd;
		}
		else {
			PhraseRestore( phraseIx );
		}
	}
}

void PhraseRestore( card8 phraseIx )
{
	if ( (phraseIx > 0) && (phraseIx < PHRASE_COUNT_USER_RUNTIME) && (phraseIx < sizeof(PhrasePresets) / sizeof(PhraseStruct)) ) {
		PhraseRepos[phraseIx] = PhrasePresets[phraseIx];
	}
}

void PhraseClearVel( card8 phraseIx )
{
	if ( (phraseIx > 0) && (phraseIx < PHRASE_COUNT_USER_RUNTIME) ) {
		memset( PhraseRepos[phraseIx].vel, 0, PHRASE_NOTE_COUNT );
	}
}

void PhraseClearPit( card8 phraseIx )
{
	if ( (phraseIx > 0) && (phraseIx < PHRASE_COUNT_USER_RUNTIME) ) {
		memset( PhraseRepos[phraseIx].pit, 0, PHRASE_NOTE_COUNT );
	}
}

void PhraseClearLen( card8 phraseIx )
{
	if ( (phraseIx > 0) && (phraseIx < PHRASE_COUNT_USER_RUNTIME) ) {
		memset( PhraseRepos[phraseIx].len, 0, PHRASE_NOTE_COUNT );
	}
}

void PhraseClearSta( card8 phraseIx )
{
	if ( (phraseIx > 0) && (phraseIx < PHRASE_COUNT_USER_RUNTIME) ) {
		memset( PhraseRepos[phraseIx].sta, 0, PHRASE_NOTE_COUNT );
	}
}

void PhraseRandomize( card8 phraseIx )
{
	PhraseRandomizeVel(phraseIx);
	PhraseRandomizePit(phraseIx);
	PhraseRandomizeLen(phraseIx);
	PhraseRandomizeSta(phraseIx);
}

void PhraseRandomizeVel( card8 phraseIx )
{
	intn ix;
	for ( ix = 0; ix < PHRASE_NOTE_COUNT; ix++ ) {
		PhraseSetVel( phraseIx, ix, (rand() % 80) - 40 );
	}
}

void PhraseRandomizePit( card8 phraseIx )
{
	intn ix;
	for ( ix = 0; ix < PHRASE_NOTE_COUNT; ix++ ) {
		PhraseSetPit( phraseIx, ix, (rand() % 80) - 40 );
	}
}

void PhraseRandomizeLen( card8 phraseIx )
{
	intn ix;
	for ( ix = 0; ix < PHRASE_NOTE_COUNT; ix++ ) {
		PhraseSetLen( phraseIx, ix, (rand() % 80) - 40 );
	}
}

void PhraseRandomizeSta( card8 phraseIx )
{
	intn ix;
	for ( ix = 0; ix < PHRASE_NOTE_COUNT; ix++ ) {
		PhraseSetSta( phraseIx, ix, (rand() % 127) );
	}
}



void PhraseMultiTweakStart( booln editBo )
{
	PhraseMultiVal = -1;
}

void PhraseMultiTweakReset( void )
{
	PhraseMultiVal = -1;
}


// Change the phrase number of a particular step
void PhraseEditStepNumber( Stepstruct* stepPt, intn direction )
{
	// Ignore inactive steps.
	if (Step_get_status(stepPt, STEPSTAT_TOGGLE) == OFF) return;

	if ( PhraseMultiVal >= 0 ) {
		stepPt->phrase_num = PhraseMultiVal;
	}

	else {
		modify_parameter( &stepPt->phrase_num, 0, STEP_MAX_GROOVE, direction, OFF, FIXED);
		PhraseSetCurrent( stepPt->phrase_num );
		PhraseMultiVal = stepPt->phrase_num;
	}

	// Recall the neutral value for the GRV attribute - disabled on Lars' hint!
	// stepPt->phrase_data = (PhraseGetType(stepPt->phrase_num) << 4) | PHRASE_GRV_FACT_NEUTRAL;

	// Set the phrase timer. While the timer is ticking the phrase index will be shown in the circle.
	PHRASE_TIMER = ON;
	cyg_alarm_initialize(alarm_hdl, cyg_current_time() + TIMEOUT_VALUE, 0);
}



// Change the phrase number that is being edited
void PhraseEditNumber( intn direction )
{
	modify_parameter( &PhraseCurPhraseIx, 0, STEP_MAX_GROOVE, direction, OFF, FIXED);

	// Set the phrase timer. While the timer is ticking the phrase index will be shown in the circle.
	PHRASE_TIMER = ON;
	cyg_alarm_initialize(alarm_hdl, cyg_current_time() + TIMEOUT_VALUE, 0);
}




void PhraseEditStepFactor( Stepstruct* stepPt, intn direction )
{
	// Ignore inactive steps.
	if (Step_get_status(stepPt, STEPSTAT_TOGGLE) == OFF) return;

	if ( PhraseMultiVal >= 0 ) {
		stepPt->phrase_data = (stepPt->phrase_data & 0xf0) | (PhraseMultiVal & 0x0f);
	}

	else {
		card8 temp = stepPt->phrase_data & 0x0f;
		modify_parameter( &temp, 0, 15, direction, ON, FIXED);
		stepPt->phrase_data = (stepPt->phrase_data & 0xf0) | (temp & 0x0f);
		PhraseMultiVal = temp;
	}
}

void PhraseEditStepType( Stepstruct* stepPt, intn direction )
{
	// Ignore inactive steps.
	if (Step_get_status(stepPt, STEPSTAT_TOGGLE) == OFF) return;

	if ( PhraseMultiVal >= 0 ) {
		stepPt->phrase_data = (stepPt->phrase_data & 0x0f) | (PhraseMultiVal << 4);
	}

	else {
		card8 temp = ((stepPt->phrase_data & 0xf0) >> 4);
		modify_parameter( &temp, PhraseTypeStrumFwd, PhraseTypePhraseRndAll, direction, ON, FIXED);
		stepPt->phrase_data = (stepPt->phrase_data & 0x0f) | (temp << 4);
		PhraseMultiVal = temp;
	}
}

void PhraseSetStepFactor( Stepstruct* stepPt, intn factor )
{
	stepPt->phrase_data = (stepPt->phrase_data & 0xf0) | (factor & 0x0f);
}


void PhraseSetStepType( Stepstruct* stepPt, PhraseTypeEnum type )
{
	stepPt->phrase_data = (stepPt->phrase_data & 0x0f) | (type << 4);
}








// Show the strum value into the outter circle
void PhraseShowPhraseNum( int16 phraseNum )
{
	MIR_write_numeric_C( (card8) phraseNum );
}




void PhraseEditVel( card8 phraseIx, card8 noteIx, intn direction )
{
	if ( (phraseIx > 0) && (phraseIx < PHRASE_COUNT_USER_RUNTIME) && (noteIx < PHRASE_NOTE_COUNT) ) {
		modify_signed_parameter( &PhraseRepos[phraseIx].vel[noteIx], STEP_MIN_VELOCITY, STEP_MAX_VELOCITY, direction, ON, FIXED );
	}
}

void PhraseEditPit( card8 phraseIx, card8 noteIx, intn direction )
{
	if ( (phraseIx > 0) && (phraseIx < PHRASE_COUNT_USER_RUNTIME) && (noteIx < PHRASE_NOTE_COUNT) ) {
		modify_signed_parameter( &PhraseRepos[phraseIx].pit[noteIx], STEP_MIN_PITCH, STEP_MAX_PITCH, direction, ON, FIXED );
	}
}

void PhraseEditLen( card8 phraseIx, card8 noteIx, intn direction )
{
	if ( (phraseIx > 0) && (phraseIx < PHRASE_COUNT_USER_RUNTIME) && (noteIx < PHRASE_NOTE_COUNT) ) {
		modify_signed_parameter( &PhraseRepos[phraseIx].len[noteIx], -40, 40, direction, ON, FIXED );
	}
}

void PhraseEditSta( card8 phraseIx, card8 noteIx, intn direction )
{
	if ( (phraseIx > 0) && (phraseIx < PHRASE_COUNT_USER_RUNTIME) && (noteIx < PHRASE_NOTE_COUNT) ) {
		modify_parameter( &PhraseRepos[phraseIx].sta[noteIx], 0, 192, direction, ON, FIXED );
	}
}

void PhraseEditType( card8 phraseIx, intn direction )
{
	if ( (phraseIx > 0) && (phraseIx < PHRASE_COUNT_USER_RUNTIME) ) {
		card8 type = PhraseGetType( phraseIx );
		modify_parameter( &type, PhraseTypePhraseFwd, PhraseTypePhraseRndAll, direction, ON, FIXED );
		PhraseSetType( phraseIx, type );
	}
}

void PhraseEditPoly( card8 phraseIx, card8 poly )
{
	if ( (phraseIx > 0) && (phraseIx < PHRASE_COUNT_USER_RUNTIME) ) {
		if ( PhraseGetPoly(phraseIx) != poly ) {
			PhraseSetPoly( phraseIx, poly );
		}
		else {
			PhraseSetPoly( phraseIx, 0 );
		}
	}
}


void PhraseEditGrv( card8 phraseIx, intn direction )
{
	if ( phraseIx == 0 || phraseIx >= PHRASE_COUNT_USER_RUNTIME ) {
		return;
	}
	PhrasePtEditGrv( PhraseRepos + phraseIx, direction, True );
}

void PhrasePtApplyGrv( PhraseStruct* phrasePt, intn factor )
{
	factor -= PHRASE_GRV_FACT_NEUTRAL;
	intn direction = (factor < 0 ? DEC : INC);
	factor = abs( factor );
	intn ix;

	for ( ix = 0; ix < factor; ix++ ) {
		PhrasePtEditGrv( phrasePt, direction, False );
	}
}


void PhrasePtEditGrv( PhraseStruct* phrasePt, intn direction, booln protectBo )
{
	intn ix;

	// 12 ticks per 16th, 48 ticks PQN.
	// Straight: 1/64:3, 1/32:6, 1/16:12, 1/8:24, 1/4:48, 1/2:96, 1/1:192.
	// Triads:           1/32:4, 1/16:8,  1/8:16, 1/4:32, 1/2:64
	// Dotted:           1/32:9, 1/16:18, 1/8:36, 1/4:72, 1/2:96
	//
	// Analysis:  3=straight, 4=triad, 5=dotted(kinda), 6=straight, 8=triad, 9=dotted, 12=straight, 16=triad, 18=dotted, 24=straight, 32=triad, 36=dotted,
	//            48=straight, 64=triad, 72=dotted, 96=straight
	//
	// Note the straight->triad->dotted->straight cycle, or S->T->D->S. This is how we want to move through the grooves.
	//
	// Forward translations:  S -> T -> D -> S
	// S->T:  T = S + S/3  (or T = 4/3 * S)
	// T->D:  D = T + T/8  (or D = 9/8 * T)
	// D->S:  S = D/3 * 4
	//
	// Backward translations:  S -> D -> T -> S
	// S->D:  D = S/4 * 3
	// D->T:  T = D/9 * 8
	// T->S:  S = T/4 * 3


	// This vector holds analysis data based on the info above.
	typedef enum { S, T, D, unknown } timingEnum;
	static card8 timingVec[16][2] = { {6,S}, {8,T}, {9,D}, {12,S}, {16,T}, {18,D}, {24,S}, {32,T}, {36,D}, {48,S}, {64,T}, {72,D}, {96,S} };

	// Find the first non-zero STA value defined for the phrase.
	card8 firstSta;
	for ( ix = 0; ix < PHRASE_NOTE_COUNT; ix++ ) {
		if ( (firstSta = phrasePt->sta[ix]) != 0 ) {
			break;
		}
	}

	// Find the timing class (Straight, Triad, Dotted) that goes with firstSta.
	timingEnum firstStaTiming = unknown;
	for ( ix = 0; ix < sizeof(timingVec)/2; ix++ ) {
		if ( timingVec[ix][0] == firstSta ) {
			firstStaTiming = timingVec[ix][1];
			break;
		}
	}

	// We cannot use timingVec for firstSta values <= 6. The logic breaks there.
	if ( direction == DEC && firstSta <= 6 ) {
		firstStaTiming = unknown;
	}

	// Loop twice through our groove update code. When during the first loop the
	// groove multiply or divide would result in a STA value that is too large or small,
	// the loop is aborted and we return from this function doing nothing.

	int loopIx = protectBo ? 0 : 1;
	for ( ; loopIx < 2; loopIx++ ) {

		// If moving upward, change the grove into one of the next higher timing class.
		// For example, if the groove is straight, change it into a triad.

		if ( direction == INC ) {
			for ( ix = 0; ix < PHRASE_NOTE_COUNT; ix++ ) {
				intn newSta, curSta = phrasePt->sta[ix];

				switch (firstStaTiming) {
				case S: 	newSta = curSta + curSta/3; break; // S -> T
				case T: 	newSta = curSta + curSta/8; break; // T -> D
				case D: 	newSta = (curSta/3) * 4; 	break; // D -> S.
				default: 	newSta = curSta * 2; 		break;
				}

				if ( newSta > PHRASE_MAX_STA ) {
					if ( protectBo ) return;
					else newSta = curSta;
				}
				if ( loopIx == 1 ) phrasePt->sta[ix] = newSta;
			}
		}

		// And do similar for downward going groove change.
		else {
			for ( ix = 0; ix < PHRASE_NOTE_COUNT; ix++ ) {
				intn newSta, curSta = phrasePt->sta[ix];

				switch (firstStaTiming) {
				case S: 	newSta = (curSta/4) * 3; break; // S -> D
				case D: 	newSta = (curSta/9) * 8; break; // D -> T
				case T: 	newSta = (curSta/4) * 3; break; // T -> S.
				default: 	newSta = curSta / 2; 	 break;
				}

				if ( curSta && newSta < 3 ) {
					if ( protectBo ) return;
					else newSta = curSta;
				}
				if ( loopIx == 1 ) phrasePt->sta[ix] = newSta;
			}
		}
	}
}



void PhrasePtShufflePit( PhraseStruct* phrasePt, card8 noteCt )
{
	card8 ix;
	card8 tmp;
	card8 srcIx;

	if ( noteCt <= 1 ) return;

	// Loop through the array elements
	for ( ix = 0; ix < noteCt; ix++ ) {

		// Calc the index to swap with.
		srcIx = ix + (rand() % (noteCt - ix));

		tmp = phrasePt->pit[ix]; phrasePt->pit[ix] = phrasePt->pit[srcIx]; phrasePt->pit[srcIx] = tmp;
	}
}

void PhrasePtShuffleAll( PhraseStruct* phrasePt, card8 noteCt )
{
	card8 ix;
	card8 tmp;
	card8 srcIx;

	if ( noteCt <= 1 ) return;

	// Loop through the array elements
	for ( ix = 0; ix < noteCt; ix++ ) {

		// Calc the index to swap with.
		srcIx = ix + (rand() % (noteCt - ix));
		tmp = phrasePt->sta[ix]; phrasePt->sta[ix] = phrasePt->sta[srcIx]; phrasePt->sta[srcIx] = tmp;

		srcIx = ix + (rand() % (noteCt - ix));
		tmp = phrasePt->len[ix]; phrasePt->len[ix] = phrasePt->len[srcIx]; phrasePt->len[srcIx] = tmp;

		srcIx = ix + (rand() % (noteCt - ix));
		tmp = phrasePt->pit[ix]; phrasePt->pit[ix] = phrasePt->pit[srcIx]; phrasePt->pit[srcIx] = tmp;

		srcIx = ix + (rand() % (noteCt - ix));
		tmp = phrasePt->vel[ix]; phrasePt->vel[ix] = phrasePt->vel[srcIx]; phrasePt->vel[srcIx] = tmp;
	}
}


void PhrasePtReversePit( PhraseStruct* phrasePt, card8 noteCt )
{
	card8 ix;
	card8 tmp;
	card8 srcIx;

	if ( noteCt <= 1 ) return;

	// Loop through the array elements
	for ( ix = 0; ix < noteCt; ix++ ) {

		srcIx = (noteCt - 1 - ix);
		if ( srcIx <= ix ) break;

		tmp = phrasePt->pit[ix]; phrasePt->pit[ix] = phrasePt->pit[srcIx]; phrasePt->pit[srcIx] = tmp;
	}
}
