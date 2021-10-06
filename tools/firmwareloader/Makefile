MKDIR = mkdir

PLATFORM :=
ifeq ($(OS), Windows_NT)
	PLATFORM := WINDOWS
else
	UNAME := $(shell uname -s)
	ifeq ($(UNAME), Darwin)
		PLATFORM := DARWIN
	else ifeq ($(UNAME), Linux)
		PLATFORM := LINUX
	else ifeq ($(UNAME), FreeBSD)
		PLATFORM := LINUX
	else ifeq ($(UNAME), NetBSD)
		PLATFORM := BSD
	else ifeq ($(UNAME), OpenBSD)
		PLATFORM := BSD
	else
		$(error "Unsupported platform: $(UNAME)")
	endif
endif

# uncomment this to use libusb on Macintosh, instead of Apple's HID manager via IOKit
# this is technically not the "correct" way to support Macs, but it's been reported to
# work.
#USE_LIBUSB ?= YES

TARGET = firmwareloader
BUILDDIR = $(CURDIR)/build

ifeq ($(PLATFORM), LINUX)  # also works on FreeBSD
CC ?= gcc
CFLAGS ?= -O2 -Wall
firmwareloader: firmwareloader.c
	@$(MKDIR) -p "$(BUILDDIR)"
	$(CC) $(CFLAGS) $(CPPFLAGS) -s -DUSE_LIBUSB -o "$(BUILDDIR)/$(TARGET)" firmwareloader.c -lusb $(LDFLAGS)

else ifeq ($(PLATFORM), WINDOWS)
CC = i586-mingw32msvc-gcc
CFLAGS ?= -O2 -Wall
firmwareloader.exe: firmwareloader.c
	@$(MKDIR) -p "$(BUILDDIR)"
	$(CC) $(CFLAGS) -s -DUSE_WIN32 -o "$(BUILDDIR)/$(TARGET).exe" firmwareloader.c -lhid -lsetupapi -lwinmm

else ifeq ($(PLATFORM), DARWIN)
ifeq ($(USE_LIBUSB), YES)
CC ?= gcc
CFLAGS ?= -O2 -Wall -mmacosx-version-min=10.5
firmwareloader: firmwareloader.c
	@$(MKDIR) -p "$(BUILDDIR)"
	$(CC) $(CFLAGS) -s -DUSE_LIBUSB -DMACOSX -o "$(BUILDDIR)/$(TARGET)" firmwareloader.c -lusb -I /usr/local/include -L/usr/local/lib
else
CC ?= gcc
SDK ?= $(shell xcrun --show-sdk-path)
CFLAGS ?= -O2 -Wall
firmwareloader: firmwareloader.c
ifeq ($(SDK),)
	$(error SDK was not found. To use this type of compilation please install Xcode)
endif
	@$(MKDIR) -p "$(BUILDDIR)"
	$(CC) $(CFLAGS) -DUSE_APPLE_IOKIT -isysroot $(SDK) -o "$(BUILDDIR)/$(TARGET)" firmwareloader.c -Wl,-syslibroot,$(SDK) -framework IOKit -framework CoreFoundation
endif

else ifeq ($(PLATFORM), BSD)  # works on NetBSD and OpenBSD
CC ?= gcc
CFLAGS ?= -O2 -Wall
firmwareloader: firmwareloader.c
	@$(MKDIR) -p "$(BUILDDIR)"
	$(CC) $(CFLAGS) -s -DUSE_UHID -o "$(BUILDDIR)/$(TARGET)" firmwareloader.c
endif

all: firmwareloader

clean:
	@rm -rf "$(BUILDDIR)"
