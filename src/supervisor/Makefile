.PHONY: all

SRCFILES   := $(shell find . '(' '!' -regex '/_.*' ')' -and '(' -iname "*.c" -or -iname "*.S" ')' | sed -e 's!\./!!g')
T_CC_FLAGS := -I../libs -Idebug -Idriver -Iinit -Ilibs -Imm -Itrap -Isync -Imp

include ${T_BASE}/mk/compk64.mk
include ${T_BASE}/mk/template.mk

all: ${T_BIN}/supervisor

${T_BIN}/supervisor: ${OBJFILES} supervisor.ld
	@echo LD $@
	${V}${LD} -T supervisor.ld -z max-page-size=0x1000 -o$@ $^ ${T_BIN}/libs-*.o
