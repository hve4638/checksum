CC=g++
CFLAGS=-g -Wall -lcrypto
OBJS=main.o makehash.o cargs.o
TARGET=checksum

all: $(TARGET)
$(TARGET): $(OBJS)
	$(CC) -o $(TARGET) $(OBJS) $(CFLAGS)

main.o: main.cpp makehash.h cargs.h
makehash.o: makehash.cpp makehash.h
cargs.o: cargs.c cargs.h

clean:
	rm -rf *.o
	rm $(TARGET)