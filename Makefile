CC=g++
CFLAGS=-O1 -Wall -std=c++17
ODIR=.\\obj\\other\\
BDIR=.\\build\\
TARGET=checksum
TESTTARGET=test
VERSION=0.0.0
SOURCEDIR=.\\

ifeq ($(OS),Windows_NT)
# Windows
	YOUR_SYSTEM=Windows
	
	CFLAGS+=-D__WINDOWS -DU16
	RM=del /Q
	EXT=.exe
	ODIR=.\\obj\\windows\\
else
# Linux
	YOUR_SYSTEM=Linux

	CFLAGS+=-D__LINUX -DU8 -lcrypto
	RM=rm -f
	EXT=
	ODIR=./obj/linux/
	BDIR=./build/
	SOURCEDIR=./
endif

OBJS=$(ODIR)checksum.o $(ODIR)cargs.o $(ODIR)checksum-sha256.o $(ODIR)checksum-md5.o \
			$(ODIR)checksum-sum.o $(ODIR)opt.o $(ODIR)makehash.o
TESTOBJ=$(ODIR)test.o $(ODIR)unity.o $(ODIR)printbuffer.o
TARGETOBJ=$(ODIR)main.o $(ODIR)printbuffer.o
DEBUGOBJ=$(ODIR)debug.o $(ODIR)printbuffer.o

%.o: %.cpp

all: $(BDIR)$(TARGET)$(EXT)

test: $(TESTOBJ) $(OBJS)
	$(CC) -o $(BDIR)$(TESTTARGET)$(EXT) $^ $(CFLAGS)
	./$(BDIR)$(TESTTARGET)$(EXT)

debug: $(DEBUGOBJ) $(OBJS)
	$(CC) -o $(BDIR)debug$(EXT) $^ $(CFLAGS)
	$(BDIR)debug$(EXT)
	$(RM) $(BDIR)debug$(EXT)

$(BDIR)$(TARGET)$(EXT): $(TARGETOBJ) $(OBJS)
	$(CC) -o $@ $^ $(CFLAGS)

$(ODIR)test.o: test.cpp checksum.h
	$(CC) -c -o $@ test.cpp $(CFLAGS)
$(ODIR)unity.o: unittest/unity.c unittest/unity.h
	$(CC) -c -o $@ unittest/unity.c
$(ODIR)printbuffer.o: unittest/printbuffer.c unittest/printbuffer.h
	$(CC) -c -o $@ unittest/printbuffer.c

$(ODIR)debug.o: debug.cpp checksum.h cargs.h opt.h
	$(CC) -c -o $@ debug.cpp $(CFLAGS)
$(ODIR)opt.o: opt.cpp opt.h
	$(CC) -c -o $@ opt.cpp $(CFLAGS)
$(ODIR)checksum.o: checksum.cpp checksum.h
	$(CC) -c -o $@ checksum.cpp $(CFLAGS)
$(ODIR)checksum-sha256.o: checksum-sha256.cpp checksum.h
	$(CC) -c -o $@ checksum-sha256.cpp $(CFLAGS)
$(ODIR)checksum-md5.o: checksum-md5.cpp checksum.h
	$(CC) -c -o $@ checksum-md5.cpp $(CFLAGS)
$(ODIR)checksum-sum.o: checksum-sum.cpp checksum.h
	$(CC) -c -o $@ checksum-sum.cpp $(CFLAGS)
$(ODIR)makehash.o: makehash.cpp makehash.h
	$(CC) -c -o $@ makehash.cpp $(CFLAGS)
$(ODIR)cargs.o: cargs.c cargs.h
	$(CC) -c -o $@ cargs.c
$(ODIR)main.o: main.cpp checksum.h cargs.h opt.h
	$(CC) -c -o $@ main.cpp $(CFLAGS) -DVERSION=\"$(VERSION)\"

clean:
	$(RM) $(ODIR)*.o
	$(RM) $(BDIR)$(TARGET)$(EXT)
#$(RM) $(TARGET)$(EXT)
#$(RM) $(TEST)$(EXT)

info:
	@echo System : $(YOUR_SYSTEM)
	@echo obj directory : $(ODIR)
	@echo RM : $(RM)
	@echo CFLAGS : $(CFLAGS)
	@echo Target : $(TARGET)$(EXT)