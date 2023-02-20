#pragma once

#include "matrix+formula.hpp"

using namespace std;

//--------------------------------------------------------------------------------------------------

extern const string STDIN;
extern const string STDOUT;

extern string input;
extern string output;
extern string latex;		// file to store latex output
extern string machine;		// machine readable CSV output

extern ifstream infile;
extern ofstream outfile;
extern ofstream latexfile;
extern ofstream machinefile;

extern bool TEST;

//--------------------------------------------------------------------------------------------------

void read_arg (int argc, char *argv[]);
void adjust_and_open (string &);
void print_arg ();
bool operator|= (const Row &, const Clause &);
bool operator|= (const Row &, const Formula &);
void print_mesh (const Mesh &);
void testing (const Formula &);

//==================================================================================================
