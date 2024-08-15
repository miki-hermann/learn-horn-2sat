.PHONY: compile horn-closure install clean scratch example

# FLAGS := -O4 -Wfatal-errors
FLAGS := -O4 -march=native -flto -fopenmp -Wfatal-errors
# FLAGS := -g -Og -fopenmp -Wfatal-errors -fsanitize=undefined -fsanitize=address
# FLAGS := -g -fsanitize=undefined -fsanitize=address

compile: learn-horn horn-closure example

learn-horn: matrix+formula.o common.o learn-horn.o
	g++ $(FLAGS) -o learn-horn learn-horn.o common.o matrix+formula.o

learn-horn.o: matrix+formula.hpp common.hpp learn-horn.cpp
	g++ $(FLAGS) -c -o learn-horn.o learn-horn.cpp

horn-closure: matrix+formula.o common.o horn-closure.o
	g++ $(FLAGS) -o horn-closure-v1 horn-closure-v1.o common.o matrix+formula.o
	g++ $(FLAGS) -o horn-closure-v2 horn-closure-v2.o common.o matrix+formula.o

horn-closure.o: matrix+formula.hpp common.hpp horn-closure-v1.cpp  horn-closure-v2.cpp
	g++ $(FLAGS) -c -o horn-closure-v1.o horn-closure-v1.cpp
	g++ $(FLAGS) -c -o horn-closure-v2.o horn-closure-v2.cpp

common.o: common.hpp common.cpp
	g++ $(FLAGS) -c -o common.o common.cpp

matrix+formula.o: matrix+formula.hpp matrix+formula.cpp
	g++ $(FLAGS) -c -o matrix+formula.o matrix+formula.cpp

example: digit.cpp digit.csv.xz
	g++ $(FLAGS) -o digit digit.cpp

clean:
	rm -f *.o
	rm -f *~

scratch: clean
	rm -f learn-horn
	rm -f horn-closure-v1 horn-closure-v2
	rm -f *.tex
	rm -f digit digit[0-9].csv

install: learn-horn
	sudo mkdir -p /usr/local/bin
	sudo cp -f learn-horn /usr/local/bin
	sudo cp -f horn-closure-v1 horn-closure-v2 /usr/local/bin
	sudo cp -f digit /usr/local/bin
