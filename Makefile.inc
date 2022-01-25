BINPATH=/usr/local/bin
CPP=$(BINPATH)/clang++13
LINK=$(BINPATH)/clang++13

.cpp.o:
	$(CPP) -std=c++20 -c $? -o $@ -I../

.PHONY: clean

