#/bin/bash
# . ./headers.sh

. ./config.sh

for PROJECT in $PROJECTS; do
  (cd $PROJECT && DESTDIR="$SYSROOT" $MAKE all)
done