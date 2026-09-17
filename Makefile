# Fetch the current directory name
CURRENT_DIR_NAME := $(shell basename $(shell pwd))

# URL to download the iso if not available in current dir
ISO_URL := https://cdn.intra.42.fr/isos/$(CURRENT_DIR_NAME).iso

HOSTNAME := $(shell uname -n)
ifeq ($(HOSTNAME),SapphireWIN)
QEMU_CMD := sudo qemu-system-x86_64
else
QEMU_CMD := qemu-system-x86_64
endif

PROJECTS_ISO_DIR := /sgoinfre/goinfre/ISO/ProjectsIso

.PHONY: run
run:
	@ISO=""; \
	if [ -d "$(PROJECTS_ISO_DIR)" ]; then \
		echo "ISO directory found: $(PROJECTS_ISO_DIR)"; \
		echo ""; \
		ISOS="$$(find "$(PROJECTS_ISO_DIR)" -maxdepth 1 -type f -name '*.iso' -printf '%f\n' | sort)"; \
		if [ -z "$$ISOS" ]; then \
			echo "No ISO found in $(PROJECTS_ISO_DIR)."; \
			exit 1; \
		fi; \
		echo "Available ISOs:"; \
		i=1; \
		for iso in $$ISOS; do \
			echo "  $$i) $$iso"; \
			i=$$((i + 1)); \
		done; \
		echo ""; \
		printf "Choose an ISO [1-$$((i - 1))]: "; \
		read choice; \
		case "$$choice" in \
			''|*[!0-9]*) \
				echo "Invalid choice."; \
				exit 1; \
				;; \
		esac; \
		ISO="$$(printf '%s\n' "$$ISOS" | sed -n "$${choice}p")"; \
		if [ -z "$$ISO" ]; then \
			echo "Invalid choice."; \
			exit 1; \
		fi; \
		ISO="$(PROJECTS_ISO_DIR)/$$ISO"; \
	else \
		ISO="$$(ls *.iso 2>/dev/null | head -n 1)"; \
		if [ -z "$$ISO" ]; then \
			echo "No ISO found in the directory."; \
			echo "Downloading from $(ISO_URL)..."; \
			curl -f -o "$(CURRENT_DIR_NAME).iso" "$(ISO_URL)" || { \
				echo "The ISO file couldn't be downloaded"; \
				exit 1; \
			}; \
			echo "Downloaded successfully."; \
			ISO="$(CURRENT_DIR_NAME).iso"; \
		else \
			echo "Using iso: $$ISO"; \
		fi; \
	fi; \
	if [ ! -f "$$ISO" ]; then \
		echo "ISO file '$$ISO' not found."; \
		exit 1; \
	fi; \
	echo "Starting $$ISO using QEMU"; \
	$(QEMU_CMD) -nic user,hostfwd=tcp::4240-:4242 -enable-kvm -m 2G -boot d -cdrom "$$ISO" -vga virtio
