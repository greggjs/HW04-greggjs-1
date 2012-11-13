//
//  greggjsStarbucks.h
//  HW04
//
//  Created by Jake Gregg on 10/29/12.
//
//

#ifndef __HW04__greggjsStarbucks__
#define __HW04__greggjsStarbucks__

#include <iostream>
#include "cinder/app/AppBasic.h"
#include "cinder/gl/gl.h"
#include "Starbucks.h"
#include "LinkList.h"
#include "CensusData.h"
#include <fstream>
#include <vector>

#endif /* defined(__HW04__greggjsStarbucks__) */

using namespace std;
using namespace cinder;

/// this class is for the nodes in our k-d tree
class Node {
public:
    /// each node contains an entry, and pointers to
    /// what is right and left of them
    Entry* data_;
    Color8u color_;
    Node* right_;
    Node* left_;
    
    /// initiallizing a Node means setting the data to NULL
    Node () {
        this->data_= NULL;
        this->color_ = NULL;
    }
    
    /// deleting a node involves deleting all components
    ~Node() {
        delete this->data_;
        delete this->color_;
        delete this->left_;
        delete this->right_;
    }
};

/// this class is for HW04 for searching for the nearest Starbucks,
/// abstracted from Dr. Brinkman's Starbucks class.
class greggjsStarbucks : public Starbucks {
public:
    /// store pointer for where root of tree is
    Node* search_root_;
    
    /// constructor/destructor for building tree
    greggjsStarbucks();
    ~greggjsStarbucks();
    
    /// imports data into a given array of Entries and a filename.
    int importData(Entry* import, string filename);
    
    /// builds the tree structure given an array of Entries
    virtual void build(Entry* c, int n);
    
    /// gets the nearest Entry to the given coordinates
    virtual Entry* getNearest(double x, double y);
    
    /// returns the correct Entry for getNearest. For testing purposes
    Entry* alwaysRightValue(Entry* c, int n, double x, double y);
    
    /// prints the current tree. Used for testing. So far.
    void printTree(Node* root);
    
    /// inserts a new Node into the tree. Returns the pointer to the Node.
    Node* insert(Entry* c, Node* root, bool xLevel);
    
    /// searches the tree for the closest entry given a query point.
    Node* search(double x, double y, Node* root, bool xLevel);
    
    /// shuffles all Entry array elements for better tree build.
    static void shuffle(Entry* c, int n);
    
    void getArea(Vec2f top_left, Vec2f bottom_right, Node* starbucks, bool xLevel, LinkList* list);
    
};