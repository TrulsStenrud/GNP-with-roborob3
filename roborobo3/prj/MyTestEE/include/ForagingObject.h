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
#include "World/CircleObject.h"

class ForagingObject : public CircleObject {
private:
    bool isCarried;
    
public:
    ForagingObject( int __id );
    ~ForagingObject() {}
    
    void step() override;
    void isWalked ( int __idAgent ) override; // callback, triggered by agent
    void isTouched ( int __idAgent ) override; // callback, triggered by agent
    void isPushed( int __id, std::tuple<double, double> __speed ) override;
    
};


#endif /* ForagingObject_hpp */
