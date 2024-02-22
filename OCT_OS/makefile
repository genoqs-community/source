
# Introduce two compile modes: 'dev' and anything else (presumably prod). 
# In dev the output is a symbol table ELF file, 
# in prod one that is ready to be loaded to the machine.
 COMPILE_MODE	= dev


# GS_20240207: adjusted for arm-eabi
# Pointers to the arm-eabi toolchain location
 TOOLPREFIX = /opt/ecos/gnutools/arm-eabi/bin
 CC  		= $(TOOLPREFIX)/arm-eabi-gcc
 STRIP 		= $(TOOLPREFIX)/arm-eabi-strip
# Pointers to the ecos runtime location
 PREFIX		= /opt/genoqs-ecos-runtime
 INCLUDE_PATH  	= /opt/genoqs-ecos-runtime/include

# Pointer to the OCT_OS code: required so Nemo can find the code that it shares with Octopus
 BASELINE_PATH	= /home/genoqs/Dev/genoqs-sources/OCT_OS


# -----------------------------------------------------------------------
# USE THE -O2 FOR FINAL VERSION! THIS GIVES US 2.6 MORE CPU PERFORMANCE
# -----------------------------------------------------------------------

# !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! TOGGLE DEBUG - '-g' flag will corrupt the Octopus memory bank overflow
#CFLAGS = -Wall -O2
CFLAGS = -g -O0

# Source files used in the system.
SOURCES = 	main.c \
			_OCT_objects/PersistentV1.c \
			_OCT_objects/PersistentV2.c \
			_OCT_objects/Persistent.c \
			_OCT_objects/Phrase.c \
			_OCT_objects/Phrase-presets.c \
			_OCT_global/flash-block.c \
			_OCT_interrupts/cpu-load.c

# Object files used in the system.
OBJECTS = $(SOURCES:.c=.o)

all:	octopus.elf

octopus.elf: $(SOURCES)
# !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! TOGGLE DEBUG - '-g' flag will corrupt the Octopus memory overflow
	$(CC) $(CFLAGS) -o octopus.elf -D__ECOS -I$(INCLUDE_PATH) $(SOURCES) -L$(PREFIX)/lib -Ttarget.ld -nostdlib
#	$(CC) $(CFLAGS) -o octopus.elf_DEBUG_DONT_DEPLOY -D__ECOS -I$(INCLUDE_PATH) $(SOURCES) -L$(PREFIX)/lib -Ttarget.ld -nostdlib
# comment this line to produce a symbol table ELF for the step debugger -- do not uplaod to Octopus!!!
# !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! TOGGLE DEBUG - '-g' flag will corrupt the Octopus memory overflow
	$(STRIP) octopus.elf

clean:
	rm -f $(OBJECTS) octopus.elf



main.o: $(SOURCES)
	$(CC) -g -c -I$(INCLUDE_PATH) main.c

_OCT_objects/PersistentV1.o:
	$(CC) -g -c -I$(INCLUDE_PATH) _OCT_objects/PersistentV1.c

_OCT_objects/PersistentV2.o:
	$(CC) -g -c -I$(INCLUDE_PATH) _OCT_objects/PersistentV2.c

_OCT_objects/Persistent.o:
	$(CC) -g -c -I$(INCLUDE_PATH) _OCT_objects/Persistent.c

_OCT_objects/Phrase.o: 
	$(CC) -g -c -I$(INCLUDE_PATH) _OCT_objects/Phrase.c

_OCT_objects/Phrase-presets.o: 
	$(CC) -g -c -I$(INCLUDE_PATH) _OCT_objects/Phrase-presets.c

_OCT_global/flash-block.o:
	$(CC) -g -c -I$(INCLUDE_PATH) _OCT_global/flash-block.c

_OCT_interrupts/cpu-load.o:
	$(CC) -g -c -I$(INCLUDE_PATH) _OCT_interrupts/cpu-load.c


