#include "MultiNEATEvolver.h"
#include "MultiNEATController.h"
#include <time.h>
#include "RoboroboMain/roborobo.h"
#include <sstream>

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
    
    std::string resultFile = gLogDirectoryname + "/NEAT_" + std::to_string(time(NULL)) + ".csv";
    
    _logFile.open(resultFile);
    
    _logManager = new LogManager();
    _logManager->setLogFile(_logFile);
    
}

MultiNEATEvolver::~MultiNEATEvolver(){
	delete _params;
	delete _genomeBase;
	delete _pop;
    delete _logManager;
    _logFile.close();
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

    
    auto str = std::to_string(dp->fitness);
    // remove trailing zeroes
    str.erase ( str.find_last_not_of('0') + 1, std::string::npos );
    _logManager->write(";" +  str);

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
    
    _logManager->write("\n");
    _logManager->flush();
    _logManager->write("Generation " + std::to_string(_generation));
}

void MultiNEATEvolver::initPopulation(){
	_params->PopulationSize = 50;
	_pop = new NEAT::Population(*_genomeBase, *_params, true, 1.0, 72); // last argument is seed.
    _logManager->write("Generation " + std::to_string(_generation));
}
