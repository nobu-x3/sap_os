#/bin/bash
set -e
. ./config.sh

for PROJECT in $SYSTEM_HEADER_PROJECTS; do
  (cd $PROJECT && DESTDIR="$SYSROOT" $MAKE clean)
done

rm -rf sysroot