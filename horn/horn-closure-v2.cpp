#include <iostream>
#include <fstream>
#include <string>
#include <map>
#include "matrix+formula.hpp"
#include "common.hpp"

using namespace std;

void read_input () {
  string line;
  while (getline(cin, line)) {
    Row m = split(line, delim);
    unsigned example = m.front();
    m.pop_front();
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

Matrix horn_closure (const Matrix &M) {	// Horn closure of a matrix
  Matrix HC;
  Matrix newM = M;

  while (! newM.empty()) {
    auto mm = newM.begin();
    for (Row m : M) {
      Row newm = min(*mm, m);
      newM.insert(newm);
    }
    HC.insert(*mm);
    newM.erase(mm);
  }
  return HC;
}

int main (int argc, char **argv) {
  read_arg(argc, argv);
  string command = argv[0];
  adjust_and_open(command);
  read_input();
  Matrix HCpos = horn_closure(positiveT);
  cout << "+++ Horn closure of positive examples" << endl;
  cout << HCpos<< endl;
  Matrix HCneg = horn_closure(negativeF);
  cout << "+++ Horn closure of negative examples" << endl;
  cout << HCneg<< endl;
}

////////////////////////////////////////////////////////////////////////////////
