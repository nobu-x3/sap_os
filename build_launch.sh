set -e
. ./config.sh

make all
qemu-system-x86_64 -hda ./bin/sap_os.bin