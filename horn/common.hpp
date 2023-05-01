#pragma once

#include "matrix+formula.hpp"

using namespace std;

//--------------------------------------------------------------------------------------------------

extern bool debug;

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

//--------------------------------------------------------------------------------------------------

void read_arg (int argc, char *argv[]);
void adjust_and_open (string &command);
void print_arg ();
Row min (const Row &a, const Row &b);
bool operator|= (const Row &t, const Clause &c);
bool operator|= (const Row &t, const Formula &f);
bool operator|= (const Matrix &T, const Clause &c);
bool in_horn_closure (const Row &f, const Matrix &T);

//==================================================================================================
