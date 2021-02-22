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

ForagingObject::ForagingObject(int __id) : EnergyItem( __id ){
    //_radius = 8;
    //_footprintRadius = 16;
}



void ForagingObject::isWalked(int __idAgent){
    
    auto targetRobotController = dynamic_cast<MyTestEEController*>(gWorld->getRobot(__idAgent)->getController());
    if(targetRobotController->isCarrying()){
        std::cout << "Cant pick up\n";
    }
    else{
        std::cout << "Pick up\n";
        EnergyItem::isWalked(__idAgent);
        targetRobotController->setCarrying(_id);
    }
}
