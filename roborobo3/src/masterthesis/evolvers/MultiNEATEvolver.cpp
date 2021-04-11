#include "MultiNEATEvolver.h"
#include "MultiNEATController.h"
#include <time.h>
#include "RoboroboMain/roborobo.h"
#include "../../../prj/ForagingTemp/include/Behavior.h"
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
    
    double populationSize;
    gProperties.checkAndGetPropertyValue("gEvolutionPopulationSize", &populationSize, true);
    
    _params->PopulationSize = populationSize;
    _params->EliteFraction = 0.02;
        
    int inputs = 35 + 1;
    int outputs = 3;
    
    if(contType == CONTROLLER::HyperNEAT || contType == CONTROLLER::ESHyperNEAT){
        _substrate = createSubstrate(inputs, outputs);
    }
	// Constructing base genome.
	NEAT::ActivationFunction activFunc = NEAT::ActivationFunction::UNSIGNED_SIGMOID;
	
	_genomeBase = new NEAT::Genome(0,inputs,0,outputs,false, activFunc, activFunc, 0, *_params, 0, 0);
    
    std::string type;
    switch (contType) {
        case CONTROLLER::HyperNEAT:
            type = "HyperNEAT";
            break;
        case CONTROLLER::NEAT:
            type = "NEAT";
            break;
        case CONTROLLER::NoveltySearch:
            type = "NoveltySearch";
            break;
        default:
            type = "default";
            break;
    }
    std::string resultFile = gLogDirectoryname + "/" + type + "_" + gStartTime + "_" + getpidAsReadableString() + ".csv";
    
    _logFile.open(resultFile);
    
    _logManager = new LogManager();
    _logManager->setLogFile(_logFile);
    
    initPopulation();
    
}

MultiNEATEvolver::~MultiNEATEvolver(){
	delete _params;
	delete _genomeBase;
	delete _pop;
    delete _logManager;
    delete _substrate;
    _logFile.close();
}

NEAT::Substrate* MultiNEATEvolver::createSubstrate(int input, int output){
    std::vector<std::vector<double>> inputs;
    for(int i = 0; i < input; i++){
        double y = i - input/2;
        inputs.push_back({-1, y});
    }
    
    std::vector<std::vector<double>> hidden;
    for(int i = 0; i < 15; i++){
        double y = i - 15/2;
        hidden.push_back({0, y});
    }
    std::vector<std::vector<double>> outputs;
    for(int i = 0; i < output; i++){
        double y = i - output/2;
        outputs.push_back({1, y});
    }
    
    
    return new NEAT::Substrate(inputs, hidden, outputs);
}

Controller* MultiNEATEvolver::make_Controller(RobotWorldModel* wm){
	NEAT::Genome genome = _pop->AccessGenomeByIndex(_evalIndex);
	MultiNEATController* cont = new MultiNEATController(wm, &genome, _contType, _substrate, _params);
	return cont;
}

void MultiNEATEvolver::evalDone(DataPacket* dp){
    
    if(_contType == CONTROLLER::NoveltySearch){
        _pop->AccessGenomeByIndex(_evalIndex).m_PhenotypeBehavior = new Behavior(dp->behaviorData);
    }
    else{
        _pop->AccessGenomeByIndex(_evalIndex).SetFitness(dp->fitness);
    }

    
    auto str = std::to_string(dp->fitness);
    // remove trailing zeroes
    str.erase ( str.find_last_not_of('0') + 1, std::string::npos );
    _logManager->write(";" +  str);

	_evalIndex++;
	if(_evalIndex == _params->PopulationSize){
		nextGeneration();
		_evalIndex = 0;
	}
	for(auto rob : gRobots){
		MultiNEATController* cont = static_cast<MultiNEATController*>(rob->getController());
		cont->rebuildBrain(&(_pop->AccessGenomeByIndex(_evalIndex)), _substrate, _params);
		cont->reset();
	}
    
	//std::cout<<"Evaluated "<<dp->generation%_params->PopulationSize<<"/"<<_params->PopulationSize<<" chromosomes"<<std::endl;
}

void MultiNEATEvolver::nextGeneration(){
	_generation++;
	std::cout<<"generation "<<_generation<<" complete"<<std::endl;
	// kanskje få denne til å skrive datapacks istedenfor? gir mer mening for generasjoner og slikt. + denne kan regne ut gjennomsnitt.
    
    if(_contType == CONTROLLER::NoveltySearch){
        for(int i = 0; i < _params->PopulationSize; i++){
            _pop->AccessGenomeByIndex(i).SetFitness(_pop->ComputeSparseness(_pop->AccessGenomeByIndex(i)));
        }
    }
    
	_pop->Epoch();
    
    _logManager->write("\n");
    _logManager->flush();
    _logManager->write("Generation " + std::to_string(_generation));
}

void MultiNEATEvolver::initPopulation(){
	_pop = new NEAT::Population(*_genomeBase, *_params, true, 1.0, 72); // last argument is seed.
    
    if(_contType == CONTROLLER::NoveltySearch){
        std::vector<NEAT::PhenotypeBehavior> *a_population = new std::vector<NEAT::PhenotypeBehavior>();
        std::vector<NEAT::PhenotypeBehavior> *a_archive = new std::vector<NEAT::PhenotypeBehavior>();
        _pop->InitPhenotypeBehaviorData(a_population, a_archive);
    }
    
    _logManager->write("Generation " + std::to_string(_generation));
}

bool MultiNEATEvolver::usesBehavior(){
    return _contType == CONTROLLER::NoveltySearch;
}
