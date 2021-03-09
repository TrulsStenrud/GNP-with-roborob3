#include "GNPController.h"
#include "../../../include/core/RoboroboMain/roborobo.h"
#include "../../../include/core/World/World.h"


GNPController::GNPController(RobotWorldModel *wm, ControllerEvolver::CONTROLLER controllerType):MyTestEEController(wm){
    _controllerType = controllerType;
}

GNPController::~GNPController(){

}

void GNPController::reset(){
    MyTestEEController::reset();
}

void GNPController::step(){
    
}

std::vector<std::function<void(double)>>* GNPController::getProcesses(){
    auto processes = new std::vector<std::function<void(double)>>();
    
    processes->push_back([&](double v){
        setTranslation(v);
    });
    processes->push_back([&](double v){
        setRotation(v);
    });
    processes->push_back([&](double v){
        if(v > 0.5){ // questionable
            dropPheromone();
        }
    });
    
    return processes;
}

//** TODO currently most of these are [0, 1], but some are just 0 and 1, or [-1, 1]... Have to be decided later
std::vector<std::function<double()>>* GNPController::getJudgements(){
    auto judgements = new std::vector<std::function<double()>>();
        
    judgements->push_back([&](){
        return 3;
    });
    
    for(int i  = 0; i < _wm->_cameraSensorsNb; i++)
    {
        if ( gSensoryInputs_distanceToContact )
            judgements->push_back([&](){
                return _wm->getDistanceValueFromCameraSensor(i) / _wm->getCameraSensorMaximumDistanceValue(i);
            });
            
        
        
        if ( gSensoryInputs_physicalObjectType )
        {
            judgements->push_back([&](){
        
                int objectId = _wm->getObjectIdFromCameraSensor(i);
                // input: physical object? which type?
                if ( PhysicalObject::isInstanceOf(objectId) )
                {
                    if(gPhysicalObjects[objectId - gPhysicalObjectIndexStartOffset]->getType() == 5) //foragingObject
                    {
                        return 1;
                    }
                    else
                    {
                        return 0;
                    }
                    
                }
                else
                {
                    // not a physical object. But: should still fill in the inputs (with zeroes)
                    return 0;
                    
                }
                
            });
        }
        
        if ( gSensoryInputs_isOtherAgent )
        {
            judgements->push_back([&](){
                int objectId = _wm->getObjectIdFromCameraSensor(i);
                // input: another agent? If yes: same group?
                if ( Agent::isInstanceOf(objectId) )
                {
                    // this is an agent
                    return 1;
                }
                else
                {
                    return 0;
                }
            });
        }
        
        if ( gSensoryInputs_isWall )
        {
            judgements->push_back([&](){
                int objectId = _wm->getObjectIdFromCameraSensor(i);
                // input: wall or empty?
                if ( objectId >= 0 && objectId < gPhysicalObjectIndexStartOffset ) // not empty, but cannot be identified: this is a wall.
                {
                    return 1;
                }
                else
                {
                    return 0;
                }
            });
        }
        
    }
    
    // floor sensor
    if ( gSensoryInputs_groundSensors )
    {
        judgements->push_back([&](){
            return getPheromoneValue();
            
        });
    }
    
    // nest sensor
    judgements->push_back([&](){
        return getNestRelativeOrientation();
    });
    
    
    return judgements;
}


