PWD := $(shell pwd)

ARCH ?= esp32
BOARD ?= ATK_DNESP32S3B
C_MODULES_DIR ?= $(PWD)/cmodules

BOARD_DIR := $(PWD)/boards/$(ARCH)/$(BOARD)
MICROPYTHON_PORT_DIR := $(PWD)/components/micropython/ports/$(ARCH)
BUILD_DIR := $(PWD)/build-$(ARCH)-$(BOARD)

ifeq ($(wildcard $(BOARD_DIR)/.),)
$(error Invalid BOARD specified: $(BOARD_DIR))
endif

.PHONY: all clean deploy submodules

all:
	$(MAKE) -C $(MICROPYTHON_PORT_DIR) BOARD_DIR=$(BOARD_DIR) USER_C_MODULES=$(C_MODULES_DIR) BUILD=$(BUILD_DIR)

clean:
	$(MAKE) -C $(MICROPYTHON_PORT_DIR) BOARD_DIR=$(BOARD_DIR) USER_C_MODULES=$(C_MODULES_DIR) BUILD=$(BUILD_DIR) clean

deploy:
	$(MAKE) -C $(MICROPYTHON_PORT_DIR) BOARD_DIR=$(BOARD_DIR) USER_C_MODULES=$(C_MODULES_DIR) BUILD=$(BUILD_DIR) deploy

submodules:
	$(MAKE) -C $(MICROPYTHON_PORT_DIR) BOARD_DIR=$(BOARD_DIR) USER_C_MODULES=$(C_MODULES_DIR) BUILD=$(BUILD_DIR) submodules
