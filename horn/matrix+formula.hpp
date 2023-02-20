#pragma once

#include <vector>
#include <deque>
#include <set>

using namespace std;

//------------------------------------------------------------------------------

typedef deque<unsigned> Row;
typedef set<Row> Matrix;

enum Print {pVOID = 0, pCLAUSE = 1, pIMPL = 2};

enum Sign {lnone = 0, lneg = 1, lpos = 2, lboth = 3};
class Literal {	// variable <= nval or variable >= pval
public:
  Sign sign;
  unsigned pval, nval;

  Literal() = default;
  ~Literal () = default;
  Literal(Sign s, unsigned pv, unsigned nv) {
    sign = s;
    pval = pv;
    nval = nv;
  }

  bool operator== (const Literal &other) const {
    return this->sign == other.sign
      && this->pval == other.pval
      && this->nval == other.nval;
  }

  bool operator!= (const Literal &other) const {
    return this->sign != other.sign
      || this->pval != other.pval
      || this->nval != other.nval;
  }

  bool operator< (const Literal &other) const {
    return this->sign < other.sign
      || this->sign == other.sign && this->nval < other.nval
      || this->sign == other.sign && this->pval < other.pval
      || false;
  }
};
typedef vector<Literal> Clause;
typedef set<Clause> Formula;

extern Matrix positiveT;	// positive examples
extern Matrix negativeF;	// negative examples

extern string varid;		// variables prefix
extern char delim;		// delimiter
extern int DCARD;		// domain cardinality
extern Print print;		// formula print indicator
extern int arity;		// arity of rows

//------------------------------------------------------------------------------

deque<unsigned> split (const string &strg, char delimiter);
string to_string (const Formula &formula);
string to_latex (const Formula &formula);
ostream& operator<< (ostream &output, const Row &row);
ostream& operator<< (ostream &output, const Matrix &M);
bool operator>= (const Row &lhs, const Row &rhs);
string to_string (const Row &r);
string to_string (const Literal &l);
string to_string (const Clause &c);
