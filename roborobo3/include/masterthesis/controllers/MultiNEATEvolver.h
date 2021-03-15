#pragma once

#include "ControllerEvolver.h"
#include "../../MultiNEAT/Parameters.h"
#include "../../core/Controllers/Controller.h"
#include "../../core/WorldModels/RobotWorldModel.h"
#include "../../MultiNEAT/Genome.h"
#include "../../core/World/World.h"
#include "../../MultiNEAT/Population.h"
#include "../../core/RoboroboMain/roborobo.h"
#include "../DataListener.h"

class MultiNEATEvolver : public ControllerEvolver{
private:
	ControllerEvolver::CONTROLLER _contType;
	NEAT::Parameters* _params;
	NEAT::Genome* _genomeBase; // Starting population is constructed using this as a base.
	void initPopulation();
	void nextGeneration();
	NEAT::Population* _pop;
	int _evalIndex; // Index of genome to evaluate.
public:
	MultiNEATEvolver(ControllerEvolver::CONTROLLER contType);
	~MultiNEATEvolver();
	Controller* make_Controller(RobotWorldModel* wm) override;
	void evalDone(DataPacket* dp) override;
};