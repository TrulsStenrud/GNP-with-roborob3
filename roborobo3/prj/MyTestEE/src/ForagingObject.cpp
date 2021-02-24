//
//  ForagingObject.cpp
//  roborobo3
//
//  Created by Truls Stenrud on 21/02/2021.
//  Copyright © 2021 Nicolas Bredeche. All rights reserved.
//

#include "ForagingObject.h"
#include "RoboroboMain/roborobo.h"
#include "MyTestEEController.h"
#include "World/World.h"

ForagingObject::ForagingObject(int __id) : CircleObject( __id ){
    _radius = 4;
    _footprintRadius = 8;
    setType(0);
}

void ForagingObject::step()
{
    stepPhysicalObject();
}

void ForagingObject::isWalked(int __idAgent){
std::cout << "im walked" << std::endl;
}

void ForagingObject::isTouched(int __idAgent){
    
}

void ForagingObject::isPushed(int __idAgent, std::tuple<double, double> __speed){

    auto targetRobotController = dynamic_cast<MyTestEEController*>(gWorld->getRobot(__idAgent-gRobotIndexStartOffset)->getController());
    if(!targetRobotController->isCarrying()){
        //EnergyItem::isWalked(__idAgent);
        
        regrowTime = regrowTimeMax;
        unregisterObject();
        registered = false;
        hide();
        _visible = false;
        
        targetRobotController->setCarrying(_id);
    }
    else{
        
    }
}
