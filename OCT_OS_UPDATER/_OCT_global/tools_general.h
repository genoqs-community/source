/* OCTOPUS
 * Miscellaneous defines used in the designated code parts
 * 14.11.2003 genoQs Machines
 */



signed int my_max_value( signed int in_1, signed int in_2 ){

	if ( in_1 < in_2 ){ 
		return in_2;
	}
	else{ 
		return in_1;
	}
}


// Return the smaller of two numbers
signed int my_min_value( signed int in_1, signed int in_2 ){

	if ( in_1 < in_2 ){ 
		return in_1;
	}
	else{ 
		return in_2;
	}
}




void my_print_bits(unsigned int value){
// Scaffold: display an integer as a string of bits
	int i=0;
	// Value to mask off the bits in value
	int mask=1<<15;	

	// Lock mutex, print the string, unlock mutex
	for (i=0; i<16; i++){
		putchar (value & mask ? '1' : '-');
		value <<= 1;		
		if ((15-i)%8 == 0) putchar(' ');
	}	
	putchar('\n');
}


void my_print_bits32(unsigned int value){
// Scaffold: display an integer as a string of bits
	unsigned int i=0;
	// Value to mask off the bits in value
	unsigned int mask=1<<31;	

	// Lock mutex, print the string, unlock mutex
	for (i=0; i<32; i++){
		putchar (value & mask ? '1' : '-');
		value <<= 1;		
		if ((31-i)%8 == 0) putchar(' ');
	}	
	putchar('\n');
}


// Returns the position of the first set bit in a bitpattern;
// 33 is the "bit not found" flag
unsigned int my_bit2ndx(unsigned int bitpattern){
	unsigned char i=0;
	for (i=0; i<33; i++){
		if (bitpattern & (1<<i)) break;
	}
	return i;
}


unsigned char my_bit_cardinality( unsigned int bitpattern ){
// Returns the number of set bits in the bitpattern
	unsigned char 	i=0,
					j=0;
	
	// Max length of bitpattern is 32 - integer
	for (i=0; i<32; i++) {
		if ( ((1<<i) & bitpattern) != 0 ) {
			j++;
		}
	}
	return j;
}


// Used to produce time delays using busy wait
void my_wait( unsigned int wait ){

	unsigned int i=0;

	for ( i=0; i<wait; i++ ){};
}



//
// Takes a bitpattern of given length, 
// Shifts its contents in given direction over a distance of distance
// and wraps contents accordigly.
//
unsigned int my_shift_bitpattern (	unsigned int bitpattern, 
									unsigned char length,
									unsigned char direction, 
									unsigned char distance) {

	// Get the trackpattern
	unsigned int rightMostChunk = 0;
	unsigned int leftMostChunk = 0;
	unsigned int targetpattern = 0;
	unsigned int bitmask = 0xffffffff >> (32 - length);
	
	//d_iag_printf( "*my_shift_bitp: l:%d dr:%d ds:%d  bitmask/bitp\n", length, direction, distance);
	//my_print_bits32( bitmask);
	//my_print_bits32( bitpattern );

	// Compose the new bitpattern
	switch (direction) {
		case INC:
			// Wrap right
			// rightMostChunk = bitpattern & ( (1<<distance) - 1 );
			rightMostChunk 	= bitpattern & ( bitmask >> (length - distance));
	//d_iag_printf( "   rmC:\n" );
	//my_print_bits32( rightMostChunk);

			// leftMostChunk = bitpattern >> distance;
			leftMostChunk 	= bitpattern  >> distance;
	//d_iag_printf( "   lmC:\n" );
	//my_print_bits32( leftMostChunk);

			targetpattern = (leftMostChunk) | (rightMostChunk << (length-distance));		
	//d_iag_printf( "   target:\n" );
	//my_print_bits32( targetpattern );

			break;
		
		case DEC:
			// Wrap left
			rightMostChunk = bitpattern & ( (1<<(length-distance)) - 1 );
			leftMostChunk = bitpattern >> (length-distance);	
			targetpattern = (leftMostChunk) | (rightMostChunk << distance);
			break;
	}
	
	// Return the new bitpattern
	return targetpattern;
}


// Takes the natural index of a matrix key (1-160) and returns its machine index.
unsigned int machine_index_of_key( unsigned int matrix_ndx ){

	return( 11 + matrix_ndx / 16 ) + ( (matrix_ndx) %16 )*11  ;
}




