//
//  Population.hpp
//  roborobo3
//
//  Created by Truls Stenrud on 31/03/2021.
//  Copyright © 2021 Nicolas Bredeche. All rights reserved.
//

#ifndef Population_hpp
#define Population_hpp

#include "vector"
#include "GNPGenome.h"
#include "GNPParameters.h"
#include "NodeInformation.h"

namespace GNP
{

class Population
{
    
private:
    std::vector<Genome> _genes;
    Parameters* _params;
    NodeInformation _nodeInformation;
public:
    Population(NodeInformation nodeInformation, Parameters* _params);
    Genome& AccessGenomeByIndex(int const index);
    void Epoch();
};
}



#endif /* Population_hpp */
