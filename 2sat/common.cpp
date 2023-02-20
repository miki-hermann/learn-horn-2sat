#include <iostream>
#include <fstream>
#include <set>
#include <map>
#include <algorithm>
#include <string>
#include "matrix+formula.hpp"
#include "common.hpp"

using namespace std;

const string STDIN  = "STDIN";
const string STDOUT = "STDOUT";

string input        = STDIN;
string output       = STDOUT;
string latex        = "";		// file to store latex output
string machine      = "";		// machine readable CSV file

ifstream infile;
ofstream outfile;
ofstream latexfile;
ofstream machinefile;

bool TEST = false;

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
    else if (arg == "--machine"
	     || arg == "-m")
      machine = argv[++argument];
    else if (arg == "--test"
	     || arg == "-t")
      TEST = true;
    else if (arg == "--help"
	     || arg == "-h") {
      cerr << "Usage: " << argv[0]
	   << " [-i <input file>]"
	   << " [-o <output file>]"
	   << " [-l <latex-output-file>]"
	   << " [-m <CSV-output-file>]"
	   << " [-t]"
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

  if (machine.length() > 0) {
    if (machine.find(".") == string::npos
	|| machine.length() < 4
	|| machine.substr(machine.length()-4) != ".mvc")
      machine += ".mvc";
    machinefile.open(machine);
    if (!machinefile.is_open()){
      cerr << "+++ Cannot open machine-readble CSV file " << machine << endl;
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
  if (machine.length() > 0)
    cout << "@@@ CSV output         = " << machine << endl;
  cout << endl;
}

//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

bool operator|= (const Row &t, const Clause &c) {	// does t |= c ?
  bool satisfied = false;
  for (int i = X; i <= Y; ++i)
    if (c[i].sign == lpos && t[c[i].coord] >= c[i].val
	||
	c[i].sign == lneg && t[c[i].coord] <= c[i].val) {
      satisfied = true;
      break;
    }
  return satisfied;
}

bool operator|= (const Row &t, const Formula &f) {	// does t |= f
  for (Clause c : f)
    if (!(t |= c))
      return false;
  return true;
}

void print_mesh (const Mesh &mesh) {
  cout << "++++++++++++++++++++++++++++++++++++++++++++++++++++++" << endl;
  cout << "+++ mesh" << endl;
  for (int i = 0; i < arity-2; ++i)
    for (int j = i+1; j < arity-1; ++j)
      cout <<  "*** i = " << i << ", j = " << j << endl
	   << mesh[i][j] << endl;
  cout << "++++++++++++++++++++++++++++++++++++++++++++++++++++++"
       << endl << endl;
}

void testing (const Formula &varphi) {
  cout << "+++ testing formula" << endl;
  for (Row r : negativeF)
    if (r |= varphi) {
      cout << "+++ WARNING: NEGATIVE example '"
	   << r << "' SATISFIES formula"
	   << endl;
      break;
    }
  for (Row r : positiveT)
    if (!(r |= varphi)) {
      cout << "*** WARNING: POSITIVE example '"
	   << r << "' FALSIFIES formula"
	   << endl;
      for (Clause c : varphi)
	if (!(r |= c)) {
	  cout << "... falsified clause is " << to_string(c) << endl;
	  break;
	}
      break;
    }
}

//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
