//
//  GNPEvolver.hpp
//  roborobo3
//
//  Created by Truls Stenrud on 07/04/2021.
//  Copyright © 2021 Nicolas Bredeche. All rights reserved.
//

#ifndef GNPEvolver_h
#define GNPEvolver_h

#include "ControllerEvolver.h"
#include <stdio.h>
#include "../DataListener.h"
#include "../../core/Controllers/Controller.h"
#include "../../GNP/GNPPopulation.h"
#include "../../MultiNEAT/Population.h"
#include "../../GNP/GNPParameters.h"

class GNPEvolver : public ControllerEvolver{
private:
    int _evalIndex; // Index of genome to evaluate.
    GNP::Population* _pop;
    GNP::Parameters* _params;
    NEAT::Parameters* _neatParams = nullptr;
    NEAT::Genome* _neatGenomeBase = nullptr;;
    void nextGeneration();
    std::vector<NEAT::Population*> _neatPopulations;
    Logger* _nodeUsageLogger = nullptr;

public:
    GNPEvolver(ControllerEvolver::CONTROLLER);
    ~GNPEvolver();
    Controller* make_Controller(RobotWorldModel* wm) override;
    void evalDone(DataPacket* dp) override;
    bool usesBehavior() override;

};



#endif /* GNPEvolver_hpp */
