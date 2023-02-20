#include <iostream>
#include "matrix+formula.hpp"
#include "common.hpp"
#include "bucket.hpp"

using namespace std;

//------------------------------------------------------------------------------

void insert (const Clause &c, Bucket &B) {
  Pattern pattern{c[X].sign, c[X].coord,
		  c[Y].sign, c[Y].coord};
  Point point{c[X].val, c[Y].val};

  if (B.count(pattern) > 0) {
    auto it = B[pattern].begin();
    while (it != B[pattern].end()) {
      if (*it == point)
	return;							// already there
      if (c[X].sign == lpos && c[Y].sign == lpos) {		// SW
	if ((*it)[X] <= point[X] && (*it)[Y] <= point[Y]) {
	  it = B[pattern].erase(it);
	  continue;
	} else if (point[X] <= (*it)[X] && point[Y] <= (*it)[Y])
	  return;
      } else if (c[X].sign == lneg && c[Y].sign == lpos) {	// SE
	if ((*it)[X] >= point[X] && (*it)[Y] <= point[Y]) {
	  it = B[pattern].erase(it);
	  continue;
	} else if (point[X] >= (*it)[X] && point[Y] <= (*it)[Y])
	  return;
      } else if (c[X].sign == lpos && c[Y].sign == lneg) {	// NW
	if ((*it)[X] <= point[X] && (*it)[Y] >= point[Y]) {
	  it = B[pattern].erase(it);
	  continue;
	} else if (point[X] <= (*it)[X] && point[Y] >= (*it)[Y])
	  return;
      } else if (c[X].sign == lneg && c[Y].sign == lneg) {	// NE
	if ((*it)[X] >= point[X] && (*it)[Y] >= point[Y]) {
	  it = B[pattern].erase(it);
	  continue;
	} else if (point[X] >= (*it)[X] && point[Y] >= (*it)[Y])
	  return;
      } else {
	cerr << "+++ bucket insert: you should not be here" << endl;
	exit(1);
      }
      it++;
    }
    if (c[X].coord == c[Y].coord) {
      it = B[pattern].begin();
      while (it != B[pattern].end()) {
	if ((*it)[X] < point[X]
	    && point[X] < (*it)[Y]
	    && (*it)[Y] < point[Y])
	  point[X] = (*it)[X];		// stretch to left
	else if (point[X] < (*it)[X]
		 && (*it)[X] < point[Y]
		 && point[Y] < (*it)[Y])
	  point[Y] = (*it)[Y];		// stretch to right
	else {
	  it++;
	  continue;
	}
	it = B[pattern].erase(it);
      }
    }
  }
  B[pattern].insert(point);
}

Formula get_formula (const Bucket &B) {
  Formula f;
  for (const auto &b : B) {
    Pattern pattern = b.first;
    Clause c{Literal(pattern.lsign, pattern.lcoord, 0),
	     Literal(pattern.rsign, pattern.rcoord, 0)};
    for (const Point &point : b.second) {
      c[X].val = point[X];
      c[Y].val = point[Y];
      f.insert(c);
    }
  }
  return f;
}

bool operator|= (const Row &t, const Bucket &B) {
  // Bucket is a formula encoded differently.
  // Reconstruct each clause and test the satisfiability.

  for (const auto &b : B) {
    Pattern pattern = b.first;
    for (const Point &point : b.second) {
      Clause c{Literal(pattern.lsign, pattern.lcoord, point[X]),
	       Literal(pattern.rsign, pattern.rcoord, point[Y])};
      if (!(t |= c))
	return false;
    }
  }
  return true;
}

void print_bucket (const Bucket &bucket) {
  cout << "*** Buckets:" << endl;
  for (const auto &b : bucket) {
    Pattern pattern = b.first;
    cout << "... pattern " << to_string(pattern) << ":";
    for (const Point &point : b.second)
      cout << " " << point;
    cout << endl;
  }
  cout << endl;
}
