//
//  NestObject.cpp
//  roborobo3
//
//  Created by Truls Stenrud on 22/02/2021.
//  Copyright © 2021 Nicolas Bredeche. All rights reserved.
//

#include "NestObject.h"
#include "World/EnergyItem.h"
#include "MyTestEEController.h"
#include "World/World.h"
#include "RoboroboMain/roborobo.h"

NestObject::NestObject(int __id ) : EnergyItem( __id ){
    
    setDisplayColor(0, 0, 100);
    _radius = 0;
    _footprintRadius = 80;
    
    resetValues();
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
