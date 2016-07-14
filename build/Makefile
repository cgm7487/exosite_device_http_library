CC = gcc
CFLAGS = -g -Wall
INC = -I ../source -I ../include
LDFLAGS += -lrt -lm -pthread -static -L../lib -lexositehttp -lcyassl

.PHONY: clean

all: release linux-test

.c.o:
	${CC} ${CFLAGS} -lstatic ${INC} -Os -c ../source/exosite_http.c \
	     -o ../source/exosite_http.o

	${CC} ${CFLAGS} -lstatic ${INC} -Os -c ../source/utility.c \
	     -o ../source/utility.o

	${CC} ${CFLAGS} -lstatic ${INC} -Os -c ../source/platform/exosite_pal_linux_non_security.c \
	     -o ../source/platform/exosite_pal.o

release: ../source/exosite_http.o ../source/utility.o ../source/platform/exosite_pal.o
	ar rcs ../lib/libexositehttp.a ../source/exosite_http.o ../source/utility.o ../source/platform/exosite_pal.o

linux-test: ../application/test/source/test_linux_socket.c
	${CC} ${CFLAGS} ${INC} ../application/test/source/test_linux_socket.c \
	     -o ../application/test/bin/test_linux ${LDFLAGS}

clean:
	@rm -rf *.o *.a ../application/test/bin/* ../lib/libexositehttp.a ../source/*.o ../source/platform/*.o
