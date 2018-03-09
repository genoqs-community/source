

			// Don't show anything, and don't react.
			MIR_write_dot( LED_TEMPO, MIR_GREEN );
			MIR_write_dot( LED_TEMPO, MIR_RED   );
			
			if ( CLOCK_SOURCE != EXT ){
				MIR_write_dot( LED_TEMPO, MIR_BLINK );
			}

