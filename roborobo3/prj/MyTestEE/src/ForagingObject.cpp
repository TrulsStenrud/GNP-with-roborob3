//
//  ForagingObject.cpp
//  roborobo3
//
//  Created by Truls Stenrud on 21/02/2021.
//  Copyright © 2021 Nicolas Bredeche. All rights reserved.
//

#include "../include/ForagingObject.h"
#include "RoboroboMain/roborobo.h"
#include "../include/MyTestEEController.h"
#include "World/World.h"
#include "MyTestEE/include/ObjectFactory.h"

ForagingObject::ForagingObject(int __id) : CircleObject( __id ){
    setType(5);
    _radius = 4;
    unregisterObject();
    _footprintRadius = 0;
    regrowTime = -1;
}

void ForagingObject::step()
{
    stepPhysicalObject();
}

void ForagingObject::isWalked(int __idAgent){

}

void ForagingObject::isTouched(int __idAgent){

}

void ForagingObject::makeVisible(){
    _visible = true;
    registered = true;
    registerObject();
}

void ForagingObject::hideObject(){
    registered = false;
    _visible = false;
    unregisterObject();
    hide();
    ObjectFactory::recycleForagingObject(this);
}

void ForagingObject::isPushed(int __idAgent, std::tuple<double, double> __speed){

    auto targetRobotController = dynamic_cast<MyTestEEController*>(gWorld->getRobot(__idAgent-gRobotIndexStartOffset)->getController());
    if(!targetRobotController->isCarrying()){
        hideObject();
        targetRobotController->setCarrying(_id);
    }
    else{

    }
}
