# Learning Horn and 2SAT formulas
				   
## Table of contents

* [Brief Description](#brief-description)
* [Horn part](#horn-part)
* [2SAT part](#2sat-part)
* [Compilation and Installation](#compilation-and-installation)
* [Examples](#examples)


## Brief Description

These two C++ software packages contains the learning algorithms for
Horn and 2SAT formulas, as well as the algorithm for constructing the
Horn closure of a set of vectors.

Both learning algorithms produce the learned Horn or 2SAT formulas,
respectively, from positive and negative samples. Each sample is a
vectors of positive integer values separated by commas, preceded by an
indicator (first value on the line). The leading indicator is 0 for a
negative sample and 1 for a positive sample.

Detailed description of the algorithms together with the syntax of
produced formulas is in the file `paper.pdf`.

## Horn part

### Invocation

Both Horn formula learning, and Horn closure algorithms are invoked by
the following commands:

	learn-horn      [-i <input file>] [-o <output file>] [-l <latex-output-file>] [-p <print-indicators>]
	horn-closure-v1 [-i <input file>] [-o <output file>]
	horn-closure-v2 [-i <input file>] [-o <output file>]

The `learn-horn` algorithm also accepts the input from from STDIN and
prints the output on STDOUT by default, if the options `-i` or `-o`
are not specified. If the user requires output in LaTeX form, it can
be done using the `-l` option followed by the target filename.  The
`-p` option allows to specify the print indicator: either `clause` or
`impl`, depending if you want to see the clauses written as clauses or
implications, respectively. The default is `clause`.

There exist two versions of the Horn closure algorithm:
`horn-closure-v1` and `horn-closure-v2`. Two different strategies are
used for producing the Horn closure, but they produce the same result
on the same input. Be aware that these algorithms are exponential in
the worst case.

## 2SAT part

### Invocation

The 2SAT learning algorithm is invoked by the command:

	learn-2sat [-i <input file>] [-o <output file>] [-l <latex-output-file>] [-p <print-indicators>]

The `learn-2sat` algorithm also accepts the input from from STDIN and
prints the output on STDOUT by default, if the options `-i` or `-o`
are not specified. If the user requires output in LaTeX form, it can
be done using the `-l` option followed by the target filename. The
`-p` option allows to specify the print indicator: either `clause` or
`impl`, depending if you want to see the clauses written as clauses or
implications, respectively. The default is `clause`.

## Compilation and Installation
* [Compilation](#compilation)
* [Execution](#execution)

### Compilation

A C++ compiler satisfying at least the C++11 revision is necessary to
successfully compile the algorithms. Only the standard library is
used, therefore there is no need to install any additional C++
libraries.  The g++ GNU Project compiler is used in the `Makefile`. If
you have a different compiler, please modify the `Makefile` according
to your installation.

### Execution

To compile the algorithms, both for Horn and 2SAT learning, write the command
```Makefile
   make
```
in the root directory. You can also compile only one part of the
algorithms, either Horn or 2SAT, by first changing the directory using
the command `cd horn` or `cd 2sat`, respectively, followed by the
invocation of `make`. There exist an additional command  `make
horn-closure` in the Horn part to compile only the Horn closure
algorithms.

To install the algorithms, write the command
```Makefile
   make install
```
This command installs the executables in the directory
`/usr/local/bin`. You need to have superuser priviledges to execute
this command.

## Examples

The distribution also contains CSV files with examples in both
parts. You can find them in the directory `examples` The test files
`test*.csv` contain small examples designated to test if the
algorithms run correctly and also serve as examples how to write the
input.

The file `graph24000.csv` contains 24,000 lines indicating the
presence (by a leading 1) or absence (by a leading 0) of an arc in an
oriented graph, where each node is labeled by a number. The task is to
construct a formula recognizing the graph. Surprisingly, a 2SAT
formula with 65 clauses is sufficient.

In the Horn part, there is another, much more sophisticated example
from `kaggle` present in the Horn part, but it needs some
preprocessing. The file `digit.csv.xz` contains compressed data for
recognition of digits.

Compile the digit treatment by `make example` in the Horn part, then
run the command `digit`. This command will ask you to choose the digit
`d` which you want to recognize. The software produces a CSV file
`digitd.csv` for the chosen digit `d`. Use it as input for the
algorithms.


EOF
