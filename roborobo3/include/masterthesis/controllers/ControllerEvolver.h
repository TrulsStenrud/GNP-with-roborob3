#pragma once

#include "../../core/Agents/Robot.h"
#include "../../core/Controllers/Controller.h"
#include "../../core/WorldModels/RobotWorldModel.h"

// Interface for controller evolvers
class ControllerEvolver{
protected:

public:
	enum CONTROLLER:int{GNP, NEAT, NoveltySearch, HyperNEAT, ESHyperNEAT, MPFA};
	virtual Controller* make_Controller(RobotWorldModel* wm) = 0;
	virtual void evalDone(std::vector<Robot*>* robots) = 0;
};
