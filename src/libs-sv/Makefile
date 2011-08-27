.PHONY: all

SRCFILES   := $(shell find . '(' '!' -regex '.*/_.*' ')' -and '(' -iname "*.c" -or -iname "*.S" ')' | sed -e 's!\./!!g')
T_CC_FLAGS := -I.

include ${T_BASE}/mk/compsv.mk
include ${T_BASE}/mk/template.mk

all: ${OBJFILES}
