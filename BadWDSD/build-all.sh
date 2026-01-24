#!/bin/bash

rm -rf out
mkdir out

#

mkdir BadWDSD/build
echo -e '#define SC_IS_SW 1\n#define XDR_IS_X32 1' > BadWDSD/build/Config.h || exit 1
./build.sh || exit 1

cp -a BadWDSD/build/BadWDSD.uf2 out/BadWDSD_SW_x32.uf2 || exit 1

#

mkdir BadWDSD/build
echo -e '#define PICO_IS_ZERO 1\n#define SC_IS_SW 1\n#define XDR_IS_X32 1' > BadWDSD/build/Config.h || exit 1
./build.sh || exit 1

cp -a BadWDSD/build/BadWDSD.uf2 out/BadWDSD_SW_x32_Zero.uf2 || exit 1

#

mkdir BadWDSD/build
echo -e '' > BadWDSD/build/Config.h || exit 1
./build.sh || exit 1

cp -a BadWDSD/build/BadWDSD.uf2 out/BadWDSD_CXRF_x16.uf2 || exit 1