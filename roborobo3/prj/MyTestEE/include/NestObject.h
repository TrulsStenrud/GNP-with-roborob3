//
//  NestObject.hpp
//  roborobo3
//
//  Created by Truls Stenrud on 22/02/2021.
//  Copyright © 2021 Nicolas Bredeche. All rights reserved.
//

#ifndef NestObject_hpp
#define NestObject_hpp

#include <stdio.h>
#include "World/EnergyItem.h"

class NestObject : public EnergyItem{
public:
    
    NestObject( int __id );
    ~NestObject() {}
    
    void isWalked( int __idAgent ) override;
    void resetValues();
    
protected:
    int _collectedGoods;
};

#endif /* NestObject_hpp */
