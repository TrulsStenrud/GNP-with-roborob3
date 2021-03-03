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

std::unordered_set<PheromoneObject*> PheromoneObjectFactory::_unusedPheromones;

PheromoneObject* PheromoneObjectFactory::placePheromoneObject(int x, int y){
    if(_unusedPheromones.empty()){
        
        int id = PhysicalObjectFactory::getNextId();
        auto pheromoneObject = new PheromoneObject(id);
        
        pheromoneObject->setCoordinates(x, y);
        pheromoneObject->makeVisible();
        gPhysicalObjects.push_back(pheromoneObject);
        gNbOfPhysicalObjects = (int)gPhysicalObjects.size();
        
        return pheromoneObject;
    }
    else{
        auto pheromoneObject = *_unusedPheromones.begin();
        _unusedPheromones.erase(pheromoneObject);
        pheromoneObject->setCoordinates(x, y);
        pheromoneObject->makeVisible();
        return pheromoneObject;
    }
}

void PheromoneObjectFactory::recyclePheromoneObject(PheromoneObject *p){
    if(_unusedPheromones.count(p) == 0)
        _unusedPheromones.insert(p);
}
