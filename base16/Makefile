CC=c99
CFLAGS=
debug: CFLAGS=-g
TARGET=hex
INSTALL_TARGET=/bin/dhex
SOURCE=src/hex.c

default: $(TARGET)

debug: default

install: default
	cp $(TARGET) $(INSTALL_TARGET)

$(TARGET): $(SOURCE)
	$(CC) $(CFLAGS) $^ -o $@
