

// 
// Just some playground to figure out the flash story
//
void my_flash_101( unsigned int in_length ) {

	unsigned int i = 0;
	unsigned int err = 0;
	char* err_address = 0;

	// length can now be passed, to experiment a bit
	unsigned int length = in_length;

	unsigned char out_bytes[length];
	unsigned char in_bytes[length] ;

	unsigned int my_flash_base = 0x01900000;
	unsigned int my_some_flash = 0x0182fb70;


	// Disable interrupts
	cyg_interrupt_disable();


	// I N I T
	// Initialize the write bytes and read bytes
	for (i=0; i<10; i++) {
		out_bytes[i] 	= (unsigned char)(i+10);
		in_bytes[i] 	= 0;
	}	
//	d_iag_printf( "initial RAM array contents:\n" );
//	// Display the bytes to be written
//	for(i = 0; i < 10; i++) { 
//		d_iag_printf("out_bytes[%d]:%u in_bytes[%d]:%u\n", 
//			i, out_bytes[i], i, in_bytes[i]);
//	}

	// R E A D  1
	// Read flash content and show it
	err = flash_read(		(void *) my_some_flash, 
							(void *) in_bytes,
							length, 
							(void **) &err_address		);
	d_iag_printf( "\nR1: flash_read content from:0x%x (err:%d)\n", my_some_flash, err);
//	for(i = 0; i < 10; i++) { d_iag_printf("in_bytes[%d]:%u\n", i, in_bytes[i]);  }


//	// R E A D  2
//	// Read some other flash content and show it
//	memcpy( &in_bytes, (void*) my_some_flash, length );
//	d_iag_printf( "\nR2: memcpy'd flash content from:0x%x (err:%d)\n", my_some_flash, err);
//	for(i = 0; i < 10; i++) { d_iag_printf("in_bytes[%d]:%u\n", i, in_bytes[i]);  }
//
//
//	d_iag_printf( "\nsanity check:\n" );
//	for(i = 0; i < 10; i++) { d_iag_printf("out_bytes[%d]:%u in_bytes[%d]:%u\n", 
//			i, out_bytes[i], i, in_bytes[i]);
//	}
	
	
	// R E A D 3
	// Read flash content from target address
	err = flash_read(		(void *) my_flash_base, 
							(void *) in_bytes,
							length, 
							(void **) &err_address		);
	d_iag_printf( "\nR3: flash_read content from:0x%x (err:%d)\n", my_flash_base, err);
//	for(i = 0; i < 10; i++) { d_iag_printf("in_bytes[%d]:%u\n", i, in_bytes[i]);  }


	// R E A D  4
	// Read some other flash content and show it
	memcpy( &in_bytes, (void*) my_flash_base, length );
	d_iag_printf( "\nR4: memcpy'd flash content from:0x%x (err:%d)\n", my_flash_base, err);
//	for(i = 0; i < 10; i++) { d_iag_printf("in_bytes[%d]:%u\n", i, in_bytes[i]);  }


 	// Erase the flash before rewriting it
	err = flash_erase(		(void *) my_flash_base, 
							length,
							(void**) err_address  		);
	// Read the bytes back from flash
	err = flash_read(		(void *) my_flash_base, 
							(void *) in_bytes,
							length, 
							(void **) err_address		);
	d_iag_printf( "\nE1: flash_erase'd content read from:0x%x (err:%d)\n", my_flash_base, err);
//	for(i = 0; i < 10; i++) { d_iag_printf("in_bytes[%d]:%u\n", i, in_bytes[i]);  }
							
							
 	// Re-initialize the out bytes and in bytes
	for (i=0; i<10; i++) {
		out_bytes[i] 	= (unsigned char)(i+10);
		in_bytes[i] 	= 0;
	}	
//	d_iag_printf( "\nsanity check after re-init:\n" );
//	for(i = 0; i < 10; i++) { d_iag_printf("out_bytes[%d]:%u in_bytes[%d]:%u\n", 
//			i, out_bytes[i], i, in_bytes[i]);
//	}


    // write the bytes to the flash
    err = flash_program(	(void *) my_flash_base, 
							(void *) out_bytes, 
							length, 
							(void **) err_address    	);
	// Read the bytes back from flash
	err = flash_read(		(void *) my_flash_base, 
							(void *) in_bytes,
							length, 
							(void **) err_address		);

	
	// Enable Interrupts back again
	cyg_interrupt_enable();
	
	d_iag_printf( "\nP1: flash_program'd content from:0x%x (err:%d)\n", my_flash_base, err);
	for(i = 0; i < 10; i++) { d_iag_printf("in_bytes[%d]:%u\n", i, in_bytes[i]);  }
	d_iag_printf( "\n\n" );

	d_iag_printf( "my_flash_101(): completed with length:%d\n", length );

}





