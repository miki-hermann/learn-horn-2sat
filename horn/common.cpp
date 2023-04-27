#include <iostream>
#include <fstream>
#include <set>
#include <map>
#include <algorithm>
#include <string>
#include "matrix+formula.hpp"
#include "common.hpp"

using namespace std;

bool debug = false;

const string STDIN  = "STDIN";
const string STDOUT = "STDOUT";

string input        = STDIN;
string output       = STDOUT;
string latex        = "";		// file to store latex output

ifstream infile;
ofstream outfile;
ofstream latexfile;

//------------------------------------------------------------------------------

void read_arg (int argc, char *argv[]) {	// reads the input parameters
  int argument = 1;
  while (argument < argc) {
    string arg = argv[argument];
    if (arg == "--input"
	|| arg == "-i")
      input = argv[++argument];
    else if (arg == "--output"
	     || arg == "-o")
      output = argv[++argument];
    else if (arg == "--latex"
	     || arg == "-l")
      latex = argv[++argument];
    else if (arg == "--card"
	     || arg == "-c")
      DCARD = stoi(argv[++argument]);
    else if (arg == "--help"
	     || arg == "-h") {
      cerr << "Usage: " << argv[0]
	   << " [-i <input file>]"
	   << " [-o <output file>]"
	   << " [-l <latex-output-file>]"
	   << " [-m <CSV-output-file>]"
	   << " [-c <domain cardinality>]"
	   << " [-p <print indicator>]"
	   << endl;
      exit(0);
    } else if (arg == "--print"
	     || arg == "-p") {
      string prt = argv[++argument];
      if (prt == "clause"
	  || prt == "c")
	print = pCLAUSE;
      else if (prt == "implication"
	       || prt == "impl"
	       || prt == "i")
	print = pIMPL;
      else
	cerr << "+++ unknown print option " << prt << endl;
    } else if (arg == "--debug")
      debug = true;
    else
      cerr <<  "+++ unknown option " << arg << endl;
    ++argument;
  }
}

void adjust_and_open (string &command) {	// adjust input parameters
  if (input != STDIN) {
    infile.open(input);
    if (infile.is_open())
      cin.rdbuf(infile.rdbuf());
    else {
      cerr << "+++ Cannot open input file " << input << endl;
      exit(2);
    }
  }
  
  if (output != STDOUT) {
    outfile.open(output);
    if (outfile.is_open())
      cout.rdbuf(outfile.rdbuf());
    else {
      cerr << "+++ Cannot open output file " << output << endl;
      exit(2);
    }
  }

  if (latex.length() > 0) {
    if (latex.find(".") == string::npos
	|| latex.length() < 4
	|| latex.substr(latex.length()-4) != ".tex")
      latex += ".tex";
    latexfile.open(latex);
    if (!latexfile.is_open()){
      cerr << "+++ Cannot open latex file " << latex << endl;
      exit(2);
    }
  }

  if (command.substr(0,6) == "online" && DCARD < 2) {
    cerr << "+++ domain cardinality cannot be unspecified or smaller than 2" << endl;
    exit(1);
  }
}

void print_arg () {
  cout << "@@@ input              = " << input  << endl;
  cout << "@@@ output             = " << output << endl;
  cout << "@@@ domain cardinality = " << DCARD  << endl;
  cout << "@@@ formula output     = " << print_string[print] << endl;
  if (latex.length() > 0)
    cout << "@@@ LaTeX output       = " << latex << endl;
  cout << endl;
}

Row min (const Row &a, const Row &b) {
  // computes the minimum of two tuples by coordinates
  Row c;
  for (int i = 0; i < a.size(); ++i)
    c.push_back(min(a[i], b[i]));
  return  c;
}

//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

bool operator|= (const Matrix &T, const Clause &c) {	// does T |= c ?
  for (Row t : T) {
    bool satisfied = false;
    for (int i = 0; i < c.size(); ++i)
      if (c[i].sign & lpos && t[i] >= c[i].pval
	  ||
	  c[i].sign & lneg && t[i] <= c[i].nval) {
	satisfied = true;
	break;
      }
    if (!satisfied)
      return false;
  }
  return true;
}

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

bool in_horn_closure (const Row &f, const Matrix &T) {
  // is the tuple f in the Horn closure of matrix T?
  Row m_geq_f(f.size(), DCARD-1);
  for (Row t : T)
    if (t >= f)
      m_geq_f = min(t, m_geq_f);
  return f == m_geq_f;
}

//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
