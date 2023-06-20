//-----------------------------------------------------------------------------
// Brian Huang
// bryhuang@ucsc.edu
// 2023 Winter CSE101 PA8
// DictionaryTest.cpp
// Test client for Dictionary ADT
//-----------------------------------------------------------------------------

#include <iostream>
#include "Dictionary.h"

using namespace std;

int main() {
    Dictionary D;

    // Test inserting key-value pairs
    D.setValue("apple", 5);
    D.setValue("banana", 3);
    D.setValue("cherry", 9);
    D.setValue("date", 2);

    // Test size() and contains()
    cout << "Dictionary size: " << D.size() << endl;
    cout << "Contains key 'banana': " << D.contains("banana") << endl;
    cout << "Contains key 'grape': " << D.contains("grape") << endl;

    // Test getValue()
    try {
        cout << "Value of key 'apple': " << D.getValue("apple") << endl;
        cout << "Value of key 'grape': " << D.getValue("grape") << endl;
    }
    catch (const invalid_argument& e) {
        cout << e.what() << endl;
    }

    // Test begin() and end()
    D.begin();
    while (D.hasCurrent()) {
        cout << "Current key: " << D.currentKey() << ", Current value: " << D.currentVal() << endl;
        D.next();
    }

    // Test remove()
    D.remove("cherry");
    cout << "Dictionary size after removing 'cherry': " << D.size() << endl;

    // Test clear()
    D.clear();
    cout << "Dictionary size after clearing: " << D.size() << endl;

    return 0;
}
