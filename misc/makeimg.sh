#!/bin/sh

kernel_img=${T_BIN}/kernel.img
swap_img=${T_BIN}/swap.img
sfs_img=${T_BIN}/sfs.img

# KERNEL IMG

dd if=/dev/zero of=${kernel_img} count=10000
dd if=${T_BIN}/bootsect of=${kernel_img} conv=notrunc
dd if=${T_BIN}/kern-bin of=${kernel_img} seek=1 conv=notrunc
dd if=${T_BIN}/supervisor of=${kernel_img} seek=$(( $(cat ${T_BIN}/kern-sect_size) + 1 )) conv=notrunc

# SWAP IMG

dd if=/dev/zero of=${swap_img} bs=1M count=128

# SFS IMG

dd if=/dev/zero of=${sfs_img} bs=1M count=256
${T_BIN}/tools-mksfs ${sfs_img} ${T_BIN}/user-sfs
