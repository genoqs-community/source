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





// FUNCTIONS: Defines for the function activation for panel use
#define TGGL			1<<0
#define SOLO			1<<1
#define CLEAR			1<<2
#define RNDMZ			1<<3
#define FLT				1<<4
#define	RMX				1<<5
#define EFF				1<<6
#define ZOOM			1<<7
#define COPY			1<<8
#define	PASTE			1<<9
#define MODIFY			1<<10


// FUNCTION KEYS: get rid of index memorizing..
#ifdef NEMO
	#define KEY_TGGL		241
	#define	LED_TGGL		241

	#define KEY_CHAIN		187
	#define LED_CHAIN		187	
#else
	#define KEY_TGGL		187
	#define	LED_TGGL		187
#endif


#ifdef NEMO
	#define KEY_SOLO		164
	#define LED_SOLO		164

	#define KEY_FLT			188
	#define LED_FLT			188
#else
	#define KEY_SOLO		188
	#define LED_SOLO		188

	#define KEY_FLT			191
	#define LED_FLT			191
#endif


#ifdef NEMO
	#define	KEY_CLEAR		190
	#define	LED_CLEAR		190
	
	#define	KEY_COPY		65
	#define LED_COPY		65

	#define KEY_PASTE		76
	#define LED_PASTE		76

	#define	KEY_RMX			189
	#define LED_RMX			189
#else
	#define	KEY_CLEAR		189
	#define	LED_CLEAR		189

	#define KEY_RANDOMIZE	190
	#define	LED_RANDOMIZE	190

	#define	KEY_COPY		195
	#define LED_COPY		195

	#define KEY_PASTE		196
	#define LED_PASTE		196

	#define	KEY_RMX			192
	#define LED_RMX			192
#endif


#define KEY_EFF			193
#define LED_EFF			193

#define KEY_ZOOM		194
#define	LED_ZOOM		194

#define CHECK_BIT(var,pos) ((var) & (1<<(pos)))
#define TOGGLE_BIT(var,pos) ((var) = ((var) ^ (1<<(pos))))
#define SET_BIT(var,pos) ((var) = ((var) | (1<<(pos))))
#define CLEAR_BIT(var,pos) ((var) = ((var) & ~(1<<(pos))))
#define SET_BIT_VALUE(var,pos,value) ((value>0)?SET_BIT(var,pos):CLEAR_BIT(var,pos))

#define CHECK_MASK(var,mask) (((var) & (mask)) == mask)
#define SET_MASK(var,mask) (var = (var | mask))
#define APPLY_MASK(var,mask) (var = (var & mask))
#define TOGGLE_MASK(var,mask) (var = (var ^ mask))
#define CLEAR_MASK(var,mask) (var = (var & ~mask))

// Round up providing a width with an even power of 2, will return a number equal to or greater than n, which is a multiple of width, and which is the smallest value meeting that criteria
#define ROUNDUP(n,width) (((n) + (width) - 1) & ~((width) - 1))

// Remap range.
#define REMAP_RANGE( low1, high1, low2, high2, value ) ( low2 + ( value - low1 ) * ( high2 - low2 ) / ( high1 - low1 ) )
