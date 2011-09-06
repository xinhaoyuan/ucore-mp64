.PHONY: all

OFILELIST = libv3vee.a
TFILELIST = $(addprefix ${T_OBJ}/,${OFILELIST} vm_image.o)

include ${T_BASE}/mk/compk.mk

all: ${TFILELIST}

${T_OBJ}/%: %
	@echo CP $@
	${V}cp $< $@

${T_OBJ}/vm_image.o: vm_image
	@echo OBJCOPY $@
	${V}${OBJCOPY} -I binary -O elf64-x86-64 -B "i386:x86-64" $< $@

