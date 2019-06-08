.SUFFIXES:

ifeq ($(strip $(DEVKITPRO)),)
$(error "Please set DEVKITPRO in your environment. export DEVKITPRO=<path to>/devkitpro")
endif

TOPDIR ?= $(CURDIR)

include $(DEVKITPRO)/devkitA64/base_rules

all: subsdk1.nso Pleiades.kip

subsdk1.nso:
	@cd Stardust && make

Pleiades.kip:
	@cd Pleiades && make

clean:
	@cd Stardust && make clean
	@cd Pleiades && make clean