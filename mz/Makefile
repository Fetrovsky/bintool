all: ../libmz.a

include ../Makefile.inc

clean:
	rm -fv *.a *.o

libmz.a: mz.o
	ar -r $@ $?

../libmz.a: libmz.a
	cp $? $@

