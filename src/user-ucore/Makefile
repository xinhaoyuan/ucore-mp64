E_ENCODE = $(shell echo $(1) | sed -e 's!_!_1!g' -e 's!/!_2!g')
E_DECODE = $(shell echo $(1) | sed -e 's!_1!_!g' -e 's!_2!/!g')

.PHONY: all

SRCFILES   := $(shell find . '(' '!' -regex '.*/_.*' ')' -and -iname "*.c"  | sed -e 's!\./!!g')
T_CC_FLAGS := 	-I../libs-user-ucore/ -I../libs-user-ucore/common

include ${T_BASE}/mk/compu.mk
include ${T_BASE}/mk/template.mk

BINFILES := $(addprefix ${T_OBJ}/user-sfs/bin/,${SRCFILES:.c=})

all: ${T_OBJ}/user-sfs ${BINFILES}
	${V}cp _initial/* ${T_OBJ}/user-sfs/

${T_OBJ}/user-sfs/bin/%: ${T_OBJ}/user-ucore-$(call E_ENCODE,%).c.o
	@echo LD $@
	${V}${LD} -T user.ld ${T_OBJ}/libs-user-ucore-*.o $< -o$@
	@touch ${T_OBJ}/user-sfs-timestamp


${T_OBJ}/user-sfs:
	${V}mkdir -p $@
	${V}mkdir -p $@/bin
