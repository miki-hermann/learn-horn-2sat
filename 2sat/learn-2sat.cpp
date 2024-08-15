#include <iostream>
#include <fstream>
#include <string>
#include <set>
#include <algorithm>
#include <chrono>
#include "matrix+formula.hpp"
#include "common.hpp"
#include "bucket.hpp"

using namespace std;

Mesh mesh;
Strip strip;
Bucket bucket;

void read_input () {
  unsigned oldDCARD = DCARD;
  unsigned newDCARD = 0;
  string line;
  while (getline(cin, line)) {
    if (line.empty())
      continue;
    Row m = split(line, delim);
    unsigned example = m.front();
    m.pop_front();
    
    for (unsigned d : m)
      newDCARD = max(d, newDCARD);
    if (arity == 0) {
      arity = m.size();
      init(mesh);
      init(strip);
    }

    if (example > 1) {
      cerr << "+++ '" << line << "'" << endl;
      cerr << "+++ leading indicator not 0 or 1; example dropped" << endl;
    } else if (arity != m.size()) {
      cerr << "+++ '" << line << "'" << endl;
      cerr << "+++ arity discrepancy; example dropped" << endl;
    } else if (example == 0)
      negativeF.insert(m);
    else if (example == 1) {
      positiveT.insert(m);		// this is not necessary;
      for (int i = 0; i < arity; ++i) {
	strip[i].insert(m[i]);
	for (int j = i+1; j < arity; ++j)
	    mesh[i][j].insert({m[i], m[j]});
      }
    }
  }
  // if (positiveT.empty() && negativeF.empty())
  //   throw runtime_error("no input");
  newDCARD++;
  if (newDCARD != oldDCARD) {
    cout << "@@@ domain cardinality reset to " << newDCARD << endl << endl;
    DCARD = newDCARD;
  }
}

Formula learn_2sat_formula () {
  for (Row f : negativeF) {
    if (!(f |= bucket))
      continue;
    Clause c;
    bool eliminated = false;
    unsigned i = 0;
    while (!eliminated && i < arity) {
      c = isolation({f[i],f[i]}, {i,i}, SE);
      // Test if f[i] is in strip[i] and valid
      if (strip[i].count(f[i]) == 0 && valid(c))
	eliminated = true;

      unsigned j = i+1;
      while (!eliminated && j < arity) {
	const Point f2 = {f[i], f[j]};
	const Point ij = {i, j};
	// Test if f2 can be isolated
	unsigned q = NW;
	while (!eliminated && q <= SW) {
	  if (mesh[i][j].isolated(f2, q)) {
	    c = isolation(f2, ij, q);
	    eliminated = valid(c) ? true : false;
	  }
	  q++;
	}
	j++;
      }
      
      if (eliminated)
	insert(c, bucket);
      i++;
    }
    if(!eliminated) {
      cerr << "+++ 2SAT formula not possible:    No isolated point" << endl;
      exit(2);
    }
  }
  return get_formula(bucket);
}

int main (int argc, char **argv) {
  read_arg(argc, argv);
  string command = argv[0];
  adjust_and_open(command);
  print_arg();
  read_input();
  cout << "+++ " << positiveT.size() << " positive examples" << endl;
  cout << "+++ " << negativeF.size() << " negative examples" << endl;
  cout << "+++ arity = " << arity << endl << endl;
  if (positiveT.size() <= CARDlimit) {
    cout << "+++ positive examples T:" << endl;
    cout << positiveT << endl;
  }
  if (negativeF.size() <= CARDlimit) {
    cout << "+++ negative examples F:" << endl;
    cout << negativeF << endl;
  }

  if (TEST)
    print_mesh(mesh);

  // start clock
  auto clock_start = chrono::high_resolution_clock::now();

  Formula varphi = learn_2sat_formula();
  
  // stop the clock
  auto clock_stop = chrono::high_resolution_clock::now();
  auto duration = chrono::duration_cast<chrono::milliseconds>(clock_stop - clock_start);

  cout << "+++ produced 2SAT formula:" << endl;
  cout << to_string(varphi) << endl;
  unsigned ncl = varphi.size();
  cout << endl
       << "+++ 2SAT formula has " << ncl
       << (ncl == 1 ? " clause" : " clauses") << endl;
  if (!latex.empty())
    latexfile << to_latex(varphi) << endl;
  if (!machine.empty())
    machinefile << to_machine(varphi);

  if (TEST)
    testing(varphi);

  cout << endl << "+++ Elapsed time (seconds) = " << 0.001 * duration.count() << endl;
  cout << "+++ end of run" << endl;
}

////////////////////////////////////////////////////////////////////////////////
