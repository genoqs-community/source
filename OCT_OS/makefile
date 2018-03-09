
# Gabriel's paths
INCLUDE_PATH 	= /opt/ecos_libs/E7T_eA_34_1_install/include
PREFIX      	= /opt/ecos_libs/E7T_eA_34_1_install
TOOLPREFIX 		= /home/genoqs/Desktop/software/ecos/gnutools/arm-elf/bin

# Rob's paths
# INCLUDE_PATH 	= /Users/rob/Devel/octopus/octopus-ecos-libs/E7T_eA_34_1_install/include
# PREFIX 			= /Users/rob/Devel/octopus/octopus-ecos-libs/E7T_eA_34_1_install
# TOOLPREFIX 		= /Users/rob/Devel/octopus/arm-elf/bin


CC    = $(TOOLPREFIX)/arm-elf-gcc
STRIP = $(TOOLPREFIX)/arm-elf-strip

# -----------------------------------------------------------------------
# USE THE -O2 FOR FINAL VERSION! THIS GIVES US 2.6 MORE CPU PERFORMANCE
# -----------------------------------------------------------------------

# !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! TOGGLE DEBUG - '-g' flag will corrupt the Octopus memory bank overflow
CFLAGS = -Wall -O2
#CFLAGS = -g -O0

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


