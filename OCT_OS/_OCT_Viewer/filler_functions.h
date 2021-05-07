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



extern char* ptr_of_step_attr_value( 	Pagestruct* target_page,
										unsigned char row,
										unsigned char col,
										unsigned char artibute );




//
// Writes a bitpattern to the MIR, per default in green - used for step toggle
void MIR_write_trackpattern( 	unsigned int bitpattern,
								unsigned char track,
								unsigned char color) {
	// int i=0;
	// Write the LHS column
	MIR[LHS][track][color] = mirror (bitpattern >> 8, 8);
	// Write the RHS
	MIR[RHS][track][color] = mirror (bitpattern, 8);
}


// Writes a bitpattern to the MIR, in BLINK - used for step selection display
void MIR_blink_trackpattern(	unsigned int bitpattern,
								unsigned int track) {
	// int i=0;
	// Write the LHS column
	MIR[LHS][track][MIR_BLINK] |= mirror (bitpattern >> 8, 8);
	// Write the RHS
	MIR[RHS][track][MIR_BLINK] |= mirror (bitpattern, 8);
}


// Makes a row in the matrix blink - in the region between start and length
void MIR_blink_track (			unsigned int track,
								unsigned char start,
								unsigned char length) {
	// Compute pattern: length and start
	unsigned int blinkpattern = (0xffff >> (16-length) ) << (16-start-length + 1);

	// Write the LHS column
	MIR[LHS][track][MIR_BLINK] = mirror (blinkpattern >> 8, 8);
	// Write the RHS
	MIR[RHS][track][MIR_BLINK] = mirror (blinkpattern, 8);
}


// Writes additional pattern to the MIR - used for track boundaries and selected empty tracks
void MIR_augment_trackpattern(	unsigned int bitpattern,
								unsigned int track,
								unsigned char color) {
	// Write the LHS columns
	MIR[LHS][track][color] 		|= mirror (bitpattern >> 8, 8);
	// Write the RHS columns
	MIR[RHS][track][color] 		|= mirror (bitpattern, 8);

}



// Writes single dots to the GRID structure. Use this instead of the MIR when writing
// to the GRID
void GRID_write_dot ( 			unsigned int ndx,
								unsigned char color){

	unsigned int row=0, col=0, side=LHS;

	//Compute row
	row = ndx % 10;

	//Compute column
	col = ndx / 10;

	// Transform to MIR coordinates
	if (col > 7) {
		side = RHS;
		col  = col - 8;
	}

	// Write to MIR
	Page_GRID[side][row][color]  |= 1 << col;
}


// Writes the mutepattern of the given page
void GRID_write_mutepattern(	Pagestruct* target_page, unsigned char target_row ){

	unsigned char row = target_page->pageNdx % 10;

	// OFF tracks
	MIR_write_trackpattern(
		target_page->trackMutepattern << 3, target_row, MIR_RED   );

	// ON tracks
	MIR_write_trackpattern(
		( target_page->trackMutepattern ^ 0x3ff ) << 3, target_row, MIR_GREEN );

	// SELECTED tracks
	MIR_write_trackpattern(
		target_page->trackSelection << 3, target_row, MIR_BLINK   );

	// SOLO tracks
	MIR_augment_trackpattern(
		target_page->trackSolopattern << 3, target_row, MIR_RED   );

	if ( 	( CHECK_BIT( G_on_the_measure_mod_bit, row ) )
		&&	( G_on_the_measure_pattern_pageNdx[row] == target_page->pageNdx ) ) {

		if( CHECK_BIT( G_on_the_measure_operation[row], OPERATION_MUTE ) ) {
			unsigned short mutePattern = ( G_on_the_measure_pattern[row][OPERATION_MUTE] ^ target_page->trackMutepattern );
			MIR_blink_trackpattern( mutePattern << 3, target_row );
		}

		if( CHECK_BIT( G_on_the_measure_operation[row], OPERATION_SOLO ) ) {
			unsigned short soloPattern = ( G_on_the_measure_pattern[row][OPERATION_SOLO] ^ target_page->trackSolopattern );
			MIR_blink_trackpattern( soloPattern << 3, target_row );
			MIR_augment_trackpattern( target_page->trackSolopattern << 3, target_row, MIR_GREEN );
			MIR_augment_trackpattern( target_page->trackSolopattern << 3, target_row, MIR_RED );
		}
	}
}



// Writes a single dot in the MIR, at the given button index, color state (red, green, blink)
void MIR_write_dot( 			unsigned int button_ndx,
								unsigned char color) {
	unsigned char col =0;
	unsigned char row =0;
	unsigned char side = LHS;

	// This is a button in the left button tool
	if ( (button_ndx > 0 ) && (button_ndx <=5 ) ){
		MIR[LHS][11][color] |= 1 <<	(5-button_ndx);
	}
	else if ( (button_ndx > 5 ) && (button_ndx <=10) ){
		MIR[LHS][12][color] |= 1 <<	(11-button_ndx);
	}

	// This is a button in the matrix range
	else if ( (button_ndx > 10) && (button_ndx <= 185) ) {
		switch (button_ndx) {
			// Exempt utility buttons from processing
			case 21:
			case 32:
			case 43:
			case 54:
			case 65:
			case 76:
			case 87:
			case 98:
			case 109:
			case 120:
			case 131:
			case 142:
			case 153:
			case 164:
			case 175:
				break;
			default:
				// Absolute coordinates
				row = row_of    (button_ndx);
				col = column_of (button_ndx);

				// Transform to MIR coordinates
				if (col > 7) {
					side = RHS;
					col  = col - 8;
				}

				// Write to MIR
				MIR[side][row][color]  |= 1 << col;
				/* no break */
		}
	}


	// These are utility buttons
	switch (button_ndx) {
		// Rot Hndl RHS : bit 0
		case  21:	MIR[LHS][10][color] |= 1<<0;	break;

		// But Hndl LHS	: bit 1
		case  32:	MIR[LHS][10][color] |= 1<<1;	break;

		// Other.. (??)
		case  43:	MIR[LHS][10][color] |= 1<<2;	break;

		// Other.. (??)
		case  54:	MIR[LHS][10][color] |= 1<<3;	break;
		case  65:	MIR[LHS][10][color] |= 1<<4;	break;
		case  76:	MIR[LHS][10][color] |= 1<<5;	break;
		case  87:	MIR[LHS][10][color] |= 1<<6;	break;
		case  98:	MIR[LHS][10][color] |= 1<<7;	break;
		case 109:	MIR[RHS][10][color] |= 1<<0;	break;
		case 120:	MIR[RHS][10][color] |= 1<<1;	break;
		case 131:	MIR[RHS][10][color] |= 1<<2;	break;
		case 142:	MIR[RHS][10][color] |= 1<<3;	break;

		case 153:
			MIR[RHS][10][color] |= 1<<4;
			break;

		case 164:
			MIR[RHS][10][color] |= 1<<5;
			break;

		case 175:
			MIR[RHS][10][color] |= 1<<6;
			break;

		case 186:
			MIR[RHS][10][color] |= 1<<7;
			break;

		case 187:
			MIR[RHS][11][color] |= 1<<1;
			break;

		case 188:
			MIR[RHS][11][color] |= 1<<2;
			break;

		case 189:
			MIR[RHS][11][color] |= 1<<3;
			break;

		case 190:
			MIR[RHS][11][color] |= 1<<4;
			break;

		case 191:
			MIR[RHS][11][color] |= 1<<5;
			break;

		case 192:
			MIR[RHS][12][color] |= 1<<0;
			break;

		case 193:
			MIR[RHS][12][color] |= 1<<1;
			break;

		case 194:
			MIR[RHS][12][color] |= 1<<2;
			break;

		case 195:
			MIR[RHS][12][color] |= 1<<3;
			break;

		case 196:
			MIR[RHS][12][color] |= 1<<4;
			break;

		//
		// 	CIRCLE
		//
		// CIRCLE - QUADRANT: OUTTER TOP LEFT
		case 215:	MIR[LHS][13][color] |= 1<<0;	break;
		case 216:	MIR[LHS][13][color] |= 1<<1;	break;
		case 206:	MIR[LHS][13][color] |= 1<<2;	break;
		case 207:	MIR[LHS][13][color] |= 1<<3;	break;
		case 197:	MIR[LHS][13][color] |= 1<<4;	break;
		case 198:	MIR[LHS][13][color] |= 1<<5;	break;
		case 199:	MIR[LHS][13][color] |= 1<<6;	break;
		case 200:	MIR[LHS][13][color] |= 1<<7;	break;

		// CIRCLE - QUADRANT: OUTTER BOTTOM LEFT
		case 201:	MIR[LHS][14][color] |= 1<<0;	break;
		case 202:	MIR[LHS][14][color] |= 1<<1;	break;
		case 203:	MIR[LHS][14][color]	|= 1<<2;	break;
		case 204:	MIR[LHS][14][color] |= 1<<3;	break;
		case 205:	MIR[LHS][14][color] |= 1<<4;	break;
		case 213:	MIR[LHS][14][color] |= 1<<5;	break;
		case 214:	MIR[LHS][14][color] |= 1<<6;	break;
		case 223:	MIR[LHS][14][color] |= 1<<7;	break;

		// CIRCLE - QUADRANT: OUTTER BOTTOM RIGHT
		case 231:	MIR[RHS][14][color] |= 1<<0;	break;
		case 232:	MIR[RHS][14][color] |= 1<<1;	break;
		case 241:	MIR[RHS][14][color] |= 1<<2;	break;
		case 240:	MIR[RHS][14][color] |= 1<<3;	break;
		case 250:	MIR[RHS][14][color] |= 1<<4;	break;
		case 249:	MIR[RHS][14][color] |= 1<<5;	break;
		case 248:	MIR[RHS][14][color] |= 1<<6;	break;
		case 247:	MIR[RHS][14][color] |= 1<<7;	break;

		// CIRCLE - QUADRANT: OUTTER TOP RIGHT
		case 246:	MIR[RHS][13][color] |= 1<<0;	break;
		case 245:	MIR[RHS][13][color]	|= 1<<1;	break;
		case 244:	MIR[RHS][13][color] |= 1<<2;	break;
		case 243:	MIR[RHS][13][color] |= 1<<3;	break;
		case 242:	MIR[RHS][13][color] |= 1<<4;	break;
		case 234:	MIR[RHS][13][color] |= 1<<5;	break;
		case 233:	MIR[RHS][13][color] |= 1<<6;	break;
		case 224:	MIR[RHS][13][color] |= 1<<7;	break;

		// CIRCLE - QUADRANT: INNER LEFT
		case 225:	MIR[LHS][15][color] |= 1<<0;	break;
		case 217:	MIR[LHS][15][color] |= 1<<1;	break;
		case 208:	MIR[LHS][15][color] |= 1<<2;	break;
		case 209:	MIR[LHS][15][color] |= 1<<3;	break;
		case 210:	MIR[LHS][15][color] |= 1<<4;	break;
		case 211:	MIR[LHS][15][color] |= 1<<5;	break;
		case 212:	MIR[LHS][15][color] |= 1<<6;	break;
		case 221:	MIR[LHS][15][color] |= 1<<7;	break;

		// CIRCLE - QUADRANT: INNER RIGHT
		case 222:	MIR[RHS][15][color] |= 1<<0;	break;
		case 230:	MIR[RHS][15][color] |= 1<<1;	break;
		case 239:	MIR[RHS][15][color] |= 1<<2;	break;
		case 238:	MIR[RHS][15][color] |= 1<<3;	break;
		case 237:	MIR[RHS][15][color] |= 1<<4;	break;
		case 236:	MIR[RHS][15][color] |= 1<<5;	break;
		case 235:	MIR[RHS][15][color] |= 1<<6;	break;
		case 226:	MIR[RHS][15][color] |= 1<<7;	break;

		// CIRCLE - CORE
		case 218:	MIR[LHS][16][color] |= 1<<0;	break;
		case 219:	MIR[LHS][16][color] |= 1<<1;	break;
		case 227:	MIR[LHS][16][color] |= 1<<2;	break;
		case 228:	MIR[LHS][16][color] |= 1<<3;	break;
		case 220:	MIR[LHS][16][color] |= 1<<4;	break;
		case 229:	MIR[LHS][16][color] |= 1<<5;	break;

		//
		// SIDE BOW
		//
		case 258:	MIR[RHS][16][color] |= 1<<0;	break;
		case 257:	MIR[RHS][16][color] |= 1<<1;	break;
		case 256:	MIR[RHS][16][color] |= 1<<2;	break;
		case 255:	MIR[RHS][16][color] |= 1<<3;	break;
		case 254:	MIR[RHS][16][color] |= 1<<4;	break;
		case 253:	MIR[RHS][16][color] |= 1<<5;	break;
		case 252:	MIR[RHS][16][color] |= 1<<6;	break;
		case 251:	MIR[RHS][16][color] |= 1<<7;	break;

		//
		// ROTARY INDICATORS
		//
		case 259:	MIR[RHS][12][color] |= 1<<5;	break;
		case 260:	MIR[LHS][12][color] |= 1<<0;	break;

	}
}




// Writes the lauflicht of trackNdx into row with rowNdx - used in Track editing etc..
void MIR_write_lauflicht_track( unsigned char trackNdx, unsigned char rowNdx ) {

	unsigned char side = 0;
	unsigned char offset = 0;
	unsigned char locator = 0;

	// Work on the page under the grid cursor
	Pagestruct* target_page = &Page_repository[GRID_CURSOR];

	// Write the locator to MIR for the target track
	locator = target_page->Track[trackNdx]->attr_LOCATOR;

	// Compute the MIR Side
	if (locator < 9) {

		side 	= LHS;
		offset 	= 1;
	}
	else {

		side 	= RHS;
		offset 	= 9;
	}

	// If there is only a RED bit set, turn it off.
	if ( 	(( MIR[side][rowNdx][MIR_RED] 	& (1 << (locator - offset)) ) >  0)
		&&	(( MIR[side][rowNdx][MIR_GREEN] & (1 << (locator - offset)) ) == 0)
		){

		// Turn off RED bit in MIR, and BLINK too since we're at it anyways..
		MIR[side][rowNdx][MIR_RED] 		&= 0xff ^ (1 << (locator - offset));
		MIR[side][rowNdx][MIR_BLINK] 	&= 0xff ^ (1 << (locator - offset));
	}
	else {

		// This locator needs to be more prevalent and needs to be single RED
		MIR[side][rowNdx][MIR_RED] 		|= (1 << (locator - offset));

		// Turn off all BLINK and GREEN bits in the MIR
		MIR[side][rowNdx][MIR_BLINK] 	&= 0xff ^ (1 << (locator - offset));
		MIR[side][rowNdx][MIR_GREEN] 	&= 0xff ^ (1 << (locator - offset));
	}
}


// Indicates track locators in every matrix row
void MIR_write_lauflicht () {

	unsigned char side = 0;
	unsigned char locator = 0;
	unsigned char i = 0;
	unsigned char offset=0;

	// Work on the page under the grid cursor
	Pagestruct* target_page = &Page_repository[GRID_CURSOR];

	// Iterate through the tracks and write their locator to the MIR
	for (i=0; i<MATRIX_NROF_ROWS; i++) {

		// Simplify and account for the natural offset of 1
		locator = target_page->Track[i]->attr_LOCATOR;

		// Don't bother at all with non-playing tracks
		if ( locator == 0 )	continue;

		// Compute the MIR Side and the offset
		if (locator < 9) {

			side 	= LHS;
			offset 	= 1;
		}
		else {

			side 	= RHS;
			offset 	= 9;
		}

		// Clear the blink and green bit at lauflicht position
		MIR[side][i][MIR_GREEN]		&= ( 0xff ^ (1 << (locator - offset)) );
		MIR[side][i][MIR_BLINK]		&= ( 0xff ^ (1 << (locator - offset)) );

		// Set the red bit on top of what's there.
		MIR[side][i][MIR_RED] 		|= (1 << (locator - offset));

		// If the track is sending MIDI CCs the lauflicht is orange, so add green!
		if ( target_page->Track[i]->attr_MCC != MIDICC_NONE ){
			MIR[side][i][MIR_GREEN] 	|= (1 << (locator - offset));
		}

	} // Row iterator

}



// Writes to LHS or RHS button tool the first 10 bits of bitpattern. First bit is top
void MIR_write_buttool (			unsigned char side,
									unsigned int in_bitpattern,
									unsigned char color) {
	// 10 bits of bitpattern
	unsigned int bitpattern = 0x3ff & in_bitpattern;

	// Split in top and bottom chunks, 5 bits each
	unsigned char topchunk = 0x1F & bitpattern;
	unsigned char bottomchunk = bitpattern >> 5;

	// Adjust the patterns according to their side
	switch (side) {

		case (LHS) :
			// Invert the chunks
			topchunk = mirror (topchunk, 5);
			bottomchunk = (mirror (bottomchunk, 5)) << 1;
			break;

		case (RHS) :
			// Overpass the scroll led
			topchunk = topchunk << 1;
			break;
	}

	// Write to the MIR
	MIR[side][11][color] |= topchunk;
	MIR[side][12][color] |= bottomchunk;
}



// Writes an orange dot into the given matrix row pointing to a value between 1 and 16
void MIR_point_numeric (		unsigned char number,
								unsigned char target_row,
								unsigned char color) {
	unsigned char side = number / 9; // LHS:0 RHS:1
	unsigned char bitpattern = 0;

	// Only makes sense for non-zero numbers
	if (number > 0) {
		if (number < 9) {
			bitpattern = 1 << (number-1);
		}
		else {
			bitpattern = 1 << (number-9);
		}
		// Show the dot in the wanted color
		MIR[side][target_row][color] |= bitpattern;
	}
}



// Writes a bar on the target row, with length of number
void MIR_fill_numeric (	unsigned char start,
						unsigned char number,
						unsigned char target_row,
						char color) {

	unsigned char i=0;

	for ( i = start; i < number; i ++ ) {

		MIR_point_numeric (i, target_row, color);
	}
}



// Show the start value of step as horizontal bar. The other way to show it is vertical - in map mode
void MIR_show_startbar_H( unsigned char number, unsigned char target_row ){

	// The step is symbolysed by an orange dot. A red or green bar indicates start shift

	// PUSHED STEP:
	if ( number > (OCTDEF_TTC_PER_16TH/2) ){
		// The orange dot
//		MIR_point_numeric	( number - (OCTDEF_TTC_PER_16TH/2 -1), target_row, MIR_RED	);
//		MIR_point_numeric	( number - (OCTDEF_TTC_PER_16TH/2 -1), target_row, MIR_GREEN	);

		// The RED bar
		MIR_fill_numeric	( 1, number - (OCTDEF_TTC_PER_16TH/2 -1), target_row, MIR_RED );
	}

	// PULLED STEP:
	else if ( number < (OCTDEF_TTC_PER_16TH/2) ){
		// The orange dot
//		MIR_point_numeric	( number+11, target_row, MIR_RED	);
//		MIR_point_numeric	( number+11, target_row, MIR_GREEN	);

		// The GREEN bar
		MIR_fill_numeric( number+11, MATRIX_NROF_COLUMNS+1, target_row, MIR_GREEN );
	}

	// STRAIGHT STEP:
	// ..LED is OFF
}



// Writes a dot into the given matrix row pointing to a value between 1 and 16
void MIR_peek_numeric (unsigned char number, unsigned char target_row) {
	unsigned char side = number / 9; // LHS:0 RHS:1
	unsigned char bitpattern = 0;

	if (number < 9) {
		bitpattern = 1 << (number-1);
	}
	else {
		bitpattern = 1 << (number-9);
	}

	// Show a dot
	MIR[side][target_row][MIR_RED] 		|= bitpattern;
	MIR[side][target_row][MIR_GREEN] 	|= bitpattern;
}



// Writes the dot of a number between 1 and 9 into the circle nueric field
void MIR_write_dot_C( unsigned char number, unsigned char color ){

	if ((number > 0) || (number < 10)){

		MIR_write_dot( circleNrField[number], color 	);
	}
}





// Writes a factor (range 0-16) into the numeric circle quadrant
void MIR_write_factor_C ( unsigned char number ){

	unsigned char i = 0;

	// Do not take invalid input
	if ( number > 16 ) return;

	// Do not overlap with other things shown in the circle
	if ( is_pressed_rowzero() != 0 ) return;

	if ( number == 8 ){

		MIR_write_dot( circleNrField[0], MIR_RED );
		MIR_write_dot( circleNrField[0], MIR_GREEN );
	}

	else if ( number > 8 ){

		// Write the red balken
		for ( i=0; i < 1 + number-8; i++ ){

			MIR_write_dot( circleNrField[i], MIR_RED );
		}
		// Point the orange dot
		MIR_write_dot( circleNrField[i-1], MIR_GREEN );
	}

	else if ( number < 8 ){

		// Write the first green LED
		MIR_write_dot( circleNrField[8], MIR_GREEN );

		// Write the green balken
		for ( i=0; i < 8-number; i++ ){

			MIR_write_dot( circleNrField[7-i], MIR_GREEN );
		}
		// Point the orange dot
		MIR_write_dot( circleNrField[7-i+1], MIR_RED );
	}

//	// 16 case
//	if ( number == 16 ){
//
//		// Show the upper 5 LEDs in red
//		for ( i=0; i < 9; i++ ){
//			MIR_write_dot( circleNrField[i], MIR_RED );
//		}
//	}
//
//	// 0 case
//	else if ( number == 0 ){
//
//		// Show the lower 5 leds in green
//		for ( i=0; i < 9; i++ ){
//
//			MIR_write_dot( circleNrField[i], MIR_GREEN );
//		}
//	}
//
//	else{
//
//		// Write tip higher than 5 in RED
//		if ( number > 8 ){
//
//			// Top half
//			for ( i=5; i < (5 + (number-8)); i++ ){
//				MIR_write_dot( circleNrField[i], MIR_RED );
//				if ( i > 9 ) break;
//			}
//
//			// Bottom half
//			if ( number > 12 ){
//				for ( i=0; i < number-12; i++ ){
//					MIR_write_dot( circleNrField[i], MIR_RED );
//				}
//			}
//		}
//		else if ( number < 8 ){
//
//			// Top half
//			for ( i=(-number); i < 4; i++ ){
//				MIR_write_dot( circleNrField[i], MIR_GREEN );
//			}
//			// Bottom half
//			for ( i=number-1; i < 4; i++ ){
//				MIR_write_dot( circleNrField[i], MIR_GREEN );
//			}
//		}
//	}
}





// Writes a factor (range 0-16) into the numeric circle quadrant
void MIR_write_factor_C_old ( unsigned char number ){

	unsigned char i = 0;

	// Do not take invalid input
	if ( number > 10 ) return;

	// Do not overlap with other things shown in the circle
	if ( is_pressed_rowzero() != 0 ) return;


	// 10 case
	else if ( number == 10 ){

		// Show the upper 5 LEDs in red
		for ( i=4; i < 9; i++ ){
			MIR_write_dot( circleNrField[i], MIR_RED );
		}
	}

	// 0 case
	else if ( number == 0 ){

		// Show the lower 5 leds in green
		for ( i=0; i < 5; i++ ){
			MIR_write_dot( circleNrField[i], MIR_GREEN );
		}
	}

	else{
		// Number 5 is always Orange - floating line
		MIR_write_dot( circleNrField[4], MIR_GREEN );
		MIR_write_dot( circleNrField[4], MIR_RED );

		// Write tip higher than 5 in RED
		if ( number > 5 ){

			for ( i=5; i < number; i++ ){
				MIR_write_dot( circleNrField[i], MIR_RED );
			}
		}
		else if ( number < 5 ){

			for ( i=number-1; i < 4; i++ ){
				MIR_write_dot( circleNrField[i], MIR_GREEN );
			}
		}
	}
}



// Writes a number into the Circle at the position determined by the circle number field
void MIR_write_numeric_C (unsigned char number) {

	unsigned char i = 0;
	unsigned char hundreds 	= number / 100;
	unsigned char tens 		= (number /  10) % 10;
	unsigned char ones 		= number %  10;

	unsigned char tens_color 	= MIR_RED;
	unsigned char ones_color	= MIR_GREEN;

	// Cover for the -1 case
	if ( ((signed char) number) == -1 ){

		MIR_write_dot( circleNrField[10], 	ones_color );
		MIR_write_dot( circleNrField[9], 	ones_color );
		MIR_write_dot( circleNrField[8], 	ones_color );
		MIR_write_dot( circleNrField[7], 	ones_color );
		return;
	}


	// circleNrField keeps the 10 indexes of the keys to be activated
	// first fill the circleNrField
	// then map the circleNrField onto the MIR

	// HUNDREDS
	if (hundreds > 0) {
		// Light the 10 button / The 9er button orange
		MIR_write_dot( circleNrField[9], tens_color );
	}
	if (hundreds > 1) {
		MIR_write_dot( circleNrField[10], tens_color );
	}

	// TENS
	for (i=0; i<tens; i++) {
		if (i != (ones-1)) {
			MIR_write_dot( circleNrField[i], tens_color );
		}
		if ( (ones) == tens ){
			MIR_write_dot( circleNrField[i], tens_color );
		}
	}

	// ONES
	if (ones > 0){
		MIR_write_dot( circleNrField[ones-1], ones_color );
	}
}


// Writes a step length into the given matrix row (Horizontal)
void MIR_write_length_H( unsigned char length, unsigned char target_row ) {
	MIR_write_base12_H( length, target_row );
}



// Writes a signed base-12 value into the given matrix row (Horizontal)
void MIR_write_base12_H( int value, unsigned char target_row ) {

	unsigned char fullsteps_color	= MIR_RED;
	unsigned char gate_color		= MIR_GREEN;
	unsigned char fullsteps			= 0;
	unsigned char gate				= 0;
	unsigned char bitpattern		= 0;
	unsigned char sign				= POS;

	// Determine the sign of the pitch to be written and the abs value of pitch
	if ( value < 0 ) {
		sign = NEG;
		value = -value;
	}


	// Compute the 16ths and the gate to be displayed
	fullsteps 	= value / 12;
	gate 		= value % 12;

	// Left side: should be always the case as max step length is 8/16ths.
	bitpattern = (0xFF >> (8 - normalize( fullsteps, 0, 8 )));
	MIR[LHS][target_row][fullsteps_color] |= bitpattern;
	if ( sign == NEG ) MIR[RHS][target_row][MIR_GREEN] |= 0xE0;

	// Right side
	if ( fullsteps > 8 ){
		// This should be always the case as max step length is > 8/16ths.
		bitpattern = (0xFF >> (16 - fullsteps));
		MIR[RHS][target_row][fullsteps_color] |= bitpattern;
		if ( sign == NEG ) MIR[RHS][target_row][MIR_GREEN] |= 0xE0;
	}

	// GATE dot
	if (gate <= 8) {
		// Conventional bitpattern computation
		bitpattern = mirror ( 1 << (8-gate), 8 );

		// Turn off the note dot in the tens bar
		if (gate != fullsteps ){
			MIR[LHS][target_row][fullsteps_color] &= ( 0xFF ^ (1 << (gate-1)) );
			if ( sign == NEG ) MIR[RHS][target_row][MIR_GREEN] |= 0xE0;
		}

		// Write the gate dot
		MIR[LHS][target_row][gate_color] |= bitpattern;
		if ( sign == NEG ) MIR[RHS][target_row][MIR_GREEN] |= 0xE0;
	}

	if (gate >= 9) {

		bitpattern = mirror ( 1 << (16-gate), 8);

		// Turn off the gate dot in the 16ths bar
		if (gate != fullsteps ){
			MIR[RHS][target_row][fullsteps_color] &= ( 0xFF ^ (1 << (gate-8-1)) );
			if ( sign == NEG ) MIR[RHS][target_row][MIR_GREEN] |= 0xE0;
		}

		// Write the gate dot
		MIR[RHS][target_row][gate_color] |= bitpattern;
		if ( sign == NEG ) MIR[RHS][target_row][MIR_GREEN] |= 0xE0;
	}
}



// Writes a pitch into the Octave Circle using the following representation:
// GREEN bar shows how many octaves the pitch is at
// RED LED shows the note in the scale corresponding to the top line
void MIR_write_pitch_Circle( unsigned char pitch ){

}



// Writes a pitch into the given matrix row (Horizontal) using the following
// representation:
// GREEN bar shows how many octaves the pitch is at
// RED LED shows the note in the scale corresponding to the top line
void MIR_write_pitch_H( signed char pitch, unsigned char target_row ) {

	unsigned char octaves_color 	= MIR_RED;
	unsigned char note_color		= MIR_GREEN;
	unsigned char octaves 			= 0;
	unsigned char note 				= 0;
	unsigned char bitpattern 		= 0;

	unsigned char pitch_sign		= POS;

	// Determine the sign of the pitch to be written and the abs value of pitch
	if ( pitch < 0 ) {

		pitch_sign = NEG;
		pitch = abs( pitch ) - 1;
	}

	// Compute the note and its octaves to be displayed
	octaves = pitch / 12;

	switch( pitch_sign ){

		case POS:
			// The +3 offset is due to panel print layout(note labes and such)
			note 	= (pitch % 12) + 3;
			break;

		case NEG:
			note 	= 14 - (pitch % 12);
			break;
	}

	switch( pitch_sign ){

		case POS:
			// OCTAVES bar:
			if (octaves <= 8) {

				// This is always the case, so do it always.
				bitpattern = (0xFF >> (8 - octaves));
				MIR[LHS][target_row][octaves_color] |= bitpattern;
			}

			if (octaves >= 9) {

				// Fill the LHS first
				MIR[LHS][target_row][octaves_color] |= 0xFF;

				// Append the RHS bits
				bitpattern = (0xFF >> (16 - octaves));
				MIR[RHS][target_row][octaves_color] |= bitpattern;
			}
			break;

		case NEG:
			// Offset the octaves value
			octaves ++;

			// OCTAVES bar:
			if (octaves <= 8) {

				// This is always the case, so do it always.
				bitpattern = (0xFF << (8 - octaves));
				MIR[RHS][target_row][octaves_color] |= bitpattern;
			}

			if (octaves >= 9) {

				// Fill the LHS first
				MIR[RHS][target_row][octaves_color] |= 0xFF;

				// Append the RHS bits
				bitpattern = (0xFF << (16 - octaves));
				MIR[LHS][target_row][octaves_color] |= bitpattern;
			}
			break;
	}

	// Clear the bitpattern so it can be used again
	bitpattern = 0;


	// NOTES dot
	switch( pitch_sign ){

		case POS:
			if (note <= 8) {
				// Conventional bitpattern computation
				bitpattern = mirror ( 1 << (8-note), 8 );

				// Turn off the note dot in the tens bar
				if (note != octaves ){
					MIR[LHS][target_row][octaves_color] &=
						( 0xFF ^ (1 << (note-1)) );
				}
			}

			// Write the ones dot
			MIR[LHS][target_row][note_color] |= bitpattern;

			if (note >= 9) {

				bitpattern = mirror ( 1 << (16-note), 8);
				// Turn off the ones dot in the tens bar
				if (note != octaves ){
					MIR[RHS][target_row][octaves_color] &=
						( 0xFF ^ (1 << (note-8 -1) ));
				}

				MIR[RHS][target_row][note_color] |= bitpattern;
			}
			break;

		case NEG:
			if (note <= 8) {
				// Conventional bitpattern computation
				bitpattern = mirror ( 1 << (8-note), 8 );

				// Turn off the note dot in the tens bar
				if (note != (octaves-1) ){
					MIR[LHS][target_row][octaves_color] &=
						( 0xFF ^ (1 << (note-1)) );
				}
			}

			// Write the ones dot
			MIR[LHS][target_row][note_color] |= bitpattern;

			if (note >= 9) {

				bitpattern = mirror ( 1 << (16-note), 8);
				// Turn off the ones dot in the tens bar
				if ((16-note) != (octaves-1) ){
//					MIR[LHS][target_row][octaves_color] &=
					MIR[RHS][target_row][octaves_color] &=
						( 0xFF ^ (1 << (note-8 -1)) );
				}

				MIR[RHS][target_row][note_color] |= bitpattern;
			}
			break;
	}
}



// Writes a number into the given matrix row (Horizontal) using decimal
// representation over 10 LEDS. Numbers between -99 and +99
// This version is taken over from Nemo - the representation of negative values is different.
void MIR_write_numeric_H( signed short number, unsigned char target_row ) {

	unsigned char bitpattern 	= 0;
	unsigned char tens 			= 0;
	unsigned char ones 			= 0;
	unsigned char tens_color 	= MIR_RED;
	unsigned char ones_color	= MIR_GREEN;
	unsigned char value_sign	= POS;

	// Note negative values
	if ( number < 0 ) {
		value_sign = NEG;
		number = abs( number );
	}

	// Compute base components
	tens 		= number /  10;
	ones 		= number %  10;

	// MIR_RED content - The tens bar
	if (tens <= 8) {
		bitpattern = (0xFF >> (8 - tens) );
		MIR[LHS][target_row][tens_color] |= bitpattern;

		if ( value_sign == NEG ){
			// Light the last 3 LEDs green
			MIR[RHS][target_row][MIR_GREEN] |= 0xE0;
		}
	}

	if (tens >= 9) {
		// Fill the LHS completely
		MIR[LHS][target_row][tens_color] |= 0xFF;

		if ( value_sign == NEG ){
			// Light the last LED green
			MIR[RHS][target_row][MIR_GREEN] |= 0xE0;
		}

		// Append the RHS bits
		bitpattern = (0xFF >> (16 - tens) );
		MIR[RHS][target_row][tens_color] |= bitpattern;

		if ( value_sign == NEG ){
			// Light the last LED green
			MIR[RHS][target_row][MIR_GREEN] |= 0xE0;
		}
	}

	// Clear the variable so it can be used again.
	bitpattern = 0;

	// The ones dot
	if (ones <= 8) {
		// Conventional bitpattern computation
		bitpattern = mirror ( 1 << (8-ones), 8 );

		// Turn off the ones dot in the tens bar
		if (ones != tens ){
			MIR[LHS][target_row][tens_color] &= ( 0xFF ^ (1 << (ones-1)) );

			if ( value_sign == NEG ){
				// Light the last LED green
				MIR[RHS][target_row][MIR_GREEN] |= 0xE0;
			}
		}
	}

	// Write the ones dot
	MIR[LHS][target_row][ones_color] |= bitpattern;

	if ( value_sign == NEG ){
		// Light the last LED green
		MIR[RHS][target_row][MIR_GREEN] |= 0xE0;
	}

	if (ones > 8) {
		bitpattern = mirror ( 1 << (16-ones), 8);
		// Turn off the ones dot in the tens bar
		if (ones != tens ){
			MIR[RHS][target_row][tens_color] &= ( 0xFF ^ (1 << 0) );

			if ( value_sign == NEG ){
				// Light the last LED green
				MIR[RHS][target_row][MIR_GREEN] |= 0xE0;
			}
		}

		MIR[RHS][target_row][ones_color] |= bitpattern;

		if ( value_sign == NEG ){
			// Light the last three LEDs green
			MIR[RHS][target_row][MIR_GREEN] |= 0xE0;
		}
	}
}


#if GROEF || 1

//
// VERTICAL BAR NUMERIC DISPLAY
//
// Show the length value as vertical bars, but only the fullsteps, no gate values.
void MIR_show_groovebar_V( unsigned char row ){

	unsigned char total = 0;
	unsigned char col = 0;
	unsigned char j = 0;
	unsigned char number 	= 0;
	unsigned char tens 		= 0;
	unsigned char side 		= 0;
	unsigned char tens_color = MIR_RED;
	unsigned char ones_color = MIR_GREEN;

	// Work on the page under the grid cursor
	Pagestruct* target_page = &Page_repository[GRID_CURSOR];


	// Iterate over the entire track, column by column
	for ( col = 0; col < MATRIX_NROF_COLUMNS; col ++ ){

		// Compute total attribute value commented out. The Map shows only the step offsets.
		total = (unsigned char) target_page->Step[row][col]->phrase_num;

		// Consider the fact that we start counting at 1 for displaying purposes!
		number = total;

		// Need to amplify the number by ten
		tens = number;

		// Decide which side we are working on
		if (col < 8)	{ side = LHS; 	} 	else { side = RHS; }

		// Populate the MIR. Green balken means straight 16th.
		for ( j = 0; j < tens; j ++ ){

			// Write the tens
			MIR[side][8-j][tens_color] 	|= (1<<(col % 8));
		}

		// The tip of the balken..
		MIR[side][8-tens][tens_color] 	|= (1<<(col % 8));
		MIR[side][8-tens][ones_color] 	|= (1<<(col % 8));
	}
}

#endif


// Show the length value as vertical bars, but only the fullsteps, no gate values.
void MIR_show_lengthbar_V( unsigned char row ){

	unsigned char total = 0;
	unsigned char col = 0;
	unsigned char j = 0;
	unsigned char number 	= 0;
	unsigned char tens 		= 0;
	unsigned char side 		= 0;
	unsigned char tens_color = MIR_RED;
	unsigned char ones_color = MIR_GREEN;

	// Work on the page under the grid cursor
	Pagestruct* target_page = &Page_repository[GRID_CURSOR];

	// Iterate over the entire track, column by column (for each step)
	for (col=0; col<MATRIX_NROF_COLUMNS; col++) {

		// Cast value to unsigned.
		total = (unsigned char) target_page->Step[row][col]->attr_LEN;

		// Ceiling of the map display at length 144. Makes no other sense
		if ( total > 145 ){

			number = 144;
		}
		else {
			// Consider the fact that we start counting at 1, not at 0!
			number = total-1;
		}

		// Compute base coordinates
		tens = number / 16;

		// Decide which side we are working on
		if (col < 8)	{ side = LHS; 	} 	else { side = RHS; }

		// Populate the MIR. Tens cannot be greater than 9 because 8 is max for j!!
		for ( j = 0; j < tens; j++ ){

			// Write the tens
			MIR[side][8-j][tens_color] 	|= (1<<(col % 8));
		}

		// The tip of the balken..
		if ( (number > 0) && (total <= 144) ){

			MIR[side][8-tens][tens_color] 	|= (1<<(col % 8));
			MIR[side][8-tens][ones_color] 	|= (1<<(col % 8));
		}
	}
}



// Show the start value of steps as vertical bars - used by the map mode
void MIR_show_startbar_V( unsigned char row ){

	unsigned char 	j		=	0,
					col		= 	0,
					total 	= 	0,
					side	= 	0;

	// Work on the page under the grid cursor
	Pagestruct* target_page = &Page_repository[GRID_CURSOR];

	// Get the start values of the steps in track [row] and display them
	for ( col=0; col < MATRIX_NROF_COLUMNS; col++ ){

		// Get the real value before computing the other offsets, etc.
		total = target_page->Step[row][col]->attr_STA;

		// Decide which side we are working on
		if (col < 8)	{ side = LHS; 	} 	else { side = RHS; }

		if ( total <= 1 ){

			for (j=4; j < 8; j++) {
				MIR[side][j][MIR_GREEN] 	|= (1<<(col % 8));
			}
//			MIR[side][8][MIR_RED]	|= (1<<(col % 8));
			MIR[side][8][MIR_GREEN]	|= (1<<(col % 8));
		}
		else if ( total <= 2 ){

			for (j=4; j < 8; j++) {
				MIR[side][j][MIR_GREEN] 	|= (1<<(col % 8));
			}
			MIR[side][8][MIR_RED]	|= (1<<(col % 8));
			MIR[side][8][MIR_GREEN]	|= (1<<(col % 8));
		}
		else if ( total <= 3 ){

			for (j=4; j < 7; j++) {
				MIR[side][j][MIR_GREEN] 	|= (1<<(col % 8));
			}
			MIR[side][7][MIR_RED]	|= (1<<(col % 8));
			MIR[side][7][MIR_GREEN]	|= (1<<(col % 8));
		}
		else if ( total <= 4 ){
			for (j=4; j < 6; j++) {
				MIR[side][j][MIR_GREEN] 	|= (1<<(col % 8));
			}
			MIR[side][6][MIR_RED]	|= (1<<(col % 8));
			MIR[side][6][MIR_GREEN]	|= (1<<(col % 8));
		}
		else if ( total <= 5 ){
			for (j=4; j < 5; j++) {
				MIR[side][j][MIR_GREEN] 	|= (1<<(col % 8));
			}
			MIR[side][5][MIR_RED]	|= (1<<(col % 8));
			MIR[side][5][MIR_GREEN]	|= (1<<(col % 8));
		}
		else if ( total == 6 ){

			MIR[side][4][MIR_RED]	|= (1<<(col % 8));
			MIR[side][4][MIR_GREEN]	|= (1<<(col % 8));
		}
		else if ( total <= 7 ){
			for (j=3; j < 5; j++) {
				MIR[side][j][MIR_RED] 	|= (1<<(col % 8));
			}
			MIR[side][3][MIR_RED]	|= (1<<(col % 8));
			MIR[side][3][MIR_GREEN]	|= (1<<(col % 8));
		}
		else if ( total <= 8 ){
			for (j=2; j < 5; j++) {
				MIR[side][j][MIR_RED] 	|= (1<<(col % 8));
			}
			MIR[side][2][MIR_RED]	|= (1<<(col % 8));
			MIR[side][2][MIR_GREEN]	|= (1<<(col % 8));
		}
		else if ( total <= 9 ){
			for (j=1; j < 5; j++) {
				MIR[side][j][MIR_RED] 	|= (1<<(col % 8));
			}
			MIR[side][1][MIR_RED]	|= (1<<(col % 8));
			MIR[side][1][MIR_GREEN]	|= (1<<(col % 8));
		}
		else if ( total <= 10 ){
			for (j=0; j < 5; j++) {
				MIR[side][j][MIR_RED] 	|= (1<<(col % 8));
			}
			MIR[side][0][MIR_RED]	|= (1<<(col % 8));
			MIR[side][0][MIR_GREEN]	|= (1<<(col % 8));
		}
		else if ( total <= 11 ){
			for (j=0; j < 5; j++) {
				MIR[side][j][MIR_RED] 	|= (1<<(col % 8));
			}
			MIR[side][0][MIR_RED]	|= (1<<(col % 8));
//			MIR[side][0][MIR_GREEN]	|= (1<<(col % 8));
		}

	} // column iterator

}


// pg. 352 in the book
// Supported interval is [-49..+49] - this determines the max offset value
void MIR_show_lengthbar_V_LEN(	unsigned char row ){

	unsigned char total = 0;
	unsigned char col = 0;
	unsigned char j = 0;
	unsigned char side 		= 0;

	// Work on the page under the grid cursor
	Pagestruct* target_page = &Page_repository[GRID_CURSOR];

	// Do this for the entire track, column by column
	for (col=0; col < MATRIX_NROF_COLUMNS; col++) {

		// Cast value to unsigned.
		total = (unsigned char) target_page->Step[row][col]->attr_LEN;

		// Decide which side we are working on
		if (col < 8)	{ side = LHS; 	} 	else { side = RHS; }

		// Middle line
		MIR[side][4][MIR_RED]	|= (1<<(col % 8));
		MIR[side][4][MIR_GREEN]	|= (1<<(col % 8));

		if ( total <= 2 ){

			// Green bar
			for (j=4; j < 9; j++) {
				MIR[side][j][MIR_GREEN] 	|= (1<<(col % 8));
			}
		}
		else if ( total <= 4 ){

			for (j=4; j < 8; j++) {
				MIR[side][j][MIR_GREEN] 	|= (1<<(col % 8));
			}
			// MIR[side][8][MIR_RED]	|= (1<<(col % 8));
			// MIR[side][8][MIR_GREEN]	|= (1<<(col % 8));
		}
		else if ( total <= 7 ){

			for (j=4; j < 7; j++) {
				MIR[side][j][MIR_GREEN] 	|= (1<<(col % 8));
			}
			// MIR[side][7][MIR_RED]	|= (1<<(col % 8));
			// MIR[side][7][MIR_GREEN]	|= (1<<(col % 8));
		}
		else if ( total <= 10 ){
			for (j=4; j < 6; j++) {
				MIR[side][j][MIR_GREEN] 	|= (1<<(col % 8));
			}
			// MIR[side][6][MIR_RED]	|= (1<<(col % 8));
			// MIR[side][6][MIR_GREEN]	|= (1<<(col % 8));
		}
		else if ( total <= 11 ){
			for (j=4; j < 5; j++) {
				MIR[side][j][MIR_GREEN] 	|= (1<<(col % 8));
			}
			// MIR[side][5][MIR_RED]	|= (1<<(col % 8));
			// MIR[side][5][MIR_GREEN]	|= (1<<(col % 8));
		}
		else if ( total == 12 ){

			MIR[side][4][MIR_RED]	|= (1<<(col % 8));
			MIR[side][4][MIR_GREEN]	|= (1<<(col % 8));
		}
		else if ( total <= 24 ){
			for (j=3; j < 5; j++) {
				MIR[side][j][MIR_RED] 	|= (1<<(col % 8));
			}
			// MIR[side][3][MIR_RED]	|= (1<<(col % 8));
			// MIR[side][3][MIR_GREEN]	|= (1<<(col % 8));
		}
		else if ( total <= 32 ){
			for (j=2; j < 5; j++) {
				MIR[side][j][MIR_RED] 	|= (1<<(col % 8));
			}
			// MIR[side][2][MIR_RED]	|= (1<<(col % 8));
			// MIR[side][2][MIR_GREEN]	|= (1<<(col % 8));
		}
		else if ( total <= 64 ){
			for (j=1; j < 5; j++) {
				MIR[side][j][MIR_RED] 	|= (1<<(col % 8));
			}
			// MIR[side][1][MIR_RED]	|= (1<<(col % 8));
			// MIR[side][1][MIR_GREEN]	|= (1<<(col % 8));
		}
		else if ( total <= 128 ){
			for (j=0; j < 5; j++) {
				MIR[side][j][MIR_RED] 	|= (1<<(col % 8));
			}
			// MIR[side][0][MIR_RED]	|= (1<<(col % 8));
			// MIR[side][0][MIR_GREEN]	|= (1<<(col % 8));
		}
		else{
			for (j=0; j < 5; j++) {
				MIR[side][j][MIR_RED] 	|= (1<<(col % 8));
			}
		}

	} // track iterator
}





// pg. 352 in the book
// Supported interval is [-49..+49] - this determines the max offset value
void MIR_write_numeric_V_zeroline_PIT(	unsigned char center_line,
										// signed char min_val,
										// signed char max_val,
										unsigned char row 				){

	signed char total = 0;
	unsigned char col = 0;
	unsigned char j = 0;
	unsigned char side 		= 0;
	unsigned char tens_color = MIR_RED;

	// Work on the page under the grid cursor
	Pagestruct* target_page = &Page_repository[GRID_CURSOR];

	// Do this for the entire track, column by column
	for (col=0; col < MATRIX_NROF_COLUMNS; col++) {

		// Fetch the total attribute value for PIT
		total = target_page->Step[row][col]->attr_PIT;

		// Decide which side we are working on
		if (col < 8)	{ side = LHS; 	} 	else { side = RHS; }

		if ( total <= -29 ){

			for (j=4; j < 8; j++) {
				MIR[side][j][tens_color] 	|= (1<<(col % 8));
			}
			MIR[side][8][MIR_RED]	|= (1<<(col % 8));
//			MIR[side][8][MIR_GREEN]	|= (1<<(col % 8));
		}
		else if ( total <= -20 ){

			for (j=4; j < 8; j++) {
				MIR[side][j][tens_color] 	|= (1<<(col % 8));
			}
			MIR[side][8][MIR_RED]	|= (1<<(col % 8));
			MIR[side][8][MIR_GREEN]	|= (1<<(col % 8));
		}
		else if ( total <= -9 ){

			for (j=4; j < 7; j++) {
				MIR[side][j][tens_color] 	|= (1<<(col % 8));
			}
			MIR[side][7][MIR_RED]	|= (1<<(col % 8));
			MIR[side][7][MIR_GREEN]	|= (1<<(col % 8));
		}
		else if ( total <= -6 ){
			for (j=4; j < 6; j++) {
				MIR[side][j][tens_color] 	|= (1<<(col % 8));
			}
			MIR[side][6][MIR_RED]	|= (1<<(col % 8));
			MIR[side][6][MIR_GREEN]	|= (1<<(col % 8));
		}
		else if ( total <= -1 ){
			for (j=4; j < 5; j++) {
				MIR[side][j][tens_color] 	|= (1<<(col % 8));
			}
			MIR[side][5][MIR_RED]	|= (1<<(col % 8));
			MIR[side][5][MIR_GREEN]	|= (1<<(col % 8));
		}
		else if ( total == 0 ){

			MIR[side][4][MIR_RED]	|= (1<<(col % 8));
			MIR[side][4][MIR_GREEN]	|= (1<<(col % 8));
		}
		else if ( total <= 5 ){
			for (j=3; j < 5; j++) {
				MIR[side][j][tens_color] 	|= (1<<(col % 8));
			}
			MIR[side][3][MIR_RED]	|= (1<<(col % 8));
			MIR[side][3][MIR_GREEN]	|= (1<<(col % 8));
		}
		else if ( total <= 8 ){
			for (j=2; j < 5; j++) {
				MIR[side][j][tens_color] 	|= (1<<(col % 8));
			}
			MIR[side][2][MIR_RED]	|= (1<<(col % 8));
			MIR[side][2][MIR_GREEN]	|= (1<<(col % 8));
		}
		else if ( total <= 19 ){
			for (j=1; j < 5; j++) {
				MIR[side][j][tens_color] 	|= (1<<(col % 8));
			}
			MIR[side][1][MIR_RED]	|= (1<<(col % 8));
			MIR[side][1][MIR_GREEN]	|= (1<<(col % 8));
		}
		else if ( total <= 28 ){
			for (j=0; j < 5; j++) {
				MIR[side][j][tens_color] 	|= (1<<(col % 8));
			}
			MIR[side][0][MIR_RED]	|= (1<<(col % 8));
			MIR[side][0][MIR_GREEN]	|= (1<<(col % 8));
		}
		else if ( total <= 40 ){
			for (j=0; j < 5; j++) {
				MIR[side][j][tens_color] 	|= (1<<(col % 8));
			}
			MIR[side][0][MIR_RED]	|= (1<<(col % 8));
		}

	} // track iterator
}


// Supported interval is [-49..+49] - this determines the max offset value
void MIR_write_numeric_V_zeroline_VEL(	unsigned char 	center_line,
										// signed char 	min_val,
										// signed char 	max_val,
										unsigned char 	row,
										unsigned char 	attr ){

	signed char total = 0;
	unsigned char col = 0;
	unsigned char j = 0;
	unsigned char side 		= 0;
	unsigned char tens_color = MIR_RED;

	// Work on the page under the grid cursor
	Pagestruct* target_page = &Page_repository[GRID_CURSOR];

	// Do this for the entire track, column by column
	for (col=0; col < MATRIX_NROF_COLUMNS; col++) {

		// Fetch the total attribute value
//		total = target_page->Step[row][col]->attribute[attr+1];
//		total = target_page->Step[row][col]->attr_VEL;
		total = (signed char) *(ptr_of_step_attr_value( target_page, row, col, attr+1 ) );


		// Decide which side we are working on
		if (col < 8){
			side = LHS;
		}
		else{
			side = RHS;
		}

		// Execute on the values
		if ( total <= -30 ){

			for (j=4; j < 8; j++) {
				MIR[side][j][tens_color] 	|= (1<<(col % 8));
			}
			MIR[side][8][MIR_RED]	|= (1<<(col % 8));
			MIR[side][8][MIR_GREEN]	|= (1<<(col % 8));
		}
		else if ( total <= -20 ){

			for (j=4; j < 7; j++) {
				MIR[side][j][tens_color] 	|= (1<<(col % 8));
			}
			MIR[side][7][MIR_RED]	|= (1<<(col % 8));
			MIR[side][7][MIR_GREEN]	|= (1<<(col % 8));
		}
		else if ( total <= -10 ){
			for (j=4; j < 6; j++) {
				MIR[side][j][tens_color] 	|= (1<<(col % 8));
			}
			MIR[side][6][MIR_RED]	|= (1<<(col % 8));
			MIR[side][6][MIR_GREEN]	|= (1<<(col % 8));
		}
		else if ( total <= -1 ){
			for (j=4; j < 5; j++) {
				MIR[side][j][tens_color] 	|= (1<<(col % 8));
			}
			MIR[side][5][MIR_RED]	|= (1<<(col % 8));
			MIR[side][5][MIR_GREEN]	|= (1<<(col % 8));
		}
		else if ( total == 0 ){

			MIR[side][4][MIR_RED]	|= (1<<(col % 8));
			MIR[side][4][MIR_GREEN]	|= (1<<(col % 8));
		}
		else if ( total <= 10 ){
			for (j=3; j < 5; j++) {
				MIR[side][j][tens_color] 	|= (1<<(col % 8));
			}
			MIR[side][3][MIR_RED]	|= (1<<(col % 8));
			MIR[side][3][MIR_GREEN]	|= (1<<(col % 8));
		}
		else if ( total <= 20 ){
			for (j=2; j < 5; j++) {
				MIR[side][j][tens_color] 	|= (1<<(col % 8));
			}
			MIR[side][2][MIR_RED]	|= (1<<(col % 8));
			MIR[side][2][MIR_GREEN]	|= (1<<(col % 8));
		}
		else if ( total <= 30 ){
			for (j=1; j < 5; j++) {
				MIR[side][j][tens_color] 	|= (1<<(col % 8));
			}
			MIR[side][1][MIR_RED]	|= (1<<(col % 8));
			MIR[side][1][MIR_GREEN]	|= (1<<(col % 8));
		}
		else if ( total <= 40 ){
			for (j=0; j < 5; j++) {
				MIR[side][j][tens_color] 	|= (1<<(col % 8));
			}
			MIR[side][0][MIR_RED]	|= (1<<(col % 8));
			MIR[side][0][MIR_GREEN]	|= (1<<(col % 8));
		}
	} // track iterator
}



// Supported interval is [-49..+49] - this determines the max offset value
void MIR_write_numeric_V_zeroline_MCC_BENDER(	unsigned char 	center_line,
												// signed char 	min_val,
												// signed char 	max_val,
												unsigned char 	row ){

	signed char total = 0;
	unsigned char col = 0;
	unsigned char j = 0;
	unsigned char side 		= 0;
	unsigned char tens_color = MIR_RED;

	// Work on the page under the grid cursor
	Pagestruct* target_page = &Page_repository[GRID_CURSOR];

	// Do this for the entire track, column by column
	for (col=0; col < MATRIX_NROF_COLUMNS; col++) {

		// Do not bother with steps that carry no bender data - in total
		if (	( target_page->Step[row][col]->attr_MCC == MIDICC_NONE )
			&&	( target_page->Step[row][col]->attr_POS == 0 )
			){

			continue;
		}

		// Decide which side we are working on
		if (col < 8)	{ side = LHS; 	} 	else { side = RHS; }


		// Fetch the total attribute value
		// total = target_page->Step[row][col]->attr_MCC - 64;
		total = map_to_8bit(
					( target_page->Step[row][col]->attr_MCC << 7 )
					| target_page->Step[row][col]->attr_POS );

		// Adjust total value for display
		total = total - 64;

		if ( total <= -40 ){
			for (j=4; j < 9; j++) {
				MIR[side][j][tens_color] 	|= (1<<(col % 8));
			}
		}
		else if ( total <= -30 ){
			for (j=4; j < 8; j++) {
				MIR[side][j][tens_color] 	|= (1<<(col % 8));
			}
			MIR[side][8][MIR_RED]	|= (1<<(col % 8));
			MIR[side][8][MIR_GREEN]	|= (1<<(col % 8));
		}
		else if ( total <= -20 ){
			for (j=4; j < 7; j++) {
				MIR[side][j][tens_color] 	|= (1<<(col % 8));
			}
			MIR[side][7][MIR_RED]	|= (1<<(col % 8));
			MIR[side][7][MIR_GREEN]	|= (1<<(col % 8));
		}
		else if ( total <= -10 ){
			for (j=4; j < 6; j++) {
				MIR[side][j][tens_color] 	|= (1<<(col % 8));
			}
			MIR[side][6][MIR_RED]	|= (1<<(col % 8));
			MIR[side][6][MIR_GREEN]	|= (1<<(col % 8));
		}
		else if ( total <= -1 ){
			for (j=4; j < 5; j++) {
				MIR[side][j][tens_color] 	|= (1<<(col % 8));
			}
			MIR[side][5][MIR_RED]	|= (1<<(col % 8));
			MIR[side][5][MIR_GREEN]	|= (1<<(col % 8));
		}
		else if ( total == 0 ){
			MIR[side][4][MIR_RED]	|= (1<<(col % 8));
			MIR[side][4][MIR_GREEN]	|= (1<<(col % 8));
		}
		else if ( total <= 10 ){
			for (j=3; j < 5; j++) {
				MIR[side][j][tens_color] 	|= (1<<(col % 8));
			}
			MIR[side][3][MIR_RED]	|= (1<<(col % 8));
			MIR[side][3][MIR_GREEN]	|= (1<<(col % 8));
		}
		else if ( total <= 20 ){
			for (j=2; j < 5; j++) {
				MIR[side][j][tens_color] 	|= (1<<(col % 8));
			}
			MIR[side][2][MIR_RED]	|= (1<<(col % 8));
			MIR[side][2][MIR_GREEN]	|= (1<<(col % 8));
		}
		else if ( total <= 30 ){
			for (j=1; j < 5; j++) {
				MIR[side][j][tens_color] 	|= (1<<(col % 8));
			}
			MIR[side][1][MIR_RED]	|= (1<<(col % 8));
			MIR[side][1][MIR_GREEN]	|= (1<<(col % 8));
		}
		else if ( total <= 40 ){
			for (j=0; j < 5; j++) {
				MIR[side][j][tens_color] 	|= (1<<(col % 8));
			}
			MIR[side][0][MIR_RED]	|= (1<<(col % 8));
			MIR[side][0][MIR_GREEN]	|= (1<<(col % 8));
		}
		else if ( total > 40 ){
			for (j=0; j < 5; j++) {
				MIR[side][j][tens_color] 	|= (1<<(col % 8));
			}
		}
	} // track iterator
}





// Shows the approximation value bar vertically,
// ..mapping values in [0..127] to [0..99]
void MIR_write_numeric_V_MCC ( Pagestruct* target_page, unsigned char row ) {

	signed char total = 0;
	unsigned char col = 0;
	unsigned char j = 0;
	unsigned char side 		= 0;


	// Do this for the entire track, column by column
	for (col=0; col < MATRIX_NROF_COLUMNS; col++) {

		// Get the total attribute value
		total = target_page->Step[row][col]->attr_MCC;

		// Decide which side we are working on
		if (col < 8){
			side = LHS;
		}
		else{
			side = RHS;
		}

		// Display the number
		if ( total == MIDICC_NONE ){

			// Do nothing, as nothing to show
		}
		// Range 0-16
		else if ( total <= 16 ){

			MIR[side][8][MIR_RED] 		|= (1<<(col % 8));
			MIR[side][8][MIR_GREEN] 	|= (1<<(col % 8));
		}
		// Range 17-32
		else if ( total <= 32 ){

			// The big bar
			for (j=7; j < 9; j++) {
				MIR[side][j][MIR_RED] 	|= (1<<(col % 8));
			}
			// The orange tip
			MIR[side][7][MIR_RED] 		|= (1<<(col % 8));
			MIR[side][7][MIR_GREEN] 	|= (1<<(col % 8));
		}
		// Range 33-48
		else if ( total <= 48 ){

			// The big bar
			for (j=6; j < 9; j++) {
				MIR[side][j][MIR_RED] 	|= (1<<(col % 8));
			}
			// The orange tip
			MIR[side][6][MIR_RED] 		|= (1<<(col % 8));
			MIR[side][6][MIR_GREEN] 	|= (1<<(col % 8));
		}
		// Range 49-63
		else if ( total <= 63 ){

			// The big bar
			for (j=5; j < 9; j++) {
				MIR[side][j][MIR_RED] 	|= (1<<(col % 8));
			}
			// The orange tip
			MIR[side][5][MIR_RED] 		|= (1<<(col % 8));
			MIR[side][5][MIR_GREEN] 	|= (1<<(col % 8));
		}
		// Middle value: 64
		else if ( total == 64 ){

			// The big bar
			for (j=4; j < 9; j++) {
				MIR[side][j][MIR_RED] 	|= (1<<(col % 8));
			}
			// The orange tip
			MIR[side][4][MIR_RED] 		|= (1<<(col % 8));
			MIR[side][4][MIR_GREEN] 	|= (1<<(col % 8));
		}
		// Range 65-80
		else if ( total <= 80 ){

			// The big bar
			for (j=3; j < 9; j++) {
				MIR[side][j][MIR_RED] 	|= (1<<(col % 8));
			}
			// The orange tip
			MIR[side][3][MIR_RED] 		|= (1<<(col % 8));
			MIR[side][3][MIR_GREEN] 	|= (1<<(col % 8));
		}
		// Range 81-96
		else if ( total <= 96 ){

			// The big bar
			for (j=2; j < 9; j++) {
				MIR[side][j][MIR_RED] 	|= (1<<(col % 8));
			}
			// The orange tip
			MIR[side][2][MIR_RED] 		|= (1<<(col % 8));
			MIR[side][2][MIR_GREEN] 	|= (1<<(col % 8));
		}
		// Range 97-112
		else if ( total <= 112 ){

			// The big bar
			for (j=1; j < 9; j++) {
				MIR[side][j][MIR_RED] 	|= (1<<(col % 8));
			}
			// The orange tip
			MIR[side][1][MIR_RED] 		|= (1<<(col % 8));
			MIR[side][1][MIR_GREEN] 	|= (1<<(col % 8));
		}
		// Range 113-127
		else if (	(total >= 113 )  ){

			// The big bar
			for (j=0; j < 9; j++) {
				MIR[side][j][MIR_RED] 	|= (1<<(col % 8));
			}
			// The orange tip
			MIR[side][0][MIR_RED] 		|= (1<<(col % 8));
			MIR[side][0][MIR_GREEN] 	|= (1<<(col % 8));
		}

	}
}



// 99 is now the value for display cutoff - this is the max offset value
void MIR_write_numeric_V (unsigned char row, unsigned char attr) {

	signed char total = 0;
	unsigned char col = 0;
	unsigned char j = 0;
	// unsigned char bitpattern = 0;
	unsigned char number 	= 0;
	unsigned char tens 		= 0;
	unsigned char ones 		= 0;
	unsigned char side 		= 0;
	unsigned char tens_color = MIR_RED;
	unsigned char ones_color = MIR_GREEN;
	unsigned char sign = POS;

	// Work on the page under the grid cursor
	Pagestruct* target_page = &Page_repository[GRID_CURSOR];

	// Check whether step is set. If not, next iteration.
	unsigned int trackpattern = Page_get_trackpattern( target_page, row );

//	d_iag_printf("c_V row:%d attr:%d\n", target_row, attr);
//	my_print_bits( trackpattern );

	// Do this for the entire track, column by column
	for (col=0; col<MATRIX_NROF_COLUMNS; col++) {

		// If the step is set show its attribute value
		if (((trackpattern & (1<< (15-col) ) ) != 0) || (1) ) {

			// Compute total attribute value commented out.
//			total = target_page->Step[row][col]->attribute[attr+1];
			total = (signed char) *(ptr_of_step_attr_value( target_page, row, col, attr+1 ) );

			// Note the negative values of attr
			if ( total < 0 ){

				sign = NEG;
				total = abs( total );
			}

			// Cealing at 99
			if (total >= 127){ 	number = 99;	}
			else 			{	number = total;	}

			// Compute base coordinates
			tens 				= number /  10;
			ones 				= number %  10;

			// Decide which side we are working on
			if (col < 8)	{ side = LHS; 	} 	else { side = RHS; }
			if (tens > 9) 	{ tens = 9; 	}

			// Tens
			for (j=0; j<tens; j++) {

				MIR[side][8-j][tens_color] 	|= (1<<(col % 8));

				// Blink the tens of negative values
				if (	( sign == NEG )
					){

					MIR[side][8-j][MIR_BLINK] 	|= (1<<(col % 8));
				}
			} // tens

			// Ones
			if (ones > 0) {

				// Have the ones be always green unless falling together with the tens
				if (ones != tens) {
					MIR[side][9-ones][tens_color]	 &= ( 0xFF ^ (1 << (col % 8)) );
				}

				MIR[side][9-ones][ones_color]     	 |= (1 << (col % 8));

				// Blink the ones of negative values
				if (	( sign == NEG )
					&	( tens == 0 )
					){

					MIR[side][9-ones][MIR_BLINK]     	 |= (1 << (col % 8));
				}
			} // ones
		} // (trackpattern & 1<<i) !=0

		// Reset the sign
		sign = POS;

	} // track iterator
}



// Writes a number in vertical bar representation to the buttool LHS
void MIR_write_buttool_numeric_LHS( unsigned char number ){

	// char total = 0;
	// char col = 0;
	// char j = 0;

	int bitpattern = 0;
	unsigned char hundreds 	= 0;
	unsigned char tens 		= 0;
	unsigned char tens_over_hundred =0;
	unsigned char ones 		= 0;

	// Compute bitpattern and use writeButtool to show it

	// Compute base coordinates
	hundreds 			= number / 100;
	tens 				= number /  10;
	tens_over_hundred 	= tens   %  10;
	ones 				= number %  10;

	bitpattern = mirror( (0x3FF >> (9-tens)) 	& 0x3FE, 10 );
	MIR_write_buttool( LHS, bitpattern, MIR_RED );

	bitpattern = mirror( (1 << ones) 			& 0x3FE, 10 );
	MIR_write_buttool( LHS, bitpattern, MIR_GREEN );

	// Light 0 LED orange if beyond 100
	if (hundreds > 0) {
		MIR_write_buttool( LHS, mirror( 1, 10 ), MIR_RED   );
		MIR_write_buttool( LHS, mirror( 1, 10 ), MIR_GREEN );

		bitpattern = mirror( (0x3FF >> (9-tens_over_hundred)) 	& 0x3FE, 10 );
		MIR_write_buttool( LHS, bitpattern, MIR_RED );
	}
}



//
// 12 LED NUMERIC DISPLAY
//
// Writes a number into the given matrix row using the decimal representation over 12 LEDS
void MIR_write_numeric_12LED (unsigned char number, unsigned char target_row) {

	unsigned char bitpattern = 0;
	unsigned char tens = number / 10;
	unsigned char ones = number % 10;

	// MIR_RED content
	bitpattern = 0xFF;
	if (tens <= 8) {
		bitpattern = (0xFF >> (8 - tens) );
	}
	MIR[LHS][target_row][MIR_RED] |= bitpattern;

	if (tens > 8) {
		bitpattern = (0xFF >> (16-tens) );
		MIR[RHS][target_row][MIR_RED] |= bitpattern;
	}
	bitpattern =0;

	// MIR_GREEN content
	if (ones <= 8) {
		bitpattern = mirror ( 1 << (8-ones), 8 );
	}
	MIR[LHS][target_row][MIR_GREEN] |= bitpattern;

	if (ones > 8) {
		bitpattern = mirror ( 1 << (16-ones), 8);
		MIR[RHS][target_row][MIR_GREEN] |= bitpattern;
	}
}


// Returns all defined attributes of the infotarget
unsigned int attributes_of (unsigned char obj_type, int obj_target) {
	unsigned int bitpattern=0;
	unsigned char attr=0;

	// Attributes that have an entry
	for (attr=0; attr<10; attr++){
		if (G_PANEL_matrix[obj_type][attr] > 0){
			bitpattern |= (1 << attr);
		}
	}

	return bitpattern;
}

// Returns the dot of a page index
unsigned char dot_of_page (unsigned char in_ndx) {

	return (11 + ((in_ndx/10)*11) + (in_ndx%10) );
}




// Displays the value of the tempo multiplier in given track.
// Logic is on book pg. 256 ff
void display_stepLEN_multiplier( Stepstruct* target_step ){

	// Show the track's tempo multiplier value as defined in book p.257
//	switch( target_step->LEN_multiplier ){
	switch( (target_step->event_data & 0xF0) >> 4 ){

		case 1:
			MIR_write_dot( LED_PLAY1, MIR_RED );
			break;

		case 2:
			MIR_write_dot( LED_PLAY2, MIR_RED );
			break;

		case 3:
			MIR_write_dot( LED_PLAY1, MIR_RED );
			MIR_write_dot( LED_PLAY2, MIR_RED );
			break;

		case 4:
			MIR_write_dot( LED_PLAY4, MIR_RED );
			break;

		case 5:
			MIR_write_dot( LED_PLAY1, MIR_RED );
			MIR_write_dot( LED_PLAY4, MIR_RED );
			break;

		case 6:
			MIR_write_dot( LED_PLAY2, MIR_RED );
			MIR_write_dot( LED_PLAY4, MIR_RED );
			break;

		case 7:
			MIR_write_dot( LED_PLAY1, MIR_RED );
			MIR_write_dot( LED_PLAY2, MIR_RED );
			MIR_write_dot( LED_PLAY4, MIR_RED );
			break;

		case 8:
			MIR_write_dot( LED_PLAY2, MIR_RED );
			MIR_write_dot( LED_PLAY4, MIR_GREEN );
			MIR_write_dot( LED_PLAY4, MIR_RED );
			break;

		case 16:
			MIR_write_dot( LED_PLAY1, MIR_RED );
			MIR_write_dot( LED_PLAY2, MIR_GREEN );
			MIR_write_dot( LED_PLAY2, MIR_RED );
			MIR_write_dot( LED_PLAY4, MIR_GREEN );
			MIR_write_dot( LED_PLAY4, MIR_RED );
			break;
	} // switch on the LEN multiplier
}




// Displays the value of the tempo multiplier in given track.
// Logic is on book pg. 256 ff
void display_Track_G_master_tempoMUL( Trackstruct* target_track ){

	// Show the track's tempo multiplier value as defined in book p.257
	switch( target_track->attr_TEMPOMUL ){

		case 0:
			// This is the PAUSE state
			MIR_write_dot( LED_PAUSE, MIR_RED 	);
			MIR_write_dot( LED_PAUSE, MIR_GREEN );

			break;

		case 1:
			// This also contains the divisor cases

			switch( target_track->attr_TEMPOMUL_SKIP & 0x0F ){

				// No track clock divisor, just normal speed
				case 0:
					// MIR_write_dot( LED_PLAY1, MIR_GREEN );
					MIR_write_dot( LED_PLAY1, MIR_RED );
					break;

				// Track clock divisor is 2 - leaving one count out, active on the 2nd
				case 1:
					// MIR_write_dot( LED_PLAY1, MIR_GREEN );
					// MIR_write_dot( LED_PLAY1, MIR_RED );

					MIR_write_dot( LED_PLAY2, MIR_GREEN	);
					break;

				// Track clock divisor is 3 - leaving two counts out, active on the 3rd
				case 2:
					MIR_write_dot( LED_PLAY1, MIR_GREEN );
					MIR_write_dot( LED_PLAY2, MIR_GREEN	);
					break;

				// Track clock divisor is 4 - leaving three counts out, active on the 4th
				case 3:
					MIR_write_dot( LED_PLAY4, MIR_GREEN	);
					break;

				// Track clock divisor is 5 - leaving four counts, active on the 5th
				case 4:
					MIR_write_dot( LED_PLAY1, MIR_GREEN );
					MIR_write_dot( LED_PLAY4, MIR_GREEN	);
					break;

				// Track clock divisor is 6 - leaving five counts, active on the 6th
				case 5:
					MIR_write_dot( LED_PLAY2, MIR_GREEN );
					MIR_write_dot( LED_PLAY4, MIR_GREEN	);
					break;

				// Track clock divisor is 7 - leaving six counts, active on the 7th
				case 6:
					MIR_write_dot( LED_PLAY1, MIR_GREEN );
					MIR_write_dot( LED_PLAY2, MIR_GREEN );
					MIR_write_dot( LED_PLAY4, MIR_GREEN	);
					break;

				case 7:
					MIR_write_dot( LED_PLAY2, MIR_GREEN );
					MIR_write_dot( LED_PLAY4, MIR_GREEN	);
					MIR_write_dot( LED_PLAY4, MIR_RED );
					break;

				case 15:
					MIR_write_dot( LED_PLAY1, MIR_GREEN );
					MIR_write_dot( LED_PLAY2, MIR_GREEN );
					MIR_write_dot( LED_PLAY2, MIR_RED );
					MIR_write_dot( LED_PLAY4, MIR_GREEN );
					MIR_write_dot( LED_PLAY4, MIR_RED );
					break;
			}
			break;


		case 2:
			// MIR_write_dot( LED_PLAY2, MIR_GREEN );
			MIR_write_dot( LED_PLAY2, MIR_RED );
			break;

		case 3:
			// MIR_write_dot( LED_PLAY1, MIR_GREEN );
			MIR_write_dot( LED_PLAY1, MIR_RED );

			// MIR_write_dot( LED_PLAY2, MIR_GREEN );
			MIR_write_dot( LED_PLAY2, MIR_RED );
			break;

		case 4:
			// MIR_write_dot( LED_PLAY4, MIR_GREEN );
			MIR_write_dot( LED_PLAY4, MIR_RED );
			break;

		case 5:
			// MIR_write_dot( LED_PLAY1, MIR_GREEN );
			MIR_write_dot( LED_PLAY1, MIR_RED );

			// MIR_write_dot( LED_PLAY4, MIR_GREEN );
			MIR_write_dot( LED_PLAY4, MIR_RED );
			break;

		case 6:
			// MIR_write_dot( LED_PLAY2, MIR_GREEN );
			MIR_write_dot( LED_PLAY2, MIR_RED );

			// MIR_write_dot( LED_PLAY4, MIR_GREEN );
			MIR_write_dot( LED_PLAY4, MIR_RED );
			break;

		case 7:
			// MIR_write_dot( LED_PLAY1, MIR_GREEN );
			MIR_write_dot( LED_PLAY1, MIR_RED );

			// MIR_write_dot( LED_PLAY2, MIR_GREEN );
			MIR_write_dot( LED_PLAY2, MIR_RED );

			// MIR_write_dot( LED_PLAY4, MIR_GREEN );
			MIR_write_dot( LED_PLAY4, MIR_RED );
			break;

		case 8:
			MIR_write_dot( LED_PLAY2, MIR_RED );
			MIR_write_dot( LED_PLAY4, MIR_GREEN );
			MIR_write_dot( LED_PLAY4, MIR_RED );
			break;

		case 12:
			MIR_write_dot( LED_PLAY1, MIR_GREEN );
			MIR_write_dot( LED_PLAY1, MIR_RED );

			MIR_write_dot( LED_PLAY2, MIR_RED 	);
			break;

		case 14:
			MIR_write_dot( LED_PLAY1, MIR_GREEN );
			MIR_write_dot( LED_PLAY1, MIR_RED );

			MIR_write_dot( LED_PLAY4, MIR_RED 	);
			break;

		case 15:
//			MIR_write_dot( LED_PLAY1, MIR_GREEN );
			MIR_write_dot( LED_PLAY1, MIR_RED );
			MIR_write_dot( LED_PLAY2, MIR_GREEN );
			MIR_write_dot( LED_PLAY2, MIR_RED );
			break;

		case 25:
			MIR_write_dot( LED_PLAY1, MIR_GREEN );
//			MIR_write_dot( LED_PLAY1, MIR_RED );
			MIR_write_dot( LED_PLAY2, MIR_GREEN );
			MIR_write_dot( LED_PLAY2, MIR_RED );
			break;

		case 16:
			MIR_write_dot( LED_PLAY1, MIR_RED );
			MIR_write_dot( LED_PLAY2, MIR_GREEN );
			MIR_write_dot( LED_PLAY2, MIR_RED );
			MIR_write_dot( LED_PLAY4, MIR_GREEN );
			MIR_write_dot( LED_PLAY4, MIR_RED );
			break;
	}
}


// Checks whether all playing tracks in page are aligned and returns status
unsigned char page_needs_align( Pagestruct* target_page ){

	unsigned char i = 0;
	unsigned char last_LOC = 0;
	unsigned char last_TTC = 0;
	unsigned char result = FALSE;

	// Iterate through all rows
	for ( i=0; i < MATRIX_NROF_ROWS; i++ ){

		// Consider the playing tracks only
		if ( target_page->Track[i]->attr_LOCATOR != 0 ){

			// First time that a playing LOC value is found
			if ( last_LOC == 0){

				// Take that track's LOC and TTC as reference point
				last_LOC = target_page->Track[i]->attr_LOCATOR;
				last_TTC = target_page->Track[i]->TTC;
			}
			else {

				// If the value deviates from the reference
				if (	( last_LOC != target_page->Track[i]->attr_LOCATOR )
					||	( last_TTC != target_page->Track[i]->TTC )
				){

					// Signal that page can be aligned
					result = TRUE;
					break;
				}
			}
		} // current track locator != 0
	} // row iterator

	return result;
}



