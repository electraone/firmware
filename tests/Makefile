CXX = c++
TARGET=firmware.test

BUILDDIR := ./build
STUBSDIR := ./stubs
FAKESDIR := ./fakes
MOCKSDIR := ./mocks
TESTED_FILESDIR := ../src
TESTED_FILES := $(TESTED_FILESDIR)/extram.cpp


C_FILES := $(wildcard *.c)
CPP_FILES := $(wildcard *.cpp)
STUBS_FILES := $(wildcard $(STUBSDIR)/*.cpp)
FAKES_FILES := $(wildcard $(FAKESDIR)/*.cpp)
MOCKS_FILES := $(wildcard $(MOCKSDIR)/*.cpp)

SRC_FILES=$(CPP_FILES) $(STUBS_FILES) $(FAKES_FILES) $(MOCKS_FILES)

CXXSOURCES = $(SRC_FILES) $(TESTED_FILES)

INCDIRS=-I./ -I./$(STUBSDIR) -I./$(FAKESDIR) -I./$(MOCKSDIR) -I../src

CXXFLAGS = -Wall -Werror -c $(INCDIRS) -std=c++11

CPPUTEST_HOME = /usr/local/Cellar/cpputest/4.0
CXXFLAGS += -I$(CPPUTEST_HOME)/include -include "Arduino.h"

LDFLAGS = -lCppUTest -lCppUTestExt

SOURCES := $(C_FILES:.c=.o) $(CPP_FILES:.cpp=.o) \
		   $(STUBS_FILES:.cpp=.o) $(FAKES_FILES:.cpp=.o) \
			 $(MOCKS_FILES:.cpp=.o) $(TESTED_FILES:.cpp=.o)
OBJS := $(foreach src,$(SOURCES), $(BUILDDIR)/$(src))

$(BUILDDIR)/%.o: %.cpp
	@echo -e "[CXX]\t$<"
	@mkdir -p "$(dir $@)"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(BUILDDIR)/$(TARGET): $(OBJS)
	@$(CXX) -lstdc++ -o "$@" $(OBJS) $(LDFLAGS)
	@$(BUILDDIR)/$(TARGET)

all:	$(BUILDDIR)/$(TARGET)

.PHONY: clean
clean:
	@$(RM) -rf "$(BUILDDIR)"
