//
//  ForagingObject.h
//  roborobo3
//
//  Created by Truls Stenrud on 21/02/2021.
//  Copyright © 2021 Nicolas Bredeche. All rights reserved.
//

#ifndef ForagingObject_hpp
#define ForagingObject_hpp

#include <stdio.h>
#include "World/EnergyItem.h"

class ForagingObject : public EnergyItem {
private:
    bool isCarried;
    
public:
    ForagingObject( int __id );
    ~ForagingObject() {}
    
    void isWalked ( int __idAgent ) override; // callback, triggered by agent
    
};


#endif /* ForagingObject_hpp */
