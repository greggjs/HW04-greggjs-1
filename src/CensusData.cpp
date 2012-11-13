//
//  CensusData.cpp
//  HW04
//
//  Created by Jake Gregg on 11/12/12.
//
//

#include "CensusData.h"

using namespace std;
using namespace ci;
using namespace ci::app;

CensusData::CensusData(string file) {
    file_location_ = file;
    this->left_ = NULL;
    this->right_ = NULL;
}
CensusData::CensusData() {}
CensusData::~CensusData() {}
void CensusData::buildCensusData(string file) {
    
	console() << "Building Census Data From File: " << file << endl;

	int trash, pop, i=0;
    double x, y;
    ifstream import(file);
    while (import.good()) {
        string trash;
        getline(import, trash, '\r');
        i++;
    }
    CensusData* census = new CensusData[i];
    
    i=0;
    import.clear();
    import.seekg(0);
    while(import.good()) {
        import >> trash;
        import.get();
        import >> trash;
        import.get();
        import >> trash;
        import.get();
        import >> trash;
        import.get();
        /// get the data
        import >> pop;
        import.get();
        import >> x;
        import.get();
        import >> y;
        import.get();
        CensusData* temp = &census[i];
        temp->population_ = pop;
        temp->x_ = 1-y;
        temp->y_ = x;
        i++;
    }
    
    build(census, i+1);
}

void CensusData::build(CensusData* census, int n) {
	ci::app::console() << "Building KD Tree " <<endl;
	randomize(census, n);
    this->population_ = census[0].population_;
    this->x_ = census[0].x_;
    this->y_ = census[0].y_;
    this->left_ = NULL;
    this->right_ = NULL;
    for (int i=1; i < n; i++)
        build(&census[i], this, true);
	delete[] census;
}

void CensusData::randomize(CensusData* census, int n) {
    for (int i = 0; i < n; i++) {
        CensusData temp = census[i];
        int index = rand() % n;
        census[i] = census[index];
        census[index] = temp;
    }
}

CensusData* CensusData::getNearest(double x, double y) {
    return getNearest(x, y, this, true);
}

CensusData* CensusData::getNearest(double x, double y, CensusData* census, bool xLevel) {
    
    double cur_x = census->x_;
	double cur_y = census->y_;
	double curDistance = ((x-cur_x)*(x-cur_x))+((y-cur_y)*(y-cur_y));

	CensusData* candidate=NULL;
	CensusData* other=NULL;

	//Compare Relevant Values and move in that direction.
	if(((xLevel) ? x : y) < ((xLevel) ? cur_x : cur_y))
	{
		if(census->left_==NULL) return census;
		candidate = getNearest(x,y,census->left_, !xLevel);
		other = census->right_;
	}
	else
	{
		if(census->right_==NULL) return census;
		candidate = getNearest(x,y,census->right_, !xLevel);
		other = census->left_;
	}

	double bestDistance = (x-(candidate->x_))*(x-(candidate->x_))+(y-(candidate->y_))*(y-(candidate->y_));
	if(curDistance <= bestDistance)
	{
		candidate = census;
		bestDistance = curDistance;
	}

	if(other!=NULL)
	{
		double oppositeValue = (xLevel) ? x : y;
		double currentValue = (xLevel) ? cur_x : cur_y;

		double checkDistance = (oppositeValue-currentValue)*(oppositeValue-currentValue);
		if(checkDistance < bestDistance)
		{
			CensusData* candidate2 = getNearest(x,y,other,!xLevel);
			if(((x-candidate2->x_)*(x-candidate2->x_)+(y-candidate2->x_)*(y-candidate2->x_))<bestDistance)
			{
				candidate = candidate2;
			}
		}
	}
	return candidate;

}

void CensusData::build(CensusData* one, CensusData* two, bool xLevel) {
    bool building = true;
    while (building) {
        double dist = (one->x_ - two->x_)*(one->x_ - two->x_)+(one->y_ - two->y_)*(one->y_ - two->y_);
        if (((xLevel) ? one->y_ : one->x_) < ((xLevel) ? two->y_ : two->x_)) {
            if (two->left_!=NULL) two = two->left_;
            else {
                two->left_ = one;
                one->left_ = NULL;
                one->right_ = NULL;
                building = false;
            }
        } else {
            if (two->right_!=NULL) two = two->right_;
            else {
                two->right_ = one;
                one->right_ = NULL;
                one->left_ = NULL;
                building = false;
            }
        }
        xLevel = !xLevel;
    }
}















