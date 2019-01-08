
#define TONE_BTN_OFFSET 23
#define MAX_CHORD_WORD 14
#define MAX_CHORDS 70
#define MAX_NOTES 8
#define ARP_TRACK 0
#define ARP_BUFFER_TRACK 1
// The Chromatic scale which includes all keys is not included
#define SCALE_COUNT 5

// Chord associations
const char chords[SCALE_COUNT][OCTAVE][MAX_CHORDS][MAX_CHORD_WORD] =
{
	{ // https://www.scales-chords.com/shscalesbytype.php?sname=major%20pentatonic
		{ // C maj
			"C E G", // C
			"A C E", // Am
			"C D G", // Csus2
			"D G A", // Dsus4
			"D E A", // Dsus2
			"G C D", // Gsus4
			"G A D", // Gsus2
			"A D E", // Asus4
			"C E G A", // C6
			"D G A C", // D7sus4
			"D E A C", // D7sus2
			"A C E G", // Am7
			"A D E G", // A7sus4
			"C E G", // C\E
			"C E G", // C\G
			"C E G A D", // C6/9
			"D G A C E", // D9sus4
			"D E A C E", // D9sus2
			"A C E G D", // Am11
			"C E G A", // C6\G
			"C D E G A", // C6/9\E
			"C D E G A", // C6/9\G
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			""
		},
		{ // C#/Db
			"C# F G#", // C#
			"A# Db F", // A#m
			"C# D# G#", // C#sus2
			"D# G# A#", // D#sus4
			"D# F A#", // D#sus2
			"G# C# D#", // G#sus4
			"G# A# D#", // G#sus2
			"A# D# F", // A#sus4
			"C# F G# A#", // C#6
			"D# G# A# Db", // D#7sus4
			"D# F A# Db", // D#7sus2
			"A# Db F Ab", // A#m7
			"A# D# F Ab", // A#7sus4
			"C# F G# A# D#", // C#6/9
			"D# G# A# Db F", // D#9sus4
			"D# F A# Db F", // D#9sus2
			"A# Db F Ab D#", // A#m11
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			""
		},
		{ // D maj
			"D Gb A", // D
			"B D Gb", // Bm
			"D E A", // Dsus2
			"E A B", // Esus4
			"E Gb B", // Esus2
			"A D E", // Asus4
			"A B E", // Asus2
			"B E Gb", // Bsus4
			"D Gb A B", // D6
			"E A B D", // E7sus4
			"E Gb B D", // E7sus2
			"B D Gb A", // Bm7
			"B E Gb A", // B7sus4
			"D Gb A B E", // D6/9
			"E A B D Gb", // E9sus4
			"E Gb B D Gb", // E9sus2
			"B D Gb A E", // Bm11
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			""
		},
		{ // D#/Eb
			"C Eb G", // Cm
			"D# G A#", // D#
			"C F G", // Csus4
			"D# F A#", // D#sus2
			"F Bb C", // Fsus4
			"F G C", // Fsus2
			"A# D# F", // A#sus4
			"A# C F", // A#sus2
			"C Eb G Bb", // Cm7
			"C F G Bb", // C7sus4
			"D# G A# C", // D#6
			"F Bb C Eb", // F7sus4
			"F G C Eb", // F7sus2
			"C F G", // Csus4\F
			"C F G", // Csus4\G
			"C Eb G", // Cm\D#
			"C Eb G", // Cm\G
			"C Eb G", // Cm\Eb
			"C Eb G Bb F", // Cm11
			"D# G A# C F", // D#6/9
			"F Bb C Eb G", // F9sus4
			"F G C Eb G", // F9sus2
			"C Eb G Bb", // Cm7\D#
			"C F G Bb", // C7sus4\G
			"C Eb G Bb", // Cm7\Eb
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			""
		},
		{ // E maj
			"C# E G#", // C#m
			"E Ab B", // E
			"C# F# G#", // C#sus4
			"E Gb B", // Esus2
			"F# B C#", // F#sus4
			"F# G# C#", // F#sus2
			"B E Gb", // Bsus4
			"B Db Gb", // Bsus2
			"C# E G# B", // C#m7
			"C# F# G# B", // C#7sus4
			"E Ab B Db", // E6
			"F# B C# E", // F#7sus4
			"F# G# C# E", // F#7sus2
			"C# E G# B F#", // C#m11
			"E Ab B Db Gb", // E6/9
			"F# B C# E G#", // F#9sus4
			"F# G# C# E G#", // F#9sus2
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			""
		},
		{ // F maj
			"D F A", // Dm
			"F A C", // F
			"C F G", // Csus4
			"C D G", // Csus2
			"D G A", // Dsus4
			"F G C", // Fsus2
			"G C D", // Gsus4
			"G A D", // Gsus2
			"D F A C", // Dm7
			"D G A C", // D7sus4
			"F A C D", // F6
			"G C D F", // G7sus4
			"G A D F", // G7sus2
			"C F G", // Csus4\F
			"C F G", // Csus4\G
			"D F A C G", // Dm11
			"F A C D G", // F6/9
			"G C D F A", // G9sus4
			"G A D F A", // G9sus2
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			""
		},
		{ // F#/Gb
			"D# Gb A#", // D#m
			"F# A# C#", // F#
			"C# F# G#", // C#sus4
			"C# D# G#", // C#sus2
			"D# G# A#", // D#sus4
			"F# G# C#", // F#sus2
			"G# C# D#", // G#sus4
			"G# A# D#", // G#sus2
			"D# Gb A# Db", // D#m7
			"D# G# A# Db", // D#7sus4
			"F# A# C# D#", // F#6
			"G# C# D# Gb", // G#7sus4
			"G# A# D# Gb", // G#7sus2
			"D# Gb A# Db G#", // D#m11
			"F# A# C# D# G#", // F#6/9
			"G# C# D# Gb A#", // G#9sus4
			"G# A# D# Gb A#", // G#9sus2
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			""
		},
		{ // G maj
			"E G B", // Em
			"G B D", // G
			"D G A", // Dsus4
			"D E A", // Dsus2
			"E A B", // Esus4
			"G A D", // Gsus2
			"A D E", // Asus4
			"A B E", // Asus2
			"E G B D", // Em7
			"E A B D", // E7sus4
			"G B D E", // G6
			"A D E G", // A7sus4
			"A B E G", // A7sus2
			"E G B D A", // Em11
			"G B D E A", // G6/9
			"A D E G B", // A9sus4
			"A B E G B", // A9sus2
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			""
		},
		{ // G#/Ab
			"F Ab C", // Fm
			"G# C D#", // G#
			"D# G# A#", // D#sus4
			"D# F A#", // D#sus2
			"F Bb C", // Fsus4
			"G# A# D#", // G#sus2
			"A# D# F", // A#sus4
			"A# C F", // A#sus2
			"F Ab C Eb", // Fm7
			"F Bb C Eb", // F7sus4
			"G# C D# F", // G#6
			"A# D# F Ab", // A#7sus4
			"A# C F Ab", // A#7sus2
			"F Ab C Eb Bb", // Fm11
			"G# C D# F A#", // G#6/9
			"A# D# F Ab C", // A#9sus4
			"A# C F Ab C", // A#9sus2
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			""
		},
		{ // A maj
			"F# A C#", // F#m
			"A Db E", // A
			"E A B", // Esus4
			"E Gb B", // Esus2
			"F# B C#", // F#sus4
			"A B E", // Asus2
			"B E Gb", // Bsus4
			"B Db Gb", // Bsus2
			"F# A C# E", // F#m7
			"F# B C# E", // F#7sus4
			"A Db E Gb", // A6
			"B E Gb A", // B7sus4
			"B Db Gb A", // B7sus2
			"F# A C# E B", // F#m11
			"A Db E Gb B", // A6/9
			"B E Gb A Db", // B9sus4
			"B Db Gb A Db", // B9sus2
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			""
		},
		{ // A#/Bb
			"G Bb D", // Gm
			"A# D F", // A#
			"C F G", // Csus4
			"C D G", // Csus2
			"F Bb C", // Fsus4
			"F G C", // Fsus2
			"G C D", // Gsus4
			"A# C F", // A#sus2
			"C F G Bb", // C7sus4
			"C D G Bb", // C7sus2
			"G Bb D F", // Gm7
			"G C D F", // G7sus4
			"A# D F G", // A#6
			"C F G", // Csus4\F
			"C F G", // Csus4\G
			"C F G Bb D", // C9sus4
			"C D G Bb D", // C9sus2
			"G Bb D F C", // Gm11
			"A# D F G C", // A#6/9
			"C F G Bb", // C7sus4\G
			"C F G Bb D", // C9sus4\G
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			""
		},
		{ // B maj
			"G# B D#", // G#m
			"B Eb Gb", // B
			"C# F# G#", // C#sus4
			"C# D# G#", // C#sus2
			"F# B C#", // F#sus4
			"F# G# C#", // F#sus2
			"G# C# D#", // G#sus4
			"B Db Gb", // Bsus2
			"C# F# G# B", // C#7sus4
			"C# D# G# B", // C#7sus2
			"G# B D# Gb", // G#m7
			"G# C# D# Gb", // G#7sus4
			"B Eb Gb Ab", // B6
			"C# F# G# B D#", // C#9sus4
			"C# D# G# B D#", // C#9sus2
			"G# B D# Gb C#", // G#m11
			"B Eb Gb Ab Db", // B6/9
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			""
		},
	},
	{ // https://www.scales-chords.com/shscalesbytype.php?sname=whole%20tone
		{ // C who
			"C E Ab", // Caug
			"D Gb Bb", // Daug
			"E Ab C", // Eaug
			"F# A# D", // F#aug
			"G# C E", // G#aug
			"A# D Gb", // A#aug
			"C E Gb Bb", // C7b5
			"C E G# Bb", // C7#5
			"D Gb Ab C", // D7b5
			"D Gb A# C", // D7#5
			"E Ab Bb D", // E7b5
			"E Ab C D", // E7#5
			"F# A# C E", // F#7b5
			"F# A# D E", // F#7#5
			"G# C D Gb", // G#7b5
			"G# C E Gb", // G#7#5
			"A# D E Ab", // A#7b5
			"A# D F# Ab", // A#7#5
			"C E Gb Bb D", // C9b5
			"C E G# Bb D", // C9#5
			"D Gb Ab C E", // D9b5
			"D Gb A# C E", // D9#5
			"E Ab Bb D Gb", // E9b5
			"E Ab C D Gb", // E9#5
			"F# A# C E G#", // F#9b5
			"F# A# D E G#", // F#9#5
			"G# C D Gb A#", // G#9b5
			"G# C E Gb A#", // G#9#5
			"A# D E Ab C", // A#9b5
			"A# D F# Ab C", // A#9#5
			"C E G# Bb D", // C9#5\E
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			""
		},
		{ // C#/Db
			"C# F A", // C#aug
			"D# G B", // D#aug
			"F A Db", // Faug
			"G B Eb", // Gaug
			"A Db F", // Aaug
			"B Eb G", // Baug
			"C# F G B", // C#7b5
			"C# F A B", // C#7#5
			"D# G A Db", // D#7b5
			"D# G B Db", // D#7#5
			"F A B Eb", // F7b5
			"F A C# Eb", // F7#5
			"G B Db F", // G7b5
			"G B D# F", // G7#5
			"A Db Eb G", // A7b5
			"A Db F G", // A7#5
			"B Eb F A", // B7b5
			"B Eb G A", // B7#5
			"C# F G B D#", // C#9b5
			"C# F A B D#", // C#9#5
			"D# G A Db F", // D#9b5
			"D# G B Db F", // D#9#5
			"F A B Eb G", // F9b5
			"F A C# Eb G", // F9#5
			"G B Db F A", // G9b5
			"G B D# F A", // G9#5
			"A Db Eb G B", // A9b5
			"A Db F G B", // A9#5
			"B Eb F A Db", // B9b5
			"B Eb G A Db", // B9#5
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			""
		},
		{ // D who
			"C E Ab", // Caug
			"D Gb Bb", // Daug
			"E Ab C", // Eaug
			"F# A# D", // F#aug
			"G# C E", // G#aug
			"A# D Gb", // A#aug
			"C E Gb Bb", // C7b5
			"C E G# Bb", // C7#5
			"D Gb Ab C", // D7b5
			"D Gb A# C", // D7#5
			"E Ab Bb D", // E7b5
			"E Ab C D", // E7#5
			"F# A# C E", // F#7b5
			"F# A# D E", // F#7#5
			"G# C D Gb", // G#7b5
			"G# C E Gb", // G#7#5
			"A# D E Ab", // A#7b5
			"A# D F# Ab", // A#7#5
			"C E Gb Bb D", // C9b5
			"C E G# Bb D", // C9#5
			"D Gb Ab C E", // D9b5
			"D Gb A# C E", // D9#5
			"E Ab Bb D Gb", // E9b5
			"E Ab C D Gb", // E9#5
			"F# A# C E G#", // F#9b5
			"F# A# D E G#", // F#9#5
			"G# C D Gb A#", // G#9b5
			"G# C E Gb A#", // G#9#5
			"A# D E Ab C", // A#9b5
			"A# D F# Ab C", // A#9#5
			"C E G# Bb D", // C9#5\E
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			""
		},
		{ // D#/Eb
			"C# F A", // C#aug
			"D# G B", // D#aug
			"F A Db", // Faug
			"G B Eb", // Gaug
			"A Db F", // Aaug
			"B Eb G", // Baug
			"C# F G B", // C#7b5
			"C# F A B", // C#7#5
			"D# G A Db", // D#7b5
			"D# G B Db", // D#7#5
			"F A B Eb", // F7b5
			"F A C# Eb", // F7#5
			"G B Db F", // G7b5
			"G B D# F", // G7#5
			"A Db Eb G", // A7b5
			"A Db F G", // A7#5
			"B Eb F A", // B7b5
			"B Eb G A", // B7#5
			"C# F G B D#", // C#9b5
			"C# F A B D#", // C#9#5
			"D# G A Db F", // D#9b5
			"D# G B Db F", // D#9#5
			"F A B Eb G", // F9b5
			"F A C# Eb G", // F9#5
			"G B Db F A", // G9b5
			"G B D# F A", // G9#5
			"A Db Eb G B", // A9b5
			"A Db F G B", // A9#5
			"B Eb F A Db", // B9b5
			"B Eb G A Db", // B9#5
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			""
		},
		{ // E who
			"C E Ab", // Caug
			"D Gb Bb", // Daug
			"E Ab C", // Eaug
			"F# A# D", // F#aug
			"G# C E", // G#aug
			"A# D Gb", // A#aug
			"C E Gb Bb", // C7b5
			"C E G# Bb", // C7#5
			"D Gb Ab C", // D7b5
			"D Gb A# C", // D7#5
			"E Ab Bb D", // E7b5
			"E Ab C D", // E7#5
			"F# A# C E", // F#7b5
			"F# A# D E", // F#7#5
			"G# C D Gb", // G#7b5
			"G# C E Gb", // G#7#5
			"A# D E Ab", // A#7b5
			"A# D F# Ab", // A#7#5
			"C E Gb Bb D", // C9b5
			"C E G# Bb D", // C9#5
			"D Gb Ab C E", // D9b5
			"D Gb A# C E", // D9#5
			"E Ab Bb D Gb", // E9b5
			"E Ab C D Gb", // E9#5
			"F# A# C E G#", // F#9b5
			"F# A# D E G#", // F#9#5
			"G# C D Gb A#", // G#9b5
			"G# C E Gb A#", // G#9#5
			"A# D E Ab C", // A#9b5
			"A# D F# Ab C", // A#9#5
			"C E G# Bb D", // C9#5\E
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			""
		},
		{ // F who
			"C# F A", // C#aug
			"D# G B", // D#aug
			"F A Db", // Faug
			"G B Eb", // Gaug
			"A Db F", // Aaug
			"B Eb G", // Baug
			"C# F G B", // C#7b5
			"C# F A B", // C#7#5
			"D# G A Db", // D#7b5
			"D# G B Db", // D#7#5
			"F A B Eb", // F7b5
			"F A C# Eb", // F7#5
			"G B Db F", // G7b5
			"G B D# F", // G7#5
			"A Db Eb G", // A7b5
			"A Db F G", // A7#5
			"B Eb F A", // B7b5
			"B Eb G A", // B7#5
			"C# F G B D#", // C#9b5
			"C# F A B D#", // C#9#5
			"D# G A Db F", // D#9b5
			"D# G B Db F", // D#9#5
			"F A B Eb G", // F9b5
			"F A C# Eb G", // F9#5
			"G B Db F A", // G9b5
			"G B D# F A", // G9#5
			"A Db Eb G B", // A9b5
			"A Db F G B", // A9#5
			"B Eb F A Db", // B9b5
			"B Eb G A Db", // B9#5
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			""
		},
		{ // F#/Gb
			"C E Ab", // Caug
			"D Gb Bb", // Daug
			"E Ab C", // Eaug
			"F# A# D", // F#aug
			"G# C E", // G#aug
			"A# D Gb", // A#aug
			"C E Gb Bb", // C7b5
			"C E G# Bb", // C7#5
			"D Gb Ab C", // D7b5
			"D Gb A# C", // D7#5
			"E Ab Bb D", // E7b5
			"E Ab C D", // E7#5
			"F# A# C E", // F#7b5
			"F# A# D E", // F#7#5
			"G# C D Gb", // G#7b5
			"G# C E Gb", // G#7#5
			"A# D E Ab", // A#7b5
			"A# D F# Ab", // A#7#5
			"C E Gb Bb D", // C9b5
			"C E G# Bb D", // C9#5
			"D Gb Ab C E", // D9b5
			"D Gb A# C E", // D9#5
			"E Ab Bb D Gb", // E9b5
			"E Ab C D Gb", // E9#5
			"F# A# C E G#", // F#9b5
			"F# A# D E G#", // F#9#5
			"G# C D Gb A#", // G#9b5
			"G# C E Gb A#", // G#9#5
			"A# D E Ab C", // A#9b5
			"A# D F# Ab C", // A#9#5
			"C E G# Bb D", // C9#5\E
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			""
		},
		{ // G who
			"C# F A", // C#aug
			"D# G B", // D#aug
			"F A Db", // Faug
			"G B Eb", // Gaug
			"A Db F", // Aaug
			"B Eb G", // Baug
			"C# F G B", // C#7b5
			"C# F A B", // C#7#5
			"D# G A Db", // D#7b5
			"D# G B Db", // D#7#5
			"F A B Eb", // F7b5
			"F A C# Eb", // F7#5
			"G B Db F", // G7b5
			"G B D# F", // G7#5
			"A Db Eb G", // A7b5
			"A Db F G", // A7#5
			"B Eb F A", // B7b5
			"B Eb G A", // B7#5
			"C# F G B D#", // C#9b5
			"C# F A B D#", // C#9#5
			"D# G A Db F", // D#9b5
			"D# G B Db F", // D#9#5
			"F A B Eb G", // F9b5
			"F A C# Eb G", // F9#5
			"G B Db F A", // G9b5
			"G B D# F A", // G9#5
			"A Db Eb G B", // A9b5
			"A Db F G B", // A9#5
			"B Eb F A Db", // B9b5
			"B Eb G A Db", // B9#5
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			""
		},
		{ // G#/Ab
			"C E Ab", // Caug
			"D Gb Bb", // Daug
			"E Ab C", // Eaug
			"F# A# D", // F#aug
			"G# C E", // G#aug
			"A# D Gb", // A#aug
			"C E Gb Bb", // C7b5
			"C E G# Bb", // C7#5
			"D Gb Ab C", // D7b5
			"D Gb A# C", // D7#5
			"E Ab Bb D", // E7b5
			"E Ab C D", // E7#5
			"F# A# C E", // F#7b5
			"F# A# D E", // F#7#5
			"G# C D Gb", // G#7b5
			"G# C E Gb", // G#7#5
			"A# D E Ab", // A#7b5
			"A# D F# Ab", // A#7#5
			"C E Gb Bb D", // C9b5
			"C E G# Bb D", // C9#5
			"D Gb Ab C E", // D9b5
			"D Gb A# C E", // D9#5
			"E Ab Bb D Gb", // E9b5
			"E Ab C D Gb", // E9#5
			"F# A# C E G#", // F#9b5
			"F# A# D E G#", // F#9#5
			"G# C D Gb A#", // G#9b5
			"G# C E Gb A#", // G#9#5
			"A# D E Ab C", // A#9b5
			"A# D F# Ab C", // A#9#5
			"C E G# Bb D", // C9#5\E
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			""
		},
		{ // A who
			"C# F A", // C#aug
			"D# G B", // D#aug
			"F A Db", // Faug
			"G B Eb", // Gaug
			"A Db F", // Aaug
			"B Eb G", // Baug
			"C# F G B", // C#7b5
			"C# F A B", // C#7#5
			"D# G A Db", // D#7b5
			"D# G B Db", // D#7#5
			"F A B Eb", // F7b5
			"F A C# Eb", // F7#5
			"G B Db F", // G7b5
			"G B D# F", // G7#5
			"A Db Eb G", // A7b5
			"A Db F G", // A7#5
			"B Eb F A", // B7b5
			"B Eb G A", // B7#5
			"C# F G B D#", // C#9b5
			"C# F A B D#", // C#9#5
			"D# G A Db F", // D#9b5
			"D# G B Db F", // D#9#5
			"F A B Eb G", // F9b5
			"F A C# Eb G", // F9#5
			"G B Db F A", // G9b5
			"G B D# F A", // G9#5
			"A Db Eb G B", // A9b5
			"A Db F G B", // A9#5
			"B Eb F A Db", // B9b5
			"B Eb G A Db", // B9#5
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			""
		},
		{ // A#/Bb
			"C E Ab", // Caug
			"D Gb Bb", // Daug
			"E Ab C", // Eaug
			"F# A# D", // F#aug
			"G# C E", // G#aug
			"A# D Gb", // A#aug
			"C E Gb Bb", // C7b5
			"C E G# Bb", // C7#5
			"D Gb Ab C", // D7b5
			"D Gb A# C", // D7#5
			"E Ab Bb D", // E7b5
			"E Ab C D", // E7#5
			"F# A# C E", // F#7b5
			"F# A# D E", // F#7#5
			"G# C D Gb", // G#7b5
			"G# C E Gb", // G#7#5
			"A# D E Ab", // A#7b5
			"A# D F# Ab", // A#7#5
			"C E Gb Bb D", // C9b5
			"C E G# Bb D", // C9#5
			"D Gb Ab C E", // D9b5
			"D Gb A# C E", // D9#5
			"E Ab Bb D Gb", // E9b5
			"E Ab C D Gb", // E9#5
			"F# A# C E G#", // F#9b5
			"F# A# D E G#", // F#9#5
			"G# C D Gb A#", // G#9b5
			"G# C E Gb A#", // G#9#5
			"A# D E Ab C", // A#9b5
			"A# D F# Ab C", // A#9#5
			"C E G# Bb D", // C9#5\E
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			""
		},
		{ // B who
			"C# F A", // C#aug
			"D# G B", // D#aug
			"F A Db", // Faug
			"G B Eb", // Gaug
			"A Db F", // Aaug
			"B Eb G", // Baug
			"C# F G B", // C#7b5
			"C# F A B", // C#7#5
			"D# G A Db", // D#7b5
			"D# G B Db", // D#7#5
			"F A B Eb", // F7b5
			"F A C# Eb", // F7#5
			"G B Db F", // G7b5
			"G B D# F", // G7#5
			"A Db Eb G", // A7b5
			"A Db F G", // A7#5
			"B Eb F A", // B7b5
			"B Eb G A", // B7#5
			"C# F G B D#", // C#9b5
			"C# F A B D#", // C#9#5
			"D# G A Db F", // D#9b5
			"D# G B Db F", // D#9#5
			"F A B Eb G", // F9b5
			"F A C# Eb G", // F9#5
			"G B Db F A", // G9b5
			"G B D# F A", // G9#5
			"A Db Eb G B", // A9b5
			"A Db F G B", // A9#5
			"B Eb F A Db", // B9b5
			"B Eb G A Db", // B9#5
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			""
		},
	},
	{ // https://www.scales-chords.com/shscalesbytype.php?sname=major
		{ // C maj
			"C E G", // C
			"D F A", // Dm
			"E G B", // Em
			"F A C", // F
			"G B D", // G
			"A C E", // Am
			"B D F", // Bdim
			"C F G", // Csus4
			"C D G", // Csus2
			"D G A", // Dsus4
			"D E A", // Dsus2
			"E A B", // Esus4
			"F G C", // Fsus2
			"G C D", // Gsus4
			"G A D", // Gsus2
			"A D E", // Asus4
			"A B E", // Asus2
			"C E G A", // C6
			"C E G B", // Cmaj7
			"D F A B", // Dm6
			"D F A C", // Dm7
			"D G A C", // D7sus4
			"D E A C", // D7sus2
			"E G B D", // Em7
			"E A B D", // E7sus4
			"F A C D", // F6
			"F A C E", // Fmaj7
			"G B D E", // G6
			"G B D F", // G7
			"G C D F", // G7sus4
			"G A D F", // G7sus2
			"A C E G", // Am7
			"A D E G", // A7sus4
			"A B E G", // A7sus2
			"B D F A", // Bm7b5
			"C E G", // C\E
			"C E G", // C\G
			"C F G", // Csus4\F
			"C F G", // Csus4\G
			"C E G A D", // C6/9
			"C E G B D", // Cmaj9
			"C E G B A", // Cmaj13
			"D F A C E", // Dm9
			"D F A C G", // Dm11
			"D F A C B", // Dm13
			"D G A C E", // D9sus4
			"D E A C E", // D9sus2
			"E G B D A", // Em11
			"F A C D G", // F6/9
			"F A C E G", // Fmaj9
			"F A C E D", // Fmaj13
			"G B D E A", // G6/9
			"G B D F A", // G9
			"G B D F E", // G13
			"G C D F A", // G9sus4
			"G A D F A", // G9sus2
			"A C E G B", // Am9
			"A C E G D", // Am11
			"A D E G B", // A9sus4
			"A B E G B", // A9sus2
			"C E G A", // C6\G
			"C D E G A", // C6/9\E
			"C D E G A", // C6/9\G
			"C E G B", // Cmaj7\G
			"C E G B D", // Cmaj9\E
			"",
			"",
			"",
			"",
			""
		},
		{ // C#/Db
			"C# F G#", // C#
			"C Eb Gb", // Cdim
			"D# Gb A#", // D#m
			"F Ab C", // Fm
			"F# A# C#", // F#
			"G# C D#", // G#
			"A# Db F", // A#m
			"C# F# G#", // C#sus4
			"C# D# G#", // C#sus2
			"D# G# A#", // D#sus4
			"D# F A#", // D#sus2
			"F Bb C", // Fsus4
			"F# G# C#", // F#sus2
			"G# C# D#", // G#sus4
			"G# A# D#", // G#sus2
			"A# D# F", // A#sus4
			"A# C F", // A#sus2
			"C# F G# A#", // C#6
			"C# F G# C", // C#maj7
			"C Eb Gb Bb", // Cm7b5
			"D# Gb A# C", // D#m6
			"D# Gb A# Db", // D#m7
			"D# G# A# Db", // D#7sus4
			"D# F A# Db", // D#7sus2
			"F Ab C Eb", // Fm7
			"F Bb C Eb", // F7sus4
			"F# A# C# D#", // F#6
			"F# A# C# F", // F#maj7
			"G# C D# F", // G#6
			"G# C D# Gb", // G#7
			"G# C# D# Gb", // G#7sus4
			"G# A# D# Gb", // G#7sus2
			"A# Db F Ab", // A#m7
			"A# D# F Ab", // A#7sus4
			"A# C F Ab", // A#7sus2
			"C# F G# A# D#", // C#6/9
			"C# F G# C D#", // C#maj9
			"C# F G# C A#", // C#maj13
			"D# Gb A# Db F", // D#m9
			"D# Gb A# Db G#", // D#m11
			"D# Gb A# Db C", // D#m13
			"D# G# A# Db F", // D#9sus4
			"D# F A# Db F", // D#9sus2
			"F Ab C Eb Bb", // Fm11
			"F# A# C# D# G#", // F#6/9
			"F# A# C# F G#", // F#maj9
			"F# A# C# F D#", // F#maj13
			"G# C D# F A#", // G#6/9
			"G# C D# Gb A#", // G#9
			"G# C D# Gb F", // G#13
			"G# C# D# Gb A#", // G#9sus4
			"G# A# D# Gb A#", // G#9sus2
			"A# Db F Ab C", // A#m9
			"A# Db F Ab D#", // A#m11
			"A# D# F Ab C", // A#9sus4
			"A# C F Ab C", // A#9sus2
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			""
		},
		{ // D maj
			"C# E G", // C#dim
			"D Gb A", // D
			"E G B", // Em
			"F# A C#", // F#m
			"G B D", // G
			"A Db E", // A
			"B D Gb", // Bm
			"D G A", // Dsus4
			"D E A", // Dsus2
			"E A B", // Esus4
			"E Gb B", // Esus2
			"F# B C#", // F#sus4
			"G A D", // Gsus2
			"A D E", // Asus4
			"A B E", // Asus2
			"B E Gb", // Bsus4
			"B Db Gb", // Bsus2
			"C# E G B", // C#m7b5
			"D Gb A B", // D6
			"D Gb A Db", // Dmaj7
			"E G B Db", // Em6
			"E G B D", // Em7
			"E A B D", // E7sus4
			"E Gb B D", // E7sus2
			"F# A C# E", // F#m7
			"F# B C# E", // F#7sus4
			"G B D E", // G6
			"G B D Gb", // Gmaj7
			"A Db E Gb", // A6
			"A Db E G", // A7
			"A D E G", // A7sus4
			"A B E G", // A7sus2
			"B D Gb A", // Bm7
			"B E Gb A", // B7sus4
			"B Db Gb A", // B7sus2
			"D Gb A B E", // D6/9
			"D Gb A Db E", // Dmaj9
			"D Gb A Db B", // Dmaj13
			"E G B D Gb", // Em9
			"E G B D A", // Em11
			"E G B D Db", // Em13
			"E A B D Gb", // E9sus4
			"E Gb B D Gb", // E9sus2
			"F# A C# E B", // F#m11
			"G B D E A", // G6/9
			"G B D Gb A", // Gmaj9
			"G B D Gb E", // Gmaj13
			"A Db E Gb B", // A6/9
			"A Db E G B", // A9
			"A Db E G Gb", // A13
			"A D E G B", // A9sus4
			"A B E G B", // A9sus2
			"B D Gb A Db", // Bm9
			"B D Gb A E", // Bm11
			"B E Gb A Db", // B9sus4
			"B Db Gb A Db", // B9sus2
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			""
		},
		{ // D#/Eb
			"C Eb G", // Cm
			"D F Ab", // Ddim
			"D# G A#", // D#
			"F Ab C", // Fm
			"G Bb D", // Gm
			"G# C D#", // G#
			"A# D F", // A#
			"C F G", // Csus4
			"C D G", // Csus2
			"D# G# A#", // D#sus4
			"D# F A#", // D#sus2
			"F Bb C", // Fsus4
			"F G C", // Fsus2
			"G C D", // Gsus4
			"G# A# D#", // G#sus2
			"A# D# F", // A#sus4
			"A# C F", // A#sus2
			"C Eb G Bb", // Cm7
			"C F G Bb", // C7sus4
			"C D G Bb", // C7sus2
			"D F Ab C", // Dm7b5
			"D# G A# C", // D#6
			"D# G A# D", // D#maj7
			"F Ab C D", // Fm6
			"F Ab C Eb", // Fm7
			"F Bb C Eb", // F7sus4
			"F G C Eb", // F7sus2
			"G Bb D F", // Gm7
			"G C D F", // G7sus4
			"G# C D# F", // G#6
			"G# C D# G", // G#maj7
			"A# D F G", // A#6
			"A# D F Ab", // A#7
			"A# D# F Ab", // A#7sus4
			"A# C F Ab", // A#7sus2
			"C F G", // Csus4\F
			"C F G", // Csus4\G
			"C Eb G", // Cm\D#
			"C Eb G", // Cm\G
			"C Eb G", // Cm\Eb
			"C Eb G Bb D", // Cm9
			"C Eb G Bb F", // Cm11
			"C F G Bb D", // C9sus4
			"C D G Bb D", // C9sus2
			"D# G A# C F", // D#6/9
			"D# G A# D F", // D#maj9
			"D# G A# D C", // D#maj13
			"F Ab C Eb G", // Fm9
			"F Ab C Eb Bb", // Fm11
			"F Ab C Eb D", // Fm13
			"F Bb C Eb G", // F9sus4
			"F G C Eb G", // F9sus2
			"G Bb D F C", // Gm11
			"G# C D# F A#", // G#6/9
			"G# C D# G A#", // G#maj9
			"G# C D# G F", // G#maj13
			"A# D F G C", // A#6/9
			"A# D F Ab C", // A#9
			"A# D F Ab G", // A#13
			"A# D# F Ab C", // A#9sus4
			"A# C F Ab C", // A#9sus2
			"C Eb G Bb", // Cm7\D#
			"C Eb G Bb D", // Cm9\D#
			"C F G Bb", // C7sus4\G
			"C F G Bb D", // C9sus4\G
			"C Eb G Bb", // Cm7\Eb
			"C Eb G Bb D", // Cm9\Eb
			"",
			"",
			""
		},
		{ // E maj
			"C# E G#", // C#m
			"D# Gb A", // D#dim
			"E Ab B", // E
			"F# A C#", // F#m
			"G# B D#", // G#m
			"A Db E", // A
			"B Eb Gb", // B
			"C# F# G#", // C#sus4
			"C# D# G#", // C#sus2
			"E A B", // Esus4
			"E Gb B", // Esus2
			"F# B C#", // F#sus4
			"F# G# C#", // F#sus2
			"G# C# D#", // G#sus4
			"A B E", // Asus2
			"B E Gb", // Bsus4
			"B Db Gb", // Bsus2
			"C# E G# B", // C#m7
			"C# F# G# B", // C#7sus4
			"C# D# G# B", // C#7sus2
			"D# Gb A Db", // D#m7b5
			"E Ab B Db", // E6
			"E Ab B Eb", // Emaj7
			"F# A C# D#", // F#m6
			"F# A C# E", // F#m7
			"F# B C# E", // F#7sus4
			"F# G# C# E", // F#7sus2
			"G# B D# Gb", // G#m7
			"G# C# D# Gb", // G#7sus4
			"A Db E Gb", // A6
			"A Db E Ab", // Amaj7
			"B Eb Gb Ab", // B6
			"B Eb Gb A", // B7
			"B E Gb A", // B7sus4
			"B Db Gb A", // B7sus2
			"C# E G# B D#", // C#m9
			"C# E G# B F#", // C#m11
			"C# F# G# B D#", // C#9sus4
			"C# D# G# B D#", // C#9sus2
			"E Ab B Db Gb", // E6/9
			"E Ab B Eb Gb", // Emaj9
			"E Ab B Eb Db", // Emaj13
			"F# A C# E G#", // F#m9
			"F# A C# E B", // F#m11
			"F# A C# E D#", // F#m13
			"F# B C# E G#", // F#9sus4
			"F# G# C# E G#", // F#9sus2
			"G# B D# Gb C#", // G#m11
			"A Db E Gb B", // A6/9
			"A Db E Ab B", // Amaj9
			"A Db E Ab Gb", // Amaj13
			"B Eb Gb Ab Db", // B6/9
			"B Eb Gb A Db", // B9
			"B Eb Gb A Ab", // B13
			"B E Gb A Db", // B9sus4
			"B Db Gb A Db", // B9sus2
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			""
		},
		{ // F maj
			"C E G", // C
			"D F A", // Dm
			"E G Bb", // Edim
			"F A C", // F
			"G Bb D", // Gm
			"A C E", // Am
			"A# D F", // A#
			"C F G", // Csus4
			"C D G", // Csus2
			"D G A", // Dsus4
			"D E A", // Dsus2
			"F Bb C", // Fsus4
			"F G C", // Fsus2
			"G C D", // Gsus4
			"G A D", // Gsus2
			"A D E", // Asus4
			"A# C F", // A#sus2
			"C E G A", // C6
			"C E G Bb", // C7
			"C F G Bb", // C7sus4
			"C D G Bb", // C7sus2
			"D F A C", // Dm7
			"D G A C", // D7sus4
			"D E A C", // D7sus2
			"E G Bb D", // Em7b5
			"F A C D", // F6
			"F A C E", // Fmaj7
			"G Bb D E", // Gm6
			"G Bb D F", // Gm7
			"G C D F", // G7sus4
			"G A D F", // G7sus2
			"A C E G", // Am7
			"A D E G", // A7sus4
			"A# D F G", // A#6
			"A# D F A", // A#maj7
			"C E G", // C\E
			"C E G", // C\G
			"C F G", // Csus4\F
			"C F G", // Csus4\G
			"C E G A D", // C6/9
			"C E G Bb D", // C9
			"C E G Bb A", // C13
			"C F G Bb D", // C9sus4
			"C D G Bb D", // C9sus2
			"D F A C E", // Dm9
			"D F A C G", // Dm11
			"D G A C E", // D9sus4
			"D E A C E", // D9sus2
			"F A C D G", // F6/9
			"F A C E G", // Fmaj9
			"F A C E D", // Fmaj13
			"G Bb D F A", // Gm9
			"G Bb D F C", // Gm11
			"G Bb D F E", // Gm13
			"G C D F A", // G9sus4
			"G A D F A", // G9sus2
			"A C E G D", // Am11
			"A# D F G C", // A#6/9
			"A# D F A C", // A#maj9
			"A# D F A G", // A#maj13
			"C E G A", // C6\G
			"C D E G A", // C6/9\E
			"C D E G A", // C6/9\G
			"C E G Bb", // C7\G
			"C E G Bb", // C7\E
			"C F G Bb", // C7sus4\G
			"C E G Bb D", // C9\G
			"C F G Bb D", // C9sus4\G
			"C E G Bb A", // C13\A#
			"C E G Bb A", // C13\Bb
		},
		{ // F#/Gb
			"C# F G#", // C#
			"D# Gb A#", // D#m
			"F Ab B", // Fdim
			"F# A# C#", // F#
			"G# B D#", // G#m
			"A# Db F", // A#m
			"B Eb Gb", // B
			"C# F# G#", // C#sus4
			"C# D# G#", // C#sus2
			"D# G# A#", // D#sus4
			"D# F A#", // D#sus2
			"F# B C#", // F#sus4
			"F# G# C#", // F#sus2
			"G# C# D#", // G#sus4
			"G# A# D#", // G#sus2
			"A# D# F", // A#sus4
			"B Db Gb", // Bsus2
			"C# F G# A#", // C#6
			"C# F G# B", // C#7
			"C# F# G# B", // C#7sus4
			"C# D# G# B", // C#7sus2
			"D# Gb A# Db", // D#m7
			"D# G# A# Db", // D#7sus4
			"D# F A# Db", // D#7sus2
			"F Ab B Eb", // Fm7b5
			"F# A# C# D#", // F#6
			"F# A# C# F", // F#maj7
			"G# B D# F", // G#m6
			"G# B D# Gb", // G#m7
			"G# C# D# Gb", // G#7sus4
			"G# A# D# Gb", // G#7sus2
			"A# Db F Ab", // A#m7
			"A# D# F Ab", // A#7sus4
			"B Eb Gb Ab", // B6
			"B Eb Gb Bb", // Bmaj7
			"C# F G# A# D#", // C#6/9
			"C# F G# B D#", // C#9
			"C# F G# B A#", // C#13
			"C# F# G# B D#", // C#9sus4
			"C# D# G# B D#", // C#9sus2
			"D# Gb A# Db F", // D#m9
			"D# Gb A# Db G#", // D#m11
			"D# G# A# Db F", // D#9sus4
			"D# F A# Db F", // D#9sus2
			"F# A# C# D# G#", // F#6/9
			"F# A# C# F G#", // F#maj9
			"F# A# C# F D#", // F#maj13
			"G# B D# Gb A#", // G#m9
			"G# B D# Gb C#", // G#m11
			"G# B D# Gb F", // G#m13
			"G# C# D# Gb A#", // G#9sus4
			"G# A# D# Gb A#", // G#9sus2
			"A# Db F Ab D#", // A#m11
			"B Eb Gb Ab Db", // B6/9
			"B Eb Gb Bb Db", // Bmaj9
			"B Eb Gb Bb Ab", // Bmaj13
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			""
		},
		{ // G maj
			"C E G", // C
			"D Gb A", // D
			"E G B", // Em
			"F# A C", // F#dim
			"G B D", // G
			"A C E", // Am
			"B D Gb", // Bm
			"C D G", // Csus2
			"D G A", // Dsus4
			"D E A", // Dsus2
			"E A B", // Esus4
			"E Gb B", // Esus2
			"G C D", // Gsus4
			"G A D", // Gsus2
			"A D E", // Asus4
			"A B E", // Asus2
			"B E Gb", // Bsus4
			"C E G A", // C6
			"C E G B", // Cmaj7
			"D Gb A B", // D6
			"D Gb A C", // D7
			"D G A C", // D7sus4
			"D E A C", // D7sus2
			"E G B D", // Em7
			"E A B D", // E7sus4
			"E Gb B D", // E7sus2
			"F# A C E", // F#m7b5
			"G B D E", // G6
			"G B D Gb", // Gmaj7
			"A C E Gb", // Am6
			"A C E G", // Am7
			"A D E G", // A7sus4
			"A B E G", // A7sus2
			"B D Gb A", // Bm7
			"B E Gb A", // B7sus4
			"C E G", // C\E
			"C E G", // C\G
			"C E G A D", // C6/9
			"C E G B D", // Cmaj9
			"C E G B A", // Cmaj13
			"D Gb A B E", // D6/9
			"D Gb A C E", // D9
			"D Gb A C B", // D13
			"D G A C E", // D9sus4
			"D E A C E", // D9sus2
			"E G B D Gb", // Em9
			"E G B D A", // Em11
			"E A B D Gb", // E9sus4
			"E Gb B D Gb", // E9sus2
			"G B D E A", // G6/9
			"G B D Gb A", // Gmaj9
			"G B D Gb E", // Gmaj13
			"A C E G B", // Am9
			"A C E G D", // Am11
			"A C E G Gb", // Am13
			"A D E G B", // A9sus4
			"A B E G B", // A9sus2
			"B D Gb A E", // Bm11
			"C E G A", // C6\G
			"C D E G A", // C6/9\E
			"C D E G A", // C6/9\G
			"C E G B", // Cmaj7\G
			"C E G B D", // Cmaj9\E
			"",
			"",
			"",
			"",
			"",
			"",
			""
		},
		{ // G#/Ab
			"C Eb G", // Cm
			"C# F G#", // C#
			"D# G A#", // D#
			"F Ab C", // Fm
			"G Bb Db", // Gdim
			"G# C D#", // G#
			"A# Db F", // A#m
			"C F G", // Csus4
			"C# D# G#", // C#sus2
			"D# G# A#", // D#sus4
			"D# F A#", // D#sus2
			"F Bb C", // Fsus4
			"F G C", // Fsus2
			"G# C# D#", // G#sus4
			"G# A# D#", // G#sus2
			"A# D# F", // A#sus4
			"A# C F", // A#sus2
			"C# F G# A#", // C#6
			"C# F G# C", // C#maj7
			"C Eb G Bb", // Cm7
			"C F G Bb", // C7sus4
			"D# G A# C", // D#6
			"D# G A# Db", // D#7
			"D# G# A# Db", // D#7sus4
			"D# F A# Db", // D#7sus2
			"F Ab C Eb", // Fm7
			"F Bb C Eb", // F7sus4
			"F G C Eb", // F7sus2
			"G Bb Db F", // Gm7b5
			"G# C D# F", // G#6
			"G# C D# G", // G#maj7
			"A# Db F G", // A#m6
			"A# Db F Ab", // A#m7
			"A# D# F Ab", // A#7sus4
			"A# C F Ab", // A#7sus2
			"C F G", // Csus4\F
			"C F G", // Csus4\G
			"C Eb G", // Cm\D#
			"C Eb G", // Cm\G
			"C Eb G", // Cm\Eb
			"C# F G# A# D#", // C#6/9
			"C Eb G Bb F", // Cm11
			"C# F G# C D#", // C#maj9
			"C# F G# C A#", // C#maj13
			"D# G A# C F", // D#6/9
			"D# G A# Db F", // D#9
			"D# G A# Db C", // D#13
			"D# G# A# Db F", // D#9sus4
			"D# F A# Db F", // D#9sus2
			"F Ab C Eb G", // Fm9
			"F Ab C Eb Bb", // Fm11
			"F Bb C Eb G", // F9sus4
			"F G C Eb G", // F9sus2
			"G# C D# F A#", // G#6/9
			"G# C D# G A#", // G#maj9
			"G# C D# G F", // G#maj13
			"A# Db F Ab C", // A#m9
			"A# Db F Ab D#", // A#m11
			"A# Db F Ab G", // A#m13
			"A# D# F Ab C", // A#9sus4
			"A# C F Ab C", // A#9sus2
			"C Eb G Bb", // Cm7\D#
			"C F G Bb", // C7sus4\G
			"C Eb G Bb", // Cm7\Eb
			"",
			"",
			"",
			"",
			"",
			""
		},
		{ // A maj
			"C# E G#", // C#m
			"D Gb A", // D
			"E Ab B", // E
			"F# A C#", // F#m
			"G# B D", // G#dim
			"A Db E", // A
			"B D Gb", // Bm
			"C# F# G#", // C#sus4
			"D E A", // Dsus2
			"E A B", // Esus4
			"E Gb B", // Esus2
			"F# B C#", // F#sus4
			"F# G# C#", // F#sus2
			"A D E", // Asus4
			"A B E", // Asus2
			"B E Gb", // Bsus4
			"B Db Gb", // Bsus2
			"C# E G# B", // C#m7
			"C# F# G# B", // C#7sus4
			"D Gb A B", // D6
			"D Gb A Db", // Dmaj7
			"E Ab B Db", // E6
			"E Ab B D", // E7
			"E A B D", // E7sus4
			"E Gb B D", // E7sus2
			"F# A C# E", // F#m7
			"F# B C# E", // F#7sus4
			"F# G# C# E", // F#7sus2
			"G# B D Gb", // G#m7b5
			"A Db E Gb", // A6
			"A Db E Ab", // Amaj7
			"B D Gb Ab", // Bm6
			"B D Gb A", // Bm7
			"B E Gb A", // B7sus4
			"B Db Gb A", // B7sus2
			"C# E G# B F#", // C#m11
			"D Gb A B E", // D6/9
			"D Gb A Db E", // Dmaj9
			"D Gb A Db B", // Dmaj13
			"E Ab B Db Gb", // E6/9
			"E Ab B D Gb", // E9
			"E Ab B D Db", // E13
			"E A B D Gb", // E9sus4
			"E Gb B D Gb", // E9sus2
			"F# A C# E G#", // F#m9
			"F# A C# E B", // F#m11
			"F# B C# E G#", // F#9sus4
			"F# G# C# E G#", // F#9sus2
			"A Db E Gb B", // A6/9
			"A Db E Ab B", // Amaj9
			"A Db E Ab Gb", // Amaj13
			"B D Gb A Db", // Bm9
			"B D Gb A E", // Bm11
			"B D Gb A Ab", // Bm13
			"B E Gb A Db", // B9sus4
			"B Db Gb A Db", // B9sus2
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			""
		},
		{ // A#/Bb
			"C Eb G", // Cm
			"D F A", // Dm
			"D# G A#", // D#
			"F A C", // F
			"G Bb D", // Gm
			"A C Eb", // Adim
			"A# D F", // A#
			"C F G", // Csus4
			"C D G", // Csus2
			"D G A", // Dsus4
			"D# F A#", // D#sus2
			"F Bb C", // Fsus4
			"F G C", // Fsus2
			"G C D", // Gsus4
			"G A D", // Gsus2
			"A# D# F", // A#sus4
			"A# C F", // A#sus2
			"C Eb G A", // Cm6
			"C Eb G Bb", // Cm7
			"C F G Bb", // C7sus4
			"C D G Bb", // C7sus2
			"D F A C", // Dm7
			"D G A C", // D7sus4
			"D# G A# C", // D#6
			"D# G A# D", // D#maj7
			"F A C D", // F6
			"F A C Eb", // F7
			"F Bb C Eb", // F7sus4
			"F G C Eb", // F7sus2
			"G Bb D F", // Gm7
			"G C D F", // G7sus4
			"G A D F", // G7sus2
			"A C Eb G", // Am7b5
			"A# D F G", // A#6
			"A# D F A", // A#maj7
			"C F G", // Csus4\F
			"C F G", // Csus4\G
			"C Eb G", // Cm\D#
			"C Eb G", // Cm\G
			"C Eb G", // Cm\Eb
			"C Eb G Bb D", // Cm9
			"C Eb G Bb F", // Cm11
			"C Eb G Bb A", // Cm13
			"C F G Bb D", // C9sus4
			"C D G Bb D", // C9sus2
			"D F A C G", // Dm11
			"D# G A# C F", // D#6/9
			"D# G A# D F", // D#maj9
			"D# G A# D C", // D#maj13
			"F A C D G", // F6/9
			"F A C Eb G", // F9
			"F A C Eb D", // F13
			"F Bb C Eb G", // F9sus4
			"F G C Eb G", // F9sus2
			"G Bb D F A", // Gm9
			"G Bb D F C", // Gm11
			"G C D F A", // G9sus4
			"G A D F A", // G9sus2
			"A# D F G C", // A#6/9
			"A# D F A C", // A#maj9
			"A# D F A G", // A#maj13
			"C Eb G A", // Cm6\A
			"C Eb G Bb", // Cm7\D#
			"C Eb G Bb D", // Cm9\D#
			"C F G Bb", // C7sus4\G
			"C F G Bb D", // C9sus4\G
			"C Eb G Bb", // Cm7\Eb
			"C Eb G Bb D", // Cm9\Eb
			"",
			""
		},
		{ // B maj
			"C# E G#", // C#m
			"D# Gb A#", // D#m
			"E Ab B", // E
			"F# A# C#", // F#
			"G# B D#", // G#m
			"A# Db E", // A#dim
			"B Eb Gb", // B
			"C# F# G#", // C#sus4
			"C# D# G#", // C#sus2
			"D# G# A#", // D#sus4
			"E Gb B", // Esus2
			"F# B C#", // F#sus4
			"F# G# C#", // F#sus2
			"G# C# D#", // G#sus4
			"G# A# D#", // G#sus2
			"B E Gb", // Bsus4
			"B Db Gb", // Bsus2
			"C# E G# A#", // C#m6
			"C# E G# B", // C#m7
			"C# F# G# B", // C#7sus4
			"C# D# G# B", // C#7sus2
			"D# Gb A# Db", // D#m7
			"D# G# A# Db", // D#7sus4
			"E Ab B Db", // E6
			"E Ab B Eb", // Emaj7
			"F# A# C# D#", // F#6
			"F# A# C# E", // F#7
			"F# B C# E", // F#7sus4
			"F# G# C# E", // F#7sus2
			"G# B D# Gb", // G#m7
			"G# C# D# Gb", // G#7sus4
			"G# A# D# Gb", // G#7sus2
			"A# Db E Ab", // A#m7b5
			"B Eb Gb Ab", // B6
			"B Eb Gb Bb", // Bmaj7
			"C# E G# B D#", // C#m9
			"C# E G# B F#", // C#m11
			"C# E G# B A#", // C#m13
			"C# F# G# B D#", // C#9sus4
			"C# D# G# B D#", // C#9sus2
			"D# Gb A# Db G#", // D#m11
			"E Ab B Db Gb", // E6/9
			"E Ab B Eb Gb", // Emaj9
			"E Ab B Eb Db", // Emaj13
			"F# A# C# D# G#", // F#6/9
			"F# A# C# E G#", // F#9
			"F# A# C# E D#", // F#13
			"F# B C# E G#", // F#9sus4
			"F# G# C# E G#", // F#9sus2
			"G# B D# Gb A#", // G#m9
			"G# B D# Gb C#", // G#m11
			"G# C# D# Gb A#", // G#9sus4
			"G# A# D# Gb A#", // G#9sus2
			"B Eb Gb Ab Db", // B6/9
			"B Eb Gb Bb Db", // Bmaj9
			"B Eb Gb Bb Ab", // Bmaj13
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			""
		},
	},
	{ // https://www.scales-chords.com/shscalesbytype.php?sname=natural%20minor
		{ // C nat
			"C Eb G", // Cm
			"D F Ab", // Ddim
			"D# G A#", // D#
			"F Ab C", // Fm
			"G Bb D", // Gm
			"G# C D#", // G#
			"A# D F", // A#
			"C F G", // Csus4
			"C D G", // Csus2
			"D# G# A#", // D#sus4
			"D# F A#", // D#sus2
			"F Bb C", // Fsus4
			"F G C", // Fsus2
			"G C D", // Gsus4
			"G# A# D#", // G#sus2
			"A# D# F", // A#sus4
			"A# C F", // A#sus2
			"C Eb G Bb", // Cm7
			"C F G Bb", // C7sus4
			"C D G Bb", // C7sus2
			"D F Ab C", // Dm7b5
			"D# G A# C", // D#6
			"D# G A# D", // D#maj7
			"F Ab C D", // Fm6
			"F Ab C Eb", // Fm7
			"F Bb C Eb", // F7sus4
			"F G C Eb", // F7sus2
			"G Bb D F", // Gm7
			"G C D F", // G7sus4
			"G# C D# F", // G#6
			"G# C D# G", // G#maj7
			"A# D F G", // A#6
			"A# D F Ab", // A#7
			"A# D# F Ab", // A#7sus4
			"A# C F Ab", // A#7sus2
			"C F G", // Csus4\F
			"C F G", // Csus4\G
			"C Eb G", // Cm\D#
			"C Eb G", // Cm\G
			"C Eb G", // Cm\Eb
			"C Eb G Bb D", // Cm9
			"C Eb G Bb F", // Cm11
			"C F G Bb D", // C9sus4
			"C D G Bb D", // C9sus2
			"D# G A# C F", // D#6/9
			"D# G A# D F", // D#maj9
			"D# G A# D C", // D#maj13
			"F Ab C Eb G", // Fm9
			"F Ab C Eb Bb", // Fm11
			"F Ab C Eb D", // Fm13
			"F Bb C Eb G", // F9sus4
			"F G C Eb G", // F9sus2
			"G Bb D F C", // Gm11
			"G# C D# F A#", // G#6/9
			"G# C D# G A#", // G#maj9
			"G# C D# G F", // G#maj13
			"A# D F G C", // A#6/9
			"A# D F Ab C", // A#9
			"A# D F Ab G", // A#13
			"A# D# F Ab C", // A#9sus4
			"A# C F Ab C", // A#9sus2
			"C Eb G Bb", // Cm7\D#
			"C Eb G Bb D", // Cm9\D#
			"C F G Bb", // C7sus4\G
			"C F G Bb D", // C9sus4\G
			"C Eb G Bb", // Cm7\Eb
			"C Eb G Bb D", // Cm9\Eb
			"",
			"",
			""
		},
		{ // C#/Db
			"C# E G#", // C#m
			"D# Gb A", // D#dim
			"E Ab B", // E
			"F# A C#", // F#m
			"G# B D#", // G#m
			"A Db E", // A
			"B Eb Gb", // B
			"C# F# G#", // C#sus4
			"C# D# G#", // C#sus2
			"E A B", // Esus4
			"E Gb B", // Esus2
			"F# B C#", // F#sus4
			"F# G# C#", // F#sus2
			"G# C# D#", // G#sus4
			"A B E", // Asus2
			"B E Gb", // Bsus4
			"B Db Gb", // Bsus2
			"C# E G# B", // C#m7
			"C# F# G# B", // C#7sus4
			"C# D# G# B", // C#7sus2
			"D# Gb A Db", // D#m7b5
			"E Ab B Db", // E6
			"E Ab B Eb", // Emaj7
			"F# A C# D#", // F#m6
			"F# A C# E", // F#m7
			"F# B C# E", // F#7sus4
			"F# G# C# E", // F#7sus2
			"G# B D# Gb", // G#m7
			"G# C# D# Gb", // G#7sus4
			"A Db E Gb", // A6
			"A Db E Ab", // Amaj7
			"B Eb Gb Ab", // B6
			"B Eb Gb A", // B7
			"B E Gb A", // B7sus4
			"B Db Gb A", // B7sus2
			"C# E G# B D#", // C#m9
			"C# E G# B F#", // C#m11
			"C# F# G# B D#", // C#9sus4
			"C# D# G# B D#", // C#9sus2
			"E Ab B Db Gb", // E6/9
			"E Ab B Eb Gb", // Emaj9
			"E Ab B Eb Db", // Emaj13
			"F# A C# E G#", // F#m9
			"F# A C# E B", // F#m11
			"F# A C# E D#", // F#m13
			"F# B C# E G#", // F#9sus4
			"F# G# C# E G#", // F#9sus2
			"G# B D# Gb C#", // G#m11
			"A Db E Gb B", // A6/9
			"A Db E Ab B", // Amaj9
			"A Db E Ab Gb", // Amaj13
			"B Eb Gb Ab Db", // B6/9
			"B Eb Gb A Db", // B9
			"B Eb Gb A Ab", // B13
			"B E Gb A Db", // B9sus4
			"B Db Gb A Db", // B9sus2
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			""
		},
		{ // D nat
			"C E G", // C
			"D F A", // Dm
			"E G Bb", // Edim
			"F A C", // F
			"G Bb D", // Gm
			"A C E", // Am
			"A# D F", // A#
			"C F G", // Csus4
			"C D G", // Csus2
			"D G A", // Dsus4
			"D E A", // Dsus2
			"F Bb C", // Fsus4
			"F G C", // Fsus2
			"G C D", // Gsus4
			"G A D", // Gsus2
			"A D E", // Asus4
			"A# C F", // A#sus2
			"C E G A", // C6
			"C E G Bb", // C7
			"C F G Bb", // C7sus4
			"C D G Bb", // C7sus2
			"D F A C", // Dm7
			"D G A C", // D7sus4
			"D E A C", // D7sus2
			"E G Bb D", // Em7b5
			"F A C D", // F6
			"F A C E", // Fmaj7
			"G Bb D E", // Gm6
			"G Bb D F", // Gm7
			"G C D F", // G7sus4
			"G A D F", // G7sus2
			"A C E G", // Am7
			"A D E G", // A7sus4
			"A# D F G", // A#6
			"A# D F A", // A#maj7
			"C E G", // C\E
			"C E G", // C\G
			"C F G", // Csus4\F
			"C F G", // Csus4\G
			"C E G A D", // C6/9
			"C E G Bb D", // C9
			"C E G Bb A", // C13
			"C F G Bb D", // C9sus4
			"C D G Bb D", // C9sus2
			"D F A C E", // Dm9
			"D F A C G", // Dm11
			"D G A C E", // D9sus4
			"D E A C E", // D9sus2
			"F A C D G", // F6/9
			"F A C E G", // Fmaj9
			"F A C E D", // Fmaj13
			"G Bb D F A", // Gm9
			"G Bb D F C", // Gm11
			"G Bb D F E", // Gm13
			"G C D F A", // G9sus4
			"G A D F A", // G9sus2
			"A C E G D", // Am11
			"A# D F G C", // A#6/9
			"A# D F A C", // A#maj9
			"A# D F A G", // A#maj13
			"C E G A", // C6\G
			"C D E G A", // C6/9\E
			"C D E G A", // C6/9\G
			"C E G Bb", // C7\G
			"C E G Bb", // C7\E
			"C F G Bb", // C7sus4\G
			"C E G Bb D", // C9\G
			"C F G Bb D", // C9sus4\G
			"C E G Bb A", // C13\A#
			"C E G Bb A", // C13\Bb
		},
		{ // D#/Eb
			"C# F G#", // C#
			"D# Gb A#", // D#m
			"F Ab B", // Fdim
			"F# A# C#", // F#
			"G# B D#", // G#m
			"A# Db F", // A#m
			"B Eb Gb", // B
			"C# F# G#", // C#sus4
			"C# D# G#", // C#sus2
			"D# G# A#", // D#sus4
			"D# F A#", // D#sus2
			"F# B C#", // F#sus4
			"F# G# C#", // F#sus2
			"G# C# D#", // G#sus4
			"G# A# D#", // G#sus2
			"A# D# F", // A#sus4
			"B Db Gb", // Bsus2
			"C# F G# A#", // C#6
			"C# F G# B", // C#7
			"C# F# G# B", // C#7sus4
			"C# D# G# B", // C#7sus2
			"D# Gb A# Db", // D#m7
			"D# G# A# Db", // D#7sus4
			"D# F A# Db", // D#7sus2
			"F Ab B Eb", // Fm7b5
			"F# A# C# D#", // F#6
			"F# A# C# F", // F#maj7
			"G# B D# F", // G#m6
			"G# B D# Gb", // G#m7
			"G# C# D# Gb", // G#7sus4
			"G# A# D# Gb", // G#7sus2
			"A# Db F Ab", // A#m7
			"A# D# F Ab", // A#7sus4
			"B Eb Gb Ab", // B6
			"B Eb Gb Bb", // Bmaj7
			"C# F G# A# D#", // C#6/9
			"C# F G# B D#", // C#9
			"C# F G# B A#", // C#13
			"C# F# G# B D#", // C#9sus4
			"C# D# G# B D#", // C#9sus2
			"D# Gb A# Db F", // D#m9
			"D# Gb A# Db G#", // D#m11
			"D# G# A# Db F", // D#9sus4
			"D# F A# Db F", // D#9sus2
			"F# A# C# D# G#", // F#6/9
			"F# A# C# F G#", // F#maj9
			"F# A# C# F D#", // F#maj13
			"G# B D# Gb A#", // G#m9
			"G# B D# Gb C#", // G#m11
			"G# B D# Gb F", // G#m13
			"G# C# D# Gb A#", // G#9sus4
			"G# A# D# Gb A#", // G#9sus2
			"A# Db F Ab D#", // A#m11
			"B Eb Gb Ab Db", // B6/9
			"B Eb Gb Bb Db", // Bmaj9
			"B Eb Gb Bb Ab", // Bmaj13
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			""
		},
		{ // E nat
			"C E G", // C
			"D Gb A", // D
			"E G B", // Em
			"F# A C", // F#dim
			"G B D", // G
			"A C E", // Am
			"B D Gb", // Bm
			"C D G", // Csus2
			"D G A", // Dsus4
			"D E A", // Dsus2
			"E A B", // Esus4
			"E Gb B", // Esus2
			"G C D", // Gsus4
			"G A D", // Gsus2
			"A D E", // Asus4
			"A B E", // Asus2
			"B E Gb", // Bsus4
			"C E G A", // C6
			"C E G B", // Cmaj7
			"D Gb A B", // D6
			"D Gb A C", // D7
			"D G A C", // D7sus4
			"D E A C", // D7sus2
			"E G B D", // Em7
			"E A B D", // E7sus4
			"E Gb B D", // E7sus2
			"F# A C E", // F#m7b5
			"G B D E", // G6
			"G B D Gb", // Gmaj7
			"A C E Gb", // Am6
			"A C E G", // Am7
			"A D E G", // A7sus4
			"A B E G", // A7sus2
			"B D Gb A", // Bm7
			"B E Gb A", // B7sus4
			"C E G", // C\E
			"C E G", // C\G
			"C E G A D", // C6/9
			"C E G B D", // Cmaj9
			"C E G B A", // Cmaj13
			"D Gb A B E", // D6/9
			"D Gb A C E", // D9
			"D Gb A C B", // D13
			"D G A C E", // D9sus4
			"D E A C E", // D9sus2
			"E G B D Gb", // Em9
			"E G B D A", // Em11
			"E A B D Gb", // E9sus4
			"E Gb B D Gb", // E9sus2
			"G B D E A", // G6/9
			"G B D Gb A", // Gmaj9
			"G B D Gb E", // Gmaj13
			"A C E G B", // Am9
			"A C E G D", // Am11
			"A C E G Gb", // Am13
			"A D E G B", // A9sus4
			"A B E G B", // A9sus2
			"B D Gb A E", // Bm11
			"C E G A", // C6\G
			"C D E G A", // C6/9\E
			"C D E G A", // C6/9\G
			"C E G B", // Cmaj7\G
			"C E G B D", // Cmaj9\E
			"",
			"",
			"",
			"",
			"",
			"",
			""
		},
		{ // F nat
			"C Eb G", // Cm
			"C# F G#", // C#
			"D# G A#", // D#
			"F Ab C", // Fm
			"G Bb Db", // Gdim
			"G# C D#", // G#
			"A# Db F", // A#m
			"C F G", // Csus4
			"C# D# G#", // C#sus2
			"D# G# A#", // D#sus4
			"D# F A#", // D#sus2
			"F Bb C", // Fsus4
			"F G C", // Fsus2
			"G# C# D#", // G#sus4
			"G# A# D#", // G#sus2
			"A# D# F", // A#sus4
			"A# C F", // A#sus2
			"C# F G# A#", // C#6
			"C# F G# C", // C#maj7
			"C Eb G Bb", // Cm7
			"C F G Bb", // C7sus4
			"D# G A# C", // D#6
			"D# G A# Db", // D#7
			"D# G# A# Db", // D#7sus4
			"D# F A# Db", // D#7sus2
			"F Ab C Eb", // Fm7
			"F Bb C Eb", // F7sus4
			"F G C Eb", // F7sus2
			"G Bb Db F", // Gm7b5
			"G# C D# F", // G#6
			"G# C D# G", // G#maj7
			"A# Db F G", // A#m6
			"A# Db F Ab", // A#m7
			"A# D# F Ab", // A#7sus4
			"A# C F Ab", // A#7sus2
			"C F G", // Csus4\F
			"C F G", // Csus4\G
			"C Eb G", // Cm\D#
			"C Eb G", // Cm\G
			"C Eb G", // Cm\Eb
			"C# F G# A# D#", // C#6/9
			"C Eb G Bb F", // Cm11
			"C# F G# C D#", // C#maj9
			"C# F G# C A#", // C#maj13
			"D# G A# C F", // D#6/9
			"D# G A# Db F", // D#9
			"D# G A# Db C", // D#13
			"D# G# A# Db F", // D#9sus4
			"D# F A# Db F", // D#9sus2
			"F Ab C Eb G", // Fm9
			"F Ab C Eb Bb", // Fm11
			"F Bb C Eb G", // F9sus4
			"F G C Eb G", // F9sus2
			"G# C D# F A#", // G#6/9
			"G# C D# G A#", // G#maj9
			"G# C D# G F", // G#maj13
			"A# Db F Ab C", // A#m9
			"A# Db F Ab D#", // A#m11
			"A# Db F Ab G", // A#m13
			"A# D# F Ab C", // A#9sus4
			"A# C F Ab C", // A#9sus2
			"C Eb G Bb", // Cm7\D#
			"C F G Bb", // C7sus4\G
			"C Eb G Bb", // Cm7\Eb
			"",
			"",
			"",
			"",
			"",
			""
		},
		{ // F#/Gb
			"C# E G#", // C#m
			"D Gb A", // D
			"E Ab B", // E
			"F# A C#", // F#m
			"G# B D", // G#dim
			"A Db E", // A
			"B D Gb", // Bm
			"C# F# G#", // C#sus4
			"D E A", // Dsus2
			"E A B", // Esus4
			"E Gb B", // Esus2
			"F# B C#", // F#sus4
			"F# G# C#", // F#sus2
			"A D E", // Asus4
			"A B E", // Asus2
			"B E Gb", // Bsus4
			"B Db Gb", // Bsus2
			"C# E G# B", // C#m7
			"C# F# G# B", // C#7sus4
			"D Gb A B", // D6
			"D Gb A Db", // Dmaj7
			"E Ab B Db", // E6
			"E Ab B D", // E7
			"E A B D", // E7sus4
			"E Gb B D", // E7sus2
			"F# A C# E", // F#m7
			"F# B C# E", // F#7sus4
			"F# G# C# E", // F#7sus2
			"G# B D Gb", // G#m7b5
			"A Db E Gb", // A6
			"A Db E Ab", // Amaj7
			"B D Gb Ab", // Bm6
			"B D Gb A", // Bm7
			"B E Gb A", // B7sus4
			"B Db Gb A", // B7sus2
			"C# E G# B F#", // C#m11
			"D Gb A B E", // D6/9
			"D Gb A Db E", // Dmaj9
			"D Gb A Db B", // Dmaj13
			"E Ab B Db Gb", // E6/9
			"E Ab B D Gb", // E9
			"E Ab B D Db", // E13
			"E A B D Gb", // E9sus4
			"E Gb B D Gb", // E9sus2
			"F# A C# E G#", // F#m9
			"F# A C# E B", // F#m11
			"F# B C# E G#", // F#9sus4
			"F# G# C# E G#", // F#9sus2
			"A Db E Gb B", // A6/9
			"A Db E Ab B", // Amaj9
			"A Db E Ab Gb", // Amaj13
			"B D Gb A Db", // Bm9
			"B D Gb A E", // Bm11
			"B D Gb A Ab", // Bm13
			"B E Gb A Db", // B9sus4
			"B Db Gb A Db", // B9sus2
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			""
		},
		{ // G nat
			"C Eb G", // Cm
			"D F A", // Dm
			"D# G A#", // D#
			"F A C", // F
			"G Bb D", // Gm
			"A C Eb", // Adim
			"A# D F", // A#
			"C F G", // Csus4
			"C D G", // Csus2
			"D G A", // Dsus4
			"D# F A#", // D#sus2
			"F Bb C", // Fsus4
			"F G C", // Fsus2
			"G C D", // Gsus4
			"G A D", // Gsus2
			"A# D# F", // A#sus4
			"A# C F", // A#sus2
			"C Eb G A", // Cm6
			"C Eb G Bb", // Cm7
			"C F G Bb", // C7sus4
			"C D G Bb", // C7sus2
			"D F A C", // Dm7
			"D G A C", // D7sus4
			"D# G A# C", // D#6
			"D# G A# D", // D#maj7
			"F A C D", // F6
			"F A C Eb", // F7
			"F Bb C Eb", // F7sus4
			"F G C Eb", // F7sus2
			"G Bb D F", // Gm7
			"G C D F", // G7sus4
			"G A D F", // G7sus2
			"A C Eb G", // Am7b5
			"A# D F G", // A#6
			"A# D F A", // A#maj7
			"C F G", // Csus4\F
			"C F G", // Csus4\G
			"C Eb G", // Cm\D#
			"C Eb G", // Cm\G
			"C Eb G", // Cm\Eb
			"C Eb G Bb D", // Cm9
			"C Eb G Bb F", // Cm11
			"C Eb G Bb A", // Cm13
			"C F G Bb D", // C9sus4
			"C D G Bb D", // C9sus2
			"D F A C G", // Dm11
			"D# G A# C F", // D#6/9
			"D# G A# D F", // D#maj9
			"D# G A# D C", // D#maj13
			"F A C D G", // F6/9
			"F A C Eb G", // F9
			"F A C Eb D", // F13
			"F Bb C Eb G", // F9sus4
			"F G C Eb G", // F9sus2
			"G Bb D F A", // Gm9
			"G Bb D F C", // Gm11
			"G C D F A", // G9sus4
			"G A D F A", // G9sus2
			"A# D F G C", // A#6/9
			"A# D F A C", // A#maj9
			"A# D F A G", // A#maj13
			"C Eb G A", // Cm6\A
			"C Eb G Bb", // Cm7\D#
			"C Eb G Bb D", // Cm9\D#
			"C F G Bb", // C7sus4\G
			"C F G Bb D", // C9sus4\G
			"C Eb G Bb", // Cm7\Eb
			"C Eb G Bb D", // Cm9\Eb
			"",
			""
		},
		{ // G#/Ab
			"C# E G#", // C#m
			"D# Gb A#", // D#m
			"E Ab B", // E
			"F# A# C#", // F#
			"G# B D#", // G#m
			"A# Db E", // A#dim
			"B Eb Gb", // B
			"C# F# G#", // C#sus4
			"C# D# G#", // C#sus2
			"D# G# A#", // D#sus4
			"E Gb B", // Esus2
			"F# B C#", // F#sus4
			"F# G# C#", // F#sus2
			"G# C# D#", // G#sus4
			"G# A# D#", // G#sus2
			"B E Gb", // Bsus4
			"B Db Gb", // Bsus2
			"C# E G# A#", // C#m6
			"C# E G# B", // C#m7
			"C# F# G# B", // C#7sus4
			"C# D# G# B", // C#7sus2
			"D# Gb A# Db", // D#m7
			"D# G# A# Db", // D#7sus4
			"E Ab B Db", // E6
			"E Ab B Eb", // Emaj7
			"F# A# C# D#", // F#6
			"F# A# C# E", // F#7
			"F# B C# E", // F#7sus4
			"F# G# C# E", // F#7sus2
			"G# B D# Gb", // G#m7
			"G# C# D# Gb", // G#7sus4
			"G# A# D# Gb", // G#7sus2
			"A# Db E Ab", // A#m7b5
			"B Eb Gb Ab", // B6
			"B Eb Gb Bb", // Bmaj7
			"C# E G# B D#", // C#m9
			"C# E G# B F#", // C#m11
			"C# E G# B A#", // C#m13
			"C# F# G# B D#", // C#9sus4
			"C# D# G# B D#", // C#9sus2
			"D# Gb A# Db G#", // D#m11
			"E Ab B Db Gb", // E6/9
			"E Ab B Eb Gb", // Emaj9
			"E Ab B Eb Db", // Emaj13
			"F# A# C# D# G#", // F#6/9
			"F# A# C# E G#", // F#9
			"F# A# C# E D#", // F#13
			"F# B C# E G#", // F#9sus4
			"F# G# C# E G#", // F#9sus2
			"G# B D# Gb A#", // G#m9
			"G# B D# Gb C#", // G#m11
			"G# C# D# Gb A#", // G#9sus4
			"G# A# D# Gb A#", // G#9sus2
			"B Eb Gb Ab Db", // B6/9
			"B Eb Gb Bb Db", // Bmaj9
			"B Eb Gb Bb Ab", // Bmaj13
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			""
		},
		{ // A nat
			"C E G", // C
			"D F A", // Dm
			"E G B", // Em
			"F A C", // F
			"G B D", // G
			"A C E", // Am
			"B D F", // Bdim
			"C F G", // Csus4
			"C D G", // Csus2
			"D G A", // Dsus4
			"D E A", // Dsus2
			"E A B", // Esus4
			"F G C", // Fsus2
			"G C D", // Gsus4
			"G A D", // Gsus2
			"A D E", // Asus4
			"A B E", // Asus2
			"C E G A", // C6
			"C E G B", // Cmaj7
			"D F A B", // Dm6
			"D F A C", // Dm7
			"D G A C", // D7sus4
			"D E A C", // D7sus2
			"E G B D", // Em7
			"E A B D", // E7sus4
			"F A C D", // F6
			"F A C E", // Fmaj7
			"G B D E", // G6
			"G B D F", // G7
			"G C D F", // G7sus4
			"G A D F", // G7sus2
			"A C E G", // Am7
			"A D E G", // A7sus4
			"A B E G", // A7sus2
			"B D F A", // Bm7b5
			"C E G", // C\E
			"C E G", // C\G
			"C F G", // Csus4\F
			"C F G", // Csus4\G
			"C E G A D", // C6/9
			"C E G B D", // Cmaj9
			"C E G B A", // Cmaj13
			"D F A C E", // Dm9
			"D F A C G", // Dm11
			"D F A C B", // Dm13
			"D G A C E", // D9sus4
			"D E A C E", // D9sus2
			"E G B D A", // Em11
			"F A C D G", // F6/9
			"F A C E G", // Fmaj9
			"F A C E D", // Fmaj13
			"G B D E A", // G6/9
			"G B D F A", // G9
			"G B D F E", // G13
			"G C D F A", // G9sus4
			"G A D F A", // G9sus2
			"A C E G B", // Am9
			"A C E G D", // Am11
			"A D E G B", // A9sus4
			"A B E G B", // A9sus2
			"C E G A", // C6\G
			"C D E G A", // C6/9\E
			"C D E G A", // C6/9\G
			"C E G B", // Cmaj7\G
			"C E G B D", // Cmaj9\E
			"",
			"",
			"",
			"",
			""
		},
		{ // A#/Bb
			"C# F G#", // C#
			"C Eb Gb", // Cdim
			"D# Gb A#", // D#m
			"F Ab C", // Fm
			"F# A# C#", // F#
			"G# C D#", // G#
			"A# Db F", // A#m
			"C# F# G#", // C#sus4
			"C# D# G#", // C#sus2
			"D# G# A#", // D#sus4
			"D# F A#", // D#sus2
			"F Bb C", // Fsus4
			"F# G# C#", // F#sus2
			"G# C# D#", // G#sus4
			"G# A# D#", // G#sus2
			"A# D# F", // A#sus4
			"A# C F", // A#sus2
			"C# F G# A#", // C#6
			"C# F G# C", // C#maj7
			"C Eb Gb Bb", // Cm7b5
			"D# Gb A# C", // D#m6
			"D# Gb A# Db", // D#m7
			"D# G# A# Db", // D#7sus4
			"D# F A# Db", // D#7sus2
			"F Ab C Eb", // Fm7
			"F Bb C Eb", // F7sus4
			"F# A# C# D#", // F#6
			"F# A# C# F", // F#maj7
			"G# C D# F", // G#6
			"G# C D# Gb", // G#7
			"G# C# D# Gb", // G#7sus4
			"G# A# D# Gb", // G#7sus2
			"A# Db F Ab", // A#m7
			"A# D# F Ab", // A#7sus4
			"A# C F Ab", // A#7sus2
			"C# F G# A# D#", // C#6/9
			"C# F G# C D#", // C#maj9
			"C# F G# C A#", // C#maj13
			"D# Gb A# Db F", // D#m9
			"D# Gb A# Db G#", // D#m11
			"D# Gb A# Db C", // D#m13
			"D# G# A# Db F", // D#9sus4
			"D# F A# Db F", // D#9sus2
			"F Ab C Eb Bb", // Fm11
			"F# A# C# D# G#", // F#6/9
			"F# A# C# F G#", // F#maj9
			"F# A# C# F D#", // F#maj13
			"G# C D# F A#", // G#6/9
			"G# C D# Gb A#", // G#9
			"G# C D# Gb F", // G#13
			"G# C# D# Gb A#", // G#9sus4
			"G# A# D# Gb A#", // G#9sus2
			"A# Db F Ab C", // A#m9
			"A# Db F Ab D#", // A#m11
			"A# D# F Ab C", // A#9sus4
			"A# C F Ab C", // A#9sus2
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			""
		},
		{ // B nat
			"C# E G", // C#dim
			"D Gb A", // D
			"E G B", // Em
			"F# A C#", // F#m
			"G B D", // G
			"A Db E", // A
			"B D Gb", // Bm
			"D G A", // Dsus4
			"D E A", // Dsus2
			"E A B", // Esus4
			"E Gb B", // Esus2
			"F# B C#", // F#sus4
			"G A D", // Gsus2
			"A D E", // Asus4
			"A B E", // Asus2
			"B E Gb", // Bsus4
			"B Db Gb", // Bsus2
			"C# E G B", // C#m7b5
			"D Gb A B", // D6
			"D Gb A Db", // Dmaj7
			"E G B Db", // Em6
			"E G B D", // Em7
			"E A B D", // E7sus4
			"E Gb B D", // E7sus2
			"F# A C# E", // F#m7
			"F# B C# E", // F#7sus4
			"G B D E", // G6
			"G B D Gb", // Gmaj7
			"A Db E Gb", // A6
			"A Db E G", // A7
			"A D E G", // A7sus4
			"A B E G", // A7sus2
			"B D Gb A", // Bm7
			"B E Gb A", // B7sus4
			"B Db Gb A", // B7sus2
			"D Gb A B E", // D6/9
			"D Gb A Db E", // Dmaj9
			"D Gb A Db B", // Dmaj13
			"E G B D Gb", // Em9
			"E G B D A", // Em11
			"E G B D Db", // Em13
			"E A B D Gb", // E9sus4
			"E Gb B D Gb", // E9sus2
			"F# A C# E B", // F#m11
			"G B D E A", // G6/9
			"G B D Gb A", // Gmaj9
			"G B D Gb E", // Gmaj13
			"A Db E Gb B", // A6/9
			"A Db E G B", // A9
			"A Db E G Gb", // A13
			"A D E G B", // A9sus4
			"A B E G B", // A9sus2
			"B D Gb A Db", // Bm9
			"B D Gb A E", // Bm11
			"B E Gb A Db", // B9sus4
			"B Db Gb A Db", // B9sus2
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			""
		},
	},
	{ // https://www.scales-chords.com/shscalesbytype.php?sname=diminished%20lydian
		{ // C dim
			"C Eb G", // Cm
			"C Eb Gb", // Cdim
			"D Gb A", // D
			"D# G B", // D#aug
			"D# Gb A", // D#dim
			"F# A C", // F#dim
			"G B D", // G
			"G B Eb", // Gaug
			"A C Eb", // Adim
			"B Eb Gb", // B
			"B D Gb", // Bm
			"B Eb G", // Baug
			"C D G", // Csus2
			"D G A", // Dsus4
			"G C D", // Gsus4
			"G A D", // Gsus2
			"C Eb G A", // Cm6
			"C Eb G B", // Cm(maj7)
			"C Eb Gb A", // Cº7
			"D Gb A B", // D6
			"D Gb A C", // D7
			"D G A C", // D7sus4
			"D# Gb A C", // D#º7
			"F# A C D#", // F#º7
			"G B D Gb", // Gmaj7
			"A C Eb G", // Am7b5
			"A C Eb Gb", // Aº7
			"B Eb Gb A", // B7
			"B Eb G A", // B7#5
			"B D Gb A", // Bm7
			"C Eb G", // Cm\D#
			"C Eb G", // Cm\G
			"C Eb G", // Cm\Eb
			"D Gb A C B", // D13
			"G B D Gb A", // Gmaj9
			"C Eb G A", // Cm6\A
			"C Eb Gb A", // Cº7\A
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			""
		},
		{ // C#/Db
			"C E G", // C
			"C Eb G", // Cm
			"C# E G#", // C#m
			"C E Ab", // Caug
			"C# E G", // C#dim
			"D# G A#", // D#
			"E Ab C", // Eaug
			"E G Bb", // Edim
			"G Bb Db", // Gdim
			"G# C D#", // G#
			"G# C E", // G#aug
			"A# Db E", // A#dim
			"C# D# G#", // C#sus2
			"D# G# A#", // D#sus4
			"G# C# D#", // G#sus4
			"G# A# D#", // G#sus2
			"C E G Bb", // C7
			"C E G# Bb", // C7#5
			"C# E G# A#", // C#m6
			"C# E G# C", // C#m(maj7)
			"C# E G A#", // C#º7
			"C Eb G Bb", // Cm7
			"D# G A# C", // D#6
			"D# G A# Db", // D#7
			"D# G# A# Db", // D#7sus4
			"E G Bb Db", // Eº7
			"G Bb Db E", // Gº7
			"G# C D# G", // G#maj7
			"A# Db E Ab", // A#m7b5
			"A# Db E G", // A#º7
			"C E G", // C\E
			"C E G", // C\G
			"C Eb G", // Cm\D#
			"C Eb G", // Cm\G
			"C Eb G", // Cm\Eb
			"D# G A# Db C", // D#13
			"G# C D# G A#", // G#maj9
			"C Eb G Bb", // Cm7\D#
			"C E G Bb", // C7\G
			"C E G Bb", // C7\E
			"C Eb G Bb", // Cm7\Eb
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			""
		},
		{ // D dim
			"C# F G#", // C#
			"C# E G#", // C#m
			"C# F A", // C#aug
			"D F A", // Dm
			"D F Ab", // Ddim
			"E Ab B", // E
			"F A Db", // Faug
			"F Ab B", // Fdim
			"G# B D", // G#dim
			"A Db E", // A
			"A Db F", // Aaug
			"B D F", // Bdim
			"D E A", // Dsus2
			"E A B", // Esus4
			"A D E", // Asus4
			"A B E", // Asus2
			"C# F G# B", // C#7
			"C# F A B", // C#7#5
			"C# E G# B", // C#m7
			"D F A B", // Dm6
			"D F A Db", // Dm(maj7)
			"D F Ab B", // Dº7
			"E Ab B Db", // E6
			"E Ab B D", // E7
			"E A B D", // E7sus4
			"F Ab B D", // Fº7
			"G# B D F", // G#º7
			"A Db E Ab", // Amaj7
			"B D F A", // Bm7b5
			"B D F Ab", // Bº7
			"E Ab B D Db", // E13
			"A Db E Ab B", // Amaj9
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			""
		},
		{ // D#/Eb
			"C Eb Gb", // Cdim
			"D Gb A", // D
			"D F A", // Dm
			"D Gb Bb", // Daug
			"D# Gb A#", // D#m
			"D# Gb A", // D#dim
			"F A C", // F
			"F# A# D", // F#aug
			"F# A C", // F#dim
			"A C Eb", // Adim
			"A# D F", // A#
			"A# D Gb", // A#aug
			"D# F A#", // D#sus2
			"F Bb C", // Fsus4
			"A# D# F", // A#sus4
			"A# C F", // A#sus2
			"C Eb Gb Bb", // Cm7b5
			"C Eb Gb A", // Cº7
			"D Gb A C", // D7
			"D Gb A# C", // D7#5
			"D F A C", // Dm7
			"D# Gb A# C", // D#m6
			"D# Gb A# D", // D#m(maj7)
			"D# Gb A C", // D#º7
			"F A C D", // F6
			"F A C Eb", // F7
			"F Bb C Eb", // F7sus4
			"F# A C D#", // F#º7
			"A C Eb Gb", // Aº7
			"A# D F A", // A#maj7
			"F A C Eb D", // F13
			"A# D F A C", // A#maj9
			"C Eb Gb A", // Cº7\A
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			""
		},
		{ // E dim
			"C# E G", // C#dim
			"D# G A#", // D#
			"D# Gb A#", // D#m
			"D# G B", // D#aug
			"E G B", // Em
			"E G Bb", // Edim
			"F# A# C#", // F#
			"G B Eb", // Gaug
			"G Bb Db", // Gdim
			"A# Db E", // A#dim
			"B Eb Gb", // B
			"B Eb G", // Baug
			"E Gb B", // Esus2
			"F# B C#", // F#sus4
			"B E Gb", // Bsus4
			"B Db Gb", // Bsus2
			"C# E G B", // C#m7b5
			"C# E G A#", // C#º7
			"D# G A# Db", // D#7
			"D# G B Db", // D#7#5
			"D# Gb A# Db", // D#m7
			"E G B Db", // Em6
			"E G B Eb", // Em(maj7)
			"E G Bb Db", // Eº7
			"F# A# C# D#", // F#6
			"F# A# C# E", // F#7
			"F# B C# E", // F#7sus4
			"G Bb Db E", // Gº7
			"A# Db E G", // A#º7
			"B Eb Gb Bb", // Bmaj7
			"F# A# C# E D#", // F#13
			"B Eb Gb Bb Db", // Bmaj9
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			""
		},
		{ // F dim
			"C E G", // C
			"C E Ab", // Caug
			"D F Ab", // Ddim
			"E Ab B", // E
			"E G B", // Em
			"E Ab C", // Eaug
			"F Ab C", // Fm
			"F Ab B", // Fdim
			"G B D", // G
			"G# C E", // G#aug
			"G# B D", // G#dim
			"B D F", // Bdim
			"C F G", // Csus4
			"C D G", // Csus2
			"F G C", // Fsus2
			"G C D", // Gsus4
			"C E G B", // Cmaj7
			"D F Ab C", // Dm7b5
			"D F Ab B", // Dº7
			"E Ab B D", // E7
			"E Ab C D", // E7#5
			"E G B D", // Em7
			"F Ab C D", // Fm6
			"F Ab C E", // Fm(maj7)
			"F Ab B D", // Fº7
			"G B D E", // G6
			"G B D F", // G7
			"G C D F", // G7sus4
			"G# B D F", // G#º7
			"B D F Ab", // Bº7
			"C E G", // C\E
			"C E G", // C\G
			"C F G", // Csus4\F
			"C F G", // Csus4\G
			"C E G B D", // Cmaj9
			"G B D F E", // G13
			"C E G B", // Cmaj7\G
			"C E G B D", // Cmaj9\E
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			""
		},
		{ // F#/Gb
			"C# F G#", // C#
			"C Eb Gb", // Cdim
			"C# F A", // C#aug
			"D# Gb A", // D#dim
			"F A C", // F
			"F Ab C", // Fm
			"F A Db", // Faug
			"F# A C#", // F#m
			"F# A C", // F#dim
			"G# C D#", // G#
			"A Db F", // Aaug
			"A C Eb", // Adim
			"C# F# G#", // C#sus4
			"C# D# G#", // C#sus2
			"F# G# C#", // F#sus2
			"G# C# D#", // G#sus4
			"C# F G# C", // C#maj7
			"C Eb Gb A", // Cº7
			"D# Gb A Db", // D#m7b5
			"D# Gb A C", // D#º7
			"F A C Eb", // F7
			"F A C# Eb", // F7#5
			"F Ab C Eb", // Fm7
			"F# A C# D#", // F#m6
			"F# A C# F", // F#m(maj7)
			"F# A C D#", // F#º7
			"G# C D# F", // G#6
			"G# C D# Gb", // G#7
			"G# C# D# Gb", // G#7sus4
			"A C Eb Gb", // Aº7
			"C# F G# C D#", // C#maj9
			"G# C D# Gb F", // G#13
			"C Eb Gb A", // Cº7\A
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			""
		},
		{ // G dim
			"C# E G", // C#dim
			"D Gb A", // D
			"D Gb Bb", // Daug
			"E G Bb", // Edim
			"F# A# C#", // F#
			"F# A C#", // F#m
			"F# A# D", // F#aug
			"G Bb D", // Gm
			"G Bb Db", // Gdim
			"A Db E", // A
			"A# D Gb", // A#aug
			"A# Db E", // A#dim
			"D G A", // Dsus4
			"D E A", // Dsus2
			"G A D", // Gsus2
			"A D E", // Asus4
			"C# E G A#", // C#º7
			"D Gb A Db", // Dmaj7
			"E G Bb D", // Em7b5
			"E G Bb Db", // Eº7
			"F# A# C# E", // F#7
			"F# A# D E", // F#7#5
			"F# A C# E", // F#m7
			"G Bb D E", // Gm6
			"G Bb D Gb", // Gm(maj7)
			"G Bb Db E", // Gº7
			"A Db E Gb", // A6
			"A Db E G", // A7
			"A D E G", // A7sus4
			"A# Db E G", // A#º7
			"D Gb A Db E", // Dmaj9
			"A Db E G Gb", // A13
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			""
		},
		{ // G#/Ab
			"D F Ab", // Ddim
			"D# G A#", // D#
			"D# G B", // D#aug
			"F Ab B", // Fdim
			"G B D", // G
			"G Bb D", // Gm
			"G B Eb", // Gaug
			"G# B D#", // G#m
			"G# B D", // G#dim
			"A# D F", // A#
			"B Eb G", // Baug
			"B D F", // Bdim
			"D# G# A#", // D#sus4
			"D# F A#", // D#sus2
			"G# A# D#", // G#sus2
			"A# D# F", // A#sus4
			"D F Ab B", // Dº7
			"D# G A# D", // D#maj7
			"F Ab B Eb", // Fm7b5
			"F Ab B D", // Fº7
			"G B D F", // G7
			"G B D# F", // G7#5
			"G Bb D F", // Gm7
			"G# B D# F", // G#m6
			"G# B D# G", // G#m(maj7)
			"G# B D F", // G#º7
			"A# D F G", // A#6
			"A# D F Ab", // A#7
			"A# D# F Ab", // A#7sus4
			"B D F Ab", // Bº7
			"D# G A# D F", // D#maj9
			"A# D F Ab G", // A#13
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			""
		},
		{ // A dim
			"C E Ab", // Caug
			"C Eb Gb", // Cdim
			"D# Gb A", // D#dim
			"E Ab B", // E
			"E Ab C", // Eaug
			"F# A C", // F#dim
			"G# C D#", // G#
			"G# B D#", // G#m
			"G# C E", // G#aug
			"A C E", // Am
			"A C Eb", // Adim
			"B Eb Gb", // B
			"E A B", // Esus4
			"E Gb B", // Esus2
			"A B E", // Asus2
			"B E Gb", // Bsus4
			"C Eb Gb A", // Cº7
			"D# Gb A C", // D#º7
			"E Ab B Eb", // Emaj7
			"F# A C E", // F#m7b5
			"F# A C D#", // F#º7
			"G# C D# Gb", // G#7
			"G# C E Gb", // G#7#5
			"G# B D# Gb", // G#m7
			"A C E Gb", // Am6
			"A C E Ab", // Am(maj7)
			"A C Eb Gb", // Aº7
			"B Eb Gb Ab", // B6
			"B Eb Gb A", // B7
			"B E Gb A", // B7sus4
			"E Ab B Eb Gb", // Emaj9
			"B Eb Gb A Ab", // B13
			"C Eb Gb A", // Cº7\A
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			""
		},
		{ // A#/Bb
			"C E G", // C
			"C# F A", // C#aug
			"C# E G", // C#dim
			"E G Bb", // Edim
			"F A C", // F
			"F A Db", // Faug
			"G Bb Db", // Gdim
			"A Db E", // A
			"A C E", // Am
			"A Db F", // Aaug
			"A# Db F", // A#m
			"A# Db E", // A#dim
			"C F G", // Csus4
			"F Bb C", // Fsus4
			"F G C", // Fsus2
			"A# C F", // A#sus2
			"C E G A", // C6
			"C E G Bb", // C7
			"C# E G A#", // C#º7
			"C F G Bb", // C7sus4
			"E G Bb Db", // Eº7
			"F A C E", // Fmaj7
			"G Bb Db F", // Gm7b5
			"G Bb Db E", // Gº7
			"A Db E G", // A7
			"A Db F G", // A7#5
			"A C E G", // Am7
			"A# Db F G", // A#m6
			"A# Db F A", // A#m(maj7)
			"A# Db E G", // A#º7
			"C E G", // C\E
			"C E G", // C\G
			"C F G", // Csus4\F
			"C F G", // Csus4\G
			"C E G Bb A", // C13
			"F A C E G", // Fmaj9
			"C E G A", // C6\G
			"C E G Bb", // C7\G
			"C E G Bb", // C7\E
			"C F G Bb", // C7sus4\G
			"C E G Bb A", // C13\A#
			"C E G Bb A", // C13\Bb
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			""
		},
		{ // B dim
			"C# F G#", // C#
			"D Gb Bb", // Daug
			"D F Ab", // Ddim
			"F Ab B", // Fdim
			"F# A# C#", // F#
			"F# A# D", // F#aug
			"G# B D", // G#dim
			"A# D F", // A#
			"A# Db F", // A#m
			"A# D Gb", // A#aug
			"B D Gb", // Bm
			"B D F", // Bdim
			"C# F# G#", // C#sus4
			"F# B C#", // F#sus4
			"F# G# C#", // F#sus2
			"B Db Gb", // Bsus2
			"C# F G# A#", // C#6
			"C# F G# B", // C#7
			"C# F# G# B", // C#7sus4
			"D F Ab B", // Dº7
			"F Ab B D", // Fº7
			"F# A# C# F", // F#maj7
			"G# B D Gb", // G#m7b5
			"G# B D F", // G#º7
			"A# D F Ab", // A#7
			"A# D F# Ab", // A#7#5
			"A# Db F Ab", // A#m7
			"B D Gb Ab", // Bm6
			"B D Gb Bb", // Bm(maj7)
			"B D F Ab", // Bº7
			"C# F G# B A#", // C#13
			"F# A# C# F G#", // F#maj9
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			""
		}
	}
// Max Chords in a Scale: 70
// Total Chords: 2472
};

const unsigned char white_key_pitch_to_array_index[] = { // total of 70 elements with a/b toggle
		0, 0xFF, 1, 0xFF, 2, 2, 0xFF, 3, 0xFF, 4, 0xFF, 5,
		5, 0xFF, 6, 0xFF, 7, 7, 0xFF, 8, 0xFF, 9, 0xFF, 10,
		10, 0xFF, 11, 0xFF, 12, 12, 0xFF, 13, 0xFF, 14, 0xFF, 15,
		15, 0xFF, 16, 0xFF, 17, 17, 0xFF, 18, 0xFF, 19, 0xFF, 20,
		20, 0xFF, 21, 0xFF, 22, 22, 0xFF, 23, 0xFF, 24, 0xFF, 25,
		25, 0xFF, 26, 0xFF, 27, 27, 0xFF, 28, 0xFF, 29, 0xFF, 30,
		30, 0xFF, 31, 0xFF, 32, 32, 0xFF, 33, 0xFF, 34, 0xFF, 35
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

//	diag_printf("word:%s\n", word);
	// short-circuit
	if (word[0] == '\0'){
		pitches[0] = 0xFF; // add a terminator
		return;
	}

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

unsigned char toneToIndex(unsigned char scale_ndx){

	 return ( OCTAVE - my_bit2ndx( SOLO_assistant_page->scaleLead[ scale_ndx ] )) -1 /* zero based */;
}

unsigned char modifyChordOctave(signed char val){

	if ( SOLO_scale_chords_octave + val > 3 || SOLO_scale_chords_octave + val < -3 ) return FALSE; // out of range
	SOLO_scale_chords_octave += val;
	return TRUE;
}

void modifyChordPitch(signed char val){

	signed char pit = SOLO_assistant_page->attr_PIT;

	if ( pit + val < 0 ){

		if (!modifyChordOctave(-1)) return;
	}
	else if ( pit + val >= OCTAVE ){

		if (!modifyChordOctave(1)) return;
	}
	SOLO_assistant_page->attr_PIT = (unsigned char) (abs(OCTAVE + (pit + val)) % OCTAVE);
}

void modifyChordTone(signed char val){ // add scale_ndx 2, 3 for chords, program

	unsigned char toneNdx = my_bit2ndx( SOLO_assistant_page->scaleLead[ G_scale_ndx ] ) + OCTAVE;
//	diag_printf("tone:%d\n", toneNdx);

	// TODO: check min and max octaves
	if (toneNdx - val > TONE_BTN_OFFSET){ // TODO: count octaves

//		diag_printf("down octave\n");
	}
	else if (toneNdx - val < OCTAVE){ // TODO: count octaves

//		diag_printf("up octave\n");
	}
	else {
		modify_scale_composition( SOLO_assistant_page, toneNdx - val, G_scale_ndx);
	}

	// XXX
//	SOLO_assistant_page-> scaleNotes[ G_scale_ndx ] =
//					my_shift_bitpattern( 	SOLO_assistant_page->scaleNotes_old, 12, INC,
//											(11 - my_bit2ndx(SOLO_assistant_page-> scaleLead[ G_scale_ndx ] ) )  );
//	pitch_to_noteNdx(pitch + val);
//	keyNdx_to_noteNdx( keyNdx )
//
//	key_ChordScaleSelector( keyNdx, SOLO_assistant_page );
}

unsigned char currentScaleIndex( Pagestruct* target_page ){

	unsigned int j = my_shift_bitpattern( target_page-> scaleNotes[G_scale_ndx], 12, INC,
							  my_bit2ndx( target_page-> scaleLead[G_scale_ndx] )+1);

	switch( j ) {
		case SCALE_SIG_PEN: return 0;
		case SCALE_SIG_WHL: return 1;
		case SCALE_SIG_MAJ:	return 2;
		case SCALE_SIG_MIN: return 3;
		case SCALE_SIG_DIM: return 4;
		default: return NOP;
	}
}

unsigned short modulationsOfChord( unsigned char chord_id, Pagestruct* target_page ){

	unsigned char tone = toneToIndex( G_scale_ndx );
	unsigned char scale = currentScaleIndex( target_page );
	unsigned char i, j;
	unsigned short modulations = 0;

	for ( i=0; i < OCTAVE; i++ ){

		if ( i == tone ){
			continue;
		}

		for ( j=0; j < MAX_CHORDS; j++ ){

			if ( strcmp(chords[scale][tone][chord_id], chords[scale][i][j]) == 0 ){

				SET_BIT(modulations, i);
			}
		}
	}

	return modulations;
}

void copyChord(Chordstruct* src, Chordstruct* dest){

	dest->chord_id = src->chord_id;
	dest->octave = src->octave;
	dest->pitch = src->pitch;
	dest->scale = src->scale;
	dest->strum = src->strum;
	dest->tone = src->tone;
	dest->attr_LEN = src->attr_LEN;
	dest->attr_VEL = src->attr_VEL;
}

void enterProgramEditor(){

	if ( G_run_bit == ON ) return;

	SOLO_scale_chords_program = ON; // palette editor enabled
	SOLO_scale_chords_program_keys = ON;
	GRID_CURSOR = SOLO_assistant_page->pageNdx;

	if ( SOLO_last_chord != NULL ){

		SOLO_scale_chords_program_armed = ON;
	}
}

unsigned char hasArpPattern( unsigned char chord_palette_ndx ){
	int i;

	for (i=0; i < MATRIX_NROF_COLUMNS; i++){

		if ( Note_get_status( Chord_palette_repository[chord_palette_ndx].Arp[i], STEPSTAT_TOGGLE ) == ON ){

			return ON;
		}
	}
	return OFF;
}

void clearArpPattern( unsigned char chord_palette_ndx ){
	unsigned char i;

	for (i=0; i < MATRIX_NROF_COLUMNS; i++){

		initNote(Chord_palette_repository[chord_palette_ndx].Arp[i]);
		noteToStep(Chord_palette_repository[chord_palette_ndx].Arp[i], SOLO_assistant_page->Step[ARP_TRACK][i]);
	}
	SOLO_assistant_page->attr_LEN = MATRIX_NROF_COLUMNS;
}

void saveArpPattern( unsigned char chord_palette_ndx ){
	unsigned char i, last = OFF;

	for (i=0; i < MATRIX_NROF_COLUMNS; i++){
		last = Step_get_status( SOLO_assistant_page->Step[ARP_TRACK][i], STEPSTAT_SKIP );
		noteToStep(Chord_palette_repository[chord_palette_ndx].Arp[i], SOLO_assistant_page->Step[ARP_BUFFER_TRACK][i]); // save to row 1
		if ( last == ON ){
			Step_set_status( SOLO_assistant_page->Step[ARP_BUFFER_TRACK][i], STEPSTAT_SKIP, ON );
		}
	}
	SOLO_assistant_page->Track[ARP_BUFFER_TRACK]->attr_LEN = SOLO_assistant_page->attr_LEN;
	SOLO_assistant_page->Track[ARP_BUFFER_TRACK]->attr_STATUS = ON;
}

void restoreArpPattern( unsigned char chord_palette_ndx ){
	unsigned char i, last = OFF;

	for (i=0; i < MATRIX_NROF_COLUMNS; i++){

		last = Step_get_status( SOLO_assistant_page->Step[ARP_BUFFER_TRACK][i], STEPSTAT_SKIP );
		stepToNote(SOLO_assistant_page->Step[ARP_BUFFER_TRACK][i], Chord_palette_repository[chord_palette_ndx].Arp[i]);
		noteToStep(Chord_palette_repository[chord_palette_ndx].Arp[i], SOLO_assistant_page->Step[ARP_TRACK][i]);
		if ( last == ON ){
			Step_set_status( SOLO_assistant_page->Step[ARP_TRACK][i], STEPSTAT_SKIP, ON );
		}
	}
	SOLO_assistant_page->attr_LEN = SOLO_assistant_page->Track[ARP_BUFFER_TRACK]->attr_LEN;
	SOLO_assistant_page->Track[ARP_BUFFER_TRACK]->attr_STATUS = OFF;
}

void assignChordToPalette(unsigned char in_pitch){

	if ( SOLO_last_chord == NULL || Chord_palette_repository[in_pitch % OCTAVE].chord_id != NOP ){ // don't overwrite
		SOLO_scale_chords_program_armed = OFF;
		return;
	}

	clearArpPattern(SOLO_scale_chords_palette_ndx);
	SOLO_scale_chords_palette_ndx = in_pitch % OCTAVE;
	copyChord(SOLO_last_chord, &Chord_palette_repository[SOLO_scale_chords_palette_ndx]);
	SOLO_scale_chords_program_armed = OFF;
}

void assignLastNotes(){

	int i;
	unsigned char pitches[MAX_NOTES];
	Chordstruct* chord = &Chord_palette_repository[SOLO_scale_chords_palette_ndx];
	SOLO_scale_chords_last = OFF;

	translateSymbolsToChord(chords[chord->scale][chord->tone][chord->chord_id], pitches);

	for (i=0; i < MAX_NOTES; i++){

		if (pitches[i] == 0xFF){
			break;
		}

		SET_BIT( SOLO_scale_chords_last, pitches[i] ); // show the chord keys in the grid
	}
}

Notestruct* buildNote( unsigned char col, unsigned char note_pitch ){

	Chordstruct* chord = &Chord_palette_repository[SOLO_scale_chords_palette_ndx];
	Notestruct* note = chord->Arp[col];
	Note_set_status( note, STEPSTAT_TOGGLE, ON);
	note->attr_PIT = (C3 + (OCTAVE * SOLO_scale_chords_octave) + note_pitch);
	note->attr_LEN = chord->attr_LEN;
	return note;
}

void chordPitchToStep( Pagestruct* target_page,
					   unsigned char col,
					   unsigned char note_pitch ){

	Notestruct* note = buildNote(col, note_pitch);
	Stepstruct* step = target_page->Step[ARP_TRACK][col];
	noteToStep(note, step);
}

void chordPitchAddToStep( Pagestruct* target_page,
						  unsigned char col,
					      unsigned char note_pitch ){

	Notestruct* note = buildNote(col, note_pitch);
	Stepstruct* step = target_page->Step[ARP_TRACK][col];

	if ( Step_get_status( step, STEPSTAT_TOGGLE ) == OFF ){
		noteToStep(note, step);
	}
	else {
		make_chord( step, 0, note->attr_PIT );
		stepToNote( step, note );
	}
}

void shiftArpPattern( unsigned char direction, unsigned char pressedCol ){

	signed char i, j;
	unsigned char hasArp = OFF;
	unsigned char hasStop = OFF;
	signed char stopCol = MATRIX_NROF_COLUMNS -1;
	signed char startCol;

	for (i=0; i < MATRIX_NROF_COLUMNS; i++){

		if ( Step_get_status( SOLO_assistant_page->Step[ARP_TRACK][i], STEPSTAT_SKIP ) == ON ){

			stopCol = i;
			hasStop = ON;
		}
		if ( Step_get_status( SOLO_assistant_page->Step[ARP_TRACK][i], STEPSTAT_TOGGLE ) == ON ){

			hasArp = ON;
		}
	}

	if (( hasArp == OFF ) ||
		( direction == POS && (( stopCol == MATRIX_NROF_COLUMNS -1 && hasStop == OFF ) ||
		( pressedCol >= stopCol +1 )))
	   ){
		return;
	}

	startCol = stopCol;

	if ( direction == NEG ){

		if ( Step_get_status( SOLO_assistant_page->Step[ARP_TRACK][pressedCol], STEPSTAT_TOGGLE ) == ON ){ // no skip to the right

			for (i=0; i < pressedCol -1; i++){ // are there any steps to the left

				if ( Step_get_status( SOLO_assistant_page->Step[ARP_TRACK][i], STEPSTAT_TOGGLE ) == ON ){ // yes

					return; // do nothing
				}
			}
		}
		startCol = pressedCol;
		pressedCol = 0;
	}

	if ( stopCol != 0 && stopCol < ( MATRIX_NROF_COLUMNS -1 ) && direction == POS ){

		Step_set_status( SOLO_assistant_page->Step[ARP_TRACK][stopCol +1], STEPSTAT_SKIP, ON ); // show the end of arp LED red
		Note_set_status( Chord_palette_repository[SOLO_scale_chords_palette_ndx].Arp[stopCol +1], STEPSTAT_SKIP, ON );
		SOLO_assistant_page->attr_LEN = stopCol +1;
	}

	for (i=startCol; i >= pressedCol; --i){

		if ( direction == POS ){ // shift right

			if ( i == pressedCol ) break;
			copyNote(Chord_palette_repository[SOLO_scale_chords_palette_ndx].Arp[i-1], Chord_palette_repository[SOLO_scale_chords_palette_ndx].Arp[i]);
			noteToStep(Chord_palette_repository[SOLO_scale_chords_palette_ndx].Arp[i], SOLO_assistant_page->Step[ARP_TRACK][i]);
		}
		else { // push left

			// shrink 16 steps from the right
			if ( hasStop == OFF &&
				 stopCol == ( MATRIX_NROF_COLUMNS -1 ) &&
				 stopCol == i && // only affect the last column
				 Step_get_status( SOLO_assistant_page->Step[ARP_TRACK][i], STEPSTAT_TOGGLE ) == OFF &&
				 Step_get_status( SOLO_assistant_page->Step[ARP_TRACK][i], STEPSTAT_SKIP ) == OFF){

				initNote(Chord_palette_repository[SOLO_scale_chords_palette_ndx].Arp[i]);
				noteToStep(Chord_palette_repository[SOLO_scale_chords_palette_ndx].Arp[i], SOLO_assistant_page->Step[ARP_TRACK][i]);
				Step_set_status( SOLO_assistant_page->Step[ARP_TRACK][i], STEPSTAT_SKIP, ON );
				Note_set_status( Chord_palette_repository[SOLO_scale_chords_palette_ndx].Arp[i], STEPSTAT_SKIP, ON );
				SOLO_assistant_page->attr_LEN = i;
				break;
			}

			// if the current step is empty push the right side step left
			if ( Step_get_status( SOLO_assistant_page->Step[ARP_TRACK][i], STEPSTAT_TOGGLE ) == OFF &&
				 Step_get_status( SOLO_assistant_page->Step[ARP_TRACK][i], STEPSTAT_SKIP ) == OFF ){

				// cascade
				for (j=i; j <= stopCol; j++){

					if ( Step_get_status( SOLO_assistant_page->Step[ARP_TRACK][j+1], STEPSTAT_SKIP ) == ON ||
						 j == ( MATRIX_NROF_COLUMNS -1 )){

						// push the end of arp in
						initNote(Chord_palette_repository[SOLO_scale_chords_palette_ndx].Arp[j+1]);
						noteToStep(Chord_palette_repository[SOLO_scale_chords_palette_ndx].Arp[j+1], SOLO_assistant_page->Step[ARP_TRACK][j+1]);
						initNote(Chord_palette_repository[SOLO_scale_chords_palette_ndx].Arp[j]);
						noteToStep(Chord_palette_repository[SOLO_scale_chords_palette_ndx].Arp[j], SOLO_assistant_page->Step[ARP_TRACK][j]);
						Step_set_status( SOLO_assistant_page->Step[ARP_TRACK][j], STEPSTAT_SKIP, ON );
						Note_set_status( Chord_palette_repository[SOLO_scale_chords_palette_ndx].Arp[j], STEPSTAT_SKIP, ON );
						SOLO_assistant_page->attr_LEN = j;
						break;
					}
					copyNote(Chord_palette_repository[SOLO_scale_chords_palette_ndx].Arp[j+1], Chord_palette_repository[SOLO_scale_chords_palette_ndx].Arp[j]);
					noteToStep(Chord_palette_repository[SOLO_scale_chords_palette_ndx].Arp[j], SOLO_assistant_page->Step[ARP_TRACK][j]);
				}
				break;
			}
		}
	}

	if ( direction == POS ){
		// clear the pressed page
		initNote(Chord_palette_repository[SOLO_scale_chords_palette_ndx].Arp[pressedCol]);
		noteToStep(Chord_palette_repository[SOLO_scale_chords_palette_ndx].Arp[pressedCol], SOLO_assistant_page->Step[ARP_TRACK][pressedCol]);
	}
}

void buildPresetArp( unsigned char keyNdx ){

	unsigned char i, j = 0, size = 0, pivot = 0, dir = 0, val, up = 0, down = 0;
	unsigned char pitches[MAX_NOTES];
	Chordstruct* chord = &Chord_palette_repository[SOLO_scale_chords_palette_ndx];

	translateSymbolsToChord(chords[chord->scale][chord->tone][chord->chord_id], pitches);

	for (i=0; i < MAX_NOTES; i++){

		if (pitches[i] == 0xFF){
			break;
		}
		size++;
	}

	clearArpPattern(SOLO_scale_chords_palette_ndx);

	for (i=0; i<size; i++){

		switch (keyNdx) {
			case KEY_MIXTGT_ATR: // ABC

				chordPitchToStep( SOLO_assistant_page, j, pitches[i] );
				break;

			case KEY_MIXTGT_VOL: // CBA

				chordPitchToStep( SOLO_assistant_page, j, pitches[(size -1) -i] );
				break;

			case KEY_MIXTGT_PAN: // BAC

				// inside out down then up
				if ( pivot == 0 ) pivot = size / 2;
				if ( dir == 0 ) val = pivot -i;
				else val = i;
				if ( val == 0 ) dir = 1;
				// down to zero then up past pivot to end
				chordPitchToStep( SOLO_assistant_page, j, pitches[val] );
				break;

			case KEY_MIXTGT_MOD: // BCA

				// inside out up then down
				if ( pivot == 0 ) pivot = size / 2;
				if ( dir == 0 ) val = pivot +i; // [01234]
				else val = --pivot;				// [23410]
				if ( val == size -1 ) dir = 1;
				chordPitchToStep( SOLO_assistant_page, j, pitches[val] );
				break;

			case KEY_MIXTGT_EXP: // ACB

				// outside in start low
				if ( down == 0 ) down = size -1; 				// [01234]
				val = ( (dir ^= 1) == 1 ) ? up++ : down--;		// [04132]
				chordPitchToStep( SOLO_assistant_page, j, pitches[val] );
				break;

			case KEY_MIXTGT_USR0: // CAB

				// outside in start high
				if ( down == 0 ) down = size -1; 				// [01234]
				val = ( (dir ^= 1) == 0 ) ? up++ : down--;		// [40312]
				chordPitchToStep( SOLO_assistant_page, j, pitches[val] );
				break;

			default:
				break;
		}
		j+=2; // 8th intervals
	}

	if ( size > 0 && size < MATRIX_NROF_COLUMNS ){

		Note_set_status( chord->Arp[j], STEPSTAT_SKIP, ON );
		noteToStep( chord->Arp[j], SOLO_assistant_page->Step[ARP_TRACK][j] );
		SOLO_assistant_page->attr_LEN = j;
	}
	else {

		SOLO_assistant_page->attr_LEN = MATRIX_NROF_COLUMNS;
	}
}



void playChordPitch( unsigned char in_pitch,
					 unsigned char in_channel,
					 unsigned char in_velocity,
					 unsigned char in_length
					){

	unsigned char pitch = (C3 + (OCTAVE * SOLO_scale_chords_octave) + in_pitch);

	if ( in_length == OFF ){

		MIDI_NOTE_new ( in_channel, pitch, in_velocity, 0 );
	}
	else {

		// NOTE OFF build
		MIDI_OFF_build_new( SOLO_midi_ch, pitch, in_length );

		// SEND NOTE ON
		MIDI_send(		MIDI_NOTE,
						SOLO_midi_ch,
						pitch,
						in_velocity );
	}
}



void playChord( unsigned char scale,
			    unsigned char tone,
			    unsigned char chord_id,
			    unsigned char attr_PIT,
			    unsigned char in_channel,
			    unsigned char in_velocity
			   ){
	int i;
	unsigned char pitches[MAX_NOTES];
	unsigned char hasChord = OFF;
	Chordstruct* chord ;
	SOLO_scale_chords_last = OFF; // reset

	translateSymbolsToChord(chords[scale][tone][chord_id], pitches);

	for (i=0; i < MAX_NOTES; i++){

		if (pitches[i] == 0xFF){
			break;
		}
		hasChord = ON;
		SET_BIT( SOLO_scale_chords_last, pitches[i] ); // show the chord keys in the grid
		playChordPitch( pitches[i] + attr_PIT, in_channel, in_velocity, OFF );
	}

	if ( hasChord && in_velocity != OFF ){

		SOLO_last_chord = &Chord_palette_repository[MAX_NROF_PALETTE_CHORDS]; // 13th chord is a buffer
		chord = SOLO_last_chord;
		chord->chord_id = chord_id;
		chord->octave = SOLO_scale_chords_octave;
		chord->pitch = attr_PIT;
		chord->scale = scale;
		chord->strum = 9; // TODO STRUM
		chord->tone = tone;
		chord->attr_VEL = in_velocity;

		SOLO_scale_chords_modulations = modulationsOfChord( chord_id, SOLO_assistant_page );
	}

	TEMPO_TIMER = ON;
	cyg_alarm_initialize(	alarm_hdl,
							cyg_current_time() + (TIMEOUT_VALUE / 2),
							0 );
}

unsigned char pitchToChordId( unsigned char in_pitch ){

	signed char idx = in_pitch - (C3 - (OCTAVE * 2)); // C1

	if ( idx < 0 || idx >= (OCTAVE * 7))
	{
		return NOP; // out of chord range
	}

	unsigned char arrayIndex = white_key_pitch_to_array_index[idx];
	unsigned char cid = idx - arrayIndex;
	unsigned char chord_id = cid + (SOLO_scale_chords_b * 35);

	if ((chord_id >= 35 && !SOLO_scale_chords_b ) ||
		( SOLO_scale_chords_b == ON && ( chord_id < 35 || chord_id >= 70 ))){

		return NOP; // split the A keyboard at chord 34
	}

	return chord_id;
}

void record_chord_to_track( Pagestruct* target_page,
							unsigned char row,
							unsigned char target_col,
							unsigned char target_start,
							unsigned char in_pitch,
							unsigned char in_velocity
						   ){

	int i;
	unsigned char pitches[MAX_NOTES];
	unsigned char chord_id = pitchToChordId(in_pitch);
	if ( chord_id == NOP ) return;
	unsigned char tone = toneToIndex( G_scale_ndx );
	unsigned char scale = currentScaleIndex( SOLO_assistant_page );
	unsigned char pitch = (C3 + (OCTAVE * SOLO_scale_chords_octave) + SOLO_assistant_page->attr_PIT);
 	unsigned char programOctave = ( MIDDLE_C - OCTAVE ) + ( OCTAVE * SOLO_scale_chords_program_octave );
 	unsigned char isProgramKey = ( in_pitch >= programOctave && in_pitch < ( programOctave + OCTAVE ) );

 	if ( isProgramKey ){

 		// palette chord
 		Chordstruct* chord = &Chord_palette_repository[SOLO_scale_chords_palette_ndx];
 		translateSymbolsToChord(chords[chord->scale][chord->tone][chord->chord_id], pitches);
 	}
 	else {
 		translateSymbolsToChord(chords[scale][tone][chord_id], pitches);
 	}

	for (i=0; i < MAX_NOTES; i++){

		if (pitches[i] == 0xFF){
			break;
		}

		record_note_to_track( target_page,
							  row,
							  target_col,
							  target_start,
							  (pitches[i] + pitch),
							  in_velocity );
	}
}

unsigned char record_chord_arp_to_track( Pagestruct* target_page,
								unsigned char row,
								unsigned char target_col
							   ){

	// Ignore non-record tracks
	if ( (Page_getTrackRecPattern(target_page) & (1 << row)) == 0 ) {
		return OFF;
	}

	unsigned int temp;
	unsigned char arpStep = SOLO_scale_chords_arp_cursor++ % SOLO_assistant_page->attr_LEN;

	Step_copy(SOLO_assistant_page->Step[ARP_TRACK][arpStep], target_page->Step[row][target_col], False);
	// default page pitch is middle C but solo_assist starts at zero
	target_page->Step[row][target_col]->attr_PIT -= ( MIDDLE_C - SOLO_assistant_page->attr_PIT /* apply the transpose offset */); // TODO: transpose
	temp = (target_page->Step[row][target_col]->attr_LEN * Track_LEN_factor[ SOLO_assistant_page->Track[ARP_TRACK]->LEN_factor ]) / 16;
	temp = normalize( temp, STEP_MIN_LENGTH, STEP_MAX_LENGTH );
	target_page->Step[row][target_col]->attr_LEN = (unsigned char) temp;

	capture_note_event(
			target_page->Step[row][target_col],
			target_page,
			row,
			target_col);

	return Step_get_status(target_page->Step[row][target_col] , STEPSTAT_TOGGLE);
}

void playNotesInChord( unsigned char in_channel,
					   unsigned char in_velocity,
					   unsigned char in_pitch
					  ){

	unsigned char chord_id = pitchToChordId(in_pitch);

	if ( chord_id == NOP ){
		return;
	}

	unsigned char tone = toneToIndex( G_scale_ndx );
	Pagestruct* target_page = SOLO_assistant_page;
	unsigned char scale = currentScaleIndex( target_page );

	if ( scale == NOP ){
		return;
	}

	playChord(scale, tone, chord_id, target_page->attr_PIT, in_channel, in_velocity);
}

void copyArpToSteps(Chordstruct* chord){
	unsigned char i;

	SOLO_last_chord = chord;
	SOLO_assistant_page->attr_LEN = MATRIX_NROF_COLUMNS -1;

	for (i=0; i < MATRIX_NROF_COLUMNS; i++){

		noteToStep(chord->Arp[i], SOLO_assistant_page->Step[ARP_TRACK][i]);
		if ( Note_get_status( chord->Arp[i], STEPSTAT_SKIP ) == ON ){
			SOLO_assistant_page->attr_LEN = i;
		}
	}
	SOLO_assistant_page->Track[ARP_TRACK]->LEN_factor = chord->attr_LEN;
}

void playChordstruct(unsigned char palette_ndx, unsigned char in_velocity, unsigned char in_channel, unsigned char play){

	int i;
	Chordstruct* chord = &Chord_palette_repository[palette_ndx];

	if ( SOLO_scale_chords_prev_palette_ndx != palette_ndx ){

		// we have changed palette keys so clear the history buffer
		for (i=0; i < MATRIX_NROF_COLUMNS; i++){

			Step_init(SOLO_assistant_page->Step[ARP_BUFFER_TRACK][i]);
		}
		SOLO_assistant_page->Track[ARP_BUFFER_TRACK]->attr_STATUS = OFF;
	}
	SOLO_scale_chords_prev_palette_ndx = palette_ndx;

	// play the arp if there is one
	for (i=0; i < MATRIX_NROF_COLUMNS; i++){

		if ( Note_get_status( chord->Arp[i], STEPSTAT_TOGGLE ) == ON ){ // has an arp

			if ( in_velocity == OFF ){

				if ( SOLO_scale_chords_prev_on_ndx == palette_ndx ){ // key released

					if ( G_track_rec_bit == OFF ){

						stop_solo_rec(OFF);
					}
					SOLO_scale_chords_arp_cursor = NOP;
				}
				SOLO_scale_chords_prev_on_ndx = palette_ndx;
			}
			else { // key pressed

				copyArpToSteps(chord);
				SOLO_scale_chords_prev_on_ndx = palette_ndx;

				if ( G_run_bit == OFF ){

					assignLastNotes();
					SOLO_rec_rehersal = ON;
					GRID_CURSOR = SOLO_assistant_page->pageNdx;
				}
				if ( play == ON ){

					if ( G_run_bit == OFF ){

						reset_page_cluster( SOLO_assistant_page );
						GRID_CURSOR = SOLO_assistant_page->pageNdx;
						sequencer_command_PLAY();
					}
					else {

						if ( G_track_rec_bit == OFF ){

							SOLO_assistant_page->locator = 0;
						}
						else {
							SOLO_scale_chords_arp_cursor = 0;
						}
					}
				}
			}
			return;
		}
	}

	if ( play == ON ){

		playChord(chord->scale, chord->tone, chord->chord_id, chord->pitch, in_channel, in_velocity);
	}
	else {
		assignLastNotes();
	}
}

void persistChordPalette(){
	int i, j;
	Chordstruct* chord;

	SOLO_arp_pattern_page->attr_mix2edit = ON; // flag to indicate that we have saved before
	SOLO_arp_pattern_page->attr_PIT = SOLO_scale_chords_pitch_recall;
	SOLO_arp_pattern_page->attr_STA = SOLO_scale_chords_pitch_prev;
	SOLO_arp_pattern_page->attr_LEN = (unsigned char) SOLO_scale_chords_octave;
	SOLO_arp_pattern_page->attr_VEL = (unsigned char) SOLO_scale_chords_program_octave;

	// save 0-9 to SOLO_arp_pattern_page and 10-11 to SOLO_assistant_page
	for (i=0; i < 10; i++){

		chord = &Chord_palette_repository[i];

		// assistant page overrides
		SOLO_arp_pattern_page->Track[i]->attr_VEL = chord->chord_id;
		SOLO_arp_pattern_page->Track[i]->attr_AMT = chord->scale;
		SOLO_arp_pattern_page->Track[i]->attr_PIT = chord->tone;
		SOLO_arp_pattern_page->Track[i]->attr_LEN = chord->octave;
		SOLO_arp_pattern_page->Track[i]->attr_STA = chord->pitch;
		SOLO_arp_pattern_page->Track[i]->attr_POS = chord->strum;
		SOLO_arp_pattern_page->Track[i]->attr_DIR = chord->attr_LEN;
		SOLO_arp_pattern_page->Track[i]->attr_MCC = chord->attr_VEL;

		for (j=0; j < MATRIX_NROF_COLUMNS; j++){

			noteToStep(chord->Arp[j], SOLO_arp_pattern_page->Step[i][j]);
		}
	}

	for (i=3; i < 5; i++){ // index 3 and 4

		chord = &Chord_palette_repository[i + 7];

		// assistant page overrides
		SOLO_assistant_page->Track[i]->attr_VEL = chord->chord_id;
		SOLO_assistant_page->Track[i]->attr_AMT = chord->scale;
		SOLO_assistant_page->Track[i]->attr_PIT = chord->tone;
		SOLO_assistant_page->Track[i]->attr_LEN = chord->octave;
		SOLO_assistant_page->Track[i]->attr_STA = chord->pitch;
		SOLO_assistant_page->Track[i]->attr_POS = chord->strum;
		SOLO_assistant_page->Track[i]->attr_DIR = chord->attr_LEN;
		SOLO_assistant_page->Track[i]->attr_MCC = chord->attr_VEL;

		for (j=0; j < MATRIX_NROF_COLUMNS; j++){

			noteToStep(chord->Arp[j], SOLO_assistant_page->Step[i][j]);
		}
	}
}

void restoreChordPalette(){
	int i, j;
	Chordstruct* chord;

	initAssistantPage();

	// this is is a previously saved page
	if ( SOLO_arp_pattern_page->attr_mix2edit == ON ){

		SOLO_scale_chords_pitch_recall = SOLO_arp_pattern_page->attr_PIT;
		SOLO_scale_chords_pitch_prev = SOLO_arp_pattern_page->attr_STA;
		SOLO_scale_chords_octave = (signed char) SOLO_arp_pattern_page->attr_LEN;
		SOLO_scale_chords_program_octave = (signed char) SOLO_arp_pattern_page->attr_VEL;
	}

	// save 0-9 to SOLO_arp_pattern_page and 10-11 to SOLO_assistant_page
	for (i=0; i < 10; i++){

		chord = &Chord_palette_repository[i];
		initChord(chord, i);
		// assistant page overrides
		chord->chord_id = SOLO_arp_pattern_page->Track[i]->attr_VEL;

		if ( chord->chord_id == NOP || chord->chord_id == 64 /* legacy default */ ){

			initChord(chord, i);
			continue;
		}

		chord->scale = SOLO_arp_pattern_page->Track[i]->attr_AMT;
		chord->tone = SOLO_arp_pattern_page->Track[i]->attr_PIT;
		chord->octave = SOLO_arp_pattern_page->Track[i]->attr_LEN;
		chord->pitch = SOLO_arp_pattern_page->Track[i]->attr_STA;
		chord->strum = SOLO_arp_pattern_page->Track[i]->attr_POS;
		chord->attr_LEN = SOLO_arp_pattern_page->Track[i]->attr_DIR;
		chord->attr_VEL = SOLO_arp_pattern_page->Track[i]->attr_MCC;

		for (j=0; j < MATRIX_NROF_COLUMNS; j++){

			stepToNote(SOLO_arp_pattern_page->Step[i][j], chord->Arp[j]);
		}
		SOLO_has_scale = ON;
	}

	for (i=3; i < 5; i++){ // index 3 and 4

		chord = &Chord_palette_repository[i + 7];
		initChord(chord, i);
		// assistant page overrides
		chord->chord_id = SOLO_assistant_page->Track[i]->attr_VEL;

		if ( chord->chord_id == NOP || chord->chord_id == 64 /* legacy default */ ){

			initChord(chord, i);
			continue;
		}

		chord->scale = SOLO_assistant_page->Track[i]->attr_AMT;
		chord->tone = SOLO_assistant_page->Track[i]->attr_PIT;
		chord->octave = SOLO_assistant_page->Track[i]->attr_LEN;
		chord->pitch = SOLO_assistant_page->Track[i]->attr_STA;
		chord->strum = SOLO_assistant_page->Track[i]->attr_POS;
		chord->attr_LEN = SOLO_assistant_page->Track[i]->attr_DIR;
		chord->attr_VEL = SOLO_assistant_page->Track[i]->attr_MCC;

		for (j=0; j < MATRIX_NROF_COLUMNS; j++){

			stepToNote(SOLO_assistant_page->Step[i][j], chord->Arp[j]);
		}
		SOLO_has_scale = ON;
	}
}
