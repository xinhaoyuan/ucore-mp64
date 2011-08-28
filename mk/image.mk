.PHONY: all

all: ${T_OBJ}/kernel.img ${T_OBJ}/swap.img ${T_OBJ}/sfs.img

${T_OBJ}/kernel.img: ${T_OBJ}/bootsect ${T_OBJ}/kern-bin ${T_OBJ}/supervisor
	@echo MAKE $@
	${V}dd if=/dev/zero of=$@ count=10000
	${V}dd if=${T_OBJ}/bootsect of=$@ conv=notrunc
	${V}dd if=${T_OBJ}/kern-bin of=$@ seek=1 conv=notrunc
	${V}dd if=${T_OBJ}/supervisor of=$@ seek=$(shell echo $(shell cat ${T_OBJ}/kern-sect_size) + 1 | bc) conv=notrunc

${T_OBJ}/swap.img:
	@echo MAKE $@
	${V}dd if=/dev/zero of=$@ bs=1M count=128

${T_OBJ}/sfs.img: ${T_OBJ}/user-sfs-timestamp
	@echo MAKE $@
	${V}dd if=/dev/zero of=$@ bs=1M count=256
	${V}${T_OBJ}/tools-mksfs $@ ${T_OBJ}/user-sfs
