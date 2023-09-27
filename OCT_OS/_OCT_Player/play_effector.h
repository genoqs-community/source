

	// 
	// EFF pool generation - attribute offset - regardless of the active steps in track
	//
	switch( Track_get_MISC( target_page->Track[ phys_row ], EFF_BIT ) ){

		// Move the EFF values into the EFF pool.
		// The EFF value is just an offset from the default attribute value, therefore - STEP_DEF_xx
		case SEND:

			// This means that EFF_pool has been already filled, no need to continue		
			if ( multiplier_flag != 0 ) break;


			// VEL
			#ifdef EVENTS_FACTORIZED
			target_page->EFF_pool[ ATTR_VELOCITY ] = normalize(
					target_page->EFF_pool[ ATTR_VELOCITY ]
					+ (	( 	target_page->Step[phys_row][locator-1]->attr_VEL
						* Track_VEL_factor[	target_page->Track[head_row]->VEL_factor
						+ target_page->Track[phys_row]->event_offset[ATTR_VELOCITY] ]
						/ VEL_FACTOR_NEUTRAL_VALUE	)
					- STEP_DEF_VELOCITY	),
					EFF_POOL_MIN, EFF_POOL_MAX );
			#endif // EVENTS_FACTORIZED
			
			#ifdef EVENTS_ABSOLUTE
			target_page->EFF_pool[ ATTR_VELOCITY ] = normalize(
					target_page->EFF_pool[ ATTR_VELOCITY ]
					+ (	(	target_page->Step[phys_row][locator-1]->attr_VEL
						*  Track_VEL_factor[	target_page->Track[head_row]->VEL_factor ]
						/ VEL_FACTOR_NEUTRAL_VALUE	)
					- STEP_DEF_VELOCITY	),
					EFF_POOL_MIN, EFF_POOL_MAX );
			#endif // EVENTS_ABSOLUTE



			// PIT
			#ifdef EVENTS_FACTORIZED
			//________FACTORIZED version
			target_page->EFF_pool[ ATTR_PITCH ] = normalize(
					target_page->EFF_pool[ ATTR_PITCH ]
					+ ( ( 	target_page->Step[phys_row][locator-1]->attr_PIT
						*  Track_PIT_factor[	target_page->Track[head_row]->PIT_factor
						+ target_page->Track[phys_row]->event_offset[ATTR_PITCH] ]
						/ PIT_FACTOR_NEUTRAL_VALUE )
					- STEP_DEF_PITCH	),
					EFF_POOL_MIN, EFF_POOL_MAX );
			#endif // EVENTS_FACTORIZED

			#ifdef EVENTS_ABSOLUTE
			//________ABSOLUTE version
			target_page->EFF_pool[ ATTR_PITCH ] = normalize(
					target_page->EFF_pool[ ATTR_PITCH ]
					+ (	( 	target_page->Step[phys_row][locator-1]->attr_PIT
						*  Track_PIT_factor[ target_page->Track[head_row]->PIT_factor ]
						/ PIT_FACTOR_NEUTRAL_VALUE	)
					- STEP_DEF_PITCH	)
					EFF_POOL_MIN, EFF_POOL_MAX );
			#endif // EVENTS_ABSOLUTE



			// LEN
			target_page->EFF_pool[ ATTR_LENGTH ] = normalize(
					target_page->EFF_pool[ ATTR_LENGTH ]
					+ (	( ( (unsigned char) target_page->Step[phys_row][locator-1]->attr_LEN	)
						*  Track_LEN_factor[	target_page->Track[head_row]->LEN_factor
						+ target_page->Track[phys_row]->event_offset[ATTR_LENGTH] ]
						/ LEN_FACTOR_NEUTRAL_VALUE )
					- STEP_DEF_LENGTH	),
					EFF_POOL_MIN, EFF_POOL_MAX );
			break;



		// Get the EFF values out of the EFF pool
		case RECEIVE:

			#ifdef FEATURE_EFF_LISTENER_MASK
				// Listener Step Mask
				// AMT value of -127 is arbitary as it is not possible to negatively Scale attr_AMT via Step Event
				// (See legacy CBB known issue case #2)
				if ( target_page->Step[phys_row][locator-1]->attr_AMT == -127 )  break;
			#endif //FEATURE_EFF_MASK_RECEIVE

			// Only these for now, the usual suspects. PIT and VEL watch their interval ranges!
			EFF_pool_VEL = target_page->EFF_pool[ ATTR_VELOCITY ];
			EFF_pool_PIT = target_page->EFF_pool[ ATTR_PITCH ];
			EFF_pool_LEN = target_page->EFF_pool[ ATTR_LENGTH ];
			break;



		// Send and receive the values at the same time
		case RECEIVESEND:

			#ifdef FEATURE_EFF_LISTENER_MASK
				// Listener Step Mask
				// AMT value of -127 is arbitary as it is not possible to negatively Scale attr_AMT via Step Event
				// (See legacy CBB known issue case #2)
				if ( target_page->Step[phys_row][locator-1]->attr_AMT == -127 )  break;
			#endif //FEATURE_EFF_MASK_RECEIVE

			// Only these for now, the usual suspects. PIT and VEL watch their interval ranges!
			EFF_pool_VEL = target_page->EFF_pool[ ATTR_VELOCITY ];
			EFF_pool_PIT = target_page->EFF_pool[ ATTR_PITCH ];
			EFF_pool_LEN = target_page->EFF_pool[ ATTR_LENGTH ];

			// Play them into the pool
			// This means that EFF_pool has been already filled, no need to continue		
			if ( multiplier_flag != 0 ) break;


			// VEL
			#ifdef EVENTS_FACTORIZED
			target_page->EFF_pool[ ATTR_VELOCITY ] = normalize(
					target_page->EFF_pool[ ATTR_VELOCITY ]
					+ (	( 	target_page->Step[phys_row][locator-1]->attr_VEL
						*  Track_VEL_factor[	target_page->Track[head_row]->VEL_factor
						+ target_page->Track[phys_row]->event_offset[ATTR_VELOCITY] ]
						/ VEL_FACTOR_NEUTRAL_VALUE )
					- STEP_DEF_VELOCITY	),
					EFF_POOL_MIN, EFF_POOL_MAX );
			#endif // EVENTS_FACTORIZED
			
			#ifdef EVENTS_ABSOLUTE
			target_page->EFF_pool[ ATTR_VELOCITY ] = normalize(
					target_page->EFF_pool[ ATTR_VELOCITY ]
					+ (	( 	target_page->Step[phys_row][locator-1]->attr_VEL
						*  Track_VEL_factor[	target_page->Track[head_row]->VEL_factor ]
						/ VEL_FACTOR_NEUTRAL_VALUE )
					- STEP_DEF_VELOCITY	),
					EFF_POOL_MIN, EFF_POOL_MAX );
			#endif // EVENTS_ABSOLUTE



			// PIT
			#ifdef EVENTS_FACTORIZED
			//________FACTORIZED version
			target_page->EFF_pool[ ATTR_PITCH ] = normalize(
					target_page->EFF_pool[ ATTR_PITCH ]
					+ (	( 	target_page->Step[phys_row][locator-1]->attr_PIT
						*  Track_PIT_factor[	target_page->Track[head_row]->PIT_factor
						+ target_page->Track[phys_row]->event_offset[ATTR_PITCH] ]
						/ PIT_FACTOR_NEUTRAL_VALUE )
					- STEP_DEF_PITCH	),
					EFF_POOL_MIN, EFF_POOL_MAX );
			#endif // EVENTS_FACTORIZED
			
			#ifdef EVENTS_ABSOLUTE
			//________ABSOLUTE version
			target_page->EFF_pool[ ATTR_PITCH ] = normalize(
					target_page->EFF_pool[ ATTR_PITCH ]
					+ (	( 	target_page->Step[phys_row][locator-1]->attr_PIT
						*  Track_PIT_factor[ target_page->Track[head_row]->PIT_factor ]
						/ PIT_FACTOR_NEUTRAL_VALUE )
					- STEP_DEF_PITCH	),
					EFF_POOL_MIN, EFF_POOL_MAX );
			#endif // EVENTS_ABSOLUTE



			// LEN
			target_page->EFF_pool[ ATTR_LENGTH ] = normalize(
					target_page->EFF_pool[ ATTR_LENGTH ]
					+ (	(	( (unsigned char) target_page->Step[phys_row][locator-1]->attr_LEN)
						*  Track_LEN_factor[	target_page->Track[head_row]->LEN_factor
						+ target_page->Track[phys_row]->event_offset[ATTR_LENGTH] ]
						/ LEN_FACTOR_NEUTRAL_VALUE )
					- STEP_DEF_LENGTH	),
					EFF_POOL_MIN, EFF_POOL_MAX );
			break;

	}
	// EFF pool generation - switch on EFF bit of track




