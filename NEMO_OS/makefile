# GS_20240207: adjusted for arm-eabi
# Pointers to the arm-eabi toolchain location
 TOOLPREFIX = /opt/ecos/gnutools/arm-eabi/bin
 CC  		= $(TOOLPREFIX)/arm-eabi-gcc
 STRIP 		= $(TOOLPREFIX)/arm-eabi-strip
# Pointers to the ecos runtime location
 PREFIX		= /opt/genoqs-ecos-runtime
 INCLUDE_PATH  	= /opt/genoqs-ecos-runtime/include

# Pointer to the OCT_OS code: required so Nemo can find the code that it shares with Octopus
 BASELINE_PATH	= /home/genoqs/Dev/genoqs-sources


# -----------------------------------------------------------------------
# USE THE -O2 FOR FINAL VERSION! THIS GIVES US 2.6 MORE CPU PERFORMANCE
# -----------------------------------------------------------------------

# !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! TOGGLE DEBUG - '-g' flag will corrupt the Octopus memory bank overflow
CFLAGS = -Wall -O2 #-finline-functions
CFLAGS_DEBUG = -g -O0

# Source files used in the system.
SOURCES = 	$(BASELINE_PATH)/NEMO_OS/nemo.c \
			$(BASELINE_PATH)/OCT_OS/_OCT_objects/PersistentV1.c \
			$(BASELINE_PATH)/OCT_OS/_OCT_objects/PersistentV2.c \
			$(BASELINE_PATH)/OCT_OS/_OCT_objects/Persistent.c \
			$(BASELINE_PATH)/OCT_OS/_OCT_objects/Phrase.c \
			$(BASELINE_PATH)/OCT_OS/_OCT_objects/Phrase-presets.c \
			$(BASELINE_PATH)/OCT_OS/_OCT_global/flash-block.c \
			$(BASELINE_PATH)/OCT_OS/_OCT_interrupts/cpu-load.c

# Object files used in the system.
OBJECTS = $(SOURCES:.c=.o)

all: build debug

build: clean.build $(SOURCES)

	# Produce an ELF that can be deployed for production use
	$(CC) $(CFLAGS) -o $(BASELINE_PATH)/NEMO_OS/nemo.elf -D__ECOS -I$(INCLUDE_PATH) $(SOURCES) -L$(PREFIX)/lib -Ttarget.ld -nostdlib
	# Strip the ELF file of all symbols etc.
	$(STRIP) $(BASELINE_PATH)/NEMO_OS/nemo.elf

debug: clean.debug $(SOURCES)

	# This produces the ELF that contains the symbol table used for the step debugger. This ELF should not be uploaded to Nemo!!
	$(CC) $(CFLAGS_DEBUG) -o $(BASELINE_PATH)/NEMO_OS/nemo.elf_DEBUG_DONT_DEPLOY -D__ECOS -I$(INCLUDE_PATH) $(SOURCES) -L$(PREFIX)/lib -Ttarget.ld -nostdlib

clean: clean.build clean.debug

clean.build:

	rm -f $(BASELINE_PATH)/NEMO_OS/nemo.elf $(OBJECTS)

clean.debug:

	rm -f $(BASELINE_PATH)/NEMO_OS/nemo.elf_DEBUG_DONT_DEPLOY

main.o: $(SOURCES)
	$(CC) -g -c -I$(INCLUDE_PATH) $(BASELINE_PATH)/OCT_OS/main.c

nemo.o: $(SOURCES)
	$(CC) -g -c -I$(INCLUDE_PATH) $(BASELINE_PATH)/NEMO_OS/nemo.c

PersistentV1.o:
	$(CC) -g -c -I$(INCLUDE_PATH) $(BASELINE_PATH)/OCT_OS/_OCT_objects/PersistentV1.c

PersistentV2.o:
	$(CC) -g -c -I$(INCLUDE_PATH) $(BASELINE_PATH)/OCT_OS/_OCT_objects/PersistentV2.c

Persistent.o:
	$(CC) -g -c -I$(INCLUDE_PATH) $(BASELINE_PATH)/OCT_OS/_OCT_objects/Persistent.c

Phrase.o:
	$(CC) -g -c -I$(INCLUDE_PATH) $(BASELINE_PATH)/OCT_OS/_OCT_objects/Phrase.c

Phrase-presets.o:
	$(CC) -g -c -I$(INCLUDE_PATH) $(BASELINE_PATH)/OCT_OS/_OCT_objects/Phrase-presets.c

flash-block.o:
	$(CC) -g -c -I$(INCLUDE_PATH) $(BASELINE_PATH)/OCT_OS/_OCT_global/flash-block.c

/cpu-load.o:
	$(CC) -g -c -I$(INCLUDE_PATH) $(BASELINE_PATH)/OCT_OS/_OCT_interrupts/cpu-load.c


