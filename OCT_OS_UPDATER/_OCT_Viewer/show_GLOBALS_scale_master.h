

	// Status of Scale selection. Book pg. 110..
void show_GLOBALS_scale_master( Pagestruct* target_page ){
	
				
		switch( target_page->scaleStatus ) {

			case OFF:
			
				// The available options
				if ( target_page->force_to_scale == OFF ){
		
					MIR_write_dot( LED_SCALE_SEL, MIR_GREEN );
				}
				else{
		
					MIR_write_dot( LED_SCALE_SEL, MIR_RED 	);
					MIR_write_dot( LED_SCALE_SEL, MIR_GREEN );
					MIR_write_dot( LED_SCALE_SEL, MIR_BLINK );
		
					if ( target_page->scaleStatus != OFF ){
						MIR_write_dot( LED_SCALE_MOD, MIR_GREEN );
						MIR_write_dot( LED_SCALE_MOD, MIR_RED );
					}
				}
			
				break;



			case SCALE_MOD:
				// Indicate that SCALE_MOD is selected
				MIR_write_dot( LED_SCALE_MOD, MIR_GREEN );
				MIR_write_dot( LED_SCALE_MOD, MIR_RED   );
				MIR_write_dot( LED_SCALE_MOD, MIR_BLINK );

				// Indicate force-to-scale state
				MIR_write_dot( LED_SCALE_SEL, MIR_RED );

				break;


								
			case SCALE_SEL:
				// SEL is active - now shown in the CAD LED - case is outdated
				MIR_write_dot( LED_SCALE_MYSEL, MIR_GREEN );
				MIR_write_dot( LED_SCALE_MYSEL, MIR_RED	);
				MIR_write_dot( LED_SCALE_MYSEL, MIR_BLINK );

				// MOD is selectable
				MIR_write_dot( LED_SCALE_MOD, MIR_GREEN	);

				// Indicate force-to-scale state
				MIR_write_dot( LED_SCALE_SEL, MIR_RED );


				// PROGRAM: Export contents into track pitches				
				// ..currently disabled

				break;
			

			case SCALE_CAD:

				// CAD is active
				MIR_write_dot( LED_SCALE_CAD, MIR_GREEN );
				MIR_write_dot( LED_SCALE_CAD, MIR_RED	);
				MIR_write_dot( LED_SCALE_CAD, MIR_BLINK );
				break;
				
		} // switch( target_page->scaleStatus );
		
}

