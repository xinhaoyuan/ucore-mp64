.PHONY: all ${T_BIN}/image

all: ${T_BIN}/image

${T_BIN}/image:
	@echo makeimg.sh
	${V}T_BIN=${T_BIN} sh ${T_BASE}/misc/makeimg.sh
