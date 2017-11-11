TOOLSET=/clang5
BINPATH=$(TOOLSET)/bin
CPP=$(BINPATH)/clang++
LINK=$(BINPATH)/clang++

.cpp.o:
	$(CPP) -std=c++17 -c $? -o $@ -I../

.PHONY: clean

