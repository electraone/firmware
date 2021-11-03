#
# Project configuration
#

# Path locations for firmware Loader, firmware image tools, etc
TOOLSPATH = .bin
HEX2IMAGE = $(TOOLSPATH)/hex2k66img
SENDMIDI  = $(TOOLSPATH)/sendmidi
LOADER    = $(TOOLSPATH)/firmwareloader

TOOLCHAINPATH = /usr/local
DOXYGEN = $(TOOLCHAINPATH)/doxygen/doxygen

# Location of the arm-none-eabi compiler
COMPILERPATH = $(TOOLCHAINPATH)/arm/bin

# Location of project scripts
SCRIPTSPATH = scripts


# The name of your project
TARGET = firmware

# Path location of Electra app
APPPATH = demo/app

# Location of the low-level firmware functionality
BASEPATH = base

# Location own and 3rd party libraries
LIBRARYPATH = lib

# Directory to build in
BUILDDIR = build

# Directory with tests
TESTDIR = tests

# Path where the generated documentation will be stored
DOCS = docs

# Optional compiler parameters
OPTIONS = -Wno-deprecated-declarations

# CPU speed
CPU_SPEED = 180000000


#
# Changes to lines below should not be needed
#

# Detect Host system OS
OS := $(shell uname -s)

# Set MIDI CTRL port based on the OS (Linux vs MacOSX)
ifeq ($(OS),Linux)
  CTRL_PORT = "Electra Controller MIDI 3"
else
  CTRL_PORT = "Electra Controller Electra CTRL"
endif


# CPPFLAGS = compiler options for C and C++
CPPFLAGS = -Wall -Os -mthumb -ffunction-sections -fdata-sections \
      -fshort-enums $(OPTIONS) -nostdlib -MMD -DF_CPU=$(CPU_SPEED) \
			-Isrc -I$(BASEPATH)

# compiler options for C++ only
CXXFLAGS = -std=gnu++17 -felide-constructors -fexceptions \
      -fshort-enums -Wno-reorder

# sqlite options
CFLAGS_SQLITE= -DSTDC_HEADERS=1 -DHAVE_SYS_TYPES_H=0 -DHAVE_SYS_STAT_H=0 \
	-DHAVE_STDLIB_H=1 -DHAVE_STRING_H=1 -DHAVE_MEMORY_H=1 -DHAVE_STRINGS_H=1 \
	-DHAVE_INTTYPES_H=1 -DHAVE_STDINT_H=1 -DHAVE_UNISTD_H=1 -DHAVE_DLFCN_H=0 \
	-DLT_OBJDIR=\".libs/\" -DHAVE_FDATASYNC=0 -DHAVE_USLEEP=0 \
	-DHAVE_LOCALTIME_R=0 -DHAVE_GMTIME_R=0 -DHAVE_DECL_STRERROR_R=1 \
	-DHAVE_STRERROR_R=1 -DHAVE_ZLIB_H=0 -DSQLITE_THREADSAFE=0 \
	-DSQLITE_OMIT_LOAD_EXTENSION=1 -DSQLITE_OMIT_WAL=1 -DSQLITE_OS_OTHER=1

# compiler options for C only
CFLAGS = -ggdb $(CFLAGS_SQLITE)

# linker options
LDFLAGS = -Os -Wl,--gc-sections -mthumb

# additional libraries to link
LIBS = -lstdc++ -lm #lib/sqlite/sqlite3.o

# compiler options
CPPFLAGS += -D__MK66FX1M0__ -mcpu=cortex-m4 -mfloat-abi=hard -mfpu=fpv4-sp-d16 \
		-DARDUINO=182 -DTEENSYDUINO # Required to compile Arduino / Teensy libs
LDSCRIPT = $(BASEPATH)/mk66fx1m0.ld
LDFLAGS += -mcpu=cortex-m4 -mfloat-abi=hard -mfpu=fpv4-sp-d16 -T$(LDSCRIPT)


# names for the compiler programs
CC = $(abspath $(COMPILERPATH))/arm-none-eabi-gcc
CXX = $(abspath $(COMPILERPATH))/arm-none-eabi-g++
OBJCOPY = $(abspath $(COMPILERPATH))/arm-none-eabi-objcopy
SIZE = $(abspath $(COMPILERPATH))/arm-none-eabi-size

# automatically create lists of the sources and objects
LC_FILES := $(wildcard $(LIBRARYPATH)/*/*.c)
LCPP_FILES := $(wildcard $(LIBRARYPATH)/**/*.cpp) \
				$(wildcard $(LIBRARYPATH)/*/src/FsLib/*.cpp) \
				$(wildcard $(LIBRARYPATH)/*/src/common/*.cpp) \
				$(wildcard $(LIBRARYPATH)/*/src/iostream/*.cpp) \
				$(wildcard $(LIBRARYPATH)/*/src/ExFatLib/*.cpp) \
				$(wildcard $(LIBRARYPATH)/*/src/DigitalIO/*.cpp) \
			  $(wildcard $(LIBRARYPATH)/*/src/FatLib/*.cpp) \
			  $(wildcard $(LIBRARYPATH)/*/src/SdCard/*.cpp) \
			  $(wildcard $(LIBRARYPATH)/*/src/SpiDriver/*.cpp) \
			  $(wildcard $(LIBRARYPATH)/*/src/*.cpp)
TC_FILES := $(wildcard $(BASEPATH)/*.c)
TCPP_FILES := $(wildcard $(BASEPATH)/*.cpp)
C_FILES := $(wildcard src/*.c)
CPP_FILES := $(wildcard src/*.cpp) \
				$(wildcard src/**/*.cpp)

APPC_FILES := $(wildcard $(APPPATH)/*/*.c) \
				$(wildcard $(APPPATH)/lib/*/*.c)
APPCPP_FILES := $(wildcard $(APPPATH)/*/*.cpp) \
				$(wildcard $(APPPATH)/src/**/*.cpp) \
			  $(wildcard $(APPPATH)/lib/*/*.cpp)

# include paths for libraries
L_INC := $(foreach x,$(filter %/, $(wildcard $(LIBRARYPATH)/**/)), -I$(x)) \
		 $(foreach x,$(filter %/, $(wildcard $(LIBRARYPATH)/**/src/FsLib/)), -I$(x)) \
		 $(foreach x,$(filter %/, $(wildcard $(LIBRARYPATH)/**/src/ExFatLib/)), -I$(x)) \
		 $(foreach x,$(filter %/, $(wildcard $(LIBRARYPATH)/**/src/common/)), -I$(x)) \
		 $(foreach x,$(filter %/, $(wildcard $(LIBRARYPATH)/**/src/DigitalIO/)), -I$(x)) \
		 $(foreach x,$(filter %/, $(wildcard $(LIBRARYPATH)/**/src/FatLib/)), -I$(x)) \
		 $(foreach x,$(filter %/, $(wildcard $(LIBRARYPATH)/**/src/SdCard/)), -I$(x)) \
		 $(foreach x,$(filter %/, $(wildcard $(LIBRARYPATH)/**/src/SpiDriver/)), -I$(x)) \
		 $(foreach x,$(filter %/, $(wildcard $(LIBRARYPATH)/**/src/iostream/)), -I$(x)) \
		 $(foreach x,$(filter %/, $(wildcard $(LIBRARYPATH)/**/src/)), -I$(x))

# include paths for the sources
INC := $(foreach x,$(filter %/, $(wildcard src/**/)), -I$(x))

# include paths for Electra application
L_APP_INC := $(foreach x,$(filter %/, $(wildcard $(APPPATH)/**/*/)), -I$(x)) \
		 $(foreach x,$(filter %/, $(wildcard $(APPPATH)/**/)), -I$(x))


SOURCES := $(C_FILES:.c=.o) $(CPP_FILES:.cpp=.o) \
			 $(APPC_FILES:.c=.o) $(APPCPP_FILES:.cpp=.o) \
		   $(TC_FILES:.c=.o) $(TCPP_FILES:.cpp=.o) \
			 $(LC_FILES:.c=.o) $(LCPP_FILES:.cpp=.o)
OBJS := $(foreach x,$(SOURCES), $(BUILDDIR)/$(x))


all:	image

format:
	@$(SCRIPTSPATH)/format ./src
	@$(SCRIPTSPATH)/format ./demo
	@$(SCRIPTSPATH)/format $(TESTDIR)

pristine:
	@$(SCRIPTSPATH)/ensure-pristine-tree

test:
	make -C $(TESTDIR)

build:	$(BUILDDIR)/$(TARGET).elf

hex:	$(BUILDDIR)/$(TARGET).hex

image:	$(BUILDDIR)/$(TARGET).img

upload: $(BUILDDIR)/$(TARGET).hex
	$(SENDMIDI) dev $(CTRL_PORT) syx 00h 21h 45h 7fh 7fh
	$(LOADER) $<

docs: $(OBJS)
	@$(DOXYGEN)

$(BUILDDIR)/%.o: %.c
	@echo -e "[CC] \t$<"
	@mkdir -p "$(dir $@)"
	@$(CC) $(CPPFLAGS) $(CFLAGS) $(INC) $(L_INC) $(L_APP_INC) -o "$@" -c "$<"

$(BUILDDIR)/%.o: %.cpp
	@echo -e "[CXX]\t$<"
	@mkdir -p "$(dir $@)"
	@$(CXX) $(CPPFLAGS) $(CXXFLAGS) $(INC) $(L_INC) $(L_APP_INC) -o "$@" -c "$<"

$(BUILDDIR)/$(TARGET).elf: $(OBJS) $(LDSCRIPT)
	@echo -e "[LD]\t$@"
	@$(CC) $(LDFLAGS) -o "$@" $(OBJS) $(LIBS)

$(BUILDDIR)/%.hex:	$(BUILDDIR)/%.elf
	@echo -e "[HEX]\t$@"
	@$(SIZE) "$<"
	@$(OBJCOPY) -O ihex -R .eeprom "$<" "$@"

$(BUILDDIR)/%.img:	$(BUILDDIR)/%.hex
	@echo -e "[IMG]\t$@"
	@$(HEX2IMAGE) $< $(basename $<).img

# compiler generated dependency info
-include $(OBJS:.o=.d)

clean:
	@echo Cleaning...
	@rm -rf "$(BUILDDIR)"
	@rm -f "$(TARGET).elf" "$(TARGET).hex"
	@rm -rf $(DOCS)
	@make -C $(TESTDIR) clean
