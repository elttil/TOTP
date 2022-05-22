CFLAGS=-Wall -pedantic -Werror -Wimplicit-fallthrough
LIBS=-lm

all: totp

totp: totp.c hotp.c hmac.c SHA1/sha1.c base32.c
	$(CC) $(CFLAGS) $(LIBS) $^ -o $@

install:
	mkdir -p /usr/bin
	cp -f totp /usr/bin
	chmod 755 /usr/bin/totp

run:
	./totp

clean:
	rm totp
