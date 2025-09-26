#/bin/bash
set -e
. ./build.sh

qemu-system-x86_64 -hda ./build/sap_os.bin
