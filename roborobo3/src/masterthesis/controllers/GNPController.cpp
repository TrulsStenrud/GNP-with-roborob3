#include "GNPController.h"
#include "../../../include/core/RoboroboMain/roborobo.h"
#include "../../../include/core/World/World.h"
#include "RoboroboMain/roborobo.h"

#define NB_SENSORS 8 // should be coherent with gRobotSpecsImageFilename value read from the property file.

GNPController::GNPController(RobotWorldModel *wm, GNP::Genome* genome):MyTestEEController(wm){
    _gnpNetwork = genome->buildNetwork(getProcesses(), getJudgements());
}

GNPController::~GNPController(){

}

void print(std::string text){
    std::cout << text << std::endl;
}

void GNPController::reset(){
    MyTestEEController::reset();
}

void GNPController::step(){
    _gnpNetwork->step();
}

NodeInformation GNPController::getNodeLibrary(){
    
    NodeInformation info;
    info.nbProcessingNodes = 4;
    
    
//    for(int i  = 0; i < 8; i++) // must match the number of camera sensors
//    {
//        if ( gSensoryInputs_distanceToContact )
//            info.judgementNodeOutputs.push_back(3);
//
//
//        if ( gSensoryInputs_physicalObjectType )
//        {
//            info.judgementNodeOutputs.push_back(2); // is foraging object or not foraging object
//        }
//
//        if ( gSensoryInputs_isOtherAgent )
//        {
//            info.judgementNodeOutputs.push_back(2); // is agent or is not agent
//        }
//
//        if ( gSensoryInputs_isWall )
//        {
//            info.judgementNodeOutputs.push_back(2); // is wall or not wall
//        }
//
//    }
    
    // object sensor: left, forward, right, back
    info.judgementNodeOutputs.push_back(4); // nothing, agent, foraging object, wall
    info.judgementNodeOutputs.push_back(4);
    info.judgementNodeOutputs.push_back(4);
    info.judgementNodeOutputs.push_back(4);
    
    
    // floor sensor
    if ( gSensoryInputs_groundSensors )
    {
        info.judgementNodeOutputs.push_back(2); // pheromone or not
    }
    
    // nest sensor
    info.judgementNodeOutputs.push_back(4); // 4 directions makes sense, right?
    
    // carrying sensor
    info.judgementNodeOutputs.push_back(2);
    
    return info;
}

void GNPController::buildBrain(GNP::Genome& genome){
    delete _gnpNetwork;
    _gnpNetwork = genome.buildNetwork(getProcesses(), getJudgements());
}

std::vector<std::function<void(double)>>* GNPController::getProcesses(){
    auto processes = new std::vector<std::function<void(double)>>();
    
    // move forward
    processes->push_back([&](double v){
        setTranslation(v);
    });
    
    // Rotate clockwise
    processes->push_back([&](double v){
        setRotation(0.01*v);
    });
    
    // Rotate counter clockwise
    processes->push_back([&](double v){
        setRotation(0.01*(-v));
    });
    
    // Drop pheromone
    processes->push_back([&](double v){
        if(v > 0.5){ // questionable
            dropPheromone();
        }
    });
    
    return processes;
}

int GNPController::judgeObjectTypeForSensors(std::vector<int> sensors) {
    bool wall = false, agent = false, foragingObject = false;
    for(int i : sensors){
        int objectId = _wm->getObjectIdFromCameraSensor(i);
        if(PhysicalObject::isInstanceOf(objectId) && gPhysicalObjects[objectId - gPhysicalObjectIndexStartOffset]->getType() == 5){
            foragingObject = true;
        }
        // input: another agent? If yes: same group?
        if ( Agent::isInstanceOf(objectId) )
        {
            // this is an agent
            agent = true;
        }
        // input: wall or empty?
        if ( objectId >= 0 && objectId < gPhysicalObjectIndexStartOffset ){
            wall = true;
        }
    }
    
    if(foragingObject){
        return 0;
    }
    if(agent){
        return 1;
    }
    if(wall){
        return 2;
    }
    
    return 3;
}

//** TODO currently most of these are [0, 1], but some are just 0 and 1, or [-1, 1]... Have to be decided later
std::vector<std::function<double()>>* GNPController::getJudgements(){
    auto judgements = new std::vector<std::function<double()>>();
        
//    for(int i  = 0; i < _wm->_cameraSensorsNb; i++)
//    {
//        if ( gSensoryInputs_distanceToContact )
//            judgements->push_back([&, i](){
//
//                double distance = _wm->getDistanceValueFromCameraSensor(i) / _wm->getCameraSensorMaximumDistanceValue(i);
//
//                if(distance <= 1){
//                    return 0;
//                }
//                else if(distance < 0.5)
//                    return 1;
//                else
//                    return 2;
//
//            });
//
//
//
//        if ( gSensoryInputs_physicalObjectType )
//        {
//            judgements->push_back([&, i](){
//
//                int objectId = _wm->getObjectIdFromCameraSensor(i);
//                // input: physical object? which type?
//                if ( PhysicalObject::isInstanceOf(objectId) )
//                {
//                    if(gPhysicalObjects[objectId - gPhysicalObjectIndexStartOffset]->getType() == 5) //foragingObject
//                    {
//                        return 1;
//                    }
//                    else
//                    {
//                        return 0;
//                    }
//
//                }
//                else
//                {
//                    // not a physical object. But: should still fill in the inputs (with zeroes)
//                    return 0;
//
//                }
//
//            });
//        }
//
//        if ( gSensoryInputs_isOtherAgent )
//        {
//            judgements->push_back([&, i](){
//                int objectId = _wm->getObjectIdFromCameraSensor(i);
//                // input: another agent? If yes: same group?
//                if ( Agent::isInstanceOf(objectId) )
//                {
//                    // this is an agent
//                    return 1;
//                }
//                else
//                {
//                    return 0;
//                }
//            });
//        }
//
//        if ( gSensoryInputs_isWall )
//        {
//            judgements->push_back([&, i](){
//
//                int objectId = _wm->getObjectIdFromCameraSensor(i);
//                // input: wall or empty?
//                if ( objectId >= 0 && objectId < gPhysicalObjectIndexStartOffset ) // not empty, but cannot be identified: this is a wall.
//                {
//                    return 1;
//                }
//                else
//                {
//                    return 0;
//                }
//            });
//        }
//
//    }
    
    // left
    judgements->push_back([&](){
        return judgeObjectTypeForSensors({7, 0, 1});
    });
    
    // forward
    judgements->push_back([&](){
        return judgeObjectTypeForSensors({1, 2, 3});
    });
    
    // right
    judgements->push_back([&](){
        return judgeObjectTypeForSensors({3, 4, 5});
    });
    
    // back
    judgements->push_back([&](){
        return judgeObjectTypeForSensors({5, 6, 7});
    });
    
    // floor sensor
    if ( gSensoryInputs_groundSensors )
    {
        judgements->push_back([&](){
            return getPheromoneValue() > 0 ? 0 : 1;
        });
    }
    
    // nest sensor
    judgements->push_back([&](){
        double direction = getNestRelativeOrientation();
        if(direction < -0.75){
            return 0;
        }
        if(direction < -0.25){
            return 1;
        }
        if(direction <= 0.25){
            return 2;
        }
        if(direction <= 0.75){
            return 3;
        }
        return 0;
    });
    
    // is carrying
    judgements->push_back([&](){
        return isCarrying() ? 1 : 0;
    });
    
    
    return judgements;
}

