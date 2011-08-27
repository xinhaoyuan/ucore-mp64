.PHONY: all ${T_OBJ}/image

all: ${T_OBJ}/image

${T_OBJ}/image:
	@echo makeimg.sh
	${V}T_OBJ=${T_OBJ} sh ${T_BASE}/misc/makeimg.sh
