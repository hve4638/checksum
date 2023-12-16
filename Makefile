CC=g++
CFLAGS=-g -Wall -lcrypto
OBJS=checksum.o cargs.o checksum-sha256.o checksum-md5.o checksum-sum.o opt.o
TESTOBJ=unity.o test.o
TESTLIBOBJS=unity.o
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
	YOUR_SYSTEM=Linux
	TAGS=-D__LINUX -std=c++17
	RM=rm -f
	TARGETEXT=
endif

all: $(TARGET)$(TARGETEXT)

test: $(TESTOBJ) $(OBJS)
	$(CC) -o $(TEST)$(TARGETEXT) $(TESTOBJ) $(OBJS) $(CFLAGS)
	./$(TEST)$(TARGETEXT)

$(TARGET)$(TARGETEXT): $(TARGETOBJ) $(OBJS)
	$(CC) -o $@ $(TARGETOBJ) $(OBJS) $(CFLAGS)

unity.o: unity/unity.c unity/unity.h
	$(CC) -c -o $@ unity/unity.c

test.o: test.cpp checksum.h
	$(CC) -c -o $@ test.cpp $(TAGS)
main.o: main.cpp checksum.h cargs.h opt.h
	$(CC) -c -o $@ main.cpp $(TAGS)
opt.o: opt.cpp opt.h
	$(CC) -c -o $@ opt.cpp $(TAGS)
checksum.o: checksum.cpp checksum.h
	$(CC) -c -o $@ checksum.cpp $(TAGS)
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

info:
	@echo System : $(YOUR_SYSTEM)
	@echo RM : $(RM)
	@echo TAGS : $(TAGS)
	@echo Target : $(TARGET)$(TARGETEXT)