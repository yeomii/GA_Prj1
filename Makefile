all: ga

ga: ga.cpp
	g++ -o ga -std=c++0x -O3 \
	ga.h ga.cpp util.cpp gen.cpp crossover.cpp selection.cpp mutation.cpp replacement.cpp localopt.cpp

run: ga
	./ga < cycle.in > cycle.out

clean:
	rm ga
