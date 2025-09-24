set -e
. ./config.sh

make all
qemu-system-x86_64 -S -s -hda ./bin/sap_os.bin