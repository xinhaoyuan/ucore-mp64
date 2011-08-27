.PHONY: all clean qemu

export V        ?= @
export T_BASE   ?= ${PWD}
export T_OBJ    ?= ${PWD}/obj
export MAKE     := make -s

all: ${T_OBJ}
	${V}${MAKE} -f mk/mods.mk all
	${V}${MAKE} -f mk/image.mk all

${T_OBJ}:
	@mkdir -p ${T_OBJ}

clean:
	-rm -rf ${T_OBJ}/*


qemu: all
	qemu-system-x86_64 -smp 4 \
	-hda ${T_OBJ}/kernel.img \
	-drive file=${T_OBJ}/swap.img,media=disk,cache=writeback \
	-drive file=${T_OBJ}/sfs.img,media=disk,cache=writeback \
	-s -S \
	-serial file:obj/serial.log -monitor stdio
