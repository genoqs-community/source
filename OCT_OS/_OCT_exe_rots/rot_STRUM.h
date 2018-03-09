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





void rot_exe_STRUM( 	Pagestruct* target_page,
						unsigned char rotNdx,
						unsigned char direction ){

	unsigned char row = 0;
	unsigned char col = 0;

	//
	// STRUM level regulation
	//

	switch( rotNdx ){

		// BIG KNOB and GRV encoders.
		case 0:
		case ROT_GRV:
			// Only one step is selected

			// May also come from track mode, and no step is selected there..
			if ( target_page->stepSelection == 1 ){

				// Edit the step's phrase number.
				row = target_page->stepSelectionSingleRow;
				col = target_page->stepSelectionSingleCol;
				PhraseMultiTweakReset();
				PhraseEditStepNumber( target_page->Step[row][col], direction );
			}
			else{
//				PhraseMultiTweakReset();
				PhraseEditNumber( direction );
			}
			break;

#if PHRASE_EDIT_VIEW_NEMO
		case ROT_VEL:
			PhraseEditVel( PhraseCurPhraseIx, PhraseCurNoteIx, direction );
			break;

		case ROT_PIT:
			PhraseEditPit( PhraseCurPhraseIx, PhraseCurNoteIx, direction );
			break;

		case ROT_LEN:
			PhraseEditLen( PhraseCurPhraseIx, PhraseCurNoteIx, direction );
			break;

		case ROT_STA:
			PhraseEditSta( PhraseCurPhraseIx, PhraseCurNoteIx, direction );
			break;

		case ROT_DIR:
			PhraseEditDir( PhraseCurPhraseIx, direction );
			break;

		case ROT_GRV:
			PhraseEditGrv( PhraseCurPhraseIx, direction );
			break;
#endif

#if PHRASE_EDIT_VIEW_OCTO
		case 12: case 13: case 14: case 15: case 16: case 17: case 18: case 19:

			PhraseCurNoteIx = 19-rotNdx;

			switch( (PhraseAttribEnum) PhraseCurAttrib) {
			case PhraseAttribVel:
				PhraseEditVel( PhraseCurPhraseIx, 19-rotNdx, direction );
				break;
			case PhraseAttribPit:
				PhraseEditPit( PhraseCurPhraseIx, 19-rotNdx, direction );
				break;
			case PhraseAttribLen:
				PhraseEditLen( PhraseCurPhraseIx, 19-rotNdx, direction );
				break;
			case PhraseAttribSta:
				PhraseEditSta( PhraseCurPhraseIx, 19-rotNdx, direction );
				break;
			}
			break;

		case 11:
		case ROT_DIR:
			PhraseEditType( PhraseCurPhraseIx, direction );
			break;

		case ROT_POS:
			PhraseEditGrv( PhraseCurPhraseIx, direction );
			break;
#endif
	}
}



