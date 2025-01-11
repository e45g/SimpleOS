LD = $(HOME)/opt/cross/bin/i686-elf-ld
GCC = $(HOME)/opt/cross/bin/i686-elf-gcc

LINKER_SRC = src/linker.ld
KERNEL_SRC = src/kernel
BOOT_SRC = src/bootloader

BOOT_BIN = bin/boot.bin
KERNEL_BIN = bin/kernel.bin

KERNEL_OUT = build/kernel.o
KERNEL_ASM_OUT = build/kernel.asm.o
COMPLETE_KERNEL_OUT = build/complete_kernel.o

FLOPPY = bin/os.img

all: clean boot kernel
	dd if=/dev/zero of=$(FLOPPY) bs=512 count=2880
	dd if=$(BOOT_BIN) of=$(FLOPPY) bs=512 conv=notrunc
	dd if=$(KERNEL_BIN) of=$(FLOPPY) bs=512 seek=1 conv=notrunc



run: all
	qemu-system-i386 -fda $(FLOPPY)

boot:
	nasm -f bin -o $(BOOT_BIN) $(BOOT_SRC)/boot.asm

kernel:
	$(GCC) -c -ffreestanding -o build/kernel.o $(KERNEL_SRC)/kernel.c -lgcc
	$(LD) -T$(LINKER_SRC) -nostdlib --nmagic -o build/kernel.elf build/kernel.o
	objcopy -O binary build/kernel.elf $(KERNEL_BIN)

clean:
	rm -f build/*
	rm -f bin/*
