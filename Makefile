.PHONY: all clean

export V        ?= @
export T_BASE   ?= ${PWD}
export T_BIN    ?= ${PWD}/bin
export MAKE     := make -s

all:
	-@mkdir bin
	${V}${MAKE} -f mk/mods.mk all
	${V}${MAKE} -f mk/image.mk all

clean:
	rm -rf ${T_BIN}/*

