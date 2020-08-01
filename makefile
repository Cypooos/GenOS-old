#
# REMEMBER :
# bootsector loaded in 0x07C00
# data segment ready from 0x07C00 to 0x17C00
# stack start at 0x8F000 and finish at 0x80000
# OS loaded in 0x1000
# My OS is loaded at 0x1000 and my data segment goes from 0x07C00 to 0x17C00.
# If my OS is bigger that 0x6C00 bytes then it's getting out of the data segment
#
KER_SRC_C = system/src/main.c $(wildcard system/src/**/*.c) $(wildcard system/src/*.c)
KER_SRC_ASM = $(wildcard system/src/**/*.asm) $(wildcard system/src/*.asm)
KER_INC = system/include
BOOT_OBJ = out/bootloaderObj/main.o
KER_OBJ = $(KER_SRC_C:system/src/%.c=out/kernelObj/%.o) $(KER_SRC_ASM:system/src/%.asm=out/kernelObj/%.o)

CC = gcc # compiler/x86_64-elf-cross/bin/x86_64-elf-gcc # place CC compiler, temp is GCC
CC_F = -Wall -m32 -fno-pie -Wbuiltin-declaration-mismatch -fno-stack-protector
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

out/kernelObj/%.o: system/src/%.asm
	mkdir -p $(@D)
	nasm -f elf -o $@ $^ -i system/src/;

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

.PHONY: debug
debug: out/floppy
	sudo gnome-terminal -e "sudo qemu-system-x86_64 -s -S -machine q35 -enable-kvm -smp 1 -cpu host -d cpu_reset -no-shutdown -no-reboot -serial stdio -m 2G -boot menu=on -fda out/floppy"
	echo 'target remote localhost:1234' >debug.tmp
	#echo 'continue' >>debug.tmp
	exec gdb -x debug.tmp
	rm debug.tmp

.PHONY: test
test: out/floppy
	sudo qemu-system-x86_64 -machine q35 -no-shutdown -no-reboot -serial stdio -m 2G -boot menu=on -fda out/floppy
