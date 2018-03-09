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


#if PHRASE_EDIT_VIEW_NEMO

		// Show values depending on the OCT_strum_note_idx and OCT_strum_phrase_idx.
		MIR_write_numeric_H( PhraseGetVel(PhraseCurPhraseIx, PhraseCurNoteIx), 0);
		MIR_write_numeric_H( PhraseGetPit(PhraseCurPhraseIx, PhraseCurNoteIx), 1);
		MIR_write_numeric_H( PhraseGetLen(PhraseCurPhraseIx, PhraseCurNoteIx), 2);
		MIR_write_numeric_H( PhraseGetSta(PhraseCurPhraseIx, PhraseCurNoteIx), 3);
		MIR_write_numeric_H( PhraseGetDir(PhraseCurPhraseIx), 5);

#endif


#if PHRASE_EDIT_VIEW_OCTO

		{
			intn ix;
			for ( ix = 0; ix < PHRASE_NOTE_COUNT; ix++ ) {
				switch( (PhraseAttribEnum) PhraseCurAttrib ) {
				case PhraseAttribVel:
					MIR_write_numeric_H( PhraseGetVel(PhraseCurPhraseIx, ix), 8-ix);
					break;
				case PhraseAttribPit:
					MIR_write_base12_H( PhraseGetPit(PhraseCurPhraseIx, ix), 8-ix);
					break;
				case PhraseAttribLen:
					MIR_write_base12_H( PhraseGetLen(PhraseCurPhraseIx, ix), 8-ix);
					break;
				case PhraseAttribSta:
					MIR_write_length_H( PhraseGetSta(PhraseCurPhraseIx, ix), 8-ix);
					break;
				}

				if ( PhraseCurPhraseIx == 0 ) {
					MIR_fill_numeric( 16, 17, 8-ix, MIR_RED );
				}
				else if ( ix < PhraseGetNoteCount(PhraseCurPhraseIx) ) {
					MIR_fill_numeric( 16, 17, 8-ix, MIR_GREEN );
					MIR_fill_numeric( 16, 17, 8-ix, MIR_RED );
				}
			}

			intn type = PhraseGetType(PhraseCurPhraseIx);
			MIR_fill_numeric( 1, type-3+1, 0, MIR_GREEN );
			MIR_fill_numeric( 1, type-3+1, 0, MIR_RED );
		}

#endif

