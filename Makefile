CC=g++
CFLAGS=-O1 -Wall	
OBJS=checksum.o cargs.o checksum-sha256.o checksum-md5.o checksum-sum.o opt.o
TESTOBJ= test.o unity.o printbuffer.o
TARGETOBJ=main.o
TARGET=checksum
TEST=test

ifeq ($(OS),Windows_NT)
	# Windows
	YOUR_SYSTEM=Windows
	TAGS=-D__WINDOWS -std=c++17
	RM=del /Q
	TARGETEXT=.exe
else
	# Linux
	CFLAGS+=-lcrypto
	YOUR_SYSTEM=Linux
	TAGS=-D__LINUX -std=c++17
	RM=rm -f
	TARGETEXT=
endif

all: $(TARGET)$(TARGETEXT)

test: $(TESTOBJ) $(OBJS)
	$(CC) -o $(TEST)$(TARGETEXT) $(TESTOBJ) $(OBJS) $(CFLAGS)
	./$(TEST)$(TARGETEXT)

debug: debug.o $(OBJS)
	$(CC) -o debug$(TARGETEXT) debug.o $(OBJS) $(CFLAGS)
	./debug$(TARGETEXT)
	$(RM) debug$(TARGETEXT)

$(TARGET)$(TARGETEXT): $(TARGETOBJ) $(OBJS)
	$(CC) -o $@ $(TARGETOBJ) $(OBJS) $(CFLAGS)

test.o: test.cpp checksum.h
	$(CC) -c -o $@ test.cpp $(TAGS) $(CFLAGS)
unity.o: unittest/unity.c unittest/unity.h
	$(CC) -c -o $@ unittest/unity.c
printbuffer.o: unittest/printbuffer.c unittest/printbuffer.h
	$(CC) -c -o $@ unittest/printbuffer.c

debug.o: debug.cpp checksum.h cargs.h opt.h
	$(CC) -c -o $@ debug.cpp $(TAGS) $(CFLAGS)
main.o: main.cpp checksum.h cargs.h opt.h
	$(CC) -c -o $@ main.cpp $(TAGS) $(CFLAGS)
opt.o: opt.cpp opt.h
	$(CC) -c -o $@ opt.cpp $(TAGS) $(CFLAGS)
checksum.o: checksum.cpp checksum.h
	$(CC) -c -o $@ checksum.cpp $(TAGS) $(CFLAGS)
checksum-sha256.o: checksum-sha256.cpp checksum.h
	$(CC) -c -o $@ checksum-sha256.cpp $(TAGS) $(CFLAGS)
checksum-md5.o: checksum-md5.cpp checksum.h
	$(CC) -c -o $@ checksum-md5.cpp $(TAGS) $(CFLAGS)
checksum-sum.o: checksum-sum.cpp checksum.h
	$(CC) -c -o $@ checksum-sum.cpp $(TAGS) $(CFLAGS)
cargs.o: cargs.c cargs.h

clean:
	$(RM) *.o
	$(RM) $(TARGET)$(TARGETEXT)
	$(RM) $(TEST)$(TARGETEXT)

info:
	@echo System : $(YOUR_SYSTEM)
	@echo RM : $(RM)
	@echo TAGS : $(TAGS)
	@echo Target : $(TARGET)$(TARGETEXT)