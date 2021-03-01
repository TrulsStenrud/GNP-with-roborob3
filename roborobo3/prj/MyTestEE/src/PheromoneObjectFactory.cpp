//
//  PheromoneObjectFactory.cpp
//  roborobo3
//
//  Created by Truls Stenrud on 26/02/2021.
//  Copyright © 2021 Nicolas Bredeche. All rights reserved.
//

#include "PheromoneObjectFactory.h"
#include "World/World.h"
#include "RoboroboMain/roborobo.h"
#include "WorldModels/RobotWorldModel.h"

std::vector<PheromoneObject*> PheromoneObjectFactory::_unusedPheromones;

PheromoneObject* PheromoneObjectFactory::placePheromoneObject(int x, int y){
    if(_unusedPheromones.size() == 0){
        
        int id = PhysicalObjectFactory::getNextId();
        auto pheromoneObject = new PheromoneObject(id);
        pheromoneObject->unregisterObject();
        pheromoneObject->setCoordinates(x, y);
        pheromoneObject->registerObject();
        gPhysicalObjects.push_back(pheromoneObject);
        gNbOfPhysicalObjects = (int)gPhysicalObjects.size();
        
        return pheromoneObject;
    }
    else{
        auto pheromoneObject = _unusedPheromones.back();
        _unusedPheromones.pop_back();
        pheromoneObject->setCoordinates(x, y);
        pheromoneObject->makeVisible();
        return pheromoneObject;
    }
}

void PheromoneObjectFactory::recyclePheromoneObject(PheromoneObject *p){
    _unusedPheromones.push_back(p);
}
