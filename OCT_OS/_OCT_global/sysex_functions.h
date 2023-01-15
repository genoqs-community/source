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

#include "../_OCT_objects/Persistent.h"
#include "../_OCT_objects/PersistentV1.h"
#include "../_OCT_objects/PersistentV2.h"


//___________________________________________________________________________________________
//Send received byte to Sysex via UART0 only (MIDI out 1).
void send_sysex( unsigned char in_byte ){

	// Assumption is that the byte is already stripped accordingly (7 bits rule)
	HAL_WRITE_UINT32( E7T_UART0_BASE + E7T_UART_TXBUF, in_byte );
	Player_wait( E7T_UART0_BASE );
}



//___________________________________________________________________________________________
// Assembles one byte from two given nibbles.
unsigned char sysex_to_byte( unsigned char high_nibble, unsigned char low_nibble ){

	return (unsigned char) ( ((high_nibble << 4) & 0xF0) | (low_nibble & 0x0F) );
}



//___________________________________________________________________________________________
// Sends bytes of data as MIDI sysex dividing it into nibbles nibbles, returning the checksum
unsigned char byte_to_sysex( unsigned char in_char ){

	unsigned char high_nibble = ( 0xf0 & in_char ) >> 4;
	unsigned char low_nibble  = 0x0f & in_char;

	send_sysex( high_nibble );
	send_sysex( low_nibble	);

	// Sysex speed regulator
	my_wait( (SYSEX_DUMP_SPEED_MAX - (G_master_tempo/2)) * 50 );

	return ( high_nibble ^ low_nibble );
}


//___________________________________________________________________________________________
// Sends shorts of data as MIDI sysex dividing it into nibbles nibbles, returning the checksum
unsigned char short_to_sysex( unsigned short in_short ){

	unsigned char checksum 	= 0;
	unsigned char high_char = ( 0xFF00 & in_short ) >> 8;
	unsigned char low_char  = ( 0x00FF & in_short );

	checksum ^= byte_to_sysex( high_char );
	checksum ^= byte_to_sysex( low_char  );

	return checksum;
}


//___________________________________________________________________________________________
// Sends ints of data as MIDI sysex dividing it into nibbles nibbles, returning the checksum
unsigned char int_to_sysex( unsigned int in_int ){

	unsigned char checksum 	= 0;
	unsigned short high_short = ( 0xFFFF0000 & in_int ) >> 16;
	unsigned short low_short  = ( 0x0000FFFF & in_int );

	checksum ^= short_to_sysex( high_short );
	checksum ^= short_to_sysex( low_short  );

	return checksum;
}


 card8 range_to_sysex( const card8* dataPt, size_t rangeSizeBt )
 {
 	card8 checkSum = 0;
 	const card8* endPtEx = dataPt + rangeSizeBt;
 	while ( dataPt < endPtEx ) {
 		checkSum ^= byte_to_sysex( *dataPt++ );
  	}
 	return checkSum;
  }


 unsigned char send_sysex_page( const Pagestruct* sourcePagePt )
 {
  	static PERSISTENT_CUR_PAGE_TP persistentPage;
  	PersPageExport( sourcePagePt, (card8*) &persistentPage, sizeof(persistentPage) );
  	return range_to_sysex( (const card8*) &persistentPage, sizeof(persistentPage) );
 }


 unsigned char send_sysex_grid()
 {
 	static PERSISTENT_CUR_GRID_TP persistentGrid;
 	PersGridExport( (card8*) &persistentGrid, sizeof(persistentGrid) );
 	return range_to_sysex( (const card8*) &persistentGrid, sizeof(persistentGrid) );
 }


 unsigned char send_sysex_scene()
 {
  	static PERSISTENT_CUR_SCENE_TP persistentScene;
  	PersSceneExport( (card8*) &persistentScene, sizeof(persistentScene) );
  	return range_to_sysex( (const card8*) &persistentScene, sizeof(persistentScene) );
 }


//___________________________________________________________________________________________
// Sends the Sysex init sequence
unsigned char send_sysex_init( unsigned char dump_type ){

	unsigned char checksum = 0;

	// Sysex init byte													1
	send_sysex( 0xF0 );

	// Manufacturer ID
	send_sysex( 0x00 );			// Europe / USA ID						2
	checksum ^= 0x00;

	send_sysex( 0x20 );			// Europe ID							3
	checksum ^= 0x20;

	send_sysex( 0x64 );			// genoQs Machines ID					4
	checksum ^= 0x64;

	// Message Type
	send_sysex( dump_type );	// genoQs message type: PAGE or GRID	5
	checksum ^= dump_type;

	// Machine type														6
	#ifdef NEMO
	checksum ^= byte_to_sysex( 0x02 );		// Message comes from Nemo
	#else
	checksum ^= byte_to_sysex( 0x01 );			// Message comes from Octopus
	#endif

	// OS version														7, 8, 9, 10
	checksum ^= short_to_sysex( 	SW_VERSION_MAJOR*100
								+ 	SW_VERSION_MINOR*10
								+ 	SW_VERSION_RELEASE 		);

	return checksum;
}



void my_sysex_dump( unsigned char dump_type, unsigned char pageNdx, int objectIx, int objectCt )
{
	// Show progress if requested.
	if ( objectCt > 0 ) {
		G_master_blinker = ON;

		if ( pageNdx != 0 ) {
			GRID_write_dot( pageNdx, MIR_RED );
			MIR_write_GRID();
		}


		int barPos = ((objectIx + 1) * 8) / objectCt;
		if ( barPos < 1 ) barPos = 1;
		if ( barPos > 8 ) barPos = 8;

		int ix;
		for ( ix = 1; ix <= barPos; ix++ ) {
			show_progress_bar( ix, MIR_GREEN);
			show_progress_bar( ix, MIR_RED);
		}

		VIEWER_show_MIR();
	}


	unsigned char checksum = 0; // Simple XOR operation over all data bytes

	// SYSEX INIT SEQUENCE
	checksum ^= send_sysex_init( dump_type );

	// Return the checksum from sending the page record
	switch( dump_type ){

		case def_PAGE_SYSEX_DUMP:
			//d_iag_printf( "sysex dump page\n");
			checksum ^= send_sysex_page( &Page_repository[pageNdx] );
			break;

		case def_GRID_SYSEX_DUMP:
			//d_iag_printf( "sysex dump grid\n");
			checksum ^= send_sysex_grid();
			break;

		case def_SCENE_SYSEX_DUMP:
			//diag_printf( "sysex dump scene\n");
			checksum ^= send_sysex_scene();
			break;
	}

	// Send the checksum from the transmission
	send_sysex( checksum );

	// SYSEX terminator
	send_sysex( 0xF7 );
}



////____________________________________________________________________________________________
//// Reads the flash content and outputs it as MIDI sysex stream
//void my_flash_content_dump( unsigned char dump_type ){
//
//	unsigned int i = 0; // caution: will take on very large values
//	unsigned char checksum = 0; // Simple XOR operation over all data bytes
//	unsigned int length = 0;
//	unsigned int base_address = 0;
//
//	switch( dump_type ){
//
////		case def_CONFIGFILE_SYSEX_DUMP:
////			base_address 	= MY_CONFIGFILE_BASE;
////			length 			= def_CONFIGFILE_SIZE;
////			break;
//
//		case def_OS_SYSEX_DUMP:
//		default:
//			base_address 	= MY_OSFILE_BASE;
//			length 			= def_OSFILE_SIZE;
//			break;
//	}
//
//	// Copy the executable binary into the holder array
//	memcpy( (void*) G_sysex_stream_keeper, (void*) base_address, length );
//
//	// SYSEX INIT SEQUENCE
//	send_sysex( 0xF0 );
//
//	// Send the manufacturer ID
//	send_sysex( 0x00 );
//	send_sysex( 0x20 );
//	send_sysex( 0x64 );
//
//	// Send the genoQs Machines message type
//	send_sysex( dump_type );
//
//	// Send the contents of the array
//	for( i=0; i < length; i++ ){
//
//		// Compute the running checksum
//		checksum ^= byte_to_sysex( G_sysex_stream_keeper[i] );
//	}
//
//	// Send the checksum
//	send_sysex( checksum );
//
//	// Send sysex terminator
//	send_sysex( 0xF7 );
//}




//#ifdef NEMO
//
////____________________________________________________________________________________________
//// Reads the flash content at the OS position and outputs it as MIDI sysex stream
//void my_OS_dump(){
//
//	unsigned int i = 0; // caution: may take on very large values
//	unsigned int j = 0;
//	unsigned int length = 0;
//	unsigned int base_address = 0;
//	unsigned int passes = 0;
//	unsigned char checksum = 0; // Simple XOR operation over all data bytes
//
//	// Set the coordinates for the dump
//	passes			= 1;	// Make sure this is HEX!
//	length 			= def_OSFILE_SIZE / passes;
//
//	// Set the initial base address
//	base_address 	= MY_OSFILE_BASE;
//
//
//	// SYSEX INIT SEQUENCE
//	//
//	send_sysex( 0xF0 );
//	// Manufacturer ID
//	send_sysex( 0x00 );
//	send_sysex( 0x20 );
//	send_sysex( 0x64 );
//	// genoQs Machines message type
//	send_sysex( def_OS_SYSEX_DUMP );
//
//	// SYSEX transmit the data in several passes, as defined
//	//
//	for ( i=0; i < passes; i++ ){
//
//		// Copy the binary chunk into the stream keeper location
//		memcpy( 	(void*) G_sysex_stream_keeper,
//					(void*) base_address,
//					length );
//
//		// Send the contents of the array
//		for( j=0; j < length; j++ ){
//
//			// Compute the running checksum
//			checksum ^= byte_to_sysex( G_sysex_stream_keeper[j] );
//		}
//
//		// Update the base address for the next pass
//		base_address += length;
//
//	} // Read-and-send passes iterator
//
//	// SYSEX CHECKSUM
//	send_sysex( checksum );
//
//	// SYSEX TERMINATOR
//	send_sysex( 0xF7 );
//
// 	// Wait some time to insert a gap in the recording source. When played back,
// 	// this gaps allows us to process the sysex object without loosing sysex bytes of the next block.
// 	long long int ix;
// 	for ( ix = 0; ix < (50000LL * 20); ix++ ) ix++;
//}
//
//
//
//#else

//____________________________________________________________________________________________
// Reads the flash content at the OS position and outputs it as MIDI sysex stream
void my_OS_dump(){

	unsigned int i = 0; // caution: may take on very large values
	unsigned int j = 0;
	unsigned int length = 0;
	unsigned int base_address = 0;
	unsigned int passes = 0;
	unsigned char checksum = 0; // Simple XOR operation over all data bytes

	// Set the coordinates for the dump
//	passes			= 0x100;	// Make sure this is HEX!
	passes			= 1;
	length 			= def_OSFILE_SIZE / passes;

	// If the stream-keeper buffer isn't large enough to handle length bytes, change length and passes.
	// Note that in normal operation mode, the stream-keeper buffer size is FLASH_BLOCK_SIZE.

//	if ( length > G_sysex_stream_keeper_size ) {
//		length = G_sysex_stream_keeper_size;
//		passes = def_OSFILE_SIZE / length;
//	}

	// Set the initial base address
	base_address 	= MY_OSFILE_BASE;


	// SYSEX INIT SEQUENCE
	//
	send_sysex( 0xF0 );

	// Manufacturer ID
	send_sysex( 0x00 );
	checksum ^= 0x00;

	send_sysex( 0x20 );
	checksum ^= 0x20;

	send_sysex( 0x64 );
	checksum ^= 0x64;

	// genoQs Machines message type
	send_sysex( def_OS_SYSEX_DUMP );
	checksum ^= def_OS_SYSEX_DUMP;

	// SYSEX transmit the data in several passes, as defined
	//
	int bytes_sent = 0;
	for ( i=0; i < passes; i++ ){

		// Copy the binary chunk into the stream keeper location
		memcpy( 	(void*) G_sysex_stream_keeper,
					(void*) base_address,
					length );

// d_iag_printf( "length:0x%X decimal:%d\n", length, length );

		// Send the contents of the array
		for( j=0; j < length; j++ ){

			// Compute the running checksum
			checksum ^= byte_to_sysex( G_sysex_stream_keeper[j] );

			bytes_sent++;
		}

		// Update the base address for the next pass
		base_address += length;

	} // Read-and-send passes iterator

// d_iag_printf( "core bytes sent:%d\n", bytes_sent );

	// SYSEX CHECKSUM
	send_sysex( checksum );

	// SYSEX TERMINATOR
	send_sysex( 0xF7 );


 	// Wait some time to insert a gap in the recording source. When played back,
 	// this gaps allows us to process the sysex object without loosing sysex bytes of the next block.
 	long long int ix;
 	for ( ix = 0; ix < (50000LL * 20); ix++ ) ix++;

}

// #endif




