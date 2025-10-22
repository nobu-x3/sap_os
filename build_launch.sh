#/bin/bash
set -e
. ./build.sh

qemu-system-x86_64 -hda ./bin/sap_os.bin
