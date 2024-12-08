BINPATH=/usr/bin
CPP=$(BINPATH)/clang++
LINK=$(BINPATH)/clang++

.cpp.o:
	$(CPP) -std=c++20 -c $? -o $@ -I../

.PHONY: clean

