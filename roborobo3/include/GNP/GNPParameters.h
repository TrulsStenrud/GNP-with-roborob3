//
//  Parameters.hpp
//  roborobo3
//
//  Created by Truls Stenrud on 09/04/2021.
//  Copyright © 2021 Nicolas Bredeche. All rights reserved.
//

#ifndef Parameters_hpp
#define Parameters_hpp

#include <stdio.h>


namespace GNP{

class Parameters{
  
private:
void reset();

public:
    Parameters();
    
    int populationSize;
    int nbParents;
    double mutationRate;
    int processT;
    int judgeT;
    int connectionT;
    int nbEachProcessingNode;
    int nbEachJudgementNode;
    int nbNEATNodes;
};
}
#endif /* Parameters_hpp */
