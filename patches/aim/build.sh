#!/bin/bash

export FLAGS="-g -O1 -Wall -mcpu=cell -mabi=elfv1 -ffreestanding -mtoc -nostdlib -Wl,--build-id=none -static"
export CC=powerpc64-linux-gnu-gcc

$CC $FLAGS -T ld.ld aim_get_device_type.S -o aim_get_device_type.elf || exit 1
$CC $FLAGS -T ld.ld aim_get_device_type.S -o aim_get_device_type.bin -Wl,--oformat=binary || exit 1

$CC $FLAGS -T ld.ld aim_get_device_id.S -o aim_get_device_id.elf || exit 1
$CC $FLAGS -T ld.ld aim_get_device_id.S -o aim_get_device_id.bin -Wl,--oformat=binary || exit 1

$CC $FLAGS -T ld.ld aim_get_ps_code.S -o aim_get_ps_code.elf || exit 1
$CC $FLAGS -T ld.ld aim_get_ps_code.S -o aim_get_ps_code.bin -Wl,--oformat=binary || exit 1