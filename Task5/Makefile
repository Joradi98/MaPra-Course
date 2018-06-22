all: prog

LIBS = -pthread

CXXFLAGS = -O2 -Wall -std=c++11 -D_GLIBCXX_USE_CXX11_ABI=0

prog: a_stern.cpp
	$(CXX) $(CXXFLAGS) -o $@ unit.o a_stern.cpp $(LIBS)


.PHONY: clean
clean:
	rm -f prog
