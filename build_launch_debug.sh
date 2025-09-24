set -e
. ./build.sh

qemu-system-x86_64 -S -s -hda ./sysroot/boot/sap_os.bin