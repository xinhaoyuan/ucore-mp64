.PHONY: all

SRCFILES   := $(shell find . '(' '!' -regex '.*/_.*' ')' -and '(' -iname "*.c" -or -iname "*.S" ')' | sed -e 's!\./!!g')
T_CC_ALL_FLAGS := -Os -I. -DKERN_START_SECT=$(shell echo $(shell cat ${T_OBJ}/kern-sect_size) + 1 | bc)

include ${T_BASE}/mk/compbl.mk
include ${T_BASE}/mk/template.mk

all: ${T_OBJ}/bootsect

${T_OBJ}/bootsect: ${T_OBJ}/bootloader
	@echo OBJCOPY $@
	${V}${OBJCOPY} -S -O binary $^ $@.original
	@${T_OBJ}/tools-sign $@.original $@

${T_OBJ}/bootloader: ${OBJFILES} bootloader.ld
	@echo LD $@
	${V}${LD} -N -e start -Tbootloader.ld -o$@ ${OBJFILES}
	${V}${STRIP} -g -R .eh_frame $@


