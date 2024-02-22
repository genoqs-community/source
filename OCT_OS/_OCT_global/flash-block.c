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

#include "../_OCT_global/flash-block.h"
#include "../_OCT_objects/Persistent.h"


// Buffer used when reading/writing flash page blocks.
card32 AlignedFlashBlockBuffer[ FLASH_BLOCK_SIZE / sizeof(card32) ];
card8* FlashBlockBuffer = (card8*) &AlignedFlashBlockBuffer;



/**
 * Reads flash block with given block ID into FlashBlockBuffer.
 *
 * @author Robert van der Kamp
 */
void Flash_read_block( int blockId )
{
	unsigned char* sourcePt = ((unsigned char*) MY_FLASH_BASE) + (blockId * FLASH_BLOCK_SIZE);
	unsigned char* targetPt = FlashBlockBuffer;
	unsigned int   err = 0;
	unsigned int*  err_address = &err;

	cyg_mutex_lock( &mutex_FlashOperations );

	err = flash_read( (void *) sourcePt, (void*) targetPt, FLASH_BLOCK_SIZE, (void**) err_address );

	cyg_mutex_unlock( &mutex_FlashOperations );

	//d_iag_printf( "rb-%d-%p-%02x%02x%02x ", blockId, sourcePt, targetPt[0], targetPt[1], targetPt[2] );
}



/**
 * Writes flash block currently stored in FlashBlockBuffer to flash as block #blockId.
 * Interrupts are disabled during the writing process.
 *
 * @author Robert van der Kamp
 */
void Flash_write_block( int blockId )
{
	unsigned char* targetPt = ((unsigned char*) MY_FLASH_BASE) + (blockId * FLASH_BLOCK_SIZE);
	unsigned char* sourcePt = FlashBlockBuffer;
	unsigned int   err = 0;
	unsigned int*  err_address = &err;

	//d_iag_printf( "wb-%d-%p-%p ", blockId, sourcePt, targetPt );

	cyg_interrupt_disable();
	cyg_mutex_lock( &mutex_FlashOperations );

	err = flash_erase( targetPt, FLASH_BLOCK_SIZE, (void**) err_address );
	err = flash_program( (void*) targetPt, (void*) sourcePt, FLASH_BLOCK_SIZE, (void**) err_address );

	cyg_mutex_unlock( &mutex_FlashOperations );
	cyg_interrupt_enable();

	//d_iag_printf( "done\n" );
}



/**
 * Reads the singleton Grid object from flash and updates the global grid data members.
 *
 * @author Robert van der Kamp
 */
void Flash_read_grid( void )
{
	G_master_blinker = ON;

	// The grid object is written in the space of a number of pages, starting at page #MAX_NROF_PAGES.
	int pageId = MAX_NROF_PAGES;
	int blockId = pageId / FLASH_PAGE_PER_BLOCK_COUNT;
	int pageInBlockIx = pageId % FLASH_PAGE_PER_BLOCK_COUNT;

	// Read the flash block that houses the grid object.
	Flash_read_block( blockId );

	// Point sourceGridPt to the grid object in FlashBlockBuffer and import the data.
	const card8* srcGridPt = (card8*) (FlashBlockBuffer + (pageInBlockIx * FLASH_PAGE_SIZE));
	PersGridImport( srcGridPt, FLASH_GRID_SIZE );
}



/**
 * Writes the singleton Grid object to flash.
 *
 * @author Robert van der Kamp
 */
void Flash_write_grid( void )
{
	intn ix;

	G_master_blinker = ON;

	// The grid object is written in the space of a number of pages, starting at page #MAX_NROF_PAGES.
	int pageId = MAX_NROF_PAGES;
	int blockId = pageId / FLASH_PAGE_PER_BLOCK_COUNT;
	int pageInBlockIx = pageId % FLASH_PAGE_PER_BLOCK_COUNT;

	// We must make sure that the block hosting the grid object has enough space available.
	if ( (pageInBlockIx * FLASH_PAGE_SIZE + FLASH_GRID_SIZE) > FLASH_BLOCK_SIZE ) {
		return;
	}

	// Read the flash block housing the grid object.
	Flash_read_block( blockId );

	// Point taretGridPt into the FlashBlockBuffer at the right offset and export the global data into this space.
	card8* targetBufPt = (card8*) (FlashBlockBuffer + (pageInBlockIx * FLASH_PAGE_SIZE));
	PersGridExport( targetBufPt, FLASH_GRID_SIZE );

	// Insert a short delay between the flash read and write call to prevent the system from freezing.
	for ( ix = 0; ix < FLASH_RDWR_DELAY; ix++ );

	// Write the flash block.
	Flash_write_block( blockId );
}



/**
 * Reads the page with given page ID from flash into memory and updates the associated runtime page object.
 *
 * @author Robert van der Kamp
 */
void Flash_read_page( int pageId )
{
	G_master_blinker = ON;

	// Calc the flash block ID from the page ID, and the offset of the wanted page in the block.
	int blockId = pageId / FLASH_PAGE_PER_BLOCK_COUNT;
	int pageInBlockIx = pageId % FLASH_PAGE_PER_BLOCK_COUNT;

	// Read the flash block that houses the given page object.
	Flash_read_block( blockId );

	// Point sourcePagePt into FlashBlockBuffer at the correct offset and import the page into the runtime object.
	card8* sourceBufPt = (card8*) (FlashBlockBuffer + (pageInBlockIx * FLASH_PAGE_SIZE));
	Pagestruct* targetPagePt = &Page_repository[pageId];
	PersPageImport( sourceBufPt, FLASH_PAGE_SIZE, targetPagePt );
}



/**
 * Writes the page with given ID to flash.
 *
 * @author Robert van der Kamp
 */
void Flash_write_page( int pageId )
{
	intn ix;
	//d_iag_printf("page write %d\n", pageId );

	G_master_blinker = ON;

	// Calc the flash block ID from the page ID, and the offset of the wanted page in the block.
	int blockId = pageId / FLASH_PAGE_PER_BLOCK_COUNT;
	int pageInBlockIx = pageId % FLASH_PAGE_PER_BLOCK_COUNT;

	// Read the page block that houses the given page object.
	Flash_read_block( blockId );

	// Export the runtime page into targetPagePt that points into FlashBlockBuffer.
	Pagestruct* sourcePagePt = &Page_repository[pageId];
	card8* targetBufPt = (card8*) (FlashBlockBuffer + (pageInBlockIx * FLASH_PAGE_SIZE));
	PersPageExport( sourcePagePt, targetBufPt, FLASH_PAGE_SIZE );

	#warning RvdK: why do we need this delay to prevent the system from freezing?
	// Insert a short delay between the flash read and write call to prevent the system from freezing.
	for ( ix = 0; ix < FLASH_RDWR_DELAY; ix++ );

	// Write the flash block.
	Flash_write_block( blockId );
	//d_iag_printf("page write %d done\n", pageId );
}



/**
 * Reads all the MAX_NROF_PAGES from flash into memory and updates the associated runtime page objects.
 * Keep a nice lightshow going in the meanwhile by updating the LEDS in the inner circle for status report.
 *
 * @author Robert van der Kamp
 */
void Flash_read_all_pages( void )
{
	int pageId, pageInBlockIx;

	// Make sure no lights go off as they should not
	int orgBlinker = G_master_blinker;
	G_master_blinker = ON;

	// Loop through all of the pages, incrementing with FLASH_PAGE_PER_BLOCK_COUNT steps.
	// This addresses each of the flash blocks that hold pages.

	for ( pageId = 0; pageId < MAX_NROF_PAGES; pageId += FLASH_PAGE_PER_BLOCK_COUNT ) {

		// Update LEDS in inner circle for status report.
		show_progress_bar( 1 + ((pageId * 8) / MAX_NROF_PAGES), MIR_RED );
		VIEWER_show_MIR();

		// Calc the flash block ID from the page ID, and read the flash block.
		int blockId = pageId / FLASH_PAGE_PER_BLOCK_COUNT;
		Flash_read_block( blockId );

		// Now, for all of the pages stored in the flash block just read in...
		for ( pageInBlockIx = 0; pageInBlockIx < FLASH_PAGE_PER_BLOCK_COUNT; pageInBlockIx++ ) {
			// We're done if we reached the MAX_NROF_PAGES limit.
			if ( (pageId + pageInBlockIx) >= MAX_NROF_PAGES ) break;

			// Point sourcePagePt into the FlashBlockBuffer at the right offset and import the page into memory.
			const card8* sourceBufPt = (card8*) (FlashBlockBuffer + (pageInBlockIx * FLASH_PAGE_SIZE));
			Pagestruct* targetPagePt = &Page_repository[pageId + pageInBlockIx];
			PersPageImport( sourceBufPt, FLASH_PAGE_SIZE, targetPagePt );
		}
	}

	G_master_blinker = orgBlinker;
}



/**
 * Writes all the MAX_NROF_PAGES from memory to flash.
 * Keep a nice lightshow going in the meanwhile by updating the LEDS in the inner circle for status report.
 *
 * @author Robert van der Kamp
 */
void Flash_write_all_pages( void )
{
	int pageId, pageInBlockIx;

	// Make sure no lights go off as they should not
	int orgBlinker = G_master_blinker;
	G_master_blinker = ON;

	// Loop through all of the pages, incrementing with FLASH_PAGE_PER_BLOCK_COUNT steps.
	// This addresses each of the flash blocks that hold pages.

	for ( pageId = 0; pageId < MAX_NROF_PAGES; pageId += FLASH_PAGE_PER_BLOCK_COUNT ) {

		// Update LEDS in inner circle for status report.
		show_progress_bar( 1 + ((pageId * 8) / MAX_NROF_PAGES), MIR_RED );
		VIEWER_show_MIR();

		// Calc the flash block ID from page ID and read the flash block.
		// We have to read the block because we want to keep the Grid data
		// alive in the last number of page slots.

		int blockId = pageId / FLASH_PAGE_PER_BLOCK_COUNT;
		Flash_read_block( blockId );

		// Now, for all of the pages to be stored in the flash block just read in...
		for ( pageInBlockIx = 0; pageInBlockIx < FLASH_PAGE_PER_BLOCK_COUNT; pageInBlockIx++ ) {

			// Point targetPagePt into the FlashBlockBuffer at the right offset and export the runtime page into targetPagePt.
			if ( (pageId + pageInBlockIx) < MAX_NROF_PAGES ) {
				Pagestruct* sourcePagePt = &Page_repository[pageId + pageInBlockIx];
				card8* targetBufPt = (card8*) (FlashBlockBuffer + (pageInBlockIx * FLASH_PAGE_SIZE));
				PersPageExport( sourcePagePt, targetBufPt, FLASH_PAGE_SIZE );
			}
		}

		// Write the flash block.
		Flash_write_block( blockId );
	}

	G_master_blinker = orgBlinker;
}

