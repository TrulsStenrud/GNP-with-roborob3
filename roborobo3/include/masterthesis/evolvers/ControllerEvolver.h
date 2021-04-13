#pragma once

#include "../../core/Agents/Robot.h"
#include "../../core/Controllers/Controller.h"
#include "../../core/WorldModels/RobotWorldModel.h"
#include "../DataListener.h"

// Interface for controller evolvers
class ControllerEvolver{
protected:
	int _generation = 0;
public:
	enum CONTROLLER:int{GNP, NEAT, NoveltySearch, HyperNEAT, ESHyperNEAT, MPFA};
	virtual Controller* make_Controller(RobotWorldModel* wm) = 0;
	virtual void evalDone(DataPacket* dp) = 0;
    virtual bool usesBehavior() = 0;
};