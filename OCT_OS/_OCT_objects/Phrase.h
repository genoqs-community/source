#ifndef PHRASE_H_
#define PHRASE_H_

#include "../_OCT_global/includes-declarations.h"


// ----------------------------------------------------
// CONSTANTS
// ----------------------------------------------------

typedef enum {
	PhraseTypeStrumFwd  	= 1,
	PhraseTypeStrumRev  	= 2,
	PhraseTypeStrumRndPit  	= 3,
	PhraseTypePhraseFwd 	= 4,
	PhraseTypePhraseRev 	= 5,
	PhraseTypePhraseRndPit 	= 6,	// Shuffle pitch only.
	PhraseTypePhraseRndAll	= 7,	// Shuffle all attribs, wobble.
} PhraseTypeEnum;


typedef enum {
	PhraseAttribVel = 0,
	PhraseAttribPit,
	PhraseAttribLen,
	PhraseAttribSta,
} PhraseAttribEnum;


#define PHRASE_MODE_COMPACT						1
#define PHRASE_MODE_SERIOUS						0

#if PHRASE_MODE_COMPACT
	#define PHRASE_COUNT_USER_RUNTIME			49		// Number of user phrase slots available at runtime.
	#define PHRASE_COUNT_USER_PERSISTENT		256		// Max number of user phrases stored to flash and sysex.
	#define PHRASE_COUNT_PRESET					75		// Number of preset phrases available.
	#define PHRASE_STRUM_PRESET_NUM				49
#endif

#define PHRASE_NOTE_COUNT						8
#define PHRASE_EDIT_VIEW_OCTO					1
#define PHRASE_EDIT_VIEW_NEMO					0
#define PHRASE_MAX_STA							192
#define PHRASE_GRV_FACT_NEUTRAL					7
#define PHRASE_DATA_INIT_VAL					((PhraseTypePhraseFwd << 4) | PHRASE_GRV_FACT_NEUTRAL)

#define PHRASE_PRINT							0	// 1: prints on    0: prints off

// ----------------------------------------------------
// TYPES
// ----------------------------------------------------

typedef struct {
	card8	sta[PHRASE_NOTE_COUNT];
	int8	vel[PHRASE_NOTE_COUNT];
	int8	pit[PHRASE_NOTE_COUNT];
	int8	len[PHRASE_NOTE_COUNT];
	card8	polyType;					// Type in bottom 4 bits (PhraseTypeEnum). Poly in top 4 bits (0 = all notes).
	card8 	filler[3];					// Future expansion. Makes the struct 36 bytes.
} PhraseStruct;



// ----------------------------------------------------
// DATA DECLARATIONS
// ----------------------------------------------------

extern card8 PhraseCurPhraseIx;
extern card8 PhraseCurNoteIx;
extern card8 PhraseCurAttrib;
extern int16 PhraseMultiVal;


// The phrase repository.
extern PhraseStruct PhraseRepos[PHRASE_COUNT_USER_RUNTIME];
extern PhraseStruct PhrasePresets[PHRASE_COUNT_PRESET];


// ----------------------------------------------------
// MACROS
// ----------------------------------------------------

#define PhraseGetVel( phraseIx, noteIx ) 								PhraseRepos[phraseIx].vel[noteIx]
#define PhraseGetPit( phraseIx, noteIx ) 								PhraseRepos[phraseIx].pit[noteIx]
#define PhraseGetLen( phraseIx, noteIx ) 								PhraseRepos[phraseIx].len[noteIx]
#define PhraseGetSta( phraseIx, noteIx ) 								PhraseRepos[phraseIx].sta[noteIx]
#define PhraseGetPoly( phraseIx ) 										((PhraseRepos[phraseIx].polyType >> 4) & 0x0f)
#define PhraseGetType( phraseIx ) 										(PhraseRepos[phraseIx].polyType & 0x0f)

#define PhrasePtGetVel( phrasePt, noteIx ) 								(phrasePt)->vel[noteIx]
#define PhrasePtGetPit( phrasePt, noteIx ) 								(phrasePt)->pit[noteIx]
#define PhrasePtGetLen( phrasePt, noteIx ) 								(phrasePt)->len[noteIx]
#define PhrasePtGetSta( phrasePt, noteIx ) 								(phrasePt)->sta[noteIx]
#define PhrasePtGetPoly( phrasePt ) 									(((phrasePt)->polyType >> 4) & 0x0f)
#define PhrasePtGetType( phrasePt ) 									((phrasePt)->polyType & 0x0f)

#define PhraseSetVel( phraseIx, noteIx, val )							PhraseRepos[phraseIx].vel[noteIx] = (val)
#define PhraseSetPit( phraseIx, noteIx, val )							PhraseRepos[phraseIx].pit[noteIx] = (val)
#define PhraseSetLen( phraseIx, noteIx, val )							PhraseRepos[phraseIx].len[noteIx] = (val)
#define PhraseSetSta( phraseIx, noteIx, val )							PhraseRepos[phraseIx].sta[noteIx] = (val)
#define PhraseSetPoly( phraseIx, val )									PhraseRepos[phraseIx].polyType = (((val) & 0x0f) << 4) | (PhraseGetType(phraseIx))
#define PhraseSetType( phraseIx, val )									PhraseRepos[phraseIx].polyType = ((PhraseGetPoly(phraseIx)) << 4) | ((val) & 0x0f)

#define PhrasePtSetType( phrasePt, val )								(phrasePt)->polyType = ((PhrasePtGetPoly(phrasePt)) << 4) | ((val) & 0x0f)
#define PhrasePtAddPit( phrasePt, noteIx, val )							(phrasePt)->pit[noteIx] += (val)
#define PhrasePtSetSta( phrasePt, noteIx, val )							(phrasePt)->sta[noteIx] = (val)
#define PhrasePtSetPit( phrasePt, noteIx, val )							(phrasePt)->pit[noteIx] = (val)



// ----------------------------------------------------
// FUNCTION PROTOTYPES
// ----------------------------------------------------

extern void 	PhraseInit( void );
extern void 	PhraseReposRestore( void );
extern void 	PhrasePrintRepos( void );
extern void 	PhrasePrint( card8 phraseIx );
extern void		PhrasePtPrint( PhraseStruct* phrasePt );
extern void 	PhrasePrintVec( int8* vecPt, const char8* textSt, booln staBo );
extern void 	PhraseShowPhraseNum( int16 phraseIx );
extern void 	PhraseSetCurrent( card8 phraseIx );
extern void		PhraseCopy( void );
extern void 	PhrasePaste( void );
extern void  	PhraseUndo( void );
extern card8 	PhraseGetNoteCount( card8 phraseIx );
extern card8 	PhrasePtGetNoteCount( PhraseStruct* phrasePt );
extern void 	PhraseClear( card8 phraseIx );
extern void		PhraseRestore( card8 phraseIx );
extern void 	PhraseClearVel( card8 phraseIx );
extern void 	PhraseClearPit( card8 phraseIx );
extern void 	PhraseClearLen( card8 phraseIx );
extern void 	PhraseClearSta( card8 phraseIx );
extern void 	PhraseRandomize( card8 phraseIx );
extern void 	PhraseRandomizeVel( card8 phraseIx );
extern void 	PhraseRandomizePit( card8 phraseIx );
extern void 	PhraseRandomizeLen( card8 phraseIx );
extern void 	PhraseRandomizeSta( card8 phraseIx );
extern void 	PhraseMultiTweakStart( booln editBo );
extern void 	PhraseMultiTweakReset( void );
extern void 	PhraseEditNumber( intn direction );
extern void 	PhraseEditStepNumber( Stepstruct* stepPt, intn direction );
extern void		PhraseEditGlobalStrum( intn direction );
extern void 	PhraseEditStepType( Stepstruct* stepPt, intn direction );
extern void 	PhraseEditStepFactor( Stepstruct* stepPt, intn direction );
extern void 	PhraseEditVel( card8 phraseIx, card8 noteIx, intn direction );
extern void 	PhraseEditPit( card8 phraseIx, card8 noteIx, intn direction );
extern void 	PhraseEditLen( card8 phraseIx, card8 noteIx, intn direction );
extern void 	PhraseEditSta( card8 phraseIx, card8 noteIx, intn direction );
extern void 	PhraseEditType( card8 phraseIx, intn direction );
extern void     PhraseEditPoly( card8 phraseIx, card8 poly );
extern void 	PhraseEditGrv( card8 phraseIx, intn direction );
extern void 	PhrasePtEditGrv( PhraseStruct* phrasePt, intn direction, booln protectBo );
extern void 	PhraseSetStepFactor( Stepstruct* stepPt, intn factor );
extern void 	PhraseSetStepType( Stepstruct* stepPt, PhraseTypeEnum type );
extern void 	PhrasePtApplyGrv( PhraseStruct* phrasePt, intn factor );
//extern booln	PhraseIsStrum( card8 phraseIx );
//extern booln	PhraseIsPhrase( card8 phraseIx );
//extern booln	PhraseIsFwd( card8 phraseIx );
//extern booln	PhraseIsRev( card8 phraseIx );
//extern booln	PhraseIsRndPit( card8 phraseIx );
//extern booln	PhraseIsRndAll( card8 phraseIx );
extern booln 	PhraseIsEmpty( card8 phraseIx );
extern booln	PhrasePtIsStrum( PhraseStruct* phrasePt );
extern booln	PhrasePtIsFwd( PhraseStruct* phrasePt );
extern booln	PhrasePtIsRev( PhraseStruct* phrasePt );
extern booln	PhrasePtIsRndPit( PhraseStruct* phrasePt );
extern booln	PhrasePtIsRndAll( PhraseStruct* phrasePt );
extern void 	PhrasePtShufflePit( PhraseStruct* phrasePt, card8 noteCt );
extern void 	PhrasePtShuffleAll( PhraseStruct* phrasePt, card8 noteCt );
extern void		PhrasePtReversePit( PhraseStruct* phrasePt, card8 noteCt );


#endif /*PHRASE_H_*/
