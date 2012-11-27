//
//  greggjsStarbucks.cpp
//  HW04
//
//  Created by Jake Gregg on 10/29/12.
//
//

#include "greggjsStarbucks.h"

using namespace std;
using namespace ci;
using namespace ci::app;

/// creates a new search tree. Intiallizes all parameters
greggjsStarbucks::greggjsStarbucks() {
    this->search_root_ = new Node();
    this->search_root_->data_ = new Entry();
    this->search_root_->left_ = new Node();
    this->search_root_->right_ = new Node();
}

/// deletes the search tree
greggjsStarbucks::~greggjsStarbucks() {
    delete this->search_root_;
    search_root_ = NULL;
}

/// imports all the Starbucks data into an entry array of a specified filename.
int greggjsStarbucks::importData(Entry* import, string filename) {
    try{
		ifstream fid (filename);
        
		int index = 0;
		string value;
		double x, y;
        
		while(fid.good()){
            
			getline(fid, value, ','); /// get the identifier
			fid >> x;
			fid.get();
			fid >> y;
			fid.get();

			Entry* entry = &import[index];
			entry->identifier = value;
			entry->x = x;
			entry->y = y;
            
			///import[index] = *temp; /// store the entry in the array to be returned
            
			//checking output for correctness:
			//console() << (locations->back())->identifier << ","<< (locations->back())->x << ","<< (locations->back())->y << std::endl ;
            
			index++;
		}
        
		fid.close(); /// close the file reader and return how many entries were written.
        
		return index;
        
	}catch(ios_base::failure){ /// if the file system could not read, then tell the user.
        
		cout << "Error Reading File\n";
		return NULL;
        
	}
}

/// this builds the tree for searching.
void greggjsStarbucks::build(Entry *c, int n) {
    
    /// shuffle all entries so we cut build time.
    this->shuffle(c, n);
    
    /// initiallize the root as the first node in the array
    this->search_root_->data_ = &c[0];
    
    for (int i = 1; i < n; i++) {
        /// insert the new entry
		c[i].y = 1-c[i].y;
        Node* new_node = this->insert(&c[i], this->search_root_, true);
        /// initiallize all of the data contained by the entry, as well as neighbors
        new_node->data_ = &c[i];
		new_node->color_ = Color8u((int)((new_node->data_->x)*255), 
			(int)((1-(new_node->data_->y)*255)), 
			(int)((new_node->data_->x)*(new_node->data_->y)*255));
        new_node->left_ = new Node();
        new_node->right_ = new Node();
    }
}

/// shuffles all entries in the array so we can acheive an almost-balanced tree
/// with a shorter build time than sorting the tree and finding the medians.
void greggjsStarbucks::shuffle(Entry *c, int n) {

    /// initiallize a random seed and a variable to store it
    srand ( unsigned ( time (NULL) ) );
    int random;
    
    /// for all the elements, swap two entries positions
    if (n>1) {
        for (int i = 0; i < n; ++i) {
            random = rand() % n;
            Entry temp = c[random];
            c[random] = c[i];
            c[i] = temp;
        }
    }
}

/// this inserts a new Node into the array recursively. It returns the pointer
/// of the where the entry is.
Node* greggjsStarbucks::insert(Entry *c, Node *root, bool xLevel) {
    /// base case: if we hit an empty Node
    if (root->data_ == NULL) return root;
    
    /// if we're looking at the x level...
    if (xLevel) {
        /// if our entry is greater than the curr x go right
        if (c->x > root->data_->x)
            return insert(c, root->right_, !xLevel);
        /// less than go left
        else
            return insert(c, root->left_, !xLevel);
    /// if we're looking at the y level...
    } else {
        /// if our entry is greater than the curr y go right
        if (c->y > root->data_->y)
            return insert(c, root->right_, !xLevel);
        /// less than go left
        else
            return insert(c, root->left_, !xLevel);
    }
}

/// this prints out the current tree from left, center, right
void greggjsStarbucks::printTree(Node *root) {
    /// base case: if we hit an empty Node
    if (root->data_ == NULL) return;
    /// print the left tree
    printTree(root->left_);
    /// print out the current Node
    console() << root->data_->identifier << "," << root->data_->x << "," << root->data_->y << endl;
    /// print out the right tree
    printTree(root->right_);
}

/// this method gets the nearest Entry given a set of x and y coordinates.
/// it does this by calling the search method and returning the data within
/// the Node.
Entry* greggjsStarbucks::getNearest(double x, double y) {
    Node* nearest = search(x, y, this->search_root_, true);
    return nearest->data_;
}

/// this searches the array recursively to find the nearest neighbor to the given
/// coordinates.
Node* greggjsStarbucks::search(double x, double y, Node *root, bool xLevel) {
    /// base case: if we hit an empty Node
	if(root->data_ == NULL) return NULL;
    
    /// first find the parent distance. if it's within the specified range of
    /// 0.00001, then we just return the Node we're searching on.
	double parentDistance = sqrt((root->data_->x - x)*(root->data_->x - x)
                                 + (root->data_->y - y)*(root->data_->y - y));
    
	if(parentDistance < .00001)
		return root;
    
    /// define some variables to use
	bool rightSide = false;
	Node* candidate = NULL;
	Node* otherPath = NULL;
    
    /// if we're searching on x...
	if(xLevel){
        /// if the x is greater than the entry, go right
		if(x > root->data_->x){
			candidate = search(x, y, root->right_, !xLevel);
			otherPath = root->left_; /// store other path as left
		/// else go left
        }else{
			candidate = search(x, y, root->left_, !xLevel);
			otherPath = root->right_; /// store other path as right
		}
    /// if we're searching on y...
	}else{
        /// if the y is greater than the entry, go right
		if(y > root->data_->y){
			candidate = search(x, y, root->right_, !xLevel);
			otherPath = root->left_; /// store other path as left
		/// else go left
        }else{
			candidate = search(x, y, root->left_, !xLevel);
			otherPath = root->right_; /// store other path as right
		}
	}
    
    /// if the candidate is NULL....
	if(candidate == NULL){
		candidate = root;
        /// search down the other path specified...
		Node* twinCandidate = search(x, y, otherPath, !xLevel);
        
        /// if we find a twin
		if(twinCandidate != NULL){
            /// if the distance from the twin is less than our parent distance, then save that as the
            /// new candidate
			double twinDistance = sqrt((twinCandidate->data_->x - x)*(twinCandidate->data_->x - x)
                                       + (twinCandidate->data_->y - y)*(twinCandidate->data_->y - y));
            
			if(twinDistance < parentDistance)
				candidate = twinCandidate;
		}
	}
    /// if the candidate is not NULL....
	else{
        /// calculate the candidate's distance from query...
		double candidateDistance = sqrt((candidate->data_->x - x)*(candidate->data_->x - x)
                                        + (candidate->data_->y - y)*(candidate->data_->y - y));
        
        /// if the parent is better candidate than the child...
		if(parentDistance < candidateDistance){ /// go through same thing as when candidate was NULL
			candidate = root;
			Node* twinCandidate = search(x, y, otherPath, !xLevel);
            
			if(twinCandidate != NULL){
				double twinDistance = sqrt((twinCandidate->data_->x - x)*(twinCandidate->data_->x - x)
                                           + (twinCandidate->data_->y - y)*(twinCandidate->data_->y - y));
                
				if(twinDistance < parentDistance)
					candidate = twinCandidate;
			}
		}
        
        /// if the candidate is greater than the x boundary...
		else if(xLevel && candidateDistance > abs(root->data_->x - x)){
			Node* twinCandidate = search(x, y, otherPath, !xLevel);
            
            /// checks to see if there is a twin...
			if(twinCandidate != NULL){
				double twinDistance = sqrt((twinCandidate->data_->x - x)*(twinCandidate->data_->x - x)
                                           + (twinCandidate->data_->y - y)*(twinCandidate->data_->y - y));
                
				if(twinDistance < candidateDistance)
					candidate = twinCandidate;
			}
		}
        
        /// candidate greater than y boundary...
		else if(!xLevel && candidateDistance > abs(root->data_->y - y)){
			Node* twinCandidate = search(x, y, otherPath, !xLevel);
            
            /// check to see if there is a twin...
			if(twinCandidate != NULL){//~~~~~~CHECK TWIN~~~~~~//
				double twinDistance = sqrt((twinCandidate->data_->x - x)*(twinCandidate->data_->x - x) 
                                           + (twinCandidate->data_->y - y)*(twinCandidate->data_->y - y));
                
				if(twinDistance < candidateDistance)
					candidate = twinCandidate;
			}
		}
        
	}
    
    /// return the candidate Node
	return candidate;
}

/// this runs through the array of imported Entries and gets the closest Starbucks.
/// brute force technique used for debugging.
Entry* greggjsStarbucks::alwaysRightValue(Entry *c, int n, double x, double y) {
    // start minimum distance at 10, which is more than the maximum possible distance
    double distance = 0 , minDistance = 10;
	Entry* minEntry = NULL;
    
	for(int i = 0; i < n; i++){
        
		Entry* current = &c[i];
		distance = sqrt((current->x - x)*(current->x - x) + (current->y - y)*(current->y - y));
        
		if(distance < minDistance){
			minDistance = distance;
			minEntry = current;
		}
        
	}
    
	return minEntry;
}

/// This method was modeled from https://github.com/harmonbc/HW04_harmonbc/blob/master/src/HarmonStarBucks.cpp
/// But modified for use with my tree. 
void greggjsStarbucks::getArea(Vec2f top_left, Vec2f bottom_right, Node* root, bool xLevel, LinkList* list) {
	if (root->data_ == NULL) return;

	double x_temp = root->data_->x;
	double y_temp = root->data_->y;
    if (((xLevel) ? top_left.x : top_left.y) < ((xLevel) ? (float)x_temp : (float)y_temp)) {
        getArea(top_left, bottom_right, root->left_, !xLevel, list);
    }
    if (((xLevel) ? top_left.x : top_left.y) > ((xLevel) ? (float)x_temp : (float)y_temp)) {
        getArea(top_left, bottom_right, root->right_, !xLevel, list);
    }
    Entry temp = *root->data_;
    if (temp.x >= top_left.x && temp.x<= bottom_right.x && temp.y >= top_left.y && temp.y <= bottom_right.y) {
        LinkList* list_temp = new LinkList();
		
        list_temp->vec_ = Vec2f((float)x_temp, (float)y_temp);
        list_temp->next_ = list->next_;
        list_temp->address_ = temp.identifier;
        list_temp->color_ = root->color_;
        list->next_ = list_temp;
    }
}







