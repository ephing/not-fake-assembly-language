CPPFILES := $(wildcard *.cpp)
OBJFILES := $(CPPFILES:.cpp=.o)
HFILES := $(wildcard *.hpp)

.PHONY: clean test

emulator: $(OBJFILES)
	g++ -Wall -std=c++14 -g $(OBJFILES) -o $@

%.o: %.cpp $(HFILES)
	g++ -Wall -std=c++14 -g -c -o $@ $<

test: emulator
	time ./emulator test.ass

clean:
	rm emulator *.o