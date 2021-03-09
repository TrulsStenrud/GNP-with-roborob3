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

NestObject::NestObject(int __id ) : EnergyItem( __id ){

    _radius = 0;
    _footprintRadius = 80;
    _footprintColorRed = 0xFF;
    _footprintColorGreen = 0xC0;
    _footprintColorBlue = 0xC0;
    _collectedGoods = 0;
    resetValues();
}

int NestObject::getCollectedGoods(){
    return _collectedGoods;
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
