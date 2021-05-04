//
//  Parameters.cpp
//  roborobo3
//
//  Created by Truls Stenrud on 09/04/2021.
//  Copyright © 2021 Nicolas Bredeche. All rights reserved.
//

#include "GNPParameters.h"


namespace GNP{


Parameters::Parameters(){
    reset();
}

void Parameters::reset(){
    populationSize = 50;
    nbParents = 10;
    mutationRate = 0.5;

    processT = 5;
    judgeT = 1;
    connectionT = 0;
    nbEachProcessingNode = 8;
    nbEachJudgementNode = 4;
//    gProperties.checkAndGetPropertyValue("gNbParents", &nbParents, false);
//    gProperties.checkAndGetPropertyValue("gNbEachProcessingNode", &nbEachProcessingNode, false);
//    gProperties.checkAndGetPropertyValue("gNbEachJudgementNode", &nbEachJudgementNode, false);
    nbNEATNodes = 0;

}

}
