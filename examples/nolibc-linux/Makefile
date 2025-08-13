CFLAGS = -nostdlib \
	-no-pie \
	-fcf-protection=none \
	-fno-stack-protector \
	-fno-asynchronous-unwind-tables \
	-fno-unwind-tables \
	-fomit-frame-pointer \
	-ffunction-sections \
	-Wl,--build-id=none,--strip-all,--gc-sections \
	-Os

INCLUDE = -I../../source -I.

HELLO_SOURCES = nolibc-hello.c ../../source/ak_tools.c ../../source/ak_options.c ../../source/ak_libakrypt.c

hello: $(HELLO_SOURCES)
	gcc $(INCLUDE) $(HELLO_SOURCES) -o hello-libc
	gcc $(INCLUDE) $(HELLO_SOURCES) ../../source/ak_string.c -nostdlib -Wl,-e,startup -Os -o hello-nolibc
	gcc $(INCLUDE) $(HELLO_SOURCES) ../../source/ak_string.c $(CFLAGS) -Wl,-e,startup -o hello-nolibc-strip


HASH_SOURCES = nolibc-hash.c ../../source/ak_tools.c ../../source/ak_mac.c ../../source/ak_hash.c 

hash: $(HASH_SOURCES)
	gcc $(INCLUDE) $(HASH_SOURCES) -o hash-libc
	gcc $(INCLUDE) $(HASH_SOURCES) ../../source/ak_string.c -nostdlib -Wl,-e,startup -o hash-nolibc
	gcc $(INCLUDE) $(HASH_SOURCES) ../../source/ak_string.c $(CFLAGS) -Wl,-e,startup -o hash-nolibc-strip
	gcc $(INCLUDE) $(HASH_SOURCES) ../../source/ak_string.c -DAK_HAVE_SLOW_HASH $(CFLAGS) -Wl,-e,startup -o hash-nolibc-strip-slow


# служба очистки
clean:
	rm -f *-libc *-nolibc *-nolibc-strip *-nolibc-strip-slow

