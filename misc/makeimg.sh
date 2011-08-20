#!/bin/sh

dd if=/dev/zero of=${T_BIN}/image count=10000
dd if=${T_BIN}/bootsect of=${T_BIN}/image conv=notrunc
dd if=${T_BIN}/kern-bin of=${T_BIN}/image seek=1 conv=notrunc
dd if=${T_BIN}/supervisor of=${T_BIN}/image seek=$(( $(cat ${T_BIN}/kern-sect_size) + 1 )) conv=notrunc
