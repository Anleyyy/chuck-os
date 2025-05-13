# Variables
CC=gcc
LD=ld
NASM=nasm

CFLAGS=-m32 -nostdlib -nostdinc -fno-builtin -fno-stack-protector -nostartfiles -nodefaultlibs -Wall -Wextra -Werror -c
LDFLAGS=-T link.ld -melf_i386

BUILD=build
ISO=os.iso
KERNEL=kernel.elf
ISO_DIR=$(BUILD)/iso
GRUB_DIR=$(ISO_DIR)/boot/grub

# Fichiers
OBJS=$(BUILD)/loader.o $(BUILD)/kmain.o

# Règles

all: $(ISO)

$(BUILD):
	mkdir -p $(BUILD)

$(BUILD)/loader.o: loader.asm | $(BUILD)
	$(NASM) -f elf32 loader.asm -o $@

$(BUILD)/kmain.o: kmain.c | $(BUILD)
	$(CC) $(CFLAGS) -c kmain.c -o $@

$(BUILD)/$(KERNEL): $(OBJS)
	$(LD) $(LDFLAGS) -o $@ $(OBJS)

# Création de l'ISO GRUB
$(ISO): $(BUILD)/$(KERNEL)
	mkdir -p $(GRUB_DIR)
	cp $(BUILD)/$(KERNEL) $(ISO_DIR)/boot/

	# Création de grub.cfg
	echo 'set timeout=0' > $(GRUB_DIR)/grub.cfg
	echo 'set default=0' >> $(GRUB_DIR)/grub.cfg
	echo '' >> $(GRUB_DIR)/grub.cfg
	echo 'menuentry "My OS" {' >> $(GRUB_DIR)/grub.cfg
	echo '    multiboot /boot/$(KERNEL)' >> $(GRUB_DIR)/grub.cfg
	echo '    boot' >> $(GRUB_DIR)/grub.cfg
	echo '}' >> $(GRUB_DIR)/grub.cfg

	grub-mkrescue -o $@ $(ISO_DIR)

run: all
	qemu-system-i386 -cdrom $(ISO)

clean:
	rm -rf $(BUILD) $(ISO)

.PHONY: all run clean
