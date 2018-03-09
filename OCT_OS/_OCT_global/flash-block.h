#ifndef FLASHBLOCK_H_
#define FLASHBLOCK_H_

#include "../_OCT_global/includes-declarations.h"
#include "../_OCT_objects/Runtime.h"





// Size of flash page block in bytes. Holds several pages. Must be power of two.
// We have 15 of these blocks available for data storage.

#define FLASH_BLOCK_SIZE				(1024 * 64)

// Number of pages stored in a flash page block. 
// 15 flash blocks will thus give us 15 * 11 = 165 page blocks.
// 160 of these are needed for actual page objects. The other 5 are available for the grid.

#define FLASH_PAGE_PER_BLOCK_COUNT 		11

// Max size of a single page in a page block, and a multiple of 8 for alignment. 
// 11 of these fit in a page block.

#define FLASH_PAGE_SIZE					5952

// Max size of the persistent grid object.
// We use several page 'slots' to store the bigger grid object.

#define FLASH_GRID_SIZE					(FLASH_PAGE_SIZE * 5)


// Counter value for a simple delay loop used to prevent system freeze when doing a flash read followed by a write.
#define FLASH_RDWR_DELAY				50000LL



// Function prototypes.
extern void 			Flash_read_page( int pageId );
extern void 			Flash_read_all_pages( void );
extern void 			Flash_read_grid( void );
extern void 			Flash_write_page( int pageId );
extern void 			Flash_write_all_pages( void );
extern void 			Flash_write_grid( void );




#endif /*FLASHBLOCK_H_*/
