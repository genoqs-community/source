
# Gabriel's paths
# INCLUDE_PATH 	= /opt/ecos_libs/E7T_eA_34_1_debug_install/include
# PREFIX      	= /opt/ecos_libs/E7T_eA_34_1_debug_install
 INCLUDE_PATH 	= /opt/ecos_libs/E7T_eA_34_1_install/include
 PREFIX      	= /opt/ecos_libs/E7T_eA_34_1_install
 TOOLPREFIX 	= /home/genoqs/Desktop/software/ecos/gnutools/arm-elf/bin
 BASELINE_PATH	= /home/genoqs/Desktop/Octopus-fork/OCT_OS_v1.60

CC    = $(TOOLPREFIX)/arm-elf-gcc
STRIP = $(TOOLPREFIX)/arm-elf-strip

# -----------------------------------------------------------------------
# USE THE -O2 FOR FINAL VERSION! THIS GIVES US 2.6 MORE CPU PERFORMANCE
# -----------------------------------------------------------------------

CFLAGS = -Wall -O2

# Source files used in the system.
SOURCES = 	nemo.c \
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
	$(CC) $(CFLAGS) -o nemo.elf -D__ECOS -I$(INCLUDE_PATH) $(SOURCES) -L$(PREFIX)/lib -Ttarget.ld -nostdlib
	$(STRIP) nemo.elf

clean:	
	rm -f nemo.elf $(OBJECTS)



nemo.o: $(SOURCES)
	$(CC) -g -c -I$(INCLUDE_PATH) nemo.c

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


