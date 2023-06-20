//-----------------------------------------------------------------------------
// Brian Huang
// bryhuang@ucsc.edu
// 2023 Winter CSE101 PA8
// Dictionary.h
// Header file for Dictionary ADT based on a Binary Search Tree. Includes a
// built in iterator called current that can perform a smallest-to-largest
// (forward) or a largest-to-smallest (reverse) iteration over all keys.
//-----------------------------------------------------------------------------
#include<iostream>
#include<string>

#ifndef DICTIONARY_H_INCLUDE_
#define DICTIONARY_H_INCLUDE_

#define RED 1
#define BLACK 2

// Exported types -------------------------------------------------------------
typedef std::string keyType;  // must be comparable using <, <=, >, >=, ==, !=
typedef int         valType;

class Dictionary{

private:
   // private Node struct
   struct Node{
      // Node fields
      keyType key;
      valType val;
      Node* parent;
      Node* left;
      Node* right;
      int color;
      // Node constructor
      Node(keyType k, valType v);
   };

   // Dictionary fields
   Node* nil;
   Node* root;
   Node* current;
   int   num_pairs;

   // Helper Functions (Optional) ---------------------------------------------

   // inOrderString()
   // Appends a string representation of the tree rooted at R to string s. The
   // string appended consists of: "key : value \n" for each key-value pair in
   // tree R, arranged in order by keys.
   void inOrderString(std::string& s, Node* R) const{
       if(R != nil){
           inOrderString(s, R -> left);
           s += R -> key + " : " + std::to_string(R -> val) + "\n";
           inOrderString(s, R -> right);
       }
   }

   // preOrderString()
   // Appends a string representation of the tree rooted at R to s. The appended
   // string consists of keys only, separated by "\n", with the order determined
   // by a pre-order tree walk.
   void preOrderString(std::string& s, Node* R) const{
       if(R != nil){
           s += R -> key + "\n";
           preOrderString(s, R -> left);
           preOrderString(s, R -> right);
       }
   }

   // preOrderCopy()
   // Recursively inserts a deep copy of the subtree rooted at R into this 
   // Dictionary. Recursion terminates at N.
   void preOrderCopy(Node* R, Node* N){
       if(R != N) {
           setValue(R -> key, R -> val);
           preOrderCopy(R -> left, N);
           preOrderCopy(R -> right, N);
       }
   }

   // postOrderDelete()
   // Deletes all Nodes in the subtree rooted at R, sets R to nil.
   void postOrderDelete(Node* R){
       if(R != nil){
           postOrderDelete(R -> left);
           postOrderDelete(R -> right);
           delete R;
       }
   }

   // search()
   // Searches the subtree rooted at R for a Node with key==k. Returns
   // the address of the Node if it exists, returns nil otherwise.
   Node* search(Node* R, keyType k) const{
       if(R == nil || R -> key == k){
           return R;
       }
       
       if(k < R -> key){
           return search(R -> left, k);
       }
       else{
           return search(R -> right, k);
       }
   }

   // findMin()
   // If the subtree rooted at R is not empty, returns a pointer to the 
   // leftmost Node in that subtree, otherwise returns nil.
   Node* findMin(Node* R){
       if(R == nil){
           return nil;
       }
       
       while(R -> left != nil){
           R = R -> left;
       }
       
       return R;
   }

   // findMax()
   // If the subtree rooted at R is not empty, returns a pointer to the 
   // rightmost Node in that subtree, otherwise returns nil.
   Node* findMax(Node* R){
       if(R == nil){
           return nil;
       }
       
       while(R -> right != nil){
           R = R -> right;
       }
       
       return R;
   }

   // findNext()
   // If N does not point to the rightmost Node, returns a pointer to the
   // Node after N in an in-order tree walk.  If N points to the rightmost 
   // Node, or is nil, returns nil. 
   Node* findNext(Node* N){
       if(N == nil){
           return nil;
       }
       
       if(N -> right != nil){
           return findMin(N -> right);
       }
       
       Node* parent = N -> parent;
       
       while(parent != nil && N == parent -> right){
           N = parent;
           parent = parent -> parent;
       }
       
       return parent;
   }

   // findPrev()
   // If N does not point to the leftmost Node, returns a pointer to the
   // Node before N in an in-order tree walk.  If N points to the leftmost 
   // Node, or is nil, returns nil.
   Node* findPrev(Node* N){
       if(N == nil){
           return nil;
       }
       
       if(N -> left != nil){
           return findMax(N -> left);
       }
       
       Node* parent = N -> parent;
       
       while(parent != nil && N == parent -> left){
           N = parent;
           parent = parent -> parent;
       }
       
       return parent;
   }
    
    // RBT Helper Functions (Optional) -----------------------------------------
    
    // LeftRotate()
    void LeftRotate(Node* N){
        Node* y = N -> right;
        N -> right = y -> left;

        if(y -> left != nil){
            y -> left -> parent = N;
        }

        y -> parent = N -> parent;

        if(N -> parent == nil){
            root = y;
        }
        else if(N == N -> parent -> left){
            N -> parent -> left = y;
        }
        else{
            N -> parent -> right = y;
        }

        y -> left = N;
        N -> parent = y;
    }

    // RightRotate()
    void RightRotate(Node* N){
        Node* y = N -> left;
        N -> left = y -> right;

        if(y -> right != nil){
            y -> right -> parent = N;
        }

        y -> parent = N -> parent;

        if(N -> parent == nil){
            root = y;
        }
        else if(N == N -> parent -> right){
            N -> parent -> right = y;
        }
        else{
            N -> parent -> left = y;
        }

        y -> right = N;
        N -> parent = y;
    }

    // RB_InsertFixUP()
    void RB_InsertFixUp(Node* N){
        while(N -> parent -> color == RED){
            if(N -> parent == N -> parent -> parent -> left){
                Node* y = N -> parent -> parent -> right;
                
                if(y -> color == RED){
                    N -> parent -> color = BLACK;
                    y -> color = BLACK;
                    N -> parent -> parent -> color = RED;
                    N = N -> parent -> parent;
                }
                else{
                    if(N == N->parent->right){
                        N = N -> parent;
                        LeftRotate(N);
                    }
                    N -> parent -> color = BLACK;
                    N -> parent -> parent -> color = RED;
                    RightRotate(N -> parent -> parent);
                }
            }
            else{
                Node* y = N -> parent -> parent -> left;
                
                if(y -> color == RED){
                    N -> parent -> color = BLACK;
                    y -> color = BLACK;
                    N -> parent -> parent -> color = RED;
                    N = N -> parent -> parent;
                }
                else{
                    if(N == N -> parent -> left){
                        N = N -> parent;
                        RightRotate(N);
                    }
                    
                    N -> parent -> color = BLACK;
                    N -> parent -> parent -> color = RED;
                    LeftRotate(N -> parent -> parent);
                }
            }
        }
        
        root -> color = BLACK;
    }

    // RB_Transplant()
    void RB_Transplant(Node* u, Node* v){
        if(u -> parent == nil){
            root = v;
        }
        else if(u == u -> parent -> left){
            u -> parent -> left = v;
        }
        else{
            u -> parent -> right = v;
        }
        
        v -> parent = u -> parent;
    }

    // RB_DeleteFixUp()
    void RB_DeleteFixUp(Node* N){
        while(N != root && N -> color == BLACK){
            if(N == N -> parent -> left){
                Node* y = N -> parent -> right;
                
                if(y -> color == RED){
                    y -> color = BLACK;
                    N -> parent -> color = RED;
                    LeftRotate(N -> parent);
                    y = N -> parent -> right;
                }
                
                if(y -> left -> color == BLACK && y -> right -> color == BLACK){
                    y -> color = RED;
                    N = N -> parent;
                }
                else{
                    if(y -> right -> color == BLACK){
                        y -> left -> color = BLACK;
                        y -> color = RED;
                        RightRotate(y);
                        y = N -> parent -> right;
                    }
                    
                    y -> color = N -> parent -> color;
                    N -> parent -> color = BLACK;
                    y -> right -> color = BLACK;
                    LeftRotate(N -> parent);
                    N = root;
                }
            }
            else{
                Node* y = N -> parent -> left;
                
                if(y -> color == RED){
                    y -> color = BLACK;
                    N -> parent -> color = RED;
                    RightRotate(N -> parent);
                    y = N -> parent -> left;
                }
                
                if(y -> right -> color == BLACK && y -> left -> color == BLACK){
                    y -> color = RED;
                    N = N -> parent;
                }
                else{
                    if(y -> left -> color == BLACK){
                        y -> right -> color = BLACK;
                        y -> color = RED;
                        LeftRotate(y);
                        y = N -> parent -> left;
                    }
                    
                    y -> color = N -> parent -> color;
                    N -> parent -> color = BLACK;
                    y -> left -> color = BLACK;
                    RightRotate(N -> parent);
                    N = root;
                }
            }
        }
        
        N -> color = BLACK;
    }

    // RB_Delete()
    void RB_Delete(Node* N){
        Node* y = N;
        int y_original_color = y -> color;
        Node* x;

        if(N -> left == nil){
            x = N -> right;
            RB_Transplant(N, N -> right);
        }
        else if(N -> right == nil){
            x = N -> left;
            RB_Transplant(N, N -> left);
        }
        else{
            y = findMin(N -> right);
            y_original_color = y -> color;
            x = y -> right;

            if(y -> parent == N){
                x -> parent = y;
            }
            else{
                RB_Transplant(y, y -> right);
                y -> right = N -> right;
                y -> right -> parent = y;
            }
            
            RB_Transplant(N, y);
            y -> left = N -> left;
            y -> left -> parent = y;
            y -> color = N -> color;
        }
        
        if(y_original_color == BLACK){
            RB_DeleteFixUp(x);
        }
    }

public:

   // Class Constructors & Destructors ----------------------------------------
   
   // Creates new Dictionary in the empty state. 
   Dictionary();

   // Copy constructor.
   Dictionary(const Dictionary& D);

   // Destructor
   ~Dictionary();


   // Access functions --------------------------------------------------------

   // size()
   // Returns the size of this Dictionary.
   int size() const;

   // contains()
   // Returns true if there exists a pair such that key==k, and returns false
   // otherwise.
   bool contains(keyType k) const;

   // getValue()
   // Returns a reference to the value corresponding to key k.
   // Pre: contains(k)
   valType& getValue(keyType k) const;

   // hasCurrent()
   // Returns true if the current iterator is defined, and returns false 
   // otherwise.
   bool hasCurrent() const;

   // currentKey()
   // Returns the current key.
   // Pre: hasCurrent() 
   keyType currentKey() const;

   // currentVal()
   // Returns a reference to the current value.
   // Pre: hasCurrent()
   valType& currentVal() const;


   // Manipulation procedures -------------------------------------------------

   // clear()
   // Resets this Dictionary to the empty state, containing no pairs.
   void clear();

   // setValue()
   // If a pair with key==k exists, overwrites the corresponding value with v, 
   // otherwise inserts the new pair (k, v).
   void setValue(keyType k, valType v);

   // remove()
   // Deletes the pair for which key==k. If that pair is current, then current
   // becomes undefined.
   // Pre: contains(k).
   void remove(keyType k);

   // begin()
   // If non-empty, places current iterator at the first (key, value) pair
   // (as defined by the order operator < on keys), otherwise does nothing. 
   void begin();

   // end()
   // If non-empty, places current iterator at the last (key, value) pair
   // (as defined by the order operator < on keys), otherwise does nothing. 
   void end();

   // next()
   // If the current iterator is not at the last pair, advances current 
   // to the next pair (as defined by the order operator < on keys). If 
   // the current iterator is at the last pair, makes current undefined.
   // Pre: hasCurrent()
   void next();

   // prev()
   // If the current iterator is not at the first pair, moves current to  
   // the previous pair (as defined by the order operator < on keys). If 
   // the current iterator is at the first pair, makes current undefined.
   // Pre: hasCurrent()
   void prev();


   // Other Functions ---------------------------------------------------------

   // to_string()
   // Returns a string representation of this Dictionary. Consecutive (key, value)
   // pairs are separated by a newline "\n" character, and the items key and value 
   // are separated by the sequence space-colon-space " : ". The pairs are arranged 
   // in order, as defined by the order operator <.
   std::string to_string() const;

   // pre_string()
   // Returns a string consisting of all keys in this Dictionary. Consecutive
   // keys are separated by newline "\n" characters. The key order is given
   // by a pre-order tree walk.
   std::string pre_string() const;

   // equals()
   // Returns true if and only if this Dictionary contains the same (key, value)
   // pairs as Dictionary D.
   bool equals(const Dictionary& D) const;


   // Overloaded Operators ----------------------------------------------------
   
   // operator<<()
   // Inserts string representation of Dictionary D into stream, as defined by
   // member function to_string().
   friend std::ostream& operator<<( std::ostream& stream, Dictionary& D );

   // operator==()
   // Returns true if and only if Dictionary A equals Dictionary B, as defined
   // by member function equals(). 
   friend bool operator==( const Dictionary& A, const Dictionary& B );

   // operator=()
   // Overwrites the state of this Dictionary with state of D, and returns a
   // reference to this Dictionary.
   Dictionary& operator=( const Dictionary& D );

};

#endif
