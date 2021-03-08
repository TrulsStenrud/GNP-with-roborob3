#pragma once

#include "../../../include/core/Controllers/Controller.h"
#include "../../MultiNEAT/Genome.h"
#include "../../MultiNEAT/NeuralNetwork.h"
#include "ControllerEvolver.h"
#include "../../core/WorldModels/RobotWorldModel.h"

class MultiNEATController : public Controller{
	// hente nn fra evolveren. kjøre den. rapportere resultatet til evolveren.
private:
	NEAT::Genome* _genome;
	ControllerEvolver::CONTROLLER _controllerType;
	NEAT::NeuralNetwork* _nn;
	std::vector<double>* buildInputVector();
	double normalize(double num); // normalizes a number to [-1, 1]
public:
	MultiNEATController(RobotWorldModel *wm, NEAT::Genome* genome, ControllerEvolver::CONTROLLER controllerType);
	~MultiNEATController();
	void reset() override;
	void step() override;
	void RebuildBrain(NEAT::Genome* genome);
};
