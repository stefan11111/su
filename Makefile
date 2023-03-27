CFLAGS = -Wall -Wpedantic -Wextra -Werror -Wl,-z,now
CFLAGS_RELEASE = ${CFLAGS} -O2 -s -D_FORTIFY_SOURCE=2
CFLAGS_DEBUG = ${CFLAGS} -O0 -g -fsanitize=undefined
CFLAGS_STATIC = ${CFLAGS_RELEASE} -static-pie
LIBS = -lcrypt
CC = gcc

all: su.c su.h
	${CC} ${CFLAGS_RELEASE} su.c -o su ${LIBS}

static: su.c su.h
	${CC} ${CFLAGS_STATIC} su.c -o su ${LIBS}

debug: su.c su.h
	${CC} ${CFLAGS_DEBUG} su.c -o su ${LIBS}

install: su
	cp su ${DESTDIR}/usr/local/bin
	cp su.1 ${DESTDIR}/usr/local/share/man/man1
	chown root:root ${DESTDIR}/usr/local/bin/su
	chmod 4755 ${DESTDIR}/usr/local/bin/su

uninstall:
	rm ${DESTDIR}/usr/local/bin/su
	rm ${DESTDIR}/usr/local/share/man/man1/su.1

clean:
	rm su
