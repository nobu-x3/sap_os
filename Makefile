all:
	nasm -f bin ./src/boot/boot.S -o ./bin/boot.bin

clean:
	rm -rf ./bin/boot.bin