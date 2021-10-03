CC = gcc
CFLAGS = -Wall -pedantic
MKDIR = mkdir

TARGET = hex2k66img
BUILDDIR = $(abspath $(CURDIR)/build)

all: $(TARGET)

$(TARGET): $(TARGET).c
	@$(MKDIR) -p $(BUILDDIR)
	$(CC) $(CFLAGS) -o $(BUILDDIR)/$(TARGET) $(TARGET).c

clean:
	@$(RM) -r $(BUILDDIR)
