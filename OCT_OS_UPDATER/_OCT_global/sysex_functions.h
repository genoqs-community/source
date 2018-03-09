

//___________________________________________________________________________________________
void send_sysex( unsigned char in_byte ){

	//Send controller number
	HAL_WRITE_UINT32( E7T_UART0_BASE + E7T_UART_TXBUF, in_byte );
	Player_wait( E7T_UART0_BASE );
}



//___________________________________________________________________________________________
// Assembles one byte from two given nibbles.
unsigned char sysex_to_byte( unsigned char high_nibble, unsigned char low_nibble ){
	
	return ( (high_nibble << 4) | low_nibble );
}



//___________________________________________________________________________________________
// Sends a byte as MIDI sysex dividing it into two nibbles, returning the checksum
unsigned char byte_to_sysex( unsigned char in_byte ){

	unsigned char high_nibble = ( 0xf0 & in_byte ) >> 4;
	unsigned char low_nibble  = 0x0f & in_byte;
	
	send_sysex( high_nibble );
	send_sysex( low_nibble	);

	return ( high_nibble ^ low_nibble );
}



//___________________________________________________________________________________________
// Sends the contents of the given page flatrecord as a sysex stream
unsigned char send_sysex_page( unsigned char pageNdx ){

	unsigned char* pointer = NULL;
	unsigned int i = 0;
	unsigned char checksum = 0;
	 
	// Set the pointer to the beginning of the page record
	pointer = (unsigned char* ) &(memory_image.Page_flatrecord_repository[ pageNdx ]);

	// Send the full content of the page record via sysex
	for ( i=0; i < ((unsigned int) sizeof( Page_flatrecord_struct )); i++ ){
	
		// Compute the running checksum as the bytes are being sent
		checksum ^= byte_to_sysex( (unsigned char) pointer[i] );
	}
	
	return checksum;
}


//___________________________________________________________________________________________
// Sends the contents of the grid flatrecord as a sysex stream
unsigned char send_sysex_grid(){

	unsigned char* pointer = NULL;
	unsigned int i = 0;
	unsigned char checksum = 0;
	 
	// Set the pointer to the beginning of the page record
	pointer = (unsigned char* ) &(memory_image.Grid_flatrecord_buffer);

	// Send the full content of the page record via sysex
	for ( i=0; i < ((unsigned int) sizeof( Grid_flatrecord_struct )); i++ ){
	
		// Compute the running checksum as the bytes are being sent
		checksum ^= byte_to_sysex( (unsigned char) pointer[i] );
	}
	
	return checksum;
}




//___________________________________________________________________________________________
// Dumps the full memory content as sysex data
void my_sysex_dump( unsigned char dump_type, unsigned char pageNdx ){

	unsigned char checksum = 0; // Simple XOR operation over all data bytes

	// Prepare the flat record to be sent out
	switch( dump_type ){
	
		case _PAGE_SYSEX_DUMP:

			// Move the memory content of the given page to the flat file image, as if saving in flash
			Page_export_to_flatrecord_buffer( 	&Page_repository[ pageNdx ], 
											    &(memory_image.Page_flatrecord_repository[ pageNdx ]) );
			break;

		case _GRID_SYSEX_DUMP:
			
			// Move the memory content of the given page to the flat file image, as if saving in flash
			Grid_export_to_flatrecord_buffer();
			break;
	}

	// SYSEX INIT SEQUENCE
	send_sysex( 0xF0 );
 
	// Send the manufacturer ID
	send_sysex( 0x00 );
	send_sysex( 0x20 );
	send_sysex( 0x64 );

	// Send the manufacturer message type
	send_sysex( dump_type );

	// Return the checksum from sending the page record
	switch( dump_type ){
	
		case _PAGE_SYSEX_DUMP:
			checksum = send_sysex_page( pageNdx );	
			break;

		case _GRID_SYSEX_DUMP:
			checksum = send_sysex_grid();	
			break;
	}

	// Send the checksum
	send_sysex( checksum );

	// Send sysex terminator
	send_sysex( 0xF7 );

	// Clear the used page flatbuffer - leave it empty
	Page_flatrecord_buffer_init( &(memory_image.Page_flatrecord_repository[ pageNdx ]) );
	Grid_flatrecord_buffer_init();
}


//____________________________________________________________________________________________
// Reads the flash content and outputs it as MIDI sysex stream
void my_flash_content_dump( unsigned char dump_type ){

	unsigned int i = 0; // caution: will take on very large values
	unsigned char checksum = 0; // Simple XOR operation over all data bytes
	unsigned int length = 0;
	unsigned int base_address = 0;

	switch( dump_type ){
	
		case _CONFIGFILE_SYSEX_DUMP:
			base_address = MY_CONFIGFILE_BASE;
			length = CONFIGFILE_SIZE;
			break;

		case _OS_SYSEX_DUMP:
		default:
			base_address = MY_EXECUTABLE_BASE;
			length = OSFILE_SIZE;	// Used when dumping existing OS file from flash
//			length = 0x00050000;	// Used when dumping itself from flash
			break;		
	}

	// Copy the executable binary into the holder array
	memcpy( (void*) sysex_stream_keeper, (void*) base_address, length );

	// SYSEX INIT SEQUENCE
	send_sysex( 0xF0 );

	// Send the manufacturer ID
	send_sysex( 0x00 );
	send_sysex( 0x20 );
	send_sysex( 0x64 );

	// Send the manufacturer message type
	send_sysex( dump_type );

	// Send the contents of the array
	for( i=0; i < length; i++ ){

		// Compute the running checksum
		checksum ^= byte_to_sysex( sysex_stream_keeper[i] );
	}

	// Send the checksum
	send_sysex( checksum );

	// Send sysex terminator
	send_sysex( 0xF7 );

//d_iag_printf( " SYSEX FLASH DUMP ok: type: %x base:%x len:%x checksum:%x\n", 
//	dump_type, base_address, length, checksum );

}

