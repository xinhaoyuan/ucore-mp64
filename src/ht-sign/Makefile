.PHONY: all

SRCFILES   := $(shell find . '(' '!' -regex '/_.*' ')' -and '(' -iname "*.c" -or -iname "*.S" ')' | sed -e 's!\./!!g')
T_CC_FLAGS := 

include ${T_BASE}/mk/comphost.mk
include ${T_BASE}/mk/template.mk

all: ${T_BIN}/tools-sign

${T_BIN}/tools-sign: ${OBJFILES}
	@echo LD $@
	${V}${CC} -o$@ $^
