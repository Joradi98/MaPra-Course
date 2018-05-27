all: prog

LIBS = -lX11 -pthread

CXXFLAGS = -O2 -Wall -std=c++11

prog: vierGewinnt.cpp plotx11.o unit.o
	$(CXX) $(CXXFLAGS) -o $@ vierGewinnt.cpp plotx11.o unit.o $(LIBS)


.PHONY: clean
clean:
	rm -f prog
