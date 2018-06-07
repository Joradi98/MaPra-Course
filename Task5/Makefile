all: prog

LIBS = -pthread

CXXFLAGS = -O2 -Wall -std=c++11

prog: a_stern.cpp
	$(CXX) $(CXXFLAGS) -o $@ unit.o a_stern.cpp $(LIBS)


.PHONY: clean
clean:
	rm -f prog
