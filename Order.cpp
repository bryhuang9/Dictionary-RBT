//-----------------------------------------------------------------------------
// Brian Huang
// bryhuang@ucsc.edu
// 2023 Winter CSE101 PA7
// Order.cpp
//-----------------------------------------------------------------------------

#include <iostream>
#include <fstream>
#include <sstream>
#include "Dictionary.h"

using namespace std;

int main(int argc, char* argv[]){
    // Check command line arguments
    if (argc != 3) {
        cerr << "Usage: " << argv[0] << " <input file> <output file>" << endl;
        return 1;
    }

    // Open input file
    ifstream input(argv[1]);
    if (!input) {
        cerr << "Error: could not open input file " << argv[1] << endl;
        return 1;
    }

    // Create dictionary
    Dictionary dict;

    // Read input file and insert keys into dictionary
    string line;
    int lineNumber = 1;
    while (getline(input, line)) {
        dict.setValue(line, lineNumber++);
    }

    // Close input file
    input.close();

    // Open output file
    ofstream output(argv[2]);
    if (!output) {
        cerr << "Error: could not open output file " << argv[2] << endl;
        return 1;
    }

    // Output dictionary as key-value pairs in alphabetical order
    dict.begin();
    while (dict.hasCurrent()) {
        output << dict.currentKey() << " : " << dict.currentVal() << endl;
        dict.next();
    }

    // Output dictionary keys in pre-order tree walk order
    string preOrderStr = dict.pre_string();
    output << preOrderStr;

    // Close output file
    output.close();

    return 0;
}





