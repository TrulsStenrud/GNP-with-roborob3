//
//  PheromoneObjectFactory.cpp
//  roborobo3
//
//  Created by Truls Stenrud on 26/02/2021.
//  Copyright © 2021 Nicolas Bredeche. All rights reserved.
//

#include "ObjectFactory.h"
#include "World/World.h"
#include "RoboroboMain/roborobo.h"
#include "WorldModels/RobotWorldModel.h"

std::unordered_set<PheromoneObject*> ObjectFactory::_unusedPheromones;
std::unordered_set<ForagingObject*> ObjectFactory::_unusedForagingObjects;


PheromoneObject* ObjectFactory::placePheromoneObject(int x, int y){
    if(_unusedPheromones.empty()){
        
        int id = PhysicalObjectFactory::getNextId();
        auto newObject = new PheromoneObject(id);
        
        newObject->setCoordinates(x, y);
        newObject->registerObject();
        
        gPhysicalObjects.push_back(newObject);
        gNbOfPhysicalObjects = (int)gPhysicalObjects.size();
        
        return newObject;
    }
    else{
        auto newObject = *_unusedPheromones.begin();
        _unusedPheromones.erase(newObject);
        newObject->setCoordinates(x, y);
        newObject->makeVisible();
        return newObject;
    }
}

void ObjectFactory::recyclePheromoneObject(PheromoneObject *p){
    if(_unusedPheromones.count(p) == 0)
        _unusedPheromones.insert(p);
}

ForagingObject* ObjectFactory::placeForagingObject(int x, int y){
    if(_unusedForagingObjects.empty()){
        
        int id = PhysicalObjectFactory::getNextId();
        
        auto newObject = new ForagingObject(id);
        newObject->setDisplayColor(255,128,64);
        newObject->setCoordinates(x, y);
        newObject->registerObject();
        
        gPhysicalObjects.push_back(newObject);
        gNbOfPhysicalObjects = (int)gPhysicalObjects.size();
        
        return newObject;
    }
    else{
        auto newObject = *_unusedForagingObjects.begin();
        _unusedForagingObjects.erase(newObject);
        newObject->setCoordinates(x, y);
        newObject->makeVisible();
        return newObject;
    }
}

void ObjectFactory::recycleForagingObject(ForagingObject *p){
    if(_unusedForagingObjects.count(p) == 0)
        _unusedForagingObjects.insert(p);
}


