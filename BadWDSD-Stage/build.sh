#!/bin/bash

export FLAGS="-g -O1 -Wall -mcpu=cell -mabi=elfv1 -ffreestanding -mtoc -nostdlib -Wl,--build-id=none -static"

powerpc64-linux-gnu-gcc $FLAGS -T Stage0.ld Stage0.S -o Stage0.elf || exit 1
powerpc64-linux-gnu-gcc $FLAGS -T Stage0.ld Stage0.S -o Stage0.bin -Wl,--oformat=binary || exit 1

powerpc64-linux-gnu-gcc $FLAGS -T Stagex.ld Stagex.c -o Stagex.elf || exit 1
powerpc64-linux-gnu-gcc $FLAGS -T Stagex.ld Stagex.c -o Stagex.bin -Wl,--oformat=binary || exit 1

powerpc64-linux-gnu-gcc $FLAGS -T Stage2j.ld Stage2j.S -o Stage2j.elf || exit 1
powerpc64-linux-gnu-gcc $FLAGS -T Stage2j.ld Stage2j.S -o Stage2j.bin -Wl,--oformat=binary || exit 1

powerpc64-linux-gnu-gcc $FLAGS -T Stage3j.ld Stage3j.S -o Stage3j.elf || exit 1
powerpc64-linux-gnu-gcc $FLAGS -T Stage3j.ld Stage3j.S -o Stage3j.bin -Wl,--oformat=binary || exit 1

powerpc64-linux-gnu-gcc $FLAGS -T Stage4j.ld Stage4j.c -o Stage4j.elf || exit 1
powerpc64-linux-gnu-gcc $FLAGS -T Stage4j.ld Stage4j.c -o Stage4j.bin -Wl,--oformat=binary || exit 1