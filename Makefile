all: ga

ga: ga.cpp
	g++ -o ga -std=c++11 -O3 -Wno-unused-result  \
	ga.h ga.cpp util.cpp gen.cpp crossover.cpp selection.cpp mutation.cpp replacement.cpp

run: ga
	./ga config < cycle.in > cycle.out

clean:
	rm ga
