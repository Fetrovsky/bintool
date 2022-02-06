all: bintool

include Makefile.inc

clean:
	rm -fv lib*.a
	cd main && make clean
	cd elf && make clean
	cd mz && make clean

libmain.a:
	cd main && make

libelf.a:
	cd elf && make

libmz.a:
	cd mz && make

bintool: libmain.a libelf.a libmz.a
	$(LINK) -o $@ $?

