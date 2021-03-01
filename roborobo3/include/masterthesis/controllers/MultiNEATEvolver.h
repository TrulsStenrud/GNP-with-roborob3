#pragma once

#include "ControllerEvolver.h"
#include "../../MultiNEAT/Parameters.h"

class MultiNEATEvolver : public ControllerEvolver{
private:
	ControllerEvolver::CONTROLLER _contType;
	NEAT::Parameters params;
public:
	MultiNEATEvolver(ControllerEvolver::CONTROLLER contType);
	~MultiNEATEvolver();
	void generationalUpdate(std::vector<Robot*> robots);
};
