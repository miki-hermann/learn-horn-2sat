#include <iostream>
#include <fstream>
#include <string>
#include <set>
#include <chrono>
#include "matrix+formula.hpp"
#include "common.hpp"

using namespace std;

void read_input () {
  string line;
  while (getline(cin, line)) {
    Row m = split(line, delim);
    unsigned example = m.front();
    m.pop_front();
    for (int i = 0; i < m.size(); ++i)
      DCARD = max(DCARD, m[i]+1);
    if (arity == 0)
      arity = m.size();
    if (example > 1) {
      cerr << "+++ '" << line << "'" << endl;
      cerr << "+++ leading indicator not 0 or 1; example dropped" << endl;
    } else if (arity != m.size()) {
      cerr << "+++ '" << line << "'" << endl;
      cerr << "+++ arity discrepancy; example dropped" << endl;
    } else if (example == 0)
      negativeF.insert(m);
    else if (example == 1)
      positiveT.insert(m);
  }
}

Formula learn_horn_formula () {
  Formula varphi;
  for (Row f : negativeF) {
    if (!(f |= varphi))
      continue;
    else if (in_horn_closure(f, positiveT)) {
      cerr << "+++ negative example present in Horn closure of T" << endl;
      cerr << "+++ the negative culprit is '" << f << "'" << endl;
      exit(2);
    }
    Clause c;
    for (int i = 0; i < arity; ++i) {
      Sign sign = f[i] > 0 ? lneg : lnone;
      unsigned val = f[i] > 0 ? f[i]-1 : 0;
      Literal lit(sign, 0, val);
      c.push_back(lit);
    }
    if (positiveT |= c)
      // varphi.push_back(c);
      varphi.insert(c);
    else {
      bool eliminated = false;
      int i = 0;
      while (!eliminated) {
	if (f[i] < DCARD-1) {
	  Clause newc = c;
	  newc[i].sign = (Sign) (c[i].sign | lpos);
	  newc[i].pval = f[i]+1;
	  if (positiveT |= newc) {
	    // varphi.push_back(newc);
	    varphi.insert(newc);
	    eliminated = true;
	  }
	}
	i++;
      }
    }
  }
  return varphi;
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

  // start clock
  auto clock_start = chrono::high_resolution_clock::now();

  Formula varphi = learn_horn_formula();
  
  // stop the clock
  auto clock_stop = chrono::high_resolution_clock::now();
  auto duration = chrono::duration_cast<chrono::milliseconds>(clock_stop - clock_start);

  cout << "+++ produced Horn formula:" << endl;
  cout << to_string(varphi) << endl;
  unsigned ncl = varphi.size();
  cout << endl
       << "+++ Horn formula has " << ncl
       << (ncl == 1 ? " clause" : " clauses") << endl;
  if (!latex.empty())
    latexfile << to_latex(varphi) << endl;
  if (!machine.empty())
    machinefile << to_machine(varphi);

  cout << endl << "+++ Elapsed time (seconds) = " << 0.001 * duration.count() << endl;
  cout << "+++ end of run" << endl;}

////////////////////////////////////////////////////////////////////////////////
