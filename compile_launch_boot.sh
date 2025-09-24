#!/bin/sh

make
qemu-system-x86_64 -hda ./bin/boot.bin