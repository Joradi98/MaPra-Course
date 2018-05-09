all: test solver

CXXFLAGS = -O2 -Wall

test: vektor.o matrix.o test.o
	$(CXX) $(CXXFLAGS) -o $@ vektor.o matrix.o test.o

test.o: test.cpp vektor.h matrix.h
	$(CXX) $(CXXFLAGS) -c $<

vektor.o: vektor.cpp vektor.h
	$(CXX) $(CXXFLAGS) -c $<

matrix.o: matrix.cpp matrix.h vektor.h
	$(CXX) $(CXXFLAGS) -c $<

solver: IterativeSolver.cpp vektor.cpp matrix.cpp unit.o
	$(CXX) $(CXXFLAGS) -o $@ IterativeSolver.cpp vektor.cpp matrix.cpp unit.o


.PHONY: clean
clean:
	rm -f test vektor.o matrix.o test.o solver
