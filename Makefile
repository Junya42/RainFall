# Fetch the current directory name
CURRENT_DIR_NAME := $(shell basename $(shell pwd))

# Find the first iso file in the directory
ISO ?= $(shell ls *.iso 2>/dev/null | head -n 1)

# URL to download the iso if not available in current dir
ISO_URL := https://cdn.intra.42.fr/isos/$(CURRENT_DIR_NAME).iso

.PHONY: run
run:
	@if [ -z "$(ISO)" ]; then \
		echo "No ISO found in the directory."; \
		echo "Downloading from $(ISO_URL)..."; \
		curl -f -o "$(CURRENT_DIR_NAME).iso" "$(ISO_URL)" || { \
			echo "The ISO file couldn't be downloaded"; \
			exit 1; \
		}; \
		echo "Downloaded successfully."; \
		ISO="$(CURRENT_DIR_NAME).iso"; \
	else \
		echo "Using iso : $(ISO)"; \
		ISO="$(ISO)"; \
	fi; \
	if [ ! -f "$$ISO" ]; then \
		echo "ISO file '$$ISO' not found."; \
		exit 1; \
	fi; \
	echo "Starting $$ISO using QEMU"; \
	sudo qemu-system-x86_64 -nic user,hostfwd=tcp::4242-:4242 -enable-kvm -m 2G -boot d -cdrom "$$ISO" -vga virtio

