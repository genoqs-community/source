
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
#CFLAGS = -Wall -O2 #-finline-functions
CFLAGS = -g -O0

# Source files used in the system.
SOURCES = 	$(BASELINE_PATH)/../NEMO_OS/nemo.c \
			$(BASELINE_PATH)/_OCT_objects/PersistentV1.c \
			$(BASELINE_PATH)/_OCT_objects/PersistentV2.c \
			$(BASELINE_PATH)/_OCT_objects/Persistent.c \
			$(BASELINE_PATH)/_OCT_objects/Phrase.c \
			$(BASELINE_PATH)/_OCT_objects/Phrase-presets.c \
			$(BASELINE_PATH)/_OCT_global/flash-block.c \
			$(BASELINE_PATH)/_OCT_interrupts/cpu-load.c

# Object files used in the system.
OBJECTS = $(SOURCES:.c=.o)

all:	nemo.elf

nemo.elf: $(SOURCES)

    ifeq ($(COMPILE_MODE),dev)

		# This produces the ELF that contains the symbol table used for the step debugger. This ELF should not be uploaded to Nemo!!
		$(CC) $(CFLAGS) -o $(BASELINE_PATH)/../NEMO_OS/nemo.elf_DEBUG_DONT_DEPLOY -D__ECOS -I$(INCLUDE_PATH) $(SOURCES) -L$(PREFIX)/lib -Ttarget.ld -nostdlib

    else

		# Produce an ELF that can be deployed for production use
		$(CC) $(CFLAGS) -o nemo.elf -D__ECOS -I$(INCLUDE_PATH) $(SOURCES) -L$(PREFIX)/lib -Ttarget.ld -nostdlib
		# Strip the ELF file of all symbols etc.
		$(STRIP) nemo.elf

    endif


clean:

    ifeq ($(COMPILE_MODE),dev)

		rm -f nemo.elf_DEBUG_DONT_DEPLOY

    else

		rm -f nemo.elf $(OBJECTS)

    endif



nemo.o: $(SOURCES)
	$(CC) -g -c -I$(INCLUDE_PATH) $(BASELINE_PATH)/../NEMO_OS/nemo.c

PersistentV1.o:
	$(CC) -g -c -I$(INCLUDE_PATH) $(BASELINE_PATH)/_OCT_objects/PersistentV1.c

PersistentV2.o:
	$(CC) -g -c -I$(INCLUDE_PATH) $(BASELINE_PATH)/_OCT_objects/PersistentV2.c

Persistent.o:
	$(CC) -g -c -I$(INCLUDE_PATH) $(BASELINE_PATH)/_OCT_objects/Persistent.c

Phrase.o:
	$(CC) -g -c -I$(INCLUDE_PATH) $(BASELINE_PATH)/_OCT_objects/Phrase.c

Phrase-presets.o:
	$(CC) -g -c -I$(INCLUDE_PATH) $(BASELINE_PATH)/_OCT_objects/Phrase-presets.c

flash-block.o:
	$(CC) -g -c -I$(INCLUDE_PATH) $(BASELINE_PATH)/_OCT_global/flash-block.c

/cpu-load.o:
	$(CC) -g -c -I$(INCLUDE_PATH) $(BASELINE_PATH)/_OCT_interrupts/cpu-load.c


