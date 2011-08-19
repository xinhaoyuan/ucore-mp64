.PHONY: all clean qemu

export V        ?= @
export T_BASE   ?= ${PWD}
export T_BIN    ?= ${PWD}/bin
export MAKE     := make -s

all: bin
	${V}${MAKE} -f mk/mods.mk all
	${V}${MAKE} -f mk/image.mk all

bin:
	@mkdir bin

clean:
	rm -rf ${T_BIN}/*

qemu: all
	qemu-system-x86_64 -smp 2 ${T_BIN}/image \
	-serial file:bin/serial.log -monitor stdio
