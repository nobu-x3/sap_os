DEFAULT_HOST!=./default-host.sh
HOST?=DEFAULT_HOST
FILES=./build/kernel.asm.o ./build/kernel.o
INCLUDES= -I./src
FLAGS=-g -ffreestanding -falign-jumps -falign-functions -falign-labels -falign-loops -fstrength-reduce -fomit-frame-pointer -finline-functions -Wno-unused-functions -fno-builtin -Werror -Wno-unused-label -Wno-cpp -Wno-unused-parameter -nostdlib -nostartfiles -nodefaultlibs -Wall -O0 -Iinc

all: ./bin/boot.bin ./bin/kernel.bin
	rm -rf ./bin/sap_os.bin
	dd if=./bin/boot.bin >> ./bin/sap_os.bin
	dd if=./bin/kernel.bin >> ./bin/sap_os.bin
	dd if=/dev/zero bs=512 count=100 >> ./bin/sap_os.bin

./bin/kernel.bin: $(FILES)
	$(LD) -g -relocatable $(FILES) -o ./build/kernelfull.o
	$(CC) $(FLAGS) -T ./src/linker.ld -o ./bin/kernel.bin ./build/kernelfull.o

./bin/boot.bin:
	$(AS) -f bin ./src/boot/boot.asm -o ./bin/boot.bin

./build/kernel.asm.o: ./src/kernel.asm
	$(AS) -f elf -g ./src/kernel.asm -o ./build/kernel.asm.o

./build/kernel.o : ./src/kernel.c
	$(CC) $(INCLUDES) $(FLAGS) -std=gnu99 -c ./src/kernel.c -o ./build/kernel.o

clean:
	rm -rf ./bin/*
	rm -rf ./build/*