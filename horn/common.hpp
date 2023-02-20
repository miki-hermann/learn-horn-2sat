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

extern ifstream infile;
extern ofstream outfile;
extern ofstream latexfile;

//--------------------------------------------------------------------------------------------------

void read_arg (int argc, char *argv[]);
void adjust_and_open (string &command);
Row min (const Row &a, const Row &b);
bool operator|= (const Matrix &T, const Clause &c);
bool in_horn_closure (const Row &f, const Matrix &T);

//==================================================================================================
