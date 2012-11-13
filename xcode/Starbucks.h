/*****
 * Author   : brinkmwj
 * Date     : 2012-10-08
 * Sources  : All code is original
 * Purpose  : This class is designed to be an "Interface." Your starbucks data structure should be
 *            a sub-class of this class, but you should not implement this class. Instead, you should
 *            create a <uniqueid>Starbucks class, in the files <uniqueid>Starbucks.h and 
 *            <uniqueid>Starbucks.cpp
 *            Declare your class (in your <uniqueid>Starbucks.h) like this:
 *  class brinkmwjStarbucks : public Starbucks {
 *    // declaration of member variables and methods here
 *  };
 *            In your class you should declare all of your methods to be virtual: This is how
 *            polymorphism is accomplished in C++. Do NOT copy the part that says "= 0" that I use
 *            below. That part means that the function, though declared, will not have any implementation.
 * Note     : DO NOT CHANGE THIS FILE!!! 
 */

#pragma once
#include <string>
using namespace std;

/*
 * Note that the origin is in the LOWER LEFT corner, unlike in previous assignments.
 */
class Entry {
public:
	string identifier;
	double x;
	double y;
};

/*
 * This is an "abstract class" ... it contains a list of the public methods of the class, but
 *  they are all "pure virtual," which means they are not implemented anywhere in the code.
 *
 * An abstract class is used like an interface in Java: The only purpose of this class is to be the base
 *  class for your own data structure. You should make a .h and .cpp for you data structure ... in my
 *  case I made brinkmwjStarbucks.h and brinkmwjStarbucks.cpp, and my class was called "brinkmwjStarbucks"
 */
class Starbucks {
public:
	/*
	 * add all entries in the array to your data structure
	 *
	 * The "c" parameter is a pointer to an array of all the entries to be added, and n is the length of
	 * the array.
	 *
	 * Note: If you detect that two items have the same coordinates, then do not add the new item
	 *       that has the same coordinates as another item. This is guaranteed to happen, by the way,
	 *       because some Starbucks locations are listed in the database twice. We will define two locations
	 *       to be the "same location" if both |x1 - x2| <= 0.00001 and |y1 - y2| < 0.00001
	 */
	virtual void build(Entry* c, int n) = 0;
	
	/*
	 * Return a pointer to the entry that is closest to the given coordinates. Your
	 *  answer may be approximate, but then you will lose points on the "Accuracy" quality measure
	 */
	virtual Entry* getNearest(double x, double y) = 0;
};