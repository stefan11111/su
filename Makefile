CFLAGS = -Wall -Wpedantic -Wextra -Wl,-z,now
CFLAGS_RELEASE = ${CFLAGS} -O2 -D_FORTIFY_SOURCE=2
CFLAGS_DEBUG = ${CFLAGS} -O0 -g -fsanitize=undefined
CFLAGS_STATIC = ${CFLAGS_RELEASE} -static-pie
LIBS = -lcrypt
CC = gcc
PREFIX ?= /usr/local

all: su.c su.h
	${CC} ${CFLAGS_RELEASE} su.c -o su ${LIBS}

static: su.c su.h
	${CC} ${CFLAGS_STATIC} su.c -o su ${LIBS}

debug: su.c su.h
	${CC} ${CFLAGS_DEBUG} su.c -o su ${LIBS}

install: su
	mkdir -p ${DESTDIR}${PREFIX}/bin
	mkdir -p ${DESTDIR}${PREFIX}/share/man/man1
	cp su ${DESTDIR}${PREFIX}/bin
	cp su.1 ${DESTDIR}${PREFIX}/share/man/man1
	chown root:root ${DESTDIR}${PREFIX}/bin/su
	chmod 4755 ${DESTDIR}${PREFIX}/bin/su

uninstall:
	rm ${DESTDIR}${PREFIX}/bin/su
	rm ${DESTDIR}${PREFIX}/share/man/man1/su.1

clean:
	rm su
