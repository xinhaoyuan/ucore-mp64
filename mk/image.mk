.PHONY: all ${T_BIN}/image

all: ${T_BIN}/image

${T_BIN}/image:
	@echo DD $@
	$(V)dd if=/dev/zero of=$@ count=10000
	$(V)dd if=${T_BIN}/bootsect of=$@ conv=notrunc
	$(V)dd if=${T_BIN}/kern of=$@ seek=1 conv=notrunc
