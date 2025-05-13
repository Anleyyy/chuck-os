#!/bin/bash

set -e

# Config
KERNEL=kernel.elf
ISO_NAME=os.iso
BUILD_DIR=build
ISO_DIR=$BUILD_DIR/iso
GRUB_DIR=$ISO_DIR/boot/grub

echo "[+] Nettoyage..."
rm -rf "$BUILD_DIR"
mkdir -p "$GRUB_DIR"

# Compilation
echo "[+] Compilation avec NASM..."
nasm -f elf32 loader.asm -o "$BUILD_DIR/loader.o"

echo "[+] Linkage..."
ld -T link.ld -m elf_i386 "$BUILD_DIR/loader.o" -o "$BUILD_DIR/$KERNEL"

# Copie kernel et config GRUB
echo "[+] Copie dans l'arborescence ISO..."
cp "$BUILD_DIR/$KERNEL" "$ISO_DIR/boot/"

cat > "$GRUB_DIR/grub.cfg" <<EOF
set timeout=0
set default=0

menuentry "Mon OS" {
    multiboot /boot/$KERNEL
    boot
}
EOF

# Assure que tout est bien là
if [ ! -d "$ISO_DIR/boot" ]; then
    echo "ERREUR : Le dossier iso/boot est manquant"
    exit 1
fi

# Création ISO
echo "[+] Création de l'ISO..."
grub-mkrescue -o "$ISO_NAME" "$ISO_DIR"

# Lancement QEMU
echo "[+] Lancement QEMU..."
qemu-system-i386 -cdrom "$ISO_NAME"
