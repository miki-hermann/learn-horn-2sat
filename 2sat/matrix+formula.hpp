#pragma once

#include <vector>
#include <deque>
#include <string>
#include <unordered_map>
#include <set>
#include <unordered_set>
#include <array>
#include <memory>

using namespace std;

//------------------------------------------------------------------------------

extern bool debug;

typedef deque<unsigned> Row;
typedef set<Row> Matrix;
#define CARDlimit 50

enum Print {pVOID = 0, pCLAUSE = 1, pIMPL = 2};
const string print_string[] = {"void", "clause", "implication"};

enum Sign : unsigned char {lnone = 0, lneg = 1, lpos = 2};
const unordered_map<Sign, string> sign_string = {
  {lnone, "?"},
  {lneg, "<="},
  {lpos, ">="}
};
class Literal {	// variable <= val or variable >= val
public:
  Sign sign;
  unsigned coord;
  unsigned val;

  Literal  () = default;
  ~Literal () = default;
  Literal(Sign s, unsigned c, unsigned v) {
    sign = s;
    coord = c;
    val = v;
  }

  bool operator== (const Literal &other) const {
    return this->sign == other.sign
      && this->coord == other.coord
      && this->val == other.val;
  }

  bool operator!= (const Literal &other) const {
    return this->sign != other.sign
      || this->coord != other.coord
      || this->val != other.val;
  }

  bool operator< (const Literal &other) const {
    return this->coord < other.coord
      || this->coord == other.coord && this->val < other.val
      || this->coord == other.coord &&
      this->val == other.val && this->sign < other.sign
      || false;
    // return this->coord < other.coord
    //   || this->coord == other.coord && this->sign < other.sign
    //   || this->coord == other.coord &&
    //   this->sign == other.sign && this->val < other.val
    //   || false;
    // return this->sign < other.sign
    //   || this->sign == other.sign && this->coord < other.coord
    //   || this->sign == other.sign && this->val < other.val
    //   || false;
  }
};

typedef array<Literal, 2> Clause;	// will have only 2 elements
#define X 0
#define Y 1

typedef set<Clause> Formula;
extern const Clause empty_clause;

extern Matrix positiveT;	// positive examples
extern Matrix negativeF;	// negative examples

typedef array<unsigned, 2> Point;

// ordinal directions
#define NW    1
#define NE    2
#define SE    3
#define SW    4
const unordered_map<unsigned, string> dir_string = {
  {NW,		"NW"},
  {NE,		"NE"},
  {SE,		"SE"},
  {SW,		"SW"}
};

class ColNode {
public:
  unsigned value_j;
  shared_ptr<ColNode> north;
  shared_ptr<ColNode> south;

  ColNode () = default;
  ~ColNode () = default;
  ColNode (const Point &p) {
    value_j = p[Y];
    north = nullptr;
    south = nullptr;
  }

  bool isolated (const Point &, const unsigned) const ;
};

class ColTree {
public:
  ColTree() { root = nullptr; }
  ~ColTree() = default;

  bool empty () const { return root == nullptr; }
  shared_ptr<ColNode> tree() const { return root; }
  void insert (const Point &);
  bool isolated (const Point &, const unsigned) const ;
private:
  shared_ptr<ColNode> root;
};

class RowNode {
public:
  unsigned value_i;
  shared_ptr<RowNode> west;
  shared_ptr<RowNode> east;
  shared_ptr<ColTree> column;

  RowNode () = default;
  ~RowNode () = default;
  RowNode (const Point &point) {
    value_i = point[X];
    west = nullptr;
    east = nullptr;
    column = make_shared<ColTree>();
    column->insert(point);
  }

  bool isolated (const Point &, const unsigned) const ;
};

class RowTree {
public:
  RowTree() { root = nullptr; }
  ~RowTree() = default;

  bool empty() const { return root == nullptr; }
  shared_ptr<RowNode> tree() const { return root; }
  void insert (const Point &);
  void insert (const vector<Point> &);
  bool isolated (const Point &, const unsigned) const ;
private:
  shared_ptr<RowNode> root;
  void build (const vector<Point> &, const int, const int);
};
typedef vector<vector<RowTree>> Mesh;
typedef vector<unordered_set<unsigned>> Strip;

// Meaning of Pattern:
//   Sign,		// left sign
//   unsigned,		// left coordinate
//   Sign,		// right sign
//   unsigned,		// right coordinate
// typedef array<unsigned, 4> Pattern;
class Pattern {
public:
  Sign lsign;		// left sign
  unsigned lcoord;	// left coordinate
  Sign rsign;		// right sign
  unsigned rcoord;	// right coordinate

  Pattern () = default;
  ~Pattern () = default;
  Pattern (Sign ls, unsigned lc, Sign rs, unsigned rc) {
    lsign = ls;
    lcoord = lc;
    rsign = rs;
    rcoord = rc;
  }
};

namespace std {
  template<> struct hash<Pattern> {
    using argument_type = Pattern;
    using result_type = size_t;

    result_type operator() (const argument_type &pattern) const {
      string strg = to_string(pattern.lsign) + ":"
	+ to_string(pattern.lcoord) + ":"
	+ to_string(pattern.rsign) + ":"
	+ to_string(pattern.rcoord);
      return hash<string>()(strg);
    }
  };
}

//------------------------------------------------------------------------------

extern string varid;		// variables prefix
extern char delim;		// delimiter
extern unsigned DCARD;		// domain cardinality
extern Print print;		// formula print indicator
extern int arity;		// arity of rows

//------------------------------------------------------------------------------

deque<unsigned> split (const string &, char);
string to_string (const Clause &);
string to_string (const Formula &);
string to_latex (const Formula &);
string to_machine (const Formula &);
ostream& operator<< (ostream &, const Row &);
ostream& operator<< (ostream &, const Matrix &);
ostream& operator<< (ostream &, const Point &);
ostream& operator<< (ostream &, const RowTree &);
bool operator>= (const Row &, const Row &);
bool operator< (const Row &, const Row &);
string to_string (const Row &);
string to_string (const Point &);
string to_string (const Pattern &);
Clause isolation (const Point &, const Point &, const unsigned);
void init (Mesh &);
void init (Strip &);
bool valid (const Clause &);
bool operator== (const Pattern &, const Pattern &);
