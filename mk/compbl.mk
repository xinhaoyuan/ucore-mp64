CC      := gcc -m32 -fno-builtin -fno-builtin-function -nostdinc
LD		:= ld -m $(shell ld -V | grep elf_i386 2>/dev/null)
OBJDUMP	:= objdump
OBJCOPY := objcopy
STRIP   := strip

include ${T_BASE}/mk/compopt.mk
