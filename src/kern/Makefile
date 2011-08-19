.PHONY: all

SRCFILES   := $(shell find . '(' '!' -regex '/_.*' ')' -and '(' -iname "*.c" -or -iname "*.S" ')' | sed -e 's!\./!!g')
T_CC_FLAGS := -I../libs -I.

include ${T_BASE}/mk/compk.mk
include ${T_BASE}/mk/template.mk

all: ${T_BIN}/kern-bin.o

${T_BIN}/kern-bin.o: ${T_BIN}/kern-bin
	@echo OBJCOPY $@
	${V}cd ${T_BIN}; ${OBJCOPY} -I binary -O elf64-x86-64 -B i386 $(notdir $<) $@

${T_BIN}/kern-bin: ${T_BIN}/kern
	@echo OBJCOPY $@
	${V}${OBJCOPY} -S -O binary $^ $@

${T_BIN}/kern: ${OBJFILES} kern.ld
	@echo LD $@
	${V}${LD} -T kern.ld -z max-page-size=0x1000 -o$@ $^ ${T_BIN}/libs-kern-*.o
