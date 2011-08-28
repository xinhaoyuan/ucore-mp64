#!/bin/sh

kernel_img=${T_OBJ}/kernel.img
swap_img=${T_OBJ}/swap.img
sfs_img=${T_OBJ}/sfs.img

if [ "$1" = "kernel" ]; then
# KERNEL IMG
	dd if=/dev/zero of=${kernel_img} count=10000
	dd if=${T_OBJ}/bootsect of=${kernel_img} conv=notrunc
	dd if=${T_OBJ}/kern-bin of=${kernel_img} seek=1 conv=notrunc
	dd if=${T_OBJ}/supervisor of=${kernel_img} seek=$(( $(cat ${T_OBJ}/kern-sect_size) + 1 )) conv=notrunc
elif [ "$1" = "swap" ]; then
# SWAP IMG
	dd if=/dev/zero of=${swap_img} bs=1M count=128
elif [ "$1" = "sfs" ]; then
# SFS IMG
	dd if=/dev/zero of=${sfs_img} bs=1M count=256
	${T_OBJ}/tools-mksfs ${sfs_img} ${T_OBJ}/user-sfs
fi
