CC=g++
CFLAGS=-g -Wall -lcrypto
OBJS=main.o checksum.o cargs.o checksum-sha256.o checksum-md5.o checksum-sum.o test.o opt.o
TARGET=checksum

ifeq ($(OS),Windows_NT)
	# Windows
	YOUR_SYSTEM=Windows
	TAGS=-D__WINDOWS
	RM=del /Q
else
	# Linux
	YOUR_SYSTEM=Linux
	TAGS=-D__LINUX
	RM=rm -f
endif

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) -o $(TARGET) $(OBJS) $(CFLAGS)

main.o: main.cpp opt.cpp checksum.h cargs.h opt.h
opt.o: opt.cpp opt.h
checksum.o: checksum.cpp checksum.h
test.o: checksum.cpp checksum.h
checksum-sha256.o: checksum-sha256.cpp checksum.h
	$(CC) -c -o $@ checksum-sha256.cpp $(TAGS)
checksum-md5.o: checksum-md5.cpp checksum.h
	$(CC) -c -o $@ checksum-md5.cpp $(TAGS)
checksum-sum.o: checksum-sum.cpp checksum.h
	$(CC) -c -o $@ checksum-sum.cpp $(TAGS)
cargs.o: cargs.c cargs.h

clean:
	$(RM) *.o
	$(RM) $(TARGET)

test:
	@echo Your System : $(YOUR_SYSTEM)
	@echo Your RM : $(RM)
	@echo Your TAGS : $(TAGS)