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

GNPEvolver::GNPEvolver(){
    _evalIndex = 0;
    
    double populationSize;
    gProperties.checkAndGetPropertyValue("gEvolutionPopulationSize", &populationSize, true);
    
    auto library = GNPController::getNodeLibrary();
    auto genome = new GNP::Genome(library.nbProcessingNodes, library.judgementNodeOutputs);
    _params = new GNP::Parameters();
    
    _logger = new Logger("GNP");
    
    _pop = new GNP::Population(library, _params);
    _logger->log("Generation " + std::to_string(_generation));
}

void GNPEvolver::evalDone(DataPacket* dp){
    _pop->AccessGenomeByIndex(_evalIndex).setFitness(dp->fitness);

    _logger->log(dp->fitness);
    
    _evalIndex++;
    
    if(_evalIndex == _params->populationSize){
        nextGeneration();
        _evalIndex = 0;
    }

    for(auto rob : gRobots){
        GNPController* cont = static_cast<GNPController*>(rob->getController());
        
        cont->reset();
    }
}

void GNPEvolver::nextGeneration(){
    _generation++;
    
    std::cout<<"generation "<<_generation<<" complete"<<std::endl;
    
    _pop->Epoch();
    _logger->newLine();
    _logger->log("Generation " + std::to_string(_generation));
}

bool GNPEvolver::usesBehavior(){
    return false;
}

Controller* GNPEvolver::make_Controller(RobotWorldModel *wm){
    GNP::Genome genome = _pop->AccessGenomeByIndex(_evalIndex);
    GNPController* cont = new GNPController(wm, &genome);
    return cont;
}
