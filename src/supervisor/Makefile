.PHONY: all

SRCFILES   := $(shell find . '(' '!' -regex '.*/_.*' ')' -and '(' -iname "*.c" -or -iname "*.S" ')' | sed -e 's!\./!!g')
T_CC_FLAGS := -I../libs-sv -Idebug -Idriver -Iinit -Ilibs -Imm -Itrap -Isync -Imp -Icontext

include ${T_BASE}/mk/compsv.mk
include ${T_BASE}/mk/template.mk

ifeq (${HAS_DRIVER_OS},y)
OBJFILES   += ${T_OBJ}/bzImage.o
T_CC_FLAGS += -DHAS_DRIVER_OS=1
endif

all: ${T_OBJ}/supervisor

${T_OBJ}/supervisor: ${OBJFILES} supervisor.ld
	@echo LD $@
	${V}${LD} -T supervisor.ld -z max-page-size=0x1000 -o$@ ${OBJFILES$} ${T_OBJ}/libs-sv-*.o 
