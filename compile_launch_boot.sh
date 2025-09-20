#!/bin/sh

nasm -f bin boot.S -o boot.bin -wall
qemu-system-x86_64 -hda boot.bin