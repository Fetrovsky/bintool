all: bintool

include Makefile.inc

clean:
	rm -fv lib*.a
	cd main && make clean
	cd elf && make clean

libmain.a:
	cd main && make

libelf.a:
	cd elf && make

bintool: libmain.a libelf.a
	$(LINK) -o $@ $?

