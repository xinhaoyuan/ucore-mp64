.PHONY: all mod-%

E_ENCODE ?= $(shell echo $(1) | sed -e 's!_!_1!g' -e 's!/!_2!g')
E_DECODE ?= $(shell echo $(1) | sed -e 's!_1!_!g' -e 's!_2!/!g')

# The order makes sense
ifeq (${BRANCH},ucore)
MODS := libs-sv supervisor kern-ucore ht-sign ht-mksfs libs-user-ucore user-ucore bootloader
KERN := ucore
endif

ifeq (${BRANCH},river)
MODS := libs-sv supervisor kern-river ht-sign ht-mksfs libs-user-ucore user-ucore bootloader
KERN := river
endif

ifeq (${BRANCH},linux-dos-module)
MODS := linux-dos-module
else

ifeq (${HAS_DRIVER_OS},y)
MODS := prebuilt ${MODS}
mod-supervisor: mod-prebuilt
endif

endif

MODDIRS := $(addprefix mod-,${MODS})

all: ${MODDIRS}

mod-supervisor: mod-libs-sv

mod-kern-river: mod-supervisor

mod-kern-ucore: mod-supervisor

mod-user-ucore: mod-ht-mksfs mod-libs-user-ucore

mod-bootloader: mod-ht-sign mod-kern-${KERN} mod-supervisor 

mod-%:
	@echo MAKE $* {
	${V}MOD="$*" ${MAKE} -C src/$* all
	@echo }


