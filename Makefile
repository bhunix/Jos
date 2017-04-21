C_SOURCES = $(shell find . -name "*.c")
C_OBJECTS = $(patsubst %.c, %.o, $(C_SOURCES))
S_SOURCES = $(shell find . -name "*.s")
S_OBJECTS = $(patsubst %.s, %.o, $(S_SOURCES))

QEMU=qemu-system-i386

CC = gcc
LD = ld
ASM = nasm

HOSTNAME = $(shell hostname)
ifeq ($(HOSTNAME), debian)
	HOME = /root
else
	HOME = /home/j
endif

C_FLAGS = -c -Wall -m32 -ggdb -gstabs+ -nostdinc -fno-builtin -fno-stack-protector -I include
LD_FLAGS = -T scripts/kernel.lds -m elf_i386 -nostdlib
ASM_FLAGS = -f elf -g -F stabs

all: $(S_OBJECTS) $(C_OBJECTS) link update_image

.c.o:
		$(CC) $(C_FLAGS) $< -o $@

.s.o:
		$(ASM) $(ASM_FLAGS) $<

link:
		$(LD) $(LD_FLAGS) $(S_OBJECTS) $(C_OBJECTS) -o j_kernel

.PHONY:clean
clean:
		$(RM) $(S_OBJECTS) $(C_OBJECTS) j_kernel

.PHONY:update_image
update_image:
		sudo mount -o loop floppy.img $(HOME)/workspace/jos/sda
		sudo cp j_kernel $(HOME)/workspace/jos/sda
		sleep 1
		sudo umount $(HOME)/workspace/jos/sda

.PHONY:mount_image
mount_image:
		sudo mount floppy.img $(HOME)/workspace/jos/sda

.PHONY:umount_image
umount_image:
		sudo umount $(HOME)/workspace/jos/sda

.PHONY:qemu
qemu:
		$(QEMU) -fda floppy.img -boot a

.PHONY:bochs
bochs:
		bochs -f tools/bochsrc.txt

.PHONY:debug
debug:
		$(QEMU) -S -s -fda floppy.img -boot a &
		#sleep 1
		#gdb -x scripts/gdbinit
