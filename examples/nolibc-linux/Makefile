all: akrypt example

akrypt:
	mkdir -p build
	cmake -DCMAKE_C_COMPILER=gcc -DCMAKE_C_FLAGS="-nostdlib -no-pie -Wl,--build-id=none -fcf-protection=none -fno-stack-protector -fno-asynchronous-unwind-tables -fno-unwind-tables -fomit-frame-pointer -Wl,--strip-all" -DAK_SHARED_LIB=OFF -DAK_STATIC_LIB=ON -S ../.. -B build
	cp libakrypt-config.h build
	cat build/libakrypt-config.h
	cd build; make; cd ..

example: example-nolibc.c
	gcc -nostdlib -no-pie -Wl,-e,startup,--build-id=none,--strip-all -fcf-protection=none -fno-stack-protector -fno-asynchronous-unwind-tables -fno-unwind-tables -fomit-frame-pointer -I../../source -I. example-nolibc.c -o example -lakrypt -Lbuild

clean:
	rm -f ./example
	find build -type f | xargs rm -f
	find build -type d | xargs rmdir --ignore-fail-on-non-empty -p

