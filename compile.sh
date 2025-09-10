~/cross/bin/i686-elf-as boot.s -o boot.o
~/cross/bin/i686-elf-gcc -c kernel.c -s kernel.o -std=gnu99 -ffreestanding -O2 -Wall -Wextra

# or ~/cross/bin/i686-elf-g++ -c kernel.cpp -o kernel.o -ffreestanding -O2 -Wall -Wextra -fno-exceptions -fno-rtti

~/cross/bin/i686-elf-gcc -T linker.ld -o sap_os.bin -ffreestanding -O2 -nostdlib boot.o kernel.o -lgcc
