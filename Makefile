# $@ means to use the target file
# $< use the first dependency
# $^ use all dependencies



C_SOURCES = $(wildcard kernel/*.c drivers/*.c cpu/*.c)
HEADERS = $(wildcard kernel/*.h drivers/*.h cpu/*.h)

OBJ = ${C_SOURCES:.c=.o cpu/interrupt.o}

CC = /usr/local/i386elfgcc/bin/i386-elf-gcc

#Makefile format
#create: using these files 
#	using this command

os-image.bin: boot/bootsect.bin kernel.bin
	cat $^ > os-image.bin

#Makes the kernel using the kernel entry and all the object files
#by linking them together starting at position 1000 which is also in our
#bootloader program
kernel.bin: boot/kernel_entry.o ${OBJ}
	i386-elf-ld -o $@ -Ttext 0x1000 $^ --oformat binary

run: os-image.bin
	qemu-system-i386 -fda os-image.bin

#Makes lots of object files from all of our c files, the -ffreestanding 
#flag tells the compiler to ignore the c standard library
%.o: %.c ${HEADERS}
	${CC} ${CFLAGS} -ffreestanding -c $< -o $@

#Turns our assembly into elf files so that they can be linked with our c object files
#
%.o: %.asm
	nasm $< -f elf -o $@

%.bin: %.asm
	nasm $< -f bin -o $@

clean:
	rm -rf *.bin *.dis *.o os-image.bin *.elf
	rm -rf kernel/*.o boot/*.bin drivers/*.o boot/*.o
