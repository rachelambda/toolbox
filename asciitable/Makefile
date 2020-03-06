CC=c99
CFLAGS=
debug: CFLAGS=-g
TARGET=ascii
INSTALL_TARGET=/bin/dascii
SOURCE=src/ascii.c

default: $(TARGET)

debug: default

install: default
	cp $(TARGET) $(INSTALL_TARGET)

$(TARGET): $(SOURCE)
	$(CC) $(CFLAGS) $^ -o $@
