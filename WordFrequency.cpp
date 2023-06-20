//-----------------------------------------------------------------------------
// Brian Huang
// bryhuang@ucsc.edu
// 2023 Winter CSE101 PA8
// WordFrequency.cpp
//-----------------------------------------------------------------------------

#include <iostream>
#include <fstream>
#include <string>
#include "Dictionary.h"

using namespace std;

int main(int argc, char* argv[]) {
    if(argc != 3){
        cerr << "Usage: " << argv[0] << " input_file output_file" << endl;
        return 1;
    }

    ifstream infile(argv[1]);
    
    if(!infile){
        cerr << "Error: could not open " << argv[1] << endl;
        return 1;
    }

    Dictionary dict;
    string line;
    string delim = " \t\\\"\',<.>/?;:[{]}|`~!@#$%^&*()-_=+0123456789";
    
    while(getline(infile, line)){
        size_t pos = line.find_first_not_of(delim, 0);
        
        while(pos != string::npos){
            size_t end_pos = line.find_first_of(delim, pos);
            
            if(end_pos == string::npos){
                end_pos = line.length();
            }
            
            string word = line.substr(pos, end_pos - pos);
            
            for (size_t i = 0; i < word.length(); i++) {
                word[i] = tolower(word[i]);
            }
            
            if(dict.contains(word)){
                int& freq = dict.getValue(word);
                freq++;
            }
            else{
                dict.setValue(word, 1);
            }
            
            pos = line.find_first_not_of(delim, end_pos);
        }
    }

    infile.close();
    ofstream outfile(argv[2]);
    
    if(!outfile){
        cerr << "Error: could not open " << argv[2] << endl;
        return 1;
    }

    string dict_str = dict.to_string();
    outfile << dict_str;

    outfile.close();

    return 0;
}
