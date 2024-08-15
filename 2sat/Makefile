.PHONY: compile install clean scratch

# FLAGS := -O4 -Wfatal-errors
FLAGS := -O4 -march=native -flto -fopenmp -Wfatal-errors
# FLAGS := -g -Og -fopenmp -Wfatal-errors -fsanitize=undefined -fsanitize=address
# FLAGS := -g -fsanitize=undefined -fsanitize=address

compile: learn-2sat

learn-2sat: matrix+formula.o common.o bucket.o learn-2sat.o
	g++ $(FLAGS) -o learn-2sat learn-2sat.o bucket.o common.o matrix+formula.o

learn-2sat.o: matrix+formula.hpp common.hpp bucket.hpp learn-2sat.cpp
	g++ $(FLAGS) -c -o learn-2sat.o learn-2sat.cpp

bucket.o: matrix+formula.hpp common.hpp bucket.hpp bucket.cpp
	g++ $(FLAGS) -c -o bucket.o bucket.cpp

common.o: matrix+formula.hpp common.hpp common.cpp
	g++ $(FLAGS) -c -o common.o common.cpp

matrix+formula.o: matrix+formula.hpp matrix+formula.cpp
	g++ $(FLAGS) -c -o matrix+formula.o matrix+formula.cpp

clean:
	rm -f *.o
	rm -f *~

scratch: clean
	rm -f learn-2sat
	rm -f *.tex *.mvc

install: learn-2sat
	sudo mkdir -p /usr/local/bin
	sudo cp -f learn-2sat /usr/local/bin
