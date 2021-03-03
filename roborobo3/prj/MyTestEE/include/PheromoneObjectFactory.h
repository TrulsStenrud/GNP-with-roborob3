//
//  PheromoneObjectFactory.hpp
//  roborobo3
//
//  Created by Truls Stenrud on 26/02/2021.
//  Copyright © 2021 Nicolas Bredeche. All rights reserved.
//

#ifndef PheromoneObjectFactory_hpp
#define PheromoneObjectFactory_hpp

#include <stdio.h>
#include "MyTestEE/include/PheromoneObject.h"
#include <unordered_set>

class PheromoneObjectFactory{
public:
    static PheromoneObject* placePheromoneObject(int x, int y);
    static void recyclePheromoneObject(PheromoneObject* p);
private:
    static std::unordered_set<PheromoneObject*> _unusedPheromones;
};

#endif /* PheromoneObjectFactory_hpp */
