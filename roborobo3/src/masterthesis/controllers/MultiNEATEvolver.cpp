#include "MultiNEATEvolver.h"
#include "MultiNEATController.h"

using namespace std;

MultiNEATEvolver::MultiNEATEvolver(ControllerEvolver::CONTROLLER contType){
	if(contType != CONTROLLER::NEAT && contType != CONTROLLER::NoveltySearch && contType != CONTROLLER::HyperNEAT && contType != CONTROLLER::ESHyperNEAT){
		cout<<"Controller type not supported by MultiNEAT. Exiting..."<<endl;
		exit(3);
	}
	_evalIndex = 0;
	_contType = contType;
	_params = new NEAT::Parameters();
	_pop = nullptr;

	// Constructing base genome.
	NEAT::ActivationFunction activFunc = NEAT::ActivationFunction::UNSIGNED_SIGMOID;
	int inputs = 123;
	int outputs = 2;
	_genomeBase = new NEAT::Genome(0,inputs+1,0,outputs,false, activFunc, activFunc, 0, *_params, 0, 0);
}

MultiNEATEvolver::~MultiNEATEvolver(){
	delete _params;
	delete _genomeBase;
}

Controller* MultiNEATEvolver::make_Controller(RobotWorldModel* wm){
	if(_pop == nullptr)
		initPopulation();
	NEAT::Genome genome = _pop->AccessGenomeByIndex(_evalIndex);
	MultiNEATController* cont = new MultiNEATController(wm, &genome, _contType);
	return cont;
}

void MultiNEATEvolver::evalDone(std::vector<Robot*>* robots){
	// kan hende denne ikke trenger vektoren.
	std::cout<<"Evolver genUpdate called. Type: "<<_contType<<std::endl;
}

void MultiNEATEvolver::initPopulation(){
	_params->PopulationSize = 50;
	_pop = new NEAT::Population(*_genomeBase, *_params, true, 1.0, 72); // last argument is seed.
}
