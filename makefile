KER_SRC_C = system/src/main.c $(wildcard system/src/**/*.c) $(wildcard system/src/*.c)
KER_SRC_ASM = $(wildcard system/src/**/*.asm) $(wildcard system/src/*.asm)
KER_INC = system/include
BOOT_OBJ = out/bootloaderObj/main.o
KER_OBJ = $(KER_SRC_C:system/src/%.c=out/kernelObj/%.o) $(KER_SRC_ASM:system/src/%.asm=out/kernelObj/%.o)

CC = gcc # compiler/x86_64-elf-cross/bin/x86_64-elf-gcc # place CC compiler, temp is GCC
CC_F = -Wall -m32 -fno-pie -Wbuiltin-declaration-mismatch
# --------------------- MAKE COMPILER

$(CC): # for now, not use
	sudo chmod +x compiler/make_compiler.sh
	@echo '------ INSTALLING GCC-ELF ------'
	sudo compiler/make_compiler.sh


# --------------------- MAKE OBJS

out/bootloaderObj/main.o: system/bootloader/main.asm
	nasm -f bin -o $@ $^ -i system/bootloader/;

out/kernelObj/%.o: system/src/%.c
	mkdir -p $(@D)
	$(CC) $(CC_F) -I $(KER_INC) -c $< -o $@

# --------------------- MAKE COMPILED

out/kernel: $(KER_OBJ)
	ld -m elf_i386 --oformat binary -Ttext 1000 $^ -o $@

out/bootloader: $(BOOT_OBJ)
	cp out/bootloaderObj/main.o out/bootloader

# --------------------- MAKE BOOTABLE DEVICE

out/floppy: out/kernel out/bootloader
	cat out/bootloader out/kernel /dev/zero | dd of=out/floppy bs=512 count=2880

# --------------------- RUN & CLEAN
.PHONY: clean
clean:
	rm -f ${KER_OBJ} ${BOOT_OBJ}

.PHONY: clear
clear:
	rm -f ${KER_OBJ} ${BOOT_OBJ} out/kernel out/bootloader out/floppy


.PHONY: run
run: out/floppy
	sudo qemu-system-x86_64 -machine q35 -enable-kvm -smp 1 -cpu host -d cpu_reset -no-shutdown -no-reboot -serial stdio -m 2G -boot menu=on -fda out/floppy

.PHONY: test
test: out/floppy
	sudo qemu-system-x86_64 -machine q35 -no-shutdown -no-reboot -serial stdio -m 2G -boot menu=on -fda out/floppy
