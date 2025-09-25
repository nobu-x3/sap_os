#/bin/bash
set -e
. ./build.sh

qemu-system-x86_64 -hda ./sysroot/boot/sap_os.bin