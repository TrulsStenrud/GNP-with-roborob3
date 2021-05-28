//
//  GNPEvolver.cpp
//  roborobo3
//
//  Created by Truls Stenrud on 07/04/2021.
//  Copyright © 2021 Nicolas Bredeche. All rights reserved.
//

#include "GNPEvolver.h"
#include "GNPController.h"
#include "RoboroboMain/roborobo.h"
#include "../../../include/GNP/GNPGenome.h"
#include "MscMain.h"

GNPEvolver::GNPEvolver(ControllerEvolver::CONTROLLER controllerType){
    _evalIndex = 0;

    int populationSize;

    gProperties.checkAndGetPropertyValue("gEvolutionPopulationSize", &populationSize, true);

    auto library = GNPController::getNodeLibrary();

    _params = new GNP::Parameters();

    if(controllerType == CONTROLLER::GNPPlusPLus){
        _params->nbNEATNodes = gMscNbNeatNodes;
    }
    _params->populationSize = populationSize;
    _params->mutationRate = gMscMutationRate;
    _params->crossoverRate = gMscMutationRate;
    _params->tournamentSize = gMscTournamentSize;
    _params->nbEachJudgementNode = gMscNbEachJudgementNode;
    _params->nbEachProcessingNode = gMscNbEachProcessingNode;
    _params->processT = gMscProcessT;
    _params->judgeT = gMscJudgeT;
    _params->neatT = gMscNeatT;


    if(_params->nbNEATNodes > 0)
    {
        _neatParams = new NEAT::Parameters();
        _neatParams->PopulationSize = _params->populationSize;
        _neatParams->EliteFraction = 0.02;

        int inputs = 35 + 1;
        int outputs = 3;


        // Constructing base genome.
        NEAT::ActivationFunction activFunc = NEAT::ActivationFunction::UNSIGNED_SIGMOID;

        _neatGenomeBase = new NEAT::Genome(0,inputs,0,outputs,false, activFunc, activFunc, 0, *_neatParams, 0, 0);

        for(int i = 0;  i < _params->nbNEATNodes; i++){
            _neatPopulations.push_back(new NEAT::Population(*_neatGenomeBase, *_neatParams, true, 1.0, 72)); // last argument is seed
        }
    }
    std::string name = _params->nbNEATNodes > 0 ? "GIN" : "GNP";

    _logger = new Logger(name + std::to_string(gInitialNumberOfRobots));
    
    _pop = new GNP::Population(library, _params);
    
    if(gMscLogGnpNodeUsage){
        _nodeUsageLogger = new Logger("usage_" + name + std::to_string(gInitialNumberOfRobots));
        for(GNP::Node node : _pop->AccessGenomeByIndex(0).getNodes()){
            
            _nodeUsageLogger->log(std::to_string(node.type) + ":" + std::to_string(node.index));
        }
        _nodeUsageLogger->newLine();
    }
    _logger->log("Generation " + std::to_string(_generation));
}

GNPEvolver::~GNPEvolver(){
    delete _params;
    delete _pop;
    delete _logger;
    delete _neatParams;
    delete _neatGenomeBase;
    for(NEAT::Population* pop : _neatPopulations){
        delete pop;
    }
    delete _nodeUsageLogger;
}

void GNPEvolver::evalDone(DataPacket* dp){
    _pop->AccessGenomeByIndex(_evalIndex).setFitness(dp->fitness);
    
    for(int i = 0; i < _params->nbNEATNodes; i++){
        _neatPopulations[i]->AccessGenomeByIndex(_evalIndex).SetFitness(dp->fitness);
    }
    
    
    if(dp->fitness > _currentGenBest){
        _currentGenBest = dp->fitness;
        _currentGenBestIndex = _evalIndex;
    }
        
    
//    _pop->AccessGenomeByIndex(_evalIndex).printUsage();
    _logger->log(dp->foragingPercentage);

    _evalIndex++;

    if(_evalIndex == _params->populationSize){
        nextGeneration();
        _evalIndex = 0;
    }

    for(auto rob : gRobots){
        GNPController* cont = static_cast<GNPController*>(rob->getController());

        std::vector<NEAT::Genome> neatGenomes;
        for(int i = 0; i < _params->nbNEATNodes; i++){
            neatGenomes.push_back(_neatPopulations[i]->AccessGenomeByIndex(_evalIndex));
        }

        cont->buildBrain(_pop->AccessGenomeByIndex(_evalIndex), neatGenomes);
        cont->reset();
    }
}

void GNPEvolver::nextGeneration(){
    _generation++;

    std::cout<<"generation "<<_generation<<" complete"<<std::endl;
    
    
    if(gMscLogGnpNodeUsage){
        if(_currentGenBestIndex != -1){
            std::vector <int> result = _pop->AccessGenomeByIndex(_currentGenBestIndex).getNodeUsage();
            
            for(auto value : result){
                _nodeUsageLogger->log(value);
            }
            _nodeUsageLogger->newLine();
        }
        else{
            std::cout << "[WARNING] no current best" << std::endl;
        }
        
        
    }
    
    _currentGenBestIndex = -1;
    _currentGenBest=-1;
    
    
    _pop->Epoch();
    for(auto pop : _neatPopulations){
        pop->Epoch();
    }

    _logger->newLine();
    _logger->log("Generation " + std::to_string(_generation));
}

bool GNPEvolver::usesBehavior(){
    return false;
}

Controller* GNPEvolver::make_Controller(RobotWorldModel *wm){
    GNP::Genome& genome = _pop->AccessGenomeByIndex(_evalIndex);

    std::vector<NEAT::Genome> neatGenomes;
    for(int i = 0; i < _params->nbNEATNodes; i++){
        neatGenomes.push_back(_neatPopulations[i]->AccessGenomeByIndex(_evalIndex));
    }

    GNPController* cont = new GNPController(wm, genome, neatGenomes);
    return cont;
}
