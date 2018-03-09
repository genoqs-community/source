all: octopus.elf

clean:
	rm main.o octopus.elf


CC = /opt/ecos/gnutools/arm-elf/bin/arm-elf-gcc
# CFLAGS = -Wall -g -O2
CFLAGS = -Wall -g

# INCLUDE_PATH = /home/Root/ecos_work/ecos_builds/E7T/E7T_RAM2x_1_install/include
# PREFIX = /home/Root/ecos_work/ecos_builds/E7T/E7T_RAM2x_1_install

INCLUDE_PATH = /home/Root/ecos_work/ecos_builds/E7T/E7T_eA_34_1_install/include
PREFIX =       /home/Root/ecos_work/ecos_builds/E7T/E7T_eA_34_1_install

octopus.elf:
#	/opt/ecos/gnutools/arm-elf/bin/arm-elf-gcc -Wall -o octopus.exe main.o -L/home/Root/ecos_work/ecos_builds/E7T/E7T_RAM_install/lib -Ttarget.ld -nostdlib
#	/opt/ecos/gnutools/arm-elf/bin/arm-elf-objcopy --strip-debug octopus.exe octopus.elf
	
	$(CC) $(CFLAGS) -o octopus.elf -D__ECOS -I$(INCLUDE_PATH) main.c -L$(PREFIX)/lib -Ttarget.ld -nostdlib
	/opt/ecos/gnutools/arm-elf/bin/arm-elf-strip octopus.elf

main.o:
#	/opt/ecos/gnutools/arm-elf/bin/arm-elf-gcc -c -I/home/Root/ecos_work/ecos_builds/E7T/E7T_RAM_install/include /opt/workspaces/Workspace0/PACIFIC/main.c
	/opt/ecos/gnutools/arm-elf/bin/arm-elf-gcc -c -I$(INCLUDE_PATH) /opt/workspaces/Workspace_UPD/OCT_OS_UPDATER/main.c







