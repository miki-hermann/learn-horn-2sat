#include <iostream>
#include <sstream>
#include <string>
#include <memory>
#include "matrix+formula.hpp"

using namespace std;

//------------------------------------------------------------------------------

bool debug = false;

const Clause empty_clause = {Literal(lnone, 0, 0), Literal(lnone, 0, 0)};
Matrix positiveT;		// positive examples
Matrix negativeF;		// negative examples

string varid   = "x";		// variable prefix
char delim     = ',';		// delimiter
unsigned DCARD = 2;		// domain cardinality
Print print    = pCLAUSE;	// formula print indicator
int arity      = 0;		// arity of rows

//------------------------------------------------------------------------------

deque<unsigned> split (const string &strg, char delimiter) {
  // splits a string into chunks separated by delimiter (split in perl)
  deque<unsigned> chunks;
  string token;
  istringstream iss(strg);

  if (! isprint(delimiter)) {
    cout << "*** delimiter is not printable" << endl;
    exit(2);
  }

  while (getline(iss, token, delimiter)) {
    // for (int i = 0; i < token.length(); ++i)
    //   if (! isprint(token[i])) {
    // 	cout << "*** token on input has a non-printable character" << endl;
    // 	exit(2);
    //   }
    chunks.push_back(stoi(token));
  }
  return chunks;
}

string literal2string (const Literal &lit, const Sign sign) {
  string output;
  output = varid + to_string(lit.coord);
  if (sign == lneg)
    output += "<=";
  else if (sign == lpos)
    output += ">=";
  else
    output += "??";
  output += to_string(lit.val);
  return output;
}

string rlcl2string (const Clause &clause) {
  string output;
  bool plus = false;
  for (int lit = 0; lit < clause.size(); ++lit) {
    output += plus ? " + " : "";
    plus = true;
    output += literal2string(clause[lit], clause[lit].sign);      
  }
  return output;
}

string impl2string (const Clause &clause) {
  string output;
  for (int lit = 0; lit < clause.size(); ++lit)
    if (clause[lit].sign == lneg)
      output += literal2string(clause[lit], lpos) + " ";
  output += "->";
  for (int lit = 0; lit < clause.size(); ++lit)
    if (clause[lit].sign == lpos)
      output += " " + literal2string(clause[lit], lpos);
  return output;
}

string to_string (const Clause &clause) {
  string output = "(";
  if (print == pCLAUSE)
    output += rlcl2string(clause);
  else if (print == pIMPL)
    output += impl2string(clause);
  output += ')';
  return output;
}

string to_string (const Formula &formula) {
  // transforms formula into readable clausal, implication form to print
  if (formula.empty())
    return " ";

  string output;
  bool times = false;
  for (Clause clause : formula) {
    output += times ? "\n\t* " : "\t  ";
    times = true;
    output += to_string(clause);
  }
  return output;
}

string to_string (const Row &r) {
  string rstrg = to_string(r[0]);
  for (int i = 1; i < r.size(); ++i)
    rstrg += " " + to_string(r[i]);
  return rstrg;
}

string to_string (const Point &p) {
  return "[" + to_string(p[0]) + "," + to_string(p[1]) + "]";
}

string to_string (const Pattern &p) {
  return "[x_" + to_string(p.lcoord)
    + sign_string.at(p.lsign) + " , "
    + "x_" + to_string(p.rcoord)
    + sign_string.at(p.rsign)
    + "]";
}

string literal2latex (const Literal &lit, const Sign sign) {
    string output;
  output = varid + "_" + to_string(lit.coord);
  if (sign == lneg)
    output += " \\leq ";
  else if (sign == lpos)
    output += " \\geq ";
  else
    output += " \\bullet ";
  output += to_string(lit.val);
  return output;
}

string rlcl2latex (const Clause &clause) {
  string output;
  bool lor = false;
  for (int lit = 0; lit < clause.size(); ++lit) {
    output += lor ? " \\lor " : "";
    lor = true;
    output += literal2latex(clause[lit], clause[lit].sign);      
  }
  return output;
}

string impl2latex (const Clause &clause) {
  string output;
  for (int lit = 0; lit < clause.size(); ++lit)
    if (clause[lit].sign == lneg)
      output += literal2latex(clause[lit], lpos) + " ";
  output += "->";
  for (int lit = 0; lit < clause.size(); ++lit)
    if (clause[lit].sign == lpos)
      output += " " + literal2latex(clause[lit], lpos);
  return output;
}

string to_latex (const Clause &clause) {
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
    output += to_latex(clause);
  }
  return output;
}

string to_machine (const Clause &clause) {
  // coordinate, sign, value, coordinate, sign, value
  string output;

  for (int i = X; i <= Y; ++i) {
    output += "x" + to_string(clause[i].coord);
    output += " " + sign_string.at(clause[i].sign) + " ";
    output += to_string(clause[i].val);
    if (i == X)
      output += " + ";
  }
  return output;
}

string to_machine (const Formula &formula) {
  if (formula.empty())
    return " ";

  string output;
  for (Clause clause : formula)
    output += to_machine(clause) + "\n";
  return output;
}

//------------------------------------------------------------------------------

ostream& operator<< (ostream &output, const Row &row) {
  // overloading ostream to print a row
  // transforms a tuple (row) to a printable form
  output << row[0];
  for (int i = 1; i < row.size(); ++i)
    output << " " << row[i];
  return output;
}

ostream& operator<< (ostream &output, const Matrix &M) {
  // overloading ostream to print a matrix
  // transforms a matrix to a printable form
  for (Row row : M)
    output << "\t" << row << "\n";
  return output;
}

ostream& operator<< (ostream &output, const Point &p) {
  // overloading ostream to print a point
  output << "[" << p[X] << "," << p[Y] << "]";
  return output;
}

ostream& operator<< (ostream &output, const ColNode &cn) {
  if (cn.south != nullptr)
    output << *(cn.south);
  output << " " << cn.value_j;
  if (cn.north != nullptr)
    output << *(cn.north);
  return output;
}

ostream& operator<< (ostream &output, const ColTree &ct) {
  if (! ct.empty())
    output << *(ct.tree());
  return output;
}

ostream& operator<< (ostream &output, const RowNode &rn) {
  if (rn.west != nullptr)
    output << *(rn.west);
  output << "... row " << rn.value_i << ": "
	 << *(rn.column) << "." << endl;
  if (rn.east != nullptr)
    output << *(rn.east);
  return output;
}

ostream& operator<< (ostream &output, const RowTree &rt) {
  if (! rt.empty())
    output << *(rt.tree());
  return output;
}

//------------------------------------------------------------------------------

bool operator>= (const Row &lhs, const Row &rhs) {
  for (int i = 0; i < lhs.size(); ++i)
    if (lhs[i] < rhs[i])
      return false;
  return true;
}

bool operator< (const Row &lhs, const Row &rhs) {
  for (int i = 0; i < lhs.size(); ++i)
    if (lhs[i] >= rhs[i])
      return false;
  return true;
}

//------------------------------------------------------------------------------

bool operator== (const Pattern &p1, const Pattern &p2) {
  return p1.lsign == p2.lsign && p1.lcoord == p2.lcoord
    && p1.rsign == p2.rsign && p1.rcoord == p2.rcoord;
}

//------------------------------------------------------------------------------

void ColTree::insert (const Point &point) {
  if (this->empty())
    this->root = make_shared<ColNode>(point);
  else {
    shared_ptr<ColNode> colnode = this->root;
    shared_ptr<ColNode> colparent;
    while (colnode != nullptr) {
      colparent = colnode;
      if (point[Y] == colnode->value_j) {
	// point is already in the tree
	return;
      } else
	colnode = point[Y] < colnode->value_j
	  ? colnode->south : colnode->north;
    }
    // y-axis does not exist and point[Y] != colparent->value_j
    if (point[Y] < colparent->value_j)
      colparent->south = make_shared<ColNode>(point);
    else
      colparent->north = make_shared<ColNode>(point);
  }
}

void RowTree::insert (const Point &point) {
  if (this->empty())
    this->root = make_shared<RowNode>(point);
  else {
    shared_ptr<RowNode> rownode = this->root;
    shared_ptr<RowNode> rowparent;
    while (rownode != nullptr) {
      rowparent = rownode;
      if (point[X] == rownode->value_i) {
	// x-axis exists, insert on it
	rownode->column->insert(point);
	return;
      } else
	rownode = point[X] < rownode->value_i
	  ? rownode->west : rownode->east;
    }
    // x-axis does not exist and point[X] != rowparent->value
    if (point[X] < rowparent->value_i)
      rowparent->west = make_shared<RowNode>(point);
    else
      rowparent->east = make_shared<RowNode>(point);
  }
}

void RowTree::build (const vector<Point> &pts,
		     const int low, const int high) {
  if (low <= high) {
    int med = (low + high) / 2;
    this->insert(pts[med]);
    this->build(pts, low, med-1);
    this->build(pts, med+1, high);
  }
}

void RowTree::insert (const vector<Point> &pts) {
  this->build(pts, 0, pts.size()-1);
}

//------------------------------------------------------------------------------

bool ColNode::isolated (const Point &point, const unsigned dir) const {
  // checks if the point is isolated in the quadrant dir
  // for all columns in the tree
  // isolated = no other points in the quadrant
  if ((dir == NE || dir == NW)		// looking NORTH = larger
      && this->value_j >= point[Y])
    return false;
  if ((dir == SE || dir == SW)		// looking SOUTH = smaller
      && this->value_j <= point[Y])
    return false;
  if (this->north != nullptr
      && ! this->north->isolated(point, dir))
    return false;
  if (this->south != nullptr
      && ! this->south->isolated(point, dir))
    return false;
  return true;
}

bool ColTree::isolated (const Point &point, const unsigned dir) const {
  // checks if the point is isolated in the quadrant dir
  // isolated = no other points in the quadrant

  shared_ptr<ColNode> colptr = this->root;
  if (dir == NE || dir == NW)		// looking NORTH = larger
    while (colptr != nullptr) {
      if (colptr->value_j >= point[Y])
	return colptr->isolated(point, dir);
      colptr = colptr->north;
    }
  else if (dir == SE || dir == SW)	// looking SOUTH = smaller
    while (colptr != nullptr) {
      if (colptr->value_j <= point[Y])
	return colptr->isolated(point, dir);
      colptr = colptr->south;
    }
  return true;
}

bool RowNode::isolated (const Point &point, const unsigned dir) const {
  // checks if the point is isolated in the quadrant dir
  // for all rows in the tree
  // isolated = no other points in the quadrant
  if ((dir == NE || dir == SE)		// looking EAST = larger
      && this->value_i >= point[X]
      && ! this->column->isolated(point, dir))
    return false;
  if ((dir == NW || dir == SW)		// looking WEST = smaller
      && this->value_i <= point[X]
      && ! this->column->isolated(point, dir))
    return false;
  if (this->west != nullptr
      && ! this->west->isolated(point, dir))
    return false;
  if (this->east != nullptr
      && ! this->east->isolated(point, dir))
    return false;
  return true;
}

bool RowTree::isolated (const Point &point, const unsigned dir) const {
  // checks if the point is isolated in the quadrant dir
  // isolated = no other points in the quadrant

  shared_ptr<RowNode> rowptr = this->root;
  if (dir == NE || dir == SE)		// looking EAST = larger
    while (rowptr != nullptr) {
      if (rowptr->value_i >= point[X])
	return rowptr->isolated(point, dir);
      rowptr = rowptr->east;
    }
  else if (dir == NW || dir == SW)	// looking WEST = smaller
    while (rowptr != nullptr) {
      if (rowptr->value_i <= point[X])
	return rowptr->isolated(point, dir);
      rowptr = rowptr->west;
    }
  else
    throw runtime_error(to_string(dir) + " is not an ordinal direction");
  return true;
}

Clause isolation (const Point &p, const Point &xy, const unsigned quadrant) {
  Clause c = empty_clause;
  switch (quadrant) {
  case NW:
    c[X] = Literal(lpos, xy[X], p[X]+1);
    if (p[Y] > 0)
      c[Y] = Literal(lneg, xy[Y], p[Y]-1);
    break;
  case NE:
    if (p[X] > 0)
      c[X] = Literal(lneg, xy[X], p[X]-1);
    if (p[Y] > 0)
      c[Y] = Literal(lneg, xy[Y], p[Y]-1);
    break;
  case SE:
    if (p[X] > 0)
      c[X] = Literal(lneg, xy[X], p[X]-1);
    c[Y] = Literal(lpos, xy[Y], p[Y]+1);
    break;
  case SW:
    c[X] = Literal(lpos, xy[X], p[X]+1);
    c[Y] = Literal(lpos, xy[Y], p[Y]+1);
    break;
  default:
    throw runtime_error("+++ isolation: quadrant " + to_string(quadrant)
			+ " is not an ordinal direction");
  }
  return c;
}

//------------------------------------------------------------------------------

void init (Mesh &mesh) {
  vector<RowTree> tmp4(arity, RowTree());
  for (unsigned j = 0 ; j < arity; ++j)
    mesh.push_back(tmp4);
}

void init (Strip &strip) {
  vector<unordered_set<unsigned>> tmp4(arity, unordered_set<unsigned>());
  strip = tmp4;
}

//------------------------------------------------------------------------------

bool valid (const Clause &c) {
  return c[X].sign != lnone && c[Y].sign != lnone;
}

//==============================================================================
