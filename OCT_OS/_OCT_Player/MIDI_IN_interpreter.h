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





#include "MIDI_IN_functions.h"
#include "MIDI_IN_executer.h"


// Interpreter functions collect the data. Once they have a valid package,
// the package gets passed on for execution by the executor functions.



//_______________________________________________________________________________________
//
// Interprets the midi_byte as a CONTROL message
/* Status: 	0xB0 to 0xBF where the low nibble is the MIDI channel.
 * Data:	Two data bytes follow the Status.
 * 			First:  controller number (0 to 127). Indicates which controller is affected.
 * 			Second: value to which the controller should be set (0 to 127).
 */
void G_midi_interpret_CONTROL( unsigned char midi_byte, unsigned char UART_ndx ){

	#ifdef FEATURE_SOLO_REC
	if ( G_zoom_level == zoomSOLOREC && SOLO_rec_MCC_enabled == OFF ){
		return;
	}
	#endif

	// If data byte was received without prior status byte, fill in the running status
	// but only if the runnig status is valid..
	if ( G_midi_controller_ndx == 0 ){

		if ( G_running_status_byte_IN_UART[ UART_ndx ] != 0xff ){

			// Take the runnning status byte into account
			G_midi_controller_IN_UART[UART_ndx][G_midi_controller_ndx] =
				G_running_status_byte_IN_UART[ UART_ndx ];

			// Update running index of the data bytes
			G_midi_controller_ndx = 1;
		}
	}
	else {
		// Take over the running status byte on the current UART
		G_midi_controller_IN_UART[UART_ndx][0] = G_running_status_byte_IN_UART[ UART_ndx ];
	}

	// Interpret the byte as a DATA byte.
	G_midi_controller_IN_UART[ UART_ndx ][ G_midi_controller_ndx ] = midi_byte;

	// Point the index to the next data byte
	G_midi_controller_ndx = (G_midi_controller_ndx + 1) % 3;

	// Check for the completeness of the note.
	// If the second data byte is not invalid (0xff) then continue..
	if ( G_midi_controller_IN_UART[UART_ndx][2] != 0xff ) {

		midi_controller_execute( 	UART_ndx,
									G_midi_controller_IN_UART[UART_ndx][0],
									G_midi_controller_IN_UART[UART_ndx][1],
									G_midi_controller_IN_UART[UART_ndx][2] 	);

		// Clear the note and the data byte index
		G_midi_controller_IN_UART[UART_ndx][1] = 0xff;
		G_midi_controller_IN_UART[UART_ndx][2] = 0xff;

		G_midi_controller_ndx = 0;
	}
}



//_______________________________________________________________________________________
//
// Interprets the midi_byte as a REALTIME messages
void G_midi_interpret_REALTIME( unsigned char midi_byte ){

	// MTC reference:
	// 0xf8   timing clock
	// 0xf9   undefined			->not implemented
	// 0xfa   start
	// 0xfb   continue
	// 0xfc   stop
	// 0xfd   undefined			->not implemented
	// 0xfe   active sensing	->not implemented
	// 0xff   system reset		->not implemented

 	#define PULSE_HISTORY_CT 	8

	static float pulseTicksVc[PULSE_HISTORY_CT] = { 0 };
	static int pulseIx = 0;
	static float avgPulseTicks = 0;
	float curPulseTicks;


	// EXT clock is a must to continue..
	if ( G_clock_source != EXT ) return;

	// REAL TIME MESSAGES
	switch ( midi_byte ) {

		// CLOCK
		case MIDICLOCK_CLOCK:

			// Drive the sequencer to play..
			// TWICE!! ..as required by the MIDI Clock resolution (we internally run at twice the resolution).
			// Store the number of timer ticks measured for a 24 PPQN pulse.

			curPulseTicks = kickSequencerThread();

			// Having put the sequencer to work, we can now use the measured curPulseTicks to derive the tempo.
			// First store the measured ticks value in pulseTicksVc, which basically works like a fifo that averages
			// the values it contains.

			pulseTicksVc[pulseIx] = curPulseTicks;

			// Everytime we have gathered PULSE_HISTORY_CT new measured pulseTicks values, it's time to recalculate
			// the global tempo, so that we can update the tempo LEDS and global BPM value.

			if ( ++pulseIx >= PULSE_HISTORY_CT ) {
				int ix;

				// Reset the vector scan index.
				pulseIx = 0;

				// Calc the average number of ticks it took for a single 24 PPQN pulse, using the data of the last N measured values.
				avgPulseTicks = 0;
				for ( ix = 0; ix < PULSE_HISTORY_CT; ix++ ) {
					avgPulseTicks += pulseTicksVc[ix];
				}
				avgPulseTicks /= ((float) PULSE_HISTORY_CT);

				// Now convert the average pulse ticks value to tempo in BPM.
				// We first calc the number of ticks it takes for a quarter note (24 pulses, a beat).
				// Next we calc the number of beats we can fit in 60 secs.
				// If the bpm is within range, we update G_master_tempo and recalc the G_TIMER_REFILL
				// value, which is also used in external clock mode for CPU overload protection.

				if ( avgPulseTicks > 0 ) {
					float beatTicks = avgPulseTicks * (float) 24;
					unsigned int bpm = ((TIMER_TICKS_PER_SEC * 60) + ((beatTicks/2)) / beatTicks);
					if ( (bpm >= MIN_TEMPO) && (bpm <= MAX_TEMPO) && (bpm != G_master_tempo) ) {
						G_master_tempo = bpm;
						G_TIMER_REFILL_update();
					}
				}
			}
			break;


		// START
		case MIDICLOCK_START:

			// Only react if running on external clock
			// Need to reset because ...
			#ifdef FEATURE_ENABLE_SONG_UPE
			sequencer_RESET(ON);
			#else
		 	sequencer_RESET(OFF);
			#endif
			midi_clock_in_counter++;

			// Jumpstart the sequencer
			#ifdef FEATURE_SOLO_REC
			if ( G_zoom_level == zoomSOLOREC ){

				if ( SOLO_rec_page != NULL || SOLO_scale_chords_program == ON ){ // A record page cluster is selected

					if ( SOLO_scale_chords_program == ON && hasArpPattern( SOLO_scale_chords_palette_ndx ) == ON ){

						SOLO_rec_rehearsal = ON;
						reset_page_cluster( SOLO_assistant_page );
						GRID_CURSOR = SOLO_assistant_page->pageNdx;
						SOLO_transpose_GRID_CURSOR = GRID_CURSOR;
						sequencer_command_PLAY();
					}
					else if ( G_track_rec_bit == OFF ) { // rehearse

						SOLO_rec_rehearsal = ON;

						if ( SOLO_has_rec == OFF ){
							SOLO_rec_measure_hold = ON;
						}

						if ( G_run_bit == OFF ){

							reset_page_cluster( SOLO_rec_page );
							playSoloRecCluster();
							sequencer_command_PLAY();
						}
					}
					else { // record

						sequencer_command_PLAY();
					}
				}
			}
			else {
				sequencer_START();
			}
			#else
			sequencer_START();
			#endif
			break;


		// CONTINUE
		case MIDICLOCK_CONTINUE:

			G_run_bit 		= ON;
			G_pause_bit 	= OFF;
			break;


		// STOP
		case MIDICLOCK_STOP:

			// First STOP causes a pause, second one stops slave for good.
			switch ( G_run_bit ){
				case ON:
					// This is not a full STOP command, can be pause as well
					//sequencer_command_PAUSE();
					sequencer_HALT();

					// MIDI CLOCK: STOP - echo the first stop command as well (Duncan).
					if (	( G_clock_source == INT )
							||	(	( G_clock_source == EXT )
								&&	MIDICLOCK_PASSTHROUGH == TRUE )
						){

						// Only active when the clock is active or machine is slave
						MIDI_send( MIDI_CLOCK, MIDICLOCK_STOP, 0, 0 );
					}
					break;

				case OFF:
					// This is a full stop for the slave machine.
					// sequencer_command_STOP();
					sequencer_command_STOP();
					break;
			}
			midi_clock_in_counter = 0;

			// Make all normally blinking LEDs visible - the Gene slave bug
			G_master_blinker = ON;

			// Just show the page to refresh the stop update
			Page_requestRefresh();

			break;
	}
}



//_______________________________________________________________________________________
//
// Interprets the midi_byte as part of a NOTE_ON (or NOTE_OFF) message
void G_midi_interpret_NOTE_ON( unsigned char midi_byte, unsigned char UART_ndx ){

	#ifdef FEATURE_SOLO_REC
	// Solo Recording - handle the first note of the initial recording
	breakSoloRecordingMeasureHold();
	#endif
	// Store the byte in the circular array	- just so it can be printed out.
	// DO NOT USE PRINTFS OR THE LIKE HERE, OR THE INPUT IS DELAYED AND SCRAMBLED

	// If data byte was received without prior status byte,
	// .. or the status byte was not filled in yet, fill with in the running status
	if (	( G_midi_data_ndx == 0 ) || ( G_midi_note_IN_UART[UART_ndx][0] == 0xff ) ){

		// Take the runnning status byte into account - can be one of NOTE_ON or NOTE_OFF
		G_midi_note_IN_UART[UART_ndx][0] = G_running_status_byte_IN_UART[ UART_ndx ];

		// Print buffer
		G_midi_in[0] = G_running_status_byte_IN_UART[ UART_ndx ];

		// Update running index of the data bytes
		G_midi_data_ndx = 1;
	}

	// Interpret the byte as a DATA byte.
	G_midi_note_IN_UART[UART_ndx][G_midi_data_ndx] = midi_byte;
	G_midi_in[G_midi_data_ndx] = midi_byte;

	// Point the index to the next data byte
	G_midi_data_ndx++;

	// Check for the completeness of the note.
	if ( G_midi_data_ndx > 2 ) {

//		d_iag_printf( "assembled: st:0x%x pi:0x%x ve:0x%x\n",
//									G_midi_note_IN_UART[UART_ndx][0],
//									G_midi_note_IN_UART[UART_ndx][1],
//									G_midi_note_IN_UART[UART_ndx][2]	);


		// Propagate the change to the current page.
		midi_note_execute( UART_ndx,
				G_midi_note_IN_UART[UART_ndx][0],
				G_midi_note_IN_UART[UART_ndx][1],
				G_midi_note_IN_UART[UART_ndx][2] );

		// Clear the note and the data byte index
		G_midi_note_IN_UART[UART_ndx][0] = 0xff;
		G_midi_note_IN_UART[UART_ndx][1] = 0xff;
		G_midi_note_IN_UART[UART_ndx][2] = 0xff;

		G_midi_data_ndx = 0;
	}

}


//_______________________________________________________________________________________
//
// Interprets the midi_byte as a BENDER / PITCH WHEEL message
/* Status: 	0xE0 to 0xEF where the low nibble is the MIDI channel.
 * Data:	Two data bytes follow the Status. They are combined to a 14-bit value see above.
 */
void G_midi_interpret_BENDER( unsigned char midi_byte, unsigned char UART_ndx ){

	#ifdef FEATURE_SOLO_REC
	if ( G_zoom_level == zoomSOLOREC && SOLO_rec_MCC_enabled == OFF ){
		return;
	}
	#endif

	// If data byte was received without prior status byte, fill in the running status
	if ( G_midi_bender_ndx == 0 ){

		// Apply the runnning status byte
		G_midi_bender_IN_UART[ UART_ndx ][ G_midi_bender_ndx ] =
			G_running_status_byte_IN_UART[ UART_ndx ];

		// Update running index of the data bytes
		G_midi_bender_ndx = 1;
	}

	// Interpret the byte as a DATA byte.
	G_midi_bender_IN_UART[UART_ndx][G_midi_bender_ndx] = midi_byte;

	// Point the index to the next data byte
	// old and bad: G_midi_controller_ndx++;
	// Allow index to take only values of 0,1,2
	// G_midi_bender_ndx = (G_midi_bender_ndx + 1) % 3;

	if ( G_midi_bender_ndx < 2 ) G_midi_bender_ndx++;

	// Check for the completeness of the message.
	// If the second data byte is not invalid (0xff) then we have a note.
	if ( G_midi_bender_IN_UART[ UART_ndx ][ 2 ] != 0xff ) {

		// Execute the bender message -> Record only when the sequencer is running
		midi_bender_execute( 	UART_ndx,
				G_midi_bender_IN_UART[UART_ndx][0],
				G_midi_bender_IN_UART[UART_ndx][1],
				G_midi_bender_IN_UART[UART_ndx][2] 	);

		// Clear the note and the data byte index
		G_midi_bender_IN_UART[UART_ndx][1] = 0xff;
		G_midi_bender_IN_UART[UART_ndx][2] = 0xff;

		// Reset array index
		G_midi_bender_ndx = 0;
	}
}



//_______________________________________________________________________________________
//
// Interprets the midi_byte as data for a CHANNEL PRESSURE message
/* Status: 	0xD0 to 0xDF where the low nibble is the MIDI channel.
 * Data:	One data byte follows the Status. It is the pressure amount, a value from 0(min) to 127(max).
 */
void G_midi_interpret_PRESSURE( unsigned char midi_byte, unsigned char UART_ndx ){

	// If data byte was received without prior status byte, fill in the running status
	if ( G_midi_pressure_ndx == 0 ){

		// Apply the runnning status byte
		G_midi_pressure_IN_UART[ UART_ndx ][ G_midi_pressure_ndx ] =
			G_running_status_byte_IN_UART[ UART_ndx ];

		// Update running index of the data bytes
		G_midi_pressure_ndx = 1;
	}

	// Interpret the byte as a DATA byte.
	G_midi_pressure_IN_UART[UART_ndx][G_midi_pressure_ndx] = midi_byte;

	// The message should be complete, since it uses a single data byte only
	// No need for explicit check of message complexity

	// Execute the pressure message -> Record only when the sequencer is running
	midi_pressure_execute( 	UART_ndx,
			G_midi_pressure_IN_UART[UART_ndx][0],
			G_midi_pressure_IN_UART[UART_ndx][1] 	);

	// Clear the note and the data byte index
	G_midi_pressure_IN_UART[UART_ndx][1] = 0xff;
	G_midi_pressure_IN_UART[UART_ndx][2] = 0xff;

	// Reset the index to 0
	G_midi_pressure_ndx = 0;
}



//_______________________________________________________________________________________
//
unsigned int G_status_byte 	= 0;
unsigned int G_data_byte1 	= 0;
unsigned int G_data_byte2 	= 0;

// Interprets the input MIDI message - as it was sent by a virtual channel
void G_midi_interpret_VIRTUAL( 	unsigned char type,
								unsigned char val0,
								unsigned char val1,
								unsigned char val2,
								unsigned char UART_ndx ){

	unsigned int status_byte 	= 0;
	unsigned int data_byte1 	= 0;
	unsigned int data_byte2 	= 0;
	cyg_handle_t UART_VIRTUAL_mbox_handle = (cyg_handle_t) NULL;

	// UART mailbox assignment
	switch( UART_ndx ){
		case 1:
			UART_VIRTUAL_mbox_handle 	= UART0_IN_mbox_handle;
			break;
		case 2:
			UART_VIRTUAL_mbox_handle 	= UART1_IN_mbox_handle;
			break;
	}

	// Resend bytes to the IN mailboxes
	switch( type ){

		case MIDI_NOTE:
			// Generate status byte: lower byte is the channel
			status_byte = MIDI_CMD_NOTE_ON | (val0 -1);
			// Note number (pitch)
			data_byte1	= val1;
			// Velocity
			data_byte2	= val2;

			// Place the MIDI message in the mailbox, for the interpreter
			cyg_mbox_tryput( UART_VIRTUAL_mbox_handle, &G_status_byte );
			cyg_mbox_tryput( UART_VIRTUAL_mbox_handle, &G_data_byte1 );
			cyg_mbox_tryput( UART_VIRTUAL_mbox_handle, &G_data_byte2 );
			break;


		case MIDI_CC:
			// Generate status byte: lower byte is the channel
			status_byte = MIDI_CMD_CONTROL | (val0 -1);
			// Controller number
			data_byte1	= val1;
			// Controller amount
			data_byte2	= val2;

			// Place the MIDI message in the mailbox, for the interpreter
			cyg_mbox_tryput( UART_VIRTUAL_mbox_handle, &G_status_byte );
			cyg_mbox_tryput( UART_VIRTUAL_mbox_handle, &G_data_byte1 );
			cyg_mbox_tryput( UART_VIRTUAL_mbox_handle, &G_data_byte2 );
			break;


		case MIDI_BENDER:
			// Generate status byte: lower byte is the channel
			status_byte = MIDI_CMD_BENDER | (val0 -1);
			// First 7 bits
			data_byte1	= val1;
			// Second 7 bits
			data_byte2	= val2;

			// Place the MIDI message in the mailbox, for the interpreter
			cyg_mbox_tryput( UART_VIRTUAL_mbox_handle, &G_status_byte );
			cyg_mbox_tryput( UART_VIRTUAL_mbox_handle, &G_data_byte1 );
			cyg_mbox_tryput( UART_VIRTUAL_mbox_handle, &G_data_byte2 );
			break;


		case MIDI_PRESSURE:
			// Generate status byte: lower byte is the channel
			status_byte = MIDI_CMD_CHANNEL_PRESSURE | (val0 -1);
			// Single data byte here
			data_byte1	= val1;

			// Place the MIDI message in the mailbox, for the interpreter
			cyg_mbox_tryput( UART_VIRTUAL_mbox_handle, &G_status_byte );
			cyg_mbox_tryput( UART_VIRTUAL_mbox_handle, &G_data_byte1 );
			break;


		case MIDI_PGMCH:
			// Generate status byte: lower byte is the channel
			status_byte = MIDI_CMD_PGM_CHANGE | (val0 -1);
			// Program Number
			data_byte1	= val1;

			// Place the MIDI message in the mailbox, for the interpreter
			cyg_mbox_tryput( UART_VIRTUAL_mbox_handle, &G_status_byte );
			cyg_mbox_tryput( UART_VIRTUAL_mbox_handle, &G_data_byte1 );
			break;
	}


}



