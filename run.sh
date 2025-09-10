./clean.sh
./compile.sh

if grub-file --is-x86-multiboot sap_os.bin; then
  echo multiboot confirmed
else
  echo the file is not multiboot
  exit
fi

# make bootable image
mkdir -p isodir/boot/grub
cp sap_os.bin isodir/boot/sap_os.bin
cp grub.cfg isodir/boot/grub/grub.cfg
grub-mkrescue -o sap_os.iso isodir

# alternatively just run qemu-system-i368 -kernel sap_os.bin

sudo qemu-system-i386 -cdrom sap_os.iso
