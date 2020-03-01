CC=c99
CFLAGS=
debug: CFLAGS=-g
TARGET=xor
INSTALL_TARGET=/bin/dxor
SOURCE=src/xor.c

default: $(TARGET)

debug: default

install: default
	cp $(TARGET) $(INSTALL_TARGET)

$(TARGET): $(SOURCE)
	$(CC) $(CFLAGS) $^ -o $@
