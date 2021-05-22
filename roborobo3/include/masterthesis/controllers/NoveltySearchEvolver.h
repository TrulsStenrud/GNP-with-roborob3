//
//  NoveltySearchEvolver.hpp
//  roborobo3
//
//  Created by Truls Stenrud on 22/05/2021.
//  Copyright © 2021 Nicolas Bredeche. All rights reserved.
//

#ifndef NoveltySearchEvolver_h
#define NoveltySearchEvolver_h

#include <stdio.h>

#include "ControllerEvolver.h"
#include "../../MultiNEAT/Parameters.h"
#include "../../core/Controllers/Controller.h"
#include "../../core/WorldModels/RobotWorldModel.h"
#include "../../MultiNEAT/Genome.h"
#include "../../core/World/World.h"
#include "../../MultiNEAT/Population.h"
#include "../../core/RoboroboMain/roborobo.h"
#include "../DataListener.h"

class NoveltySearchEvolver : public ControllerEvolver{
private:
    NEAT::Parameters* _params;
    NEAT::Genome* _genomeBase; // Starting population is constructed using this as a base.
    NEAT::Population* _pop;
    int _evalIndex; // Index of genome to evaluate.
    int _evalCounter;
    NEAT::Substrate* _substrate = NULL;

    void initPopulation();
    void nextGeneration();
    NEAT::Substrate* createSubstrate(int input, int output);

public:
    NoveltySearchEvolver();
    ~NoveltySearchEvolver();
    Controller* make_Controller(RobotWorldModel* wm) override;
    void evalDone(DataPacket* dp) override;
    bool usesBehavior() override;
};

#endif /* NoveltySearchEvolver_hpp */
