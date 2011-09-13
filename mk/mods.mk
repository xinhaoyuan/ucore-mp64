.PHONY: all mod-%

E_ENCODE ?= $(shell echo $(1) | sed -e 's!_!_1!g' -e 's!/!_2!g')
E_DECODE ?= $(shell echo $(1) | sed -e 's!_1!_!g' -e 's!_2!/!g')

BRANCH ?= ucore

# The order makes sense
ifeq (${BRANCH},ucore)
MODS := prebuilt libs-sv supervisor kern-ucore ht-sign ht-mksfs libs-user-ucore user-ucore bootloader
endif

ifeq (${BRANCH},river)
MODS := prebuilt libs-sv supervisor kern-river ht-sign ht-mksfs libs-user-ucore user-ucore bootloader
endif


MODDIRS := $(addprefix mod-,${MODS})

all: ${MODDIRS}

mod-%:
	@echo MAKE $* {
	${V}MOD="$*" ${MAKE} -C src/$* all
	@echo }
