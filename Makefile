PEFIX=$(HOME)
CC=g++
AR=ar
CFLAGS= -ansi -Wall -O3
LDFLAGS= -L. -lfhew -lfftw3 
INCLUDE=distrib.h LWE.h FHEW.h FFT.h params.h variables.h

all: libfhew.a cmd fhewTest

cmd: cmd/gen cmd/enc cmd/nand cmd/dec cmd/add cmd/carry cmd/sub cmd/mul cmd/multi cmd/addF cmd/encF cmd/decF

install: $(INCLUDE) libfhew.a
	install $(INCLUDE) $(PREFIX)/include
	install libfhew.a $(PREFIX)/lib

uninstall:
	rm $(PREFIX)/lib/libfhew.a
	rm $(PREFIX)/include/{distrib,LWE,FHEW,FFT,params}.h

clean:
	rm *.o libfhew.a fhewTest cmd/gen cmd/enc cmd/dec cmd/nand cmd/add cmd/carry cmd/sub cmd/mul || echo nothing to clean

libfhew.a: distrib.o FFT.o LWE.o FHEW.o
	$(AR) -q libfhew.a distrib.o FFT.o LWE.o FHEW.o

distrib.o: distrib.cpp distrib.h params.h
	$(CC) $(CFLAGS) -c distrib.cpp

FFT.o: FFT.h FFT.cpp params.h FHEW.h
	$(CC) $(CFLAGS) -c FFT.cpp
				
LWE.o: LWE.h LWE.cpp FFT.h params.h distrib.h
	$(CC) $(CFLAGS) -c LWE.cpp

FHEW.o: FHEW.h FHEW.cpp FFT.h LWE.h params.h
	$(CC) $(CFLAGS) -c FHEW.cpp

fhewTest: fhewTest.cpp libfhew.a
	$(CC) $(CFLAGS) -o fhewTest fhewTest.cpp $(LDFLAGS)

common.o: cmd/common.cpp cmd/common.h cmd/variables.h libfhew.a
	$(CC) $(CFLAGS) -c cmd/common.cpp 

cmd/gen: cmd/gen.cpp common.o libfhew.a
	$(CC) $(CFLAGS) -o cmd/gen cmd/gen.cpp common.o $(LDFLAGS)

cmd/enc: cmd/enc.cpp common.o libfhew.a
	$(CC) $(CFLAGS) -o cmd/enc cmd/enc.cpp common.o $(LDFLAGS)

cmd/nand: cmd/nand.cpp common.o libfhew.a
	$(CC) $(CFLAGS) -o cmd/nand cmd/nand.cpp common.o $(LDFLAGS)

cmd/add: cmd/add.cpp common.o libfhew.a
	$(CC) $(CFLAGS) -o cmd/add cmd/add.cpp common.o $(LDFLAGS)

cmd/sub: cmd/sub.cpp common.o libfhew.a
	$(CC) $(CFLAGS) -o cmd/sub cmd/sub.cpp common.o $(LDFLAGS)

cmd/carry: cmd/carry.cpp common.o libfhew.a
	$(CC) $(CFLAGS) -o cmd/carry cmd/carry.cpp common.o $(LDFLAGS)

cmd/mul: cmd/mul.cpp common.o libfhew.a
	$(CC) $(CFLAGS) -o cmd/mul cmd/mul.cpp common.o $(LDFLAGS)

cmd/multi: cmd/multi.cpp common.o libfhew.a
	$(CC) $(CFLAGS) -o cmd/multi cmd/multi.cpp common.o $(LDFLAGS)

cmd/addF: cmd/addF.cpp common.o libfhew.a
	$(CC) $(CFLAGS) -o cmd/addF cmd/addF.cpp common.o $(LDFLAGS)

cmd/encF: cmd/encF.cpp common.o libfhew.a
	$(CC) $(CFLAGS) -o cmd/encF cmd/encF.cpp common.o $(LDFLAGS)

cmd/dec: cmd/dec.cpp common.o libfhew.a
	$(CC) $(CFLAGS) -o cmd/dec cmd/dec.cpp common.o $(LDFLAGS)

cmd/decF: cmd/decF.cpp common.o libfhew.a
	$(CC) $(CFLAGS) -o cmd/decF cmd/decF.cpp common.o $(LDFLAGS)
