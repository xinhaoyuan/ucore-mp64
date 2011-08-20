.PHONY: all

SRCFILES   := $(shell find . '(' '!' -regex '/_.*' ')' -and '(' -iname "*.c" -or -iname "*.S" ')' | sed -e 's!\./!!g')
T_CC_ALL_FLAGS := -Os -I. -DKERN_START_SECT=$(shell echo $(shell cat ${T_BIN}/kern-sect_size) + 1 | bc)

include ${T_BASE}/mk/compbl.mk
include ${T_BASE}/mk/template.mk

all: ${T_BIN}/bootsect

${T_BIN}/bootsect: ${T_BIN}/bootloader
	@echo OBJCOPY $@
	${V}${OBJCOPY} -S -O binary $^ $@.original
	@${T_BIN}/tools-sign $@.original $@

${T_BIN}/bootloader: ${OBJFILES}
	@echo LD $@
	${V}${LD} -N -e start -Ttext 0x7C00 -o$@ $^
	@${STRIP} -g -R .eh_frame $@


