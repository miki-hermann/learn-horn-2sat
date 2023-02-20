#include <iostream>
#include <sstream>
#include <string>
#include "matrix+formula.hpp"

using namespace std;

//------------------------------------------------------------------------------

Matrix positiveT;	// positive examples
Matrix negativeF;	// negative examples

string varid = "x";	// variable prefix
char delim   = ',';	// delimiter
int DCARD    = 0;	// domain cardinality
Print print  = pCLAUSE;	// formula print indicator
int arity    = 0;	// arity of rows

//------------------------------------------------------------------------------

deque<unsigned> split (const string &strg, char delimiter) {
  // splits a string into chunks separated by delimiter (split in perl)
  deque<unsigned> chunks;
  string token;
  istringstream iss(strg);

  if (! isprint(delimiter)) {
    cerr << "*** delimiter is not printable" << endl;
    exit(2);
  }

  while (getline(iss, token, delimiter)) {
    // for (int i = 0; i < token.length(); ++i)
    //   if (! isprint(token[i])) {
    // 	cerr << "*** token on input has a non-printable character" << endl;
    // 	exit(2);
    //   }
    chunks.push_back(stoi(token));
  }
  return chunks;
}

string literal2string (const int &litname, const Sign sign, const int val) {
  string output;
  output = varid + to_string(litname);
  if (sign == lneg)
    output += "<=";
  else if (sign == lpos)
    output += ">=";
  else
    output += "??";
  output += to_string(val);
  return output;
}

string rlcl2string (const Clause &clause) {
  string output;
  bool plus = false;
  for (int lit = 0; lit < clause.size(); ++lit) {
    if (clause[lit].sign & lneg) {
      if (plus)
	output += " + ";
      else
	plus = true;
      output += literal2string(lit, lneg, clause[lit].nval);      
    }
    if (clause[lit].sign & lpos) {
      if (plus)
	output += " + ";
      else
	plus = true;
      output += literal2string(lit, lpos, clause[lit].pval);      
    }
  }
  return output;
}

string impl2string (const Clause &clause) {
  string output;
  for (int lit = 0; lit < clause.size(); ++lit)
    if (clause[lit].sign & lneg)
      output += literal2string(lit, lpos, clause[lit].nval+1) + " ";
  output += "->";
  for (int lit = 0; lit < clause.size(); ++lit)
    if (clause[lit].sign & lpos)
      output += " " + literal2string(lit, lpos, clause[lit].pval);
  return output;
}

string clause2string (const Clause &clause) {
  string output = "(";
  if (print == pCLAUSE)
    output += rlcl2string(clause);
  else if (print == pIMPL)
    output += impl2string(clause);
  output += ')';
  return output;
}

string to_string (const Formula &formula) {
  // transforms formula into readable clausal, implication or mixed form to print
  if (formula.empty())
    return " ";

  string output;
  bool times = false;
  for (Clause clause : formula) {
    output += (times) ? "\n\t* " : "\t  ";
    times = true;
    output += clause2string(clause);
  }
  return output;
}

string literal2latex (const int &litname, const Sign sign, const int val) {
    string output;
  output = varid + "_" + to_string(litname);
  if (sign == lneg)
    output += " \\leq ";
  else if (sign == lpos)
    output += " \\geq ";
  else
    output += " \\bullet ";
  output += to_string(val);
  return output;
}

string rlcl2latex (const Clause &clause) {
  string output;
  bool lor = false;
  for (int lit = 0; lit < clause.size(); ++lit) {
    if (clause[lit].sign & lneg) {
      if (lor)
	output += " \\lor ";
      else
	lor = true;
      output += literal2latex(lit, lneg, clause[lit].nval);      
    }
    if (clause[lit].sign & lpos) {
      if (lor)
	output += " \\lor ";
      else
	lor = true;
      output += literal2latex(lit, lpos, clause[lit].pval);      
    }
  }
  return output;
}

string impl2latex (const Clause &clause) {
  string output;
  for (int lit = 0; lit < clause.size(); ++lit)
    if (clause[lit].sign & lneg)
      output += literal2latex(lit, lpos, clause[lit].nval+1) + " ";
  output += "->";
  for (int lit = 0; lit < clause.size(); ++lit)
    if (clause[lit].sign & lpos)
      output += " " + literal2latex(lit, lpos, clause[lit].pval);
  return output;
}

string clause2latex (const Clause &clause) {
  string output = "(";
  if (print == pCLAUSE)
    output += rlcl2latex(clause);
  else if (print == pIMPL)
    output += impl2latex(clause);
  output += ')';
  return output;
}

string to_latex (const Formula &formula) {
  // transforms formula into readable clausal form in LaTeX format to print
  if (formula.empty())
    return " ";

  string output;
  bool land = false;
  for (Clause clause : formula) {
    output += land ? "\n\t\\land " : "\t  ";
    land = true;
    output += clause2latex(clause);
  }
  return output;
}

ostream& operator<< (ostream &output, const Row &row) {
  // overloading ostream to print a row
  // transforms a tuple (row) to a printable form
  // for (bool bit : row)
  for (int bit : row)
    output << bit << " ";
  return output;
}

ostream& operator<< (ostream &output, const Matrix &M) {
  // overloading ostream to print a matrix
  // transforms a matrix to a printable form
  for (Row row : M)
    output << "\t" << row << "\n";
  return output;
}

bool operator>= (const Row &lhs, const Row &rhs) {
  for (int i = 0; i < lhs.size(); ++i)
    if (lhs[i] < rhs[i])
      return false;
  return true;
}

string to_string (const Row &r) {
  string rstrg = to_string(r[0]);
  for (int i = 1; i < r.size(); ++i)
    rstrg += "+" + to_string(r[i]);
  return rstrg;
}

string to_string (const Literal &l) {
  return to_string(l.sign) + ":" + to_string(l.pval) + ":" + to_string(l.nval);
}

string to_string (const Clause &c) {
  string cstrg = to_string(c[0]);
  for (int i = 1; i < c.size(); ++i)
    cstrg += "*" + to_string(c[i]);
  return cstrg;
}
