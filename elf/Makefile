all: ../libelf.a

include ../Makefile.inc

clean:
	rm -fv *.a *.o

libelf.a: elf.o
	ar -r $@ $?

../libelf.a: libelf.a
	cp $? $@

