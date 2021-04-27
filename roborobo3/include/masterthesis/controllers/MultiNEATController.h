#pragma once

#include "../../../include/core/Controllers/Controller.h"
#include "../../MultiNEAT/Genome.h"
#include "../../MultiNEAT/NeuralNetwork.h"
#include "../evolvers/ControllerEvolver.h"
#include "../../core/WorldModels/RobotWorldModel.h"
#include "../../../prj/MyTestEE/include/MyTestEEController.h"

class MultiNEATController : public MyTestEEController{
	// hente nn fra evolveren. kjøre den. rapportere resultatet til evolveren.
private:
	ControllerEvolver::CONTROLLER _controllerType;
	NEAT::NeuralNetwork* _nn;
	double normalize(double num); // normalizes a number to [-1, 1]
public:
	MultiNEATController(RobotWorldModel *wm, NEAT::Genome* genome, ControllerEvolver::CONTROLLER controllerType, NEAT::Substrate* substrate = NULL, NEAT::Parameters* parameters = NULL);
	~MultiNEATController();
	void reset() override;
	void step() override;
	void rebuildBrain(NEAT::Genome* genome, NEAT::Substrate* substrate = NULL, NEAT::Parameters* parameters = NULL);
    std::vector<double> buildInputVector();
};
