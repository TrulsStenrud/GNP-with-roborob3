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
    nbParents = 25;
    mutationRate = 0.2;
    
    processT = 5;
    judgeT = 1;
    connectionT = 0;
    nbEachNode = 4;
}

}
