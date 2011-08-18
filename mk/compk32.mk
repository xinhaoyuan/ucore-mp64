CC      := gcc -fno-builtin -fno-builtin-function -nostdinc
LD		:= ld
OBJDUMP	:= objdump
OBJCOPY := objcopy
STRIP   := strip

include ${T_BASE}/mk/compopt.mk
