#pragma once

#include "../../core/Agents/Robot.h"

// Interface for controller evolvers
class ControllerEvolver{
public:
	enum CONTROLLER:int{GNP, NEAT, HyperNEAT, ESHyperNEAT, MPFA};
	virtual void generationalUpdate(std::vector<Robot*> robots) = 0;
};
