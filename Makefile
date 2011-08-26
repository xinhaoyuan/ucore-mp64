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
	-rm -rf ${T_BIN}/*


qemu: all
	qemu-system-x86_64 -smp 4 \
	-hda ${T_BIN}/kernel.img \
	-drive file=${T_BIN}/swap.img,media=disk,cache=writeback \
	-drive file=${T_BIN}/sfs.img,media=disk,cache=writeback \
	-s -S \
	-serial file:bin/serial.log -monitor stdio
