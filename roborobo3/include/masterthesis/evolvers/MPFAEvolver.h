#pragma once

#include "../../core/Agents/Robot.h"
#include "../../core/Controllers/Controller.h"
#include "../../core/WorldModels/RobotWorldModel.h"
#include "../DataListener.h"
#include "ControllerEvolver.h"

using namespace std;

struct MPFAGenome{
	vector<tuple<double,double>> limits{
		make_tuple(0.0, 1.0),	// probSwitchToSearch;// [0,1]
		make_tuple(0.0, 1.0),	// ProbabilityOfReturningToNest;// [0,1]
		make_tuple(0.0, 1.0),	// UninformedSearchVariation;// [0,1]
		make_tuple(0.0, 0.2),	// RateOfInformedSearchDecay; // [0,0.2]
		make_tuple(1.0, 10.0),	// RateOfSiteFidelity; // [1,10]. 1 is almost never and 10 is extremely often. Poisson distribution.
		make_tuple(1.0, 10.0)	// RateOfLayingPheromone; // [1,10]. 1 is almost never and 10 is extremely often. Poisson distribution.
	};
	vector<double> values;

	double fitness;
};

class MPFAEvolver : public ControllerEvolver{


private:
	vector<MPFAGenome>* _pop;
	int _evalIndex;
	int _popSize;
	int _tournamentSize = 2;
	double _crossoverChance = 0.7;
	double _mutationChance = 0.2;
	double _mutationModifier = 0.5; // mutation modifies a value by randgaussian * _mutationModifier * range, where range is the difference between limits.

	double getRandBetween(double low, double high);
	void initPopulation();
	void newGeneration();
	MPFAGenome crossover(MPFAGenome p1, MPFAGenome p2);
	MPFAGenome mutate(MPFAGenome ind);

public:
	MPFAEvolver();
	~MPFAEvolver();
	Controller* make_Controller(RobotWorldModel* wm) override;
	void evalDone(DataPacket* dp) override;
    bool usesBehavior() override;
};
