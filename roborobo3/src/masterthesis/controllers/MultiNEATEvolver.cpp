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
	int inputs = 35;
	int outputs = 3;
	_genomeBase = new NEAT::Genome(0,inputs+1,0,outputs,false, activFunc, activFunc, 0, *_params, 0, 0);
}

MultiNEATEvolver::~MultiNEATEvolver(){
	delete _params;
	delete _genomeBase;
	delete _pop;
}

Controller* MultiNEATEvolver::make_Controller(RobotWorldModel* wm){
	if(_pop == nullptr)
		initPopulation();
	NEAT::Genome genome = _pop->AccessGenomeByIndex(_evalIndex);
	MultiNEATController* cont = new MultiNEATController(wm, &genome, _contType);
	return cont;
}

void MultiNEATEvolver::evalDone(DataPacket* dp){
	_pop->AccessGenomeByIndex(_evalIndex).SetFitness(dp->fitness);
	_evalIndex++;
	if(_evalIndex == _params->PopulationSize){
		nextGeneration();
		_evalIndex = 0;
	}
	for(Robot* rob : *(dp->robots)){
		MultiNEATController* cont = static_cast<MultiNEATController*>(rob->getController());
		cont->rebuildBrain(&(_pop->AccessGenomeByIndex(_evalIndex)));
		cont->reset();
	}
	//std::cout<<"Evaluated "<<dp->generation%_params->PopulationSize<<"/"<<_params->PopulationSize<<" chromosomes"<<std::endl;
}

void MultiNEATEvolver::nextGeneration(){
	_generation++;
	std::cout<<"generation "<<_generation<<" complete"<<std::endl;
	// kanskje få denne til å skrive datapacks istedenfor? gir mer mening for generasjoner og slikt. + denne kan regne ut gjennomsnitt.
	_pop->Epoch();
}

void MultiNEATEvolver::initPopulation(){
	_params->PopulationSize = 50;
	_pop = new NEAT::Population(*_genomeBase, *_params, true, 1.0, 72); // last argument is seed.
}
