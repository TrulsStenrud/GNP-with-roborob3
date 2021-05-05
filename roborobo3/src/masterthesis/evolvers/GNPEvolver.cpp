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
    _params->nbParents = gMscNbParents;
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
        auto neatParams = new NEAT::Parameters();
        neatParams->PopulationSize = _params->populationSize;
        neatParams->EliteFraction = 0.02;

        int inputs = 35 + 1;
        int outputs = 3;

        
        // Constructing base genome.
        NEAT::ActivationFunction activFunc = NEAT::ActivationFunction::UNSIGNED_SIGMOID;

        auto genomeBase = new NEAT::Genome(0,inputs,0,outputs,false, activFunc, activFunc, 0, *neatParams, 0, 0);
        
        for(int i = 0;  i < _params->nbNEATNodes; i++){
            _neatPopulations.push_back(new NEAT::Population(*genomeBase, *neatParams, true, 1.0, 72)); // last argument is seed
        }
    }
    std::string name = _params->nbNEATNodes > 0 ? "GNP++" : "GNP";
    
    _logger = new Logger(name + std::to_string(gInitialNumberOfRobots));
    
    _pop = new GNP::Population(library, _params);
    _logger->log("Generation " + std::to_string(_generation));
}

void GNPEvolver::evalDone(DataPacket* dp){
    _pop->AccessGenomeByIndex(_evalIndex).setFitness(dp->fitness);
    
    for(int i = 0; i < _params->nbNEATNodes; i++){
        _neatPopulations[i]->AccessGenomeByIndex(_evalIndex).SetFitness(dp->fitness);
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
