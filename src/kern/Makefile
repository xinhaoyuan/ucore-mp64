.PHONY: all

SRCFILES   := $(shell find . '(' '!' -regex '/_.*' ')' -and '(' -iname "*.c" -or -iname "*.S" ')' | sed -e 's!\./!!g')
T_CC_FLAGS := 	-I../libs-kern -Iglue -Ilibs -Idebug -Idriver -Imm -Isync -Itrap -Ischedule -Iprocess -Ifs \
				-Ifs/swap -Ifs/vfs -Ifs/pipe -Ifs/sfs -Ifs/devs -Isyscall

include ${T_BASE}/mk/compk.mk
include ${T_BASE}/mk/template.mk

all: ${T_BIN}/kern-bin

${T_BIN}/kern-bin: ${T_BIN}/kern
	@echo OBJCOPY $@
	${V}${OBJCOPY} -S -O binary $^ $@
	@T_BIN=${T_BIN} sh ${T_BASE}/misc/kerninfo.sh

${T_BIN}/kern: ${OBJFILES} ${T_BIN}/kern-svsym.S.o kern.ld
	@echo LD $@
	${V}${LD} -T kern.ld -z max-page-size=0x1000 -o$@ $^ ${T_BIN}/libs-kern-*.o

${T_BIN}/svsym.o: ${T_BIN}/supervisor
	${V}objcopy --extract-symbol $< $@
	${V}strip -x $@

${T_BIN}/kern-svsym.S: ${T_BIN}/svsym.o
	@echo GEN $@
	${V}echo ".data\n.align 8\n" > $@
	${V}objdump -t $< -j .text -j .rodata -j .data -j .bss \
		| sed -n -e 's/^\([0-9a-f][0-9a-f]*\).* \([a-zA-Z0-9_][a-zA-Z0-9_]*\)$$/\2_ptr: .quad 0x\1/gp' \
		| sed -e 's/__TO_EXPORT_\([a-zA-Z0-9_][a-zA-Z0-9_]*\).*/.global \1/g' >> $@

${T_BIN}/kern-svsym.S.o: ${T_BIN}/kern-svsym.S
	@echo CC $@
	@${CC} ${CC_ALL_FLAGS} $< -c -o$@
	@strip -x $@
