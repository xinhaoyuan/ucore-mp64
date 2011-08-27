.PHONY: all

SRCFILES   := $(shell find . '(' '!' -regex '.*/_.*' ')' -and '(' -iname "*.c" -or -iname "*.S" ')' | sed -e 's!\./!!g')
T_CC_FLAGS := -I../libs-sv -Idebug -Idriver -Iinit -Ilibs -Imm -Itrap -Isync -Imp -Icontext

include ${T_BASE}/mk/compsv.mk
include ${T_BASE}/mk/template.mk

all: ${T_OBJ}/supervisor

${T_OBJ}/supervisor: ${OBJFILES} supervisor.ld
	@echo LD $@
	${V}${LD} -T supervisor.ld -z max-page-size=0x1000 -o$@ $^ ${T_OBJ}/libs-sv-*.o
