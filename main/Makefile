all: ../libmain.a

include ../Makefile.inc

clean:
	rm -fv *.a *.o

libmain.a: main.o elf-dumper.o
	ar -r $@ $?

../libmain.a: libmain.a
	cp $? $@

