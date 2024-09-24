.PHONY: all clean scratch

all: accent-fr.out \
     diabetes10.out diabetes-280-2.out diabetes-280-3.out diabetes-280-5.out \
	divorce.out \
	graph24000.out \
	prime.out \
	shuttle.out \
	sport.out \
	test1a.out \
	test1.out \
	test2.out \
	test3.out \
	test4.out \
	test5.out \
	three.out \
	two.out

accent-fr.out: accent-fr.csv
	learn-2sat -i accent-fr.csv -o accent-fr.out

banknotes.out: banknotes.csv
	learn-2sat -i banknotes.csv -o banknotes.out

diabetes10.out: diabetes10.csv
	learn-2sat -i diabetes10.csv -o diabetes10.out

diabetes-280-2.out: diabetes-280-2.csv
	learn-2sat -i diabetes-280-2.csv -o diabetes-280-2.out

diabetes-280-3.out: diabetes-280-3.csv
	learn-2sat -i diabetes-280-3.csv -o diabetes-280-3.out

diabetes-280-5.out: diabetes-280-5.csv
	learn-2sat -i diabetes-280-5.csv -o diabetes-280-5.out

diabetes.out: diabetes.csv
	learn-2sat -i diabetes.csv -o diabetes.out | true

divorce.out: divorce.csv
	learn-2sat -i divorce.csv -o divorce.out

graph24000.out: graph24000.csv
	learn-2sat -i graph24000.csv -o graph24000.out

monks-1.out: monks-1.csv
	learn-2sat -i monks-1.csv -o monks-1.out | true

monks-2.out: monks-2.csv
	learn-2sat -i monks-2.csv -o monks-2.out | true

monks-3.out: monks-3.csv
	learn-2sat -i monks-3.csv -o monks-3.out | true

prime.out: prime.csv
	learn-2sat -i prime.csv -o prime.out

shuttle.out: shuttle.csv
	learn-2sat -i shuttle.csv -o shuttle.out

sport.out: sport.csv
	learn-2sat -i sport.csv -o sport.out

test1a.out: test1a.csv
	learn-2sat -i test1a.csv -o test1a.out

test1.out: test1.csv
	learn-2sat -i test1.csv -o test1.out

test2.out: test2.csv
	learn-2sat -i test2.csv -o test2.out

test3.out: test3.csv
	learn-2sat -i test3.csv -o test3.out

test4.out: test4.csv
	learn-2sat -i test4.csv -o test4.out

test5.out: test5.csv
	learn-2sat -i test5.csv -o test5.out

three.out: three.csv
	learn-2sat -i three.csv -o three.out

two.out: two.csv
	learn-2sat -i two.csv -o two.out

scratch:
	rm -f *.out
