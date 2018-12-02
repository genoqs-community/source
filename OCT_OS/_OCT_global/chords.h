
#define MAX_CHORD_WORD 13
#define MAX_CHORDS 31
#define MAX_NOTES 8

// Chord associations
const char chords[6][0xC][MAX_CHORDS][MAX_CHORD_WORD] =
{ // Scale - Whole
	{ // Tone - C
		{ // Chords
			"C E Ab",
			"D Gb Bb",
			"E Ab C",
			"F# A# D",
			"G# C E",
			"G# C E",
			"A# D Gb",
			"C E Gb Bb",
			"C E G# Bb",
			"D Gb Ab C",
			"D Gb A# C",
			"E Ab Bb D",
			"E Ab C D",
			"F# A# C E",
			"F# A# D E",
			"G# C D Gb",
			"G# C E Gb",
			"A# D E Ab",
			"C E Gb Bb D",
			"C E G# Bb D",
			"D Gb Ab C E",
			"D Gb A# C E",
			"E Ab Bb D Gb",
			"E Ab C D Gb",
			"F# A# C E G#",
			"F# A# D E G#",
			"G# C D Gb A#",
			"G# C E Gb A#",
			"A# D E Ab C",
			"A# D F# Ab C",
			"C E G# Bb D"
		}
	}
};

unsigned char index_of_key_symbol(char symbol){

	switch (symbol) {
		case 'C': return 0;
		case 'D': return 2;
		case 'E': return 4;
		case 'F': return 5;
		case 'G': return 7;
		case 'A': return 9;
		case 'B': return 11;
	}
	return 0;
}

unsigned char index_of_key_symbol_sharp(char symbol){

	switch (symbol) {
		case 'C': return 1;
		case 'D': return 3;
		case 'F': return 6;
		case 'G': return 8;
		case 'A': return 10;
	}
	return 0;
}

unsigned char index_of_key_symbol_flat(char symbol){

	switch (symbol) {
		case 'D': return 1;
		case 'E': return 3;
		case 'G': return 6;
		case 'A': return 8;
		case 'B': return 10;
	}
	return 0;
}

void translateSymbolsToChord(const char word[], unsigned char pitches[]){
	int i, j=0;

	for (i=0; i < MAX_CHORD_WORD; i++){

		if (word[i] == '\0' || word[i] == ' '){ // delimiters

			if (word[i -1] == '#'){

				pitches[j++] = index_of_key_symbol_sharp(word[i -2]); // sharp
			}
			else if (word[i -1] == 'b'){

				pitches[j++] = index_of_key_symbol_flat(word[i -2]); // flat
			}
			else {
				pitches[j++] = index_of_key_symbol(word[i -1]); // whole
			}
		}
		if (word[i] == '\0'){
			break;
		}
	}
	pitches[j] = 0xFF; // add a terminator
}

//void validatePitches(){
//
//	int i, j;
//	unsigned char pitches[MAX_NOTES];
//
//	for (i=0; i < MAX_CHORDS; i++){
//
//		translateSymbolsToChord(chords[0][0][i], pitches);
//
//		diag_printf("chord:%d", i);
//		for (j=0; j < MAX_NOTES; j++){
//
//			if (pitches[j] == 0xFF){
//				break;
//			}
//			diag_printf(" p:%d", pitches[j]);
//		}
//		diag_printf("\n");
//	}
//}

void playNotesInChord(unsigned char in_channel,
					  unsigned char in_velocity,
					  unsigned char in_pitch,
					  unsigned char scale,
					  unsigned char tone
					  ){

	int i;
	unsigned char pitches[MAX_NOTES];

	translateSymbolsToChord(chords[scale][tone][in_pitch], pitches);

	for (i=0; i < MAX_NOTES; i++){

		if (pitches[i] == 0xFF){
			break;
		}
		MIDI_NOTE_new ( in_channel, pitches[i], in_velocity, 0 );
	}
}
