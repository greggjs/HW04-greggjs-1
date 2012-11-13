//
//  LinkList.h
//  HW04
//
//  Created by Jake Gregg on 11/12/12.
//
//

#ifndef __HW04__LinkList__
#define __HW04__LinkList__

#include <iostream>
#include "cinder/app/App.h"

using namespace ci;
using namespace std;

class LinkList {
public:
    LinkList() {};
    void clearList() {
        if (this->next_ == NULL) return;
        LinkList* current = next_;
        LinkList* temp;
        do {
            temp = current->next_;
            delete current;
            current = temp;
        } while (temp != NULL);
    }
    
    Vec2f vec_;
    LinkList* next_;
    string address_;
    ColorA color_;
    ColorA closest_starbucks_color_;
    Vec2f closest_starbucks_;
    Rectf rect_;
    
};

#endif /* defined(__HW04__LinkList__) */
