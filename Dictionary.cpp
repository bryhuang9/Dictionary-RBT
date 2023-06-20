//-----------------------------------------------------------------------------
// Brian Huang
// bryhuang@ucsc.edu
// 2023 Winter CSE101 PA8
// BigInteger.cpp
// Implement file for Dictionary ADT
//-----------------------------------------------------------------------------

#include<iostream>
#include<string>
#include "Dictionary.h"

using namespace std;

// Class Constructors & Destructors ----------------------------------------

// Creates new Dictionary in the empty state.
Dictionary::Node::Node(keyType k, valType v){
    key = k;
    val = v;
    parent = nullptr;
    left = nullptr;
    right = nullptr;
}

// constructor
Dictionary::Dictionary(){
    nil = new Node("", 0);
    nil -> color = BLACK;
    nil -> left = nil -> right = nil -> parent = nil;
    root = current = nil;
    num_pairs = 0;
}

// Copy constructor.
Dictionary::Dictionary(const Dictionary& D){
    nil = new Node("", 0);
    nil -> color = BLACK;
    nil -> left = nil -> right = nil -> parent = nil;
    root = current = nil;
    num_pairs = 0;
    preOrderCopy(D.root, D.nil);
    num_pairs = D.num_pairs;
}

// Destructor
Dictionary::~Dictionary(){
    postOrderDelete(root);
    delete nil;
}


// Access functions --------------------------------------------------------

// size()
// Returns the size of this Dictionary.
int Dictionary::size() const{
    return num_pairs;
}

// contains()
// Returns true if there exists a pair such that key==k, and returns false
// otherwise.
bool Dictionary::contains(keyType k) const{
    Node* N = search(root, k);
    
    return (N != nil);
}

// getValue()
// Returns a reference to the value corresponding to key k.
// Pre: contains(k)
valType& Dictionary::getValue(keyType k) const{
    Node* N = search(root, k);
    
    if(N == nil){
        throw std::invalid_argument("getValue() called with non-existent key");
    }
    
    return N -> val;
}

// hasCurrent()
// Returns true if the current iterator is defined, and returns false
// otherwise.
bool Dictionary::hasCurrent() const{
    return (current != nil);
}

// currentKey()
// Returns the current key.
// Pre: hasCurrent()
keyType Dictionary::currentKey() const{
    if(!hasCurrent()){
        throw std::invalid_argument("currentKey() called when iterator is undefined");
    }
    
    return current -> key;
}

// currentVal()
// Returns a reference to the current value.
// Pre: hasCurrent()
valType& Dictionary::currentVal() const{
    if(!hasCurrent()){
        throw std::invalid_argument("currentVal() called when iterator is undefined");
    }
    
    return current -> val;
}


// Manipulation procedures -------------------------------------------------

// clear()
// Resets this Dictionary to the empty state, containing no pairs.
void Dictionary::clear(){
    postOrderDelete(root);
    root = current = nil;
    num_pairs = 0;
}

// setValue()
// If a pair with key==k exists, overwrites the corresponding value with v,
// otherwise inserts the new pair (k, v).
void Dictionary::setValue(keyType k, valType v){
    Node* current = root;
    Node* parent = nil;
    
    while(current != nil){
        parent = current;
        
        if(k < current -> key){
            current = current -> left;
        }
        else if(k > current -> key){
            current = current -> right;
        }
        else{
            current -> val = v;
            return;
        }
    }
    
    Node* N = new Node(k, v);
    N -> parent = parent;

    if(parent == nil){
        root = N;
    }
    else if(k < parent -> key){
        parent -> left = N;
    }
    else{
        parent -> right = N;
    }
    
    N -> left = N -> right = nil;
    N -> color = RED;
    
    RB_InsertFixUp(N);
    num_pairs++;
}

// remove()
// Deletes the pair for which key==k. If that pair is current, then current
// becomes undefined.
// Pre: contains(k).
void Dictionary::remove(keyType k) {
    Node* N = search(root, k);
    
    if(N == nil){
        return;
    }
    
    if(N == current){
        current = nil;
    }
    
    RB_Delete(N);
    num_pairs--;
}

// begin()
// If non-empty, places current iterator at the first (key, value) pair
// (as defined by the order operator < on keys), otherwise does nothing.
void Dictionary::begin(){
    current = root;
    
    while(current -> left != nil){
       current = current -> left;
    }
}

// end()
// If non-empty, places current iterator at the last (key, value) pair
// (as defined by the order operator < on keys), otherwise does nothing.
void Dictionary::end(){
    current = root;
    
    while(current -> right != nil){
       current = current -> right;
    }
}

// next()
// If the current iterator is not at the last pair, advances current
// to the next pair (as defined by the order operator < on keys). If
// the current iterator is at the last pair, makes current undefined.
// Pre: hasCurrent()
void Dictionary::next(){
    if(!hasCurrent()){
        throw std::invalid_argument("next() called when iterator is undefined");
    }
    
    current = findNext(current);
}

// prev()
// If the current iterator is not at the first pair, moves current to
// the previous pair (as defined by the order operator < on keys). If
// the current iterator is at the first pair, makes current undefined.
// Pre: hasCurrent()
void Dictionary::prev(){
    if(!hasCurrent()){
        throw std::invalid_argument("prev() called when iterator is undefined");
    }
    
    current = findPrev(current);
}


// Other Functions ---------------------------------------------------------

// to_string()
// Returns a string representation of this Dictionary. Consecutive (key, value)
// pairs are separated by a newline "\n" character, and the items key and value
// are separated by the sequence space-colon-space " : ". The pairs are arranged
// in order, as defined by the order operator <.
std::string Dictionary::to_string() const{
    std::string s;
    inOrderString(s, root);
    return s;
}

// pre_string()
// Returns a string consisting of all keys in this Dictionary. Consecutive
// keys are separated by newline "\n" characters. The key order is given
// by a pre-order tree walk.
std::string Dictionary::pre_string() const{
    std::string s;
    preOrderString(s, root);
    return s;
}

// equals()
// Returns true if and only if this Dictionary contains the same (key, value)
// pairs as Dictionary D.
bool Dictionary::equals(const Dictionary& D) const{
    std::string str, str1;
    inOrderString(str, root);
    D.inOrderString(str1, D.root);
    return (str == str1);
}


// Overloaded Operators ----------------------------------------------------

// operator<<()
// Inserts string representation of Dictionary D into stream, as defined by
// member function to_string().
std::ostream& operator<<( std::ostream& stream, Dictionary& D ){
    stream << D.to_string();
    return stream;
}

// operator==()
// Returns true if and only if Dictionary A equals Dictionary B, as defined
// by member function equals().
bool operator==( const Dictionary& A, const Dictionary& B ){
    return A.equals(B);
}

// operator=()
// Overwrites the state of this Dictionary with state of D, and returns a
// reference to this Dictionary.
Dictionary& Dictionary::operator=( const Dictionary& D ){
    if(this != &D){
        postOrderDelete(root);
        nil = new Node("", 0);
        nil -> left = nil -> right = nil -> parent = nil;
        root = nil;
        num_pairs = 0;
        preOrderCopy(D.root, D.nil);
    }

    return *this;
}
