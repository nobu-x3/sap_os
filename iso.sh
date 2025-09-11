#!/bin/sh
set -e
. ./build.sh

mkdir -p isodir
mkdir -p isodir/boot
mkdir -p isodir/boot/grub

cp sysroot/boot/sap_os.kernel isodir/boot/sap_os.kernel
cat > isodir/boot/grub/grub.cfg << EOF
menuentry "sap_os" {
	multiboot /boot/sap_os.kernel
}
EOF
grub-mkrescue -o sap_os.iso isodir
