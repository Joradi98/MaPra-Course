CXX      = g++
CXXFLAGS = -std=c++11 -Wall -pedantic -Wextra -g -O2 -Ilib/SFML-2.5.0/include/ -D_GLIBCXX_USE_CXX11_ABI=0

LIBS = -Llib/SFML-2.5.0/lib/ -lsfml-graphics -lsfml-window -lsfml-system 

.PHONY: clean run debug gdb

a_stern: a_stern.o
	$(CXX) $(CXXFLAGS) -o $@ $^ unit.o $(LIBS)

a_stern.o: a_stern.cpp
	$(CXX) $(CXXFLAGS) -c $<

clean:
	rm -f a_stern.o a_stern

run: a_stern
	export LD_LIBRARY_PATH=./lib/SFML-2.5.0/lib && ./a_stern

debug: a_stern
	export LD_LIBRARY_PATH=./lib/SFML-2.5.0/lib && kdbg ./a_stern

gdb: a_stern
	gdb -ex "set environment LD_LIBRARY_PATH=./lib/SFML-2.5.0/lib" a_stern
