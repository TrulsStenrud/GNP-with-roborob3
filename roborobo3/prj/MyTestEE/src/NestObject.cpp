//
//  NestObject.cpp
//  roborobo3
//
//  Created by Truls Stenrud on 22/02/2021.
//  Copyright © 2021 Nicolas Bredeche. All rights reserved.
//

#include "../include/NestObject.h"
#include "World/EnergyItem.h"
#include "../include/MyTestEEController.h"
#include "World/World.h"
#include "RoboroboMain/roborobo.h"
#include <vector>

NestObject::NestObject(int __id ) : EnergyItem( __id ){

    _radius = 0;
    _footprintRadius = 30;
    _footprintColorRed = 0xFF;
    _footprintColorGreen = 0xC0;
    _footprintColorBlue = 0xC0;
    _collectedGoods = 0;

    PheromoneList = new std::vector<ImaginaryPheromone>();
	gProperties.checkAndGetPropertyValue("gPheromoneDecay", &_decay, true);
	gProperties.checkAndGetPropertyValue("gPheromoneEvaporationTreshold", &_evaporationTreshold, true);

    resetValues();
}

NestObject::~NestObject(){
	delete PheromoneList;
	EnergyItem::~EnergyItem();
}

int NestObject::getCollectedGoods(){
    return _collectedGoods;
}

void NestObject::step(){
	EnergyItem::step();

	for(auto it = PheromoneList->begin(); it != PheromoneList->end(); it++){
		it->strength *= (1-_decay);
		if(it->strength < _evaporationTreshold){
			it = PheromoneList->erase(it);
			if(it == PheromoneList->end())
				break;
		}
	}
}

void NestObject::resetValues(){
    _collectedGoods = 0;
}
void NestObject::isWalked(int __idAgent ){
    auto targetRobotController = dynamic_cast<MyTestEEController*>(gWorld->getRobot(__idAgent)->getController());
    if(targetRobotController->isCarrying()){
        targetRobotController->unLoad();
        _collectedGoods++;
        
    }
}
