#include <iostream>
#include <math.h> // for asserting height
#include <queue>
#include <cassert>

using namespace std;

#define RED 0
#define BLACK 1

template <class T>
class RBT;

// swapColor swaps the color from red to black and vice versa
int swapColor(int c) {
    return (c==0)?1:0;
}

template <class T>
class RBTNode {
    RBTNode<T> *parent, *left, *right;
    T data;
    int color;

public:
    RBTNode(T data)
        : data(data),
          color(RED),
          parent(nullptr),
          left(nullptr),
          right(nullptr) {}
    friend class RBT<T>;
    void prettyPrint(int indent) const;

    template <class T1>
    friend void swapColor(RBTNode<T1> *);
    template <class T1>
    friend int getColor(RBTNode<T1> *);
    
    int height() const;
};

template <class T>
int RBTNode<T>::height() const {
    int left_h = 0;
    if (left != nullptr) {
        left_h = left->height();
    }
    int right_h = 0;
    if (right != nullptr) {
        right_h = right->height();
    }
    return 1 + max(left_h, right_h);
}

template <class T>
void RBTNode<T>::prettyPrint(int indent) const {
    if (right != nullptr) {
        right->prettyPrint(indent + 1);
    }
    int margin = indent * 2;
    for (int i = 0; i < margin; ++i) {
        cout << '\t';
    }
    cout << "DATA: " << data << endl;
    for (int i = 0; i < margin; ++i) {
        cout << '\t';
    }
    cout << "COLOR: " << (color == RED ? "RED" : "BLACK") << endl;
    if (left != nullptr) {
        left->prettyPrint(indent + 1);
    }
}

template <class T>
void swapColor(RBTNode<T> *node) {
    if (node != nullptr) {
        node->color = swapColor(node->color);
    }
}

// getColor handles null nodes
template <class T>
int getColor(RBTNode<T> *node) {
    if (node != nullptr) {
        return node->color;
    }
    return BLACK;
}

template <class T>
class RBT {
    RBTNode<T> *root;
    void singleCCR(RBTNode<T> *&point);
    void doubleCR(RBTNode<T> *&point);
    void singleCR(RBTNode<T> *&point);
    void doubleCCR(RBTNode<T> *&point);

public:
    RBT() : root(nullptr) {}

    void insert(const T &);
    void insert(const T &, RBTNode<T> *&point, RBTNode<T> *parent);
    void prettyPrint() const { root->prettyPrint(0); }
    void blackRotations(RBTNode<T> *&point, RBTNode<T> *parent);
    void redUncle(RBTNode<T> *&node);
    void analyzeColoring(RBTNode<T> *&node, RBTNode<T> *&parent);
    int height() const { return root->height(); }
};

template <class T>
void RBT<T>::doubleCCR(RBTNode<T> *&point) {
    singleCR(point->right);
    singleCCR(point->parent);
}

template <class T>
void RBT<T>::doubleCR(RBTNode<T> *&point) {
    singleCCR(point->left);
    singleCR(point->parent);
}

template <class T>
void RBT<T>::singleCR(RBTNode<T> *&point) {
    RBTNode<T> *grandparent = point;
    RBTNode<T> *parent = point->left;
    // TODO: ADD ROTATION CODE HERE
    if(parent->right != nullptr){
        grandparent->left = parent->right;
        parent->right->parent = grandparent;
    }
    else{
        grandparent->left = nullptr;
    }

    if(grandparent == root){
        root = parent;
        root->right = grandparent;
        root->left = parent->left;
    }   
    else if(grandparent == grandparent->parent->right){    
        grandparent->parent->right = parent;
        parent->parent = grandparent->parent;
    }
    else{
        grandparent->parent->left = parent;
        parent->parent = grandparent->parent;
    }




    parent->right = grandparent;
    grandparent->parent = parent;
}

template <class T>
void RBT<T>::singleCCR(RBTNode<T> *&point) {
    RBTNode<T> *grandparent = point;
    RBTNode<T> *parent = point->right;
    // TODO: ADD ROTATION CODE HERE
    if(grandparent == root){
        root = parent;
        //root->left = grandparent;
        //root->right = parent->right;
        if(root->color == RED)
            swapColor(root);
    }   
    else if(grandparent == grandparent->parent->right){    
        grandparent->parent->right = parent;
        parent->parent = grandparent->parent;
    }
    else{
        grandparent->parent->left = parent;
        parent->parent = grandparent->parent;
    }


    if(parent->left != nullptr){
        grandparent->right = parent->left;
        parent->left->parent = grandparent;
    }
    else{
        grandparent->right = nullptr;
    }

    parent->left = grandparent;
    grandparent->parent = parent;
}

template <class T>
void RBT<T>::blackRotations(RBTNode<T> *&point, RBTNode<T> *parent){
    //node is on outside right
    if(point == parent->right && parent == parent->parent->right){
        singleCCR(parent->parent);
        if(parent!=root|| root->color == RED)
            swapColor(parent);
        swapColor(parent->left);     
    }
    
    //node is outside left
    else if(point == parent->left && parent == parent->parent->left){
        singleCR(parent->parent);
        if(parent!=root || root->color == RED)
            swapColor(parent);
        swapColor(parent->right);
    }
    
    //node is inside right
    else if(point == parent->left && parent == parent->parent->right){
        doubleCCR(parent->parent);
        if(root->color == RED)
            swapColor(root);
        if(point!=root)
            swapColor(point);
        if(point->left->color == BLACK)
            swapColor(point->left);
    }
                
    //node is inside left
    else{
        doubleCR(parent->parent);
        if(root->color == RED)
            swapColor(root);
        if(point!=root)
            swapColor(point);
        if(point->right->color == BLACK)
            swapColor(point->right);
    }
}

template<class T>
void RBT<T>::redUncle(RBTNode<T> *&node){
    swapColor(node->parent->right);
    swapColor(node->parent->left);
    if(node->parent != root)
        swapColor(node->parent);
}

template <class T>
void RBT<T>::analyzeColoring(RBTNode<T> *&node, RBTNode<T> *&parent){
    //if parent is black, do nothing
    if(root->color == RED)
        swapColor(root);
    if(parent->color == RED && node->color == RED){
        //uncle does not exist
        if(parent->parent->right == nullptr || parent->parent->left == nullptr){ 
            blackRotations(node, parent);
        }
        //uncle is red
        else if(parent->parent->right->color == RED && parent->parent->left->color == RED){
            redUncle(parent);
            RBTNode<T>* temp = parent->parent;
            while(temp != root){
                analyzeColoring(temp, temp->parent);
                temp = temp->parent;
            }
        }
        //uncle is black
        else{
            blackRotations(node, parent);
        }
    }
}
template <class T>
void RBT<T>::insert(const T &toInsert, RBTNode<T> *&point, RBTNode<T> *parent) {
    if (point == nullptr) {               // leaf location is found so insert node
        point = new RBTNode<T>(toInsert); // modifies the pointer itself since *point
                                          // is passed by reference
            point->parent = parent;
            if(parent == nullptr){
                root = point;
                swapColor(root);
                return;
            }
        RBTNode<T> *curr_node = point; // curr_node will be set appropriately when walking up the tree
        // TODO: ADD RBT RULES HERE
    
        analyzeColoring(curr_node, parent);

    
    } else if (toInsert < point->data) { // recurse down the tree to left to find correct leaf location
        insert(toInsert, point->left, point);
    } else { // recurse down the tree to right to find correct leaf location
        insert(toInsert, point->right, point);
    }
}

template <class T>
void RBT<T>::insert(const T &toInsert) {
    insert(toInsert, root, nullptr);
}

// NOTE: DO NOT MODIFY THE MAIN FUNCTION BELOW
int main() {
    RBT<int> b;

    int count = 10;
    for (int i = 0; i < count; i++) {
        b.insert(i);
    }

    b.prettyPrint();
    /* EXPECTED OUTPUT:
                                                                    Data: 9
                                                                    COLOR: RED
                                                    Data: 8
                                                    COLOR: BLACK
                                    Data: 7
                                    COLOR: RED
                                                    Data: 6
                                                    COLOR: BLACK
                    Data: 5
                    COLOR: BLACK
                                    Data: 4
                                    COLOR: BLACK
    Data: 3
    COLOR: BLACK
                                    Data: 2
                                    COLOR: BLACK
                    Data: 1
                    COLOR: BLACK
                                    Data: 0
                                    COLOR: BLACK
    */

    // TEST
    // the below tests the validity of the height of the RBT
    // if the assertion fails, then your tree does not properly self-balance
    int height = b.height();
    assert(height <= 2 * log2(count));
    
}
