all: ga

ga: ga.cpp
	g++ -o ga -O3 \
	ga.h ga.cpp util.cpp gen.cpp crossover.cpp selection.cpp mutation.cpp replacement.cpp

run: ga
	./ga < cycle.in > cycle.out

clean:
	rm ga
