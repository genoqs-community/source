
#include "../_OCT_objects/Persistent.h"



// Gets the chunk of the sysex stream keeper starting at index and of given size
unsigned int get_next_chunk( unsigned int stream_ndx, unsigned int chunk_size ){

	unsigned int i 			= 0;
	unsigned int pattern	= 0;

	// Compile the pattern
	for( i = stream_ndx; i < (stream_ndx + chunk_size); i++ ){

		// Compile the pattern from the bytes of the stream
		pattern += (G_sysex_stream_keeper[i] << ((stream_ndx+chunk_size-1-i)*8) );
	}

	return pattern;
}



// Interprets the contents of the SYSEX stream keeper array
void interpret_SYSEX_stream( unsigned int stream_size ){

	unsigned int chunk_size	= 0;
	unsigned int stream_ndx	= 0;
	unsigned int pattern 	= 0;
	unsigned int objectType = 0;

	// SYSEX header (genoQs) in the first 3 bytes
	chunk_size 	= 3;
	pattern 	= get_next_chunk( stream_ndx, chunk_size );
	stream_ndx 	= stream_ndx + chunk_size;
	if ( pattern != 0x002064 ){
		return;
	}

	// TRANSMISSION TYPE: PAGE, GRID, TRACK, STEP, OS
	chunk_size = 1;
	pattern = get_next_chunk( stream_ndx, chunk_size );
	stream_ndx 	= stream_ndx + chunk_size;

	switch( pattern ){
		// PAGE to be received
		case def_PAGE_SYSEX_DUMP:
			// d_iag_printf( "-> stream type: PAGE\n" );
			G_sysex_header = 0x00206400 + def_PAGE_SYSEX_DUMP;
			objectType = def_PAGE_SYSEX_DUMP;
			break;

		// GRID to be received
		case def_GRID_SYSEX_DUMP:
			//d_iag_printf( "-> stream type: GRID\n" );
			G_sysex_header = 0x00206400 + def_GRID_SYSEX_DUMP;
			objectType = def_GRID_SYSEX_DUMP;
			break;

		// SCENE to be received
		case def_SCENE_SYSEX_DUMP:
			// diag_printf( "-> stream type: SCENE\n" );
			G_sysex_header = 0x00206400 + def_SCENE_SYSEX_DUMP;
			objectType = def_SCENE_SYSEX_DUMP;
			break;

		// OSFILE Dump - indicate completeness
		case def_OS_SYSEX_DUMP:
			// d_iag_printf( "def_OS_SYSEX_DUMP\n" );
			G_sysex_header = 0x00206400 + def_OS_SYSEX_DUMP;
			/* no break */

		case def_CONFIGFILE_SYSEX_DUMP:
			// Tell the system we are done
			// d_iag_printf( "def_CONFIGFILE_SYSEX_DUMP\n" );
			G_zoom_level = zoomDEVICE;
			DEVICE_STATE = UPLOAD_COMPLETE;
			break;
	}

	// No need to proceed if we are in device mode - this is handled elsewhere
	if ( G_zoom_level == zoomDEVICE ) {
		return;
	}

	// MACHINE TYPE
	chunk_size = 1;
	pattern = get_next_chunk( stream_ndx, chunk_size );
	stream_ndx 	= stream_ndx + chunk_size;
	G_sysex_machine_type = pattern;
	switch( pattern ){
		case 0x01:
			//d_iag_printf( "-> machine type: OCTOPUS\n" );
			break;
		case 0x02:
			//d_iag_printf( "-> machine type: NEMO\n" );
			break;
	}

	// OS VERSION
	chunk_size = 2;
	pattern = get_next_chunk( stream_ndx, chunk_size );
	stream_ndx 	= stream_ndx + chunk_size;
	G_sysex_OS_version = pattern;
 	//d_iag_printf( "-> OS version:%d %x\n", G_sysex_OS_version, pattern );

	 	// Decode the object received through sysex.
	 	switch( objectType ) {
	 		case def_PAGE_SYSEX_DUMP:
	 		{
	 			// We use this static page object buffer to get the incoming sysex data properly ALIGNED!
	 			static PERSISTENT_LARGEST_PAGE_TP pageObj;

	 			// Copy the sysex data into our static (aligned) page object buffer.
	 			const card8* srcDataPt = G_sysex_stream_keeper + stream_ndx;
	 			size_t srcDataSizeBt = stream_size - stream_ndx;
	 			size_t copyBtCt = min( sizeof(pageObj), srcDataSizeBt );
	 			memcpy( &pageObj, srcDataPt, copyBtCt );

	 			// Decode the persistent page received. Note the null pointer for target page pointer.
	 			// This is how we tell the page import code to find the associated runtime page.
	 			PersPageImport( (card8*) &pageObj, copyBtCt , NULL );
	  		}
	 		break;

	 		case def_GRID_SYSEX_DUMP:
	 		{
				// We use this static grid object buffer to get the incoming sysex data properly ALIGNED!
	 			static PERSISTENT_LARGEST_GRID_TP gridObj;

	 			// Copy the sysex data into our static (aligned) grid object buffer.
	 			const card8* srcDataPt = G_sysex_stream_keeper + stream_ndx;
	 			size_t srcDataSizeBt = stream_size - stream_ndx;
	 			size_t copyBtCt = min( sizeof(gridObj), srcDataSizeBt );
	 			memcpy( &gridObj, srcDataPt, copyBtCt );

	 			// Decode the persistent grid object received. Note that at this point we do not know
	 			// the version/size of the received grid object.

	 			PersGridImport( (card8*) &gridObj, copyBtCt );
	 		}
	 		break;

	 		case def_SCENE_SYSEX_DUMP:
	 		{
	 			// We use this static grid object buffer to get the incoming sysex data properly ALIGNED!
	 			static PERSISTENT_LARGEST_SCENE_TP sceneObj;

	 			// Copy the sysex data into our static (aligned) scene object buffer.
	 			const card8* srcDataPt = G_sysex_stream_keeper + stream_ndx;
	 			size_t srcDataSizeBt = stream_size - stream_ndx;
	 			size_t copyBtCt = min( sizeof(sceneObj), srcDataSizeBt );
	 			memcpy( &sceneObj, srcDataPt, copyBtCt );

	 			// Decode the persistent grid object received. Note that at this point we do not know
	 			// the version/size of the received grid object.

	 			PersSceneImport( (card8*) &sceneObj, copyBtCt );
	 		}
	 		break;
	 	}

}




