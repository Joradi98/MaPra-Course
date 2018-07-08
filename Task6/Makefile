CXXFLAGS = -Wall -pedantic
LIBS  = -lGL -lpthread -lX11

all: loesung

loesung.o: loesung.cpp unit.h
	$(CXX) $(CXXFLAGS) -c $<

loesung: loesung.o unit.o IGL.o
	$(CXX) $(CXXFLAGS) -o $@ $^ $(LIBS)

clean:
	rm -f loesung loesung.o

.PHONY: clean
