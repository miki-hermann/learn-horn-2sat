#include <iostream>
#include <fstream>
#include <string>
#include <cstdlib>

using namespace std;

ifstream infile;
ofstream outfile;

int main (int argc, char **argv) {
  size_t dummy = system("unxz --keep --force digit.csv.xz");
  cerr << "Which digit (0-9) do you want to recognize? ";
  int digit;
  cin >> digit;
  cerr <<  "Recognizing digit " << digit << endl;

  infile.open("digit.csv");
  if (! infile.is_open()) {
    cerr << "+++ Cannot open input file digit.csv" << endl;
    exit(1);
  }

  string filename = "digit" + to_string(digit) + ".csv";
  outfile.open(filename);
  if (! outfile.is_open()) {
    cerr << "+++ Cannot open output file " << filename << endl;
    exit(1);
  }

  string line;
  while (getline(infile, line)) {
    int leader = stoi(line.substr(0,1)) == digit ? 1 : 0;
    outfile << leader
	    << line.substr(1)
	    << endl;
  }

  infile.close();
  outfile.close();
  dummy = system("rm -f digit.csv");
  cerr << "CSV file " << filename << " generated" << endl;
}

////////////////////////////////////////////////////////////////////////////////
