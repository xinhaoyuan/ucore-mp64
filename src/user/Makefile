E_ENCODE = $(shell echo $(1) | sed -e 's!_!_1!g' -e 's!/!_2!g')
E_DECODE = $(shell echo $(1) | sed -e 's!_1!_!g' -e 's!_2!/!g')

.PHONY: all

SRCFILES   := $(shell find . '(' '!' -regex '/_.*' ')' -and -iname "*.c"  | sed -e 's!\./!!g')
T_CC_FLAGS := 	-I../libs-user/ -I../libs-user/common

include ${T_BASE}/mk/compu.mk
include ${T_BASE}/mk/template.mk

BINFILES := $(addprefix ${T_BIN}/user-sfs/bin/,${SRCFILES:.c=})

all: ${T_BIN}/user-sfs ${BINFILES}
	${V}cp _initial/* ${T_BIN}/user-sfs/

${T_BIN}/user-sfs/bin/%: ${T_BIN}/user-$(call E_ENCODE,%).c.o
	@echo LD $@
	${V}${LD} -T user.ld ${T_BIN}/libs-user-*.o $< -o$@


${T_BIN}/user-sfs:
	${V}mkdir -p $@
	${V}mkdir -p $@/bin
