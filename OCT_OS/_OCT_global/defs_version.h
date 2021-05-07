

#define 	SW_VERSION_MAJOR				 0	// Max 10 - Hundreds
#define		SW_VERSION_MINOR				 5	// Max 10 - Tens
#define		SW_VERSION_RELEASE				 2	// Max 10 - Ones

#define		SW_VERSION_INTERNAL				 4	// Max 127 - Row 0 number

// SWITCH between 'MIDI' and 'USB' modes of the MIDI 2 Port
#define		OCTDEF_UART2_MODE	 			MIDI
//#define		OCTDEF_UART2_MODE	 			USB
// POWER-ON LOAD BEHAVIOR
#define 	LOAD_ON_POWERON					TRUE

// BUILD CE OS WITH ADDON FEATURES
//#define 	CE_OS_ADDON_BUILD				TRUE

#ifdef CE_OS_ADDON_BUILD

#ifdef NEMO
// NEMO FEATURE ENABLE
//#define 	FEATURE_ENABLE_DICE				TRUE

#else
// OCTOPUS FEATURE ENABLE
//#define 	FEATURE_ENABLE_SONG_UPE			TRUE
//#define 	FEATURE_ENABLE_KEYB_TRANSPOSE   TRUE

#endif

#endif

// CORE BUILD FEATURE ENABLE
#define 	FEATURE_ENABLE_CHORD_OCTAVE		TRUE
#define		FEATURE_SOLO_REC				FALSE
#define		FEATURE_NOTE_DRUM_CTRL			TRUE


/*
 * Bug tracking spreadsheet
 * https://docs.google.com/spreadsheets/d/1eM8S2VbLz8OsRawqJ4Pzusfqcv13wKgeyLU071252Nk/edit#gid=520555008
 *
 * COMMUNITY EDITION VERSION CHANGES
 *  0.0.5.25		Port Nemo Device globals, OTM fixes (cases v5.25), Nemo/Oct platform integration & UI/UX refinements)
 *  0.0.5.24		Port on the measure / cluster mute (Complete OTM implementation with mute, solo and master)
 *  0.0.5.00		FINAL VERSION of 0.0.5
 *
 *  0.0.4.64	- 	Cases : 1.62x (64-68)
 *
 *  0.0.4.63	- 	Cases : 1.62x (62-63)
 *  						Nemo x2 (41-47)
 *
 *  0.0.4.62	- 	Cases : 1.62x (55-61)
 *  						Nemo x2 (41-44)
 *
 *  0.0.4.61	- 	Cases : 1.62x (52-54)
 *  				Performance tuning
 *
 *  0.0.4.60	- 	Merge SONG UPE addon feature (octopus UI only)
 *
 * 	0.0.4.50	-   Grid Page Cluster Selections:
 *				    implement grid page cluster select/move/copy/clear
 *				    implement grid page cluster selection state for MUT operations
 *					implement continuous play chain move/clobber
 *    				implement scene export/import functionality
 * 	  	  	  	    implement button to blink control track containing pages in grid
 * 				-	Track Events:
 *					fix: stop event toggles grid page - don't toggle if already stopped
 *					fix: enable mute functionality for track program change, etc. events
 *					enable track events for internal scene change
 *					enable track events for internal track mute toggle
 *					implement track events for internal page cluster clear
 *    				implement track events for internal page cluster move
 *					implement track events for external start/stop MIDI (FA, FC)
 *					implement control track indicator green for track select column in page view
 *					implement track events for target page tempo modifier inherit
 *					implement control track step buffer
 *				-   Measure Independent Control Track:
 *					expand page measure repeat from max 16 to max 127
 *					have the current measure repeat count indicator progress forward rather than count down
 *					implement control track event measure start offset
 *					control track event as single use
 *					implement control track independent repeat count
 *				- 	Stop | Pause Behavior:
 *					pressing stop maintains page cluster page and repeat position
 *					double click stop rolls all page chains back to start position
 *					pause may be pressed while the machine is stopped in grid view
 * 			  	- 	Measure Replay Marker:
 *					enable all UI indicators
 *					press [pause] to enter PMLS transport area button override context
 *					pressing ALN sets the active replay marker taken from the value of the transpose LEDs
 *					pressing ALN jumps the measure locator
 *					chord buttons set measure repeats interval value
 *					pressing [stop] while paused in grid mode will jump the measure locator to the active replay marker
 *					[!play] to increment the current measure value by 1
 *					[X-del] to decrement the current measure value by 1
 *					override 4 track chain presets buttons to indicate 1/4 measure nibble value - press button to set
 *					override numeric LED indicator section + rotary to set the current measure in units of 4 measures
 *				- 	MISC:
 *					fix page clear so that it erases all set page parameters
 *					record [MIDI] lost after MIDI channel change
 *					implement GRID - mute all head clusters event (everything unselected)
 *					use TRACK->DIR as event type where 1 = page toggle, 2 = mute all
 *  0.0.4.17	- 	Cases : 1.62x (43-51)
 *  0.0.4.16	- 	Cases : 1.62x (32-42)
 *  						Nemo x2 (32-40)
 *  0.0.4.15	- 	Cases : 1.62x (9-13)
 *  						Nemo x2 (17-31)
 *  0.0.4.14	- 	Cases : 1.62x (9-13)
 *  0.0.4.13	- 	Cases : 1.62x (1-8)
 *  0.0.4.12	- 	Cases : Nemo x2 (1-16)
 *  0.0.4.11	- 	implement DICE addon feature (Nemo UI only)
 *  0.0.4.10	- 	implement Nemo x2
 *  0.0.4		-	[feature] Internal Program Change:
 *					{
 *						desc: toggle grid pages w/o needing 2nd sequencer to echo MIDI port 2
 *						use: [track edit]->[double click btn 200]->[set pgmch to 1-16 for grid horizontal pos]->
 *						[set midich to 1-9 for grid vertical pos]
 * 						When track is invoked the grid page will toggle
 * 					}
 *  0.0.3		-	Fix - [unknown contributor] midi slave timing alignment correction (same as 1.69.03)
 *  0.0.2		-	Fix - Track Vol was not being sent with program change (always loud!)
 *  0.0.1		-	Fix - Track edit bank change was not persisting after save
 *  0.0.0		- 	Initial import of 1.62.02
 */

/* VERSION CHANGES
 *  1.69.03		- 	Fix - MIDI Clock glitch (according to Eric Ellis fix)
 *  				Started numbering in a higher space in order to not confuse with other code.
 *  				For example there is presumably a non-official 1.63 in circulation.
 *
 *  1.62.02		-	Fix - Step copy does not copy the hyperstep status of the source step.
 *
 *  1.62.01		-	Fix - shifting tracks would cause pitch problems.
 *  			-	Fix - Nemo double click on playing page in play mode was causing interruption
 *
 * 	--------------------------------------------------------------------------------------
 *  1.62.00		FINAL VERSION of 1.62
 *  1.61.11		-	Fix - FLT bug fixed
 *  			- 	Nemo RMX bug fixed
 *  			- 	Nemo Tempo selector: inverted click logic
 *  				FINAL VERSION OF 1.62
 *
 *  1.61.10		-	Fix - Hypersteps exported and imported via sysex
 *
 *  1.61.09		- 	Fix - Hypersteps saved to flash
 *
 *  1.61.08		PUBLIC BETA 2 of 1.62
 *  			-	Fix - Page double click in GRID EDIT mode works as expected.
 *  			- 	Fix - Page EDIT mode previews steps correctly, for all STA values.
 *  			-	Fix - MIDI clock commands are echoed as expected.
 *  			-	Fix - Hypersteps are copied with pages as they should.
 *
 *  1.61.07		-	Fix - Fixed the overload on start issue.
 *  			-	Fix - Track CLR keeps the adjusted pitch of the track
 *  			- 	Fix - PREVIEW_PERFORM mode supports Step CPY/PST correctly
 *  			-	Fix - Hypersteps may be exempted from sending own note information
 *  			-	Fix - Start and stop of hyped tracks behaves as expected.
 *  			- 	Fix - Page double click in Grid is more toggle resistant.
 *
 *  1.61.06		PUBLIC BETA 1 of 1.62
 *  			- 	Step skipping is possible the new and the legacy way.
 *  			-	Long chords (>192/192) are now played with the correct length
 *  			- 	Step skips will keep the step state after an unskip operation.
 *  			-	Unskip is active in EDIT / preview mode: Step+MUT combination
 *  			-	Step skip via MUT (MUT+Step) acts as toggle for SKIP state
 *  			- 	PREVIEW PERFORM mode introduced for Berlin School type performance
 *  			-	Disabled stubs for the still unused abtn mode.
 *  			-	Cleanup of #include mesh to save memory footprint.
 *
 *  1.61.05		- 	Preliminary fix for the LDT bug of recording in chained tracks.
 *  			-	Recording is now done in the right position on chain wrap positions.
 *  			- 	Playing pulled steps in the first track position is fixed now.
 *  			-	Blinking freq of LEDs is now a bit lower and hence easier on the eyes
 *  			-	Recording in chained tracks with chainsplayhead now records the correct pitch
 *  			-	Reaction speed of GRIDTRACK key presses is now as it should
 *  			- 	Step phrases are now shifted correctly on shifted tracks
 *  			- 	GRID page double click does not mess up the page play status any more
 *
 * 	1.61.04		- 	EDIT MASTER LED is now green in playmode and blinks orange in Edit mode
 * 				- 	Double-click in GRID mode (non-EDIT) does not toggle page status. Safe!
 * 				-	Queue was lost on zoom in GRID play mode - fixed that!
 * 				- 	PgmCh is now sent on the timeout of double-click, not on the single click.
 *
 *  1.61.03		- 	PgmCh sent on queueing in OCLOCK mode compatible with MD (K. Gstettner)
 *  			- 	PgmCh are being sent for head tracks only when page is selected
 *  			- 	PgmCh are sent inside a page when pgmCh tracks become chain heads
 *  			-	When setting a page's locators to 0,0,0, all chains are reset to play head.
 *  			-	Hyperstep length is set to 16/16 when hyper invoked and 1/16 when removed
 *  			-	GRIDTRACK mode matrix key presses are now as responsive as they should be
 *  			-	EDIT LED blinks orange when activated in PAGE and GRIDTRACK mode (Octopus)
 *
 * 	1.61.02		- 	Hypersteps transmit adjusted length (from their track), pitch and velocity
 *				-	Copy and paste steps in the Page EDIT mode.
 *
 *  1.61.01		- 	Introduced SHINE_GREEN and SHINE_RED flags into MIR infrastructure
 * 				-	Introduced the SHINE_X states into the DIAG mode
 * 				- 	Introduced hypersteps. Hypersteps project their length onto hyped tracks
 *
 * 	--------------------------------------------------------------------------------------
 *  1.60.32		FINAL VERSION of 1.60
 *  			-	Events logic glitch removed. POS event working again.
 *
 * 	1.60.31		-	MIDI clock signal now sent prior to sequencer processing
 *
 *  1.60.30		- 	Mutex initialization consolidated
 *
 * 	1.60.29		- 	Tempo accuracy fix and clock drift fix
 *
 * 	1.60.28		- 	MIDI queue mutex lock installed
 *
 * 	1.60.27		- 	Adapted the changes to the Nemo code
 * 				-	Single page save is now done from the GRID via Matrix buttons.
 *
 *  1.60.26		- 	REC GUI changes so we can be consistent with the Nemo model
 *
 *	1.60.25		-	Included last round of patches from Robert - orange LED in phrase edit
 *
 *	1.60.24		- 	Force to scale works for all REC monitoring states
 *				- 	Orange chase light on tracks that have a valid MCC set
 *				- 	CC maps are now all global and only 6 of them
 *				-	The RndAll phrase type fixed to work as it should
 *				-	Negative display convention shows negative with last 3 green LEDs on
 *				-	Step polyphony carry-over fixed for position shifts
 *
 *  1.60.23		- 	POS value survives GRV changes in Step mode
 *
 *	1.60.22		- 	REC visual fixes by Robert
 *
 *  1.60.21		- 	Fixed queue of MIDI-in
 *
 *	1.60.20		- 	Reworked REC capability
 *
 *	1.60.15		-	Issues with Page re-load via sysex have been fixed
 *
 *	1.60.14		-	Step muting done the old way has been removed
 *
 *  1.60.12		- 	Step muting done the inverse way - hold Step and press MUT
 * 				-	Made track SEL sticky on Octopus, for track transposition tricks
 * 				-	Negative STA value bug fixed
 * 				- 	Recording into slow tracks - note length bug fixed
 * 				- 	Track chain cursor confusion when exiting PLAY mode fixed
 * 				-	Track chain play disruption when modifying chains at play time fixed
 * 				-	Negative value display (Octopus) modified from blink to last green dot mode.
 *
 *	1.60.11		-	Released with Nemo pre-shipments - see changes document
 */


