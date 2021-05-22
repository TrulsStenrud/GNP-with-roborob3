//
//  NoveltySearchEvolver.cpp
//  roborobo3
//
//  Created by Truls Stenrud on 22/05/2021.
//  Copyright © 2021 Nicolas Bredeche. All rights reserved.
//

#include "NoveltySearchEvolver.h"
#include "MultiNEATController.h"
#include <time.h>
#include "RoboroboMain/roborobo.h"
#include "../../../prj/ForagingTemp/include/Behavior.h"
#include <sstream>
#include "MscMain.h"

using namespace std;

NoveltySearchEvolver::NoveltySearchEvolver(){
    _evalIndex = 0;
    _evalCounter = 0;
    _params = new NEAT::Parameters();

    double populationSize;
    gProperties.checkAndGetPropertyValue("gEvolutionPopulationSize", &populationSize, true);

    _params->PopulationSize = populationSize;
    _params->EliteFraction = 0.02;
    
    // not even sure if it ises them
    _params->RouletteWheelSelection = false;
    _params->NoveltySearch_Dynamic_Pmin = true;
    _params->OverallMutationRate = gMscMutationRate;
    _params->CrossoverRate = gMscCrossoverRate;
    _params->TournamentSize = gMscTournamentSize;
    _params->YoungAgeTreshold = gMscYoungAgeThreshold;
    _params->OldAgeTreshold = gMscOldAgeThreshold;
    _params->PhasedSearching = gMscPhasedSearching;
    _params->DeltaCoding = gMscDeltaCoding;
    
    _params->NoveltySearch_K = gMscNoveltySearchK;
    _params->NoveltySearch_P_min = gMscNoveltySearchPMin;
    _params->NoveltySearch_Dynamic_Pmin = gMscDynamicPMin;
    _params->NoveltySearch_No_Archiving_Stagnation_Treshold = gMscNoveltySearch_No_Archiving_Stagnation_Treshold;
    _params->NoveltySearch_Pmin_lowering_multiplier = gMscNoveltySearch_Pmin_lowering_multiplier;
    _params->NoveltySearch_Pmin_min = gMscNoveltySearch_Pmin_min;
    _params->NoveltySearch_Quick_Archiving_Min_Evaluations = gMscNoveltySearch_Quick_Archiving_Min_Evaluations;
    _params->NoveltySearch_Pmin_raising_multiplier = gMscNoveltySearch_Pmin_raising_multiplier;
    _params->NoveltySearch_Recompute_Sparseness_Each = gMscNoveltySearch_Recompute_Sparseness_Each;
    

    int inputs = 35 + 1;
    int outputs = 3;


    // Constructing base genome.
    NEAT::ActivationFunction activFunc = NEAT::ActivationFunction::UNSIGNED_SIGMOID;

    _genomeBase = new NEAT::Genome(0,inputs,0,outputs,false, activFunc, activFunc, 0, *_params, 0, 0);

    std::string type = "NoveltySearch";;
    
    _logger = new Logger(type + std::to_string(gInitialNumberOfRobots));

    initPopulation();

}

NoveltySearchEvolver::~NoveltySearchEvolver(){
    delete _params;
    delete _genomeBase;
    delete _pop;
    delete _substrate;
    delete _logger;
}


Controller* NoveltySearchEvolver::make_Controller(RobotWorldModel* wm){
    NEAT::Genome genome = _pop->AccessGenomeByIndex(_evalIndex);
    MultiNEATController* cont = new MultiNEATController(wm, &genome, ControllerEvolver::NoveltySearch, _substrate, _params);
    return cont;
}

void NoveltySearchEvolver::evalDone(DataPacket* dp){
    
    auto& genome = _pop->AccessGenomeByIndex(_evalIndex);
    genome.m_PhenotypeBehavior = new Behavior(dp->behaviorData);
    genome.SetEvaluated();
    
    _evalCounter++;
    
    _logger->log(dp->foragingPercentage);

    std::vector<int> indices;
    for(int i = 0; i < _params->PopulationSize; i++){
        if(!_pop->AccessGenomeByIndex(i).IsEvaluated()){
            indices.push_back(i);
        }
    }
    
    if(indices.empty()){
        NEAT::Genome genome;
        _pop->NoveltySearchTick(genome);
        for(int i = 0; i < _params->PopulationSize; i++){
            if(_pop->AccessGenomeByIndex(i).GetID() == genome.GetID())
            {
                _evalIndex = i;
                break;
            }
        }
    }
    else{
        _evalIndex = indices[randint()%indices.size()];
    }
    
    if(_evalCounter % _params->PopulationSize == 0){
        nextGeneration();
    }
    for(auto rob : gRobots){
        MultiNEATController* cont = static_cast<MultiNEATController*>(rob->getController());
        cont->rebuildBrain(&(_pop->AccessGenomeByIndex(_evalIndex)), _substrate, _params);
        cont->reset();
    }

    //std::cout<<"Evaluated "<<dp->generation%_params->PopulationSize<<"/"<<_params->PopulationSize<<" chromosomes"<<std::endl;
}

void NoveltySearchEvolver::nextGeneration(){
    
    _generation++;
    std::cout<<"generation "<<_generation<<" complete"<<std::endl;
    _logger->newLine();
    _logger->log("Generation " + std::to_string(_generation));
}

void NoveltySearchEvolver::initPopulation(){
    _pop = new NEAT::Population(*_genomeBase, *_params, true, 1.0, 72); // last argument is seed.

    
        std::vector<NEAT::PhenotypeBehavior> *a_population = new std::vector<NEAT::PhenotypeBehavior>();
        std::vector<NEAT::PhenotypeBehavior> *a_archive = new std::vector<NEAT::PhenotypeBehavior>();
        _pop->InitPhenotypeBehaviorData(a_population, a_archive);
    

    _logger->log("Generation " + std::to_string(_generation));
}

bool NoveltySearchEvolver::usesBehavior(){
    return true;
}
