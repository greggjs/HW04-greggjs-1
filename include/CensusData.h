//
//  CensusData.h
//  HW04
//
//  Created by Jake Gregg on 11/12/12.
//
//

#ifndef __HW04__CensusData__
#define __HW04__CensusData__

#include <iostream>
#include <fstream>
#include "greggjsStarbucks.h"
#include "cinder/app/App.h"
#include "cinder/Surface.h"

using namespace std;

class CensusData {
public:
    CensusData(string file);
    CensusData();
	~CensusData();
    
    double x_, y_;
    int population_;
    string file_location_;
    
    CensusData* left_;
    CensusData* right_;
    CensusData* getNearest(double x, double y);
    void buildCensusData(string file);
private:
    void build(CensusData* one, CensusData* two, bool xLevel);
    void build(CensusData* census, int n);
    void getNextCensus();
    void insertData(int pop, double x, double y);
    void insertData(double x, double y, CensusData* cd, bool xLevel);
    void randomize(CensusData* census, int n);
    CensusData* getNearest(double x, double y, CensusData* census, bool xLevel);
};

#endif /* defined(__HW04__CensusData__) */
