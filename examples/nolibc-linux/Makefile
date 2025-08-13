CFLAGS = -nostdlib -no-pie -Wl,--build-id=none,--strip-all -fcf-protection=none -fno-stack-protector -fno-asynchronous-unwind-tables -fno-unwind-tables -fomit-frame-pointer -ffunction-sections -Wl,--gc-sections

HASH_SOURCE_01 = example-hash-01.c ../../source/ak_tools.c ../../source/ak_hash.c ../../source/ak_mac.c
HASH_SOURCE_02 = example-hash-02.c ../../source/ak_tools.c ../../source/ak_hash.c ../../source/ak_mac.c

all: akrypt hello hash

akrypt:
	mkdir -p build
	cmake -DCMAKE_C_COMPILER=gcc -DCMAKE_C_FLAGS="${CFLAGS}" -DAK_SHARED_LIB=OFF -DAK_STATIC_LIB=ON -S ../.. -B build
	cp libakrypt-config.h build
	cat build/libakrypt-config.h
	cd build; make; cd ..

hello: example-hello.c
	gcc ${CFLAGS} -Wl,-e,startup -I../../source -I. example-hello.c -o hello -lakrypt -Lbuild

hash: ${HASH_SOURCE_01}
	gcc ${CFLAGS} -Wl,-e,startup -I../../source -I. ${HASH_SOURCE_01} -o hash-01
	gcc ${CFLAGS} -Wl,-e,startup -I../../source -I. ${HASH_SOURCE_02} -o hash-02

clean:
	rm -f hello hash-01 hash-02
	find build -type f | xargs rm -f
	find build -type d | xargs rmdir --ignore-fail-on-non-empty -p

