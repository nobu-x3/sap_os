#/bin/bash
. ./headers.sh

mkdir -p build/kernel
make -C kernel

for PROJECT in $PROJECTS; do
  (cd $PROJECT && DESTDIR="$SYSROOT" $MAKE install)
done