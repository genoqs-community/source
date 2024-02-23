OCT_MAKE=./OCT_OS/makefile
NEMO_MAKE=./NEMO_OS/makefile

all: build.octopus build.nemo debug.octopus debug.nemo

build: build.octopus build.nemo

debug: debug.octopus debug.nemo

build.octopus: 
	${MAKE} -f ${OCT_MAKE} build

debug.octopus:
	${MAKE} -f ${OCT_MAKE} debug

build.nemo: 
	${MAKE} -f ${NEMO_MAKE} build

debug.nemo:
	${MAKE} -f ${NEMO_MAKE} debug

clean: clean.build clean.debug

clean.build: clean.build.octopus clean.build.nemo

clean.debug: clean.debug.octopus clean.debug.nemo

clean.build.octopus:
	${MAKE} -f ${OCT_MAKE} clean.build

clean.debug.octopus:
	${MAKE} -f ${OCT_MAKE} clean.debug

clean.build.nemo:
	${MAKE} -f ${NEMO_MAKE} clean.build

clean.debug.nemo:
	${MAKE} -f ${NEMO_MAKE} clean.debug